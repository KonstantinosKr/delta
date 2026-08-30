#include "peano/parallel/loadbalancing/Oracle.h"
#include "peano/datatraversal/autotuning/OracleForOnePhase.h"
#include "tarch/parallel/Node.h"
#include "tarch/Assertions.h"
#include "peano/performanceanalysis/Analysis.h"


tarch::logging::Log  peano::parallel::loadbalancing::Oracle::_log( "peano::parallel::loadbalancing::Oracle" );


peano::parallel::loadbalancing::Oracle& peano::parallel::loadbalancing::Oracle::getInstance() {
  static peano::parallel::loadbalancing::Oracle singleton;
  return singleton;
}


peano::parallel::loadbalancing::Oracle::Oracle():
  _currentOracle(-1),
  _oraclePrototype(0),
  _workers(),
  _startCommand(peano::parallel::loadbalancing::LoadBalancingFlag::ForkAllChildrenAndBecomeAdministrativeRank),
  _loadBalancingActivated(true),
  _numberOfOracles(0) {
}


peano::parallel::loadbalancing::Oracle::~Oracle() {
  plotStatistics();
  deleteOracles();
  delete _oraclePrototype;
  _oraclePrototype = 0;
}


bool peano::parallel::loadbalancing::Oracle::workersListContainsRank( int rank ) const {
  for (
    WorkerContainer::const_iterator p = _workers.begin();
    p!= _workers.end();
    p++
  ) {
    if (p->_persistentRecords._rank == rank) return true;
  }

  return false;
}


void peano::parallel::loadbalancing::Oracle::addWorker(
  int                                          rank,
  const tarch::la::Vector<DIMENSIONS,double>&  boundingBoxOffset,
  const tarch::la::Vector<DIMENSIONS,double>&  boundingBoxSize,
  int                                          level
) {
  assertion4( !workersListContainsRank(rank), rank, boundingBoxOffset, boundingBoxSize, level );
  assertion4( level>=0, rank, boundingBoxOffset, boundingBoxSize, level );
  _workers.push_back( WorkerEntry(rank, level, boundingBoxOffset, boundingBoxSize) );
  if (static_cast<int>(_startCommand)>static_cast<int>(peano::parallel::loadbalancing::LoadBalancingFlag::ForkOnce)) {
    _startCommand = static_cast<LoadBalancingFlag>(static_cast<int>(_startCommand)-1);
  }
  else if (_startCommand==peano::parallel::loadbalancing::LoadBalancingFlag::ForkOnce) {
    _startCommand = LoadBalancingFlag::Continue;
  }

  peano::performanceanalysis::Analysis::getInstance().addWorker(rank,level);
}


void peano::parallel::loadbalancing::Oracle::removeWorker(int rank) {
  assertion( workersListContainsRank(rank) );

  for (
    WorkerContainer::iterator p = _workers.begin();
    p!= _workers.end();
    p++
  ) {
    if (p->_persistentRecords._rank == rank) {
      peano::performanceanalysis::Analysis::getInstance().removeWorker(
        p->_persistentRecords._rank,
        p->_persistentRecords._level
      );
      _workers.erase( p );
      return;
    }
  }

  assertion(false);
}


peano::parallel::loadbalancing::Oracle::WorkerContainer::iterator peano::parallel::loadbalancing::Oracle::getWorkerRecord( int rank ) {
  assertion1( workersListContainsRank(rank), rank );

  for (
    WorkerContainer::iterator p = _workers.begin();
    p!= _workers.end();
    p++
  ) {
    if (p->_persistentRecords._rank == rank) {
      return p;
    }
  }

  assertionMsg( false, "worker not found" );
  return _workers.begin();
}


peano::parallel::loadbalancing::Oracle::WorkerContainer::const_iterator peano::parallel::loadbalancing::Oracle::getWorkerRecord( int rank ) const {
  assertion1( workersListContainsRank(rank), rank );

  for (
    WorkerContainer::const_iterator p = _workers.begin();
    p!= _workers.end();
    p++
  ) {
    if (p->_persistentRecords._rank == rank) {
      return p;
    }
  }

  assertionMsg( false, "worker not found" );
  return _workers.begin();
}


bool peano::parallel::loadbalancing::Oracle::hasWorkers() const {
  return !_workers.empty();
}


int peano::parallel::loadbalancing::Oracle::getWorkerRank(int index) const {
  assertion( _workers.size()>0 );
  assertion( index>=0 );
  assertion( index < getNumberOfWorkers());
  return _workers[index]._persistentRecords._rank;
}


