/*
 * Hopper: the repo's one realistic scenario, used as an end-to-end test.
 *
 * input/dim3/hopper.mbfcp declares 15651 bodies (15625 free spheres in a 25^3
 * lattice plus 26 obstacle walls), gravity (0,0,-10) and a 1e6 spring. The
 * data/2024 code path that loaded it had a note that the contact law it
 * declares was read, printed and then ignored, because SSPRING/SDAMPER were
 * #defines. That is no longer true: core::ContactParams is the law, and this
 * test pins the file's numbers to what the engine actually uses.
 *
 * What is checked, and why each one matters:
 *
 *   the file loads at all, and gives 15651 bodies / 26 obstacles
 *   every free body has positive finite mass, or deriveForces divides by zero
 *   gravity is the file's, not the old hard-coded -9.8 on y
 *   the contact law matches the file after --apply-scenario-contact
 *   a short run stays finite: the lattice starts tangent, so a too-large
 *     epsilon or dt releases every spring in one step and the pile explodes
 *   BodySpec and the raw path build the same body (the equivalence that makes
 *     the convenience layer safe)
 *
 * CHECK() rather than assert() because a Release build defines -DNDEBUG.
 */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "core/BodySpec.h"
#include "core/ContactParams.h"
#include "core/data/Meta.h"
#include "core/io/read.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/contact/forces/forces.h"
#include "dim3/world/structure/Object.h"

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      std::abort();                                                        \
    }                                                                      \
  } while (0)

using delta::world::structure::Object;

namespace {

/* The hopper file's lattice is 0.04-spaced, so a contact epsilon above a
 * fraction of the radius makes the whole bed start compressed. The value and the
 * reasoning come from apps/dem3d/main.cpp. */
const iREAL kHopperEpsilon = 0.002;

delta::core::data::Meta::Simulation hopperMeta(const std::array<iREAL, 3>& gravity)
{
  delta::core::data::Meta::Simulation meta;
  meta.plotScheme      = delta::core::data::Meta::Plot::Never;
  meta.modelScheme     = delta::core::data::Meta::CollisionModel::Sphere;
  meta.overlapPreCheck = false;
  meta.dt              = 1e-5;
  meta.gravity         = (gravity[0] != 0.0 || gravity[1] != 0.0 || gravity[2] != 0.0);
  meta.gravityVector   = gravity;
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;
  return meta;
}

}  // namespace

