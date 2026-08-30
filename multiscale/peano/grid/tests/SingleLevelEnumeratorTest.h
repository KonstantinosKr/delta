// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_SINGLE_LEVEL_ENUMERATOR_TEST_H_
#define _PEANO_GRID_TESTS_SINGLE_LEVEL_ENUMERATOR_TEST_H_


#include "tarch/tests/TestCase.h"
#include "peano/grid/tests/records/TestCell.h"
#include "peano/grid/Cell.h"


namespace peano {
  namespace grid {
    namespace tests {
      class SingleLevelEnumeratorTest;
    }
  }
}


class peano::grid::tests::SingleLevelEnumeratorTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log   _log;

    void test2DLineariseCellIndex();
    void test2DLineariseVertexIndex();
    void testFunctorWithZeroArgument();
    void testGetVertexPositionOnCoarserLevel();
    void test2DFunctor();


    /**
     * I had the following error:
     *
\code
_fineGridVertices[positionInVertexArray].getLevel()=3 _fineGridVerticesEnumerator.getLevel()=2
1st argument positionInVertexArray: 61
2nd argument _fineGridVerticesEnumerator.toString(): (domain-offset:[1,0,0],discrete-offset:[0,2,2],cell-size:[0.333333,0.333333,0.333333],level:2,adj-flags:-1)[type=SingleLevelEnumerator]
3rd argument _fineGridVertices[positionInVertexArray].toString(): (numberOfAdjacentEraseTriggers:4,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:1,adjacentCellsHeightOfPreviousIteration:1,numberOfAdjacentRefinedCells:4,insideOutsideDomain:Boundary,x:[0.555556,0.333333,-5.55112e-17],level:3)
4th argument _coarseGridVertices[_coarseGridVerticesEnumerator(0)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,0,0],level:1)
5th argument _coarseGridVertices[_coarseGridVerticesEnumerator(1)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:1,refinementControl:Unrefined,adjacentCellsHeight:-1,adjacentCellsHeightOfPreviousIteration:-1,numberOfAdjacentRefinedCells:4,insideOutsideDomain:Outside,x:[2,0,0],level:1)
6th argument _coarseGridVertices[_coarseGridVerticesEnumerator(2)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,1,0],level:1)
7th argument _coarseGridVertices[_coarseGridVerticesEnumerator(3)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:1,refinementControl:Unrefined,adjacentCellsHeight:-1,adjacentCellsHeightOfPreviousIteration:-1,numberOfAdjacentRefinedCells:4,insideOutsideDomain:Outside,x:[2,1,0],level:1)
8th argument _coarseGridVertices[_coarseGridVerticesEnumerator(4)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,0,1],level:1)
9th argument _coarseGridVertices[_coarseGridVerticesEnumerator(5)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:1,refinementControl:Unrefined,adjacentCellsHeight:-1,adjacentCellsHeightOfPreviousIteration:-1,numberOfAdjacentRefinedCells:4,insideOutsideDomain:Outside,x:[2,0,1],level:1)
10th argument _coarseGridVertices[_coarseGridVerticesEnumerator(6)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,1,1],level:1)
11th argument _coarseGridVertices[_coarseGridVerticesEnumerator(7)].toString(): (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,1,1],level:1)
12th argument _fineGridVertices[positionInVertexArray].getX(): (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,1,1],level:1)
13th argument _fineGridVerticesEnumerator.getVertexPosition(positionInLocalCell): (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,1,1],level:1)
14th argument stackNumber: (numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Refined,adjacentCellsHeight:3,adjacentCellsHeightOfPreviousIteration:3,numberOfAdjacentRefinedCells:8,insideOutsideDomain:Boundary,x:[1,1,1],level:1)
\endcode
     *
     * Obviously, the enumerator-based access returns the same vertex for index 6 and 7.
     */
    void test3DFunctor();
  public:
    SingleLevelEnumeratorTest();

    virtual ~SingleLevelEnumeratorTest();

    virtual void run();

    void virtual setUp();
};

#endif
