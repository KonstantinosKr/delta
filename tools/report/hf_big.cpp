/* Drives the real hopper scenario (input/dim3/hopper.mbfcp, 15 651 bodies) and
 * records it for the report video, printing the timings along the way.
 *
 * Usage: ./hf_big [outdir] [steps] [frames] [dt]
 *   outdir  default output/report
 *   steps   default 6000      (simulated steps to run)
 *   frames  default 300       (frames to write, evenly spaced over the run)
 *   dt      default 1e-5      (explicit step)
 *
 * Writes <outdir>/big_particles.csv (positions, one block per frame) and
 * <outdir>/big_walls.csv (the obstacle triangles, once). Timing goes to stdout.
 */
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "core/ContactParams.h"
#include "core/data/Meta.h"
#include "core/io/read.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/contact/forces/forces.h"
#include "dim3/world/structure/Object.h"

namespace {

std::string gDir = "output/report";

std::string path(const char* name) { return gDir + "/" + name; }

double seconds(std::chrono::steady_clock::time_point a,
               std::chrono::steady_clock::time_point b)
{
  return std::chrono::duration<double>(b - a).count();
}

}  // namespace

int main(int argc, char** argv)
{
  if (argc > 1) gDir = argv[1];
  const int    steps  = (argc > 2) ? std::atoi(argv[2]) : 6000;
  const int    frames = (argc > 3) ? std::atoi(argv[3]) : 300;
  const double dt     = (argc > 4) ? std::atof(argv[4]) : 1e-5;

  /* Same loading path as apps/dem3d/main.cpp: epsilon 0.002 keeps the reach
   * above the 0.04 lattice spacing without starting the bed 2*epsilon
   * compressed. */
  const iREAL epsilon = 0.002;
  std::vector<delta::world::structure::Object> bodies;
  const std::string file = delta::core::io::inputPath("hopper.mbfcp");

  auto t0 = std::chrono::steady_clock::now();
  const delta::core::io::Scenario scene =
      delta::core::io::readmbfcp(file, bodies, epsilon);
  auto t1 = std::chrono::steady_clock::now();

  /* The file's contact law (SPRING = 1e6). */
  delta::core::ContactParams params;
  if (scene.surface.spring > 0.0) params.spring = scene.surface.spring;
  if (scene.surface.dashpot >= 0.0 && scene.surface.dashpot <= 1.0)
    params.dashpot = scene.surface.dashpot;
  if (scene.surface.friction > 0.0) params.friction = scene.surface.friction;
  delta::contact::forces::setParams(params);

  int obstacles = 0;
  for (std::size_t i = 0; i < bodies.size(); ++i)
    if (bodies[i].getIsObstacle()) ++obstacles;

  std::printf("loaded %zu bodies (%d obstacles, %zu free) in %.2f s\n",
              bodies.size(), obstacles, bodies.size() - obstacles, seconds(t0, t1));
  std::printf("contact law: spring=%.3g dashpot=%.3g friction=%.3g  dt=%.3g\n",
              params.spring, params.dashpot, params.friction, dt);

  /* Wall triangles in world space, before the mesh recentres them. */
  {
    std::FILE* f = std::fopen(path("big_walls.csv").c_str(), "w");
    for (std::size_t i = 0; i < bodies.size(); ++i)
    {
      if (!bodies[i].getIsObstacle()) continue;
      const int n = bodies[i].getNumberOfTriangles();
      const std::vector<iREAL> xs = bodies[i].getMesh().getXCoordinatesAsVector();
      const std::vector<iREAL> ys = bodies[i].getMesh().getYCoordinatesAsVector();
      const std::vector<iREAL> zs = bodies[i].getMesh().getZCoordinatesAsVector();
      for (int t = 0; t < n * 3; ++t)
        std::fprintf(f, "%.6g,%.6g,%.6g\n", xs[t], ys[t], zs[t]);
    }
    std::fclose(f);
  }

  delta::core::data::Meta::Simulation meta;
  meta.plotScheme      = delta::core::data::Meta::Plot::Never;
  meta.modelScheme     = delta::core::data::Meta::CollisionModel::Sphere;
  meta.overlapPreCheck = false;
  meta.dt              = dt;
  meta.gravity         = (scene.gravity[0] != 0.0 || scene.gravity[1] != 0.0 ||
                          scene.gravity[2] != 0.0);
  meta.gravityVector   = scene.gravity;
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;

  const std::array<iREAL, 6> boundary = {{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}};

  auto t2 = std::chrono::steady_clock::now();
  delta::core::Engine engine(bodies, boundary, meta);
  auto t3 = std::chrono::steady_clock::now();
  std::printf("engine built in %.2f s\n", seconds(t2, t3));

  std::FILE* out = std::fopen(path("big_particles.csv").c_str(), "w");
  std::fprintf(out, "# %d %d\n", frames, (int)(bodies.size() - obstacles));

  const int every = (steps > frames) ? (steps / frames) : 1;
  int written = 0;
  double maxSpeedEver = 0.0;

  auto tRun0 = std::chrono::steady_clock::now();
  for (int step = 0; step < steps; ++step)
  {
    engine.iterate();

    if (step % every != 0 || written >= frames) continue;

    std::vector<delta::core::data::ParticleRecord>& recs = engine.getParticleRecords();
    std::fprintf(out, "# %d %.9g %zu\n", written, step * dt,
                 engine.getContactPairs().size());
    for (std::size_t p = 0; p < recs.size(); ++p)
    {
      if (recs[p].getIsObstacle()) continue;
      const iREAL* c = recs[p]._centre.data();
      const iREAL* v = recs[p]._linearVelocity.data();
      const iREAL speed = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
      if (speed > maxSpeedEver) maxSpeedEver = speed;
      std::fprintf(out, "%.5f,%.5f,%.5f\n", c[0], c[1], c[2]);
    }
    ++written;

    if (written % 25 == 0)
    {
      auto now = std::chrono::steady_clock::now();
      const double el = seconds(tRun0, now);
      std::printf("  frame %3d/%d  step %6d  t=%.4f s  contacts=%zu  "
                  "max|v|=%.3g  %.0f ms/step\n",
                  written, frames, step, step * dt,
                  engine.getContactPairs().size(), maxSpeedEver,
                  1000.0 * el / (step + 1));
      std::fflush(stdout);
    }
  }
  auto tRun1 = std::chrono::steady_clock::now();
  std::fclose(out);

  const double run = seconds(tRun0, tRun1);
  std::printf("\nran %d steps in %.1f s  (%.1f ms/step, %.1f steps/s)\n",
              steps, run, 1000.0 * run / steps, steps / run);
  std::printf("simulated %.4g s of motion; max|v| = %.3g m/s\n", steps * dt,
              maxSpeedEver);
  std::printf("frames written: %d (every %d steps)\n", written, every);
  return 0;
}
