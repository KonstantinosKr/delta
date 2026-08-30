// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_FORK_REGULAR_GRID_TEST_H_
#define _PEANO_GRID_TESTS_FORK_REGULAR_GRID_TEST_H_


#include "tarch/tests/TestCase.h"
#include "peano/grid/tests/records/TestCell.h"
#include "peano/grid/Cell.h"


namespace peano {
  namespace grid {
    namespace tests {
      class ForkRegularTreeTest;
    }
  }
}


/**
 *
 * @author Tobias Weinzierl
 */
class peano::grid::tests::ForkRegularTreeTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log _log;

    void testMayForkLoadOrStoreVertexTaskOnRegularSubtree();
  public:
    ForkRegularTreeTest();

    virtual ~ForkRegularTreeTest();

    virtual void run();
};

#endif
