#include "peano/datatraversal/autotuning/OracleForOnePhaseDummy.h"
#include "tarch/Assertions.h"

#include <cstdlib>
#include <limits>
#include <iostream>
#include <fstream>


tarch::logging::Log  peano::datatraversal::autotuning::OracleForOnePhaseDummy::_log( "peano::datatraversal::autotuning::OracleForOnePhaseDummy" );


peano::datatraversal::autotuning::OracleForOnePhaseDummy::OracleForOnePhaseDummy(
  bool useMultithreading                  ,
  bool measureRuntimes                    ,
  int  grainSizeOfUserDefinedRegions      ,
  int  splitTheTree                       ,
  bool pipelineDescendProcessing          ,
  bool pipelineAscendProcessing           ,
  int  smallestGrainSizeForAscendDescend  ,
  int  grainSizeForAscendDescend          ,
  int  smallestGrainSizeForEnterLeaveCell ,
  int  grainSizeForEnterLeaveCell         ,
  int  smallestGrainSizeForTouchFirstLast ,
  int  grainSizeForTouchFirstLast         ,
  int  smallestGrainSizeForSplitLoadStore ,
  int  grainSizeForSplitLoadStore         ,
  int  adapterNumber                      ,
  const MethodTrace& methodTrace
):
  _useMulticore(useMultithreading),
  _measureRuntimes(measureRuntimes),
  _grainSizeOfUserDefinedRegions(grainSizeOfUserDefinedRegions),
  _executionTime(),
  _adapterNumber(adapterNumber),
  _methodTrace(methodTrace),
  _splitTheTree(splitTheTree),
  _pipelineDescendProcessing(pipelineDescendProcessing),
  _pipelineAscendProcessing(pipelineAscendProcessing),
  _grainSize(std::numeric_limits<int>::max()),
  _smallestProblemSize(std::numeric_limits<int>::max()),
  _lastProblemSize(-1),
  _smallestGrainSizeForAscendDescend(smallestGrainSizeForAscendDescend),
  _grainSizeForAscendDescend(grainSizeForAscendDescend),
  _smallestGrainSizeForEnterLeaveCell(smallestGrainSizeForEnterLeaveCell),
  _grainSizeForEnterLeaveCell(grainSizeForEnterLeaveCell),
  _smallestGrainSizeForTouchFirstLast(smallestGrainSizeForTouchFirstLast),
  _grainSizeForTouchFirstLast(grainSizeForTouchFirstLast),
  _smallestGrainSizeForSplitLoadStore(smallestGrainSizeForSplitLoadStore),
  _grainSizeForSplitLoadStore(grainSizeForSplitLoadStore) {

  if ( _pipelineAscendProcessing && _methodTrace == PipelineAscendTask ) {
    _grainSize           = 1;
    _smallestProblemSize = 0;
  }
  else if ( _pipelineDescendProcessing && _methodTrace == PipelineDescendTask ) {
    _grainSize           = 1;
    _smallestProblemSize = 0;
  }
  else if (
    ( _methodTrace==AscendOnRegularStationaryGrid        ||
      _methodTrace==DescendOnRegularStationaryGrid
    ) &&
    _splitTheTree !=2
  ) {
    _grainSize           = grainSizeForAscendDescend;
    _smallestProblemSize = smallestGrainSizeForAscendDescend;
  }
  else if (
    ( _methodTrace==CallEnterCellOnRegularStationaryGrid ||
      _methodTrace==CallLeaveCellOnRegularStationaryGrid
    ) &&
    _splitTheTree !=2
  ) {
    _grainSize           = grainSizeForEnterLeaveCell;
    _smallestProblemSize = smallestGrainSizeForEnterLeaveCell;
  }
  else if (
    (
      _methodTrace==CallTouchFirstTimeOnRegularStationaryGrid ||
      _methodTrace==CallTouchLastTimeOnRegularStationaryGrid
    ) &&
    _splitTheTree !=2
  ) {
    _grainSize           = grainSizeForTouchFirstLast;
    _smallestProblemSize = smallestGrainSizeForTouchFirstLast;
  }
  else if (
    _splitTheTree > 0  &&
    (
      _methodTrace == SplitLoadVerticesTaskOnRegularStationaryGrid  ||
      _methodTrace == SplitStoreVerticesTaskOnRegularStationaryGrid
    )
  ) {
    _grainSize           = grainSizeForSplitLoadStore;
    _smallestProblemSize = smallestGrainSizeForSplitLoadStore;
  }
  else if (
    _methodTrace>=UserDefined0 && _methodTrace<=UserDefined12
  )
  {
    _grainSize           = grainSizeOfUserDefinedRegions;
    _smallestProblemSize = grainSizeOfUserDefinedRegions;
  }
}


