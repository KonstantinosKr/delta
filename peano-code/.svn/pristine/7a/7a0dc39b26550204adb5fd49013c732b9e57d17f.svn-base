#include "peano/performanceanalysis/DefaultAnalyser.h"

#include "tarch/parallel/Node.h"
#include "tarch/logging/CommandLineLogger.h"

#include "tarch/multicore/Lock.h"

tarch::logging::Log  peano::performanceanalysis::DefaultAnalyser::_log( "peano::performanceanalysis::DefaultAnalyser" );

double peano::performanceanalysis::DefaultAnalyser::MinTimeInBetweenTwoConcurrencyLogs( 1.0 );
double peano::performanceanalysis::DefaultAnalyser::MaxTimeInBetweenTwoConcurrencyLogs( 4.0 );


peano::performanceanalysis::DefaultAnalyser::DefaultAnalyser():
  _totalWatch("peano::performanceanalysis::DefaultAnalyser", "-", false),
  _traversalWatch("peano::performanceanalysis::DefaultAnalyser", "-", false),
  _actualDomainTraversalWatch("peano::performanceanalysis::DefaultAnalyser", "-", false),
  _waitForWorkerDataWatch("peano::performanceanalysis::DefaultAnalyser", "-", false),
  _synchronousHeapWatch("peano::performanceanalysis::DefaultAnalyser", "-", false),
  _asynchronousHeapWatch("peano::performanceanalysis::DefaultAnalyser", "-", false),
  _concurrencyReportWatch("peano::performanceanalysis::DefaultAnalyser", "-", false),
  _currentConcurrencyLevel(0),
  _currentMaxPotentialConcurrencyLevel(0),
  _numberOfSpawnedBackgroundTask(0),
  _concurrentTimeSpent(0),
  _maxPotentialConcurrentTimeSpent(0),
  _maxConcurrencyLevel(0),
  _maxPotentialConcurrencyLevel(0),
  _lastConcurrencyDataUpdateTimeStamp(0) {
  if (!tarch::logging::CommandLineLogger::getInstance().getLogMachineName() && tarch::parallel::Node::getInstance().isGlobalMaster() ) {
    logWarning( "DefaultAnalyser()", "performance analysis might yield invalid results as logging of machine name is disabled. See command line logger" );
  }
  if (!tarch::logging::CommandLineLogger::getInstance().getLogTrace() && tarch::parallel::Node::getInstance().isGlobalMaster() ) {
    logWarning( "DefaultAnalyser()", "performance analysis might yield invalid results as logging of trace is disabled. See command line logger" );
  }
  if (!tarch::logging::CommandLineLogger::getInstance().getLogTimeStamp() && tarch::parallel::Node::getInstance().isGlobalMaster() ) {
    logWarning( "DefaultAnalyser()", "performance analysis might yield invalid results as logging of time stamps is disabled. See command line logger" );
  }
  if (tarch::logging::CommandLineLogger::getInstance().filterOut("info","peano::performanceanalysis::DefaultAnalyser") && tarch::parallel::Node::getInstance().isGlobalMaster() ) {
    logWarning( "DefaultAnalyser()", "performance analysis might yield invalid results as log filters for peano::performanceanalysis::DefaultAnalyser are installed" );
  }
}


peano::performanceanalysis::DefaultAnalyser::~DefaultAnalyser() {
}


