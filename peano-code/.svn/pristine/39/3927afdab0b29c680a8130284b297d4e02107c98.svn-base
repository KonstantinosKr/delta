// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_ASCEND_H_
#define _PEANO_GRID_NODES_TASKS_ASCEND_H_


#include "peano/utils/Globals.h"

#include "peano/grid/nodes/Node.h"
#include "peano/grid/RegularGridContainer.h"

#include "peano/datatraversal/autotuning/OracleForOnePhase.h"

#include "peano/grid/nodes/loops/CallLeaveCellLoopBodyOnRegularRefinedPatch.h"
#include "peano/grid/nodes/loops/CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch.h"
#include "peano/grid/nodes/loops/CallAscendLoopBodyOnRegularRefinedPatch.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template <class Vertex, class Cell, class State, class EventHandle>
        class Ascend;
      }
    }
  }
}




/**
 * Ascend on regular refined subtree
 */
template <class Vertex, class Cell, class State, class EventHandle>
class peano::grid::nodes::tasks::Ascend {
  public:
    /**
     * @see Destructor of any of the loop bodies.
     */
    static tarch::multicore::BooleanSemaphore  _semaphore;
  private:
    typedef peano::grid::RegularGridContainer<Vertex,Cell>                                                             RegularGridContainer;

    typedef peano::grid::nodes::loops::CallLeaveCellLoopBodyOnRegularRefinedPatch<Vertex,Cell,State,EventHandle>             LeaveCellLoopBody;
    typedef peano::grid::nodes::loops::CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch<Vertex,Cell,State,EventHandle>   TouchVertexLastTimeLoopBody;
    typedef peano::grid::nodes::loops::CallAscendLoopBodyOnRegularRefinedPatch<Vertex,Cell,State,EventHandle>                AscendLoopBody;

    static tarch::logging::Log  _log;

    const int              _treeDepth;
    State&                 _state;
    EventHandle&           _eventHandle;
    RegularGridContainer&  _gridContainer;
    bool                   _treeRemainsStatic;

    TouchVertexLastTimeLoopBody  _touchVertexLastTimeLoopBody;
    LeaveCellLoopBody            _leaveCellLoopBody;
    AscendLoopBody               _ascendLoopBody;

    /**
     * !!! Optimisation
     *
     * If the specification says NOP or LEAVES_ONLY we may not skip whole
     * levels, as some vertices might have changed their refinement state. The
     * user might have triggered a coarsening for example. As a consequence, I
     * always invoke the touch last time loop on each level, but sometimes I
     * decide to switch off the event invocation.
     */
    void touchVerticesLastTime(int level);
    void ascend(int fineGridLevel);
    void leaveCells(int level);
  public:
    Ascend(
      const int              treeDepth,
      State&                 state,
      EventHandle&           eventHandle,
      RegularGridContainer&  gridContainer
    );

    void operator() ();
};


#include "peano/grid/nodes/tasks/Ascend.cpph"

#endif

