// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_REFINED_H_
#define _PEANO_GRID_NODES_REFINED_H_


#include "peano/utils/Globals.h"
#include "peano/grid/SingleLevelEnumerator.h"
#include "peano/grid/nodes/Node.h"

#include <bitset>



/**
 * Refined Node
 *
 * <h1> Order of events on one cell level</h1>
 *
 * There is a detailed description on the order of the events on the page
 * "Order of cell events in serial code". See the webpages created from the
 * sources or consult the file event-order.doxys.
 *
 * @author Tobias Weinzierl
 */
template <
  class Vertex,
  class Cell,
  class State,
  class VertexStack,
  class CellStack,
  class EventHandle
>
class peano::grid::nodes::Refined: public peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle> {
  private:
    typedef peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle>               Base;
    typedef peano::grid::nodes::Leaf<Vertex,Cell,State,VertexStack,CellStack,EventHandle>               LeafNode;
    typedef peano::grid::nodes::RegularRefined<Vertex,Cell,State,VertexStack,CellStack,EventHandle>     RegularRefinedNode;

    typedef peano::grid::RegularGridContainer<Vertex,Cell>                                    RegularGridContainer;

    static tarch::logging::Log _log;

    LeafNode&              _leafNode;
    RegularRefinedNode&    _regularRefinedNode;
    RegularGridContainer&  _regularGridContainer;

    /**
     * Set Counter, create hanging nodes and new nodes
     *
     * @param coarseGridVertices That is the coarse grid vertices throughout
     *                           the descend process, i.e. it is the fine grid
     *                           vertices of the refined event.
     */
    void setCounter(
      Vertex                        coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&  coarseGridVerticesEnumerator,
      int                           fineGridCounter[FOUR_POWER_D]
    );

    /**
     * @see Node::updateCellAfterLoad()
     */
    void updateRefinedCellBeforeStore(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
    );

    void validateRefinementStateOfVertices(
      Vertex const * const          fineGridVertices,
      const SingleLevelEnumerator&  fineGridVerticesEnumerator
    );

    /**
     * Split up the grid further
     *
     * This operation is active if and only
     * - if the code is compiled with MPI, and
     * - if the current node is the master node or its oracle is set to fork at least once, and
     * - if the current node is not involved in a fork.
     *
     * !!! Invoke setAdjacentSubtreeForksIntoOtherRankFlag()
     *
     * This is the place where I mark the new subtree, i.e. where I flag all
     * the vertices that will in the next iteration be copied to a new worker.
     * If I don't do it here, the grid might coarse out the respective subgrid
     * right before the actual fork takes place. This is kind of a starvation
     * process. As soon as data is deployed to a worker, the master-worker
     * communication routines take care of the erase flags and erase control.
     * Before, I have to do it here manually.
     */
    void splitUpGrid(
      State&                                       state,
      Cell&                                        fineGridCell,
      Vertex                                       fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&                 fineGridVerticesEnumerator,
      Cell                                         newFineGridCells[THREE_POWER_D],
      const std::vector< SingleLevelEnumerator >&  newFineGridVerticesEnumerators,
      Vertex*                                      newFineGridVertices
    );

    /**
     * Identify cells of that could be forked
     *
     * Runs through the cells of a @f$ 3^d @f$ patch. The bitset at the
     * beginning is unset. The operation then flags each bit for each
     * cell that is local. A cell may be forked if and only if all of the
     * following criteria do hold:
     *
     * - the cell is refined (cmp Cell::isCellAForkCandidate())
     * - the cell is not yet spawned to another rank
     * - the cell is not refined, local, but a descendent in turn has spawned stuff
     * - the cell overlaps with the computational domain, i.e. it is not completely outside
     *
     * For the geometric analysis whether a cell is completely outside (Geometry::isCompletelyOutside()),
     * we do not pass any Minkowski parameter, i.e. the parameter h is zero. This does make sense
     * here as we want to have the precise overlap.
     */
    std::bitset<THREE_POWER_D> getForkCandidates(
      const std::vector< SingleLevelEnumerator >&  newFineGridVerticesEnumerators,
      Cell                   newFineGridCells[THREE_POWER_D],
      Vertex                 fineGridVertices[FOUR_POWER_D]
    ) const;
  public:
    Refined(
      VertexStack&                vertexStack,
      CellStack&                  cellStack,
      EventHandle&                eventHandle,
      peano::geometry::Geometry&  geometry,
      LeafNode&                   leafNode,
      RegularRefinedNode&         regularRefinedNode,
      peano::grid::RegularGridContainer<Vertex,Cell>&  regularGridContainer
    );

