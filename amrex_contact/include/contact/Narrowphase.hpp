#pragma once
// Narrowphase: triangle-to-triangle minimum distance.
//
// Replicates thesis chapter 5:
//   5.4 / Algorithm 2  brute force: 6 point-to-triangle + 9 segment-to-segment
//   5.5 / Algorithm 5  iterative (Johnson sub-distance / GJK style)
//   5.6                hybrid: iterative first, brute-force fallback when the
//                      iterative solver cannot certify its answer
//
// Pure C++17, no AMReX.

#include "contact/Types.hpp"

namespace contact {

// Hybrid     : iterative pass as a certification gate, but the *exact* brute
//              answer is always returned, so the contact set is identical to
//              brute force (robustness guarantee, no speed win).
// HybridFast : thesis 5.6 "hybrid on triangle pairs" -- return the penalty
//              solver's answer when it certifies, fall back to brute force
//              otherwise (speed win, within the solver tolerance).
enum class NarrowphaseKind { Brute, Iterative, Hybrid, HybridFast };

inline const char* toString(NarrowphaseKind k) noexcept {
    switch (k) {
        case NarrowphaseKind::Brute: return "brute";
        case NarrowphaseKind::Iterative: return "iterative";
        case NarrowphaseKind::Hybrid: return "hybrid";
        case NarrowphaseKind::HybridFast: return "hybrid-fast";
    }
    return "unknown";
}

struct TriDist {
    Real dist{0.0};       // minimum distance, 0 when the triangles intersect
    Vec3 p{};             // closest point on A
    Vec3 q{};             // closest point on B
    bool overlap{false};  // true when the triangles intersect/penetrate
    int feature_a{-1};    // 0..2 vertex, 3..5 edge (see .cpp for the encoding)
    int feature_b{-1};
    bool fallback{false};
};

// Algorithm 4: point P to triangle (a,b,c), closest point written to `closest`.
Real pointTriangleDistance(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c,
                           Vec3& closest) noexcept;

// Algorithm 3: segment [p0,p1] to segment [q0,q1]; closest points written to
// `cp` / `cq`. Returns the distance.
Real segmentSegmentDistance(const Vec3& p0, const Vec3& p1, const Vec3& q0, const Vec3& q1,
                            Vec3& cp, Vec3& cq) noexcept;

// Algorithm 2: exhaustive, robust, branchy.
TriDist triangleTriangleBrute(const Triangle& A, const Triangle& B) noexcept;

// Algorithm 5: iterative simplex sub-distance; fast, may fail on degenerate
// (coplanar / touching / penetrating) configurations. `certified` is set false
// when the iteration hits its cap or the simplex degenerates.
TriDist triangleTriangleIterative(const Triangle& A, const Triangle& B, bool& certified) noexcept;

// Dispatcher honouring NarrowphaseKind.
TriDist trianglePairDistance(const Triangle& A, const Triangle& B,
                            NarrowphaseKind kind, bool* used_fallback = nullptr) noexcept;

struct NarrowphaseStats {
    long tri_pairs{0};
    long sphere_rejects{0};
    long brute_calls{0};
    long iterative_calls{0};
    long fallbacks{0};
    long contacts{0};

    void reset() noexcept { *this = NarrowphaseStats{}; }
};

class Narrowphase {
  public:
    explicit Narrowphase(NarrowphaseKind kind = NarrowphaseKind::Hybrid,
                         Real margin = 0.0) noexcept
        : kind_(kind), margin_(margin) {}

    // Broadphase already produced the candidate pair; here the bounding spheres
    // are re-checked (thesis 7.2 uses the halo spheres as an explicit filter)
    // and every triangle pair is tested until the minimum is certified.
    //
    // Returns true when a contact within `margin` was found. `out` is filled
    // with the deepest contact.
    bool query(const Particle& a, const Particle& b, ContactPoint& out);

    // Same, but uses `margin` instead of the constructor value.
    bool query(const Particle& a, const Particle& b, Real margin, ContactPoint& out);

    void resetStats() noexcept { stats_.reset(); }
    [[nodiscard]] const NarrowphaseStats& stats() const noexcept { return stats_; }
    [[nodiscard]] NarrowphaseKind kind() const noexcept { return kind_; }
    void setKind(NarrowphaseKind k) noexcept { kind_ = k; }

  private:
    NarrowphaseKind kind_;
    Real margin_;
    NarrowphaseStats stats_;
};

}  // namespace contact
