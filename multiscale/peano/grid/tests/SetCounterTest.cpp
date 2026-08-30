#include "peano/grid/tests/SetCounterTest.h"
#include "peano/grid/nodes/loops/SetCounterLoopBody.h"
#include "peano/grid/nodes/Constants.h"

#include "peano/utils/Globals.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::SetCounterTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif



tarch::logging::Log peano::grid::tests::SetCounterTest::_log( "peano::grid::tests::SetCounterTest" );


peano::grid::tests::SetCounterTest::SetCounterTest():
  tarch::tests::TestCase( "peano::grid::tests::SetCounterTest" ) {
}


peano::grid::tests::SetCounterTest::~SetCounterTest() {
}


void peano::grid::tests::SetCounterTest::run() {
  logTraceIn( "run() ");
  testMethod( test2DSpacetreeConstructionFirstLevel );
  testMethod( isOnBoundaryTest );
  testMethod( testAPosterioriRefinement2d );
  testMethod( testOptimisedAnalysis );
  testMethod( testRefinementDueToJoinThoughWorkerIsAlreadyErasing0 );
  testMethod( testRefinementDueToJoinThoughWorkerIsAlreadyErasing1 );
  testMethod( test3DLevel1 );

  logTraceOut( "run() ");
}


void peano::grid::tests::SetCounterTest::setUp() {
}


void peano::grid::tests::SetCounterTest::test3DLevel1() {
  #ifdef Dim3
  TestVertex coarseGridVertices[FOUR_POWER_D];

  peano::grid::SingleLevelEnumerator   coarseGridVerticesEnumerator(
    peano::grid::SingleLevelEnumerator::Vector( 3.0),
    peano::grid::SingleLevelEnumerator::Vector(-1.0),
    0
  );
  int                                                   fineGridCounter[FOUR_POWER_D];

  coarseGridVertices[0].switchToNonhangingNode();
  coarseGridVertices[0]._vertexData.setRefinementControl( TestVertex::Records::Refined);
//  coarseGridVertices[1].switchToNonhangingNode();
//  coarseGridVertices[1]._vertexData.setRefinementControl( TestVertex::Records::Refined);
  coarseGridVertices[2].switchToNonhangingNode();
  coarseGridVertices[2]._vertexData.setRefinementControl( TestVertex::Records::Refined);
//  coarseGridVertices[3].switchToNonhangingNode();
//  coarseGridVertices[3]._vertexData.setRefinementControl( TestVertex::Records::Refined);
  coarseGridVertices[4].switchToNonhangingNode();
  coarseGridVertices[4]._vertexData.setRefinementControl( TestVertex::Records::Refined);
//  coarseGridVertices[5].switchToNonhangingNode();
//  coarseGridVertices[5]._vertexData.setRefinementControl( TestVertex::Records::Refined);
  coarseGridVertices[6].switchToNonhangingNode();
  coarseGridVertices[6]._vertexData.setRefinementControl( TestVertex::Records::Refined);
//  coarseGridVertices[7].switchToNonhangingNode();
//  coarseGridVertices[7]._vertexData.setRefinementControl( TestVertex::Records::Refined);

  peano::grid::nodes::loops::SetCounterLoopBody<TestVertex> loopBody(coarseGridVertices,coarseGridVerticesEnumerator,fineGridCounter);

  tarch::la::Vector<3,int> vertex;
  int indexInCounterArray;

  vertex = 0,0,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 1,0,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 2,0,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 3,0,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 0,1,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 1,1,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 2,1,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 3,1,0;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 0,1,2;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 1,1,2;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 2,1,2;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  vertex = 3,1,2;
  indexInCounterArray = peano::grid::SingleLevelEnumerator::lineariseVertexIndex(vertex);
  loopBody(vertex);
  validateEqualsWithParams3( fineGridCounter[indexInCounterArray], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[indexInCounterArray]), fineGridCounter[indexInCounterArray], vertex );

  #endif
}

