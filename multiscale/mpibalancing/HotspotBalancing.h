// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _MPI_BALANCING_HOTSPOT_BALANCING_H_
#define _MPI_BALANCING_HOTSPOT_BALANCING_H_

#include "peano/parallel/loadbalancing/OracleForOnePhase.h"
#include "tarch/logging/Log.h"


#include <map>
#include <set>


namespace mpibalancing {
  class HotspotBalancing;
}


/**
 * Hotspot Balancing
 *
 * This balancing identifies the critical path locally, i.e. if the oracle is
 * told by its master that it shall fork at least once, it identifies its worker
 * with the biggest load and makes this one fork as well. Please note that the
 * critical path can be multiple ranks that are forked at the same time.
 *
 *
 * <h2>Usage</h2>
 *
 * First of all, ensure that you include the balancing's header in your runner
 * and then add
 * <pre>
peano::parallel::loadbalancing::Oracle::getInstance().setOracle(
  new mpibalancing::HotspotBalancing(false)
);
   </pre>
 * on each rank. I strongly recommend to switch off joins for a first try -
 * therefore the false argument in the snippet above.
 *
 * This oracle will not work properly, as we haven't fed it yet with a cost
 * model. It cannot know which rank is member along the critical paths.
 *
 * There are many different cost models one could design and I originally
 * proposed to use a tree-based cost model in most conversations and also in my
 * dissertation. Such a model assigns each leaf a cost and then restricts this
 * cost bottom-up. The approach works fine as long as you do not use Peano's
 * new feature to overlap the grid with the domain boundary and to deploy
 * boundary grid segments to other ranks. In this case, a grammar-based cost
 * analysis has to fail as information is not propagated through all the
 * coarse outside tree parts.
 *
 * I therefore propose a more global approach with this balancing that is even
 * simpler and applies this tree-grammar cost idea to the rank topology only.
 * To use it, I propose to add a counter _numberOfInnerCells to your mappping
 * that connects to the load balancing. This flag is set to zero in
 * beginIteration() and we increment it in enterCell() or leaveCell(). Please
 * ensure that you synchronise accesses properly in a TBB environment.
 *
 * <pre>
void boxmg::mappings::CreateGrid::beginIteration(
  boxmg::State&  solverState
) {
  srand(time(NULL));

  _numberOfInnerCells = 0;
}

void boxmg::mappings::CreateGrid::leaveCell(
  boxmg::Cell&           fineGridCell,
  boxmg::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  boxmg::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  boxmg::Cell&           coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
  _numberOfInnerCells++;
}
   </pre>
 *
 * We could use Peano's grid statistics to derive exactly this information, but
 * most people prefer to count manually and kick out the (expensive) grid analysis
 * of Peano in production runs. The analysed counter now is sent to the master in
 * prepareSendToMaster():
 *
 <pre>
 void boxmg::mappings::CreateGrid::prepareSendToMaster(
  boxmg::Cell&                       localCell,
  boxmg::Vertex *                    vertices,
  const peano::grid::VertexEnumerator&       verticesEnumerator,
  const boxmg::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  const boxmg::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
  logTraceInWith2Arguments( "prepareSendToMaster(...)", localCell, verticesEnumerator.toString() );

  mpibalancing::HotspotBalancing::setLocalWeightAndNotifyMaster(
    _numberOfInnerCells
  );

  logTraceOut( "prepareSendToMaster(...)" );
}
 <\pre>
 *
 * The same routine also bookkeeps the cost locally. In return for the send, we
 * have to plug into mergeWithMaster to receive all load balancing data:
 *
 * <pre>
void boxmg::mappings::CreateGrid::mergeWithMaster(
  const boxmg::Cell&           workerGridCell,
  boxmg::Vertex * const        workerGridVertices,
  const peano::grid::VertexEnumerator& workerEnumerator,
  boxmg::Cell&                 fineGridCell,
  boxmg::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  boxmg::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  boxmg::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker,
  const boxmg::State&          workerState,
  boxmg::State&                masterState
) {
  mpibalancing::HotspotBalancing::mergeWithMaster(
    worker,
    workerState.getCouldNotEraseDueToDecompositionFlag()
  );
}
   </pre>
 *
 *
 * <h2> Troubshooting </h2>
 *
 * I sometimes run into the situation that the grid is correctly balanced for
 * small(er) grids but that the load balancing degenerates into something
 * ill-balanced when you create a finer grid. Finding such issues can be
 * very painful. From time to time, I had the issue that I had fused grid
 * construction and load balancing. This led to the situation that the load
 * balancing started to fork off some subpartitions on some ranks. Such a
 * fork-off unfortunately leads to the situation that refine instructions
 * along the domain boundary are postponed by one iteration while others
 * run through straight ahead.
 *
 *
 * <h2>Behaviour</h2>
 *
 * <h3> getCommandForWorker </h3>
 *
 * getCommandForWorker() here is pretty simple and evaluates basically
 * _forksToConduct and _criticalWorker identifying whether a worker shall
 * fork (only if it is member of the set _criticalWorker) and how often it
 * shall fork (_forksToConduct). There are few special cases besides this:
 *
 * - If the oracle is running on the global master and the weight table holds
 *   only one entry (i.e. the weight of the local rank), this is the very
 *   root of the logic mpi tree and calls its worker to fork all of its nodes,
 *   i.e. we make rank 0 and rank 1 fork all their children. This way, we can
 *   evaluate on rank 1 really which subtree is the biggest one. If we'd fork
 *   all subtrees besides one on rank 1, this would not be possible that easy,
 *   as it is complicated to identify the real local workload throughout a
 *   traversal.
 * - If the oracle is asked for the command for a worker who couldn't erase
 *   before due to the domain decomposition and if joins are globally allowed,
 *   we join this one to facilitate that erases pass through and the global
 *   grid becomes smaller.
 *
 * <h3>  receivedStartCommand </h3>
 *
 * This operation triggers the critical path analysis which consists of two
 * steps:
 *
 * - Determine the critial path
 * - Compute the number of forks along the critical paths
 *
 * If its command from its master is unequal to fork, there's no analysis to
 * be done, i.e. we evaluate the critical path recursively which works as we
 * are working in a tree environment. Otherwise, the two operations map onto
 * the routines identifyCriticalPathes() and
 * computeMaxForksOnCriticalWorker().
 *
 * If we shall fork, we first identify the biggest and the smallest local
 * workload. All workloads are always greater or equal to 1.
 * Once we have the minimum and maximum workload as well as the critical
 * worker, i.e. the one having the biggest workload (which might be the
 * local rank as well), we determine the number of forks to conduct. The
 * formula for this endeavour is as follows: We assume (at the beginning)
 * that k forks reduce the worker's workload by
 *
 * @f$ w \gets w \cdot \frac{3^d-k}{3^d} @f$
 *
 * This assumption is based upon a regular grid. If the critical weight shall
 * equal the smallest weight, we obtain
 *
 * @f$ w_{critical} \cdot \frac{3^d-k}{3^d} = w_{minimum} @f$
 * @f$ k = 3^d \left( 1-\frac{w_{minimum}}{w_{critical}} \right) @f$
 *
 * Finally, we ensure that the forks are bounded by @f$ 3^d-1 @f$ or reset
 * the critical worker if no forks are to be done.
 *
 * <h3>  Local Minima </h3>
 *
 * If we have p ranks, it can happen that the oracle achieves perfect balancing with
 * @f$ \hat p < p @f$ ranks already. If we have 50 ranks, 10 in 2d already are perfectly
 * balanced though then 40 ranks remain idle. In this case, the critical path analysis
 * runs into a local minimum.
 *
 * If we identify a local minimum, we set the numbers to fork manually to
 * @f$(3^d-1)/2 @f$ and label all workers as critical.
 *
 * @image html HotspotBalancing.png
 * @author Tobias Weinzierl
 */
