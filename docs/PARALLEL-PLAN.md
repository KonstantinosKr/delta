# Parallel + SIMD contact kernels — plan

Goal: every contact method has a multicore path and a SIMD path, gated at build
time, with the serial scalar loop unchanged as the reference.

## What the thesis says, and what the code has

| Thesis | In this repo before this work |
|---|---|
| Ch. 5.4/5.6/5.8 — vectorised narrowphase, loop-footprint optimisation, Algorithm 7 hybrid-on-batches | scalar C++; `THESIS-MAPPING.md` §3: "No explicit SIMD; compiler auto-vectorisation only" |
| Ch. 8.1 triangle-mesh parallelisation, 8.2 particle-to-particle, 8.3 grid producer–consumer, 8.4 grain size, Algorithms 13/15/16 | none; `THESIS-MAPPING.md` §3: out of scope |
| reference implementation (`tests/thesis/`) | scalar — checked, no `omp`/`simd`/`thread` outside comments |

What *did* exist is dead scaffolding:

* `#ifdef OMPProcess` / `#ifdef OMPTriangle` pragmas in `detection/bf.cpp`,
  `detection/hybrid.cpp`, `detection/penalty.cpp`, `contact/contactpoint.cpp`,
  `geometry/operators/mesh.cpp`, `geometry/hull/hullcube.cpp`. Neither macro is
  defined anywhere, and the build had no `-fopenmp`, so all of it compiled out.
* `detection/bf.ispc`, `detection/penalty.ispc` — real SIMD kernels, but the
  project is not built with `ispc` (not installed, no CMake rule, no call site);
  `ispcMakefile` targets Intel `icpc`.
* One bare `#pragma omp parallel for` in `Engine::updatePosition` (mesh vertices),
  inert for the same reason.

So: not in the code. This plan adds it.

## Two layers

**1. Driver (`backends/serial/Engine.cpp`)** — where the work actually is:
`binnedCandidates`, `evaluateCandidates` (the pair loop) and `deriveForces`
(the per-particle force accumulation). Parallelising the driver gives *every*
`CollisionModel` multicore behaviour with one change, because they all flow
through `evaluateCandidates`.

**2. Kernels (`detection/*.cpp`)** — per method, inside the existing signatures:

| model | function | parallel over | SIMD over |
|---|---|---|---|
| `Sphere` | `sphere()` | pairs (driver), triangles for the barrier variants | batched pairs (SoA) |
| `BruteForce` | `bf()` | triangles of A | triangle pairs of the inner loop |
| `Penalty` | `penalty()` | triangles of A | triangle pairs, penalty iterations |
| `HybridOnBatches` | `hybridWithPerBatchFallBack()` | batches | batch lanes |
| `HybridOnTrianglePairs` | `hybridWithPerTriangleFallBack()` | triangles | triangle pairs |

Rules for both layers:

* include `src/core/Parallel.h`, never `<omp.h>` directly;
* no `std::vector::push_back` inside a parallel region — per-thread buffers,
  appended in a deterministic order at the end;
* nested parallel regions are disabled by the driver
  (`omp_set_max_active_levels(1)`), so a kernel region inside a pair region
  runs with one thread and costs nothing;
* with `-DDELTA_ENABLE_OPENMP=OFF` the code must be the scalar loop exactly as
  today, and the suite must stay green.

## Build

`DELTA_ENABLE_OPENMP` (default ON). `find_package(OpenMP)` first; on AppleClang,
where CMake's module cannot see MacPorts/Homebrew, fall back to
`/opt/local/include/libomp` + `/opt/local/lib/libomp` (verified working, 8
threads). Without libomp the build prints a status line and stays scalar.

## Acceptance

1. `ctest` green with OpenMP off **and** on.
2. For each `CollisionModel`: same contact-pair set and same positions after N
   steps at 1 thread vs T threads (tolerance 1e-12 relative — accumulation order
   may differ, the physics must not).
3. A benchmark table: ms/step for each model, 1 thread / all threads, on the
   hopper scenario.

## Files

| workstream | owns |
|---|---|
| foundation (done) | `src/core/Parallel.h`, the `DELTA_ENABLE_OPENMP` block in `CMakeLists.txt` |
| kernels | `src/dim3/contact/detection/{sphere,bf,penalty,hybrid}.cpp` |
| driver | `src/dim3/backends/serial/Engine.{h,cpp}` |
| tests + bench | `tests/parallel_test.cpp`, `tools/bench/parallel_bench.cpp` |

## Result (measured)

`./build/parallel_bench` on this machine (8 CPUs, macOS, AppleClang, Release).
All numbers are ms/step; speedup is against 1 thread.