void peano::performanceanalysis::DefaultAnalyser::beginIteration() {
  logInfo( "beginIteration()", "start traversal" );
  _traversalWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::endIteration(double numberOfInnerLeafCells, double numberOfOuterLeafCells, double numberOfInnerCells, double numberOfOuterCells, double numberOfLocalCells, double numberOfLocalVertices) {
  _traversalWatch.stopTimer();
  _totalWatch.stopTimer();

  logInfo( "endIteration()", "cells=(" << numberOfInnerLeafCells << "," << numberOfOuterLeafCells << "," << numberOfInnerCells << "," << numberOfOuterCells << "," << numberOfLocalCells << ")" );
  logInfo( "endIteration()", "local-vertices=" << numberOfLocalVertices );
  logInfo( "endIteration()", "t_total=(" << _totalWatch.getCalendarTime() << "," << _totalWatch.getCPUTime() << ")" );
  logInfo( "endIteration()", "t_traversal=(" << _traversalWatch.getCalendarTime() << "," << _traversalWatch.getCPUTime() << ")" );

  _totalWatch.startTimer();
  // is required by join time analysis
  _traversalWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::enterCentralElementOfEnclosingSpacetree() {
  logInfo( "enterCentralElementOfEnclosingSpacetree()", "enter central element" );
  _actualDomainTraversalWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::leaveCentralElementOfEnclosingSpacetree() {
  _actualDomainTraversalWatch.stopTimer();
  logInfo( "leaveCentralElementOfEnclosingSpacetree()", "t_central-tree-traversal=(" << _actualDomainTraversalWatch.getCalendarTime() << "," << _actualDomainTraversalWatch.getCPUTime() << ")" );
}


void peano::performanceanalysis::DefaultAnalyser::addWorker(
  int                                 workerRank,
  int                                 level
) {
  logInfo(
    "addWorker(int,Vector,Vector)",
    tarch::parallel::Node::getInstance().getRank()
    << "->"
    << tarch::parallel::Node::getInstance().getRank()
    << "+"
    << workerRank
    << " [level:"
    << level
    << "]"
  );
}


void peano::performanceanalysis::DefaultAnalyser::removeWorker(
  int                                 workerRank,
  int                                 level
) {
  logInfo(
    "removeWorker()",
    tarch::parallel::Node::getInstance().getRank()
    << "+"
    << workerRank
    << "->"
    << tarch::parallel::Node::getInstance().getRank()
    << " [level:"
    << level
    << "]"
  );
}


void peano::performanceanalysis::DefaultAnalyser::beginToReceiveDataFromWorker() {
  _waitForWorkerDataWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::endToReceiveDataFromWorker( int fromRank ) {
  _waitForWorkerDataWatch.stopTimer();
  const double elapsedTime = _waitForWorkerDataWatch.getCalendarTime();

  if (tarch::la::greater(elapsedTime,0.0)) {
    logInfo(
      "endToReceiveDataFromWorker()",
      "rank had to wait for worker " << fromRank <<
      " for " << elapsedTime <<
      "s"
    );
  }
  _waitForWorkerDataWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::dataWasNotReceivedInBackground( int fromRank, int tag, int cardinality, int pageSize ) {
  logInfo(
    "dataWasNotReceivedInBackground()",
    "rank had to wait for " << cardinality <<
    " record(s) from " << fromRank <<
    " on tag " << tag <<
    " with page size " << pageSize
  );
}


void peano::performanceanalysis::DefaultAnalyser::beginToReleaseSynchronousHeapData() {
  _synchronousHeapWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::endToReleaseSynchronousHeapData() {
  _synchronousHeapWatch.stopTimer();

  logInfo(
    "endToReleaseSynchronousHeapData()",
    "time=" <<
    _synchronousHeapWatch.getCalendarTime() <<
    ", cpu time=" <<
    _synchronousHeapWatch.getCPUTime()
  );
}


void peano::performanceanalysis::DefaultAnalyser::beginToPrepareAsynchronousHeapDataExchange() {
  _asynchronousHeapWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::endToPrepareAsynchronousHeapDataExchange() {
  _asynchronousHeapWatch.stopTimer();

  logInfo(
    "endToPrepareAsynchronousHeapDataExchange()",
    "time=" <<
    _asynchronousHeapWatch.getCalendarTime() <<
    ", cpu time=" <<
    _asynchronousHeapWatch.getCPUTime()
  );
}


void peano::performanceanalysis::DefaultAnalyser::endReleaseOfJoinData() {
  _traversalWatch.stopTimer();

  logInfo(
    "endReleaseOfJoinData()",
    "time=" <<
    _traversalWatch.getCalendarTime() <<
    ", cpu time=" <<
    _traversalWatch.getCPUTime()
  );

  _traversalWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::endReleaseOfBoundaryData() {
  _traversalWatch.stopTimer();

  logInfo(
    "endReleaseOfBoundaryData()",
    "time=" <<
    _traversalWatch.getCalendarTime() <<
    ", cpu time=" <<
    _traversalWatch.getCPUTime()
  );

  _traversalWatch.startTimer();
}


void peano::performanceanalysis::DefaultAnalyser::changeConcurrencyLevel(int actualChange, int maxPossibleChange) {
  tarch::multicore::Lock lock(_concurrencyReportSemaphore);

  assertion4(
    (actualChange>=0 && maxPossibleChange>0)
    ||
    (actualChange<=0 && maxPossibleChange<0),
    actualChange, maxPossibleChange, _currentConcurrencyLevel, _currentMaxPotentialConcurrencyLevel
  );

  _concurrencyReportWatch.stopTimer();

  const double currentTimeStamp       = _concurrencyReportWatch.getCalendarTime();
  const double deltaToLastDataUpdate  = currentTimeStamp - _lastConcurrencyDataUpdateTimeStamp;

  _lastConcurrencyDataUpdateTimeStamp = _concurrencyReportWatch.getCalendarTime();

  _concurrentTimeSpent             += _currentConcurrencyLevel             * deltaToLastDataUpdate;
  _maxPotentialConcurrentTimeSpent += _currentMaxPotentialConcurrencyLevel * deltaToLastDataUpdate;

  _currentConcurrencyLevel             += actualChange;
  _currentMaxPotentialConcurrencyLevel += maxPossibleChange;

  _maxConcurrencyLevel          = _maxConcurrencyLevel          > _currentConcurrencyLevel             ? _maxConcurrencyLevel : _currentConcurrencyLevel;
  _maxPotentialConcurrencyLevel = _maxPotentialConcurrencyLevel > _currentMaxPotentialConcurrencyLevel ? _maxPotentialConcurrencyLevel : _currentMaxPotentialConcurrencyLevel;

  assertion4( _currentConcurrencyLevel>=0,             actualChange, maxPossibleChange, _currentConcurrencyLevel, _currentMaxPotentialConcurrencyLevel);
  assertion4( _currentMaxPotentialConcurrencyLevel>=0, actualChange, maxPossibleChange, _currentConcurrencyLevel, _currentMaxPotentialConcurrencyLevel);

  if (
    (_concurrencyReportWatch.getCalendarTime() > MaxTimeInBetweenTwoConcurrencyLogs)
    ||
    (_currentMaxPotentialConcurrencyLevel==0 && _concurrencyReportWatch.getCalendarTime()> MinTimeInBetweenTwoConcurrencyLogs )
  ) {
    logInfo(
      "changeConcurrencyLevel(int,int)",
      "dt=" << _concurrencyReportWatch.getCalendarTime() <<
      ", cpu-time=" << _concurrencyReportWatch.getCPUTime() <<
      ", concurrent-time=" << _concurrentTimeSpent <<
      ", potential-concurrent-time=" << _maxPotentialConcurrentTimeSpent <<
      ", max-concurrency-level=" << _maxConcurrencyLevel <<
      ", max-potential-concurrency-level=" << _maxPotentialConcurrencyLevel <<
      ", background-tasks=" << _numberOfSpawnedBackgroundTask
    );

    _numberOfSpawnedBackgroundTask      = 0;

    _concurrentTimeSpent                = 0.0;
    _maxPotentialConcurrentTimeSpent    = 0.0;

    _maxConcurrencyLevel          = 0;
    _maxPotentialConcurrencyLevel = 0;

    _lastConcurrencyDataUpdateTimeStamp = 0.0;

    _concurrencyReportWatch.startTimer();
  }
}


void peano::performanceanalysis::DefaultAnalyser::fireAndForgetBackgroundTask(int taskCount) {
  tarch::multicore::Lock lock(_concurrencyReportSemaphore);

  _numberOfSpawnedBackgroundTask += taskCount;
}
