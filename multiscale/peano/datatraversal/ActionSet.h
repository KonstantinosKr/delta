// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_ACTION_SET_H_
#define _PEANO_DATA_TRAVERSAL_ACTION_SET_H_


#include "peano/datatraversal/Action.h"
#include <vector>


namespace peano {
    namespace datatraversal {
      class ActionSet;
    }
}


/**
 * Action Set
 *
 * Represents a set of actions, i.e. activities on cells. There's no order on a
 * set, i.e. you may execute all the elements of one set in parallel. Before you
 * run through the actions, you however have to ensure that all the data up to a
 * certain level is loaded (see getCurrentMaximumLevel()).
 *
 * All actions contained within an action set are either
 *
 * @author Svetlana Nogina, Tobias Weinzierl
 */
class peano::datatraversal::ActionSet {
  private:
    std::vector< Action >  _actionSet;
  public:
    /**
     * Construct an action set
     */
    ActionSet();

    ~ActionSet();

    void addAction( const Action& action );

    int getNumberOfParallelActions() const;

    const Action& getAction( int i ) const;
};

#endif