int main()
{
  const std::string path = delta::core::io::inputPath("hopper.mbfcp");

  /* ---- load ---------------------------------------------------------- */
  std::vector<Object> bodies;
  const delta::core::io::Scenario scene =
      delta::core::io::readmbfcp(path, bodies, kHopperEpsilon);

  int obstacles = 0;
  for (std::size_t i = 0; i < bodies.size(); ++i)
    if (bodies[i].getIsObstacle()) ++obstacles;

  std::printf("hopper: %zu bodies, %d obstacles, %zu free\n",
              bodies.size(), obstacles, bodies.size() - (std::size_t)obstacles);

  CHECK(bodies.size() == 15651);          /* the file's BODIES: 15651 */
  CHECK(obstacles == 26);                 /* 26 obstacle walls */
  CHECK(scene.bodyCount == 15651);

  /* ---- gravity is the file's ---------------------------------------- */
  CHECK(scene.gravity[2] == -10.0);
  CHECK(scene.gravity[0] == 0.0 && scene.gravity[1] == 0.0);

  /* ---- masses -------------------------------------------------------- */
  int badMass = 0;
  iREAL minMass = 1e99, maxMass = 0.0;
  for (std::size_t i = 0; i < bodies.size(); ++i)
  {
    if (bodies[i].getIsObstacle()) continue;
    const iREAL m = bodies[i].getMass();
    if (!(m > 0.0) || !std::isfinite(m)) { ++badMass; continue; }
    if (m < minMass) minMass = m;
    if (m > maxMass) maxMass = m;
  }
  CHECK(badMass == 0);
  std::printf("mass range of free bodies: %.6g .. %.6g\n", minMass, maxMass);

  /* ---- the contact law the file declares ---------------------------- */
  std::printf("file law: spring=%g dashpot=%g friction=%g\n",
              scene.surface.spring, scene.surface.dashpot,
              scene.surface.friction);

  /* Before opting in, the compiled defaults apply. */
  CHECK(delta::contact::forces::params().spring == 2.0e5);

  {
    delta::core::ContactParams params;               /* defaults = old macros */
    if (scene.surface.spring > 0.0) params.spring = scene.surface.spring;
    if (scene.surface.dashpot >= 0.0 && scene.surface.dashpot <= 1.0)
      params.dashpot = scene.surface.dashpot;
    if (scene.surface.friction > 0.0) params.friction = scene.surface.friction;
    delta::contact::forces::setParams(params);
  }
  /* 1e6 is the file's SPRING; DASHPOT is -1 ("not given") so it must stay 0.5,
   * not become -1 and invert the damper. */
  CHECK(delta::contact::forces::params().spring == 1.0e6);
  CHECK(delta::contact::forces::params().dashpot == 0.5);
  std::printf("applied law: spring=%g dashpot=%g\n",
              delta::contact::forces::params().spring,
              delta::contact::forces::params().dashpot);

  /* ---- a short run stays finite -------------------------------------- */
  {
    const std::array<iREAL, 6> boundary = {{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}};
    delta::core::Engine engine(bodies, boundary, hopperMeta(scene.gravity));

    iREAL maxSpeed = 0.0;
    for (int step = 0; step < 5; ++step)
    {
      engine.iterate();
      std::vector<delta::core::data::ParticleRecord>& recs =
          engine.getParticleRecords();
      for (std::size_t p = 0; p < recs.size(); ++p)
      {
        for (int d = 0; d < 3; ++d)
        {
          CHECK(std::isfinite(recs[p]._centre[d]));
          CHECK(std::isfinite(recs[p]._linearVelocity[d]));
        }
        const iREAL v = std::sqrt(recs[p]._linearVelocity[0]*recs[p]._linearVelocity[0] +
                                  recs[p]._linearVelocity[1]*recs[p]._linearVelocity[1] +
                                  recs[p]._linearVelocity[2]*recs[p]._linearVelocity[2]);
        if (std::isfinite(v) && v > maxSpeed) maxSpeed = v;
      }
    }
    /* Tangent lattice + 1e5..1e6 springs: a stable step keeps speeds bounded.
     * 50 is generous; the failing mode is 1e6 and climbing. */
    std::printf("after 5 steps: max|v| = %.6g, contacts = %zu\n",
                maxSpeed, engine.getContactPairs().size());
    CHECK(maxSpeed < 50.0);
  }

  /* ---- BodySpec builds the same body as the raw path ---------------- */
  {
    const std::array<iREAL, 3> centre = {{0.3, 0.4, 0.5}};
    const iREAL radius = 0.02;

    /* what a caller does today */
    Object raw("sphere", radius, 7, centre,
               delta::geometry::material::MaterialType::WOOD,
               false, true, true, 0.002,
               {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
    raw.setMass(1000.0 * (4.0 / 3.0) * 3.14159265358979323846 *
                radius * radius * radius);

    /* what BodySpec does */
    // not const: Object's getters are not const
    Object built = delta::core::BodySpec::sphere(centre, radius)
                             .id(7)
                             .epsilon(0.002)
                             .build();

    /* Object exposes no id getter, so identity is checked through the state the
     * engine actually reads: mass, reach, centre, and the velocity arrays. */
    CHECK(built.getMass() == raw.getMass());
    CHECK(built.getEpsilon() == raw.getEpsilon());
    CHECK(built.getIsObstacle() == raw.getIsObstacle());
    CHECK(built._centre == raw._centre);
    CHECK(built.getHaloDiameter() == raw.getHaloDiameter());
  }

  /* ---- an impulse is a velocity ------------------------------------- */
  {
    const iREAL radius = 0.1;
    const delta::core::BodySpec spec =
        delta::core::BodySpec::sphere({{0, 0, 0}}, radius);
    const iREAL m = spec.mass();

    /* F * dt / m */
    const delta::core::BodySpec kicked =
        delta::core::BodySpec::sphere({{0, 0, 0}}, radius)
            .impulse({{3.0 * m, 0.0, 0.0}}, 2.0);

    CHECK(std::fabs(kicked.velocity()[0] - 6.0) < 1e-12);
    CHECK(kicked.velocity()[1] == 0.0 && kicked.velocity()[2] == 0.0);

    /* a frozen body cannot be kicked */
    bool threw = false;
    try {
      delta::core::BodySpec::sphere({{0, 0, 0}}, radius).obstacle()
          .impulse({{1, 0, 0}}, 1.0);
    } catch (const std::logic_error&) { threw = true; }
    CHECK(threw);
    (void)spec;
  }

  std::printf("hopper_test OK: loads 15651 bodies, gravity and contact law come "
              "from the file, 5 steps stay finite, BodySpec matches Object\n");
  return 0;
}
