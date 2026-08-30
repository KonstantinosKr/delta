#include "peano/datatraversal/tests/dForLoopTest.h"

#include "peano/datatraversal/dForLoop.h"
#include "tarch/multicore/Lock.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::datatraversal::tests::dForLoopTest)


tarch::logging::Log peano::datatraversal::tests::dForLoopTest::_log("peano::datatraversal::tests::dForLoopTest");

#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif

peano::datatraversal::tests::dForLoopTest::dForLoopTest()
{
}

peano::datatraversal::tests::dForLoopTest::~dForLoopTest()
{
}

void peano::datatraversal::tests::dForLoopTest::run()
{
  testMethod( testCreateRangesVectorGrainSize1 );
  testMethod( testParallelReduction );
}


void peano::datatraversal::tests::dForLoopTest::setUp()
{
}

void peano::datatraversal::tests::dForLoopTest::testCreateRangesVectorGrainSize1()
{
/*
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS,int> range(4);
  int grainSize = 1;

  TestLoopBody testLoopBody;
  peano::datatraversal::dForLoop<TestLoopBody> loop(range, testLoopBody, grainSize, 1, true);

  std::vector<peano::datatraversal::dForRange> ranges = loop.createRangesVector(range, grainSize);

  validateEquals(16, ranges.size());

  for(int i = 0; i < static_cast<int>(ranges.size()); i++) {
    for(int d = 0; d < DIMENSIONS; d++){
      validateEquals(1, ranges[i].getRange()(d));
    }
  }

  validateEquals(3, ranges[0].getOffset()(0))
  validateEquals(3, ranges[0].getOffset()(1))
  validateEquals(1, ranges[1].getOffset()(0))
  validateEquals(3, ranges[1].getOffset()(1))
  validateEquals(3, ranges[2].getOffset()(0))
  validateEquals(1, ranges[2].getOffset()(1))
  validateEquals(1, ranges[3].getOffset()(0))
  validateEquals(1, ranges[3].getOffset()(1))
  validateEquals(2, ranges[4].getOffset()(0))
  validateEquals(3, ranges[4].getOffset()(1))
  validateEquals(0, ranges[5].getOffset()(0))
  validateEquals(3, ranges[5].getOffset()(1))
  validateEquals(2, ranges[6].getOffset()(0))
  validateEquals(1, ranges[6].getOffset()(1))
  validateEquals(0, ranges[7].getOffset()(0))
  validateEquals(1, ranges[7].getOffset()(1))
  validateEquals(3, ranges[8].getOffset()(0))
  validateEquals(2, ranges[8].getOffset()(1))
  validateEquals(1, ranges[9].getOffset()(0))
  validateEquals(2, ranges[9].getOffset()(1))
  validateEquals(3, ranges[10].getOffset()(0))
  validateEquals(0, ranges[10].getOffset()(1))
  validateEquals(1, ranges[11].getOffset()(0))
  validateEquals(0, ranges[11].getOffset()(1))
  validateEquals(2, ranges[12].getOffset()(0))
  validateEquals(2, ranges[12].getOffset()(1))
  validateEquals(0, ranges[13].getOffset()(0))
  validateEquals(2, ranges[13].getOffset()(1))
  validateEquals(2, ranges[14].getOffset()(0))
  validateEquals(0, ranges[14].getOffset()(1))
  validateEquals(0, ranges[15].getOffset()(0))
  validateEquals(0, ranges[15].getOffset()(1))
  #endif
*/
}


