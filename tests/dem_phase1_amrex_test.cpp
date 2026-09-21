/*
 * Phase 1 acceptance tests for the parallel AMReX DEM path
 * (delta/core/parallel/DemAmrexEngine).
 *
 *   1. contact pair set == brute-force serial detection, for a mixed random
 *      cloud + lattice (global reach/halo completeness).
 *   2. one force pass is per-particle identical to the serial Engine
 *      (velocities after the pass, i.e. contact force and torque).
 *   3. partition invariance: a single-box engine and a 27-box engine produce
 *      the same particle count, contact count and summed position/velocity/
 *      angular momentum, within floating point reduction noise.
 *   4. 1000-sphere pile, 20 steps, contacts on: counts hold and nothing blows up.
 *   5. the halo/bin sizing guard fires for an under-sized grid, and such a grid
 *      really does drop contacts (the hazard the guard exists for).
 *
 * Run: ctest --test-dir build-amrex
 *      mpirun --allow-run-as-root -n 2 ./build-amrex/delta_dem_amrex_test
 *
 * The scenario helpers are the ones from tests/dem_phase0_test.cpp so the two
 * suites exercise the same geometry. A local CHECK() is used rather than
 * assert() because the build type is Release (-DNDEBUG) and because a failure
 * has to abort the whole MPI job, not just this rank.
 */

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <AMReX.H>
#include <AMReX_ParallelDescriptor.H>

#include "core/Engine.h"
#include "core/data/Meta.h"
#include "core/parallel/DemAmrexEngine.h"

#include <mpi.h>
#include "geometry/material.h"
#include "world/structure/Object.h"

using delta::core::parallel::DemAmrexEngine;
using delta::core::parallel::DemParticleContainer;
using delta::core::parallel::DemParallelConfig;
using delta::core::data::ParticleRecord;

typedef std::pair<long long, long long> GidPair;
typedef std::set<GidPair> GidPairSet;