class mpibalancing::HotspotBalancing: public peano::parallel::loadbalancing::OracleForOnePhase {
  public:
    /**
     * Constructor
     *
     * @param joinsAllowed The static balancing does, as the name suggests, not
     *                     join any partitions throughout the computation. Only
     *                     if grid erases do not pass through due to the domain
     *                     decomposition, it supports joins if this flag is set
     *                     true.
     *
     * @param coarsestRegularInnerAndOuterGridLevel If you use this balancing,
     *                     the grid is refined regularly up to level
     *                     coarsestRegularInnerAndOuterGridLevel - independent
     *                     of whether grid elements are inside or outside of
     *                     the domain. Too regular grids facilitate a
     *                     proper load balancing in several cases. The value
     *                     has however no direct impact on the load balancing.
     *                     It solely controls the grid refinement pattern.
     */
    HotspotBalancing( bool joinsAllowed, int coarsestRegularInnerAndOuterGridLevel, int maxRanksThatCanBeUsedAsAdministors = std::numeric_limits<int>::max() );

    virtual ~HotspotBalancing();

    /**
     * Analyse which workers are to be forked next.
     *
     * This operation is described in detail within the class documentation as
     * it realises the actual algorithm.
     */
    void receivedStartCommand(peano::parallel::loadbalancing::LoadBalancingFlag commandFromMaster ) override;

