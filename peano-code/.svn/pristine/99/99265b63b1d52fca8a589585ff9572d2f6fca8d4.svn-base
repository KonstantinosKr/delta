// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_TRAVERSAL_ORDER_ON_TOP_LEVEL_H_
#define _PEANO_GRID_TRAVERSAL_ORDER_ON_TOP_LEVEL_H_


#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"
#include "peano/utils/Globals.h"
#include "peano/utils/Loop.h"


#include <bitset>


namespace peano {
  namespace grid {
      class TraversalOrderOnTopLevel;
      namespace tests {
        class TraversalOrderOnTopLevelTest;
      }
  }
}



/**
* Holds a sequence of integers representing in which order the @f$ 3^d @f$
* fine grid cells are processed.
*
* @author Tobias Weinzierl
*/
class peano::grid::TraversalOrderOnTopLevel {
  private:
    friend class peano::grid::tests::TraversalOrderOnTopLevelTest;

    /**
     * Logging device
     */
    static tarch::logging::Log  _log;

    static const int InvalidOrderEntry;

    tarch::la::Vector<DIMENSIONS,int>   _order[THREE_POWER_D];
    bool                                _coarseBoundaryInBetweenCells[THREE_POWER_D-1];

    /**
     * Wait Until Cell Data is Available
     *
     * Is only defined in the parallel mode. In the serial version, it becomes
     * nop. This operation first
     *
     * While cellIndexInOrder is the next cell that has to be read, this refers
     * to the next cell on the local level one. It might happen, that you wanna
     * read the cell 7 after having processed cell 6, but then you receive a
     * flag that tells you to descend before going to cell 7. In this case, the
     * operation sets the corresponding entry in _coarseBoundaryInBetweenCells
     * and returns, i.e. the operation's name is, in this special case, wrong.
     *
     * !!! Algorithm
     *
     * - IsUnavailable: We check whether this cell is already available. If it
     *   is, then there's no need to receive anything. We can directly jump
     *   back.
     * - Otherwise: We receive a flag telling us which vertices are adjacent to
     *   the next cell.
     * - Due to getCellIdentifiedByReceivedBitset() we can find out whether
     *   this cell is already stored in _order. If it is not stored, we save it
     *   away and return.
     * - If it is already held, then this means that we should descend before
     *   we continue. We set the corresponding entry in
     *   _coarseBoundaryInBetweenCells and jump back.
     *
     * @param cellIndexInOrder     Next cell that has to be read.
     * @param isTraversalInverted  Is the traversal inverted?
     */
    void waitUntilCellIsAvailable(int cellIndexInOrder, bool isTraversalInverted);

    /**
     * Get Cell Identified by the Argument
     *
     * Takes a bitset that basically tells you which vertices adjacent to
     * the next cell in the MPI buffer are adjacent to this worker's
     * domain. If the left bottom vertex, e.g., is the only one that is
     * adjacent, then this implies that the corresponding cell on the
     * worker is the upper right one (2d example).
     *
     * The algorithm works as follows:
     * - Create the result vector = (1,1,1,1,1,...).
     * - Run along each coordinate direction (from now on 1-dimensional explanation):
     * - (d=0): If bit 0,2,4,6, or ... is set, result(0)++;
     * - (d=0): If bit 1,3,5,7, or ... is set, result(0)--;
     * - (d=1): If bit 0,1,4,5, or ... is set, result(1)++;
     * - (d=1): If bit 2,3,6,7, or ... is set, result(1)--;
     */
    static tarch::la::Vector<DIMENSIONS,int> getCellIdentifiedByReceivedBitset( const std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT>& mask );

    bool isCellContainedInOrder( const tarch::la::Vector<DIMENSIONS,int>& cell ) const;

    TraversalOrderOnTopLevel(const TraversalOrderOnTopLevel& copy) {}

    TraversalOrderOnTopLevel& operator=(const TraversalOrderOnTopLevel& copy) { return *this; }
  public:
    TraversalOrderOnTopLevel();

    /**
     * Is not const as it might wait for MPI messages and enrich the local
     * arrays with new data.
     */
    tarch::la::Vector<DIMENSIONS,int> getNextCellToTraverseOnLevelOne(
      int     numberOfCell,
      bool    isTraversalInverted
    );

    /**
     * Descend Before Continuing
     *
     * Is not const as it might wait for MPI messages and enrich the local
     * arrays with new data.
     *
     * In the parallel mode, some root nodes do not realise a local sfc order
     * on their level-one-cells, i.e. their @f$ 3^d @f$ patch is not traversed
     * along the Peano curve but along the global Peano curve. That means, the
     * traversal might be disconnected, and it also implies that the traversal
     * is not any longer a level-wise dfs but that some cells have to descend
     * immediately. For this, the load loop in the Root type asks after each
     * load call, whether to descend now or not.
     *
     * The realisation is pretty straightforward. We just have a look which one
     * is the right entry in _order. Then we wait until the next cell integer
     * is received because there'll always be a next cell - either the next on
     * the local level one or one from a finer level.
     *
     * @param numberOfCell Next cell that is to be traversed. Number ranging
     *                     from zero to @f$ 3^d-1 @f$.
     *
     * @see waitUntilCellIsAvailable()
     */
    bool descendBeforeContinuingWithCell(
      int     numberOfCell,
      bool    isTraversalInverted
    );

    void initialiseWithPeanoSFC(peano::utils::LoopDirection loopDirection);

    /**
     * Erase all information and make object invalid.
     */
    void reset();

    /**
     * Validates the entries of the struct technically, i.e. does not check
     * for semantical correctness. Technically soundness implies that all
     * the _order entries are equal or greater than zero and smaller than
     * three.
     */
    bool isValid() const;

    std::string toString() const;
};

#endif
