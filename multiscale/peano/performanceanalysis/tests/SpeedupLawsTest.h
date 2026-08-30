// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef PEANO_PERFORMANCE_ANALYSIS_TESTS_SPEEDUP_LAWS_H_
#define PEANO_PERFORMANCE_ANALYSIS_TESTS_SPEEDUP_LAWS_H_

#include "peano/utils/Globals.h"
#include "tarch/tests/TestCase.h"

namespace peano {
  namespace performanceanalysis {
    namespace tests {
      class SpeedupLawsTest;
    }
  }
}

class peano::performanceanalysis::tests::SpeedupLawsTest: public tarch::tests::TestCase {
  private:
   /**
    * Test whether an exact solution is reconstructed once many exact values
    * are given.
    */
    void testAmdahl1();

    /**
     * Test whether an exact solution is reconstructed once we use two, three or
     * four exact values. We basically check whether the difference to the real
     * value becomes smaller.
     */
    void testAmdahl2();

    /**
     * The weird case, where the problem is undetermined.
     */
    void testAmdahl3();

    void testGetOptimalNumberOfThreadsWithOneRank();
    void testGetOptimalNumberOfThreadsWithTwoRanks();

    /**
     * A real-world data set
     *
     *
 c[0]=2 (t_1=0.00515314,f=0.5,s=0)
 c[1]=2 (t_1=0,f=1,s=1)
 c[2]=2 (t_1=0.00516363,f=0.5,s=0)
 c[3]=2 (t_1=0.00514758,f=0.5,s=0)
 c[4]=2 (t_1=0.00506833,f=0.5,s=0)
 c[5]=2 (t_1=0.00516871,f=0.5,s=0)
     *
     */
    void testGetOptimalNumberOfThreadsWithSixRanks();

   double getAmdahlPrediction(double t1, double f, double c);
 public:
   SpeedupLawsTest();
   virtual ~SpeedupLawsTest();

   virtual void run();
};

#endif
