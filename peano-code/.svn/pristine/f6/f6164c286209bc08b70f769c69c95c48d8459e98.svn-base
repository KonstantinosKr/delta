#include "peano/datatraversal/autotuning/OracleForOnePhase.h"
#include "tarch/Assertions.h"


std::string peano::datatraversal::autotuning::toString( const MethodTrace& methodTrace ) {
  switch (methodTrace) {
    case LoadVertices:
      return "load-vertices";
    case LoadVerticesOnRegularStationaryGrid:
      return "load-vertices-on-regular-stationary-grid";
    case LoadVerticesOnIrregularStationaryGrid:
      return "load-vertices-on-irregular-stationary-grid";
    case StoreVertices:
      return "store-vertices";
    case StoreVerticesOnIrregularStationaryGrid:
      return "store-vertices-on-irregular-stationary-grid";
    case StoreVerticesOnRegularStationaryGrid:
      return "store-vertices-on-regular-stationary-grid";
    case CallEnterCellAndLoadSubCellsWithinTraverse:
      return "enter-cell-and-load-sub-cells";
    case CallLeaveCellAndStoreSubCellsWithinTraverse:
      return "leave-cell-and-store-sub-cells";
    case CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid:
      return "call-enter-cell-and-initialise-enumerators-on-regular-stationary-grid";
    case CallTouchFirstTimeOnRegularStationaryGrid:
      return "call-touch-first-time-on-regular-stationary-grid";
    case CallTouchLastTimeOnRegularStationaryGrid:
      return "call-touch-last-time-on-regular-stationary-grid";
    case CallEnterCellOnRegularStationaryGrid:
      return "call-enter-cell-on-regular-stationary-grid";
    case CallLeaveCellOnRegularStationaryGrid:
      return "call-leave-cell-on-regular-stationary-grid";
    case PipelineAscendTask:
      return "pipeline-ascend-task";
    case PipelineDescendTask:
      return "pipeline-descend-task";
    case SplitLoadVerticesTaskOnRegularStationaryGrid:
      return "split-load-vertices-task-on-regular-stationary-grid";
    case SplitStoreVerticesTaskOnRegularStationaryGrid:
      return "split-store-vertices-task-on-regular-stationary-grid";
    case AscendOnRegularStationaryGrid:
      return "ascend-on-regular-stationary-grid";
    case DescendOnRegularStationaryGrid:
      return "descend-on-regular-stationary-grid";

    case UserDefined0:
      return "user-defined-0";
    case UserDefined1:
      return "user-defined-1";
    case UserDefined2:
      return "user-defined-2";
    case UserDefined3:
      return "user-defined-3";
    case UserDefined4:
      return "user-defined-4";
    case UserDefined5:
      return "user-defined-5";
    case UserDefined6:
      return "user-defined-6";
    case UserDefined7:
      return "user-defined-7";
    case UserDefined8:
      return "user-defined-8";
    case UserDefined9:
      return "user-defined-9";
    case UserDefined10:
      return "user-defined-10";
    case UserDefined11:
      return "user-defined-11";
    case UserDefined12:
      return "user-defined-12";

    case NumberOfDifferentMethodsCalling:
      return "<error>";
  }

  return "<error>";
}


peano::datatraversal::autotuning::MethodTrace peano::datatraversal::autotuning::toMethodTrace(int value) {
  assertion( value>=0 );
  assertion( value<NumberOfDifferentMethodsCalling );
  switch (value) {
    case static_cast<int>(LoadVertices):
      return LoadVertices;
    case static_cast<int>(LoadVerticesOnRegularStationaryGrid):
      return LoadVerticesOnRegularStationaryGrid;
    case static_cast<int>(LoadVerticesOnIrregularStationaryGrid):
      return LoadVerticesOnIrregularStationaryGrid;
    case static_cast<int>(StoreVertices):
      return StoreVertices;
    case static_cast<int>(StoreVerticesOnRegularStationaryGrid):
      return StoreVerticesOnRegularStationaryGrid;
    case static_cast<int>(StoreVerticesOnIrregularStationaryGrid):
      return StoreVerticesOnIrregularStationaryGrid;
    case static_cast<int>(CallEnterCellAndLoadSubCellsWithinTraverse):
      return CallEnterCellAndLoadSubCellsWithinTraverse;
    case static_cast<int>(CallLeaveCellAndStoreSubCellsWithinTraverse):
      return CallLeaveCellAndStoreSubCellsWithinTraverse;
    case static_cast<int>(CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid):
      return CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid;
    case static_cast<int>(CallTouchFirstTimeOnRegularStationaryGrid):
      return CallTouchFirstTimeOnRegularStationaryGrid;
    case static_cast<int>(CallTouchLastTimeOnRegularStationaryGrid):
      return CallTouchLastTimeOnRegularStationaryGrid;
    case static_cast<int>(CallEnterCellOnRegularStationaryGrid):
      return CallEnterCellOnRegularStationaryGrid;
    case static_cast<int>(CallLeaveCellOnRegularStationaryGrid):
      return CallLeaveCellOnRegularStationaryGrid;
    case static_cast<int>(PipelineAscendTask):
      return PipelineAscendTask;
    case static_cast<int>(PipelineDescendTask):
      return PipelineDescendTask;
    case static_cast<int>(SplitLoadVerticesTaskOnRegularStationaryGrid):
      return SplitLoadVerticesTaskOnRegularStationaryGrid;
    case static_cast<int>(SplitStoreVerticesTaskOnRegularStationaryGrid):
      return SplitStoreVerticesTaskOnRegularStationaryGrid;
    case static_cast<int>(AscendOnRegularStationaryGrid):
      return AscendOnRegularStationaryGrid;
    case static_cast<int>(DescendOnRegularStationaryGrid):
      return DescendOnRegularStationaryGrid;

    case static_cast<int>(UserDefined0):
      return UserDefined0;
    case static_cast<int>(UserDefined1):
      return UserDefined1;
    case static_cast<int>(UserDefined2):
      return UserDefined2;
    case static_cast<int>(UserDefined3):
      return UserDefined3;
    case static_cast<int>(UserDefined4):
      return UserDefined4;
    case static_cast<int>(UserDefined5):
      return UserDefined5;
    case static_cast<int>(UserDefined6):
      return UserDefined6;
    case static_cast<int>(UserDefined7):
      return UserDefined7;
    case static_cast<int>(UserDefined8):
      return UserDefined8;
    case static_cast<int>(UserDefined9):
      return UserDefined9;
    case static_cast<int>(UserDefined10):
      return UserDefined10;
    case static_cast<int>(UserDefined11):
      return UserDefined11;
    case static_cast<int>(UserDefined12):
      return UserDefined12;

    case static_cast<int>(NumberOfDifferentMethodsCalling):
      return NumberOfDifferentMethodsCalling;
  }

  return NumberOfDifferentMethodsCalling;
}
