#if defined(SharedTBB)
#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/tbb/Core.h"
#include "tarch/multicore/Jobs.h"
#include "tarch/multicore/tbb/Jobs.h"
#include "tarch/Assertions.h"
#include <thread>


tarch::logging::Log  tarch::multicore::Core::_log( "tarch::multicore::Core" );


tarch::multicore::Core::Core():
  _numberOfThreads(std::thread::hardware_concurrency()),
  _globalThreadCountControl(nullptr),
  _globalStackSizeControl(nullptr),
  _pinningObserver(1) {
}


tarch::multicore::Core::~Core() {
  shutDown();
}


void tarch::multicore::Core::pinThreads(bool value) {
  _pinningObserver.observe(value);
}


tarch::multicore::Core& tarch::multicore::Core::getInstance() {
  static tarch::multicore::Core singleton;
  return singleton;
}


void tarch::multicore::Core::shutDown() {
  jobs::terminateAllPendingBackgroundConsumerJobs();

  if (_globalThreadCountControl!=nullptr) {
    delete _globalThreadCountControl;
    _globalThreadCountControl = nullptr;
  }
  if (_globalStackSizeControl!=nullptr) {
    delete _globalStackSizeControl;
    _globalStackSizeControl = nullptr;
  }
  _numberOfThreads = -1;
}


void tarch::multicore::Core::configure( int numberOfThreads, int stackSize ) {
  logInfo( "configure(int)", "manually set number of threads to " << numberOfThreads );

  if (_globalThreadCountControl!=nullptr) {
    delete _globalThreadCountControl;
    _globalThreadCountControl = nullptr;
  }
  if (_globalStackSizeControl!=nullptr) {
    delete _globalStackSizeControl;
    _globalStackSizeControl = nullptr;
  }

  if (numberOfThreads==UseDefaultNumberOfThreads) {
    _numberOfThreads = std::thread::hardware_concurrency();
  }
  else {
    _numberOfThreads = numberOfThreads;
  }
  _globalThreadCountControl = new tbb::global_control(tbb::global_control::max_allowed_parallelism,_numberOfThreads);

  if (stackSize>UseDefaultStackSize) {
    _globalStackSizeControl = new tbb::global_control(tbb::global_control::thread_stack_size,stackSize);
  }
}


int tarch::multicore::Core::getNumberOfThreads() const {
  assertion( isInitialised() );
  return _numberOfThreads;
}


bool tarch::multicore::Core::isInitialised() const {
  //return _globalThreadCountControl!=nullptr;
  return true;
}

#endif
