/*
 The MIT License (MIT)

 Copyright (c) 2022 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "DemAmrexEngine.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <set>

#include <AMReX.H>
#include <AMReX_MFIter.H>
#include <AMReX_ParallelDescriptor.H>

#include "../../contact/detection/sphere.h"
#include "../../contact/forces/forces.h"
#include "../../dynamics/dynamics.h"

namespace {

/*
 * ParticleRecord's accessors are not const-qualified even though none of them
 * mutate, and ParticleRecord.h is read-only for this change, so a const_cast is
 * used to read them from the const input vector. Safe: only getters are called.
 */
inline delta::core::data::ParticleRecord& readable(const delta::core::data::ParticleRecord& r)
{
  return const_cast<delta::core::data::ParticleRecord&>(r);
}

using delta::core::parallel::DemIntComp;
using delta::core::parallel::DemParticleContainer;
using delta::core::parallel::DemRealComp;

/*
 * AMReX CheckPair functor. AMReX calls it as check_pair(src[i], dst[j]) where
 * ParticleTileData::operator[] yields the AoS particle; for Particle<0,0> that
 * carries position and id/cpu only, which is all this reach test needs.
 *
 * The test is a superset of the exact kernel condition
 * |d - (rA+rB)| <= epsA+epsB (it uses the (diameter+2*eps)*1.1 halo reach), so
 * the kernel still decides whether a candidate really contacts. That is what
 * makes the pair set independent of the broad phase.
 */
struct DemCheckPair
{
  amrex::Real reach2;

  template <class P1, class P2>
  AMREX_GPU_HOST_DEVICE bool operator()(const P1& p1, const P2& p2) const noexcept
  {
    const amrex::ParticleReal dx = p1.pos(0) - p2.pos(0);
#if AMREX_SPACEDIM > 1
    const amrex::ParticleReal dy = p1.pos(1) - p2.pos(1);
#endif
#if AMREX_SPACEDIM > 2
    const amrex::ParticleReal dz = p1.pos(2) - p2.pos(2);
#endif
    const amrex::ParticleReal d2 = AMREX_D_TERM(dx*dx, + dy*dy, + dz*dz);
    return d2 <= reach2;
  }
};

/*
 * Everything the delta kernels need per particle. The SoA components are
 * separate single-component arrays, so the 3- and 9-vectors the kernels take as
 * pointers have to be gathered per particle.
 */
struct ParticleView
{
  iREAL pos[3];
  iREAL linVel[3];
  iREAL angVel[3];
  iREAL refAngVel[3];
  iREAL orient[9];
  iREAL inverse[9];
  iREAL inertia[9];
  iREAL mass;
  iREAL epsilon;
  iREAL diameter;
  int   material;
  bool  friction;
  long long gid;

  ParticleView() : mass(0.0), epsilon(0.0), diameter(0.0), material(0), friction(false), gid(0) {}
};

/* Object/Object.cpp inlines the halo diameter as (_diameter+epsilon*2)*1.1. */
inline iREAL haloDiameter(iREAL diameter, iREAL epsilon)
{
  return (diameter + epsilon*2) * 1.1;
}

template <class PTD>
void loadView(const PTD& ptd, int i, ParticleView& v)
{
  for (int d = 0; d < 3; ++d) {
    v.pos[d]       = ptd.pos(d, i);
    v.linVel[d]    = ptd.rdata(DemRealComp::linVel + d)[i];
    v.angVel[d]    = ptd.rdata(DemRealComp::angVel + d)[i];
    v.refAngVel[d] = ptd.rdata(DemRealComp::refAngVel + d)[i];
  }
  for (int d = 0; d < 9; ++d) {
    v.orient[d]  = ptd.rdata(DemRealComp::orientation + d)[i];
    v.inverse[d] = ptd.rdata(DemRealComp::inverse + d)[i];
    v.inertia[d] = ptd.rdata(DemRealComp::inertia + d)[i];
  }
  v.mass     = ptd.rdata(DemRealComp::mass)[i];
  v.epsilon  = ptd.rdata(DemRealComp::epsilon)[i];
  v.diameter = ptd.rdata(DemRealComp::diameter)[i];
  v.material = ptd.idata(DemIntComp::material)[i];
  v.friction = ptd.idata(DemIntComp::friction)[i] != 0;
  v.gid      = ptd.idata(DemIntComp::globalId)[i];
}

