#include "peano/parallel/loadbalancing/OracleForOnePhaseWithGreedyPartitioning.h"

#include "tarch/Assertions.h"
#include "tarch/la/ScalarOperations.h"


tarch::logging::Log peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::_log( "peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning" );


bool peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::_forkHasFailed = false;



peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::OracleForOnePhaseWithGreedyPartitioning(
  bool joinsAllowed,
  bool forksAllowed,
  int regularLevelAlongBoundary):
  _joinsAllowed(joinsAllowed),
  _forksAllowed(forksAllowed),
  _idleWorkers(),
  _regularLevelAlongBoundary(regularLevelAlongBoundary) {
}


peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::~OracleForOnePhaseWithGreedyPartitioning() {
}


void peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::receivedStartCommand(LoadBalancingFlag commandFromMaster ) {
  if (commandFromMaster==LoadBalancingFlag::Join) {
    _idleWorkers.clear();
  }
}


peano::parallel::loadbalancing::LoadBalancingFlag peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::getCommandForWorker( int workerRank, bool forkIsAllowed, bool joinIsAllowed ) {
  logTraceInWith5Arguments( "getCommandForWorker(int,bool)", workerRank, forkIsAllowed, joinIsAllowed, _joinsAllowed, _idleWorkers.count(workerRank) );

  LoadBalancingFlag result = LoadBalancingFlag::Continue;

  if ( joinIsAllowed
    && _joinsAllowed
    && _idleWorkers.count(workerRank)>0
  ) {
    _idleWorkers.clear();
    _forkHasFailed = false;
    result         = LoadBalancingFlag::Join;
  }
  else if (!_forkHasFailed && forkIsAllowed && _forksAllowed) {
    result = LoadBalancingFlag::ForkGreedy;
  }

  logTraceOutWith1Argument( "getCommandForWorker(int,bool)", convertLoadBalancingFlagToString(result) );
  return result;
}


void peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::plotStatistics() {
}



int peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::getRegularLevelAlongBoundary() const {
  return _regularLevelAlongBoundary;
}


peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::OracleForOnePhase* peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::createNewOracle(int adapterNumber) const {
  return new OracleForOnePhaseWithGreedyPartitioning(_joinsAllowed,_forksAllowed,_regularLevelAlongBoundary);
}


void peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::forkFailed() {
  logInfo( "forkFailed()", "fork has failed. Switch off greedy forking" );
  _forkHasFailed = true;
}
