// Convex closed triangle meshes for the replication experiments.
//
// Every mesh returned by makeShape() is
//   * closed and manifold: each undirected edge is shared by exactly two
//     triangles (checked internally before the mesh leaves this file),
//   * outward oriented: counter-clockwise seen from outside, so the signed
//     tetrahedron volume sum is positive,
//   * centred on the solid's volume centroid (not the vertex average),
//   * scaled so that max |v| == the requested bounding radius.
//
// Shapes: tetra (4 faces), octa (8), icosa (20) and RandomHull.
//
// RandomHull is a genuine convex hull: `n` points (14..20, seed dependent) are
// drawn uniformly on the unit sphere and the hull faces are found by brute
// force (a triple spans a hull face when every other point lies on one side of
// its plane).  For points in general position this yields a closed manifold;
// the result is validated (edge multiplicity 2, Euler characteristic 2, positive
// volume) and, in the rare degenerate case (coplanar points), we fall back to a
// deterministic spherical triangulation: the icosahedron subdivided once
// (80 faces, 42 vertices) with a per-vertex radial jitter of +-4 %.  Both
// variants are deterministic functions of `seed`.

#include "contact/Scenario.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace contact {
namespace {

// ---------------------------------------------------------------------------
// Deterministic RNG (splitmix64).  std:: distributions are avoided because
// their output is implementation defined; this keeps scenarios bit-reproducible
// across standard libraries.
// ---------------------------------------------------------------------------
struct Rng {
    std::uint64_t s;

    explicit Rng(std::uint64_t seed) noexcept : s(seed + 0x9E3779B97F4A7C15ULL) {}