/*
 * B side of a pair is an obstacle: it lives in the replicated ParticleRecord
 * rather than in the container, so it needs its own loader. Uses the stored
 * _xCoordinates as the mesh and _centreOfMass as the contact reference, exactly
 * like Engine::evaluateCandidates does for the sphereWithBarrierAB branch.
 */
void loadObstacleView(const delta::core::data::ParticleRecord& o, ParticleView& v)
{
  delta::core::data::ParticleRecord& rr = readable(o);

  for (int d = 0; d < 3; ++d) {
    v.pos[d]       = o._centreOfMass[d];
    v.linVel[d]    = o._linearVelocity[d];
    v.angVel[d]    = o._angularVelocity[d];
    v.refAngVel[d] = o._refAngularVelocity[d];
  }
  for (int d = 0; d < 9; ++d) {
    v.orient[d]  = o._orientation[d];
  }
  const std::array<iREAL, 9> inv = rr.getInverse();
  const std::array<iREAL, 9> inr = rr.getInertia();
  for (int d = 0; d < 9; ++d) {
    v.inverse[d] = inv[d];
    v.inertia[d] = inr[d];
  }
  v.mass     = rr.getMass();
  v.epsilon  = rr.getEpsilon();
  v.diameter = rr.getDiameter();
  v.material = static_cast<int>(rr.getMaterial());
  v.friction = rr.getIsFriction();
  v.gid      = rr.getGlobalParticleID();
}

/*
 * Serial Engine::deriveForces body for one (local particle A, partner B) pair.
 * A is always the local real particle and the normals are used as the detection
 * produced them: serial stores every contact twice with the B copy's normal
 * negated, and sphere(A,B) is the bit-exact mirror of sphere(B,A), so this
 * reproduces serial's per-endpoint force without exchanging ghost forces.
 */
void accumulateContacts(ParticleView& a, ParticleView& b,
                        std::vector<delta::contact::contactpoint>& pts,
                        iREAL* force, iREAL* torque)
{
  iREAL rforce[3]  = {0.0, 0.0, 0.0};
  iREAL rtorque[3] = {0.0, 0.0, 0.0};

  delta::contact::forces::getContactsForces(
      pts,
      a.pos, a.pos, a.angVel, a.refAngVel, a.linVel, a.mass, a.inverse, a.orient, a.material,
      b.pos, b.pos, b.angVel, b.refAngVel, b.linVel, b.mass, b.inverse, b.orient, b.material,
      rforce, rtorque, true);

  for (int d = 0; d < 3; ++d) {
    force[d]  += rforce[d];
    torque[d] += rtorque[d];
  }
}

void abortHere(const std::string& what)
{
  std::fprintf(stderr, "delta DemAmrexEngine: %s\n", what.c_str());
  amrex::Abort(what.c_str());
}

}  // namespace

delta::core::parallel::DemAmrexEngine::DemAmrexEngine(
    const std::vector<Record>& records, const DemParallelConfig& config)
{
  initFrom(records, config);
}

delta::core::parallel::DemAmrexEngine::~DemAmrexEngine() = default;

