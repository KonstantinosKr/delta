#include "HotspotBalancing.h"
#include "tarch/Assertions.h"
#include "tarch/la/ScalarOperations.h"
#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"
#include "peano/parallel/loadbalancing/Oracle.h"


#ifdef Parallel
#include <mpi.h>
#endif


/**
 * We provide two ways to exchange load balancing information: blocking and
 * non-blocking. Blocking should be faster, but we did encounter a couple of
 * deadlocks if we used blocking data exchange.
 */
namespace {
  const int UseBlockingSendAndReceive = false;
}


tarch::logging::Log  mpibalancing::HotspotBalancing::_log( "mpibalancing::HotspotBalancing" );
int                  mpibalancing::HotspotBalancing::_loadBalancingTag = -1;


bool                        mpibalancing::HotspotBalancing::_forkHasFailed = false;
std::map<int,double>        mpibalancing::HotspotBalancing::_weightMap;
std::map<int,bool>          mpibalancing::HotspotBalancing::_workerCouldNotEraseDueToDecomposition;
std::map<int,bool>          mpibalancing::HotspotBalancing::_workerShouldBecomeAdministrativeRank;
int                         mpibalancing::HotspotBalancing::_regularLevelAlongBoundary = 0;
int                         mpibalancing::HotspotBalancing::_finestLevelWhereRanksArePurelyAdministrative = -1;


mpibalancing::HotspotBalancing::HotspotBalancing(
  bool joinsAllowed,
  int coarsestRegularInnerAndOuterGridLevel,
  int maxRanksThatCanBeUsedAsAdministors
):
  _joinsAllowed(joinsAllowed),
  _criticalWorker(),
  _maxForksOnCriticalWorker(THREE_POWER_D) {
  _workerCouldNotEraseDueToDecomposition.insert( std::pair<int,bool>(tarch::parallel::Node::getInstance().getRank(), false) );
  _regularLevelAlongBoundary = coarsestRegularInnerAndOuterGridLevel;

  if (_loadBalancingTag<0) {
    _loadBalancingTag = tarch::parallel::Node::reserveFreeTag("mpibalancing::HotspotBalancing");
    assertion(_loadBalancingTag>=0);
    logInfo( "HotspotBalancing(bool,int,int)", "reserved tag " << _loadBalancingTag << " for load balancing" );
  }

  if (_finestLevelWhereRanksArePurelyAdministrative<=0) {
    int administrativeRanks = 1;
    _finestLevelWhereRanksArePurelyAdministrative = 0;

    while (
      _finestLevelWhereRanksArePurelyAdministrative < _regularLevelAlongBoundary-1
      &&
      administrativeRanks < maxRanksThatCanBeUsedAsAdministors
    ) {
      _finestLevelWhereRanksArePurelyAdministrative++;
      administrativeRanks *= THREE_POWER_D;
    }
    logInfo(
      "HotspotBalancing(bool,int,int)",
      "up to level " << _finestLevelWhereRanksArePurelyAdministrative <<
      " all ranks shall become purely administrative ranks as up to " <<
      maxRanksThatCanBeUsedAsAdministors << " were allowed to be admins"
    );
  }
}


mpibalancing::HotspotBalancing::HotspotBalancing(bool joinsAllowed):
  _joinsAllowed(joinsAllowed),
  _criticalWorker(),
  _maxForksOnCriticalWorker(THREE_POWER_D) {
  _workerCouldNotEraseDueToDecomposition.insert( std::pair<int,bool>(tarch::parallel::Node::getInstance().getRank(), false) );
}


mpibalancing::HotspotBalancing::~HotspotBalancing() {
}


double mpibalancing::HotspotBalancing::getMaximumWeightOfWorkers() {
  double maximumWeight = std::numeric_limits<double>::min();
  for ( std::map<int,double>::const_iterator p=_weightMap.begin(); p!=_weightMap.end(); p++ ) {
    if ( p->second > maximumWeight ) {
      maximumWeight = p->second;
    }
  }
  return maximumWeight;
}


double mpibalancing::HotspotBalancing::getMinimumWeightOfWorkers() {
  double minimumWeight  = std::numeric_limits<double>::max();
  for ( std::map<int,double>::const_iterator p=_weightMap.begin(); p!=_weightMap.end(); p++ ) {
    if ( p->second < minimumWeight ) {
      minimumWeight = p->second;
    }
  }
  return minimumWeight;
}


