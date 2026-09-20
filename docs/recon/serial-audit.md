# Serial / global-state audit — delta DEM core (AMReX domain-decomposition readiness)

- Repo: `/workspace/delta`, branch `amrex-integration`, HEAD `5408cff2` ("io: read the real hopper (mass, gravity, obstacles) and stabilize it").
- Mode: **read-only** audit. No repository file was modified, no commit was made. Scratch probes live in `/tmp` only.
- Paths below are relative to `/workspace/delta` unless absolute. `delta/` is the library root (CMake globs `delta/**/*.cpp` only; `multiscale/` and `zoltan/` are **not** in the build — greps can surface same-named symbols from `multiscale/dem/`, ignore those).
- Reference for the target architecture: `AMREX_INTEGRATION_PLAN.md` (ParticleContainer/NeighborParticleContainer + shared MeshLibrary + Redistribute; delta contact kernels stay).

> **Status update.** §3 bugs L2, L3, L4, L5, L6, L7, L8 and L10 are fixed in
> `delta/`, each guarded by `tests/regression_test.cpp` (`add_test regression`).
> The hopper header now prints `# Triangles : 208` (was 0) and the per-step log
> prints real contact counts (was `cpt:0`); resolved pair counts are unchanged.
> L1 (`expmap()`) is **not** fixed: it changes baseline trajectories, per the
> order-of-attack note below. L9 (`OctTree` leak) and L11 (dead code) remain.

## Evidence gathered (executed, not inferred)

| # | command | result |
|---|---------|--------|
| E1 | `cd build && ./delta_dem_test` | `dem_phase0_test OK: 1000 particles, 20 steps, max contacts=2348`; serial baseline green. |
| E2 | `cd build && ./delta` (real `input/hopper.mbfcp`) | `loaded particles=15651 spheres=15625 obstacles=26` then `# Triangles : 0`; 10 steps, `contacts=44500`, `finite=yes`. |
| E3 | `python3 -c ... subprocess ./delta ... resource.getrusage` | peak RSS **484 MB**, wall **13.7 s** for 15651 bodies / 10 steps serial. |
| E4 | `/tmp/expmap_check` + python re-derivation of `expmap()` | as-is `max|R R^T - I| = 6.951e-01`, `det = 1.3524`; with `v02 = O1*O3` `2.2e-16`, `det = 1.0`. Axis-aligned Ω unaffected. |
| E5 | `/tmp/mpi_audit_probe` (links `build/libdelta_lib.a`, unchanged library) | `modelScheme=0 -> 54 pairs; =3 -> 0; =9 -> 0; =8 -> 0` (no error, exit 0). After 1000 `iterate()` with ω=(1,2,3), `resolveContacts=false`: `max|R R^T - I| = 1.680e-03`. `resolved pairs=144  State.getCollisions()=0  Engine.getNumberOfCollisions()=64`. |

---

## 1. Serial / global assumptions that block MPI domain decomposition

