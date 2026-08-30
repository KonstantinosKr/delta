// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_CELL_FLAGS_H_
#define _PEANO_GRID_CELL_FLAGS_H_


#include <string>


namespace peano {
    namespace grid {
      /**
       * Subtree identifier
       *
       * Characterises for a cell's vertex enumerator or a vertex its
       * underlying tree or the 2^d adjacent trees:
       *
       * - If the flag equals Leaf, the vertex enumerator corresponds to a leaf or
       *   all adjacent cells of a vertex are leaves, respectively.
       * - If the flag is positive, its specifies the size of the tree induced
       *   by the current vertex enumerator, i.e. the vertex enumerator
       *   describes size and offset of a regular tree's root element. If the
       *   flag of a vertex is greater Leaf, all 2^d adjacent trees are regular
       *   and have the same height.
       * - If the flag is StationaryButIrregular, the induced tree or the 2^d
       *   adjacent trees, respectively, are stationary yet are not regular, i.e.
       *   define adaptive grids.
       * - The remaining flags are self-explaining.
       */

      // @todo use new class construct
      enum CellFlags {
        Undefined                       = 65536,
        Leaf                            = 0,
        StationaryButIrregular          = -1,
        NotStationary                   = -2,
        NotStationaryDueToInvalidation  = -3
      };

      peano::grid::CellFlags min( const peano::grid::CellFlags& lhs, const peano::grid::CellFlags& rhs);

      void inc( peano::grid::CellFlags& value);

      /**
       * Creates a legend of all the cell flag states. Used by the
       * visualisation, e.g., to plot legends.
       */
      std::string getCellFlagsLegend();
  }
}


/**
 * Gives you a readable representation of the cell flags.
 */
std::string toString( const peano::grid::CellFlags& cellFlags );

#endif