void peano::datatraversal::tests::dForLoopTest::testParallelReduction() {
/*
  tarch::la::Vector<DIMENSIONS,int> range;
  range(0) = 4;
  range(1) = 4;
  int grainSize = 4;

  std::vector<tarch::multicore::dForRange> ranges = peano::datatraversal::dForLoop<TestLoopBody>::createRangesVector(range, grainSize);

  validateEquals(ranges.size(), 4);

  validateWithParams1( !ranges[0].empty(),     ranges[0].toString());
  validateEqualsWithParams1( ranges[0].getOffset()(0), 2, ranges[0].toString());
  validateEqualsWithParams1( ranges[0].getOffset()(1), 2, ranges[0].toString());
  validateEqualsWithParams1( ranges[0].getRange()(0),  2, ranges[0].toString());
  validateEqualsWithParams1( ranges[0].getRange()(1),  2, ranges[0].toString());

  validateWithParams1( !ranges[1].empty(),ranges[1].toString());
  validateEqualsWithParams1( ranges[1].getOffset()(0), 0, ranges[1].toString());
  validateEqualsWithParams1( ranges[1].getOffset()(1), 2, ranges[1].toString());
  validateEqualsWithParams1( ranges[1].getRange()(0),  2, ranges[1].toString());
  validateEqualsWithParams1( ranges[1].getRange()(1),  2, ranges[1].toString());

  validateWithParams1( !ranges[2].empty(),ranges[2].toString());
  validateEqualsWithParams1( ranges[2].getOffset()(0), 2, ranges[2].toString());
  validateEqualsWithParams1( ranges[2].getOffset()(1), 0, ranges[2].toString());
  validateEqualsWithParams1( ranges[2].getRange()(0),  2, ranges[2].toString());
  validateEqualsWithParams1( ranges[2].getRange()(1),  2, ranges[2].toString());

  validateWithParams1( !ranges[3].empty(),ranges[3].toString());
  validateEqualsWithParams1( ranges[3].getOffset()(0), 0, ranges[3].toString());
  validateEqualsWithParams1( ranges[3].getOffset()(1), 0, ranges[3].toString());
  validateEqualsWithParams1( ranges[3].getRange()(0),  2, ranges[3].toString());
  validateEqualsWithParams1( ranges[3].getRange()(1),  2, ranges[3].toString());



  TestLoopBody testLoopBody;
  TestLoopBody::resetGlobalCounter();
  peano::datatraversal::dForLoop<TestLoopBody> loop(range, testLoopBody, grainSize, peano::datatraversal::dForLoop<TestLoopBody>::NoColouring, true);

  // we cannot say for sure whether the rank has been split or not. It depends
  // on the runtime behaviour. But we can check other things.
  validateEqualsWithParams5(TestLoopBody::_operatorCounter, 16, TestLoopBody::_operatorCounter,    TestLoopBody::_constructorCounter, TestLoopBody::_destructorCounter, grainSize, range);
  validateEqualsWithParams5(TestLoopBody::_constructorCounter, TestLoopBody::_destructorCounter, TestLoopBody::_operatorCounter, TestLoopBody::_constructorCounter, TestLoopBody::_destructorCounter, grainSize, range);
  validateWithParams5(TestLoopBody::_constructorCounter>=1, TestLoopBody::_operatorCounter, TestLoopBody::_constructorCounter, TestLoopBody::_destructorCounter, grainSize, range);

  #endif
*/
}




//TestLoopBody
int peano::datatraversal::tests::TestLoopBody::_constructorCounter;
int peano::datatraversal::tests::TestLoopBody::_operatorCounter;
int peano::datatraversal::tests::TestLoopBody::_destructorCounter;


tarch::multicore::BooleanSemaphore peano::datatraversal::tests::TestLoopBody::_semaphore;

peano::datatraversal::tests::TestLoopBody::TestLoopBody() {
  tarch::multicore::Lock lock(_semaphore);
  _constructorCounter++;
}


peano::datatraversal::tests::TestLoopBody::TestLoopBody(const TestLoopBody& copy) {
  tarch::multicore::Lock lock(_semaphore);
  _constructorCounter++;
}


peano::datatraversal::tests::TestLoopBody::~TestLoopBody() {
}


void peano::datatraversal::tests::TestLoopBody::mergeWithWorkerThread(const TestLoopBody& worker) {
  tarch::multicore::Lock lock(_semaphore);

  _destructorCounter++;
}


void peano::datatraversal::tests::TestLoopBody::mergeIntoMasterThread(TestLoopBody& master) const {
  tarch::multicore::Lock lock(_semaphore);

  _destructorCounter++;
}


void peano::datatraversal::tests::TestLoopBody::operator()(const tarch::la::Vector<2,int>& i)
{
  tarch::multicore::Lock lock(_semaphore);
  _operatorCounter++;
}

void peano::datatraversal::tests::TestLoopBody::operator()(const tarch::la::Vector<3,int>& i)
{
  tarch::multicore::Lock lock(_semaphore);
  _operatorCounter++;
}

void peano::datatraversal::tests::TestLoopBody::resetGlobalCounter() {
  _constructorCounter = 0;
  _operatorCounter    = 0;
  _destructorCounter  = 0;
}

#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
