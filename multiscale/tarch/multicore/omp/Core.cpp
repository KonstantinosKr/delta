#if defined(SharedOMP)

#include "tarch/multicore/omp/Core.h"
#include "tarch/Assertions.h"

#include <iostream>

#ifdef SharedOMP
#include <omp.h>
#endif

tarch::multicore::Core::Core():
  _numberOfThreads(omp_get_num_procs()) {
  omp_set_num_threads(_numberOfThreads);
}


tarch::multicore::Core::~Core() {

}


tarch::multicore::Core& tarch::multicore::Core::getInstance() {
  static tarch::multicore::Core singleton;
  return singleton;
}


void tarch::multicore::Core::shutDown() {
  _numberOfThreads = -1;
}


void tarch::multicore::Core::configure( int numberOfThreads ) {
  if (numberOfThreads==0) {
    _numberOfThreads = omp_get_num_procs();
  }
  else {
    _numberOfThreads = numberOfThreads;
    omp_set_num_threads(_numberOfThreads);
  }
}


int tarch::multicore::Core::getNumberOfThreads() const {
  return _numberOfThreads;
}

bool tarch::multicore::Core::isInitialised() const {
  return _numberOfThreads > 0;
}

#endif
