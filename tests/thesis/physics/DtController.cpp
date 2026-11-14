// Dynamic time step scheme, thesis chapter 7 (Algorithms 11 and 12), plus the
// ContactModel <-> string helpers.

#include "contact/Physics.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>

namespace contact {

namespace {

// Distance from the body centre to its surface along the unit direction n
// (world frame). For a triangulated convex body this is the support function;
// for a body without a mesh it degenerates to the bounding sphere, which keeps
// the pure-sphere unit tests exact.
Real supportAlong(const Particle& p, const Vec3& n) noexcept {
    Real s = -std::numeric_limits<Real>::max();
    for (const Triangle& t : p.mesh) {
        for (int k = 0; k < 3; ++k) s = std::max(s, dot(p.worldVertex(t, k), n));
    }
    if (s == -std::numeric_limits<Real>::max()) return dot(p.pos, n) + p.radius;
    return s;
}

}  // namespace

// ---------------------------------------------------------------------------
// ContactModel names
// ---------------------------------------------------------------------------
const char* toString(ContactModel m) noexcept {
    switch (m) {
        case ContactModel::Penalty: return "Penalty";
        case ContactModel::SpringDashpot: return "SpringDashpot";
    }
    return "Penalty";
}

bool contactModelFromString(const std::string& s, ContactModel& out) noexcept {
    std::string k;
    k.reserve(s.size());
    for (char c : s) {
        if (c == '-' || c == '_' || c == ' ' || c == '\t') continue;
        k.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    if (k == "penalty") { out = ContactModel::Penalty; return true; }
    if (k == "springdashpot" || k == "springdashpots" || k == "dashpot" || k == "sd") {
        out = ContactModel::SpringDashpot;
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Pair state, thesis 7.2
// ---------------------------------------------------------------------------
//
// d_AB = c_B - c_A, gap = |d_AB| - (rr_A + rr_B) is the minimum surface
// distance, and v_BA is the rate of change of that gap: negative while the
// pair closes.
//
// NOTE ON THE THESIS SIGN.  Section 7.2 prints
//     v_BA = (v_A . d_AB) - (v_B . d_AB)
// and then states that v_BA < 0 means approach and v_BA >= 0 means
// separation.  Those two statements are inconsistent: with d_AB = c_B - c_A
// the printed expression is positive for a closing pair.  The *state
// definitions* (and every consumer: the reluctant refinement criterion of
// section 6.3, Algorithm 11 line 6/9, the critical step below) rely on
// "negative == closing", so the projected relative velocity is evaluated as
// v_B - v_A along d_AB, i.e. the velocity of B relative to A, which is what
// the name v_BA denotes.
ApproachState DtController::stateOf(const Particle& a, const Particle& b, Real& v_ba,
                                    Real& gap) const noexcept {
    const Vec3 d = b.pos - a.pos;
    const Real dist = norm(d);

    if (dist <= kEps) {
        // Coincident centres: no usable direction.  Treat as separated (the
        // narrowphase owns that degenerate case).
        v_ba = 0.0;
        return ApproachState::Separation;
    }

    const Vec3 dhat = d / dist;

    // Signed separation along the centre line measured on the *actual*
    // geometry, not on the bounding spheres: a triangulated body sits well
    // inside its circumscribed sphere, so the sphere gap would report contact
    // (or "critical") while the surfaces are still visibly apart. For spheres
    // this reduces exactly to |d_AB| - (rr_A + rr_B).
    // Separation along the centre line: the nearest point of B minus the
    // furthest point of A in that direction.
    gap = -supportAlong(a, dhat) - supportAlong(b, -dhat);
    v_ba = dot(b.vel - a.vel, dhat);  // < 0: closing

    if (v_ba >= 0.0) return ApproachState::Separation;

    // Already touching or interpenetrating: the contact law owns the pair, and
    // the critical step (which is proportional to the *remaining* gap) would
    // otherwise collapse dt to zero and freeze the simulation.
    if (gap <= 0.0) return ApproachState::NonCritical;

    // Critical approach: extrapolate both bodies over the next *grown* step,
    // pdt = dt + dt*inc, and test whether the geometric gap closes
    // (Algorithm 11 lines 13-17).
    const Real pdt = state_.dt * (1.0 + state_.inc);
    const Vec3 pa = a.pos + a.vel * pdt;
    const Vec3 pb = b.pos + b.vel * pdt;
    const Real pd = norm(pb - pa);
    const Real contact_dist = dist - gap;  // centre distance at which the surfaces touch
    if (pd < contact_dist) return ApproachState::Critical;
    return ApproachState::NonCritical;
}

// ---------------------------------------------------------------------------
// Global step update, Algorithms 11 (scan) and 12 (variance)
// ---------------------------------------------------------------------------
DtDecision DtController::update(const std::vector<Particle>& bodies,
                                const std::vector<ContactPair>& pairs, Real h_min,
                                Real h_max) {
    (void)h_max;  // the thesis bound uses h_min only (7.2)

    DtDecision dec;
    dec.dt_next = state_.dt;
    dec.worst = ApproachState::Separation;

    const Real eps = kEps;
    const Real inc = (state_.inc > 1.0) ? state_.inc : 1.1;

    // Maximum speed over every body (Algorithm 11 line 10: Vmax).
    Real vmax = 0.0;
    for (const Particle& b : bodies) {
        const Real s = norm(b.vel);
        if (std::isfinite(s) && s > vmax) vmax = s;
    }
    dec.v_max = vmax;

    // dt_max = h_min / (2 * inc * Vmax); with no motion keep the incoming cap.
    // The incoming cap carries the explicit-stability limit of the contact
    // spring, so the grid bound may only tighten it, never relax it.
    Real dt_max = state_.dt_max;
    const bool gridOk = std::isfinite(h_min) && h_min > 0.0;
    if (gridOk && vmax > eps) dt_max = std::min(dt_max, h_min / (2.0 * inc * vmax));
    if (!std::isfinite(dt_max) || dt_max <= 0.0) dt_max = std::numeric_limits<Real>::max();

    const int nbody = static_cast<int>(bodies.size());
    Real localMin = std::numeric_limits<Real>::infinity();
    long critical = 0, approaching = 0, separating = 0;

    for (const ContactPair& p : pairs) {
        if (p.i < 0 || p.i >= nbody || p.j < 0 || p.j >= nbody || p.i == p.j) continue;

        Real v_ba = 0.0, gap = 0.0;
        const ApproachState s =
            stateOf(bodies[static_cast<std::size_t>(p.i)],
                    bodies[static_cast<std::size_t>(p.j)], v_ba, gap);

        switch (s) {
            case ApproachState::Critical: {
                ++critical;
                const Real speed = std::abs(v_ba);
                // Thesis: dt_local = (|d_AB| - rr_A - rr_B) / (2 v_BA), i.e.
                // half the remaining surface gap divided by the closing rate.
                // Only a *positive* remaining gap gives a meaningful bound; once
                // the sphere envelopes overlap the penalty contact owns the
                // interaction and shrinking dt would freeze the simulation.
                Real dtl = state_.dt;
                if (speed > eps && gap > 0.0) dtl = 0.5 * gap / speed;
                if (!std::isfinite(dtl) || dtl <= 0.0) dtl = state_.dt;
                localMin = std::min(localMin, dtl);
                if (dec.worst != ApproachState::Critical) dec.worst = ApproachState::Critical;
                break;
            }
            case ApproachState::NonCritical:
                ++approaching;
                if (dec.worst == ApproachState::Separation) dec.worst = ApproachState::NonCritical;
                break;
            case ApproachState::Separation:
                ++separating;
                break;
        }
    }

    dec.critical_pairs = critical;
    dec.approaching_pairs = approaching;
    dec.separating_pairs = separating;
    dec.dt_local = std::isfinite(localMin) ? localMin : 0.0;

    if (state_.dynamic) {
        if (critical > 0) {
            // Algorithm 12: shrink to the tightest critical step, never below
            // dt_min (which would stall the simulation).
            dec.dt_next = std::max(state_.dt_min, localMin);
        } else {
            dec.dt_next = state_.dt * inc;
        }
        if (dec.dt_next > dt_max) dec.dt_next = dt_max;
    } else {
        dec.dt_next = state_.dt;
    }

    // No-tunnelling rule of 7.1 (iii): a body may not move more than half a
    // cell per step.  Kept outside the dynamic branch because a fixed step can
    // be unsafe too.
    if (gridOk && vmax > eps) {
        const Real tunnel = 0.5 * h_min / vmax;
        if (dec.dt_next > tunnel) dec.dt_next = tunnel;
    }

    if (!std::isfinite(dec.dt_next) || dec.dt_next <= 0.0) dec.dt_next = state_.dt_min;

    // Self-advancing controller: the decision becomes the new current step, so
    // the next stateOf() predicts with the dt that will actually be used.
    state_.dt = dec.dt_next;
    return dec;
}

}  // namespace contact
