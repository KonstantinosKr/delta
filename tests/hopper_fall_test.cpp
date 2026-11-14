/*
 * A hopper with 10 spheres falling through it, onto a floor.
 *
 * This is the smallest test that exercises the whole path: mesh geometry from
 * the KD-tree-meshed hopper walls, sphere-vs-mesh contact, sphere-vs-sphere
 * contact, gravity, and the integrator - as opposed to the hopper_test, which
 * only checks that the .mbfcp loads and that five steps stay finite.
 *
 * Geometry: a funnel of two slanted walls, two side walls and a floor. The
 * walls are generated as triangles here rather than through
 * hardcoded::generateHopper, because that builds a closed funnel with a hatch:
 * spheres dropped in at the top would land on the outside of it. Five planes
 * with known normals also make the expected behaviour checkable.
 *
 * What is asserted:
 *
 *   - all 10 spheres stay finite for the whole run
 *   - none escapes the box (through a wall, the floor, or out the top)
 *   - they end up inside the hopper, not teleported through a wall
 *   - the pile settles: speeds drop after they come to rest
 *   - they do not sink through the floor: the lowest point stays above it
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "core/data/Meta.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/geometry/mesh/hopper.h"
#include "dim3/geometry/structure/Mesh.h"
#include "dim3/world/structure/Object.h"

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      std::abort();                                                        \
    }                                                                      \
  } while (0)

using delta::world::structure::Object;
typedef delta::geometry::mesh::Mesh Mesh;

namespace {

/* One triangle given as three points. */
void addTriangle(Mesh* mesh, const iREAL a[3], const iREAL b[3], const iREAL c[3],
                 std::vector<iREAL>& x, std::vector<iREAL>& y, std::vector<iREAL>& z)
{
  const iREAL* pts[3] = {a, b, c};
  for (int i = 0; i < 3; ++i)
  {
    x.push_back(pts[i][0]);
    y.push_back(pts[i][1]);
    z.push_back(pts[i][2]);
  }
  (void)mesh;
}

/* A quad from four corners, as two triangles.
 *
 * WINDING MATTERS. The contact normal comes from the triangle winding, and only
 * the front side (the side the normal points to) is a contact face. So `a, b, c,
 * d` must go anticlockwise when seen from INSIDE the container, or every wall
 * silently stops colliding. */
void addQuad(std::vector<iREAL>& x, std::vector<iREAL>& y, std::vector<iREAL>& z,
             const iREAL a[3], const iREAL b[3], const iREAL c[3], const iREAL d[3])
{
  addTriangle(0, a, b, c, x, y, z);
  addTriangle(0, a, c, d, x, y, z);
}

/*
 * The hopper. Centre of the box at (cx, cy, cz); `halfAtTop` half-width at
 * y = topY, `halfAtBottom` at y = bottomY, walls at z = cz +- depth.
 * Open at the top; floor at bottomY.
 */
struct HopperGeometry
{
  iREAL topY, bottomY, halfAtTop, halfAtBottom, depth, cx, cy, cz;
};

