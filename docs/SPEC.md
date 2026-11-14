# Replication spec — thesis → AMReX

Source: K. Krestenitis, *Parallel Multiscale Contact Dynamics for Rigid Non-spherical
Bodies*, PhD thesis, Durham University, 2018 (184 pp.).
Local text: `/tmp/thesis.txt` (page markers `===== PAGE n =====`).

This file is the contract all modules implement. Keep the thesis vocabulary.

## 0. Common conventions

* Domain: unit cube `[0,1]^3`, AMReX `Geometry` with `BoxArray` covering it.
* `Particle::radius` is the **real geometry bounding radius** `rr` (thesis 7.2);
  `Particle::halo = 1.1 * radius` is the epsilon-expanded halo used by the
  broadphase pre-check and by the dt scheme.
* All geometry is in **world frame** when the narrowphase runs: transform body
  vertices with `Particle::rotate(quat, v) + pos` (`Particle::worldVertex`).
* Canonical pair ordering `i < j` everywhere.
* No global state; one `GridHierarchy`, one `Narrowphase`, one `ContactDynamics`
  per simulation, all deterministic for a fixed seed.

## 1. Broadphase: four grid morphologies (thesis 6.3)

Level 0 cell edge is the domain edge (1.0); level `L` edge is
`h_L = h_0 / refine_factor^L` with `refine_factor = 3` (9 children in 2D,
27 in 3D). `max_level` is configurable (default 4).

**Body → level rule (thesis 6.1/6.3):** a body resides on the finest level `L`
such that `h_L >= size_slack * diameter`; if none qualifies, `L = max_level`.
This is the "drop" of 6.4.1. Bodies never sit in a cell smaller than themselves
(7.1, condition i).

### NoGrid
One mono-cell (thesis 6.3, "deteriorated space grid"): a single `Box` covering
the domain, all bodies binned there. Candidate pairs = all `N(N-1)/2` pairs.
This is the reference/oracle broadphase.

### Regular
Uniform refinement: every cell of every level `0..L_max` exists, where `L_max`
is the level of the *smallest* body (`size_min`). Bodies are binned on their own
level. Candidate pairs: within a level, a body is compared against bodies in the
same cell and the 13 forward half-stencil neighbour cells (26-neighbourhood,
each unordered cell pair visited once). Bodies on different levels are compared
via virtual inheritance (below). Cells with no bodies still exist and are still
traversed — that is the cost this variant pays.