void mpibalancing::HotspotBalancing::identifyCriticalPathes( peano::parallel::loadbalancing::LoadBalancingFlag commandFromMaster ) {
  /**
   * We consider the CritialPathThreshold upper percent of the workers to be
   * critical.
   */
  const double CritialPathThreshold = 0.1;

  _criticalWorker.clear();
  assertion(_criticalWorker.size()==0);

  if (
    (
    commandFromMaster>=peano::parallel::loadbalancing::LoadBalancingFlag::ForkOnce ||
    commandFromMaster==peano::parallel::loadbalancing::LoadBalancingFlag::Continue
    )
    &&
    !_forkHasFailed
  ) {
    double maximumWeight = getMaximumWeightOfWorkers();

    for ( std::map<int,double>::const_iterator p=_weightMap.begin(); p!=_weightMap.end(); p++ ) {
      if ( p->second >= (1.0-CritialPathThreshold) * maximumWeight ) {
        _criticalWorker.insert(p->first);
        assertion(_criticalWorker.size()>0);
      }
    }
  }
}


void mpibalancing::HotspotBalancing::computeMaxForksOnCriticalWorker( peano::parallel::loadbalancing::LoadBalancingFlag commandFromMaster ) {
  if ( !_criticalWorker.empty() ) {
    _maxForksOnCriticalWorker =
      static_cast<int>(std::ceil(
        static_cast<int>(commandFromMaster)*( 1.0-getMinimumWeightOfWorkers()/getMaximumWeightOfWorkers() )
      ));

    if ( _maxForksOnCriticalWorker>static_cast<int>(commandFromMaster) ) {
      _maxForksOnCriticalWorker = static_cast<int>(commandFromMaster);
      logDebug( "receivedStartCommand(LoadBalancingFlag)", "manually reduced forks to " << _maxForksOnCriticalWorker << " due to restriction from master" );
    }
    else if (_maxForksOnCriticalWorker>static_cast<int>(peano::parallel::loadbalancing::LoadBalancingFlag::ForkGreedy)) {
      _maxForksOnCriticalWorker = static_cast<int>(peano::parallel::loadbalancing::LoadBalancingFlag::ForkGreedy)/2;
    }
    else if (_maxForksOnCriticalWorker<=0) {
      _maxForksOnCriticalWorker = 1;
    }

    if (_maxForksOnCriticalWorker>0 && !_forkHasFailed) {
      logInfo(
        "computeMaxForksOnCriticalWorker(LoadBalancingFlag)",
        _maxForksOnCriticalWorker << " forks should be done (-2 = continue) next on " << _criticalWorker.size() <<
        " worker(s) given the master fork command " << peano::parallel::loadbalancing::convertLoadBalancingFlagToString(commandFromMaster) <<
        ". Number of weight entries (workers+1)=" << _weightMap.size() <<
        ". Fork has failed before and vetos forks=" << _forkHasFailed <<
        ". max weight=" << getMaximumWeightOfWorkers() <<
        ". min weight=" << getMinimumWeightOfWorkers() <<
        ". Load balancing is activated=" << peano::parallel::loadbalancing::Oracle::getInstance().isLoadBalancingActivated()
      );
      std::ostringstream msg;
      msg << "critical workers are/is";
      for ( auto p: _criticalWorker ) {
        if (p!=*_criticalWorker.begin()) {
          msg << ",";
        }
        msg << " (" << p << "," << _weightMap[p] << ")";
      }
      msg << ". local weight= " << _weightMap[tarch::parallel::Node::getInstance().getRank()];
      logInfo( "identifyCriticalPathes(LoadBalancingFlag)", msg.str() );
    }
  }
  else {
    logInfo( "computeMaxForksOnCriticalWorker(LoadBalancingFlag)", "set of critical workers is empty" );
  }
}


void mpibalancing::HotspotBalancing::receivedStartCommand( peano::parallel::loadbalancing::LoadBalancingFlag commandFromMaster ) {
  logTraceInWith1Argument("receivedStartCommand(LoadBalancingFlag)", peano::parallel::loadbalancing::convertLoadBalancingFlagToString(commandFromMaster));

  identifyCriticalPathes( commandFromMaster );
  computeMaxForksOnCriticalWorker( commandFromMaster );

  logTraceOut("receivedStartCommand(LoadBalancingFlag)" );
}