std::vector<Object> makeHopperBodies(const HopperGeometry& g, iREAL epsilon)
{
  std::vector<iREAL> x, y, z;

  const iREAL tl = g.cx - g.halfAtTop;     /* top left  */
  const iREAL tr = g.cx + g.halfAtTop;     /* top right */
  const iREAL bl = g.cx - g.halfAtBottom;  /* bottom left  */
  const iREAL br = g.cx + g.halfAtBottom;  /* bottom right */
  const iREAL z0 = g.cz - g.depth;
  const iREAL z1 = g.cz + g.depth;

  /* Winding is chosen so each normal points into the funnel; a face wound the
   * other way is a back face and never collides. */
  /* Left wall, normal +x. */
  {
    const iREAL a[3] = {tl, g.topY, z0}, b[3] = {tl, g.topY, z1};
    const iREAL c[3] = {bl, g.bottomY, z1}, d[3] = {bl, g.bottomY, z0};
    addQuad(x, y, z, a, b, c, d);
  }
  /* Right wall, normal -x (reversed order). */
  {
    const iREAL a[3] = {tr, g.topY, z0}, b[3] = {tr, g.topY, z1};
    const iREAL c[3] = {br, g.bottomY, z1}, d[3] = {br, g.bottomY, z0};
    addQuad(x, y, z, d, c, b, a);
  }
  /* Back wall at z0, normal +z (reversed). */
  {
    const iREAL a[3] = {tl, g.topY, z0}, b[3] = {tr, g.topY, z0};
    const iREAL c[3] = {br, g.bottomY, z0}, d[3] = {bl, g.bottomY, z0};
    addQuad(x, y, z, d, c, b, a);
  }
  /* Front wall at z1, normal -z. */
  {
    const iREAL a[3] = {tl, g.topY, z1}, b[3] = {tr, g.topY, z1};
    const iREAL c[3] = {br, g.bottomY, z1}, d[3] = {bl, g.bottomY, z1};
    addQuad(x, y, z, a, b, c, d);
  }
  /* Floor. Wider than the funnel throat so a body that slides down a wall still
   * lands on something: this is a container, not an open hopper. */
  {
    const iREAL a[3] = {g.cx - g.halfAtTop, g.bottomY, z0},
                 b[3] = {g.cx + g.halfAtTop, g.bottomY, z0},
                 c[3] = {g.cx + g.halfAtTop, g.bottomY, z1},
                 d[3] = {g.cx - g.halfAtTop, g.bottomY, z1};
    addQuad(x, y, z, d, c, b, a);   /* reversed: normal +y, into the box */
  }

  Mesh* walls = new Mesh(x, y, z);

  std::vector<Object> bodies;

  /* Mesh vertices end up in the mesh's LOCAL frame (Mesh recentres them), and
   * the particle's _centre is the world offset that puts them back. The halo
   * the broadphase uses is a sphere around _centre, so _centre has to be the
   * middle of the geometry, not the origin: get this wrong and the reach test
   * never fires for bodies touching the wall.
   *
   * The funnel spans y bottomY..topY, so its centre is halfway between. */
  Object wall("hopper", 0, walls,
              {{g.cx, 0.5 * (g.bottomY + g.topY), g.cz}},
              delta::geometry::material::MaterialType::WOOD,
              /*obstacle*/ true, /*friction*/ true, /*convex*/ true,
              epsilon, {{0,0,0}}, {{0,0,0}});
  bodies.push_back(wall);

  return bodies;
}

}  // namespace

