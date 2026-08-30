#include "peano/grid/tests/CellLocalPeanoCurveTest.h"
#include "peano/grid/aspects/CellLocalPeanoCurve.h"
#include "peano/utils/Globals.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::CellLocalPeanoCurveTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif



tarch::logging::Log peano::grid::tests::CellLocalPeanoCurveTest::_log( "peano::grid::tests::CellLocalPeanoCurveTest" );


peano::grid::tests::CellLocalPeanoCurveTest::CellLocalPeanoCurveTest():
  tarch::tests::TestCase( "peano::grid::tests::CellLocalPeanoCurveTest" ) {
}


peano::grid::tests::CellLocalPeanoCurveTest::~CellLocalPeanoCurveTest() {
}


void peano::grid::tests::CellLocalPeanoCurveTest::run() {
  logTraceIn( "run() ");
  testMethod( test2DCreateReadVertexSequenceForP0P0P1P2 );
  testMethod( test2DCreateReadVertexSequenceForM1M2P2P3 );
  testMethod( test2DCreateReadVertexSequenceForM1P0P0P1 );
  testMethod( test2DCreateReadVertexSequenceForP1M1O0P2 );
  testMethod( test2DCreateReadVertexSequenceForP3P2P1M1 );
  testMethod( test2DCreateReadVertexSequenceForP3M1P1P2 );
  testMethod( test2DCreateReadVertexSequenceForM1P3P1P2 );
  testMethod( test2DCreateWriteVertexSequenceForM1P0P1P2 );
  testMethod( test2DCreateWriteVertexSequenceForP1M1M2P2 );

  testMethod( test3DCreateReadVertexSequenceForM2P4M1P1P2P3 );

  testMethod( test4D );

  logTraceOut( "run() ");
}


void peano::grid::tests::CellLocalPeanoCurveTest::setUp() {
}


void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateReadVertexSequenceForP0P0P1P2() {
  TestCell cell;

  //  (isInside:0,state:New,level:1,evenFlags:[0,0],accessNumber:[0,0,1,2])
  cell._cellData.setAccessNumber(0,0);
  cell._cellData.setAccessNumber(1,0);
  cell._cellData.setAccessNumber(2,1);
  cell._cellData.setAccessNumber(3,2);
  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,0);

  peano::datatraversal::ActionSetTraversal sequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(),false );
  peano::datatraversal::ActionSetTraversal parallelTraversal   = peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._id,-1);



  validateEquals(parallelTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(parallelTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(parallelTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(parallelTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(parallelTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._id,-1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._id,-1);
  validateEquals(parallelTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(2).getAction(0)._id,-1);
  validateEquals(parallelTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(3).getAction(0)._id,-1);
}


void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateReadVertexSequenceForM1M2P2P3() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0,-1);
  cell._cellData.setAccessNumber(1,-2);
  cell._cellData.setAccessNumber(2,1);
  cell._cellData.setAccessNumber(3,2);
  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,0);

  peano::datatraversal::ActionSetTraversal sequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );
  peano::datatraversal::ActionSetTraversal parallelTraversal   = peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._id,0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._id,1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._id,0);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._id,-1);

  validateEqualsWithParams1(parallelTraversal.getMaximumPath(),2,parallelTraversal.toString());

  validateEquals(parallelTraversal.getActionSet(0).getNumberOfParallelActions(),3);
  validateEquals(parallelTraversal.getActionSet(1).getNumberOfParallelActions(),1);

  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._id,0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._id,1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._id,0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(2)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(2)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(2)._id,-1);
  #endif
}


void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateReadVertexSequenceForM1P0P0P1() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0,-1);
  cell._cellData.setAccessNumber(1,0);
  cell._cellData.setAccessNumber(2,0);
  cell._cellData.setAccessNumber(3,1);
  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,0);

  peano::datatraversal::ActionSetTraversal sequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );
  peano::datatraversal::ActionSetTraversal parallelTraversal   = peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._id,0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._id,0);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._id,-1);


  validateEquals(parallelTraversal.getMaximumPath(),2);

  validateEquals(parallelTraversal.getActionSet(0).getNumberOfParallelActions(),2);
  validateEquals(parallelTraversal.getActionSet(1).getNumberOfParallelActions(),2);

  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._id,0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._id,-1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._id,0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._id,-1);
  #endif
}


