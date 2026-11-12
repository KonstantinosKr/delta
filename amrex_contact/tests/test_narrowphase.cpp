// Standalone correctness tests for the triangle-to-triangle narrowphase.
//
//   g++ -std=c++17 -O2 -Iinclude src/geom/TriangleDistance.cpp
//       src/geom/Narrowphase.cpp tests/test_narrowphase.cpp -o /tmp/test_narrowphase
//
// No test framework: plain checks, a counter, non-zero exit on failure.

#include "contact/Narrowphase.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <random>
#include <string>
#include <vector>

using namespace contact;

namespace {

Real area2of(const Triangle& t) { return norm(cross(t.v[1] - t.v[0], t.v[2] - t.v[0])); }

Real triScaleOf(const Triangle& t) {
    return std::sqrt(std::max({norm2(t.v[1] - t.v[0]), norm2(t.v[2] - t.v[1]), norm2(t.v[0] - t.v[2])}));
}

int g_pass = 0;
int g_fail = 0;

void reportFail(const std::string& what) {
    ++g_fail;
    std::printf("  FAIL  %s\n", what.c_str());
}

void checkTrue(const std::string& what, bool cond) {
    if (cond) {
        ++g_pass;
    } else {
        reportFail(what);
    }
}

void checkNear(const std::string& what, Real got, Real want, Real tol) {
    const Real err = std::abs(got - want);
    if (std::isfinite(got) && err <= tol) {
        ++g_pass;
    } else {
        char buf[512];
        std::snprintf(buf, sizeof(buf), "%s: got %.17g want %.17g err %.3g > %.3g",
                      what.c_str(), got, want, err, tol);
        reportFail(buf);
    }
}

Triangle tri(Real x0, Real y0, Real z0,
             Real x1, Real y1, Real z1,
             Real x2, Real y2, Real z2) {
    Triangle t;
    t.v[0] = makeVec3(x0, y0, z0);
    t.v[1] = makeVec3(x1, y1, z1);
    t.v[2] = makeVec3(x2, y2, z2);
    return t;
}

Triangle triFrom(const Real c[9]) {
    Triangle t;
    for (int k = 0; k < 3; ++k) t.v[k] = makeVec3(c[3 * k + 0], c[3 * k + 1], c[3 * k + 2]);
    return t;
}

// ---------------------------------------------------------------------------
// 0. Primitive kernels against hand-computed answers.
// ---------------------------------------------------------------------------
void testPrimitives() {
    std::printf("[primitives] point-triangle / segment-segment\n");
    const Real zero = 0.0;

    {   // face interior (region 0)
        Triangle T = tri(0, 0, 0, 1, 0, 0, 0, 1, 0);
        Vec3 cl;
        const Real d = pointTriangleDistance(makeVec3(0.25, 0.25, 1.0), T.v[0], T.v[1], T.v[2], cl);
        checkNear("PT interior: distance", d, 1.0, 1e-14);
        checkNear("PT interior: closest.z", cl[2], zero, 1e-15);
        checkNear("PT interior: closest.x", cl[0], 0.25, 1e-15);
        checkNear("PT interior: closest.y", cl[1], 0.25, 1e-15);
    }
    {   // edge AB
        Triangle T = tri(0, 0, 0, 1, 0, 0, 0, 1, 0);
        Vec3 cl;
        const Real d = pointTriangleDistance(makeVec3(0.5, -1.0, 0.0), T.v[0], T.v[1], T.v[2], cl);
        checkNear("PT edge: distance", d, 1.0, 1e-14);
        checkNear("PT edge: closest.x", cl[0], 0.5, 1e-15);
        checkNear("PT edge: closest.y", cl[1], 0.0, 1e-15);
    }
    {   // vertex A
        Triangle T = tri(0, 0, 0, 1, 0, 0, 0, 1, 0);
        Vec3 cl;
        const Real d = pointTriangleDistance(makeVec3(-1.0, -1.0, 0.0), T.v[0], T.v[1], T.v[2], cl);
        checkNear("PT vertex: distance", d, std::sqrt(2.0), 1e-14);
        checkNear("PT vertex: closest.x", cl[0], 0.0, 1e-15);
    }
    {   // zero-area triangle: the point set is the segment [0,2] on x
        Triangle T = tri(0, 0, 0, 1, 0, 0, 2, 0, 0);
        Vec3 cl;
        const Real d = pointTriangleDistance(makeVec3(3.0, 4.0, 0.0), T.v[0], T.v[1], T.v[2], cl);
        checkNear("PT degenerate: distance", d, std::sqrt(17.0), 1e-14);
        checkNear("PT degenerate: closest.x", cl[0], 2.0, 1e-15);
    }
    {   // single-point triangle
        Triangle T = tri(1, 2, 3, 1, 2, 3, 1, 2, 3);
        Vec3 cl;
        const Real d = pointTriangleDistance(makeVec3(1.0, 2.0, 5.0), T.v[0], T.v[1], T.v[2], cl);
        checkNear("PT point-triangle: distance", d, 2.0, 1e-15);
    }
    {   // skew segments, interior-interior
        Vec3 cp, cq;
        const Real d = segmentSegmentDistance(makeVec3(-1, 0, 0), makeVec3(1, 0, 0),
                                              makeVec3(0, -1, 1), makeVec3(0, 1, 1), cp, cq);
        checkNear("SS skew: distance", d, 1.0, 1e-14);
        checkNear("SS skew: cp.x", cp[0], 0.0, 1e-15);
        checkNear("SS skew: cq.z", cq[2], 1.0, 1e-15);
    }
    {   // parallel segments
        Vec3 cp, cq;
        const Real d = segmentSegmentDistance(makeVec3(0, 0, 0), makeVec3(1, 0, 0),
                                              makeVec3(0, 1, 1), makeVec3(1, 1, 1), cp, cq);
        checkNear("SS parallel: distance", d, std::sqrt(2.0), 1e-14);
    }
    {   // disjoint collinear segments
        Vec3 cp, cq;
        const Real d = segmentSegmentDistance(makeVec3(0, 0, 0), makeVec3(1, 0, 0),
                                              makeVec3(2, 0, 0), makeVec3(3, 0, 0), cp, cq);
        checkNear("SS collinear gap: distance", d, 1.0, 1e-15);
    }
    {   // crossing segments
        Vec3 cp, cq;
        const Real d = segmentSegmentDistance(makeVec3(-1, 0, 0), makeVec3(1, 0, 0),
                                              makeVec3(0, -1, 0), makeVec3(0, 1, 0), cp, cq);
        checkNear("SS crossing: distance", d, 0.0, 1e-15);
    }
}

// ---------------------------------------------------------------------------
// 1. Analytic triangle-pair cases ( >= 12 ), |err| <= 1e-12.
// ---------------------------------------------------------------------------
struct AnalyticCase {
    const char* name;
    Real A[9];
    Real B[9];
    Real want;
    bool overlap;
};

const AnalyticCase kCases[] = {
    // vertex-vertex: (1,0,0) of A against (3,4,0) of B -> sqrt(20)
    {"vertex-vertex",
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     {3, 4, 0, 4, 4, 0, 3, 5, 0},
     std::sqrt(18.0), false},
    // vertex-edge: A's origin against B's edge x=1 -> 1
    {"vertex-edge",
     {0, 0, 0, -1, 1, 0, -1, -1, 0},
     {1, -2, 0, 1, 2, 0, 2, 0, 0},
     1.0, false},
    // vertex-face (interior projection): plane z=2 -> 2
    {"vertex-face",
     {0, 0, 0, -1, 1, 0, -1, -1, 0},
     {5, 5, 2, -5, 5, 2, 0, -5, 2},
     2.0, false},
    // edge-edge skew: x-axis edge vs y-edge at z=1 -> 1
    {"edge-edge skew",
     {-1, 0, 0, 1, 0, 0, 0, 0, -1},
     {0, -1, 1, 0, 1, 1, 0, 0, 2},
     1.0, false},
    // parallel edges, parallel planes: translation by z=1 -> 1
    {"parallel edges",
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 1, 0, 1, 1},
     1.0, false},
    // coplanar, in-plane gap 2
    {"coplanar parallel",
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     {3, 0, 0, 4, 0, 0, 3, 1, 0},
     2.0, false},
    // vertex touches the other triangle's face at one point
    {"touching vertex-face",
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     {0.25, 0.25, 0, 1, 1, 1, 1, 0, 1},
     0.0, true},
    // two triangles sharing the edge (0,0,0)-(1,0,0)
    {"sharing an edge",
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     {0, 0, 0, 1, 0, 0, 0, -1, 0},
     0.0, true},
    // parallel planes, gap 3
    {"separated gap 3",
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     {0, 0, 3, 1, 0, 3, 0, 1, 3},
     3.0, false},
    // one triangle strictly inside the other, same plane
    {"nested coplanar",
     {0, 0, 0, 10, 0, 0, 0, 10, 0},
     {1, 1, 0, 2, 1, 0, 1, 2, 0},
     0.0, true},
    // zero-area triangle (the segment x in [0,2]) against a triangle at z=5
    {"degenerate zero-area",
     {0, 0, 0, 1, 0, 0, 2, 0, 0},
     {0, 0, 5, 1, 0, 5, 0, 1, 5},
     5.0, false},
    // identical triangles
    {"identical",
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     0.0, true},
    // penetrating: B pierces the interior of A; all 15 primitives are positive
    // (> 1.9) yet the true distance is 0.
    {"penetrating face crossing",
     {0, 0, 0, 10, 0, 0, 0, 10, 0},
     {2, 2, -1, 2.2, 2.2, 1, 2.1, 2.2, 1},
     0.0, true},
    // offset in all three axes, exact 3-4-5 style gap: closest (0,0,0)-(3,4,0)
    // -> sqrt(25) (vertex against the interior of B's edge region)
    {"diagonal gap",
     {-3, -4, 0, -4, -4, 0, -3, -5, 0},
     {0, 0, 0, 1, 0, 0, 0, 1, 0},
     5.0, false},
};

