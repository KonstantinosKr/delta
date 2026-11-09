# Thesis → code map

Source: K. Krestenitis, *Parallel Multiscale Contact Dynamics for Rigid Non-spherical
Bodies*, PhD thesis, Durham University, 2018. Page/line references are to the local
text dump `/tmp/thesis.txt`; section numbers are the thesis' own.

This document maps **implementation units to thesis sections and algorithm numbers**.
It deliberately describes the *contract* in `include/contact/*.hpp` plus the modules in
`src/scenario/` (shapes, scenarios) and `tools/bench.py`, because those are the parts
that exist as code at the time of writing. It states plainly which claims are
replicated, which are approximated, and which are out of scope.

---

## 1. Unit → thesis map

| Implementation unit | Thesis reference | Algorithm | Status |
|---|---|---|---|
| `include/contact/Types.hpp` — `Triangle`, `Mesh`, `Quat`, `Particle`, `Aabb`, `ContactPoint` | 2.1 *Geometric Representation of Rigid Bodies*; 7.2 (halo radii) | — | Faithful data model, simplified inertia (see §3) |
| `include/contact/Narrowphase.hpp` → `pointTriangleDistance` | 5.4 *The Brute Force Geometric Comparison* | Algorithm 4 (point-to-triangle) | Faithful |
| `include/contact/Narrowphase.hpp` → `segmentSegmentDistance` | 5.4 | Algorithm 3 (segment-to-segment) | Faithful |
| `include/contact/Narrowphase.hpp` → `triangleTriangleBrute` | 5.4: 6 point-to-triangle + 9 segment-to-segment = 15 primitive evaluations per triangle pair | Algorithm 2 (brute force distance) | Faithful |
| `include/contact/Narrowphase.hpp` → `triangleTriangleIterative` | 5.5 *An Iterative Method*, 5.5.1 *Penalty-Based Formalism*, eq. (5.1) | **Algorithm 5** (penalty solver: Newton on the penalised barycentric objective) | Faithful to Algorithm 5; Appendix A.4's GJK (Algorithm 14) is **not** implemented — see §5, D1 |
| `include/contact/Narrowphase.hpp` → `trianglePairDistance` (`Hybrid`, `HybridFast`) | 5.6 *A Hybrid Method*, "hybrid on triangle pairs" (p. 71–72) | Algorithm 6 (hybrid on triangle pairs); Algorithm 7 (batches) **not** implemented | `HybridFast` follows 5.6 (keep the penalty answer when it is within tolerance); `Hybrid` adds a stricter always-brute reading used by the equivalence tests. Per-pair fallback, not per-batch |
| `include/contact/Narrowphase.hpp` → `Narrowphase::query` (halo-sphere pre-check, contact decision, `depth`, `fallback` flag) | 7.2 ("bounding spheres … explicit filter check before the expensive triangle-to-triangle collision detection"); 7.3 ("comparisons … persistent throughout the duration of the collision") | used inside Algorithms 9/10 | Faithful (single deepest contact per pair) |
| `include/contact/Grid.hpp` — `GridMode::NoGrid` | 6.3 *Multiscale Grid Morphology*, "deteriorated space grid … mono-cell grid … single cell of eight vertices" | Algorithms 9/10 with a 1-cell tree | Faithful (oracle broadphase, all `N(N-1)/2` pairs) |
| `include/contact/Grid.hpp` — `GridMode::Regular` | 6.3 "recursively refines all spacetree nodes until all children cell lengths are just larger than the smallest particle diameter" | Algorithms 9 + 10 | Approximated: every AMReX `Box` of every level exists (see §4) |
| `include/contact/Grid.hpp` — `GridMode::Adaptive` | 6.3 "refines the grid around particles but does not refine cells that hold no particles"; 6.4.1 drop/lift; 6.4.2 virtual particles | Algorithm 10 (multiscale grid DEM) | Approximated: cell-based `BoxArray` instead of vertex-based spacetree |
| `include/contact/Grid.hpp` — `GridMode::Reluctant` | 6.3/6.4 (reluctant adaptive grid) + chapter 7: refine only around pairs that approach | Algorithms 10 + 11 (refinement decision folded into the dt scheme) | Faithful in criterion, approximated in mechanism |
| `GridParams::refine_factor = 3` | 6.1 *Space Discretisation*: the unit cell is cut into three equidistant pieces per axis → `3^3 = 27` children; thesis 6.3 mentions `2^d = 8` virtual associations for the regular grid | — | Faithful (3-partitioning is the thesis/Peano default) |
| `GridParams::size_slack` (level rule `h_L >= size_slack * diameter`) | 6.3/6.4.1 drop rule ("cell no smaller than its diameter"); 7.1 condition (i) | Algorithm 10, `touchVertexFirstTime` drop | Faithful in rule, slack exposed as a parameter (thesis hard-codes `1/3` of the mesh edge as the trigger, see §5 D3) |
| `GridParams::inherit_coarse` (`virtual_links`) | 6.4.2 *Virtual Particles*: "single way (downwards) data transfer", "inherited particles are called virtual particles" | Algorithm 10, `touchVertexFirstTime` / `enterCell` | Faithful in semantics (real-vs-virtual pairs, downward only) |
| `GridParams::approach_gap`, `min_bodies_to_refine` | 6.3 reluctant criterion: "visited holds at least two particles that approach each other" | Algorithm 11 | Faithful |
| `NarrowphaseStats`, `GridStats` (`raw_pairs`, `unique_pairs`, `cell_pair_checks`, `virtual_links`, `cells_total`, `cells_occupied`, `levels`, `refinements`, `coarsenings`, `cells_refused`) | 7.3 comparison counts (Figure 7.3 top); 6.3/6.4 morphology statistics | — | Faithful as diagnostics |
| `include/contact/Physics.hpp` → `ContactDynamics::computeForces` (`ContactModel::Penalty`) | 2.3.1 *Repulsive Force* (linear spring–dashpot, Cundall & Strack), 2.3.2 *Frictional Force*, eq. (2.1) | — | Approximated: linear penalty spring + Coulomb cap, not the full Hertzian/spring-dashpot formula (see §3) |
| `ContactModel::SpringDashpot` | 2.3.1: `f = min(0, -ks (ε - d) + kd · 2 sqrt(ks / (1/mA + 1/mB)) · vAB) · unit(n)` | — | Approximated: `fn = kn*depth - gamma_n*v_n`, same structure, different damping calibration |
| `ContactDynamics::integrate` | 5.1 *Time Integration*: "velocities are updated by the interaction forces before the positions are updated" | Algorithm 1 (DEM blueprint) | Faithful: semi-implicit (symplectic) Euler, quaternion update `q += dt/2 ω q`, renormalisation |
| `ContactDynamics::applyWalls` | 2.3 (floor contact, Figure 2.9); chapter 3 hopper walls in spirit | — | Approximated: axis-aligned box walls only, no triangulated hopper/floor geometry |
| `DtController::update`, `DtState{dt_min=1e-4, inc=1.1, dt_max}` | 7.1 *Particle-to-Grid Relationship*, 7.2 *Particle-to-Particle Relationship* | Algorithm 11 (multiscale grid DEM dynamic dt), Algorithm 12 (dt variance & bookkeeping) | Faithful in structure (see §5 D2, D4 for the two constants that differ) |
| `DtController::stateOf` (`v_BA`, `gap`, separation/non-critical/critical) | 7.2: `v_BA = (v_A · d_AB) - (v_B · d_AB)`, positive ⇒ separation; critical when predicted halo overlap | Algorithm 11 lines 13–24 | Faithful |
| `Dynamics`/`Diagnostics` (`kinetic_linear`, `kinetic_angular`, `momentum`, `max_penetration`, `contacts`, `tri_pairs`) | 7.3 metrics; 4.3/8.4 performance discussion | — | Faithful as diagnostics |
| `src/scenario/Shapes.cpp` — `makeShape` (tetra, octa, icosa, random hull) | 2.1 (triangulated rigid bodies; chapter 3 varies triangle count 10–40); 5.3 (triangle meshes as the collision primitive) | — | Faithful in spirit: convex closed meshes; triangle counts differ from chapter 3's 10–40 (see §6) |
| `src/scenario/Scenario.cpp` — `buildScenario` (lattice-jittered placement) | chapter 3 initial drop configuration (Figure 3.1: particles above the hopper); 6.4.1 "on simulation initiation all particles placed in the hierarchy are dropped …" | Algorithm 1, initialisation phase | Approximated: no hopper/funnel/floor geometry, uniform box domain |
| `src/scenario/Scenario.cpp` — `buildTwoBodyCollision` | **7.3** *Particles that Reside on Different Grid Levels*, p. 111: unit cube, centres `(0.2,0.2,0.2)` and `(0.8,0.8,0.8)`, velocities `(0.1,0.1,0.1)` and `(-0.1,-0.1,-0.1)`, diameters 0.02 vs 0.2 | — | Faithful (exact coordinates, velocities, diameters) |
| `tools/bench.py` | 7.3 Figure 7.3: comparisons vs time (top), step size vs time (middle), wall time (bottom); 3.1 Table 3.1 symmetry metrics analogue | — | Faithful as a reporting tool; no run-time measurement of its own |
| `tests/test_scenario.cpp` | — | — | Test harness, no thesis counterpart |

