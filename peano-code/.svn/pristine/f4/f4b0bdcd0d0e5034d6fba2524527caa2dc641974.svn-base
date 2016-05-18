// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_REGULAR_REFINED_H_
#define _PEANO_GRID_NODES_REGULAR_REFINED_H_


#include "peano/utils/Globals.h"

#include "peano/grid/nodes/Node.h"
#include "peano/grid/RegularGridContainer.h"

#include "peano/datatraversal/autotuning/OracleForOnePhase.h"


namespace peano {
    namespace grid {
      namespace nodes {
        void computePositionRelativeToNextCoarserLevelFromFineGridVertexPosition(
          const tarch::la::Vector<DIMENSIONS,int>&  fineGridPosition,
          tarch::la::Vector<DIMENSIONS,int>&        offsetOfCoarseGridEnumerator,
          tarch::la::Vector<DIMENSIONS,int>&        fineGridPositionRelativeToCoarserCell
        );

        void computePositionRelativeToNextCoarserLevelFromFineGridCellPosition(
          const tarch::la::Vector<DIMENSIONS,int>&  fineGridPosition,
          tarch::la::Vector<DIMENSIONS,int>&        offsetOfCoarseGridEnumerator,
          tarch::la::Vector<DIMENSIONS,int>&        fineGridPositionRelativeToCoarserCell
        );

        /**
         * Translate result from oracle
         *
         * The oracle has a grain size semantics and not the level-wise semantics,
         * i.e. if it is given k and it returns k/2, the oracle expects the calling
         * function to split up the problem such that the concurrency level is two.
         * However, the tree splits for the read and write vertex processes wanna
         * know up to which level they may split up the tree.
         *
         * Consequently, we have to translate the result due to the following
         * formula:
         *
         * - oracleResult=0, return -1
         * - otherwise return T-s
         *
         * where s is the normalised grain size, i.e. the size divided by the total
         * number of vertices times the tree depth.
         *
         * @param NumberOfVerticesInSubtree Is redundant as we could reconstruct it
         *          from TreeDepth, but I decided to use a precomputed value that
         *          is needed for the stacks anyway.
         */
        int transformOracleResult( int oracleResult, int TreeDepth, int NumberOfVerticesInSubtree );
      }
    }
}


/**
 * Regulary RegularRefined Node
 *
 * This node has neither a load nor a store operation, as the refined node
 * should use its own load and store operation to implement this. So, it is
 * basically a traverse operation.
 *
 * This operation executes the following two task sequences per level:
 *
 * - Create the data fields
 * - Set counter to zero (everything zero)
 * - Load cells from input stream into array (stream)
 * - Load vertices into array (stream and some actions)
 * - Call enter cell for each cell
 *
 * Step one and two could run in parallel. Step three requires results from
 * step 1, and the fourth step requires the vertice, i.e. the results from
 * step 3.
 *
 * - Call leave cell for each cell
 * - Store vertices from array (stream and some actions)
 * - Store cells from array into output stream
 *
 * Step two and three can run in parallel.
 *
 * !!! Naiv (and wrong) approach
 *
 * The parallelisation of this whole process is based upon a manual
 * combination of loop and recurson unrolling over all levels as well as a
 * loop fusion. The algorithm then reads as follows:
 *
 * - For i=-3 up to maximum level do these steps in parallel:
 *   - If (i+3<max):
 *     - Allocate new memory for counters on level i+3 and add it to container.
 *     - Allocate new memory for cells on level i+3 and add it to container.
 *     - Allocate new memory for vertices on level i+3 and add it to container.
 *   - If (i+2<max && i+2>0):
 *     - Set vertex counters of level i+2 to zero
 *     - Load cells into stream of level i+2
 *   - If (i+1<max && i+1>0):
 *     - Load vertices into level i+1
 *   - If (i>0):
 *     - Call enter cell for all cells of level i (red-black GS pattern).
 *
 * - For i=max+1 down to 0:
 *   - if (i-1<max && i-1>0):
 *     - Call leave cell for all cells of level i-1
 *   - if (i<max):
 *     - Store vertices to array
 *     - Store cells to to array
 *
 *
 * Besides the last entry, everything is embarassingly parallel.
 *
 * That does not work. And the reason is very simple: Peano's ordering is
 * inherently recursive.
 *
 * @image html peano/grid/nodes/RegularRefined_CellOrder.png
 *
 * If we processed level-wise the (level-wise) recursive order along the cell
 * boundaries would not be preserved. So the appraoch sketched above doesn't
 * work. Moreoever, this insight implies that Peano's data management is
 * sequential, or, more precisely, its concurrency level is bounded by the
 * maximum number of stacks used.
 *
 * A simple solution is to split up the traversal into three different phases:
 *
 * - A load process not triggering any operations
 * - All the operations
 * - A store process
 *
 * Load and store have a fixed concurrency level, i.e. they really are
 * bounded by the maximum number of streams used. The second phase
 * however is very parallel, and we can process is with a loop over the
 * grid levels once running top-down and then bottom up. Again, this is a
 * sequence/fusion of two loops working on grid level i and i+1. The
 * first one calls touchFirstTime() (on level i+1), the other one processes
 * the cells on level i in a red-black fashion.
 *
 * For this implementation, we assume that the computation is by magnitudes
 * more expensive than the load and store processes. If this were not the
 * case, probably it is faster to run without the blocking strategy on regular
 * patches. Do switch it off, unset UseRecursionUnrollingOnRegularPatches in
 * Peano's optimisation header.
 *
 *
 * @image html peano/grid/nodes/RegularRefined_Parallelisation.png
 *
 * The shared memory parallel version of the code then consists of, besides
 * some initialisation, four different steps:
 *
 * - load vertices
 * - descend
 * - ascend
 * - store vertices
 *
 * Load and store are serial due to the recursive definition of the spacetree.
 * Unfortunately, throughout the descend, the enter cell and the load vertices
 * also are pretty sequential if we study the dependency graph.
 * For ascend, the same arguing holds as for descend with another order of
 * level dependencies.
 *
 * @image html peano/grid/nodes/regular-grid-dependency-graph.png
 *
 * The dependency graph reveals that we have a concurrency level of one.
 *
 * !!! Parallel Code
 *
 * The code thus realises another pattern: We fire all load, enter cell, and
 * touchVertexFirstTime as well as their counterparts throughout the tree
 * ascend as parallel tasks. These tasks communicate through three different
 * types of semaphores: Each semaphore is a natural number indicating which
 * levels have already been completed. We thus hope that multiple tasks can
 * overlap.
 *
 * @image html peano/grid/nodes/regular-grid-parallelisation.png
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
class peano::grid::nodes::RegularRefined: public peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle> {
  private:
    typedef peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle>     Base;

    typedef peano::grid::RegularGridContainer<Vertex,Cell>                                    RegularGridContainer;

    static tarch::logging::Log _log;

    RegularGridContainer&  _regularGridContainer;

  public:
    RegularRefined(
      VertexStack&                vertexStack,
      CellStack&                  cellStack,
      EventHandle&                eventHandle,
      peano::geometry::Geometry&  geometry,
      peano::grid::RegularGridContainer<Vertex,Cell>&  regularGridContainer
    );

    /**
     * @see Standard constructor
     */
    virtual ~RegularRefined();

    /**
     * Traverse the subcells
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
     * Proxy for the regular grid container. If there were no such proxy, the
     * refined event would have to hold a reference to the container as well.
     */
    bool isRegularSubtreeAvailable( int requestedHeight );
};


#include "peano/grid/nodes/RegularRefined.cpph"

#endif
