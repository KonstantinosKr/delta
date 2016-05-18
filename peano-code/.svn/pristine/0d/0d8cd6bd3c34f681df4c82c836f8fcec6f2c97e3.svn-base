// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_LOOPS_CALL_TOUCHE_VERTEX_LAST_TIME_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_LOOPS_CALL_TOUCHE_VERTEX_LAST_TIME_ON_REGULAR_REFINED_PATCH_H_


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
        class CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch;
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
 * Load Loop Body
 *
 * This loop body is responsible to fetch data from the vertex containers and
 * write them to the Cartesian grid data structure. See Grid::loadVertices()
 * for a detailed description of the loop body's semantics.
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell, class State, class EventHandle>
class peano::grid::nodes::loops::CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch {
  private:
    static tarch::logging::Log _log;

    int                                        _level;
    const int                                  _treeDepth;

#if defined(SharedMemoryParallelisation)
    EventHandle&                                                _eventHandle;
    EventHandle                                                 _threadLocalEventHandle;
#else
    EventHandle&                                                _eventHandle;
    EventHandle&                                                _threadLocalEventHandle;
#endif

    peano::grid::RegularGridContainer<Vertex,Cell>&  _regularGridContainer;

    /**
     * This field is shared among all threads.
     */
    bool&                                     _treeRemainsStatic;
    bool                                      _localTreeRemainsStatic;

    UnrolledLevelEnumerator  _fineGridEnumerator;
    UnrolledLevelEnumerator  _coarseGridEnumerator;

  public:
    /**
     * Study refinement state of vertex. If the vertex does not change, the
     * operation sets the height of the adjacent
     *
     * <h2> Thread safety </h2>
     *
     * This operation is not thread-safe.
     */
    void performVertexTransition( Vertex& vertex, int level );

    CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch(
      const int                                        treeDepth,
      EventHandle&                                     eventHandle,
      peano::grid::RegularGridContainer<Vertex,Cell>&  regularGridContainer,
      bool&                                            treeRemainsStatic
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
    ~CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch();

    void setLevel(int value);
    int getLevel() const;

    /**
     * Getter for the local copy of the tree-remains-static-flag to manually copy
     * it back to the global tree-remains-static-flag.
     */
    bool getLocalTreeRemainsStatic() const;

    /**
     * @see RegularRefined::callTouchVertexFirstTime()
     */
    void operator() (const tarch::la::Vector<DIMENSIONS, int>& i);
};


#include "peano/grid/nodes/loops/CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch.cpph"


#endif
