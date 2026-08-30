// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_MULTICORE_LOOP_H_
#define _TARCH_MULTICORE_LOOP_H_


#include "tarch/multicore/dForRange.h"
#include "tarch/la/Vector.h"
#include "tarch/multicore/MulticoreDefinitions.h"

#include <functional>






namespace tarch {
  namespace multicore {
    /**
     * Loop over range but ensure that any copy made is merged again
     * into input class. Therefore, the input has to have a functor
     * which accepts an integer vector, and it has to have an operation
     *
     * mergeIntoMasterThread().
     *
     * <h2> Serial case </h2>
     *
     * If you compile without TBB or OpenMP, then the parallel reduce becomes
     * parallel for.
     *
     * <h2> Use it with lambda calculus </h2>
     *
     * In some cases, I did struggle to get the code compile with lambda
     * calculus, where I wanted to plug in a lambda functor directly. In this
     * case, the following explicit functor creation did help:
     *
     * <pre>
    auto func = [&loopBody,traversal,currentStepInPath] (const tarch::la::Vector<1,int>& i) -> void {
        loopBody(traversal.getActionSet(currentStepInPath).getAction(i(0)));
      };

    tarch::multicore::parallelFor(
      tarch::multicore::dForRange<1>( 0, traversal.getActionSet(currentStepInPath).getNumberOfParallelActions(), grainSize, 1 ),
	  func
    );
       </pre>
     *
     *  while the direct variant
     *
     * <pre>
    tarch::multicore::parallelFor(
      tarch::multicore::dForRange<1>( 0, traversal.getActionSet(currentStepInPath).getNumberOfParallelActions(), grainSize, 1 ),
	  [&loopBody,traversal,currentStepInPath] (const tarch::la::Vector<1,int>& i) -> void {
        loopBody(traversal.getActionSet(currentStepInPath).getAction(i(0)));
      }
    );
       </pre>
     *
     * did not compile.
     */
    template <typename F>
    void parallelReduce(
      const tarch::multicore::dForRange<1>&  range,
      F&                                     function
    );

    template <typename F>
    void parallelReduce(
      const tarch::multicore::dForRange<2>&  range,
      F&                                     function
    );

    template <typename F>
    void parallelReduce(
      const tarch::multicore::dForRange<3>&  range,
      F&                                     function
    );

    template <typename F>
    void parallelReduce(
      const tarch::multicore::dForRange<4>&  range,
      F&                                     function
    );

    template <typename F>
    void parallelFor(
      const tarch::multicore::dForRange<1>&  range,
      F&                                     function
    );

    template <typename F>
    void parallelFor(
      const tarch::multicore::dForRange<2>&  range,
      F&                                     function
    );

    template <typename F>
    void parallelFor(
      const tarch::multicore::dForRange<3>&  range,
      F&                                     function
    );

    template <typename F>
    void parallelFor(
      const tarch::multicore::dForRange<4>&  range,
      F&                                     function
    );
  }
}



#if defined(SharedTBB)
#include "tarch/multicore/tbb/Loop.h"
#elif SharedOMP
#include "tarch/multicore/omp/Loop.h"
#elif SharedCPP
#include "tarch/multicore/cpp/Loop.h"
#else
#include "tarch/multicore/Loop.cpph"
#endif


#endif
