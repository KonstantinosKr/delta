// Standalone tests for the shape + scenario modules (no AMReX).
//
//   g++ -std=c++17 -O2 -Iinclude src/scenario/*.cpp tests/test_scenario.cpp
//       -o /tmp/test_scenario && /tmp/test_scenario
//
// Covers: mesh validity (closed manifold, outward orientation, bounding radius,
// centroid), scenario determinism, non-overlapping insertion, the thesis 7.3
// two-body setup, and per-particle field consistency.

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "contact/Scenario.hpp"

using namespace contact;

namespace {

int g_failures = 0;
int g_checks = 0;

void check(bool ok, const std::string& what) {
    ++g_checks;
    if (!ok) {
        ++g_failures;
        std::printf("  FAIL  %s\n", what.c_str());
    }
}

void checkClose(Real got, Real want, Real tol, const std::string& what) {
    ++g_checks;
    if (!(std::abs(got - want) <= tol)) {
        ++g_failures;
        std::printf("  FAIL  %s: got %.17g want %.17g (tol %.3g)\n", what.c_str(), got, want, tol);
    }
}

Real meshVolume(const Mesh& mesh) {
    Real sum6 = 0.0;
    for (const Triangle& t : mesh) sum6 += dot(t.v[0], cross(t.v[1], t.v[2]));
    return sum6 / 6.0;   // signed: positive when outward oriented
}

bool isClosedManifold(const Mesh& mesh, std::string& why) {
    std::map<std::pair<long, long>, int> edge_count;
    // Quantise vertices so shared corners are recognised exactly.
    std::vector<std::array<long long, 3>> keys;
    auto vertexKey = [&](const Vec3& p) {
        std::array<long long, 3> k{};
        for (int d = 0; d < 3; ++d) k[d] = static_cast<long long>(std::llround(p[d] * 1e12));
        return k;
    };
    for (const Triangle& t : mesh) {
        for (int e = 0; e < 3; ++e) {
            const auto ka = vertexKey(t.v[e]);
            const auto kb = vertexKey(t.v[(e + 1) % 3]);
            if (ka == kb) {
                why = "degenerate edge (repeated vertex)";
                return false;
            }
            long a = 0, b = 0;
            for (int d = 0; d < 3; ++d) {
                a = a * 1000003 + ka[d];
                b = b * 1000003 + kb[d];
            }
            if (a > b) std::swap(a, b);
            ++edge_count[std::make_pair(a, b)];
        }
    }
    for (const auto& kv : edge_count) {
        if (kv.second != 2) {
            why = "edge shared by " + std::to_string(kv.second) + " triangles";
            return false;
        }
    }
    return true;
}

void reportShape(ShapeKind kind, const char* name, Real radius, unsigned seed) {
    const Mesh mesh = makeShape(kind, radius, seed);

    std::printf("  %-10s seed=%-6u faces=%3zu  volume=%.9e  radius=%.17g\n", name, seed,
                mesh.size(), meshVolume(mesh),
                [&] {
                    Real m = 0.0;
                    for (const Triangle& t : mesh)
                        for (int k = 0; k < 3; ++k) m = std::max(m, norm(t.v[k]));
                    return m;
                }());
}

double boundingRadius(const Mesh& mesh) {
    Real m = 0.0;
    for (const Triangle& t : mesh)
        for (int k = 0; k < 3; ++k) m = std::max(m, norm(t.v[k]));
    return m;
}

Vec3 volumeCentroid(const Mesh& mesh) {
    Real sum6 = 0.0;
    Vec3 acc{0.0, 0.0, 0.0};
    for (const Triangle& t : mesh) {
        const Real d = dot(t.v[0], cross(t.v[1], t.v[2]));
        sum6 += d;
        acc += (t.v[0] + t.v[1] + t.v[2]) * d;
    }
    if (std::abs(sum6) < 1e-300) return Vec3{0.0, 0.0, 0.0};
    return acc / (4.0 * sum6);
}

void testShape(ShapeKind kind, const char* name, int expected_faces, Real radius, unsigned seed) {
    const Mesh mesh = makeShape(kind, radius, seed);
    const std::string tag = std::string(name) + " seed=" + std::to_string(seed);

    if (expected_faces > 0) {
        check(static_cast<int>(mesh.size()) == expected_faces,
              tag + ": expected " + std::to_string(expected_faces) + " faces, got " +
                  std::to_string(mesh.size()));
    } else {
        check(mesh.size() >= 12, tag + ": expected >= 12 faces, got " + std::to_string(mesh.size()));
    }

    std::string why;
    check(isClosedManifold(mesh, why), tag + ": closed 2-manifold (" + why + ")");

    const Real vol = meshVolume(mesh);
    check(vol > 0.0, tag + ": signed volume > 0 (outward winding)");
    check(std::isfinite(vol) && vol > 1e-12 * radius * radius * radius,
          tag + ": non-degenerate volume");

    checkClose(boundingRadius(mesh), radius, 1e-9 * radius, tag + ": bounding radius");

    const Vec3 c = volumeCentroid(mesh);
    checkClose(norm(c), 0.0, 1e-12 * radius, tag + ": volume centroid at origin");

    // Convex, outward oriented faces: the face normal must point away from the
    // centre (all shapes here are convex hulls or near-spherical).
    int outward = 0;
    for (const Triangle& t : mesh) {
        const Vec3 n = cross(t.v[1] - t.v[0], t.v[2] - t.v[0]);
        const Vec3 mid = (t.v[0] + t.v[1] + t.v[2]) / 3.0;
        if (dot(n, mid) > 0.0) ++outward;
    }
    check(outward == static_cast<int>(mesh.size()),
          tag + ": all face normals outward (" + std::to_string(outward) + "/" +
              std::to_string(mesh.size()) + ")");

    // No vertex outside the bounding sphere.
    Real mx = 0.0;
    for (const Triangle& t : mesh)
        for (int k = 0; k < 3; ++k) mx = std::max(mx, norm(t.v[k]));
    check(mx <= radius * (1.0 + 1e-12), tag + ": vertices inside bounding sphere");
}

void testShapes() {
    std::printf("[shapes] mesh validity\n");
    testShape(ShapeKind::Tetra, "tetra", 4, 0.05, 0u);
    testShape(ShapeKind::Octa, "octa", 8, 0.05, 0u);
    testShape(ShapeKind::Icosa, "icosa", 20, 0.05, 0u);
    for (unsigned seed : {1u, 2u, 7u, 42u, 12345u, 999983u}) {
        const Mesh m = makeShape(ShapeKind::RandomHull, 0.03, seed);
        testShape(ShapeKind::RandomHull, "randhull", 0, 0.03, seed);
        check(m.size() >= 12, "randhull: >= 12 triangles");
    }
    // Scaling must be linear in the requested radius.
    const Mesh a = makeShape(ShapeKind::Icosa, 1.0, 0u);
    const Mesh b = makeShape(ShapeKind::Icosa, 0.25, 0u);
    checkClose(meshVolume(b) / meshVolume(a), 0.25 * 0.25 * 0.25, 1e-12, "icosa: volume scales r^3");

    // Name round-trip, including the strings used by the CLI.
    ShapeKind parsed = ShapeKind::Tetra;
    check(shapeFromString("icosahedron", parsed) && parsed == ShapeKind::Icosa,
          "shapeFromString(icosahedron)");
    check(shapeFromString("Tetra", parsed) && parsed == ShapeKind::Tetra, "shapeFromString(Tetra)");
    check(shapeFromString("random-hull", parsed) && parsed == ShapeKind::RandomHull,
          "shapeFromString(random-hull)");
    check(shapeFromString("octa", parsed) && parsed == ShapeKind::Octa, "shapeFromString(octa)");
    check(!shapeFromString("sphere", parsed), "shapeFromString rejects unknown");
    check(std::string(toString(ShapeKind::RandomHull)) == "randomhull", "toString(randomhull)");

    std::printf("[shapes] mesh inventory\n");
    reportShape(ShapeKind::Tetra, "tetra", 0.05, 0u);
    reportShape(ShapeKind::Octa, "octa", 0.05, 0u);
    reportShape(ShapeKind::Icosa, "icosa", 0.05, 0u);
    for (unsigned seed : {1u, 2u, 7u, 42u, 12345u, 999983u}) {
        reportShape(ShapeKind::RandomHull, "randomhull", 0.03, seed);
    }
}

void testParticleFields(const std::vector<Particle>& bodies, const std::string& tag) {
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        const Particle& p = bodies[i];
        check(p.id == static_cast<int>(i), tag + ": id sequential");
        check(p.level == 0, tag + ": level 0");
        check(p.quat.w == 1.0 && p.quat.x == 0.0 && p.quat.y == 0.0 && p.quat.z == 0.0,
              tag + ": identity quaternion");
        check(p.radius > 0.0, tag + ": positive radius");
        checkClose(p.halo, 1.1 * p.radius, 1e-15, tag + ": halo = 1.1 r");
        const Real vol = std::abs(meshVolume(p.mesh));
        checkClose(p.mass, vol, 1e-12 * std::max(1.0, vol), tag + ": mass == mesh volume");
        check(p.mass > 0.0, tag + ": positive mass");
        checkClose(p.inv_mass, 1.0 / p.mass, 1e-12 * p.inv_mass, tag + ": inv_mass");
        checkClose(p.inertia, 0.4 * p.mass * p.radius * p.radius, 1e-12 * p.inertia,
                   tag + ": inertia = 0.4 m r^2");
        checkClose(p.inv_inertia, 1.0 / p.inertia, 1e-12 * p.inv_inertia, tag + ": inv_inertia");
        check(p.force[0] == 0.0 && p.force[1] == 0.0 && p.force[2] == 0.0,
              tag + ": zero initial force");
    }
}

