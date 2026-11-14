/*
 * Dynamic time step, thesis chapter 7 (Algorithms 11 and 12), ported from the
 * amrex_contact experiment tree.
 *
 * Two bounds, both tightening only:
 *   - grid bound  dt <= h_min / (2*inc*Vmax), and the no-tunnelling rule of
 *     7.1(iii) dt <= 0.5*h_min/Vmax;
 *   - critical-approach bound dt <= 0.5*gap/|v_n| for the tightest approaching
 *     pair, else grow by the constant factor inc.
 *
 * Such a step is computed from the last force pass, so it applies to the *next*
 * step (the thesis controller is also self-advancing: its decision is the new
 * current step).
 *
 * shortcut: gap and approach use the bounding spheres, which is exact for the
 * Sphere collision model delta runs today. A triangulated body sits inside its
 * circumscribed sphere, so the moment the tri-tri path is used here the gap must
 * become the support function (thesis eq. in 7.2) or dt will be over-tightened.
 */
#ifndef DELTA_CORE_PARALLEL_DTCONTROLLER_H_
#define DELTA_CORE_PARALLEL_DTCONTROLLER_H_

#include <utility>
#include <vector>

#include "core/delta.h"
#include "GridHierarchy.h"

namespace delta {
  namespace core {
    namespace parallel {

enum class ApproachState
{
  Separation,
  NonCritical,
  Critical
};

struct DtState
{
  iREAL dt      = 1.0e-3;
  iREAL dtMin   = 1.0e-5;
  iREAL dtMax   = 1.0e-2;
  iREAL inc     = 1.1;    /* constant growth factor (Algorithm 12) */
  iREAL safety  = 0.9;
  bool  dynamic = true;
};

struct DtDecision
{
  iREAL dtNext   = 1.0e-3;
  iREAL vMax     = 0.0;
  iREAL dtLocal  = 0.0;
  ApproachState worst = ApproachState::Separation;
  long  criticalPairs    = 0;
  long  approachingPairs = 0;
  long  separatingPairs  = 0;
};

class DtController
{
public:
  DtController() {}
  explicit DtController(const DtState& state) : _state(state) {}

  ApproachState stateOf(const GridBody& a, const GridBody& b,
                        iREAL& normalVelocity, iREAL& gap) const;

  /* hMin: smallest cell edge of the grid the pairs were found on. */
  DtDecision update(const std::vector<GridBody>& bodies,
                    const std::vector<std::pair<int, int> >& pairs,
                    iREAL hMin);

  const DtState& state() const { return _state; }
  void setState(const DtState& state) { _state = state; }
  void setDynamic(bool dynamic) { _state.dynamic = dynamic; }
  iREAL dt() const { return _state.dt; }

private:
  DtState _state;
};

    }
  }
}

#endif /* DELTA_CORE_PARALLEL_DTCONTROLLER_H_ */
