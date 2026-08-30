// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_UNROLLED_LEVEL_ENUMERATOR_TEST_H_
#define _PEANO_GRID_TESTS_UNROLLED_LEVEL_ENUMERATOR_TEST_H_


#include "tarch/tests/TestCase.h"
#include "peano/grid/tests/records/TestCell.h"
#include "peano/grid/Cell.h"


namespace peano {
  namespace grid {
    namespace tests {
      class UnrolledLevelEnumeratorTest;
    }
  }
}


class peano::grid::tests::UnrolledLevelEnumeratorTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log   _log;


    /**
     * I had the following error:
     *
\code
l=3,
p=841,
v=(numberOfAdjacentEraseTriggers:0,isHangingNode:0,refinementControl:Unrefined,adjacentCellsHeight:0,adjacentCellsHeightOfPreviousIteration:0,numberOfAdjacentRefinedCells:0,insideOutsideDomain:Inside,x:[0.037037,0.0740741,0.037037],level:4),
e=(domain-offset:[0,0,0],discrete-offset:[0,0,0],cell-size:[0.037037,0.037037,0.037037],level:4,adj-flags:0,cells-per-axis:27,vertices-per-axis:28)[type=UnrolledLevelEnumerator]
\endcode
     *
     * The access on this enumerator due to getVertexPosition() with argument 841 did
     * core dump. Instead it should return the position of the vertex v.
     */
    void test3DGetVertexPosition();
  public:
    UnrolledLevelEnumeratorTest();

    virtual ~UnrolledLevelEnumeratorTest();

    virtual void run();

    void virtual setUp();
};

#endif
