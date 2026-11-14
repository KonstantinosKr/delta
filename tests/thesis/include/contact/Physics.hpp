#pragma once
// Contact response and time integration, thesis 2.3 (contact forces),
// 5.1 (time integration) and chapter 7 (dynamic time step).
//
// Pure C++17, no AMReX, so it can be unit-tested standalone.

#include <string>
#include <vector>

#include "contact/Types.hpp"

namespace contact {

enum class ContactModel {
    Penalty,       // linear penalty spring in the normal direction + Coulomb cap
    SpringDashpot  // Hooke + viscous dashpot (restitution-controlled)
};

[[nodiscard]] const char* toString(ContactModel m) noexcept;
[[nodiscard]] bool contactModelFromString(const std::string& s, ContactModel& out) noexcept;

struct Material {
    Real kn{1.0e5};        // normal stiffness  [N/m]
    Real kt{2.0e5};        // tangential stiffness
    Real gamma_n{1.0};     // normal damping
    Real gamma_t{1.0};     // tangential damping
    Real mu{0.5};          // Coulomb friction coefficient
    Real restitution{0.5};
};

struct BodyState {
    Vec3 vel{};
    Vec3 omega{};
};

// Per-pair bookkeeping kept across steps (thesis 5.7 memory organisation:
// persistent contact set keyed by global particle ids).
struct ContactHistory {
    Vec3 tangential_anchor{};  // world point, for stick/slip
    bool active{false};
};

class ContactDynamics {
  public:
    void computeForces(std::vector<Particle>& bodies,
                       const std::vector<ContactPoint>& contacts,
                       const Material& mat,
                       ContactModel model,
                       Real dt);

    // Semi-implicit (symplectic) Euler, thesis 5.1.
    void integrate(std::vector<Particle>& bodies, Real dt) const;

    // Axis-aligned box walls; `restitution` < 0 means "use material value".
    void applyWalls(std::vector<Particle>& bodies, const Aabb& box, Real restitution) const;

    [[nodiscard]] const std::vector<Real>& lastPairForce() const noexcept { return pair_force_; }

  private:
    std::vector<Real> pair_force_;
};

// ---------------------------------------------------------------------------
// Dynamic time step, thesis chapter 7 (Algorithms 11 and 12)
// ---------------------------------------------------------------------------

enum class ApproachState { Separation, NonCritical, Critical };

struct DtState {
    Real dt{1.0e-3};
    Real dt_min{1.0e-4};
    Real dt_max{1.0e-2};
    Real inc{1.1};       // thesis: constant increment factor
    Real safety{0.9};
    bool dynamic{true};
};

struct DtDecision {
    Real dt_next{1.0e-3};
    Real v_max{0.0};
    Real dt_local{0.0};        // smallest critical-approach step seen
    ApproachState worst{ApproachState::Separation};
    long critical_pairs{0};
    long approaching_pairs{0};
    long separating_pairs{0};
};

// Uses the halo spheres (r_halo = 1.1 * rr) and the signed normal relative
// velocity v_BA = (v_A . d_BA) - (v_B . d_BA), d_BA = c_B - c_A.
class DtController {
  public:
    DtController() = default;
    explicit DtController(DtState s) : state_(s) {}

    // h_min / h_max come from the grid hierarchy (thesis 7.1/7.2).
    DtDecision update(const std::vector<Particle>& bodies,
                      const std::vector<ContactPair>& pairs,
                      Real h_min, Real h_max);

    [[nodiscard]] ApproachState stateOf(const Particle& a, const Particle& b,
                                        Real& v_ba, Real& gap) const noexcept;

    [[nodiscard]] const DtState& state() const noexcept { return state_; }
    void setState(const DtState& s) noexcept { state_ = s; }
    void setDynamic(bool on) noexcept { state_.dynamic = on; }

  private:
    DtState state_;
};

// ---------------------------------------------------------------------------
// Diagnostics
// ---------------------------------------------------------------------------

struct Diagnostics {
    Real kinetic_linear{0.0};
    Real kinetic_angular{0.0};
    Vec3 momentum{};
    Real max_penetration{0.0};
    long contacts{0};
    long tri_pairs{0};
};

[[nodiscard]] Diagnostics computeDiagnostics(const std::vector<Particle>& bodies,
                                             const std::vector<ContactPoint>& contacts);

}  // namespace contact
