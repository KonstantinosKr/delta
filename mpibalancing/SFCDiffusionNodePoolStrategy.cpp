#include "mpibalancing/SFCDiffusionNodePoolStrategy.h"

#include "tarch/parallel/Node.h"
#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/parallel/NodePool.h"

#include "peano/utils/Globals.h"

#include <sstream>
#include <limits>
#include <map>


tarch::logging::Log mpibalancing::SFCDiffusionNodePoolStrategy::_log( "mpibalancing::SFCDiffusionNodePoolStrategy" );


mpibalancing::SFCDiffusionNodePoolStrategy::SFCDiffusionNodePoolStrategy(int mpiRanksPerNode, int primaryMPIRanksPerNode, double waitTimeOutSec):
  NodePoolStrategy(),
  _tag(-1),
  _nodes(),
  _waitTimeOut(waitTimeOutSec),
  _mpiRanksPerNode(mpiRanksPerNode),
  _primaryMPIRanksPerNode(primaryMPIRanksPerNode),
  _numberOfPrimaryRanksPerNodeThatAreCurrentlyDeployed(_primaryMPIRanksPerNode),
  _numberOfNodesToSkipPerRequestPlusOne(1),
  _nodePoolState(NodePoolState::DeployingIdlePrimaryRanks) {

  assertion( mpiRanksPerNode>0 );
  assertion( primaryMPIRanksPerNode<=mpiRanksPerNode );
  assertion( (mpiRanksPerNode%primaryMPIRanksPerNode==0) );

  _totalNumberOfRanks = tarch::parallel::Node::getInstance().getNumberOfNodes();
}


mpibalancing::SFCDiffusionNodePoolStrategy::~SFCDiffusionNodePoolStrategy() {
}


void mpibalancing::SFCDiffusionNodePoolStrategy::fillWorkerRequestQueue(RequestQueue& queue) {
  switch (_nodePoolState) {
    case NodePoolState::DeployingAlsoSecondaryRanks:
    {
      if (queue.empty()) {
        logInfo(
          "fillWorkerRequestQueue(RequestQueue)",
          "running out of ranks. Answered all pending MPI questions before but new requests keep on dropping in. Stop to deliver MPI ranks"
        );
        _nodePoolState = NodePoolState::NoNodesLeft;
      }
      else {
        queue = sortRequestQueue( queue );
      }
    }
    break;
    case NodePoolState::NoNodesLeft:
    {
      #ifdef Parallel
      // get the messages out of the system
      while (tarch::parallel::messages::WorkerRequestMessage::isMessageInQueue(_tag, true)) {
        tarch::parallel::messages::WorkerRequestMessage message;
        message.receive(MPI_ANY_SOURCE,_tag, true, SendAndReceiveLoadBalancingMessagesBlocking);
        queue.push_back( message );
      }
      #endif
    }
    break;
    case NodePoolState::DeployingIdlePrimaryRanks:
    {
      #ifdef Parallel
      assertion( _tag >= 0 );
      std::clock_t waitTimeoutTimeStamp;
      bool continueToWait = true;
      while ( continueToWait ) {
        while (tarch::parallel::messages::WorkerRequestMessage::isMessageInQueue(_tag, true)) {
          tarch::parallel::messages::WorkerRequestMessage message;
          message.receive(MPI_ANY_SOURCE,_tag, true, SendAndReceiveLoadBalancingMessagesBlocking);
          queue.push_back( message );
          waitTimeoutTimeStamp = clock() + static_cast<std::clock_t>(std::floor(_waitTimeOut * CLOCKS_PER_SEC));
        }

       continueToWait =
         hasIdleNode(-1) &&
         (static_cast<int>(queue.size())+1 < getNumberOfRegisteredNodes()-getNumberOfIdleNodes()) &&
         (clock() < waitTimeoutTimeStamp);
      }
      #endif


      int totalNumberOfRequestedWorkers = 0;
      for (auto m: queue) {
        totalNumberOfRequestedWorkers += m.getNumberOfRequestedWorkers();
      }
      if (
        (totalNumberOfRequestedWorkers>getNumberOfIdlePrimaryRanks())
        &&
        !hasCompleteIdleNode()
      ) {
        _nodePoolState = NodePoolState::DeployingAlsoSecondaryRanks;
        logInfo(
          "fillWorkerRequestQueue(RequestQueue)",
          "have " << totalNumberOfRequestedWorkers <<
          " worker requests but only " << getNumberOfIdlePrimaryRanks() <<
          " primary node(s), i.e. code is running out of idle nodes. Start to deploy secondary nodes"
        );
        buildUpPriorityMap(queue);
        queue = sortRequestQueue( queue );
      }
      else if (totalNumberOfRequestedWorkers>0) {
        configureForPrimaryRanksDelivery(totalNumberOfRequestedWorkers);
      }
    }
    break;
  }
}


