#include "MulticoreDefinitions.h"

#ifndef SharedMemoryParallelisation

#include "tarch/multicore/Core.h"

const int tarch::multicore::Core::UseDefaultNumberOfThreads = 0;


tarch::multicore::Core::Core() {
}


tarch::multicore::Core::~Core() {
}


tarch::multicore::Core& tarch::multicore::Core::getInstance() {
  static Core instance;
  return instance;
}

void tarch::multicore::Core::configure( int numberOfThreads ) {
}


void tarch::multicore::Core::shutDown() {
}


bool tarch::multicore::Core::isInitialised() const {
  return true;
}


int tarch::multicore::Core::getNumberOfThreads() const {
  return 1;
}

#endif
