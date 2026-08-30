#include "tarch/logging/Log.h"
#include "peano/utils/Loop.h"

#include "peano/peano.h"

#include "peano/grid/aspects/CellLocalPeanoCurve.h"
#include "tarch/multicore/MulticoreDefinitions.h"


#ifdef SharedMemoryParallelisation
#include "tarch/multicore/Core.h"
#endif


#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"


void peano::fillLookupTables() {
  setupLookupTableForTwoPowI();
  setupLookupTableForThreePowI();
  setupLookupTableForFourPowI();
  setupLookupTableForDPowI();

  peano::utils::setupLookupTableForDLinearised();
  peano::utils::setupLookupTableForDDelinearised();
}


int peano::initParallelEnvironment(int* argc, char*** argv) {
  #ifdef Parallel
  if ( tarch::parallel::Node::getInstance().init(argc,argv) ) {
    tarch::parallel::NodePool::getInstance().init();
    return 0;
  }
  else {
    return -2;
  }
  #else
  return 0;
  #endif
}


void peano::shutdownParallelEnvironment() {
  tarch::parallel::NodePool::getInstance().shutdown();
  tarch::parallel::Node::getInstance().shutdown();
}


int peano::initSharedMemoryEnvironment() {
  #ifdef SharedMemoryParallelisation
  if ( tarch::multicore::Core::getInstance().isInitialised() ) {
    return 0;
  }
  else {
    return -3;
  }
  #else
  return 0;
  #endif
}


void peano::shutdownSharedMemoryEnvironment() {
  #ifdef SharedMemoryParallelisation
  tarch::multicore::Core::getInstance().shutDown();
  #endif
}


void peano::releaseCachedData() {
  peano::grid::aspects::CellLocalPeanoCurve::releaseCachedData();
}
