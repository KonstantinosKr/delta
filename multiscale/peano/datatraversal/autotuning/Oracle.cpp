#include "peano/datatraversal/autotuning/Oracle.h"
#include "peano/datatraversal/autotuning/OracleForOnePhaseDummy.h"
#include "tarch/Assertions.h"
#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/Lock.h"


#include <fstream>
#include <sstream>


#include <cerrno>
#include <cstring>


tarch::logging::Log                 peano::datatraversal::autotuning::Oracle::_log( "peano::datatraversal::autotuning::Oracle" ) ;


peano::datatraversal::autotuning::Oracle& peano::datatraversal::autotuning::Oracle::getInstance() {
  static peano::datatraversal::autotuning::Oracle singleton;
  return singleton;
}


peano::datatraversal::autotuning::Oracle::Oracle():
  _oracles(),
  #if defined(SharedMemoryParallelisation)
  _currentOracle(0),
  #endif
  _oraclePrototype(nullptr),
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


void peano::datatraversal::autotuning::Oracle::plotStatistics(const std::string& filename) {
  if (!filename.empty()) {
    std::ofstream f(filename.c_str(),std::ios::out );
    if (f.is_open()) {
      f << "number of oracles=" << _numberOfOracles << std::endl;
      f << "oracles reserved for Peano kernel=4" << std::endl;

      for (int i=0; i<_numberOfOracles;i++) {
        assertion(_oracles[i]!=nullptr);
        _oracles[i]->plotStatistics(f,i);
      }

      f.flush();
      f.close();
    }
    else {
      logError("plotStatistics", "could not write into " << filename << ", error message: " << std::strerror(errno) );
    }
  }
  else {
    std::ostringstream f;
    for (int i=0; i<_numberOfOracles;i++) {
      _oracles[i]->plotStatistics(f,i);
    }
    logInfo( "plotStatistics(string)", f.str() );
  }
}


void peano::datatraversal::autotuning::Oracle::loadStatistics(const std::string& filename) {
  assertion(_numberOfOracles>0);
  for (int i=0; i<_numberOfOracles;i++) {
    assertion3(_oracles[i]!=nullptr,i,_numberOfOracles, "please ensure that loadStatistics is called after the repository has been created");
    _oracles[i]->loadStatistics(filename,i);
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
  logTraceInWith1Argument( "setNumberOfOracles(int)", value);
  assertion( value>0 );

  deleteOracles();
  _numberOfOracles=value;

  logTraceOut( "setNumberOfOracles(int)");
}


void peano::datatraversal::autotuning::Oracle::createOracles() {
  #if defined(SharedMemoryParallelisation)
  logTraceIn( "createOracles()");
  assertion( _oracles.size()==0 );

  if (_numberOfOracles==0) {
    logWarning( "createOracles(int)", "no number of oracles set. Have you created repositories before?" );
  }
  else if (_oraclePrototype!=0) {
    _oracles.resize(_numberOfOracles);
    for (int i=0; i<_numberOfOracles; i++) {
      _oracles[i] = _oraclePrototype->createNewOracle();
    }
  }

  logTraceOut( "createOracles()");
  #endif
}


void peano::datatraversal::autotuning::Oracle::deleteOracles() {
  logTraceIn( "deleteOracles()");

  for (auto oracle: _oracles) {
    delete oracle;
  }

  logTraceOut( "deleteOracles()");
}


void peano::datatraversal::autotuning::Oracle::switchToOracle(int id) {
  #if defined(SharedMemoryParallelisation)
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));
  _oracles[_currentOracle]->deactivateOracle();

  _currentOracle =id;

  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()) );
  _oracles[_currentOracle]->activateOracle();
  #endif
}


peano::datatraversal::autotuning::GrainSize peano::datatraversal::autotuning::Oracle::parallelise(int problemSize, MethodTrace askingMethod ) {
  assertion2( problemSize>=0, problemSize, toString(askingMethod) );

  #if defined(SharedMemoryParallelisation)
  assertion( _currentOracle>=0 );
  assertion4( _currentOracle<static_cast<int>(_oracles.size()), _currentOracle, _oracles.size(), problemSize, toString(askingMethod) );
  return std::move( _oracles[_currentOracle]->parallelise(problemSize,askingMethod) );
  #else
  return GrainSize( 0, false, problemSize, askingMethod, nullptr);
  #endif
}