void delta::core::parallel::DemAmrexEngine::initFrom(
    const std::vector<Record>& records, const DemParallelConfig& config)
{
  static_assert(sizeof(iREAL) == sizeof(amrex::Real),
                "delta iREAL and amrex::Real must agree in precision; the particles store iREAL");

  _config = config;
  _obstacles.clear();
  _indexOfGlobalId.clear();
  _pairList.clear();
  _contactPairs = 0;
  _maxForce = 0.0;
  _maxTorque = 0.0;
  _reach = 0.0;
  _maxHalo = 0.0;

  std::vector<const Record*> freeParticles;
  freeParticles.reserve(records.size());
  for (std::size_t k = 0; k < records.size(); ++k) {
    const Record& r = records[k];
    Record& rr = readable(r);
    _indexOfGlobalId[rr.getGlobalParticleID()] = static_cast<int>(k);
    if (rr.getIsObstacle()) {
      /* Obstacles stay out of the container in phase 1: AMReX's cross-container
       * pair path requires compatible iterators, and the serial obstacle scan is
       * a local sphere x mesh loop. They are replicated read-only instead. */
      _obstacles.push_back(r);
      continue;
    }
    freeParticles.push_back(&r);
    _reach = std::max(_reach, 0.5 * rr.getDiameter() + rr.getEpsilon());
    _maxHalo = std::max(_maxHalo, rr.getHaloDiameter());
  }
  _reach *= 2.0;
  _expectedParticles = static_cast<long long>(freeParticles.size());

  buildGrid(freeParticles);
  seedParticles(freeParticles);
}

void delta::core::parallel::DemAmrexEngine::buildGrid(
    const std::vector<const Record*>& freeParticles)
{
  iREAL lo[3] = {std::numeric_limits<iREAL>::max(), std::numeric_limits<iREAL>::max(),
                 std::numeric_limits<iREAL>::max()};
  iREAL hi[3] = {std::numeric_limits<iREAL>::lowest(), std::numeric_limits<iREAL>::lowest(),
                 std::numeric_limits<iREAL>::lowest()};
  for (std::size_t k = 0; k < freeParticles.size(); ++k) {
    const Record& r = *freeParticles[k];
    for (int d = 0; d < 3; ++d) {
      lo[d] = std::min(lo[d], r._centre[d]);
      hi[d] = std::max(hi[d], r._centre[d]);
    }
  }

  const iREAL margin = (_config.margin >= 0.0) ? static_cast<iREAL>(_config.margin)
                                               : 2.0 * _maxHalo;
  for (int d = 0; d < 3; ++d) {
    lo[d] -= margin;
    hi[d] += margin;
  }

  /* dx aims at reach/cellsPerReach and floor() keeps it >= reach for
   * cellsPerReach = 1. nx >= 1 always, so a box smaller than the reach yields
   * dx < reach and the sizing diagnostic below catches it. */
  const iREAL target = _reach / std::max(_config.cellsPerReach, 1.0e-12);
  int nx[3] = {1, 1, 1};
  iREAL dx[3] = {1.0, 1.0, 1.0};

  for (int d = 0; d < 3; ++d) {
    iREAL length = hi[d] - lo[d];
    if (!(length > 0.0)) {
      lo[d] -= _reach;
      hi[d] += _reach;
      length = hi[d] - lo[d];
    }
    if (!(length > 0.0)) {
      abortHere("degenerate particle bounding box: the domain would have zero extent");
    }
    nx[d] = std::max(1, static_cast<int>(std::floor(length / target)));
    dx[d] = length / static_cast<iREAL>(nx[d]);
  }
  _dxMin = std::min(dx[0], std::min(dx[1], dx[2]));

  const amrex::IntVect domLo(0, 0, 0);
  const amrex::IntVect domHi(nx[0] - 1, nx[1] - 1, nx[2] - 1);
  const amrex::Box domain(domLo, domHi);
  const amrex::RealBox realBox({lo[0], lo[1], lo[2]}, {hi[0], hi[1], hi[2]});
  const int periodic[3] = {0, 0, 0};

  _geom = amrex::Geometry(domain, &realBox, 0, periodic);
  _ba = amrex::BoxArray(domain);
  if (_config.maxGridSize > 0) {
    _ba.maxSize(_config.maxGridSize);
  }
  _dmap = amrex::DistributionMapping(_ba);
  _pc.reset(new DemParticleContainer(_geom, _dmap, _ba, _config.nneighbor));

  /* buildNeighborList(CheckPair, bin_size) is called with bin_size = 2*reach
   * (see forcePass); both conditions are still verified because AMReX's own
   * guard is an AMREX_ASSERT and this AMReX is built with assertions off. */
  const char* why = DemParticleContainer::sizingDiagnostic(
      _reach, _dxMin, _config.nneighbor, 2.0 * _reach);
  _sizingDiagnostic = (why != nullptr) ? std::string(why) : std::string();
  if (why != nullptr && _config.enforceSizing) {
    abortHere(std::string("unsound neighbour sizing: ") + why +
              "; increase cellsPerReach/nneighbor or disable enforceSizing");
  }
}