void testScenario(const ScenarioSpec& spec, const Aabb& domain, const std::string& tag) {
    const std::vector<Particle> a = buildScenario(spec, domain);
    const std::vector<Particle> b = buildScenario(spec, domain);

    check(a.size() == static_cast<std::size_t>(spec.count),
          tag + ": produced " + std::to_string(a.size()) + " bodies");
    check(a.size() == b.size(), tag + ": same size twice");

    bool identical = (a.size() == b.size());
    for (std::size_t i = 0; i < a.size() && identical; ++i) {
        identical = std::memcmp(&a[i].pos, &b[i].pos, sizeof(Vec3)) == 0 &&
                    std::memcmp(&a[i].vel, &b[i].vel, sizeof(Vec3)) == 0 &&
                    a[i].radius == b[i].radius && a[i].mass == b[i].mass;
    }
    check(identical, tag + ": deterministic (bit-identical positions/velocities)");

    testParticleFields(a, tag);

    // Inside the domain, fully.
    bool inside = true;
    for (const Particle& p : a) {
        for (int d = 0; d < 3; ++d) {
            if (p.pos[d] - p.radius < domain.lo[d] - 1e-12 ||
                p.pos[d] + p.radius > domain.hi[d] + 1e-12) {
                inside = false;
            }
        }
    }
    check(inside, tag + ": bodies fully inside the domain");

    // No initial bounding-sphere overlap.
    double worst_gap = 1e300;
    bool clean = true;
    for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = i + 1; j < a.size(); ++j) {
            const Real rr = a[i].radius + a[j].radius;
            const Real d = norm(a[i].pos - a[j].pos);
            worst_gap = std::min(worst_gap, (d - rr) / rr);
            if (d < rr * (1.0 - 1e-12)) clean = false;
        }
    }
    check(clean, tag + ": no initial bounding-sphere overlap");
    Real dmin = 1e300;
    Real dmax = 0.0;
    for (const Particle& p : a) {
        dmin = std::min(dmin, 2.0 * p.radius);
        dmax = std::max(dmax, 2.0 * p.radius);
    }
    std::printf("    %-28s n=%zu  min gap/radius=%+.3e  diameters=[%.4f, %.4f]\n", tag.c_str(),
                a.size(), worst_gap, dmin, dmax);

    // Velocities.
    if (spec.velocity != 0.0) {
        bool speeds = true;
        for (const Particle& p : a) speeds = speeds && std::abs(norm(p.vel) - spec.velocity) < 1e-12;
        check(speeds, tag + ": initial speed == spec.velocity");
    } else {
        bool zero = true;
        for (const Particle& p : a) zero = zero && norm2(p.vel) == 0.0;
        check(zero, tag + ": zero initial velocity");
    }
}