void testAnalytic() {
    std::printf("[analytic] %zu triangle-pair configurations\n", sizeof(kCases) / sizeof(kCases[0]));
    Real maxErr = 0.0;
    for (const AnalyticCase& c : kCases) {
        const Triangle A = triFrom(c.A);
        const Triangle B = triFrom(c.B);
        const TriDist bf = triangleTriangleBrute(A, B);
        bool fb = false;
        const TriDist hy = trianglePairDistance(A, B, NarrowphaseKind::Hybrid, &fb);

        const std::string n(c.name);
        checkNear(n + " brute dist", bf.dist, c.want, 1e-12);
        maxErr = std::max(maxErr, std::abs(bf.dist - c.want));
        checkTrue(n + " brute overlap", bf.overlap == c.overlap);

        // hybrid must be the brute answer, bit for bit
        checkTrue(n + " hybrid == brute (dist)", hy.dist == bf.dist);
        checkTrue(n + " hybrid == brute (p)", hy.p == bf.p);
        checkTrue(n + " hybrid == brute (q)", hy.q == bf.q);
        checkTrue(n + " hybrid == brute (overlap)", hy.overlap == bf.overlap);

        // closest points really lie on their triangles
        Vec3 tmp;
        const Real scale = std::max({1.0, std::abs(bf.dist)});
        if (!bf.overlap) {
            const Real dp = pointTriangleDistance(bf.p, A.v[0], A.v[1], A.v[2], tmp);
            const Real dq = pointTriangleDistance(bf.q, B.v[0], B.v[1], B.v[2], tmp);
            checkTrue(n + " p on A", dp <= 1e-9 * scale);
            checkTrue(n + " q on B", dq <= 1e-9 * scale);
            checkNear(n + " |p-q| == dist", norm(bf.p - bf.q), bf.dist, 1e-12 * scale);
        }
        checkTrue(n + " feature range",
                  bf.feature_a >= -1 && bf.feature_a <= 5 &&
                  bf.feature_b >= -1 && bf.feature_b <= 5);

        // direct feature checks where the answer is unique
        if (n == "vertex-edge") {
            checkTrue(n + " feature_a == vertex 0", bf.feature_a == 0);
            checkTrue(n + " feature_b == edge 3", bf.feature_b == 3);
        }
        if (n == "edge-edge skew") {
            checkTrue(n + " feature_a == edge 3", bf.feature_a == 3);
            checkTrue(n + " feature_b == edge 3", bf.feature_b == 3);
        }
    }
    std::printf("         max |brute - analytic| = %.3e\n", static_cast<double>(maxErr));
}

