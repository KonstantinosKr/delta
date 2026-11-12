# AMReX backend and grid morphologies — work report

Date: 2026-11-12  
Branch: `amrex-integration`

## What this covers

Making AMReX the default 3D backend of delta, porting the thesis grid
morphologies out of the `amrex_contact` experiment tree into the backend, and
shrinking that tree to the parts delta does not have.

## Commits

| commit | subject |
|---|---|
| `fc447e06` | build: AMReX backend on by default, `--backend`/`--mode`, grid mode `regular` |
| `7a7918e6` | backend: adaptive broadphase hierarchy (thesis 6.3/6.4.2), `--mode adaptive` |
| `c3ef011b` | backend: reluctant mode + chapter-7 dt controller |
| `bfaf28a1` | layout: `docs/`, `tools/` and a shrunken thesis reference tree |

### `fc447e06` — AMReX reachable, default, and correct

- `find_package(AMReX CONFIG QUIET)` now drives `DELTA_USE_AMREX`, so a machine
  with the vendored install builds the parallel backend without an extra flag.
  `-DDELTA_USE_AMREX=OFF` still forces the serial build.
- `apps/dem3d`: `--backend serial|amrex` and `--mode` through a small `Backend`
  adapter (`apps/dem3d/backend.h`); `amrex::Initialize`/`Finalize` are owned by
  the app, which passes only the program name so delta's own flags never reach
  AMReX's ParmParse.
- `GridMode::Regular`: `dx` from the *smallest* body (thesis 6.3) instead of the
  largest, with the halo widened automatically (`nneighbor = ceil(reach/dx)`,
  which the soundness condition `nneighbor*dx >= reach` requires). `NoGrid`
  deliberately keeps the configured `nneighbor`, because the sizing test needs
  to build an unsound grid on purpose.
- **Bug fixed**: `DemAmrexEngine::syncTo` aborted on any scene with static
  obstacles (`particle N was not found on exactly one rank`). Obstacles are
  never in the AMReX container, so they carry no marker, but the loop checked
  every record. Its test scene has no obstacles, so this had never run.
- **Bug fixed**: `delta_dem_amrex_test` did not compile at all — with
  `AMReX_MPI=OFF`, `amrex::mpidatatypes::MPI_Comm` is an `int` and does not
  convert to OpenMPI's pointer type. The test is now gated on `AMReX_MPI`.
- Renames: `amrex_grids` → `amrex_contact`, `delta_lib` → `delta3d_core`,
  `delta_dem2d` → `delta2d_core`, `delta_dem2d_io` → `delta2d_io`; the dead
  `-DDim3` compile definition is gone (0 references in the tree).
- `tools/` and `viz/` moved under `apps/dem3d/`; `input/dim2/` removed.
- `tests/grid_modes_test.cpp`: mixed-size scene (0.01/0.02/0.04 radii), demands
  the identical contact pair set as `nogrid`.

### `7a7918e6` — adaptive

`src/dim3/backends/amrex/GridHierarchy.{h,cpp}`: `naturalLevel` per body, a
level exists only where a body of that level or deeper sits, coarse bodies
inherited downwards as virtual particles, candidate pairs from the 3³ stencil
plus a reach test, deduplicated.

The completeness argument is in the header: a pair whose bodies live on
different levels is found on the finer of the two, because the coarser body is
present there; a pair on one level is found there. That is what makes it legal
to replace AMReX's neighbour list.

`DemAmrexEngine`: `GridMode::Adaptive` routes `forcePass` through
`forcePassByPairs`, and the apply pass (`applyForceCache`) is shared with the
neighbour-list path so both go through the same serial ordering. Per pair both
endpoints are evaluated from their own perspective — the serial engine stores
every contact twice with the B copy's normal negated, so one-sided accumulation
would halve the force.

**Recorded limit:** single rank only. AMReX's neighbour list does the cross-rank
ghost exchange for the other modes; a hierarchy pair has no notion of a remote
body.

### `c3ef011b` — reluctant + dt controller

- `buildReluctant`: everything starts on the mono-cell; a cell is refined only
  when it holds `>= minBodiesToRefine` bodies and one pair within
  `approachGap` cells is not separating. At rest the approach dot product is
  exactly 0, so rest counts as approaching — otherwise a static pile would never
  refine and reluctant would collapse to the mono-cell for every scenario here.
  This deviation is documented in the source.
- Pair collection now bins a body only on its own level and the finer ones it is
  inherited into. The previous version put every body on level 0, which was
  correct but made the search asymptotically equivalent to `nogrid`.
- `DtController` (Algorithms 11/12): critical-approach bound `0.5*gap/|v_n|`,
  growth by the constant factor, no-tunnelling `0.5*h_min/Vmax`, and it never
  relaxes the incoming step — that value carries the contact spring's
  explicit-stability limit. `--dt-source grid` wires it in; the engine owns the
  step from then on.