void delta::core::parallel::DemAmrexEngine::seedParticles(
    const std::vector<const Record*>& freeParticles)
{
  /* Seeded on one rank only, then Redistribute() spreads them. The seeding rank
   * is the owner of grid 0, which is rank 0 for the default mapping. */
  const int seedRank = _dmap[0];
  if (amrex::ParallelDescriptor::MyProc() == seedRank) {
    auto& ptile = _pc->DefineAndReturnParticleTile(0, 0, 0);
    ptile.resize(freeParticles.size());
    auto ptd = ptile.getParticleTileData();

    for (std::size_t k = 0; k < freeParticles.size(); ++k) {
      const Record& r = *freeParticles[k];
      Record& rr = readable(r);
      const int i = static_cast<int>(k);
      /* AMReX identifies a particle by its (id, cpu) pair when it builds the
       * Redistribute plan, so resize()'d particles must get unique ids before
       * they are moved. Delta's own identity is the globalId component. */
      ptd.id(i) = DemParticleContainer::ParticleType::NextID();
      ptd.cpu(i) = amrex::ParallelDescriptor::MyProc();
      /* Positions come from _centre, which is what the serial detection uses and
       * is bit-identical to _centreOfMass (both get the same increments). */
      ptd.pos(0, i) = r._centre[0];
      ptd.pos(1, i) = r._centre[1];
      ptd.pos(2, i) = r._centre[2];
      for (int d = 0; d < 3; ++d) {
        ptd.rdata(DemRealComp::linVel + d)[i]    = r._linearVelocity[d];
        ptd.rdata(DemRealComp::angVel + d)[i]    = r._angularVelocity[d];
        ptd.rdata(DemRealComp::refAngVel + d)[i] = r._refAngularVelocity[d];
        ptd.rdata(DemRealComp::force + d)[i]     = 0.0;
        ptd.rdata(DemRealComp::torque + d)[i]    = 0.0;
      }
      for (int d = 0; d < 9; ++d) {
        ptd.rdata(DemRealComp::orientation + d)[i] = r._orientation[d];
        ptd.rdata(DemRealComp::inverse + d)[i]     = rr.getInverse()[d];
        ptd.rdata(DemRealComp::inertia + d)[i]     = rr.getInertia()[d];
      }
      ptd.rdata(DemRealComp::mass)[i]     = rr.getMass();
      ptd.rdata(DemRealComp::epsilon)[i]  = rr.getEpsilon();
      ptd.rdata(DemRealComp::diameter)[i] = rr.getDiameter();
      ptd.idata(DemIntComp::material)[i]  = static_cast<int>(rr.getMaterial());
      ptd.idata(DemIntComp::friction)[i]  = rr.getIsFriction() ? 1 : 0;
      ptd.idata(DemIntComp::globalId)[i]  = rr.getGlobalParticleID();
    }
  }

  _pc->Redistribute();
  checkParticleCount("initFrom");
}

void delta::core::parallel::DemAmrexEngine::checkParticleCount(const char* where) const
{
  const long long n = particleCount();
  if (n != _expectedParticles) {
    char msg[512];
    std::snprintf(msg, sizeof(msg),
                  "particle count %lld != expected %lld after %s "
                  "(a particle left the domain or was lost in the neighbour exchange)",
                  n, _expectedParticles, where);
    abortHere(msg);
  }
}