| file | function | line | what breaks under MPI | smallest proposed change | callers |
|---|---|---|---|---|---|
| `delta/core/data/Structure.h` | `_particles` (the only store) | 52 | one flat `std::vector<ParticleRecord>` holds the *whole* scene on every rank; no owner rank, no tile/SoA split, nothing to hand to a particle container | keep the vector but make it the per-rank **local + ghost** set and add owner/tile accessors (`getParticle(i)` becomes `(tile,index)`); the plan's ParticleContainer replaces it at the boundary | `Structure.cpp:26-40,48-65,73-101`; `Engine.cpp:54,75-82,294-295,499-500,515-516,525-526,550,555` |
| `delta/core/data/Structure.h` | `getAll()` returns mutable ref | `Structure.cpp:103-106` | hands out a writable reference to every particle on every rank, so any rank may mutate any body — no ownership invariant to enforce during Redistribute | make it `const&`/view-returning and route writers through ownership-aware accessors | `Engine.cpp:743` (`getParticleRecords`), `Engine.cpp:750` (`plot`), `tests/viz_write_test.cpp:46`; `Engine.cpp:751` (commented out) |
| `delta/core/data/Structure.cpp` | `Structure(objects)` ctor | 43-66 | builds one `ParticleRecord` per body for the entire scene **per rank** (all mesh vertex vectors copied), no partitioning, no "which rank owns this" | take a per-rank object range (or build locally from a distributed IO/Redistribute) | `Engine.cpp:81` |
| `delta/core/data/Structure.cpp` | `Structure(objects, refine, enforce)` ctor | 18-41 | same as above; also accumulates `_numberOfTriangles` globally | same | `Engine.cpp:75-79` |
| `delta/core/Engine.h` | `_data` member | 127 | `Engine` owns a full `Structure` **value**, so a second complete copy of the global scene per rank | hold a reference/handle; let the container own storage | `Engine.cpp:54,75-82` |
| `delta/core/Engine.cpp` | ctor #2 | 58-59 | takes `std::vector<Object> particles` **by value** → deep copy of the whole scene on construction | take `const&` (or view) | `delta/main.cpp:125`; `tests/dem_phase0_test.cpp:99,153,194,251` |
| `delta/world/World.cpp` | `getObjects()` | 151-154 | returns the particle vector by value (another full copy) | return `const&` | `Engine.cpp:54` |
| `delta/core/data/Meta.h` | `Collisions::_copyOfPartnerParticle` | 19-23 | **per-contact deep copy of the partner**: 6 vertex vectors + OctTree handle + inertia + all state. Two inserts per pair (`Engine.cpp:243-246`), one per direction. Under MPI this snapshot is a ghost frozen at detection time: the force pass then integrates against a stale position, and each rank that sees the pair holds its own divergent copy | store `(int partnerGlobalId, int ownerRank)` (or a neighbour index) and look the live record up in `deriveForces()` | `Engine.cpp:243,245,265,275,655-663`, `Engine.cpp:605-608` |
| `delta/core/Engine.cpp` | `binnedCandidates` — `maxHalo` | 496-502 | halo is reduced over the **local** set only; under MPI the cell size (and the ghost width AMReX needs) must be one globally agreed value | `MPI_Allreduce(MPI_MAX, &maxHalo, 1, …, MPI_COMM_WORLD)` (or pin it in `Meta`/Geometry) | `Engine.cpp:586` |
| `delta/core/Engine.cpp` | `binnedCandidates` — cell size | 504-509 | single global cell size `2*maxHalo` derived from the local max halo: (a) ranks can disagree on cell size and therefore on candidates; (b) one outsized body coarsens every cell toward O(N²) (already flagged with a `ponytail:` note); (c) equals the required ghost width — the 3×3×3 scan only finds pairs with `|Δc|∞ ≤ cellSize`, so the AMReX ghost width must be ≥ `maxHalo` of the *global* set | after the Allreduce, use the global `maxHalo` both as cell size and as the minimum ghost width | `Engine.cpp:586` |
| `delta/core/Engine.cpp` | `binnedCandidates` — `std::map<std::array<int,3>,…> cells` | 511-521 | one global hash of the whole domain, rebuilt from scratch every step; under MPI it must be a per-rank tile index populated from local + ghost particles | per-rank bin index; AMReX `NeighborParticleContainer` supplies the halo | `Engine.cpp:586` |
| `delta/core/Engine.cpp` | `binnedCandidates` — bin key cast | 517-519, 527-529 | `(int)std::floor(c[k]/cellSize)` on unvalidated coordinates: a non-finite `_centre` (which `contactpoint.cpp:14-27`, `sphere.cpp:42`, `main.cpp:145-153` all defensively guard against) makes `std::floor(NaN)` and the int cast UB; out-of-range values give garbage keys, and the `bz±1` overflow can wrap | skip/clamp non-finite `_centre` before binning (`std::isfinite`) | `Engine.cpp:586` |
| `delta/core/Engine.cpp` | `binnedCandidates` — obstacle loop | 546-565 | obstacles are excluded from the cell bins and then paired with **every** local particle in a global `n_obst × n_part` double loop; correct only if all obstacles exist locally (replicated) and all particles are local. If obstacles are owner-only, every wall contact is lost; if replicated, cost is N_ranks × (26 × local N) per step | replicate obstacles read-only per rank (plan §3.3) and loop over the **local** free particles only; resolve wall contacts on the particle's owner | `Engine.cpp:586` |
| `delta/core/Engine.cpp` | `contactDetection` → `binnedCandidates` → `evaluateCandidates` | 583-588 | no halo/neighbour exchange anywhere: pairs straddling a rank boundary are never enumerated | put the ghost layer (width from the reduced `maxHalo`) in front of the two passes | `Engine.cpp:179,591` |
| `delta/core/Engine.h` | `_activeCollisions`, `_collisionsOfNextTraversal` | 133-134 | two `std::map<int, std::vector<Collisions>>` keyed by **global particle id**, holding full particle copies for the active contact set. Each rank's map is a private view of the pair set; nothing decides which rank resolves a shared pair, so a pair detected on both ranks applies the force twice, and a pair detected on neither loses it | neighbour-list keyed by `(tile,index)`; resolve each pair on exactly one rank (owner rule), then reduce force/torque to the owner | `Engine.cpp:195,200,212,221,243,246,250,251,291,472-473,602-611,631,637-638` |
| `delta/core/Engine.cpp` | `evaluateCandidates` — swap | 472-473 | the swap promotes a whole new generation and keeps the previous generation alive until `clear()` on the next call, so two full copies of the contact set (with per-contact mesh copies) are briefly resident per rank | clear/`std::swap` on raw buffers, or keep one neighbour list and rebuild it in place | `Engine.cpp:179` |
| `delta/core/Engine.cpp` | `addCollision` | 187-276 | keys on `getGlobalParticleID()` only — no rank component, so a ghost and its owner are indistinguishable as map keys; the "already exists" probes at 212-228 scan the vector linearly per contact | extend the key with owner rank, or index by neighbour-list slot (`particleNeighborList()`) | `Engine.cpp:464` |
| `delta/core/Engine.cpp` | `deriveForces` — accumulation | 626-673 | `force[3]/torque[3]` accumulate from `_activeCollisions[gid]` and are applied immediately to `_linearVelocity` (681-683) and `updateAngular` (685-691) on the **local** record. No reduce-to-owner, no ownership check: ghost contributions and duplicate detections both corrupt the update, and the summation order follows candidate order (decomposition-dependent FP result, plan §6) | accumulate per-pair force into an SoA buffer, `Redistribute`+sum into the owner, apply once after the reduction | `Engine.cpp:180` |
| `delta/core/Engine.cpp` | `deriveForces` — obstacle branch | 679-692 | the update is skipped for obstacles via `getIsObstacle()`; a rank must know the obstacle flag of every ghost/partner it tests (currently true only because every rank holds the whole scene) | carry the flag in the ghost/neighbour data | `Engine.cpp:180` |
| `delta/core/data/ParticleRecord.h` | `_bbox`, `_tree` | 109, 111 (set at `ParticleRecord.cpp:35-38,70-72,91`) | both are **construction-time caches**, refined only from `Structure.cpp:31,54`. `updatePosition` (Engine.cpp:696-738) moves `_centre/_centreOfMass/_orientation` and rewrites the world-space vertices in place but never refreshes `_bbox`/`_tree`, so the tree domain is stale for every moving mesh. Under MPI a stale per-rank bbox also poisons any ghost-width/extent computation derived from it | reuse the octree only if refreshed (`_bbox` recomputed in `updatePosition`) — otherwise drop `_tree` from `ParticleRecord`, its only live consumer (`hyperContacts`) has no caller | `Engine.cpp:110,118`, `Structure.cpp:98`, `ParticleRecord.cpp:187,213,236` (all three `getClosest*CoordinatesTriangles` are dead) |
| `delta/core/data/ParticleRecord.h` | `_xCoordinates` etc. | 78-83 | the mesh lives once, mutated in place by `updateVertices` (`Engine.cpp:724-737`) on whichever rank owns the record; a ghost copy must be refreshed every step or the contact kernel runs on stale geometry | refresh ghost comps each step (AMReX does this for the intended container), or run the kernels on the owner only | `Engine.cpp:332-334,352-354,382-384,390-392,402-404,410-412,422-424,430-432,442-444,450-452,467,727-729` |
| `delta/world/structure/Object.cpp` | `_localParticleID` | 63, 91, 168 | assigned `0` in all three ctors and never updated, so `ParticleRecord::getLocalParticleID()` (`ParticleRecord.cpp:107`) is always 0 for every body — there is no rank-local index anywhere in the tree | either delete the field or set it when a body becomes local (the AMReX `(tile,index)` pair is that value) | none (no callers anywhere in `delta/`, `tests/`, `zoltan/`) |
| `delta/core/io/read.cpp` | `readmbfcp` | 359-597 (ctr at 534, 543+) | every rank opens and parses the full 188k-line file and builds the identical scene: `Δ = 0` failure detection, N× the parse cost, N× the memory (E2/E3: 484 MB, 13.7 s serial) | parse on rank 0 + broadcast, or read the already-distributed form; at minimum split by ID range | `delta/main.cpp:62` |
| `delta/core/io/write.cpp` | `writeGeometryToVTKVTK` | 64-153 (filename at 69) | fixed `<path>geometry_<step>.vtu` written by every rank → clobbering, no rank in the name, and the `global_id` array (87-89, 133) is the only rank-identifying output | rank-tagged filenames (`geometry_<step>_<rank>.vtu`) or a single gather-then-write on rank 0 | `Engine.cpp:750` |
| `delta/core/io/write.cpp` | `writeGeometryPVD` | 27-58 | `glob()`s the shared output directory and rewrites `geometry.pvd`; concurrent ranks race on both the read and the rewrite | single collector pass (rank 0, after a barrier) over rank-tagged files | `Engine.cpp:750` (indirect) |
| `delta/core/Engine.cpp` | `plot` | 746-753 (call at 176) | hard-coded `"../output/"`; called at the **top** of `iterate()` so it writes the pre-step state; per-rank, same target path | pass the output root in and gate the write on a rank-owned/gathered buffer | `Engine.cpp:176` |
| `delta/core/State.h` | `_noOfParticles`, `_noOfObstacles`, `_numberOfCollisions` | `State.cpp:38-39,44` | all counts are per-rank local values, and the log (`State.cpp:53-57`, `Log.cpp:72`) prints them un-reduced; they read as global totals to anyone parsing the log | reduce the reported counters (or print them as `mine/total`) | `State.cpp:55-56,134`, `Log.cpp:72` |
| `delta/core/State.cpp` | comparison counters | `State.cpp:96-104`, incremented at `Engine.cpp:467-468` | per-rank sums, never reduced and never printed; meaningless as a global work metric | `MPI_Reduce`/`Allreduce` at report time | `Engine.cpp:467-468` |

