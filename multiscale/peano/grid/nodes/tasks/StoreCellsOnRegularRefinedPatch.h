// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_STORE_CELLS_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_TASKS_STORE_CELLS_ON_REGULAR_REFINED_PATCH_H_


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template <class Vertex, class Cell, class CellStack>
        class StoreCellsOnRegularRefinedPatch;
      }
    }
  }
}


/**
 * I need both the vertex and the cell as template argument as the task fills
 * the grid container with data (only with cells), but the grid container is
 * specified in both vertices and cells.
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell, class CellStack>
class peano::grid::nodes::tasks::StoreCellsOnRegularRefinedPatch {
  private:
    static tarch::logging::Log  _log;

    peano::utils::LoopDirection                                       _topLevelLoopDirection;
    const int                                                         _treeDepth;
    const bool                                                        _isTraversalInverted;
    peano::grid::RegularGridContainer<Vertex,Cell>&                   _regularGridContainer;
    CellStack&                                                        _cellStack;
    const bool                                                        _storeProcessRunsInParallelToOtherTasks;

    /**
     * Store cells
     *
     * This operation is a recursive operation running through all levels
     * bottom-up. It first waits until all events have been called for a
     * given level. This wait immediately terminates if no events are called
     * in parallel, i.e. if no ascend process is running in parallel.
     *
     * Afterwards, it invokes storedCellOfRegularRefinedGrid(), i.e. it
     * switches the input and output stack statistics (how many vertices are
     * stored/read from which stack) and streams the cell to the output
     * stream. If the vertex push process is running in parallel, we may not
     * invert the statistics on the regular grid data structures, as these
     * properties might be required by the store vertex process. Instead, this
     * process graps a copy of the cells and streams the copy to the output
     * stack.
     *
     * !!! Inefficiency patterns
     *
     * If the cell storage is ran in parallel to the cell processing
     * (pipelining), it can happen that the process waits quite some time
     * before it can start to store the cells actually. In this case, it does
     * not permanently poll the cell state, but it moves to the background
     * several times (see the macro sendThisTaskToBackground).
     *
     * If it has to wait too long, it writes a warning. In this case, it
     * might make sense to switch off pipelining for the store process -
     * obviously the store process has to wait that long before it can start
     * that it does make more sense to invest all computational ressources no
     * the processing and afterwards run the store process serially.
     */
    void storeCells(
      peano::utils::LoopDirection               loopDirection,
      int                                       currentLevel,
      const tarch::la::Vector<DIMENSIONS,int>&  offsetWithinPatch
    );

  public:
    StoreCellsOnRegularRefinedPatch(
      peano::utils::LoopDirection                                       topLevelLoopDirection,
      const int                                                         treeDepth,
      const bool                                                        isTraversalInverted,
      peano::grid::RegularGridContainer<Vertex,Cell>&                   regularGridContainer,
      CellStack&                                                        cellStack,
      bool                                                              storeProcessRunsInParallelToOtherTasks
   );

    bool operator()();
};


#include "peano/grid/nodes/tasks/StoreCellsOnRegularRefinedPatch.cpph"


#endif
