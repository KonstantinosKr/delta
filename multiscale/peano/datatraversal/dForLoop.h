// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_DFOR_LOOP_H_
#define _PEANO_DATA_TRAVERSAL_DFOR_LOOP_H_


#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"


#include "tarch/multicore/BooleanSemaphore.h"

#include "peano/utils/Globals.h"

#include "peano/datatraversal/tests/dForLoopTest.h"

#include <vector>


namespace peano {
  namespace datatraversal {
    template <class LoopBody>
    class dForLoop;
  }
}



/**
 * Simple d-dimensional For-Loop
 *
 * This class is a simple wrapper for for-loops that has one significant
 * advantage: It is parallelised if TBBs or OpenMP are included. The class
 * needs, besides the range (iteration space), an event handle (loop body). It
 * has to implement an operator() (const tarch::la::Vector<DIMENSIONS,int>& i)
 * at least (see remarks below).
 *
 * This means you write a simple class with a constructor and this single
 * operation. Then, you move the body of your original for-loop to the
 * implementation of the operator() and replace the original for loop by
 * something similar to
 * \code
  typedef loops::GridStoreVertexLoopBody LoopBody;
  LoopBody loopBody(*this,other arguments you need within the loop body);
  peano::datatraversal::dForLoop<LoopBody>(int-vector with loop range,loopBody);
\endcode
 *
 * The range implementation will copy the loop body for each thread, i.e. you
 * have to ensure that there is an appropriate copy constructor for your loop
 * body. The loop body class itself may have a state and also may modify this
 * state from call to call. However, this state ain't persistent, i.e. after
 * the very last iteration (and you actually don't know how many iterations
 * there will be due to load stealing), the complete object is thrown away.
 *
 * If you want to merge a thread's local copies into a global variable, you
 * do this in the operation mergeWithWorkerThread(). The grid's loops for
 * example all have a merge operation (TBB calls this merge join) that first
 * locks a global semaphore, and then it calls a merge
 * operation on the mappings.
 *
 * Merging the whole loop body back however is not a cheap process. If it is
 * not required (this holds notably if your loop already does restrict
 * on-the-fly, i.e. it sets some global values protected by a semaphore), then
 * you can run the parallel loop without a reduction. mergeWithMaster then is
 * omitted.
 *
 * We finally emphasise that thread merges in this code are done in two steps.
 * The loop class creates copies of the loop body and merges those guys back
 * via mergeWithWorkerThread(). At the very end, i.e. after the very last
 * thread has terminated, it then merges back this thread into the prototype
 * (original loop body handed over) via mergeIntoMasterThread(). This allows
 * you to do different things, notably if several loop types run in parallel
 * in your code.
 *
 *
 * <h2> Parallel Implementation (TBB) </h2>
 *
 * The TBB implementation is straightforward. While I could deploy the range
 * management to the loop bodys, i.e. the loop bodys' operator() runs over a
 * whole range, this would make the implementation more difficult. Instead, I
 * decided to require the loop body only to provide an operator() operating on
 * one single element.
 *
 * However, forcing TBB to create a task per loop element in turn induces a big
 * overhead. Thus, I introduced an intermediate layer: The embedded class
 * dForLoopInstance implements the loop over a (sub)-range, i.e. the dForLoop
 * itself just creates an instance of dForLoopInstance and invokes a parallel
 * for for this object. The object accepts a Peano range, traverses this range,
 * and invokes the loop body's operator() for each range element. It is
 * important that dForLoopInstance creates a thread-local copy of the loop body
 * - Peano allows loop bodys to have a (thread-wise) state. Thus, we have to
 * ensure that we are working on a copy and that not all threads are working on
 * the same loop body instance.
 *
 * One may not use TBB's parallel_for as parallel for requires the loop bodies
 * to be const. Instead, we have to use the parallel_reduce even though we
 * merge the copies ourself in the destructors.
 *
 *
 * <h2> Serial runs </h2>
 *
 * If the code is running serial, the loop body is not copied at all. If the
 * code runs in parallel, each thread creates a copy of the loop body. In
 * particular, also the master thread first creates a copy and then loops,
 * i.e. we also create a copy even if we work only with one thread.
 *
 * <h2> Runs without reduction </h2>
 *
 * You can run the whole code without a reduction. For this, you may omit the
 * merge operations, but the important thing is that you make operator() const.
 *
 * @author Tobias Weinzierl
 */
template <class LoopBody>
class peano::datatraversal::dForLoop {
  private:
    friend class peano::datatraversal::tests::dForLoopTest;

    static tarch::logging::Log _log;

    void runSequentially(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      LoopBody&                                 loopBody
    );

    void runParallelWithoutColouring(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      LoopBody&                                 loopBody,
      int                                       grainSize,
      bool                                      altersState
    );

    void runParallelWithColouring(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      LoopBody&                                 loopBody,
      int                                       grainSize,
      int                                       colouring,
      bool                                      altersState
    );
  public:
    enum ParallelisationStrategy {
      Serial               = 0,
      NoColouring          = 1,
      TwoPowerDColouring   = 2,
      SixPowerDColouring   = 6,
      SevenPowerDColouring = 7
    };

    /**
     * Constructor
     *
     * @param grainSize  Grain size of problem. See dForLoop for a
     *                   documentation. If the grain size equals zero, the
     *                   multithreading is switched off. Another term for grain
     *                   size is chunk size (OpenMP prefers this term).
     */
    inline dForLoop(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      LoopBody&                                 body,
      int                                       grainSize,
      int                                       colouring,
      bool                                      altersState
    );
};

#include "peano/datatraversal/dForLoop.cpph"

#endif
