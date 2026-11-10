# amrex_grids — grid morphologies and contact detection on AMReX

A self-contained experiment tree inside the Delta repo that reproduces the contact-detection
machinery of K. Krestenitis, *Parallel Multiscale Contact Dynamics for Rigid Non-spherical
Bodies* (PhD thesis, Durham 2018), on top of AMReX.

Four broadphase morphologies (thesis 6.3), one binary, selected with `--mode`:

| `--mode` | thesis | what it does |
|---|---|---|
| `nogrid` | "deteriorated space grid" | one mono-cell; every body against every body. The reference/oracle |
| `regular` | 6.3 | uniform refinement to the smallest body; the complete `BoxArray` exists at every level, empty cells included |
| `adaptive` | 6.3 + 6.4.2 | cells only where bodies are; bodies drop/lift across levels by size; coarse bodies are inherited downwards as *virtual particles* |
| `reluctant` | 6.3 + ch. 7 | refine a cell only when it holds ≥ 2 bodies that are **approaching**, so `h_min` — and with it the chapter-7 step bound — stays large |

Narrowphase (5.4–5.6): `brute`, `iterative` (Algorithm 5 penalty solver), `hybrid` (penalty pass
as a gate, exact brute answer returned), `hybrid-fast` (5.6: keep the penalty answer when it
certifies). Contact law (2.3): `penalty` or `springdashpot`. Time stepping: fixed, explicit-stability
capped, or the chapter-7 dynamic controller.

Everything below runs on **.125 inside the `herdr` container**, which is where the repo lives.

---

## 1. Compile

```bash
# inside the dev container (docker exec -it herdr bash)
cd /workspace/delta/amrex_grids

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
```

Requirements — already present in the container:

| tool | version | why |
|---|---|---|
| g++ | 14.2 | C++17 |
| cmake | ≥ 3.20 | the project uses `CONFIGURE_DEPENDS` globs |
| OpenMPI dev | 4.x | **the AMReX in `../third_party/amrex-install` is MPI-enabled**, so the project must enable the C language as well as C++ (`project(... LANGUAGES C CXX)`), otherwise `find_dependency(MPI)` fails on the C component |
| AMReX | vendored | `../third_party/amrex-install` (3D, double, MPI); the build searches `third_party/amrex-install` relative to this directory, then `../third_party/amrex-install` |

Tests:

```bash
ctest --test-dir build --output-on-failure
```

Expected: **4/4 suites pass** — `narrowphase` (1,324,282 checks), `physics` (74), `scenario`
(3,447), `grid` (149).

### Per-morphology binaries

The build also emits one driver per broadphase morphology, with that mode baked in as the
default (the `--mode` flag still overrides it):

```
build/contact_dem            # --mode all by default, runs the four in sequence
build/contact_dem_nogrid
build/contact_dem_regular
build/contact_dem_adaptive
build/contact_dem_reluctant
```

So these two are equivalent:

```bash
./build/contact_dem_adaptive --scenario two-body --steps 7000
./build/contact_dem --scenario two-body --mode adaptive --steps 7000
```

---

## 2. Run the two-particle collision

The thesis 7.3 experiment: a 0.02-diameter body at (0.2, 0.2, 0.2) and a 0.2-diameter body at
(0.8, 0.8, 0.8), each moving ±0.1 along the diagonal of the unit cube. The size ratio is what
forces the bodies onto different grid levels, so it is the cleanest demonstration of the
multiscale machinery.

```bash
# all four morphologies, contacts checked against nogrid+brute at every step
./build/contact_dem --scenario two-body --mode all --steps 7000 --verify
```

Measured on .125:

| mode | contacts | candidate pairs | tri pairs | wall |
|---|---|---|---|---|
| NoGrid | 27 | 7,000 | 91,624 | 202 ms |
| Regular | 27 | 736 | 91,624 | 3,449 ms |
| Adaptive | 27 | 736 | 91,624 | 178 ms |
| Reluctant | 27 | 3,745 | 91,624 | 151 ms |

`--verify` recomputes the contact set with `nogrid + brute` **on the same body positions** every
step and exits non-zero on any missing contact, so `PASS (missing=0, extra=0)` in all four rows is
the equivalence claim: the grid changes how many candidate pairs are produced, never which contacts
are found. The regular grid is 20× slower than the adaptive one here purely on grid administration.

Useful variants:

