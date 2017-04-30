#include "sharedmemoryoracles/OracleForOnePhaseWithShrinkingGrainSize.h"
#include "peano/utils/Globals.h"
#include "tarch/Assertions.h"
#include "tarch/multicore/Core.h"
#include "tarch/multicore/Lock.h"


#include <cstdlib>
#include <limits>
#include <fstream>
#include <stdexcept>


tarch::logging::Log  sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::_log( "sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize" );

const double   sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::_InitialRelativeAccuracy(1e-2);
const double   sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::_MaxAccuracy( 1.0 );
const double   sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::_WideningFactor( 0.9 );


bool                                 sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::_hasLearnedSinceLastQuery( false );
tarch::multicore::BooleanSemaphore   sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::_semaphore;


sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::OracleForOnePhaseWithShrinkingGrainSize(
  bool learn,
  bool restart,
  SelectNextStudiedMeasureTraceStrategy selectNextStudiedMeasureTraceStrategy
):
  _activeMethodTrace(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling),
  _learn(learn && tarch::multicore::Core::getInstance().getNumberOfThreads()>1),
  _restart(restart),
  _selectNextStudiedMeasureTraceStrategy(selectNextStudiedMeasureTraceStrategy),
  _measurements() {
  for (int i=0; i<static_cast<int>(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling); i++) {
    peano::datatraversal::autotuning::MethodTrace askingMethod = peano::datatraversal::autotuning::toMethodTrace(i);
    _measurements.insert( std::pair<peano::datatraversal::autotuning::MethodTrace,MethodTraceData >(askingMethod,MethodTraceData()) );
    _measurements[askingMethod].push_back( DatabaseEntry(2) );
    assertion( _measurements.count(askingMethod)==1 );
    assertion2( _measurements[askingMethod].back().getCurrentGrainSize()>0, _measurements[askingMethod].back().toString(), toString(_activeMethodTrace) );
    logDebug(
      "getDatabaseEntry(int)",
      "inserted trivial entry for " + peano::datatraversal::autotuning::toString(askingMethod)
      << ": " << _measurements[askingMethod].rbegin()->toString()
    );
  }
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::operator<(const DatabaseEntry& cmp) const {
  return ( _biggestProblemSize<cmp._biggestProblemSize );
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::hasDatabaseEntry(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod) const {
  return _measurements.count(askingMethod)>0 && _measurements.at(askingMethod).rbegin()->getBiggestProblemSize()>=problemSize;
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::createDatabaseEntries(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod) {
  //tarch::multicore::Lock lock(_semaphore);
  assertion(_measurements[askingMethod].rbegin()->getBiggestProblemSize()>0);

  while (_measurements[askingMethod].rbegin()->getBiggestProblemSize()<problemSize*2) {
    _measurements[askingMethod].push_back( DatabaseEntry(
      *_measurements[askingMethod].rbegin(),
      _measurements[askingMethod].rbegin()->getBiggestProblemSize()*2
    ));
    assertion2( _measurements[askingMethod].back().getCurrentGrainSize()>0, _measurements[askingMethod].back().toString(), toString(_activeMethodTrace) );
    logDebug(
      "getDatabaseEntry(int)",
      "inserted new entry for " + peano::datatraversal::autotuning::toString(askingMethod)
      << ": " << _measurements[askingMethod].rbegin()->toString()
    );
  }

  assertion4( hasDatabaseEntry(problemSize, askingMethod), problemSize, toString(askingMethod), _measurements.at(askingMethod).rbegin()->toString(), _measurements.at(askingMethod).rbegin()->getBiggestProblemSize() );
}


sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry&  sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::getDatabaseEntry(
  int problemSize,
  peano::datatraversal::autotuning::MethodTrace askingMethod
) {
  MethodTraceData::iterator entry = _measurements.at(askingMethod).begin();
  while (
    entry->getBiggestProblemSize()<problemSize
    and
    (entry+1)!=_measurements.at(askingMethod).end()
  ) {
    entry++;
    assertion(entry!=_measurements[askingMethod].end());
    assertion(entry!=_measurements.at(askingMethod).end());
  }

  DatabaseEntry& result = *entry;
  assertion5(
    result.getCurrentGrainSize()>0,
    result.toString(), _measurements[askingMethod].back().toString(), toString(_activeMethodTrace), _measurements[askingMethod].size(),
    problemSize);
  return result;
}


sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry  sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::getDatabaseEntry(
  int problemSize,
  peano::datatraversal::autotuning::MethodTrace askingMethod
) const {
  MethodTraceData::const_iterator entry = _measurements.at(askingMethod).begin();
  while (
    entry->getBiggestProblemSize()<problemSize
    and
    (entry+1)!=_measurements.at(askingMethod).end()
  ) {
    entry++;
    assertion(entry!=_measurements.at(askingMethod).end());
  }

  DatabaseEntry result = *entry;
  assertion5(
    result.getCurrentGrainSize()>0,
    result.toString(), _measurements.at(askingMethod).back().toString(), toString(_activeMethodTrace), _measurements.at(askingMethod).size(),
    problemSize);
  return result;
}


peano::datatraversal::autotuning::GrainSize  sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::parallelise(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod) {
  // tarch::multicore::Lock lock(_semaphore);
  if ( problemSize==1 ) {
    return peano::datatraversal::autotuning::GrainSize(
      0,
      false,
      problemSize,
      askingMethod, this
    );
  }
  else if (
    _activeMethodTrace==askingMethod
  ) {
    // @todo Das eigentliche Oracle
    assertion( askingMethod != peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling );
    assertion(askingMethod!=peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling);
    assertion( _measurements.count(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling)==0 );

    tarch::multicore::Lock lock(_semaphore);
    createDatabaseEntries(problemSize, askingMethod);
    auto databaseEntry = getDatabaseEntry(problemSize,askingMethod);
    lock.free();

    assertion( _measurements.count(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling)==0 );
    assertion( _measurements.count(askingMethod)>0 );

    const bool trackTime = databaseEntry.isSearching();

    logDebug(
      "parallelise()",
      "will track time for " << toString(askingMethod) << "=" << trackTime <<
      " with active trace " << toString(_activeMethodTrace) <<
      " for maximum problem size " << problemSize
    );

    assertion2( databaseEntry.getCurrentGrainSize()>0, databaseEntry.toString(), toString(_activeMethodTrace) );
    const int chosenParallelGrainSize     = databaseEntry.isStudyingScalingSetup() ? databaseEntry.getCurrentGrainSize() : 0;


    if (trackTime) {
      assertion( hasDatabaseEntry(problemSize, askingMethod) );

      const int EveryXthIsSerialMeasurement = databaseEntry.getBiggestProblemSize() / databaseEntry.getCurrentGrainSize();
      const bool passedGrainSize            = rand()%EveryXthIsSerialMeasurement==0 ? 0 : chosenParallelGrainSize;
      return peano::datatraversal::autotuning::GrainSize(
        passedGrainSize,
        trackTime,
        problemSize,
        askingMethod, this
      );
    }
    else {
      return peano::datatraversal::autotuning::GrainSize(
        chosenParallelGrainSize,
        trackTime,
        problemSize,
        askingMethod, this
      );
    }
  }
  else {
    // ohne den hammer an Segfault
    const auto databaseEntry = getDatabaseEntry(problemSize,askingMethod);

    assertion( _measurements.count(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling)==0 );
    assertion( _measurements.count(askingMethod)>0 );

    const int chosenParallelGrainSize     = databaseEntry.isStudyingScalingSetup() ? databaseEntry.getCurrentGrainSize() : 0;

    return peano::datatraversal::autotuning::GrainSize(
      chosenParallelGrainSize,
      false,
      problemSize,
      askingMethod, this
    );
  }
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::widenAccuracyOfCurrentlyStudiedMethodTraceAndRandomlyRestart() {
  bool widenedEntry = false;
  for (auto& p: _measurements[_activeMethodTrace]) {
    widenedEntry |= p.widenAccuracy();
  }
  if (!widenedEntry && _restart) {
    for (auto& p: _measurements[_activeMethodTrace]) {
      if (rand()%100==0) {
        p.restart();
      }
    }
  }
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::isStableDatabaseEntrySet( const peano::datatraversal::autotuning::MethodTrace&  askingMethod) const {
  if ( _measurements.count(askingMethod)==0 ) {
    return true;
  }
  else {
    bool result             = true;

    for (auto p: _measurements.at(askingMethod)) {
      result             &= p.isStable();
    }

    return result;
  }
}



void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::changeMeasuredMethodTrace() {
  assertion( _measurements.count(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling)==0 );
  assertion( !_measurements.empty() );

  // We just make it big enough to run through every variant if we used a deterministic scheme.
  int remainingTriesToFindSearchingTrace = (int)(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling);

  switch (_selectNextStudiedMeasureTraceStrategy) {
    case SelectNextStudiedMeasureTraceStrategy::Randomised:
      _activeMethodTrace             = peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling;

      while ( _measurements.count(_activeMethodTrace)==0 ) {
//        xxx Kann das ne Endlosschleife geben?
        _activeMethodTrace = peano::datatraversal::autotuning::toMethodTrace( rand() % (int)(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling) );
        if (
          remainingTriesToFindSearchingTrace>0
          &&
          _measurements.count(_activeMethodTrace)==1
          &&
          isStableDatabaseEntrySet(_activeMethodTrace)
        ) {
          remainingTriesToFindSearchingTrace--;
          logDebug( "changeMeasuredMethodTrace()", "skip " << toString(_activeMethodTrace) );
          _activeMethodTrace = peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling;
        }
      }
      break;

    case SelectNextStudiedMeasureTraceStrategy::Cyclic:
      do {
        _activeMethodTrace = peano::datatraversal::autotuning::toMethodTrace( static_cast<int>(_activeMethodTrace) + 1 );
        if (_activeMethodTrace>=peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling) {
          _activeMethodTrace = peano::datatraversal::autotuning::toMethodTrace( 0 );
        }
        remainingTriesToFindSearchingTrace--;
      }
      while (
        _measurements.count(_activeMethodTrace)==0
        ||
        ( remainingTriesToFindSearchingTrace>0 && isStableDatabaseEntrySet(_activeMethodTrace))
      );
      break;

    case SelectNextStudiedMeasureTraceStrategy::RandomisedWithHigherPrioritiesForSmallProblemSizes:
      {
        const peano::datatraversal::autotuning::MethodTrace oldActiveMethodTrace = _activeMethodTrace;

        _activeMethodTrace                               = peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling;
        static int maxProblemSizeOfLastMethodTraceChoice = 1;

        // Second statements avoid locking in one trace which is stupid if exactly this
        // trace is not called anymore as the grid has changed
        while ( _measurements.count(_activeMethodTrace)==0 || oldActiveMethodTrace==_activeMethodTrace ) {
          _activeMethodTrace = peano::datatraversal::autotuning::toMethodTrace( rand() % (int)(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling) );
          if (
            remainingTriesToFindSearchingTrace>0
            &&
            _measurements.count(_activeMethodTrace)==1
            &&
            (
              isStableDatabaseEntrySet(_activeMethodTrace)
              ||
              _measurements[_activeMethodTrace].back().getBiggestProblemSize () > maxProblemSizeOfLastMethodTraceChoice
            )
          ) {
            remainingTriesToFindSearchingTrace--;
            logDebug( "changeMeasuredMethodTrace()", "skip " << toString(_activeMethodTrace) << " with internal max size=" << maxProblemSizeOfLastMethodTraceChoice);
            _activeMethodTrace = peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling;
          }
        }

        if ( _measurements.count(_activeMethodTrace)==1 ) {
          maxProblemSizeOfLastMethodTraceChoice = (maxProblemSizeOfLastMethodTraceChoice + _measurements[_activeMethodTrace].back().getBiggestProblemSize()) / 2;
        }
      }
      break;
  }

  assertion(_measurements.count(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling)==0);
  assertion(_activeMethodTrace!=peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling);

  logInfo(
    "changeMeasuredMethodTrace()", "next active method trace " << toString(_activeMethodTrace) << " after " <<
    ((int)(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling)-remainingTriesToFindSearchingTrace) <<
    " search iterations to identify next analysed method trace. Is already known to oracle=" << ( _measurements.count(_activeMethodTrace)>0 )
  );
}



bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::isAccurate() const {
  return _measurementsAreAccurate;
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::isSearching() const {
  return _searchDelta>0;
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::isScaling() const {
  return (_currentGrainSize<=_biggestProblemSize/2 && _searchDelta==0)
      || (
        _searchDelta>0 &&
        _numberOfParallelMeasurements > 0 &&
        _numberOfSerialMeasurements > 0 &&
        (_accumulatedParallelMeasurement/_numberOfParallelMeasurements) < (_accumulatedParallelMeasurement/_numberOfParallelMeasurements)
      );
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::isStudyingScalingSetup() const {
  return _currentGrainSize<=_biggestProblemSize/2;
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::isStable() const {
  return _searchDelta==0 || ( _numberOfParallelMeasurements==0 && _numberOfSerialMeasurements==0 );
}


int sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::getCurrentGrainSize() const {
  return _currentGrainSize;
}


int  sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::getBiggestProblemSize() const {
  return _biggestProblemSize;
}



void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::learn() {
  assertion( _searchDelta>0 );

  const double newSpeedup =
    (_accumulatedSerialMeasurement / _numberOfSerialMeasurements ) /
    (_accumulatedParallelMeasurement / _numberOfParallelMeasurements );

  if ( newSpeedup > _previousSpeedup ) {
    logInfo( "learn()", "found better scaling parameter choice/serial runtime for " << toString() );

    while ( _currentGrainSize - _searchDelta <= 0 && _searchDelta>0 ) {
      if (_currentGrainSize>tarch::multicore::Core::getInstance().getNumberOfThreads()*2) {
        _searchDelta--;
      }
      else {
        _searchDelta /= 2;
      }
    }

    if (_searchDelta>0) {
      _currentGrainSize               -= _searchDelta;
      _accumulatedParallelMeasurement  = 0.0;
      _numberOfParallelMeasurements    = 0.0;
      _accuracy                       /= 2.0;
      _previousSpeedup                 = newSpeedup;
      _measurementsAreAccurate         = false;
    }
  }
  else {
    logInfo( "learn()", "parameter choice for " << toString() << " does not scale. New speedup is " << newSpeedup << " while old/previous speedup had been " << _previousSpeedup );

    _currentGrainSize               += _searchDelta;
    _accumulatedParallelMeasurement  = 0.0;
    _numberOfParallelMeasurements    = 0.0;
    _accuracy                       /= 2.0;
    _measurementsAreAccurate         = false;
  }

  if (_currentGrainSize>_biggestProblemSize/2) {
    _searchDelta      = 0;
    _currentGrainSize = _biggestProblemSize;
    logInfo( "learn()", "stop search for " << toString() << " as it searches in non-scaling regime");
  }
  else {
    logInfo( "learn()", "continue with " << toString() );
  }

  assertion1( _currentGrainSize>0, toString() );
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::parallelSectionHasTerminated(int problemSize, int grainSize, peano::datatraversal::autotuning::MethodTrace askingMethod, double costPerProblemElement) {
  assertion( askingMethod!=peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling );

  assertion( hasDatabaseEntry(problemSize, askingMethod) );

  tarch::multicore::Lock lock(_semaphore);
  auto& databaseEntry = getDatabaseEntry(problemSize,askingMethod);
  if (!databaseEntry.isAccuracyInitialised() ) {
    const double computeTime   = costPerProblemElement * static_cast<double>(problemSize);
    databaseEntry.initAccuracy(computeTime);
    logDebug(
      "parallelSectionHasTerminated(...)",
      "fix accuracy for " << toString(askingMethod) << " to " << databaseEntry.toString()
    );
  }

  databaseEntry.setValue( grainSize, costPerProblemElement );
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::setValue(int grainSize, double value) {
  double delta = 0.0;

  if (grainSize==0) {
    const double oldSerialTime   = _numberOfSerialMeasurements   > 0.0 ? _accumulatedSerialMeasurement   / _numberOfSerialMeasurements   : 0.0;

    _accumulatedSerialMeasurement += value;
    _numberOfSerialMeasurements   += 1.0;

    delta = _accumulatedSerialMeasurement   / _numberOfSerialMeasurements - oldSerialTime;
  }
  else {
    const double oldParallelTime = _numberOfParallelMeasurements > 0.0 ? _accumulatedParallelMeasurement / _numberOfParallelMeasurements : 0.0;

    _accumulatedParallelMeasurement += value;
    _numberOfParallelMeasurements   += 1.0;

    delta = _accumulatedParallelMeasurement   / _numberOfParallelMeasurements - oldParallelTime;
  }

  const double averageTime = 0.5 * (_accumulatedSerialMeasurement   / _numberOfSerialMeasurements + _accumulatedParallelMeasurement / _numberOfParallelMeasurements);
  _measurementsAreAccurate  = delta / (averageTime) < _accuracy
                           && _numberOfSerialMeasurements   > 10
                           && _numberOfParallelMeasurements > 10;
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::initAccuracy(double serialComputeTime) {
  _accuracy =  serialComputeTime * _InitialRelativeAccuracy;
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::widenAccuracy() {
  _accuracy /= _WideningFactor;
  return _accuracy < 1.0;
}


std::string sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::toString() const {
  std::ostringstream msg;
  msg <<        _biggestProblemSize
      << "," << _currentGrainSize
      << "," << _searchDelta
      << "," << (_measurementsAreAccurate ? "is-accurate" : "not-accurate" )
      << "," << _accuracy
      << "," << _accumulatedSerialMeasurement
      << "," << _accumulatedParallelMeasurement
      << "," << _numberOfSerialMeasurements
      << "," << _numberOfParallelMeasurements
      << "," << _previousSpeedup;
  return msg.str();
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::plotStatistics(std::ostream& out, int oracleNumber) const {
  out << "# " << std::endl;
  out << "# trace=biggest problem size, current grain size, search delta, is accurate, accuracy, accum serial time, acuum parallel time, no of serial samples, no of parallel samples, previous speedup" << std::endl;
  out << "# -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  out << "# " << std::endl;
  out << "begin OracleForOnePhaseWithShrinkingGrainSize" << std::endl;
  out << "initial-relative-accuracy=" << _InitialRelativeAccuracy << std::endl;
  out << "adapter-number=" << oracleNumber << std::endl;

  for (auto measurement: _measurements)
  for (auto p: measurement.second) {
    out << peano::datatraversal::autotuning::toString(measurement.first)
        << "=" << p.toString() << std::endl;
  }

  out << "end OracleForOnePhaseWithShrinkingGrainSize" << std::endl;

  logDebug( "plotStatistics(std::ostream,int)", "piped statistics for oracle no " << oracleNumber );
}


sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::DatabaseEntry(int problemSize) {
  assertion(problemSize>0);

  if (
    problemSize < tarch::multicore::Core::getInstance().getNumberOfThreads()*2
    ||
    tarch::multicore::Core::getInstance().getNumberOfThreads() <= 2
  ) {
    _searchDelta       = problemSize/2;
    _currentGrainSize  = problemSize/2;
  }
  else {
    _searchDelta      = problemSize / tarch::multicore::Core::getInstance().getNumberOfThreads();
    _currentGrainSize = problemSize / tarch::multicore::Core::getInstance().getNumberOfThreads();
  }

  _biggestProblemSize             = problemSize;
  _measurementsAreAccurate        = false;
  _accuracy                       = 0.0;
  _accumulatedSerialMeasurement   = 0.0;
  _accumulatedParallelMeasurement = 0.0;
  _numberOfSerialMeasurements     = 0.0;
  _numberOfParallelMeasurements   = 0.0;
  _previousSpeedup                = 1.0;

  assertion1( _currentGrainSize>0 || problemSize==1, toString() );
}


sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::DatabaseEntry( const DatabaseEntry& prototype, int newProblemSize ):
  DatabaseEntry(newProblemSize) {
  assertion(prototype._biggestProblemSize<newProblemSize);
  if (prototype._currentGrainSize < prototype._biggestProblemSize && prototype.isAccuracyInitialised() ) {
    // These are all integers and it should be possible to  determine all
    // settings through integer arithmetics if we do not determine a rescaling
    // factor (integer) once and then add it but first multiply and then
    // divide. However, if we do so, we quickly run into integer overflows:
    //     _currentGrainSize = prototype._currentGrainSize * newProblemSize / prototype._biggestProblemSize;
    //     _searchDelta      = prototype._searchDelta      * newProblemSize / prototype._biggestProblemSize;
    //
    // So we have to work with first computing the ratio though it might introduce truncation errors:
    int scaleUp = newProblemSize / prototype._biggestProblemSize;
    if (scaleUp==0) {
      scaleUp = 1;
    }

    _currentGrainSize = prototype._currentGrainSize * scaleUp;
    _searchDelta      = prototype._searchDelta      * scaleUp;

    assertion1( _currentGrainSize>0, toString() );
  }

  if (_searchDelta==0) {
    restart();
  }

  assertion1(_currentGrainSize<=_biggestProblemSize, toString() );
  assertion1(_currentGrainSize>0,                    toString() );
  assertion1(_biggestProblemSize>0,                  toString() );
  assertion1(_searchDelta>=0,                        toString() );
}


bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::isAccuracyInitialised() const {
  return _accuracy>0.0;
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::restart() {
  assertion1( _currentGrainSize>0, toString() );

  if (
    _biggestProblemSize>tarch::multicore::Core::getInstance().getNumberOfThreads()
    &&
    _currentGrainSize<_biggestProblemSize/4
  ) {
    _searchDelta        = _currentGrainSize;
    _currentGrainSize   = _currentGrainSize * 2;
  }
  else {
    _searchDelta        = _biggestProblemSize/2;
    _currentGrainSize   = _biggestProblemSize/2;
  }

  _measurementsAreAccurate        = false;
  _accuracy                       = 0.0;
  _accumulatedSerialMeasurement   = 0.0;
  _accumulatedParallelMeasurement = 0.0;
  _numberOfSerialMeasurements     = 0.0;
  _numberOfParallelMeasurements   = 0.0;
  _previousSpeedup                = 1.0;

  assertion1( _currentGrainSize>0, toString() );
  logInfo( "restart(...)", "restarted " << toString() );
}


sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::DatabaseEntry::DatabaseEntry(std::string  inputLine) {

  std::string leftToken    = "";
  std::string rightString  = inputLine;
  std::string methodTrace;

  logDebug( "DatabaseEntry(std::string)", "interpret line " << inputLine );

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _biggestProblemSize = std::stoi(leftToken);
  logDebug( "DatabaseEntry(std::string)", "got biggest problem size " << _biggestProblemSize );

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _currentGrainSize = std::stoi(leftToken);
  logDebug( "DatabaseEntry(std::string)", "got current grain size " <<  _currentGrainSize );

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _searchDelta = std::stoi(leftToken);
  logDebug( "DatabaseEntry(std::string)", "search delta is " <<  _searchDelta );

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _measurementsAreAccurate = leftToken.compare("is-accurate")==0;
  logDebug( "DatabaseEntry(std::string)", "_measurementsAreAccurate is " <<  _measurementsAreAccurate );

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _accuracy = std::stof(leftToken);
  logDebug( "DatabaseEntry(std::string)", "accuracy is " <<  _accuracy);

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _accumulatedSerialMeasurement = std::stof(leftToken);
  logDebug( "DatabaseEntry(std::string)", "_accumulatedSerialMeasurement is " <<  _accumulatedSerialMeasurement);

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _accumulatedParallelMeasurement = std::stof(leftToken);
  logDebug( "DatabaseEntry(std::string)", "_accumulatedParallelMeasurement is " <<  _accumulatedParallelMeasurement);

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _numberOfSerialMeasurements = std::stof(leftToken);
  logDebug( "DatabaseEntry(std::string)", "_numberOfSerialMeasurements is " <<  _numberOfSerialMeasurements);

  leftToken   = rightString.substr( 0, rightString.find(",") );
  rightString = rightString.substr( leftToken.size()+1 );
  _numberOfParallelMeasurements = std::stof(leftToken);
  logDebug( "DatabaseEntry(std::string)", "_numberOfParallelMeasurements is " <<  _numberOfParallelMeasurements);

  leftToken     = rightString;
  //leftToken   = rightString.substr( 0, rightString.find(",") );
  //rightString = rightString.substr( leftToken.size()+1 );
  _previousSpeedup = std::stof(leftToken);
  logDebug( "DatabaseEntry(std::string)", "_previousSpeedup is " <<  _previousSpeedup);

  logDebug( "DatabaseEntry(std::string)", "built up entry is " <<  toString() );

  bool isValid = _biggestProblemSize>0
              && _currentGrainSize>0
              && _currentGrainSize<=_biggestProblemSize;

  if (!isValid) {
    if (_biggestProblemSize<=0) {
      _biggestProblemSize = 65536;
      logError( "DatabaseEntry(std::string)", "unable to parse file entries w.r.t. biggest problem size" );
    }
    logError( "DatabaseEntry(std::string)", "input file seems to have been corrupted. Restart entry " << toString() << ". Corrupted line: " << inputLine );

    *this = DatabaseEntry(_biggestProblemSize);
    restart();
  }
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::loadStatistics(const std::string& filename, int oracleNumber) {
  std::ifstream file(filename);

  if (!file.is_open() and _learn) {
    logInfo(
      "loadStatistics(...)",
      "was not able to load input file " << filename << ". Oracle will start to learn from scratch"
    );
  }
  else if (!file.is_open()) {
    logError(
      "loadStatistics(...)",
      "was not able to load input file " << filename << ". Code might fall back to single-core version"
    );
  }

  _measurements.clear();

  std::string str = "";

  bool        tagOpen = false;
  while (std::getline(file, str)) {
    tagOpen &= str.compare("end OracleForOnePhaseWithShrinkingGrainSize")!=0;

    if (tagOpen) {
      #ifdef __EXCEPTIONS
      try {
      #endif

      std::string leftToken = "";
      std::string rightString = str;

      leftToken   = rightString.substr( 0, rightString.find("=") );
      rightString = rightString.substr( leftToken.size()+1 );
      peano::datatraversal::autotuning::MethodTrace  methodTrace = peano::datatraversal::autotuning::toMethodTrace(leftToken);
      logDebug( "loadStatistics(...)", "parse properties for " << toString(methodTrace) );

      DatabaseEntry newEntry(rightString);

      _measurements[methodTrace].push_back( newEntry );

      logDebug( "loadStatistics(...)", "added " << newEntry.toString() << " for " << toString(methodTrace) );
      #ifdef __EXCEPTIONS
      }
      catch (std::out_of_range& exception) {
        logError(
          "loadStatistics(...)",
          "failed to parse shared memory configuration file " << filename << " with error in " << exception.what() << " in adapter " << oracleNumber
        );
        logError(
          "loadStatistics(...)",
          "flawed string: " << str
        );
      }
      #endif
    }

    // Older GCC versions require an explicit cast here
    tagOpen |= str.compare( "adapter-number=" + std::to_string( (long long)oracleNumber) )==0;
  }


  for (int i=0; i<static_cast<int>(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling); i++) {
    peano::datatraversal::autotuning::MethodTrace askingMethod = peano::datatraversal::autotuning::toMethodTrace(i);
    if (_measurements.count(askingMethod)==0) {
      _measurements.insert( std::pair<peano::datatraversal::autotuning::MethodTrace,MethodTraceData >(askingMethod,MethodTraceData()) );
      _measurements[askingMethod].push_back( DatabaseEntry(2) );
      assertion( _measurements.count(askingMethod)==1 );
      assertion2( _measurements[askingMethod].back().getCurrentGrainSize()>0, _measurements[askingMethod].back().toString(), toString(_activeMethodTrace) );
      logDebug(
        "getDatabaseEntry(int)",
        "inserted trivial entry for " + peano::datatraversal::autotuning::toString(askingMethod)
        << ": " << _measurements[askingMethod].rbegin()->toString()
      );
    }
  }

  assertion( _measurements.count(peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling)==0 );
}


sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::~OracleForOnePhaseWithShrinkingGrainSize() {
}


peano::datatraversal::autotuning::OracleForOnePhase* sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::createNewOracle() const {
  return new OracleForOnePhaseWithShrinkingGrainSize(_learn,_restart,_selectNextStudiedMeasureTraceStrategy);
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::deactivateOracle() {
}


void sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::activateOracle() {
  if (_learn) {
    //
    // First check is very important. If we skip it, the second statement would
    // insert elements into map
    //
    if (
      _activeMethodTrace!=peano::datatraversal::autotuning::MethodTrace::NumberOfDifferentMethodsCalling
    ) {
      bool          oneMeasurementDidTerminateSearch = false;
      DatabaseEntry learningEntry(1);
      for (auto& p: _measurements[_activeMethodTrace]) {
        if ( p.isAccurate() && p.isSearching() ) {
          logInfo( "activateOracle()", "start to learn for trace " << toString(_activeMethodTrace) );
          p.learn();
          if ( !p.isSearching() ) {
            logDebug( "activateOracle()", "entry fixed, so propagate its data: " << p.toString() );
            oneMeasurementDidTerminateSearch = true;
            learningEntry                    = p;
          }
        }
        else if (
          oneMeasurementDidTerminateSearch
          &&
          ( p.isSearching() || !p.isScaling() )
        ) {
          // propagate data
          p = DatabaseEntry( learningEntry, p.getBiggestProblemSize() );
          logDebug( "activateOracle()", "have propagated solution from " << learningEntry.toString() << " into " << p.toString() );
        }
      }
    }

    if (!_measurements.empty() ) {
      changeMeasuredMethodTrace();
      widenAccuracyOfCurrentlyStudiedMethodTraceAndRandomlyRestart();
    }
  }
}



bool sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize::hasLearnedSinceLastQuery() {
  bool result = _hasLearnedSinceLastQuery;
  _hasLearnedSinceLastQuery = false;
  return result;
}
