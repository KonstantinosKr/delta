/* Dump the hopper_fall scenario for the report figures and the video.
 *
 * Same scene as tests/hopper_fall_test.cpp. Writes into <outdir>:
 *   video_walls.csv      the five walls as triangles, world space (once)
 *   video_particles.csv  frame,time,id,x,y,z,speed every kFrameDump steps
 *   hopper_after.csv     step,time,id,z,speed for body 1 only
 *
 * Usage: ./hf_video [outdir]        (default: output/report)
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "core/data/Meta.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/geometry/structure/Mesh.h"
#include "dim3/world/structure/Object.h"

using delta::world::structure::Object;
typedef delta::geometry::mesh::Mesh Mesh;

namespace {

const int kFrameDump = 200;   /* steps between frames */

std::string gDir = "output/report";

std::string path(const char* name)
{
  return gDir + "/" + name;
}

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

struct HopperGeometry
{
  iREAL topY, bottomY, halfAtTop, halfAtBottom, depth, cx, cy, cz;
};

std::vector<Object> makeHopperBodies(const HopperGeometry& g, iREAL epsilon)
{
  std::vector<iREAL> x, y, z;

  const iREAL tl = g.cx - g.halfAtTop;
  const iREAL tr = g.cx + g.halfAtTop;
  const iREAL bl = g.cx - g.halfAtBottom;
  const iREAL br = g.cx + g.halfAtBottom;
  const iREAL z0 = g.cz - g.depth;
  const iREAL z1 = g.cz + g.depth;

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

  /* World-space walls, written before Mesh recentres them into its local frame. */
  std::FILE* f = std::fopen(path("video_walls.csv").c_str(), "w");
  std::fprintf(f, "x,y,z\n");
  for (std::size_t i = 0; i < x.size(); ++i)
    std::fprintf(f, "%.6g,%.6g,%.6g\n", x[i], y[i], z[i]);
  std::fclose(f);

  Mesh* walls = new Mesh(x, y, z);
  std::vector<Object> bodies;
  Object wall("hopper", 0, walls,
              {{g.cx, 0.5 * (g.bottomY + g.topY), g.cz}},
              delta::geometry::material::MaterialType::WOOD,
              true, true, true, epsilon, {{0,0,0}}, {{0,0,0}});
  bodies.push_back(wall);
  return bodies;
}

}  // namespace

int main(int argc, char** argv)
{
  if (argc > 1) gDir = argv[1];

  const iREAL radius  = 0.04;
  const iREAL epsilon = 0.002;
  const int   nSpheres = 10;

  const HopperGeometry g = {0.60, 0.10, 0.30, 0.10, 0.10, 0.0, 0.0, 0.0};

  std::vector<Object> bodies = makeHopperBodies(g, epsilon);

  const iREAL density = 1000.0;
  const iREAL mass = density * (4.0/3.0) * 3.14159265358979323846 *
                     radius*radius*radius;

  int id = 1;
  for (int row = 0; row < 2; ++row)
    for (int col = 0; col < 5; ++col)
    {
      const iREAL xx = -0.24 + col * 0.12;
      const iREAL yy = g.topY + 0.12 + row * 0.15;
      Object s("sphere", radius, id++, {{xx, yy, g.cz}},
               delta::geometry::material::MaterialType::WOOD,
               false, true, true, epsilon,
               {{0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0}});
      s.setMass(mass);
      bodies.push_back(s);
    }

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

  std::FILE* out = std::fopen(path("video_particles.csv").c_str(), "w");
  std::FILE* body = std::fopen(path("hopper_after.csv").c_str(), "w");
  std::fprintf(out,  "frame,time,id,x,y,z,speed\n");
  std::fprintf(body, "step,time,id,z,speed\n");

  const int steps = 120000;
  for (int step = 0; step < steps; ++step)
  {
    engine.iterate();
    if (step % kFrameDump != 0) continue;

    const int frame = step / kFrameDump;
    std::vector<delta::core::data::ParticleRecord>& recs = engine.getParticleRecords();
    for (std::size_t p = 0; p < recs.size(); ++p)
    {
      if (recs[p].getIsObstacle()) continue;
      const iREAL* c = recs[p]._centre.data();
      const iREAL* v = recs[p]._linearVelocity.data();
      const iREAL speed = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
      const int pid = recs[p].getGlobalParticleID();

      std::fprintf(out, "%d,%.6g,%d,%.6g,%.6g,%.6g,%.6g\n",
                   frame, step * 1e-5, pid, c[0], c[1], c[2], speed);
      if (pid == 1)
        std::fprintf(body, "%d,%.6g,%d,%.6g,%.6g\n",
                     step, step * 1e-5, pid, c[2], speed);
    }
  }
  std::fclose(out);
  std::fclose(body);
  std::printf("wrote %d frames and the body-1 trajectory into %s\n",
              steps / kFrameDump, gDir.c_str());
  return 0;
}
