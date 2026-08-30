// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_VERTEX_H_
#define _PEANO_GRID_VERTEX_H_


#include "tarch/la/Vector.h"
#include "peano/utils/PeanoOptimisations.h"
#include "peano/grid/VertexEnumerator.h"
#include "peano/grid/State.h"


#include <set>


namespace peano {
  namespace grid {
      template <class VertexData>
      class Vertex;

      namespace tests {
        class SetCounterTest;
      }

      namespace aspects {
        class ParallelMerge;
      }
    }

    namespace parallel {
      template <class T>
      class AdjacencyListAspect;
  }
}



/**
 * This class is the base class of all spacetree grid vertices, i.e. the
 * application's vertex class extends this class.
 *
 * @author Tobias Weinzierl
 */
template <class VertexData>
class peano::grid::Vertex {
  public:
    /**
     * This class exists to allow Peano to overload the standard constructor.
     */
    class DoNotCallStandardConstructor {};

    #if defined(StoreOnlyPersistentAttributes) && defined(PackedRecords)
    typedef typename VertexData::Packed::PersistentRecords  PersistentVertex;
    #endif

    #if !defined(StoreOnlyPersistentAttributes) && defined(PackedRecords)
    typedef typename VertexData::Packed                     PersistentVertex;
    #endif

    #if defined(StoreOnlyPersistentAttributes) && !defined(PackedRecords)
    typedef typename VertexData::PersistentRecords          PersistentVertex;
    #endif

    #if !defined(StoreOnlyPersistentAttributes) && !defined(PackedRecords)
    typedef VertexData                                      PersistentVertex;
    #endif

    #ifdef Parallel
    /**
     * This operation is robust, i.e. you can call it several times.
     */
    static void initDatatype();
    static void shutdownDatatype();

    friend class peano::parallel::AdjacencyListAspect<Vertex<VertexData> >;
    #endif

    #if defined(ParallelExchangePackedRecordsBetweenMasterAndWorker)
    typedef typename VertexData::Packed       MPIDatatypeContainer;
    #else
    typedef VertexData                        MPIDatatypeContainer;
    #endif

    typedef VertexData                                      Records;

    friend class peano::grid::tests::SetCounterTest;
    friend class peano::grid::aspects::ParallelMerge;

  protected:
    VertexData _vertexData;

    /**
     * Standard constructor.
     *
     * The created vertex by default is an outer vertex, and it is not
     * persistent, i.e. a hanging node.
     *
     * !!! Asserts mode
     *
     * Usually, all adjacent ranks are set by a create event in 
     * LoadVertexLoopBody. It is either create for the hanging 
     * nodes or they are created as persistent nodes and afterwards taken 
     * from the input stream. The thing is slightly different for the 
     * 'hanging' nodes at the boundary of the whole spacetree. Those 
     * guys always are outside and they are never created. To avoid 
     * confusion (which seems to pop up only for hand-written integer 
     * vectors), I thus initialise all vertices with the right adjacent 
     * rank information. 
     */
    Vertex();

    Vertex(const PersistentVertex& argument);

    Vertex(const DoNotCallStandardConstructor&);

  public:
    ~Vertex();

    /**
     * Proxy.
     */
    std::string toString() const;

    /**
     * Proxy.
     */
    void toString(std::ostream& out) const;

    /**
     * A freshly generated vertex is a hanging node. You can however switch its
     * state to non-hanging with this operation.
     */
    void switchToNonhangingNode();

    /**
     * Is vertex outside, i.e. can we perhaps skip to invoke any events?
     */
    bool isOutside() const;
    bool isBoundary() const;
    bool isInside() const;

    /**
     * A node is considered to be hanging if not all @f$ 2^d @f$ adjacent
     * cells exist.
     */
    bool isHangingNode() const;

    /**
     * Make vertex be inside or on the boundary of the domain.
     *
     * Means that the vertex either is inside or at the boundary of the domain.
     * This implies that the events for this vertex have to be called and that
     * there is at least one adjacent cell inside the domain, too.
     *
     * @image html peano/grid/geometry-vertex-inside-outside.png
     *
     * !!! Assertions
     *
     * Before we switch, we validate the following:
     *
     * - The vertex should be boundary or outside. There's no reason to switch
     *   an inner vertex to inside.
     * - If the vertex is a hanging vertex, we may switch to inside always.
     *   However, the underlying tree height attributes (adjacent cell heights)
     *   then have to be invalid.
     * - Due to the switch, the adjacent cell heights should be invalidated.
     *
     * The last issue is interesting: If we switch a vertex, the geometry
     * before has told the algorithm that this part of the domain will
     * change. At this time, the algorithm already has set the whole data
     * in the tracked region to instationary. Hence, when this operation is
     * called, everything already has been set to instationary due to
     * invalidateAdjacentCellInformation().
     */
    void switchToInside();