peano::parallel::loadbalancing::LoadBalancingFlag  mpibalancing::HotspotBalancing::getCommandForWorker( int workerRank, bool forkIsAllowed, bool joinIsAllowed ) {
  logTraceInWith4Arguments( "getCommandForWorker(int,bool)", workerRank, forkIsAllowed, joinIsAllowed, _joinsAllowed );
  
  peano::parallel::loadbalancing::LoadBalancingFlag  result = peano::parallel::loadbalancing::LoadBalancingFlag::Continue;

  if (
    tarch::parallel::Node::getInstance().isGlobalMaster()
    &&
    forkIsAllowed
  ) {
    result = peano::parallel::loadbalancing::LoadBalancingFlag::ForkAllChildrenAndBecomeAdministrativeRank;
  }
  else if (_joinsAllowed && _workerCouldNotEraseDueToDecomposition[workerRank] && joinIsAllowed) {
    _forkHasFailed = false;
    result         = peano::parallel::loadbalancing::LoadBalancingFlag::Join;
  }
  else if (_joinsAllowed && _workerCouldNotEraseDueToDecomposition[workerRank] && !joinIsAllowed) {
    _forkHasFailed = false;
    result         = peano::parallel::loadbalancing::LoadBalancingFlag::ContinueButTryToJoinWorkers;
  }
  else if (
    _criticalWorker.count(workerRank)>0 &&
    forkIsAllowed &&
    !_forkHasFailed &&
    _maxForksOnCriticalWorker==0
  ) {
    result = peano::parallel::loadbalancing::LoadBalancingFlag::Continue;
    assertion( result!=peano::parallel::loadbalancing::LoadBalancingFlag::UndefinedLoadBalancingFlag );
  }
  else if (
    _workerShouldBecomeAdministrativeRank.count(workerRank) > 0 &&
    _workerShouldBecomeAdministrativeRank[workerRank] &&
    _criticalWorker.count(workerRank)>0 &&
    forkIsAllowed &&
    !_forkHasFailed
  ) {
    logInfo( "getCommandForWorker(int)", "worker " << workerRank << " is responsible for coarse level and thus should become purely administrative rank" );
    result = peano::parallel::loadbalancing::LoadBalancingFlag::ForkAllChildrenAndBecomeAdministrativeRank;
  }
  else if (
    _criticalWorker.count(workerRank)>0 &&
    forkIsAllowed &&
    !_forkHasFailed
  ) {
    result = static_cast<peano::parallel::loadbalancing::LoadBalancingFlag>(_maxForksOnCriticalWorker);
    assertion( result!=peano::parallel::loadbalancing::LoadBalancingFlag::UndefinedLoadBalancingFlag );
  }

  assertion( result!=peano::parallel::loadbalancing::LoadBalancingFlag::UndefinedLoadBalancingFlag );
  logTraceOutWith1Argument( "getCommandForWorker(int,bool)", static_cast<int>(result) );
  return result;
}


void mpibalancing::HotspotBalancing::mergeWithMaster(
  int     workerRank,
  bool    workerCouldNotEraseDueToDecomposition,
  int     coarsestWorkerRankLevel
) {
  #ifdef Parallel
  logDebug( "mergeWithMaster(int,bool,int)", "receive load balancing information on tag " << _loadBalancingTag << " from worker " << workerRank );

  if (coarsestWorkerRankLevel<=_finestLevelWhereRanksArePurelyAdministrative) {
    _workerShouldBecomeAdministrativeRank[workerRank] = true;
  }
  else {
    _workerShouldBecomeAdministrativeRank[workerRank] = false;
  }

  double workerWeight;

  if (UseBlockingSendAndReceive) {
    MPI_Recv( &workerWeight, 1, MPI_DOUBLE, workerRank, _loadBalancingTag, tarch::parallel::Node::getInstance().getCommunicator(), MPI_STATUS_IGNORE );
  }
  else {
    MPI_Request* sendRequestHandle = new MPI_Request();
    MPI_Status   status;
    int          flag = 0;
    clock_t      timeOutWarning   = -1;
    clock_t      timeOutShutdown  = -1;
    bool         triggeredTimeoutWarning = false;
    MPI_Irecv(
      &workerWeight, 1, MPI_DOUBLE, workerRank, _loadBalancingTag,
      tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
    );
    MPI_Test( sendRequestHandle, &flag, &status );
    while (!flag) {
      if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
      if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
      MPI_Test( sendRequestHandle, &flag, &status );
      if (
        tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
        (clock()>timeOutWarning) &&
        (!triggeredTimeoutWarning)
      ) {
        tarch::parallel::Node::getInstance().writeTimeOutWarning(
          "mpibalancing::HotspotBalancing",
          "mergeWithMaster(...)", workerRank,_loadBalancingTag,1
        );
        triggeredTimeoutWarning = true;
      }
      if (
        tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
        (clock()>timeOutShutdown)
      ) {
        tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
          "mpibalancing::HotspotBalancing",
          "mergeWithMaster(...)",workerRank,_loadBalancingTag,1
        );
      }
      tarch::parallel::Node::getInstance().receiveDanglingMessages();
    }
    delete sendRequestHandle;
  }

  _workerCouldNotEraseDueToDecomposition[workerRank] = workerCouldNotEraseDueToDecomposition;
  _weightMap[workerRank]                             = workerWeight > 1.0 ? workerWeight : 1.0;
  #endif
}