tarch::la::Vector<DIMENSIONS,double>  peano::parallel::loadbalancing::Oracle::getBoundingBoxOffset( int workerRank ) const {
  assertion1( workersListContainsRank(workerRank), workerRank );
  return getWorkerRecord( workerRank )->_persistentRecords._boundingBoxOffset;
}


tarch::la::Vector<DIMENSIONS,double>  peano::parallel::loadbalancing::Oracle::getBoundingBoxSizeOfWorker( int workerRank ) const {
  assertion1( workersListContainsRank(workerRank), workerRank );
  return getWorkerRecord( workerRank )->_persistentRecords._boundingBoxSize;
}


int peano::parallel::loadbalancing::Oracle::getNumberOfWorkers() const {
  return static_cast<int>( _workers.size() );
}


void peano::parallel::loadbalancing::Oracle::plotStatistics() {
  for (auto oracle: _oracles) {
    oracle->plotStatistics();
  }
}


void peano::parallel::loadbalancing::Oracle::setNumberOfOracles(int value) {
  assertion( value>0 );

  _numberOfOracles = value;

  deleteOracles();
  createOracles();
}


void peano::parallel::loadbalancing::Oracle::switchToOracle(int id) {
  assertion1( id>=0, id );
  assertion3( id<static_cast<int>(_oracles.size()), id, _oracles.size(), _numberOfOracles);

  _currentOracle=id;
}


void peano::parallel::loadbalancing::Oracle::setOracle( OracleForOnePhase* oraclePrototype ) {
  assertion( oraclePrototype!=0 );

  if (_oraclePrototype!=0) {
    delete _oraclePrototype;
  }
  _oraclePrototype = oraclePrototype;

  deleteOracles();
  createOracles();
}


void peano::parallel::loadbalancing::Oracle::receivedStartCommand( peano::parallel::loadbalancing::LoadBalancingFlag  commandFromMaster ) {
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  assertion2( commandFromMaster!=LoadBalancingFlag::Join || _workers.empty(),   _workers.size(), tarch::parallel::Node::getInstance().getRank() );
  assertion2( commandFromMaster!=LoadBalancingFlag::UndefinedLoadBalancingFlag, _workers.size(), tarch::parallel::Node::getInstance().getRank() );

  if (_oraclePrototype==0) {
    logWarning( "receivedStartCommand(int)", "no oracle type configured. Perhaps forgot to call peano::kernel::loadbalancing::Oracle::setOracle()" );
  }
  else {
    _oracles[_currentOracle]->receivedStartCommand(commandFromMaster);
  }

  _startCommand = commandFromMaster;
}


int peano::parallel::loadbalancing::Oracle::getRegularLevelAlongBoundary() const {
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  return  _oracles[_currentOracle]->getRegularLevelAlongBoundary();
}


peano::parallel::loadbalancing::LoadBalancingFlag peano::parallel::loadbalancing::Oracle::getLastStartCommand() const {
  assertion( _startCommand!=LoadBalancingFlag::UndefinedLoadBalancingFlag );
  return _startCommand;
}


void peano::parallel::loadbalancing::Oracle::forkFailed() {
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  if (_oraclePrototype==0) {
    logWarning( "forkFailed(int)", "no oracle type configured. Perhaps forgot to call peano::kernel::loadbalancing::Oracle::setOracle()" );
  }
  else {
    _oracles[_currentOracle]->forkFailed();
    if (_startCommand>=LoadBalancingFlag::ForkOnce) {
      _startCommand = LoadBalancingFlag::Continue;
    }
  }
}


peano::parallel::loadbalancing::LoadBalancingFlag peano::parallel::loadbalancing::Oracle::getCommandForWorker(
  int workerRank,
  bool forkIsAllowed,
  bool joinIsAllowed
) {
  logTraceInWith1Argument( "getCommandForWorker(int)", workerRank );

  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  joinIsAllowed &= !tarch::parallel::Node::getInstance().isGlobalMaster();

  peano::parallel::loadbalancing::LoadBalancingFlag result = LoadBalancingFlag::UndefinedLoadBalancingFlag;
  if (_oraclePrototype==0) {
    logWarning( "getCommandForWorker(int)", "no oracle type configured. Perhaps forgot to call peano::kernel::loadbalancing::Oracle::setOracle()" );
    result = peano::parallel::loadbalancing::LoadBalancingFlag::Continue;
  }
  else {
    result = _oracles[_currentOracle]->getCommandForWorker(
      workerRank,
      forkIsAllowed & _loadBalancingActivated,
      joinIsAllowed & _loadBalancingActivated
    );
  }

/*  assertion6( result!=LoadBalancingFlag::UndefinedLoadBalancingFlag, workerRank, forkIsAllowed, joinIsAllowed, (_oraclePrototype==0), _loadBalancingActivated, result );*/

  logTraceOutWith1Argument( "getCommandForWorker(int)", convertLoadBalancingFlagToString(result) );
  return result;
}


