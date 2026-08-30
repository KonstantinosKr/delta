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

    bool mayRunEventsOnMultipleLevelsInParallel(int levelOfTouchLastTime) const;
  public:
    Ascend(
      const int              treeDepth,
      State&                 state,
      EventHandle&           eventHandle,
      RegularGridContainer&  gridContainer
    );

    /**
     * The task creates copy of the event through the three loop bodies. Those
     * guys have to be merged back into the master's loop body in the very end.
     * As this is a task, there is no explicit join() or merge() operation
     * provided. We simply hijack the destructor.
     */
    ~Ascend() = default;


    /**
     *
     * <h2>Ascend with nop or only leaves</h2>
     *
     * If we do ascend with no operations active or with operations only on the
     * leaves, the purpose of the present class is to update the vertex flags
     * (depth of adjacent subtrees, e.g.). Furthermore, we 'release' the
     * individual levels to touchVertexLastTime. If pipelining is active, the
     * touch last will run in parallel. As touch last relies on valid vertex
     * flags, it is therefore important to set/invalidate all flags before
     * asap - notably before we release the levels.
     *
     * We note that the ascend's manual vertex transition can be shortened
     * significantly in some cases:
     *
     * If none of the mappings does work on the whole grid, all the mappings
     * can only manipulate the finest grid or the next coarser grid (as we
     * always pass two levels into the events). So there's no need to run
     * through all levels.
     *
     */
    bool operator() ();

    /**
     * Can be called after operator() has terminated. It reveals whether the
     * tree remains static. Most codes do not have to query this flag as the
     * operator automatically updates the flags of the vertices on the coarsest
     * level.
     */
    bool treeRemainsStatic() const;
};


#include "peano/grid/nodes/tasks/Ascend.cpph"

#endif

