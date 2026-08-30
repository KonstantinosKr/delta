#include "tarch/multicore/RecursiveSemaphore.h"


#ifdef SharedCPP

tarch::multicore::RecursiveSemaphore::RecursiveSemaphore():
  _recursiveMutex() {
}


tarch::multicore::RecursiveSemaphore::~RecursiveSemaphore() {
}


void tarch::multicore::RecursiveSemaphore::enterCriticalSection() {
  _recursiveMutex.lock();
}


void tarch::multicore::RecursiveSemaphore::leaveCriticalSection() {
  _recursiveMutex.unlock();
}

#endif