// ---------------------------------------------------------------------------
// 2. Randomized fuzz.
// ---------------------------------------------------------------------------
struct Rng {
    std::mt19937_64 g;
    explicit Rng(std::uint64_t seed) : g(seed) {}
    Real uni(Real lo, Real hi) {
        return std::uniform_real_distribution<Real>(lo, hi)(g);
    }
    int pick(int n) { return static_cast<int>(std::uniform_int_distribution<int>(0, n - 1)(g)); }
};

Triangle randomTriangle(Rng& rng, Real s, const Vec3& c) {
    Triangle t;
    for (int k = 0; k < 3; ++k) {
        for (int d = 0; d < 3; ++d) t.v[k][d] = c[d] + s * rng.uni(-1.0, 1.0);
    }
    return t;
}

// A triangle whose third vertex is almost on the line through the first two.
Triangle sliverTriangle(Rng& rng, Real s, const Vec3& c) {
    Triangle t;
    t.v[0] = c + makeVec3(s * rng.uni(-1, 1), s * rng.uni(-1, 1), s * rng.uni(-1, 1));
    t.v[1] = c + makeVec3(s * rng.uni(-1, 1), s * rng.uni(-1, 1), s * rng.uni(-1, 1));
    const Vec3 dir = normalized(t.v[1] - t.v[0]);
    Vec3 perp = normalized(cross(dir, makeVec3(0.13, 0.71, -0.37)));
    if (norm2(perp) == 0.0) perp = makeVec3(1, 0, 0);
    const Real tpar = rng.uni(0.1, 0.9);
    t.v[2] = t.v[0] + (t.v[1] - t.v[0]) * tpar + perp * (s * 1e-9 * rng.uni(0.5, 1.5));
    return t;
}

