// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_STATE_H_
#define _PEANO_GRID_STATE_H_


#include "tarch/logging/Log.h"
#include "peano/utils/PeanoOptimisations.h"


#include <set>
#include <map>


namespace peano {
  namespace grid {
    template <class StateData>
    class State;
  }
}



/**
 * Super class for all states of the grid.
 *
 * !!! Remarks on the protected attribute _stateData
 *
 * The state class or its pde-specific specialisation, respectively, are
 * basically semantic wrappers around the a records class that holds the
 * state properties. This records class is the attribute _stateData. The
 * fields within this class can be classified into two categories: Peano
 * attributes and pde-specific attributes. Please note that I highly
 * recommend to modify the pde-specific attribute only and not to modify
 * any attributes Peano does provide. If you have to modify those attributes,
 * use exclusively the modificators of the present class and study their
 * comments carefully.
 *
 * !!! Parallel state exchange
 *
 * The state is always copied from the master to the worker unless you use a
 * differing communication specification. It is then merged back into the
 * master through mergeWithWorkerState(). This way, the master has the full
 * overview how many vertices have been processed, how many cells there are
 * in the domain, whether a refinement has been triggered and so forth. Again,
 * please study your communciation specification carefully to identify how
 * 'reliable' all these statistics are.
 *
 * Prior to the iteration, each worker calls resetStateAtBeginOfIteration().
 * This is called on the copy received from the master. At the end of the
 * traversal, i.e. after endIteration(), we call resetStateAtEndOfIteration().
 * It is called by Grid::iterate() as one of the very last actions, i.e. way
 * after the state has been sent up to the master. See both operations for
 * details information what they do.
 *
 * @author Tobias Weinzierl
 */
template <class StateData>
class peano::grid::State {
  public:
    #if defined(StoreOnlyPersistentAttributes) && defined(PackedRecords)
    typedef typename StateData::Packed::PersistentRecords  PersistentState;
    #endif

    #if !defined(StoreOnlyPersistentAttributes) && defined(PackedRecords)
    typedef typename StateData::Packed                     PersistentState;
    #endif

    #if defined(StoreOnlyPersistentAttributes) && !defined(PackedRecords)
    typedef typename StateData::PersistentRecords          PersistentState;
    #endif

    #if !defined(StoreOnlyPersistentAttributes) && !defined(PackedRecords)
    typedef StateData                                      PersistentState;
    #endif

    #ifdef Parallel
    /**
     * This operation is robust, i.e. you can call it several times.
     */
    static void initDatatype();
    static void shutdownDatatype();
    #endif

    #if defined(ParallelExchangePackedRecordsBetweenMasterAndWorker)
    typedef typename StateData::Packed       MPIDatatypeContainer;
    #else
    typedef StateData                        MPIDatatypeContainer;
    #endif

    typedef StateData                                      Records;

  protected:
    StateData           _stateData;

    State();
    State(const PersistentState& argument);
  private:
    /**
     * Logging device
     */
    static tarch::logging::Log  _log;

    static const int IterationsInBetweenRebalancing;

    enum LoadBalancingState {
      NoRebalancing,
      ForkTriggered,
      Forking,
      JoinTriggered,
      Joining,
      JoinWithMasterTriggered,
      JoiningWithMaster,
      HasJoinedWithMaster,
      IsNewWorkerDueToForkOfExistingDomain
    };

    static std::string toString(LoadBalancingState value);

    #ifdef Parallel
    /*
     * So, this flag either holds all the joining ranks or all the
     * forking ranks.
     */
    std::set<int>       _loadRebalancingRemoteRanks;
    LoadBalancingState  _loadRebalancingState;

    /**
     * Stores for each state whether to reduce or not. We reset it
     * at the beginning of each traversal.
     */
    std::map<int,bool>  _stateWillReduce;

    /**
     * We may fork/join only every third iteration.
     *
     * The reason for this is tricky: Lets study two vertices a and
     * b. Our master M node holding both forks into M and W and, in
     * the same iteration, triggers a refine for a. b remains refined as it
     * has been refined before. That is iteration 1.
     *
     * In iteration 2, a and b both are streamed to the new worker W. At the
     * same time, M erases b as b has been streamed to W and is not needed on
     * M anymore. It however cannot trigger erase for vertex a, as a is set to
     * refining and we cannot both refine and simultaneously erase a vertex.
     *
     * In iteration 3, finally vertex a is erased as well, i.e. it is set to
     * erase-triggered an in iteration 4 it switches to erasing.
     *
     * So far, everything works fine. Problems arise if already in iteration 3,
     * the master decides to join the worker again. In this case, data is
     * streamed from W back to M in iteration 4. Now, the vertex a is set
     * refined on the worker but set to erasing on the master. And this is a
     * situation we cannot handle as an erase probagates over multiple levels
     * and makes the data inconsistent.
     *
     */
    int                          _iterationCounter;

    /**
     * @see mayForkCellsOnLevel()
     */
    int                          _maxForkLevel;

    bool                         _currentlyRunsMultipleIterations;

    #ifdef Asserts
    LoadBalancingState           _previousLoadRebalancingState;
    #endif

    #endif
  public:
     ~State();

    /**
     * Proxy.
     */
    std::string toString() const;

    /**
     * Proxy.
     */
    void toString(std::ostream& out) const;

    /**
     * @see getMaximumMeshWidth() for comments on the validity of the result
     */
     double getNumberOfInnerVertices() const;

    /**
     * @see getMaximumMeshWidth() for comments on the validity of the result
     */
     double getNumberOfBoundaryVertices() const;


     /**
      * @see getNumberOfOuterCells() for a discussion on the validity of the
      *        result if the code runs with mpi.
      */
     double getNumberOfOuterVertices() const;

    /**
     * @see getMaximumMeshWidth() for comments on the validity of the result
     */
     double getNumberOfInnerCells() const;

    /**
     *
     * !!! Validity of result
     *
     * The operation returns the correct number of outer cells for non-mpi
     * runs always.
     *
     * @see getMaximumMeshWidth() for comments on the validity of the result
     *
     * On top of the discussion in getMaximumMeshWidth(), please consider the
     * following:
     *
     * If you run with mpi, not all outer cells are traversed on a worker
     * before it returns its results to its master: Every worker tries to send
     * back all results as soon as possible even though there might be outer
     * cells remaining that have to be traversed to make the internal data
     * structures consistent for the subsequent traversal again. As a result,
     * the state might have forgotten to count some outer cells and vertices.
     * On the global spacetree, these count errors sum up.
     *
     * If you call the present operation on rank 0, you hence obtain only a
     * lower estimate of outer cells/vertices on the global spacetree. The real
     * number if higher.
     *
     * If you switch off reduction or broadcast of tighten it to your
     * traversal's needs, you will get corrumpted figures as well.
     *
     * If you fork a domain, a new master-worker relation is established. In
     * the subsequent iteration, the master then will transfer data from its
     * domain to the new worker and the new worker will invoke all required
     * operations. However, reduction is not done in this first step, i.e.
     * your counters will slightly go down for one iteration.
     */
     double getNumberOfOuterCells() const;

     double getNumberOfInnerLeafVertices() const;
     double getNumberOfBoundaryLeafVertices() const;

     /**
      * @see getNumberOfOuterCells() for a discussion on the validity of the
      *        result if the code runs with mpi.
      */
     double getNumberOfOuterLeafVertices() const;

     double getNumberOfInnerLeafCells() const;

     /**
      * @see getNumberOfOuterCells() for a discussion on the validity of the
      *        result if the code runs with mpi.
      */
     double getNumberOfOuterLeafCells() const;

     /**
     * Get maximum  mesh width
     *
     * This operation returns a valid value at the end of the iteration, i.e.
     * in endIteration(). Throughout the traversal and in particular in
     * beginIteration(), it holds invalid values. If you need this value
     * throughout the traversal, add your application-specific state a field,
     * copy this attribute's value into this field in the traversal before, and
     * read out this buffered value afterwards instead of using this function.
     *
     * Be aware that for such a pattern, the value still might be invalid, if
     * the grid changes throughout the traversal.
     *
     * @see resetStateAtBeginOfIteration()
     */
    tarch::la::Vector<DIMENSIONS,double> getMaximumMeshWidth() const;

    /**
     * @see getMaximumMeshWidth() for comments on the validity of the result
     */
    tarch::la::Vector<DIMENSIONS,double> getMinimumMeshWidth() const;

    int getMaxLevel() const;

    /**
     * Only for non leaves.
     */
    void incNumberOfInnerVertices( double increment );

    /**
     * Only for non leaves.
     */
    void incNumberOfBoundaryVertices( double increment );

    /**
     * Only for non leaves.
     */
    void incNumberOfOuterVertices( double increment  );

    /**
     * Only for non leaves.
     */
    void incNumberOfInnerCells( double increment );

    /**
     * Only for non leaves.
     */
    void incNumberOfOuterCells( double incrementLeaf );

    void incNumberOfInnerLeafVertices( double increment );
    void incNumberOfBoundaryLeafVertices( double increment );
    void incNumberOfOuterLeafVertices( double increment  );
    void incNumberOfInnerLeafCells( double increment );
    void incNumberOfOuterLeafCells( double incrementLeaf );

    bool isTraversalInverted() const;

    void updateRefinementHistoryAfterLoad( bool hasRefinened, bool hasErased, bool hasFailedToEraseDueToDomainDecomposition, bool hasChangedVertexState );
    void updateRefinementHistoryBeforeStore( bool hasTriggeredRefinementForNextIteration, bool hasTriggeredEraseForNextIteration );
    void updateInLeaf( int level, const tarch::la::Vector<DIMENSIONS,double>& h );

    /**
     * Inform state that caller has changed a cell.
     */
    void changedCellState();

    /**
     * This flag is valid if and only if you have a full reduction and you
     * broadcast the state around. If your communication specification skips
     * some of these steps on some ranks, the result is not well-defined.
     */
    bool hasGridChangedInPreviousIteration() const;

    /**
     * Is the grid stationary
     *
     * This operation tells you whether the grid has changed or is about to
     * change in the next iteration. Often, one uses this as guard for a
     * while loop that iterates until the grid doesn't change anymore - in a
     * setup phase typically.
     *
     * The story is slightly different if you have a distributed mpi parallel
     * code. Here, it can happen that erase commands don't pass through as
     * this erase would destroy whole workers. In such a case,
     * isGridStationary() is false all the time though the grid doesn't change
     * from traversal to traversal. Though, if there were a join (rebalancing),
     * it would change immediately. Also compare to isGridBalanced() for these
     * cases.
     *
     * In parallel, it is does either a good idea to use isGridBalanced() or,
     * even more precise, to combine both operations.
     *
     * @see isGridBalanced()
     * @see isInvolvedInJoinOrFork().
     * @see getCouldNotEraseDueToDecompositionFlag()
     *
     * @return Has the grid or have cells or vertices in the last iteration changed.
     *         Also returns false if the grid would like to erase vertices but
     *         cannot do so due to a domain decomposition. The latter case arises
     *         in parallel only.
     */
    bool isGridStationary() const;

    /**
     * Is the grid balanced (in parallel mode)
     *
     * In serial mode, this equals isGridStationary(). If we call this in
     * the parallel case, it is only similar to grid stationary. Different to
     * the latter one, this one returns true even though an erase
     * is triggered that cannot pass due to the domain decomposition. However,
     * it returns true if and only if the last traversal could have triggered
     * a rebalancing but didn't do so (see isInvolvedInJoinOrFork()), i.e. if
     * the grid could - in theory - fork further.
     *
     * @see isGridStationary()
     *
     * @return If grid is stationary and no rebalancing is happing though it
     *         would be possible to rebalance.
     */
    bool isGridBalanced() const;

    /**
     * Reset state at begin of iteration
     *
     * - All the have... (somehow changed the grid) are set to false,
     * - the maximum level is reset to zero,
     * - the maximum mesh width is reset to zero, and
     * - the minimum mesh width is reset to the maximum a double can hold.
     *
     * Throughout the traversal, the individual fields are updated. In
     * endIteration(), they contain valid values.
     *
     * For a lifetime analysis of all the reset operations see the class
     * documentation. Please note that we copy states, i.e. once we receive
     * data from the master, this master might already have done something
     * on the grid and changed flags. It is thus important that all flags
     * are cleared per rank through this operation.
     */
    void resetStateAtBeginOfIteration();