void testScenarioDeterminismSameDomain() {
    std::printf("[scenario] lattice insertion, determinism, non-overlap\n");
    const Aabb unit{Vec3{0.0, 0.0, 0.0}, Vec3{1.0, 1.0, 1.0}};

    ScenarioSpec s;
    s.name = "default-200";
    s.count = 200;
    s.shape = ShapeKind::Icosa;
    s.size_min = 0.02;
    s.size_max = 0.06;
    s.seed = 12345;
    testScenario(s, unit, "default-200");

    ScenarioSpec m = s;
    m.count = 60;
    m.shape = ShapeKind::RandomHull;
    m.size_min = 0.01;
    m.size_max = 0.04;
    m.seed = 7;
    m.velocity = 0.25;
    testScenario(m, unit, "randomhull-60");

    // Different seed must change the layout.
    ScenarioSpec s2 = s;
    s2.seed = 999;
    const std::vector<Particle> p1 = buildScenario(s, unit);
    const std::vector<Particle> p2 = buildScenario(s2, unit);
    bool differ = false;
    for (std::size_t i = 0; i < std::min(p1.size(), p2.size()); ++i) {
        if (std::memcmp(&p1[i].pos, &p2[i].pos, sizeof(Vec3)) != 0) differ = true;
    }
    check(differ, "different seed => different layout");

    // Small cases.
    ScenarioSpec one = s;
    one.count = 1;
    one.seed = 3;
    const auto single = buildScenario(one, unit);
    check(single.size() == 1, "count=1 produces one body");

    ScenarioSpec none = s;
    none.count = 0;
    check(buildScenario(none, unit).empty(), "count=0 produces no bodies");

    // Non-cubic domain.
    const Aabb box{Vec3{-1.0, 0.0, 0.5}, Vec3{1.0, 0.5, 1.5}};
    ScenarioSpec boxed = s;
    boxed.count = 40;
    boxed.size_min = 0.02;
    boxed.size_max = 0.05;
    boxed.seed = 11;
    testScenario(boxed, box, "rect-domain-40");
}

