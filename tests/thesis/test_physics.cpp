// Standalone (no framework) unit tests for the contact response, integrator,
// dynamic time-step controller and diagnostics.
//
//   g++ -std=c++17 -O2 -Iinclude src/physics/*.cpp tests/test_physics.cpp
//       -o /tmp/test_physics && /tmp/test_physics
//
// Exits non-zero if any assertion fails.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "contact/Physics.hpp"

using namespace contact;

namespace {

int g_checks = 0;
int g_failures = 0;

void report(bool ok, const std::string& what, const std::string& detail = {}) {
    ++g_checks;
    if (!ok) ++g_failures;
    std::printf("  [%s] %s%s%s\n", ok ? " ok " : "FAIL", what.c_str(),
                detail.empty() ? "" : "  ", detail.c_str());
}

void checkNear(const std::string& what, Real got, Real want, Real tol) {
    const Real err = std::abs(got - want);
    const Real scale = std::max(std::abs(want), 1.0);
    char buf[256];
    std::snprintf(buf, sizeof buf, "got=%.17g want=%.17g |err|=%.3g tol=%.3g", got, want,
                  err, tol);
    report(err <= tol * scale, what, buf);
}

void checkTrue(const std::string& what, bool ok, const std::string& detail = {}) {
    report(ok, what, detail);
}

std::string fmt(const char* f, double a, double b = 0.0, double c = 0.0) {
    char buf[256];
    std::snprintf(buf, sizeof buf, f, a, b, c);
    return std::string(buf);
}

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

Particle makeSphere(int id, Vec3 pos, Vec3 vel, Real radius, Real mass) {
    Particle p;
    p.id = id;
    p.pos = pos;
    p.vel = vel;
    p.radius = radius;
    p.halo = 1.1 * radius;
    p.mass = mass;
    p.inv_mass = 1.0 / mass;
    p.inertia = 0.4 * mass * radius * radius;  // solid sphere
    p.inv_inertia = 1.0 / p.inertia;
    p.quat = Quat{};
    return p;
}

// Sphere-sphere narrowphase stand-in: one contact when the surfaces touch.
bool sphereContact(const Particle& a, const Particle& b, ContactPoint& cp) {
    const Vec3 d = b.pos - a.pos;
    const Real dist = norm(d);
    const Real gap = dist - (a.radius + b.radius);
    if (dist <= kEps || gap > 0.0) return false;
    const Vec3 n = d / dist;
    cp.i = a.id;
    cp.j = b.id;
    cp.normal = n;
    cp.depth = -gap;
    cp.dist = dist;
    cp.point = a.pos + n * (a.radius + 0.5 * gap);
    return true;
}

Material penaltyMat(Real kn, Real mu = 0.0) {
    Material m;
    m.kn = kn;
    m.kt = 0.0;
    m.gamma_n = 0.0;
    m.gamma_t = 0.0;
    m.mu = mu;
    return m;
}

// ---------------------------------------------------------------------------
// 1 + 2. Momentum conservation and energy behaviour of a head-on collision
// ---------------------------------------------------------------------------
void testMomentumAndEnergy() {
    std::printf("\n[1/2] head-on collision of two equal spheres (penalty)\n");

    const Real r = 0.1, m = 1.0, dt = 1e-4;
    const Real kn = 2.0e4;
    // Slightly off-axis so the contact normal is not a coordinate axis, and a
    // common +z drift so the total momentum is non-zero (|p| = 2*m*0.05).
    std::vector<Particle> bodies;
    bodies.push_back(
        makeSphere(0, makeVec3(-0.1001, 0.4999, 0.5), makeVec3(0.1, 0.02, 0.05), r, m));
    bodies.push_back(
        makeSphere(1, makeVec3(0.1001, 0.5001, 0.5), makeVec3(-0.1, -0.02, 0.05), r, m));

    const Material mat = penaltyMat(kn, 0.0);
    ContactDynamics dyn;

    const Real ke0 = computeDiagnostics(bodies, {}).kinetic_linear;
    Vec3 p0{0, 0, 0};
    for (const Particle& b : bodies) p0 += b.vel * b.mass;

    Real pWorst = 0.0;
    Real scaleMax = 0.0;
    Real keMax = 0.0;
    Real maxDepth = 0.0;
    int contactSteps = 0;

    for (int step = 0; step < 200; ++step) {
        std::vector<ContactPoint> contacts;
        ContactPoint cp;
        if (sphereContact(bodies[0], bodies[1], cp)) {
            contacts.push_back(cp);
            ++contactSteps;
            maxDepth = std::max(maxDepth, cp.depth);
        }

        dyn.computeForces(bodies, contacts, mat, ContactModel::Penalty, dt);
        dyn.integrate(bodies, dt);

        Vec3 p{0, 0, 0};
        Real scale = 0.0;
        Real ke = 0.0;
        for (const Particle& b : bodies) {
            p += b.vel * b.mass;
            scale += b.mass * norm(b.vel);
            ke += 0.5 * b.mass * norm2(b.vel);
        }
        pWorst = std::max(pWorst, norm(p - p0));
        scaleMax = std::max(scaleMax, scale);
        keMax = std::max(keMax, ke);
    }

    const Real pRel = pWorst / std::max(scaleMax, 1e-30);
    std::printf("  momentum |dp|max = %.3e, velocity scale = %.3e, relative = %.3e\n",
                pWorst, scaleMax, pRel);
    checkTrue("total linear momentum conserved (|dp|/|mv| <= 1e-10)",
              pRel <= 1e-10, fmt("relative=%.3e", pRel));
    checkNear("absolute momentum drift", pWorst, 0.0, 1e-10);

    const Real keGain = (keMax - ke0) / ke0 * 100.0;
    std::printf("  KE0 = %.12g, KEmax = %.12g, gain = %.6f %% (contacts on %d steps, "
                "max depth = %.3e)\n",
                ke0, keMax, keGain, contactSteps, maxDepth);
    checkTrue("kinetic energy does not increase by more than 1%", keGain <= 1.0,
              fmt("gain=%.6f%%", keGain));

    const Real keEnd = computeDiagnostics(bodies, {}).kinetic_linear;
    std::printf("  KE_end = %.12g (%.4f %% of KE0), v0 = %.6f, v1 = %.6f\n", keEnd,
                keEnd / ke0 * 100.0, bodies[0].vel[0], bodies[1].vel[0]);
    checkTrue("collision completes and separates", bodies[0].vel[0] < 0.0 && bodies[1].vel[0] > 0.0);
}

// ---------------------------------------------------------------------------
// 3. Resting body on the floor: depth ~ m*g/kn, bounded over time
// ---------------------------------------------------------------------------
void testRestingBody() {
    std::printf("\n[3] resting body on the floor (penalty)\n");

    const Real m = 1.0, g = 9.81, kn = 1.0e5, r = 0.1, dt = 1e-4;
    const int steps = 5000;

    std::vector<Particle> bodies;
    bodies.push_back(makeSphere(0, makeVec3(0.5, r, 0.5), makeVec3(0, 0, 0), r, m));

    // Static floor proxy: valid index j so computeForces applies the contact
    // pair properly, inv_mass = inv_inertia = 0 so it cannot move.
    Particle floor;
    floor.id = 1;
    floor.pos = makeVec3(0.5, -0.5, 0.5);
    floor.radius = r;
    floor.halo = 1.1 * r;
    floor.mass = 0.0;
    floor.inv_mass = 0.0;
    floor.inertia = 0.0;
    floor.inv_inertia = 0.0;
    bodies.push_back(floor);

    const Material mat = penaltyMat(kn, 0.3);
    ContactDynamics dyn;

    const Real depthExact = m * g / kn;

    std::vector<Real> depths;
    depths.reserve(steps);
    Real depthMax = 0.0;

    for (int step = 0; step < steps; ++step) {
        std::vector<ContactPoint> contacts;
        const Real pen = r - bodies[0].pos[1];
        if (pen >= 0.0) {
            ContactPoint cp;
            cp.i = 0;
            cp.j = 1;
            cp.normal = makeVec3(0.0, -1.0, 0.0);  // from body 0 towards the floor
            cp.depth = pen;
            cp.dist = pen;
            cp.point = makeVec3(bodies[0].pos[0], 0.0, bodies[0].pos[2]);
            contacts.push_back(cp);
        }

        dyn.computeForces(bodies, contacts, mat, ContactModel::Penalty, dt);
        bodies[0].force[1] -= m * g;  // gravity
        dyn.integrate(bodies, dt);

        const Real d = std::max(0.0, r - bodies[0].pos[1]);
        depths.push_back(d);
        depthMax = std::max(depthMax, d);
    }

    const int w = 1000;
    Real meanFirst = 0.0, meanLast = 0.0, maxFirst = 0.0, maxLast = 0.0;
    for (int i = 0; i < w; ++i) {
        meanFirst += depths[i];
        maxFirst = std::max(maxFirst, depths[i]);
        meanLast += depths[steps - w + i];
        maxLast = std::max(maxLast, depths[steps - w + i]);
    }
    meanFirst /= w;
    meanLast /= w;

    const Real relMean = std::abs(meanLast - depthExact) / depthExact;
    std::printf("  analytic depth m*g/kn = %.6e\n", depthExact);
    std::printf("  mean depth (last %d steps) = %.6e  (%.2f %% from analytic)\n", w, meanLast,
                relMean * 100.0);
    std::printf("  mean depth (first %d steps) = %.6e, max = %.6e / %.6e, global max = %.6e\n",
                w, meanFirst, maxFirst, maxLast, depthMax);

    checkTrue("penetration within 20% of m*g/kn", relMean <= 0.20,
              fmt("rel err=%.3f%%", relMean * 100.0));
    checkTrue("penetration does not grow", maxLast <= maxFirst * 1.01 + 1e-12,
              fmt("maxLast=%.6e maxFirst=%.6e", maxLast, maxFirst));
    checkTrue("body stays at the floor (never floats away)", depths[steps - 1] >= 0.0 &&
                  bodies[0].pos[1] <= r + 1e-9,
              fmt("y=%.9f", bodies[0].pos[1]));
}

// ---------------------------------------------------------------------------
// 4. dt increments: separation vs critical approach
// ---------------------------------------------------------------------------
void testDtIncrementAndCritical() {
    std::printf("\n[4] dynamic dt: separation and critical approach\n");

    DtState st;
    st.dt = 1.0e-3;
    st.dt_min = 1.0e-4;
    st.dt_max = 1.0e-2;
    st.inc = 1.1;
    st.dynamic = true;

    const Real hmin = 0.5, hmax = 1.0;
    const Real r = 0.1;

    // --- all bodies separating -> dt * inc (dt_max not binding) ------------
    {
        std::vector<Particle> bodies;
        bodies.push_back(makeSphere(0, makeVec3(-0.1, 0.5, 0.5), makeVec3(-0.2, 0, 0), r, 1.0));
        bodies.push_back(makeSphere(1, makeVec3(0.1, 0.5, 0.5), makeVec3(0.2, 0, 0), r, 1.0));
        std::vector<ContactPair> pairs{{0, 1}};

        DtController ctrl(st);
        const DtDecision d = ctrl.update(bodies, pairs, hmin, hmax);

        Real v_ba = 0, gap = 0;
        const ApproachState s0 = ctrl.stateOf(bodies[0], bodies[1], v_ba, gap);
        std::printf("  separation: v_ba = %+.6g (>= 0 => separation), state = %d, worst = %d\n",
                    v_ba, static_cast<int>(s0), static_cast<int>(d.worst));

        const Real vmax = 0.2;
        const Real dtmax = hmin / (2.0 * st.inc * vmax);
        const Real expected = std::min(st.dt * st.inc, dtmax);
        std::printf("  dt = %.12g -> dt_next = %.12g, expected min(dt*1.1, dt_max=%.12g) = %.12g\n",
                    st.dt, d.dt_next, dtmax, expected);
        checkTrue("all-separating: dt_next == min(dt*inc, dt_max) exactly", d.dt_next == expected,
                  fmt("delta=%.3e", d.dt_next - expected));
        checkNear("separating pair counter", static_cast<Real>(d.separating_pairs), 1.0, 0.0);
        checkNear("critical pair counter", static_cast<Real>(d.critical_pairs), 0.0, 0.0);
        checkTrue("worst state is Separation", d.worst == ApproachState::Separation);
    }

    // --- one critical approach -> analytic 0.5*gap/|v_ba| ------------------
    {
        const Real gap = 4.0e-4;
        const Real half = (2.0 * r + gap) * 0.5;
        std::vector<Particle> bodies;
        bodies.push_back(makeSphere(0, makeVec3(0.5 - half, 0.5, 0.5), makeVec3(0.2, 0, 0), r, 1.0));
        bodies.push_back(makeSphere(1, makeVec3(0.5 + half, 0.5, 0.5), makeVec3(-0.2, 0, 0), r, 1.0));
        std::vector<ContactPair> pairs{{0, 1}};

        DtController ctrl(st);
        Real v_ba = 0, g2 = 0;
        const ApproachState s = ctrl.stateOf(bodies[0], bodies[1], v_ba, g2);

        const Real analytic = 0.5 * gap / std::abs(v_ba);
        const DtDecision d = ctrl.update(bodies, pairs, hmin, hmax);

        std::printf("  critical: gap = %.6e, v_ba = %+.6g, state = %d (2 = Critical)\n", g2, v_ba,
                    static_cast<int>(s));
        std::printf("  analytic 0.5*gap/|v_ba| = %.12g, dt_local = %.12g, dt_next = %.12g "
                    "(dt = %.12g)\n",
                    analytic, d.dt_local, d.dt_next, st.dt);

        checkTrue("stateOf classifies the closing pair as Critical",
                  s == ApproachState::Critical);
        checkNear("dt_local == 0.5*gap/|v_ba|", d.dt_local, analytic, 1e-9);
        checkNear("dt_next == 0.5*gap/|v_ba|", d.dt_next, analytic, 1e-9);
        checkTrue("critical dt is smaller than the incoming dt", d.dt_next < st.dt,
                  fmt("dt_next=%.6e dt=%.6e", d.dt_next, st.dt));
        checkTrue("worst state is Critical", d.worst == ApproachState::Critical);
        checkNear("critical pair counter", static_cast<Real>(d.critical_pairs), 1.0, 0.0);
        checkTrue("dt_next respects dt_min", d.dt_next >= st.dt_min);
    }

    // --- beyond the critical gap the pair is only non-critical -------------
    {
        const Real gap = 0.05;
        const Real half = (2.0 * r + gap) * 0.5;
        std::vector<Particle> bodies;
        bodies.push_back(makeSphere(0, makeVec3(0.5 - half, 0.5, 0.5), makeVec3(0.2, 0, 0), r, 1.0));
        bodies.push_back(makeSphere(1, makeVec3(0.5 + half, 0.5, 0.5), makeVec3(-0.2, 0, 0), r, 1.0));
        std::vector<ContactPair> pairs{{0, 1}};

        DtController ctrl(st);
        Real v_ba = 0, g2 = 0;
        const ApproachState s = ctrl.stateOf(bodies[0], bodies[1], v_ba, g2);
        const DtDecision d = ctrl.update(bodies, pairs, hmin, hmax);
        std::printf("  far approach: gap = %.6g -> state = %d (1 = NonCritical), dt_next = %.6g\n",
                    g2, static_cast<int>(s), d.dt_next);
        checkTrue("approaching but far => NonCritical", s == ApproachState::NonCritical);
        checkNear("non-critical pair counter", static_cast<Real>(d.approaching_pairs), 1.0, 0.0);
        checkTrue("non-critical does not shrink dt (only dt_max may cap)",
                  d.dt_next == std::min(st.dt * st.inc, hmin / (2.0 * st.inc * 0.2)));
    }
}

// ---------------------------------------------------------------------------
// 5. No-tunnelling invariant
// ---------------------------------------------------------------------------
void testNoTunnelling() {
    std::printf("\n[5] no-tunnelling invariant v_max*dt <= 0.5*h_min\n");

    DtState st;
    st.dt = 1.0e-2;
    st.dt_min = 1.0e-4;
    st.dt_max = 1.0e-2;
    st.inc = 1.1;

    // (a) fixed (non-dynamic) step: only the no-tunnelling clamp may act.
    {
        st.dynamic = false;
        std::vector<Particle> bodies;
        bodies.push_back(makeSphere(0, makeVec3(0.2, 0.5, 0.5), makeVec3(10, 0, 0), 0.01, 1.0));
        DtController ctrl(st);
        const DtDecision d = ctrl.update(bodies, {}, 0.1, 1.0);
        std::printf("  fixed dt: dt=%.6g v_max=%.6g h_min=%.6g -> dt_next=%.6g (bound %.6g)\n",
                    st.dt, d.v_max, 0.1, d.dt_next, 0.5 * 0.1 / d.v_max);
        checkNear("fixed dt is clamped by 0.5*h_min/v_max", d.dt_next, 5.0e-3, 1e-15);
        checkTrue("fixed dt: v_max*dt_next <= 0.5*h_min", d.v_max * d.dt_next <= 0.5 * 0.1);
    }

    // (b) dynamic step where dt_max alone would still be too large.
    {
        st.dynamic = true;
        std::vector<Particle> bodies;
        bodies.push_back(makeSphere(0, makeVec3(0.2, 0.5, 0.5), makeVec3(100, 0, 0), 0.01, 1.0));
        DtController ctrl(st);
        const DtDecision d = ctrl.update(bodies, {}, 0.1, 1.0);
        std::printf("  dynamic: dt=%.6g v_max=%.6g h_min=0.1 -> dt_next=%.6g, product=%.6g\n",
                    st.dt, d.v_max, d.dt_next, d.v_max * d.dt_next);
        checkTrue("dynamic: dt_next <= dt_max", d.dt_next <= 0.1 / (2.0 * st.inc * d.v_max) * (1 + 1e-15));
        checkTrue("dynamic: v_max*dt_next <= 0.5*h_min", d.v_max * d.dt_next <= 0.5 * 0.1);
    }

    // (c) sweep: the invariant holds for every (v, h_min) and for both modes.
    {
        unsigned long long s = 12345;
        auto next = [&s]() {
            s = s * 6364136223846793005ULL + 1442695040888963407ULL;
            return static_cast<double>((s >> 11) & 0xFFFFFF) / 16777216.0;
        };
        Real worst = -1.0;
        bool ok = true;
        for (int k = 0; k < 500; ++k) {
            const Real v = 1e-3 + next() * 50.0;
            const Real hmin = 1e-3 + next() * 0.2;
            DtState s2 = st;
            s2.dt = 1e-4 + next() * 5e-3;
            s2.dynamic = (k % 2 == 0);
            std::vector<Particle> bodies;
            bodies.push_back(makeSphere(0, makeVec3(0.5, 0.5, 0.5), makeVec3(v, 0, 0), 0.001, 1.0));
            DtController ctrl(s2);
            const DtDecision d = ctrl.update(bodies, {}, hmin, 1.0);
            const Real ratio = (d.v_max * d.dt_next) / (0.5 * hmin);
            worst = std::max(worst, ratio);
            if (!(ratio <= 1.0 + 1e-12)) ok = false;
        }
        std::printf("  sweep of 500 (v, h_min, mode) cases: worst (v_max*dt)/(0.5*h_min) = %.17g\n",
                    worst);
        checkTrue("no-tunnelling holds over the whole sweep", ok);
    }
}

// ---------------------------------------------------------------------------
// 6. Torque from an off-centre contact
// ---------------------------------------------------------------------------
void testTorque() {
    std::printf("\n[6] off-centre contact generates torque with the correct sign\n");

    const Real r = 0.1, m = 1.0, dt = 1e-4;
    std::vector<Particle> bodies;
    bodies.push_back(makeSphere(0, makeVec3(0.0, r, 0.0), makeVec3(1.0, 0.0, 0.0), r, m));  // slides +x

    Particle floor;
    floor.id = 1;
    floor.pos = makeVec3(0.0, -0.5, 0.0);
    floor.radius = r;
    floor.halo = 1.1 * r;
    floor.mass = 0.0;
    floor.inv_mass = 0.0;
    floor.inertia = 0.0;
    floor.inv_inertia = 0.0;
    bodies.push_back(floor);

    Material mat;
    mat.kn = 1.0e5;
    mat.kt = 2.0e5;
    mat.gamma_n = 0.0;
    mat.gamma_t = 1.0;
    mat.mu = 0.5;

    ContactPoint cp;
    cp.i = 0;
    cp.j = 1;
    cp.normal = makeVec3(0.0, -1.0, 0.0);
    cp.depth = 1.0e-3;
    cp.point = makeVec3(0.0, 0.0, 0.0);

    const Vec3 omega0 = bodies[0].omega;
    ContactDynamics dyn;
    dyn.computeForces(bodies, {cp}, mat, ContactModel::Penalty, dt);

    const Real fn = mat.kn * cp.depth;      // 100
    const Real ft = mat.kt * dt + mat.gamma_t;  // 21 (below the Coulomb cap of 50)

    std::printf("  fn = %.6g, friction magnitude = %.6g (cap mu*fn = %.6g)\n", fn, ft,
                mat.mu * fn);
    std::printf("  force = (%.6g, %.6g, %.6g), torque = (%.6g, %.6g, %.6g)\n", bodies[0].force[0],
                bodies[0].force[1], bodies[0].force[2], bodies[0].torque[0], bodies[0].torque[1],
                bodies[0].torque[2]);

    checkNear("normal force on the body is +y", bodies[0].force[1], fn, 1e-12);
    checkNear("tangential force opposes sliding (-x)", bodies[0].force[0], -ft, 1e-12);
    checkNear("torque = r x f about -z", bodies[0].torque[2], -r * ft, 1e-12);
    checkNear("torque has no other components", bodies[0].torque[0], 0.0, 0.0);
    checkNear("reaction force on the floor is equal and opposite", bodies[1].force[1], -fn, 1e-12);

    dyn.integrate(bodies, dt);
    const Real expectedOmega = -dt * (r * ft) * bodies[0].inv_inertia;
    std::printf("  omega before = %.6g, after = (%.6g, %.6g, %.6g), expected dz = %.6g\n",
                omega0[2], bodies[0].omega[0], bodies[0].omega[1], bodies[0].omega[2],
                expectedOmega);
    checkTrue("omega_z becomes negative (rolling sense for +x sliding)", bodies[0].omega[2] < 0.0);
    checkNear("omega_z = dt*tau_z/I", bodies[0].omega[2], expectedOmega, 1e-12);
    checkTrue("static floor does not move", bodies[1].vel[0] == 0.0 && bodies[1].omega[2] == 0.0);

    // A purely normal contact through the centre must not spin the body.
    std::vector<Particle> rest;
    rest.push_back(makeSphere(0, makeVec3(0.0, r, 0.0), makeVec3(0, 0, 0), r, m));
    rest.push_back(floor);
    ContactDynamics dyn2;
    dyn2.computeForces(rest, {cp}, mat, ContactModel::Penalty, dt);
    dyn2.integrate(rest, dt);
    std::printf("  head-on normal contact: torque = (%.3e, %.3e, %.3e), omega_z = %.3e\n",
                rest[0].torque[0], rest[0].torque[1], rest[0].torque[2], rest[0].omega[2]);
    checkNear("central normal contact produces zero torque", rest[0].torque[2], 0.0, 0.0);
    checkTrue("central normal contact keeps omega zero", rest[0].omega[2] == 0.0);

    // Coulomb cap: a very fast slip saturates at mu*fn.
    std::vector<Particle> fast;
    fast.push_back(makeSphere(0, makeVec3(0.0, r, 0.0), makeVec3(1000.0, 0.0, 0.0), r, m));
    fast.push_back(floor);
    ContactDynamics dyn3;
    dyn3.computeForces(fast, {cp}, mat, ContactModel::Penalty, dt);
    std::printf("  fast slip 1000 m/s: |f_t| = %.6g (cap = %.6g)\n",
                std::abs(fast[0].force[0]), mat.mu * fn);
    checkNear("friction is Coulomb-capped at mu*fn", std::abs(fast[0].force[0]), mat.mu * fn,
              1e-12);
}

// ---------------------------------------------------------------------------
// 7. Diagnostics with hand-computable energies
// ---------------------------------------------------------------------------
void testDiagnostics() {
    std::printf("\n[7] diagnostics on a hand-built system\n");

    std::vector<Particle> bodies(2);
    bodies[0].id = 0;
    bodies[0].mass = 2.0;
    bodies[0].inv_mass = 0.5;
    bodies[0].vel = makeVec3(1.0, 0.0, 0.0);
    bodies[0].omega = makeVec3(0.0, 0.0, 3.0);
    bodies[0].inertia = 0.5;
    bodies[0].inv_inertia = 2.0;

    bodies[1].id = 1;
    bodies[1].mass = 1.0;
    bodies[1].inv_mass = 1.0;
    bodies[1].vel = makeVec3(0.0, 2.0, 0.0);
    bodies[1].inertia = 0.1;
    bodies[1].inv_inertia = 10.0;

    std::vector<ContactPoint> contacts(3);
    for (int k = 0; k < 3; ++k) {
        contacts[k].i = 0;
        contacts[k].j = 1;
        contacts[k].depth = 0.0;
    }
    contacts[0].depth = 0.01;
    contacts[1].depth = 0.02;
    contacts[2].depth = 0.005;

    const Diagnostics d = computeDiagnostics(bodies, contacts);
    const Real keLin = 0.5 * 2.0 * 1.0 + 0.5 * 1.0 * 4.0;      // 3
    const Real keAng = 0.5 * 0.5 * 9.0;                        // 2.25

    std::printf("  KE_lin = %.12g (expect %.12g), KE_ang = %.12g (expect %.12g)\n",
                d.kinetic_linear, keLin, d.kinetic_angular, keAng);
    std::printf("  momentum = (%.6g, %.6g, %.6g), max_pen = %.6g, contacts = %ld\n",
                d.momentum[0], d.momentum[1], d.momentum[2], d.max_penetration, d.contacts);

    checkNear("translational KE", d.kinetic_linear, keLin, 1e-14);
    checkNear("rotational KE", d.kinetic_angular, keAng, 1e-14);
    checkNear("momentum x", d.momentum[0], 2.0, 0.0);
    checkNear("momentum y", d.momentum[1], 2.0, 0.0);
    checkNear("momentum z", d.momentum[2], 0.0, 0.0);
    checkNear("max penetration", d.max_penetration, 0.02, 0.0);
    checkNear("contact count", static_cast<Real>(d.contacts), 3.0, 0.0);
}

// ---------------------------------------------------------------------------
// 8. Bookkeeping: force zeroing, SpringDashpot, invalid indices, pair force
// ---------------------------------------------------------------------------
void testBookkeepingAndSpringDashpot() {
    std::printf("\n[8] force bookkeeping, SpringDashpot clamp, pair force order\n");

    const Real r = 0.1;
    std::vector<Particle> bodies;
    bodies.push_back(makeSphere(0, makeVec3(0.0, 0.0, 0.0), makeVec3(0, 0, 0), r, 1.0));
    bodies.push_back(makeSphere(1, makeVec3(0.19, 0.0, 0.0), makeVec3(0, 0, 0), r, 1.0));
    bodies[0].force = makeVec3(7.0, -3.0, 1.0);   // stale junk must be cleared
    bodies[0].torque = makeVec3(1.0, 1.0, 1.0);

    ContactPoint cp;
    cp.i = 0;
    cp.j = 1;
    cp.normal = makeVec3(1.0, 0.0, 0.0);
    cp.depth = 0.01;
    cp.point = makeVec3(0.095, 0.0, 0.0);

    ContactPoint bad;
    bad.i = 0;
    bad.j = 99;  // out of range
    bad.normal = makeVec3(1.0, 0.0, 0.0);
    bad.depth = 0.5;

    Material mat;
    mat.kn = 1000.0;
    mat.kt = 0.0;
    mat.gamma_n = 10.0;
    mat.gamma_t = 0.0;
    mat.mu = 0.0;

    ContactDynamics dyn;

    // Touching, closing at 1 m/s along +x: fn = kn*d - gamma_n*(-1) = 20.
    bodies[1].vel = makeVec3(-1.0, 0.0, 0.0);
    dyn.computeForces(bodies, {cp, bad}, mat, ContactModel::SpringDashpot, 1e-4);
    std::printf("  closing: force0 = (%.6g, %.6g, %.6g), pair_force = {%.6g, %.6g}\n",
                bodies[0].force[0], bodies[0].force[1], bodies[0].force[2],
                dyn.lastPairForce()[0], dyn.lastPairForce()[1]);
    checkNear("stale force is cleared before accumulation", bodies[0].force[0], -20.0, 1e-12);
    checkNear("spring-dashpot fn = kn*d + gamma_n*closing", dyn.lastPairForce()[0], 20.0, 1e-12);
    checkNear("spring-dashpot reaction on j", bodies[1].force[0], 20.0, 1e-12);
    checkTrue("pair_force_ has one entry per contact", dyn.lastPairForce().size() == 2);
    checkNear("invalid contact keeps its slot at 0", dyn.lastPairForce()[1], 0.0, 0.0);

    // Separating at 1 m/s: fn = 10 - 10 = 0, clamped (no adhesion).
    bodies[1].vel = makeVec3(1.0, 0.0, 0.0);
    bodies[0].force = Vec3{0, 0, 0};
    bodies[1].force = Vec3{0, 0, 0};
    dyn.computeForces(bodies, {cp}, mat, ContactModel::SpringDashpot, 1e-4);
    std::printf("  separating: fn = %.6g (clamped at 0)\n", dyn.lastPairForce()[0]);
    checkNear("dashpot clamp keeps fn non-negative", dyn.lastPairForce()[0], 0.0, 0.0);
    checkNear("no tensile force on the bodies", bodies[0].force[0], 0.0, 0.0);

    // Penalty model ignores gamma_n: fn = kn*depth.
    bodies[1].vel = Vec3{0, 0, 0};
    dyn.computeForces(bodies, {cp}, mat, ContactModel::Penalty, 1e-4);
    checkNear("penalty fn = kn*depth", dyn.lastPairForce()[0], 10.0, 1e-12);

    // Model names round-trip.
    ContactModel out = ContactModel::Penalty;
    checkTrue("contactModelFromString(\"spring-dashpot\")",
              contactModelFromString("spring-dashpot", out) && out == ContactModel::SpringDashpot);
    checkTrue("contactModelFromString(\"Penalty\")",
              contactModelFromString("Penalty", out) && out == ContactModel::Penalty);
    checkTrue("contactModelFromString rejects garbage", !contactModelFromString("herz", out));
    checkTrue("toString round-trip", std::string(toString(ContactModel::SpringDashpot)) ==
                                         "SpringDashpot");

    // Many contacts on one body must accumulate, not overwrite.
    std::vector<Particle> multi;
    multi.push_back(makeSphere(0, makeVec3(0.0, 0.0, 0.0), makeVec3(0, 0, 0), r, 1.0));
    multi.push_back(makeSphere(1, makeVec3(0.19, 0.0, 0.0), makeVec3(0, 0, 0), r, 1.0));
    multi.push_back(makeSphere(2, makeVec3(0.0, 0.19, 0.0), makeVec3(0, 0, 0), r, 1.0));
    ContactPoint cx = cp;
    cx.i = 0;
    cx.j = 1;
    cx.normal = makeVec3(1.0, 0.0, 0.0);
    cx.point = makeVec3(0.095, 0.0, 0.0);
    ContactPoint cy = cp;
    cy.i = 0;
    cy.j = 2;
    cy.normal = makeVec3(0.0, 1.0, 0.0);
    cy.point = makeVec3(0.0, 0.095, 0.0);
    ContactDynamics dyn4;
    dyn4.computeForces(multi, {cx, cy}, mat, ContactModel::Penalty, 1e-4);
    std::printf("  two contacts on body 0: force = (%.6g, %.6g, %.6g), pair_force = {%.6g, %.6g}\n",
                multi[0].force[0], multi[0].force[1], multi[0].force[2],
                dyn4.lastPairForce()[0], dyn4.lastPairForce()[1]);
    checkNear("multi-contact accumulation x", multi[0].force[0], -10.0, 1e-12);
    checkNear("multi-contact accumulation y", multi[0].force[1], -10.0, 1e-12);
    checkTrue("pair_force_ order matches contacts",
              dyn4.lastPairForce().size() == 2 && dyn4.lastPairForce()[0] == 10.0 &&
                  dyn4.lastPairForce()[1] == 10.0);
}

// ---------------------------------------------------------------------------
// 9. Box walls
// ---------------------------------------------------------------------------
void testWalls() {
    std::printf("\n[9] axis-aligned box walls\n");

    Aabb box;
    box.lo = makeVec3(0.0, 0.0, 0.0);
    box.hi = makeVec3(1.0, 1.0, 1.0);

    const Real r = 0.1;
    std::vector<Particle> bodies;
    bodies.push_back(makeSphere(0, makeVec3(0.05, 0.5, 0.5), makeVec3(-1.0, 0.5, 0.0), r, 1.0));
    bodies.push_back(makeSphere(1, makeVec3(0.98, 0.5, 0.5), makeVec3(2.0, -0.4, 0.0), r, 1.0));
    bodies.push_back(makeSphere(2, makeVec3(0.5, 0.5, 0.5), makeVec3(0.3, 0.0, -0.2), r, 1.0));

    ContactDynamics dyn;
    dyn.applyWalls(bodies, box, 0.5);

    std::printf("  body0 pos.x = %.6f vel = (%.6f, %.6f, %.6f)\n", bodies[0].pos[0],
                bodies[0].vel[0], bodies[0].vel[1], bodies[0].vel[2]);
    std::printf("  body1 pos.x = %.6f vel = (%.6f, %.6f, %.6f)\n", bodies[1].pos[0],
                bodies[1].vel[0], bodies[1].vel[1], bodies[1].vel[2]);
    std::printf("  body2 pos = (%.6f, %.6f, %.6f) vel = (%.6f, %.6f, %.6f)\n", bodies[2].pos[0],
                bodies[2].pos[1], bodies[2].pos[2], bodies[2].vel[0], bodies[2].vel[1],
                bodies[2].vel[2]);

    checkNear("lo wall: position floored at lo + radius", bodies[0].pos[0], 0.1, 1e-15);
    checkNear("lo wall: normal velocity reflected with restitution",
              bodies[0].vel[0], 0.5, 1e-15);
    checkNear("lo wall: tangential velocity damped by 0.9", bodies[0].vel[1], 0.45, 1e-15);
    checkNear("lo wall: zero tangential stays zero", bodies[0].vel[2], 0.0, 0.0);

    checkTrue("hi wall: position floored at hi - radius", bodies[1].pos[0] <= 0.9 + 1e-15);
    checkNear("hi wall: normal velocity reflected with restitution", bodies[1].vel[0], -1.0, 1e-15);
    checkNear("hi wall: tangential velocity damped by 0.9", bodies[1].vel[1], -0.36, 1e-15);

    checkNear("interior body position untouched", bodies[2].pos[1], 0.5, 0.0);
    checkNear("interior body velocity untouched", bodies[2].vel[0], 0.3, 0.0);

    // Negative restitution means "unspecified": documented default is e = 1.
    std::vector<Particle> elastic;
    elastic.push_back(makeSphere(0, makeVec3(0.05, 0.5, 0.5), makeVec3(-1.0, 0.0, 0.0), r, 1.0));
    dyn.applyWalls(elastic, box, -1.0);
    checkNear("restitution < 0 uses the default e = 1", elastic[0].vel[0], 1.0, 1e-15);

    // A body that is exactly touching (no penetration) is left alone: the wall
    // test is strict, so the sphere is not squeezed.  Once it penetrates the
    // position is floored and the normal velocity is killed for e = 0.
    std::vector<Particle> resting;
    resting.push_back(makeSphere(0, makeVec3(0.5, 0.099, 0.5), makeVec3(0.2, -0.5, 0.0), r, 1.0));
    dyn.applyWalls(resting, box, 0.0);
    checkNear("penetrating body is floored at y = radius", resting[0].pos[1], 0.1, 1e-15);
    checkNear("restitution 0 kills the normal velocity", resting[0].vel[1], 0.0, 0.0);
    checkNear("tangential velocity damped on the wall", resting[0].vel[0], 0.18, 1e-15);

    std::vector<Particle> touching;
    touching.push_back(makeSphere(0, makeVec3(0.5, 0.1, 0.5), makeVec3(0.2, 0.0, 0.0), r, 1.0));
    dyn.applyWalls(touching, box, 0.0);
    checkNear("exactly touching body keeps its position", touching[0].pos[1], 0.1, 0.0);
    checkNear("exactly touching body keeps its tangential velocity", touching[0].vel[0], 0.2, 0.0);
}

}  // namespace

int main() {
    std::printf("=== amrex-contact physics core tests ===\n");

    testMomentumAndEnergy();
    testRestingBody();
    testDtIncrementAndCritical();
    testNoTunnelling();
    testTorque();
    testDiagnostics();
    testBookkeepingAndSpringDashpot();
    testWalls();

    std::printf("\n=== %d checks, %d failure(s) ===\n", g_checks, g_failures);
    return g_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
