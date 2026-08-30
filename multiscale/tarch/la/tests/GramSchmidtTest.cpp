#include "GramSchmidtTest.h"
#include "tarch/la/Matrix.h"
#include "tarch/la/GramSchmidt.h"
#include "tarch/la/MatrixMatrixOperations.h"
#include "tarch/la/ScalarOperations.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(tarch::la::GramSchmidtTest)


tarch::la::GramSchmidtTest::GramSchmidtTest():
  TestCase ("tarch::la::GramSchmidtTest") {
}


void tarch::la::GramSchmidtTest::run() {
  testMethod (testModifiedGramSchmidt);
}


void tarch::la::GramSchmidtTest::testModifiedGramSchmidt() {
  Matrix<4,4,double> A;

  // Set values according to Hilbert matrix.
  for (int i=0; i < 4; i++) {
     for (int j=0; j < 4; j++) {
        double entry = 1.0 / static_cast<double>(i + j + 1);
        A(i,j) = entry;
    }
  }

  Matrix<4,4,double> Acopy(A);
  Matrix<4,4,double> Q;
  Matrix<4,4,double> R;
  modifiedGramSchmidt (Acopy, Q, R);

  Matrix<4,4,double> QTQ(0.0);
  QTQ = multiply(transpose(Q), Q);

  for (int i=0; i < 4; i++) {
    for (int j=0; j < 4; j++) {
      if (i == j) {
        validate (equals(QTQ(i,j), 1.0, 1e-12));
      }
      else {
        validate (equals(QTQ(i,j), 0.0, 1e-12));
      }
    }
  }

  for (int i=0; i < 4; i++) {
     for (int j=0; j < 4; j++) {
        double value = 0.0;
        for ( int k=0; k < Q.cols(); k++ ) {
           value += Q(i,k) * R(k,j);
        }
        validate (equals(value, A(i,j)));
        value = 0.0;
     }
  }
}