void peano::grid::tests::SetCounterTest::testRefinementDueToJoinThoughWorkerIsAlreadyErasing0() {
  #if defined(Dim2) && defined(Parallel)
  TestVertex                                            coarseGridVertices[FOUR_POWER_D];
  peano::grid::SingleLevelEnumerator   coarseGridVerticesEnumerator(
    peano::grid::SingleLevelEnumerator::Vector( 3.0),
    peano::grid::SingleLevelEnumerator::Vector(-1.0),
    0
  );
  int                                                   fineGridCounter[FOUR_POWER_D];

  coarseGridVertices[0].switchToNonhangingNode();
  coarseGridVertices[0]._vertexData.setRefinementControl( TestVertex::Records::RefineDueToJoinThoughWorkerIsAlreadyErasing);

  coarseGridVertices[1].switchToNonhangingNode();
  coarseGridVertices[1]._vertexData.setRefinementControl( TestVertex::Records::Unrefined);

  coarseGridVertices[4].switchToNonhangingNode();
  coarseGridVertices[4]._vertexData.setRefinementControl( TestVertex::Records::Unrefined);

  coarseGridVertices[5].switchToNonhangingNode();
  coarseGridVertices[5]._vertexData.setRefinementControl( TestVertex::Records::Unrefined);

  peano::grid::nodes::loops::SetCounterLoopBody<TestVertex> loopBody(coarseGridVertices,coarseGridVerticesEnumerator,fineGridCounter);

  tarch::la::Vector<2,int> vertex;

  vertex = 0,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 0], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 0]), fineGridCounter[ 0] );

  vertex = 1,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 1], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 1]), fineGridCounter[ 1] );

  vertex = 2,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 2], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 2]), fineGridCounter[ 2] );

  vertex = 3,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 3], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[ 3]), fineGridCounter[ 3] );

  vertex = 0,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 4], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 4]), fineGridCounter[ 4] );

  vertex = 1,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 5], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 5]), fineGridCounter[ 5] );

  vertex = 2,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 6], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 6]), fineGridCounter[ 6] );

  vertex = 3,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 7], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[ 7]), fineGridCounter[ 7] );

  vertex = 0,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 8], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 8]), fineGridCounter[ 8] );

  vertex = 1,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 9], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 9]), fineGridCounter[ 9] );

  vertex = 2,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[10], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[10]), fineGridCounter[10] );

  vertex = 3,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[11], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[11]), fineGridCounter[11] );

  vertex = 0,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[12], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[12]), fineGridCounter[12] );

  vertex = 1,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[13], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[13]), fineGridCounter[13] );

  vertex = 2,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[14], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[14]), fineGridCounter[14] );

  vertex = 3,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[15], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[15]), fineGridCounter[15] );
  #endif
}


void peano::grid::tests::SetCounterTest::testRefinementDueToJoinThoughWorkerIsAlreadyErasing1() {
  #if defined(Dim2) && defined(Parallel)
  TestVertex                                            coarseGridVertices[FOUR_POWER_D];
  peano::grid::SingleLevelEnumerator   coarseGridVerticesEnumerator(
    peano::grid::SingleLevelEnumerator::Vector( 3.0),
    peano::grid::SingleLevelEnumerator::Vector(-1.0),
    0
  );
  int                                                   fineGridCounter[FOUR_POWER_D];

  coarseGridVertices[0].switchToNonhangingNode();
  coarseGridVertices[0]._vertexData.setRefinementControl( TestVertex::Records::Unrefined);

  coarseGridVertices[1].switchToNonhangingNode();
  coarseGridVertices[1]._vertexData.setRefinementControl( TestVertex::Records::RefineDueToJoinThoughWorkerIsAlreadyErasing);

  coarseGridVertices[4].switchToNonhangingNode();
  coarseGridVertices[4]._vertexData.setRefinementControl( TestVertex::Records::Unrefined);

  coarseGridVertices[5].switchToNonhangingNode();
  coarseGridVertices[5]._vertexData.setRefinementControl( TestVertex::Records::Unrefined);

  peano::grid::nodes::loops::SetCounterLoopBody<TestVertex> loopBody(coarseGridVertices,coarseGridVerticesEnumerator,fineGridCounter);

  tarch::la::Vector<2,int> vertex;

  vertex = 0,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 0], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[ 0]), fineGridCounter[ 0] );

  vertex = 1,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 1], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 1]), fineGridCounter[ 1] );

  vertex = 2,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 2], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 2]), fineGridCounter[ 2] );

  vertex = 3,0;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 3], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 3]), fineGridCounter[ 3] );

  vertex = 0,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 4], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[ 4]), fineGridCounter[ 4] );

  vertex = 1,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 5], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 5]), fineGridCounter[ 5] );

  vertex = 2,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 6], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 6]), fineGridCounter[ 6] );

  vertex = 3,1;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 7], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 7]), fineGridCounter[ 7] );

  vertex = 0,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 8], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[ 8]), fineGridCounter[ 8] );

  vertex = 1,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[ 9], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[ 9]), fineGridCounter[ 9] );

  vertex = 2,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[10], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[10]), fineGridCounter[10] );

  vertex = 3,2;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[11], peano::grid::nodes::CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing, peano::grid::nodes::counterToString(fineGridCounter[11]), fineGridCounter[11] );

  vertex = 0,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[12], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[12]), fineGridCounter[12] );

  vertex = 1,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[13], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[13]), fineGridCounter[13] );

  vertex = 2,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[14], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[14]), fineGridCounter[14] );

  vertex = 3,3;
  loopBody(vertex);
  validateEqualsWithParams2( fineGridCounter[15], peano::grid::nodes::CounterHangingNode,                                        peano::grid::nodes::counterToString(fineGridCounter[15]), fineGridCounter[15] );
  #endif
}