void mpibalancing::HotspotBalancing::setLocalWeightAndNotifyMaster(
  double localWeight
) {
  _weightMap[tarch::parallel::Node::getInstance().getRank()] = localWeight;
  #ifdef Parallel
  assertion(_loadBalancingTag>=0);

  double ranksWeight = getMaximumWeightOfWorkers();

  logDebug( "HotspotBalancing(bool,int)", "send load balancing information on tag " << _loadBalancingTag << " to master " << tarch::parallel::NodePool::getInstance().getMasterRank() );
  if (UseBlockingSendAndReceive) {
    MPI_Send( &ranksWeight, 1, MPI_DOUBLE, tarch::parallel::NodePool::getInstance().getMasterRank(), _loadBalancingTag, tarch::parallel::Node::getInstance().getCommunicator() );
  }
  else {
    MPI_Request* sendRequestHandle = new MPI_Request();
    MPI_Status   status;
    int          flag = 0;
    clock_t      timeOutWarning   = -1;
    clock_t      timeOutShutdown  = -1;
    bool         triggeredTimeoutWarning = false;
    MPI_Isend(
      &ranksWeight, 1, MPI_DOUBLE, tarch::parallel::NodePool::getInstance().getMasterRank(), _loadBalancingTag,
      tarch::parallel::Node::getInstance().getCommunicator(), sendRequestHandle
    );
    MPI_Test( sendRequestHandle, &flag, &status );
    while (!flag) {
      if (timeOutWarning==-1)   timeOutWarning   = tarch::parallel::Node::getInstance().getDeadlockWarningTimeStamp();
      if (timeOutShutdown==-1)  timeOutShutdown  = tarch::parallel::Node::getInstance().getDeadlockTimeOutTimeStamp();
      MPI_Test( sendRequestHandle, &flag, &status );
      if (
        tarch::parallel::Node::getInstance().isTimeOutWarningEnabled() &&
        (clock()>timeOutWarning) &&
        (!triggeredTimeoutWarning)
      ) {
        tarch::parallel::Node::getInstance().writeTimeOutWarning(
          "mpibalancing::HotspotBalancing",
          "setLocalWeightAndNotifyMaster(double)", tarch::parallel::NodePool::getInstance().getMasterRank(), _loadBalancingTag, 1
        );
        triggeredTimeoutWarning = true;
      }
      if (
        tarch::parallel::Node::getInstance().isTimeOutDeadlockEnabled() &&
        (clock()>timeOutShutdown)
      ) {
        tarch::parallel::Node::getInstance().triggerDeadlockTimeOut(
          "mpibalancing::HotspotBalancing",
          "setLocalWeightAndNotifyMaster(double)", tarch::parallel::NodePool::getInstance().getMasterRank(),_loadBalancingTag,1
        );
      }
      tarch::parallel::Node::getInstance().receiveDanglingMessages();
    }
    delete sendRequestHandle;
  }
  #endif
}


void mpibalancing::HotspotBalancing::plotStatistics() {
}


peano::parallel::loadbalancing::OracleForOnePhase* mpibalancing::HotspotBalancing::createNewOracle(int adapterNumber) const {
  return new HotspotBalancing(_joinsAllowed);
}


void mpibalancing::HotspotBalancing::forkFailed() {
  if (!_forkHasFailed) {
    logInfo(
      "forkFailed()",
      "oracle was informed that fork has failed. No further fork attempts in this iteration"
    );
  }
  _forkHasFailed = true;
}


int mpibalancing::HotspotBalancing::getRegularLevelAlongBoundary() const {
  return _regularLevelAlongBoundary;
}
