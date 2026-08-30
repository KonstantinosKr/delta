
// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef _PEANO_HEAP_TESTS_COMPRESSED_FLOATING_POINT_NUMBERS_TESTS_
#define _PEANO_HEAP_TESTS_COMPRESSED_FLOATING_POINT_NUMBERS_TESTS_
 


#include "tarch/tests/TestCase.h"
#include "tarch/logging/Log.h"


namespace peano {
  namespace heap {
    namespace tests {
      class CompressedFloatingPointNumbersTest;
    }
  }
}
 

/**
 * This is just a default test case that demonstrated how to write unit tests 
 * in Peano. Feel free to rename, remove, or duplicate it. 
 */ 
class peano::heap::tests::CompressedFloatingPointNumbersTest: public tarch::tests::TestCase {
  private:
    static tarch::logging::Log  _log;

    void testDecomposeWithLongInt();
    void testDecomposeWithInt();
    void testDecomposeWithChar();
    void testBatchDecomposition();
    void testBatchDecompositionWithArrayOfFour();
    void testErrorComputation();

    void testTinyValues();

    /**
     * Something taken from the ParCo presentation
     */
    void testDecompose();

    /**
     * From ExaHyPE:
     *
  assertion in file /home/tobias/git/ExaHyPE/Code/./ExaHyPE/exahype/solvers/ADERDGSolver.cpp, line 2983 failed: tarch::la::equals( DataHeap::getInstance().getData(heapIndex)[i], reconstructedValue, CompressionAccuracy )
  parameter DataHeap::getInstance().getData(heapIndex)[i]: 1.68855695855260723448e+00
  parameter reconstructedValue: 1.01768855695855017984e+03
  parameter DataHeap::getInstance().getData(heapIndex)[i] - reconstructedValue: -1.01599999999999761258e+03
  parameter CompressionAccuracy: 1.00000000000000006228e-09
  parameter bytesForMantissa: 5
  ExaHyPE-Euler: /home/tobias/git/ExaHyPE/Code/./ExaHyPE/exahype/solvers/ADERDGSolver.cpp:2983: void exahype::solvers::ADERDGSolver::glueTogether(int, int, int): Assertion `false' failed.

     But I used /4.0 here for the accuracy.


     */
    void testComposeDecompose();
public:
    CompressedFloatingPointNumbersTest();
    virtual ~CompressedFloatingPointNumbersTest();
     
    virtual void run();
};


#endif
