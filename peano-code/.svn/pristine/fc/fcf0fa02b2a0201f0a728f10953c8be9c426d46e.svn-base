// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_DESCEND_H_
#define _PEANO_GRID_NODES_TASKS_DESCEND_H_


#include "peano/utils/Globals.h"

#include "peano/grid/nodes/Node.h"
#include "peano/grid/RegularGridContainer.h"

#include "peano/datatraversal/autotuning/OracleForOnePhase.h"

#include "peano/grid/nodes/loops/CallEnterCellLoopBodyOnRegularRefinedPatch.h"
#include "peano/grid/nodes/loops/CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch.h"
#include "peano/grid/nodes/loops/CallDescendLoopBodyOnRegularRefinedPatch.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template <class Vertex, class Cell, class State, class EventHandle>
        class Descend;
      }
    }
  }
}




/**
 *
 * !!! Rationale
 *
 * This is realised as task but actually it should not be a task: The vertex
 * and cell events are strictly sequential in-between the levels, i.e. these
 * are not tasks that can run in parallel. Only the load and store process can
 * be deployed to an additional thread.
 */
template <class Vertex, class Cell, class State, class EventHandle>
class peano::grid::nodes::tasks::Descend {
  public:
    /**
     * @see Destructor of any of the loop bodies.
     */
    static tarch::multicore::BooleanSemaphore  _semaphore;
  private:
    typedef peano::grid::RegularGridContainer<Vertex,Cell>                                                                    RegularGridContainer;

    typedef peano::grid::nodes::loops::CallEnterCellLoopBodyOnRegularRefinedPatch<Vertex,Cell,State,EventHandle>              EnterCellLoopBody;
    typedef peano::grid::nodes::loops::CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch<Vertex,Cell,State,EventHandle>   TouchVertexFirstTimeLoopBody;
    typedef peano::grid::nodes::loops::CallDescendLoopBodyOnRegularRefinedPatch<Vertex,Cell,State,EventHandle>                DescendLoopBody;

    const int              _treeDepth;
    State&                 _state;
    EventHandle&           _eventHandle;
    RegularGridContainer&  _gridContainer;

    TouchVertexFirstTimeLoopBody  _touchVertexFirstTimeLoopBody;
    EnterCellLoopBody             _enterCellLoopBody;
    DescendLoopBody               _descendLoopBody;

    const bool                   _descendProcessRunsInParallelToOtherTasks;

    void touchVerticesFirstTime(int level);
    void descend(int fineGridLevel);
    void enterCells(int level);
public:
    Descend(
      const int              treeDepth,
      State&                 state,
      EventHandle&           eventHandle,
      RegularGridContainer&  gridContainer,
      bool                   descendProcessRunsInParallelToOtherTasks
    );

    void operator() ();
};


#include "peano/grid/nodes/tasks/Descend.cpph"

#endif

