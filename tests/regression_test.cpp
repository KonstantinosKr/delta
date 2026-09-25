/*
 * Regression checks for the latent bugs fixed on the amrex-integration branch.
 * Every block below fails (or crashes) against the pre-fix code:
 *
 *   L1  expmap() rotation matrix was not orthonormal (fixed)
 *   L2  Structure triangle count clobbered to 0 by a following sphere
 *   L3  unsupported CollisionModel silently resolved zero contacts
 *   L4  State collision counter never assigned -> log always printed cpt:0
 *   L5  sphere friction flag overwritten by isConvex
 *   L6  World bbox written to globalMin / boundary taken from the last body
 *   L7  Object inverse inertia stored as the plain inertia; max bbox = min vertex
 *   L8  Object::getNumberOfTriangles() null-derefs a meshless body
 *   I1  readPartGeometry null-derefs Assimp's scene for a missing mesh file
 *   I2  readmbfcp dropped every body's VELOCITY/FORCES and the material block
 *
 * Run: ctest --test-dir build  (or ./build/delta_regression_test)
 * CHECK() rather than assert() because the default build is Release (-DNDEBUG).
 */

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "core/Engine.h"
#include "core/data/Meta.h"
#include "core/data/Structure.h"
#include "core/io/read.h"
#include "dynamics/dynamics.h"
#include "geometry/material.h"
#include "world/World.h"
#include "world/structure/Object.h"

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      std::abort();                                                        \
    }                                                                      \
  } while (0)

