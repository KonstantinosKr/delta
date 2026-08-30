// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_LOAD_SUB_CELL_H_
#define _PEANO_GRID_NODES_TASKS_LOAD_SUB_CELL_H_


#include "tarch/logging/Log.h"

#include "peano/utils/Globals.h"
#include "peano/datatraversal/TaskSet.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template < class Cell, class CellStack >
        class LoadSubCells;
      }
    }
  }
}




template < class Cell, class CellStack >
class peano::grid::nodes::tasks::LoadSubCells {
  private:
    static tarch::logging::Log _log;

    Cell * const                 _subCells;
    CellStack&                   _cellStack;
    peano::utils::LoopDirection  _loopDirection;

    #ifdef Debug
    const int                    _coarseFatherCellLevel;
    #endif

  public:
    LoadSubCells(
      Cell * const                        subCells,
      CellStack&                          cellStack,
      const peano::utils::LoopDirection&  loopDirection
      #ifdef Debug
      , int                                 coarseFatherCellLevel
      #endif
    );

    bool operator()();
};


#include "peano/grid/nodes/tasks/LoadSubCells.cpph"


#endif