    /**
     * Make vertex be outside the domain.
     *
     * Means that the vertex is outside and this implies that the events for
     * this vertex are not called anymore. Furthermore, none of the adjacent
     * cells is inside the domain, too.
     *
     * @image html peano/grid/geometry-vertex-inside-outside.png
     *
     */
    void switchToOutside();

    /**
     * Well, that works always.
     */
    void switchToBoundary();

    typename Records::RefinementControl getRefinementControl() const;


    /**
     * @return The persistent subattributes of the vertex.
     */
    PersistentVertex getRecords() const;


    /**
     * Refine Vertex
     *
     * Refine all adjacent elements along every axis. This operation may not
     * called on a vertex that does not correspond to fine grid vertex, i.e.
     * check whether a vertex has the state unrefined prior to invoking this
     * operation.
     *
     * Peano will conduct the refinement
     *
     * - immediately if the vertex is not adjacent to a parallel domain
     *   boundary and if the refinement is triggered in a touch first event.
     *   Otherwise,
     * - Peano bookmarks your refinement wish and refines the grid in the
     *   subsequent traversal.
     *
     * The picture below illustrates the parallel behaviour for a vertex that
     * is shared between two ranks (red and green). It is obvious that we may
     * never refine immediately in this case. If we did, the grid would become
     * inconsistent as the green rank would lack behind.
     *
     * @image html peano/grid/Vertex_enforceRefine.png
     *
     * In parallel codes, this can lead to a very slow grid setup, as each
     * additional level in the grid requires one grid sweep. Please see the
     * operation enforceRefine() for an alternative.
     *
     * Please note that the above definition clarifies that Peano tries to
     * refine immediately. This terminology implies a pitfall: Users typically
     * initialise data within createCell(), createInnerVertex() and
     * createBoundaryVertex(). Let a mapping M1 embedded into an adapter A1
     * trigger a refinement. Whenever a grid sweep with adapter A1 has
     * terminated, the user switches to an adapter A2 which does not include
     * M1 but another mapping M2. Then, both M1 and M2 have to realise the
     * creational routines as you cannot be sure a priori whether the grid
     * refinement is realised directly in the run of A1 or in the subsequent
     * sweep.
     *
     */
    void refine();

    /**
     * Counterpart of refine().
     *
     * The illustartion below is out-of-date as coarse now is called erase.
     * However, it still illustrates the right process.
     *
     * @image html peano/grid/erase-process.png
     *
     * An erase becomes nop if it is done for a vertex belonging to an
     * instationary or forked subpartition, i.e. parallelisation and refinement
     * have a higher priority than making the grid coarser.
     *
     * !!! Parallel
     *
     * In the parallel mode, it can happen that Peano does not erase the grid
     * if this would induce a starvation effect. If this happens, Peano rolls
     * back the erase (i.e. the vertex is reset to refined). However, it
     * informs the load balancing that it wasn't able to perfom some erases.
     * So, the load balancing might react to such an erase call and join
     * partitions. If you then call erase again, it might go through.
     */
    void erase();

    /**
     * Rollback any triggered operation.
     *
     * This operation is provided for the load vertex process. We recommend not
     * to use it for any PDE solver.
     *
     * It is invoked by the load process for all vertices that will be
     * destroyed after this iteration. As a consequence, we may not invoke this
     * operation if erase processes are disabled on this vertex. Otherwise, the
     * data consistency is harmed: Such a flag has to propagate bottom-up in the
     * tree and thus avoid on a coarser level that an erase takes place at all.
     *
     * @see setAdjacentSubtreeForksIntoOtherRankFlag().
     */
    void rollbackRefinementOrEraseTriggered();

    enum class SwitchRefinementTriggeredResult {
      Nop, SwitchedIntoRefining, WasntAbleToSwitchDueToRegularPersistentSubgrids
    };

