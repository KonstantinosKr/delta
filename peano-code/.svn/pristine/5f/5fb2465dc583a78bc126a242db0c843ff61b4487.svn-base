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
  _startCommand(Continue),
  _loadBalancingActivated(true) {
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
  if (_startCommand>ForkOnce) {
    _startCommand = _startCommand-1;
  }
  else if (_startCommand==ForkOnce) {
    _startCommand = Continue;
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
  int i=0;
  for (
    OracleDatabase::iterator p = _oracles.begin();
    p != _oracles.end();
    p++
  ) {
    i++;

    if (i>peano::datatraversal::autotuning::NumberOfPredefinedAdapters) {
      (*p)->plotStatistics();
    }
  }
}


void peano::parallel::loadbalancing::Oracle::masterStartsToWaitForWorkers() {
  peano::performanceanalysis::Analysis::getInstance().beginToReceiveDataFromWorker();
}


void peano::parallel::loadbalancing::Oracle::setNumberOfOracles(int value) {
  assertion( value>0 );
  deleteOracles();
  createOracles(value);
}


void peano::parallel::loadbalancing::Oracle::switchToOracle(int id) {
  assertion1( id>=0, id );
  assertion2( id<static_cast<int>(_oracles.size()), id, _oracles.size() );

  _currentOracle=id;
}


void peano::parallel::loadbalancing::Oracle::setOracle( OracleForOnePhase* oraclePrototype ) {
  assertion( oraclePrototype!=0 );

  if (_oraclePrototype!=0) {
    delete _oraclePrototype;
  }
  _oraclePrototype = oraclePrototype;

  int numberOfOracles = static_cast<int>(_oracles.size());
  deleteOracles();
  createOracles(numberOfOracles);
}


void peano::parallel::loadbalancing::Oracle::receivedStartCommand( int  commandFromMaster ) {
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  assertion2( commandFromMaster!=Join || _workers.empty(),   _workers.size(), tarch::parallel::Node::getInstance().getRank() );
  assertion2( commandFromMaster!=UndefinedLoadBalancingFlag, _workers.size(), tarch::parallel::Node::getInstance().getRank() );

  if (_oraclePrototype==0) {
    logWarning( "createOracles(int)", "no oracle type configured. Perhaps forgot to call peano::kernel::loadbalancing::Oracle::setOracle()" );
  }
  else {
    _oracles[_currentOracle]->receivedStartCommand(commandFromMaster);
  }

  _startCommand = commandFromMaster;
}


int peano::parallel::loadbalancing::Oracle::getCoarsestRegularInnerAndOuterGridLevel() const {
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  return  _oracles[_currentOracle]->getCoarsestRegularInnerAndOuterGridLevel();
}


int peano::parallel::loadbalancing::Oracle::getLastStartCommand() const {
  assertion( _startCommand!=UndefinedLoadBalancingFlag );
  return _startCommand;
}



void peano::parallel::loadbalancing::Oracle::forkFailed() {
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  if (_oraclePrototype==0) {
    logWarning( "createOracles(int)", "no oracle type configured. Perhaps forgot to call peano::kernel::loadbalancing::Oracle::setOracle()" );
  }
  else {
    _oracles[_currentOracle]->forkFailed();
    if (_startCommand>=ForkOnce) {
      _startCommand = Continue;
    }
  }
}


int peano::parallel::loadbalancing::Oracle::getCommandForWorker(
  int workerRank,
  bool forkIsAllowed,
  bool joinIsAllowed
) {
  logTraceInWith1Argument( "getCommandForWorker(int)", workerRank );

  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));

  joinIsAllowed &= !tarch::parallel::Node::getInstance().isGlobalMaster();

  int result = UndefinedLoadBalancingFlag;
  if (_oraclePrototype==0) {
    logWarning( "createOracles(int)", "no oracle type configured. Perhaps forgot to call peano::kernel::loadbalancing::Oracle::setOracle()" );
    result = peano::parallel::loadbalancing::Continue;
  }
  else {
    result = _oracles[_currentOracle]->getCommandForWorker(
      workerRank,
      forkIsAllowed & _loadBalancingActivated,
      joinIsAllowed & _loadBalancingActivated
    );
  }

  assertion5( result!=UndefinedLoadBalancingFlag, workerRank, forkIsAllowed, joinIsAllowed, (_oraclePrototype==0), _loadBalancingActivated );

  logTraceOutWith1Argument( "getCommandForWorker(int)", convertLoadBalancingFlagToString(result) );
  return result;
}


void peano::parallel::loadbalancing::Oracle::activateLoadBalancing(bool value) {
  _loadBalancingActivated = value;
}


bool peano::parallel::loadbalancing::Oracle::isLoadBalancingActivated() const {
  return _loadBalancingActivated;
}


void peano::parallel::loadbalancing::Oracle::receivedTerminateCommand(
  int     workerRank,
  double  workerNumberOfInnerVertices,
  double  workerNumberOfBoundaryVertices,
  double  workerNumberOfOuterVertices,
  double  workerNumberOfInnerCells,
  double  workerNumberOfOuterCells,
  int     workerMaxLevel,
  int     currentLevel,
  const tarch::la::Vector<DIMENSIONS,double>& boundingBoxOffset,
  const tarch::la::Vector<DIMENSIONS,double>& boundingBoxSize,
  bool    workerCouldNotEraseDueToDecomposition
) {
  assertion( _currentOracle>=0 );
  assertion( _currentOracle<static_cast<int>(_oracles.size()));


  _oracles[_currentOracle]->receivedTerminateCommand(
    workerRank,
    workerNumberOfInnerVertices,
    workerNumberOfBoundaryVertices,
    workerNumberOfOuterVertices,
    workerNumberOfInnerCells,
    workerNumberOfOuterCells,
    workerMaxLevel,
    currentLevel,
    boundingBoxOffset,
    boundingBoxSize,
    workerCouldNotEraseDueToDecomposition
  );

  peano::performanceanalysis::Analysis::getInstance().endToReceiveDataFromWorker(workerRank);
}


void peano::parallel::loadbalancing::Oracle::createOracles(int numberOfOracles) {
  assertion( _oracles.size()==0 );

  if (_oraclePrototype==0) {
    logWarning( "createOracles(int)", "no oracle type configured. Perhaps forgot to call peano::kernel::loadbalancing::Oracle::setOracle()" );
  }
  else {
    assertion( _oraclePrototype!=0 );
    for (int i=0; i<numberOfOracles; i++) {
      OracleForOnePhase* newOracle = _oraclePrototype->createNewOracle(i);
      _oracles.push_back( newOracle );
    }
  }
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