std::pair<int,bool> peano::datatraversal::autotuning::OracleForOnePhaseDummy::parallelise(int problemSize) {
  assertionEquals1( _lastProblemSize, -1, peano::datatraversal::autotuning::toString(_methodTrace) );
  if (_useMulticore) {
    if (problemSize < _smallestProblemSize) {
      if (_measureRuntimes) _lastProblemSize = problemSize;
      return std::pair<int,bool>(0,_measureRuntimes);
    }
    else {
      if (_measureRuntimes) _lastProblemSize = problemSize;
      return std::pair<int,bool>(_grainSize,_measureRuntimes);
    }
  }
  else {
    if (_measureRuntimes) _lastProblemSize = problemSize;
    return std::pair<int,bool>(0,_measureRuntimes);
  }
}


void peano::datatraversal::autotuning::OracleForOnePhaseDummy::parallelSectionHasTerminated(double elapsedCalendarTime) {
  assertion1( _lastProblemSize>0, toString() );
  _executionTime[_lastProblemSize].setValue( elapsedCalendarTime );
  _executionTime[_lastProblemSize].setAccuracy(1.0e-5);
  _lastProblemSize = -1;
}


void peano::datatraversal::autotuning::OracleForOnePhaseDummy::loadStatistics(const std::string& filename) {
}


void peano::datatraversal::autotuning::OracleForOnePhaseDummy::plotStatistics(std::ostream& out) const {
  for (std::map<int, tarch::timing::Measurement>::const_iterator p=_executionTime.begin(); p!=_executionTime.end(); p++) {
    if (p->second.getNumberOfMeasurements()>0) {
      out <<
          ",averaged runtime for " << peano::datatraversal::autotuning::toString(_methodTrace)
          << " in " << _adapterNumber-peano::datatraversal::autotuning::NumberOfPredefinedAdapters+1 << "th adapter "
          << " for problem size " << p->first << ": " <<
          p->second.toString();
    }
  }
}


peano::datatraversal::autotuning::OracleForOnePhaseDummy::~OracleForOnePhaseDummy() {
}


peano::datatraversal::autotuning::OracleForOnePhase* peano::datatraversal::autotuning::OracleForOnePhaseDummy::createNewOracle(int adapterNumber, const MethodTrace& methodTrace) const {
  return new OracleForOnePhaseDummy(
    _useMulticore,
    _measureRuntimes,
    _grainSizeOfUserDefinedRegions,
    _splitTheTree,
    _pipelineDescendProcessing,
    _pipelineAscendProcessing,
    _smallestGrainSizeForAscendDescend,
    _grainSizeForAscendDescend,
    _smallestGrainSizeForEnterLeaveCell,
    _grainSizeForEnterLeaveCell,
    _smallestGrainSizeForTouchFirstLast,
    _grainSizeForTouchFirstLast,
    _smallestGrainSizeForSplitLoadStore,
    _grainSizeForSplitLoadStore,
    adapterNumber,
    methodTrace
  );
}


void peano::datatraversal::autotuning::OracleForOnePhaseDummy::informAboutElapsedTimeOfLastTraversal(double elapsedTime) {
}


std::string peano::datatraversal::autotuning::OracleForOnePhaseDummy::toString() const {
  std::ostringstream msg;

  msg << "(multicore="           << _useMulticore
      << ",measure-runtimes="    << _measureRuntimes
      << ",adapter-number="      << _adapterNumber
      << ",method="              << peano::datatraversal::autotuning::toString(_methodTrace)
      << ",split-tree="          << _splitTheTree
      << ",pipeline-descend="    << _pipelineDescendProcessing
      << ",pipeline-ascend="     << _pipelineAscendProcessing
      << ",grain-size="            << _grainSize
      << ",smallest-problem-size=" << _smallestProblemSize
      << ",last-problem-size="   << _lastProblemSize
      << ")";

  return msg.str();
}
