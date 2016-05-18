// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_ACTION_SET_TRAVERSAL_LOOP_H_
#define _PEANO_DATA_TRAVERSAL_ACTION_SET_TRAVERSAL_LOOP_H_


#include "peano/datatraversal/ActionSetTraversal.h"


#ifdef SharedTBB
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#endif

#ifdef SharedCobra
#include <cobra/scheduler.hpp>
#include <cobra/range.hpp>
#include "tarch/multicore/cobra/Core.h"
#endif

namespace peano {
    namespace datatraversal {
      template <class LoopBody>
      class ActionSetTraversalLoop;
    }
}


/**
 * Action Set Traversal Loop
 *
 * The realisation of this class follows dForLoop. So please see this class for
 * implementation details.
 *
 * @author Tobias Weinzierl
 */
template <class LoopBody>
class peano::datatraversal::ActionSetTraversalLoop {
  private:
    /**
     * Logging device
     */
    static tarch::logging::Log _log;

    void loopParallel(
      const peano::datatraversal::ActionSetTraversal&  traversal,
      const LoopBody&                                  loopBody,
      int                                              grainSize
    );
    void loopSequential(
      const peano::datatraversal::ActionSetTraversal&  traversal,
      LoopBody&                                        loopBody
    );

    #ifdef SharedTBB
    class ActionSetTraversalLoopInstance {
      private:
        LoopBody                                _loopBody;
        const peano::datatraversal::ActionSet&  _actionSet;
      public:
        ActionSetTraversalLoopInstance(
          const LoopBody&                         loopBody,
          const peano::datatraversal::ActionSet&  actionSet
        );

        ActionSetTraversalLoopInstance(
          const ActionSetTraversalLoopInstance&  copy,
          tbb::split
        );

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
         *
         * Unfortunately, the operator() has to be const according to the
         */
        void operator() (const tbb::blocked_range<int>& range);

        /**
         * nop
         */
        void join(const ActionSetTraversalLoopInstance&  with);
    };
    #endif


    #ifdef SharedCobra
    /**
     * For Cobra only
     */
    void realiseParallelForAsTaskBipartitioning(
      ::cobra::blocked_range<int>                     range,
      ::cobra::continuator&                           ctr,
      const peano::datatraversal::ActionSet&  actionSet,
      const LoopBody&                                 loopBody
    );
    #endif

  public:
    /**
     * Constructor.
     *
     * The loop bodies may not be const references, as they might hold their
     * own data such as enumerators. See comment.
     *
     * @param grainSize Grain size for the problem. If the grain size equals
     *                  zero, no parallelisation is taken into account. If the
     *                  grain size is bigger than the problem size, also no
     *                  parallelisation is taken into account. However, these
     *                  two cases are formally different aspects.
     */
    ActionSetTraversalLoop(
      peano::datatraversal::ActionSetTraversal traversal,
      LoopBody&                                        loopBody,
      int                                              grainSize
    );
};



#include "peano/datatraversal/ActionSetTraversalLoop.cpph"

#endif
