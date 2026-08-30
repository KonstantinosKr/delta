#include "tarch/multicore/BooleanSemaphore.h"


// This implementation is valid iff neither OpenMP nor TBBs nor any other
// shared memory parallelisation are active

#if defined(SharedCPP)

tarch::multicore::BooleanSemaphore::BooleanSemaphore():
  #ifdef BooleanSemaphoreUsesASpinLock
  _spinLock( ATOMIC_FLAG_INIT ) {
  #else
  _mutex() {
  #endif
}


tarch::multicore::BooleanSemaphore::~BooleanSemaphore() {
}


void tarch::multicore::BooleanSemaphore::enterCriticalSection() {
  #ifdef BooleanSemaphoreUsesASpinLock
  while (_spinLock.test_and_set(std::memory_order_acquire)); // spin
  #else
  _mutex.lock();
  #endif
}


void tarch::multicore::BooleanSemaphore::leaveCriticalSection() {
  #ifdef BooleanSemaphoreUsesASpinLock
  _spinLock.clear(std::memory_order_release);
  #else
  _mutex.unlock();
  #endif
}


#endif
