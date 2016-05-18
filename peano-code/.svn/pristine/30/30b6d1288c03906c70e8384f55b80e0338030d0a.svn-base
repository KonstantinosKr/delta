// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_CONSTANTS_H_
#define _PEANO_GRID_NODES_CONSTANTS_H_


#include "peano/utils/Globals.h"
#include <string>


namespace peano {
    namespace grid {
      namespace nodes {
        /**
         * If a counter is set CounterPersistentNode it represents a standard node.
         * Such a vertex is loaded from the stacks at the beginning and finally written
         * to the stacks again.
         */
        static const int CounterPersistentNode = 0;

        /**
         * If a counter is set CounterHangingNode it represents a hanging node. See
         * loadVerticesIntoArray() for a detailed description.
         */
        static const int CounterHangingNode = TWO_POWER_D;


        static const int CounterNewNode = 2*TWO_POWER_D;


        static const int CounterPersistentNodeDelete = 3*TWO_POWER_D;

        /**
         * Only needed by the root cell which handles vertices that are not
         * stored on the stacks. If we set a counter to this value, it is not
         * taken into account by LoadVertexLoopBody() at all.
         */
        static const int CounterNodeWithoutLifecycle = 4*TWO_POWER_D;

        static const int CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing = 5*TWO_POWER_D;

        std::string counterToString( int counter );
      }
  }
}

#endif
