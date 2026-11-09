// Triangle-to-triangle narrowphase distance kernels.
//
// Replicates thesis chapter 5 (Krestenitis 2018):
//   * Algorithm 2  brute force: 6 point-to-triangle + 9 segment-to-segment
//   * Algorithm 3  SEGSEG, parametric (s,t) box clamping
//   * Algorithm 4  point-to-triangle, 7-region (s,t) solve (thesis Figure 5.2)
//   * Algorithm 5  ITERATIVE = penalty-based constrained Newton solver
//                  (section 5.5.1, eq. 5.1, pseudocode p.69).  This is *not*
//                  GJK/simplex: the unknowns are the four barycentric
//                  coordinates (a,b,g,d) of the two closest points and the
//                  objective is the penalised squared distance.
//   * section 5.6  hybrid: iterative first, brute-force fallback
//
// Feature encoding used by TriDist::feature_a / feature_b:
//   0..2  vertex of that triangle (0 = first vertex)
//   3..5  edge of that triangle (3 = v0-v1, 4 = v1-v2, 5 = v2-v0)
//   -1    not classified (face interior for PT, or the iterative solver,
//         which works in barycentric space and does not classify features)
//
// Two conditioning thresholds are used:
//   * corner angle below 1e-6 rad  -> the 7-region discriminants of Algorithm 4
//     have cancelled, so the robust candidate solve (vertices / clamped edges /
//     plane projection) is used instead.  The answer is then exact to the
//     triangle's own thickness.
//   * area == 0 (below 1e-14 of the longest edge squared) -> the triangle *is*
//     a segment; the candidate solve is exact and the overlap predicate uses
//     segment-vs-triangle tests.
//
// Brute force necessarily also answers the boolean intersection question:
// the 15 primitives give the correct distance only for *disjoint* pairs.  A
// pair that pierces one another (an edge of one crossing the interior of the
// other) has distance 0 while every one of the 15 primitives is positive, so
// an explicit triangle-triangle overlap test (Moller-style, plus a coplanar
// branch) is run as well and reports 0 / overlap.  This does not change the
// 15-primitive distance evaluation, it only decides the `overlap` flag and
// the distance-0 case.

#include "contact/Narrowphase.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

namespace contact {
namespace {

constexpr Real kInf = std::numeric_limits<Real>::max();

inline Real safeDiv(Real n, Real d) noexcept { return (d != 0.0) ? n / d : 0.0; }

inline Real maxEdgeLen2(const Triangle& t) noexcept {
    return std::max({norm2(t.v[1] - t.v[0]), norm2(t.v[2] - t.v[1]), norm2(t.v[0] - t.v[2])});
}

inline Real triScale(const Triangle& t) noexcept { return std::sqrt(maxEdgeLen2(t)); }

// Mean edge length of the two triangles (thesis eq. 5.2 uses the average side
// length to scale the penalty parameters).
inline Real meanEdgeLength(const Triangle& A, const Triangle& B) noexcept {
    Real s = 0.0;
    s += norm(A.v[1] - A.v[0]) + norm(A.v[2] - A.v[1]) + norm(A.v[0] - A.v[2]);
    s += norm(B.v[1] - B.v[0]) + norm(B.v[2] - B.v[1]) + norm(B.v[0] - B.v[2]);
    return s / 6.0;
}

// Twice the triangle area.
inline Real doubleArea(const Triangle& t) noexcept {
    return norm(cross(t.v[1] - t.v[0], t.v[2] - t.v[0]));
}

inline bool isDegenerate(const Triangle& t) noexcept {
    const Real l2 = maxEdgeLen2(t);
    if (!(l2 > 0.0)) return true;
    const Real a2 = doubleArea(t);
    return a2 * a2 <= 1e-28 * l2 * l2;
}

// Robust fallback solve: minimum over the three vertices, the three clamped
// edge projections and (when it falls inside) the plane projection.  Used for
// segment-like triangles, where the 7-region discriminants of Algorithm 4 are
// differences of products that cancel and can select the wrong region.
Real pointTriangleCandidates(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c,
                             Vec3& closest, Real lam[3]) noexcept {
    Real best = kInf;
    auto consider = [&](const Vec3& x, Real l0, Real l1, Real l2v) {
        const Real d2 = norm2(p - x);
        if (d2 < best) {
            best = d2;
            closest = x;
            lam[0] = l0;
            lam[1] = l1;
            lam[2] = l2v;
        }
    };
    consider(a, 1.0, 0.0, 0.0);
    consider(b, 0.0, 1.0, 0.0);
    consider(c, 0.0, 0.0, 1.0);
    for (int e = 0; e < 3; ++e) {
        const Vec3 s0 = (e == 0) ? a : ((e == 1) ? b : c);
        const Vec3 s1 = (e == 0) ? b : ((e == 1) ? c : a);
        const Vec3 d = s1 - s0;
        const Real dd = norm2(d);
        const Real t = (dd > 0.0) ? clamp(dot(p - s0, d) / dd, 0.0, 1.0) : 0.0;
        const Vec3 x = s0 + d * t;
        if (e == 0)      consider(x, 1.0 - t, t, 0.0);
        else if (e == 1) consider(x, 0.0, 1.0 - t, t);
        else             consider(x, t, 0.0, 1.0 - t);  // edge c -> a
    }
    // Plane projection, accepted only when its barycentric weights are inside
    // the triangle.  For a needle the weights are ill-conditioned; a wild
    // solution is rejected and the boundary candidates (which are exact to the
    // needle width) win.
    const Vec3 n = cross(b - a, c - a);
    const Real nn = norm2(n);
    if (nn > 0.0) {
        const Vec3 proj = p - n * (dot(p - a, n) / nn);
        const Real v = dot(cross(proj - a, c - a), n) / nn;
        const Real w = dot(cross(b - a, proj - a), n) / nn;
        const Real tol = 1e-9;
        if (v >= -tol && w >= -tol && (v + w) <= 1.0 + tol) {
            consider(proj, 1.0 - v - w, v, w);
        }
    }
    return std::sqrt(best);
}

// ---------------------------------------------------------------------------
// Algorithm 4: point to triangle, 7 regions of thesis Figure 5.2.
// Returns the distance and writes the closest point plus its barycentric
// coordinates (lam[0..2] for a, b, c).
// ---------------------------------------------------------------------------
Real pointTriangleClosest(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c,
                          Vec3& closest, Real lam[3]) noexcept {
    const Vec3 ab = b - a;
    const Vec3 ac = c - a;
    const Vec3 ap = p - a;

    const Real l2 = std::max({norm2(ab), norm2(ac), norm2(c - b)});
    const Real a2 = norm(cross(ab, ac));

    // Segment-like triangle (corner angle below 1e-6 rad, or zero area): the
    // 7-region discriminants va/vb/vc have lost their significant digits, so
    // the robust candidate solve is used instead.  For every well conditioned
    // triangle the exact 7-region code below is used.
    if (!(l2 > 0.0) || a2 <= 1e-6 * l2) {
        return pointTriangleCandidates(p, a, b, c, closest, lam);
    }

    const Real d1 = dot(ab, ap);
    const Real d2 = dot(ac, ap);
    if (d1 <= 0.0 && d2 <= 0.0) {  // vertex a
        closest = a;
        lam[0] = 1.0; lam[1] = 0.0; lam[2] = 0.0;
        return norm(p - a);
    }

    const Vec3 bp = p - b;
    const Real d3 = dot(ab, bp);
    const Real d4 = dot(ac, bp);
    if (d3 >= 0.0 && d4 <= d3) {  // vertex b
        closest = b;
        lam[0] = 0.0; lam[1] = 1.0; lam[2] = 0.0;
        return norm(p - b);
    }

    const Real vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0 && d1 >= 0.0 && d3 <= 0.0) {  // region 3: edge ab
        const Real v = safeDiv(d1, d1 - d3);
        closest = a + ab * v;
        lam[0] = 1.0 - v; lam[1] = v; lam[2] = 0.0;
        return norm(p - closest);
    }

