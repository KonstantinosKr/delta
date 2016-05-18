// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_LOAD_BALANCING_ORACLE_H_
#define _PEANO_PARALLEL_LOAD_BALANCING_ORACLE_H_


#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"
#include "peano/parallel/loadbalancing/WorkerEntry.h"
#include "peano/parallel/loadbalancing/OracleForOnePhase.h"
#include "peano/utils/Globals.h"


#include <vector>


#ifdef Parallel
#include <mpi.h>
#endif


namespace peano {
  namespace parallel {
    namespace loadbalancing {
      class Oracle;
    }
  }
}


/**
 * Load balancing oracle
 *
 * The load distribution in Peano is (logically) a tree topology. Whenever the
 * grid is traversed, basically startup messages are passed top-down and state
 * messages are passed bottom-up. Such a scheme naturally yields master-worker
 * relations. Whenever data or algorithms are duplicated, always the master has
 * the responsibility and the correct data. It is never the worker.
 *
 * The oracles run on every node. Whenever the tree traversal encounters yet
 * another worker, it asks the local oracle what to do with this worker, i.e.
 * the master decides on the worker. In turn, the oracle on the worker always
 * is informed about the master's decision. It may not veto, but it is
 * informed.
 *
 * @author Tobias Weinzierl
 */
class peano::parallel::loadbalancing::Oracle {
  private:
    static tarch::logging::Log  _log;

    /**
     * Set of oracles.
     */
    typedef std::vector<OracleForOnePhase*>  OracleDatabase;

    /**
     * Maps ranks to oracles
     */
    OracleDatabase                           _oracles;

    /**
     * Current active oracle.
     *
     * Identifies which oracle out of _oracles to chose.
     */
    int                                      _currentOracle;

    OracleForOnePhase*                       _oraclePrototype;

    typedef std::vector<WorkerEntry>         WorkerContainer;

    /**
     * It is basically a set. However, I wanna access it both element-wisely and in a
     * stack manner.
     */
    WorkerContainer                          _workers;

    /**
     * Typically of type LoadBalancingFlag.
     */
    int                                      _startCommand;

    bool                                     _loadBalancingActivated;

    void createOracles(int numberOfOracles);
    void deleteOracles();

    Oracle();

    WorkerContainer::iterator       getWorkerRecord( int rank );
    WorkerContainer::const_iterator getWorkerRecord( int rank ) const;
  public:
    virtual ~Oracle();

    static Oracle& getInstance();

    /**
     * Inform the oracle about a new worker. This worker may not be contained
     * in the database before. The worker is appended, i.e. from now on, it is
     * the last one.
     *
     * @param boundingBoxOffset  Information about the worker's domain
     * @param boundingBoxSize    Information about the worker's domain
     */
    void addWorker(
      int                                          rank,
      const tarch::la::Vector<DIMENSIONS,double>&  boundingBoxOffset,
      const tarch::la::Vector<DIMENSIONS,double>&  boundingBoxSize,
      int                                          level
    );

    #ifdef Parallel
    /**
     * Exchange worker information among all ranks involved
     *
     * This is a global operation that has to be called by all ranks at the same
     * time. Even those that are idle currently. It is a very expensive
     * operation. The resulting list is not ordered at all but contains all
     * worker entries stored anywhere on the system. Also, the ordering on each
     * rank might be different.
     *
     * Please note that the result typically does not contain an entry for the
     * global master, i.e. for the bounding box of the problem.
     *
     * !!! Implementation
     *
     * I cannot use MPI's Bcast here as the cardinality of the individual
     * packages is not known a priori. And it differs from rank to rank.
     *
     * This operation is protected with the ifdef to make the code translate in
     * serial variants as well. If you work without -DParallel, mpi.h is not
     * included and the datatype MPI_Comm hence is undefined.
     *
     * @return List of all worker informations (available on each rank now).
     */
    std::vector<WorkerEntry> exchangeWorkerDataWithAllRanks(int tag, MPI_Comm communicator);
    #endif

    /**
     * Is public to enable other classes to validate their state as well.
     */
    bool workersListContainsRank( int rank ) const;

    /**
     * Remove a worker from the database. It has to be inserted there before.
     * This operation is not used by the regular grid, as the regular grid
     * always only removes the last worker inserted. For the spacetree grid
     * however it is a different story.
     */
    void removeWorker(int rank);

    bool hasWorkers() const;

    int getNumberOfWorkers() const;

    int getWorkerRank(int index) const;

    tarch::la::Vector<DIMENSIONS,double>  getBoundingBoxOffset( int workerRank ) const;