void peano::grid::tests::SetCounterTest::testOptimisedAnalysis() {
//  bool oneFatherCarriesDeleteFlag   = false;
//  bool oneFatherCarriesRefiningFlag = false;
//  bool oneFatherCarriesRefinedFlag  = false;
//  bool oneFatherCarriesNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasingFlag  = false;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag,oneFatherCarriesNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasingFlag) );
//
//  oneFatherCarriesDeleteFlag   = true;
//  oneFatherCarriesRefiningFlag = false;
//  oneFatherCarriesRefinedFlag  = false;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) );
//
//  oneFatherCarriesDeleteFlag   = false;
//  oneFatherCarriesRefiningFlag = true;
//  oneFatherCarriesRefinedFlag  = false;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) );
//
//  oneFatherCarriesDeleteFlag   = true;
//  oneFatherCarriesRefiningFlag = true;
//  oneFatherCarriesRefinedFlag  = false;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) );
//
//  oneFatherCarriesDeleteFlag   = false;
//  oneFatherCarriesRefiningFlag = false;
//  oneFatherCarriesRefinedFlag  = true;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) );
//
//  oneFatherCarriesDeleteFlag   = true;
//  oneFatherCarriesRefiningFlag = false;
//  oneFatherCarriesRefinedFlag  = true;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) );
//
//  oneFatherCarriesDeleteFlag   = false;
//  oneFatherCarriesRefiningFlag = true;
//  oneFatherCarriesRefinedFlag  = true;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) );
//
//  oneFatherCarriesDeleteFlag   = true;
//  oneFatherCarriesRefiningFlag = true;
//  oneFatherCarriesRefinedFlag  = true;
//  validateEquals( peano::grid::nodes::loops::SetCounterLoopBody<TestVertex>::analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) , analyseFineGridVertex(oneFatherCarriesDeleteFlag,oneFatherCarriesRefiningFlag,oneFatherCarriesRefinedFlag) );
}


void peano::grid::tests::SetCounterTest::isOnBoundaryTest() {
  logTraceIn ("isOnBoundaryTest()" );
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int>  vertex;

  tarch::la::assignList(vertex) = 3,1;

  bool isOnBoundary = false;
  for (int d=0; d<DIMENSIONS; d++) {
    isOnBoundary |= (vertex(d)==0);
    isOnBoundary |= (vertex(d)==3);
  }
  validate( isOnBoundary );

  const tarch::la::Vector<DIMENSIONS,int>&  vertex2(vertex);
  isOnBoundary = false;
  for (int d=0; d<DIMENSIONS; d++) {
    isOnBoundary |= (vertex2(d)==0);
    isOnBoundary |= (vertex2(d)==3);
  }
  validate( isOnBoundary );

  tarch::la::Vector<DIMENSIONS,int>&  vertex3 = vertex;
  isOnBoundary = false;
  for (int d=0; d<DIMENSIONS; d++) {
    isOnBoundary |= (vertex3(d)==0);
    isOnBoundary |= (vertex3(d)==3);
  }
  validate( isOnBoundary );
  #endif
  logTraceOut("isOnBoundaryTest()" );
}