void testFuzz() {
    // NP_FUZZ_N lets the sanitizer / CI runs use a smaller sample.
    int N = 200000;
    if (const char* env = std::getenv("NP_FUZZ_N")) {
        const int v = std::atoi(env);
        if (v > 0) N = v;
    }
    std::printf("[fuzz] %d random triangle pairs\n", N);

    Rng rng(0x5EED1234ULL);
    long overlapCount = 0;
    long certified = 0;
    long fallbacks = 0;
    long genericPairs = 0;
    long genericFallbacks = 0;
    long iterOutOfTol = 0;
    Real maxIterAbsErr = 0.0;
    Real maxIterRelErr = 0.0;
    Real worstScale = 1.0;
    bool hybridMismatch = false;

    const Real iterTolRel = 1e-6;
    Vec3 tmpPt;

    // One driver reused across the sampled queries so its counters accumulate
    // over the fuzz run (spec check 3).
    Narrowphase npFuzz(NarrowphaseKind::Hybrid, 0.0);
    long sampled = 0;

    for (int n = 0; n < N; ++n) {
        const int mode = n % 10;
        const Real sa = std::pow(10.0, rng.uni(-2.0, 2.0));
        const Real sb = std::pow(10.0, rng.uni(-2.0, 2.0));
        Triangle A, B;

        if (mode == 6) {
            A = sliverTriangle(rng, sa, makeVec3(0, 0, 0));
            B = randomTriangle(rng, sb, makeVec3(rng.uni(-3, 3) * sa, rng.uni(-3, 3) * sa, rng.uni(-3, 3) * sa));
        } else if (mode == 7) {
            // coplanar pair (z = 0): must never certify
            const Vec3 ca = makeVec3(rng.uni(-2, 2) * sa, rng.uni(-2, 2) * sa, 0.0);
            const Vec3 cb = makeVec3(ca[0] + rng.uni(-2, 2) * sb, ca[1] + rng.uni(-2, 2) * sb, 0.0);
            A = randomTriangle(rng, sa, ca);
            B = randomTriangle(rng, sb, cb);
            for (int k = 0; k < 3; ++k) { A.v[k][2] = 0.0; B.v[k][2] = 0.0; }
        } else if (mode == 8) {
            // near-touching: B sits a hair above A's plane
            A = randomTriangle(rng, sa, makeVec3(0, 0, 0));
            B = randomTriangle(rng, sb, makeVec3(rng.uni(-0.5, 0.5) * sa, rng.uni(-0.5, 0.5) * sa,
                                                 1e-7 * sa));
        } else if (mode == 9) {
            // B built around a point of A so that the pair often intersects
            A = randomTriangle(rng, sa, makeVec3(0, 0, 0));
            const Real u = rng.uni(0.05, 0.55), v = rng.uni(0.05, 0.55 - std::min<Real>(u, 0.5));
            const Vec3 inside = A.v[0] + (A.v[1] - A.v[0]) * u + (A.v[2] - A.v[0]) * v;
            B = randomTriangle(rng, sa * 0.6, inside);
        } else {
            A = randomTriangle(rng, sa, makeVec3(0, 0, 0));
            B = randomTriangle(rng, sb, makeVec3(rng.uni(-4, 4) * sa, rng.uni(-4, 4) * sa,
                                                 rng.uni(-4, 4) * sa));
        }

        const TriDist bf = triangleTriangleBrute(A, B);
        bool fb = false;
        const TriDist hy = trianglePairDistance(A, B, NarrowphaseKind::Hybrid, &fb);
        bool cert = false;
        const TriDist it = triangleTriangleIterative(A, B, cert);

        const Real scale = std::max({triScaleOf(A), triScaleOf(B), 1e-12});

        if (!(hy.dist == bf.dist) || !(hy.p == bf.p) || !(hy.q == bf.q) ||
            hy.overlap != bf.overlap || fb != hy.fallback) {
            if (!hybridMismatch) {
                char buf[512];
                std::snprintf(buf, sizeof(buf),
                              "hybrid != brute at pair %d (modes %d): hy=%.17g bf=%.17g",
                              n, mode, hy.dist, bf.dist);
                reportFail(buf);
            }
            hybridMismatch = true;
        }
        if (bf.overlap) ++overlapCount;
        checkTrue("brute result finite", std::isfinite(bf.dist) && bf.dist >= 0.0);
        checkTrue("iterative result finite", std::isfinite(it.dist) && it.dist >= 0.0);

        // Independent optimality audit of the brute-force answer: for a convex
        // Minkowski difference C = A - B the support bound
        //     lb = min_{c in C} u.c,   u = (p-q)/|p-q|
        // is a lower bound on the distance and it is tight (lb == dist) exactly
        // when (p,q) really is the closest pair.  C is the convex hull of the
        // nine vertex differences, so lb is a min over nine dots.
        if (!bf.overlap && bf.dist > 0.0) {
            const Vec3 u = (bf.p - bf.q) / bf.dist;
            Real lb = std::numeric_limits<Real>::max();
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    lb = std::min(lb, dot(u, A.v[i] - B.v[j]));
                }
            }
            checkTrue("brute: duality gap tight", (bf.dist - lb) <= 1e-6 * scale);
            // A closest point can only be localised to within the triangle's
            // own thickness (area2 / longest edge) when the triangle is a
            // needle; the *distance* stays accurate because that error is
            // perpendicular to the closest direction.
            const Real thickA = area2of(A) / std::max(triScaleOf(A), 1e-300);
            const Real thickB = area2of(B) / std::max(triScaleOf(B), 1e-300);
            const Real onTol = 1e-9 * scale + 2.0 * std::max(thickA, thickB);
            const Real dpA = pointTriangleDistance(bf.p, A.v[0], A.v[1], A.v[2], tmpPt);
            const Real dqB = pointTriangleDistance(bf.q, B.v[0], B.v[1], B.v[2], tmpPt);
            checkTrue("brute: p on A", dpA <= onTol);
            checkTrue("brute: q on B", dqB <= onTol);
            checkTrue("brute: |p-q| == dist", std::abs(norm(bf.p - bf.q) - bf.dist) <= 1e-9 * scale);
            // no vertex pair may be closer than the reported minimum
            Real vpMin = std::numeric_limits<Real>::max();
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) vpMin = std::min(vpMin, norm(A.v[i] - B.v[j]));
            }
            checkTrue("brute: dist <= vertex-pair bound", bf.dist <= vpMin + 1e-9 * scale);
        }

        if (fb) ++fallbacks;
        if (mode <= 5) {
            ++genericPairs;
            if (fb) ++genericFallbacks;
        }
        if (cert) {
            ++certified;
            const Real err = std::abs(it.dist - bf.dist);
            const Real rel = err / std::max(bf.dist, 1e-12 * scale);
            if (err > maxIterAbsErr) {
                maxIterAbsErr = err;
                worstScale = scale;
            }
            maxIterRelErr = std::max(maxIterRelErr, rel);
            const Real allowed = iterTolRel * scale + 1e-12;
            if (err > allowed) {
                if (iterOutOfTol < 5) {
                    char buf[512];
                    std::snprintf(buf, sizeof(buf),
                                  "iterative certified but off at pair %d (mode %d): it=%.17g bf=%.17g err=%.3g allowed=%.3g",
                                  n, mode, it.dist, bf.dist, err, allowed);
                    reportFail(buf);
                }
                ++iterOutOfTol;
            }
        }

        // Sample the Narrowphase driver statistics on the same geometry.
        if (n % 97 == 0) {
            Particle pa, pb;
            pa.id = 1; pb.id = 2;
            pa.radius = 10.0 * scale; pa.halo = 11.0 * scale;
            pb.radius = 10.0 * scale; pb.halo = 11.0 * scale;
            pa.mesh.push_back(A);
            pb.mesh.push_back(B);
            ContactPoint cp;
            const bool hit = npFuzz.query(pa, pb, 0.01 * scale, cp);
            ++sampled;
            if (hit) {
                checkTrue("query contact consistent", cp.dist <= 0.01 * scale + 1e-12);
            }
        }
    }

    std::printf("         pairs=%d  overlaps(brute)=%ld (%.2f%%)\n", N, overlapCount,
                100.0 * static_cast<double>(overlapCount) / N);
    std::printf("         iterative certified=%ld (%.2f%%)  fallbacks=%ld (%.3f%%)\n",
                certified, 100.0 * static_cast<double>(certified) / N,
                fallbacks, 100.0 * static_cast<double>(fallbacks) / N);
    std::printf("         fallbacks on generic pairs (modes 0-5): %ld/%ld (%.3f%%)\n",
                genericFallbacks, genericPairs,
                100.0 * static_cast<double>(genericFallbacks) / std::max<long>(genericPairs, 1));
    std::printf("         max |iterative - brute| (certified) = %.3e (scale %.3g)\n",
                static_cast<double>(maxIterAbsErr), static_cast<double>(worstScale));
    std::printf("         max relative error (certified)      = %.3e\n",
                static_cast<double>(maxIterRelErr));
    std::printf("         certified pairs outside %.0e*scale   = %ld\n",
                static_cast<double>(iterTolRel), iterOutOfTol);

    checkTrue("fuzz: hybrid never diverged from brute", !hybridMismatch);
    checkTrue("fuzz: driver tri_pairs == sampled queries",
              npFuzz.stats().tri_pairs == sampled && sampled > 0);
    checkTrue("fuzz: driver fallbacks counter > 0", npFuzz.stats().fallbacks > 0);
    std::printf("         driver stats over %ld sampled pairs: tri_pairs=%ld fallbacks=%ld "
                "contacts=%ld sphere_rejects=%ld\n",
                sampled, npFuzz.stats().tri_pairs, npFuzz.stats().fallbacks,
                npFuzz.stats().contacts, npFuzz.stats().sphere_rejects);
    checkTrue("fuzz: some pairs certify", certified > N / 2);
    checkTrue("fuzz: fallbacks counter is non-zero", fallbacks > 0);
    checkTrue("fuzz: certified iterative is accurate", iterOutOfTol == 0);
    checkTrue("fuzz: hybrid fallback flag matches", fallbacks > 0 && fallbacks < N);
}

