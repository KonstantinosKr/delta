// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_LOOPS_CALL_TOUCH_VERTEX_FIRST_TIME_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_LOOPS_CALL_TOUCH_VERTEX_FIRST_TIME_ON_REGULAR_REFINED_PATCH_H_


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "tarch/multicore/MulticoreDefinitions.h"

#include "peano/utils/Globals.h"

#include "peano/datatraversal/Action.h"
#include "peano/grid/RegularGridContainer.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace loops {
        template <class Vertex, class Cell, class State, class EventHandle>
        class CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch;
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
 * Load Loop Body
 *
 * This loop body is responsible to fetch data from the vertex containers and
 * write them to the Cartesian grid data structure. See Grid::loadVertices()
 * for a detailed description of the loop body's semantics.
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell, class State, class EventHandle>
class peano::grid::nodes::loops::CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch {
  private:
    static tarch::logging::Log _log;

    int                                        _level;

    State&                                     _state;

#if defined(SharedMemoryParallelisation)
    EventHandle&                                                _eventHandle;
    EventHandle                                                 _threadLocalEventHandle;
#else
    EventHandle&                                                _eventHandle;
    EventHandle&                                                _threadLocalEventHandle;
#endif

    peano::grid::RegularGridContainer<Vertex,Cell>&  _regularGridContainer;

    double _numberOfInnerVertices;
    double _numberOfBoundaryVertices;
    double _numberOfOuterVertices;

    double _numberOfInnerLeafVertices;
    double _numberOfBoundaryLeafVertices;
    double _numberOfOuterLeafVertices;

    UnrolledLevelEnumerator  _fineGridEnumerator;
    UnrolledLevelEnumerator  _coarseGridEnumerator;

  public:
    CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch(
      State&        state,
      EventHandle&  eventHandle,
      peano::grid::RegularGridContainer<Vertex,Cell>&  regularGridContainer
    );

    /**
     * Copy constructor
     *
     * This type keeps track of some statistics such as the number of inner
     * vertices. These bookkeeping records are reduced in the destructor of the
     * class which in turn holds a semaphore (see destructor comments).
     * Consequently, my copy constructor has to inherit all the aggregated
     * data, but the bookkeeping records have to be reset to zero in the
     * copy constructor - otherwise partial results are reduced twice.
     */
    CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch(
      const CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch& copy
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
    ~CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch();

    void setLevel(int value);
    int getLevel() const;

    /**
     * @see RegularRefined::callTouchVertexFirstTime()
     */
    void operator() (const tarch::la::Vector<DIMENSIONS, int>& i);
};


#include "peano/grid/nodes/loops/CallTouchVertexFirstTimeLoopBodyOnRegularRefinedPatch.cpph"


#endif