void peano::grid::tests::SetCounterTest::testAPosterioriRefinement2d() {
  logTraceIn ("testAPosterioriRefinement2d()" );
  #ifdef Dim2
  TestVertex                                            coarseGridVertices[FOUR_POWER_D];
  peano::grid::SingleLevelEnumerator   coarseGridVerticesEnumerator(
    peano::grid::SingleLevelEnumerator::Vector( 3.0),
    peano::grid::SingleLevelEnumerator::Vector(-1.0),
    0
  );
  int                                                   fineGridCounter[FOUR_POWER_D];
  tarch::la::Vector<DIMENSIONS,int>                     vertex;

  coarseGridVertices[0].switchToNonhangingNode();
  coarseGridVertices[0].refine();
  coarseGridVertices[0].switchRefinementTriggeredToRefining();

  coarseGridVertices[1].switchToNonhangingNode();

  coarseGridVertices[4].switchToNonhangingNode();
  coarseGridVertices[4].refine();
  coarseGridVertices[4].switchRefinementTriggeredToRefining();
  coarseGridVertices[4].updateTransientRefinementFlagsBeforeVertexIsStoredToOutputStack();

  coarseGridVertices[5].switchToNonhangingNode();

  peano::grid::nodes::loops::SetCounterLoopBody<TestVertex> loopBody(coarseGridVertices,coarseGridVerticesEnumerator,fineGridCounter);

  tarch::la::assignList(vertex) = 0,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[0], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[0]) );

  tarch::la::assignList(vertex) = 1,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[1], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[1]) );

  tarch::la::assignList(vertex) = 2,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[2], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[2]) );

  tarch::la::assignList(vertex) = 3,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[3], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[3]) );

  tarch::la::assignList(vertex) = 0,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[4], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[4]) );

  tarch::la::assignList(vertex) = 1,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[5], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[5]) );

  tarch::la::assignList(vertex) = 2,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[6], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[6]) );

  tarch::la::assignList(vertex) = 3,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[7], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[7]) );

  tarch::la::assignList(vertex) = 0,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[8], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[8]) );

  tarch::la::assignList(vertex) = 1,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[9], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[9]) );

  tarch::la::assignList(vertex) = 2,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[10], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[10]) );

  tarch::la::assignList(vertex) = 3,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[11], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[11]) );

  tarch::la::assignList(vertex) = 0,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[12], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[12]) );

  tarch::la::assignList(vertex) = 1,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[13], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[13]) );

  tarch::la::assignList(vertex) = 2,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[14], peano::grid::nodes::CounterPersistentNode, peano::grid::nodes::counterToString(fineGridCounter[14]) );

  tarch::la::assignList(vertex) = 3,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[15], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[15]) );
  #endif
  logTraceOut("testAPosterioriRefinement2d()" );
}


