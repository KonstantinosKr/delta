// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_DATA_TRAVERSAL_ACTION_H_
#define _PEANO_DATA_TRAVERSAL_ACTION_H_


#include "tarch/la/Vector.h"
#include "peano/utils/Globals.h"


namespace peano {
    namespace datatraversal {
      struct Action;
    }
}


/**
 * Action of Shared Memory Parallelisation
 *
 * Represents one action on a cell.
 */
struct peano::datatraversal::Action {
    /**
     * Often the cell index
     */
    tarch::la::Vector<DIMENSIONS,int> _cartesianPosition;

    /**
     * Often the level or the stack number to be used.
     */
    int                               _id;

    /**
     *
     */
    Action(
      tarch::la::Vector<DIMENSIONS,int> cartesianPosition,
      int                               id
    );
    virtual ~Action();
};

#endif