---

## 2. Reusable unchanged under MPI

Kernels that touch only their arguments (no member state, no globals, no I/O). These are the pieces the AMReX integration should keep verbatim; give them `const iREAL*` tile views and they are already domain-decomposed.

| kernel | signature source | why it is safe |
|---|---|---|
| `delta::contact::detection::bf` (2 overloads) | `delta/contact/detection/bf.h:46-62`, `:65-81` | `const iREAL*` x/y/z + counts + epsilon/friction/particleID; pure |
| `delta::contact::detection::bfSolver` | `delta/contact/detection/bf.h:95` | same shape; pure |
| `delta::contact::detection::penalty`, `penaltyStat` | `delta/contact/detection/penalty.h:62,80,99` | `const iREAL*` + scalars; pure result vector (the two file-static statistics vectors, `penalty.cpp:29-30`, are per-rank reporting state — see §3/L10) |
| `delta::contact::detection::penaltySolver` | `penalty.h:141` (Peano `extern` twin at `:120`) | `const iREAL*`, pure |
| `delta::contact::detection::hybridWithPerTriangleFallBack` | `delta/contact/detection/hybrid.h:58,76` | `const iREAL*`, pure |
| `delta::contact::detection::hybridWithPerBatchFallBack` | `hybrid.h:95,113` | `const iREAL*`, pure |
| `delta::contact::detection::hybridTriangleStat` / `hybridBatchStat` | `hybrid.h:132,149` | `const iREAL*`, pure |
| `delta::contact::detection::sphere`, `sphereWithBarrierAB`, `sphereWithBarrierBA` | `delta/contact/detection/sphere.h:35,53,71` | values + `const iREAL*` mesh; pure (they read only the geometry passed in) |
| `delta::contact::detection::isSphereOverlayInContact` | `sphere.h:89` | 8 scalars, pure — safe as the per-pair early-out |
| `delta::contact::detection::pointToGeometry` | `point.h:18-30` | `const iREAL*` + scalars, pure |
| `delta::contact::detection::pt` | `point.h:32` | `iREAL[3]` by value, pure |
| `delta::contact::detection::isInBox` | `delta/contact/detection/box.h:24-26` | `iREAL[3]` + `std::array<iREAL,6>`, pure (bbox overlap test, usable as the local/ghost filter) |
| `delta::contact::filter::filterOldContacts` (3 overloads, `38/44/48`), `filterNewContacts` (`53`) | `delta/contact/filter.h:38-58` | operate on the passed `std::vector<contactpoint>&` only |
| `delta::contact::contactpoint` ctors + `contactNormal` | `delta/contact/contactpoint.cpp:14-27`, ctors `32,34,64,145` | pure; NaN-guarded normal |
| `delta::contact::forces::getContactsForces`, `getContactForce`, `spring`, `springSphere`, `friction` | `forces.h:51,84,107,143,172` | raw pointers/value args, no globals |
| `delta::dynamics::expmap`, `updateRotationMatrix`, `updateAngular`, `updateVertices` | `delta/dynamics/dynamics.cpp:26,104,140,215`; `dynamics.h:64,86` | raw pointers, per-particle, no shared state (numerics caveat: §3/L1) |
| `delta::geometry::operators::*` (`vertex::computeBbox`, mesh/triangle helpers) | `delta/geometry/operators/…` | raw pointers, pure |

