# AMReX Integration Plan for Delta DEM

Status: proposal (no code changed yet)
Scope: integrate the `delta/` DEM tree with AMReX. `zoltan/` and `multiscale/` are legacy parallel paths and are out of scope except as a source of requirements.

> **Status update (branch `amrex-integration`).** Phase 0 is done: serial
> physics, a uniform-cell broad phase, real `.mbfcp` hopper input (mass, gravity,
> obstacles) and a third-law regression guard. Commits `aac362b3`, `5408cff2`.
> The legacy `zoltan/` and `multiscale/` trees named below have since been deleted
> (they were never in the build); references to them here are historical.
> Two read-only recon reports now supersede parts of this document:
>
> - `docs/recon/serial-audit.md` - every serial/global assumption that blocks
>   domain decomposition (file:line), 13 latent bugs proven from source, and the
>   kernels that port unchanged.
> - `docs/recon/amrex-fit.md` - the AMReX mapping, the corrections to §2/§3/§4/§5
>   below, and a first-PR plan (~855 new LOC, 0 deletions, 0 serial edits).
>
> Known errors in this document, per `docs/recon/amrex-fit.md` §9: the O(N²)
> claim (now a cell list, `Engine::binnedCandidates`), the
> `for_each_pair`/`fillGhosts` API names (they do not exist in AMReX 26.09), the
> "hand-rolled Makefile" claim (the tree is CMake), and the
> `project(delta_dem CXX)` snippet (must enable C).

---

## 1. Why AMReX

Delta already has two hand-rolled parallel layers:

- `delta/` - modern C++14, class-based (`Engine`, `World`, `Structure`, `ParticleRecord`), serial / OpenMP / TBB, O(N^2) contact detection, VTK output. No MPI.
- `zoltan/` - C-style MPI + Zoltan RCB load balancing + manual ghost migration (`migrate`, `migrateGhosts`), flat arrays `iREAL *t[6][3]`.
- `multiscale/` - Peano-coupled grid variant.

AMReX replaces exactly the parts that are expensive to maintain: domain decomposition, particle migration, ghost/neighbor exchange, load balancing, runtime config, I/O, and eventually GPU portability. It also opens the door to coupling DEM with an AMReX-based CFD solver (MFiX-Exa / incflo style) where the AMR mesh matters.

AMReX does **not** provide non-spherical DEM contact geometry. The existing `delta/contact/*` kernels are the value of this repo and stay.

## 2. Target architecture

| Concern | Today (`delta/`) | With AMReX |
|---|---|---|
| Particle storage | `std::vector<ParticleRecord>`, each holds a full copy of its triangle mesh | `amrex::ParticleContainer` SoA; per-particle rigid state only |
| Mesh geometry | duplicated per particle | shared `MeshLibrary`, indexed by `shapeId` |
| Neighbor search | `Engine::contactDetection()` O(N^2) double loop + optional sphere bbox precheck | `amrex::NeighborParticleContainer` + `for_each_pair`, ghost width = max halo |
| Decomposition / migration | none (serial) / Zoltan RCB in `zoltan/` | `BoxArray` + `DistributionMapping` + `ParticleContainer::Redistribute` |
| Ghost exchange | manual MPI in `zoltan/migration.cpp` | `ParticleContainer::fillGhosts` / `RealCommunication` |
| Contact model | `delta::contact::detection::{sphere,bf,penalty,hybrid,...}` | unchanged, wrapped by an adapter |
| Forces | `forces::getContactsForces` over `std::vector<contactpoint>` | per-pair accumulation into SoA force/torque; reuse `getContactForce` |
| Time loop | `Engine::iterate()` | AMReX `AmrCore` (or bare) loop over `MFIter`/`ParIter` |
| Config | literals in `main.cpp` | `ParmParse` inputs file |
| Output | VTK legacy (`io/write.cpp`, needs VTK) | `WriteSingleLevelPlotfile` + particle plotfiles; VTK optional |
| Build | hand-rolled `Makefile` + `files.mk` | CMake `find_package(AMReX)` |

## 3. Key design decisions

