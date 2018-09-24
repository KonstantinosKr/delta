// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _MPI_BALANCING_GREEDY_BALANCING_H_
#define _MPI_BALANCING_GREEDY_BALANCING_H_

#include "peano/parallel/loadbalancing/OracleForOnePhase.h"
#include "tarch/logging/Log.h"


#include <map>
#include <set>


namespace mpibalancing {
  class GreedyBalancing;
}


/**
 * Greedy Balancing
 *
 * This is a minor extension of the greedy balancing coming along with the
 * Peano kernel. The oracle determines what the coarsest level would be that
 * can fork aggressively if the grid is regular. Up to this coarsest level,
 * the oracle returns ForkGreedy, i.e. asks each rank to fork up to 3^d-1
 * times per iteration. For finer levels, it returns ForkOnce to slow down
 * the forking a little bit. As soon as a fork cannot be accomplished, i.e.
 * the system runs out of idle ranks, we stop the forking for the remainder
 * of the simulation.
 *
 * @author Tobias Weinzierl
 */
class mpibalancing::GreedyBalancing: public peano::parallel::loadbalancing::OracleForOnePhase {
  public:
    /**
     * Constructor
     *
     * @param coarsestRegularInnerAndOuterGridLevel If you use this balancing,
     *                     the grid is refined regularly up to level
     *                     coarsestRegularInnerAndOuterGridLevel - indepenent
     *                     of whether grid elements are inside or outside of
     *                     the domain. Too regular grids facilitate a
     *                     proper load balancing in several cases.
     */
    GreedyBalancing( int coarsestLevelWithRealWork, int coarsestRegularInnerAndOuterGridLevel = 3 );

    virtual ~GreedyBalancing();

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

    const int                   _coarsestLevelWithRealWork;

    static int                  _regularLevelAlongBoundary;

    std::map<int,int>           _workersLevel;

    int                         _finestLevelToForkAggressively;
};



#endif
