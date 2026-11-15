/*
 * Speed of the contact path at 1..N threads, for every contact model.
 *
 * Three sections, because the two parallelism layers show up in different
 * places:
 *
 *   1. the real hopper (15 651 bodies, CollisionModel::Sphere) — the driver's
 *      pair loop and force pass dominate;
 *   2. a box pile with each triangle-vs-triangle model — again driver-bound,
 *      the individual meshes are smaller than the kernels' fork/join grain;
 *   3. a direct kernel call on two 4096-triangle walls — big enough to clear
 *      the grain, so this is where the kernels' own parallel region and their
 *      `omp simd` inner loop earn their keep.
 *
 * Not a test: run it by hand.  ./build/parallel_bench
 */
#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

#include "core/ContactParams.h"
#include "core/Parallel.h"
#include "core/data/Meta.h"
#include "core/io/read.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/contact/detection/bf.h"
#include "dim3/contact/detection/hybrid.h"
#include "dim3/contact/detection/penalty.h"
#include "dim3/contact/detection/sphere.h"
#include "dim3/geometry/structure/Mesh.h"
#include "dim3/world/structure/Object.h"

using delta::world::structure::Object;
typedef delta::geometry::mesh::Mesh Mesh;

namespace {

double now()
{
  return std::chrono::duration<double>(
             std::chrono::steady_clock::now().time_since_epoch()).count();
}

int setThreads(int n)
{
#if DELTA_OMP
  omp_set_num_threads(n);
  return omp_get_max_threads();
#else
  (void)n;
  return 1;
#endif
}

Mesh* makeBox(const std::array<iREAL, 3>& c, iREAL h)
{
  const iREAL x0 = c[0] - h, x1 = c[0] + h;
  const iREAL y0 = c[1] - h, y1 = c[1] + h;
  const iREAL z0 = c[2] - h, z1 = c[2] + h;
  const iREAL v[8][3] = {{x0, y0, z0}, {x1, y0, z0}, {x1, y1, z0}, {x0, y1, z0},
                         {x0, y0, z1}, {x1, y0, z1}, {x1, y1, z1}, {x0, y1, z1}};
  const int quads[6][4] = {{0, 3, 2, 1}, {4, 5, 6, 7}, {0, 1, 5, 4},
                           {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}};
  std::vector<iREAL> xs, ys, zs;
  for (int q = 0; q < 6; ++q)
  {
    const int* f = quads[q];
    const int tri[2][3] = {{f[0], f[1], f[2]}, {f[0], f[2], f[3]}};
    for (int t = 0; t < 2; ++t)
      for (int k = 0; k < 3; ++k)
      {
        xs.push_back(v[tri[t][k]][0]);
        ys.push_back(v[tri[t][k]][1]);
        zs.push_back(v[tri[t][k]][2]);
      }
  }
  return new Mesh(xs, ys, zs);
}

delta::core::data::Meta::Simulation makeMeta(delta::core::data::Meta::CollisionModel m)
{
  delta::core::data::Meta::Simulation meta;
  meta.plotScheme      = delta::core::data::Meta::Plot::Never;
  meta.modelScheme     = m;
  meta.overlapPreCheck = false;
  meta.dt              = 1e-5;
  meta.gravity         = true;
  meta.gravityVector   = {{0.0, 0.0, -10.0}};
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;
  return meta;
}

/* ---- section 1: the hopper ------------------------------------------------ */
double hopperStep(int threads)
{
  std::vector<Object> bodies;
  const delta::core::io::Scenario scene =
      delta::core::io::readmbfcp(delta::core::io::inputPath("hopper.mbfcp"), bodies, 0.002);

  delta::core::ContactParams params;
  if (scene.surface.spring > 0.0) params.spring = scene.surface.spring;
  delta::contact::forces::setParams(params);

  setThreads(threads);
  delta::core::Engine engine(bodies, {{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}},
                             makeMeta(delta::core::data::Meta::CollisionModel::Sphere));
  const int steps = 20;
  const double t0 = now();
  for (int i = 0; i < steps; ++i) engine.iterate();
  return 1000.0 * (now() - t0) / steps;
}

/* ---- section 2: a box pile, per model ------------------------------------- */
double pileStep(delta::core::data::Meta::CollisionModel model, int threads)
{
  std::vector<Object> bodies;
  const iREAL half = 0.03, top = 0.0;
  Mesh* floorMesh = makeBox({{0.5, 0.5, top - 0.5}}, 0.5);
  bodies.push_back(Object("floor", 0, floorMesh, {{0.5, 0.5, top - 0.5}},
                          delta::geometry::material::MaterialType::WOOD,
                          true, false, true, 0.002, {{0,0,0}}, {{0,0,0}}));

  int id = 1;
  for (int i = 0; i < 6; ++i)
    for (int j = 0; j < 6; ++j)
    {
      const iREAL x = 0.2 + 0.059 * i, y = 0.2 + 0.059 * j;
      Mesh* box = makeBox({{x, y, top + half - 0.001}}, half);
      bodies.push_back(Object("box", id++, box, {{x, y, top + half - 0.001}},
                              delta::geometry::material::MaterialType::WOOD,
                              false, true, true, 0.002, {{0,0,0}}, {{0,0,0}}));
    }

  setThreads(threads);
  delta::core::Engine engine(bodies, {{-1.0, -1.0, -1.0, 2.0, 2.0, 2.0}},
                             makeMeta(model));
  const int steps = 10;
  const double t0 = now();
  for (int i = 0; i < steps; ++i) engine.iterate();
  return 1000.0 * (now() - t0) / steps;
}

/* ---- section 3: one kernel call on two big walls -------------------------- */
struct Wall
{
  std::vector<iREAL> x, y, z;
  int triangles = 0;
};

/* A flat n x n grid of triangles in the plane z = `zc`, with a gap in x. */
Wall makeGrid(int n, iREAL x0, iREAL zc)
{
  Wall w;
  const iREAL h = 0.002;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
    {
      const iREAL xa = x0 + i * h, xb = xa + h;
      const iREAL ya = j * h, yb = ya + h;
      const iREAL tri[2][3][3] = {{{xa, ya, zc}, {xb, ya, zc}, {xb, yb, zc}},
                                  {{xa, ya, zc}, {xb, yb, zc}, {xa, yb, zc}}};
      for (int t = 0; t < 2; ++t)
        for (int k = 0; k < 3; ++k)
        {
          w.x.push_back(tri[t][k][0]);
          w.y.push_back(tri[t][k][1]);
          w.z.push_back(tri[t][k][2]);
        }
      w.triangles += 2;
    }
  return w;
}

double kernelCall(const char* which, int threads)
{
  const int n = 45;                       /* 45*45*2 = 4050 triangles */
  const Wall a = makeGrid(n, 0.0, 0.0);
  const Wall b = makeGrid(n, 0.0015, 0.0); /* 1.5 mm apart, inside epsilon */

  setThreads(threads);
  const int reps = 5;
  const double t0 = now();
  std::size_t contacts = 0;
  for (int r = 0; r < reps; ++r)
  {
    std::vector<delta::contact::contactpoint> cps;
    if (std::string(which) == "bf")
      cps = delta::contact::detection::bf(a.x.data(), a.y.data(), a.z.data(), a.triangles, 0.002, true, 1,
                                          b.x.data(), b.y.data(), b.z.data(), b.triangles, 0.002, true, 2);
    else if (std::string(which) == "penalty")
      cps = delta::contact::detection::penalty(a.x.data(), a.y.data(), a.z.data(), a.triangles, 0.002, true, 1,
                                               b.x.data(), b.y.data(), b.z.data(), b.triangles, 0.002, true, 2);
    else if (std::string(which) == "hybrid")
      cps = delta::contact::detection::hybridWithPerTriangleFallBack(
              a.x.data(), a.y.data(), a.z.data(), a.triangles, 0.002, true, 1,
              b.x.data(), b.y.data(), b.z.data(), b.triangles, 0.002, true, 2);
    contacts = cps.size();
  }
  const double ms = 1000.0 * (now() - t0) / reps;
  std::printf("    %-10s %4d triangles  %8.2f ms  (%zu contacts)\n",
              which, a.triangles, ms, contacts);
  return ms;
}

const char* nameOf(delta::core::data::Meta::CollisionModel m)
{
  switch (m)
  {
    case delta::core::data::Meta::CollisionModel::Sphere:               return "Sphere";
    case delta::core::data::Meta::CollisionModel::BruteForce:           return "BruteForce";
    case delta::core::data::Meta::CollisionModel::Penalty:              return "Penalty";
    case delta::core::data::Meta::CollisionModel::HybridOnBatches:      return "HybridOnBatches";
    case delta::core::data::Meta::CollisionModel::HybridOnTrianglePairs:return "HybridOnTrianglePairs";
    default:                                                            return "?";
  }
}

}  // namespace