    /**
     * Switch State's Refinement Triggered into Refining
     *
     * This operation reads the refinement flag. If it is set to
     * refinement triggered, the operation changes refinement triggered into
     * refining. This operation does not analyse the parallel state of the
     * vertex.
     *
     * If the vertex's refinement state is not equal to refinement triggered,
     * the operation becomes nop.
     *
     * @return Has done a transition.
     */
    SwitchRefinementTriggeredResult switchRefinementTriggeredToRefining();

    /**
     * @see switchEraseTriggeredToErasing()
     */
    enum class SwitchEraseTriggeredResult {
      Nop, SwitchedIntoErasing, WasntAbleToSwitchDueToFork, WasntAbleToSwitchDueToRegularPersistentSubgrids
    };

    /**
     * Switch State's Erase Triggered into Erasing
     *
     * This operation reads the refinement flag. If it is set to
     * erase triggered, the operation changes erase triggered into
     * erasing. This operation does not analyse the parallel state of the
     * vertex.
     *
     * If the vertex's refinement state is not equal to erase triggered,
     * the operation becomes nop.
     *
     * !!! Parallel mode
     *
     * In parallel, a vertex may not become erasing, if any of the adjacent
     * subtrees forks into another ranker. Otherwise, we would observe
     * starvation, i.e. workers running out of work in one step. To avoid
     * this, we do not switch to erasing but instead return a flag that
     * indicates that we are in that special case.
     *
     * If the code decides to join workers afterwards, it might happen that
     * the trigger passes through later.
     *
     * The flag stopping the erase process always is unset as this operation
     * is called only once per traversal per vertex. If erases shall be
     * stopped, the corresponding code parts will reset the flag throughout
     * the traversal again.
     *
     * !!! Vertices adjacent to @f$ 2^d @f$ remote cells
     *
     * Vertices that are adjacent to @f$ 2^d @f$ are erased by
     * StoreVertexLoopBody, as they are remote and all remote vertices are
     * erased. While the latter statement is fine in principle, we wanna keep
     * the coarsest vertices that correspond to a remote tree refined on the
     * master refined, too. This later makes the joins simpler, as they then
     * implicitly hold information how the sfc wraps around the remote cell.
     * This operation thus does not erase such vertices.
     *
     * It also does not tell the calling operation that it keeps an erase
     * triggered. If it returned WasntAbleToSwitchDueToFork, each rank
     * forking often would report its master that it wasn't able to fork
     * due to the domain decomposition in each traversal. That would be
     * wrong - it has nothing to do with the decomposition but there are
     * technical reasons tied to the sfc.
     *
     * @return Has done a transition.
     */
    SwitchEraseTriggeredResult switchEraseTriggeredToErasing();

    /**
     * Switch refinement flags
     *
     * There are two operations that indicate a transient state: refining and
     * erasing. These states are set whenever the code reads data with a
     * triggered state from the input stack to indicate that now the vertex
     * really is refined. Before somebody writes back such vertices to the
     * output stacks, it has to switch refining to refined or erasing to
     * unrefined, respectively. This operation does this.
     */
    void updateTransientRefinementFlagsBeforeVertexIsStoredToOutputStack();

    /**
     * If we just use Peano's refinement facilities, the following scenario
     * often occurs: Two patches are refined and spread in space. At some
     * point, they becomes adjacent to each other. However, the coarse
     * vertices along the boundary are not refined. And they won't be refined
     * as the solution is rather smooth along the patch interface. However,
     * these hanging node pollute the solution. Consequently, I introdcued a
     * counter for each node that counts how many of the adjacent cells are
     * refined. Before I store the vertex, it is checked whether this counter
     * equals @f$ 2^d @f$ and whether the vertex is unrefined. In this case, I
     * trigger a refinement.
     *
     * @image html peano/grid/hanging-nodes-in-patches.png
     */
    void clearCounterOfAdjacentRefinedCells();

