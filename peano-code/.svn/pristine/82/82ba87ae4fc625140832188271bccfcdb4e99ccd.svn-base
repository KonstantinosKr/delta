#if defined(SharedTBB) || defined(SharedTBBInvade)
#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/tbb/Core.h"
#include "tarch/Assertions.h"


tarch::logging::Log  tarch::multicore::Core::_log( "tarch::multicore::Core" );

const int tarch::multicore::Core::UseDefaultNumberOfThreads = 0;


tarch::multicore::Core::Core():
  #if defined(SharedTBBInvade)
  _invadeRoot(),
  #endif
  _numberOfThreads(::tbb::task_scheduler_init::default_num_threads()),
  _task_scheduler_init(_numberOfThreads) {
}


tarch::multicore::Core::~Core() {
}


tarch::multicore::Core& tarch::multicore::Core::getInstance() {
  static tarch::multicore::Core singleton;
  return singleton;
}


void tarch::multicore::Core::shutDown() {
  _task_scheduler_init.terminate();
  _numberOfThreads = -1;
}


void tarch::multicore::Core::configure( int numberOfThreads ) {
  if (_task_scheduler_init.is_active()) {
    _task_scheduler_init.terminate();
  }

  if (numberOfThreads==UseDefaultNumberOfThreads) {
    _numberOfThreads = ::tbb::task_scheduler_init::default_num_threads();
  }
  else {
    _numberOfThreads = numberOfThreads;
  }

  _task_scheduler_init.initialize( _numberOfThreads );
}


int tarch::multicore::Core::getNumberOfThreads() const {
  assertion( isInitialised() );
  return _numberOfThreads;
}


bool tarch::multicore::Core::isInitialised() const {
  return _task_scheduler_init.is_active();
}

#endif
