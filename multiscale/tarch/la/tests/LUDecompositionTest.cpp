// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#include "LUDecompositionTest.h"
#include "tarch/la/LUDecomposition.h"
#include "tarch/la/Matrix.h"
#include "tarch/la/Vector.h"

#include "tarch/tests/TestCaseFactory.h"
registerTest(tarch::la::LUDecompositionTest)


tarch::la::LUDecompositionTest::LUDecompositionTest():
  TestCase ("tarch::la::LUDecompositionTest") {
}


void tarch::la::LUDecompositionTest::run() {
  testMethod (testLUNoPivoting);
  testMethod (testLU);
}


void tarch::la::LUDecompositionTest::testLUNoPivoting() {
}


void tarch::la::LUDecompositionTest::testLU() {
  // Test is obviously buggy. The pivot values are doubles but the test uses
  // them as integer in line 57.
/*
  Matrix<4,4,double> A;
  assignList(A) =
      5.0, 2.0, 3.0, 1.0,
      6.0, 3.0, 6.0, 1.0,
      3.0, 2.0, 5.0, 4.0,
      4.0, 1.0, 2.0, 1.0;
  Matrix<4,4,double> Acopy(A);
  Vector<4,double> pivots;
  lu(A,pivots);
  Matrix<4,4,double> L(A);
  for (int i=0; i < 4; i++){
    L(i,i) = 1.0;
    for (int j=i+1; j < 4; j++){
      L(i,j) = 0.0;
    }
  }
  Matrix<4,4,double> R(A);
  for (int j=0; j < 4; j++){
    for (int i=j+1; i < 4; i++){
      R(i,j) = 0.0;
    }
  }
  A = L * R;
  for (int i=0; i < 4; i++){
    for (int j=0; j < 4; j++){
      double temp = Acopy(i,j);
      Acopy(i,j) = Acopy(pivots(i),j);
      Acopy(pivots(i),j) = temp;
    }
  }
*/
}
