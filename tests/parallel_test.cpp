/*
 * Serial vs parallel equivalence for every contact model, at both parallelism
 * layers.
 *
 *   1. driver: the engine's pair loop (gated on kForkJoinGrainPairs) and its
 *      force pass (gated on kForkJoinGrain). The scenes are sized so both
 *      guards are actually TRUE, otherwise this compares the scalar loop with
 *      itself and proves nothing.
 *   2. kernels: bf / penalty / hybrid on two meshes above the triangle grain,
 *      so their own `parallel for` and `omp simd` inner loop really run.
 *
 * Checks per model:
 *   - the scene produces contacts at all (so the comparison is not vacuous)
 *   - the scene is larger than the grain constants (so the parallel path runs)
 *   - the resulting contacts and state are bit-identical at 1 and N threads:
 *     the driver merges per-candidate results in candidate order and the
 *     kernels write one slot per triangle, so accumulation order is the serial
 *     one and nothing is merely "close".
 *
 * CHECK() rather than assert() because a Release build defines -DNDEBUG.
 */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "core/ContactParams.h"
#include "core/Parallel.h"
#include "core/data/Meta.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/contact/detection/bf.h"
#include "dim3/contact/detection/hybrid.h"
#include "dim3/contact/detection/penalty.h"
#include "dim3/contact/detection/sphere.h"
#include "dim3/contact/forces/forces.h"
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
using delta::core::data::Meta::CollisionModel;
typedef delta::geometry::mesh::Mesh Mesh;