    const Vec3 cp = p - c;
    const Real d5 = dot(ab, cp);
    const Real d6 = dot(ac, cp);
    if (d6 >= 0.0 && d5 <= d6) {  // vertex c
        closest = c;
        lam[0] = 0.0; lam[1] = 0.0; lam[2] = 1.0;
        return norm(p - c);
    }

    const Real vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0 && d2 >= 0.0 && d6 <= 0.0) {  // region 5: edge ac
        const Real w = safeDiv(d2, d2 - d6);
        closest = a + ac * w;
        lam[0] = 1.0 - w; lam[1] = 0.0; lam[2] = w;
        return norm(p - closest);
    }

    const Real va = d3 * d6 - d5 * d4;
    if (va <= 0.0 && (d4 - d3) >= 0.0 && (d5 - d6) >= 0.0) {  // edge bc
        const Real w = safeDiv(d4 - d3, (d4 - d3) + (d5 - d6));
        closest = b + (c - b) * w;
        lam[0] = 0.0; lam[1] = 1.0 - w; lam[2] = w;
        return norm(p - closest);
    }

    const Real denom = va + vb + vc;
    if (!(denom > 0.0) || !std::isfinite(denom)) {
        // Numerically degenerate: minimum over the boundary.
        Real best = norm2(p - a);
        closest = a;
        lam[0] = 1.0; lam[1] = 0.0; lam[2] = 0.0;
        auto consider = [&](const Vec3& x, Real l0, Real l1, Real l2v) {
            const Real d2v = norm2(p - x);
            if (d2v < best) { best = d2v; closest = x; lam[0] = l0; lam[1] = l1; lam[2] = l2v; }
        };
        for (int e = 0; e < 3; ++e) {
            const Vec3 s0 = (e == 0) ? a : ((e == 1) ? b : c);
            const Vec3 s1 = (e == 0) ? b : ((e == 1) ? c : a);
            const Vec3 d = s1 - s0;
            const Real dd = norm2(d);
            const Real t = (dd > 0.0) ? clamp(dot(p - s0, d) / dd, 0.0, 1.0) : 0.0;
            const Vec3 x = s0 + d * t;
            if (e == 0)      consider(x, 1.0 - t, t, 0.0);
            else if (e == 1) consider(x, 0.0, 1.0 - t, t);
            else             consider(x, t, 0.0, 1.0 - t);
        }
        return std::sqrt(best);
    }

