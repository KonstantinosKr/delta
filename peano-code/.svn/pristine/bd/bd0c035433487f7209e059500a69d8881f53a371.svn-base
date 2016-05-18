#include "peano/grid/tests/UnrolledLevelEnumeratorTest.h"
#include "peano/grid/UnrolledLevelEnumerator.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::UnrolledLevelEnumeratorTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


tarch::logging::Log peano::grid::tests::UnrolledLevelEnumeratorTest::_log( "peano::grid::tests::UnrolledLevelEnumeratorTest" );


peano::grid::tests::UnrolledLevelEnumeratorTest::UnrolledLevelEnumeratorTest():
  tarch::tests::TestCase( "peano::grid::tests::UnrolledLevelEnumeratorTest" ) {
}


peano::grid::tests::UnrolledLevelEnumeratorTest::~UnrolledLevelEnumeratorTest() {
}


void peano::grid::tests::UnrolledLevelEnumeratorTest::run() {
  logTraceIn( "run() ");
  testMethod( test3DGetVertexPosition );
  logTraceOut( "run() ");
}



void peano::grid::tests::UnrolledLevelEnumeratorTest::setUp() {
}


void peano::grid::tests::UnrolledLevelEnumeratorTest::test3DGetVertexPosition() {
  #ifdef Dim3
  const int  totalHeightOfSubtree = 4;

  const UnrolledLevelEnumerator::Vector domainOffset          = 0.0;
  const UnrolledLevelEnumerator::Vector coarsestGridCellSize  = 1.0;

  UnrolledLevelEnumerator enumerator(
    coarsestGridCellSize,
    domainOffset,
    0,
    totalHeightOfSubtree,
    4
  );

  const int    verticesPerDimension = 3*3*3*3+1; // 82 = 3^4+1
  const double h                    = 1.0/3.0/3.0/3.0/3.0;

  validateEqualsWithParams1( enumerator.getCellSize(), UnrolledLevelEnumerator::Vector(h), enumerator.toString() );
  validateEqualsWithParams1( enumerator.getLevel(),    4,                                  enumerator.toString() );
  validateEqualsWithParams1( enumerator.getVerticesPerAxis(), verticesPerDimension,        enumerator.toString() );

  UnrolledLevelEnumerator::Vector x;

  tarch::la::assignList(x) = 0.0, 0.0, 0.0;
  validateEqualsWithParams1( enumerator.getVertexPosition(0),  x, enumerator.toString() );

  tarch::la::assignList(x) = 1.0*h, 0.0, 0.0;
  validateEqualsWithParams1( enumerator.getVertexPosition(1),  x, enumerator.toString() );

  tarch::la::assignList(x) = 2.0*h, 0.0, 0.0;
  validateEqualsWithParams1( enumerator.getVertexPosition(2),  x, enumerator.toString() );

  tarch::la::assignList(x) = 3.0*h, 0.0, 0.0;
  validateEqualsWithParams1( enumerator.getVertexPosition(3),  x, enumerator.toString() );
  #endif
}




#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
