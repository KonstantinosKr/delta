// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_INVOKE_ENTER_CELL_H_
#define _PEANO_GRID_NODES_TASKS_INVOKE_ENTER_CELL_H_


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "tarch/multicore/MulticoreDefinitions.h"

#include "peano/utils/Globals.h"
#include "peano/datatraversal/TaskSet.h"
#include "peano/grid/SingleLevelEnumerator.h"


namespace peano {
    namespace grid {
      namespace nodes {
        namespace tasks {
          template <
            class Vertex,
            class Cell,
            class State,
            class EventHandle
          >
          class InvokeEnterCell;
        }
      }
    }
}




template < class Vertex, class Cell, class State, class EventHandle
>
class peano::grid::nodes::tasks::InvokeEnterCell {
  private:
    static tarch::logging::Log _log;

    State&                                                      _state;
    Cell&                                                       _fineGridCell;
    Vertex* const                                               _fineGridVertices;
    const peano::grid::SingleLevelEnumerator&  _fineGridVerticesEnumerator;
    Cell&                                                       _coarseGridCell;
    Vertex* const                                               _coarseGridVertices;
    const peano::grid::SingleLevelEnumerator&  _coarseGridVerticesEnumerator;
    const tarch::la::Vector<DIMENSIONS,int>&                    _fineGridPositionOfCell;

#if defined(SharedMemoryParallelisation)
    EventHandle&                                                _eventHandle;
    EventHandle                                                 _threadLocalEventHandle;
#else
    EventHandle&                                                _eventHandle;
    EventHandle&                                                _threadLocalEventHandle;
#endif

    double  _innerCells;
    double  _outerCells;

    double  _innerLeafCells;
    double  _outerLeafCells;

  public:
    InvokeEnterCell(
      State&                                     state,
      Cell&                                      fineGridCell,
      Vertex* const                              fineGridVertices,
      const peano::grid::SingleLevelEnumerator&  fineGridVerticesEnumerator,
      Cell&                                      coarseGridCell,
      Vertex* const                              coarseGridVertices,
      const peano::grid::SingleLevelEnumerator&  coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell,
      EventHandle&                               eventHandle
    );

    ~InvokeEnterCell();

    void operator() ();
};


#include "peano/grid/nodes/tasks/InvokeEnterCell.cpph"


#endif
