// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_LOAD_CELLS_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_TASKS_LOAD_CELLS_ON_REGULAR_REFINED_PATCH_H_


#include "peano/grid/RegularGridContainer.h"
#include "peano/utils/Globals.h"
#include "tarch/logging/Log.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template <class Vertex, class Cell, class CellStack>
        class LoadCellsOnRegularRefinedPatch;
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
class peano::grid::nodes::tasks::LoadCellsOnRegularRefinedPatch {
  private:
    static tarch::logging::Log  _log;

    peano::utils::LoopDirection                                       _topLevelLoopDirection;
    const int                                                         _treeDepth;
    const bool                                                        _isTraversalInverted;
    peano::grid::RegularGridContainer<Vertex,Cell>&                   _regularGridContainer;
    CellStack&                                                        _cellStack;

    const bool                                                        _runsInParallelToOtherTasks;

    void loadCells(
      peano::utils::LoopDirection               loopDirection,
      int                                       currentLevel,
      const tarch::la::Vector<DIMENSIONS,int>&  offsetWithinPatch
    );

  public:
    LoadCellsOnRegularRefinedPatch(
      peano::utils::LoopDirection                                       topLevelLoopDirection,
      const int                                                         treeDepth,
      const bool                                                        isTraversalInverted,
      peano::grid::RegularGridContainer<Vertex,Cell>&                   regularGridContainer,
      CellStack&                                                        cellStack,
      bool                                                              runsInParallelToOtherTasks
   );

    bool operator()();
};


#include "peano/grid/nodes/tasks/LoadCellsOnRegularRefinedPatch.cpph"


#endif
