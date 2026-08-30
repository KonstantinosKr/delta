// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#include "tarch/multicore/MulticoreDefinitions.h"

#include "tarch/multicore/cpp/RecursiveSemaphore.h"
#include "tarch/multicore/tbb/RecursiveSemaphore.h"


#if !defined(_TARCH_MULTICORE_RECURSIVE_SEMAPHORE_H_) && !defined(SharedMemoryParallelisation)
#define _TARCH_MULTICORE_RECURSIVE_SEMAPHORE_H_

#include <string>
#include <thread>

#include "tarch/multicore/BooleanSemaphore.h"
#include "tarch/logging/Log.h"


namespace tarch {
  namespace multicore {
    class RecursiveSemaphore;
    class RecursiveLock;
  }
}



/**
 * Recursive Semaphore
 *
 * A recursive semaphore is a boolean semphore that one thread (the first one)
 * can lock an arbitrary number of times.
 *
 *
 * @author Tobias Weinzierl
 */
class tarch::multicore::RecursiveSemaphore {
  private:
    friend class tarch::multicore::RecursiveLock;

    /**
     * Waits until I can enter the critical section.
     */
    void enterCriticalSection();

    /**
     * Tells the semaphore that it is about to leave.
     */
    void leaveCriticalSection();

    /**
     * You may not copy a semaphore
     */
    RecursiveSemaphore( const RecursiveSemaphore& semaphore ) {}

    /**
     * You may not copy a semaphore
     */
    RecursiveSemaphore& operator=( const RecursiveSemaphore& semaphore ) {return *this;}
  public:
    RecursiveSemaphore();
    ~RecursiveSemaphore();
};


#endif