    /**
     * Save and clear adjacency information
     *
     * Adjacency information is kind of state information about the surrounding
     * of a vertex. It is analysed per traversal. This routine takes the
     * adjacency information and backs it up into a non-persistent field. From
     * hereon, getAdjacentCellsHeightOfPreviousIteration() allows access to the
     * grid state information, while a new flag (accessible through
     * getCurrentAdjacentCellsHeight(), e.g.) is used to analyse the current
     * grid's state.
     *
     * The routine is consequently invoked per vertex load.
     *
     * <h2> Marker semantics </h2>
     *
     * For details on the marker semantics, please consult
     *
     * @see peano::grid::CellFlags
     *
     * <h2> Implementation </h2>
     *
     * - Store the current adjacency information into the field holding the last iteration's record.
     * - If the vertex is unrefined, set current height to zero.
     * - Else if the vertex is not stationary, set the current height to instationary.
     * - If the vertex is adjacent to a parallel domain boundary (only in parallel mode), invalidate vertex flag.
     *
     * This operation is called by vertex load processes, i.e. by
     * LoadVertexLoopBody and by its regular counterpart. The flag afterward
     * is updated (downgraded) by the grid manipulation routines erase and
     * refine, or it is analysed throughout the steps up. As a consequence it
     * might always still be downgraded as long as the vertex is not written
     * to the output stream.
     *
     * This operation has primarily impact on the semantics of
     * StoreVertexLoopBody::updateCoarseGridTreeHeightAttributes()
     * where the adjacency information is restricted bottom-up in the
     * spacetree. The routine uses setCurrentAdjacentCellsHeight().
     * In the regular grid counterpart, we find the update of the vertex state
     * in CallTouchVertexLastTimeLoopBodyOnRegularRefinedPatch::performVertexTransition()
     * while the invalidation is done in LoadVerticesOnRegularRefinedPatch::loadVerticesOfOneCellAtBoundaryofSubtree().
     *
     * <h2> Parallelisation remarks </h2>
     *
     * The bottom-up vertex state analysis does not really work along parallel
     * boundaries: Those vertices are sent away at the end of the traversal and
     * their counterpart is not received before the subsequent traversal. As a
     * result, we cannot say anything at the end of one traversal about the
     * regularity of the subtree in the next traversal. As a consequence, all
     * parallel boundary vertices are set to invalid.
     */
    void saveAndClearAdjacentCellsInformation();

    /**
     * @see saveAndClearAdjacentCellsInformation()
     */
    peano::grid::CellFlags getCurrentAdjacentCellsHeight() const;

    /**
     * @see saveAndClearAdjacentCellsInformation()
     */
    peano::grid::CellFlags getAdjacentCellsHeightOfPreviousIteration() const;

    /**
     * @see saveAndClearAdjacentCellsInformation()
     */
    void setCurrentAdjacentCellsHeight(peano::grid::CellFlags value);

    /**
     * Invalidate all the adjacency information
     *
     * The operation erases all the information about the heights and
     * invariancies of adjacent trees. If you invalidate this information,
     * you switch off the static tree optimisations and the recursion
     * unrolling manually.
     *
     * This is for example done due to a merge on the worker side. Implicitly,
     * any erase() or refine() call invalidates as well, so usually it is not a
     * PDE task to invalidate any vertex anytime.
     */
    void invalidateAdjacentCellInformation();

    /**
     * @see clearCounterOfAdjacentRefinedCells()
     */
    void incCounterOfAdjacentRefinedCells();

    /**
     * Refine Vertex if surrounded by refined cells
     *
     * We do check explicitly whether to a vertex is surrounded only by refined
     * cells. If that is the case, the grid manually refines it. Otherwise, it
     * can happen that there is a hanging vertex (on the next finer level) within
     * a @f$ 7^d @f$ patch.
     *
     * !!! Parallel Mode
     *
     * The important exception to that rule is remote vertices. If a vertex is
     * surrounded by cells deployed to another subtree, we should not refine it
     * manually. If we did, we would introduce a vertex oscillation. The vertex
     * is refined, but in the next iteration, it is erased again as remote
     * refined vertices always are erased.
     *
     * @see clearCounterOfAdjacentRefinedCells()
     * @see StoreVertexLoopBody::updateRefinementFlagsAndStoreVertexToOutputStream()
     */
    void refineVertexIfItHasOnlyRefinedAdjacentCells();

    #ifdef Asserts
    /**
     * Only available if compiled with -DAsserts
     *
     * Returns the position of the vertex in assertion mode, i.e. the operation is
     * not available in the release mode. If you need the vertex's position
     * outside an assertion guarded by a ifdef Asserts, you have to recalculate
     * the position manually.
     */
    tarch::la::Vector<DIMENSIONS,double> getX() const;

    int getLevel() const;

    /**
     * Set the vertex's position
     *
     * This position is stored persistently only if we are in debug mode. Here,
     * it acts as validation field.
     */
    void setPosition( const tarch::la::Vector<DIMENSIONS,double>&  x, int level );
    #endif