---

## 2. What "replicated faithfully" means here

1. **Brute force is the oracle.** Algorithm 2's 15 primitive evaluations per triangle
   pair, with the pair of closest points returned, is the reference used by the
   broadphase-equivalence validation (spec §4.2).
2. **Hybrid robustness claim.** 5.6's argument is that the hybrid returns the brute
   force answer while paying the iterative price for most pairs. The code enforces
   this bit-identically (`fallback` flag, `NarrowphaseStats::fallbacks`).
3. **Four grid morphologies.** 6.3's no-grid / regular / adaptive / reluctant set is
   preserved with the same names and the same intended asymptotics: no-grid = all
   pairs, regular = uniform refinement to the smallest particle, adaptive = refinement
   only around bodies, reluctant = refinement only around approaching pairs.
4. **Multiscale, downward-only inheritance.** 6.4.2: coarse bodies become *virtual*
   bodies at finer levels, never the other way round; a pair is emitted once, at the
   level of its finest member.
5. **Dynamic Δt.** Chapters 7.1/7.2: three approach states from the sign of `v_BA`, a
   predicted step `pdt = dt (1 + inc)` to detect critical approach, a local step
   `dt_local = 0.5 · gap / |v_BA|`, global `dt = min(dt_local)` when critical,
   otherwise `dt *= 1.1`, clamped by `dt_max = h_min / (2 · inc · v_max)` and by the
   no-tunnelling rule `v_max · dt <= 0.5 · h_min` (7.1 iii).