    const Real v = vb / denom;  // region 0: face interior
    const Real w = vc / denom;
    closest = a + ab * v + ac * w;
    lam[0] = 1.0 - v - w;
    lam[1] = v;
    lam[2] = w;
    return norm(p - closest);
}

// ---------------------------------------------------------------------------
// Triangle-triangle overlap predicate (used only for the boolean result).
// ---------------------------------------------------------------------------
struct Plane {
    Vec3 n{};     // not normalised
    Real d{0.0};  // n . x + d = 0
};

inline Real planeSigned(const Plane& pl, const Vec3& p) noexcept { return dot(pl.n, p) + pl.d; }

inline Plane planeOf(const Triangle& t) noexcept {
    Plane pl;
    pl.n = cross(t.v[1] - t.v[0], t.v[2] - t.v[0]);
    pl.d = -dot(pl.n, t.v[0]);
    return pl;
}

// Unit-normal plane: signed distances are then real lengths and can be
// compared against a length epsilon.
inline Plane unitPlaneOf(const Triangle& t) noexcept {
    Plane pl = planeOf(t);
    const Real l = norm(pl.n);
    if (l > 0.0) {
        pl.n = pl.n / l;
        pl.d = pl.d / l;
    }
    return pl;
}

// A triangle whose corner angle is below 1e-9 rad has a plane normal that is
// only good to ~eps/sin = 1e-7; the plane/interval intersection test is not
// trustworthy there.  Such a triangle is a segment to within 1e-9 of its own
// edge length, so the overlap predicate abstains and the segment-based tests
// decide instead.
inline bool planeUnreliable(const Triangle& t) noexcept {
    const Real l2 = maxEdgeLen2(t);
    if (!(l2 > 0.0)) return true;
    return doubleArea(t) <= 1e-9 * l2;
}

inline Real cross2(Real ax, Real ay, Real bx, Real by) noexcept { return ax * by - ay * bx; }

bool pointInTri2D(const Real p[2], const Real a[2], const Real b[2], const Real c[2], Real eps) noexcept {
    const Real d1 = cross2(b[0] - a[0], b[1] - a[1], p[0] - a[0], p[1] - a[1]);
    const Real d2 = cross2(c[0] - b[0], c[1] - b[1], p[0] - b[0], p[1] - b[1]);
    const Real d3 = cross2(a[0] - c[0], a[1] - c[1], p[0] - c[0], p[1] - c[1]);
    return (d1 >= -eps && d2 >= -eps && d3 >= -eps) || (d1 <= eps && d2 <= eps && d3 <= eps);
}

// Collect the points of `t` that lie on `pl` (1 or 2 for a straddling
// triangle, 3 when the whole triangle is in the plane).
int cutAgainstPlane(const Triangle& t, const Plane& pl, Real eps, Vec3 out[3]) noexcept {
    Real sd[3];
    for (int i = 0; i < 3; ++i) sd[i] = planeSigned(pl, t.v[i]);
    int n = 0;
    for (int i = 0; i < 3 && n < 3; ++i) {
        const int j = (i + 1) % 3;
        if (sd[i] * sd[j] < 0.0) {
            const Real s = sd[i] / (sd[i] - sd[j]);
            out[n++] = t.v[i] + (t.v[j] - t.v[i]) * s;
        } else if (std::abs(sd[i]) <= eps) {
            out[n++] = t.v[i];
        }
    }
    return n;
}