void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateReadVertexSequenceForP1M1O0P2() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0,1);
  cell._cellData.setAccessNumber(1,-1);
  cell._cellData.setAccessNumber(2,0);
  cell._cellData.setAccessNumber(3,2);
  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,1);

  peano::datatraversal::ActionSetTraversal sequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );
  peano::datatraversal::ActionSetTraversal parallelTraversal   = peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._id,3);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._id,3);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._id,-1);

  validateEquals(parallelTraversal.getMaximumPath(),2);

  validateEquals(parallelTraversal.getActionSet(0).getNumberOfParallelActions(),2);
  validateEquals(parallelTraversal.getActionSet(1).getNumberOfParallelActions(),2);

  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._id,3);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._id,-1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._id,3);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._id,-1);
  #endif
}



void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateReadVertexSequenceForP3P2P1M1() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0,-3);
  cell._cellData.setAccessNumber(1,-2);
  cell._cellData.setAccessNumber(2,-1);
  cell._cellData.setAccessNumber(3,1);
  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,1);

  peano::datatraversal::ActionSetTraversal sequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), true );
  peano::datatraversal::ActionSetTraversal parallelTraversal   = peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), true );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),1,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._id,1,sequentialTraversal.toString());
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._id,1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._id,-1);

  validateEquals(parallelTraversal.getMaximumPath(),2);

  validateEquals(parallelTraversal.getActionSet(0).getNumberOfParallelActions(),2);
  validateEquals(parallelTraversal.getActionSet(1).getNumberOfParallelActions(),2);

  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._id,1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._id,-1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._id,1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._id,-1);
  #endif
}


void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateReadVertexSequenceForP3M1P1P2() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0, 3);
  cell._cellData.setAccessNumber(1,-1);
  cell._cellData.setAccessNumber(2, 1);
  cell._cellData.setAccessNumber(3, 2);
  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,0);

  peano::datatraversal::ActionSetTraversal sequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );
  peano::datatraversal::ActionSetTraversal parallelTraversal   = peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._id,1,sequentialTraversal.toString());
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._id,1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._id,-1);

  validateEquals(parallelTraversal.getMaximumPath(),2);

  validateEquals(parallelTraversal.getActionSet(0).getNumberOfParallelActions(),2);
  validateEquals(parallelTraversal.getActionSet(1).getNumberOfParallelActions(),2);

  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(0)._id,1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(0),0);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(0).getAction(1)._id,-1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(parallelTraversal.getActionSet(1).getAction(0)._id,1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(0),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._cartesianPosition(1),1);
  validateEquals(parallelTraversal.getActionSet(1).getAction(1)._id,-1);
  #endif
}


void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateReadVertexSequenceForM1P3P1P2() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0,-1);
  cell._cellData.setAccessNumber(1, 3);
  cell._cellData.setAccessNumber(2, 1);
  cell._cellData.setAccessNumber(3, 2);
  cell._cellData.setEvenFlags(0,1);
  cell._cellData.setEvenFlags(1,0);

  peano::datatraversal::ActionSetTraversal sequentialTraversal         = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );
  peano::datatraversal::ActionSetTraversal invertedSequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), true  );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);
  validateEquals(invertedSequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEquals(invertedSequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(invertedSequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(invertedSequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(invertedSequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),1,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._id,2,sequentialTraversal.toString());
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),1);
  validateEquals(sequentialTraversal.getActionSet(1).getAction(0)._id,-1);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(2).getAction(0)._id,2);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),0);
  validateEquals(sequentialTraversal.getActionSet(3).getAction(0)._id,-1);

  validateEquals(invertedSequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(invertedSequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(invertedSequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(invertedSequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEquals(invertedSequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),1);
  validateEquals(invertedSequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),0);
  validateEqualsWithParams1(invertedSequentialTraversal.getActionSet(0).getAction(0)._id,0,invertedSequentialTraversal.toString());
  validateEquals(invertedSequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),0);
  validateEquals(invertedSequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),0);
  validateEquals(invertedSequentialTraversal.getActionSet(1).getAction(0)._id,1);
  validateEquals(invertedSequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),1);
  validateEquals(invertedSequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),1);
  validateEquals(invertedSequentialTraversal.getActionSet(2).getAction(0)._id,0);
  validateEquals(invertedSequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),0);
  validateEquals(invertedSequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),1);
  validateEquals(invertedSequentialTraversal.getActionSet(3).getAction(0)._id,3);
  #endif
}