void peano::grid::tests::SetCounterTest::test2DSpacetreeConstructionFirstLevel() {
  logTraceIn ("test2DSpacetreeConstructionFirstLevel()" );
  #ifdef Dim2
  TestVertex                                            coarseGridVertices[FOUR_POWER_D];
  peano::grid::SingleLevelEnumerator   coarseGridVerticesEnumerator(
    peano::grid::SingleLevelEnumerator::Vector( 3.0),
    peano::grid::SingleLevelEnumerator::Vector(-1.0),
    0
  );
  int                                                   fineGridCounter[FOUR_POWER_D];
  tarch::la::Vector<DIMENSIONS,int>                     vertex;

  peano::grid::nodes::loops::SetCounterLoopBody<TestVertex> loopBody(coarseGridVertices,coarseGridVerticesEnumerator,fineGridCounter);

  #ifdef Asserts
  coarseGridVertices[0].setPosition( 1.0, 0 );
  coarseGridVertices[1].setPosition( 2.0, 1 );
  coarseGridVertices[2].setPosition( 3.0, 2 );
  coarseGridVertices[3].setPosition( 4.0, 3 );
  coarseGridVertices[4].setPosition( 5.0, 4 );
  #endif

  validate( coarseGridVertices[0].isHangingNode() );
  validate( coarseGridVertices[1].isHangingNode() );
  validate( coarseGridVertices[2].isHangingNode() );
  validate( coarseGridVertices[3].isHangingNode() );
  validate( coarseGridVertices[4].isHangingNode() );


  coarseGridVertices[5].switchToNonhangingNode();
  coarseGridVertices[5].refine();
  coarseGridVertices[5].switchRefinementTriggeredToRefining();
  coarseGridVertices[6].switchToNonhangingNode();
  coarseGridVertices[6].refine();
  coarseGridVertices[6].switchRefinementTriggeredToRefining();
  validate( coarseGridVertices[7].isHangingNode() );
  validate( coarseGridVertices[8].isHangingNode() );
  coarseGridVertices[9].switchToNonhangingNode();
  coarseGridVertices[9].refine();
  coarseGridVertices[9].switchRefinementTriggeredToRefining();
  coarseGridVertices[10].switchToNonhangingNode();
  coarseGridVertices[10].refine();
  coarseGridVertices[10].switchRefinementTriggeredToRefining();
  validate( coarseGridVertices[11].isHangingNode() );
  validate( coarseGridVertices[12].isHangingNode() );
  validate( coarseGridVertices[13].isHangingNode() );
  validate( coarseGridVertices[14].isHangingNode() );
  validate( coarseGridVertices[15].isHangingNode() );

  tarch::la::assignList(vertex) = 0,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[0], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[0]) );

  tarch::la::assignList(vertex) = 1,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[1], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[1]) );

  tarch::la::assignList(vertex) = 2,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[2], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[2]) );

  tarch::la::assignList(vertex) = 3,0;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[3], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[3]) );

  tarch::la::assignList(vertex) = 0,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[4], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[4]) );

  tarch::la::assignList(vertex) = 1,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[5], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[5]) );

  tarch::la::assignList(vertex) = 2,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[6], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[6]) );

  tarch::la::assignList(vertex) = 3,1;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[7], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[7]) );

  tarch::la::assignList(vertex) = 0,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[8], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[8]) );

  tarch::la::assignList(vertex) = 1,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[9], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[9]) );

  tarch::la::assignList(vertex) = 2,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[10], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[10]) );

  tarch::la::assignList(vertex) = 3,2;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[11], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[11]) );

  tarch::la::assignList(vertex) = 0,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[12], peano::grid::nodes::CounterHangingNode, peano::grid::nodes::counterToString(fineGridCounter[12]) );

  tarch::la::assignList(vertex) = 1,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[13], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[13]) );

  tarch::la::assignList(vertex) = 2,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[14], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[14]) );

  tarch::la::assignList(vertex) = 3,3;
  loopBody(vertex);
  validateEqualsWithParams1( fineGridCounter[15], peano::grid::nodes::CounterNewNode, peano::grid::nodes::counterToString(fineGridCounter[15]) );
  #endif
  logTraceOut("test2DSpacetreeConstructionFirstLevel()" );
}


int peano::grid::tests::SetCounterTest::analyseFineGridVertex(
  bool oneFatherCarriesDeleteFlag,
  bool oneFatherCarriesRefiningFlag,
  bool oneFatherCarriesRefinedFlag
) const {
  logTraceIn ("analyseFineGridVertex()" );
  if ( oneFatherCarriesRefiningFlag &&  oneFatherCarriesRefinedFlag &&  oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterPersistentNode;
  }
  else if ( oneFatherCarriesRefiningFlag &&  oneFatherCarriesRefinedFlag && !oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterPersistentNode;
  }
  else if ( oneFatherCarriesRefiningFlag && !oneFatherCarriesRefinedFlag &&  oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterPersistentNode;
  }
  else if ( oneFatherCarriesRefiningFlag && !oneFatherCarriesRefinedFlag && !oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterNewNode;
  }
  else if (!oneFatherCarriesRefiningFlag &&  oneFatherCarriesRefinedFlag &&  oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterPersistentNode;
  }
  else if (!oneFatherCarriesRefiningFlag &&  oneFatherCarriesRefinedFlag && !oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterPersistentNode;
  }
  else if (!oneFatherCarriesRefiningFlag && !oneFatherCarriesRefinedFlag &&  oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterPersistentNodeDelete;
  }
  else if (!oneFatherCarriesRefiningFlag && !oneFatherCarriesRefinedFlag && !oneFatherCarriesDeleteFlag) {
    return peano::grid::nodes::CounterHangingNode;
  }

  assertionMsg(false, "all cases handled in the if construct from above");

  logTraceOut("analyseFineGridVertex()" );
  return peano::grid::nodes::CounterPersistentNode;
}




#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
