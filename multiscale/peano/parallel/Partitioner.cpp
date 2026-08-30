#include "peano/parallel/Partitioner.h"

#include "peano/parallel/messages/LoadBalancingMessage.h"
#include "peano/parallel/messages/ForkMessage.h"

#include "tarch/Assertions.h"
#include "peano/utils/Loop.h"

#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"

#include "peano/parallel/loadbalancing/Oracle.h"



void peano::parallel::Partitioner::initDatatypes() {
  #ifdef Parallel
  if (peano::parallel::messages::ForkMessage::Datatype==0) {
    peano::parallel::messages::ForkMessage::initDatatype();
    peano::parallel::messages::LoadBalancingMessage::initDatatype();
  }
  #endif
}


void peano::parallel::Partitioner::shutdownDatatypes() {
  #ifdef Parallel
  if (peano::parallel::messages::ForkMessage::Datatype!=0) {
    peano::parallel::messages::ForkMessage::shutdownDatatype();
    peano::parallel::messages::LoadBalancingMessage::shutdownDatatype();

    peano::parallel::messages::ForkMessage::Datatype = 0;
    peano::parallel::messages::LoadBalancingMessage::Datatype = 0;
  }
  #endif
}




tarch::logging::Log peano::parallel::Partitioner::_log( "peano::parallel::Partitioner" );



peano::parallel::Partitioner::Partitioner( const std::bitset<THREE_POWER_D>& localCellsOfPatch ):
  _ranks(),
  _localCellsOfPatch( localCellsOfPatch ),
  _assignedNodes(THREE_POWER_D) {
  assertion2(_localCellsOfPatch.count()>0,_localCellsOfPatch,tarch::parallel::Node::getInstance().getRank());
}


peano::parallel::Partitioner::~Partitioner() {
}


int peano::parallel::Partitioner::getNumberOfReservedWorkers() const {
  return static_cast<int>(_ranks.size())-1;
}


void peano::parallel::Partitioner::reserveNodes() {
  logTraceInWith1Argument( "reserveNodes()", _localCellsOfPatch );

  #ifdef Parallel
  assertionEquals( _ranks.size(), 0 );
  assertion(peano::parallel::messages::ForkMessage::Datatype!=0);
  #endif

  const int NodesToBook = static_cast<int>(peano::parallel::loadbalancing::Oracle::getInstance().getLastStartCommand());
        int LocalCells  = static_cast<int>(_localCellsOfPatch.count());

  if (NodesToBook!=static_cast<int>(peano::parallel::loadbalancing::LoadBalancingFlag::ForkAllChildrenAndBecomeAdministrativeRank)) {
    LocalCells--;
  }

  const int MaxRemoteRanksToBook = std::min( LocalCells, NodesToBook );

  if (MaxRemoteRanksToBook>0) {
    _ranks = tarch::parallel::NodePool::getInstance().reserveFreeNodes(MaxRemoteRanksToBook);

    if ( static_cast<int>(_ranks.size())!=MaxRemoteRanksToBook ) {
      peano::parallel::loadbalancing::Oracle::getInstance().forkFailed();
    }
  }

  _ranks.push_back( tarch::parallel::Node::getInstance().getRank() );

  logTraceOutWith1Argument( "reserveNodes()", _ranks.size() );
}


void peano::parallel::Partitioner::sendForkMessages(
  const tarch::la::Vector<DIMENSIONS,double>&  domainOffset,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  peano::utils::LoopDirection                  loopDirection,
  int                                          levelOfPatch,
  const std::bitset<DIMENSIONS>&               bitfieldOfCoarseLevelLevel
) {
  logTraceInWith4Arguments( "sendForkMessages(...)", domainOffset, h, loopDirection, _ranks.size() );

  int assignedRemoteRanks = 0;

  if ( _ranks[0] < tarch::parallel::Node::getInstance().getRank() ) {
    // Not sure whether this works
    loopDirection.flip();
  }

  zfor3(k,loopDirection)
    const int  CellIndex = peano::utils::dLinearised(k,3);
    const bool OutsourceCell =
      assignedRemoteRanks < static_cast<int>( _ranks.size()-1 ) &&
      _localCellsOfPatch[CellIndex];
    if (OutsourceCell) {
      peano::parallel::messages::ForkMessage message;

      message.setH(h);
      message.setPositionOfFineGridCellRelativeToCoarseGridCell(k);

      std::bitset<DIMENSIONS> evenFlags = bitfieldOfCoarseLevelLevel;
      for (int d=0; d<DIMENSIONS; d++) {
        if (k(d)==1) evenFlags.flip(d);
      }
      message.setLevel(levelOfPatch+1);

      tarch::la::Vector<DIMENSIONS,double>  remoteDomainOffset;
      for (int d=0; d<DIMENSIONS; d++) {
        remoteDomainOffset(d)  = domainOffset(d) + static_cast<double>(k(d)) * h(d);
      }
      message.setDomainOffset(remoteDomainOffset);

      peano::parallel::loadbalancing::Oracle::getInstance().addWorker(
        _ranks[assignedRemoteRanks],
        message.getDomainOffset(),
        h,
        levelOfPatch+1
      );
      #ifdef Parallel
      message.send(
        _ranks[assignedRemoteRanks],
        tarch::parallel::NodePool::getInstance().getTagForForkMessages(),
        true, peano::parallel::messages::ForkMessage::ExchangeMode::NonblockingWithPollingLoopOverTests
      );
      #endif
      _assignedNodes[CellIndex] = _ranks[assignedRemoteRanks];
      assignedRemoteRanks++;
    }
    else {
      _assignedNodes[CellIndex] = tarch::parallel::Node::getInstance().getRank();
    }
  endzfor

  logTraceOutWith1Argument( "sendForkMessages(...)", getPartitiongDescription() );
}


int peano::parallel::Partitioner::getRankOfWorkerReponsibleForCell( tarch::la::Vector<DIMENSIONS,int> cellIndex ) const {
  int cellNumber = peano::utils::dLinearised(cellIndex,3);
  assertion( cellNumber>=0 );
  assertion( cellNumber<THREE_POWER_D );
  return _assignedNodes[cellNumber];
}


std::string peano::parallel::Partitioner::getPartitiongDescription() const {
  std::ostringstream result;
  result << "(";

  for (int i=0; i<THREE_POWER_D; i++) {
    if (i>0) result << ",";
    result << tarch::parallel::Node::getInstance().getRank()
           << "->"
           << tarch::parallel::Node::getInstance().getRank()
           << "+"
           << _assignedNodes[i];
  }

  result << ")";
  return result.str();
}


bool peano::parallel::Partitioner::hasSuccessfullyReservedAdditionalWorkers() const {
  assertion(_ranks.size()>0);

  return _ranks.size()!=1;
}


int peano::parallel::Partitioner::getNumberOfSuccessfullyReservedAdditionalWorkers() const {
  assertion(_ranks.size()>0);

  return static_cast<int>(_ranks.size())-1;
}
