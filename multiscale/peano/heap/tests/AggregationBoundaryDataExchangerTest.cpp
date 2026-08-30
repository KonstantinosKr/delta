#include "peano/heap/tests/AggregationBoundaryDataExchangerTest.h"
#include "peano/heap/AggregationBoundaryDataExchanger.h"

#include "peano/heap/CompressedFloatingPointNumbers.h"
#include "peano/heap/SendReceiveTask.h"

#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/tests/TestCaseFactory.h"


registerTest(peano::heap::tests::AggregationBoundaryDataExchangerTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


tarch::logging::Log  peano::heap::tests::AggregationBoundaryDataExchangerTest::_log( "peano::heap::tests::AggregationBoundaryDataExchangerTest" );

 
peano::heap::tests::AggregationBoundaryDataExchangerTest::AggregationBoundaryDataExchangerTest():
  tarch::tests::TestCase( "peano::heap::tests::AggregationBoundaryDataExchangerTest" ) {
}


peano::heap::tests::AggregationBoundaryDataExchangerTest::~AggregationBoundaryDataExchangerTest() {
}


void peano::heap::tests::AggregationBoundaryDataExchangerTest::run() {
  testMethod( testHeaderComposeDecomposeOnCharHeap );
  testMethod( testHeaderComposeDecomposeOnIntHeap );
  testMethod( testHeaderComposeDecomposeOnDoubleHeap );
  testMethod( testWholeMessageDecompositionInCharHeap );
  testMethod( testWholeMessageDecompositionInDoubleHeap );
}


void peano::heap::tests::AggregationBoundaryDataExchangerTest::testHeaderComposeDecomposeOnCharHeap() {
  typedef peano::heap::AggregationBoundaryDataExchanger<char, SendReceiveTask<char>, std::vector<char> >  Exchanger;

  Exchanger exchanger;
  exchanger._numberOfSentMessages = 0;

  for (int i=0; i<exchanger.getNumberOfHeaderEntries(); i++) {
    exchanger._aggregatedSendData.push_back(0);
  }

  for (int i=0; i<100000; i++) {
    exchanger.setAggregatedMessageHeader();

    SendReceiveTask<char> receivedTask;
    receivedTask.getMetaInformation().setLength( exchanger.getNumberOfHeaderEntries() );
    receivedTask.wrapData( exchanger._aggregatedSendData.data() );

    int sentNumberOfMessgaes = exchanger.getNumberOfMessages( receivedTask );

    validateEqualsWithParams6(
      sentNumberOfMessgaes,i,
	  exchanger._numberOfSentMessages,
	  (int)(std::numeric_limits<char>::max()),
	  (int)(exchanger._aggregatedSendData[0]),
	  (int)(exchanger._aggregatedSendData[1]),
	  (int)(exchanger._aggregatedSendData[2]),
	  (int)(exchanger._aggregatedSendData[3])
	);

    receivedTask.freeMemory();

    exchanger._numberOfSentMessages++;
  }
}



void peano::heap::tests::AggregationBoundaryDataExchangerTest::testHeaderComposeDecomposeOnIntHeap() {
/*
  typedef peano::heap::AggregationBoundaryDataExchanger<int, SendReceiveTask<int>, std::vector<int> >  Exchanger;

  Exchanger exchanger;
  exchanger._numberOfSentMessages = 0;

  for (int i=0; i<exchanger.getNumberOfHeaderEntries(); i++) {
    exchanger._aggregatedSendData.push_back(0);
  }

  for (int i=0; i<100000; i++) {
    exchanger.setAggregatedMessageHeader();

    SendReceiveTask<int> receivedTask;
    receivedTask.getMetaInformation().setLength( exchanger.getNumberOfHeaderEntries() );
    receivedTask.wrapData( exchanger._aggregatedSendData.data() );

    int sentNumberOfMessgaes = exchanger.getNumberOfMessages( receivedTask );

    validateEqualsWithParams3(
      sentNumberOfMessgaes,i,
	  exchanger._numberOfSentMessages,
	  (int)(std::numeric_limits<int>::max()),
	  (int)(exchanger._aggregatedSendData[0])
	);

    receivedTask.freeMemory();

    exchanger._numberOfSentMessages++;
  }
*/
}


void peano::heap::tests::AggregationBoundaryDataExchangerTest::testHeaderComposeDecomposeOnDoubleHeap() {
  typedef peano::heap::AggregationBoundaryDataExchanger<double, SendReceiveTask<double>, std::vector<double> >  Exchanger;

  Exchanger exchanger;
  exchanger._numberOfSentMessages = 0;

  for (int i=0; i<exchanger.getNumberOfHeaderEntries(); i++) {
    exchanger._aggregatedSendData.push_back(0);
  }

  for (int i=0; i<100000; i++) {
    exchanger.setAggregatedMessageHeader();
    SendReceiveTask<double> receivedTask;
    receivedTask.getMetaInformation().setLength( exchanger.getNumberOfHeaderEntries() );
    receivedTask.wrapData( exchanger._aggregatedSendData.data() );

    int sentNumberOfMessgaes = exchanger.getNumberOfMessages( receivedTask );

    validateEqualsWithParams3(
      sentNumberOfMessgaes,i,
	  exchanger._numberOfSentMessages,
	  std::numeric_limits<double>::max(),
	  exchanger._aggregatedSendData[0]
	);

    receivedTask.freeMemory();

    exchanger._numberOfSentMessages++;
  }
}


void peano::heap::tests::AggregationBoundaryDataExchangerTest::testWholeMessageDecompositionInCharHeap() {
  typedef peano::heap::AggregationBoundaryDataExchanger<char, SendReceiveTask<char>, std::vector<char> >  Exchanger;

  Exchanger exchanger;
  exchanger._currentReceiveBuffer = 0;

  const int NumberOfMessages = 6;

  std::vector<char> data;
  data.push_back( NumberOfMessages );
  data.push_back(0);
  data.push_back(0);
  data.push_back(0);
  for (int i=0; i<NumberOfMessages; i++ ) {
    data.push_back(i);
    for (int j=0; j<i; j++) {
      data.push_back(-j);
    }
  }

  std::ostringstream msg;
  msg << "[";
  for (auto p: data) {
	msg << " " << (int)p;
  }
  msg << " ]";
  logInfo( "testWholeMessageDecompositionInCharHeap()", msg.str() );

  SendReceiveTask<char>  receivedTask;
  receivedTask.getMetaInformation().setLength( data.size() );
  receivedTask.wrapData( data.data() );

  exchanger._receiveTasks[1].push_back( receivedTask );
  exchanger.postprocessStartToSendData();

  validateEquals( exchanger._receiveTasks[1].size(), NumberOfMessages );
  int currentMessageLenght = 0;
  for (auto p: exchanger._receiveTasks[1]) {
    validateEqualsWithParams1( p.getMetaInformation().getLength(), currentMessageLenght, p.getMetaInformation().toString() );
    for (int j=0; j<currentMessageLenght; j++) {
      validateNumericalEqualsWithParams3( p.data()[j], -j, p.getMetaInformation().toString(), currentMessageLenght, j );
    }
    currentMessageLenght++;
  }

  exchanger._receiveTasks[1].clear();
}


void peano::heap::tests::AggregationBoundaryDataExchangerTest::testWholeMessageDecompositionInDoubleHeap() {
  typedef peano::heap::AggregationBoundaryDataExchanger<double, SendReceiveTask<double>, std::vector<double> >  Exchanger;

  Exchanger exchanger;
  exchanger._currentReceiveBuffer = 0;

  const int NumberOfMessages = 6;

  std::vector<double> data;
  data.push_back( NumberOfMessages );
  for (int i=0; i<NumberOfMessages; i++ ) {
    data.push_back(i);
    for (int j=0; j<i; j++) {
      data.push_back(-j);
    }
  }

  std::ostringstream msg;
  msg << "[";
  for (auto p: data) {
	msg << " " << p;
  }
  msg << " ]";
  logInfo( "testWholeMessageDecompositionInDoubleHeap()", msg.str() );

  SendReceiveTask<double>  receivedTask;
  receivedTask.getMetaInformation().setLength( data.size() );
  receivedTask.wrapData( data.data() );

  exchanger._receiveTasks[1].push_back( receivedTask );
  exchanger.postprocessStartToSendData();

  validateEquals( exchanger._receiveTasks[1].size(), NumberOfMessages );
  int currentMessageLenght = 0;
  for (auto p: exchanger._receiveTasks[1]) {
    validateEqualsWithParams1( p.getMetaInformation().getLength(), currentMessageLenght, p.getMetaInformation().toString() );
    for (int j=0; j<currentMessageLenght; j++) {
      validateNumericalEqualsWithParams3( p.data()[j], -j, p.getMetaInformation().toString(), currentMessageLenght, j );
    }
    currentMessageLenght++;
  }

  exchanger._receiveTasks[1].clear();
}


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
