// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PARALLEL_LOADBALANCING_ORACLE_FOR_ONE_PHASE_WITH_GREEDY_PARTITIONING_H_
#define _PEANO_PARALLEL_LOADBALANCING_ORACLE_FOR_ONE_PHASE_WITH_GREEDY_PARTITIONING_H_


#include "peano/parallel/loadbalancing/OracleForOnePhase.h"
#include "tarch/logging/Log.h"


#include <set>


namespace peano {
  namespace parallel {
    namespace loadbalancing {
      class OracleForOnePhaseWithGreedyPartitioning;
    }
  }
}


/**
 * Simple Oracle
 *
 * This oracle is very straightforward. As long as no fork has failed, it tells
 * every asking node to fork once. If it is informed that a fork has failed, it
 * stops to ask for additional forks. If a worker has no cells anymore, the
 * oracle remembers this worker and tells to join this one the next time. As a
 * consequence it then continues to fork again.
 *
 * @author Tobias Weinzierl
 */
class peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning: public peano::parallel::loadbalancing::OracleForOnePhase {
  private:
    /**
     * Logging device
     */
    static tarch::logging::Log  _log;

    /**
     * If a fork failed, all the oracles should stop to ask for further forks.
     * Wouldn't make sense and just slow down the application.
     */
    static bool                 _forkHasFailed;

    /**
     * Global flag set at construction time.
     */
    bool                        _joinsAllowed;

    bool                        _forksAllowed;

    /**
     * Set of all the workers that are idle, i.e. have no cells to handle
     * anymore.
     */
    std::set<int>               _idleWorkers;

    const int                   _regularLevelAlongBoundary;
  public:
    OracleForOnePhaseWithGreedyPartitioning(bool joinsAllowed, bool forkIsAllowed = true, int regularLevelAlongBoundary = 0);
    virtual ~OracleForOnePhaseWithGreedyPartitioning();

    void receivedStartCommand(LoadBalancingFlag commandFromMaster ) override;

    /**
     * This operation is not const, as it might update some local stuff.
     */
    LoadBalancingFlag getCommandForWorker( int workerRank, bool forkIsAllowed, bool joinIsAllowed ) override;

    /**
     * Plot something to info log device.
     */
    void plotStatistics() override;

    /**
     * Clone this oracle. This operation is used by the singleton whenever a
     * piece of code asks for parallelisation that never asked before.
     *
     * @param adapterNumber Number of your adapter. Have a closer look to your
     *        repository's state if you want to find out which adapters are
     *        mapped to which state. You can even use the toString() operation
     *        there to map this parameter to a string.
     */
    OracleForOnePhase* createNewOracle(int adapterNumber) const override;

    void forkFailed() override;

    int getRegularLevelAlongBoundary() const override;
};



#endif