    /**
     * Reset the state at the end of the iteration.
     *
     * !!! Forking
     *
     * It can happen that throughout a fork that the grid did not change anymore.
     * The simplest example for this is a fork of rank 0 while the worker (rank
     * 1) doesn't refine anything. In this case, the data is deployed to the
     * worker. It is deployed before any vertex update on rank 0 has happened.
     * Consequently, rank 0 remains stationary. However, the new worker has
     * changed its grid in any case. So, the local state may indicate that
     * nothing has changed if and only if the state reduction is switched off.
     *
     * !!! Join Process
     *
     * Peano constraints that each worker has a nontrivial spacetree, i.e. it
     * holds at least a tree of height one. As a result, each join has to come
     * along with a tree modification on the worker side, i.e. an erase of
     * spacetree substructures.
     *
     * !!! Lifetime
     *
     * See the class documentation on details when the reset operations are
     * called. As Peano realises a copy-from-master semantics, a state always
     * is overwritten per iteration (unless you switch it off explicitly).
     * Consequently, this operation modifies solely attributes that are not
     * modelled as state statistics and thus are rank-local. As the description
     * suggests, the operation (almost) degenerates to nop in the sequential
     * case. However, please note that this operation also is called on the
     * global master, i.e. all attribute transitions then apply to all workers
     * as soon as the state has been sent out.
     *
     * A different story is the flag indicating whether the transition is
     * inverted or not. This one has to be set by each rank - if you skip a
     * broadcast or if you receive the state late, i.e. right before you dive
     * into the central elements, your code otherwise would load invalid data.
     *
     * The flag HasModifiedGridInPreviousIteration is not reset by
     * resetStateAtBeginOfIteration(). We hence may update it on the global
     * master. It is then sent around to the workers. We may not update it
     * on the other ranks (they would reset it to false).
     */
    void resetStateAtEndOfIteration();

    #ifdef Parallel
    /**
     * Blocking send. initDatatype() has to be called before.
     *
     * This operation basically transfers the state's parallelise arguments
     * bit-by-bit to the master. However, the WorkerHasWorkerFlag might be
     * changed throughout the call (not locally).
     */
    void send(int destination, int tag, bool exchangeDataBlocking );

    /**
     * Blocking receive. initDatatype() has to be called before.
     *
     * This operation always exchanges all data, i.e. not only the attributes
     * marked with parallelise. Please note that the attribute
     * _loadRebalancingRemoteRanks is a local attribute to this class and not
     * to the type modeled due to DaStGen. Hence, this container remains
     * unaltered.
     *
     * An exception to the anticipate-everything-from-master rule is the load
     * balancing flag. It is modeled by DaStGen and normally would be
     * overwritten by the received state. As it shall be worker-local and shall
     * not be overwritten by the master's data, we restore it manually.
     */
    void receive(int source, int tag, bool exchangeDataBlocking );

    /**
     * Notify state about join
     *
     * This operation is used both to notify the state that some subworkers
     * shall join and notify the state that this worker joins into its master.
     */
    void joinWithRank( int rank );
    void splitIntoRank( int rank );

    bool isInvolvedInJoinOrFork() const;

    /**
     * The user may optimise the traversals by using multiple iterations in a
     * row in combination with lb switched off. In this case, one can exchange
     * data lazy.
     */
    bool mayUseLazyStateAndDataReceives() const;
    void currentlyRunsMultipleIterations(bool value);

    /**
     * Has a subworker rebalanced?
     *
     * This operation gives you information whether one of the workers has
     * rebalanced or is still involved in rebalancing. The operation also
     * returns true if the local worker is rebalancing. If the present
     * result holds, isGridBalanced() should not return true.
     *
     * The result of this operation is not valid before endIteration().
     */
    bool hasSubworkerRebalanced() const;

