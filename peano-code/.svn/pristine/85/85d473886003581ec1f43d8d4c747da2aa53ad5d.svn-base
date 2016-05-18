// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_MULTICORE_BOOLEAN_SEMAPHORE_H_
#define _TARCH_MULTICORE_BOOLEAN_SEMAPHORE_H_

#include <string>

#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/omp/BooleanSemaphore.h"
#include "tarch/multicore/tbb/BooleanSemaphore.h"


namespace tarch {
  namespace multicore {
    class BooleanSemaphore;
    class Lock;
  }
}


/**
 * Boolean Semaphore
 *
 * A boolean semaphore is an atomic variable that protects a section/scope. If
 * k scopes are guarded by a boolean semaphore, any thread can enter this
 * scope. As soon as it enters the scope, the boolean semaphore switches and
 * all other threads that want to enter a section that is protected by the same
 * semaphore have to wait.
 *
 * !!! Usage
 *
 * - Add your class an attribute of BooleanSemaphore.
 * - For each operation with a critical section:
 *   - Create within the operation an instance of Lock and pass it the
 *     semaphore instance.
 *   - Free the lock (optional). If you don't free it, it will be freed as soon
 *     as the  scope is left and the lock object is destroyed.
 *
 * \code
   class MyClass {
     ...

     BooleanSemaphore _mySemaphore;
   };



   MyClass::foo() {
     ...
     Lock myLock( _mySemphore );
     // critical Section A
     ...
   }


   MyClass::bar() {
     ...
     Lock myLock( _mySemphore );
     // critical Section B
     ...
   }

 \endcode
 *
 * In the code above, the critical sections A and B are never executed at the
 * same time.
 *
 * !!! Rationale
 *
 * - A pure critical section does not work for many applications: A plotter for
 *   example writes both vertices and cells to an output file. The write
 *   process has to be protected. A criticial section within the vertex plotter
 *   scope does not prevent the cells to be written to the file simultaneously.
 *   This has to be forbidden by a plotter semaphore.
 * - Intel TBB's have a built-in semaphore concept called scoped lock.
 * - OpenMP has named critical sections that might work.
 * - I added this class to have a unified usage model (even without any multicore
 *   parallelisation switched on) for everybody.
 * - You cannot lock a semaphore manually as the corresponding operations are
 *   hidden. Instead, you have to create a lock object that does the lock for
 *   you and frees the semaphore as soon as the lock object is destroyed. The
 *   reason for this process is simple: This way, you cannot forget to free a
 *   semaphore (e.g. due to a return right in the middle of a critical
 *   section).
 * - The lock is a class that can be implemented without any OpenMP- or
 *   TBB-specific stuff.
 * - The specification of the semaphore is independent of the parallelisation
 *   model chosen.
 * - The subdirectories of this directory hold the TBB- and OpenMP-specific
 *   implementations of the semaphore.
 *
 * @author Tobias Weinzierl
 */
#if !defined(SharedMemoryParallelisation)
class tarch::multicore::BooleanSemaphore {
  private:
    friend class tarch::multicore::Lock;

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

    /**
     *
     * Macro for pipelining
     *
     * You should send your task to the background if no input data are available
     * to continue. This might e.g. materialise in a semaphore not released yet.
     * Please invoke continuedWithTask() as soon as your waiting code fragment
     * continues. The operation does not termine/stop the task. It might only stop
     * it for a brief time and then continue.
     *
     * A typical pattern how to use the macro reads as follows:
     * \code

    bool hasFinished = false;

    while (!hasFinished) {
      {
        tarch::multicore::Lock lock(mySemaphore);
        hasFinished = get data from some place protected by mySemaphore;
      }
      tarch::multicore::BooleanSemaphore::sendTaskToBack();
    }

    tarch::multicore::BooleanSemaphore::continuedWithTask();

 \endcode
    *
    * Please be aware of the inner brackets. We thus explicitly invoke the
    * lock's destructor. Alternatively, we could unlock manually.
    */
    static void sendTaskToBack();

    /**
     * Each sendCurrentTaskToBack() should be followed by a continueWithTask().
     */
    static void continuedWithTask();
};
#endif


#endif
