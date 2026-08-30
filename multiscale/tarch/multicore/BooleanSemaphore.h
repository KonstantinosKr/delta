// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_MULTICORE_BOOLEAN_SEMAPHORE_H_
#define _TARCH_MULTICORE_BOOLEAN_SEMAPHORE_H_

#include <string>

#include "tarch/multicore/MulticoreDefinitions.h"
#include "tarch/multicore/omp/BooleanSemaphore.h"
#include "tarch/multicore/tbb/BooleanSemaphore.h"
#include "tarch/multicore/cpp/BooleanSemaphore.h"


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
 * <h2> Usage </h2>
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
 * <h2> Rationale </h2>
 *
 * - A pure critical section does not work for many applications: A plotter for
 *   example writes both vertices and cells to an output file. The write
 *   process has to be protected. A critical section within the vertex plotter
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
 *
 * <h2> Bugs due to the locks </h2>
 *
 * Inserting Locks and relying on the lock destructor turns out to be
 * problematic in two cases:
 *
 * - If the Lock is used within a routine that contains a return, it seems
 *   that some compilers invoke the destructor of Lock before the return
 *   statement (regardless whether it is called or not). This means variable
 *   accesses following a branch with a return are not protected anymore.
 *
 * - If the Lock is used within a template, it seems that some compilers
 *   simply embed the code block into the calling function. The scoping
 *   rules are not preserved.
 *
 * If you run into such cases, we recommend that you call the lock's free()
 * explicitly. In our codes, this did resolve all the issues.
 *
 *
 * <h2> Semaphores protecting more than one variable</h2>
 *
 * If a semaphore protects more than a single variable, memory reordering, caching
 * and non-barriering can induce data inconsistencies: Assume one thread writes
 * three variables A,B,C and then sets a fourth variable D to true. The last
 * operation if protected by a lock. A second thread waits busily for variable D
 * to become true and then reads A, B and C. Though D might be set, it can happen
 * that A, B and C hold invalid data as the writes between the threads have been
 * reordered. To avoid this, you have to add
 *
  std::atomic_thread_fence(std::memory_order_release);
 *
 * just before you set the flag in the first thread, and you have to invoke
 *
    std::atomic_thread_fence(std::memory_order_acquire);
 *
 * just before you have received the go in the second thread. Obviously, this
 * problem arises only for producer-consumer patterns where the reading thread
 * has already an instance of a variable but reads multiple variables from the
 * other guy.
 *
 * Remark: Statements alike these guys are typically not required for correct code!
 *
 * @author Tobias Weinzierl
 */
#if !defined(SharedMemoryParallelisation)
class tarch::multicore::BooleanSemaphore {
  private:
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


#endif
