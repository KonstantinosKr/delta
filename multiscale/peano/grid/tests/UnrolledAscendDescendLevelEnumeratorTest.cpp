#include "peano/grid/tests/UnrolledAscendDescendLevelEnumeratorTest.h"

#include "peano/utils/Globals.h"
#include "peano/grid/UnrolledLevelEnumerator.h"
#include "peano/grid/UnrolledAscendDescendLevelEnumerator.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif



tarch::logging::Log peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest::_log( "peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest" );


peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest::UnrolledAscendDescendLevelEnumeratorTest():
  tarch::tests::TestCase( "peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest" ) {
}


peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest::~UnrolledAscendDescendLevelEnumeratorTest() {
}


void peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest::run() {
  logTraceIn( "run() ");
  testMethod( testLevel1 );
  testMethod( testLevel2 );
  logTraceOut( "run() ");
}


void peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest::setUp() {
}


void peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest::testLevel1() {
  #ifdef Dim2
  peano::grid::UnrolledLevelEnumerator::Vector coarsestGridCellSize;
  peano::grid::UnrolledLevelEnumerator::Vector domainOffset;

  tarch::la::assignList( coarsestGridCellSize ) = 1.0/9.0, 1.0/9.0;
  tarch::la::assignList( domainOffset )         = 4.0/9.0, 6.0/9.0;

  peano::grid::UnrolledLevelEnumerator              coarseEnumerator(
    coarsestGridCellSize,
    domainOffset,
    3,
    1,
    0
  );
  peano::grid::UnrolledLevelEnumerator              fineEnumerator(
    coarsestGridCellSize,
    domainOffset,
    3,
    1,
    1
  );

  peano::grid::UnrolledAscendDescendLevelEnumerator fineUnrolledEnumerator(fineEnumerator);

  peano::grid::UnrolledLevelEnumerator::Vector                  v;
  peano::grid::UnrolledLevelEnumerator::LocalVertexIntegerIndex p;

  validateEqualsWithParams3( coarseEnumerator.getLevel(),         3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator.getLevel(),           4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator.getLevel(),   4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  validateEqualsWithParams3( coarseEnumerator(0),         0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(0),           0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(0),   0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,0;
  tarch::la::assignList( v ) = 4.0/9.0, 6.0/9.0;
  validateEqualsWithParams3( coarseEnumerator(p),         0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),           0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateNumericalVectorEquals( coarseEnumerator.getVertexPosition(p),       v );
  validateNumericalVectorEquals( fineEnumerator.getVertexPosition(p),         v );
  validateNumericalVectorEquals( fineUnrolledEnumerator.getVertexPosition(p), v );

  tarch::la::assignList( p ) = 1,0;
 validateEqualsWithParams3( coarseEnumerator(p),         1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),           1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = (4.0+1.0)/9.0, 6.0/9.0;
  validateNumericalVectorEquals( coarseEnumerator.getVertexPosition(p),       v );
  tarch::la::assignList( v ) = (4.0*3.0+1.0)/(9.0*3.0), (6.0*3.0+0.0)/(9.0*3.0);
  validateNumericalVectorEqualsWithParams3( fineEnumerator.getVertexPosition(p),         v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,0;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   2, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = (4.0*3.0+2.0)/(9.0*3.0), (6.0*3.0+0.0)/(9.0*3.0);
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 3,0;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = (4.0*3.0+3.0)/(9.0*3.0), (6.0*3.0+0.0)/(9.0*3.0);
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,1;
  validateEqualsWithParams3( coarseEnumerator(p),         2, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),           4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = (4.0*3.0+0.0)/(9.0*3.0), (6.0*3.0+1.0)/(9.0*3.0);
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,1;
  validateEqualsWithParams3( coarseEnumerator(p),         3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),           5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = (4.0*3.0+1.0)/(9.0*3.0), (6.0*3.0+1.0)/(9.0*3.0);
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,1;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   6, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = (4.0*3.0+2.0)/(9.0*3.0), (6.0*3.0+1.0)/(9.0*3.0);
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 3,1;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   7, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = (4.0*3.0+3.0)/(9.0*3.0), (6.0*3.0+1.0)/(9.0*3.0);
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   2, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,1;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,1;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  #endif
}


void peano::grid::tests::UnrolledAscendDescendLevelEnumeratorTest::testLevel2() {
  #ifdef Dim2
  peano::grid::UnrolledLevelEnumerator::Vector coarsestGridCellSize;
  peano::grid::UnrolledLevelEnumerator::Vector domainOffset;

  tarch::la::assignList( coarsestGridCellSize ) = 1.0/9.0, 1.0/9.0;
  tarch::la::assignList( domainOffset )         = 4.0/9.0, 6.0/9.0;

  peano::grid::UnrolledLevelEnumerator              coarseEnumerator(
    coarsestGridCellSize,
    domainOffset,
    3,
    2,
    1
  );
  peano::grid::UnrolledLevelEnumerator              fineEnumerator(
    coarsestGridCellSize,
    domainOffset,
    3,
    2,
    2
  );

  peano::grid::UnrolledAscendDescendLevelEnumerator fineUnrolledEnumerator(fineEnumerator);

  peano::grid::UnrolledLevelEnumerator::Vector                  v;
  peano::grid::UnrolledLevelEnumerator::LocalVertexIntegerIndex p;

  validateEqualsWithParams3( coarseEnumerator.getLevel(),         4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator.getLevel(),           5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator.getLevel(),   5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  validateEqualsWithParams3( coarseEnumerator(0),         0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(0),           0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(0),   0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,0;
  tarch::la::assignList( v ) = 4.0/9.0, 6.0/9.0;
  validateEqualsWithParams3( coarseEnumerator(p),         0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),           0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateNumericalVectorEquals( coarseEnumerator.getVertexPosition(p),       v );
  validateNumericalVectorEquals( fineEnumerator.getVertexPosition(p),         v );
  validateNumericalVectorEquals( fineUnrolledEnumerator.getVertexPosition(p), v );

  tarch::la::assignList( p ) = 1,0;
  validateEqualsWithParams3( coarseEnumerator(p),         1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),           1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0, 6.0/9.0;
  validateNumericalVectorEquals( coarseEnumerator.getVertexPosition(p),       v );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/81.0, 6.0/9.0 + 0.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineEnumerator.getVertexPosition(p),         v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,0;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   2, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 2.0/81.0, 6.0/9.0 + 0.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 3,0;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 3.0/81.0, 6.0/9.0 + 0.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,1;
  validateEqualsWithParams3( coarseEnumerator(p),         4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),          10, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  10, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 0.0/81.0, 6.0/9.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,1;
  validateEqualsWithParams3( coarseEnumerator(p),         5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator(p),          11, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  11, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/81.0, 6.0/9.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,1;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  12, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 2.0/81.0, 6.0/9.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 3,1;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  13, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 3.0/81.0, 6.0/9.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   0, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   2, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,1;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   9, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,1;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),  10, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );








  tarch::la::assignList( p ) = 1,0;
  coarseEnumerator.setOffset(p);
  fineUnrolledEnumerator.setOffset(p*3);

  validateEqualsWithParams3( coarseEnumerator.getLevel(),         4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineEnumerator.getLevel(),           5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator.getLevel(),   5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  validateEqualsWithParams3( coarseEnumerator(0),         1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(0),   3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,0;
  validateEqualsWithParams3( coarseEnumerator(p),         1, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0, 6.0/9.0;
  validateNumericalVectorEquals( coarseEnumerator.getVertexPosition(p),       v );
  validateNumericalVectorEquals( fineUnrolledEnumerator.getVertexPosition(p), v );

  tarch::la::assignList( p ) = 1,0;
  validateEqualsWithParams3( coarseEnumerator(p),         2, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 2.0/27.0, 6.0/9.0;
  validateNumericalVectorEquals( coarseEnumerator.getVertexPosition(p),       v );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0 + 1.0/81.0, 6.0/9.0 + 0.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,0;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0 + 2.0/81.0, 6.0/9.0 + 0.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 3,0;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),   6, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0 + 3.0/81.0, 6.0/9.0 + 0.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,1;
  validateEqualsWithParams3( coarseEnumerator(p),         5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  13, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0 + 0.0/81.0, 6.0/9.0 + 0.0/27.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,1;
  validateEqualsWithParams3( coarseEnumerator(p),         6, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  14, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0 + 1.0/81.0, 6.0/9.0 + 0.0/27.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,1;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  15, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0 + 2.0/81.0, 6.0/9.0 + 0.0/27.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 3,1;
  validateEqualsWithParams3( fineUnrolledEnumerator(p),  16, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  tarch::la::assignList( v ) = 4.0/9.0 + 1.0/27.0 + 3.0/81.0, 6.0/9.0 + 0.0/27.0 + 1.0/81.0;
  validateNumericalVectorEqualsWithParams3( fineUnrolledEnumerator.getVertexPosition(p), v, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   3, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   4, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 2,0;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),   5, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 0,1;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),  12, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );

  tarch::la::assignList( p ) = 1,1;
  validateEqualsWithParams3( fineUnrolledEnumerator.cell(p),  13, coarseEnumerator.toString(), fineEnumerator.toString(), fineUnrolledEnumerator.toString() );
  #endif
}


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
