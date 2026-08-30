#ifdef SharedCPP

#include "tarch/Assertions.h"
#include "tarch/multicore/Jobs.h"
#include "tarch/multicore/cpp/JobConsumer.h"
#include "tarch/multicore/cpp/JobQueue.h"
#include "tarch/multicore/Core.h"

#include <thread>
#include <sched.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdint>
#include <climits>


tarch::logging::Log   tarch::multicore::internal::JobConsumer::_log( "tarch::multicore::internal::JobConsumer" );
const int             tarch::multicore::internal::JobConsumer::MinNumberOfJobs = 32;


tarch::multicore::internal::JobConsumer::JobConsumer(int pinCore, bool hyperthreading, JobConsumerController* controller, cpu_set_t*  mask):
  _pinCore(pinCore),
  _hyperthreading(hyperthreading),
  _controller(controller),
  _mask(mask) {
}


tarch::multicore::internal::JobConsumer::~JobConsumer() {
}


void tarch::multicore::internal::JobConsumer::operator()() {
  if (_pinCore!=NoPinning) {
	addMask(_pinCore,_mask);
  }

  JobConsumerController::State state = JobConsumerController::State::Running;
  while (state!=JobConsumerController::State::TerminateTriggered) {
	switch (state) {
      case JobConsumerController::State::Running:
        {
          bool foundJob = true;
          bool processedJob = false;
          int  lastRecentlyBefilledQueue = tarch::multicore::internal::JobQueue::getLatestQueueBefilled();
          while (foundJob) {
            foundJob = false;
            for (int i=0; i<internal::JobQueue::MaxNormalJobQueues; i++) {
              const int queueNumber = (i + lastRecentlyBefilledQueue);
              const int jobs = internal::JobQueue::getStandardQueue(queueNumber).getNumberOfPendingJobs();
              if (jobs>0) {
                logDebug( "operator()", "consumer task (pin=" << _pinCore << ") grabbed " << jobs << " job(s) from class " <<  queueNumber );
                internal::JobQueue::getStandardQueue(queueNumber).processJobs( MinNumberOfJobs );
                foundJob = true;
                processedJob = true;
                i--;
              }
              else if (i==0) {
                tarch::multicore::internal::JobQueue::latestQueueBefilledIsEmpty();
              }

              if (lastRecentlyBefilledQueue != tarch::multicore::internal::JobQueue::getLatestQueueBefilled()) {
                logDebug( "operator()", "another queue is befilled, so restart" );
                lastRecentlyBefilledQueue = tarch::multicore::internal::JobQueue::getLatestQueueBefilled();
                i = 0;
              }
        	}
          }

          processedJob |= tarch::multicore::jobs::finishToProcessBackgroundJobs();

          if (!processedJob and _hyperthreading) {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
          }
        }
    	break;
      case JobConsumerController::State::TerminateTriggered:
    	logInfo( "operator()", "found terminate-triggered flag" );
        break;
      case JobConsumerController::State::Terminated:
      	assertionMsg(false, "should not enter" );
        break;
      case JobConsumerController::State::Suspended:
    	assertionMsg(false, "not supported yet" );
    	break;
	}

    _controller->lock();
	state = _controller->state;
	_controller->unlock();
  }

  if (_pinCore!=NoPinning) {
    removeMask();
  }

  #ifdef Asserts
  logInfo( "operator()", "job consumer is going down" );
  #endif

  _controller->lock();
  _controller->state = JobConsumerController::State::Terminated;
  _controller->unlock();
}


void tarch::multicore::internal::JobConsumer::addMask(int core, cpu_set_t* mask) {
  const int    ncpus = std::thread::hardware_concurrency();
  const size_t size = CPU_ALLOC_SIZE( ncpus );
  const int    num_cpus = CPU_COUNT_S( size, mask );
  const int    pinningStep = 1;
  int thr_idx =  core % num_cpus; // To limit unique number in [0; num_cpus-1] range

  // Place threads with specified step
  int cpu_idx = 0;
  for ( int i = 0, offset = 0; i<thr_idx; ++i ) {
    cpu_idx += pinningStep;
    if ( cpu_idx >= num_cpus ) {
      cpu_idx = ++offset;
    }
  }

  // Find index of 'cpu_idx'-th bit equal to 1
  int mapped_idx = -1;
  while ( cpu_idx >= 0 ) {
    if ( CPU_ISSET_S( ++mapped_idx, size, mask ) )
      --cpu_idx;
  }
  cpu_set_t *target_mask = CPU_ALLOC( ncpus );
  CPU_ZERO_S( size, target_mask );
  CPU_SET_S( mapped_idx, size, target_mask );
  const int err = sched_setaffinity( 0, size, target_mask );
  if ( err ) {
    logError( "addMask()","failed to set thread affinity!");
    exit( EXIT_FAILURE );
  }
  else {
    struct rlimit l;
    getrlimit(RLIMIT_STACK, &l);
    logInfo( "addMask()", "consumer task (pin=" << core << ") was mapped onto hardware thread " << thr_idx << " and got pinned to CPU " << mapped_idx << ", stack size is " << l.rlim_cur );
  }
  CPU_FREE( target_mask );
}


void tarch::multicore::internal::JobConsumer::removeMask() {

}


tarch::multicore::internal::JobConsumerController::JobConsumerController():
  _spinLock( ATOMIC_FLAG_INIT ) {
  lock();
  state = State::Running;
  unlock();
}


tarch::multicore::internal::JobConsumerController::~JobConsumerController() {
  assertion( state==State::Terminated );
}



std::string tarch::multicore::internal::JobConsumerController::toString() const {
  switch (state) {
    case State::Running:
      return "(running)";
      break;
    case State::TerminateTriggered:
      return "(terminate-triggered)";
      break;
    case State::Terminated:
      return "(terminated)";
      break;
    case State::Suspended:
      return "(suspended)";
      break;
  }
  return "<undef>";
}


void tarch::multicore::internal::JobConsumerController::lock() {
  while (_spinLock.test_and_set(std::memory_order_acquire)); // spin
}


void tarch::multicore::internal::JobConsumerController::unlock() {
  _spinLock.clear(std::memory_order_release);
}

#endif