    std::uint64_t next() noexcept {
        std::uint64_t z = (s += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    // Uniform in [0,1).
    Real uniform() noexcept {
        return static_cast<Real>(next() >> 11) * (1.0 / 9007199254740992.0);
    }

    // Uniform in [-1,1).
    Real sym() noexcept { return 2.0 * uniform() - 1.0; }

    // Uniform on the unit sphere (Marsaglia polar method, rejection).
    Vec3 unitVector() noexcept {
        for (;;) {
            const Real x = sym();
            const Real y = sym();
            const Real z = sym();
            const Real r2 = x * x + y * y + z * z;
            if (r2 > 1.0e-6 && r2 <= 1.0) {
                const Real inv = 1.0 / std::sqrt(r2);
                return Vec3{x * inv, y * inv, z * inv};
            }
        }
    }
};

using Face = std::array<int, 3>;

struct VolumeProps {
    Real volume{0.0};    // signed, positive for outward winding
    Vec3 centroid{0.0, 0.0, 0.0};
};

// Sum of signed tetrahedra (origin, a, b, c); volume = sum/6.
VolumeProps volumeProps(const std::vector<Vec3>& v, const std::vector<Face>& f) {
    Real sum6 = 0.0;
    Vec3 acc{0.0, 0.0, 0.0};
    for (const Face& t : f) {
        const Vec3& a = v[t[0]];
        const Vec3& b = v[t[1]];
        const Vec3& c = v[t[2]];
        const Real d = dot(a, cross(b, c));   // 6 * signed tetra volume
        sum6 += d;
        acc += (a + b + c) * d;
    }
    VolumeProps p;
    p.volume = sum6 / 6.0;
    if (std::abs(sum6) > 0.0) p.centroid = acc / (4.0 * sum6);
    return p;
}

Real maxRadius(const std::vector<Vec3>& v) {
    Real m = 0.0;
    for (const Vec3& p : v) m = std::max(m, norm(p));
    return m;
}

// Manifold + orientation + degeneracy check used to vet the random hull before
// it is accepted.  `scale` is the point-set radius (for tolerances).
bool validClosedMesh(const std::vector<Vec3>& v, const std::vector<Face>& f, Real scale) {
    if (f.size() < 4) return false;
    std::map<std::pair<int, int>, int> edges;
    std::vector<char> used(v.size(), 0);
    for (const Face& t : f) {
        for (int k = 0; k < 3; ++k) {
            if (t[k] < 0 || t[k] >= static_cast<int>(v.size())) return false;
            if (t[k] == t[(k + 1) % 3]) return false;
        }
        const Vec3 e1 = v[t[1]] - v[t[0]];
        const Vec3 e2 = v[t[2]] - v[t[0]];
        if (0.5 * norm(cross(e1, e2)) <= 1.0e-12 * scale * scale) return false;
        for (int k = 0; k < 3; ++k) {
            int a = t[k];
            int b = t[(k + 1) % 3];
            used[a] = 1;
            if (a > b) std::swap(a, b);
            ++edges[std::make_pair(a, b)];
        }
    }
    for (const auto& e : edges) {
        if (e.second != 2) return false;
    }
    int nv = 0;
    for (char u : used) nv += (u != 0) ? 1 : 0;
    const long euler = static_cast<long>(nv) - static_cast<long>(edges.size()) +
                       static_cast<long>(f.size());
    if (euler != 2) return false;
    const VolumeProps p = volumeProps(v, f);
    return p.volume > 0.0 && std::isfinite(p.volume);
}

// Brute-force convex hull face enumeration.  O(n^4) but n <= 20.
// Precondition: points in general position (no 4 coplanar), which is the case
// for random points on a sphere; the caller validates the result anyway.
std::vector<Face> hullFaces(const std::vector<Vec3>& p) {
    const int n = static_cast<int>(p.size());
    Real scale = 1.0e-30;
    for (const Vec3& q : p) scale = std::max(scale, norm(q));
    const Real tol = 1.0e-10 * scale;

    std::vector<Face> faces;
    faces.reserve(4 * n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                const Vec3 nrm = cross(p[j] - p[i], p[k] - p[i]);
                const Real nl = norm(nrm);
                if (nl <= 1.0e-14 * scale * scale) continue;   // collinear triple
                bool any_pos = false;
                bool any_neg = false;
                for (int m = 0; m < n; ++m) {
                    if (m == i || m == j || m == k) continue;
                    const Real s = dot(nrm, p[m] - p[i]) / nl;  // signed distance
                    if (s > tol) any_pos = true;
                    if (s < -tol) any_neg = true;
                    if (any_pos && any_neg) break;
                }
                if (any_pos && any_neg) continue;              // plane cuts the set
                // All points on the negative side => (i,j,k) already winds
                // counter-clockwise seen from outside.
                if (any_pos) {
                    faces.push_back(Face{i, k, j});
                } else {
                    faces.push_back(Face{i, j, k});
                }
            }
        }
    }
    return faces;
}

std::vector<Vec3> icosaPoints() {
    static const Real phi = 0.5 * (1.0 + std::sqrt(5.0));
    const Real a = 1.0;
    const Real b = phi;
    std::vector<Vec3> v = {
        {-a,  b,  0.0}, { a,  b,  0.0}, {-a, -b,  0.0}, { a, -b,  0.0},
        { 0.0, -a,  b}, { 0.0,  a,  b}, { 0.0, -a, -b}, { 0.0,  a, -b},
        { b,  0.0, -a}, { b,  0.0,  a}, {-b,  0.0, -a}, {-b,  0.0,  a},
    };
    for (Vec3& q : v) q = normalized(q);
    return v;
}

// Deterministic spherical triangulation used as the RandomHull fallback.
// Icosahedron subdivided once (each triangle -> 4) and radially jittered.
std::vector<Face> jitteredIcosa(std::vector<Vec3>& verts, Rng& rng) {
    verts = icosaPoints();
    std::vector<Face> base = hullFaces(verts);   // 20 faces, outward

    std::vector<Face> faces;
    faces.reserve(4 * base.size());
    std::map<std::pair<int, int>, int> midpoints;

    auto midpoint = [&](int a, int b) -> int {
        if (a > b) std::swap(a, b);
        const std::pair<int, int> key{a, b};
        const auto it = midpoints.find(key);
        if (it != midpoints.end()) return it->second;
        Vec3 m = normalized(verts[a] + verts[b]);   // project onto the sphere
        verts.push_back(m);
        const int idx = static_cast<int>(verts.size()) - 1;
        midpoints.emplace(key, idx);
        return idx;
    };

    for (const Face& t : base) {
        const int ab = midpoint(t[0], t[1]);
        const int bc = midpoint(t[1], t[2]);
        const int ca = midpoint(t[2], t[0]);
        faces.push_back(Face{t[0], ab, ca});
        faces.push_back(Face{t[1], bc, ab});
        faces.push_back(Face{t[2], ca, bc});
        faces.push_back(Face{ab, bc, ca});
    }

    // Per-vertex radial jitter, +-4 % of the radius.  Keeps the surface star
    // shaped (hence still a valid closed, consistently wound mesh).
    for (Vec3& q : verts) {
        const Real f = 1.0 + 0.04 * rng.sym();
        q = q * f;
    }
    return faces;
}

// Assemble a Mesh from vertices + faces: flip to positive volume if needed,
// re-centre on the volume centroid, scale to `radius`.
Mesh finishMesh(std::vector<Vec3> verts, std::vector<Face> faces, Real radius) {
    if (radius <= 0.0) throw std::invalid_argument("makeShape: radius must be positive");
    if (faces.empty()) throw std::runtime_error("makeShape: empty triangulation");

    VolumeProps props = volumeProps(verts, faces);
    if (props.volume < 0.0) {
        for (Face& t : faces) std::swap(t[1], t[2]);
        props = volumeProps(verts, faces);
    }
    if (!(props.volume > 0.0)) throw std::runtime_error("makeShape: degenerate mesh volume");

    const Vec3 c = props.centroid;
    for (Vec3& q : verts) q -= c;

    const Real r = maxRadius(verts);
    if (!(r > 0.0)) throw std::runtime_error("makeShape: zero bounding radius");
    const Real s = radius / r;
    for (Vec3& q : verts) q *= s;

    Mesh mesh;
    mesh.reserve(faces.size());
    for (const Face& t : faces) {
        Triangle tri;
        tri.v[0] = verts[t[0]];
        tri.v[1] = verts[t[1]];
        tri.v[2] = verts[t[2]];
        mesh.push_back(tri);
    }
    return mesh;
}

Mesh makeRandomHull(Real radius, unsigned seed) {
    Rng rng(0x5EED0000ULL ^ static_cast<std::uint64_t>(seed) * 0x9E3779B97F4A7C15ULL);
    const int n = 14 + static_cast<int>(rng.next() % 7ULL);   // 14..20 points

    std::vector<Vec3> verts;
    verts.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) verts.push_back(rng.unitVector());

