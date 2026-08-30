// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_LOOPS_CALL_ASCEND_LOOP_BODY_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_LOOPS_CALL_ASCEND_LOOP_BODY_ON_REGULAR_REFINED_PATCH_H_


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
        class CallAscendLoopBodyOnRegularRefinedPatch;
      }


      namespace tasks {
        /**
         * Forward declaration
         */
        template <class Vertex, class Cell, class State, class EventHandle>
        class Ascend;
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
class peano::grid::nodes::loops::CallAscendLoopBodyOnRegularRefinedPatch {
  private:
    static tarch::logging::Log _log;

    const int                                        _coarseLevel;

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
    CallAscendLoopBodyOnRegularRefinedPatch(
      EventHandle&                                      eventHandle,
      peano::grid::RegularGridContainer<Vertex,Cell>&   regularGridContainer,
      int                                               level
    );

    ~CallAscendLoopBodyOnRegularRefinedPatch() = default;

    /**
     * The Ascend/Descend tasks do copy around events indirectly
     * through the loop objects. Each of the loop objects can fork
     * further through parallel loops. These sets of loops do merge
     * automatically through mergeWithWorkerThread(). What we have
     * to do in the end is to call the loop to merge back their local
     * event handle copy into the global event handle. And this has
     * to be done explicitly - we are not allowed to use the destructor
     * as also the copies created by the parallel loops are destroyed.
     *
     * We have to protect the merger into the master event handle with a
     * semaphore. If the code uses pipelining, we otherwise might mess up the
     * result.
     */
    void mergeIntoMasterThread() const;

    /**
     * @see RegularRefined::callTouchVertexFirstTime()
     */
    void operator() (const tarch::la::Vector<DIMENSIONS, int>& i);
};


#include "peano/grid/nodes/loops/CallAscendLoopBodyOnRegularRefinedPatch.cpph"


#endif
