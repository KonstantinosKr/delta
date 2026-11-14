/*
 * Chapter-7 step controller (thesis Algorithms 11/12): the step may only be
 * tightened by the grid, never relaxed past the explicit-stability cap the
 * caller brought in.
 *
 *   - a closing pair with a positive gap gives dt <= 0.5*gap/|v_n|;
 *   - an interpenetrating pair must NOT tighten (the contact law owns it, and a
 *     remaining-gap bound would collapse dt to zero and freeze the run);
 *   - no-tunnelling: dt <= 0.5*h_min/Vmax, applied even with a fixed step;
 *   - without a critical pair the step grows by the constant factor inc, capped
 *     at dtMax.
 *
 * Run: ctest --test-dir build  (or ./build/delta_dt_controller_test)
 */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <vector>

#include "dim3/backends/amrex/DtController.h"

using delta::core::parallel::ApproachState;
using delta::core::parallel::DtController;
using delta::core::parallel::DtDecision;
using delta::core::parallel::DtState;
using delta::core::parallel::GridBody;

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond);          \
      std::exit(1);                                                        \
    }                                                                      \
  } while (0)

namespace {

GridBody body(double x, double vx, double diameter = 0.1)
{
  GridBody b;
  b.pos = {{x, 0.0, 0.0}};
  b.vel = {{vx, 0.0, 0.0}};
  b.diameter = diameter;
  b.reach = 0.5 * diameter;
  return b;
}

DtState baseState()
{
  DtState s;
  s.dt = 1.0e-5;
  s.dtMin = 1.0e-7;
  s.dtMax = 1.0e-5;   /* the explicit-stability cap: tightening only */
  s.inc = 1.1;
  s.dynamic = true;
  return s;
}

}  // namespace

int main()
{
  // ---- 1. closing with a positive gap is critical, and the bound is tight ---
  {
    std::vector<GridBody> bodies;
    bodies.push_back(body(0.0, 1.0));        // A moves +x
    bodies.push_back(body(0.10001, -1.0));   // B moves -x; gap = 1e-5
    std::vector<std::pair<int, int> > pairs(1, std::make_pair(0, 1));

    DtState state = baseState();
    state.dtMax = 1.0;                    // do not hide the bound behind the cap
    DtController controller(state);

    iREAL normalVelocity = 0.0, gap = 0.0;
    const ApproachState kind = controller.stateOf(bodies[0], bodies[1], normalVelocity, gap);
    CHECK(kind == ApproachState::Critical);
    CHECK(std::fabs(gap - 1.0e-5) < 1e-12);
    CHECK(normalVelocity < 0.0);          /* closing */

    const DtDecision decision = controller.update(bodies, pairs, /*hMin=*/1.0);
    CHECK(decision.criticalPairs == 1);
    /* 0.5 * gap / |v_n| = 0.5*1e-5/2 = 2.5e-6, dtMax = 1.0 does not hide it */
    CHECK(std::fabs(decision.dtNext - 2.5e-6) < 1e-12);
  }

  // ---- 2. an interpenetrating pair must not tighten the step ---------------
  {
    std::vector<GridBody> bodies;
    bodies.push_back(body(0.0, 1.0));
    bodies.push_back(body(0.05, -1.0));   // overlap of 0.05
    std::vector<std::pair<int, int> > pairs(1, std::make_pair(0, 1));

    DtController controller(baseState());
    const DtDecision decision = controller.update(bodies, pairs, 1.0);
    CHECK(decision.criticalPairs == 0);
    CHECK(decision.approachingPairs == 1);
    /* grows by inc from 1e-5, then capped at dtMax = 1e-5 */
    CHECK(std::fabs(decision.dtNext - 1.0e-5) < 1e-15);
  }

  // ---- 3. no-tunnelling: half a cell per step, even for a fixed step -------
  {
    std::vector<GridBody> bodies;
    bodies.push_back(body(0.0, 10.0));
    bodies.push_back(body(5.0, 10.0));
    std::vector<std::pair<int, int> > pairs(1, std::make_pair(0, 1));

    DtState state = baseState();
    state.dt = 1.0e-3;
    state.dtMax = 1.0e-3;
    state.dynamic = false;               /* fixed step still gets the tunnel rule */
    DtController controller(state);
    const DtDecision decision = controller.update(bodies, pairs, /*hMin=*/0.01);
    CHECK(decision.separatingPairs + decision.approachingPairs == 1);
    CHECK(decision.dtNext <= 0.5 * 0.01 / 10.0 + 1e-15);
    CHECK(decision.dtNext > 0.0);
  }

  // ---- 4. growth is capped by dtMax and never exceeds it ------------------
  {
    std::vector<GridBody> bodies;
    bodies.push_back(body(0.0, 0.0));
    bodies.push_back(body(100.0, 0.0));
    std::vector<std::pair<int, int> > pairs;   /* no pairs at all */

    DtState state = baseState();
    state.dtMax = 2.0e-5;
    DtController controller(state);
    for (int k = 0; k < 20; ++k) {
      const DtDecision decision = controller.update(bodies, pairs, 1.0);
      CHECK(decision.dtNext <= 2.0e-5 + 1e-15);
    }
  }

  // ---- 5. separating pairs never tighten --------------------------------
  {
    std::vector<GridBody> bodies;
    bodies.push_back(body(0.0, -1.0));    // moving apart
    bodies.push_back(body(0.2, 1.0));
    std::vector<std::pair<int, int> > pairs(1, std::make_pair(0, 1));

    DtController controller(baseState());
    iREAL normalVelocity = 0.0, gap = 0.0;
    CHECK(controller.stateOf(bodies[0], bodies[1], normalVelocity, gap) ==
          ApproachState::Separation);
    const DtDecision decision = controller.update(bodies, pairs, 1.0);
    CHECK(decision.separatingPairs == 1);
    CHECK(decision.criticalPairs == 0);
  }

  std::printf("delta_dt_controller_test OK: gap bound, overlap guard, tunnel rule, "
              "growth cap, separation\n");
  return 0;
}
