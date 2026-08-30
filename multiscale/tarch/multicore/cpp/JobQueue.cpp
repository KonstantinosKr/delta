#ifdef SharedCPP

#include "tarch/multicore/cpp/JobQueue.h"
#include "tarch/multicore/Jobs.h"


#include <sstream>


tarch::logging::Log tarch::multicore::internal::JobQueue::_log( "tarch::multicore::internal::JobQueue" );
#ifdef JobQueueUsesStackOfBefilledQueues
std::stack<int>    tarch::multicore::internal::JobQueue::latestQueuesBefilled;
std::atomic_flag   tarch::multicore::internal::JobQueue::latestQueueSpinLock;
#else
std::atomic<int>    tarch::multicore::internal::JobQueue::LatestQueueBefilled(0);
#endif


tarch::multicore::internal::JobQueue::JobQueue():
  _spinLock( ATOMIC_FLAG_INIT ) {
  _numberOfPendingJobs = 0;
}


tarch::multicore::internal::JobQueue::~JobQueue() {
}





std::string tarch::multicore::internal::JobQueue::toString() {
  std::ostringstream msg;
  msg << "(no-of-background-tasks:" << getBackgroundQueue().getNumberOfPendingJobs();
  for (int i=0; i<MaxNormalJobQueues; i++) {
	msg << ",queue[" << i << "]:" << getStandardQueue(i).getNumberOfPendingJobs();
  }
  msg << ")";
  return msg.str();
}


bool tarch::multicore::internal::JobQueue::processJobs( int maxNumberOfJobs ) {
  #ifdef UseNaiveImplementation
  _mutex.lock();
  if ( !_jobs.empty() && maxNumberOfJobs>0 ) {
    maxNumberOfJobs--;
    result = true;
    jobs::Job* job = _jobs.front();
    _jobs.pop_front();
    bool reenqueue = job->run();
    if (reenqueue) {
      _jobs.push_back( job );
    }
    else {
      delete job;
      _numberOfPendingJobs.fetch_sub(1);
    }
  }
  _mutex.unlock();
  return result;
  #else

  if (_numberOfPendingJobs.load()>0) {
    #ifdef JobQueueUsesSpinLockInsteadOfMutex
    while (_spinLock.test_and_set(std::memory_order_acquire)); // spin
    #else
    _mutex.lock();
    #endif

    // might have changed meanwhile
    maxNumberOfJobs = std::min( maxNumberOfJobs, static_cast<int>( _jobs.size() ));

    if (maxNumberOfJobs==0) {
      #ifdef JobQueueUsesSpinLockInsteadOfMutex
      _spinLock.clear(std::memory_order_release);
      #else
      _mutex.unlock();
      #endif

      return false;
    }
    else {
      // This is the first thing we do to avoid that others try to read from
      // the queue even if we are about to take all elements out.
      _numberOfPendingJobs.fetch_sub(maxNumberOfJobs);

      std::list< jobs::Job* >::iterator lastElementToBeProcessed  = _jobs.begin();
      for (int i=0; i<maxNumberOfJobs; i++) {
        lastElementToBeProcessed++;
      }
      std::list< jobs::Job* > localList;
      localList.splice( localList.begin(), _jobs, _jobs.begin(), lastElementToBeProcessed );

      logDebug( "processJobs(int)", "spliced " << maxNumberOfJobs << " job(s) from job queue and will process those now" );

      #ifdef JobQueueUsesSpinLockInsteadOfMutex
      _spinLock.clear(std::memory_order_release);
      #else
      _mutex.unlock();
      #endif

      for (auto& p: localList) {
        bool reenqueue = p->run();
        if (reenqueue) {
          addJob( p );
        }
        else {
          delete p;
        }
      }

      return true;
    }
  }
  else return false;
  #endif
}


void tarch::multicore::internal::JobQueue::addJob( jobs::Job* job ) {
  #ifdef JobQueueUsesSpinLockInsteadOfMutex
  while (_spinLock.test_and_set(std::memory_order_acquire)); // spin
  #else
  _mutex.lock();
  #endif

  _jobs.push_back(job);

  #ifdef JobQueueUsesSpinLockInsteadOfMutex
  _spinLock.clear(std::memory_order_release);
  #else
  _mutex.unlock();
  #endif

  _numberOfPendingJobs.fetch_add(1);
}


void tarch::multicore::internal::JobQueue::addJobWithHighPriority( jobs::Job* job ) {
  #ifdef JobQueueUsesSpinLockInsteadOfMutex
  while (_spinLock.test_and_set(std::memory_order_acquire)); // spin
  #else
  _mutex.lock();
  #endif

  _jobs.push_front(job);

  #ifdef JobQueueUsesSpinLockInsteadOfMutex
  _spinLock.clear(std::memory_order_release);
  #else
  _mutex.unlock();
  #endif

  _numberOfPendingJobs.fetch_add(1);
}


#endif
