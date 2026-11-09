# amrex-contact

An AMReX implementation of the four broadphase grid morphologies and the
contact-detection strategies from:

> K. Krestenitis, *Parallel Multiscale Contact Dynamics for Rigid Non-spherical
> Bodies*, PhD thesis, Durham University, 2018.

The thesis builds a DEM (discrete element method) contact-dynamics engine for
triangulated, rigid, non-spherical bodies. Its central claims are about how the
space is discretised for contact detection:

| Thesis | Mode | Idea |
| --- | --- | --- |
| 6.3 "deteriorated space grid" | `nogrid` | one mono-cell → every body against every body |
| 6.3 regular grid | `regular` | uniformly refine all cells to the smallest body |
| 6.3 adaptive grid | `adaptive` | refine only around bodies, coarsen empty space, inherit coarse bodies as *virtual particles* (6.4.2) |
| 6.3 + ch. 7 reluctant adaptive | `reluctant` | refine only when two bodies are close **and approaching**, keeping the grid coarse so larger time steps are admissible |

Narrowphase (thesis 5.4–5.6): `brute` (15 primitive checks per triangle pair),
`iterative` (the Algorithm 5 penalty solver), `hybrid` (penalty pass as a gate,
exact brute answer returned) and `hybrid-fast` (the thesis' better-of-the-two rule:
keep the penalty answer when it certifies). Contact response (2.3): `penalty` and
`springdashpot`. Time stepping (5.1, ch. 7): fixed or the dynamic Δt controller
(Algorithms 11–12).

## Build

AMReX 25.11 (3D, serial, CPU) is vendored under `third_party/amrex-install`.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j2
ctest --test-dir build --output-on-failure
```

## Run

```bash
# all four morphologies on the same 200-body polydisperse packing
./build/contact_dem --scenario pack --n 200 --steps 300 --verify

# the thesis 7.3 two-body experiment (0.02 vs 0.2 diameter), reluctant grid
./build/contact_dem --scenario two-body --mode reluctant --dynamic-dt

# brute-force vs hybrid narrowphase, CSV for the bench script
./build/contact_dem --mode adaptive --narrow brute --csv /tmp/brute.csv
python3 tools/bench.py /tmp/brute.csv
```

`--dt-source grid` reproduces the thesis' chapter-7 step (limited by the grid and
`v_max` only); the default `stiffness` caps the step by the contact stiffness so the
explicit integrator stays stable. `--verify` compares the contact set of the mode
under test against the `nogrid + brute` oracle **on the same body positions**, every
step — that is the equivalence check, and it exits non-zero on any missing contact.

## Results (short version)

Full numbers in [`docs/RESULTS.md`](docs/RESULTS.md). On 150 polydisperse bodies,
1,500 steps:

| mode | contacts | candidate pairs | tri pairs | wall |
|---|---|---|---|---|
| NoGrid | 6,231 | 16,762,500 | 5,068,271 | 12.7 s |
| Regular | 6,231 | 518,975 | 5,068,271 | 6.5 s |
| Adaptive | 6,231 | 518,975 | 5,068,271 | 6.0 s |
| Reluctant | 6,231 | 669,577 | 5,068,271 | 6.1 s |

Identical contacts in every mode (the grid changes the candidate set, not the
answer); 32x fewer candidate pairs than brute force, 85x on a sparse packing. The
adaptive grid allocates 179 cells where the regular grid allocates 20,440 for the
same 139 occupied cells. With `--dt-source grid` the reluctant grid keeps `h_min`
3x larger and takes 1.5x larger mean steps, at the cost of 6 missed contacts out of
36 — the tunnelling caveat the thesis itself raises.

## Layout

```
include/contact/   fixed interfaces (Types, Narrowphase, Grid, Physics, Scenario)
src/geom/          triangle-triangle distance: brute, iterative, hybrid
src/grid/          broadphase: four morphologies on AMReX BoxArray/MultiFab
src/physics/       contact laws, integrator, dynamic dt, diagnostics
src/scenario/      convex shapes and packing generation
src/app/           driver
tests/             standalone unit tests (also wired into ctest)
docs/SPEC.md       the replication contract (formulas, invariants)
docs/THESIS-MAPPING.md  thesis section -> code map
```

## Status / fidelity

See `docs/THESIS-MAPPING.md`. The grid is realised with AMReX `BoxArray` levels
rather than the thesis' Peano spacetree, so traversal order is
`MFIter`/lexicographic instead of a space-filling curve; the refinement rules,
virtual-particle inheritance, reluctant criterion and all contact kernels follow
the thesis. No MPI (serial build), no GPU.
