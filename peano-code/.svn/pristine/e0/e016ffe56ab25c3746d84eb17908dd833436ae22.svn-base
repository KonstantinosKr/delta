// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_UNROLLED_ASCEND_DESCEND_LEVEL_ENUMERATOR_TEST_H_
#define _PEANO_GRID_TESTS_UNROLLED_ASCEND_DESCEND_LEVEL_ENUMERATOR_TEST_H_


#include "tarch/tests/TestCase.h"
#include "tarch/logging/Log.h"


namespace peano {
  namespace grid {
    namespace tests {
      class UnrolledAscendDescendLevelEnumeratorTest;
    }
  }
}


class peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log _log;

    /**
     * Test following setting
     * coarse enumerator: (domain-offset:[0.444444,0.666667],discrete-offset:[0,0],cell-size:[0.111111,0.111111],level:3,adj-flags:4,cells-per-axis:1,vertices-per-axis:2)[type=UnrolledLevelEnumerator]
     * fine enumerator:   (domain-offset:[0.444444,0.666667],discrete-offset:[0,0],cell-size:[0.037037,0.037037],level:4,adj-flags:3,cells-per-axis:3,vertices-per-axis:4)[type=UnrolledAscendDescendLevelEnumerator]
     */
    void testLevel1();


    /**
     * Same as testLevel1 but this time we assume that the tree has depth 2 and study level 2 instead of 0 and 1.
     */
    void testLevel2();
  public:
    UnrolledAscendDescendLevelEnumeratorTest();

    virtual ~UnrolledAscendDescendLevelEnumeratorTest();

    virtual void run();

    void virtual setUp();
};

#endif
