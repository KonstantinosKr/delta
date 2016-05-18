// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_CELL_LOCAL_PEANO_CURVE_TEST_H_
#define _PEANO_GRID_TESTS_CELL_LOCAL_PEANO_CURVE_TEST_H_


#include "tarch/tests/TestCase.h"
#include "peano/grid/tests/records/TestCell.h"
#include "peano/grid/Cell.h"


namespace peano {
  namespace grid {
    namespace tests {
      class CellLocalPeanoCurveTest;
    }
  }
}


/**
 *
 * @author Tobias Weinzierl
 */
class peano::grid::tests::CellLocalPeanoCurveTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log _log;

    typedef peano::grid::Cell<peano::grid::tests::records::TestCell> TestCell;

    /**
     *
     * @image html peano/grid/tests/RefinementTest_testRootNodeRefinement.png
     */
    void test2DCreateReadVertexSequenceForP0P0P1P2();
    void test2DCreateReadVertexSequenceForM1M2P2P3();
    void test2DCreateReadVertexSequenceForM1P0P0P1();
    void test2DCreateReadVertexSequenceForP1M1O0P2();

    /**
     * A cell (+3,+2,+1,-1) does not exist originally. It can only be
     * reconstructed by an inverted traversal. So, we construct
     * (-3,-2,-1,+1) and analyse the reverse.
     */
    void test2DCreateReadVertexSequenceForP3P2P1M1();

    void test2DCreateReadVertexSequenceForP3M1P1P2();
    void test2DCreateReadVertexSequenceForM1P3P1P2();

    void test2DCreateWriteVertexSequenceForM1P0P1P2();

    /**
     * Test
     *
 09:12:23 peano::kernel::spacetreegrid::nodes::Leaf::store(...)
   out:
     state:(updateMax:8.71601e-153,updateL2:6.01388e-154,updateEukledian:3.33313e-28,minMeshWidth:[0.111111,0.111111],maxMeshWidth:[0.333333,0.333333],numberOfInnerVertices:0,numberOfBoundaryVertices:20,numberOfOuterVertices:64,numberOfInnerCells:0,numberOfOuterCells:88,maxLevel:3,hasRefined:0,hasTriggeredRefinementForNextIteration:0,hasCoarsened:0,hasTriggeredCoarseningForNextIteration:0,hasChangedVertexOrCellState:0,isTraversalInverted:1),
     fineGridCell:(isInside:0,state:Leaf,level:3,evenFlags:[0,1],accessNumber:[1,-1,-2,2]) (file:/work_fast/weinzier/workspace/peano/src/peano/kernel/spacetreegrid/nodes/Leaf.cpph,line:239)
     *
     */
    void test2DCreateWriteVertexSequenceForP1M1M2P2();

    void test3DCreateReadVertexSequenceForM2P4M1P1P2P3();

    void test4D();

  public:
    CellLocalPeanoCurveTest();

    virtual ~CellLocalPeanoCurveTest();

    virtual void run();

    void virtual setUp();
};

#endif
