#include "peano/grid/tests/RegularRefinedTest.h"
#include "peano/grid/nodes/RegularRefined.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::RegularRefinedTest)



#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


tarch::logging::Log peano::grid::tests::RegularRefinedTest::_log( "peano::grid::tests::RegularRefinedTest" );


peano::grid::tests::RegularRefinedTest::RegularRefinedTest():
  TestCase( "peano::grid::tests::RegularRefinedTest" ) {
}


peano::grid::tests::RegularRefinedTest::~RegularRefinedTest() {
}


void peano::grid::tests::RegularRefinedTest::run() {
  logTraceIn( "run() ");
  testMethod( test2DComputePositionRelativeToNextCoarserLevelFromFineGridVertexPosition );
  testMethod( test2DComputePositionRelativeToNextCoarserLevelFromFineGridCellPosition );
  logTraceOut( "run() ");
}



void peano::grid::tests::RegularRefinedTest::setUp() {
}


void peano::grid::tests::RegularRefinedTest::test2DComputePositionRelativeToNextCoarserLevelFromFineGridVertexPosition() {
  logTraceIn( "test2DComputePositionRelativeToNextCoarserLevelFromFineGridVertexPosition()" );
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int> fineGridPosition;
  tarch::la::Vector<DIMENSIONS,int> offsetOfCoarseGridEnumerator;
  tarch::la::Vector<DIMENSIONS,int> fineGridPositionRelativeToCoarserCell;

  fineGridPosition = 0,0;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridVertexPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 0 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 0 );

  fineGridPosition = 3,1;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridVertexPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 0 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 3 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 1 );

  fineGridPosition = 4,2;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridVertexPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 1 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 1 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 2 );

  fineGridPosition = 6,3;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridVertexPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 1 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 3 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 3 );

  fineGridPosition = 7,4;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridVertexPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 2 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 1 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 1 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 1 );
  #endif
  logTraceOut( "test2DComputePositionRelativeToNextCoarserLevelFromFineGridVertexPosition()" );
}


void peano::grid::tests::RegularRefinedTest::test2DComputePositionRelativeToNextCoarserLevelFromFineGridCellPosition() {
  logTraceIn( "test2DComputePositionRelativeToNextCoarserLevelFromFineGridCellPosition()" );
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int> fineGridPosition;
  tarch::la::Vector<DIMENSIONS,int> offsetOfCoarseGridEnumerator;
  tarch::la::Vector<DIMENSIONS,int> fineGridPositionRelativeToCoarserCell;

  fineGridPosition = 0,0;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridCellPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 0 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 0 );

  fineGridPosition = 3,1;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridCellPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 1 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 1 );

  fineGridPosition = 4,2;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridCellPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 1 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 1 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 2 );

  fineGridPosition = 6,3;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridCellPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 2 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 1 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 0 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 0 );

  fineGridPosition = 7,4;
  peano::grid::nodes::computePositionRelativeToNextCoarserLevelFromFineGridCellPosition(
    fineGridPosition,
    offsetOfCoarseGridEnumerator,
    fineGridPositionRelativeToCoarserCell
  );
  validateEquals( offsetOfCoarseGridEnumerator(0), 2 );
  validateEquals( offsetOfCoarseGridEnumerator(1), 1 );
  validateEquals( fineGridPositionRelativeToCoarserCell(0), 1 );
  validateEquals( fineGridPositionRelativeToCoarserCell(1), 1 );
  #endif
  logTraceOut( "test2DComputePositionRelativeToNextCoarserLevelFromFineGridCellPosition()" );
}


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif

