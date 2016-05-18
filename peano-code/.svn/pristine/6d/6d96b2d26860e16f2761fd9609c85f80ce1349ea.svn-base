// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_SET_COUNTER_TEST_H_
#define _PEANO_GRID_TESTS_SET_COUNTER_TEST_H_


#include "tarch/tests/TestCase.h"
#include "peano/grid/tests/records/TestVertex.h"
#include "peano/grid/Vertex.h"




namespace peano {
  namespace grid {
    namespace tests {
      class SetCounterTest;
    }
  }
}


class peano::grid::tests::SetCounterTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log _log;

    typedef peano::grid::Vertex<peano::grid::tests::records::TestVertex> TestVertex;

    /**
     * Test whether (3,1) is a boundary point. It should be, but in my test
     * cases it is not recognised as a boundary point. Probably there's an
     * error in the LA component if this test fails. If it doesn't fail but
     * the other tests fail, the error is not due to LA.
     */
    void isOnBoundaryTest();

    void test2DSpacetreeConstructionFirstLevel();

    /**
     * @imgage html SetCounterTest_testAPosterioriRefinement2d.png
     */
    void testAPosterioriRefinement2d();

    /**
     * Old non-optimised version of the fundamental operation.
     */
    int analyseFineGridVertex(
      bool oneFatherCarriesDeleteFlag,
      bool oneFatherCarriesRefiningFlag,
      bool oneFatherCarriesRefinedFlag
    ) const;

    void testOptimisedAnalysis();

    void testRefinementDueToJoinThoughWorkerIsAlreadyErasing0();
    void testRefinementDueToJoinThoughWorkerIsAlreadyErasing1();

    void test3DLevel1();
  public:
    SetCounterTest();

    virtual ~SetCounterTest();

    virtual void run();

    void virtual setUp();
};

#endif
