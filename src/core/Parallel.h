/*
 * Parallel/SIMD configuration for the contact kernels.
 *
 * The contact path has two levels of parallelism, and both are optional at
 * build time so the project still builds on a machine without libomp:
 *
 *   DELTA_OMP       multicore: OpenMP threads over candidate pairs, particles
 *                   and triangles (thesis 8.1 triangle-mesh parallelisation,
 *                   8.2 particle-to-particle).
 *   DELTA_OMP_SIMD  SIMD: `omp simd` on the innermost independent loop of each
 *                   contact method (thesis 5.4/5.6/5.8).
 *
 * Both are off when DELTA_HAVE_OPENMP is not defined, and every use site must
 * compile to the plain scalar loop in that case. Include this header instead of
 * <omp.h> so a build without OpenMP never sees the pragma or the runtime.
 */
#ifndef DELTA_CORE_PARALLEL_H_
#define DELTA_CORE_PARALLEL_H_

#ifdef DELTA_HAVE_OPENMP
#include <omp.h>
#define DELTA_OMP 1
#else
#define DELTA_OMP 0
#endif

/* `#pragma omp simd iff(...)` cannot be written conditionally, so the whole
 * pragma goes through _Pragma and becomes empty when OpenMP is off. */
#if DELTA_OMP
#define DELTA_PRAGMA_(x) _Pragma(#x)
#define DELTA_PRAGMA(x) DELTA_PRAGMA_(x)
#define DELTA_OMP_SIMD DELTA_PRAGMA(omp simd)
#define DELTA_OMP_SIMD_REDUCTION(op, var) DELTA_PRAGMA(omp simd reduction(op : var))
#define DELTA_OMP_PARALLEL_FOR DELTA_PRAGMA(omp parallel for)
#define DELTA_OMP_PARALLEL_FOR_IF(cond) DELTA_PRAGMA(omp parallel for if(cond))
/* Unbalanced loops (a sphere-vs-mesh pair costs orders of magnitude more than
 * sphere-vs-sphere) take small dynamic chunks. */
#define DELTA_OMP_PARALLEL_FOR_DYNAMIC_IF(cond) DELTA_PRAGMA(omp parallel for schedule(dynamic, 8) if(cond))
#else
#define DELTA_OMP_SIMD
#define DELTA_OMP_SIMD_REDUCTION(op, var)
#define DELTA_OMP_PARALLEL_FOR_IF(cond)
#define DELTA_OMP_PARALLEL_FOR_DYNAMIC_IF(cond)
#endif

namespace delta {
namespace core {
namespace parallel {

/* Fork/join costs ~24 us at 8 threads on the machine this was tuned on, while
 * one triangle-to-triangle check costs ~0.05 us. Below this many iterations the
 * scalar loop wins, so the kernels gate their parallel regions on it.
 * shortcut: one fixed grain size for every kernel, retune if the hardware or
 * the mesh sizes change. */
constexpr int kForkJoinGrain = 512;

/* One candidate pair costs a whole narrowphase call (tens of microseconds), so
 * the pair loop breaks even far earlier than a triangle loop does. */
constexpr int kForkJoinGrainPairs = 16;

/* Threads OpenMP will hand out for the next parallel region. 1 without OpenMP. */
inline int threads()
{
#if DELTA_OMP
  return omp_get_max_threads();
#else
  return 1;
#endif
}

}  // namespace parallel
}  // namespace core
}  // namespace delta

#endif  // DELTA_CORE_PARALLEL_H_
