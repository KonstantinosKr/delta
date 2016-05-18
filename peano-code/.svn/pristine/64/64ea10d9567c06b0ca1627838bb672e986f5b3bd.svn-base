#include "peano/datatraversal/tests/dForRangeTest.h"
#include "peano/datatraversal/dForRange.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::datatraversal::tests::dForRangeTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


peano::datatraversal::tests::dForRangeTest::dForRangeTest():
  TestCase( "peano::datatraversal::tests::dForRangeTest" ) {
}


peano::datatraversal::tests::dForRangeTest::~dForRangeTest() {
}


void peano::datatraversal::tests::dForRangeTest::run() {
  testMethod( test2D10x10Range1 );
  testMethod( test2D10x10Range12 );
  testMethod( test2D10x10Range23 );
  testMethod( test2D10x10Range40 );
  testMethod( test2D10x10Range80 );
}


void peano::datatraversal::tests::dForRangeTest::setUp() {
}


void peano::datatraversal::tests::dForRangeTest::test2D10x10Range1() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int>  range(10);

  peano::datatraversal::dForRange testRange1(range,1);

  validate( testRange1.is_divisible() );

  peano::datatraversal::dForRange::Split split;

  peano::datatraversal::dForRange testRange1_1(testRange1, split);

  validateWithParams2( testRange1.is_divisible(), testRange1.toString(), testRange1_1.toString() );
  validateWithParams2( !testRange1.empty(), testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1.getOffset()(0), 5, testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1.getOffset()(1), 0, testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1.getRange()(0),  5, testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1.getRange()(1), 10, testRange1.toString(), testRange1_1.toString() );

  validateWithParams2( testRange1_1.is_divisible(), testRange1.toString(), testRange1_1.toString() );
  validateWithParams2( !testRange1_1.empty(), testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1_1.getOffset()(0), 0, testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1_1.getOffset()(1), 0, testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1_1.getRange()(0),  5, testRange1.toString(), testRange1_1.toString() );
  validateEqualsWithParams2( testRange1_1.getRange()(1), 10, testRange1.toString(), testRange1_1.toString() );

  #endif
}


void peano::datatraversal::tests::dForRangeTest::test2D10x10Range12() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int>  range(10);

  peano::datatraversal::dForRange testRange12(range,12);

  validate( testRange12.is_divisible() );

  peano::datatraversal::dForRange::Split split;

  peano::datatraversal::dForRange testRange12_1(testRange12, split);

  validateWithParams2( testRange12.is_divisible(), testRange12.toString(), testRange12_1.toString() );
  validateWithParams2( !testRange12.empty(), testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12.getOffset()(0), 5, testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12.getOffset()(1), 0, testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12.getRange()(0),  5, testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12.getRange()(1), 10, testRange12.toString(), testRange12_1.toString() );

  validateWithParams2( testRange12_1.is_divisible(), testRange12.toString(), testRange12_1.toString() );
  validateWithParams2( !testRange12_1.empty(), testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12_1.getOffset()(0), 0, testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12_1.getOffset()(1), 0, testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12_1.getRange()(0),  5, testRange12.toString(), testRange12_1.toString() );
  validateEqualsWithParams2( testRange12_1.getRange()(1), 10, testRange12.toString(), testRange12_1.toString() );

  #endif
}


void peano::datatraversal::tests::dForRangeTest::test2D10x10Range23() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int>  range(10);

  peano::datatraversal::dForRange testRange23(range,23);

  validate( testRange23.is_divisible() );

  peano::datatraversal::dForRange::Split split;

  peano::datatraversal::dForRange testRange23_1(testRange23, split);

  validateWithParams2( testRange23.is_divisible(), testRange23.toString(), testRange23_1.toString() );
  validateWithParams2( !testRange23.empty(), testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23.getOffset()(0), 5, testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23.getOffset()(1), 0, testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23.getRange()(0),  5, testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23.getRange()(1), 10, testRange23.toString(), testRange23_1.toString() );

  validateWithParams2( testRange23_1.is_divisible(), testRange23.toString(), testRange23_1.toString() );
  validateWithParams2( !testRange23_1.empty(), testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23_1.getOffset()(0), 0, testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23_1.getOffset()(1), 0, testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23_1.getRange()(0),  5, testRange23.toString(), testRange23_1.toString() );
  validateEqualsWithParams2( testRange23_1.getRange()(1), 10, testRange23.toString(), testRange23_1.toString() );

  #endif
}