### Adaptive
Only cells that matter exist. Build level by level:
1. Bin bodies on their natural level (rule above).
2. Mark a cell "active" when it holds >= 1 body (cell of the body's centre).
3. A coarse body must be visible to finer levels: every active cell at level
   `L+1` inherits as *virtual* every body from level `<= L` whose AABB (real
   radius) overlaps the cell's parent at level `L` (6.4.2, downward only).
4. Empty child cells are not created; if a level has no active cells it is not
   created at all (`refinements`/`coarsenings` count the deltas vs the previous
   build).

Candidate pairs per level: real-vs-real (same cell + half stencil) and
real-vs-virtual (the virtual body compared against occupants of the cell that
inherited it). Coarse-vs-coarse pairs are produced at the coarse level, never
duplicated at fine levels: a pair is emitted at the level of its *finest* member.

### Reluctant
Adaptive, with the refinement criterion of 6.3 replaced by:

> Refine the neighbourhood of a body only when the cell holds **at least two**
> bodies that are **approaching**, i.e. `v_BA < 0` with
> `v_BA = (v_A · d_BA) - (v_B · d_BA)`, `d_BA = c_B - c_A`.

Concretely, for each cell at each level: if it holds < 2 bodies, or all body
pairs in the cell+neighbourhood are separating (`v_BA >= 0`), the cell is *not*
refined (`cells_refused++`), and its bodies stay on the coarser level — even if
they are small enough to be dropped further. A cell is refined when it holds a
pair with `v_BA < 0` and `gap < approach_gap * h_L` (gap = centre distance -
radii sum). This keeps the grid as coarse as possible so `h_min` (and therefore
`dt_max`, section 3) stays large.

### Candidate pair contract
`GridHierarchy::candidatePairs()` returns each unordered pair **at most once**,
sorted by `(i, j)`, and it must be a **superset of the true contact set**
(no false negatives) in every mode. Broadphase may be conservative (false
positives are fine, they are filtered by the halo-sphere test in the
narrowphase). `stats()` must report `raw_pairs`, `unique_pairs`, `cell_pair_checks`,
`virtual_links`, `cells_total`, `cells_occupied`, `levels`, `refinements`,
`coarsenings`, `cells_refused`.

## 2. Narrowphase (thesis 5.4–5.6)

**Brute force (Algorithm 2).** For triangles `TA=(A,B,C)`, `TB=(D,E,F)`:
6 point-to-triangle distances (each vertex of one triangle against the other
triangle) + 9 segment-to-segment distances (each edge pair) = **15 geometric
evaluations**; the minimum is the answer; the pair `(P,Q)` of closest points is
returned. Robust by construction, cannot be vectorised (branching).

**Iterative / penalty solver (Algorithm 5, "An Iterative Method").** The
thesis solves the triangle distance as a constrained optimisation: barycentric
unknowns `x=(a,b,g,d)`, `x_pt = A + BA·a + CA·b`, `y_pt = D + ED·g + FD·d`,
minimise `f = |x_pt - y_pt|^2` subject to the six constraints
`{-a, -b, a+b-1, -g, -d, g+d-1} <= 0`. A penalty term
`P = f + rho·Σ max(0,h)^2` (eq. 5.1) is minimised by Newton iterations on the
perturbed Hessian `H = hf + rho·Dmax^T·Dmax + eps·I` (`hf` as in Algorithm 5,
`eps` regularises the singular/ill-conditioned Hessian), with a 4×4 Gaussian
elimination for the Newton direction, starting from `(1/3,1/3,1/3,1/3)`.
`certified == false` when the iteration cap (8) is hit or the solution leaves
the feasible region beyond penalty slack — that is the hybrid's fallback trigger.
`rho ≈ 1e10·s^2`, `eps ≈ 1e-6·s^2`, `tol ≈ 1e-14·s^2` with `s` the mean edge
length (thesis 5.5.2 tuning). Because the penalty allows a small excursion
outside the feasible set, the solver may return a slightly *shorter* distance
than the true one — this is expected and is why brute force remains the oracle.

**Hybrid (5.6).** Run the iterative solver first; if it does not certify, fall
back to brute force **for that pair** (`TriDist::fallback = true`,
`NarrowphaseStats::fallbacks++`). The hybrid must return bit-identical distances
to brute force in the unit tests (that is the robustness claim) while using the
iterative path for the majority of pairs.

**Contact decision.** Bodies `a,b` contact when the minimum triangle-pair
distance `d < margin` (default `0.0`, i.e. touching counts). Normal =
`normalized(p - q)` pointing from `a` to `b` (flip when `d == 0` and the
centroids decide the side). Contact point = midpoint of `(p,q)`.
`depth = margin - d` (>= 0).

Bounding-sphere filter before any triangle work (7.2: halos are an explicit
pre-check): reject when `|c_b - c_a| > halo_a + halo_b + margin`.

## 3. Contact response and time integration

**Penalty model (2.3).** Normal force magnitude `fn = kn * depth` (linear
penalty spring), clamped to `fn >= 0`; tangential force from the relative
tangential velocity with `ft = clamp(-kt * |vt| * dt - gamma_t * vt, -mu*fn, mu*fn)`.
Apply equal-and-opposite forces at the contact point, split into centre force
and torque `r x f`. `SpringDashpot` uses
`fn = kn*depth - gamma_n * v_n` (restitution-controlled) instead.
Under-relaxation is not allowed to change the sign of `fn`.

**Integration (5.1).** Semi-implicit Euler:
`v += dt * f/m`, `x += dt * v`, `omega += dt * tau/I`,
`q += dt * 0.5 * omega_quat * q`, renormalise `q`.

**Dynamic Δt (chapter 7, Algorithms 11–12).**
* `inc = 1.1`, `dt_min = 1e-4`, `dt_max = h_min / (2 * inc * v_max)`.
* Pair state from the sign of `v_BA` (separation when `>= 0`).
* A **critical approach** is predicted by extrapolating both bodies by
  `pdt = dt * (1 + inc)`; if the halos then overlap, restrict
  `dt_local = 0.5 * gap / |v_BA|` (guard `|v_BA| > eps`).
* Global `dt` at the end of the step: if any critical pair, `dt = min(dt_local)`
  (never below `dt_min`); else `dt *= inc`; finally `dt = min(dt, dt_max)`.
* The dt must additionally respect the no-tunnelling rule of 7.1
  (iii): `v_max * dt <= 0.5 * h_min`.
* The `NoGrid` mono-cell has no meaningful cell cutoff, so its `h_min` for the
  step bound is taken as the smallest body diameter (`2 * min(radius)`). This
  keeps the oracle mode physically safe and comparable with the gridded modes
  instead of letting it take arbitrarily large steps.

## 4. Validation protocol

1. **Narrowphase unit tests** (no AMReX): analytic configurations with known
   distances (vertex-face, edge-edge, parallel, coplanar, touching, penetrating,
   degenerate triangles) — brute force must match the analytic value to 1e-12,
   hybrid must match brute force exactly.
2. **Broadphase equivalence**: for every scenario/grid mode, the set of
   *contacts* (not just candidate pairs) after the narrowphase must equal the
   NoGrid+Brute reference set, `id_a,id_b` exact. Any missing contact is a hard
   failure; extra candidates are recorded as broadphase efficiency.
3. **Determinism**: same seed + same mode ⇒ identical diagnostics and contact
   counts, run twice.
4. **Performance**: report `unique_pairs`, `tri_pairs`, `contacts` per mode for
   the same scenario; the adaptive/reluctant modes must reduce candidate pairs
   versus NoGrid, and the reflected `dt` histogram must show the reluctant mode
   taking fewer, larger steps than the adaptive mode for the two-body test.
5. **Physics sanity**: two-body head-on collision conserves linear momentum to
   1e-10 and does not increase energy; resting contact does not sink through the
   floor beyond `depth ~ m*g/kn`.