    peano::parallel::loadbalancing::LoadBalancingFlag getCommandForWorker( int workerRank, bool forkIsAllowed, bool joinIsAllowed ) override;

    void plotStatistics() override;

    peano::parallel::loadbalancing::OracleForOnePhase* createNewOracle(int adapterNumber) const override;

    void forkFailed() override;
 
    int getRegularLevelAlongBoundary() const override;


    /**
     * Receive load balancing information from worker rank.
     */
    static void mergeWithMaster(
      int     workerRank,
      bool    workerCouldNotEraseDueToDecomposition,
      int     coarsestWorkerLevel
    );

    /**
     * Inform about number of local cells (or, more abstract, local cost),
     * bookmark this information and send the updated value to the parent
     * rank.
     *
     * This operation is typically called in prepareSendToMaster().
     */
    static void setLocalWeightAndNotifyMaster(
      double  localWeight
    );

  private:
    HotspotBalancing( bool joinsAllowed );

    /**
     * Runs an analysis on the _weightMap. This operation returns consistent
     * data if it is used within receivedStartCommand() and if all children
     * do restrict. It then holds all the weights from the previous iteration.
     * Otherwise, some results might be older (two or three iterations ago,
     * e.g.) or some values might also have been updated in the current
     * traversal.
     */
    static double getMaximumWeightOfWorkers();

    /**
     * @see getMaximumWeightOfWorkers()
     * @see Class documentation clarifying why we have to exclude weights
     *      smaller than one explicitly.
     */
    static double getMinimumWeightOfWorkers();

    /**
     * Run through all the workers and insert those that have a critical weight
     * into the _criticalWorker set. The latter is cleared upon entering the
     * routine. We do the path analysis if and only if the master has told us
     * to fork further.
     */
    void identifyCriticalPathes( peano::parallel::loadbalancing::LoadBalancingFlag commandFromMaster );

    /**
     * See class documentation. We rely on the fact that _criticalWorker is
     * properly initialised, i.e. identifyCriticalPathes() has been called
     * before.
     */
    void computeMaxForksOnCriticalWorker( peano::parallel::loadbalancing::LoadBalancingFlag commandFromMaster );

    /**
     * Logging device
     */
    static tarch::logging::Log  _log;

    /**
     * If a fork failed, all the oracles should stop to ask for further forks.
     * Wouldn't make sense and just slow down the application.
     */
    static bool                 _forkHasFailed;

    static int                  _regularLevelAlongBoundary;

    static int                  _finestLevelWhereRanksArePurelyAdministrative;

    static int                  _loadBalancingTag;

    /**
     * Global flag set at construction time.
     */
    const bool                  _joinsAllowed;
    
    /**
     * Holds for each worker its local weight. Is never empty as it always
     * holds something for the local rank.
     */
    static std::map<int,double>        _weightMap;

    /**
     * Bookkeeps for each worker whether this one couldn't erase due to the
     * domain decomposition.
     */
    static std::map<int,bool>          _workerCouldNotEraseDueToDecomposition;

    static std::map<int,bool>          _workerShouldBecomeAdministrativeRank;

    /**
     * Set of critical workers
     *
     * This set might be empty, but it can also contain multiple critical
     * workers. All critical workers will receive fork commands if Peano
     * asks for a load balancing command next. What type of fork command
     * they receive is held in _forksToConduct.
     */
    std::set<int>               _criticalWorker;

    /**
     * Determines the number of forks for a worker along the critical path.
     */
    int                         _maxForksOnCriticalWorker;
};



#endif