Caveats before reusing blindly:
- The `#if defined(SharedTBB) && defined(peanoCall)` branches take a `tarch::multicore::BooleanSemaphore&` — a threading, not MPI, dependency, and they need Peano to compile.
- `delta/contact/detection/gjk.h:32-45` takes **non-const** `iREAL*` for both geometries, so it cannot be fed `const` tile data as-is (and it is unreachable today — see §3/L3).
- File-static reporting counters (`hybrid.cpp:29-32`, `penalty.cpp:29-30`) are process-global: per-rank under MPI and needing a reduction to mean anything.

---

## 3. Latent bugs proven from source (several also reproduced at runtime)

**L1. `expmap()` off-diagonal term is duplicated — rotations are non-orthonormal.**
`delta/dynamics/dynamics.cpp:78-79`
```
77:  v01 = Omega1 * Omega2;
78:  v02 = Omega2 * Omega3;   // <-- should be Omega1 * Omega3
79:  v12 = Omega2 * Omega3;
```
With `v02 == v12` the `Λ3/Λ7` block is built from `Ω2Ω3` instead of `Ω1Ω3`; every other entry matches `R = I − sx[Ω]× + cx[Ω]×²` exactly, so `v02` is the single defect.
- Proof (E4): Ω=(1,2,3) → as-is `max|R R^T − I| = 6.951e-01`, `det = 1.3524`; with `v02 = Ω1·Ω3` → `2.2e-16`, `det = 1.0`. Axis-aligned Ω (0,0,3) is orthonormal either way, which is why simple tests miss it.
- End-to-end (E5): through the real `Engine`, 1000 `iterate()` calls with ω=(1,2,3), `resolveContacts=false` → `_orientation` drifts to `max|R R^T − I| = 1.680e-03`. `updateAngular` (`dynamics.cpp:140-211`, tensor product at 168) also feeds the non-rotation into `DL·A`, and `updateRotationMatrix` (`dynamics.cpp:104-138`, called at `Engine.cpp:716`) compounds it into `_orientation` every step. Note the same expression exists at `zoltan/dynamics.cpp:390-391`, i.e. it is inherited legacy behaviour, so "fix" changes baseline trajectories → decide before changing.