void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateWriteVertexSequenceForM1P0P1P2() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0,-1);
  cell._cellData.setAccessNumber(1, 0);
  cell._cellData.setAccessNumber(2, 1);
  cell._cellData.setAccessNumber(3, 2);
  cell._cellData.setEvenFlags(0,1);
  cell._cellData.setEvenFlags(1,0);

  peano::datatraversal::ActionSetTraversal sequentialTraversal         = peano::grid::aspects::CellLocalPeanoCurve::createSequentialWriteVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),1,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._id,3,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(0),1,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(1).getAction(0)._cartesianPosition(1),1,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(1).getAction(0)._id,0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(0),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(2).getAction(0)._cartesianPosition(1),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(2).getAction(0)._id,-1,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(0),1,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(3).getAction(0)._cartesianPosition(1),0,sequentialTraversal.toString());
  validateEqualsWithParams1(sequentialTraversal.getActionSet(3).getAction(0)._id,0,sequentialTraversal.toString());
  #endif
}


void peano::grid::tests::CellLocalPeanoCurveTest::test2DCreateWriteVertexSequenceForP1M1M2P2() {
  #ifdef Dim2
  TestCell cell;

  cell._cellData.setAccessNumber(0, 1);
  cell._cellData.setAccessNumber(1,-1);
  cell._cellData.setAccessNumber(2,-2);
  cell._cellData.setAccessNumber(3, 2);
  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,1);

  peano::datatraversal::ActionSetTraversal sequentialTraversalInverted = peano::grid::aspects::CellLocalPeanoCurve::createSequentialWriteVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), true );

  validateEquals(sequentialTraversalInverted.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversalInverted.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversalInverted.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversalInverted.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversalInverted.getActionSet(3).getNumberOfParallelActions(),1);

  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(0).getAction(0)._cartesianPosition(0),  0, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(0).getAction(0)._cartesianPosition(1),  1, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(0).getAction(0)._id,                   -1, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(1).getAction(0)._cartesianPosition(0),  1, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(1).getAction(0)._cartesianPosition(1),  1, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(1).getAction(0)._id,                    2, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(2).getAction(0)._cartesianPosition(0),  0, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(2).getAction(0)._cartesianPosition(1),  0, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(2).getAction(0)._id,                    3, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(3).getAction(0)._cartesianPosition(0),  1, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(3).getAction(0)._cartesianPosition(1),  0, sequentialTraversalInverted.toString());
  validateEqualsWithParams1(sequentialTraversalInverted.getActionSet(3).getAction(0)._id,                    3, sequentialTraversalInverted.toString());
  #endif
}


void peano::grid::tests::CellLocalPeanoCurveTest::test3DCreateReadVertexSequenceForM2P4M1P1P2P3() {
  #ifdef Dim3
  #endif
}


void peano::grid::tests::CellLocalPeanoCurveTest::test4D() {
  #ifdef Dim4
  TestCell cell;

  //  (isInside:0,state:New,level:1,evenFlags:[0,0],accessNumber:[0,0,1,2])
  cell._cellData.setAccessNumber(0,1);
  cell._cellData.setAccessNumber(1,2);
  cell._cellData.setAccessNumber(2,3);
  cell._cellData.setAccessNumber(3,-1);
  cell._cellData.setAccessNumber(4,0);
  cell._cellData.setAccessNumber(5,0);
  cell._cellData.setAccessNumber(6,0);
  cell._cellData.setAccessNumber(7,4);

  cell._cellData.setEvenFlags(0,0);
  cell._cellData.setEvenFlags(1,0);
  cell._cellData.setEvenFlags(2,0);
  cell._cellData.setEvenFlags(3,1);

  peano::datatraversal::ActionSetTraversal sequentialTraversal = peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(),false );
  //peano::datatraversal::ActionSetTraversal parallelTraversal   = peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( cell._cellData.getAccessNumber(), cell._cellData.getEvenFlags(), false );

  validateEquals(sequentialTraversal.getMaximumPath(),NUMBER_OF_VERTICES_PER_ELEMENT);

  validateEquals(sequentialTraversal.getActionSet(0).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(1).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(2).getNumberOfParallelActions(),1);
  validateEquals(sequentialTraversal.getActionSet(3).getNumberOfParallelActions(),1);

  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(0),1,sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition);
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(1),1,sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition);
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(2),1,sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition);
  validateEqualsWithParams1(sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition(3),0,sequentialTraversal.getActionSet(0).getAction(0)._cartesianPosition);
  validateEquals(sequentialTraversal.getActionSet(0).getAction(0)._id,7);
  #endif
}



#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