void mpibalancing::SFCDiffusionNodePoolStrategy::configureForPrimaryRanksDelivery(int totalNumberOfRequestedWorkers) {
  assertion(totalNumberOfRequestedWorkers>0);

  _numberOfPrimaryRanksPerNodeThatAreCurrentlyDeployed = std::max(1,totalNumberOfRequestedWorkers / getNumberOfPhysicalNodes());
  _numberOfNodesToSkipPerRequestPlusOne   = std::max(1,getNumberOfPhysicalNodes() / totalNumberOfRequestedWorkers);
}


void mpibalancing::SFCDiffusionNodePoolStrategy::buildUpPriorityMap(const RequestQueue& queue) {
  #ifdef Parallel
  // Compute priorities of the nodes which equals the total number of
  // requests issued through all ranks of a node.
  std::map<int, int>  nodeToPriorityMap;
  for (auto p: queue) {
    assertion( p.getSenderRank()>=0 );
    assertion( p.getSenderRank()<static_cast<int>(_nodes.size()) );

    const int nodeNumber = p.getSenderRank()/_mpiRanksPerNode;

    if (nodeToPriorityMap.count(nodeNumber)) {
      nodeToPriorityMap.insert( std::pair<int,int>(nodeNumber,0) );
    }

    nodeToPriorityMap[nodeNumber] += p.getNumberOfRequestedWorkers();
    assertion( nodeToPriorityMap[nodeNumber]>0 );
  }
  logDebug(
    "buildUpPriorityMap(Queue)",
    "node priority map has " << nodeToPriorityMap.size() << " entries. First entry=" <<
    nodeToPriorityMap.begin()->first << " with priority " << nodeToPriorityMap.begin()->second
  );

  for (auto p: queue) {
    DeploymentPriority priority;
    assertion(p.getNumberOfRequestedWorkers()>0);
    if (isFirstOrLastRankInQueueAlongSFC(p.getSenderRank(),queue)) {
      priority._maxNumberOfSecondaryRanksToBeDeployed =
        std::min(THREE_POWER_D-1,p.getNumberOfRequestedWorkers());
      priority._priority = nodeToPriorityMap[p.getSenderRank()/_mpiRanksPerNode];
      logInfo(
        "buildUpPriorityMap(Queue)",
        "configure rank " << p.getSenderRank() << ": " << priority._maxNumberOfSecondaryRanksToBeDeployed << " workers max with priority " << priority._priority
      );
    }
    else {
      priority._maxNumberOfSecondaryRanksToBeDeployed = 0;
      priority._priority = std::numeric_limits<int>::max();
      logInfo(
        "buildUpPriorityMap(Queue)",
        "disable " << p.getSenderRank()
      );
    }
    _priorities.insert( std::pair<int,DeploymentPriority>(p.getSenderRank(),priority) );
  }
  #endif
}


mpibalancing::SFCDiffusionNodePoolStrategy::RequestQueue mpibalancing::SFCDiffusionNodePoolStrategy::sortRequestQueue( const RequestQueue&  queue ) {
  assertion1( _nodePoolState == NodePoolState::DeployingAlsoSecondaryRanks, nodePoolStateToString() );
  RequestQueue result;

  for (auto p: queue) {
    result.push_back(p);
  }

  #ifdef Parallel
  // sort
  result.sort(
    [&](const tarch::parallel::messages::WorkerRequestMessage & a, const tarch::parallel::messages::WorkerRequestMessage & b) {
      return _priorities[a.getSenderRank()]._priority > _priorities[b.getSenderRank()]._priority;
    }
  );
  #endif

  assertionEquals( result.size(), queue.size() );
  return result;
}


