#pragma once
// Core value types for the DEM contact-dynamics replication of
// Krestenitis (2018), "Parallel Multiscale Contact Dynamics for Rigid
// Non-spherical Bodies" (Durham thesis), on top of AMReX.
//
// This header is deliberately AMReX-free so that geometry / physics modules
// can be unit-tested as plain C++.

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace contact {

using Real = double;
using Vec3 = std::array<Real, 3>;

constexpr Real kEps = 1.0e-12;

inline Vec3 makeVec3(Real x, Real y, Real z) noexcept { return Vec3{x, y, z}; }
inline Vec3 operator+(const Vec3& a, const Vec3& b) noexcept { return {a[0]+b[0], a[1]+b[1], a[2]+b[2]}; }
inline Vec3 operator-(const Vec3& a, const Vec3& b) noexcept { return {a[0]-b[0], a[1]-b[1], a[2]-b[2]}; }
inline Vec3 operator-(const Vec3& a) noexcept { return {-a[0], -a[1], -a[2]}; }
inline Vec3 operator*(const Vec3& a, Real s) noexcept { return {a[0]*s, a[1]*s, a[2]*s}; }
inline Vec3 operator*(Real s, const Vec3& a) noexcept { return a * s; }
inline Vec3 operator/(const Vec3& a, Real s) noexcept { return {a[0]/s, a[1]/s, a[2]/s}; }
inline Vec3& operator+=(Vec3& a, const Vec3& b) noexcept { a[0]+=b[0]; a[1]+=b[1]; a[2]+=b[2]; return a; }
inline Vec3& operator-=(Vec3& a, const Vec3& b) noexcept { a[0]-=b[0]; a[1]-=b[1]; a[2]-=b[2]; return a; }
inline Vec3& operator*=(Vec3& a, Real s) noexcept { a[0]*=s; a[1]*=s; a[2]*=s; return a; }

inline Real dot(const Vec3& a, const Vec3& b) noexcept { return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]; }
inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
    return {a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]};
}
inline Real norm2(const Vec3& a) noexcept { return dot(a, a); }
inline Real norm(const Vec3& a) noexcept { return std::sqrt(norm2(a)); }
inline Vec3 normalized(const Vec3& a) noexcept {
    const Real n = norm(a);
    return (n > kEps) ? a / n : Vec3{0.0, 0.0, 0.0};
}
inline Real clamp(Real v, Real lo, Real hi) noexcept { return (v < lo) ? lo : ((v > hi) ? hi : v); }

// ---------------------------------------------------------------------------
// Geometry
// ---------------------------------------------------------------------------

struct Triangle {
    Vec3 v[3]{};
};

using Mesh = std::vector<Triangle>;  // body frame, centroid at origin

// Unit quaternion, body -> world rotation.
struct Quat {
    Real w{1.0}, x{0.0}, y{0.0}, z{0.0};
};

inline Vec3 rotate(const Quat& q, const Vec3& p) noexcept {
    const Vec3 u{q.x, q.y, q.z};
    const Vec3 t = 2.0 * cross(u, p);
    return p + q.w * t + cross(u, t);
}

inline Quat normalized(const Quat& q) noexcept {
    const Real n = std::sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    if (n <= kEps) return Quat{};
    return Quat{q.w/n, q.x/n, q.y/n, q.z/n};
}

// q_dot = 0.5 * omega_quat * q
inline Quat rotateRate(const Quat& q, const Vec3& omega) noexcept {
    return Quat{
        -0.5 * (omega[0]*q.x + omega[1]*q.y + omega[2]*q.z),
         0.5 * (omega[0]*q.w + omega[1]*q.z - omega[2]*q.y),
         0.5 * (omega[1]*q.w + omega[2]*q.x - omega[0]*q.z),
         0.5 * (omega[2]*q.w + omega[0]*q.y - omega[1]*q.x)};
}

// ---------------------------------------------------------------------------
// Bodies
// ---------------------------------------------------------------------------

struct Particle {
    int id{0};
    Vec3 pos{};      // centre of mass, world frame
    Vec3 vel{};
    Vec3 omega{};    // angular velocity
    Vec3 force{};
    Vec3 torque{};
    Quat quat{};

    Real radius{0.1};        // real-geometry bounding-sphere radius (rr)
    Real halo{0.11};         // 1.1 * radius, thesis eq. in 7.2
    Real mass{1.0};
    Real inv_mass{1.0};
    Real inertia{0.4};       // scalar (solid sphere) approximation
    Real inv_inertia{2.5};

    int level{0};            // grid level the body currently resides on
    int bin{0};              // scratch: last bin index
    Mesh mesh;               // body-frame triangles

    [[nodiscard]] Vec3 worldVertex(const Triangle& t, int k) const noexcept {
        return pos + rotate(quat, t.v[k]);
    }
};

struct Aabb {
    Vec3 lo{ std::numeric_limits<Real>::max(),  std::numeric_limits<Real>::max(),  std::numeric_limits<Real>::max()};
    Vec3 hi{-std::numeric_limits<Real>::max(), -std::numeric_limits<Real>::max(), -std::numeric_limits<Real>::max()};

    void grow(const Vec3& p) noexcept {
        for (int d = 0; d < 3; ++d) {
            lo[d] = (p[d] < lo[d]) ? p[d] : lo[d];
            hi[d] = (p[d] > hi[d]) ? p[d] : hi[d];
        }
    }
    void grow(const Aabb& b) noexcept { grow(b.lo); grow(b.hi); }
    void expand(Real m) noexcept { for (int d = 0; d < 3; ++d) { lo[d] -= m; hi[d] += m; } }
    [[nodiscard]] bool overlaps(const Aabb& b) const noexcept {
        for (int d = 0; d < 3; ++d) if (lo[d] > b.hi[d] || b.lo[d] > hi[d]) return false;
        return true;
    }
};

// ---------------------------------------------------------------------------
// Contacts
// ---------------------------------------------------------------------------

struct ContactPair {  // canonical: i < j
    int i{0};
    int j{0};
};

struct ContactPoint {
    int i{-1};
    int j{-1};
    Vec3 point{};        // world-frame contact point (midpoint of closest features)
    Vec3 normal{};       // unit, from i towards j
    Real depth{0.0};     // overlap along normal (>= 0 when touching)
    Real dist{0.0};      // minimum triangle-triangle distance
    int tri_a{-1};
    int tri_b{-1};
    bool fallback{false};   // narrowphase had to fall back to brute force
};

}  // namespace contact