**1. The real scenario — `hopper.mbfcp`, 15 651 bodies, `CollisionModel::Sphere`**
(measured with `hf_big`, 500 steps, 1 vs 8 threads: 262.9 → 136.8 ms/step,
1.92×, and the two runs' 15 625 positions are bit-identical.)

| threads | 1 | 2 | 4 | 8 |
|---|---|---|---|---|
| ms/step | 261.0 | 185.6 | 147.6 | 134.2 |
| speedup | 1.00× | 1.41× | 1.77× | **1.95×** |

Serial reference: the same driver linked against the pre-parallel library
(`git worktree`, HEAD) runs this scenario at 237.3 ms/step, so the parallel
build at 8 threads is **1.77× faster than the code it replaced**, and its own
1-thread figure carries ~10 % overhead from the deterministic merge (per-pair
result slots and the serial replay).

2× and not 8× because the parallel part is the pair loop and the force pass;
the binned broad phase's binning, the serial merge into the collision map and
`addCollision` are still one thread, and they are a large share of 15 651
bodies × ~47 600 contacts.

**2. 36-box pile + floor, every triangle model (driver-level parallelism)**

| model | 1 | 2 | 4 | 8 |
|---|---|---|---|---|
| `Sphere` | 0.35 | 0.31 (1.13×) | 0.32 (1.12×) | 0.33 (1.06×) |
| `BruteForce` | 18.98 | 10.49 (1.81×) | 5.76 (3.30×) | 3.80 (**4.99×**) |
| `Penalty` | 14.45 | 7.61 (1.90×) | 3.99 (3.62×) | 2.51 (**5.75×**) |
| `HybridOnBatches` | 14.65 | 7.64 (1.92×) | 4.12 (3.56×) | 2.61 (**5.61×**) |
| `HybridOnTrianglePairs` | 14.60 | 7.58 (1.93×) | 4.09 (3.68×) | 2.62 (**5.57×**) |

The meshes here are 12 triangles, below the kernels' fork/join grain (512), so
this is the driver's pair loop doing the work — which is the point of putting
parallelism there: every model benefits without touching its kernel.

**3. Kernel-level path — one `detection::*` call, two 4050-triangle walls**

| kernel | 1 thread | 8 threads | speedup |
|---|---|---|---|
| `bf` | 2272.8 ms | 599.3 ms | 3.79× |
| `penalty` | 3903.0 ms | 638.8 ms | 6.11× |
| `hybrid` (per-triangle) | 3868.8 ms | 642.1 ms | 6.03× |

This is where the kernels' own `parallel for` over triangles of A and their
`omp simd` inner loop over triangles of B engage. The SIMD half is compiler
work (clang vectorises the `bfSolver`/`penaltySolver` inner loop, which is why
those helpers carry `declare simd` with the `linear(...:3)` clause); the
parallel half is the pragma.

## Two bugs this work surfaced

* **560 KB of stack per loop iteration.** `bf`/`penalty`/`hybrid` kept
  `xPA[10000] … d[10000]` on the stack. Once the triangle loop is parallel, each
  thread needs that frame and the 512 KB OpenMP thread stack overflows — a bus
  error the first time a 4050-triangle mesh ran at 8 threads. They are heap
  buffers now, and sized from the partner mesh instead of a hard-coded 10000
  (that bound was never checked).
* `updatePosition`'s bare `#pragma omp parallel for` (mesh vertices, inside the
  per-particle loop) fired 15 651 fork/joins per step once OpenMP was actually
  on: 353 ms/step at 8 threads versus 2 ms at 1. It is gated now
  (`if(numberOfVertices > 1024)`), which is why the suite's wall time fell from
  44.9 s to 15.1 s.

## Verification

* `tests/parallel_test.cpp` (ctest `parallel`): every model, 1 thread versus all
  threads, **bit-identical** contacts and state — the driver merges per-candidate
  results in candidate order, so accumulation order is the serial one.
  The scenes are sized so the guards are actually true (730 bodies / 2025
  contacts for `Sphere`, 110 contacts for the mesh models) and a second section
  calls `bf`, `penalty` and `hybridWithPerTriangleFallBack` directly on two
  1352-triangle walls, which clears the 512-triangle grain: 66k/67k contacts
  compared element-wise at 1 and 8 threads. Without that, the first version of
  this test compared the scalar loop with itself — every grain guard was false.
* `ctest` is green with `-DDELTA_ENABLE_OPENMP=ON` (10/10) and
  `-DDELTA_ENABLE_OPENMP=OFF` (10/10), so the scalar path is untouched.
* The three kernels return the same contact count in the ON and OFF builds
  (205 194 `bf`, 211 469 `penalty`/`hybrid` on the 4050-triangle pair), which is
  how the `declare simd` layout was checked.

## Review findings that were acted on

* `bf.h` declared `linear(...:3)` for the **A** triangle pointers too, but the
  enclosing `omp simd` loop runs over triangles of B while A is loop-invariant:
  a lane k would be told to read `A + 3k`. Dropped; only the B pointers are
  linear now. (No behaviour change with the current clang — the contact counts
  above are unchanged — but the clause described a layout the call site does not
  have.)
* The grain guards made the first version of `parallel_test` vacuous (5 bodies,
  ≤12 triangles: every `if()` false). Fixed as described above.
* Left as-is, with comments: `omp_set_max_active_levels(1)` is process-global
  (deliberate — the driver owns the outer region); `hybridWithPerTriangleFallBack`
  walks A's triangles with B's triangle count (pre-existing bug, flagged with a
  `shortcut:` comment — fixing it changes results); per-triangle scratch is
  heap-allocated per outer iteration rather than thread-local (works, ~35k small
  allocations in the worst measured call).