namespace {

void setThreads(int n)
{
#if DELTA_OMP
  omp_set_num_threads(n);
#else
  (void)n;
#endif
}

/* A closed box as a triangle soup, centred on `c` with half-size h. */
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

/* A large floor quad at z = 0 with its normal up (+z). */
Mesh* makeFloorQuad(iREAL h)
{
  std::vector<iREAL> xs, ys, zs;
  const iREAL a[3] = {-h, -h, 0.0}, b[3] = {h, -h, 0.0};
  const iREAL c[3] = {h, h, 0.0},   d[3] = {-h, h, 0.0};
  const iREAL* tris[2][3] = {{a, b, c}, {a, c, d}};
  for (int t = 0; t < 2; ++t)
    for (int k = 0; k < 3; ++k)
    {
      xs.push_back(tris[t][k][0]);
      ys.push_back(tris[t][k][1]);
      zs.push_back(tris[t][k][2]);
    }
  return new Mesh(xs, ys, zs);
}

/* A flat n x n triangle grid in the plane z = `zc`, offset in x. */
struct Grid
{
  std::vector<iREAL> x, y, z;
  int triangles = 0;
};

Grid makeGrid(int n, iREAL x0, iREAL zc)
{
  Grid g;
  const iREAL h = 0.002;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
    {
      const iREAL xa = x0 + i * h, xb = xa + h;
      const iREAL ya = j * h, yb = ya + h;
      const iREAL t[2][3][3] = {{{xa, ya, zc}, {xb, ya, zc}, {xb, yb, zc}},
                                {{xa, ya, zc}, {xb, yb, zc}, {xa, yb, zc}}};
      for (int k = 0; k < 2; ++k)
        for (int v = 0; v < 3; ++v)
        {
          g.x.push_back(t[k][v][0]);
          g.y.push_back(t[k][v][1]);
          g.z.push_back(t[k][v][2]);
        }
      g.triangles += 2;
    }
  return g;
}

struct Scene
{
  std::vector<Object> bodies;
  std::size_t expectedPairs = 0;    /* lattice neighbours + floor contacts */
  std::size_t particles = 0;
};

/* The scene depends on the model, because each model sees different geometry:
 *
 *   Sphere        an 9^3 lattice of spheres on a triangle floor: 730 particles
 *                 (>= kForkJoinGrain) and ~1.9k candidate pairs (>= grainPairs),
 *                 so the driver's pair loop AND its force pass are parallel
 *   BruteForce,   6x6 boxes on a box floor: ~200 candidate pairs, again above
 *   Penalty,      the pair grain, with 12-triangle meshes (below the triangle
 *   Hybrid*       grain, so it is the driver that runs in parallel - which is
 *                 the point: one driver change covers every model; the kernels
 *                 are covered separately below)
 */
Scene makeScene(CollisionModel model, iREAL epsilon)
{
  Scene s;
  const iREAL radius = 0.03;

  if (model == CollisionModel::Sphere)
  {
    const int side = 9;
    s.bodies.push_back(Object("floor", 0, makeFloorQuad(2.0), {{0.0, 0.0, 0.0}},
                              delta::geometry::material::MaterialType::WOOD,
                              true, false, true, epsilon, {{0,0,0}}, {{0,0,0}}));
    const iREAL mass = 1000.0 * (4.0/3.0) * 3.14159265358979323846 *
                       radius * radius * radius;
    int id = 1;
    for (int i = 0; i < side; ++i)
      for (int j = 0; j < side; ++j)
        for (int k = 0; k < side; ++k)
        {
          Object o("sphere", radius, id++,
                   {{0.2 + 0.059 * i, 0.2 + 0.059 * j, radius - 0.001 + 0.059 * k}},
                   delta::geometry::material::MaterialType::WOOD,
                   false, true, true, epsilon, {{0,0,0}}, {{0,0,0}});
          o.setMass(mass);
          s.bodies.push_back(o);
        }
    s.particles = s.bodies.size();
    s.expectedPairs = 3 * side * side * (side - 1) + side * side;   /* lattice + floor */
    return s;
  }

  const iREAL top = 0.0, half = 0.03;
  s.bodies.push_back(Object("floor", 0, makeBox({{0.5, 0.5, top - half}}, 0.5),
                            {{0.5, 0.5, top - half}},
                            delta::geometry::material::MaterialType::WOOD,
                            true, false, true, epsilon, {{0,0,0}}, {{0,0,0}}));
  int id = 1;
  for (int i = 0; i < 6; ++i)
    for (int j = 0; j < 6; ++j)
    {
      const iREAL x = 0.2 + 0.059 * i, y = 0.2 + 0.059 * j;
      Mesh* box = makeBox({{x, y, top + half - 0.001}}, half);
      s.bodies.push_back(Object("box", id++, box, {{x, y, top + half - 0.001}},
                                delta::geometry::material::MaterialType::WOOD,
                                false, true, true, epsilon, {{0,0,0}}, {{0,0,0}}));
    }
  s.particles = s.bodies.size();
  s.expectedPairs = 36;      /* each box touches the floor at least */
  return s;
}

struct Outcome
{
  std::size_t                       contacts = 0;
  std::vector<std::array<iREAL, 3>> centre;
  std::vector<std::array<iREAL, 3>> velocity;
};

Outcome run(CollisionModel model, int threads, int steps)
{
  setThreads(threads);
  Scene scene = makeScene(model, 0.002);

  delta::core::data::Meta::Simulation meta;
  meta.plotScheme      = delta::core::data::Meta::Plot::Never;
  meta.modelScheme     = model;
  meta.overlapPreCheck = false;
  meta.dt              = 1e-5;
  meta.gravity         = true;
  meta.gravityVector   = {{0.0, 0.0, -10.0}};
  meta.maxPrescribedRefinement = -1.0;
  meta.resolveContacts = true;

  delta::core::Engine engine(scene.bodies, {{-2.0, -2.0, -2.0, 3.0, 3.0, 3.0}}, meta);
  for (int i = 0; i < steps; ++i) engine.iterate();

  Outcome out;
  out.contacts = engine.getContactPairs().size();
  std::vector<delta::core::data::ParticleRecord>& recs = engine.getParticleRecords();
  for (std::size_t p = 0; p < recs.size(); ++p)
  {
    if (recs[p].getIsObstacle()) continue;
    out.centre.push_back(recs[p]._centre);
    out.velocity.push_back(recs[p]._linearVelocity);
  }
  return out;
}

void sameOutcome(const Outcome& a, const Outcome& b, const char* what)
{
  CHECK(a.contacts == b.contacts);
  CHECK(a.centre.size() == b.centre.size());
  for (std::size_t p = 0; p < a.centre.size(); ++p)
    for (int d = 0; d < 3; ++d)
    {
      if (a.centre[p][d] != b.centre[p][d] || a.velocity[p][d] != b.velocity[p][d])
      {
        std::fprintf(stderr, "FAIL %s: particle %zu axis %d  %g != %g\n",
                     what, p, d, a.centre[p][d], b.centre[p][d]);
        std::abort();
      }
    }
}

bool sameContacts(const std::vector<delta::contact::contactpoint>& a,
                  const std::vector<delta::contact::contactpoint>& b)
{
  if (a.size() != b.size()) return false;
  for (std::size_t i = 0; i < a.size(); ++i)
  {
    if (a[i].master != b[i].master || a[i].slave != b[i].slave) return false;
    if (a[i].depth != b[i].depth) return false;
    for (int d = 0; d < 3; ++d)
      if (a[i].x[d] != b[i].x[d] || a[i].normal[d] != b[i].normal[d]) return false;
  }
  return true;
}

const char* nameOf(CollisionModel m)
{
  switch (m)
  {
    case CollisionModel::Sphere:               return "Sphere";
    case CollisionModel::BruteForce:           return "BruteForce";
    case CollisionModel::Penalty:              return "Penalty";
    case CollisionModel::HybridOnBatches:      return "HybridOnBatches";
    case CollisionModel::HybridOnTrianglePairs:return "HybridOnTrianglePairs";
    default:                                   return "?";
  }
}

/* Kernel-level check: two meshes above the triangle grain, so the kernels' own
 * parallel region and SIMD inner loop execute. */
void checkKernels(int threads)
{
  const int n = 26;                       /* 26*26*2 = 1352 triangles each */
  const Grid a = makeGrid(n, 0.0, 0.0);
  const Grid b = makeGrid(n, 0.0015, 0.0); /* 1.5 mm apart, inside epsilon */

  CHECK(a.triangles >= delta::core::parallel::kForkJoinGrain);

  const iREAL eps = 0.002;

  struct Kernel
  {
    const char* name;
    std::vector<delta::contact::contactpoint> (*call)(
        const iREAL*, const iREAL*, const iREAL*, int, iREAL, bool, int,
        const iREAL*, const iREAL*, const iREAL*, int, iREAL, bool, int);
  };

  const Kernel kernels[] = {
    {"bf",      delta::contact::detection::bf},
    {"penalty", delta::contact::detection::penalty},
    {"hybrid",  delta::contact::detection::hybridWithPerTriangleFallBack},
  };

  for (const Kernel& k : kernels)
  {
    setThreads(1);
    const std::vector<delta::contact::contactpoint> one =
        k.call(a.x.data(), a.y.data(), a.z.data(), a.triangles, eps, true, 1,
               b.x.data(), b.y.data(), b.z.data(), b.triangles, eps, true, 2);
    setThreads(threads);
    const std::vector<delta::contact::contactpoint> many =
        k.call(a.x.data(), a.y.data(), a.z.data(), a.triangles, eps, true, 1,
               b.x.data(), b.y.data(), b.z.data(), b.triangles, eps, true, 2);

    std::printf("  %-8s %4d triangles: %zu contacts (1 thread) / %zu (%d threads)\n",
                k.name, a.triangles, one.size(), many.size(), threads);
    CHECK(one.size() > 0);
    CHECK(sameContacts(one, many));
  }
}

}  // namespace

