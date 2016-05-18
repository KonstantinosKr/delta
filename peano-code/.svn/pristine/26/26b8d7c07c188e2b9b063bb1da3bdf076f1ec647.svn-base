#include "peano/parallel/AdjacencyList.h"

#include "tarch/Assertions.h"
#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"


void peano::parallel::getMergedAdjacencyLists(
  const tarch::la::Vector<TWO_POWER_D,int>&   ownListBeforeMerge,
  tarch::la::Vector<TWO_POWER_D,int>&         ownListAfterMerge,
  const tarch::la::Vector<TWO_POWER_D,int>&   neighbourList,
  int                                         neighbourRank
) {
  static tarch::logging::Log _log( "peano::kernel::parallel" );
  logTraceInWith5Arguments( "getMergedAdjacencyLists(...)", ownListBeforeMerge, ownListAfterMerge, neighbourList, neighbourRank, tarch::parallel::Node::getInstance().getRank() );

  for (int i=0; i<NUMBER_OF_VERTICES_PER_ELEMENT; i++) {

    bool neighbourIsResponsible = ownListBeforeMerge(i) == neighbourRank;

    if (neighbourIsResponsible) {
      // if neighbourList(i) is not equal to neighbour's rank, the neighbour
      // has forked or joined. You may not call this operation, if the the
      // neighbour is joining.
      ownListAfterMerge(i) = neighbourList(i);
    }
  }

  logTraceOutWith1Argument( "getMergedAdjacencyLists(...)", ownListAfterMerge );
}


tarch::la::Vector<TWO_POWER_D,int> peano::parallel::getAdjacencyListFromCoarseGrid(
  const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>&  coarseGridVerticesAdjacencyLists,
  const tarch::la::Vector<DIMENSIONS,int>&                     coordinates
) {
  static tarch::logging::Log _log( "peano::kernel::parallel" );
  logTraceInWith2Arguments( "getAdjacencyListFromCoarseGrid(...)", coarseGridVerticesAdjacencyLists, coordinates );

  tarch::la::Vector<TWO_POWER_D,int>  result;
  tarch::la::Vector<DIMENSIONS,int>   fromCoarseGridVertex;
  tarch::la::Vector<DIMENSIONS,int>   coarseGridVertexAdjacencyFlag;

  dfor2(k)
    for (int d=0; d<DIMENSIONS; d++) {
      if (coordinates(d)==0) {
        fromCoarseGridVertex(d)          = 0;
        coarseGridVertexAdjacencyFlag(d) = k(d);
      }
      else if (coordinates(d)==3) {
        fromCoarseGridVertex(d)          = 1;
        coarseGridVertexAdjacencyFlag(d) = k(d);
      }
      else if (k(d)==0) {
        fromCoarseGridVertex(d)          = 0;
        coarseGridVertexAdjacencyFlag(d) = 1;
      }
      else {
        fromCoarseGridVertex(d)          = 1;
        coarseGridVertexAdjacencyFlag(d) = 0;
      }
    }
    const int coarseGridEntry = utils::dLinearised(fromCoarseGridVertex,2) * TWO_POWER_D + utils::dLinearised(coarseGridVertexAdjacencyFlag,2);
    result(kScalar) = coarseGridVerticesAdjacencyLists(coarseGridEntry);
  enddforx

  logTraceOutWith1Argument( "getAdjacencyListFromCoarseGrid(...)", result );

  return result;
}

