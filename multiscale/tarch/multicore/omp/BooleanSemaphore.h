// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#if !defined( _TARCH_MULTICORE_BOOLEAN_SEMAPHORE_OPENMP_H_) && defined(SharedOMP)
#define _TARCH_MULTICORE_BOOLEAN_SEMAPHORE_OPENMP_H_


#include <string>


namespace tarch {
  namespace multicore {
    class BooleanSemaphore;
    class Lock;
  }
}


#if defined(SharedOMP)
#include <omp.h>

class tarch::multicore::BooleanSemaphore {
  private:
    friend class tarch::multicore::Lock;

    omp_lock_t   _lock;

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


#endif