bool coplanarOverlap(const Triangle& A, const Triangle& B, const Vec3& n, Real eps, Vec3& point) noexcept {
    int drop = 0;
    Real best = std::abs(n[0]);
    for (int k = 1; k < 3; ++k) {
        if (std::abs(n[k]) > best) { best = std::abs(n[k]); drop = k; }
    }
    const int i0 = (drop + 1) % 3;
    const int i1 = (drop + 2) % 3;
    Real a[3][2], b[3][2];
    for (int i = 0; i < 3; ++i) {
        a[i][0] = A.v[i][i0]; a[i][1] = A.v[i][i1];
        b[i][0] = B.v[i][i0]; b[i][1] = B.v[i][i1];
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            const int i2 = (i + 1) % 3;
            const int j2 = (j + 1) % 3;
            const Real d1 = cross2(a[i2][0] - a[i][0], a[i2][1] - a[i][1],
                                   b[j][0] - a[i][0], b[j][1] - a[i][1]);
            const Real d2 = cross2(a[i2][0] - a[i][0], a[i2][1] - a[i][1],
                                   b[j2][0] - a[i][0], b[j2][1] - a[i][1]);
            const Real d3 = cross2(b[j2][0] - b[j][0], b[j2][1] - b[j][1],
                                   a[i][0] - b[j][0], a[i][1] - b[j][1]);
            const Real d4 = cross2(b[j2][0] - b[j][0], b[j2][1] - b[j][1],
                                   a[i2][0] - b[j][0], a[i2][1] - b[j][1]);
            if (((d1 > eps && d2 < -eps) || (d1 < -eps && d2 > eps)) &&
                ((d3 > eps && d4 < -eps) || (d3 < -eps && d4 > eps))) {
                const Real s = d1 / (d1 - d2);
                point = A.v[i] + (A.v[i2] - A.v[i]) * s;
                return true;
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (pointInTri2D(a[i], b[0], b[1], b[2], eps)) { point = A.v[i]; return true; }
    }
    for (int i = 0; i < 3; ++i) {
        if (pointInTri2D(b[i], a[0], a[1], a[2], eps)) { point = B.v[i]; return true; }
    }
    return false;
}

inline int dropAxis(const Vec3& n) noexcept {
    int d = 0;
    Real b = std::abs(n[0]);
    for (int k = 1; k < 3; ++k) {
        if (std::abs(n[k]) > b) { b = std::abs(n[k]); d = k; }
    }
    return d;
}

// 3D point against a triangle, projected along the triangle normal.
bool pointInTri3D(const Vec3& p, const Triangle& T, const Vec3& n, Real eps) noexcept {
    const int drop = dropAxis(n);
    const int i0 = (drop + 1) % 3;
    const int i1 = (drop + 2) % 3;
    const Real pp[2] = {p[i0], p[i1]};
    Real a[2], b[2], c[2];
    a[0] = T.v[0][i0]; a[1] = T.v[0][i1];
    b[0] = T.v[1][i0]; b[1] = T.v[1][i1];
    c[0] = T.v[2][i0]; c[1] = T.v[2][i1];
    return pointInTri2D(pp, a, b, c, eps);
}

// Does the segment [p0,p1] touch/intersect the triangle T?  Exact for a
// segment, which is what a degenerate triangle reduces to; used when the
// plane/interval test cannot be trusted.
bool segmentTriOverlap(const Vec3& p0, const Vec3& p1, const Triangle& T, Real scale) noexcept {
    const Plane pl = unitPlaneOf(T);
    if (!(norm(pl.n) > 0.0)) return false;
    const Real eps = 1e-12 * scale;
    const Real s0 = planeSigned(pl, p0);
    const Real s1 = planeSigned(pl, p1);
    if (s0 > eps && s1 > eps) return false;
    if (s0 < -eps && s1 < -eps) return false;

    const Real areaEps = 1e-14 * scale * scale;
    if (std::abs(s0) <= eps && std::abs(s1) <= eps) {
        // Segment lies in the triangle plane: 2D test.
        const int drop = dropAxis(pl.n);
        const int i0 = (drop + 1) % 3;
        const int i1 = (drop + 2) % 3;
        Real q0[2] = {p0[i0], p0[i1]};
        Real q1[2] = {p1[i0], p1[i1]};
        Real a[2], b[2], c[2];
        a[0] = T.v[0][i0]; a[1] = T.v[0][i1];
        b[0] = T.v[1][i0]; b[1] = T.v[1][i1];
        c[0] = T.v[2][i0]; c[1] = T.v[2][i1];
        if (pointInTri2D(q0, a, b, c, areaEps) || pointInTri2D(q1, a, b, c, areaEps)) return true;
        Real tri[3][2];
        for (int k = 0; k < 2; ++k) { tri[0][k] = a[k]; tri[1][k] = b[k]; tri[2][k] = c[k]; }
        for (int e = 0; e < 3; ++e) {
            const Real* r0 = tri[e];
            const Real* r1 = tri[(e + 1) % 3];
            const Real d1 = cross2(q1[0] - q0[0], q1[1] - q0[1], r0[0] - q0[0], r0[1] - q0[1]);
            const Real d2 = cross2(q1[0] - q0[0], q1[1] - q0[1], r1[0] - q0[0], r1[1] - q0[1]);
            const Real d3 = cross2(r1[0] - r0[0], r1[1] - r0[1], q0[0] - r0[0], q0[1] - r0[1]);
            const Real d4 = cross2(r1[0] - r0[0], r1[1] - r0[1], q1[0] - r0[0], q1[1] - r0[1]);
            if (((d1 > areaEps && d2 < -areaEps) || (d1 < -areaEps && d2 > areaEps)) &&
                ((d3 > areaEps && d4 < -areaEps) || (d3 < -areaEps && d4 > areaEps))) {
                return true;
            }
        }
        return false;
    }

    Vec3 x;
    if (std::abs(s0) <= eps) x = p0;
    else if (std::abs(s1) <= eps) x = p1;
    else x = p0 + (p1 - p0) * (s0 / (s0 - s1));
    return pointInTri3D(x, T, pl.n, areaEps);
}

// Does any edge of one triangle touch the other?  Complete for convex sets,
// and the only option when a triangle is (numerically) a segment.
bool edgeWiseOverlap(const Triangle& A, const Triangle& B, Real scale) noexcept {
    for (int e = 0; e < 3; ++e) {
        if (segmentTriOverlap(A.v[e], A.v[(e + 1) % 3], B, scale)) return true;
        if (segmentTriOverlap(B.v[e], B.v[(e + 1) % 3], A, scale)) return true;
    }
    return false;
}

// Both triangles must be non-degenerate.
bool triTriOverlap(const Triangle& A, const Triangle& B, Real scale, Vec3& point) noexcept {
    if (planeUnreliable(A) || planeUnreliable(B)) return false;
    const Plane pa = unitPlaneOf(A);
    const Plane pb = unitPlaneOf(B);
    const Real eps = 1e-12 * scale;
    const Real nA = norm(pa.n);
    const Real nB = norm(pb.n);
    if (!(nA > 0.0) || !(nB > 0.0)) return false;

    Real da[3], db[3];
    for (int i = 0; i < 3; ++i) {
        da[i] = planeSigned(pb, A.v[i]);
        db[i] = planeSigned(pa, B.v[i]);
    }
    bool aPos = true, aNeg = true, bPos = true, bNeg = true;
    Real maxAbsDa = 0.0, maxAbsDb = 0.0;
    for (int i = 0; i < 3; ++i) {
        aPos = aPos && (da[i] > eps);
        aNeg = aNeg && (da[i] < -eps);
        bPos = bPos && (db[i] > eps);
        bNeg = bNeg && (db[i] < -eps);
        maxAbsDa = std::max(maxAbsDa, std::abs(da[i]));
        maxAbsDb = std::max(maxAbsDb, std::abs(db[i]));
    }
    if (aPos || aNeg || bPos || bNeg) return false;  // separated by a plane

    if (maxAbsDa <= eps && maxAbsDb <= eps) {  // coplanar
        return coplanarOverlap(A, B, pa.n, 1e-14 * scale * scale, point);
    }

    const Vec3 dir = cross(pa.n, pb.n);
    const Real dl = norm(dir);
    if (dl <= 1e-12 * nA * nB) {
        // Parallel planes, not coplanar (handled above): disjoint.
        return false;
    }
    const Vec3 u = dir / dl;

    Vec3 ca[3], cb[3];
    const int na = cutAgainstPlane(A, pb, eps, ca);
    const int nb = cutAgainstPlane(B, pa, eps, cb);
    if (na < 1 || nb < 1) return false;
    if (na > 2 || nb > 2) return false;  // coplanar, handled above

    // Both cuts lie on the common line; compare the 1D intervals.
    const Vec3 origin = ca[0];
    Real tminA = dot(ca[0] - origin, u), tmaxA = tminA;
    for (int i = 1; i < na; ++i) {
        const Real t = dot(ca[i] - origin, u);
        tminA = std::min(tminA, t);
        tmaxA = std::max(tmaxA, t);
    }
    Real tminB = dot(cb[0] - origin, u), tmaxB = tminB;
    for (int i = 1; i < nb; ++i) {
        const Real t = dot(cb[i] - origin, u);
        tminB = std::min(tminB, t);
        tmaxB = std::max(tmaxB, t);
    }
    const Real lo = std::max(tminA, tminB);
    const Real hi = std::min(tmaxA, tmaxB);
    if (lo > hi + eps) return false;
    point = origin + u * (0.5 * (lo + hi));
    return true;
}

// Coplanar pair detection for certification (both planes parallel and equal).
bool isCoplanarPair(const Triangle& A, const Triangle& B, Real scale) noexcept {
    const Vec3 nA = cross(A.v[1] - A.v[0], A.v[2] - A.v[0]);
    const Vec3 nB = cross(B.v[1] - B.v[0], B.v[2] - B.v[0]);
    const Real lA = norm(nA);
    const Real lB = norm(nB);
    if (!(lA > 0.0) || !(lB > 0.0)) return false;
    const Real c = dot(nA, nB) / (lA * lB);
    if (std::abs(c) < 1.0 - 1e-12) return false;
    const Real off = std::abs(dot(nA, B.v[0] - A.v[0])) / lA;
    return off <= 1e-12 * scale;
}

int classifyPoint(const Vec3& x, const Triangle& t, Real tol) noexcept {
    const Real tol2 = tol * tol;
    for (int i = 0; i < 3; ++i) {
        if (norm2(x - t.v[i]) <= tol2) return i;
    }
    for (int i = 0; i < 3; ++i) {
        const int j = (i + 1) % 3;
        const Vec3 d = t.v[j] - t.v[i];
        const Real dd = norm2(d);
        const Real s = (dd > 0.0) ? clamp(dot(x - t.v[i], d) / dd, 0.0, 1.0) : 0.0;
        if (norm2(x - (t.v[i] + d * s)) <= tol2) return 3 + i;
    }
    return -1;
}

// 4x4 dense solve with partial pivoting (the thesis fuses Gaussian
// elimination into the penalty kernel; we keep it as a small helper).
bool solve4(const Real H[4][4], const Real g[4], Real dx[4]) noexcept {
    Real M[4][5];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) M[i][j] = H[i][j];
        M[i][4] = g[i];
    }
    for (int col = 0; col < 4; ++col) {
        int piv = col;
        Real best = std::abs(M[col][col]);
        for (int r = col + 1; r < 4; ++r) {
            const Real a = std::abs(M[r][col]);
            if (a > best) { best = a; piv = r; }
        }
        if (!(best > 0.0) || !std::isfinite(best)) return false;
        if (piv != col) {
            for (int k = col; k < 5; ++k) std::swap(M[col][k], M[piv][k]);
        }
        for (int r = col + 1; r < 4; ++r) {
            const Real f = M[r][col] / M[col][col];
            if (f == 0.0) continue;
            for (int k = col; k < 5; ++k) M[r][k] -= f * M[col][k];
        }
    }
    for (int row = 3; row >= 0; --row) {
        Real sum = M[row][4];
        for (int k = row + 1; k < 4; ++k) sum -= M[row][k] * dx[k];
        const Real d = M[row][row];
        if (!(std::abs(d) > 0.0)) return false;
        dx[row] = sum / d;
        if (!std::isfinite(dx[row])) return false;
    }
    return true;
}

}  // namespace

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

