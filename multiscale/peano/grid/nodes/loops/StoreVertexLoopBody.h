// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_LOOPS_STORE_VERTEX_LOOP_BODY_H_
#define _PEANO_GRID_NODES_LOOPS_STORE_VERTEX_LOOP_BODY_H_


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "tarch/multicore/MulticoreDefinitions.h"

#include "peano/utils/Globals.h"
#include "peano/datatraversal/Action.h"
#include "peano/geometry/Geometry.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace loops {
        template <class Vertex,class Cell,class State,class VertexStack,class EventHandle, int CellFlagsFromEnumerator>
        class StoreVertexLoopBody;
      }
    }
  }
}




/**
 * Store Loop Body
 *
 * <h2> Optimisation </h2>
 *
 * If there were a hanging node adjacent to the leaf, the vertex enumerators's
 * flag, i.e. CellFlagsFromEnumerator would not be equal to leaf anymore.
 *
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell, class State, class VertexStack, class EventHandle, int CellFlagsFromEnumerator>
class peano::grid::nodes::loops::StoreVertexLoopBody {
  private:
    static tarch::logging::Log _log;

    State&                                    _state;
    Vertex*                                   _fineGridVertices;
    const SingleLevelEnumerator&              _fineGridVerticesEnumerator;
    Cell&                                     _coarseGridCell;
    Vertex*                                   _coarseGridVertices;
    const SingleLevelEnumerator&              _coarseGridVerticesEnumerator;
    const tarch::la::Vector<DIMENSIONS,int>&  _fineGridPositionOfCell;
    int*                                      _counter;
    VertexStack&                              _vertexStack;

    EventHandle&                              _eventHandle;

    /**
     * Usually, the geometry should be const. See getters of geometry for a
     * description.
     */
    peano::geometry::Geometry&                _geometry;

    /**
     * Update Refinement Status Before Data is Written to Output Stream
     *
     * Operation basically calls Vertex::updateTransientRefinementFlagsBeforeVertexIsStoredToOutputStack()
     * and updates the state's global fields to track whether the grid
     * will change and has changed. Finally, the vertex is piped to the
     * output stream.
     *
     * The method is not to be called for hanging nodes.
     *
     * <h2> Thread-safety </h2>
     *
     * The operation does not modify the state directly but works on the
     * local attributes such as _hasRefined(). These fields then are merged
     * into the shared state in the destructor. The destructor hence has to
     * be thread safe.
     *
     * <h2> Note on dynamic refinement </h2>
     *
     * We we have complicated refinement patterns, it can happen that
     * vertices surrounded by @f$ 2^d @f$ refined cells are unrefined.
     * This induces that there is a haning node on the next finer level
     * that cannot be seen by the vis, as it is sourounded by @f$ 2^d @f$
     * cells as well.
     *
     * I thus keep track of the number of adjacent refined cells throughout
     * the traversal. If a vertex has @f$ 2^d @f$ refined cells around it
     * but is unrefined itself, I can trigger a refinement automatically by
     * calling updateRefinementFlagsAndStoreVertexToOutputStream().
     *
     * Though this operation is a refinement operation, I may not call it
     * here. If I called it here, I'd introduce a bug in the MPI
     * parallelisation mode: If a vertex is member of the domain
     * boundary and runs into the case described before, one node would
     * trigger a refinement here. However, the vertex copy is already sent
     * to the neighbours, i.e. next iteration the local node refined. The
     * neighbours however are not aware yet of this refinement. It is thus
     * important to call updateRefinementFlagsAndStoreVertexToOutputStream()
     * before the vertex is passed to the send/receive buffer.
     *
     * <h2> Parallel mode </h2>
     *
     * If the code is running in parallel, it coarsens grid regions after each
     * fork. These grid modifications however are not really a grid transition,
     * as the real grid is just handled on a different rank. So we check
     * whether a vertex is remote before we update _hasTriggeredEraseForNextIteration.
     *
     * @see updateRefinementFlagsAndStoreVertexToOutputStream()
     */
    void updateRefinementFlagsAndStoreVertexToOutputStream(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);

    /**
     * Exchange and update parallel information
     *
     * The operation degrades (almost entierly) to nop if the code is not compiled with MPI
     * support. We call this operation for each persistent vertex before
     * it is streamed to the output stack, i.e. before
     * updateRefinementFlagsAndStoreVertexToOutputStream() is called.
     * The operation also validates that the vertex is adjacent to a remote
     * rank. If it is not, it immediately returns. Otherwise, it runs through
     * a sequence of checks.
     *
     * Independent of the MPI flags, the operation calls refineVertexIfItHasOnlyRefinedAdjacentCells()
     * on the vertex. There is a dedicated section in updateRefinementFlagsAndStoreVertexToOutputStream()
     * on this issue.
     *
     * <h2> Code is joining with a worker </h2>
     *
     * We are in the second phase of the join, i.e. now data is transferred
     * from the worker to this node. As a consequence, we can replace all
     * worker entries by the local rank's number before we write the vertex
     * to the output stream.
     *
     * <h2> Code is joining with master </h2>
     *
     * We are in the second phase of the joint, i.e. the local data is moved
     * to the master. There is nothing to do on the local rank - all data will
     * be thrown away after this iteration anyway:
     *
     * Usually, non-remote vertices that are adjacent to a neighbour are
     * passed to the send receive buffer pool. Yet, such vertices are
     * ignored if we join with the master. As this is the second step of
     * a join process where actually the whole local tree is transferred
     * to the master, we can be sure that noone has sent us any vertices
     * anymore and nobody is expecting this node to send any information.
     *
     *
     * <h2> Code is not joining with master </h2>
     *
     * If the vertex is not remote (it holds the local rank number as adjacent
     * number as well), send a copy to the neighbour. If the join with the
     * master is already triggered, we have to replace the entries before.
     * Otherwise, we can take the plain copy but have to pass it to the event
     * handle before.
     *
     * <h2> Vertex is not adjacent to local domain </h2>
     *
     * In this case, the vertex has not to be sent away. The local rank is not
     * adjacent to it, so it is not involved in any communication for this
     * vertex. If such a vertex is not outside yet, we have deployed it to
     * another rank in this iteration and now are not responsible for this
     * vertex anymore. Consequently, we can call a destroy event for this
     * vertex and coarse it.
     *
     * <h2> Remote vertex detected throughout first traversal of new worker </h2>
     *
     * If we detect a remote vertex in the first iteration on a worker, we
     * set all adjacency information to the local master: The vertex adjacency
     * now is correct but we will not be informed when it changes later on
     * due to load balancing. So it makes sense, to set all data to the master
     * rank indicating that the master knows more about the state of this
     * vertex while we keep not track of its state.
     *
     * If this check is removed, the following scenario may happen: Node 1
     * forks into 2 and 3 in one step. On node 2, are remote vertex has the
     * adjacent ranks 0 and 3. It is not kept consistent anymore - it does not
     * take part in any communication. Let's now assume that 1 and 3 merge.
     * So fine, everything is fine, but our vertex with 0,3 is not updated to
     * hold exclusively 0s. Unfortunately, our local node 2 accepts
     * 3 as new worker. Now, this (invalid) adjacency information makes
     * problems, as Peano considers a remote vertex suddenly as domain
     * boundary vertex.
     *
     * @see updateRefinementFlagsAndStoreVertexToOutputStream()
     */
    void exchangeVerticesDueToParallelisation(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);


    void destroyHangingNode(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);

    /**
     *
     * <h2> Parallelisation </h2>
     *
     * Shall not be called for remote vertices, i.e. for such vertices the operation becomes nop.
     */
    void destroyPersistentNode(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);

    /**
     *
     * <h2> Parallelisation </h2>
     *
     * Shall not be called for remote vertices, i.e. for such vertices the operation becomes nop.
     *
     * <h2> Optimisation </h2>
     *
     * Peano's mapping specifications would allow to skip this statement for
     * refined vertices if the marker is set to false. However, we observed
     * that the evaluation of the statement on most architectures is more
     * expensive than the actual savings in runtime. Thus, this operation
     * calls touchVertexFirstTime() always.
     */
    void invokeStoreVertexEvent(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);

    /**
     * In debug mode, this operation compares the vertex's position and level
     * to the cell data.
     */
    void validateVertexInArray( int positionInVertexArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell ) const;

    /**
     * Restrict the Information on Static Subtrees to the Coarser Levels
     *
     * <h2> Thread-safety </h2>
     *
     * Should be thread safe, as writes to the output stream are synchronised
     * anyway. The operation basically does the analysed tree grammar and its
     * behaviour is sketched below.
     *
     * @image html peano/grid/nodes/loops/StoreVertexLoopBody_updateCoarseGridTreeHeightAttributes.png
     *
     * This sketch stems from a regular grid for the unit square. The coloured
     * cells are inside cells. The grid is regular within the domain but very
     * adaptive outside. The red markers are the vertex flags (with h meaning
     * hanging and -1 only stationary) and the blue markers are the resulting
     * cell flags.
     *
     * We see that the blue markers results from a minimum over all adjacent
     * vertex markers of a cell. And we see that this is not efficient, as the
     * boundary cells on the coarsest levels are not treated as regular blocks
     * but as stationary adaptive grids. We loose some efficiency here. In the
     * ideal case, there should be (for this sketch) one subgrid of height
     * three and no other regular patches.
     *
     * <h2> Tuning </h2>
     *
     * We can tune up the code significantly, if we realise the following two
     * constraints:
     *
     * - Outer fine grid vertices may influence exclusively outer fine grid vertices.
     * - Inner and boundary fine grid vertices influence coarser vertices.
     *
     * This is possible as the domain covered by Peano grids (the volume) grows
     * monotonically with each additional grid level, i.e. a grid of level k
     * always covers at least the domain covered by a grid of level k-1.
     *
     * <h2> Parallel mode </h2>
     *
     * If we compile with MPI, the operation restricts the flag
     * adjacentSubtreeForksIntoOtherRank. This flag is used at other places
     * to avoid starvation. See LoadVertexLoopBody::updateRefinementFlagsOfVertexAfterLoad()
     * for documentation on starvation.
     *
     * he tuning statement from above does hold in the parallel case. There,
     * there can be wholes in an inner coarse domain induced by the finer grid
     * where cells are deployed to another rank.
     */
    void updateCoarseGridTreeHeightAttributes(
      int                                       positionInVertexArray,
      const tarch::la::Vector<DIMENSIONS,int>&  positionInLocalCell
    );

    /**
     * Invalidate Coarse Grid Entries
     *
     * Should be called only for (persistent) vertices written to the temporary
     * stacks.
     *
     * Usually, I restrict regularity information (where are regular blocks)
     * solely for vertices written to the output stream. The story however is
     * different, if we do an on-the-fly refinement, i.e. if the user calls
     * refine() in touchVertexFirstTime() and the code decides to refine
     * immediately.
     *
     * @image html peano/grid/nodes/loops/StoreVertexLoopBody_invalidateCoarseGridTreeHeightAttributesIfRefined.png
     *
     * In the example above let the grey block be instationary, the blue one is
     * a regular tree. Grey is visited before blue. Within the grey block, the
     * code decides to refine the red vertex throughout the
     * touchVertexFirstTime(). The traversal automaton does the refinement
     * immediately. Afterwards, they bluegrey grid does exist. The red vertex is
     * written to the temporary stack for the blue adjacent domain.
     *
     * However, the code still assumes that blue is stationary, as the flags
     * have not been invalidated. For this reason, this operation immediately
     * invalidates all coarse grid flags, if an immediate-refine happens. If it
     * were a refinement process split up into two iterations, i.e. into
     * refinement-triggered and refining, this problem would not occur.
     *
     * To make this operation work recursively, i.e. among several levels, it
     * is important that not only refining vertices make their coarse grid
     * vertices invalid. Also invalidated vertices have in turn to invalidate
     * their coarse grid vertices.
     *
     * <h2> State update </h2>
     *
     * Though we invalidate parts of the tree, we do not inform the state about
     * this. Only refinement information is restricted, i.e. the state already
     * has to be informed about changes.
     */
    void invalidateCoarseGridTreeHeightAttributesIfRefined(
      int                                       positionInVertexArray,
      const tarch::la::Vector<DIMENSIONS,int>&  positionInLocalCell
    );
  public:
    StoreVertexLoopBody(
      State&                                    state,
      Vertex*                                   fineGridVertices,
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex*                                   coarseGridVertices,
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell,
      int*                                      counter,
      VertexStack&                              vertexStack,
      EventHandle&                              eventHandle,
      peano::geometry::Geometry&                geometry
    );

    ~StoreVertexLoopBody() = default;

    /**
     *
     *
     *
|| Stack \\ Counter || persistent                        || hanging                           || new                               || delete                            || none
|| InOut            |  store(-1)                         |  destroy hanging node              |  store(-1)                         |  destroy persistent vertex         |  nop (vertex is still needed)
|| Temporary        |  store(stack number)               |  store(stack number)               |  store(stack number)               |  store(stack number)               |  nop (vertex is still needed)
     */
    void operator() (const peano::datatraversal::Action& action);
};


#include "peano/grid/nodes/loops/StoreVertexLoopBody.cpph"


#endif