- `tests/dt_controller_test.cpp`: gap bound, interpenetration guard (an
  overlapping pair must *not* tighten, or dt collapses to zero and the run
  freezes), tunnel rule, growth cap, separation.

### `bfaf28a1` — layout

- `amrex_contact/docs/*.md` → `docs/`; `build_amrex.sh` → `tools/`;
  `bench.py`, `verify.sh` → `tools/bench/`.
- `amrex_contact` keeps only what delta does not have: `src/grid`,
  `src/physics`, `include/contact`, and the two suites that measured them.
  `geom`, `scenario`, `io` and `app` were duplicates and are deleted.
  `DELTA_BUILD_AMREX_CONTACT` now defaults to `OFF` because nothing links it.

## Verification (before `bfaf28a1`)

```
hopper, 15 651 bodies, 10 steps of dt = 1e-5
--mode nogrid     step 0 contacts=47600 max|v|=1.01835622   step 9 max|v|=10.29417399
--mode regular    identical
--mode adaptive   identical
--mode reluctant  identical

ctest: 12/12 pass  (viz_write, dem_phase0, dem2d, dem2d_viz, regression,
                    mesh_buffer, grid_modes, dt_controller, narrowphase,
                    physics, scenario, grid)
```

`--mode regular` is a no-op on the hopper and that is correct: all 15 625
spheres are radius 0.02, so `minReach == maxReach`. The mixed-size test scene is
what distinguishes the modes.

`--dt-source grid` on the hopper changes the trajectory slightly
(`max|v|` at step 9: 9.40 instead of 10.29) because the controller found
critical approaches and tightened the step. That is the intended behaviour.

## Not verified

- The shrunken `amrex_contact` was configured and built with
  `-DDELTA_BUILD_AMREX_CONTACT=ON` (10/10 tests) *before* `bfaf28a1`. The moves
  in that commit changed `build_amrex.sh`'s `ROOT` and nothing else CMake reads,
  so configuration is expected to be unaffected — but it was not re-run.
- `tools/bench/verify.sh` and `bench.py` are **stale**: they still drive the
  deleted `contact_dem` binaries. Both are marked `STALE` in a header comment.
  `verify.sh` will fail at its first `BIN` call.
- `docs/RESULTS.md` still reports numbers produced by the deleted experiment
  binaries. Nothing regenerates them.

## Known gaps

1. **`src/dim3/contact/detection` has no direct test coverage.** The 2 472 lines
   of narrowphase/scenario assertions were deleted with the code they tested;
   `grid_modes` exercises the kernel only indirectly through the sphere path.
   This is the largest gap and it predates this work.
2. **Two tri-tri implementations became one by deletion, not by decision.** The
   survivor is delta's (`bf`/`penalty`/`hybrid`/`gjk`), which is the less-tested
   one. Port the deleted assertions onto it.
3. **`adaptive`/`reluctant` are single-rank.** Making them parallel means asking
   AMReX for the multi-level pair list and keeping `GridHierarchy` only as the
   cell-set constructor (option "A" in the design discussion).
4. **Contact force direction is still unverified.** An earlier investigation
   found `src/dim3/contact/forces/forces.cpp` applying the normal spring along
   the normal rather than against it, so overlapping bodies accelerate towards
   each other — reproduced with two spheres, and visible at scale as the
   hopper's `max|v|` ramping 1 → 10 m/s from rest. Both backends call the same
   `forces.cpp`, so both agree on it. No test asserts a contact direction.
5. **Naming:** `DemAmrexEngine` / `DemParticleContainer` / `DemParallelConfig`
   carry a redundant `Dem` prefix inside a codebase called delta; the `Backend`
   contract lives in `apps/dem3d/backend.h` rather than `src/core/backend.h`.

## Interfaces added

```cpp
// src/dim3/backends/amrex/GridHierarchy.h
void build(const std::vector<GridBody>& bodies, lo, hi, iREAL maxReach);
const std::vector<std::pair<int,int>>& candidatePairs() const;
int naturalLevel(iREAL diameter) const;

// src/dim3/backends/amrex/DtController.h
ApproachState stateOf(const GridBody& a, const GridBody& b, iREAL& vn, iREAL& gap) const;
DtDecision update(const std::vector<GridBody>&, const std::vector<std::pair<int,int>>&, iREAL hMin);

// src/dim3/backends/amrex/DemAmrexEngine.h
enum class GridMode { NoGrid, Regular, Adaptive, Reluctant };   // DemParallelConfig::mode
bool dynamicDt; DtState dtState;                                 // DemParallelConfig
iREAL stepSize() const;
const DtDecision& lastDtDecision() const;
int levels() const; long cells() const; iREAL minReach() const;
```