long long delta::core::parallel::DemAmrexEngine::particleCount() const
{
  return static_cast<long long>(_pc->TotalNumberOfParticles(true, false));
}

void delta::core::parallel::DemAmrexEngine::step(iREAL dt, const std::array<iREAL, 3>& gravity)
{
  detectAndDeriveForces(dt);
  integrate(dt, gravity);
}

void delta::core::parallel::DemAmrexEngine::detectAndDeriveForces(iREAL dt)
{
  _pc->clearNeighbors();
  /* Redistribute() also rebuilds the neighbour machinery, so it has to happen
   * before the halo exchange: a particle that moved to another box would
   * otherwise be matched against the wrong tiles. */
  _pc->Redistribute();
  checkParticleCount("step");

  _pc->fillNeighbors();
  _pc->updateNeighbors();
  /* 2 * reach, not reach: see DemParticleContainer::sizingDiagnostic. AMReX
   * computes the bin width as extent/ceil(extent/bin_size), which can be half of
   * what is requested, and the +/-1 bin scan only covers every partner of
   * distance <= reach if the *actual* width is >= reach. */
  _pc->buildNeighborList(DemCheckPair{_reach * _reach}, 2.0 * _reach);

  forcePass(dt);
}

void delta::core::parallel::DemAmrexEngine::forcePass(iREAL dt)
{
  _maxForce = 0.0;
  _maxTorque = 0.0;
  _contactPairs = 0;
  _pairList.clear();

  /* Two loops on purpose. Serial deriveForces computes every particle's force
   * from a partner state frozen at detection time and only then moves on to the
   * next particle; applying the contact impulse inside the accumulation loop
   * would feed already-updated velocities into later pairs. The first loop
   * resolves contacts off the pre-step state into _forceCache, the second
   * applies them in serial deriveForces order. */
  for (amrex::MFIter mfi = _pc->MakeMFIter(0); mfi.isValid(); ++mfi)
  {
    const int grid = mfi.index();
    const int tile = mfi.LocalTileIndex();
    const DemParticleContainer::PairIndex key(grid, tile);
    /* Empty boxes have no particle tile and no neighbour list; skip them before
     * ParticlesAt() would materialise an empty tile. */
    if (!_pc->hasNeighborList(0, key)) {continue;}

    auto& ptile = _pc->ParticlesAt(0, mfi);
    auto ptd = ptile.getParticleTileData();
    const int npReal = static_cast<int>(ptile.numRealParticles());
    const int npTotal = static_cast<int>(ptile.numTotalParticles());

    const auto& nlist = _pc->neighborList(0, key);
    const auto& counts = nlist.GetCounts();   /* one per real particle */
    const auto& list = nlist.GetList();       /* 0-based tile indices */

    std::vector<iREAL> frc(3 * static_cast<std::size_t>(npReal), 0.0);
    std::vector<iREAL> trq(3 * static_cast<std::size_t>(npReal), 0.0);

    unsigned int offset = 0;
    for (int i = 0; i < npReal; ++i)
    {
      ParticleView a;
      loadView(ptd, i, a);
      iREAL force[3]  = {0.0, 0.0, 0.0};
      iREAL torque[3] = {0.0, 0.0, 0.0};

      const unsigned int count = counts[i];
      for (unsigned int q = offset; q < offset + count; ++q)
      {
        const int j = static_cast<int>(list[q]);
        if (j < 0 || j >= npTotal || j == i) {continue;}

        ParticleView b;
        loadView(ptd, j, b);

        std::vector<delta::contact::contactpoint> pts = delta::contact::detection::sphere(
            a.pos[0], a.pos[1], a.pos[2], a.diameter, a.epsilon, a.friction,
            static_cast<int>(a.gid),
            b.pos[0], b.pos[1], b.pos[2], b.diameter, b.epsilon, b.friction,
            static_cast<int>(b.gid));
        if (pts.empty()) {continue;}

        accumulateContacts(a, b, pts, force, torque);

        /* Unique global pair key: only the endpoint with the smaller global id
         * reports it. Every particle is real on exactly one rank and the halo
         * exchange is symmetric, so each pair is reported exactly once. */
        if (a.gid < b.gid) {
          _pairList.push_back(std::make_pair(a.gid, b.gid));
        }
      }
      offset += count;

      /* Static obstacles: replicated read-only records, sphere x mesh scan. */
      for (std::size_t o = 0; o < _obstacles.size(); ++o)
      {
        const Record& obstacle = _obstacles[o];
        Record& obs = readable(obstacle);
        if (!delta::contact::detection::isSphereOverlayInContact(
                a.pos[0], a.pos[1], a.pos[2], haloDiameter(a.diameter, a.epsilon),
                obstacle._centreOfMass[0], obstacle._centreOfMass[1], obstacle._centreOfMass[2],
                obs.getHaloDiameter())) {continue;}

        std::vector<delta::contact::contactpoint> pts = delta::contact::detection::sphereWithBarrierAB(
            a.pos[0], a.pos[1], a.pos[2], a.diameter, a.epsilon, a.friction,
            static_cast<int>(a.gid),
            obstacle._xCoordinates.data(), obstacle._yCoordinates.data(),
            obstacle._zCoordinates.data(), obs.getNumberOfTriangles(),
            obs.getEpsilon(), obs.getIsFriction(),
            obs.getGlobalParticleID());
        if (pts.empty()) {continue;}

        ParticleView b;
        loadObstacleView(obstacle, b);
        accumulateContacts(a, b, pts, force, torque);

        const long long obsGid = obs.getGlobalParticleID();
        _pairList.push_back(obsGid < a.gid ? std::make_pair(obsGid, a.gid)
                                           : std::make_pair(a.gid, obsGid));
      }

      const std::size_t base = 3 * static_cast<std::size_t>(i);
      for (int d = 0; d < 3; ++d) {
        frc[base + d] = force[d];
        trq[base + d] = torque[d];
      }

      const iREAL fmag = std::sqrt(force[0]*force[0] + force[1]*force[1] + force[2]*force[2]);
      if (fmag > _maxForce) {_maxForce = fmag;}
      const iREAL tmag = std::sqrt(torque[0]*torque[0] + torque[1]*torque[1] + torque[2]*torque[2]);
      if (tmag > _maxTorque) {_maxTorque = tmag;}

    }

    std::vector<iREAL>& acc = _forceCache[key];
    acc.resize(6 * static_cast<std::size_t>(npReal));
    for (int i = 0; i < npReal; ++i) {
      for (int d = 0; d < 3; ++d) {
        acc[6 * static_cast<std::size_t>(i) + d] = frc[3 * static_cast<std::size_t>(i) + d];
        acc[6 * static_cast<std::size_t>(i) + 3 + d] = trq[3 * static_cast<std::size_t>(i) + d];
      }
    }
  }

  /* Every pair is recorded exactly once globally (smaller global id wins), so
   * summing this over ranks is the unique global pair count. */
  _contactPairs = static_cast<long long>(_pairList.size());

  /* Pass B: apply the accumulated contact impulse, in serial deriveForces order
   * (linear velocity first, then updateAngular). */
  for (amrex::MFIter mfi = _pc->MakeMFIter(0); mfi.isValid(); ++mfi)
  {
    const int tile = mfi.LocalTileIndex();
    const DemParticleContainer::PairIndex key(mfi.index(), tile);
    const auto cached = _forceCache.find(key);
    if (cached == _forceCache.end()) {continue;}
    const std::vector<iREAL>& acc = cached->second;

    auto& ptile = _pc->ParticlesAt(0, mfi);
    auto ptd = ptile.getParticleTileData();
    const int npReal = static_cast<int>(ptile.numRealParticles());

    for (int i = 0; i < npReal; ++i)
    {
      const std::size_t base = 6 * static_cast<std::size_t>(i);
      iREAL force[3];
      iREAL torque[3];
      for (int d = 0; d < 3; ++d) {
        force[d]  = acc[base + d];
        torque[d] = acc[base + 3 + d];
        ptd.rdata(DemRealComp::force + d)[i]  = force[d];
        ptd.rdata(DemRealComp::torque + d)[i] = torque[d];
      }

      ParticleView a;
      loadView(ptd, i, a);
      if (!(a.mass > 0.0)) {continue;}

      for (int d = 0; d < 3; ++d) {
        ptd.rdata(DemRealComp::linVel + d)[i] = a.linVel[d] + dt * (force[d] / a.mass);
      }
      iREAL refAng[3] = {a.refAngVel[0], a.refAngVel[1], a.refAngVel[2]};
      delta::dynamics::updateAngular(refAng, a.orient, a.inertia, a.inverse, torque, dt);
      for (int d = 0; d < 3; ++d) {
        ptd.rdata(DemRealComp::refAngVel + d)[i] = refAng[d];
      }
    }
  }
}

