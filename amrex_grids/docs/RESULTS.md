# Results

Machine: 2 vCPU, 4 GB RAM, Debian, g++ 14.2. AMReX 25.11, 3D, **serial CPU** build
(`third_party/amrex-install`), `-O3`. All numbers below were produced by the commands
in `tools/verify.sh`; the raw CSVs are in `results/` (git-ignored, regenerable).

Narrowphase in every run: `hybrid` (Algorithm 5 penalty solver + brute-force
fallback). Contact law: linear penalty + Coulomb cap. `dt` is explicit-stability
capped unless `--dt-source grid` is given.

## 1. Unit tests

```
ctest: 4/4 suites pass
  narrowphase  1,324,282 checks, 0 failures
  physics             74 checks, 0 failures
  scenario         3,447 checks, 0 failures
  grid               149 checks, 0 failures
```

* Brute force matches 14 analytic triangle-pair configurations exactly
  (`max |brute - analytic| = 0`).
* On 200,000 random triangle pairs the penalty solver certifies 78.9 % of pairs
  (max relative error `2.4e-8`); 21.1 % fall back to brute force — 4.5 % on
  non-degenerate pairs, the rest on coplanar / touching / degenerate ones.

### The two hybrid variants (thesis 5.6)

The thesis' "hybrid on triangle pairs" runs the penalty solver and *keeps its
answer* when it is within tolerance, falling back to brute force otherwise. Both
readings are implemented and measured, because they buy different things:

| narrowphase | what it does | accuracy vs brute | work |
|---|---|---|---|
| `hybrid` (default) | iterative pass as a certification gate, always returns the exact brute answer | bit-identical on 200k pairs | brute runs on every pair: a robustness guarantee, no speed win |
| `hybrid-fast` | keeps the penalty answer when certified (the thesis' rule) | max rel. error `6.2e-9`, 0/50,000 pairs outside `1e-6*scale` | brute runs on 10.3 % of pairs; 0.67x the speed of brute on random triangle mixes |

So on this hardware the penalty solver is **slower per pair** than the 15-primitive
brute force (up to 8 Newton iterations with a 4x4 solve each), which is the caveat
the thesis itself raises on p.70. In situ, on 150 bodies × 300 steps with the
adaptive grid, `hybrid-fast` costs 172 ms against 203 ms for the always-brute hybrid
and 167 ms for plain brute force, with identical contact counts. The default stays
`hybrid` because identical contact sets are what the equivalence tests rely on.

## 2. Contact-set equivalence: every mode finds the same contacts

Thesis 7.3 two-body experiment (diameters 0.02 vs 0.20, ±0.1 diagonal velocities),
7,000 steps. At **every** step the contact set of the mode under test is compared
against the `nogrid + brute` oracle on the *same* body positions.

| mode | contacts | candidate pairs | tri pairs | verify |
|---|---|---|---|---|
| NoGrid | 27 | 7,000 | 91,624 | PASS (missing=0, extra=0) |
| Regular | 27 | 736 | 91,624 | PASS (missing=0, extra=0) |
| Adaptive | 27 | 736 | 91,624 | PASS (missing=0, extra=0) |
| Reluctant | 27 | 3,745 | 91,624 | PASS (missing=0, extra=0) |

Dense packing, 150 polydisperse icosahedra (0.03–0.08), gravity, 1,500 steps:

| mode | contacts | candidate pairs | tri pairs | levels | cells_total | cells_occupied | cell_pair_checks | wall |
|---|---|---|---|---|---|---|---|---|
| NoGrid | 6,231 | 16,762,500 | 5,068,271 | 1 | 1 | 1 | 0 | 12,718 ms |
| Regular | 6,231 | 518,975 | 5,068,271 | 4 | 20,440 | 139 | 1,401 | 6,543 ms |
| Adaptive | 6,231 | 518,975 | 5,068,271 | 4 | 179 | 139 | 303 | 6,049 ms |
| Reluctant | 6,231 | 669,577 | 5,068,271 | 4 | 159 | 139 | 358 | 6,082 ms |

All four modes produce **identical contact counts and identical triangle-pair
counts**, i.e. the grid changes how many *candidate pairs* are produced, never which
contacts are found. Where the wall time only improves ~2x it is because the
narrowphase (5.07 M triangle tests) dominates; the broadphase work drops 32x.

## 3. Broadphase scaling (no contacts, narrowphase cheap)

120 bodies, 0.03–0.07, 200 steps, sparse:

| mode | candidate pairs | pairs/step | wall | speedup |
|---|---|---|---|---|
| NoGrid | 1,428,000 | 7,140 | 690 ms | 1.00x |
| Regular | 16,821 | 84 | 174 ms | 4.0x |
| Adaptive | 16,821 | 84 | 139 ms | 5.0x |
| Reluctant | 27,943 | 140 | 134 ms | 5.1x |

**85x fewer candidate pairs** for the gridded variants on the same trajectory.

## 4. What the grid morphology actually costs

Same 139 occupied cells in the dense packing:

* **Regular** allocates 20,440 cells at four levels and walks 1,401 cell-pair
  checks per build; **Adaptive** allocates 179 cells and 303 checks. Both emit the
  same 346 candidate pairs per step. The regular grid's penalty is grid
  administration (allocation + traversal), not particle comparisons — matching the
  thesis' argument in 6.3 / Fig. 6.10, where the regular grid peaks at 850k
  comparisons against 25k for the adaptive grid.
* **Reluctant** keeps `cells_refused = 20` cells coarse and drops to 159 cells, but
  because a coarse cell holds more bodies it emits *more* candidate pairs (446 vs
  346). Fewer refinements, more comparisons — the trade the thesis describes.

## 5. Dynamic time step (chapter 7)

Same two-body experiment, step limited by the grid only (`--dt-source grid`), which
is the thesis' regime:

| mode | h_min | mean dt | max dt | contacts |
|---|---|---|---|---|
| NoGrid | 0.020 (body diameter) | 3.9e-03 | 5.2e-02 | 26 |
| Regular | 0.037 | 4.3e-02 | 3.7e-01 | 36 |
| Adaptive | 0.037 | 4.3e-02 | 3.7e-01 | 36 |
| Reluctant | 0.111 (3 levels only) | **6.4e-02** | 1.07 | 30 |

The reluctant grid keeps `h_min` three times larger than the adaptive grid, so it
takes **1.5x larger mean steps** (and reaches the collision in fewer steps). It also
*misses 6 of the 36 contacts* the fine-grid runs resolve, and its largest step
(1.07 s) is a clear cell-tunnelling event: the trajectory diverges because the two
runs no longer sample the same states. This is the caveat the thesis itself raises
about the one-cell-per-step rule (7.1) — the reluctant variant buys step size with
contact resolution, and it only pays off when the approach test refines before the
pair can jump a cell.

With the default explicit-stability cap (contact stiffness sets the step), all four
modes take the same `dt` and find the same 27 contacts; the grid then only changes
the broadphase work, as in section 2.

## 6. Reproduce

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j2
ctest --test-dir build --output-on-failure
tools/verify.sh
```
