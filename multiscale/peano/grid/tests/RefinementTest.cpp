#include "peano/grid/tests/RefinementTest.h"
#include "peano/grid/aspects/CellRefinement.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::grid::tests::RefinementTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


tarch::logging::Log peano::grid::tests::RefinementTest::_log( "peano::grid::tests::RefinementTest" );


peano::grid::tests::RefinementTest::RefinementTest():
  TestCase( "peano::grid::tests::RefinementTest" ) {
}


peano::grid::tests::RefinementTest::~RefinementTest() {
}


void peano::grid::tests::RefinementTest::run() {
  logTraceIn( "run() ");
  testMethod( testRootNodeRefinementCells );
  testMethod( test2DCellM1M2P0P0 );
  logTraceOut( "run() ");
}


void peano::grid::tests::RefinementTest::testRootNodeRefinementCells() {
  #ifdef Dim2
  TestCell coarseGridTestCell;
  TestCell fineGridTestCell[THREE_POWER_D];

  validateEqualsWithParams1( coarseGridTestCell._cellData.getAccessNumber(0),0,  coarseGridTestCell);
  validateEqualsWithParams1( coarseGridTestCell._cellData.getAccessNumber(1),0,  coarseGridTestCell);
  validateEqualsWithParams1( coarseGridTestCell._cellData.getAccessNumber(2),0,  coarseGridTestCell);
  validateEqualsWithParams1( coarseGridTestCell._cellData.getAccessNumber(3),0,  coarseGridTestCell);
  validateEqualsWithParams1( coarseGridTestCell._cellData.getEvenFlags(0),false, coarseGridTestCell);
  validateEqualsWithParams1( coarseGridTestCell._cellData.getEvenFlags(1),false, coarseGridTestCell);

  coarseGridTestCell.switchToRoot(0);
  peano::grid::aspects::CellRefinement::refine(coarseGridTestCell,fineGridTestCell);

  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(0),0,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(1),0,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(2),1,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(3),2,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getEvenFlags(0),false, fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(0),-1, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(1), 0, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(2), 1, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(3), 2, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getEvenFlags(0),true,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(0),-1, fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(1),0,  fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(2),0,  fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(3),1,  fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getEvenFlags(0),false, fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(0),0,  fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(1),-2, fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(2),-1, fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(3),1,  fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getEvenFlags(0),false, fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getEvenFlags(1),true,  fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(0),1,  fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(1),-2, fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(2),-1, fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(3),2,  fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getEvenFlags(0),true,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getEvenFlags(1),true,  fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(0),1,  fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(1),-1, fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(2),0,  fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(3),2,  fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getEvenFlags(0),false, fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getEvenFlags(1),true,  fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(0),0,  fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(1),-1, fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(2),1,  fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(3),0,  fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getEvenFlags(0),false, fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(0),-1, fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(1),-2, fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(2),1,  fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(3),0,  fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getEvenFlags(0),true,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(0),-1, fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(1),-2, fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(2),0,  fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(3),0,  fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getEvenFlags(0),false, fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);
  #endif
}



void peano::grid::tests::RefinementTest::test2DCellM1M2P0P0() {
  #ifdef Dim2
  TestCell coarseGridTestCell;
  TestCell fineGridTestCell[THREE_POWER_D];

  coarseGridTestCell._cellData.setAccessNumber(0,-1);
  coarseGridTestCell._cellData.setAccessNumber(1,-2);
  coarseGridTestCell._cellData.setAccessNumber(2,0);
  coarseGridTestCell._cellData.setAccessNumber(3,0);
  coarseGridTestCell._cellData.setEvenFlags(0,false);
  coarseGridTestCell._cellData.setEvenFlags(1,false);

  peano::grid::aspects::CellRefinement::refine(coarseGridTestCell,fineGridTestCell);

  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(0),-1,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(1),-2,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(2), 1,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getAccessNumber(3), 2,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getEvenFlags(0),false,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[0]._cellData.getEvenFlags(1),false,  fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(0),-1, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(1),-2, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(2), 1, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getAccessNumber(3), 2, fineGridTestCell[1]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getEvenFlags(0),true,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[1]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(0),-1,  fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(1),-2,  fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(2), 0,  fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getAccessNumber(3), 1,  fineGridTestCell[2]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getEvenFlags(0),false,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[2]._cellData.getEvenFlags(1),false,  fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(0),-3,  fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(1),-2,  fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(2),-1,  fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getAccessNumber(3), 1,  fineGridTestCell[3]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getEvenFlags(0),false,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[3]._cellData.getEvenFlags(1),true,   fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(0), 1,  fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(1),-2,  fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(2),-1,  fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getAccessNumber(3), 2,  fineGridTestCell[4]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getEvenFlags(0),true,   fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[4]._cellData.getEvenFlags(1),true,   fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(0), 1,  fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(1),-1,  fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(2), 0,  fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getAccessNumber(3), 2,  fineGridTestCell[5]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getEvenFlags(0),false,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[5]._cellData.getEvenFlags(1),true,   fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(0),-2,  fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(1),-1,  fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(2), 1,  fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getAccessNumber(3), 0,  fineGridTestCell[6]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getEvenFlags(0),false,  fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[6]._cellData.getEvenFlags(1),false,  fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(0), -1,  fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(1), -2,  fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(2),  1,  fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getAccessNumber(3),  0,  fineGridTestCell[7]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getEvenFlags(0),true,    fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[7]._cellData.getEvenFlags(1),false,   fineGridTestCell[0]);

  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(0),-1, fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(1),-2, fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(2),0,  fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getAccessNumber(3),0,  fineGridTestCell[8]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getEvenFlags(0),false, fineGridTestCell[0]);
  validateEqualsWithParams1( fineGridTestCell[8]._cellData.getEvenFlags(1),false, fineGridTestCell[0]);
  #endif
}



#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