void delta::core::parallel::DemAmrexEngine::integrate(iREAL dt,
                                                     const std::array<iREAL, 3>& gravity)
{
  for (amrex::MFIter mfi = _pc->MakeMFIter(0); mfi.isValid(); ++mfi)
  {
    auto& ptile = _pc->ParticlesAt(0, mfi);
    auto ptd = ptile.getParticleTileData();
    const int npReal = static_cast<int>(ptile.numRealParticles());

    for (int i = 0; i < npReal; ++i)
    {
      iREAL linVel[3];
      for (int d = 0; d < 3; ++d) {
        linVel[d] = ptd.rdata(DemRealComp::linVel + d)[i] + dt * gravity[d];
        ptd.rdata(DemRealComp::linVel + d)[i] = linVel[d];
      }
      for (int d = 0; d < 3; ++d) {
        ptd.pos(d, i) += dt * linVel[d];
      }
      iREAL angVel[3];
      iREAL refAng[3];
      iREAL orient[9];
      for (int d = 0; d < 3; ++d) {
        angVel[d] = ptd.rdata(DemRealComp::angVel + d)[i];
        refAng[d] = ptd.rdata(DemRealComp::refAngVel + d)[i];
      }
      for (int d = 0; d < 9; ++d) {
        orient[d] = ptd.rdata(DemRealComp::orientation + d)[i];
      }
      /* updateRotationMatrix() also writes the world-frame angular velocity
       * (rotated referential one), exactly like Engine::updatePosition. */
      delta::dynamics::updateRotationMatrix(angVel, refAng, orient, dt);
      for (int d = 0; d < 3; ++d) {
        ptd.rdata(DemRealComp::angVel + d)[i] = angVel[d];
      }
      for (int d = 0; d < 9; ++d) {
        ptd.rdata(DemRealComp::orientation + d)[i] = orient[d];
      }
    }
  }
}