```bash
# one morphology
./build/contact_dem --scenario two-body --mode reluctant --steps 7000

# narrowphase comparison on the same trajectory
for np in brute iterative hybrid hybrid-fast; do
  ./build/contact_dem --scenario two-body --mode adaptive --narrow "$np" --steps 7000 --quiet
done

# chapter-7 regime: dt limited by the grid, not by contact stiffness
./build/contact_dem --scenario two-body --mode all --steps 7000 --dt-source grid
```

In the `--dt-source grid` run the reluctant grid keeps `h_min` three times larger and takes 1.5×
larger mean steps (6.4e-2 vs 4.3e-2) — but misses 6 of 36 contacts, which is the tunnelling caveat
the thesis raises in 7.1. Numbers and discussion: [`docs/RESULTS.md`](docs/RESULTS.md).

---

## 3. Write the visualisation output

```bash
./build/contact_dem --scenario two-body --mode adaptive --steps 7000 \
    --vtk /tmp/twobody/frame --vtk-every 250
```

That writes `frame_00000.vtu … frame_06750.vtu` plus the ParaView collection `frame.pvd`
(28 frames, ≈ 240 KB for the adaptive grid, 6.8 MB per frame for the regular one). Each frame is an ASCII VTU `UnstructuredGrid` with:

* **body triangles** — one `VTK_POLYGON` per triangle, world frame, `kind = 0`
* **broadphase cells** — one `VTK_VOXEL` per cell that currently exists in the grid, `kind = 1`

Cell data on every element: `kind` (0 body / 1 grid), `body` (particle id, −1 for cells), `level`
(grid level of the body or of the cell), `occupied` (real + inherited bodies in that cell).

```bash
# with several modes at once, files are suffixed per mode
./build/contact_dem --scenario two-body --mode all --steps 7000 \
    --vtk /tmp/twobody/frame --vtk-every 500
ls /tmp/twobody/frame.*.pvd     # frame.NoGrid.pvd, frame.Adaptive.pvd, ...
```

### In ParaView

1. `File → Open` → `frame.pvd` → `Apply`, then press play (the `.pvd` drives the animation).
2. Colour by **`level`** — the adaptive grid shows a 5-cell multiscale spine around the two bodies
   (levels 0–3), the regular grid shows the full 20,440-cell uniform mesh, the reluctant grid stays
   coarse until the bodies are close.
3. To see the bodies alone, `Threshold` on `kind == 0`; for the grid alone, `kind == 1` and switch
   to **Surface With Edges** (or `Wireframe`) so the cell structure is visible.
4. `Clip` or `Slice` through the diagonal to watch the fine cells appear just before contact.

Frame sizes: the regular grid writes ~20,000 voxels per frame (≈ 7 MB each), so keep
`--vtk-every` large for it. The adaptive grid is three orders of magnitude smaller.

---

## 4. Reproduce everything

```bash
tools/verify.sh        # ctest + equivalence runs + benchmarks + markdown reports -> results/
```

`tools/verify.sh` needs `python3` (for `tools/bench.py`); it is a stdlib-only script.

## Layout

```
include/contact/   Types, Narrowphase, Grid, Physics, Scenario, VtkWriter (fixed interfaces)
src/geom/          triangle-triangle distance: brute, penalty solver, hybrid
src/grid/          the four morphologies on BoxArray / DistributionMapping / iMultiFab
src/physics/       contact laws, integrator, dynamic dt, diagnostics
src/scenario/      convex shapes (tetra/octa/icosa/hull) and lattice packings
src/io/            ASCII VTU + PVD writer
src/app/           the driver
tests/             four standalone suites, wired into ctest
docs/SPEC.md       the replication contract (formulas, invariants)
docs/RESULTS.md    measured numbers and the honest caveats
docs/THESIS-MAPPING.md  thesis section -> code map, including what is *not* replicated
```

## Known limits

* Serial single rank: AMReX is built with MPI, but this app initialises one rank and does no
  domain decomposition, particle migration or ghost exchange (thesis ch. 8 is out of scope).
* Scalar inertia (sphere approximation), no full inertia tensor, no multi-contact manifolds.
* Grid levels are AMReX `BoxArray`s, not the thesis' Peano spacetree, so traversal order is
  `MFIter`/lexicographic rather than a space-filling curve.
* The narrowphase returns the single deepest contact per body pair.