namespace {

const iREAL kRadius = 0.02;
const iREAL kEpsilon = 0.01;
const std::array<iREAL, 6> kBoundary = {{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}};

delta::world::structure::Object sphere(int id, std::array<iREAL, 3> centre,
                                       bool isFriction) {
  return delta::world::structure::Object(
      "sphere", kRadius, id, centre, delta::geometry::material::MaterialType::WOOD,
      false, isFriction, true, kEpsilon, {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
}

delta::core::data::Meta::Simulation baseMeta() {
  delta::core::data::Meta::Simulation meta;
  meta.plotScheme = delta::core::data::Meta::Plot::Never;
  meta.modelScheme = delta::core::data::Meta::CollisionModel::Sphere;
  meta.overlapPreCheck = false;
  meta.dt = 1e-4;
  meta.gravity = false;
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;
  return meta;
}

// Minimal .mbfcp with one sphere body; the three variable lines are what the
// checks below bend.
void writeMbfcp(const char* path, const char* surfidLine, const char* velocityLine,
                const char* forcesLine) {
  std::ofstream out(path);
  out << "SURFACE_MATERIALS:\t1\nSURF1:\tANY\nSURF2:\tANY\nMODEL:\tSPRING_DASHPOT\n"
      << "FRICTION:\t0.25\nCOHESION:\t0\nSPRING:\t2e+05\nDASHPOT:\t-1\n\n"
      << "BULK_MATERIALS:\t1\nLABEL:\tBULK_MATERIAL_0\nMODEL:\tKIRCHHOFF\n"
      << "YOUNG:\t1e+09\nPOISSON:\t0.25\nDENSITY:\t1000\n\n"
      << "GRAVITY:\nCONSTANT:\t0\nCONSTANT:\t0\nCONSTANT:\t-10\n\n"
      << "BODIES:\t1\n\n"
      << "ID:\t7\nLABEL:\t(null)\nKINEMATICS:\tRIGID\nBULK_MATERIAL:\tBULK_MATERIAL_0\n"
      << "SHAPES:\t1\nSPHERES:\t1\nCENTER:\t0.5  0.5  0.5\nRADIUS:\t0.02\n"
      << surfidLine << "\n" << velocityLine << "\n" << forcesLine
      << "\n\nCONSTRAINTS:\t0\n";
  out.close();
}

bool close(iREAL a, iREAL b, iREAL tol = 1e-12) {
  return std::fabs(a - b) <= tol;
}

}  // namespace

int main() {
  // ---- L5: a sphere must keep the friction flag it was built with ---------
  {
    CHECK(sphere(0, {{0.5, 0.5, 0.5}}, /*isFriction=*/false).getIsFriction() == false);
    CHECK(sphere(0, {{0.5, 0.5, 0.5}}, /*isFriction=*/true).getIsFriction() == true);
  }

  // ---- L8: meshless body reports zero triangles instead of crashing ------
  {
    CHECK(sphere(0, {{0.5, 0.5, 0.5}}, true).getNumberOfTriangles() == 0);
  }

  // ---- L6: World boundary is the scene extent, not the last body's box ----
  {
    delta::world::World world("two-particles-crash", false,
                              delta::core::data::Meta::CollisionModel::Sphere, 1.0);
    auto objects = world.getObjects();
    CHECK(objects.size() == 2u);

    std::array<iREAL, 3> emin = {{1E99, 1E99, 1E99}};
    std::array<iREAL, 3> emax = {{-1E99, -1E99, -1E99}};
    for (auto& o : objects)
      for (int d = 0; d < 3; d++) {
        emin[d] = std::min(emin[d], o.getCentre()[d] - o.getRad());
        emax[d] = std::max(emax[d], o.getCentre()[d] + o.getRad());
      }

    auto b = world.getBoundary();
    for (int d = 0; d < 3; d++) {
      CHECK(close(b[d], emin[d]));
      CHECK(close(b[3 + d], emax[d]));
    }
  }

  // ---- L2 + L7: mesh objects keep triangle count / real inverse -----------
  {
    delta::world::World world("two-particles-crash", false,
                              delta::core::data::Meta::CollisionModel::BruteForce, 4);
    auto meshes = world.getObjects();
    CHECK(meshes.size() == 2u);
    CHECK(meshes[0].hasMesh());

    // L7: max bbox comes from the max vertex and inverse != inertia.
    auto meshMax = meshes[0].getMesh().getBoundaryMaxVertex();
    auto objMax = meshes[0].getMaxBoundaryVertex();
    for (int d = 0; d < 3; d++) CHECK(close(objMax[d], meshMax[d]));

    auto inertia = meshes[0].getInertia();
    auto inverse = meshes[0].getInverse();
    CHECK(std::isfinite(inertia[0]) && std::isfinite(inverse[0]));
    // The stored inverse must multiply the inertia back to the identity
    // (the bug stored the plain inertia, giving ~I^2 instead).
    for (int r = 0; r < 3; r++)
      for (int c = 0; c < 3; c++) {
        iREAL p = 0.0;
        for (int k = 0; k < 3; k++) p += inverse[3 * r + k] * inertia[3 * k + c];
        CHECK(close(p, r == c ? 1.0 : 0.0, 1e-6));
      }

    // L2: a mesh followed by a sphere must not zero the triangle total.
    std::vector<delta::world::structure::Object> mixed = meshes;
    mixed.push_back(sphere(99, {{0.5, 0.5, 0.5}}, true));
    delta::core::data::Structure structure(mixed);
    CHECK(structure.getNumberOfTriangles() > 0);
  }

  // ---- L4 + L3: collision counter assigned; bad model fails loudly --------
  {
    std::vector<delta::world::structure::Object> pair;
    pair.push_back(sphere(0, {{0.47, 0.5, 0.5}}, true));
    pair.push_back(sphere(1, {{0.50, 0.5, 0.5}}, true));

    delta::core::Engine engine(pair, kBoundary, baseMeta());
    engine.contactDetection();
    CHECK(engine.getContactPairs().size() > 0u);
    CHECK(engine.getState().getCollisions() > 0);

    auto bad = baseMeta();
    bad.modelScheme = delta::core::data::Meta::CollisionModel::GJK;
    bool threw = false;
    try {
      delta::core::Engine badEngine(pair, kBoundary, bad);
      badEngine.contactDetection();
    } catch (const std::logic_error&) {
      threw = true;
    }
    CHECK(threw);
  }

  // ---- L1: expmap produces a proper rotation (R R^T = I, det = 1) --------
  {
    iREAL angular[3] = {0.0, 0.0, 0.0};
    iREAL refAngular[3] = {1.0, 2.0, 3.0};  // non-axis-aligned exposes the bug
    iREAL R[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    delta::dynamics::updateRotationMatrix(angular, refAngular, R, 1.0);

    for (int r = 0; r < 3; r++)
      for (int c = 0; c < 3; c++) {
        iREAL p = 0.0;
        for (int k = 0; k < 3; k++) p += R[3 * r + k] * R[3 * c + k];
        CHECK(close(p, r == c ? 1.0 : 0.0, 1e-12));
      }
    iREAL det = R[0] * (R[4] * R[8] - R[5] * R[7]) -
                R[3] * (R[1] * R[8] - R[2] * R[7]) +
                R[6] * (R[1] * R[5] - R[2] * R[4]);
    CHECK(close(det, 1.0, 1e-12));
  }

  // ---- I1: a missing mesh file is a diagnosable error, not a segfault ----
  {
    bool threw = false;
    try {
      delta::core::io::readPartGeometry(
          delta::core::io::inputPath("no-such-mesh.stl"));
    } catch (const std::runtime_error&) {
      threw = true;
    }
    CHECK(threw);
  }

  // ---- I2: the body block carries initial state, not just geometry -------
  {
    const char* path = "regression_input.mbfcp";
    writeMbfcp(path, "SURFID:\t1", "VELOCITY:  1.5  -2  0.25  0  0.5  -1",
               "FORCES:\t0");

    std::vector<delta::world::structure::Object> objects;
    delta::core::io::Scenario scenario =
        delta::core::io::readmbfcp(path, objects, kEpsilon);

    CHECK(objects.size() == 1u);
    CHECK(scenario.bodyCount == 1);
    CHECK(close(scenario.gravity[2], -10.0));
    CHECK(close(scenario.surface.friction, 0.25));
    CHECK(close(scenario.surface.spring, 2.0e5, 1e-6));
    CHECK(close(scenario.bulk.density, 1000.0));
    CHECK(close(scenario.bulk.young, 1.0e9, 1e3));

    // VELOCITY is linear xyz then angular xyz; all six used to be dropped.
    CHECK(close(objects[0].getLinearVelocity()[0], 1.5));
    CHECK(close(objects[0].getLinearVelocity()[1], -2.0));
    CHECK(close(objects[0].getLinearVelocity()[2], 0.25));
    CHECK(close(objects[0].getAngularVelocity()[0], 0.0));
    CHECK(close(objects[0].getAngularVelocity()[1], 0.5));
    CHECK(close(objects[0].getAngularVelocity()[2], -1.0));

    // A SURFID outside the declared table, and any FORCES record, are refused
    // rather than parsed away.
    writeMbfcp(path, "SURFID:\t2", "VELOCITY:  0  0  0  0  0  0", "FORCES:\t0");
    bool badSurfid = false;
    try {
      std::vector<delta::world::structure::Object> o;
      delta::core::io::readmbfcp(path, o, kEpsilon);
    } catch (const std::logic_error&) {
      badSurfid = true;
    }
    CHECK(badSurfid);

    writeMbfcp(path, "SURFID:\t1", "VELOCITY:  0  0  0  0  0  0", "FORCES:\t1");
    bool badForces = false;
    try {
      std::vector<delta::world::structure::Object> o;
      delta::core::io::readmbfcp(path, o, kEpsilon);
    } catch (const std::logic_error&) {
      badForces = true;
    }
    CHECK(badForces);

    std::remove(path);
  }

  std::printf(
      "regression_test OK: L1/L2/L3/L4/L5/L6/L7/L8/I1/I2 fixes verified\n");
  return 0;
}
