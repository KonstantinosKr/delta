#include "peano/datatraversal/autotuning/MethodTrace.h"


#include "tarch/Assertions.h"
#include <sstream>


peano::datatraversal::autotuning::MethodTrace peano::datatraversal::autotuning::toMethodTrace(const std::string&  identifier) {
  for (int i=0; i<(int)(MethodTrace::NumberOfDifferentMethodsCalling); i++) {
    if ( toString(toMethodTrace(i)).compare(identifier)==0 ) return toMethodTrace(i);
  }

  return MethodTrace::NumberOfDifferentMethodsCalling;
}


std::string peano::datatraversal::autotuning::toString( const MethodTrace& methodTrace ) {
  switch (methodTrace) {
    case MethodTrace::LoadVertices:
      return "load-vertices";
    case MethodTrace::LoadVerticesOnRegularStationaryGrid:
      return "load-vertices-on-regular-stationary-grid";
    case MethodTrace::LoadVerticesOnIrregularStationaryGrid:
      return "load-vertices-on-irregular-stationary-grid";
    case MethodTrace::StoreVertices:
      return "store-vertices";
    case MethodTrace::StoreVerticesOnIrregularStationaryGrid:
      return "store-vertices-on-irregular-stationary-grid";
    case MethodTrace::StoreVerticesOnRegularStationaryGrid:
      return "store-vertices-on-regular-stationary-grid";
    case MethodTrace::CallEnterCellAndLoadSubCellsWithinTraverse:
      return "enter-cell-and-load-sub-cells";
    case MethodTrace::CallLeaveCellAndStoreSubCellsWithinTraverse:
      return "leave-cell-and-store-sub-cells";
    case MethodTrace::CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid:
      return "call-enter-cell-and-initialise-enumerators-on-regular-stationary-grid";
    case MethodTrace::CallTouchFirstTimeOnRegularStationaryGrid:
      return "call-touch-first-time-on-regular-stationary-grid";
    case MethodTrace::CallTouchLastTimeOnRegularStationaryGrid:
      return "call-touch-last-time-on-regular-stationary-grid";
    case MethodTrace::CallEnterCellOnRegularStationaryGrid:
      return "call-enter-cell-on-regular-stationary-grid";
    case MethodTrace::CallLeaveCellOnRegularStationaryGrid:
      return "call-leave-cell-on-regular-stationary-grid";
    case MethodTrace::PipelineAscendTask:
      return "pipeline-ascend-task";
    case MethodTrace::PipelineDescendTask:
      return "pipeline-descend-task";
    case MethodTrace::SplitLoadVerticesTaskOnRegularStationaryGrid:
      return "split-load-vertices-task-on-regular-stationary-grid";
    case MethodTrace::SplitStoreVerticesTaskOnRegularStationaryGrid:
      return "split-store-vertices-task-on-regular-stationary-grid";
    case MethodTrace::AscendOnRegularStationaryGrid:
      return "ascend-on-regular-stationary-grid";
    case MethodTrace::DescendOnRegularStationaryGrid:
      return "descend-on-regular-stationary-grid";

    case MethodTrace::HoldPersistentRegularSubgrid:
      return "hold-persistent-regular-subgrid";

    case MethodTrace::DecomposeAscendIntoMultilevelTasks:
      return "decompose-ascend-into-multilevel-task";
    case MethodTrace::DecomposeDescendIntoMultilevelTasks:
      return "decompose-descend-into-multilevel-task";


    case MethodTrace::UserDefined0:
      return "user-defined-0";
    case MethodTrace::UserDefined1:
      return "user-defined-1";
    case MethodTrace::UserDefined2:
      return "user-defined-2";
    case MethodTrace::UserDefined3:
      return "user-defined-3";
    case MethodTrace::UserDefined4:
      return "user-defined-4";
    case MethodTrace::UserDefined5:
      return "user-defined-5";
    case MethodTrace::UserDefined6:
      return "user-defined-6";
    case MethodTrace::UserDefined7:
      return "user-defined-7";
    case MethodTrace::UserDefined8:
      return "user-defined-8";
    case MethodTrace::UserDefined9:
      return "user-defined-9";
    case MethodTrace::UserDefined10:
      return "user-defined-10";
    case MethodTrace::UserDefined11:
      return "user-defined-11";
    case MethodTrace::UserDefined12:
      return "user-defined-12";
    case MethodTrace::UserDefined13:
      return "user-defined-13";
    case MethodTrace::UserDefined14:
      return "user-defined-14";
    case MethodTrace::UserDefined15:
      return "user-defined-15";
    case MethodTrace::UserDefined16:
      return "user-defined-16";
    case MethodTrace::UserDefined17:
      return "user-defined-17";
    case MethodTrace::UserDefined18:
      return "user-defined-18";
    case MethodTrace::UserDefined19:
      return "user-defined-19";

    case MethodTrace::UserDefined20:
      return "user-defined-20";
    case MethodTrace::UserDefined21:
      return "user-defined-21";
    case MethodTrace::UserDefined22:
      return "user-defined-22";
    case MethodTrace::UserDefined23:
      return "user-defined-23";
    case MethodTrace::UserDefined24:
      return "user-defined-24";
    case MethodTrace::UserDefined25:
      return "user-defined-25";
    case MethodTrace::UserDefined26:
      return "user-defined-26";
    case MethodTrace::UserDefined27:
      return "user-defined-27";
    case MethodTrace::UserDefined28:
      return "user-defined-28";
    case MethodTrace::UserDefined29:
      return "user-defined-29";
    case MethodTrace::UserDefined30:
      return "user-defined-30";

    case MethodTrace::NumberOfDifferentMethodsCalling:
      std::ostringstream msg;
      msg << "<error(" << (int)(methodTrace) << ")>";
      return msg.str();
  }

  return "<error>";
}