6. **The 7.3 experiment.** The two-body scenario is the thesis' own numerical setup,
   not a lookalike: same cube, same two centres, same two velocities, same 0.02/0.2
   diameters. The predicted collision time follows from the geometry
   (`(|d| - r_a - r_b) / |v_rel| = (1.0392 - 0.11) / 0.3464 ≈ 2.68`), which matches the
   "collision around t ≈ 2.5" statement in 7.3.

---

## 3. What is approximated (and why)

| Thesis | Code | Reason / consequence |
|---|---|---|
| Peano spacetree + space-filling-curve linearisation (6.1/6.2), vertex-based particle association | AMReX `BoxArray` + `DistributionMapping` + `MultiFab`, cell-based association | AMReX is the substrate; a spacetree is emulated by a stack of `BoxArray` levels with `refine_factor = 3`. Consequences: cells, not vertices, are the bucket; the 28 vertex-to-vertex comparisons per cell of 6.4.2 become cell-pair stencils; lexicographic `MFIter` order replaces Peano order, so *traversal order* (and hence floating-point accumulation order) differs. |
| Full inertia tensor from the triangulated geometry (5.1: "construct the inertia matrix … adding up a series of simplices") | Scalar `inertia = 0.4 · m · r²` (`inv_inertia` scalar) | The header fixes a scalar inertia ("solid sphere approximation"). Consequence: torque response is isotropic; a non-spherical body spins up too easily about its long axis. Documented in `Types.hpp` and here. |
| Hertzian / spring-dashpot normal force with `ks`, `kd`, `ε = 0.001` (2.3.1) | Linear penalty `fn = kn · depth` clamped at 0, `kn = 1e5`, `kt = 2e5`, `mu = 0.5`; `SpringDashpot` variant uses `fn = kn·depth - gamma_n·v_n` | Coefficients are exposed in `Material`; the thesis' specific constants are not reproduced because the replication targets the *contact detection / broadphase* claims (chapters 5–7), not the constitutive law. Absolute penetration depths are therefore not comparable to the thesis. |
| Contact-point de-duplication "closer than `min(h_A,min, h_B,min)`" and per-contact accumulation over the set `C_A` (2.3.1) | One contact per body pair (the deepest), accumulated pairwise | The narrowphase contract returns a single `ContactPoint` per pair; multi-point manifolds (needed for stable stacking of flat faces) are out of scope. |
| Halo radius `r = 1.1 · e` with `e = 0.001` (7.2, 2.3.1) | `halo = radius · (1 + 0.1)` (`halo_epsilon = 0.1`, `Types.hpp` / `GridParams`) | The fixed spec/header choose `ε = 0.1`. The `1.1 · r` *form* is faithful; the magnitude is 100× the thesis'. It only changes the conservative pre-check radius and `dt_local`, not the exact triangle distances. |
| `dt_max = hmax / (2 · 1.1 · Vmax)` (7.1, where `hmax` is the *maximum* cell length) vs `dt_max = hmin / (2 · inc · Vmax)` (7.2, *minimum* cell length) | `dt_max = h_min / (2 · inc · v_max)` | The thesis contradicts itself between 7.1 and 7.2; the code (and `SPEC.md` §3) follows 7.2, which is the conservative reading and the one the 7.3 results are consistent with (regular grid pinned to its finest cell). |
| Hybrid-on-batches (Algorithm 7), batch granularity | Hybrid-on-pairs only (Algorithm 6) | Batching is a vectorisation/SIMD concern (5.8); out of scope for a first correct replication. |
| SIMD vectorisation and memory-layout work (5.4 "cannot be vectorised", 5.6, 5.8 *Loop Footprint Optimisation*) | Scalar C++17 | No explicit SIMD; compiler auto-vectorisation only. |
| Manycore concurrency: 8.1 triangle-mesh parallelisation, 8.2 particle-to-particle, 8.3 grid-based producer–consumer, 8.4 grain size, 8.5 many-particle systems; Algorithms 13, 15, 16 | AMReX MPI/OpenMP/GPU execution of the same kernels; no producer–consumer pipeline, no asynchronous exchange | Out of scope for this replication; only the serial algorithmic content of chapters 5–7 is reproduced. |
| Chapter 3 hopper: funnel + floor geometry, 1000 particles, 10–40 triangles per particle, uniform/non-uniform mass, symmetry metrics `IEx`, `IEmass`, `Mx`, `Mm`, `Varx`, `Varm` (Table 3.1) | Box domain, convex shapes with 4/8/20/≈26–34 faces, no hopper, no floor mesh; symmetry metrics not implemented | The hopper study is a *physics* result about shape-induced asymmetry; it needs triangulated wall geometry (not in `Scenario.hpp`) and a long enough run. `ScenarioSpec::ground_plane` only signals an AABB floor. |

