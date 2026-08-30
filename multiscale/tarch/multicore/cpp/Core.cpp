#include "tarch/multicore/Core.h"
#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/compiler/CompilerSpecificSettings.h"


#ifdef CompilerHasSysinfo
#include <sched.h>
#endif

#ifdef SharedCPP

#include "JobConsumer.h"

#include <thread>
#include <sched.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdint>
#include <climits>


tarch::logging::Log  tarch::multicore::Core::_log( "tarch::multicore::Core" );


tarch::multicore::Core::Core():
  _numberOfThreads(-1),
  _pin(false) {

  configure( UseDefaultNumberOfThreads );

  const int MaxNumberOfSupportedCPUs = 16*1024;
  for ( int ncpus = sizeof(cpu_set_t)/CHAR_BIT; ncpus < MaxNumberOfSupportedCPUs; ncpus <<= 1 ) {
    _mask = CPU_ALLOC( ncpus );
    if ( !_mask ) break;
    const size_t size = CPU_ALLOC_SIZE( ncpus );
    CPU_ZERO_S( size, _mask );
    const int err = sched_getaffinity( 0, size, _mask );
    if ( !err ) break;

    CPU_FREE( _mask );
    _mask = NULL;
    if ( errno != EINVAL )  break;
  }
  if ( !_mask ) {
    logWarning( "Core()","failed to obtain process affinity mask");
  }
}


tarch::multicore::Core::~Core() {
  shutDown();

  for (auto& p: _jobConsumerControllers) {
	delete p;
	p = nullptr;
  }
}


tarch::multicore::Core& tarch::multicore::Core::getInstance() {
  static Core instance;
  return instance;
}


void tarch::multicore::Core::createOneJobConsumerPerThread() {
  for (int i=1; i<_numberOfThreads; i++) {
    tarch::multicore::internal::JobConsumerController* newController = new tarch::multicore::internal::JobConsumerController();
    _jobConsumerControllers.push_back( newController );

    std::thread t(
      tarch::multicore::internal::JobConsumer(
        _pin ? i : tarch::multicore::internal::JobConsumer::NoPinning,
        _numberOfThreads > static_cast<int>(std::thread::hardware_concurrency())/2,
        newController,
		_mask
      )
    );
    t.detach();

    #ifdef Asserts
    logInfo( "createOneJobConsumerPerThread()", "spawn consumer for hardware thread " << i << " (" << _numberOfThreads << " threads in total)");
    #endif
  }
}


void tarch::multicore::Core::shutdownRunningJobConsumers() {
  for (auto& p: _jobConsumerControllers) {
    p->lock();
    if (p->state != internal::JobConsumerController::State::Terminated ) {
      p->state = internal::JobConsumerController::State::TerminateTriggered;
    }
    p->unlock();
  }
}


void tarch::multicore::Core::configure( int numberOfThreads ) {
  shutdownRunningJobConsumers();

  if (numberOfThreads==UseDefaultNumberOfThreads) {
    _numberOfThreads = std::thread::hardware_concurrency();
  }
  else {
	_numberOfThreads = numberOfThreads;
  }

  createOneJobConsumerPerThread();
}


void tarch::multicore::Core::shutDown() {
  shutdownRunningJobConsumers();

  #ifdef Asserts
  logInfo( "createOneJobConsumerPerThread()", "wait for all " << _jobConsumerControllers.size() << " job consumer threads to go down" );
  #endif

  bool allConsumersHaveTerminated = false;
  while (!allConsumersHaveTerminated) {
    allConsumersHaveTerminated = true;

    for (auto& p: _jobConsumerControllers) {
      p->lock();
      allConsumersHaveTerminated &= (p->state == internal::JobConsumerController::State::Terminated);
      p->unlock();
    }
  }

  #ifdef Asserts
  logInfo( "createOneJobConsumerPerThread()", "all job consumer threads are down" );
  #endif
}


bool tarch::multicore::Core::isInitialised() const {
  return true;
}


int tarch::multicore::Core::getNumberOfThreads() const {
  return _numberOfThreads;
}


void tarch::multicore::Core::pinThreads(bool value) {
  _pin = value;

  #ifdef Asserts
  logInfo( "createOneJobConsumerPerThread()", "shut down and restart with pinning" );
  #endif

  shutdownRunningJobConsumers();

  createOneJobConsumerPerThread();

  if (_pin) {
	internal::JobConsumer::addMask( 0, _mask );
  }
  else {
	internal::JobConsumer::removeMask();
  }
}

#endif