Real pointTriangleDistance(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c,
                           Vec3& closest) noexcept {
    Real lam[3];
    return pointTriangleClosest(p, a, b, c, closest, lam);
}

Real segmentSegmentDistance(const Vec3& p0, const Vec3& p1, const Vec3& q0, const Vec3& q1,
                            Vec3& cp, Vec3& cq) noexcept {
    const Vec3 d1 = p1 - p0;
    const Vec3 d2 = q1 - q0;
    const Vec3 r = p0 - q0;
    const Real a = norm2(d1);
    const Real e = norm2(d2);
    const Real f = dot(d2, r);

    Real s = 0.0;
    Real t = 0.0;
    if (a <= 0.0 && e <= 0.0) {  // both degenerate to points
        s = 0.0;
        t = 0.0;
    } else if (a <= 0.0) {
        s = 0.0;
        t = clamp(f / e, 0.0, 1.0);
    } else {
        const Real cc = dot(d1, r);
        if (e <= 0.0) {
            t = 0.0;
            s = clamp(-cc / a, 0.0, 1.0);
        } else {
            const Real b = dot(d1, d2);
            const Real denom = a * e - b * b;
            if (denom != 0.0) {
                s = clamp((b * f - cc * e) / denom, 0.0, 1.0);
            } else {
                s = 0.0;  // parallel: any s is a valid starting point
            }
            t = (b * s + f) / e;
            if (t < 0.0) {
                t = 0.0;
                s = clamp(-cc / a, 0.0, 1.0);
            } else if (t > 1.0) {
                t = 1.0;
                s = clamp((b - cc) / a, 0.0, 1.0);
            }
        }
    }
    cp = p0 + d1 * s;
    cq = q0 + d2 * t;
    return norm(cp - cq);
}