void delta::core::parallel::DemAmrexEngine::syncTo(std::vector<Record>& out) const
{
  /* Every rank has to end up with the full state for the equivalence tests, so
   * each rank writes only the particles it owns and the buffers are summed
   * (zero-filled elsewhere). ponytail: O(N) allreduce of the whole state per
   * call; if a real run needs per-step output, gather only the requested ids. */
  const int N = static_cast<int>(out.size());
  const int fields = 22;
  std::vector<iREAL> buffer(static_cast<std::size_t>(N) * fields, 0.0);

  for (amrex::MFIter mfi = _pc->MakeMFIter(0); mfi.isValid(); ++mfi)
  {
    auto& ptile = _pc->ParticlesAt(0, mfi);
    auto ptd = ptile.getConstParticleTileData();
    const int npReal = static_cast<int>(ptile.numRealParticles());
    const int* gids = ptd.idata(DemIntComp::globalId);

    for (int i = 0; i < npReal; ++i)
    {
      std::map<int, int>::const_iterator it = _indexOfGlobalId.find(gids[i]);
      if (it == _indexOfGlobalId.end() || it->second < 0 || it->second >= N) {continue;}
      const std::size_t base = static_cast<std::size_t>(it->second) * fields;
      for (int d = 0; d < 3; ++d) {
        buffer[base + 0 + d]  = ptd.pos(d, i);
        buffer[base + 12 + d] = ptd.rdata(DemRealComp::linVel + d)[i];
        buffer[base + 15 + d] = ptd.rdata(DemRealComp::angVel + d)[i];
        buffer[base + 18 + d] = ptd.rdata(DemRealComp::refAngVel + d)[i];
      }
      for (int d = 0; d < 9; ++d) {
        buffer[base + 3 + d] = ptd.rdata(DemRealComp::orientation + d)[i];
      }
      buffer[base + 21] = 1.0;
    }
  }

  if (N > 0) {
    amrex::ParallelDescriptor::ReduceRealSum(buffer.data(), N * fields);
  }

  for (int k = 0; k < N; ++k)
  {
    const std::size_t base = static_cast<std::size_t>(k) * fields;
    if (buffer[base + 21] != 1.0) {
      char msg[512];
      std::snprintf(msg, sizeof(msg),
                    "particle %d (global id %d) was not found on exactly one rank "
                    "(marker %g); the container lost or duplicated it",
                    k, out[k].getGlobalParticleID(), buffer[base + 21]);
      abortHere(msg);
    }
    for (int d = 0; d < 3; ++d) {
      out[k]._centre[d] = buffer[base + 0 + d];
      out[k]._centreOfMass[d] = buffer[base + 0 + d];
      out[k]._linearVelocity[d] = buffer[base + 12 + d];
      out[k]._angularVelocity[d] = buffer[base + 15 + d];
      out[k]._refAngularVelocity[d] = buffer[base + 18 + d];
    }
    for (int d = 0; d < 9; ++d) {
      out[k]._orientation[d] = buffer[base + 3 + d];
    }
  }
}