    /**
     * Enforce an immediate refine.
     *
     * This operation refines immediately if called by a creational event or
     * touchVertexFirstTime(). It thus equals refine() in a serial code. In a
     * parallel code, refine() always postpones the refinement along boundaries.
     * See refine()'s documentation and the picture there for details.
     * enforceRefine() does not postpone the refinement. Thus, you have to
     * ensure yourself that the grid remains consistent, i.e. all ranks holding
     * a copy of this vertex call enforceRefine() in the same grid sweep.
     *
     * <h2> Semantics without MPI </h2>
     *
     * I have introduced enforceRefine for MPI codes to allow them to refine
     * without any communication with neighbour ranks. Without such a routine, a
     * code can add at most one grid level per grid sweep. In a serial code,
     * enforceRefine() does not make sense - whenever refine is triggered in
     * touchVertexFirstTime() or a creational event, the refine is done
     * immediately. Nevertheless, I do support enforceRefine() in a serial
     * code. It however just delegates to refine().
     */
    void enforceRefine();

    #ifdef Parallel
    /**
     * Set flag that blocks erases.
     *
     * The flag ist unset automatically when you evaluate it, i.e.
     * when you call switchEraseTriggeredToErasing().
     *
     * !!! Who sets the flag
     *
     * - The flag is propagated bottom-up in the gree as analysed tree grammar.
     *   If a vertex holds the flag, all its parent vertices hold it too after
     *   the traversal. This way, we avoid that coarser vertices eliminate
     *   vertices that may not be erased, i.e. hold the present flag. See also
     *   rollbackRefinementOrEraseTriggered().
     * - Node::updateCellsParallelStateAfterLoadForRootOfDeployedSubtree() sets
     *   the flag in each iteration to avoid that a master coarses away its
     *   worker (starvation). See the operation for details.
     * - nodes::Refined::splitUpGrid() sets the grid. Usually, the flag is set
     *   by the master when it starts up the worker. However, this set
     *   mechanism is not active yet when we trigger the fork. See the
     *   operation for details.
     * - StoreVertexLoopBody::exchangeVerticesDueToParallelisation() sets the
     *   flag manually when the node is preparing for a join with its master.
     *   This way, it avoids too harsh grid changes around the region
     *   throughout the join.
     * - ParallelMerge::mergeWithJoinedVertexFromWorker() sets the flag to
     *   give the master one additional traversal where no adjacent grid
     *   structure does change. A traversal to recover from the grid changes.
     * - ParallelMerge::mergeWithVertexFromMaster() sets the flag. This is the
     *   counterpart of Node::updateCellsParallelStateAfterLoadForRootOfDeployedSubtree().
     *   We also could copy the flag from there where it has been set before.
     *   However, it is important that it is set to avoid the local events to
     *   erase the root of the worker tree.
     *
     *
     * !!! Starvation
     *
     * For a detailed discussion of starvation, i.e. erases that make masters
     * loose their workers, see ParallelMerge::mergeWithVertexFromMaster().
     */
    void  setAdjacentSubtreeForksIntoOtherRankFlag();

    /**
     * Self explaining. See remarks on starvation in the code why we need this.
     *
     * @see setAdjacentSubtreeForksIntoOtherRankFlag().
     */
    bool  isAdjacentSubtreeForksIntoOtherRankFlagSet() const;


    /**
     * Is vertex remote?
     *
     * A vertex is remote, if all its adjacent cells are handled by another
     * rank. However, this rank may not have the attribute fork-triggered
     * (because then it does not yet exist) or joining (because then it is
     * already forwarding its work to its master), or forking. The latter
     * case means that the rank is just about to forward all vertices to the
     * new worker, i.e. it does not compute anything anymore on the local
     * vertex, but it still has to do the send/receive stuff, i.e. it still
     * has to handle the vertices.
     *
     * !!! Optimisation
     *
     * Instead of checking always first all the ranks, and then check once
     * again the state and its joining and fork-triggered ranks, I would like
     * to analyse the state once when I load the vertex and set the remote
     * flag. Unfortunately this does not work out, as the forks might occur
     * later, i.e. the remote() state might change in time.
     */
    template <class State>
    bool isRemote(
      const State&  state,
      bool          considerForkingNodesToBeRemote,
      bool          considerJoiningNodesToBeRemote
    ) const;

    /**
     * This operation is used by the send and receive buffers and should not
     * be used by other components of Peano.
     */
    void setVertexData(const VertexData& vertexData);

    /**
     * This operation is used by the send and receive buffers and should not
     * be used by other components of Peano.
     */
    VertexData getVertexData() const;

