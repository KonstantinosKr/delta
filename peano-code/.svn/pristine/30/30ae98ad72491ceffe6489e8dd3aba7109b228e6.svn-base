#include "peano/grid/tests/ForkRegularTreeTest.h"
#include "peano/grid/RegularGridContainer.h"
#include "peano/grid/nodes/RegularRefined.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::ForkRegularTreeTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif



tarch::logging::Log peano::grid::tests::ForkRegularTreeTest::_log( "peano::grid::tests::ForkRegularTreeTest" );


peano::grid::tests::ForkRegularTreeTest::ForkRegularTreeTest():
  TestCase( "peano::grid::tests::ForkRegularTreeTest" ) {
}


peano::grid::tests::ForkRegularTreeTest::~ForkRegularTreeTest() {
}



void peano::grid::tests::ForkRegularTreeTest::run() {
  logTraceIn( "run() ");
  testMethod( testMayForkLoadOrStoreVertexTaskOnRegularSubtree );
  logTraceOut( "run() ");
}


void peano::grid::tests::ForkRegularTreeTest::testMayForkLoadOrStoreVertexTaskOnRegularSubtree() {
  #ifdef Dim2
  validateEquals( peano::grid::nodes::transformOracleResult(0,1,9), -1 );
  validateEquals( peano::grid::nodes::transformOracleResult(0,2,9*9), -1 );
  validateEquals( peano::grid::nodes::transformOracleResult(0,3,9*9*9), -1 );
  validateEquals( peano::grid::nodes::transformOracleResult(0,4,9*9*9*9), -1 );

  validateEquals( peano::grid::nodes::transformOracleResult(9,1,9), 0 );
  validateEquals( peano::grid::nodes::transformOracleResult(5,1,9), 1 );

  validateEquals( peano::grid::nodes::transformOracleResult(8,4,9*9*9*9), 4 );

  validateEquals( mayForkLoadOrStoreVertexTaskOnRegularSubtree(1,0,2,0,9),true);
  #endif
}



#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