void peano::parallel::loadbalancing::Oracle::activateLoadBalancing(bool value) {
  _loadBalancingActivated = value;
}


bool peano::parallel::loadbalancing::Oracle::isLoadBalancingActivated() const {
  return _loadBalancingActivated;
}


void peano::parallel::loadbalancing::Oracle::createOracles() {
  logTraceIn( "createOracles()");
  assertion( _oracles.size()==0 );

  if (_numberOfOracles==0) {
    logWarning( "createOracles(int)", "no number of oracles set. Have you created repositories before?" );
  }
  else if (_oraclePrototype!=0) {
    _oracles.resize(_numberOfOracles);
    for (int i=0; i<_numberOfOracles; i++) {
      _oracles[i] = _oraclePrototype->createNewOracle(i);
    }
  }
  logTraceOut( "createOracles()");
}


void peano::parallel::loadbalancing::Oracle::deleteOracles() {
  for (
    OracleDatabase::iterator p=_oracles.begin();
    p != _oracles.end();
    p++) {
    delete *p;
  }

  _oracles.clear();
  _oracles.resize(0);
}


#ifdef Parallel
std::vector<peano::parallel::loadbalancing::WorkerEntry> peano::parallel::loadbalancing::Oracle::exchangeWorkerDataWithAllRanks(int tag, MPI_Comm communicator) {
  logTraceInWith1Argument( "exchangeWorkerDataWithAllRanks(int,Comm)", tag );
  std::vector<WorkerEntry> result( _workers );

  for (int fromRank=0; fromRank<tarch::parallel::Node::getInstance().getNumberOfNodes(); fromRank++ ) {
    if (tarch::parallel::Node::getInstance().getRank()==fromRank) {
      for (int toRank=0; toRank<tarch::parallel::Node::getInstance().getNumberOfNodes(); toRank++ ) {
        if (toRank!=fromRank) {
          logDebug( "exchangeWorkerDataWithAllRanks(int,Comm)", "send " << _workers.size() << " message(s) to rank " << toRank );
          const int result = MPI_Send( _workers.data(), _workers.size(), WorkerEntry::FullDatatype, toRank, tag, communicator);
          if  (result!=MPI_SUCCESS) {
            logError( "exchangeWorkerDataWithAllRanks(int,Comm)", "was not able to send " << _workers.size() << " message(s) to rank " << toRank << ": " << tarch::parallel::MPIReturnValueToString(result) );
          }
        }
      }
    }
    else {
      const int oldSize         = result.size();
      int       receivedEntries = 0;
      MPI_Status status;
      const int resultCount = MPI_Probe(fromRank, tag, communicator, &status);
      if  (resultCount!=MPI_SUCCESS) {
        logError( "exchangeWorkerDataWithAllRanks(int,Comm)", "was not able to identify how many messages drop in from " << fromRank << ": " << tarch::parallel::MPIReturnValueToString(resultCount) );
      }
      MPI_Get_count(&status, WorkerEntry::FullDatatype, &receivedEntries);

      assertion( receivedEntries>=0 );

      result.resize(oldSize+receivedEntries);
      const int resultRecv = MPI_Recv(&(result[oldSize]),receivedEntries,WorkerEntry::FullDatatype,fromRank,tag,communicator,&status);
      if  (resultRecv!=MPI_SUCCESS) {
        logError( "exchangeWorkerDataWithAllRanks(int,Comm)", "was not able to receive " << receivedEntries << " message(s) frpm rank " << fromRank << ": " << tarch::parallel::MPIReturnValueToString(resultRecv) );
      }
      logDebug( "exchangeWorkerDataWithAllRanks(int,Comm)", "received " << receivedEntries << " message(s) from rank " << fromRank << ". size(result)=" << result.size() );
    }
  }

  logTraceOutWith1Argument( "exchangeWorkerDataWithAllRanks(int,Comm)", result.size() );
  return result;
}
#endif
