// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_ACTION_SET_TRAVERSAL_H_
#define _PEANO_DATA_TRAVERSAL_ACTION_SET_TRAVERSAL_H_


#include "peano/datatraversal/ActionSet.h"
#include "peano/utils/PeanoOptimisations.h"


namespace peano {
    namespace datatraversal {
      class ActionSetTraversal;
    }
}



/**
 * Cell Traversal
 *
 * Represents a complete traversal of a cell data structure. It consists of a
 * sequence of
 *
 * @author Svetlana Nogina, Tobias Weinzierl, Marco Seravalli
 */
class peano::datatraversal::ActionSetTraversal {
  private:
    std::vector< ActionSet >  _actionSets;
  public:
    /**
     * Standard constructor.
     *
     * Please use static methods to create a cell traversal.
     */
    ActionSetTraversal(int numberOfActionSets);

    /**
     * Destructor
     */
    ~ActionSetTraversal();

    /**
     * This operation takes a action set traversal representing a total order,
     * i.e. each action set holds one and only one action. It then assumes that
     * only actions working with the same id cannot run in parallel, and
     * reorders all actions according to this assumption. If there's an action
     * a and an action b with a being before b in input, a will also be before
     * b in the result. Otherwise, they might run in parallel.
     */
    static ActionSetTraversal sortIntoIdBins(const ActionSetTraversal& input);

    /**
     * Return the maximum path.
     *
     * This maximum path also is the number of action sets
     */
    int getMaximumPath() const;

    /**
     * The maximum grain size is the fatest node of the overall sequence, i.e.
     * it identifies the best concurrency level possible.
     */
    int getMaximumGrainSize() const;

    const ActionSet& getActionSet( int i ) const;

    ActionSet& getActionSet( int i );

    std::string toString() const;
};



#endif
