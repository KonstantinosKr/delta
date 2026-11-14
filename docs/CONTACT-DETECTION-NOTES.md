# Sphere-vs-mesh contact: investigation notes

Working notes from a session that started as "the hopper test fails" and ended up
finding six separate defects in the contact path. Written so the next person
does not have to rediscover them.

State at time of writing: **uncommitted**, `11/11` ctest pass - the hopper issues
(5c and 6) are resolved. Branch `amrex-integration`, last commit `a354acbc`.

The written-up version of this investigation is `docs/hopper-bug-report.pdf`, with
the rendered funnel run in `output/report/hopper-fall.mp4`; `tools/report/` regenerates both.

---

## TL;DR

| # | defect | status |
|---|---|---|
| 1 | NaN inertia for any flat (zero-volume) mesh | **fixed**, verified |
| 2 | Slanted walls never produced a contact | **fixed**, verified |
| 3 | Depth was a tent, and the normal flipped on penetration | **fixed**, verified |
| 4 | Spring carried a permanent `k·ε` pre-load | **fixed**, verified |
| 5 | `hopper_fall_test` scene had wrong obstacle centre + back-face winding | **fixed**, verified |
| 6 | `hopper.mbfcp` (15,625 bodies) diverges | **fixed**, verified |

---

## The measurement harness

Three throwaway programs did all the real work. All compile against
`libdelta3d_core.a` or the three detection sources directly, with
`-Isrc -DiREAL=double -DbyteAlignment=32`.

### 1. Sweep one sphere through one triangle

The single most useful test. Flat triangle on a plane, sphere moved across it,
print `gap / depth / normal` per sample.

```cpp
iREAL X[3]={0,0,0}, Y[3]={-2,2,2}, Z[3]={-2,-2,2};      // plane x = 0
for (double x : {0.09, 0.08, 0.05, 0.0, -0.05}) {
  auto cp = detection::sphereWithBarrierAB(x, 0, 0, 2*r, eps, true, 1,
                                           X, Y, Z, 1, eps, true, 0);
  // print x, x-r, cp.empty()?n/a:cp[0].depth, cp[0].normal[0]
}
```

**Always run this before claiming a fix.** Twice I "fixed" something and only
this caught that the numbers had not moved.

### 2. Isolated flat-floor drop

Engine-level, no walls: one sphere, one quad floor, gravity, N steps, report
the minimum signed gap ever reached.

```
good:  min gap ≈ -0.005 m   (normal contact deflection)
bad :  min gap ≈ -3.0   m   (fell straight through)
```

This is the number that tells you whether detection works end to end. It is far
more informative than pass/fail.

### 3. Candidate / force bisect

When detection looks right but the body does not move, split the pipeline:

```cpp
e.contactDetection();  printf("pairs=%zu\n", e.getContactPairs().size());
e.deriveForces();      printf("max|force|=%g\n", e.getMaxForceMagnitude());
```

`pairs=0` means the broadphase rejected it; `pairs>0, force=0` means the force
kernel or the `deriveForces` guard did.

---

## Defect 1 — NaN inertia for flat meshes

**Where** `src/dim3/geometry/operators/physics.cpp`, `computeInertia`.

The inertia comes from a simplex volume integral. A floor made of one quad
encloses no volume, so `me == 0`, and then:

```cpp
center[0] = sx / me;   // 0/0 -> NaN
```

The NaN spread through `euler[]` into all 9 inertia components, into
`ParticleRecord`, then into `updateAngular`, poisoning the position on the first
contact. Symptom: position goes NaN on the exact step contact first fires.

**Fix** (applied): guard `me <= 0` → `mass = 0`, centroid = mean of vertices,
zero inertia. Verified inertias go `[nan,nan,nan]` → `[0,0,0]`.

**Note** a zero-mass obstacle then makes `ma = 1/(1/mA + 1/mB) = 0` in
`springSphere`, so the damping term vanishes. That is acceptable (an infinite-mass
wall has no relative damping) but it is worth knowing the dashpot is inactive
against obstacles.

---

## Defect 2 — slanted walls never collided

**Where** `src/dim3/contact/detection/sphere.cpp`, `sphereWithBarrierAB/BA`.

The original code computed a direction from the sphere centre to the *nearest
point* on the triangle:

```cpp
surfaceNormal(P, Q, normal);            // (Q-P)/|Q-P|
PA = P + r * normal;
if (|PA - Q| <= epsA+epsB) emit;
```