TriDist triangleTriangleBrute(const Triangle& A, const Triangle& B) noexcept {
    TriDist out;
    Real best = kInf;
    const Real scale = std::max({triScale(A), triScale(B), norm(A.v[0] - B.v[0]), 1e-300});
    const Real featTol = 1e-10 * scale;

    // Phase 1: 6 point-to-triangle evaluations (Algorithm 2, lines 4-9).
    for (int i = 0; i < 3; ++i) {
        Vec3 cl;
        const Real d = pointTriangleDistance(A.v[i], B.v[0], B.v[1], B.v[2], cl);
        if (d < best) {
            best = d;
            out.p = A.v[i];
            out.q = cl;
            out.feature_a = i;
            out.feature_b = classifyPoint(cl, B, featTol);
        }
    }
    for (int i = 0; i < 3; ++i) {
        Vec3 cl;
        const Real d = pointTriangleDistance(B.v[i], A.v[0], A.v[1], A.v[2], cl);
        if (d < best) {
            best = d;
            out.p = cl;
            out.q = B.v[i];
            out.feature_a = classifyPoint(cl, A, featTol);
            out.feature_b = i;
        }
    }

    // Phase 2: 9 segment-to-segment evaluations (lines 11-19).
    for (int i = 0; i < 3; ++i) {
        const int i2 = (i + 1) % 3;
        for (int j = 0; j < 3; ++j) {
            const int j2 = (j + 1) % 3;
            Vec3 cp, cq;
            const Real d = segmentSegmentDistance(A.v[i], A.v[i2], B.v[j], B.v[j2], cp, cq);
            if (d < best) {
                best = d;
                out.p = cp;
                out.q = cq;
                out.feature_a = 3 + i;
                out.feature_b = 3 + j;
            }
        }
    }

    // Overlap decision.  The 15 primitives are exact for disjoint pairs; an
    // intersecting pair additionally needs the explicit overlap test.
    const bool degA = isDegenerate(A);
    const bool degB = isDegenerate(B);
    bool overlap = false;
    if (!degA && !degB) {
        Vec3 ip;
        if (planeUnreliable(A) || planeUnreliable(B)) {
            // One triangle is (numerically) a segment: the plane/interval test
            // is meaningless, so fall back to edge-against-triangle tests.
            if (edgeWiseOverlap(A, B, scale)) {
                overlap = true;
                best = 0.0;
                out.p = (A.v[0] + B.v[0]) * 0.5;
                out.q = out.p;
                out.feature_a = -1;
                out.feature_b = -1;
            }
        } else if (triTriOverlap(A, B, scale, ip)) {
            overlap = true;
            best = 0.0;
            out.p = ip;
            out.q = ip;
            out.feature_a = -1;
            out.feature_b = -1;
        }
    }
    if (!overlap && (degA || degB)) {
        // A degenerate triangle is a segment.  The primitives cover the
        // touching case; a segment that pierces the other triangle's interior
        // is only visible to the edge/segment-triangle test.
        if (best <= 1e-12 * scale || edgeWiseOverlap(A, B, scale)) {
            overlap = true;
            best = 0.0;
        }
    }
    if (!overlap && best <= 1e-12 * scale) {
        overlap = true;
        best = 0.0;
    }

    out.dist = overlap ? 0.0 : best;
    out.overlap = overlap;
    out.fallback = false;
    if (overlap) out.q = out.p;
    if (!std::isfinite(out.dist)) {
        out.dist = 0.0;
        out.overlap = true;
    }
    return out;
}