    bool isForkTriggered() const;
    bool isForkTriggeredForRank(int rank) const;
    bool isForking() const;

    bool isJoinTriggered() const;
    bool isJoinTriggeredForRank(int rank) const;
    bool isJoinWithMasterTriggered() const;
    bool isJoiningWithWorker() const;
    bool isJoiningWithMaster() const;

    bool hasJoinedWithMaster() const;

    bool isForkingRank(int rank) const;
    bool isJoiningRank(int rank) const;

    bool isNewWorkerDueToForkOfExistingDomain() const;

    /**
     * Inform the state that this is the very first iteration of the current worker
     *
     * This operation is invoked by the root node of the spacetree on
     * construction time. It is not something a PDE solver should call.
     */
    void setIsNewWorkerDueToForkOfExistingDomain(bool value);

    std::set<int> getForkingOrJoiningOrTriggeredForRebalancingRanks() const;

    bool getCouldNotEraseDueToDecompositionFlag() const;

    /**
     * Switch on/off reduction locally
     */
    void setReduceStateAndCell( bool value );

    /**
     * Switch on/off reduction for one worker
     */
    void setReduceStateAndCell( int worker, bool value );

    /**
     * Shall we reduce data after the traversal or is the reduction switched off?
     *
     * The result of this attribute depends first of all on the internal flag
     * telling the state whether to reduce or not. If you are currently joining
     * with the master or a brand new worker due to a fork, you are not allowed
     * to ask this question as you are not supposed even to think about a
     * reduction. As discussed, the reduction is switched off in the first step
     * of the fork process. See Node::updateCellsParallelStateBeforeStore() for
     * a discussion on the technical reasons.
     *
     * @todo Doku ist Bloadsinn, alle war besser
     */
    bool reduceDataToMaster() const;

    /**
     * Shall we reduce data from rank rank?
     *
     * This operation is to be called for workers only. It basically returns the
     * flag from the internal data base. If no entry exists for rank, this rank
     * just has been forked. We hence return true.
     */
    bool reduceDataFromWorker( int rank ) const;

    /**
     * May fork in this traversal
     *
     * Any decision on forks and joins of Peano is prinicipally delegated to a
     * load balancing oracle. This instance is the strategy deciding when to
     * fork or not. However, we have, in some cases, to veto any load balancing
     * a priori. Those cases are:
     *
     * - If the last fork/join has happened less than IterationsInBetweenRebalancing
     *   iterations before. If we identify after an iteration that rebalancing
     *   has started, we reset the corresponding _iterationCounter to zero and
     *   wait until the grid has 'recovered'.
     * - If we are not in a fork-triggered or unbalanced state.
     *
     * This operation is used by the Node class and the result it forwarded to
     * the oracle. You may use this operation to analyse yourself at any point
     * whether a fork in principle would be possible, but usually this operation
     * is not used by any pde-specific code.
     *
     * @see Node::updateCellsParallelStateAfterLoadForRootOfDeployedSubtree()
     */
    bool mayForkDueToLoadBalancing() const;

    /**
     * Inform cell that the node would like to fork
     *
     * The following operation comes into play if the master of the current
     * node has invoked it with a fork command, i.e. if mayForkDueToLoadBalancing()
     * holds on the master. Then, the worker identifies potential fork candidates
     * and asks this operation whether this fork is o.k.
     *
     * The two-stage process is necessary as Peano basically realises a
     * depth-first traversal. The following sketch illustrates potential
     * implications:
     *
     * @image html peano/grid/State_mayForkCellsOnLevel.png
     *
     * Let the root node accept the instruction to fork three times. The labels
     * illustrate how often a node could fork its children (due to grid regularity
     * properties, e.g.). Peano descends into the light red node. It could do one
     * fork and hence deploys the blue node. Afterward, it continues to descend:
     * not into the blue node as this one is deployed, and the grey one is not
     * a fork candidate. So we run into the dark red node and fork this one twice.
     *
     * This is however not a good choice: We should instead fork the light green
     * cell. The solution is to introduce the max fork level prescribing the finest
     * level that may be forked per iteration. This fields is set to inf at the
     * beginning and is increased after each iteration. When we do a fork, we set
     * it to level.
     *
     * @return false  If the operation returns false, it vetos the fork.
     */
    bool mayForkCellsOnLevel(int level);

