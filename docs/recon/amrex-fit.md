# Can delta's parallel layer move onto AMReX? Fit assessment, mapping, deletions, first PR

Recon only. Read-only: no repo file was modified, no commit made. Branch `amrex-integration`, HEAD `5408cff2`.

> **Status update.** The legacy trees discussed below (`zoltan/`, `multiscale/`,
> `meta/`, `setup/` and the Eclipse project files) have since been deleted — they
> were never in the build. References to them here are historical.
Every claim is grounded in a delta source line, an AMReX 26.09 header line, or one of three probes that were compiled and run.
Probe sources (outside the repo): `/tmp/mirror_probe.cpp`, `/tmp/mesh_probe.cpp`, `/tmp/npcprobe/`.

---

## 0. Verdict

1. **It fits, and it is a smaller job than `AMREX_INTEGRATION_PLAN.md` describes.** delta's serial `Engine` is already structured as *broad phase → per-particle contact list → per-particle force → per-particle integrate* (`Engine.cpp:583`, `620`, `696`). AMReX `NeighborParticleContainer` supplies exactly the missing pieces (decomposition, migration, ghost/neighbour exchange, a bin-based pair list) and nothing else is needed. The hand-written parallel layer (`zoltan/`, 9,377 LOC of `*.cpp`/`*.h`, plus ~11 kLOC of vendored `zoltan/ext/hul`) is dead code today — `delta/CMakeLists.txt` is the only CMake entry point and its glob is `delta/**`.
2. **The contact kernels do not need to change, not even their signature.** `ptd.rdata(k)` hands out `iREAL*`-compatible pointers, and the forces are provably reproducible per-rank with the *local* particle in the "A" role (measured bitwise-identical, §2.3).
3. **The two real hazards are silent, both from AMReX assertions being compiled out in the bundled install**: wrong grid/ghost sizing silently drops pairs (measured: 40 missing pairs, no error), and particles outside the domain are silently `make_invalid()`-ed or abort. delta has no domain at all today, so defining one is part of the work.
4. **The asked question — plain cell list vs `NeighborParticleContainer` — split answer.** For particle–particle: use NPC (a hand-rolled cell list would have to reimplement the ghost exchange, i.e. the hard half of `zoltan/migration.cpp`). For particle–obstacle: do **not** put obstacles in an AMReX container — keep the existing local sphere×mesh loop (`Engine.cpp:521-548`); that is the case where the plain loop is simpler and already correct.
5. **`for_each_pair` does not exist in AMReX 26.09** (grep over every installed header: 0 hits), nor does `fillGhosts`. The plan document's API names are wrong; the real API is `fillNeighbors/updateNeighbors/buildNeighborList` + iterating the per-tile list by hand.

---

## 1. Evidence base

| # | What was run | Observed |
|---|---|---|
| E1 | `./build/delta_dem_test` (serial oracle, `tests/dem_phase0_test.cpp`, `add_test dem_phase0` in `delta/CMakeLists.txt:48`) | `dem_phase0_test OK: 1000 particles, 20 steps, max contacts=2348, max\|force\|=844.536`; `third law OK: dvA=0.4 dvB=-0.4 momentum=0.000e+00`. 1.33 s total. This is the acceptance oracle. |
| E2 | `mpirun -n 2 third_party/amrex-smoke/build/amrex_smoke` (minimal consumer: `project(x LANGUAGES C CXX)` + `find_package(AMReX REQUIRED CONFIG)` + `AMReX::amrex`) | `AMReX 26.09 : rank 0 of 2 … SMOKE OK: mesh sum 4096, nprocs 2`. Toolchain works. |
| E3 | `/tmp/npcprobe`: real derived `NeighborParticleContainer<0,0,7,1>`, 1000 jittered particles on an 8³ domain, `Redistribute` + `fillNeighbors` + `updateNeighbors` + `buildNeighborList` + own comp integration, 5 steps, at 1/2/3/4 ranks, each tile's neighbour list cross-checked against per-tile N² brute force | `mismatch_count=0` at every rank count; `visits=2606` **identical** at 1/2/3/4 ranks; `sum(pos)=1500.1992120361{29,35,33,29}` (agreement ~1e-15). Rank-invariant, no missed pairs, no duplicates. |
| E4 | same probe, deliberately under-sized mask: `reach=0.2, nneighbor=1` (halo `dx=0.125`) | `mismatch_count=40` (1 rank) / `20` (2) / `10` (4) → 40 missed pairs total, **no warning, no error** (`AMREX_ASSERTIONS_FOUND OFF`). `reach=0.2, nneighbor=2` → `mismatch_count=0`. Also verified `bin_size=0.2` with `nneighbor=1` stays `40` — the `bin_size` overload fixes bin *resolution* but cannot fix a too-small ghost halo. Two independent knobs, one failure mode. |
| E5 | `/tmp/mirror_probe.cpp`: one sphere pair through the real kernels (`sphere.cpp`, `contactpoint.cpp`, `forces.cpp`), compare the serial engine's B-side entry against a regenerated contact with roles swapped | `\|fB_serial − fB_mirror\| = 0.000e+00` (bitwise). `\|fA + fB_serial\| = 1.933e+02` for an oblique contact → **the serial engine's friction already violates Newton's third law per contact** (each side's friction uses its own contact-point velocity). Pre-existing, see §5.1. |
| E6 | `/tmp/mesh_probe.cpp`: sphere vs 2-triangle barrier through `sphereWithBarrierAB` / `sphereWithBarrierBA` | `AB points: 2, BA points: 2`; `\|fAB − fBA_neg\| = 0.000e+00` (role swap + normal negation is exact, as `Engine::addCollision` does), `\|fAB − fBA\| = 8.960e+03` (raw role swap without negating the normal is catastrophically wrong). |
| E7 | `build-amrex/.../flags.make` vs `build/.../flags.make` | `-std=gnu++20` vs `-std=gnu++14` for the same `delta_lib` sources. See §5.3. |

