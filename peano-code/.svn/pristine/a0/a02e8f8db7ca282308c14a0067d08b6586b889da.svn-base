// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_DFOR_LOOP_H_
#define _PEANO_DATA_TRAVERSAL_DFOR_LOOP_H_


#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"


#include "peano/datatraversal/dForRange.h"
#include "peano/datatraversal/tests/dForLoopTest.h"
#include "tarch/multicore/BooleanSemaphore.h"


#ifdef SharedCobra
#include "tarch/multicore/cobra/Core.h"
#include <cobra/thread.hpp>
#endif


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
 * needs, besides the range (iteration space), an event handle (loop body) with
 * implements $void operator() (const tarch::la::Vector<DIMENSIONS,int>& i)$.
 * This means you write a simple class with a constructor and this one
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
 * If you want to merge a thread's local copies into a global variable, you can
 * do this in the destructor. The grid's loops for example all have a
 * destructor that first locks a global semaphore, and then it calls a merge
 * operation on the mappings.
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
 * <h2> Copy Policies </h2>
 *
 * If the code is running serial, the loop body is not copied at all. If the
 * code runs in parallel, each thread creates a copy of the loop body. In
 * particular, also the master thread first creates a copy and then loops,
 * i.e. we also create a copy even if we work only with one thread.
 *
 * @author Tobias Weinzierl
 */
template <class LoopBody>
class peano::datatraversal::dForLoop {
  private:
    friend class peano::datatraversal::tests::dForLoopTest;

    static tarch::logging::Log _log;

    /**
     * Required only for OpenMP (not used currently)
     */
    static std::vector<dForRange> createRangesVector(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      int                                       grainSize
    );

    class dForLoopInstance {
      private:
        LoopBody                           _loopBody;

        tarch::la::Vector<DIMENSIONS,int>  _offset;
        const int                          _padding;

      public:
        dForLoopInstance(
          const LoopBody&                    loopBody,
          tarch::la::Vector<DIMENSIONS,int>  offset,
          const int                          padding
        );

        void setOffset(const tarch::la::Vector<DIMENSIONS,int>&  offset);

        /**
         * See documentation of TBB. This flag is just used to
         * distinguish the split constructor from a standard
         * copy constructor.
         */
        #if defined(SharedTBB)
        typedef tbb::split   SplitFlag;
        #else
        typedef int          SplitFlag;
        #endif

        /**
         * Copy Constructor
         *
         * TBB requires the copy constructor for loops to accept an additional
         * argument to be able to distinguish it from the standard copy
         * constructor. As a consequence, the code does not compile anymore
         * without tbb. I thus wrap the TBB type with a typedef of my own.
         */
        dForLoopInstance( const dForLoopInstance& loopBody, SplitFlag );

        /**
         * Process range
         *
         * Could, at first glance, be const as the method copies the loop body anyway. The
         * operation first copies the loop body. This process can run
         * in parallel, as the copy process may not modify the original
         * loop body instance. When the operation has terminated, it calls the
         * loop body copy's updateGlobalValues(). Usually, the copy holds a
         * reference to the original data. A reference not used until this
         * final operation is called. The final operation then commits changes
         * to the original data set. This operation hence is not const.
         * Consequently, the whole operator may not be const.
         */
        void operator() (const dForRange& range);

        /**
         * Empty Join
         *
         * Instead, we use the destructor. See remarks on TBB realisation for
         * encapsulating class.
         */
        void join(const dForLoopInstance&  with);
    };

    void runSequentially(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      LoopBody&                                 loopBody
    );

    void runParallelWithoutColouring(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      LoopBody&                                 loopBody,
      int                                       grainSize
    );

    void runParallelWithColouring(
      const tarch::la::Vector<DIMENSIONS,int>&  range,
      LoopBody&                                 loopBody,
      int                                       grainSize,
      int                                       colouring
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
      int                                       colouring
    );
};

#include "peano/datatraversal/dForLoop.cpph"

#endif
