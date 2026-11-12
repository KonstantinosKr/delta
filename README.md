# Delta

## Layout

```
src/core/     shared platform: ParticleRecord, State, MeshBuffer, mbcfp/STL readers
src/dim2/     the 2D solver (circles and triangles, SAT narrowphase)
src/dim3/     the 3D solver
  contact/    broad + narrow phase, contact forces
  geometry/   mesh/ (exact) hull/ (approximate) loaders/ (imported files)
  dynamics/   integration, inertia, rotation
  world/      objects, scenarios, obstacles
  io/         VTU writer
  backends/   serial (Engine) and amrex (DemAmrexEngine, particle container)
apps/         one directory per app
  dem3d/      main.cpp, backend.h, tools/ (scenario generators), viz/ (ParaView sessions)
  dem2d/      main.cpp
tests/        one executable per suite (ctest)
amrex_contact/  standalone contact/broadphase experiment tree (own CMakeLists)
third_party/  vendored AMReX install (git-ignored)
input/dim3/   scenarios (*.mbfcp) and geometry (*.STL)
output/       generated artefacts, git-ignored
```

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j && (cd build && ctest)
```

Binaries: `delta` (3D) and `delta2d` (2D), both take `--help`.

`delta` runs either 3D backend from the same binary:

```sh
./build/delta --backend serial              # default
./build/delta --backend amrex               # parallel, uniform grid
./build/delta --backend amrex --mode regular  # parallel, refined to smallest body
```

`--mode` applies to the amrex backend only: `nogrid` (the original uniform
tiling) or `regular` (thesis 6.3, `dx` from the smallest body, halo widened to
cover the largest). Both must produce the same contact set - that is what
`tests/grid_modes_test.cpp` asserts.

One CMake project builds the core, `apps/dem3d` (`delta`), every test, and
`amrex_contact` when `third_party/amrex-install` exists. The AMReX backend is
built by default whenever that install is found; a machine without one builds
serial-only instead of failing. Options: `-DDELTA_USE_AMREX=OFF` forces the
serial build, `-DDELTA_BUILD_AMREX_CONTACT=OFF` skips the experiment tree. The
AMReX install is built by `amrex_contact/tools/build_amrex.sh`.

The MPI test (`dem_phase1_amrex`, run at 1 and 2 ranks) is only configured when
AMReX itself was built with MPI; the vendored 25.11 install is built with
`AMReX_MPI=OFF`, so it is skipped there with a status message.

We present a novel DEM (discrete element method) code with explicit time stepping. DEM codes simulate billions of small particles that interact with each other primarily through collisions. Different to state-of-the-art codes, we rely on triangulated non-spherical particles. This is computationally demanding, and we thus devise an asynchronous data exchange communication technique using MPI (Message Passing Interface) on manycore supercomputers, we discuss possible solutions to handle ghost particles that overlap multiple subdomains. At the compute node level, shared memory parallelism as well as vectorised SIMD executions are studied. On the algorithmic side, we explore a hybrid parallelisation approach and memory layouts that are suited to combine robust exact geometry checks with a fast penalty-based method.

The following research would not have been possible without the funding from both the Engineering and Physical Sciences Research Council (EPSRC) and EDF Energy Generation.

I would like to thank Durham University for the supercomputing resources and technical assistance. This work was made possible with the use of Durham University's local HPC facilities Hamilton and Intel Xeon Phi coprocessor cluster. I appreciate the support from Intel through Durham's Intel Parallel Computing Centre (IPCC) which gave me access to latest Intel software. I also made use of the facilities of N8 HPC provided and funded by the N8 consortium and EPSRC (Grant No. N8HPC_DUR_TW_PEANO). The Centre is coordinated at the time by the Universities of Leeds and Manchester. I'm also grateful for the access to ATHOS HPC resources provided by EDF R\&D in France and Advanced Research Computing High End Resource (ARCHER) HPC services at the Edinburgh Parallel Computing Centre.

Experiment videos can be found here: https://www.youtube.com/channel/UCZNTVjbweeKB0eKhsqBmXZQ

Konstantinos Krestenitis (contact @ ikonstantinos [dot] com)