---

## 4. Broadphase: thesis text vs code

The thesis describes *one* spacetree whose vertices hold particles (6.1), traversed
depth-first along a Peano curve (6.2), with four morphologies (6.3) and two
inter-grid mechanisms: drop/lift (6.4.1) and virtual particles (6.4.2). The code keeps
the morphology semantics but replaces the tree with AMReX levels:

| Mode | Thesis | Code contract |
|---|---|---|
| `NoGrid` | 6.3 "deteriorated space grid", mono-cell of 8 vertices, everything compared | one `Box` covering the domain, `N(N-1)/2` candidate pairs — the oracle |
| `Regular` | 6.3: refine **every** node until the children are just larger than the smallest diameter | levels `0..L_max` fully populated; empty cells still traversed (this is the cost the variant pays, per 6.3) |
| `Adaptive` | 6.3: refine only around bodies; empty regions stay coarse; drop/lift; 6.4.2 virtual particles | only occupied cells exist; coarse bodies inherited downward as virtual; empty levels are not created; `refinements`/`coarsenings` count the deltas |
| `Reluctant` | 6.3 + 7.1: keep the grid as coarse as possible; refine only a cell holding ≥2 bodies with `v_BA < 0` and `gap < approach_gap · h_L` | identical criterion; refused cells counted in `cells_refused`; a refused cell keeps its bodies on the coarser level even if they are small enough to drop |