peano::datatraversal::autotuning::MethodTrace peano::datatraversal::autotuning::toMethodTrace(int value) {
  assertion( value>=0 );
  assertion( value<(int)(MethodTrace::NumberOfDifferentMethodsCalling) );
  switch (value) {
    case static_cast<int>(MethodTrace::LoadVertices):
      return MethodTrace::LoadVertices;
    case static_cast<int>(MethodTrace::LoadVerticesOnRegularStationaryGrid):
      return MethodTrace::LoadVerticesOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::LoadVerticesOnIrregularStationaryGrid):
      return MethodTrace::LoadVerticesOnIrregularStationaryGrid;
    case static_cast<int>(MethodTrace::StoreVertices):
      return MethodTrace::StoreVertices;
    case static_cast<int>(MethodTrace::StoreVerticesOnRegularStationaryGrid):
      return MethodTrace::StoreVerticesOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::StoreVerticesOnIrregularStationaryGrid):
      return MethodTrace::StoreVerticesOnIrregularStationaryGrid;
    case static_cast<int>(MethodTrace::CallEnterCellAndLoadSubCellsWithinTraverse):
      return MethodTrace::CallEnterCellAndLoadSubCellsWithinTraverse;
    case static_cast<int>(MethodTrace::CallLeaveCellAndStoreSubCellsWithinTraverse):
      return MethodTrace::CallLeaveCellAndStoreSubCellsWithinTraverse;
    case static_cast<int>(MethodTrace::CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid):
      return MethodTrace::CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::CallTouchFirstTimeOnRegularStationaryGrid):
      return MethodTrace::CallTouchFirstTimeOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::CallTouchLastTimeOnRegularStationaryGrid):
      return MethodTrace::CallTouchLastTimeOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::CallEnterCellOnRegularStationaryGrid):
      return MethodTrace::CallEnterCellOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::CallLeaveCellOnRegularStationaryGrid):
      return MethodTrace::CallLeaveCellOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::PipelineAscendTask):
      return MethodTrace::PipelineAscendTask;
    case static_cast<int>(MethodTrace::PipelineDescendTask):
      return MethodTrace::PipelineDescendTask;
    case static_cast<int>(MethodTrace::SplitLoadVerticesTaskOnRegularStationaryGrid):
      return MethodTrace::SplitLoadVerticesTaskOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::SplitStoreVerticesTaskOnRegularStationaryGrid):
      return MethodTrace::SplitStoreVerticesTaskOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::AscendOnRegularStationaryGrid):
      return MethodTrace::AscendOnRegularStationaryGrid;
    case static_cast<int>(MethodTrace::DescendOnRegularStationaryGrid):
      return MethodTrace::DescendOnRegularStationaryGrid;

    case static_cast<int>(MethodTrace::HoldPersistentRegularSubgrid):
      return MethodTrace::HoldPersistentRegularSubgrid;

    case static_cast<int>(MethodTrace::DecomposeAscendIntoMultilevelTasks):
      return MethodTrace::DecomposeAscendIntoMultilevelTasks;
    case static_cast<int>(MethodTrace::DecomposeDescendIntoMultilevelTasks):
      return MethodTrace::DecomposeDescendIntoMultilevelTasks;

    case static_cast<int>(MethodTrace::UserDefined0):
      return MethodTrace::UserDefined0;
    case static_cast<int>(MethodTrace::UserDefined1):
      return MethodTrace::UserDefined1;
    case static_cast<int>(MethodTrace::UserDefined2):
      return MethodTrace::UserDefined2;
    case static_cast<int>(MethodTrace::UserDefined3):
      return MethodTrace::UserDefined3;
    case static_cast<int>(MethodTrace::UserDefined4):
      return MethodTrace::UserDefined4;
    case static_cast<int>(MethodTrace::UserDefined5):
      return MethodTrace::UserDefined5;
    case static_cast<int>(MethodTrace::UserDefined6):
      return MethodTrace::UserDefined6;
    case static_cast<int>(MethodTrace::UserDefined7):
      return MethodTrace::UserDefined7;
    case static_cast<int>(MethodTrace::UserDefined8):
      return MethodTrace::UserDefined8;
    case static_cast<int>(MethodTrace::UserDefined9):
      return MethodTrace::UserDefined9;

    case static_cast<int>(MethodTrace::UserDefined10):
      return MethodTrace::UserDefined10;
    case static_cast<int>(MethodTrace::UserDefined11):
      return MethodTrace::UserDefined11;
    case static_cast<int>(MethodTrace::UserDefined12):
      return MethodTrace::UserDefined12;
    case static_cast<int>(MethodTrace::UserDefined13):
      return MethodTrace::UserDefined13;
    case static_cast<int>(MethodTrace::UserDefined14):
      return MethodTrace::UserDefined14;
    case static_cast<int>(MethodTrace::UserDefined15):
      return MethodTrace::UserDefined15;
    case static_cast<int>(MethodTrace::UserDefined16):
      return MethodTrace::UserDefined16;
    case static_cast<int>(MethodTrace::UserDefined17):
      return MethodTrace::UserDefined17;
    case static_cast<int>(MethodTrace::UserDefined18):
      return MethodTrace::UserDefined18;
    case static_cast<int>(MethodTrace::UserDefined19):
      return MethodTrace::UserDefined19;

    case static_cast<int>(MethodTrace::UserDefined20):
      return MethodTrace::UserDefined20;
    case static_cast<int>(MethodTrace::UserDefined21):
      return MethodTrace::UserDefined21;
    case static_cast<int>(MethodTrace::UserDefined22):
      return MethodTrace::UserDefined22;
    case static_cast<int>(MethodTrace::UserDefined23):
      return MethodTrace::UserDefined23;
    case static_cast<int>(MethodTrace::UserDefined24):
      return MethodTrace::UserDefined24;
    case static_cast<int>(MethodTrace::UserDefined25):
      return MethodTrace::UserDefined25;
    case static_cast<int>(MethodTrace::UserDefined26):
      return MethodTrace::UserDefined26;
    case static_cast<int>(MethodTrace::UserDefined27):
      return MethodTrace::UserDefined27;
    case static_cast<int>(MethodTrace::UserDefined28):
      return MethodTrace::UserDefined28;
    case static_cast<int>(MethodTrace::UserDefined29):
      return MethodTrace::UserDefined29;

    case static_cast<int>(MethodTrace::UserDefined30):
      return MethodTrace::UserDefined30;

    case static_cast<int>(MethodTrace::NumberOfDifferentMethodsCalling):
      return MethodTrace::NumberOfDifferentMethodsCalling;
  }

  return MethodTrace::NumberOfDifferentMethodsCalling;
}
