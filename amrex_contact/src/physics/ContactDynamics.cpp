// Contact response (thesis 2.3) and semi-implicit Euler time integration
// (thesis 5.1) plus the axis-aligned wall response. AMReX-free.

#include "contact/Physics.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace contact {
namespace {

inline bool finiteReal(Real v) noexcept { return std::isfinite(v); }

inline bool finiteVec(const Vec3& v) noexcept {
    return finiteReal(v[0]) && finiteReal(v[1]) && finiteReal(v[2]);
}

// 1/m is authoritative (it is the only way to express an immovable body with
// inv_mass == 0); mass is used as a fallback and wins when the two disagree,
// which catches callers that set `mass` only and left the default inv_mass.
inline Real invMassOf(const Particle& b) noexcept {
    const bool okM = finiteReal(b.mass) && b.mass > 0.0;
    const bool okIm = finiteReal(b.inv_mass) && b.inv_mass >= 0.0;
    if (!okIm) return okM ? 1.0 / b.mass : 0.0;
    if (b.inv_mass == 0.0) return 0.0;  // static body
    if (okM && std::abs(b.inv_mass * b.mass - 1.0) > 1.0e-6) return 1.0 / b.mass;
    return b.inv_mass;
}

inline Real invInertiaOf(const Particle& b) noexcept {
    const bool okI = finiteReal(b.inertia) && b.inertia > 0.0;
    const bool okIi = finiteReal(b.inv_inertia) && b.inv_inertia >= 0.0;
    if (!okIi) return okI ? 1.0 / b.inertia : 0.0;
    if (b.inv_inertia == 0.0) return 0.0;  // rotationally locked
    if (okI && std::abs(b.inv_inertia * b.inertia - 1.0) > 1.0e-6) return 1.0 / b.inertia;
    return b.inv_inertia;
}

}  // namespace

// ---------------------------------------------------------------------------
// Contact forces, thesis 2.3.1 (repulsive) and 2.3.2 (Coulomb friction)
// ---------------------------------------------------------------------------
//
// Convention: ContactPoint::normal is the unit vector from body i towards
// body j, so the repulsive force on i is -fn * n and the force on j is
// +fn * n.  Both are applied at the contact point and split into a centre
// force and a torque tau = r x f with r = contact point - centre of mass.
//
// The tangential force is the vector generalisation of the thesis' scalar
//     ft = clamp(-kt*|vt|*dt - gamma_t*vt, -mu*fn, mu*fn)
// i.e. f_t = -(kt*dt + gamma_t) * vt with the magnitude clamped to mu*fn,
// where vt is the tangential part of the relative velocity of i's contact
// point with respect to j's contact point.
void ContactDynamics::computeForces(std::vector<Particle>& bodies,
                                    const std::vector<ContactPoint>& contacts,
                                    const Material& mat,
                                    ContactModel model,
                                    Real dt) {
    for (Particle& b : bodies) {
        b.force = Vec3{0.0, 0.0, 0.0};
        b.torque = Vec3{0.0, 0.0, 0.0};
    }

    // One entry per contact, in the same order as `contacts` (0 for a
    // contact that could not be applied).
    pair_force_.assign(contacts.size(), 0.0);

    const int nbody = static_cast<int>(bodies.size());
    const Real step = (finiteReal(dt) && dt > 0.0) ? dt : 0.0;
    const Real kt_dt = mat.kt * step;
    const Real mu = (finiteReal(mat.mu) && mat.mu > 0.0) ? mat.mu : 0.0;
    const Real kn = (finiteReal(mat.kn) && mat.kn > 0.0) ? mat.kn : 0.0;
    const Real gamma_n = finiteReal(mat.gamma_n) ? mat.gamma_n : 0.0;
    const Real gamma_t = finiteReal(mat.gamma_t) ? mat.gamma_t : 0.0;

    for (std::size_t c = 0; c < contacts.size(); ++c) {
        const ContactPoint& cp = contacts[c];
        if (cp.i < 0 || cp.i >= nbody) continue;
        if (cp.j < 0 || cp.j >= nbody) continue;
        if (cp.i == cp.j) continue;

        const Vec3 nrm = normalized(cp.normal);
        if (nrm[0] == 0.0 && nrm[1] == 0.0 && nrm[2] == 0.0) continue;  // degenerate
        if (!finiteVec(cp.point) || !finiteReal(cp.depth)) continue;

        const Real depth = (cp.depth > 0.0) ? cp.depth : 0.0;

        Particle& A = bodies[static_cast<std::size_t>(cp.i)];
        Particle& B = bodies[static_cast<std::size_t>(cp.j)];

        const Vec3 rA = cp.point - A.pos;
        const Vec3 rB = cp.point - B.pos;

        // Contact-point velocities (rigid body kinematics).
        const Vec3 vA = A.vel + cross(A.omega, rA);
        const Vec3 vB = B.vel + cross(B.omega, rB);
        const Vec3 vrel = vA - vB;            // i's contact point relative to j's
        const Real vn = dot(vrel, nrm);       // > 0: approaching along n
        const Real sepRate = -vn;             // > 0: separating

        Real fn = 0.0;
        switch (model) {
            case ContactModel::Penalty:
                fn = kn * depth;
                break;
            case ContactModel::SpringDashpot:
                // Hooke spring minus a viscous dashpot on the separation rate,
                // so that fast separation cannot pull the bodies together.
                fn = kn * depth - gamma_n * sepRate;
                break;
        }
        if (!finiteReal(fn)) continue;
        if (fn < 0.0) fn = 0.0;  // no adhesion; under-relaxation never flips sign

        // ---- friction -----------------------------------------------------
        Vec3 ft{0.0, 0.0, 0.0};
        if (mu > 0.0 && fn > 0.0) {
            const Vec3 vt = vrel - nrm * vn;  // tangential slip of i w.r.t. j
            const Real vtMag = norm(vt);
            if (vtMag > 0.0) {
                Vec3 ftRaw = vt * (-(kt_dt + gamma_t));
                const Real maxFt = mu * fn;
                const Real mag = norm(ftRaw);
                if (mag > maxFt) ftRaw = ftRaw * (maxFt / mag);
                if (finiteVec(ftRaw)) ft = ftRaw;
            }
        }

        const Vec3 fA = nrm * (-fn) + ft;
        const Vec3 fB = nrm * fn - ft;

        A.force += fA;
        A.torque += cross(rA, fA);
        B.force += fB;
        B.torque += cross(rB, fB);

        pair_force_[c] = fn;
    }
}

