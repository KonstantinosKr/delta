// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_TESTS_DFORRANGE_TEST_H_
#define _PEANO_DATA_TRAVERSAL_TESTS_DFORRANGE_TEST_H_


#include "tarch/tests/TestCase.h"

namespace tarch {
    namespace multicore {
      namespace tests {
        class dForRangeTest;
      }
    }
}


class tarch::multicore::tests::dForRangeTest: public tarch::tests::TestCase {
  private:
	/**
	 * The last digit is the grain size.
	 */
    void test2D10x10Range1();
    void test2D10x10Range12();
    void test2D10x10Range23();
    void test2D10x10Range40();
    void test2D10x10Range80();

    void test2DgetMinimalRanges();

    void test2Dg14x14WithGrainSize1AndOffset1();

    void test2Dg14x14WithGrainSize1AndOffset1AndColouring();
  public:
    dForRangeTest();
    virtual ~dForRangeTest();
    virtual void run();
    virtual void setUp();
};


#endif
