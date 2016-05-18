#include "peano/grid/tests/TraversalOrderOnTopLevelTest.h"
#include "peano/grid/TraversalOrderOnTopLevel.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::TraversalOrderOnTopLevelTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif

tarch::logging::Log peano::grid::tests::TraversalOrderOnTopLevelTest::_log( "peano::grid::tests::TraversalOrderOnTopLevelTest" );


peano::grid::tests::TraversalOrderOnTopLevelTest::TraversalOrderOnTopLevelTest():
  tarch::tests::TestCase( "peano::grid::tests::TraversalOrderOnTopLevelTest" ) {
}


peano::grid::tests::TraversalOrderOnTopLevelTest::~TraversalOrderOnTopLevelTest() {
}


void peano::grid::tests::TraversalOrderOnTopLevelTest::run() {
  logTraceIn( "run() ");
  testMethod( test2DReconstruction );
  logTraceOut( "run() ");
}



void peano::grid::tests::TraversalOrderOnTopLevelTest::setUp() {
}


void peano::grid::tests::TraversalOrderOnTopLevelTest::test2DReconstruction() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int> result;

  std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT> inputBitset;

  inputBitset.set(0,true);
  inputBitset.set(1,false);
  inputBitset.set(2,false);
  inputBitset.set(3,false);

  result = peano::grid::TraversalOrderOnTopLevel::getCellIdentifiedByReceivedBitset( inputBitset );

  validateEqualsWithParams1(result(0),2,result);
  validateEqualsWithParams1(result(1),2,result);

  inputBitset.set(0,true);
  inputBitset.set(1,true);
  inputBitset.set(2,false);
  inputBitset.set(3,false);

  result = peano::grid::TraversalOrderOnTopLevel::getCellIdentifiedByReceivedBitset( inputBitset );

  validateEqualsWithParams1(result(0),1,result);
  validateEqualsWithParams1(result(1),2,result);

  inputBitset.set(0,false);
  inputBitset.set(1,true);
  inputBitset.set(2,false);
  inputBitset.set(3,true);

  result = peano::grid::TraversalOrderOnTopLevel::getCellIdentifiedByReceivedBitset( inputBitset );

  validateEqualsWithParams1(result(0),0,result);
  validateEqualsWithParams1(result(1),1,result);

  inputBitset.set(0,true);
  inputBitset.set(1,true);
  inputBitset.set(2,true);
  inputBitset.set(3,true);

  result = peano::grid::TraversalOrderOnTopLevel::getCellIdentifiedByReceivedBitset( inputBitset );

  validateEqualsWithParams1(result(0),1,result);
  validateEqualsWithParams1(result(1),1,result);

  #endif
}