**L2. `Structure` zeroes the triangle counter for every sphere.**
`delta/core/data/Structure.cpp:52-59` — the non-mesh branch assigns instead of skipping:
```
56:  } else
57:  {
58:    _numberOfTriangles = 0;   // clobbers the running total
59:  }
```
In `input/hopper.mbfcp` all 26 mesh obstacles come first (`input/hopper.mbfcp:29-654`, `CONVEXES:` blocks) and all 15625 spheres after (679-188175), so the final value is 0. Proof (E2): `./delta` prints `# Particles : 15651` / `# Triangles : 0`. Impact is reporting-only (`State.cpp:56`), but it is the number an operator uses to size the mesh work. Fix: delete the `else` branch (or `+= 0`).

**L3. Five valid `CollisionModel` values silently produce zero contacts.**
`delta/core/Engine.cpp:317-461` switches on `_collisionModel` with cases for `Sphere(319)`, `BruteForce(379)`, `Penalty(399)`, `HybridOnBatches(419)`, `HybridOnTrianglePairs(439)`, `none(459)` and **no `default:`**. `Meta.h:25-39` also defines `PenaltyStat`, `PenaltyTune`, `HybridTriangleStat`, `HybridBatchStat`, `GJK`, all of which fall through: `newContactPoints` stays empty, `addCollision` is skipped (464), `_activeCollisions` is empty, and `deriveForces` skips every particle (631) — a free-fall run that looks like "no contacts today". Proof (E5): `modelScheme=0 -> 54 pairs`, `=3 -> 0 pairs`, `=9 (GJK) -> 0 pairs`, `=8 -> 0 pairs`, exit 0, no diagnostic. Also, with `SharedTBB && peanoCall` defined (`Engine.cpp:314-316`) the whole `#else` body is compiled out and the same silent no-op happens.