    tarch::la::Vector<DIMENSIONS,double>  getBoundingBoxSizeOfWorker( int workerRank ) const;

    /**
     * Inform state that there has been a fork call for one worker. It is
     * important for the state to know whether one of its workers has forked
     * again. If this is the case, joins are not allowed.
     *
     * By default, this information is passed bottom-up. See the merge
     * operation. However, if the restriction is switched off, we have no
     * real information. The state thus assumes that the worker really has
     * done the fork and that the fork was successful, i.e. that there have
     * been nodes available. If might turn out to be wrong in the next
     * restriction, but till then, this is all we know.
     */

    void plotStatistics();

    /**
     * Tell the oracle how many different adapters you'll gonna use.
     */
    void setNumberOfOracles(int value);

    /**
     * Tell the oracle which adapter is used right now.
     */
    void switchToOracle(int id);

    /**
     * Set the active oracle. Do not delete this argument - the oracle will
     * do it.
     */
    void setOracle( OracleForOnePhase* oraclePrototype );

    /**
     * @see OracleForOnePhase
     *
     * @param commandFromMaster Usually is a value from the enum LoadBalancingFlag
     */
    void receivedStartCommand(int commandFromMaster);

    /**
     * Return last start command
     *
     * Returns the last start command received due to receivedStartCommand().
     * However, also a failed fork might modify the result of this operation.
     *
     * @see forkFailed()
     */
    int getLastStartCommand() const;

    /**
     * Get the command for a worker
     *
     * The operation basically forwards this call to
     * OracleForOnePhase::getCommandForWorker().
     *
     * This operation is not const, as it might update some local stuff.
     *
     * @param workerRank Rank of the worker for which the load balancing would
     *                   be interested in a command
     */
    int getCommandForWorker( int workerRank, bool forkIsAllowed = true, bool joinIsAllowed = true);

    /**
     * Notification mechanism that the master is finished. This might not be
     * sent after a getCommandForWorker() call if the bottom-up communication
     * is switched off.
     */
    void receivedTerminateCommand(
      int     workerRank,
      double  workerNumberOfInnerVertices,
      double  workerNumberOfBoundaryVertices,
      double  workerNumberOfOuterVertices,
      double  workerNumberOfInnerCells,
      double  workerNumberOfOuterCells,
      int     workerMaxLevel,
      int     workerLocalWorkload,
      int     workerTotalWorkload,
      int     workerMaxWorkload,
      int     workerMinWorkload,
      int     currentLevel,
      int     parentCellLocalWorkload,
      const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset,
      const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize,
      bool    workerCouldNotEraseDueToDecomposition
    );

    /**
     * Notifies the oracle that from now on, the master starts to wait for workers.
     */
    void masterStartsToWaitForWorkers();

    /**
     * Fork Has Failed
     *
     * Informs the active oracle that a fork has failed. This is usually due to
     * a lack of idle MPI ranks.
     *
     * Peano is robust with respect to fork attempts if no idle workers are
     * available. If the oracle says fork but no ranks are available, the grid
     * just remains unchanged. However, detecting such a case is expensive:
     * Each time the oracle says fork, a message is sent to the central node
     * pool. This node pool then can answer 'no nodes available'. This is a
     * very flexible concept but introduces a single point of contact.
     *
     * As a consequence, Peano informs the oracle whenever a fork has failed.
     * It then is a good idea for an efficient oracle to skip further fork
     * fork calls for this traversal, i.e. I recommend to return Continue or
     * Join afterwards. Such forkFailed() information is valid only until the
     * next iteration, i.e. I recommend to try again to fork in the next
     * iteration. I recommend to reset internal fork-failed states. The start
     * of a new iteration is mirrored by receivedStartCommand(). So, after
     * the next receivedStartCommand(), forks should be tried again.
     *
     * Alternatively, it might even make sense to wait for a fixed number
     * of receivedStartCommand() calls before the oracle tries to fork the
     * next time.
     */
    void forkFailed();

    /**
     * Get Coarsest Level of Grid
     *
     * For the parallelisation, might make sense to determine a coarsest grid
     * level that should be regular no matter wether cells there are inside or
     * outside. Peano can fork on this level also outer cells. Those typically
     * are cells that are refined and have inner descendands, i.e. cells along
     * the parallel boundary or cells that will become inside due to changing
     * domains. With this operation, we can ensure that they can already be
     * distributed before the domain change actually happens.
     */
    int getCoarsestRegularInnerAndOuterGridLevel() const;

    /**
     * You can switch off the load balancing via this value.
     */
    void activateLoadBalancing(bool value);

    bool isLoadBalancingActivated() const;
};



#endif