// ---------------------------------------------------------------------------
// Algorithm 5: penalty-based constrained Newton solver (thesis 5.5.1).
//
//   unknowns x = (a, b, g, d)
//   X = A + BA a + CA b,  Y = D + ED g + FD d,  BA=B-A, CA=C-A, ED=E-D, FD=F-D
//   f(x) = |X - Y|^2
//   h(x) = (-a, -b, a+b-1, -g, -d, g+d-1) <= 0
//   P(x) = f + rho * sum_i max(0, h_i)^2                       (eq. 5.1)
//   grad P = gf + rho * Dmax^T max(0, h)
//   Hess P = hf + rho * Dmax^T Dmax + reg * I
//
// hf is the symmetric 4x4 of Algorithm 5 lines 3-6 (= 2 J^T J with
// J = [BA CA -ED -FD]); it is singular in general (four barycentric unknowns
// for a three-dimensional distance) which is exactly the ill-conditioning
// section 5.5 describes, hence the reg*I perturbation.
//
// Parameter scaling (thesis eq. 5.2 and 5.5.2): with s the mean edge length,
//   rho = 1e10 * s^2,  reg = 1e-6 * s^2,  tol = 1e-14 * s^2.
// The pseudocode prints I/rho^2 for the perturbation; that is 30 orders of
// magnitude smaller than the hf block and destroys the step direction, so the
// text's "eps * I" reading is implemented instead (documented deviation).
//
// The tolerance is floored at 4 ulp of the triangle size so that the cap is
// reachable in double precision for tiny triangles; without the floor a
// triangle of size 1e-2 would require a 1e-18 absolute Newton step.
// ---------------------------------------------------------------------------
TriDist triangleTriangleIterative(const Triangle& A, const Triangle& B, bool& certified) noexcept {
    certified = false;
    TriDist out;

    const Vec3 a0 = A.v[0], a1 = A.v[1], a2 = A.v[2];
    const Vec3 b0 = B.v[0], b1 = B.v[1], b2 = B.v[2];
    const Vec3 BA = a1 - a0;
    const Vec3 CA = a2 - a0;
    const Vec3 ED = b1 - b0;
    const Vec3 FD = b2 - b0;

    Real s = meanEdgeLength(A, B);
    if (!(s > 0.0) || !std::isfinite(s)) s = 1e-30;
    const Real s2 = s * s;
    const Real rho = 1e10 * s2;
    const Real reg = 1e-6 * s2;
    const Real kUlp = std::numeric_limits<Real>::epsilon();
    const Real tol = std::max(1e-14 * s2, 4.0 * kUlp * s);
    // Dimensionless barycentric slack allowed before a solution is rejected as
    // infeasible (thesis 5.6 abandons pairs that sit outside the feasible set).
    const Real feasTol = 1e-6;
    const int maxIter = 8;

    const bool degA = isDegenerate(A);
    const bool degB = isDegenerate(B);
    const bool coplanar = isCoplanarPair(A, B, std::max({triScale(A), triScale(B), 1e-300}));

    // hf = 2 J^T J, J = [BA CA -ED -FD]   (Algorithm 5, lines 3-6)
    const Vec3 J[4] = {BA, CA, -ED, -FD};
    Real hf[4][4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) hf[i][j] = 2.0 * dot(J[i], J[j]);
    }
    // dh: derivatives of the six constraints (lines 16-17).
    static const Real dh[6][4] = {
        {-1.0,  0.0,  0.0,  0.0},
        { 0.0, -1.0,  0.0,  0.0},
        { 1.0,  1.0,  0.0,  0.0},
        { 0.0,  0.0, -1.0,  0.0},
        { 0.0,  0.0,  0.0, -1.0},
        { 0.0,  0.0,  1.0,  1.0},
    };

    Real x[4] = {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0};
    bool converged = false;
    bool solveFailed = false;

    for (int it = 0; it < maxIter; ++it) {
        const Vec3 X = a0 + BA * x[0] + CA * x[1];
        const Vec3 Y = b0 + ED * x[2] + FD * x[3];
        const Vec3 XY = X - Y;

        Real gf[4] = {
            2.0 * dot(XY, BA),
            2.0 * dot(XY, CA),
            -2.0 * dot(XY, ED),
            -2.0 * dot(XY, FD),
        };
        const Real h[6] = {
            -x[0],
            -x[1],
            x[0] + x[1] - 1.0,
            -x[2],
            -x[3],
            x[2] + x[3] - 1.0,
        };

        Real g[4] = {gf[0], gf[1], gf[2], gf[3]};
        Real H[4][4];
        std::memcpy(H, hf, sizeof(H));
        for (int c = 0; c < 6; ++c) {
            if (h[c] < 0.0) continue;          // mask <- h' >= 0 (active set)
            const Real hc = h[c];
            if (hc <= 0.0) continue;           // boundary constraint: no pull
            for (int i = 0; i < 4; ++i) {
                g[i] += rho * dh[c][i] * hc;
                for (int j = 0; j < 4; ++j) H[i][j] += rho * dh[c][i] * dh[c][j];
            }
        }
        for (int i = 0; i < 4; ++i) H[i][i] += reg;

        Real dx[4];
        if (!solve4(H, g, dx)) { solveFailed = true; break; }
        const Vec3 DX = BA * dx[0] + CA * dx[1];
        const Vec3 DY = ED * dx[2] + FD * dx[3];
        const Real err = std::sqrt(norm2(DX) + norm2(DY));
        if (!std::isfinite(err)) { solveFailed = true; break; }
        if (err < tol) { converged = true; break; }  // line 26: break before the update
        for (int i = 0; i < 4; ++i) x[i] -= dx[i];
    }

    const Vec3 X = a0 + BA * x[0] + CA * x[1];
    const Vec3 Y = b0 + ED * x[2] + FD * x[3];

    Real maxViol = 0.0;
    {
        const Real h[6] = {-x[0], -x[1], x[0] + x[1] - 1.0, -x[2], -x[3], x[2] + x[3] - 1.0};
        for (int c = 0; c < 6; ++c) maxViol = std::max(maxViol, std::max(0.0, h[c]));
    }

    Real dist = norm(X - Y);
    if (!std::isfinite(dist) || !std::isfinite(X[0]) || !std::isfinite(Y[0])) {
        // Never hand back garbage: the closest vertex pair is a safe bound.
        Real best = kInf;
        int ia = 0, ib = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                const Real d2 = norm2(A.v[i] - B.v[j]);
                if (d2 < best) { best = d2; ia = i; ib = j; }
            }
        }
        dist = std::sqrt(best);
        out.p = A.v[ia];
        out.q = B.v[ib];
        out.dist = dist;
        out.overlap = false;
        out.feature_a = -1;
        out.feature_b = -1;
        out.fallback = false;
        certified = false;
        return out;
    }

    const Real touchEps = 1e-10 * std::max({triScale(A), triScale(B), 1e-300});

    out.dist = dist;
    out.p = X;
    out.q = Y;
    out.feature_a = -1;
    out.feature_b = -1;
    out.overlap = (dist <= touchEps);
    out.fallback = false;

    // ---- optimality certificate -------------------------------------------
    // C = A - B is convex, so for any unit d,  min_{c in C} d.c  is a lower
    // bound on the distance from the origin to C (supporting-plane bound).
    // Taking d along the penalty solution gives a duality gap: |X-Y| is an
    // upper bound (the point pair is feasible up to maxViol) and lb a lower
    // bound.  A small gap certifies that the penalty iterate really is the
    // closest pair; the stiff penalty Hessian can otherwise stall the Newton
    // step (step < tol) far away from the minimum, which is exactly the
    // ill-conditioning described in thesis 5.5.
    Real gap = kInf;
    if (dist > 0.0 && std::isfinite(dist)) {
        const Vec3 u = (X - Y) / dist;
        Real lb = kInf;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                lb = std::min(lb, dot(u, A.v[i] - B.v[j]));
            }
        }
        gap = dist - lb;
    }
    const Real scale = std::max({triScale(A), triScale(B), 1e-300});
    const Real certGap = 1e-9 * scale;

    certified = converged && !solveFailed && !degA && !degB && !coplanar &&
                !out.overlap && (maxViol <= feasTol) && (gap <= certGap);
    return out;
}