    /**
     * Counterpart of mayForkDueToLoadBalancing().
     *
     * @see mayForkDueToLoadBalancing()
     */
    bool mayJoinDueToLoadBalancing() const;

    /**
     * Has to be called if you restart a repository, i.e. if you receive the
     * responsibility for a new subpartition.
     */
    void restart();

    /**
     * Merge with worker state
     *
     *
     * - beginIteration is the first time when a mapping sees a state.
     * - This state then is distributed by Peano automatically among the
     *   workers.
     * - Throughout the traversal, Peano merges back worker states into
     *   the master due to this operation, i.e.
     * - When you receive endIteration(), all the worker states already are
     *   merged into the master state and no merges are pending.
     */
    void mergeWithWorkerState(const peano::grid::State<StateData>& workerState);

    /**
     * Receive state and cell from the master
     *
     * This operation becomes nop if we do not compile Peano with -DParallel.
     * Otherwise, it does a blocking receive waiting for the the following
     * data from its master:
     *
     * - load balancing information that is directly forwarded to the load
     *   balancing's oracle, and
     * - state of the traversal automaton.
     *
     * If the operation is ran on the global master, it also becomes nop. As
     * the worker is responsible for the root cell,
     * - it is the only node that holds valid data (though it is replicated on the master), and
     * - its content is copied to the master after each iteration.
     *
     * In turn, there's no cell data to be exchanged at the startup by the grid.
     * However, the root node does exchange cell and vertex data. See
     * Root::receiveCellAndVerticesFromMaster().
     *
     * The counterpart of this operation is Node::updateCellsParallelStateAfterLoadForRootOfDeployedSubtree().
     *
     * !!! Visibility
     *
     * Has to be visible to others as the Root might want to trigger this
     * instead of the grid. It depends on the mappings' communication
     * specification. Therefore, it also has to be static.
     */
    void receiveStartupDataFromMaster();

    /**
     * Is invoked if and only if
     *
     * - there is a master node,
     * - the reduction is switched on,
     * - the node is not joining, and
     * - the node has not just forked.
     *
     * You may not call this operation is _state.reduceDataToMaster() does not hold.
     *
     * For the rationale for the latter constraint, please see Node::updateCellsParallelStateBeforeStore()
     *
     * !!! Send protocol
     *
     * If the SendMasterWorkerAndWorkerMasterMessagesBlocking is set, the code
     * sends its data blocking. It is typical among the most critical
     * communication routines. Thus, switching to a blocking send here is of
     * value as any overhead elimination here pays off.
     *
     * !!! Optimisation
     *
     * In an optimal world where reductions are on, we should send the state to
     * the master when the worker has completed its whole traversal including the
     * halo. The latter fact is the problem: the additional halo cells slow down
     * the overall parallel efficiency. In the latter case, it is also convenient
     * to make the grid invoke the reduction as it is also responsible for the
     * counterpart operation, i.e. the state receive. Anyway, the state is the
     * last thing sent up to the master after all the vertices and the cell data.
     *
     * Sometimes, 100% accurate statistics are not necessary but it is speed that
     * matters though reductions cannot be switched off completely. In this case,
     * it does make sense to send up the state information right after the worker
     * has finished the traversal of its interior cells. But before it runs through
     * all the remaining halo cells. This can be controlled by the optimisation
     * flag. In the end, this operation has to be called at most once either right
     * in by Root::sendCellAndVerticesToMaster() or by the Grid itself.
     *
     * !!! Visibility
     *
     * Has to be visible to others as the Root might want to trigger this
     * instead of the grid. It depends on the mappings' communication
     * specification. Therefore, it also has to be static.
     */
    void sendStateToMaster() const;
    #else
    bool isInvolvedInJoinOrFork() const { return false; }

    void restart();
    #endif
};


template <class StateData>
std::ostream& operator<<(std::ostream& out, const peano::grid::State<StateData>& properties);


#include "peano/grid/State.cpph"

#endif
