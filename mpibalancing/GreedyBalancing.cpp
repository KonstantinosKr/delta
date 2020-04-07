#include "mpibalancing/GreedyBalancing.h"

#include "tarch/Assertions.h"
#include "tarch/la/ScalarOperations.h"
#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"
#include "peano/parallel/loadbalancing/Oracle.h"


tarch::logging::Log mpibalancing::GreedyBalancing::_log( "mpibalancing::GreedyBalancing" );


bool mpibalancing::GreedyBalancing::_forkHasFailed = false;
int  mpibalancing::GreedyBalancing::_regularLevelAlongBoundary = 0;


mpibalancing::GreedyBalancing::GreedyBalancing(int coarsestLevelWithRealWork, int coarsestRegularInnerAndOuterGridLevel):
  _coarsestLevelWithRealWork(coarsestLevelWithRealWork) {
  _finestLevelToForkAggressively = _coarsestLevelWithRealWork;
  int activeNodes                = 1;

  while ( activeNodes<tarch::parallel::Node::getInstance().getNumberOfNodes() ) {
    activeNodes *= THREE_POWER_D;
    _finestLevelToForkAggressively++;
  }

  _regularLevelAlongBoundary = coarsestRegularInnerAndOuterGridLevel;
}


mpibalancing::GreedyBalancing::~GreedyBalancing() {
}


void mpibalancing::GreedyBalancing::receivedStartCommand( peano::parallel::loadbalancing::LoadBalancingFlag commandFromMaster ) {
}


peano::parallel::loadbalancing::LoadBalancingFlag  mpibalancing::GreedyBalancing::getCommandForWorker( int workerRank, bool forkIsAllowed, bool joinIsAllowed ) {
  logTraceInWith3Arguments( "getCommandForWorker(int,bool)", workerRank, forkIsAllowed, joinIsAllowed);

  peano::parallel::loadbalancing::LoadBalancingFlag result = peano::parallel::loadbalancing::LoadBalancingFlag::ForkGreedy;

  if (_forkHasFailed) {
    result = peano::parallel::loadbalancing::LoadBalancingFlag::Continue;
  }
  else if (_workersLevel.count(workerRank)==1) {
    const int workersLevel = _workersLevel.count(workerRank);

    if (workersLevel>=_finestLevelToForkAggressively && forkIsAllowed) {
      result = peano::parallel::loadbalancing::LoadBalancingFlag::ForkOnce;
    }
  }

  logTraceOutWith1Argument( "getCommandForWorker(int,bool)", static_cast<int>(result) );
  return result;
}


void mpibalancing::GreedyBalancing::plotStatistics() {
}


peano::parallel::loadbalancing::OracleForOnePhase* mpibalancing::GreedyBalancing::createNewOracle(int adapterNumber) const {
  return new GreedyBalancing(_coarsestLevelWithRealWork, _regularLevelAlongBoundary);
}


void mpibalancing::GreedyBalancing::forkFailed() {
  if (!_forkHasFailed) {
    logInfo(
      "forkFailed()",
      "oracle was informed that fork has failed. No further fork attempts"
    );
  }
  _forkHasFailed = true;
}


int mpibalancing::GreedyBalancing::getRegularLevelAlongBoundary() const {
  return _regularLevelAlongBoundary;
}
