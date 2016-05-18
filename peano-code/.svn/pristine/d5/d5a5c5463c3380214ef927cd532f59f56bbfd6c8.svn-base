#include "peano/grid/tests/SingleLevelEnumeratorTest.h"
#include "peano/grid/SingleLevelEnumerator.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::SingleLevelEnumeratorTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


tarch::logging::Log peano::grid::tests::SingleLevelEnumeratorTest::_log( "peano::grid::tests::SingleLevelEnumeratorTest" );


peano::grid::tests::SingleLevelEnumeratorTest::SingleLevelEnumeratorTest():
  tarch::tests::TestCase( "peano::grid::tests::SingleLevelEnumeratorTest" ) {
}


peano::grid::tests::SingleLevelEnumeratorTest::~SingleLevelEnumeratorTest() {
}


void peano::grid::tests::SingleLevelEnumeratorTest::run() {
  logTraceIn( "run() ");
  testMethod( test2DLineariseCellIndex );
  testMethod( test2DLineariseVertexIndex );
  testMethod( testFunctorWithZeroArgument );
  testMethod( testGetVertexPositionOnCoarserLevel );
  testMethod( test2DFunctor );
  testMethod( test3DFunctor );
  logTraceOut( "run() ");
}



void peano::grid::tests::SingleLevelEnumeratorTest::setUp() {
}


void peano::grid::tests::SingleLevelEnumeratorTest::test3DFunctor() {
  #ifdef Dim3
  tarch::la::Vector<3,double> size;
  tarch::la::Vector<3,double> offset;

  tarch::la::assignList(size)   = 0.3, 0.3, 0.3;
  tarch::la::assignList(offset) = 1.0, 0.0, 0.0;

  peano::grid::SingleLevelEnumerator vertexEnumerator( size, offset, 2, peano::grid::StationaryButIrregular );

  validateEqualsWithParams1(vertexEnumerator(0), 0,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(1), 1,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(2), 4,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(3), 5,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(4), 16, vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(5), 17, vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(6), 20, vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(7), 21, vertexEnumerator.toString());

  peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex discreteOffset;
  tarch::la::assignList(discreteOffset) = 0, 2, 2;
  vertexEnumerator.setOffset(discreteOffset);

  validateEqualsWithParams1(vertexEnumerator(0), 40+0,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(1), 40+1,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(2), 40+4,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(3), 40+5,  vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(4), 40+16, vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(5), 40+17, vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(6), 40+20, vertexEnumerator.toString());
  validateEqualsWithParams1(vertexEnumerator(7), 40+21, vertexEnumerator.toString());
  #endif
}


void peano::grid::tests::SingleLevelEnumeratorTest::test2DFunctor() {
  #ifdef Dim2
  const tarch::la::Vector<DIMENSIONS,double>  coarseGridCellSize(0.2);
  const tarch::la::Vector<DIMENSIONS,double>  domainOffset(0.2);
  const int                                   level(3);
  peano::grid::SingleLevelEnumerator vertexEnumerator(coarseGridCellSize,domainOffset,level,static_cast<peano::grid::CellFlags>(30) );

  assertionEquals( vertexEnumerator(0), 0 );
  assertionEquals( vertexEnumerator(1), 1 );
  assertionEquals( vertexEnumerator(2), 4 );
  assertionEquals( vertexEnumerator(3), 5 );

  vertexEnumerator.setOffset( tarch::la::Vector<DIMENSIONS,int>(2) );
  assertionEquals( vertexEnumerator(0), 10 );
  assertionEquals( vertexEnumerator(1), 11 );
  assertionEquals( vertexEnumerator(2), 14 );
  assertionEquals( vertexEnumerator(3), 15 );
  #endif
}