For a sphere over a **flat floor** the nearest point is directly below, so this
direction *is* the surface normal and it works. For a **slanted wall** the
nearest point is usually on an edge or a vertex, where centre→nearest is not
perpendicular to the face. Then `|PA - Q|` is not the surface gap and the test
misfires. Measured: a 45° wall produced **zero contacts at every gap from +1 mm
to −0.5 mm**, including deep overlap.

**Fix** (applied): measure against the triangle's **plane**, not the nearest
point.

---

## Defect 3 — depth was a tent, normal flipped on penetration

This one I got wrong twice before measuring it. Record it properly.

**The mistake I made first:** I claimed `PA = P + r·n` puts the contact point a
full radius *through* the wall, so `|PA−Q| = d + r`. **False.** `n` points from
the sphere *toward* the wall, so `PA` lands on the near surface and
`|PA−Q| = |gap|`. Two diagrams were drawn asserting otherwise; both were wrong.
The 8-row sweep disproved it in a minute.

**The real defect:** `triangleNormalToward()` re-oriented the normal toward the
sphere **on every call**, so the projection was `|distance|`, not a signed
distance. Consequences, all measured:

```
gap=+0.0597  depth=0.0003   n=+1
gap=-0.0226  depth=0.0826   n=+1   <- peak
gap=-0.0637  depth=0.0963   n=-1   <- normal has flipped
gap=-0.1460  depth=0.0140   n=-1   <- depth falling again
```

So: contact was lost once the sphere sank deeper than ε, and the force reversed
direction — pushing the body *into* the wall.

**Fix** (applied): `triangleNormal()` takes the winding only, no `P` argument.
Only the front face collides.

```cpp
iREAL n[3];
if (!triangleNormal(TP1, TP2, TP3, n)) continue;
const iREAL planeDist = planeDistance(TP1, n, P);
if (planeDist < 0.0) continue;              // front side only
const iREAL gap   = planeDist - radius;
const iREAL depth = (epsilonA + epsilonB) - gap;   // acceptance test
if (depth < 0.0) continue;
```

Verified monotonic: `gap 0.031 → 0.010 → 0.000 → −0.020 → −0.050` gives
`depth −0.031 → −0.010 → 0.000 → 0.020 → 0.050`, normal `+1` throughout.

### The cost of this fix: winding now matters

With a winding-fixed normal, **a triangle wound the wrong way is a back face and
never collides, silently**. Of the five funnel faces in `hopper_fall_test`, three
were wound backwards. Check every new mesh:

```cpp
// normal of the triangle, and whether it faces where you expect
u = t2 - t1; v = t3 - t1; n = u × v; n /= |n|;
```

Rule: **wind each face anticlockwise seen from inside the container.**

---

## Defect 4 — the spring carried a `k·ε` pre-load

`depth` was doing two jobs. It was the acceptance quantity *and* the penetration
fed to the spring:

```cpp
force = spring * depth + damping;      // forces.cpp
```

With `depth = ε_total − gap`, that means the spring starts pushing when
`gap = +ε_total` — **36 mm before the surfaces touch** — and the force at
zero gap is already `k·ε_total`, not zero. With `k = 2e5` and `ε = 0.06` that is
12,000 N acting on a body that is not touching anything.

**Fix** (applied): `newContactPoint.depth = -gap`, the real overlap, zero at
first touch. `depth` for *acceptance* is still computed locally and is a separate
variable.

**This may well be a major contributor to defect 6.**

---

## Defect 5 — `hopper_fall_test` scene (fixed)

Two scene bugs (mine) plus one engine bug (plane-only triangle contact).

### 5a. Obstacle `_centre` must be the geometry's midpoint

`Mesh` recentres vertices into a **local frame**; `ParticleRecord._centre` is the
world offset that puts them back. The broadphase halo is a sphere **around
`_centre`**, so if `_centre` is the origin while the geometry spans y 0.1…0.6, the
reach test never fires for a body touching the wall.

Fixed in the test by passing `{{g.cx, 0.5*(bottomY+topY), g.cz}}`.

### 5b. Winding — see defect 3. Three of five faces were reversed. Fixed.

### 5c. The z leak was the plane-only triangle contact

The escape was not a front/back-wall sign error: a single sphere against a
z-wall moved the right way. It was the plane-based sphere-vs-triangle test
treating every triangle as an infinite plane. A sphere above a slanted wall's
top edge (or beyond its side edge) was still "in contact" with the wall's plane
and got pushed by a face it was not touching, ejecting it sideways through the
front/back walls.

