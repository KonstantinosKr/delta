// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_AUTOTUNING_METHOD_TRACE_H_
#define _PEANO_DATA_TRAVERSAL_AUTOTUNING_METHOD_TRACE_H_


#include <string>


namespace peano {
  namespace datatraversal {
    namespace autotuning {
      enum class MethodTrace {
        LoadVertices                                     = 0,
        LoadVerticesOnRegularStationaryGrid              = 1, // not unrolled
        LoadVerticesOnIrregularStationaryGrid            = 2,
        StoreVertices                                    = 3,
        StoreVerticesOnRegularStationaryGrid             = 4, // not unrolled
        StoreVerticesOnIrregularStationaryGrid           = 5,
        CallEnterCellAndLoadSubCellsWithinTraverse       = 6,
        CallLeaveCellAndStoreSubCellsWithinTraverse      = 7,
        CallEnterCellAndInitialiseEnumeratorsOnRegularStationaryGrid = 8,
        CallTouchFirstTimeOnRegularStationaryGrid        = 9,
        CallTouchLastTimeOnRegularStationaryGrid         = 10,
        CallEnterCellOnRegularStationaryGrid             = 11,
        CallLeaveCellOnRegularStationaryGrid             = 12,
        PipelineAscendTask                               = 13,
        PipelineDescendTask                              = 14,
        SplitLoadVerticesTaskOnRegularStationaryGrid     = 15,
        SplitStoreVerticesTaskOnRegularStationaryGrid    = 16,
        AscendOnRegularStationaryGrid                    = 17,
        DescendOnRegularStationaryGrid                   = 18,

        HoldPersistentRegularSubgrid                     = 19,

        DecomposeAscendIntoMultilevelTasks               = 20,
        DecomposeDescendIntoMultilevelTasks              = 21,

        UserDefined3  = 22,
        UserDefined4  = 23,
        UserDefined5  = 24,
        UserDefined6  = 25,
        UserDefined7  = 26,
        UserDefined8  = 27,
        UserDefined9  = 28,
        UserDefined10 = 29,
        UserDefined11 = 30,
        UserDefined12 = 31,
        UserDefined13 = 32,
        UserDefined14 = 33,
        UserDefined15 = 34,
        UserDefined16 = 35,
        UserDefined17 = 36,
        UserDefined18 = 37,
        UserDefined19 = 38,
        UserDefined20 = 39,
        UserDefined21 = 40,
        UserDefined22 = 41,
        UserDefined23 = 42,
        UserDefined24 = 43,
        UserDefined25 = 44,
        UserDefined26 = 45,
        UserDefined27 = 46,
        UserDefined28 = 47,
        UserDefined29 = 48,
        UserDefined30 = 49,

        UserDefined0  = 50,
        UserDefined1  = 51,
        UserDefined2  = 52,

        NumberOfDifferentMethodsCalling                  = 53
      };

      std::string toString( const MethodTrace& methodTrace );
      MethodTrace toMethodTrace(int value);
      MethodTrace toMethodTrace(const std::string&  identifier);
    }
  }
}



#endif