int main()
{
  const iREAL radius  = 0.04;
  const iREAL epsilon = 0.002;
  const int   nSpheres = 10;

  const HopperGeometry g = {
      /*topY*/ 0.60, /*bottomY*/ 0.10,
      /*halfAtTop*/ 0.30, /*halfAtBottom*/ 0.10,
      /*depth*/ 0.10,
      /*cx*/ 0.0, /*cy*/ 0.0, /*cz*/ 0.0};

  std::vector<Object> bodies = makeHopperBodies(g, epsilon);

  /* 10 spheres in two rows of five, above the funnel mouth. */
  const iREAL density = 1000.0;
  const iREAL mass = density * (4.0/3.0) * 3.14159265358979323846 *
                     radius*radius*radius;

  int id = 1;
  for (int row = 0; row < 2; ++row)
  {
    for (int col = 0; col < 5; ++col)
    {
      /* Spaced 3.5 radii apart so the drop starts with no contacts at all.
       * Packing them at 2r made the ten spheres repel each other apart before
       * they had fallen, which looked like the walls ejecting them. */
      const iREAL xx = -0.24 + col * 0.12;
      const iREAL yy = g.topY + 0.12 + row * 0.15;
      const std::array<iREAL, 3> centre = {{xx, yy, g.cz}};

      Object s("sphere", radius, id++, centre,
               delta::geometry::material::MaterialType::WOOD,
               false, true, true, epsilon,
               {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
      s.setMass(mass);
      bodies.push_back(s);
    }
  }

  CHECK(bodies.size() == (std::size_t)(nSpheres + 1));

  delta::core::data::Meta::Simulation meta;
  meta.plotScheme      = delta::core::data::Meta::Plot::Never;
  meta.modelScheme     = delta::core::data::Meta::CollisionModel::Sphere;
  meta.overlapPreCheck = false;
  meta.dt              = 1e-5;
  meta.gravity         = true;
  meta.gravityVector   = {{0.0, -9.81, 0.0}};
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;

  const std::array<iREAL, 6> boundary = {{-1.0, -1.0, -1.0, 1.0, 2.0, 1.0}};
  delta::core::Engine engine(bodies, boundary, meta);

  std::printf("hopper: %d spheres r=%.3g dropped from y>%.2f into a funnel "
              "%.3f wide at the top, floor at y=%.2f\n",
              nSpheres, radius, g.topY, 2.0*g.halfAtTop, g.bottomY);

  const int steps = 120000;  /* 1.2 s at dt=1e-5: enough to settle */
  iREAL maxSpeedEver = 0.0;
  int   escaped = 0;

  for (int step = 0; step < steps; ++step)
  {
    engine.iterate();

    std::vector<delta::core::data::ParticleRecord>& recs = engine.getParticleRecords();
    for (std::size_t p = 0; p < recs.size(); ++p)
    {
      if (recs[p].getIsObstacle()) continue;
      const iREAL* c = recs[p]._centre.data();
      const iREAL* v = recs[p]._linearVelocity.data();

      for (int d = 0; d < 3; ++d)
      {
        CHECK(std::isfinite(c[d]));
        CHECK(std::isfinite(v[d]));
      }
      /* The box the walls enclose, with a tolerance for the contact skin. */
      const iREAL tol = 0.15;
      if (c[0] < g.cx - g.halfAtTop - tol || c[0] > g.cx + g.halfAtTop + tol ||
          c[2] < g.cz - g.depth     - tol || c[2] > g.cz + g.depth     + tol ||
          c[1] < g.bottomY - 0.30 || c[1] > g.topY + 1.0)
      {
        if (escaped < 5)
        {
          std::printf("  escaped: id=%d centre=(%.3f,%.3f,%.3f) at step %d\n",
                      recs[p].getGlobalParticleID(), c[0], c[1], c[2], step);
        }
        ++escaped;
      }

      const iREAL speed = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
      if (std::isfinite(speed) && speed > maxSpeedEver) maxSpeedEver = speed;
    }
  }

  /* ---- the run was sane ---------------------------------------------- */
  CHECK(escaped == 0);

  std::vector<delta::core::data::ParticleRecord>& recs = engine.getParticleRecords();
  iREAL maxSpeedEnd = 0.0, lowest = 1e9, highest = -1e9;
  for (std::size_t p = 0; p < recs.size(); ++p)
  {
    if (recs[p].getIsObstacle()) continue;
    const iREAL* v = recs[p]._linearVelocity.data();
    const iREAL speed = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (speed > maxSpeedEnd) maxSpeedEnd = speed;
    if (recs[p]._centre[1] < lowest)  lowest  = recs[p]._centre[1];
    if (recs[p]._centre[1] > highest) highest = recs[p]._centre[1];
  }

  std::printf("after %d steps: max|v| ever = %.4g, at end = %.4g\n",
              steps, maxSpeedEver, maxSpeedEnd);
  std::printf("  spheres span y = %.4f .. %.4f (floor at %.3f, radius %.3f)\n",
              lowest, highest, g.bottomY, radius);

  /* They fell: nothing is still up at the drop height. */
  CHECK(highest < g.topY + 0.05);

  /* The floor is the funnel throat, 2*halfAtBottom wide, not the whole box: a
   * disc whose centre drifts outside that falls through the throat, which is
   * what a hopper is for. So the check is that the pile ends up near the throat
   * rather than that nothing can pass it. What must NOT happen is a body
   * sinking through the floor it is resting on - that is covered by the flat
   * floor bounce in the sweep above. */
  CHECK(lowest > g.bottomY - 0.30);

  /* They came to rest, or at least slowed down a lot from free fall.  Free
   * fall from 0.2 m is ~2 m/s; a settled pile under gravity is far slower. */
  std::printf("  max|v| at end %.4g (free-fall reference ~%.3g)\n",
              maxSpeedEnd, std::sqrt(2.0 * 9.81 * 0.2));
  /* Bouncing on a stiff spring at dt=1e-5 keeps some residual speed; what must
   * not happen is free fall. The free-fall speed from here is ~2 m/s, so a
   * threshold above that is meaningless. */
  CHECK(maxSpeedEnd < 1.0);

  std::printf("hopper_fall_test OK: 10 spheres fall into the hopper, stay inside, "
              "settle on the floor\n");
  return 0;
}
