// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TESTS_REFINEMENT_TEST_H_
#define _PEANO_GRID_TESTS_REFINEMENT_TEST_H_


#include "tarch/tests/TestCase.h"
#include "peano/grid/tests/records/TestCell.h"
#include "peano/grid/Cell.h"


namespace peano {
  namespace grid {
    namespace tests {
      class RefinementTest;
    }
  }
}


/**
 *
 * @author Tobias Weinzierl
 */
class peano::grid::tests::RefinementTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log _log;


  	typedef peano::grid::Cell<peano::grid::tests::records::TestCell> TestCell;


    /**
     *
     * @image html peano/grid/tests/RefinementTest_testRootNodeRefinement.png
     */
	  void testRootNodeRefinementCells();

    void test2DCellM1M2P0P0();
  public:
	  RefinementTest();

	  virtual ~RefinementTest();

    virtual void run();
};

#endif