void peano::grid::tests::SingleLevelEnumeratorTest::testGetVertexPositionOnCoarserLevel() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int> fineGridPositionOfVertex;

  fineGridPositionOfVertex(0) = 0;
  fineGridPositionOfVertex(1) = 0;
  validate(peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(0),
    0
  );
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(1),
    0
  );

  fineGridPositionOfVertex(0) = 1;
  fineGridPositionOfVertex(1) = 0;
  validate(!peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));

  fineGridPositionOfVertex(0) = 2;
  fineGridPositionOfVertex(1) = 0;
  validate(!peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));

  fineGridPositionOfVertex(0) = 3;
  fineGridPositionOfVertex(1) = 0;
  validate(peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(0),
    1
  );
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(1),
    0
  );

  fineGridPositionOfVertex(0) = 0;
  fineGridPositionOfVertex(1) = 1;
  validate(!peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));

  fineGridPositionOfVertex(0) = 2;
  fineGridPositionOfVertex(1) = 1;
  validate(!peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));

  fineGridPositionOfVertex(0) = 1;
  fineGridPositionOfVertex(1) = 2;
  validate(!peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));

  fineGridPositionOfVertex(0) = 0;
  fineGridPositionOfVertex(1) = 3;
  validate(peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(0),
    0
  );
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(1),
    1
  );

  fineGridPositionOfVertex(0) = 1;
  fineGridPositionOfVertex(1) = 3;
  validate(!peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));

  fineGridPositionOfVertex(0) = 3;
  fineGridPositionOfVertex(1) = 3;
  validate(peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex));
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(0),
    1
  );
  validateEquals(
    peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex)(1),
    1
  );
  #endif
}


void peano::grid::tests::SingleLevelEnumeratorTest::test2DLineariseCellIndex() {
  #ifdef Dim2
  peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex  cellPosition;

  tarch::la::assignList(cellPosition) = 0,0;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 0 );

  tarch::la::assignList(cellPosition) = 1,0;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 1 );

  tarch::la::assignList(cellPosition) = 2,0;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 2 );

  tarch::la::assignList(cellPosition) = 0,1;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 3 );

  tarch::la::assignList(cellPosition) = 1,1;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 4 );

  tarch::la::assignList(cellPosition) = 2,1;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 5 );

  tarch::la::assignList(cellPosition) = 0,2;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 6 );

  tarch::la::assignList(cellPosition) = 1,2;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 7 );

  tarch::la::assignList(cellPosition) = 2,2;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseCellIndex( cellPosition ), 8 );
  #endif
}


void peano::grid::tests::SingleLevelEnumeratorTest::test2DLineariseVertexIndex() {
  #ifdef Dim2
  peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex  vertexPosition;

  tarch::la::assignList(vertexPosition) = 0,0;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 0 );

  tarch::la::assignList(vertexPosition) = 1,0;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 1 );

  tarch::la::assignList(vertexPosition) = 2,0;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 2 );

  tarch::la::assignList(vertexPosition) = 3,0;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 3 );

  tarch::la::assignList(vertexPosition) = 0,1;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 4 );

  tarch::la::assignList(vertexPosition) = 1,1;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 5 );

  tarch::la::assignList(vertexPosition) = 2,1;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 6 );

  tarch::la::assignList(vertexPosition) = 3,1;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 7 );

  tarch::la::assignList(vertexPosition) = 0,2;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 8 );

  tarch::la::assignList(vertexPosition) = 1,2;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 9 );

  tarch::la::assignList(vertexPosition) = 2,2;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 10 );

  tarch::la::assignList(vertexPosition) = 3,2;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 11 );

  tarch::la::assignList(vertexPosition) = 0,3;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 12 );

  tarch::la::assignList(vertexPosition) = 1,3;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 13 );

  tarch::la::assignList(vertexPosition) = 2,3;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 14 );

  tarch::la::assignList(vertexPosition) = 3,3;
  validateEquals( peano::grid::SingleLevelEnumerator::lineariseVertexIndex( vertexPosition ), 15 );
  #endif
}

void peano::grid::tests::SingleLevelEnumeratorTest::testFunctorWithZeroArgument() {
  peano::grid::SingleLevelEnumerator enumerator(tarch::la::Vector<DIMENSIONS,double>(1.0),tarch::la::Vector<DIMENSIONS,double>(0.0),0);

  peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex offset(0);

  offset(0) = 0;
  enumerator.setOffset( offset );
  validateEquals( enumerator(0), 0 );

  offset(0) = 1;
  enumerator.setOffset( offset );
  validateEquals( enumerator(0), 1 );

  offset(0) = 2;
  enumerator.setOffset( offset );
  validateEquals( enumerator(0), 2 );

  offset(0) = 3;
  enumerator.setOffset( offset );
  validateEquals( enumerator(0), 3 );

  offset(0) = 0;
  offset(1) = 1;
  enumerator.setOffset( offset );
  validateEquals( enumerator(0), 4 );
}



#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
