#include "DtController.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace delta {
  namespace core {
    namespace parallel {

namespace {

constexpr iREAL kEps = 1.0e-12;

inline iREAL norm3(const std::array<iREAL, 3>& v)
{
  return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

}  // namespace

ApproachState DtController::stateOf(const GridBody& a, const GridBody& b,
                                    iREAL& normalVelocity, iREAL& gap) const
{
  const iREAL dx = b.pos[0] - a.pos[0];
  const iREAL dy = b.pos[1] - a.pos[1];
  const iREAL dz = b.pos[2] - a.pos[2];
  const iREAL distance = std::sqrt(dx*dx + dy*dy + dz*dz);

  normalVelocity = 0.0;
  gap = -std::numeric_limits<iREAL>::max();
  if (distance <= kEps) {return ApproachState::Separation;}   /* unusable direction */

  const iREAL dir[3] = {dx / distance, dy / distance, dz / distance};

  /* Surface gap along the centre line; for spheres this is exactly
   * |d| - (rA + rB), see the shortcut note in the header. */
  gap = distance - 0.5 * (a.diameter + b.diameter);

  const std::array<iREAL, 3> dv = {{b.vel[0] - a.vel[0],
                                    b.vel[1] - a.vel[1],
                                    b.vel[2] - a.vel[2]}};
  normalVelocity = dv[0]*dir[0] + dv[1]*dir[1] + dv[2]*dir[2];   /* < 0: closing */

  if (normalVelocity >= 0.0) {return ApproachState::Separation;}
  /* Touching or interpenetrating: the contact law owns the pair, and a
   * remaining-gap bound would collapse dt to zero and freeze the run. */
  if (gap <= 0.0) {return ApproachState::NonCritical;}

  /* Critical approach: extrapolate over the next grown step and see whether the
   * surfaces would meet (Algorithm 11). */
  const iREAL probe = _state.dt * (1.0 + _state.inc);
  const iREAL ax = a.pos[0] + a.vel[0] * probe;
  const iREAL ay = a.pos[1] + a.vel[1] * probe;
  const iREAL az = a.pos[2] + a.vel[2] * probe;
  const iREAL bx = b.pos[0] + b.vel[0] * probe;
  const iREAL by = b.pos[1] + b.vel[1] * probe;
  const iREAL bz = b.pos[2] + b.vel[2] * probe;
  const iREAL probeDistance = std::sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay) + (bz-az)*(bz-az));
  const iREAL contactDistance = distance - gap;   /* centre distance at touch */

  return (probeDistance < contactDistance) ? ApproachState::Critical
                                           : ApproachState::NonCritical;
}

DtDecision DtController::update(const std::vector<GridBody>& bodies,
                                const std::vector<std::pair<int, int> >& pairs,
                                iREAL hMin)
{
  DtDecision decision;
  decision.dtNext = _state.dt;

  const iREAL inc = (_state.inc > 1.0) ? _state.inc : 1.1;

  iREAL vMax = 0.0;
  for (std::size_t b = 0; b < bodies.size(); ++b) {
    const iREAL speed = norm3(bodies[b].vel);
    if (std::isfinite(speed) && speed > vMax) {vMax = speed;}
  }
  decision.vMax = vMax;

  const bool gridOk = std::isfinite(hMin) && hMin > 0.0;
  iREAL dtMax = _state.dtMax;
  if (gridOk && vMax > kEps) {dtMax = std::min(dtMax, hMin / (2.0 * inc * vMax));}
  if (!std::isfinite(dtMax) || dtMax <= 0.0) {dtMax = std::numeric_limits<iREAL>::max();}

  const int count = static_cast<int>(bodies.size());
  iREAL localMin = std::numeric_limits<iREAL>::infinity();

  for (std::size_t k = 0; k < pairs.size(); ++k) {
    const int i = pairs[k].first;
    const int j = pairs[k].second;
    if (i < 0 || j < 0 || i >= count || j >= count || i == j) {continue;}

    iREAL normalVelocity = 0.0;
    iREAL gap = 0.0;
    const ApproachState state = stateOf(bodies[static_cast<std::size_t>(i)],
                                        bodies[static_cast<std::size_t>(j)],
                                        normalVelocity, gap);

    if (state == ApproachState::Critical) {
      ++decision.criticalPairs;
      const iREAL speed = std::abs(normalVelocity);
      iREAL local = _state.dt;
      if (speed > kEps && gap > 0.0) {local = 0.5 * gap / speed;}
      if (!std::isfinite(local) || local <= 0.0) {local = _state.dt;}
      localMin = std::min(localMin, local);
      decision.worst = ApproachState::Critical;
    } else if (state == ApproachState::NonCritical) {
      ++decision.approachingPairs;
      if (decision.worst == ApproachState::Separation) {decision.worst = ApproachState::NonCritical;}
    } else {
      ++decision.separatingPairs;
    }
  }

  decision.dtLocal = std::isfinite(localMin) ? localMin : 0.0;

  if (_state.dynamic) {
    if (decision.criticalPairs > 0) {
      decision.dtNext = std::max(_state.dtMin, localMin);
    } else {
      decision.dtNext = _state.dt * inc;
    }
    if (decision.dtNext > dtMax) {decision.dtNext = dtMax;}
  } else {
    decision.dtNext = _state.dt;
  }

  /* No-tunnelling rule 7.1(iii): half a cell per step at most. Outside the
   * dynamic branch because a fixed step can be unsafe too. */
  if (gridOk && vMax > kEps) {
    const iREAL tunnel = 0.5 * hMin / vMax;
    if (decision.dtNext > tunnel) {decision.dtNext = tunnel;}
  }

  if (!std::isfinite(decision.dtNext) || decision.dtNext <= 0.0) {
    decision.dtNext = _state.dtMin;
  }

  _state.dt = decision.dtNext;   /* self-advancing */
  return decision;
}

    }
  }
}