Consequence to keep in mind when reading benchmark numbers: a cell-based bucket can
hold a body whose *real* radius exceeds the cell edge only through the `size_slack`
rule (the drop condition prevents that), whereas the thesis' vertex association admits
several particles per vertex unconditionally (6.4.1: "each vertex … can accommodate
unlimited number of particles"). So `cells_occupied` and `cell_pair_checks` are not
numerically comparable to the thesis' vertex-comparison counts; only the *ordering* of
the four modes is.

---

## 5. Known discrepancies worth flagging upstream

**D1 — two iterative narrowphases exist in the thesis.** 5.5 ("An Iterative Method",
Algorithm 5) is a **penalty solver**: quasi-Newton on
`f(a,b,g,d) = |x(a,b) - y(g,d)|^2` with the six barycentric inequality constraints
folded in through eq. (5.1), a 4x4 Hessian (`hf` + `rho·Dmax'Dmax` + regularisation)
and Gaussian elimination for the Newton direction. Appendix A.4 / Algorithm 14 is a
separate **GJK** simplex sub-distance. This code implements **Algorithm 5** (the
penalty solver), because that is the method the thesis puts in the main algorithmic
chapter and the one the hybrid of 5.6 falls back from. GJK is out of scope. Note the
consequence measured in `tests/test_narrowphase.cpp`: the penalty solver certifies
~79% of random triangle pairs with a maximum relative error of 2.4e-8, and the
remaining ~21% (parallel faces, touching, degenerate simplices) fall back to brute
force inside the hybrid.

**D2 — `dt_max` uses `h_min`, the thesis 7.1 text says `h_max`.** See §3; 7.2 is
consistent with the code.

**D3 — size rule.** 6.3/6.4.1 phrase the drop trigger as "particle diameter smaller
than 1/3 of the mesh edge"; `GridParams::size_slack = 1.0` encodes the equivalent
"cell edge ≥ 1 × diameter". With `refine_factor = 3` these differ by one level at the
boundary. Which one is used is visible in `GridStats::levels`.

**D4 — halo epsilon.** Thesis `e = 0.001` (so `r_halo = 1.001 r`); code `0.1` (so
`1.1 r`). Fixed in `Types.hpp`; only affects conservative filtering.

**D5 — `ScenarioSpec` size units.** `size_min`/`size_max` are interpreted as
**diameters** (radius = 0.5 · size), matching 7.3's "diameter of 0.02 vs 0.2" and the
`size_slack * diameter` grid rule. Any consumer that reads them as radii will
double every body.

---

## 6. Scenario generation: what it is and what it is not

`buildScenario` is a **lattice-jittered hard-sphere insertion**, not a deposition
simulation:

* the domain is cut into `nside³` cells, `nside₀ = floor(cbrt(count/2))`, so cells hold
  ~2 bodies on average;
* bodies are assigned to distinct cells from a deterministically shuffled list and
  jittered inside their cell (kept fully inside the cell, hence inside the domain);
* acceptance requires `|c_i - c_j| ≥ r_i + r_j` for every pair, checked with a uniform
  hash grid and re-verified exhaustively; up to 200 jitter attempts, then the lattice
  is grown (`nside += 1`), up to 8 growth steps, after which the scenario is reported
  as infeasible instead of silently overlapping;
* sizes are uniform in `[size_min, size_max]` (diameters); shapes are rebuilt per body
  from `seed` (so `RandomHull` bodies differ);
* `fill_fraction` is advisory — the achieved solid fraction is `Σ V_i / V_domain`,
  determined by `count` and the size range, and is *not* enforced;
* `gravity`, `gravity_g`, `ground_plane` are flags only: `buildScenario` never writes
  `Particle::force`; the driver applies them.

Differences from chapter 3: no hopper/funnel/floor geometry, no drop height, no
10–40-triangle particles, no mass-distribution study, no symmetry metrics. It produces
an initial configuration suitable for broadphase/narrowphase/dt benchmarks, which is
what chapters 5–7 measure.

`buildTwoBodyCollision` is the exception: it is the 7.3 initial condition verbatim
(see §1 and §2.6).

---

## 7. Benchmark columns → thesis figures

`tools/bench.py` consumes `mode,narrowphase,step,time,dt,contacts,candidate_pairs,
tri_pairs,contact_checks,kinetic,penetration,wall_time_ms`:

| CSV column | Thesis figure/metric |
|---|---|
| `candidate_pairs` | Figure 7.3 top, "# particle comparisons" |
| `dt`, `time` | Figure 7.3 middle, step size vs simulated time |
| `wall_time_ms` | Figure 7.3 bottom, "real time [s]" on a single core |
| `contacts`, `tri_pairs`, `contact_checks` | 5.4/5.6 cost model (15 primitive evaluations per triangle pair, fallback frequency) |
| `kinetic`, `penetration` | physics sanity checks of `SPEC.md` §4.5 (momentum/energy, `depth ~ m g / kn`) |

Reproduction commands:

```bash
g++ -std=c++17 -O2 -Iinclude src/scenario/*.cpp tests/test_scenario.cpp -o /tmp/test_scenario
/tmp/test_scenario

python3 tools/bench.py --help
python3 tools/bench.py 'runs/*.csv' --verify --histogram
python3 tools/bench.py 'runs/*.csv' --verify --plots runs/plots
```

---

## 8. Out of scope (explicit)

* Peano/spacetree data structure, SFC traversal order, vertex-based association (6.1,
  6.2, 6.4).
* Triangle-mesh parallelisation and particle-pair parallelisation (8.1, 8.2),
  producer–consumer grid parallelisation (8.3), grain-size study (8.4), asynchronous
  data exchange (Algorithms 15/16).
* SIMD/vectorised kernels and loop-footprint optimisation (5.8).
* GJK narrowphase of Appendix A.4 (Algorithm 14) — the iterative solver here is the
  penalty solver of 5.5.1 (Algorithm 5).
* Multi-contact manifolds, contact-point de-duplication, full inertia tensors.
* Hopper/floor triangulated geometry and the chapter 3 symmetry metrics.
