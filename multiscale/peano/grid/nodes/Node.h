// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_NODE_H_
#define _PEANO_GRID_NODES_NODE_H_


#include "peano/grid/SingleLevelEnumerator.h"
#include "peano/geometry/Geometry.h"


namespace peano {
    namespace grid {
      namespace nodes {
        template <
          class Vertex,
          class Cell,
          class State,
          class VertexStack,
          class CellStack,
          class EventHandle
        > class Node;

        /**
         * Forward declaration
         */
        template <
          class Vertex,
          class Cell,
          class State,
          class VertexStack,
          class CellStack,
          class EventHandle
        > class Leaf;

        /**
         * Forward declaration
         */
        template <
          class Vertex,
          class Cell,
          class State,
          class VertexStack,
          class CellStack,
          class EventHandle
        > class Refined;

        /**
         * Forward declaration
         */
        template <
          class Vertex,
          class Cell,
          class State,
          class VertexStack,
          class CellStack,
          class EventHandle
        > class RegularRefined;

        /**
         * Forward declaration
         */
        template <
          class Vertex,
          class Cell,
          class State,
          class VertexStack,
          class CellStack,
          class EventHandle
        > class Root;
      }
    }
}


/**
 * Node
 *
 * While I do not rely on any inheritance mechanism for the cells, there's a
 * couple of operations and relations that are needed by any node. These
 * attributes and operations are collected in this very class.
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
class peano::grid::nodes::Node {
  private:

    static tarch::logging::Log _log;

  protected:
    /**
     * Holds the vertex stack associated to this event.
     */
    VertexStack& _vertexStack;

    /**
     * Holds the cell stack associated to this event. This is important for
     * all events events - even leaves need it as they might refine further.
     */
    CellStack&   _cellStack;

    /**
     * Holds the event handle associated to this event.
     */
    EventHandle&       _eventHandle;

    /**
     * Geometry handle.
     */
    peano::geometry::Geometry&  _geometry;

    Node(
      VertexStack&                vertexStack,
      CellStack&                  cellStack,
      EventHandle&                eventHandle,
      peano::geometry::Geometry&  geometry
    );

    /**
     * @see Standard constructor. There's nothing to do here.
     */
    ~Node();

    CellStack&    getCellStack() const;
    VertexStack&  getVertexStack() const;


    #ifdef Parallel
    /**
     * Assign cell to remote rank and update its vertices
     *
     * Take the cell, assigne it to the remote rank, and also update the
     * adjacency information fo the adjacent vertices. Finally, also set
     * adjacentSubtreeForksIntoOtherRankFlag.
     *
     * There should be no need to set the flag
     * setAdjacentSubtreeForksIntoOtherRankFlag() as this flag is
     * automatically set correctly throughout the store process.
     */
    void makeCellRemoteCell(
      State&                         state,
      int                            remoteRank,
      Cell&                          fineGridCell,
      Vertex                         fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&   fineGridVerticesEnumerator
    ) const;
    #endif

    /**
     * Validation Routines
     *
     * Validates all the vertex attributes that duplicate the information given
     * by the vertices. That is grid level and vertex's coordinates. If
     * information consistency isn't given, the method finished triggering an
     * assertion. This operation should be used by the load operations to
     * validate the data consistency.
     *
     * The operation becomes the nop operation if either the debug and assert
     * mode is switched off.
     */
    static void validatePositionOfVertices(
      Vertex                                fineGridVertices[FOUR_POWER_D],
      const peano::grid::VertexEnumerator&  fineGridVerticesEnumerator
    );

    /**
     * Update cell's geometric state
     *
     * This little helper is called by the load operation as soon as both the
     * vertices and the cell object are loaded. It analyses the current cell
     * state, the geometry and modifies the cell is necessary.
     *
     * If the cell belongs to a static tree or if the cell is remote, i.e. it
     * is handled by another node, then this operation does not change the
     * cell's state.
     *
     * !!! Hidden Geometries
     *
     * Besides the geometry update, we also have to check whether the
     * computationally is hidden in the cell: This happens if all the vertices
     * adjacent to a cell are outside but the domain is contained within the
     * cell.
     *
     * @image html peano/grid/nodes/Node_InvokeEnterCell.png
     *
     * In this special case, we have to refine the vertices even though they
     * are outside.
     *
     * !!! Parallelisation
     *
     * You finsd documentation on details of the parallelisation in the method
     * LoadVertexLoopBody::updateGeometry().
     */
    void updateCellsGeometryInformationAfterLoad(
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
     * Handle cell's parallel attributes
     *
     * This operation has different jobs:
     *
     * !!! Inherit remote state
     *
     * If a cell is not remote but its parent is remote, then the code has
     * triggered a fork for one of the coarser cells. In this case, we make
     * the children remote as well. There is however an exception to this rule:
     * On parallel workers, the root cell has the rank of the master node which
     * is absolutely correct. In this case, we are not allowed to change the
     * responsible rank of the middle patch cell on level one though.
     *
     * @image html peano/grid/nodes/Node_updateCellsParallelStateAfterLoad_inherit_remote_state.png
     *
     * This case basically invokes makeCellRemoteCell(). This operation in turn
     * is invariant on already remote cells, i.e. you may call it several times
     * in a row, it only changes the cell state once at the first call.
     *
     * We now encounter a special case, if a node is in the state
     * fork-triggered and at the same time refines the grid. If that happens,
     * the newly created cells already are assigned to remote nodes. However,
     * the rank lists of the adjacent vertices are not validly set. Hence, we
     * call makeCellRemoteCell() also throughout the create cell process. See
     * Leaf::traverse() for implementation details.
     *
     * !!! Startup remote nodes
     *
     * If a cell is remote but its parent is not, then we have to startup a
     * remote node (if this remote node is not just forked, but that cannot
     * happen, as forks are triggered after this operation has been called).
     *
     * @image html peano/grid/nodes/Node_updateCellsParallelStateAfterLoad_startup_remote_node.png
     *
     * The story is a different one for joins: If a remote node is joining, we
     * may not start it up, as it already continues to traverse a second time
     * after the join triggered. The reason for this immediate re-traverse is
     * that the master needs the joined data (cell markers in particular)
     * before the worker is restarted again.
     *
     * !!! Distribute Fork-triggered Information
     *
     * If there is a fork going on (i.e. triggered already has changed to
     * forking), and if the cell is adjacent to one of the new workers, we have
     * to distribute the new parallel data and (if the cell is to be
     * transferred to the remote node) send away data. This is also done in
     * this operation.
     *
     * !!! Receive data from fork
     *
     * The data exchange throughout forks and joins is exclusively based upon
     * cells, i.e. it is always handled exclusively by this operation. So, if
     * we just started up a worker due to a fork, we always check the vertices.
     * If one of them is adjacent to the local node, then we have to receive it
     * from the master.
     *
     * Fork data always comes along in triples: First each cell sends a bitset
     * identifying which vertices are exchanged (see remark on hanging
     * nodes below), second the cell data itself is
     * sent, and, third, all the vertices shared are forwarded. The leading
     * flag is important for the worker to reconstruct the local space-filling
     * curve's properties on level one and to know when to descend from level
     * one to level two. On all other levels, the information is not relevant.
     * This operation nevertheless asks for it to clear the MPI buffers. And we
     * use the data for an assertion.
     *
     * @image html peano/grid/nodes/Node_updateCellsParallelStateAfterLoad_forking.png
     *
     * !!! Comments on prepareSendToWorker()
     *
     * For a deployed cell, i.e. a cell that is handled by a remote node, (and
     * all its descendants), the enterCell() event is split up.
     *
     * @image html peano/grid/nodes/parallel-root-cell-communication.png
     *
     * First, the grid invokes prepareSendToWorker() on the master. This allows
     * you to modify the vertices adjacent to a remote cell. These vertices and
     * the cell then are sent to the worker. The worker receives them due to
     * mergeWithWorker(). At the end of the traversal, the worker takes its
     * top level cell and the @f$ 2^d @f$ adjacent vertices and sends them back
     * to the master (see prepareSendToMaster()). The master receives this data
     * and merges them into the master data structure due to mergeWithMaster().
     * The second part of the communciation, i.e. all these data transferred
     * from the worker to the master, is omitted if the state reduction is
     * switched off.
     *
     * The master's operations have a huge signature and there is some
     * redundancy hidden in this interface: The @f$ 2^d @f$ adjacent vertices
     * of one remote cell implicitly are parallel boundaries, i.e. they
     * already do communicate with their neighbours. However, if all
     * @f$ 2^d-1 @f$ adjacent cells of a vertex are deployed to other ranks as
     * well, the master doesn't get any notice from this vertex anymore. The
     * latter fact imposes a problem if you are implementing multiscale
     * algorithms. Here, we need a plug-in point to enable programmers to
     * move data between two levels. This is possible only due to the
     * mechanism discussed in this section.
     *
     * !!! Exchanging Hanging Nodes
     *
     * The fork becomes pretty complicated if hanging nodes are involved on
     * rather coarse resolutions.
     *
     * @image html peano/grid/nodes/fork-hanging-nodes.png
     *
     * Left illustration above: There is only one vertex refined in the cell
     * that the master wants to deploy to another rank. Now, if the master
     * node runs into the blue cell, this cell is not even visible on the
     * worker node. It also should not be exchanged. This scenario is an
     * example why inheriting adjacency information to hanging nodes is
     * not a clever idea.
     *
     * Right illustartion above: One vertex of the deployed cell is a hanging
     * one. If it does not carry the adjacency flags, we are not able to inform
     * the worker about the order the neighbours are visited. This scenario is
     * an example why inheriting adjacency information is necessary.
     *
     * As a consequence, we exclusively fork cells that are not adjacent
     * to hanging nodes.
     */
    void updateCellsParallelStateAfterLoad(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
    ) const;

     /**
      * Handle root cell of remote tree
      *
      * <h2> Block erase on vertices </h2>
      *
      * Peano allows to block erase on vertices to avoid that mpi processes
      * starve. See Vertex::setAdjacentSubtreeForksIntoOtherRankFlag(). These
      * block flags are analysed bottom-up and are reset in each iteration,
      * i.e. we have to set them in each traversal again if a subtree forks
      * into another subtree.
      *
      * If a cell on level L is deployed to another rank, it is
      * sufficient to flag the vertices on level L-1 to avoid starvation. It
      * does make sense to flag the vertices on level L as well to avoid a
      * degeneration of remote workers, i.e. to avoid that a remote worker
      * holds the trivial tree with only one inner cell. This way, we preserve
      * a halo layer around each worker of at least depth one.
      *
      * <h2> MPI communication mode </h2>
      *
      * Depending on the flag SendMasterWorkerMessagesBlocking, Peano exchanges
      * the messages either blocking or non-blocking.
      */
     void updateCellsParallelStateAfterLoadForRootOfDeployedSubtree(
       State&                                    state,
       Cell&                                     fineGridCell,
       Vertex                                    fineGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              fineGridVerticesEnumerator,
       Cell&                                     coarseGridCell,
       Vertex                                    coarseGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
       const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
     ) const;

     void updateCellsParallelStateAfterLoadIfNodeIsJoiningWithMaster(
       State&                                    state,
       Cell&                                     fineGridCell,
       Vertex                                    fineGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              fineGridVerticesEnumerator,
       Cell&                                     coarseGridCell,
       Vertex                                    coarseGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
       const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
     ) const;

     /**
      *
      * If a domain is split up into two domains, this routine is invoked on
      * the worker. Peano's high level domain decomposition follows the
      * following principles:
      *
      * - In a first grid sweep, those cells on a rank are marked that shall
      *   go to another rank. From hereon we use the terms (future) master and
      *   worker though no data is actually transferred. The master remains
      *   responsible to call all events on its grid.
      * - The worker is informed that there will be a grid in the subsequent
      *   iteration.
      * - In the second grid sweep, the master runs into the grid entities.
      *   Now, it streams those directly to the worker and does not invoke
      *   any operations anymore. The responsibility is streamed to the new
      *   worker as well.
      * - As a consequence, the master can now delete its local data.
      * - The worker in parallel creates a completely new grid, i.e. it invokes
      *   all the creational events.
      * - While the worker runs through the grid for the very first time, it
      *   invokes this routine. For every newly created grid entity, it now
      *   invokes the merge routines mergeWithRemoteDataDueToForkOrJoin().
      *
      * As the routine is invoked by updateCellsParallelStateAfterLoad(), it
      * is called before enterCell() or leaveCell(). It is however called after
      * a createCell() events if invoking such an event is necessary.
      *
      * <h2> Heap data </h2>
      *
      * Heap data is not automatically administered by forks and joins. If you
      * create your heap indices correctly in the creational routines, then
      * your local cell data will hold valid heap indices (it has been created
      * before anything has been merged) and the remove cells/vertices hold
      * invalid heap indices. They are simple copied over from the other rank
      * but the indices used there usually have no meaning on your local rank.
      * So, you have to transfer all data manually in the events and merge it
      * into your local data structures manually, too. You can only rely that
      * the local data structures have been subject to createCell(),
      * createInnerVertex() or createBoundaryVertex() before.
      *
      *
      * @see updateCellsParallelStateAfterLoadIfNodeIsJoiningWithWorker() which
      *   is the counterpart of the present procedure but follows the same
      *   workflow/concepts.
      *
      * @see Page "Forks and Joins of Subtrees" for more details.
      */
     void updateCellsParallelStateAfterLoadForNewWorkerDueToForkOfExistingDomain(
       State&                                    state,
       Cell&                                     fineGridCell,
       Vertex                                    fineGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              fineGridVerticesEnumerator,
       Cell&                                     coarseGridCell,
       Vertex                                    coarseGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
       const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
     ) const;

     /**
      * Postprocessing routine invoked on the master if parts of its domain are
      * deployed to another rank.
      *
      * <h2> Rationale </h2>
      *
      * We switch a cell to remote if we have sent it to a worker and it is not
      * assigned to the local node anymore. However, we do this after the loop
      * running over all forking nodes, not within this loop. If we did it inside
      * the loop, we would run over all forking ranks. If a cell is adjacent to
      * rank k but will belong to rank k+1 from now on, we switch its state already
      * after the send to rank k. We could fix this if we compared to the assigned
      * rank rather than just asking whether it belongs to another rank. This
      * however would not solve the whole issue: If there is also a rank k+1
      * forking, this one again would receive an already destroyed cell.
      */
     void updateCellsParallelStateAfterLoadIfStateIsForking(
       State&                                    state,
       Cell&                                     fineGridCell,
       Vertex                                    fineGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              fineGridVerticesEnumerator,
       Cell&                                     coarseGridCell,
       Vertex                                    coarseGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
       const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
     ) const;

     /**
      * Join cell of worker with local cell
      *
      * Peano realises a two-step rebalancing of subpartitions: First, it flags
      * all cells that shall go from one rank to another. In the subsequent
      * grid sweep, the data then is actually transferred. In the first sweep
      * (where all the flagging happens), the responsibility for all data is
      * were the data did recide before the data movement, i.e. the rank where
      * the flagged data resides is the rank that triggers all the events. In the
      * second sweep, the data first is transferred to the destination rank and
      * this destination rank than has ownership. It is responsible to call all
      * events.
      *
      * The actual data transfer in the second phase of the rebalancing is
      * organised cell-wisely. It materialises in the present routine.
      *
      * <h2> High level data flow </h2>
      *
      * - The routine identifies whether there are joining ranks. If there are
      *   ranks but no data has been received from those guys, it creates a
      *   receive buffer.
      * - If any vertex of the present cell is adjacent to a joining/forking
      *   cell, we have to exchange some data. The code does exchange join data
      *   in an overlapping sense, i.e. also cells are exchanged that are
      *   adjacent but not owned by a joining rank. Those cells do not carry
      *   information as we work non-overlappingly, but we exchange them to
      *   ensure the grid remains consistent.
      * - We take the cell and invoke mergeWithJoinedCellFromWorker().
      * - This routine sets all the cell attributes but notably tells us
      *   whether the join forces us to create a new inner cell on the local
      *   rank: This happens (a) if there has not been such a cell on the local
      *   rank before or (b) if there has been a cell on the local rank but it
      *   had been flagged as remote/outside. In both cases, we call
      *   createCell() and then invoke switchToInside().
      * - Finally, we call mergeWithRemoteDataDueToForkOrJoin().
      *
      * So mergeWithRemoteDataDueToForkOrJoin() is always called after a
      * createInnerCell() if the cell is inside the domain.
      *
      * From hereon, we handle the adjacent vertices.
      *
      * <h2> Determine which vertices to exchage </h2>
      *
      * In Peano, a merge exchanges only vertices that are adjacent to the
      * worker's domain. For this, the worker determines those vertices and
      * sends them to the master. The master in turn can identify vertices that
      * are adjacent to a joining worker, too, and trigger the corresponding
      * receives.
      *
      * For the fork mechanism, the master sends the worker a bitset
      * determining which vertices are exchanged. Throughout the fork, we
      * cannot identify the vertices to be exchanged on both the worker and
      * the master. Here, only the master knows.
      *
      * For joins, it is different. Here, we have consistent information on
      * both worker and master - even if vertices are remote:
      *
      * @image html peano/grid/nodes/Node_updateCellsParallelStateAfterLoadIfNodeIsJoiningWithWorker.png
      *
      * - The blue cells all are held by rank 7.
      * - All red point are refined. We study the bright red point from now on.
      * - Rank 7 forks the dark blue subpartition to rank 14.
      * - As a result, the bright red point switches to erase. But it remains
      *   refined on the new worker 14.
      * - The red adjacency list next is not set to [-1,-1,-1,-1] though it is
      *   remote. Cf. Vertex::eliminateAllAdjacencyInformationThatIdentifiesNeitherWorkerNorMaster()
      *   that explicitly preserves the adjacency information as 14 is a worker
      *   of 7.
      * - If 14 makes 7 join, all important adjacency information is at hand.
      */
     void updateCellsParallelStateAfterLoadIfNodeIsJoiningWithWorker(
       State&                                    state,
       Cell&                                     fineGridCell,
       Vertex                                    fineGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              fineGridVerticesEnumerator,
       Cell&                                     coarseGridCell,
       Vertex                                    coarseGridVertices[FOUR_POWER_D],
       const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
       const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
     ) const;

    /**
     * Counterpart of updateCellsParallelStateAfterLoad(), i.e. the one
     * operation where we receive data from the workers if the algorithm
     * shall receive data from workers.
     *
     * Indeed, data from the worker is received if and only if
     * - the reduction is switched on,
     * - the node is not joining, and
     * - the node has not just forked.
     *
     * The reason for the latter is a technical one: To setup the worker's
     * grid, Peano needs the whole cell and cell marker information, i.e. it
     * has to know how to run through the @f$ 3^d @f$ cells on the local
     * spacetree level one. This information also requires that the master
     * runs through the whole grid. However, the master in-between requires
     * the worker to send it worker's data. So, we have to switch off this
     * reduction to remove a deadlock.
     *
     * !!! Order of messages
     *
     * For the startup process, we first send the state, then the cells, and,
     * finally, the vertices. This does make sense: The Grid reads the state,
     * as the state is important even before we load the first data from the
     * input stream. The cells and the vertices remain in the MPI queue. Root
     * then finally takes this data from the MPI queue.
     *
     * The reduction process is the other way round: Root sends away the cell
     * and its adjacent vertices. Only afterwards, we send away the state.
     */
    void updateCellsParallelStateBeforeStore(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
    ) const;

    /**
     * Update of setAdjacentSubtreeForksIntoOtherRankFlag
     *
     * The coarse grid vertices of a deployed cell have to hold the flag
     * adjacentSubtreeForksIntoOtherRankFlag. Conceptionally, we should set
     * this flag in this routine. However, if we switch off the reduction,
     * the operation never is called. Thus, I moved the update of the respective
     * flag to the startup operation.
     *
     * !!! MPI communication mode
     *
     * Semantically, the receive of worker data is a synchronous data exchange.
     * It hence is straightforward to realise it with a blocking mpi call. If
     * done that way, Peano however hangs. The reason is that - at least on the
     * global master - load balancing requests have to be handled in the
     * background. Rank 0 is by construction underbooked. When it waits for the
     * workers, these are still running through their subdomains and triggering
     * load balancing requests. These requests have to be handled.
     *
     * A similar reasoning holds for any rank if heap data is exchanged. Soon,
     * any blocking receive breaks down and makes the application hang. The
     * receive hence is non-blocking always.
     */
    void updateCellsParallelStateBeforeStoreForRootOfDeployedSubtree(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
    ) const;

    /**
     * Get flag for a cell identified by verticesEnumerator.
     *
     * This operation runs throught the adjacent vertices and sets the right
     * height. See SingleLevelEnumerator::updateAdjacentCellsFlag().
     *
     * This operation has to be called before traverse() is invoked on a child
     * cell. The load operation might trigger some immediate refinements in a
     * neighbouring cell (see documentation of
     * peano::grid::nodes::loops::StoreVertexLoopBody::invalidateCoarseGridTreeHeightAttributesIfRefined())
     * and this refinement might change the tree information of this cell. So,
     * updating the flags after the load is not an option - it has to happen
     * before the descend.
     *
     * <h2> Persistent subtrees </h2>
     *
     * If we hold persistent subtrees, we may not allow the code to handle
     * subtrees as regular if all adjacent vertices hold the flag that
     * indicates that they parent a persistent tree. This situation can arise
     * if the majority of a grid region is held persistently and then some
     * refinement kicks in. Then the whole thing could be held as one
     * persistent big tree. However, if we set the corresponding tree, the
     * automaton descends and things everything is persistent, while some
     * of it is already outsourced to other containers. We overwrite this
     * information and the code crashes.
     *
     * <h2> Parallelisation </h2>
     *
     * If the oracle of the local node is told to fork, we always invalidate
     * the enumerator's optimisation flags, as this command means that we
     * should try to fork and not do any static optimisation. So, when a fork
     * has failed on one node, this node stops to fork on this node.
     *
     * @param vertices in   Vertices adjacent to current cell identified by
     *                      enumerator
     * @param verticesEnumerator inout The enumerator's spatial data is not
     *                      modified, but the static tree properties are
     *                      overwritten
     */
    void updateRefinedEnumeratorsCellFlag(
      const State&            state,
      const Vertex            vertices[FOUR_POWER_D],
      SingleLevelEnumerator&  verticesEnumerator
    ) const;
  public:
};



#include "peano/grid/nodes/Node.cpph"

#endif