tarch::parallel::messages::WorkerRequestMessage mpibalancing::SFCDiffusionNodePoolStrategy::extractElementFromRequestQueue(RequestQueue& queue) {
  assertion( !queue.empty() );

  RequestQueue::iterator pResultInQueue = queue.begin();
  tarch::parallel::messages::WorkerRequestMessage result = *pResultInQueue;
  queue.erase(pResultInQueue);

  return result;
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::isFirstOrLastRankInQueueAlongSFC(int rank, const RequestQueue& queue) const {
  bool thereIsASmallerRankRequestingWorkersOnThisNode = false;
  bool thereIsASmallerRankRequestingWorkers           = false;
  bool thereIsABiggerRankRequestingWorkersOnThisNode  = false;
  bool thereIsABiggerRankRequestingWorkers            = false;

  #ifdef Parallel
  const int SmallestMPIRankOfThisNode = (rank / _mpiRanksPerNode) * _mpiRanksPerNode;
  const int BiggestMPIRankOfThisNode  = (rank / _mpiRanksPerNode) * _mpiRanksPerNode + _mpiRanksPerNode-1;

  for (auto p: queue) {
    if (p.getSenderRank() < rank) {
      thereIsASmallerRankRequestingWorkers = true;
    }
    if (p.getSenderRank() < rank && p.getSenderRank()>=SmallestMPIRankOfThisNode) {
      thereIsASmallerRankRequestingWorkersOnThisNode = true;
    }
    if (p.getSenderRank() > rank) {
      thereIsABiggerRankRequestingWorkers = true;
    }
    if (p.getSenderRank() > rank && p.getSenderRank()<=BiggestMPIRankOfThisNode) {
      thereIsABiggerRankRequestingWorkersOnThisNode = true;
    }
  }
  #endif

  // exclude very first and very last request
  thereIsASmallerRankRequestingWorkersOnThisNode |= !thereIsASmallerRankRequestingWorkers;
  thereIsABiggerRankRequestingWorkersOnThisNode  |= !thereIsABiggerRankRequestingWorkers;

  const bool result =
    (!thereIsASmallerRankRequestingWorkers && !thereIsABiggerRankRequestingWorkers)
    ||
    !(thereIsASmallerRankRequestingWorkersOnThisNode && thereIsABiggerRankRequestingWorkersOnThisNode);

  logDebug( "isFirstOrLastRankInQueueAlongSFC(int,Queue)", "rank " << rank << " is first or last=" << result );

  return result;
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::isPrimaryMPIRank(int rank) const {
  const int NumberOfRanksPerPrimaryRank = _mpiRanksPerNode / _primaryMPIRanksPerNode;

  return rank % NumberOfRanksPerPrimaryRank==0;
}


void mpibalancing::SFCDiffusionNodePoolStrategy::addNode(const tarch::parallel::messages::RegisterAtNodePoolMessage& node) {
  #ifdef Parallel
  logTraceInWith1Argument( "addNode(...)", node.getSenderRank() );

  while (static_cast<int>(_nodes.size())<=node.getSenderRank()) {
    _nodes.push_back( NodePoolListEntry() );
  }

  const std::string name = tarch::parallel::StringTools::convert(node.getNodeName());
  assertion(_primaryMPIRanksPerNode!=0);
  _nodes[node.getSenderRank()] = NodePoolListEntry(
    name,
    isPrimaryMPIRank(node.getSenderRank())
  );

  logInfo( 
    "addNode(...)", 
    "added " << _nodes[node.getSenderRank()].toString() << 
    " as is-primary-rank=" << isPrimaryMPIRank(node.getSenderRank())
  );

  logTraceOutWith1Argument( "addNode(...)", _nodes[node.getSenderRank()].toString() );
  #endif
}


void mpibalancing::SFCDiffusionNodePoolStrategy::removeNode( int rank ) {
  assertion( isRegisteredNode(rank) );
  _nodes[rank] = NodePoolListEntry(); // overwrite with invalidated entry
}


int mpibalancing::SFCDiffusionNodePoolStrategy::getNumberOfIdlePrimaryRanks() const {
  int result = 0;
  for (auto node: _nodes) {
    if (node.isIdlePrimaryRank()) {
      result++;
    }
  }
  return result;
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::hasIdleNode(int forMaster) const {
  switch (_nodePoolState) {
    case NodePoolState::DeployingIdlePrimaryRanks:
      {
        for (auto node: _nodes) {
          if (node.isIdlePrimaryRank()) {
            return true;
          }
        }
      }
      break;
    case NodePoolState::DeployingAlsoSecondaryRanks:
    {
      for (auto node: _nodes) {
        if (node.isIdlePrimaryRank() || node.isIdleSecondaryRank()) {
          return true;
        }
      }
    }
    break;
    case NodePoolState::NoNodesLeft:
      break;
  }
  return false;
}


int mpibalancing::SFCDiffusionNodePoolStrategy::getNumberOfIdleNodes() const {
  int result = 0;
  for (auto node: _nodes) {
    if (node.isIdlePrimaryRank() || node.isIdleSecondaryRank()) {
      result++;
    }
  }
  return result;
}


void mpibalancing::SFCDiffusionNodePoolStrategy::setNodeIdle( int rank ) {
  assertion( isRegisteredNode(rank) );
  _nodes[rank].deActivate();
  if (isPrimaryMPIRank(rank)) {
    if (_nodePoolState==NodePoolState::NoNodesLeft) {
      logInfo( "setNodeIdle(int)", "reset node pool state to DeployingIdlePrimaryRanks" );
      _nodePoolState = NodePoolState::DeployingIdlePrimaryRanks;
    }
  }
  else if (_nodePoolState == NodePoolState::NoNodesLeft) {
    if (_nodePoolState==NodePoolState::NoNodesLeft) {
      logInfo( "setNodeIdle(int)", "reset node pool state to DeployingAlsoSecondaryRanks" );
      _nodePoolState = NodePoolState::DeployingAlsoSecondaryRanks;
    }
  }
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::isRegisteredNode(int rank) const {
  return static_cast<int>(_nodes.size())>rank
      && _nodes[rank].isRegistered();
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::isIdleNode(int rank) const {
  return (static_cast<int>(_nodes.size())>rank)
      && (_nodes[rank].isIdlePrimaryRank() || _nodes[rank].isIdleSecondaryRank());
}


int mpibalancing::SFCDiffusionNodePoolStrategy::getNumberOfRegisteredNodes() const {
  int result = 0;
  for (auto node: _nodes) {
    if (node.isRegistered()) {
      result++;
    }
  }
  return result;
}

std::string mpibalancing::SFCDiffusionNodePoolStrategy::nodePoolStateToString() const {
  std::ostringstream result;

  switch (_nodePoolState) {
    case NodePoolState::DeployingIdlePrimaryRanks:
      result << "deploying-idle-ranks";
      break;
    case NodePoolState::DeployingAlsoSecondaryRanks:
      result << "deploying-also-secondary-ranks";
      for (auto p: _priorities) {
        result << ",priority(rank-" << p.first
               << "):" << p.second._priority
               << "/" << p.second._maxNumberOfSecondaryRanksToBeDeployed;
      }
      break;
    case NodePoolState::NoNodesLeft:
      result << "no-nodes-left";
      break;
  }

  return result.str();
}


std::string mpibalancing::SFCDiffusionNodePoolStrategy::toString() const {
  std::ostringstream result;
  result << "(node-pool-state:" << nodePoolStateToString();
  for (
    NodeContainer::const_iterator p = _nodes.begin();
    p != _nodes.end();
    p++
  ) {
    result << ",";
    p->toString(result);
  }
  result << "(";
  return result.str();
}


void mpibalancing::SFCDiffusionNodePoolStrategy::reserveParticularNode(int rank) {
  assertion( isRegisteredNode(rank) );
  assertion( _nodes[rank].isIdlePrimaryRank() || _nodes[rank].isIdleSecondaryRank() );

  _nodes[rank].activate();
}


int mpibalancing::SFCDiffusionNodePoolStrategy::getNumberOfPhysicalNodes() const {
  return std::max(static_cast<int>(_nodes.size()) / _mpiRanksPerNode,1);
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::hasCompleteIdleNode() const {
  for (int node=0; node<getNumberOfPhysicalNodes(); node++) {
    bool allPrimaryRanksIdle = true;
    for (int i=0; i<_mpiRanksPerNode; i++) {
      const int rank = node*_mpiRanksPerNode + i;
      if (isPrimaryMPIRank(rank)) {
        allPrimaryRanksIdle &= isIdleNode(rank);
      }
    }
    if (allPrimaryRanksIdle) {
      return true;
    }
  }

  return false;
}


int mpibalancing::SFCDiffusionNodePoolStrategy::deployIdlePrimaryRank(int forMaster) {
  assertion(_primaryMPIRanksPerNode>0);

  const int NumberOfRanksPerPrimaryRank = _mpiRanksPerNode / _primaryMPIRanksPerNode;

  const int FirstNodeToLookAt = _numberOfNodesToSkipPerRequestPlusOne==1 ? forMaster/_mpiRanksPerNode : forMaster/_mpiRanksPerNode+1;

  int firstLoopRank = 0;
  int searchOffset  = forMaster+NumberOfRanksPerPrimaryRank;

  for (int j=0; j<getNumberOfPhysicalNodes(); j+=_numberOfNodesToSkipPerRequestPlusOne) {
    for (int i=0; i<_numberOfPrimaryRanksPerNodeThatAreCurrentlyDeployed;i++) {
      firstLoopRank = ( (FirstNodeToLookAt+j)*_mpiRanksPerNode+i*NumberOfRanksPerPrimaryRank+searchOffset) % _totalNumberOfRanks;
      if (_nodes[firstLoopRank].isIdlePrimaryRank()) {
        _nodes[firstLoopRank].activate();
        return firstLoopRank;
      }
    }
    searchOffset = searchOffset>=NumberOfRanksPerPrimaryRank ? searchOffset-NumberOfRanksPerPrimaryRank : searchOffset;
  }

  logInfo(
    "deployIdlePrimaryRank(int)",
    "can't serve request from rank " << forMaster << " with constraint " <<
    _numberOfPrimaryRanksPerNodeThatAreCurrentlyDeployed << " of primary ranks per node. Fallback to all primary ranks"
  );

  // Fallback
  for (int i=0; i<static_cast<int>(_nodes.size()); i++) {
    const int rank = (firstLoopRank + i) % _totalNumberOfRanks;
    if (_nodes[rank].isIdlePrimaryRank()) {
      _nodes[rank].activate();
      return rank;
    }
  }

  assertionMsg(false, "should never be hit");
  return tarch::parallel::NodePool::NoFreeNodesMessage;
}


int mpibalancing::SFCDiffusionNodePoolStrategy::deployIdleSecondaryRank(int forMaster) {
  assertion1( hasIdleNode(forMaster), forMaster );
  assertion1( !_nodes[forMaster].isIdlePrimaryRank(), forMaster );
  assertion1( !_nodes[forMaster].isIdleSecondaryRank(), forMaster );

  if (
    _priorities.count(forMaster)==0
    ||
    _priorities[forMaster]._maxNumberOfSecondaryRanksToBeDeployed<=0
  ) {
    logInfo(
      "deployIdleSecondaryRank(int)",
      "do not serve request from rank " << forMaster <<
      ". Entry for master does exist in priority map=" << _priorities.count(forMaster) <<
      ", number of secondary ranks allowed for this master is " <<
      (_priorities.count(forMaster)>0 ? _priorities[forMaster]._maxNumberOfSecondaryRanksToBeDeployed : 0) <<
      ", number of entries in map=" << _priorities.size()
    );
    return tarch::parallel::NodePool::NoFreeNodesMessage;
  }
  else {
    const int NodeNumber = forMaster / _mpiRanksPerNode;

    const int sign = forMaster%_mpiRanksPerNode<=_mpiRanksPerNode/2 ? -1 : 1;

    for (int i=0; i<2*_mpiRanksPerNode; i++) {
      const int rank = forMaster + i/2*sign;

      if (
        (rank>0)
        &&
        (rank<_totalNumberOfRanks)
        &&
        (rank>=(NodeNumber-1)*_mpiRanksPerNode)
        &&
        (rank<(NodeNumber+1)*_mpiRanksPerNode)
        &&
        (_nodes[rank].isIdlePrimaryRank() || _nodes[rank].isIdleSecondaryRank())
      ) {
        _nodes[rank].activate();
        haveReservedSecondaryRank(forMaster,rank);
        return rank;
      }
    }
  }
  logInfo(
    "deployIdleSecondaryRank(int)",
    "can't serve " << forMaster
  );
  return tarch::parallel::NodePool::NoFreeNodesMessage;
}


int mpibalancing::SFCDiffusionNodePoolStrategy::reserveNode(int forMaster) {
  switch (_nodePoolState) {
    case NodePoolState::DeployingIdlePrimaryRanks:
      return deployIdlePrimaryRank(forMaster);
      break;
    case NodePoolState::DeployingAlsoSecondaryRanks:
      return deployIdleSecondaryRank(forMaster);
      break;
    case NodePoolState::NoNodesLeft:
      logInfo(
        "reserveNode(int)",
        "deny request from " << forMaster << " as no more ranks left"
      );
      return tarch::parallel::NodePool::NoFreeNodesMessage;
      break;
  }
  assertion1(false,toString());
  return tarch::parallel::NodePool::NoFreeNodesMessage;
}


void mpibalancing::SFCDiffusionNodePoolStrategy::haveReservedSecondaryRank(int masterRank, int workerRank) {
  assertion( _priorities[masterRank]._maxNumberOfSecondaryRanksToBeDeployed>0 );
  _priorities[masterRank]._maxNumberOfSecondaryRanksToBeDeployed--;

  logInfo(
    "haveReservedSecondaryRank(int,int)",
    "reserve rank " << workerRank << " for master " << masterRank <<
    " and upgrade priorities of all requests from ranks running on the same node as " << _nodes[workerRank].getNodeName()
  );

  for (auto& p: _priorities) {
    if ( p.first/_mpiRanksPerNode == workerRank/_mpiRanksPerNode) {
      p.second._priority++;
    }
  }
}


void mpibalancing::SFCDiffusionNodePoolStrategy::setNodePoolTag(int tag) {
  _tag = tag;
}


mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::NodePoolListEntry():
  _state(State::Undef),
  _name("undef") {
}


mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::NodePoolListEntry(
  const std::string& name,
  bool isPrimaryNode
  ):
  _state( isPrimaryNode ? State::WorkingPrimaryRank : State::WorkingSecondaryRank ),
  _name(name) {
}


mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::~NodePoolListEntry() {
}


std::string mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::getNodeName() const {
  return _name;
}


std::string mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::toString() const {
  std::ostringstream out;
  toString(out);
  return out.str();
}


void mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::toString(std::ostream& out) const {
  out << "(state:";
  switch (_state) {
    case State::Undef:                 out << "undef";                   break;
    case State::WorkingPrimaryRank:    out << "working-primary-rank";    break;
    case State::WorkingSecondaryRank:  out << "working-secondary-rank";  break;
    case State::IdlePrimaryRank:       out << "idle-primary-rank";       break;
    case State::IdleSecondaryRank:     out << "idle-secondary-rank";     break;
  }
  out << ",name:" << _name << ")";
}


void mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::activate() {
  assertion1( _state==State::IdlePrimaryRank || _state==State::IdleSecondaryRank, toString() );
  _state = _state==State::IdlePrimaryRank ? State::WorkingPrimaryRank : State::WorkingSecondaryRank;
}


void mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::deActivate() {
  assertion( _state==State::WorkingPrimaryRank || _state==State::WorkingSecondaryRank );
  _state = _state==State::WorkingPrimaryRank ? State::IdlePrimaryRank : State::IdleSecondaryRank;
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::isRegistered() const {
  return _state!=State::Undef;
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::isIdlePrimaryRank() const {
  return _state==State::IdlePrimaryRank;
}


bool mpibalancing::SFCDiffusionNodePoolStrategy::NodePoolListEntry::isIdleSecondaryRank() const {
  return _state==State::IdleSecondaryRank;
}