Provenance note: `third_party/` is **not tracked** (`git ls-files third_party` = 0) — the prebuilt AMReX 26.09 install and the smoke project are local convenience artifacts. Tracked tree is clean at `5408cff2` (`git status --short` empty); this recon changed nothing.

Gaps worth knowing: `AMREX_ASSERTIONS_FOUND OFF` (checked in the install's `AMReXConfig*.cmake`), `AMReX_MPI=ON`, `AMReX_PARTICLES=ON`, `DOUBLE`, no GPU/OMP/SIMD/FORTRAN. Source tree `third_party/amrex` is at `a52ca73`, clean, and install matches source (verified by `diff`).

---

## 2. Mapping table

Legend: **STAY** = untouched · **ADAPT** = small wrapper/rename · **DELETE** = goes away · **N/A** = does not exist yet.

### 2.1 Parallel layer

| delta entity (with line) | AMReX 26.09 counterpart | Status |
|---|---|---|
| rank decomposition: Zoltan RCB `zoltan/loba.cpp` (488 LOC) | `BoxArray` + `DistributionMapping` (`SFC`/`RoundRobin`) + `Geometry` | DELETE (not built today) |
| particle migration `zoltan/migration.cpp:~1632 LOC` (`migrate`, 9 MPI tags) | `ParticleContainer::Redistribute()` | DELETE (not built today) |
| ghost particle exchange `migrateGhosts` (`migration.cpp:1020`) | `NeighborParticleContainer::fillNeighbors()` + `updateNeighbors()` | DELETE (not built today) |
| ghost force reduction `migrateForce` (`migration.cpp:1286`) | **not needed** — see §2.3 | DELETE |
| `Engine::binnedCandidates` uniform-cell broad phase, `cellSize = 2*maxHalo` (`Engine.cpp:508`), ±1 cell scan (`Engine.cpp:491-536`) | tile-local bins + `buildNeighborList(check_pair, bin_size)` | ADAPT — 1:1 functional map, see §4 |
| obstacle pairing, `O(#obstacles × N)` halo test (`Engine.cpp:521-548`) | none — keep as-is, obstacles stay delta-side | STAY |
| `bruteForceCandidates` (`Engine.cpp:577`) | none — keep as the test oracle | STAY |
| `OMP parallel for` in `updatePosition` (`Engine.cpp:730`) | `AMREX_OMP` / tile parallel loop (later) | ADAPT (deferred) |
| no MPI at all in `delta/` (CMake has no MPI) | `ParallelDescriptor` + MPI from AMReX | ADAPT (new) |

### 2.2 Particle data

| delta entity | AMReX counterpart | Status |
|---|---|---|
| `ParticleRecord` (`core/data/ParticleRecord.h`, 115 LOC): `_centre`, `_centreOfMass`, `_linearVelocity`, `_angularVelocity`, `_refAngularVelocity`, `_orientation`, `_inertia`, `_inverse`, `_mass`, `_epsilon`, `_diameter`, `_material`, `_isObstacle`, … | `ParticleContainer<0,0,NReal,NInt>` SoA comps; `pos` + `id` are AoS-reserved (`Particle<0,0>`), **user real comps start at index 0 via `ptd.rdata(0)`** (comp layout: `RealCompStart = AMREX_SPACEDIM + NStructReal`, `AMReX_ParticleTile.H`) | ADAPT — keep `ParticleRecord` as the host-side record for I/O/VTK; the container comps are the runtime state |
| global particle id (`getGlobalParticleID()`, `int`) | `p.id()` — `Long`, assigned per rank by `NextID()`, **not** the file's ID | ADAPT — delta's `.mbfcp` IDs are ints from the file; map them explicitly, do not rely on `NextID()` |
| per-particle triangle mesh (`_xCoordinates`, `_refxCoordinates`, `OctTree`) | none (`MeshLibrary`/`shapeId` **does not exist anywhere in the repo** — grep for `shapeId`/`ShapeId`/`MeshLibrary` over `delta/**`: 0 hits) | STAY for spheres (hopper has none); mesh-shaped bodies need either a side table keyed by gid or the plan's Phase-1 refactor — **not required for PR1** |
| `Meta::Collisions{ _copyOfPartnerParticle; _contactPoints }` (`core/data/Meta.h:19-23`) and `_activeCollisions` map | not needed: the partner is read from the ghost half of the same tile | DELETE (PR2) |
| `Engine::addCollision` (`Engine.cpp:187-276`): A-side gets the points, B-side gets copies with `normal` negated | not needed — each rank builds only its own particles' contact lists | DELETE (PR2) |
| `State` counters / `_maxForceMagnitude` (`Engine.cpp:620`) | per-rank; needs `ParallelDescriptor::ReduceMax` for equivalent reporting | ADAPT (PR2) |
| gravity (`Engine.cpp:703`) | stays in the integrate step | STAY |

### 2.3 Per-particle force semantics (the load-bearing finding)

Serial: a contact is detected once, points stored on the A-side, and a *normal-negated copy of the same points* on the B-side (`Engine.cpp:187-276`). Each particle's force is then computed over *its own* contact list with itself in the A role (`Engine.cpp:640-685`, accumulation at `Engine.cpp:667`), and the Sphere model reads only `normal`, `depth`, `x`, `friction` from the contact point (`contact/forces/forces.cpp`, `getContactsForces`).

Probe E5/E6 measured that regenerating a contact with roles swapped yields the **bitwise-identical** force on each particle (`0.000e+00`), both sphere–sphere and sphere–mesh. Consequence: **the parallel engine needs no ghost-force exchange at all.** Each rank detects contacts between its own real particles and the ghosts in the same tile, calls `getContactsForces` with the local particle as A and the partner as B, and integrates its own particles; the result equals serial per-particle physics. The single trap is normal orientation: do not swap roles without negating the normal (`\|Δf\| = 8.96e+03`).

### 2.4 Broad-phase sizing (delta → AMReX)

delta: `cellSize = 2*maxHalo ≥ maxHalo`, ±1 cell scan, exact (`Engine.cpp:508-536`).
Delta's reach: contact requires `\|P−Q\| ≤ epsA+epsB` where `\|P−Q\| = \|(rA+rB) − d\|`, i.e. centre reach `rA+rB+epsA+epsB = (haloA+haloB)/2.2 ≤ maxHalo` (halo = `(d+2ε)*1.1`, `world/structure/Object.cpp:51,137,192,340`). Hopper: r=0.02, ε=0.002 → halo 0.0484, reach 0.044.

AMReX equivalent: `nneighbor = max(1, ceil(reach/dx))` cells of ghost halo (`AMReX_NeighborParticlesCPUImpl.H:636-637` appends the halo into the same tile), plus bins with `bin_size ≥ reach` (`AMReX_NeighborParticlesI.H:988` "choose bin_size >= interaction radius for completeness"). Recommended setting: **`dx ≈ reach` (i.e. `nx = ceil(L/reach)` per axis) and `nneighbor = 1`** — then the default `buildNeighborList(check_pair, reach)` overload is complete (mesh-cell bins = `dx`), one knob, exactly one cell of ghost halo. Both `nneighbor*dx < reach` and `bin_size < reach` fail *silently* (E4).

---

## 3. The three lists

### STAY (untouched by the port)
| Path | LOC | Why |
|---|---|---|
| `delta/contact/**` (`detection/sphere.cpp`, `contactpoint.*`, `forces/forces.cpp`, …) | — | raw `const iREAL*` signatures; `ptd.rdata(k)` supplies compatible pointers |
| `delta/dynamics/**` (`updateAngular`, `updateRotationMatrix`, `updateVertices`) | — | pure per-particle math |
| `delta/core/data/ParticleRecord.*`, `Structure.*`, `OctTree.*` | 1169 | host-side state/IO, still used to read `.mbfcp` and to write VTK |
| `delta/core/io/read.cpp` (`readmbfcp`, line 346) | — | parsing is orthogonal to parallelism |
| `delta/core/Engine.cpp:491-536, 577` (candidate generation) | ~90 | becomes the *serial* path and the test oracle; keep both |
| `delta/core/Engine.cpp:521-548` (obstacles) | 28 | see §6 |
| `tests/dem_phase0_test.cpp` | — | **the** oracle; extended, not replaced |
| `zoltan/` (9,377 LOC hand-written + ~11 kLOC `ext/hul`), `multiscale/` (135,106 LOC / 234 files, a tracked older delta fork) | 155,522 | neither is reachable from `delta/CMakeLists.txt` (glob is `delta/**`; no root `CMakeLists.txt`; `multiscale` has its own `Makefile`). Deleting them is repo hygiene, not integration work — separate PR, and `multiscale/` may still be wanted. |

### ADAPT
| Path | Change | Size |
|---|---|---|
| `delta/CMakeLists.txt:2` | `project(delta_dem LANGUAGES C CXX)` — **C must stay enabled** (`AMReXConfig.cmake:213 find_dependency(MPI COMPONENTS C CXX)`; a CXX-only project fails with `MPI component 'C' was requested, but language C is not enabled`) | 0 |
| `delta/CMakeLists.txt:32-36` | already correct: `find_package(AMReX REQUIRED CONFIG)`, `AMReX::amrex`, `DELTA_USE_AMREX` define. Add the new engine sources (auto-globbed) + new test + `CMAKE_CXX_STANDARD` note (§5.3) | +12 |
| `delta/main.cpp` | `amrex::Initialize/Finalize` guard + optional AMReX engine selection | +25 (PR2) |
| `delta/core/Engine.{h,cpp}` | untouched in PR1; in PR2/PR3 keep detection/forces/integration and replace storage | +0 (PR4 deletes) |
| VTK output (`io/write.cpp`) | gather to rank 0 (or per-rank files) before writing | PR3 |
| `State` diagnostics | `ReduceMax/ReduceIntSum` | PR2, +20 |

### DELETE (when the AMReX path is the only path)
| Path | LOC | Note |
|---|---|---|
| `_activeCollisions` + `Collisions` + `Engine::addCollision` + `Engine::getContactPairs` | ~150 in `Engine.cpp` (`187-276`, `599-611`, `620-693`) + `Meta.h:19-23` | replaced by the per-tile loop |
| `zoltan/**` | 9,377 hand-written (+~11 kLOC vendored `ext/hul`) | already unbuilt |
| `AMREX_INTEGRATION_PLAN.md` claims (below) | — | fix or delete stale text |
| `OctTree` per sphere particle | ~600 | spheres don't need it; mesh path does |

---

## 4. First PR plan (CPU + MPI, sphere model, no GPU)

**Scope rule for PR1: add the parallel path next to the serial one and prove equivalence. Touch nothing that the serial build uses (except CMake and one new test target).** The current serial path stays bit-identical, so PR1 cannot regress anything.

### New files
| File | Purpose | Est. LOC |
|---|---|---|
| `delta/core/parallel/DemParticleContainer.h` | derived `NeighborParticleContainer<0,0,45,3>` exposing `m_neighbor_list`/`neighbor_list` (they are `protected`, `AMReX_NeighborParticles.H:402`) + the comp-index constants + a `static_assert` on the sizing rule | 70 |
| `delta/core/parallel/DemAmrexEngine.h` | class: `Geometry`/`BoxArray`/`DistributionMapping`, container, gid↔index map, `initFrom(const std::vector<ParticleRecord>&, const Scenario&)`, `step(dt, gravity)`, `syncTo(std::vector<ParticleRecord>&)` | 90 |
| `delta/core/parallel/DemAmrexEngine.cpp` | seed on rank 0 → `Redistribute()` → per step: `fillNeighbors(); updateNeighbors(); buildNeighborList(CheckPair, reach)`; per tile per real particle: local bin/list loop, sphere vs sphere/obstacle kernels, force/torque accumulation, gravity + integration from `dynamics/*`; `clearNeighbors(); Redistribute();` | 420 |
| `tests/dem_phase1_amrex_test.cpp` | `amrex::Initialize` oracle test, see acceptance below | 260 |
| CMake | new test + `DELTA_USE_AMREX` wiring + standard | +15 |
| **Total** | | **~855 LOC new, 0 deletions, 0 edits to serial sources** |

Comp budget (reals): `linVel(3) angVel(3) refAngVel(3) force(3) torque(3) orient(9) inverse(9) inertia(9) mass(1) eps(1) diameter(1)` = 45; ints: `material(1) friction(1) shapeOrObstacle(1)` = 3. ≈400 B/particle incl. AoS → ~6.3 MB for the 15,625-body hopper, versus today's `ParticleRecord` which carries an `OctTree` object *and* three coordinate `std::vector`s per body.

### Exact CMake wiring (delta/CMakeLists.txt)
```cmake
cmake_minimum_required(VERSION 3.20)
project(delta_dem LANGUAGES C CXX)          # C is mandatory: AMReX's MPI dep needs MPI_C
set(CMAKE_CXX_STANDARD 14)                   # keep for the serial build
option(DELTA_USE_AMREX "Build the AMReX-backed target (Phase 2+)" OFF)
...
if(DELTA_USE_AMREX)
  find_package(AMReX REQUIRED CONFIG)        # -DAMReX_DIR=<prefix>/lib/cmake/AMReX
  set(CMAKE_CXX_STANDARD 20)                 # AMReXTargets.cmake:63 INTERFACE_COMPILE_FEATURES cxx_std_20
  target_link_libraries(delta_lib PUBLIC AMReX::amrex)
  target_compile_definitions(delta_lib PUBLIC DELTA_USE_AMREX)
  add_executable(delta_dem_amrex_test ${CMAKE_CURRENT_SOURCE_DIR}/../tests/dem_phase1_amrex_test.cpp)
  target_link_libraries(delta_dem_amrex_test PRIVATE delta_lib)
  if(MPI_CXX_FOUND)                          # exported by AMReXConfig's find_dependency(MPI)
    add_test(NAME dem_phase1_amrex COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2
             ${MPIEXEC_PREFLAGS} $<TARGET_FILE:delta_dem_amrex_test>)
  endif()
endif()
```
Configure: `cmake -S delta -B build-amrex -DDELTA_USE_AMREX=ON -DAMReX_DIR=$PWD/third_party/amrex-install/lib/cmake/AMReX -DCMAKE_BUILD_TYPE=Release`. Running as root needs `--allow-run-as-root` (in `MPIEXEC_PREFLAGS` or by hand).

### Acceptance tests for PR1 (all runnable with what exists today)
1. **Pair-set equality (per rank, per tile).** AMReX neighbour list == brute-force pairs within `reach` for the hopper config. This is `dem_phase0`'s existing check, mirrored (E3: `mismatch_count=0`).
2. **Force equality vs serial, single rank, 1 step.** Load the same N=100 particles into `Engine` (serial) and into `DemAmrexEngine`, run one detection+force pass, compare per-particle force/torque. Tolerance: `≤1e-12` relative (E5/E6 give bitwise on the kernel; the container path may reorder the per-particle contact sum).
3. **Rank invariance.** Same run at 1 and 2 ranks: total particle count equal, summed position/velocity/angular momentum equal to `~1e-12` (E3: 1e-15), contact-pair count equal. This is the actual "no missed pairs, no duplicates" test for MPI.
4. **Trajectory sanity.** 20 steps of the 1000-sphere pile (`dem_phase0`'s config) at 2 ranks: finite state, contact count and `max|force|` within a documented tolerance of the serial run (expect divergence in the last bits only; a pile is chaotic — see §8 R5).
5. **Sizing assertion.** `static_assert`/runtime check that `nneighbor*dx >= reach` and `bin_size >= reach`; deliberately under-size a copy of the config in the test and assert delta's own code *catches* it (E4 shows AMReX will not).

Keep `ctest -R dem_phase0` green as the PR1 exit criterion alongside the above.

---

## 5. Behaviour changes that must be flagged (not bugs, but visible)

1. **Friction was never third-law symmetric** (`\|fA + fB_serial\| = 1.933e+02`, E5). A port that computes one pair force and applies ±F — a natural "optimisation" — changes trajectories. The port must keep per-side evaluation; the existing `dem_phase0` "third law" check (`tests/dem_phase0_test.cpp:225-265`) passes only because it is two spheres starting at rest with zero angular velocity on the x-axis — no tangential relative motion, so friction is identically zero and it will not catch this regression.
2. **Deep overlap produces no contact** (the narrow `\|(rA+rB) − d\| ≤ εA+εB` band). A different broad phase that hands the kernels overlapping pairs won't create spurious contacts, but a *too-tight* broad phase silently loses real ones — asymmetric failure, worth a test.
3. **C++ standard propagates.** With `DELTA_USE_AMREX=ON`, all of `delta_lib` compiles as C++20 (verified: `-std=gnu++20` in `build-amrex`, `-std=gnu++14` in `build`), including the VTK I/O translation units. It currently builds, but the serial build and the AMReX build differ in language version, so a future `std::` change can break one and not the other. Set the standard explicitly (as above) instead of relying on CMake's silent upgrade.
4. **`DELTA_USE_AMREX` currently has no `#ifdef` in any source** — it is a define with no consumer. PR1 gives it its first one.
5. **Diagnostics become per-rank**: `_maxForceMagnitude`, `State` contact counters, `getContactPairs()`. Without reductions, `main.cpp`'s printed stats change meaning silently.
6. **Particle ID semantics change**: AMReX `id()` is rank-assigned (`NextID()`), delta's gid comes from the input file. Keep an explicit map; a naive `id()`-as-gid swap renumbers the output.

---

## 6. Plain cell list vs `NeighborParticleContainer`

*Particle–particle:* **NPC.** The lazy check: what does a hand-rolled cell list over AMReX particles still need? Packing each particle into a grid cell *across ranks* — i.e. the ghost exchange, which is the 600-line hard part of `zoltan/migration.cpp` (`migrateGhosts` at line 1020 plus the 9-tag machinery around it). NPC's `fillNeighbors()` is that, done. The glue is small: the probe's entire working container was ~100 lines, and the measured behaviour (E3) is rank-invariant at 1–4 ranks. delta's existing cell logic also maps onto it trivially, because delta already uses a single global `cellSize = 2*maxHalo` (`Engine.cpp:508`) — one bin size, one `nneighbor`, no per-shape refinement needed.

*Particle–obstacle:* **plain loop, not NPC.** `Engine.cpp:521-548` already does this correctly and cheaply: 26 static walls × N particles with a sphere-halo precheck (400 k distance tests for the hopper, ~ms). Replicating walls into the particles container would require the cross-container `buildNeighborList(check_pair, other, lists)` path, which asserts `SameIteratorsOK(*this, other)` (`AMReX_NeighborParticlesI.H:1024-1025`) — i.e. the obstacle container must share the receiver's `BoxArray`/`DistributionMapping` and be replicated per grid — plus duplicate-`id()` handling. Obstacles carry `_inverse = 0` (fixed), so their own forces are discarded anyway (`Engine.cpp:679`: obstacles skip the velocity update), and the kernels already have the exact entry points for a sphere in the A role vs a local mesh in the B role: `sphereWithBarrierAB(local sphere, obstacle mesh)` (`contact/detection/sphere.cpp:101`, `Engine.cpp` dispatch). Keep them outside; when worlds get many/large obstacles, revisit `fillNeighbors(search_radius)` for the particle side rather than containerising the walls.

Caveat on the radius variant: `fillNeighbors(Real search_radius)` exists but is documented CPU-only (`AMReX_NeighborParticlesI.H:149-177`, `areMasksValid` recomputes `required = ceil(search_radius/dx_min)`). Irrelevant for PR1, a blocker if the GPU phase later wants a tight halo.

---

## 7. Delivered answer to "is the first PR small enough?"

Yes — ~855 new LOC, 0 deletions, 0 edits to serial physics, one new test target. The two things that could silently break correctness are both *configuration*, not code: the ghost/bin sizing rule (E4) and the domain (R1). Both are checkable with assertions delta owns.

---

## 8. Risks

**R1 (P0) — delta has no domain; AMReX punishes particles outside it.** `locateParticle` (`AMReX_ParticleContainerI.H:447-475`): outside the roundoff domain, non-periodic, `lev_min==0` → `p.id().make_invalid()`, `success=true` → **the particle is silently dropped**; if `Where()` still fails → `amrex::Abort("ParticleContainer::locateParticle(): invalid particle.")` (observed in the probe when particles were accidentally seeded twice). delta's `World` has no domain concept, so the engine must fabricate one from the scenario bbox + margin and decide a policy for escapes (kill, clamp, or periodic). Also `BinMapper` (`AMReX_ParticleUtil.H:285-297`) clamps `amrex::min(nx-1, amrex::max(0, iv3.x))`, so an out-of-box particle is binned into an edge cell under assertions-off builds. Mitigation: domain from bbox + `>= 2*maxHalo` margin, a count check every step, and a documented escape policy.

**R2 (P0) — silently incomplete neighbour lists.** Both under-sizing modes (`nneighbor*dx < reach`, `bin_size < reach`) are `AMREX_ASSERT`-only (`AMReX_NeighborParticlesI.H:820,988`) and assertions are off. Measured 40 missing pairs with no diagnostic (E4). Mitigation: delta owns a `static_assert`-style runtime check + the pair-set oracle in test 1.

**R3 (P1) — `Redistribute()` cadence.** Particles must stay within `nneighbor` cells of their own grid for `fillNeighbors` to see their partners. One AMReX step does not guarantee that for a DEM step (velocities are O(1), dt unknown), so `clearNeighbors(); Redistribute();` **every step** is the safe default. delta's zoltan path also migrated every step, so the cost profile is not new; amortising to every k steps requires bounding displacement by `nneighbor*dx − reach`.

**R4 (P1) — seeding discipline.** `NextID()` is per-rank, and seeding from all ranks duplicates particles: the probe showed 4.77×/11.3× inflated visit counts plus the `invalid particle` abort. Seed on rank 0 only, then `Redistribute()`; assert `numParticles() == expected` per rank. Also: `ptile.resize(n)` (not `aos.resize`), write through `getParticleTileData()` (not `getConstParticleTileData()`), and **`p.rdata(0)` on an AoS particle is a different namespace from `ptd.rdata(0)`** — both compile.

**R5 (P1) — parity will not be bitwise over long runs.** Per-particle force is a sum over that particle's contacts; parallel detection can order contacts differently, and the mirrored regeneration differs in the last bits. Expect per-step agreement ~1e-15 and chaotic pile divergence later. Compare with tolerances (as the plan doc already says) and use the pair-set/count invariants for the strong claims.

**R6 (P1) — obstacle counts and `SameIteratorsOK`** if anyone tries containerising obstacles anyway. See §6. Also 26 obstacle walls have 12–thousands of triangles each; the existing per-particle mesh arrays stay.

**R7 (P2) — tile/grid granularity is a new tuning knob** (`max_grid_size`). It changes both ghost volume and load balance, and interacts with `nneighbor`: with `nneighbor=1` and a hopper-scale mesh, a 2×2×2 grid split costs ~2.4× ghost volume; a single grid costs no ghosts but no parallelism. Measure, don't guess.

**R8 (P2) — cell-based halo inflates the ghost set** vs a radius search (27 cells per cell of interest at `nneighbor=1`). With `dx ≈ reach` the halo is one cell wide, so the inflation is bounded; `fillNeighbors(search_radius)` is the escape hatch (CPU only).

**R9 (P2) — memory/comp creep**: 45 reals + 3 ints per particle today's budget; the plan doc's `MeshLibrary` phase would let inertia/inverse live per *shape* instead. Fine at 15 k bodies, a decision point at 1 M.

**R10 (P2) — output path.** VTK writing currently assumes one process owns all particles (`io/write.*`, VTK is a hard `REQUIRED` dependency of `delta_lib`, `delta/CMakeLists.txt:12`). Under MPI the writer needs a gather or an AMReX plotfile; PR1 deliberately does not touch it: the test pulls state back with `syncTo(std::vector<ParticleRecord>&)` and compares numerically instead.

---

## 9. Corrections to `AMREX_INTEGRATION_PLAN.md`

| Claim in the doc | Reality |
|---|---|
| §1 "`delta/` … O(N^2) contact detection"; §3.3 "Neighbor list instead of O(N^2)" | False since `aac362b3`: `Engine::binnedCandidates` (`Engine.cpp:491`) is a uniform-cell broad phase (`cellSize = 2*maxHalo`, ±1 cell, documented ponytail ceiling) with `bruteForceCandidates` (`Engine.cpp:577`) kept only as the test oracle. The AMReX change is *ghost halo + distributed bins*, not "replace O(N²)". |
| §2/§4 `for_each_pair`, §2 "`fillGhosts` / `RealCommunication`" | None of these exist in AMReX 26.09 (grep over all installed headers: 0 hits). Real API: `fillNeighbors/updateNeighbors/buildNeighborList` + per-tile `NeighborList` (`GetCounts`/`GetList`), accessed from a derived container because the members are `protected` (`AMReX_NeighborParticles.H:402`). |
| §5 "hand-rolled `Makefile` + `files.mk`", "Keep the existing `Makefile` working until Phase 3" | There is no Makefile in `delta/`; the tree is CMake (`delta/CMakeLists.txt`), and both `build/` (serial) and `build-amrex/` already compile and run. `zoltan/Makefile` exists but nothing builds it. |
| §5 snippet `project(delta_dem CXX)` | Fails. `AMReXConfig.cmake:213` does `find_dependency(MPI COMPONENTS C CXX)`: a CXX-only project errors with `MPI component 'C' was requested, but language C is not enabled`. The repo already has it right (`project(delta_dem LANGUAGES C CXX)`). |
| §4 Phase 0 "add a headless build target that excludes VTK/Assimp" | `delta_dem_test` + `add_test(dem_phase0)` already exist and pass (E1) — but VTK is still `REQUIRED` for `delta_lib`, so "headless" is not yet true. Phase 0's oracle requirement is **met**; only the VTK split is outstanding. |
| §3.1 "`ParticleRecord` currently stores `_xCoordinates` … per particle" / `MeshLibrary` as a prerequisite | True only for mesh-shaped bodies. The live scenario (`main.cpp:112`, `tests/dem_phase0_test.cpp:112`: `CollisionModel::Sphere`) has 15,625 spheres with no mesh and 26 obstacle walls. `MeshLibrary`/`shapeId` do not exist anywhere (0 grep hits), and no PR1 work depends on them. |
| §3.3 obstacles "replicate obstacles into a static AMReX particle tile on every rank and test particles against that tile" | Heavier than necessary: the cross-container path asserts `SameIteratorsOK` (`AMReX_NeighborParticlesI.H:1024-1025`), requiring shared `BoxArray`/`DistributionMapping` and per-grid replication. `Engine.cpp:521-548` already does the equivalent locally in 28 lines; keep it. |
| §3.4 / §4 Phase 3 "accumulate force/torque directly in the pair loop … delete `_activeCollisions`" | Agreed, and E5/E6 prove the accumulation is exact if the local particle stays in the A role and the normal is negated on a swap. Add: friction is per-side, so "one pair force, applied ±F" is a behaviour change (§5.1). |
| §6 "Nondeterministic force summation … breaks bitwise parity" | Correct as a risk, and now quantified: 1e-15 per step across 1–4 ranks (E3), chaotic divergence later (R5). |
| §1 "`multiscale/` - Peano-coupled grid variant" | True (135,106 LOC, tracked) but it is an entire older delta fork with its own `delta/`, `dem/`, `compute/` and `Makefile`, unreachable from the only CMake entry point. Whether it survives is a repo-hygiene decision, independent of the AMReX port. |
| §7 open questions 2/3 | Answerable from the repo: backend is CPU+MPI (bundled install is MPI-only, no GPU/OMP/SIMD), `zoltan/` is not in the build at all, so "delete vs keep for benchmarking" is a code-organisation question with no build impact. |
| §9.1 "`write.cpp:61-62` `SetScalars(radius)` then `SetScalars(epsilon)` …" | Not verified in this pass (out of scope for the parallel-layer question). |
| §9.1 "`NumberOfCells="0"`" etc. | Not verified in this pass. |

---

## 10. Not verified / open decisions

1. **The scenario domain**. The hopper bbox was inferred from the input file (25³ lattice at 0.04 spacing, r = 0.02, 26 obstacle walls), not from a run. The engine must compute `Geometry` from the actual loaded particles + obstacle extents; the sizing formula above uses `reach = maxHalo = 0.0484`, `nx = ceil(L/reach)`.
2. **Which collision models PR1/PR2 must cover.** `CollisionModel::Sphere` is the only one instantiated (`main.cpp:112`, the test, and hopper input). The mesh-coupled models (`bf`, `penalty`, `hybrid`, GJK, AST) need the per-particle triangle arrays in the container's neighbourhood — a genuinely larger change (the plan doc's `MeshLibrary` phase). Recommend: PR1 = Sphere + sphere-vs-obstacle-mesh only; confirm.
3. **Obstacle escape policy** (R1): kill / clamp / periodic is a product decision.
4. **Does `main.cpp` keep the serial engine as a default** (with `--amrex` opt-in) or switch over at PR2? Opt-in until the oracle tests pass at ≥2 ranks.
5. `State`/diagnostics reduction policy (R5/§5.5) — reduction vs per-rank logging.