    /**
     * @see Standard constructor
     */
    virtual ~Refined();

    void load(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell,
      int                                       counter[FOUR_POWER_D]
    );

    /**
     * Traverse the subcells
     *
     * This operation executes the following task sequence:
     *
     * - Load the subcells,
     * - call enterCell() for the refined cell,
     * - invoke splitUpGrid() which is only relevant in the parallel mode,
     * - invoke descend(),
     * - call leaveCell() for the refined cell, and
     * - store the refined cells away.
     *
     * As load and stores are inherently sequential due to the stack data
     * structure, we can only call step 1 and 2 and the last two stepts in
     * parallel, i.e. the maximum concurrency level is two.
     *
     * The fine grid cell is the one identified by the fine grid enumerator.
     */
    void traverse(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
    );

    /**
     * Descend in spacetree
     *
     * This operation is called for the subcells of a refined cell, i.e. state,
     * cell, and fine grid vertices all correspond to one cell within a
     * @f$ 3^d @f$ patch.
     *
     * We already have loaded the @f$ 3^d @f$ subsequent grid cells on the next
     * finer level, i.e. one level smaller than fineGridVerticesEnumerator. So,
     * the algorithm performs the following steps:
     *
     * - Create array of enumerators. These enumerators are analysed from the
     *   refined cell, i.e. it can contain solely static information. In the
     *   first loop, we set their offsets correctly. In the second loop, we can
     *   then try to find out whether they represent regular subtrees, e.g.
     *   This would be impossible a priori, as we need the subgrid's vertices
     *   to do this analysis.
     * - Load all the vertices of the @f$ 3^d @f$ subcells and see whether we
     *   can modify (restrict) the vertex enumerators' flags (for example find
     *   out whether a subcell is a regular refined grid).
     *
     * !!! Visibility
     *
     * At first glance, this operation seems to be an internal operation of
     * the refined node's traverse(). It thus should be private. However, we
     * have to keep in mind that a leaf node also might transform into a
     * refined node throughout the traversal. Then, the leaf needs descend()
     * as well.
     */
    void descend(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell                                      newFineGridCells[THREE_POWER_D],
      bool                                      destroySubLevelsFinally
    );

    /**
     * Called by descend() for each of the @f$ 3^d @f$ children.
     *
     * !!! Regular parallel subtrees
     *
     * On regular subtrees, the descend() tries to use recursion unrolling
     * instead of the depth-frist traversal. This however is possible if and
     * only if
     *
     * - recursion unrolling is switched on as part of Peano's optimisations,
     * - the main memory can hold the whole unrolled tree, and
     * - the subtree is not marked as remote.
     *
     * The last issue might be surprising, as a remote subtree induces parallel
     * boundary vertices, and, hence, the upper tree part is not regular but
     * only static. This assumption however does not hold throughout the fork,
     * when the remote flag propagates top-down. In this case, I switch off the
     * unrolling.
     *
     *
     * !!! Visibility
     *
     * At first glance, this operation seems to be an internal operation of
     * the refined node's descend(). It thus should be private. However, the
     * root node needs this operation as well to be able to do recursion unrolling.
     */
    void descendIntoASingleCell(
      State&                                    state,
      Cell&                                     currentCell,
      Vertex*                                   fineGridVertices,
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseCell,
      Vertex*                                   coarseGridVertices,
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
    );

    void store(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell,
      int                                       counter[FOUR_POWER_D]
    );
};


#include "peano/grid/nodes/Refined.cpph"

#endif