// ---------------------------------------------------------------------------
// 3. NarrowphaseStats.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// 2b. Thesis 5.6, "hybrid on triangle pairs": keep the penalty solver's answer
//     when it certifies and fall back to brute force only otherwise.  Measure
//     the accuracy given up and the work saved against always-brute.
// ---------------------------------------------------------------------------
void testHybridFast() {
    std::printf("\n[hybrid-fast] thesis 5.6: keep the penalty answer, fall back on failure\n");

    Rng rng(20261109u);
    const int N = 50000;
    std::vector<std::pair<Triangle, Triangle>> pairs;
    pairs.reserve(N);
    for (int n = 0; n < N; ++n) {
        const Real sa = rng.uni(0.1, 3.0);
        const Real sb = rng.uni(0.1, 3.0);
        Triangle A = randomTriangle(rng, sa, makeVec3(0, 0, 0));
        Triangle B;
        const int mode = n % 10;
        if (mode < 6) {
            B = randomTriangle(rng, sb,
                               makeVec3(rng.uni(-3, 3) * sa, rng.uni(-3, 3) * sa,
                                        rng.uni(-3, 3) * sa));
        } else if (mode < 8) {
            // roughly parallel planes at a small separation
            B = randomTriangle(rng, sb, makeVec3(0, 0, rng.uni(0.9, 1.3) * sa));
        } else if (mode < 9) {
            // near-touching
            B = randomTriangle(rng, sb,
                               makeVec3(rng.uni(-0.2, 0.2) * sa, rng.uni(-0.2, 0.2) * sa,
                                        rng.uni(0.0, 0.05) * sa));
        } else {
            // nested / overlapping
            B = randomTriangle(rng, sb * 0.5, makeVec3(rng.uni(-0.2, 0.2) * sa, 0, 0));
        }
        pairs.emplace_back(A, B);
    }

    long fallbacks = 0;
    long outOfTol = 0;
    Real maxAbs = 0.0;
    Real maxRel = 0.0;

    Real sink = 0.0;
    // Warm-up so the timing below is not a cold-cache measurement of whichever
    // variant happens to run first.
    for (int i = 0; i < 2000 && i < N; ++i) {
        sink += trianglePairDistance(pairs[i].first, pairs[i].second, NarrowphaseKind::Brute).dist;
        sink += trianglePairDistance(pairs[i].first, pairs[i].second,
                                     NarrowphaseKind::HybridFast).dist;
    }

    const auto t0 = std::chrono::steady_clock::now();
    for (const auto& pr : pairs) {
        const TriDist r = trianglePairDistance(pr.first, pr.second, NarrowphaseKind::HybridFast);
        sink += r.dist;
    }
    const auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        sink += trianglePairDistance(pairs[i].first, pairs[i].second, NarrowphaseKind::Brute).dist;
    }
    const auto t2 = std::chrono::steady_clock::now();
    if (sink < Real(-1.0e300)) std::printf(" ");  // keep the optimiser honest

    for (const auto& pr : pairs) {
        const TriDist fb = trianglePairDistance(pr.first, pr.second, NarrowphaseKind::Brute);
        bool used = false;
        const TriDist ff =
            trianglePairDistance(pr.first, pr.second, NarrowphaseKind::HybridFast, &used);
        if (used) ++fallbacks;
        const Real scale = std::max(triScaleOf(pr.first), triScaleOf(pr.second));
        const Real absErr = std::abs(ff.dist - fb.dist);
        const Real relErr = absErr / std::max<Real>(scale, Real(1e-30));
        maxAbs = std::max(maxAbs, absErr);
        maxRel = std::max(maxRel, relErr);
        if (relErr > 1e-6) ++outOfTol;
    }

    const double msFast = std::chrono::duration<double, std::milli>(t1 - t0).count();
    const double msBrute = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::printf("         pairs=%d  fallbacks=%ld (%.2f%%)\n", N, fallbacks,
                100.0 * static_cast<double>(fallbacks) / N);
    std::printf("         max |hybrid-fast - brute| = %.3e   max relative = %.3e\n",
                static_cast<double>(maxAbs), static_cast<double>(maxRel));
    std::printf("         pairs outside 1e-6*scale = %ld\n", outOfTol);
    std::printf("         wall: hybrid-fast %.1f ms   brute %.1f ms   (%.2fx)\n", msFast, msBrute,
                msBrute / std::max(msFast, 1e-9));

    checkTrue("hybrid-fast: the majority of pairs are answered without brute force",
              fallbacks < N / 2);
    checkTrue("hybrid-fast: stays within 1e-6 relative of brute force", outOfTol == 0);
    // No speed assertion on purpose. Measured here the penalty solver is
    // *slower* per pair than the 15-primitive brute force on this geometry
    // mix, which is exactly the caveat the thesis raises on p.70 ("a high
    // number of Newton steps can render the penalty method slower than the
    // brute force approach"). The ratio is reported, not asserted.
    std::printf("         note: %.2fx vs brute on this mix (thesis p.70 warns this can "
                "go either way)\n", msBrute / std::max(msFast, 1e-9));
}