### 3.1 Separate shared meshes from particle state (prerequisite)
`ParticleRecord` currently stores `_xCoordinates`, `_refxCoordinates`, etc. per particle. AMReX particles are fixed-layout SoA and cannot hold a variable-length vertex list. Introduce:

```cpp
struct MeshLibrary {
  // shapeId -> reference vertices (refx/refy/refz), avg/min/max mesh size, bbox
  std::vector<Shape> shapes;               // host side
  // later: amrex::Gpu::DeviceVector<Shape> for GPU
};
```

A particle keeps `shapeId` + rigid state (`centre`, orientation, velocities, ...). World-space vertices are recomputed each step (already done by `dynamics::updateVertices`). This also removes the current N-fold duplication of identical sphere/triangle meshes.

### 3.2 Keep contact kernels, add a materialization adapter
Do not rewrite `bf/penalty/hybrid` first. Add a thin adapter that, for a candidate pair, produces the `(x,y,z, nTriangles)` pointers the kernels already take, sourced from `MeshLibrary` + current transform. This makes the AMReX migration incremental and keeps the parity oracle meaningful.

### 3.3 Neighbor list instead of O(N^2)
`amrex::NeighborParticleContainer` with `nghost` derived from the max particle halo: `ceil(halo_diameter / min_cell_size)`. Per-pair kernel then calls the existing contact model. This replaces both the double loop and the Zoltan ghost logic.

Obstacles (e.g. the hopper boundary, `KINEMATICS: OBSTACLE`, one body with thousands of triangles) must be visible to every rank that can touch them. Simplest correct approach: replicate obstacles into a static AMReX particle tile on every rank and test particles against that tile. Do not redistribute obstacles.

### 3.4 Drop `contactpoint` retention for forces
`Engine::addCollision` deep-copies the partner `ParticleRecord` (whole mesh) into `Meta::Collisions`. Under AMReX the partner is a neighbor tile entry, so store a neighbor index/pointer instead. `forces::getContactForce` already computes per-contact-point force, so accumulate force/torque directly in the pair loop and delete `_activeCollisions` / `_collisionsOfNextTraversal`.

### 3.5 Load balancing
Start with a single-level `BoxArray` over `_boundary`, `max_grid_size` tiled, SFC `DistributionMapping`. Rebalance with `ParticleContainer::AssignDensity` + new `DistributionMapping` only if measurements show imbalance. Do not port Zoltan.

## 4. Phased plan

### Phase 0 - Baseline and parity oracle
Deliverable: a deterministic reference run.
- Add a headless build target that excludes VTK/Assimp I/O so the core can build and test without those deps.
- Record `hopper.mbfcp` (or a small sphere drop) for 100 steps; dump particle centres + orientations to a text/golden file.
- Exit criterion: reference file regenerates bit-stable in serial.

### Phase 1 - MeshLibrary + ParticleRecord slimming (still serial)
Touch: `delta/core/data/ParticleRecord.{h,cpp}`, `delta/core/data/Structure.{h,cpp}`, `delta/core/io/read.cpp`, `delta/world/structure/Object.*`.
- Extract reference meshes into `MeshLibrary`.
- `ParticleRecord` keeps `shapeId` + rigid state.
- Add `materializeVertices(shapeId, centre, orientation, out x/y/z)` adapter.
- Keep `OctTree` only for mesh-subset queries still needed by `hyperContacts`; for spheres it should become unused.
- Exit criterion: Phase 0 oracle reproduced within 1e-10.

### Phase 2 - AMReX ParticleContainer + neighbor contact (CPU, MPI)
Touch: new `delta/core/amrex/` (container type, tile ↔ `ParticleRecord` view), `delta/core/Engine.cpp` (`contactDetection`, `iterate`), `delta/main.cpp`, build.
- Define container type, e.g. `ParticleContainer<0,0, NReal, NInt>` with real comps: centre(3), velocity(3), angular(3), refAngular(3), orientation(9), mass, radius, halo, epsilon; int comps: gid, shapeId, material, isObstacle, isFriction.
- Build `Geometry`/`BoxArray`/`DistributionMapping` from `_boundary`.
- Load `.mbfcp`/scenario into the container (`InitOnePerCell` for grids, direct `SoA` fill otherwise).
- `NeighborParticleContainer::for_each_pair` -> existing detection kernels via the Phase 1 adapter.
- Replace manual migration with `Redistribute()` per step.
- Exit criterion: `mpirun -n 1..4` gives oracle-matching trajectories; contact counts match serial.