long long delta::core::parallel::DemAmrexEngine::contactPairCount() const
{
  amrex::Long n = static_cast<amrex::Long>(_contactPairs);
  amrex::ParallelDescriptor::ReduceLongSum(n);
  return static_cast<long long>(n);
}

void delta::core::parallel::DemAmrexEngine::contactPairs(
    std::vector<std::pair<long long, long long> >& out) const
{
  std::set<std::pair<long long, long long> > unique(_pairList.begin(), _pairList.end());
  out.assign(unique.begin(), unique.end());
}

iREAL delta::core::parallel::DemAmrexEngine::maxForceMagnitude() const
{
  iREAL value = _maxForce;
  amrex::ParallelDescriptor::ReduceRealMax(value);
  return value;
}

iREAL delta::core::parallel::DemAmrexEngine::maxTorqueMagnitude() const
{
  iREAL value = _maxTorque;
  amrex::ParallelDescriptor::ReduceRealMax(value);
  return value;
}

bool delta::core::parallel::DemAmrexEngine::findParticle(int globalId, int& lev, int& grid,
                                                         int& tile, int& index) const
{
  for (amrex::MFIter mfi = _pc->MakeMFIter(0); mfi.isValid(); ++mfi)
  {
    const auto& ptile = _pc->ParticlesAt(0, mfi);
    auto ptd = ptile.getConstParticleTileData();
    const int* gids = ptd.idata(DemIntComp::globalId);
    const int npReal = static_cast<int>(ptile.numRealParticles());
    for (int i = 0; i < npReal; ++i) {
      if (gids[i] == globalId) {
        lev = 0;
        grid = mfi.index();
        tile = mfi.LocalTileIndex();
        index = i;
        return true;
      }
    }
  }
  return false;
}