void testStats() {
    std::printf("[stats] Narrowphase counters\n");
    Rng rng(0xABCDEF01ULL);

    Particle pa, pb;
    pa.id = 1;
    pb.id = 2;
    pa.radius = pb.radius = 5.0;
    pa.halo = pb.halo = 5.5;
    pa.mesh.push_back(randomTriangle(rng, 1.0, makeVec3(0, 0, 0)));
    pb.mesh.push_back(randomTriangle(rng, 1.0, makeVec3(0.1, 0, 0)));
    pb.mesh.push_back(randomTriangle(rng, 1.0, makeVec3(-0.1, 0, 0)));

    Narrowphase np(NarrowphaseKind::Hybrid, 0.05);
    ContactPoint cp;

    int queries = 0;
    for (int n = 0; n < 500; ++n) {
        pa.pos = makeVec3(rng.uni(-1, 1), rng.uni(-1, 1), rng.uni(-1, 1));
        // coplanar meshes guarantee that the iterative solver cannot certify
        pb.pos = makeVec3(pa.pos[0] + rng.uni(-0.5, 0.5), pa.pos[1] + rng.uni(-0.5, 0.5), pa.pos[2]);
        np.query(pa, pb, cp);
        ++queries;
    }
    const NarrowphaseStats& s = np.stats();
    std::printf("         queries=%d tri_pairs=%ld brute=%ld iterative=%ld fallbacks=%ld "
                "contacts=%ld sphere_rejects=%ld\n",
                queries, s.tri_pairs, s.brute_calls, s.iterative_calls, s.fallbacks,
                s.contacts, s.sphere_rejects);
    // Two triangle pairs per query, minus the pairs skipped by the certified
    // zero early exit.
    checkTrue("stats: at most two triangle pairs per query",
              s.tri_pairs > 0 && s.tri_pairs <= static_cast<long>(queries) * 2);
    checkTrue("stats: at least one pair per query",
              s.tri_pairs >= static_cast<long>(queries));
    checkTrue("stats: iterative called once per pair",
              s.iterative_calls == s.tri_pairs);
    checkTrue("stats: brute called once per pair in hybrid",
              s.brute_calls == s.tri_pairs);
    checkTrue("stats: fallbacks incremented", s.fallbacks > 0);

    // Sphere rejection: far apart bodies.
    const long before = s.sphere_rejects;
    pb.pos = makeVec3(100.0, 100.0, 100.0);
    checkTrue("stats: far pair is rejected", !np.query(pa, pb, cp));
    checkTrue("stats: sphere_rejects incremented", np.stats().sphere_rejects == before + 1);
    checkTrue("stats: no contact counted on reject", !np.query(pa, pb, cp));

    np.resetStats();
    checkTrue("stats: reset clears counters", np.stats().tri_pairs == 0 && np.stats().fallbacks == 0);
}

