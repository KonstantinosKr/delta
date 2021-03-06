// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef _DEM_TESTS_TEST_CASE_H_
#define _DEM_TESTS_TEST_CASE_H_
 


#include "tarch/tests/TestCase.h"


namespace dem {
    namespace tests {
      class TestCase;
    } 
}
 

/**
 * This is just a default test case that demonstrated how to write unit tests 
 * in Peano. Feel free to rename, remove, or duplicate it. 
 */ 
class dem::tests::TestCase: public tarch::tests::TestCase {
  private:
  public: 
    TestCase(); 
    virtual ~TestCase();
     
    virtual void run();
};


#endif
