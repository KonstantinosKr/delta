// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#if !defined( _TARCH_MULTICORE_CPP_BOOLEAN_SEMAPHORE_H_ ) && defined(SharedCPP)
#define _TARCH_MULTICORE_CPP_BOOLEAN_SEMAPHORE_H_


#include <string>
#include <mutex>
#include <atomic>


#include "tarch/compiler/CompilerSpecificSettings.h"


namespace tarch {
  namespace multicore {
    class BooleanSemaphore;
    class Lock;
  }
}



class tarch::multicore::BooleanSemaphore {
  private:
    #ifdef BooleanSemaphoreUsesASpinLock
    std::atomic_flag    _spinLock;
    #else
	std::mutex   _mutex;
    #endif

    friend class tarch::multicore::Lock;
    friend class RecursiveSemaphore;

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
    BooleanSemaphore( const BooleanSemaphore& semaphore ) {}

    /**
     * You may not copy a semaphore
     */
    BooleanSemaphore& operator=( const BooleanSemaphore& semaphore ) {return *this;}
  public:
    BooleanSemaphore();
    ~BooleanSemaphore();
};
#endif