**L4. `State::_numberOfCollisions` is never assigned; two different meanings of "collisions".**
`delta/core/State.cpp:44` sets it to 0 and nothing ever increments it, so `Log.cpp:72` prints `cpt:0` for the entire run. Proof (E2): every one of the hopper's 10 steps prints `i:N -- cpt:0` while the run resolves `contacts=44500`. Proof (E5): `resolved pairs=144  State.getCollisions()=0`. Separately `Engine::getNumberOfCollisions()` (`Engine.cpp:283-286`) returns `_activeCollisions.size()` — a count of *particles with contacts*, not pairs — and E5 shows `64` vs the 144 resolved pairs. Fix: set `_numberOfCollisions` from the resolved pair count (`getContactPairs().size()` already exists) and rename/document the Engine helper.

**L5. Sphere constructor overwrites the friction flag with `isConvex`.**
`delta/world/structure/Object.cpp:179` (`_isFriction = isConvex;`). Every hopper sphere is built at `read.cpp:534` as `Object("sphere", rad, id, centre, WOOD, false, false, true, …)` = (isObstacle=false, **isFriction=false**, isConvex=true), so `ParticleRecord::getIsFriction()` (`ParticleRecord.cpp:87`) returns **true**, which is what reaches `contactpoint::friction = frictionA && frictionB` in every detection kernel. The requested flag is silently ignored for spheres. Fix: assign `isFriction`.

**L6. `World` bounding box is computed into the wrong array.**
`delta/world/World.cpp:70-72` (mesh branch) and `95-97` (sphere branch): all six max tests write `globalMin[k]` instead of `globalMax[k]`, e.g.
```
70:  if(max[0] > globalMax[0]) globalMin[0] = max[0];
```
`globalMax` is initialised `{0,0,0}` (`World.cpp:54`) and is never written, so `_boundary = {min…, max…}` (`World.cpp:105`) is built from the **last** particle's box (`min`/`max` are overwritten per iteration at 63-64 / 83-89) plus a mix of first-seen minima — not the scene extent. Reachable via `Engine` ctor #1 / `World::getBoundary()`. (The hopper path does not use it: `delta/main.cpp:123` hard-codes `boundary = {0,0,0,1,1,1}` for a scene that is not inside it, which is its own latent problem for anything that later clips or decomposes on that box.)

