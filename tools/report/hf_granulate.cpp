/* Ten granulates dropped into the funnel hopper.
 *
 * The bodies are convex hulls of random point clouds (geometry/hull/granulate),
 * so the contact model has to be a triangle one - CollisionModel::BruteForce by
 * default - not Sphere, which would see only their bounding spheres.
 *
 * Writes, into <outdir>:
 *   gran_walls.csv       the five hopper walls, world space (once)
 *   gran_frames.csv      per frame: time, contacts, then per body its id and
 *                        its triangles in world space, so the renderer does not
 *                        have to re-derive the rotation
 *
 * Usage: ./hf_granulate [outdir] [steps] [frames] [dt] [pointsPerBody]
 */
#include <cmath>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "core/ContactParams.h"
#include "core/Parallel.h"
#include "core/data/Meta.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/contact/forces/forces.h"
#include "dim3/geometry/hull/granulate.h"
#include "dim3/geometry/structure/Mesh.h"
#include "dim3/world/structure/Object.h"

using delta::world::structure::Object;
typedef delta::geometry::mesh::Mesh Mesh;

namespace {

std::string gDir = "output/granulate";

void addTri(std::vector<iREAL>& x, std::vector<iREAL>& y, std::vector<iREAL>& z,
            const iREAL a[3], const iREAL b[3], const iREAL c[3])
{
  x.push_back(a[0]); y.push_back(a[1]); z.push_back(a[2]);
  x.push_back(b[0]); y.push_back(b[1]); z.push_back(b[2]);
  x.push_back(c[0]); y.push_back(c[1]); z.push_back(c[2]);
}

void addQuad(std::vector<iREAL>& x, std::vector<iREAL>& y, std::vector<iREAL>& z,
             const iREAL a[3], const iREAL b[3], const iREAL c[3], const iREAL d[3])
{
  addTri(x, y, z, a, b, c);
  addTri(x, y, z, a, c, d);
}

struct Hopper
{
  iREAL topY, bottomY, halfAtTop, halfAtBottom, depth, cx, cy, cz;
};

/* Same funnel as tests/hopper_fall_test.cpp: two slanted walls, two side walls,
 * a floor. Wound so every normal points into the funnel. */
Mesh* makeHopper(const Hopper& g, iREAL eps)
{
  (void)eps;
  std::vector<iREAL> x, y, z;
  const iREAL tl = g.cx - g.halfAtTop, tr = g.cx + g.halfAtTop;
  const iREAL bl = g.cx - g.halfAtBottom, br = g.cx + g.halfAtBottom;
  const iREAL z0 = g.cz - g.depth, z1 = g.cz + g.depth;

  { const iREAL a[3] = {tl, g.topY, z0}, b[3] = {tl, g.topY, z1};
    const iREAL c[3] = {bl, g.bottomY, z1}, d[3] = {bl, g.bottomY, z0};
    addQuad(x, y, z, a, b, c, d); }
  { const iREAL a[3] = {tr, g.topY, z0}, b[3] = {tr, g.topY, z1};
    const iREAL c[3] = {br, g.bottomY, z1}, d[3] = {br, g.bottomY, z0};
    addQuad(x, y, z, d, c, b, a); }
  { const iREAL a[3] = {tl, g.topY, z0}, b[3] = {tr, g.topY, z0};
    const iREAL c[3] = {br, g.bottomY, z0}, d[3] = {bl, g.bottomY, z0};
    addQuad(x, y, z, d, c, b, a); }
  { const iREAL a[3] = {tl, g.topY, z1}, b[3] = {tr, g.topY, z1};
    const iREAL c[3] = {br, g.bottomY, z1}, d[3] = {bl, g.bottomY, z1};
    addQuad(x, y, z, a, b, c, d); }
  { const iREAL a[3] = {g.cx - g.halfAtTop, g.bottomY, z0},
                 b[3] = {g.cx + g.halfAtTop, g.bottomY, z0},
                 c[3] = {g.cx + g.halfAtTop, g.bottomY, z1},
                 d[3] = {g.cx - g.halfAtTop, g.bottomY, z1};
    addQuad(x, y, z, d, c, b, a); }

  std::FILE* f = std::fopen((gDir + "/gran_walls.csv").c_str(), "w");
  std::fprintf(f, "x,y,z\n");
  for (std::size_t i = 0; i < x.size(); ++i)
    std::fprintf(f, "%.5f,%.5f,%.5f\n", x[i], y[i], z[i]);
  std::fclose(f);

  return new Mesh(x, y, z);
}

}  // namespace

