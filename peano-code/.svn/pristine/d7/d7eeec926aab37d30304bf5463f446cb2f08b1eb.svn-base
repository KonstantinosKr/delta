#include "peano/performanceanalysis/Analysis.h"
#include "peano/performanceanalysis/DefaultAnalyser.h"
#include "tarch/Assertions.h"




#ifdef PerformanceAnalysis


peano::performanceanalysis::Analysis::Analysis():
  _device(0) {
  _device = new DefaultAnalyser();
}

peano::performanceanalysis::Analysis::~Analysis() {
  deleteDevice();
}


peano::performanceanalysis::Analysis& peano::performanceanalysis::Analysis::getInstance() {
  static Analysis singleton;
  return singleton;
}


void peano::performanceanalysis::Analysis::deleteDevice() {
  if (_device!=0) {
    delete _device;
  }
}


void peano::performanceanalysis::Analysis::setDevice( Analyser* device ) {
  assertion( device!=0 );
  deleteDevice();
  _device = device;
}


void peano::performanceanalysis::Analysis::beginIteration() {
  assertion( _device!=0 );
  _device->beginIteration();
}


void peano::performanceanalysis::Analysis::endIteration(double numberOfInnerLeafCells, double numberOfOuterLeafCells, double numberOfInnerCells, double numberOfOuterCells, double numberOfLocalCells, double numberOfLocalVertices) {
  assertion( _device!=0 );
  _device->endIteration(numberOfInnerLeafCells,numberOfOuterLeafCells,numberOfInnerCells,numberOfOuterCells,numberOfLocalCells,numberOfLocalVertices);
}


void peano::performanceanalysis::Analysis::addWorker(
  int                                 workerRank,
  int                                 level
) {
  assertion( _device!=0 );
  _device->addWorker(workerRank,level);
}


void peano::performanceanalysis::Analysis::removeWorker(
  int                                 workerRank,
  int                                 level
) {
  assertion( _device!=0 );
  _device->removeWorker(workerRank,level);
}


void peano::performanceanalysis::Analysis::dataWasNotReceivedInBackground( int fromRank, int tag, int cardinality, int pageSize ) {
  assertion( _device!=0 );
  _device->dataWasNotReceivedInBackground(fromRank,tag,cardinality,pageSize);
}


void peano::performanceanalysis::Analysis::beginToReceiveDataFromWorker() {
  assertion( _device!=0 );
  _device->beginToReceiveDataFromWorker();
}


void peano::performanceanalysis::Analysis::endToReceiveDataFromWorker( int fromRank ) {
  assertion( _device!=0 );
  _device->endToReceiveDataFromWorker(fromRank);
}


void peano::performanceanalysis::Analysis::beginToReleaseSynchronousHeapData() {
  assertion( _device!=0 );
  _device->beginToReleaseSynchronousHeapData();
}


void peano::performanceanalysis::Analysis::endToReleaseSynchronousHeapData() {
  assertion( _device!=0 );
  _device->endToReleaseSynchronousHeapData();
}


void peano::performanceanalysis::Analysis::beginToPrepareAsynchronousHeapDataExchange() {
  assertion( _device!=0 );
  _device->beginToPrepareAsynchronousHeapDataExchange();
}


void peano::performanceanalysis::Analysis::endToPrepareAsynchronousHeapDataExchange() {
  assertion( _device!=0 );
  _device->endToPrepareAsynchronousHeapDataExchange();
}


void peano::performanceanalysis::Analysis::endReleaseOfJoinData() {
  assertion( _device!=0 );
  _device->endReleaseOfJoinData();
}


void peano::performanceanalysis::Analysis::endReleaseOfBoundaryData() {
  assertion( _device!=0 );
  _device->endReleaseOfBoundaryData();
}


void peano::performanceanalysis::Analysis::enterCentralElementOfEnclosingSpacetree() {
  assertion( _device!=0 );
  _device->enterCentralElementOfEnclosingSpacetree();
}


void peano::performanceanalysis::Analysis::leaveCentralElementOfEnclosingSpacetree() {
  assertion( _device!=0 );
  _device->leaveCentralElementOfEnclosingSpacetree();
}


void peano::performanceanalysis::Analysis::changeConcurrencyLevel(int actualChange, int maxPossibleChange) {
  assertion( _device!=0 );
  _device->changeConcurrencyLevel(actualChange,maxPossibleChange);
}


void peano::performanceanalysis::Analysis::fireAndForgetBackgroundTask(int taskCount) {
  assertion( _device!=0 );
  _device->fireAndForgetBackgroundTask(taskCount);
}

#endif