**L7. `Object` mesh/sphere setters publish the wrong inverse inertia and bbox.**
`delta/world/structure/Object.cpp:121` and `:258` copy the `inertia[]` parameter straight into `_inverse[]` (`Object::setInertia`, both overloads, `Object.cpp:110-133` / `:247-270`); only the explicit `Object::setInverse` (`Object.cpp:396-404`) stores a real inverse. `:141` sets `_maxBoundBox = mesh->getBoundaryMinVertex();` — the min vertex as the max corner. Both defects are masked today: `ParticleRecord` recomputes inertia/inverse itself (`ParticleRecord.cpp:75-76`) and takes its bbox from the **mesh** (`ParticleRecord.cpp:69-71`), not from the Object, and all three `setInertia` call sites (`geometry/operators/triangle.cpp:762,783,854`) immediately call `setInverse`, overwriting the bad copy. `Object::getInverse()` (`Object.cpp:390-393`) and `Object::getMaxBoundaryVertex()` (`Object.cpp:496`) remain public and wrong, so any AMReX port that reads Object state directly inherits them.

**L8. `Object::getNumberOfTriangles()` dereferences a possibly null mesh.**
`delta/world/structure/Object.cpp:280-283` → `_mesh->getTriangleFaces().size()` with no null check, while the meshless ctor (`Object.cpp:14-63`, exercised at `tests/dem_phase0_test.cpp:139-143`) leaves `_mesh == nullptr`. `World.cpp:61` calls it for every particle whenever `_isSphere == false` (any mesh collision model), so a meshless body in a mesh world is a segfault, and the same ctor leaves `_minBoundBox/_maxBoundBox/_centreOfMass/_inertia/_inverse` uninitialised — that garbage flows into `ParticleRecord::_bbox` (`ParticleRecord.cpp:35-38`) and thus the OctTree domain.

**L9. `OctTree` leaks every node and is shallow-copied per contact.**
`delta/core/data/OctTree.cpp:497-499` — the destructor is empty while `refine()` allocates 8 children per non-leaf (`OctTree.cpp:96-104`), and `ParticleRecord::refineTree` (`ParticleRecord.cpp:261-264`) calls `_tree.refine(...)` again without freeing the previous children. There is no user-defined copy constructor, so the implicit one copies `_isLeaf` and the raw `A…H` pointers: every particle copy (`Structure.cpp:34,60` push-back, and **one per contact per direction** via `Meta::Collisions`, `Engine.cpp:243-246`) shares the children of the live tree. It does not double-free (empty dtor) but it aliases a mutable spatial structure across copies and leaks. Fix: either delete `_tree` from `ParticleRecord` (dead consumer, see section 1) or give `OctTree` a real destructor + copy/move.

**L10. `cleanHybridStatistics()` resets shadow variables; the hybrid statistics are process-global.**
`delta/contact/detection/hybrid.cpp:35-40`:
```
37:  _numberOfPenaltyFails = 0;
38:  _numberOfBatchFails = 0;
39:  int batchSize = 0;      // shadows the static _batchSize
40:  iREAL batchError = 0;   // shadows the static _batchError
```
`_batchSize`/`_batchError` (declared `hybrid.cpp:31-32`) are accumulated at 248-249 and 535-536 and never reset, so `getBatchSize()`/`getBatchError()` (52,56) return an ever-growing running sum rather than the last pass. All four statics are file-scope, i.e. per-rank under MPI and only meaningful after an explicit reduction. (`penalty.cpp:29-30` has the same per-rank character: `_errorGlobalTracker` is only ever `clear()`ed, never appended.)

**L11. Dead/duplicated collision bookkeeping worth deleting rather than porting.**
- `Engine::hyperContacts` (`Engine.cpp:96-165`, decl `Engine.h:79`) has **no caller**; it is the only consumer of `ParticleRecord::getBbox()/getSubsetOfMesh()` and therefore of the stale OctTree. Its loop condition `getState().getCurrentStepIteration() > 0` (`Engine.cpp:107`) also makes it a no-op on the first step.
- `Structure::getGeometryGrid()` (`Structure.cpp:93-101`) has no caller beyond a commented-out line (`Engine.cpp:751`).
- `ParticleRecord::getClosestX/Y/ZCoordinatesTriangles` (`ParticleRecord.cpp:187,213,236`) have no callers.
- `addCollision`'s `bool sphere` parameter (`Engine.h:108`, `Engine.cpp:191`) is unused in the body; the `dataSetA && dataSetB` guard (231-234) drops new contact points for an already-registered pair — unreachable with the current candidate generators (`binnedCandidates` emits only `i<j`, `bruteForceCandidates` only `i<j`), so it can only mask a future duplicate-candidate broad phase.

