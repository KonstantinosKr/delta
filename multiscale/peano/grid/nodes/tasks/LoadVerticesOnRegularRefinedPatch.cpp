#include "peano/grid/nodes/tasks/LoadVerticesOnRegularRefinedPatch.h"



bool peano::grid::nodes::tasks::isSubmanifoldVertexAdjacentToDeployedSubtree(
  tarch::la::Vector<DIMENSIONS,int>   vertexPosition,
  int                                 numberOfCellsPerDimension,
  const std::bitset<THREE_POWER_D>    forkedSubtree
) {
  bool result = tarch::la::oneEquals(vertexPosition,0) || tarch::la::oneEquals(vertexPosition,numberOfCellsPerDimension);

  if (result) {
    tarch::la::Vector<DIMENSIONS,int> a;
    tarch::la::Vector<DIMENSIONS,int> b;
    for (int d=0; d<DIMENSIONS; d++) {
      a(d) = (vertexPosition(d)-1) / (numberOfCellsPerDimension/3);
      b(d) = (vertexPosition(d)  ) / (numberOfCellsPerDimension/3);
      a(d) = a(d)<0 ? 0 : a(d);
      b(d) = b(d)>2 ? 2 : b(d);
    }

    result = false;
    dfor(k,b-a+1) {
      const tarch::la::Vector<DIMENSIONS,int> p = a + k;
      result |= forkedSubtree[peano::utils::dLinearised(p,3)];
    }
  }

  return result;
}