#define CHECK(cond)                                                              \
  do {                                                                           \
    if (!(cond)) {                                                               \
      std::fflush(stdout);                                                       \
      std::fprintf(stderr, "FAIL %s:%d: %s (rank %d)\n", __FILE__, __LINE__,     \
                   #cond, amrex::ParallelDescriptor::MyProc());                  \
      std::fflush(stderr);                                                       \
      amrex::Abort("delta dem phase1 amrex test failure");                       \
    }                                                                            \
  } while (0)

namespace {

/* Runtime, not a namespace-scope constant: MPI/AMReX state does not exist yet
 * when the test's global constructors run. */
bool isRoot()
{
  return amrex::ParallelDescriptor::MyProc() == 0;
}

void say(const char* fmt, ...)
{
  if (!isRoot()) {
    return;
  }
  va_list args;
  va_start(args, fmt);
  std::vprintf(fmt, args);
  va_end(args);
  std::fflush(stdout);
}

iREAL sphereMass(iREAL radius, iREAL density = 20000.0)
{
  return density * (4.0 / 3.0) * 3.14159265358979323846 * radius * radius * radius;
}

delta::core::data::Meta::Simulation baseMeta(iREAL dt)
{
  delta::core::data::Meta::Simulation meta;
  meta.plotScheme = delta::core::data::Meta::Plot::Never;
  meta.modelScheme = delta::core::data::Meta::CollisionModel::Sphere;
  meta.overlapPreCheck = false;
  meta.dt = dt;
  /* The engines under test step with gravity, so the serial references must
   * too; otherwise they drift by dt*g per step. */
  meta.gravity = true;
  meta.gravityVector = {0.0, -9.8, 0.0};
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;
  return meta;
}

/*
 * Small boxes. The default maxGridSize leaves these scenarios as a single box
 * (the BoxArray is split by cells, not by particles), which would quietly turn
 * every "2 ranks" run into a single-rank run with an idle second rank, so the
 * parallel tests ask for many small boxes instead. Every box boundary then
 * needs a ghost exchange, and both ranks own boxes.
 */
DemParallelConfig manyBoxes()
{
  DemParallelConfig config;
  config.maxGridSize = 2;
  return config;
}

/*
 * Non-zero initial velocities. Every test scenario starts with these, so the
 * force model's velocity/friction terms (and the rotation update) are actually
 * exercised instead of being no-ops on an all-zero state.
 */
std::array<iREAL, 3> initialLinear(int id)
{
  const iREAL s = 0.01 * static_cast<iREAL>(1 + (id % 3));
  return {{s, -0.5 * s, 0.25 * s}};
}

std::array<iREAL, 3> initialAngular(int id)
{
  const iREAL s = 0.3 * static_cast<iREAL>(1 + (id % 2));
  return {{-s, 0.5 * s, s}};
}

/* Touching pile: spacing < 2r + 2*epsilon so the contact loop has work to do. */
std::vector<delta::world::structure::Object> spherePile(
    int side, iREAL radius, iREAL epsilon, unsigned seed, int firstId = 0)
{
  std::vector<delta::world::structure::Object> particles;
  std::mt19937 rng(seed);
  std::uniform_real_distribution<iREAL> jitter(-0.2 * radius, 0.2 * radius);

  iREAL spacing = 1.9 * (radius + epsilon);
  iREAL mass = sphereMass(radius);

  int id = firstId;
  for (int x = 0; x < side; x++)
    for (int y = 0; y < side; y++)
      for (int z = 0; z < side; z++) {
        std::array<iREAL, 3> centre = {
            0.2 + x * spacing + jitter(rng),
            0.2 + y * spacing + jitter(rng),
            0.2 + z * spacing + jitter(rng)};
        const int thisId = id++;
        delta::world::structure::Object o(
            "sphere", radius, thisId, centre,
            delta::geometry::material::MaterialType::WOOD,
            false, true, true, epsilon,
            initialLinear(thisId), initialAngular(thisId));
        o.setMass(mass);
        particles.push_back(o);
      }
  return particles;
}

/* Dense random cloud: positions straddle cell and box boundaries, which is
 * where a broad phase with too small a reach silently drops pairs. */
std::vector<delta::world::structure::Object> sphereCloud(
    int n, iREAL radius, iREAL epsilon, unsigned seed, int firstId = 0)
{
  std::mt19937 rng(seed);
  std::uniform_real_distribution<iREAL> pos(0.3, 0.7);
  iREAL mass = sphereMass(radius);

  std::vector<delta::world::structure::Object> particles;
  for (int i = 0; i < n; i++) {
    std::array<iREAL, 3> centre = {pos(rng), pos(rng), pos(rng)};
    const int thisId = firstId + i;
    delta::world::structure::Object o(
        "sphere", radius, thisId, centre,
        delta::geometry::material::MaterialType::WOOD,
        false, true, true, epsilon,
        initialLinear(thisId), initialAngular(thisId));
    o.setMass(mass);
    particles.push_back(o);
  }
  return particles;
}

/* Mixed scenario. Objects carry contiguous ids 0..n-1 in this order. */
std::vector<delta::world::structure::Object> sphereScene(
    int nCloud, int sideLattice, iREAL radius, iREAL epsilon, unsigned seed)
{
  std::vector<delta::world::structure::Object> objects =
      sphereCloud(nCloud, radius, epsilon, seed, 0);
  std::vector<delta::world::structure::Object> lattice =
      spherePile(sideLattice, radius, epsilon, seed + 1u, nCloud);
  for (std::size_t i = 0; i < lattice.size(); ++i) {
    objects.push_back(lattice[i]);
  }
  return objects;
}

GidPairSet serialContactPairs(std::vector<delta::world::structure::Object>& objects,
                              std::array<iREAL, 6> boundary,
                              const delta::core::data::Meta::Simulation& meta)
{
  delta::core::Engine engine(objects, boundary, meta);
  engine.contactDetectionBruteForce();
  std::vector<std::pair<int, int> > pairs = engine.getContactPairs();
  GidPairSet out;
  for (std::size_t i = 0; i < pairs.size(); ++i) {
    out.insert(GidPair(std::min(pairs[i].first, pairs[i].second),
                       std::max(pairs[i].first, pairs[i].second)));
  }
  return out;
}

/* Every unordered pair whose centre distance is within the engine reach, using
 * the same floating point expression as DemCheckPair (dx*dx+dy*dy+dz*dz
 * <= reach*reach). This is the superset the neighbour search has to cover. */
GidPairSet reachPairs(std::vector<ParticleRecord>& records, iREAL reach)
{
  GidPairSet out;
  for (std::size_t i = 0; i < records.size(); ++i) {
    for (std::size_t j = i + 1; j < records.size(); ++j) {
      const iREAL dx = records[j]._centre[0] - records[i]._centre[0];
      const iREAL dy = records[j]._centre[1] - records[i]._centre[1];
      const iREAL dz = records[j]._centre[2] - records[i]._centre[2];
      if (dx * dx + dy * dy + dz * dz <= reach * reach) {
        out.insert(GidPair(records[i].getGlobalParticleID(),
                           records[j].getGlobalParticleID()));
      }
    }
  }
  return out;
}

/*
 * Global contact set of an engine: each rank records only the pairs it owns
 * (where its real particle has the smaller global id), so the lists are
 * gathered before they are compared with the single-rank brute-force oracle.
 * MPI_Allgatherv is used instead of a reduce because the comparison is exact.
 */
GidPairSet enginePairs(DemAmrexEngine& engine)
{
  std::vector<GidPair> pairs;
  engine.contactPairs(pairs);

  std::vector<long> send;
  send.reserve(2 * pairs.size());
  for (std::size_t i = 0; i < pairs.size(); ++i) {
    send.push_back(pairs[i].first);
    send.push_back(pairs[i].second);
  }

  const int nProcs = (int)amrex::ParallelDescriptor::NProcs();
  const int localCount = (int)send.size();
  std::vector<int> counts(nProcs, 0);
  MPI_Allgather(&localCount, 1, MPI_INT, counts.data(), 1, MPI_INT,
                amrex::ParallelDescriptor::Communicator());

  std::vector<int> displs(nProcs, 0);
  int total = 0;
  for (int r = 0; r < nProcs; ++r) {
    displs[r] = total;
    total += counts[r];
  }

  std::vector<long> gathered(total, 0);
  MPI_Allgatherv(send.empty() ? nullptr : send.data(), localCount, MPI_LONG,
                 gathered.empty() ? nullptr : gathered.data(), counts.data(),
                 displs.data(), MPI_LONG,
                 amrex::ParallelDescriptor::Communicator());

  GidPairSet out;
  for (int i = 0; i + 1 < total; i += 2) {
    out.insert(GidPair(std::min(gathered[i], gathered[i + 1]),
                       std::max(gathered[i], gathered[i + 1])));
  }
  return out;
}

void reportSetDiff(const char* what, const GidPairSet& expected, const GidPairSet& got)
{
  std::vector<GidPair> diff;
  for (GidPairSet::const_iterator it = expected.begin(); it != expected.end(); ++it) {
    if (got.count(*it) == 0) {
      diff.push_back(*it);
    }
  }
  say("    %s: %d pairs, first %d missing:", what, (int)expected.size(),
      (int)std::min<std::size_t>(diff.size(), 5));
  for (std::size_t i = 0; i < diff.size() && i < 5; ++i) {
    say(" (%lld,%lld)", diff[i].first, diff[i].second);
  }
  say("\n");
}

/* Summed state digest used for the partition invariance check. */
struct Digest
{
  iREAL pos[3];
  iREAL vel[3];
  iREAL angMom[3];

  Digest() { for (int d = 0; d < 3; ++d) { pos[d] = vel[d] = angMom[d] = 0.0; } }

  iREAL maxAbsDiff(const Digest& o) const
  {
    iREAL m = 0.0;
    for (int d = 0; d < 3; ++d) {
      m = std::max(m, std::fabs(pos[d] - o.pos[d]));
      m = std::max(m, std::fabs(vel[d] - o.vel[d]));
      m = std::max(m, std::fabs(angMom[d] - o.angMom[d]));
    }
    return m;
  }

  iREAL maxRelDiff(const Digest& o) const
  {
    iREAL m = 0.0;
    for (int d = 0; d < 3; ++d) {
      const iREAL scale = std::max(std::fabs(pos[d]), std::fabs(o.pos[d]));
      if (scale > 0.0) { m = std::max(m, std::fabs(pos[d] - o.pos[d]) / scale); }
      const iREAL scaleV = std::max(std::fabs(vel[d]), std::fabs(o.vel[d]));
      if (scaleV > 0.0) { m = std::max(m, std::fabs(vel[d] - o.vel[d]) / scaleV); }
      const iREAL scaleL = std::max(std::fabs(angMom[d]), std::fabs(o.angMom[d]));
      if (scaleL > 0.0) { m = std::max(m, std::fabs(angMom[d] - o.angMom[d]) / scaleL); }
    }
    return m;
  }
};

Digest digestOf(std::vector<ParticleRecord>& records)
{
  Digest d;
  for (std::size_t i = 0; i < records.size(); ++i) {
    ParticleRecord& r = records[i];
    const iREAL m = r.getMass();
    for (int k = 0; k < 3; ++k) {
      d.pos[k] += r._centre[k];
      d.vel[k] += r._linearVelocity[k];
    }
    d.angMom[0] += m * (r._centre[1] * r._linearVelocity[2] - r._centre[2] * r._linearVelocity[1]);
    d.angMom[1] += m * (r._centre[2] * r._linearVelocity[0] - r._centre[0] * r._linearVelocity[2]);
    d.angMom[2] += m * (r._centre[0] * r._linearVelocity[1] - r._centre[1] * r._linearVelocity[0]);
  }
  return d;
}

void printDigest(const char* label, const Digest& d)
{
  say("  %s pos (%.17g %.17g %.17g)\n", label, d.pos[0], d.pos[1], d.pos[2]);
  say("  %s vel (%.17g %.17g %.17g)\n", label, d.vel[0], d.vel[1], d.vel[2]);
  say("  %s angMom (%.17g %.17g %.17g)\n", label, d.angMom[0], d.angMom[1], d.angMom[2]);
}

}  // namespace

int main(int argc, char** argv)
{
  amrex::Initialize(argc, argv);
  {
    const iREAL radius = 0.02;
    const iREAL epsilon = 0.01;
    const iREAL dt = 1.0e-4;
    const std::array<iREAL, 3> gravity = {{0.0, -9.8, 0.0}};
    std::array<iREAL, 6> boundary = {{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}};
    const delta::core::data::Meta::Simulation meta = baseMeta(dt);

    say("== delta dem phase1 amrex test: %d rank(s)\n",
        amrex::ParallelDescriptor::NProcs());

    // ---- scenario --------------------------------------------------------
    std::vector<delta::world::structure::Object> objects =
        sphereScene(300, 3, radius, epsilon, 11u);
    say("   scenario: %d spheres (300 random + 27 lattice)\n", (int)objects.size());

    // Serial reference engine; its records are also the initial state of the
    // parallel engines. _centre and _centreOfMass must agree bit for bit
    // because the parallel container carries a single position.
    std::vector<delta::world::structure::Object> objectsForSerial = objects;
    delta::core::Engine serial(objectsForSerial, boundary, meta);
    std::vector<ParticleRecord> initial = serial.getParticleRecords();
    for (std::size_t i = 0; i < initial.size(); ++i) {
      CHECK(initial[i]._centre == initial[i]._centreOfMass);
    }

    // ---- 1. pair set vs brute force --------------------------------------
    {
      GidPairSet oracle = serialContactPairs(objects, boundary, meta);

      std::vector<ParticleRecord> state = initial;
      DemAmrexEngine engine(state, manyBoxes());
      CHECK(engine.sizingIsSound());
      const iREAL reach = engine.reach();

      engine.detectAndDeriveForces(dt);
      GidPairSet got = enginePairs(engine);
      const long long gotCount = engine.contactPairCount();

      GidPairSet withinReach = reachPairs(initial, reach);

      say("  1. pairs: brute-force serial %d, amrex %d (reduced %lld), within reach %d, boxes %d\n",
          (int)oracle.size(), (int)got.size(), gotCount, (int)withinReach.size(),
          engine.container().ParticleDistributionMap(0).size());

      bool missing = false;
      for (GidPairSet::const_iterator it = oracle.begin(); it != oracle.end(); ++it) {
        if (got.count(*it) == 0) { missing = true; break; }
      }
      bool extra = false;
      for (GidPairSet::const_iterator it = got.begin(); it != got.end(); ++it) {
        if (oracle.count(*it) == 0) { extra = true; break; }
      }
      if (missing) { reportSetDiff("brute-force serial pairs missing from amrex", oracle, got); }
      if (extra) { reportSetDiff("amrex pairs not in brute-force serial", got, oracle); }
      // Every resolved contact must also be within the reach the halo covers.
      bool outsideReach = false;
      for (GidPairSet::const_iterator it = got.begin(); it != got.end(); ++it) {
        if (withinReach.count(*it) == 0) { outsideReach = true; break; }
      }
      CHECK(!missing);
      CHECK(!extra);
      CHECK(!outsideReach);
      CHECK(got.size() == oracle.size());
      CHECK(gotCount == (long long)got.size());

      /* The global id -> (grid, tile, index) lookup is local: every particle
       * must show up on exactly one rank, and there it must sit in the
       * container's own coordinates where it was seeded. */
      for (int k = 0; k < 3; ++k) {
        const int gid = initial[k].getGlobalParticleID();
        int lev = -1, grid = -1, tile = -1, index = -1;
        const bool here = engine.findParticle(gid, lev, grid, tile, index);
        int owners = here ? 1 : 0;
        MPI_Allreduce(MPI_IN_PLACE, &owners, 1, MPI_INT, MPI_SUM,
                      amrex::ParallelDescriptor::Communicator());
        CHECK(owners == 1);
        if (here) {
          const auto ptd =
              engine.container().ParticlesAt(lev, grid, tile).getConstParticleTileData();
          CHECK(ptd.idata(delta::core::parallel::DemIntComp::globalId)[index] == gid);
          for (int d = 0; d < 3; ++d) {
            CHECK(ptd.pos(d, index) == initial[k]._centre[d]);
          }
        }
      }
      int lev = -1, grid = -1, tile = -1, index = -1;
      CHECK(!engine.findParticle(-12345, lev, grid, tile, index));
    }

    // ---- 2. one force pass == serial -------------------------------------
    {
      std::vector<delta::world::structure::Object> local = objects;
      delta::core::Engine serialA(local, boundary, meta);
      std::vector<ParticleRecord> state = serialA.getParticleRecords();
      serialA.contactDetection();
      serialA.deriveForces();
      const std::vector<ParticleRecord>& ser = serialA.getParticleRecords();

      DemAmrexEngine engine(state, manyBoxes());
      engine.detectAndDeriveForces(dt);
      std::vector<ParticleRecord> got = state;
      engine.syncTo(got);

      iREAL maxAbsVel = 0.0, maxAbsOmega = 0.0, maxRel = 0.0;
      for (std::size_t i = 0; i < got.size(); ++i) {
        for (int d = 0; d < 3; ++d) {
          const iREAL dv = std::fabs(got[i]._linearVelocity[d] - ser[i]._linearVelocity[d]);
          const iREAL dw = std::fabs(got[i]._refAngularVelocity[d] - ser[i]._refAngularVelocity[d]);
          maxAbsVel = std::max(maxAbsVel, dv);
          maxAbsOmega = std::max(maxAbsOmega, dw);
          const iREAL scale = std::max(std::fabs(ser[i]._linearVelocity[d]), 1.0);
          maxRel = std::max(maxRel, dv / scale);
          const iREAL scaleW = std::max(std::fabs(ser[i]._refAngularVelocity[d]), 1.0);
          maxRel = std::max(maxRel, dw / scaleW);
        }
        CHECK(got[i]._centre == ser[i]._centre);
      }

      say("  2. force pass vs serial: %d particles, max|dv| %.3e, max|domega| %.3e, max rel %.3e\n",
          (int)got.size(), maxAbsVel, maxAbsOmega, maxRel);
      say("     max|F| amrex %.17g serial %.17g\n",
          engine.maxForceMagnitude(), serialA.getMaxForceMagnitude());
      CHECK(maxAbsVel <= 1.0e-12);
      CHECK(maxAbsOmega <= 1.0e-12);
      CHECK(maxRel <= 1.0e-12);
    }

    // ---- 3. partition invariance -----------------------------------------
    {
      DemParallelConfig single;
      single.maxGridSize = 0;  /* one box, one tile per rank */
      DemParallelConfig many;
      many.maxGridSize = 4;    /* 27 boxes: pairs span tiles and boundaries */

      std::vector<ParticleRecord> state = initial;
      DemAmrexEngine oneTile(state, single);
      DemAmrexEngine manyTiles(state, many);
      CHECK(oneTile.sizingIsSound());
      CHECK(manyTiles.sizingIsSound());
      const int steps = 3;
      /* A lockstep serial run over the same steps, so the invariant is checked
       * against the reference as well as between the two partitions. Delta's
       * per-particle impulse is summed over contacts in a different order than
       * serial's per-pair velocity updates, so agreement is 1 ulp per step and
       * amplifies slightly; the tolerance is far below any physical effect. */
      std::vector<delta::world::structure::Object> serObjects = objects;
      delta::core::Engine serialRef(serObjects, boundary, meta);
      std::vector<ParticleRecord> previous = initial;
      iREAL maxSerialPos = 0.0;
      for (int s = 0; s < steps; ++s) {
        oneTile.step(dt, gravity);
        manyTiles.step(dt, gravity);
        serialRef.iterate();
        std::vector<ParticleRecord> ser = serialRef.getParticleRecords();
        std::vector<ParticleRecord> s1 = previous;
        std::vector<ParticleRecord> s2 = previous;
        oneTile.syncTo(s1);
        manyTiles.syncTo(s2);
        for (std::size_t i = 0; i < ser.size(); ++i) {
          for (int d = 0; d < 3; ++d) {
            maxSerialPos = std::max(maxSerialPos,
                                    std::fabs(s1[i]._centre[d] - ser[i]._centre[d]));
            maxSerialPos = std::max(maxSerialPos,
                                    std::fabs(s2[i]._centre[d] - ser[i]._centre[d]));
          }
        }
        /* The contact set itself is not compared here: with contact distances
         * this stiff, a 1-ulp position difference can add or remove a pair
         * sitting exactly at the contact threshold, so only the trajectory is.
         * Test 1 compares pair sets exactly, on an identical state. */
      }
      say("     3-step lockstep vs serial: max|dpos| %.3e (both partitions)\n", maxSerialPos);
      /* Tight on purpose: the observed drift is ~1e-14, so an order of
       * magnitude more means the parallel path dropped a physics update (it is
       * how a missing angular-velocity write-back was caught). */
      CHECK(maxSerialPos <= 1.0e-13);

      const long long nOne = oneTile.particleCount();
      const long long nMany = manyTiles.particleCount();
      const long long cOne = oneTile.contactPairCount();
      const long long cMany = manyTiles.contactPairCount();

      std::vector<ParticleRecord> stateOne = initial;
      std::vector<ParticleRecord> stateMany = initial;
      oneTile.syncTo(stateOne);
      manyTiles.syncTo(stateMany);
      const Digest dOne = digestOf(stateOne);
      const Digest dMany = digestOf(stateMany);

      say("  3. partition invariance after %d steps: particles %lld vs %lld, contacts %lld vs %lld\n",
          steps, nOne, nMany, cOne, cMany);
      printDigest("1tile", dOne);
      printDigest("27box", dMany);
      say("     27box-vs-1tile max abs %.3e, max rel %.3e\n",
          dMany.maxAbsDiff(dOne), dMany.maxRelDiff(dOne));

      CHECK(nOne == nMany);
      CHECK(nOne == (long long)initial.size());
      CHECK(cOne == cMany);
      CHECK(dMany.maxRelDiff(dOne) <= 1.0e-12);
      for (std::size_t i = 0; i < stateMany.size(); ++i) {
        CHECK(std::isfinite(stateMany[i]._centre[0]));
        CHECK(std::isfinite(stateMany[i]._linearVelocity[0]));
      }
    }

    // ---- 4. 1000-sphere pile, 20 steps -----------------------------------
    {
      std::vector<delta::world::structure::Object> local =
          spherePile(10, radius, epsilon, 13u);
      std::vector<delta::world::structure::Object> localForSerial = local;
      delta::core::Engine serialPile(localForSerial, boundary, meta);
      std::vector<ParticleRecord> state = serialPile.getParticleRecords();

      DemAmrexEngine engine(state, manyBoxes());
      CHECK(engine.sizingIsSound());

      long long maxContacts = 0;
      iREAL maxForce = 0.0;
      const int steps = 20;
      for (int s = 0; s < steps; ++s) {
        engine.step(dt, gravity);
        /* the serial pile is stepped in lockstep so the pile result is checked
         * against the reference and not just for finiteness */
        serialPile.iterate();
        CHECK(engine.particleCount() == 1000);
        maxContacts = std::max(maxContacts, engine.contactPairCount());
        maxForce = std::max(maxForce, engine.maxForceMagnitude());
      }

      std::vector<ParticleRecord> got = state;
      engine.syncTo(got);
      iREAL maxPos = 0.0, maxVel = 0.0, maxOmega = 0.0;
      for (std::size_t i = 0; i < got.size(); ++i) {
        CHECK(std::isfinite(got[i]._centre[0]) && std::isfinite(got[i]._centre[1]) &&
              std::isfinite(got[i]._centre[2]));
        CHECK(std::isfinite(got[i]._linearVelocity[0]) && std::isfinite(got[i]._linearVelocity[1]) &&
              std::isfinite(got[i]._linearVelocity[2]));
        CHECK(std::isfinite(got[i]._refAngularVelocity[0]) && std::isfinite(got[i]._refAngularVelocity[1]) &&
              std::isfinite(got[i]._refAngularVelocity[2]));
        maxPos = std::max(maxPos, std::fabs(got[i]._centre[0]));
        maxVel = std::max(maxVel, std::fabs(got[i]._linearVelocity[0]));
        maxOmega = std::max(maxOmega, std::fabs(got[i]._refAngularVelocity[0]));
      }
      const std::vector<ParticleRecord>& ser = serialPile.getParticleRecords();
      iREAL maxSerialPos = 0.0;
      iREAL maxSerialVel = 0.0;
      for (std::size_t i = 0; i < got.size(); ++i) {
        for (int d = 0; d < 3; ++d) {
          maxSerialPos = std::max(maxSerialPos, std::fabs(got[i]._centre[d] - ser[i]._centre[d]));
          maxSerialVel = std::max(maxSerialVel,
                                  std::fabs(got[i]._linearVelocity[d] - ser[i]._linearVelocity[d]));
        }
      }

      say("  4. pile: %d particles, %d steps, max contacts %lld, max|F| %.6g\n",
          (int)got.size(), steps, maxContacts, maxForce);
      say("     max|x| %.6g max|vx| %.6g max|wx| %.6g\n", maxPos, maxVel, maxOmega);
      say("     20 steps vs serial: max|dpos| %.3e max|dv| %.3e\n", maxSerialPos, maxSerialVel);
      /* Printed by every rank: it is the evidence that the pile really is spread
       * over the ranks instead of all resident on one of them. */
      {
        const amrex::DistributionMapping& dm = engine.container().ParticleDistributionMap(0);
        const int me = (int)amrex::ParallelDescriptor::MyProc();
        int owned = 0;
        for (int b = 0; b < dm.size(); ++b) {
          if (dm[b] == me) { ++owned; }
        }
        std::printf("     rank %d: %d of %d boxes, %lld of %lld particles\n", me, owned,
                    dm.size(), (long long)engine.container().TotalNumberOfParticles(true, true),
                    engine.expectedParticleCount());
        std::fflush(stdout);
      }
      CHECK(maxContacts > 0);
      CHECK(maxForce > 0.0);
      CHECK(std::isfinite(maxForce));
      /* Tight on purpose: the observed drift is ~1e-14, so an order of
       * magnitude more means the parallel path dropped a physics update (it is
       * how a missing angular-velocity write-back was caught). */
      CHECK(maxSerialPos <= 1.0e-13);
      CHECK(maxSerialVel <= 1.0e-9);
    }

    // ---- 5. sizing guard, and the contact drop it prevents ----------------
    {
      const iREAL reach = 0.06;
      /* The engine requests 2*reach because AMReX divides the tile extent into
       * ceil(extent/bin_size) bins; see sizingDiagnostic. */
      CHECK(DemParticleContainer::sizingDiagnostic(reach, reach, 1, 2.0 * reach) == nullptr);
      const char* tooFewCells = DemParticleContainer::sizingDiagnostic(reach, reach / 4.0, 1, 2.0 * reach);
      const char* tooSmallBins = DemParticleContainer::sizingDiagnostic(reach, reach, 1, reach);
      const char* noGhst = DemParticleContainer::sizingDiagnostic(reach, reach, 0, 2.0 * reach);
      CHECK(tooFewCells != nullptr);
      CHECK(tooSmallBins != nullptr);
      CHECK(noGhst != nullptr);
      say("  5. sizing guard: undersized halo -> \"%s\"\n", tooFewCells);
      say("     undersized bins -> \"%s\"\n", tooSmallBins);

      GidPairSet oracle = serialContactPairs(objects, boundary, meta);

      DemParallelConfig undersized;
      undersized.cellsPerReach = 4.0;   /* dx = reach/4: halo too thin */
      undersized.enforceSizing = false; /* run it anyway, to show the drop */
      std::vector<ParticleRecord> state = initial;
      DemAmrexEngine engine(state, undersized);
      const char* why = engine.sizingDiagnostic();
      CHECK(why != nullptr);
      say("     undersized engine refuses by default: \"%s\"\n", why);

      engine.detectAndDeriveForces(dt);
      GidPairSet got = enginePairs(engine);
      say("     undersized engine resolved %d pairs vs brute-force %d (dropped %d)\n",
          (int)got.size(), (int)oracle.size(), (int)(oracle.size() - got.size()));
      CHECK(got.size() < oracle.size());
    }

    // ---- 6. AMReX decomposition -> VTK -----------------------------------
    {
      std::vector<ParticleRecord> state = initial;
      DemAmrexEngine engine(state, manyBoxes());
      const std::string dir = "/tmp/delta_amrex_grid_vtk/";
      std::system(("mkdir -p " + dir).c_str());
      engine.writeToVTK(dir, 0);

      if (amrex::ParallelDescriptor::IOProcessor()) {
        const int nboxes = (int)engine.container().ParticleBoxArray(0).size();
        std::ifstream in(dir + "grid_0.vtu");
        CHECK(in.good());
        std::string body((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
        char points[64];
        std::snprintf(points, sizeof(points), "NumberOfPoints=\"%d\"", nboxes * 8);
        CHECK(body.find("type=\"UnstructuredGrid\"") != std::string::npos);
        CHECK(body.find(points) != std::string::npos);
        std::ifstream pvd(dir + "grid.pvd");
        CHECK(pvd.good());
        say("  6. AMReX decomposition -> VTK: %d boxes, %d points in grid_0.vtu (+ grid.pvd)\n",
            nboxes, nboxes * 8);
      }
    }

    say("OK dem_phase1_amrex_test: all checks passed\n");
  }
  amrex::Finalize();
  return 0;
}