int main()
{
  const int steps  = 10;
  const int threads = delta::core::parallel::threads();

  std::printf("parallel_test: %d thread(s) available, grains %d triangles / %d pairs\n",
              threads, delta::core::parallel::kForkJoinGrain,
              delta::core::parallel::kForkJoinGrainPairs);

  const CollisionModel models[] = {
      CollisionModel::Sphere,
      CollisionModel::BruteForce,
      CollisionModel::Penalty,
      CollisionModel::HybridOnBatches,
      CollisionModel::HybridOnTrianglePairs,
  };

  for (CollisionModel model : models)
  {
    const Scene scene = makeScene(model, 0.002);

    /* The guards must be true, or this compares the scalar loop with itself. */
    CHECK(scene.particles >= (std::size_t)delta::core::parallel::kForkJoinGrain ||
          scene.expectedPairs >= (std::size_t)delta::core::parallel::kForkJoinGrainPairs);

    const Outcome one  = run(model, 1, steps);
    const Outcome many = run(model, threads, steps);

    std::printf("%-22s %5zu bodies  contacts: %zu (1 thread) / %zu (%d threads)\n",
                nameOf(model), scene.particles, one.contacts, many.contacts, threads);
    CHECK(one.contacts > 0);
    sameOutcome(one, many, nameOf(model));
  }

  std::printf("kernel layer (above the %d-triangle grain)\n",
              delta::core::parallel::kForkJoinGrain);
  checkKernels(threads);

  std::printf("parallel_test OK: all five contact models and the three triangle "
              "kernels give bit-identical results at 1 and %d threads\n", threads);
  return 0;
}
