#include "peano/parallel/loadbalancing/OracleForOnePhaseWithGreedyPartitioning.h"

#include "tarch/Assertions.h"
#include "tarch/la/ScalarOperations.h"


tarch::logging::Log peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::_log( "peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning" );


bool peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::_forkHasFailed = false;



peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::OracleForOnePhaseWithGreedyPartitioning(bool joinsAllowed, bool forksAllowed):
  _joinsAllowed(joinsAllowed),
  _forksAllowed(forksAllowed),
  _idleWorkers() {
}


peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::~OracleForOnePhaseWithGreedyPartitioning() {
}


void peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::receivedStartCommand(int commandFromMaster ) {
  if (commandFromMaster==Join) {
    _idleWorkers.clear();
  }
}


int peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::getCommandForWorker( int workerRank, bool forkIsAllowed, bool joinIsAllowed ) {
  logTraceInWith5Arguments( "getCommandForWorker(int,bool)", workerRank, forkIsAllowed, joinIsAllowed, _joinsAllowed, _idleWorkers.count(workerRank) );

  int result = Continue;

  if ( joinIsAllowed
    && _joinsAllowed
    && _idleWorkers.count(workerRank)>0
  ) {
    _idleWorkers.clear();
    _forkHasFailed = false;
    result         = Join;
  }
  else if (!_forkHasFailed && forkIsAllowed && _forksAllowed) {
    result = ForkGreedy;
  }

  logTraceOutWith1Argument( "getCommandForWorker(int,bool)", convertLoadBalancingFlagToString(result) );
  return result;
}


void peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::receivedTerminateCommand(
  int     workerRank,
  double  workerNumberOfInnerVertices,
  double  workerNumberOfBoundaryVertices,
  double  workerNumberOfOuterVertices,
  double  workerNumberOfInnerCells,
  double  workerNumberOfOuterCells,
  int     workerMaxLevel,
  double  workerLocalWorkload,
  double  workerTotalWorkload,
  double  workerMaxWorkload,
  double  workerMinWorkload,
  int     currentLevel,
  double  parentCellLocalWorkload,
  const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset,
  const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize,
  bool    workerCouldNotEraseDueToDecomposition
) {
  if ( tarch::la::equals( workerNumberOfInnerCells, tarch::la::NUMERICAL_ZERO_DIFFERENCE ) ) {
    _idleWorkers.insert( workerRank );
  }
}


void peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::plotStatistics() {
}



int peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::getCoarsestRegularInnerAndOuterGridLevel() const {
  return 3;
}


peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::OracleForOnePhase* peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::createNewOracle(int adapterNumber) const {
  return new OracleForOnePhaseWithGreedyPartitioning(_joinsAllowed,_forksAllowed);
}


void peano::parallel::loadbalancing::OracleForOnePhaseWithGreedyPartitioning::forkFailed() {
  logInfo( "forkFailed()", "fork has failed. Switch off greedy forking" );
  _forkHasFailed = true;
}
