// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_LOOPS_CALL_DESCEND_LOOP_BODY_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_LOOPS_CALL_DESCEND_LOOP_BODY_ON_REGULAR_REFINED_PATCH_H_


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "tarch/multicore/MulticoreDefinitions.h"

#include "peano/utils/Globals.h"

#include "peano/datatraversal/Action.h"
#include "peano/grid/RegularGridContainer.h"
#include "peano/grid/UnrolledLevelEnumerator.h"
#include "peano/grid/UnrolledAscendDescendLevelEnumerator.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace loops {
        template <class Vertex, class Cell, class State, class EventHandle>
        class CallDescendLoopBodyOnRegularRefinedPatch;
      }

      namespace tasks {
        /**
         * Forward declaration
         */
        template <class Vertex, class Cell, class State, class EventHandle>
        class Descend;
      }
    }
  }
}




/**
 * Ascend Loop Body
 *
 * This loop body is responsible to fetch data from the vertex containers and
 * write them to the Cartesian grid data structure. See Grid::loadVertices()
 * for a detailed description of the loop body's semantics.
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell, class State, class EventHandle>
class peano::grid::nodes::loops::CallDescendLoopBodyOnRegularRefinedPatch {
  private:
    static tarch::logging::Log _log;

    int                                        _coarseLevel;

#if defined(SharedMemoryParallelisation)
    EventHandle&                                                _eventHandle;
    EventHandle                                                 _threadLocalEventHandle;
#else
    EventHandle&                                                _eventHandle;
    EventHandle&                                                _threadLocalEventHandle;
#endif

    peano::grid::RegularGridContainer<Vertex,Cell>&  _regularGridContainer;


    UnrolledAscendDescendLevelEnumerator             _fineGridEnumerator;
    UnrolledLevelEnumerator                          _coarseGridEnumerator;

  public:
    CallDescendLoopBodyOnRegularRefinedPatch(
      EventHandle&                                      eventHandle,
      peano::grid::RegularGridContainer<Vertex,Cell>&   regularGridContainer
    );

    /**
     * Destructor
     *
     * <h2> Multithreading </h2>
     *
     * We may not use a semaphore of our own, as there's always two different
     * classes involved on regular patches (besides the fact that these classes
     * themselves might be forked among multiple threads): For cells and for
     * vertices. Furthermore, there is also an ascend loop and we do not know
     * when this type's destructor is called.
     *
     * Therefore, these three loop bodies have to share one semaphore. I could
     * assign it to one of these classes but decided to move it do the overall
     * task, i.e. to ascend/descend.
     */
    ~CallDescendLoopBodyOnRegularRefinedPatch();

    void setCoarseGridLevel(int value);
    int getCoarseGridLevel() const;

    /**
     * @see RegularRefined::callTouchVertexFirstTime()
     */
    void operator() (const tarch::la::Vector<DIMENSIONS, int>& i);
};


#include "peano/grid/nodes/loops/CallDescendLoopBodyOnRegularRefinedPatch.cpph"


#endif