// ---------------------------------------------------------------------------
// 4. Two tetrahedra through Narrowphase::query (all three kinds).
// ---------------------------------------------------------------------------
Mesh tetraMesh() {
    Mesh m;
    const Vec3 p0 = makeVec3(0.5, 0.0, -0.5);
    const Vec3 p1 = makeVec3(0.5, 1.0, 0.5);
    const Vec3 p2 = makeVec3(0.5, -1.0, 0.5);
    const Vec3 p3 = makeVec3(-1.5, 0.0, -0.5);
    auto add = [&](const Vec3& a, const Vec3& b, const Vec3& c) {
        Triangle t;
        t.v[0] = a; t.v[1] = b; t.v[2] = c;
        m.push_back(t);
    };
    add(p0, p2, p1);   // base face, plane x = 0.5
    add(p0, p1, p3);
    add(p0, p3, p2);
    add(p1, p2, p3);
    return m;
}

Particle makeParticle(int id, const Vec3& pos, const Mesh& mesh) {
    Particle p;
    p.id = id;
    p.pos = pos;
    p.quat = Quat{};
    p.radius = 2.0;
    p.halo = 2.2;
    p.mass = 1.0;
    p.inv_mass = 1.0;
    p.mesh = mesh;
    return p;
}

void testParticles() {
    std::printf("[particles] two tetrahedra, known gap 0.5\n");
    const Mesh mesh = tetraMesh();
    const Particle a = makeParticle(10, makeVec3(0, 0, 0), mesh);
    // B translated by 2.5 in x: its apex (body x = -1.5) sits at x = 1.0 while
    // A's base plane is x = 0.5, so the surface gap is exactly 0.5.
    const Particle b = makeParticle(20, makeVec3(2.5, 0, 0), mesh);

    const char* kinds[3] = {"Brute", "Iterative", "Hybrid"};
    const NarrowphaseKind kindv[3] = {NarrowphaseKind::Brute, NarrowphaseKind::Iterative,
                                      NarrowphaseKind::Hybrid};

    for (int k = 0; k < 3; ++k) {
        Narrowphase np(kindv[k], 0.6);
        ContactPoint cp;
        const bool hit = np.query(a, b, cp);
        const std::string n(kinds[k]);
        checkTrue(n + ": contact found", hit);
        if (hit) {
            const Real tol = (kindv[k] == NarrowphaseKind::Iterative) ? 1e-6 : 1e-12;
            checkNear(n + ": dist", cp.dist, 0.5, tol);
            checkNear(n + ": depth", cp.depth, 0.1, tol);
            checkNear(n + ": normal.x", cp.normal[0], 1.0, 1e-9);
            checkNear(n + ": normal.y", cp.normal[1], 0.0, 1e-9);
            checkNear(n + ": normal.z", cp.normal[2], 0.0, 1e-9);
            checkTrue(n + ": normal is unit", std::abs(norm(cp.normal) - 1.0) <= 1e-12);
            checkTrue(n + ": ids", cp.i == 10 && cp.j == 20);
            checkNear(n + ": contact point x", cp.point[0], 0.75, 1e-9);
            checkTrue(n + ": triangle ids valid", cp.tri_a >= 0 && cp.tri_a < 4 &&
                                                     cp.tri_b >= 0 && cp.tri_b < 4);
        }
        // margin smaller than the gap -> no contact, still a candidate pair
        ContactPoint cp2;
        checkTrue(n + ": no contact with margin 0.4", !np.query(a, b, 0.4, cp2));
    }

    {   // separated along x: normal must point from a to b along +x.
        // c's base plane is x=-2 and b's apex is x=1, so the gap is 3.0.
        Particle c = makeParticle(30, makeVec3(-2.5, 0, 0), mesh);
        Narrowphase np(NarrowphaseKind::Hybrid, 3.5);
        ContactPoint cp;
        checkTrue("reverse side: contact found", np.query(c, b, cp));
        checkTrue("reverse side: normal points +x", cp.normal[0] > 0.999);
        checkNear("reverse side: dist", cp.dist, 3.0, 1e-9);
        checkNear("reverse side: depth", cp.depth, 0.5, 1e-9);
    }

    {   // exact touching with the default margin 0.0 must count as a contact
        Narrowphase np(NarrowphaseKind::Hybrid, 0.0);
        Particle c = makeParticle(31, makeVec3(-2.0, 0, 0), mesh);  // base plane x = -1.5
        Particle d = makeParticle(32, makeVec3(0.0, 0, 0), mesh);   // apex exactly at x = -1.5
        ContactPoint cp;
        checkTrue("touching, margin 0: contact", np.query(c, d, cp));
        checkNear("touching, margin 0: dist", cp.dist, 0.0, 1e-12);
        checkNear("touching, margin 0: depth", cp.depth, 0.0, 1e-12);
    }

    {   // empty mesh: sphere-only fallback
        Particle e = makeParticle(40, makeVec3(0.0, 0.0, 0.0), Mesh{});
        e.radius = 1.0;
        e.halo = 1.1;
        Particle f = makeParticle(50, makeVec3(1.5, 0.0, 0.0), Mesh{});
        f.radius = 1.0;
        f.halo = 1.1;
        Narrowphase np(NarrowphaseKind::Hybrid, 0.2);
        ContactPoint cp;
        checkTrue("empty meshes: contact", np.query(e, f, cp));
        checkNear("empty meshes: dist", cp.dist, 0.0, 1e-12);
        checkNear("empty meshes: depth", cp.depth, 0.2, 1e-12);
        checkNear("empty meshes: normal.x", cp.normal[0], 1.0, 1e-12);
    }

    {   // deep penetration: distance 0, normal from the centres
        Particle g = makeParticle(60, makeVec3(0.9, 0, 0), mesh);
        Narrowphase np(NarrowphaseKind::Hybrid, 0.5);
        ContactPoint cp;
        checkTrue("penetrating: contact", np.query(a, g, cp));
        checkNear("penetrating: dist", cp.dist, 0.0, 1e-12);
        // Once the surfaces intersect, the triangle distance saturates at zero,
        // so the penetration depth comes from the geometric overlap of the two
        // support planes along the contact normal (see Narrowphase.cpp): with
        // unit support radii 0.9 apart that overlap is 2*1.0 - 0.9 = 1.1, not
        // the (meaningless) margin - dist = 0.5.
        checkNear("penetrating: depth is the support overlap", cp.depth, 1.1, 1e-9);
        checkTrue("penetrating: normal from centres", cp.normal[0] > 0.99);
    }
}

}  // namespace

int main() {
    std::printf("== narrowphase tests ==\n");
    testPrimitives();
    testAnalytic();
    testFuzz();
    testHybridFast();
    testStats();
    testParticles();

    std::printf("\n----------------------------------------\n");
    std::printf("checks passed: %d\n", g_pass);
    std::printf("checks failed: %d\n", g_fail);
    if (g_fail != 0) {
        std::printf("RESULT: FAIL\n");
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
