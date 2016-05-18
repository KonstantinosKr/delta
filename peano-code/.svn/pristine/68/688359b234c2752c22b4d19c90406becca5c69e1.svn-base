#include "peano/datatraversal/autotuning/Oracle.h"
#include "peano/datatraversal/autotuning/OracleForOnePhaseDummy.h"
#include "tarch/Assertions.h"
#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/Lock.h"

#if defined(SharedCobra)
#include "tarch/multicore/cobra/Core.h"
#endif


#include <fstream>
#include <sstream>


tarch::logging::Log                 peano::datatraversal::autotuning::Oracle::_log( "peano::datatraversal::autotuning::Oracle" ) ;
tarch::multicore::BooleanSemaphore  peano::datatraversal::autotuning::Oracle::_semaphore;


peano::datatraversal::autotuning::Oracle& peano::datatraversal::autotuning::Oracle::getInstance() {
  static peano::datatraversal::autotuning::Oracle singleton;
  return singleton;
}


peano::datatraversal::autotuning::Oracle::Oracle():
  _oracles(),
  _watchSinceLastSwitchCall("peano::datatraversal::autotuning::Oracle", "Oracle()", false),
  _currentPhase(0),
  _oraclePrototype(0),
  _numberOfOracles(0) {
}


void peano::datatraversal::autotuning::Oracle::setOracle( OracleForOnePhase* oraclePrototype ) {
  logTraceIn( "setOracle(Oracle*)");
  assertion( oraclePrototype!=0 );

  if (_oraclePrototype!=0) {
    delete _oraclePrototype;
  }
  _oraclePrototype = oraclePrototype;

  deleteOracles();
  createOracles();
  logTraceOut( "setOracle(Oracle*)");
}


int peano::datatraversal::autotuning::Oracle::getTotalNumberOfOracles() const {
  return (_numberOfOracles+AdapterStatesReservedForRepositorySteering)*peano::datatraversal::autotuning::NumberOfDifferentMethodsCalling;
}


int peano::datatraversal::autotuning::Oracle::getKey(const MethodTrace& askingMethod ) const {
  return _currentPhase*peano::datatraversal::autotuning::NumberOfDifferentMethodsCalling + askingMethod;
}


void peano::datatraversal::autotuning::Oracle::plotStatistics(const std::string& filename) {
  assertion(_oracles!=0);

  if (!filename.empty()) {
    std::ofstream f(filename.c_str(),std::ios::out );
    if (f.is_open()) {
      for (int i=0; i<getTotalNumberOfOracles();i++) {
        assertion(_oracles[i]._oracle!=0);
        _oracles[i]._oracle->plotStatistics(f);
      }
      f.flush();
      f.close();
    }
    else {
      logError("plotStatistics", "could not write into " << filename);
    }
  }
  else {
    std::ostringstream f;
    for (int i=0; i<getTotalNumberOfOracles();i++) {
      assertion(_oracles[i]._oracle!=0);
      _oracles[i]._oracle->plotStatistics(f);
    }
    logInfo( "plotStatistics(string)", f.str() );
  }
}


void peano::datatraversal::autotuning::Oracle::loadStatistics(const std::string& filename) {
  assertion(_oracles!=0);
  for (int i=0; i<getTotalNumberOfOracles();i++) {
    assertion(_oracles[i]._oracle!=0);
    _oracles[i]._oracle->loadStatistics(filename);
  }
}


peano::datatraversal::autotuning::Oracle::~Oracle() {
  deleteOracles();
  if (_oraclePrototype!=0) {
    delete _oraclePrototype;
    _oraclePrototype = 0;
  }
}


void peano::datatraversal::autotuning::Oracle::setNumberOfOracles(int value) {
  assertion( value>0 );
  logTraceInWith1Argument( "setNumberOfOracles(int)", value);

  deleteOracles();
  _numberOfOracles=value;
  createOracles();

  logTraceOut( "setNumberOfOracles(int)");
}


void peano::datatraversal::autotuning::Oracle::createOracles() {
  #if defined(SharedMemoryParallelisation)
  logTraceIn( "createOracles()");

  if (_oraclePrototype==0) {
	  logWarning( "createOracles(int)", "no oracle type configured. Perhaps forgot to call peano::datatraversal::autotuning::Oracle::setOracle(). Peano uses default oracle" );
	  _oraclePrototype = new OracleForOnePhaseDummy(true);
  }
  else {
    assertion( _oracles==0 );
    assertion( _oraclePrototype!=0 );

    _oracles = new ValuesPerOracleKey[getTotalNumberOfOracles()];

    for (int i=0; i<getTotalNumberOfOracles(); i++) {
      const int                phase = i/peano::datatraversal::autotuning::NumberOfDifferentMethodsCalling;
      const MethodTrace        trace = toMethodTrace(i-phase*peano::datatraversal::autotuning::NumberOfDifferentMethodsCalling);
      _oracles[i]._measureTime = false;
      _oracles[i]._watch       = new tarch::timing::Watch("peano::datatraversal::autotuning::Oracle", "createOracles(int)", false);
      _oracles[i]._oracle      = _oraclePrototype->createNewOracle(phase,trace);
      _oracles[i]._recursiveCallsForThisOracle  = 0;
    }
  }

  logTraceOut( "createOracles()");
  #endif
}