int main(int argc, char** argv)
{
  if (argc > 1) gDir = argv[1];
  const int    steps  = (argc > 2) ? std::atoi(argv[2]) : 20000;
  const int    frames = (argc > 3) ? std::atoi(argv[3]) : 300;
  const double dt     = (argc > 4) ? std::atof(argv[4]) : 1e-5;
  const int    points = (argc > 5) ? std::atoi(argv[5]) : 24;

  const int    nBodies = 10;                 /* "use 10" */
  const iREAL  radius  = 0.035;
  const iREAL  epsilon = 0.002;

  const Hopper g = {0.60, 0.10, 0.30, 0.10, 0.10, 0.0, 0.0, 0.0};

  std::vector<Object> bodies;
  bodies.push_back(Object("hopper", 0, makeHopper(g, epsilon),
                          {{g.cx, 0.5 * (g.bottomY + g.topY), g.cz}},
                          delta::geometry::material::MaterialType::WOOD,
                          /*obstacle*/ true, true, true, epsilon,
                          {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}}));

  /* Seeded generator: the same 10 shapes on every run and every machine. */
  int id = 1;
  for (int i = 0; i < nBodies; ++i)
  {
    const int row = i / 5, col = i % 5;
    iREAL centre[3] = {-0.24 + 0.12 * col,
                       g.topY + 0.12 + 0.15 * row,
                       g.cz};
    Mesh* hull = delta::geometry::primitive::granulate::generateParticle(
        centre, radius, points, 1234u + 7u * (unsigned)i);

    Object o("granulate", id++, hull, {{centre[0], centre[1], centre[2]}},
             delta::geometry::material::MaterialType::WOOD,
             false, true, true, epsilon, {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
    o.setMass(1000.0 * (4.0 / 3.0) * 3.14159265358979323846 * radius * radius * radius);
    bodies.push_back(o);
  }

  delta::core::ContactParams params;
  params.spring  = 2.0e5;      /* stable at dt = 1e-5 for ~1e-4 overlaps */
  params.dashpot = 0.8;        /* 0.5 leaves the hulls bouncing off the walls */
  delta::contact::forces::setParams(params);

  delta::core::data::Meta::Simulation meta;
  meta.plotScheme      = delta::core::data::Meta::Plot::Never;
  /* Triangle-vs-triangle: a granulate is a mesh, not a sphere. */
  meta.modelScheme     = delta::core::data::Meta::CollisionModel::BruteForce;
  meta.overlapPreCheck = false;
  meta.dt              = dt;
  meta.gravity         = true;
  meta.gravityVector   = {{0.0, -9.81, 0.0}};
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;

  const std::array<iREAL, 6> boundary = {{-1.0, -1.0, -1.0, 1.0, 2.0, 1.0}};
  auto t0 = std::chrono::steady_clock::now();
  delta::core::Engine engine(bodies, boundary, meta);
  auto t1 = std::chrono::steady_clock::now();

  std::printf("hopper_granulate: %d granulates (%d hull points each), "
              "%d bodies total, %d threads\n",
              nBodies, points, (int)bodies.size(), delta::core::parallel::threads());
  std::printf("engine built in %.2f s\n",
              std::chrono::duration<double>(t1 - t0).count());

  std::FILE* out = std::fopen((gDir + "/gran_frames.csv").c_str(), "w");
  std::fprintf(out, "# frames %d bodies %d\n", frames, nBodies);

  const int every = (steps > frames) ? (steps / frames) : 1;
  int written = 0;
  double maxSpeedEver = 0.0;

  auto tRun0 = std::chrono::steady_clock::now();
  for (int step = 0; step < steps; ++step)
  {
    engine.iterate();

    std::vector<delta::core::data::ParticleRecord>& recs = engine.getParticleRecords();
    for (std::size_t p = 0; p < recs.size(); ++p)
    {
      if (recs[p].getIsObstacle()) continue;
      const iREAL* v = recs[p]._linearVelocity.data();
      const iREAL speed = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
      if (speed > maxSpeedEver) maxSpeedEver = speed;
    }

    if (step % every != 0 || written >= frames) continue;

    std::fprintf(out, "# frame %d %.9g %zu\n", written, step * dt,
                 engine.getContactPairs().size());
    for (std::size_t p = 0; p < recs.size(); ++p)
    {
      if (recs[p].getIsObstacle()) continue;
      const int ntri = recs[p].getNumberOfTriangles();
      std::fprintf(out, "body %d %d\n", recs[p].getGlobalParticleID(), ntri);
      for (int t = 0; t < ntri * 3; ++t)
        std::fprintf(out, "%.5f,%.5f,%.5f\n",
                     recs[p]._xCoordinates[t], recs[p]._yCoordinates[t],
                     recs[p]._zCoordinates[t]);
    }
    ++written;

    if (written % 25 == 0)
    {
      auto now = std::chrono::steady_clock::now();
      const double el = std::chrono::duration<double>(now - tRun0).count();
      std::printf("  frame %3d/%d  step %6d  t=%.4f s  contacts=%zu  max|v|=%.3g"
                  "  %.1f ms/step\n",
                  written, frames, step, step * dt,
                  engine.getContactPairs().size(), maxSpeedEver,
                  1000.0 * el / (step + 1));
      std::fflush(stdout);
    }
  }
  auto tRun1 = std::chrono::steady_clock::now();
  std::fclose(out);

  const double run = std::chrono::duration<double>(tRun1 - tRun0).count();
  std::printf("\nran %d steps in %.1f s  (%.2f ms/step)\n", steps, run, 1000.0 * run / steps);
  std::printf("simulated %.4g s; max|v| = %.3g m/s; frames %d (every %d steps)\n",
              steps * dt, maxSpeedEver, written, every);
  return 0;
}
