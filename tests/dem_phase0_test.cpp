/*
 * Phase 0-live checks for the serial DEM loop:
 *   1. the halo diameter is finite and larger than the diameter (Object ctors).
 *   2. gravity actually moves a free particle.
 *   3. the uniform-cell broad phase resolves exactly the same contact pairs as
 *      the brute-force O(N^2) scan (random cloud + regular lattice).
 *   4. a ~1000-sphere pile runs 20 steps with contacts enabled and stays finite.
 *
 * Run: ctest --test-dir build  (or ./build/delta_dem_test)
 *
 * Note: a local CHECK() is used rather than assert() because the default build
 * type here is Release (-DNDEBUG), which compiles assert() out.
 */

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "core/Engine.h"
#include "core/data/Meta.h"
#include "geometry/material.h"
#include "world/structure/Object.h"

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      std::abort();                                                        \
    }                                                                      \
  } while (0)

namespace {

iREAL sphereMass(iREAL radius, iREAL density = 20000.0) {
  return density * (4.0 / 3.0) * 3.14159265358979323846 * radius * radius * radius;
}

std::vector<delta::world::structure::Object> spherePile(
    int side, iREAL radius, iREAL epsilon, unsigned seed) {
  std::vector<delta::world::structure::Object> particles;
  std::mt19937 rng(seed);
  std::uniform_real_distribution<iREAL> jitter(-0.2 * radius, 0.2 * radius);

  // spacing < 2r + 2*epsilon -> touching, so the contact loop has work to do.
  iREAL spacing = 1.9 * (radius + epsilon);
  iREAL mass = sphereMass(radius);

  int id = 0;
  for (int x = 0; x < side; x++)
    for (int y = 0; y < side; y++)
      for (int z = 0; z < side; z++) {
        std::array<iREAL, 3> centre = {
            0.2 + x * spacing + jitter(rng),
            0.2 + y * spacing + jitter(rng),
            0.2 + z * spacing + jitter(rng)};
        delta::world::structure::Object o(
            "sphere", radius, id++, centre,
            delta::geometry::material::MaterialType::WOOD,
            false, true, true, epsilon,
            {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
        o.setMass(mass);
        particles.push_back(o);
      }
  return particles;
}

// Uniformly random spheres in a dense box: positions straddle cell boundaries,
// so a broad phase with a too-small cell size misses pairs.
std::vector<delta::world::structure::Object> sphereCloud(
    int n, iREAL radius, iREAL epsilon, unsigned seed) {
  std::mt19937 rng(seed);
  std::uniform_real_distribution<iREAL> pos(0.3, 0.7);
  iREAL mass = sphereMass(radius);

  std::vector<delta::world::structure::Object> particles;
  for (int i = 0; i < n; i++) {
    std::array<iREAL, 3> centre = {pos(rng), pos(rng), pos(rng)};
    delta::world::structure::Object o(
        "sphere", radius, i, centre,
        delta::geometry::material::MaterialType::WOOD,
        false, true, true, epsilon,
        {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
    o.setMass(mass);
    particles.push_back(o);
  }
  return particles;
}

std::set<std::pair<int, int> > detectPairs(
    std::vector<delta::world::structure::Object>& objects,
    std::array<iREAL, 6> boundary,
    const delta::core::data::Meta::Simulation& meta,
    bool bruteForce) {
  delta::core::Engine engine(objects, boundary, meta);
  if (bruteForce) {
    engine.contactDetectionBruteForce();
  } else {
    engine.contactDetection();
  }
  auto pairs = engine.getContactPairs();
  return std::set<std::pair<int, int> >(pairs.begin(), pairs.end());
}

delta::core::data::Meta::Simulation baseMeta(iREAL dt) {
  delta::core::data::Meta::Simulation meta;
  meta.plotScheme = delta::core::data::Meta::Plot::Never;
  meta.modelScheme = delta::core::data::Meta::CollisionModel::Sphere;
  meta.overlapPreCheck = false;
  meta.dt = dt;
  meta.gravity = false;
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;
  return meta;
}

}  // namespace

int main() {
  const iREAL radius = 0.02;
  const iREAL epsilon = 0.01;
  std::array<iREAL, 6> boundary = {{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}};

  // ---- 1. halo invariant -------------------------------------------------
  {
    auto objects = spherePile(1, radius, epsilon, 7u);
    iREAL halo = objects[0].getHaloDiameter();
    CHECK(std::isfinite(halo));
    CHECK(halo > objects[0].getDiameter());
    CHECK(std::fabs(objects[0].getRad() - radius) < 1e-12);
    CHECK(std::fabs(objects[0].getDiameter() - 2.0 * radius) < 1e-12);

    // The mesh-unspecified ctor has no radius either; its halo must still be
    // finite and larger than the (zero) diameter rather than reading garbage.
    delta::world::structure::Object meshless(
        "sphere", 0, {{0.5, 0.5, 0.5}},
        delta::geometry::material::MaterialType::WOOD,
        false, true, true, epsilon,
        {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
    CHECK(std::isfinite(meshless.getHaloDiameter()));
    CHECK(meshless.getHaloDiameter() > meshless.getDiameter());
  }

  // ---- 2. gravity actually applies ---------------------------------------
  {
    auto objects = spherePile(1, radius, epsilon, 11u);
    auto meta = baseMeta(1e-3);
    meta.gravity = true;
    delta::core::Engine engine(objects, boundary, meta);

    iREAL vy0 = engine.getParticleRecords()[0]._linearVelocity[1];
    engine.iterate();
    iREAL vy1 = engine.getParticleRecords()[0]._linearVelocity[1];

    // updatePosition() adds dt * (-g) to vy; the lone particle has no partner.
    CHECK(std::fabs((vy1 - vy0) + 9.8 * meta.dt) < 1e-9);
  }

  // ---- 3. binned vs brute-force equivalence ------------------------------
  {
    const unsigned seeds[] = {3u, 17u, 91u};
    for (int overlapCheck = 0; overlapCheck < 2; overlapCheck++)
      for (unsigned seed : seeds) {
        auto lattice = spherePile(4, radius, epsilon, seed);
        auto cloud = sphereCloud(400, radius, epsilon, seed);

        auto meta = baseMeta(1e-4);
        meta.overlapPreCheck = (overlapCheck != 0);

        auto latticeBinned = detectPairs(lattice, boundary, meta, false);
        auto latticeBrute = detectPairs(lattice, boundary, meta, true);
        CHECK(!latticeBinned.empty());
        CHECK(latticeBinned == latticeBrute);

        auto cloudBinned = detectPairs(cloud, boundary, meta, false);
        auto cloudBrute = detectPairs(cloud, boundary, meta, true);
        CHECK(!cloudBinned.empty());
        CHECK(cloudBinned == cloudBrute);
      }
    std::printf("equivalence OK: binned == brute force (lattice+cloud, overlapCheck on/off)\n");
  }

  // ---- 4. ~1000-sphere pile, 20 steps, contacts on -----------------------
  {
    auto objects = spherePile(10, radius, epsilon, 5u);
    CHECK(objects.size() == 1000u);

    auto meta = baseMeta(1e-4);
    meta.gravity = true;
    delta::core::Engine engine(objects, boundary, meta);

    // 1000-particle binned-vs-brute equivalence on the same initial state.
    auto binned = detectPairs(objects, boundary, meta, false);
    auto brute = detectPairs(objects, boundary, meta, true);
    CHECK(!binned.empty());
    CHECK(binned == brute);

    unsigned maxContacts = 0;
    for (int step = 0; step < 20; step++) {
      engine.iterate();
      unsigned contacts = (unsigned)engine.getContactPairs().size();
      if (contacts > maxContacts) maxContacts = contacts;
    }

    iREAL maxForce = engine.getMaxForceMagnitude();
    CHECK(std::isfinite(maxForce));
    CHECK(maxContacts > 0);

    for (auto& r : engine.getParticleRecords())
      for (int d = 0; d < 3; d++) {
        CHECK(std::isfinite(r._centre[d]));
        CHECK(std::isfinite(r._linearVelocity[d]));
        CHECK(std::isfinite(r._angularVelocity[d]));
      }

    std::printf(
        "dem_phase0_test OK: %u particles, 20 steps, max contacts=%u, max|force|=%g\n",
        (unsigned)objects.size(), maxContacts, maxForce);
  }

  return 0;
}
