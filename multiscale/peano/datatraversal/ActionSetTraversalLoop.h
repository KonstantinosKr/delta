// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_ACTION_SET_TRAVERSAL_LOOP_H_
#define _PEANO_DATA_TRAVERSAL_ACTION_SET_TRAVERSAL_LOOP_H_


#include "peano/datatraversal/ActionSetTraversal.h"



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
      LoopBody&                                        loopBody,
      int                                              grainSize
    );


    void loopSequential(
      const peano::datatraversal::ActionSetTraversal&  traversal,
      LoopBody&                                        loopBody
    );

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