void testTwoBody() {
    std::printf("[scenario] thesis 7.3 two-body setup\n");
    const Aabb unit{Vec3{0.0, 0.0, 0.0}, Vec3{1.0, 1.0, 1.0}};
    const std::vector<Particle> b = buildTwoBodyCollision(unit);

    check(b.size() == 2, "two-body: exactly 2 bodies");
    check(b[0].id == 0 && b[1].id == 1, "two-body: ids 0,1");

    checkClose(b[0].pos[0], 0.2, 0.0, "two-body: small body x");
    checkClose(b[0].pos[1], 0.2, 0.0, "two-body: small body y");
    checkClose(b[0].pos[2], 0.2, 0.0, "two-body: small body z");
    checkClose(b[1].pos[0], 0.8, 0.0, "two-body: large body x");
    checkClose(b[1].pos[1], 0.8, 0.0, "two-body: large body y");
    checkClose(b[1].pos[2], 0.8, 0.0, "two-body: large body z");

    checkClose(b[0].vel[0], 0.1, 0.0, "two-body: small body vx");
    checkClose(b[0].vel[1], 0.1, 0.0, "two-body: small body vy");
    checkClose(b[0].vel[2], 0.1, 0.0, "two-body: small body vz");
    checkClose(b[1].vel[0], -0.1, 0.0, "two-body: large body vx");
    checkClose(b[1].vel[1], -0.1, 0.0, "two-body: large body vy");
    checkClose(b[1].vel[2], -0.1, 0.0, "two-body: large body vz");

    checkClose(2.0 * b[0].radius, 0.02, 1e-15, "two-body: small diameter 0.02");
    checkClose(2.0 * b[1].radius, 0.20, 1e-15, "two-body: large diameter 0.2");

    const Real sep = norm(b[1].pos - b[0].pos);
    checkClose(sep, std::sqrt(3.0) * 0.6, 1e-15, "two-body: centre distance sqrt(3)*0.6");

    testParticleFields(b, "two-body");

    // Custom diameters are honoured.
    const std::vector<Particle> c = buildTwoBodyCollision(unit, 0.05, 0.5);
    checkClose(2.0 * c[0].radius, 0.05, 1e-15, "two-body custom: da");
    checkClose(2.0 * c[1].radius, 0.5, 1e-15, "two-body custom: db");
}

}  // namespace

int main() {
    std::printf("== contact scenario tests ==\n");
    testShapes();
    testScenarioDeterminismSameDomain();
    testTwoBody();

    std::printf("\n%zu checks, %d failure(s)\n", static_cast<std::size_t>(g_checks), g_failures);
    if (g_failures != 0) {
        std::printf("RESULT: FAIL\n");
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
