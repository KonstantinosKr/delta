#if defined(SharedOMP)

#include "tarch/multicore/omp/BooleanSemaphore.h"
#include "tarch/multicore/BackgroundTasks.h"

#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/multicore/omp/Core.h"

tarch::multicore::BooleanSemaphore::BooleanSemaphore() {
  omp_init_lock(&_lock);
}


tarch::multicore::BooleanSemaphore::~BooleanSemaphore() {
  #ifdef CompilerICC
  // Fix for Intel Compiler due to a bug in the OpenMP support of the icc (see http://software.intel.com/en-us/forums/showthread.php?t=72204)
  if(tarch::multicore::Core::getInstance().isInitialised()) {
    omp_destroy_lock(&_lock);
  }
  #else
  omp_destroy_lock(&_lock);
  #endif
}


void tarch::multicore::BooleanSemaphore::enterCriticalSection() {
  omp_set_lock(&_lock);
}


void tarch::multicore::BooleanSemaphore::leaveCriticalSection() {
  omp_unset_lock(&_lock);
}



void tarch::multicore::BooleanSemaphore::sendTaskToBack() {
  processBackgroundTasks();
}

#endif
