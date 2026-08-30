#include "peano/datatraversal/autotuning/GrainSize.h"
#include "peano/datatraversal/autotuning/OracleForOnePhase.h"
#include "tarch/Assertions.h"


peano::datatraversal::autotuning::GrainSize::GrainSize(int grainSize, bool useTimer, int problemSize, MethodTrace askingMethod, OracleForOnePhase* hostOracle):
  _grainSize(grainSize),
  _useTimer(useTimer),
  _problemSize(problemSize),
  _askingMethod(askingMethod),
  _hostOracle(hostOracle),
  _watch(nullptr) {
  assertion5(grainSize>=0,grainSize, useTimer, problemSize, toString(askingMethod), (hostOracle==nullptr));
  assertion5(grainSize<problemSize,grainSize, useTimer, problemSize, toString(askingMethod), (hostOracle==nullptr));

  assertion5( !_useTimer || _hostOracle!=nullptr,grainSize, useTimer, problemSize, toString(askingMethod), (hostOracle==nullptr));

  if (_useTimer) {
    _watch = new tarch::timing::Watch("peano::datatraversal::autotuning::GrainSize", "GrainSize(...)", false);
    assertion(_hostOracle!=nullptr);
  }
}


peano::datatraversal::autotuning::GrainSize::GrainSize(GrainSize&& movedObject):
  _grainSize(movedObject._grainSize),
  _useTimer(movedObject._useTimer),
  _problemSize(movedObject._problemSize),
  _askingMethod(movedObject._askingMethod),
  _hostOracle(movedObject._hostOracle),
  _watch(movedObject._watch) {
  if (_useTimer) {
    assertion(_hostOracle!=nullptr);
    movedObject._watch    = nullptr;
  }
}


peano::datatraversal::autotuning::GrainSize::~GrainSize() {
  if (_useTimer && _watch!=nullptr) {
    parallelSectionHasTerminated();
  }
}


int peano::datatraversal::autotuning::GrainSize::getGrainSize() const {
  return _grainSize;
}


bool peano::datatraversal::autotuning::GrainSize::runsParallel() const {
  return _grainSize>0;
}


void peano::datatraversal::autotuning::GrainSize::parallelSectionHasTerminated() {
  if ( _useTimer ) {
    assertion( _watch!=nullptr );
    assertion( _hostOracle!=nullptr );
    assertion( _problemSize>0 );

    _watch->stopTimer();
    const double cost = _watch->getCalendarTime() / static_cast<double>(_problemSize);

    _hostOracle->parallelSectionHasTerminated(_problemSize,_grainSize,_askingMethod,cost);

    delete _watch;
    _watch = nullptr;
  }
}
