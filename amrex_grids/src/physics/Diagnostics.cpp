// Simulation diagnostics: energies, momentum, contact statistics.

#include "contact/Physics.hpp"

#include <cmath>

namespace contact {

Diagnostics computeDiagnostics(const std::vector<Particle>& bodies,
                               const std::vector<ContactPoint>& contacts) {
    Diagnostics d;
    const int nbody = static_cast<int>(bodies.size());

    for (const Particle& b : bodies) {
        const Real m = (std::isfinite(b.mass) && b.mass > 0.0) ? b.mass : 0.0;
        const Real I = (std::isfinite(b.inertia) && b.inertia > 0.0) ? b.inertia : 0.0;

        const Real v2 = norm2(b.vel);
        const Real w2 = norm2(b.omega);
        if (std::isfinite(v2)) d.kinetic_linear += 0.5 * m * v2;
        if (std::isfinite(w2)) d.kinetic_angular += 0.5 * I * w2;

        d.momentum += b.vel * m;
    }

    for (const ContactPoint& cp : contacts) {
        if (cp.i < 0 || cp.i >= nbody) continue;
        if (cp.j >= nbody) continue;  // j < 0 is a wall contact, still counted

        ++d.contacts;
        if (std::isfinite(cp.depth)) {
            const Real pen = (cp.depth > 0.0) ? cp.depth : 0.0;
            if (pen > d.max_penetration) d.max_penetration = pen;
        }
    }

    return d;
}

}  // namespace contact