void peano::datatraversal::autotuning::Oracle::deleteOracles() {
  logTraceIn( "deleteOracles()");
  if (_oracles != 0) {
    for (int i=0; i<getTotalNumberOfOracles(); i++) {
      delete _oracles[i]._oracle;
      delete _oracles[i]._watch;
    }

    delete[] _oracles;
    _oracles = 0;
  }
  logTraceOut( "deleteOracles()");
}


void peano::datatraversal::autotuning::Oracle::switchToOracle(int id) {
  #if defined(SharedMemoryParallelisation)
  assertion1( id>=0, id );

  #if defined(SharedCobra)
  tarch::multicore::cobra::Core::getInstance().getScheduler().gc();
  #endif
  _watchSinceLastSwitchCall.stopTimer();

  assertion(_oraclePrototype!=0);

  const double erasedTime = _watchSinceLastSwitchCall.getCalendarTime();
  for (int i=0; i<getTotalNumberOfOracles(); i++) {
    _oracles[i]._oracle->informAboutElapsedTimeOfLastTraversal(erasedTime);
  }

  _currentPhase=id;

  _watchSinceLastSwitchCall.startTimer();
  #endif
}


int peano::datatraversal::autotuning::Oracle::parallelise(int problemSize, MethodTrace askingMethod ) {
  logTraceInWith3Arguments( "parallelise(int,int)", problemSize, askingMethod, toString(askingMethod) );

  assertion2( problemSize>=0, problemSize, toString(askingMethod) );

  int result;

  #if defined(SharedMemoryParallelisation)
  const int key = getKey(askingMethod);
  assertionMsg(_oracles!=0, "forgot to initialise shared memory environment due to setOracle()?");
  assertion(key>=0);
  assertion(key<getTotalNumberOfOracles());

  tarch::multicore::Lock lock(_semaphore);

  assertion2( !_oracles[key]._recursiveCallsForThisOracle>=0, toString( askingMethod), _currentPhase );
  _oracles[key]._recursiveCallsForThisOracle++;

  if (problemSize>0 && _oracles[key]._recursiveCallsForThisOracle==1) {
    const std::pair<int,bool> oracleDecision =  _oracles[key]._oracle->parallelise(problemSize);
    result                               = oracleDecision.first;
    _oracles[key]._lastGrainSizeReturned = oracleDecision.first;
    _oracles[key]._measureTime           = oracleDecision.second;
    if ( oracleDecision.second ) {
      _oracles[key]._watch->startTimer();
    }
  }
  else if (problemSize>0) {
    result = _oracles[key]._lastGrainSizeReturned;
  }
  else {
    result = 0;
   _oracles[key]._measureTime = false;
  }
  #else
  result = 0;
  #endif

  assertion3( result >=0, result, problemSize, toString(askingMethod) );
  assertion3( result <= problemSize, result, problemSize, toString(askingMethod) );
  assertion2( problemSize>0 || result==0, problemSize, toString(askingMethod) );

  logTraceOutWith1Argument( "parallelise(int,int)", result );
  return result;
}


void peano::datatraversal::autotuning::Oracle::parallelSectionHasTerminated(MethodTrace askingMethod) {
  logTraceInWith1Argument( "parallelSectionHasTerminated(...)", toString(askingMethod) );
  #if defined(SharedMemoryParallelisation)
  const int key = getKey(askingMethod);
  assertion(_oraclePrototype!=0);
  assertion(_oracles!=0);
  assertion(key>=0);
  assertion(key<getTotalNumberOfOracles());

  tarch::multicore::Lock lock(_semaphore);

  _oracles[key]._recursiveCallsForThisOracle--;
  assertion2( _oracles[key]._recursiveCallsForThisOracle>=0, toString( askingMethod), _currentPhase );

  if (_oracles[key]._measureTime && _oracles[key]._recursiveCallsForThisOracle==0 ) {
    _oracles[key]._watch->stopTimer();
    _oracles[key]._oracle->parallelSectionHasTerminated(_oracles[key]._watch->getCalendarTime());
  }
  #endif
  logTraceOut( "parallelSectionHasTerminated(...)" );
}
