// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#include "MatrixTest.h"
#include "tarch/la/Matrix.h"
#include "tarch/la/MatrixAssignList.h"
#include "tarch/la/MatrixOperations.h"
#include "tarch/la/MatrixMatrixOperations.h"
#include <string>
#include <sstream>

#include "tarch/tests/TestCaseFactory.h"
registerTest(tarch::la::MatrixTest)


tarch::la::MatrixTest::MatrixTest():
  TestCase ("tarch::la::MatrixTest") {
}


void tarch::la::MatrixTest::run() {
  testMethod (testConstruction);
  testMethod (testAssignment);
  testMethod (testMatrixOperations);
  testMethod (testMatrixMatrixOperations);
  testMethod (testTransposedMatrix);
}


void tarch::la::MatrixTest::testConstruction() {
  Matrix<1,2,int> matrix(1);
  validateEquals (matrix.size(), 2);
  validateEquals (matrix.rows(), 1);
  validateEquals (matrix.cols(), 2);
  validateEquals (matrix(0,0), 1);
  validateEquals (matrix(0,1), 1);

  Matrix<1,2,int> matrix2(matrix);
  validateEquals (matrix2(0,0), 1);
  validateEquals (matrix2(0,1), 1);
}


void tarch::la::MatrixTest:: testAssignment() {
  Matrix<2,2,int> matrix(1);
  assignList(matrix) = 1, 2, 3, 4;
  validateEquals (matrix(0,0), 1);
  validateEquals (matrix(0,1), 2);
  validateEquals (matrix(1,0), 3);
  validateEquals (matrix(1,1), 4);

  Matrix<1,2,int> matrix2;
  assignList(matrix2) = 1, 2;
  validateEquals (matrix2(0,0), 1);
  validateEquals (matrix2(0,1), 2);
}


void tarch::la::MatrixTest::testMatrixOperations () {
  // Test streaming
  Matrix<2,2,int> matrix2;
  assignList(matrix2) = 1, 2, 3, 4;
  std::ostringstream stream;
  stream << matrix2;
  validateEquals (stream.str(), std::string("[[1,2],[3,4]]"));
  // Test matrix multiply scalar
  matrix2 =matrix2*2;
  validateEquals(matrix2(0,0),2);
  validateEquals(matrix2(0,1),4);
  validateEquals(matrix2(1,0),6);
  validateEquals(matrix2(1,1),8);
}


void tarch::la::MatrixTest::testMatrixMatrixOperations ()
{
  Matrix<2,3,int> lMatrix;
  Matrix<3,2,int> rMatrix;
  Matrix<2,2,int> result(0);
  assignList(lMatrix) = 1, 2, 3,
                        4, 5, 6;
  assignList(rMatrix) = 6, 5,
                        4, 3,
                        2, 1;

  // Matrix matrix multiplication
  result = lMatrix * rMatrix;
  validateEquals (result(0,0), 20);
  validateEquals (result(0,1), 14);
  validateEquals (result(1,0), 56);
  validateEquals (result(1,1), 41);

  // Bitwise comparison
  Matrix<2,3,int> matrixA(1);
  Matrix<2,3,int> matrixB(2);
  validate (matrixA == matrixA);
  validate (! (matrixA == matrixB));

  // Test equalsReturnIndex
  Matrix<2,3,double> matrix1(1);
  Matrix<2,3,double> matrix2(2);
  std::pair<int,int> i=equalsReturnIndex(matrix1,matrix2);
  validateEquals(i.first,0);
  validateEquals(i.second,0);
}


void tarch::la::MatrixTest:: testTransposedMatrix ()
{
  Matrix<3,2,int> matrix;
  assignList(matrix) = 1, 2,
                       3, 4,
                       5, 6;
  Matrix<2,3,int> transposed = transpose(matrix);
  validateEquals (transposed(0,0), 1);
  validateEquals (transposed(0,1), 3);
  validateEquals (transposed(0,2), 5);
  validateEquals (transposed(1,0), 2);
  validateEquals (transposed(1,1), 4);
  validateEquals (transposed(1,2), 6);

  validate (transpose(transposed) == matrix);
}