### Phase 3 - Forces, integration, config, scaling
Touch: `delta/core/Engine.cpp` (`deriveForces`, `updatePosition`), `delta/dynamics/*`, `delta/contact/forces/*`.
- Accumulate force/torque into SoA inside the pair loop using `getContactForce` (per contact point), not the vector-returning path.
- `ParmParse` group `dem` for `dt`, `gravity`, `collision_model`, `plot_int`, `max_grid_size`.
- Remove `_activeCollisions`, `_collisionsOfNextTraversal`, `Meta::Collisions`.
- Exit criterion: strong-scaling run on `hopper.mbfcp`; per-rank particle counts reported.

### Phase 4 - I/O and optional CFD coupling
Touch: `delta/core/io/write.cpp`, `delta/core/State.cpp`.
- Emit AMReX plotfiles (mesh fields + particles) alongside/over VTK.
- If two-phase coupling is the goal: deposit void fraction / drag sources to a `MultiFab` (`ParticleToMesh`), read fluid velocity back at particle positions (`MeshToParticle`), and introduce `AmrCore` levels at that point.
- Exit criterion: plotfile opens in ParaView/VisIt; coupled demo conserves mass/volume to tolerance.

### Phase 5 - GPU (only if needed)
- Move `MeshLibrary` to `amrex::Gpu::DeviceVector`; port contact kernels to `AMREX_GPU_DEVICE` with caller-provided output buffers (current kernels return `std::vector`).
- This is the largest lift and is deliberately deferred.

## 5. Build

Add `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(delta_dem CXX)
find_package(AMReX REQUIRED CONFIG)
add_executable(delta_dem <sources>)
target_link_libraries(delta_dem PRIVATE AMReX::amrex)
target_compile_definitions(delta_dem PRIVATE -DiREAL=double)
```

Configure with `-DAMReX_MPI=ON`, later `-DAMReX_OMP=ON`, `-DAMReX_CUDA=ON`. Keep the existing `Makefile` working until Phase 3 parity so the two paths can be diffed.

## 6. Risks

- **Non-spherical shape is not an AMReX concept.** Everything shape-related must stay in `MeshLibrary`; ghost width must use the bounding-sphere extent, not the radius, or elongated particles will miss contacts.
- **Nondeterministic force summation** across ranks/tiles breaks bitwise parity. Compare against the oracle with a tolerance, or sort contacts by gid before summing.
- **Descriptor size**: ~40 real comps + int comps per particle. Watch memory for 100k particles; consider packing inertia into per-shape tables instead of per-particle comps.
- **Obstacle replication** must be explicit and tested with `n > 1`.
- **VTK/Assimp coupling** to the current build will block a clean AMReX build until the I/O split in Phase 0.
- **`multiscale/`** already has a Peano grid coupling; if grid coupling is the real goal, decide AMReX-vs-Peano before Phase 4 to avoid maintaining two grid paths.

## 7. Open questions for the maintainer

1. Is the goal (a) parallel/GPU DEM, or (b) DEM-CFD two-phase coupling? (b) changes Phase 4 from optional to the point of the project.
2. Which AMReX backend matters - CPU+MPI, OpenMP, or CUDA/HIP?
3. Should `zoltan/` be deleted once Phase 2 lands, or kept for benchmark comparison?
4. Target AMReX version (SoA API and mesh/particle helpers changed across releases).

## 8. Recommended first PR

Phase 0 + Phase 1 only: parity oracle, headless core build, `MeshLibrary`, `ParticleRecord` slimming, no AMReX dependency yet. That is the smallest change that de-risks everything after it.

---

## 9. 3D output visualisation (investigation)

### 9.1 Current state

`Engine::plot()` -> `io::writeGeometryToVTKVTK()` writes `<path>geometry_<step>.vtu`.