    /**
     * Changes the adjacency lists
     *
     */
    void setAdjacentRanks( const tarch::la::Vector<TWO_POWER_D,int>& ranks );

    /**
     * Modify one adjacency entry
     *
     * Should probably be visible only to the kernel's parallel namespace. This
     * operation changes one entry of the adjacency lists. This change will not
     * be visible immediately. Instead, it will take effect only in the next
     * traversal.
     */
    void setAdjacentRank( int position, int rank );

    /**
     * Return set of adjacent ranks
     *
     * Returns the set of adjacent remote ranks (the own rank is not included).
     * Please do not call this operation for vertices that do not belong to the
     * parallel boundary, i.e. call it only for vertices for which
     * isAdjacentToRemoteRank() does hold. If you take the size of the result
     * and add one, you know how often this vertex globally does exist.
     *
     * @return Ranks of nodes that do hold a copy of the vertex besides the
     *         local rank.
     */
    std::set<int> getAdjacentRemoteRanks() const;

    /**
     * List of adjacent ranks
     *
     * This operation returns the ranks that are responsible for the
     * @f$ 2^d @f$ adjacent cells. It is not defined on vertices that are not
     * part of a parallel domain boundary (see isAdjacentToRemoteRank()). The
     * result is ordered along the z-curve, i.e. the very first entry is the
     * rank of the node that is responsible for the left bottom adjacent cell
     * (for d=2). The next entry corresponds to the right bottom cell, and so
     * forth.
     *
     * This operation was written for the grid management. For PDE-solvers,
     * please use getAdjacentRemoteRanks() instead.
     *
     * @return Ranks that are responsible for adjacent cells.
     */
    tarch::la::Vector<TWO_POWER_D,int> getAdjacentRanks() const;

    /**
      * Blocking send. initDatatype() has to be called before.
      */
    void send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallel, typename MPIDatatypeContainer::ExchangeMode exchangeDataBlocking );

    /**
      * Blocking receive. initDatatype() has to be called before.
     */
    void receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallel, typename MPIDatatypeContainer::ExchangeMode exchangeDataBlocking );

    /**
     * Does Vertex Belong to Parallel Boundary
     *
     * Analyses the adjacency list. If there is one entry that is not equal to
     * the local node's rank, then this attribute holds.
     *
     * This definition is important throughout forks for existing domains.
     * Here, adjacency lists are updated and it might happen that a vertex
     * belongs not remote, but this flag nevertheless still holds.
     */
    bool isAdjacentToRemoteRank() const;

    bool isAdjacentToDomainOf( const std::set<int>& ranks ) const;
    bool isAdjacentToDomainOf( int rank ) const;

    void replaceAdjancyEntriesOfLocalRankWithMaster();
    void replaceAdjancyEntriesOfRemoteRankWithLocalRank(int rank);

    static tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int> readAdjacencyLists(
      const peano::grid::VertexEnumerator&  enumerator,
      const Vertex* const                   vertices
    );

    void eliminateAllAdjacencyInformationThatIdentifiesNeitherWorkerNorMaster();
    #endif

    /**
     * Returns true if we have Refining,EraseTriggered,Erasing,RefineDueToJoinThoughWorkerIsAlreadyErasing,
    EnforceRefinementTriggered
     *
     */
    bool  isRefinedOrRefining() const;

    /**
     * The parenting flag is a flag that is available if and only if you
     * compile with -DnoPersistentRegularSubtrees. The getter allows you
     * to identify those vertices that
     *
     * - are adjacent to the root of a persistently stored regular subtree or
     * - are a (recursive) parent or parent of the parent or ...
     *
     * If the flag is set, a vertex should not be refined. Furthermore, we
     * may not handle a node as regular tree if all $2^d$ adjacent vertices
     * hold the flag.
     *
     * @see switchEraseTriggeredToErasing()
     * @see switchRefinementTriggeredToRefining()
     * @see updateRefinedEnumeratorsCellFlag()
     */
    bool isParentingRegularPersistentSubgrid() const;
    bool isParentingRegularPersistentSubgridInNewSweepAlreadySet() const;

    /**
     * Set the persistency flag.
     *
     * @see isParentingRegularPersistentSubgrid()
     */
    void setIsParentingRegularPersistentSubgridFlag();
};


template <class VertexData>
std::ostream& operator<<(std::ostream& out, const peano::grid::Vertex<VertexData>& vertex);


#include "peano/grid/Vertex.cpph"

#endif
