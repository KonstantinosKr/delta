// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_STORE_SUB_CELL_H_
#define _PEANO_GRID_NODES_TASKS_STORE_SUB_CELL_H_


#include "tarch/logging/Log.h"

#include "peano/utils/Globals.h"
#include "peano/datatraversal/TaskSet.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template < class Cell, class CellStack >
        class StoreSubCells;
      }
    }
  }
}




template < class Cell, class CellStack >
class peano::grid::nodes::tasks::StoreSubCells {
  private:
    static tarch::logging::Log _log;

    Cell&                        _coarseCell;
    Cell const * const           _subCells;
    CellStack&                   _cellStack;
    peano::utils::LoopDirection  _loopDirection;
  public:
    StoreSubCells(
      Cell&                               coarseCell,
      Cell const * const                  subCells,
      CellStack&                          cellStack,
      const peano::utils::LoopDirection&  loopDirection
    );

    bool operator()();
};


#include "peano/grid/nodes/tasks/StoreSubCells.cpph"


#endif