- The file has **`NumberOfCells="0"`** - it is a point cloud of particle centres, not geometry. Non-spherical triangles never leave the process; ParaView only shows spheres because of glyphs.
- `output/state.pvsm` is a pre-baked ParaView state: two `XMLUnstructuredGridReader` proxies (`geometry_0.vtu`, `grid_0.vtu`) feeding `Glyph` filters with `SphereSource` (12+ glyph instances) plus vector glyphs. It is hard-wired to **step 0** and has no time series.
- `io::writeGeometryToVTK()` (legacy ASCII) **already writes full triangle cells** (`CELLS`, `CELL_TYPES` = 5) but is unused by `Engine::plot`.
- Bug: `write.cpp:61-62` calls `SetScalars(radius)` then `SetScalars(epsilon)`, so `radius` is dropped and only `epsilon` is active. The glyphs cannot be scaled by radius as intended.
- `writeGridGeometryToVTKVTK` calls `SetPoints(points)` inside the boundary loop, resetting the point set per box, so its cells reference the wrong points for more than one box.

### 9.2 Options

| Option | True triangle shapes | Time series | New deps | ParaView support | Effort |
|---|---|---|---|---|---|
| A. Fix VTK writer: emit triangle cells + `.pvd` collection | yes | yes | none (VTK already linked) | first-class | low |
| B. AMReX plotfiles (`WriteSingleLevelPlotfile` + `ParticleContainer::WritePlotFile`) | no (particles are points) | yes | AMReX | VisIt native; ParaView needs AMReX/Conduit plugin | medium |
| C. Plotfile -> VTU converter | yes (if converter emits mesh) | via `.pvd` | converter code | first-class after conversion | medium |
| D. In-situ (Ascent/SENSEI) or Amrvis | yes | yes | heavy toolchain | specialised | high |
| E. `pvpython` headless render script over A | yes | yes | ParaView install | n/a (renders PNG/movie) | low, additive |

### 9.3 Recommendation

**Do A now, add B for grid fields only once the AMReX path exists, and add E when a headless render/movie is needed.** Option A is the lazy correct fix: VTK is already a dependency, and both halves of the writer already exist in `write.cpp` - one emits cells, the other emits fields.

Concrete changes (no AMReX required):

1. **Merge the two writers.** One `writeGeometryToVTU(step, particles)` that inserts both the concatenated triangle vertices and the triangle cells (reuse the `meshStartPivotPoint` / `3 A B C` logic from `writeGeometryToVTK`), plus point data `velocity`, `radius`, `epsilon` and cell data `gid`, `material`. Fix the double `SetScalars` bug by using `AddArray` + explicit active scalar selection.
2. **Add a `.pvd` collection.** After each step write/refresh `geometry.pvd` referencing `geometry_0.vtu ... geometry_N.vtu`, so ParaView loads the run as an animation instead of one frame. This is ~30 lines of XML, no VTK classes needed.
3. **Keep a point-cloud fast path.** For large runs (e.g. 15,625 bodies) full meshes are big; offer `Meta::Plot` modes `Mesh` (triangles, default for validation/non-spherical) and `Glyph` (centres only, for big sphere runs). The existing VTK writer is the `Glyph` path once fixed.
4. **Refresh `state.pvsm`** to point at `geometry.pvd` (and the mesh output), or ship the pipeline as a `pvpython` script so the state is not a checked-in binary blob.
5. **Fix `writeGridGeometryToVTKVTK`** to build all 8 points per box before inserting its 12 line cells.

### 9.4 With AMReX

- Fields and particles: `WriteSingleLevelPlotfile` / `ParticleContainer::WritePlotFile` (`AMReX_PlotFileUtil.H`). These are the standard for the CFD/coupling side.
- Caveat: **stock ParaView does not natively read AMReX plotfiles** (VisIt does; ParaView needs the AMReX plugin or a Conduit/Blueprint bridge). AMReX particles are also points, so triangulated shapes still need the VTU mesh writer from 9.3.
- Therefore keep the VTU writer as the portable 3D path and treat AMReX plotfiles as the grid/coupling artifact. If a single viewer format is mandated, add the plotfile -> VTU converter (option C) rather than dropping triangles.

### 9.5 Acceptance check

After a run, `output/geometry.pvd` opens in ParaView as a time series; the slider animates; non-spherical particles show their triangulated surface (not glyph spheres); the `radius` field is present and usable for glyph scaling on sphere runs.