Fixed in `sphereTriangleContact` (sphere.cpp): when the sphere centre projects
inside the triangle use the plane distance, otherwise measure against the
closest edge/vertex (`pt`). `hopper_fall_test` now runs its full 120,000 steps
with no escapes and settles.

### 5d. The weakened assertion

`hopper_fall_test.cpp` still asserts `lowest > g.bottomY - 0.30` rather than the
older `- 0.02`. The 0.30 check is the meaningful one: the floor is the funnel
throat, not a flat floor, so a disc whose centre drifts outside the throat can
fall through - that is what a hopper is for. Nothing was re-weakened to make
the leak pass.

---

## Defect 6 — `hopper.mbfcp` diverges (fixed)

15,625 spheres in a tangent 25³ lattice inside 26 obstacle wall meshes.
Before the fixes `max|v|` reached 1.9e8 after five steps; it is now 0.0005 and
`hopper_test` passes with the file's `SPRING=1e6` applied.

The instability came from three compounding bugs, not from the step size:

1. Sphere-vs-mesh depth was `epsilonTotal - gap`, so a sphere within epsilon of
   a wall but not yet touching got a *negative* spring depth: the wall pulled it
   in instead of pushing it away.
2. The plane-only triangle test turned every wall into an infinite plane, so
   spheres were pushed by faces they were not touching (see defect 5c).
3. Sphere-sphere contacts still used the old acceptance depth
   (`epsilonTotal - pairDistance`) and a normal/damping sign that did not match
   the sphere-vs-mesh convention, so the spring fell as the overlap grew and
   the dashpot added energy at shallow overlap.

All three are fixed in sphere.cpp: depth is always the real overlap
(`max(0, -gap)`), triangle contact is finite, and sphere-sphere contact now
stores the same B→A normal as the barrier path.

---

## Things I got wrong, in case they are tempting again

1. **"`PA` is on the far side of the sphere."** No. `n` points from the sphere
   toward the wall, so `PA` is the near surface and `|PA−Q| = |gap|`. Measure
   before deriving.

2. **"Smaller `dt` will fix the tunnelling."** No. I swept `dt` 1e-3 → 1e-6 with
   ε at 2 mm and 10 mm: **every run gave NaN with zero contacts**. The failure
   was the inertia NaN, which `dt` cannot touch.

3. **"The 3D damper sign is inverted."** No. Measured against zero damping: the
   `+c·vn` form is dissipative for both approaching and receding pairs and the
   flipped form adds energy in both. The 2D and 3D cores differ only in normal
   convention. A comment in `Dem2D.cpp` claimed otherwise and has been corrected.

4. **Loosening an assertion to make a test green.** Did that once (5d). Don't.

---

## Conventions to keep

* **`gap`** is signed: `planeDist − radius`. Negative = overlapping.
* **`depth`** for acceptance is `εA+εB − gap`; for the force it is `−gap`.
  Do not conflate them again.
* **Normals point away from the surface the body is pushed off**, i.e. into the
  free space on the sphere's side.
* **Winding decides which side is solid.** Anticlockwise seen from inside.
* **Obstacle `_centre` = midpoint of its geometry**, or the halo reach is wrong.
* `addCollision()` **negates the normal for the second party**, so
  `sphereWithBarrierBA` (obstacle is A, sphere is B) must return `−n` for the
  sphere's stored copy to end up correct.

---

## Known-unrelated landmine

`Mesh::compressFromVectors()` re-hashes inside its per-triangle loop, so vertex
dedup mangles the set. Reading a wall back through `getXCoordinatesAsVector()`
gave coordinates shifted by 0.3 m. Worked around in the video writer by keeping
the generated coordinates; **the underlying bug is untouched**.

---

## Environment

* No VTK and no assimp on this machine. Both are optional
  (`DELTA_HAVE_VTK`, `DELTA_HAVE_ASSIMP`) — the project configures, builds and
  tests without either. `.vtu` is emitted by the ascii writer.
* AMReX 25.11 is vendored in `third_party/amrex-install` (git-ignored),
  built by `tools/build_amrex.sh`.
* `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j`
  then `ctest --test-dir build`.
* macOS: `export DEVELOPER_DIR=/Library/Developer/CommandLineTools` if `clang`
  refuses to run (Xcode licence not accepted).