**L12. `getNumberOfTriangles()` duplicates geometry understanding, and per-mesh counts feed a per-rank statistic.**
`ParticleRecord::getNumberOfTriangles()` (`ParticleRecord.cpp:167-170`) is `_xCoordinates.size()/3` — 0 for every sphere, which is why `Engine.cpp:467` (`incNumberOfTriangleComparisons(A*B)`) reports 0 for sphere-sphere pairs and why the `updateVertices` loop at `Engine.cpp:724` is a no-op for spheres (correct, but implicit). Anything that later reads `Structure::getNumberOfTriangles()` as a mesh-work proxy gets 0 (L2).

**L13 (ASK, not a confident defect).** `dynamics.cpp:157` builds `T[0] = rotation[0]*torque[0]+rotation[1]*torque[1]+rotation[2]*torque[2]` (row-major, i.e. `R^T`), whereas every other transform in the same file is column-major: `dynamics.cpp:135` (`angular = R*refAngular`), `dynamics.cpp:168` (`A = I*refAngular`) and `dynamics.cpp:234-236` (`updateVertices`, `x = R*X`) all use `rotation[0],[3],[6]` for component 0. One of the two is the transpose of the other. The two conventions are used consistently in the legacy `zoltan/dynamics.cpp`, so this may be intentional (referential vs spatial frame). It is not obviously wrong, but it should be pinned by a unit test before the physics is frozen, because the AMReX port will redistribute exactly these 3+9 doubles per particle.

---

## 4. Reproducing the runtime evidence

```
cd /workspace/delta/build && ./delta_dem_test                  # E1: serial baseline
cd /workspace/delta/build && ./delta                           # E2: hopper, "# Triangles : 0", 44500 contacts
cd /workspace/delta/build && python3 -c "import resource,subprocess,time; t=time.time(); \
  subprocess.run(['./delta'],stdout=open('/tmp/h3.txt','w'),stderr=subprocess.DEVNULL); \
  r=resource.getrusage(resource.RUSAGE_CHILDREN); print(r.ru_maxrss/1024.,'MB',time.time()-t)"   # E3
g++ -O2 -o /tmp/expmap_check /tmp/expmap_check.cpp && /tmp/expmap_check                          # E4
/usr/bin/c++ -std=gnu++14 -DDim3 -DbyteAlignment=32 -DiREAL=double -I/workspace/delta/delta \
  -isystem /usr/include/vtk-9.3 -o /tmp/mpi_audit_probe /tmp/mpi_audit_probe.cpp \
  /workspace/delta/build/libdelta_lib.a <VTK libs from build/CMakeFiles/delta_dem_test.dir/link.txt>
/tmp/mpi_audit_probe                                                                             # E5
```
Both probe programs are throwaway scratch files under `/tmp`; no repository file was created or edited.

## 5. Suggested order of attack (smallest first)

1. L3 + L4 (silent-wrong-results and lying counters) — few lines each, no numeric change.
2. L2 + L5 + L6 + L10 — one-line assignments.
3. L1 — decide first (changes baseline trajectories; do it before freezing AMReX comparisons).
4. Section 1 rows in plan order: global-`maxHalo` Allreduce + ghost width → per-rank bin index → plan/neighbour-list with an owner rule → force reduce-to-owner → rank-0 IO. The `Collisions::_copyOfPartnerParticle` removal and the `OctTree`/`hyperContacts` deletion are prerequisites for any per-step memory budget: the current design keeps one full partner copy per contact per direction for the whole step (E3: 484 MB baseline for 15651 bodies, multiplied by rank count).
