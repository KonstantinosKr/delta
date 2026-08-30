#include "tarch/parallel/FCFSNodePoolStrategy.h"


#include <sstream>


tarch::logging::Log tarch::parallel::FCFSNodePoolStrategy::_log( "tarch::parallel::FCFSNodePoolStrategy" );


tarch::parallel::FCFSNodePoolStrategy::FCFSNodePoolStrategy():
  NodePoolStrategy(),
  _tag(-1),
  _nodes() {
}


tarch::parallel::FCFSNodePoolStrategy::~FCFSNodePoolStrategy() {
}


void tarch::parallel::FCFSNodePoolStrategy::fillWorkerRequestQueue(RequestQueue& queue) {
  #ifdef Parallel
  assertion( _tag >= 0 );
  while ( tarch::parallel::messages::WorkerRequestMessage::isMessageInQueue(_tag, true) ) {
    tarch::parallel::messages::WorkerRequestMessage message;
    message.receive(MPI_ANY_SOURCE,_tag, true, tarch::parallel::messages::WorkerRequestMessage::ExchangeMode::Blocking );
    queue.push_back( message );
  }
  #endif
}


void tarch::parallel::FCFSNodePoolStrategy::logQueue( const RequestQueue& queue ) const {
  #ifdef Parallel
  if (queue.empty()) {
	_log.debug( "logQueue()", "queue is empty" );
  }
  else {
    std::ostringstream msg;
    msg << "queue: ";

	  for (RequestQueue::const_iterator p = queue.begin(); p != queue.end(); p++ ) {
	    msg << p->getSenderRank() << ",";
	  }
	  _log.debug( "logQueue()", msg.str() );
  }
  #endif
}


tarch::parallel::messages::WorkerRequestMessage tarch::parallel::FCFSNodePoolStrategy::extractElementFromRequestQueue(RequestQueue& queue) {
  assertion( !queue.empty() );
  tarch::parallel::messages::WorkerRequestMessage result = queue.front();
  queue.pop_front();
  return result;
}


void tarch::parallel::FCFSNodePoolStrategy::removeNode(int rank) {
  for (auto& p: _nodes) {
    if (p.getRank()==rank) {
      _nodes.remove(p);
      return;
    }
  }
}


void tarch::parallel::FCFSNodePoolStrategy::addNode(const tarch::parallel::messages::RegisterAtNodePoolMessage& node) {
  #ifdef Parallel
  assertion( !isRegisteredNode(node.getSenderRank()) );

  logTraceInWith1Argument( "addNode(...)", node.getSenderRank() );
  NodePoolListEntry newEntry(
    node.getSenderRank(),
    tarch::parallel::StringTools::convert(node.getNodeName())
  );
  _nodes.push_back( newEntry ) ;
  _nodes.sort();
  logTraceOutWith1Argument( "addNode(...)", newEntry.toString() );
  #endif
}


bool tarch::parallel::FCFSNodePoolStrategy::hasIdleNode(int forMaster) const {
  return !_nodes.empty() &&
         _nodes.front().isIdle();
}


int tarch::parallel::FCFSNodePoolStrategy::getNumberOfIdleNodes() const {
  int result = 0;
  NodeContainer::const_iterator p = _nodes.begin();
  while (p != _nodes.end()&& p->isIdle() ) {
	  p++;
	  result++;
  }
  return result;
}


void tarch::parallel::FCFSNodePoolStrategy::setNodeIdle( int rank ) {
  for (
    NodeContainer::iterator p = _nodes.begin();
    p != _nodes.end();
    p++
  ) {
    if ( p->getRank() == rank ) {
      p->deActivate();
    }
  }

  _nodes.sort();
}


bool tarch::parallel::FCFSNodePoolStrategy::isRegisteredNode(int rank) const {
  for (
    NodeContainer::const_iterator p = _nodes.begin();
    p != _nodes.end();
    p++
  ) {
    if ( p->getRank() == rank ) {
      return true;
    }
  }
  return false;
}


bool tarch::parallel::FCFSNodePoolStrategy::isIdleNode(int rank) const {
  assertion1( isRegisteredNode(rank), rank );
  for (
    NodeContainer::const_iterator p = _nodes.begin();
    p != _nodes.end();
    p++
  ) {
    if ( p->getRank() == rank && p->isIdle() ) {
      return true;
    }
  }
  return false;
}


int tarch::parallel::FCFSNodePoolStrategy::getNumberOfRegisteredNodes() const {
  return static_cast<int>( _nodes.size() );
}


std::string tarch::parallel::FCFSNodePoolStrategy::toString() const {
  std::ostringstream result;
  for (
    NodeContainer::const_iterator p = _nodes.begin();
    p != _nodes.end();
    p++
  ) {
    p->toString(result);
  }
  return result.str();
}


int tarch::parallel::FCFSNodePoolStrategy::reserveNode(int forMaster) {
  assertion1(hasIdleNode(forMaster),forMaster);

  NodePoolListEntry result = _nodes.front();
  _nodes.pop_front();

  logDebug( "getFreeNode(int)", "found free node: " << result.toString() );

  result.activate();
  _nodes.push_back(result);
  _nodes.sort();

  return result.getRank();
}


void tarch::parallel::FCFSNodePoolStrategy::reserveParticularNode(int rank) {
  assertion1( isIdleNode(rank), rank );

  for (
    NodeContainer::iterator p = _nodes.begin();
    p != _nodes.end();
    p++
  ) {
    if ( p->getRank() == rank ) {
      p->activate();
    }
  }

  _nodes.sort();
}


void tarch::parallel::FCFSNodePoolStrategy::setNodePoolTag(int tag) {
  _tag = tag;
}


tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::NodePoolListEntry(int rank, const std::string& name):
  _rank(rank),
  _state(WORKING),
  _name(name) {
}


tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::~NodePoolListEntry() {
}


std::string tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::getNodeName() const {
  return _name;
}


std::string tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::toString() const {
  std::ostringstream out;
  toString(out);
  return out.str();
}


void tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::toString(std::ostream& out) const {
  out << "(rank:" << _rank;
  switch (_state) {
    case IDLE:    out << ",state:idle";     break;
    case WORKING: out << ",state:working";  break;
  }
  out << ",name:" << _name << ")";
}


bool tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::operator==( const NodePoolListEntry& than ) const {
  return _rank==than._rank;
}


void tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::activate() {
  assertionEquals1( _state, IDLE, toString() );
  _state = WORKING;
}


void tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::deActivate() {
  _state = IDLE;
}


int tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::getRank() const {
  return _rank;
}


bool tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::isIdle() const {
  return _state == IDLE;
}


bool tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry::operator<( const tarch::parallel::FCFSNodePoolStrategy::NodePoolListEntry& than ) const {
  bool moveIdleToSmaller = isIdle() && !than.isIdle();

  #if defined(Asserts) || defined(Debug)
  bool orderRanks        = (isIdle()==than.isIdle()) && (_rank < than._rank);
  #else
  bool orderRanks = false;
  #endif

  return moveIdleToSmaller || orderRanks;

}