int main()
{
  std::printf("OpenMP: %s\n", DELTA_OMP ? "on" : "off");

  const int counts[] = {1, 2, 4, 8};

  std::printf("\n1. hopper.mbfcp, 15 651 bodies, CollisionModel::Sphere (20 steps)\n");
  double base = 0.0;
  for (int t : counts)
  {
    const double ms = hopperStep(t);
    if (t == 1) base = ms;
    std::printf("    threads=%d  %8.1f ms/step  speedup %.2fx\n", t, ms, base / ms);
  }

  std::printf("\n2. 36-box pile + floor, per model (10 steps)\n");
  const delta::core::data::Meta::CollisionModel models[] = {
      delta::core::data::Meta::CollisionModel::Sphere,
      delta::core::data::Meta::CollisionModel::BruteForce,
      delta::core::data::Meta::CollisionModel::Penalty,
      delta::core::data::Meta::CollisionModel::HybridOnBatches,
      delta::core::data::Meta::CollisionModel::HybridOnTrianglePairs,
  };
  for (auto m : models)
  {
    double b = 0.0;
    std::printf("  %s\n", nameOf(m));
    for (int t : counts)
    {
      const double ms = pileStep(m, t);
      if (t == 1) b = ms;
      std::printf("    threads=%d  %8.2f ms/step  speedup %.2fx\n", t, ms, b / ms);
    }
  }

  std::printf("\n3. single kernel call, two 4050-triangle walls, 1 vs 8 threads\n");
  for (const char* which : {"bf", "penalty", "hybrid"})
  {
    const double one = kernelCall(which, 1);
    const double many = kernelCall(which, 8);
    std::printf("    %-10s speedup %.2fx\n", which, one / many);
  }
  return 0;
}
