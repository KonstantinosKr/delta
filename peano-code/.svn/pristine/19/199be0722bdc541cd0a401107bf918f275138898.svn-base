#include "peano/grid/RegularGridContainer.h"


bool peano::grid::mayForkLoadOrStoreVertexTaskOnRegularSubtree(
  const int   currentLevel,
  const bool  isParentCellAtPatchBoundary,
  const int   maxLevelToFork,
  const int   coarsestLevelOfThisTask,
  const int   expectedNumberOfLoadsOrStores
) {
  static tarch::logging::Log _log("peano::grid");

  const bool result = !isParentCellAtPatchBoundary &&
         expectedNumberOfLoadsOrStores>0 &&
         maxLevelToFork>=currentLevel &&
         currentLevel>coarsestLevelOfThisTask;

  logDebug(
    "mayForkLoadOrStoreVertexTaskOnRegularSubtree(...)",
    "currentLevel=" << currentLevel <<
    ",isParentCellAtPatchBoundary=" << isParentCellAtPatchBoundary <<
    ",maxLevelToFork=" << maxLevelToFork <<
    ",coarsestLevelOfThisTask=" << coarsestLevelOfThisTask <<
    ",expectedNumberOfLoadsOrStores=" << expectedNumberOfLoadsOrStores <<
    ",result=" << result
  );

  return result;
}

