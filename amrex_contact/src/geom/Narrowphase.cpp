// Narrowphase driver (thesis 5.4-5.6, 7.2).
//
// `Narrowphase::query` takes a broadphase candidate pair, re-checks the two
// halo spheres (thesis 7.2 uses them as an explicit pre-filter), then walks
// every triangle pair of the two world-frame meshes and keeps the minimum
// distance.  The triangle-pair kernel is selected by NarrowphaseKind.
//
// Contact decision (SPEC section 2):
//   contact when d <= margin.  The spec writes "d < margin (default 0.0, i.e.
//   touching counts)"; a strict '<' would make the documented default margin
//   unable to ever report a contact (d >= 0), so the "touching counts" reading
//   is implemented.
//   normal = normalised(q - p) from a towards b (centres decide the side when
//   the closest points coincide, i.e. touching/penetrating),
//   point  = midpoint of (p, q), depth = margin - d.

#include "contact/Narrowphase.hpp"

#include <cmath>
#include <vector>

namespace contact {
namespace {

inline Triangle worldTriangle(const Particle& p, const Triangle& t) noexcept {
    Triangle w;
    for (int k = 0; k < 3; ++k) w.v[k] = p.worldVertex(t, k);
    return w;
}

}  // namespace

bool Narrowphase::query(const Particle& a, const Particle& b, ContactPoint& out) {
    return query(a, b, margin_, out);
}

bool Narrowphase::query(const Particle& a, const Particle& b, Real margin, ContactPoint& out) {
    // ---- bounding-sphere (halo) pre-check -----------------------------------
    const Vec3 delta = b.pos - a.pos;
    const Real centreDist = norm(delta);
    if (centreDist > a.halo + b.halo + margin) {
        ++stats_.sphere_rejects;
        return false;
    }

    // ---- empty meshes: sphere-only contact ----------------------------------
    if (a.mesh.empty() || b.mesh.empty()) {
        const Vec3 n = (centreDist > kEps) ? delta / centreDist : Vec3{1.0, 0.0, 0.0};
        const Real dist = std::max<Real>(0.0, centreDist - a.radius - b.radius);
        if (!(dist <= margin)) return false;

        const Vec3 pa = a.pos + n * a.radius;
        const Vec3 pb = b.pos - n * b.radius;

        out = ContactPoint{};
        out.i = a.id;
        out.j = b.id;
        out.normal = n;
        out.point = (pa + pb) * 0.5;
        out.dist = dist;
        out.depth = margin - dist;
        out.tri_a = -1;
        out.tri_b = -1;
        out.fallback = false;
        ++stats_.contacts;
        return true;
    }

    // ---- world-frame meshes -------------------------------------------------
    const int na = static_cast<int>(a.mesh.size());
    const int nb = static_cast<int>(b.mesh.size());
    std::vector<Triangle> wa(static_cast<std::size_t>(na));
    std::vector<Triangle> wb(static_cast<std::size_t>(nb));
    for (int i = 0; i < na; ++i) wa[static_cast<std::size_t>(i)] = worldTriangle(a, a.mesh[static_cast<std::size_t>(i)]);
    for (int j = 0; j < nb; ++j) wb[static_cast<std::size_t>(j)] = worldTriangle(b, b.mesh[static_cast<std::size_t>(j)]);

    Real dmin = std::numeric_limits<Real>::max();
    TriDist best;
    int triA = -1;
    int triB = -1;
    bool found = false;
    bool stop = false;

    for (int i = 0; i < na && !stop; ++i) {
        for (int j = 0; j < nb && !stop; ++j) {
            ++stats_.tri_pairs;
            bool fb = false;
            const TriDist r = trianglePairDistance(wa[static_cast<std::size_t>(i)],
                                                   wb[static_cast<std::size_t>(j)], kind_, &fb);
            switch (kind_) {
                case NarrowphaseKind::Brute:
                    ++stats_.brute_calls;
                    break;
                case NarrowphaseKind::Iterative:
                    ++stats_.iterative_calls;
                    break;
                case NarrowphaseKind::Hybrid:
                    ++stats_.iterative_calls;
                    ++stats_.brute_calls;   // the hybrid returns the brute answer
                    if (fb) ++stats_.fallbacks;
                    break;
            }
            if (r.dist < dmin) {
                dmin = r.dist;
                best = r;
                triA = i;
                triB = j;
                found = true;
            }
            // Early exit only on a *certified* zero: brute force and the hybrid
            // return the exact answer, the bare iterative path does not.
            if (r.dist <= 0.0 && kind_ != NarrowphaseKind::Iterative) stop = true;
        }
    }

    if (!found || !(dmin <= margin)) return false;

    Vec3 nrm = best.q - best.p;
    const Real nl = norm(nrm);
    if (nl > kEps) {
        nrm = nrm / nl;
    } else {
        // Touching / penetrating: the closest points coincide, so the centres
        // decide which side b is on (SPEC section 2).
        nrm = (centreDist > kEps) ? delta / centreDist : Vec3{1.0, 0.0, 0.0};
    }

    // Triangle-to-triangle distance saturates at zero as soon as the surfaces
    // intersect, so it says nothing about how deep two convex bodies have
    // interpenetrated. The penalty law needs that depth, so for the overlapping
    // case measure the overlap of the two support planes along the normal:
    // depth = max_{v in A}(v.n) + max_{v in B}(-v.n).
    Real depth = margin - dmin;
    if (dmin <= kEps) {
        const Real bad = -std::numeric_limits<Real>::max();
        Real sa = bad;
        Real sb = bad;
        for (const Triangle& t : a.mesh) {
            for (int k = 0; k < 3; ++k) sa = std::max(sa, dot(a.worldVertex(t, k), nrm));
        }
        for (const Triangle& t : b.mesh) {
            for (int k = 0; k < 3; ++k) sb = std::max(sb, dot(b.worldVertex(t, k), -nrm));
        }
        if (sa > bad && sb > bad) depth = std::max(depth, sa + sb);
    }

    out = ContactPoint{};
    out.i = a.id;
    out.j = b.id;
    out.normal = nrm;
    out.point = (best.p + best.q) * 0.5;
    out.dist = dmin;
    out.depth = depth;
    out.tri_a = triA;
    out.tri_b = triB;
    out.fallback = best.fallback;
    ++stats_.contacts;
    return true;
}

}  // namespace contact
