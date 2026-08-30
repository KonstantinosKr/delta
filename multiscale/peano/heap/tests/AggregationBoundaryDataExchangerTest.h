
// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef _PEANO_HEAP_TESTS_AGGREGATION_BOUNDARY_DATA_EXCHANGER_TESTS_
#define _PEANO_HEAP_TESTS_AGGREGATION_BOUNDARY_DATA_EXCHANGER_TESTS_
 


#include "tarch/tests/TestCase.h"
#include "tarch/logging/Log.h"


namespace peano {
  namespace heap {
    namespace tests {
      class AggregationBoundaryDataExchangerTest;
    }
  }
}
 

/**
 * This is just a default test case that demonstrated how to write unit tests 
 * in Peano. Feel free to rename, remove, or duplicate it. 
 */ 
class peano::heap::tests::AggregationBoundaryDataExchangerTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log  _log;

    void testHeaderComposeDecomposeOnCharHeap();
    void testHeaderComposeDecomposeOnIntHeap();
    void testHeaderComposeDecomposeOnDoubleHeap();

    /**
     * We construct the message
     *
     * 6, 0, 1, 0, 2, 0, -1, 3, 0, -1, -2, 4, ...
     */
    void testWholeMessageDecompositionInDoubleHeap();
    void testWholeMessageDecompositionInCharHeap();
  public:
    AggregationBoundaryDataExchangerTest();
    virtual ~AggregationBoundaryDataExchangerTest();
     
    virtual void run();
};


#endif