void peano::datatraversal::tests::dForRangeTest::test2D10x10Range40() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int>  range(10);

  peano::datatraversal::dForRange testRange40(range,40);

  validate( testRange40.is_divisible() );

  peano::datatraversal::dForRange::Split split;

  peano::datatraversal::dForRange testRange40_1(testRange40, split);

  validateWithParams2( testRange40.is_divisible(), testRange40.toString(), testRange40_1.toString() );
  validateWithParams2( !testRange40.empty(), testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40.getOffset()(0), 5, testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40.getOffset()(1), 0, testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40.getRange()(0),  5, testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40.getRange()(1), 10, testRange40.toString(), testRange40_1.toString() );

  validateWithParams2( testRange40_1.is_divisible(), testRange40.toString(), testRange40_1.toString() );
  validateWithParams2( !testRange40_1.empty(), testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40_1.getOffset()(0), 0, testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40_1.getOffset()(1), 0, testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40_1.getRange()(0),  5, testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40_1.getRange()(1), 10, testRange40.toString(), testRange40_1.toString() );

  peano::datatraversal::dForRange testRange40_2(testRange40, split);
  peano::datatraversal::dForRange testRange40_3(testRange40_1, split);

  validateWithParams2( !testRange40_2.is_divisible(), testRange40.toString(), testRange40_1.toString() );
  validateWithParams2( !testRange40_2.empty(), testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40_2.getOffset()(0), 5, testRange40.toString(), testRange40_2.toString() );
  validateEqualsWithParams2( testRange40_2.getOffset()(1), 0, testRange40.toString(), testRange40_2.toString() );
  validateEqualsWithParams2( testRange40_2.getRange()(0),  5, testRange40.toString(), testRange40_2.toString() );
  validateEqualsWithParams2( testRange40_2.getRange()(1),  5, testRange40.toString(), testRange40_2.toString() );

  validateWithParams2( !testRange40.is_divisible(), testRange40.toString(), testRange40_1.toString() );
  validateWithParams2( !testRange40.empty(), testRange40.toString(), testRange40_1.toString() );
  validateEqualsWithParams2( testRange40.getOffset()(0), 5, testRange40.toString(), testRange40_2.toString() );
  validateEqualsWithParams2( testRange40.getOffset()(1), 5, testRange40.toString(), testRange40_2.toString() );
  validateEqualsWithParams2( testRange40.getRange()(0),  5, testRange40.toString(), testRange40_2.toString() );
  validateEqualsWithParams2( testRange40.getRange()(1),  5, testRange40.toString(), testRange40_2.toString() );
  #endif
}


void peano::datatraversal::tests::dForRangeTest::test2D10x10Range80() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int>  range(10);

  peano::datatraversal::dForRange testRange80(range,80);

  validate( testRange80.is_divisible() );

  peano::datatraversal::dForRange::Split split;

  peano::datatraversal::dForRange testRange80_1(testRange80, split);

  validateWithParams2( !testRange80.is_divisible(), testRange80.toString(), testRange80_1.toString() );
  validateWithParams2( !testRange80.empty(), testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80.getOffset()(0), 5, testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80.getOffset()(1), 0, testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80.getRange()(0),  5, testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80.getRange()(1), 10, testRange80.toString(), testRange80_1.toString() );

  validateWithParams2( !testRange80_1.is_divisible(), testRange80.toString(), testRange80_1.toString() );
  validateWithParams2( !testRange80_1.empty(), testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80_1.getOffset()(0), 0, testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80_1.getOffset()(1), 0, testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80_1.getRange()(0),  5, testRange80.toString(), testRange80_1.toString() );
  validateEqualsWithParams2( testRange80_1.getRange()(1), 10, testRange80.toString(), testRange80_1.toString() );
  #endif
}

#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
