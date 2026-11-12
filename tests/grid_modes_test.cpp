/*
 * GridMode comparison (thesis 6.3): the broadphase grid must change which pairs
 * are *tested*, never which pairs are *in contact*.
 *
 *   - NoGrid resolves dx from the largest body, Regular from the smallest.
 *     With mixed body sizes Regular must therefore build a finer grid and a
 *     wider halo, or it is not doing anything.
 *   - Both grids must be sound (nneighbor*dx >= reach, bin_size >= 2*reach),
 *     which Regular only achieves if the halo is widened automatically.
 *   - The contact pair set must be identical: same narrowphase, same state, so
 *     any difference is the grid having missed or invented a candidate.
 *
 * A uniform scene cannot distinguish the two modes (minReach == maxReach), so
 * the scene below mixes three body sizes on purpose.
 *
 * Run: ctest --test-dir build  (or ./build/delta_grid_modes_test)
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <AMReX.H>

#include "dim3/backends/serial/Engine.h"
#include "core/data/Meta.h"
#include "core/data/ParticleRecord.h"
#include "dim3/backends/amrex/DemAmrexEngine.h"
#include "dim3/geometry/material.h"
#include "dim3/world/structure/Object.h"

using delta::core::data::ParticleRecord;
using delta::core::parallel::DemAmrexEngine;
using delta::core::parallel::DemParallelConfig;
using delta::core::parallel::GridMode;
using delta::world::structure::Object;

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond);          \
      std::exit(1);                                                        \
    }                                                                      \
  } while (0)

namespace {

delta::core::data::Meta::Simulation baseMeta(iREAL dt)
{
  delta::core::data::Meta::Simulation meta;
  meta.plotScheme = delta::core::data::Meta::Plot::Never;
  meta.modelScheme = delta::core::data::Meta::CollisionModel::Sphere;
  meta.overlapPreCheck = false;
  meta.dt = dt;
  meta.gravity = false;
  meta.gravityVector = std::array<iREAL, 3>{{0.0, 0.0, 0.0}};
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;
  return meta;
}

/* Mixed sizes on a lattice: 4:1 between the smallest and largest body, and a
 * spacing small enough that plenty of pairs are within reach. */
std::vector<Object> mixedScene(iREAL epsilon)
{
  const iREAL radii[3] = {0.01, 0.02, 0.04};
  const iREAL spacing  = 0.05;
  const iREAL density  = 20000.0;
  std::vector<Object> objects;
  int id = 0;
  for (int x = 0; x < 4; ++x)
    for (int y = 0; y < 4; ++y)
      for (int z = 0; z < 4; ++z) {
        const iREAL radius = radii[(x + y + z) % 3];
        const std::array<iREAL, 3> centre = {
            0.1 + x * spacing, 0.1 + y * spacing, 0.1 + z * spacing};
        Object o("sphere", radius, id++, centre,
                 delta::geometry::material::MaterialType::WOOD, false, true, true,
                 epsilon, std::array<iREAL, 3>{{0, 0, 0}},
                 std::array<iREAL, 3>{{0, 0, 0}});
        o.setMass(density * (4.0 / 3.0) * 3.14159265358979323846 * radius * radius * radius);
        objects.push_back(o);
      }
  return objects;
}

std::vector<std::pair<long long, long long> > pairsOf(DemAmrexEngine& engine)
{
  std::vector<std::pair<long long, long long> > pairs;
  engine.contactPairs(pairs);
  return pairs;
}

}  // namespace

int main(int argc, char** argv)
{
  amrex::Initialize(argc, argv);

  const iREAL epsilon = 0.005;
  const iREAL dt = 1e-5;
  const std::array<iREAL, 6> boundary = {{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}};
  delta::core::data::Meta::Simulation meta = baseMeta(dt);

  // Initial state through the serial engine: _centre and _centreOfMass must
  // agree, because the container carries one position per particle.
  std::vector<Object> objects = mixedScene(epsilon);
  std::vector<Object> forSerial = objects;
  delta::core::Engine serial(forSerial, boundary, meta);
  std::vector<ParticleRecord> initial = serial.getParticleRecords();
  CHECK(initial.size() == objects.size());

  DemParallelConfig cfgNoGrid;
  cfgNoGrid.mode = GridMode::NoGrid;
  DemParallelConfig cfgRegular;
  cfgRegular.mode = GridMode::Regular;
  DemParallelConfig cfgAdaptive;
  cfgAdaptive.mode = GridMode::Adaptive;

  std::vector<ParticleRecord> stateA = initial;
  std::vector<ParticleRecord> stateB = initial;
  std::vector<ParticleRecord> stateC = initial;

  DemAmrexEngine noGrid(stateA, cfgNoGrid);
  DemAmrexEngine regular(stateB, cfgRegular);
  DemAmrexEngine adaptive(stateC, cfgAdaptive);

  CHECK(noGrid.sizingIsSound());
  CHECK(regular.sizingIsSound());

  // The mode must actually change the grid, otherwise the comparison is empty.
  CHECK(regular.minReach() < regular.reach());
  CHECK(regular.dx() < noGrid.dx());
  CHECK(regular.nneighbor() >= noGrid.nneighbor());

  noGrid.detectAndDeriveForces(dt);
  regular.detectAndDeriveForces(dt);
  adaptive.detectAndDeriveForces(dt);

  std::vector<std::pair<long long, long long> > pairsNoGrid = pairsOf(noGrid);
  std::vector<std::pair<long long, long long> > pairsRegular = pairsOf(regular);
  std::vector<std::pair<long long, long long> > pairsAdaptive = pairsOf(adaptive);

  CHECK(!pairsNoGrid.empty());
  /* The grid may change which pairs are tested, never which are in contact. */
  CHECK(pairsNoGrid == pairsRegular);
  CHECK(pairsNoGrid == pairsAdaptive);

  std::printf("delta_grid_modes_test OK: %zu bodies, %zu contacts; "
              "nogrid dx=%g nneighbor=%d, regular dx=%g nneighbor=%d, "
              "adaptive %d levels\n",
              objects.size(), pairsNoGrid.size(), (double)noGrid.dx(),
              noGrid.nneighbor(), (double)regular.dx(), regular.nneighbor(),
              adaptive.levels());

  amrex::Finalize();
  return 0;
}