TriDist trianglePairDistance(const Triangle& A, const Triangle& B, NarrowphaseKind kind,
                             bool* used_fallback) noexcept {
    if (used_fallback) *used_fallback = false;
    switch (kind) {
        case NarrowphaseKind::Brute:
            return triangleTriangleBrute(A, B);
        case NarrowphaseKind::Iterative: {
            bool cert = false;
            return triangleTriangleIterative(A, B, cert);
        }
        case NarrowphaseKind::Hybrid: {
            // Iterative first: its certification decides the fallback flag.
            bool cert = false;
            (void)triangleTriangleIterative(A, B, cert);
            // SPEC section 2 requires the hybrid to be *bit identical* to brute
            // force (the robustness claim), so the returned result is always the
            // exact brute-force one; `fallback` records that the iterative pass
            // could not certify this pair (thesis 5.6).
            TriDist r = triangleTriangleBrute(A, B);
            r.fallback = !cert;
            if (used_fallback) *used_fallback = !cert;
            return r;
        }
        case NarrowphaseKind::HybridFast: {
            // Thesis 5.6, "hybrid on triangle pairs": run the penalty solver
            // first and keep its answer when it certifies; only an
            // un-certified (or out-of-tolerance) pair pays for brute force.
            bool cert = false;
            TriDist r = triangleTriangleIterative(A, B, cert);
            if (!cert) {
                r = triangleTriangleBrute(A, B);
                r.fallback = true;
                if (used_fallback) *used_fallback = true;
            } else {
                r.fallback = false;
            }
            return r;
        }
    }
    return triangleTriangleBrute(A, B);
}

}  // namespace contact