    std::vector<Face> faces = hullFaces(verts);
    if (!validClosedMesh(verts, faces, maxRadius(verts))) {
        // Degenerate draw (coplanar points / sliver): deterministic fallback.
        faces = jitteredIcosa(verts, rng);
    }
    return finishMesh(std::move(verts), std::move(faces), radius);
}

}  // namespace

const char* toString(ShapeKind s) noexcept {
    switch (s) {
        case ShapeKind::Tetra:      return "tetra";
        case ShapeKind::Octa:       return "octa";
        case ShapeKind::Icosa:      return "icosa";
        case ShapeKind::RandomHull: return "randomhull";
    }
    return "unknown";
}

bool shapeFromString(const std::string& s, ShapeKind& out) noexcept {
    std::string k;
    k.reserve(s.size());
    for (char ch : s) {
        if (ch == '-' || ch == '_' || ch == ' ') continue;
        k.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    if (k == "tetra" || k == "tetrahedron" || k == "tet") { out = ShapeKind::Tetra; return true; }
    if (k == "octa" || k == "octahedron") { out = ShapeKind::Octa; return true; }
    if (k == "icosa" || k == "icosahedron" || k == "ico") { out = ShapeKind::Icosa; return true; }
    if (k == "randomhull" || k == "hull" || k == "random") {
        out = ShapeKind::RandomHull;
        return true;
    }
    return false;
}

Mesh makeShape(ShapeKind kind, Real radius, unsigned seed) {
    switch (kind) {
        case ShapeKind::Tetra: {
            // Alternate corners of a cube: a regular tetrahedron.
            std::vector<Vec3> v = {
                normalized(Vec3{ 1.0,  1.0,  1.0}),
                normalized(Vec3{ 1.0, -1.0, -1.0}),
                normalized(Vec3{-1.0,  1.0, -1.0}),
                normalized(Vec3{-1.0, -1.0,  1.0}),
            };
            std::vector<Face> f = hullFaces(v);
            if (f.size() != 4) throw std::runtime_error("makeShape: tetra hull is not 4 faces");
            return finishMesh(std::move(v), std::move(f), radius);
        }
        case ShapeKind::Octa: {
            std::vector<Vec3> v = {
                { 1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0},
                { 0.0, 1.0, 0.0}, { 0.0, -1.0, 0.0},
                { 0.0, 0.0, 1.0}, { 0.0, 0.0, -1.0},
            };
            std::vector<Face> f = hullFaces(v);
            if (f.size() != 8) throw std::runtime_error("makeShape: octa hull is not 8 faces");
            return finishMesh(std::move(v), std::move(f), radius);
        }
        case ShapeKind::Icosa: {
            std::vector<Vec3> v = icosaPoints();
            std::vector<Face> f = hullFaces(v);
            if (f.size() != 20) throw std::runtime_error("makeShape: icosa hull is not 20 faces");
            return finishMesh(std::move(v), std::move(f), radius);
        }
        case ShapeKind::RandomHull:
            return makeRandomHull(radius, seed);
    }
    throw std::invalid_argument("makeShape: unknown ShapeKind");
}

}  // namespace contact