// ---------------------------------------------------------------------------
// Semi-implicit (symplectic) Euler, thesis 5.1
// ---------------------------------------------------------------------------
void ContactDynamics::integrate(std::vector<Particle>& bodies, Real dt) const {
    if (!finiteReal(dt) || dt <= 0.0) return;

    for (Particle& b : bodies) {
        const Real im = invMassOf(b);
        const Real ii = invInertiaOf(b);

        const Vec3 vel = b.vel + b.force * (dt * im);
        const Vec3 omega = b.omega + b.torque * (dt * ii);
        const Vec3 pos = b.pos + vel * dt;  // new velocity: semi-implicit

        if (!finiteVec(vel) || !finiteVec(omega) || !finiteVec(pos)) {
            // Reject the update: dropping the forces lets the next step recover
            // instead of propagating NaNs through the whole simulation.
            b.force = Vec3{0.0, 0.0, 0.0};
            b.torque = Vec3{0.0, 0.0, 0.0};
            continue;
        }

        const Quat dq = rotateRate(b.quat, omega);
        Quat q{b.quat.w + dt * dq.w, b.quat.x + dt * dq.x,
               b.quat.y + dt * dq.y, b.quat.z + dt * dq.z};
        if (!finiteReal(q.w) || !finiteReal(q.x) || !finiteReal(q.y) || !finiteReal(q.z)) {
            q = Quat{};
        }
        q = normalized(q);

        b.vel = vel;
        b.omega = omega;
        b.pos = pos;
        b.quat = q;
    }
}

// ---------------------------------------------------------------------------
// Axis-aligned box walls
// ---------------------------------------------------------------------------
//
// Position and velocity are reflected at the six faces; the normal component
// is scaled by the restitution, the tangential components are damped by 0.9.
// Bodies whose bounding sphere is entirely inside the box are untouched.
void ContactDynamics::applyWalls(std::vector<Particle>& bodies, const Aabb& box,
                                 Real restitution) const {
    // applyWalls has no Material argument; "use the material value" in the
    // header comment therefore degenerates to a documented default of a
    // perfectly reflecting wall (e = 1).  Values outside [0,1] are clamped.
    constexpr Real kDefaultWallRestitution = 1.0;
    constexpr Real kTangentialDamping = 0.9;

    Real e = (finiteReal(restitution) && restitution >= 0.0) ? restitution
                                                            : kDefaultWallRestitution;
    e = clamp(e, 0.0, 1.0);

    for (Particle& b : bodies) {
        if (!finiteVec(b.pos) || !finiteVec(b.vel)) continue;

        Real r = (finiteReal(b.radius) && b.radius > 0.0) ? b.radius : 0.0;

        for (int d = 0; d < 3; ++d) {
            const Real lo = box.lo[d];
            const Real hi = box.hi[d];
            if (!finiteReal(lo) || !finiteReal(hi) || !(hi > lo)) continue;

            // A box thinner than the body: centre it instead of ping-ponging.
            const Real rr = std::min(r, 0.5 * (hi - lo));

            if (b.pos[d] - rr < lo) {
                b.pos[d] = lo + rr;              // floor at the wall
                if (b.vel[d] < 0.0) b.vel[d] = -e * b.vel[d];
                for (int k = 0; k < 3; ++k)
                    if (k != d) b.vel[k] *= kTangentialDamping;
            } else if (b.pos[d] + rr > hi) {
                b.pos[d] = hi - rr;
                if (b.vel[d] > 0.0) b.vel[d] = -e * b.vel[d];
                for (int k = 0; k < 3; ++k)
                    if (k != d) b.vel[k] *= kTangentialDamping;
            }
        }
    }
}

}  // namespace contact
