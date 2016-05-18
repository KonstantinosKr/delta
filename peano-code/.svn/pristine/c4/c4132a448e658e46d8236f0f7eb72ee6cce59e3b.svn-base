#include "tarch/parallel/NodePool.h"
#include "tarch/parallel/Node.h"


#include "tarch/compiler/CompilerSpecificSettings.h"


#include "tarch/parallel/messages/ActivationMessage.h"
#include "tarch/parallel/messages/JobRequestMessage.h"
#include "tarch/parallel/messages/NodePoolAnswerMessage.h"

#include <sstream>

#include "tarch/services/ServiceFactory.h"
registerService(tarch::parallel::NodePool)


tarch::logging::Log tarch::parallel::NodePool::_log("tarch::parallel::NodePool");


const int tarch::parallel::NodePool::NoFreeNodesMessage = -1;

const int tarch::parallel::NodePool::JobRequestMessageAnswerValues::NewMaster = 0;
const int tarch::parallel::NodePool::JobRequestMessageAnswerValues::Terminate = -1;
const int tarch::parallel::NodePool::JobRequestMessageAnswerValues::RunAllNodes = -2;


tarch::parallel::NodePool::NodePool():
  _masterNode(-1),
  _registrationTag(-1),
  _jobManagementTag(-1),
  _jobServicesTag(-1),
  _isAlive(false),
  _strategy(0) {
  #ifdef Asserts
  _isInitialised = false;
  #endif
}


void tarch::parallel::NodePool::restart() {
  assertion1WithExplanation( !_isAlive, Node::getInstance().getRank(), "perhaps restart() called without terminate()" );
  assertion2( !Node::getInstance().isGlobalMaster() || _strategy!=0 , Node::getInstance().getRank(), "perhaps forgot to set a proper strategy" );
  assertion1( !Node::getInstance().isGlobalMaster() || _strategy->getNumberOfRegisteredNodes()==0, Node::getInstance().getRank() );

  logTraceIn( "restart()" );

  _isAlive = true;

  #ifdef Parallel
  MPI_Barrier(Node::getInstance().getCommunicator());

  if ( !Node::getInstance().isGlobalMaster() ) {
    logDebug( "restart()", "start to register at node pool" );
    tarch::parallel::messages::RegisterAtNodePoolMessage registerMessage(
      tarch::parallel::StringTools::convert(_log.getMachineInformation())
    );
    registerMessage.send( Node::getGlobalMasterRank(), _registrationTag, true, SendAndReceiveLoadBalancingMessagesBlocking);
    logDebug( "restart()", "register message sent: " << registerMessage.toString() << " on tag " << _registrationTag );
  }
  #endif

  logTraceOut( "restart()" );
}


int tarch::parallel::NodePool::getNumberOfWorkingNodes() const {
  #ifdef Parallel
  assertion1( Node::getInstance().isGlobalMaster(), Node::getInstance().getRank() );
  assertion1( _strategy!=0, Node::getInstance().getRank() );

  return _strategy->getNumberOfRegisteredNodes() - _strategy->getNumberOfIdleNodes() + 1; // +1 is the master
  #else
  return 1;
  #endif
}



int tarch::parallel::NodePool::getNumberOfIdleNodes() const {
  #ifdef Parallel
  assertion1( Node::getInstance().isGlobalMaster(), Node::getInstance().getRank() );
  assertion1( _strategy!=0, Node::getInstance().getRank() );

  return _strategy->getNumberOfIdleNodes();
  #else
  return 1;
  #endif
}



bool tarch::parallel::NodePool::areAllNodesWorking() const {
  #ifdef Parallel
  assertion1( Node::getInstance().isGlobalMaster(), Node::getInstance().getRank() );
  assertion1( _strategy!=0, Node::getInstance().getRank() );

  return _strategy->getNumberOfIdleNodes()==0;
  #else
  return true;
  #endif
}


void tarch::parallel::NodePool::init() {
  _registrationTag  = Node::getInstance().reserveFreeTag( "tarch::parallel::NodePool[registration]" );
  _jobManagementTag = Node::getInstance().reserveFreeTag( "tarch::parallel::NodePool[job-management]" );
  _jobServicesTag   = Node::getInstance().reserveFreeTag( "tarch::parallel::NodePool[job-services]" );

  #ifdef Parallel
  tarch::parallel::messages::ActivationMessage::initDatatype();
  tarch::parallel::messages::JobRequestMessage::initDatatype();
  tarch::parallel::messages::NodePoolAnswerMessage::initDatatype();
  tarch::parallel::messages::RegisterAtNodePoolMessage::initDatatype();
  tarch::parallel::messages::WorkerRequestMessage::initDatatype();

  assertion( tarch::parallel::messages::ActivationMessage::Datatype!=0 );
  assertion( tarch::parallel::messages::JobRequestMessage::Datatype!=0 );
  assertion( tarch::parallel::messages::NodePoolAnswerMessage::Datatype!=0 );
  assertion( tarch::parallel::messages::RegisterAtNodePoolMessage::Datatype!=0 );
  assertion( tarch::parallel::messages::WorkerRequestMessage::Datatype!=0 );
  #endif
}


tarch::parallel::NodePool& tarch::parallel::NodePool::getInstance() {
  static tarch::parallel::NodePool pool;
  return pool;
}


int tarch::parallel::NodePool::getTagForForkMessages() const {
  return _jobManagementTag;
}


void tarch::parallel::NodePool::setStrategy(NodePoolStrategy* strategy) {
  assertion1( Node::getInstance().isGlobalMaster(), Node::getInstance().getRank() );

  logTraceIn( "setStrategy(...)" );

  if (_strategy!=0) {
    assertion1( _strategy->getNumberOfRegisteredNodes()==0, Node::getInstance().getRank() );

    delete _strategy;
  }

  _strategy = strategy;
  _strategy->setNodePoolTag(_jobServicesTag);

  logTraceOut( "setStrategy(...)" );
}



void tarch::parallel::NodePool::waitForAllNodesToBecomeIdle() {
  #ifdef Parallel
  if (Node::getInstance().isGlobalMaster() ) {
    assertion1( _strategy!=0, Node::getInstance().getRank() );

    while ( _strategy->getNumberOfIdleNodes() < Node::getInstance().getNumberOfNodes()-1) {
      replyToJobRequestMessages();
    }
  }
  #endif
}


void tarch::parallel::NodePool::shutdown() {
  emptyReceiveBuffers();

  if ( _isAlive ) {
    _log.error("shutdown()", "called destructor for alive node" );
  }

  if (_strategy!=0) {
    delete _strategy;
    _strategy = 0;
  }

  #ifdef Parallel
  assertion1WithExplanation( tarch::parallel::messages::ActivationMessage::Datatype!=0, tarch::parallel::Node::getInstance().getRank(), "NodePool::shutdown() called twice" );
  assertion1WithExplanation( tarch::parallel::messages::JobRequestMessage::Datatype!=0, tarch::parallel::Node::getInstance().getRank(), "NodePool::shutdown() called twice" );
  assertion1WithExplanation( tarch::parallel::messages::NodePoolAnswerMessage::Datatype!=0, tarch::parallel::Node::getInstance().getRank(), "NodePool::shutdown() called twice" );
  assertion1WithExplanation( tarch::parallel::messages::RegisterAtNodePoolMessage::Datatype!=0, tarch::parallel::Node::getInstance().getRank(), "NodePool::shutdown() called twice" );
  assertion1WithExplanation( tarch::parallel::messages::WorkerRequestMessage::Datatype!=0, tarch::parallel::Node::getInstance().getRank(), "NodePool::shutdown() called twice" );

  tarch::parallel::messages::ActivationMessage::shutdownDatatype();
  tarch::parallel::messages::JobRequestMessage::shutdownDatatype();
  tarch::parallel::messages::NodePoolAnswerMessage::shutdownDatatype();
  tarch::parallel::messages::RegisterAtNodePoolMessage::shutdownDatatype();
  tarch::parallel::messages::WorkerRequestMessage::shutdownDatatype();

  tarch::parallel::messages::ActivationMessage::Datatype         = 0;
  tarch::parallel::messages::JobRequestMessage::Datatype         = 0;
  tarch::parallel::messages::NodePoolAnswerMessage::Datatype     = 0;
  tarch::parallel::messages::RegisterAtNodePoolMessage::Datatype = 0;
  tarch::parallel::messages::WorkerRequestMessage::Datatype      = 0;
  #endif
}


tarch::parallel::NodePool::~NodePool() {
  if (_strategy != 0) {
    std::cerr << "forgot to call shutdown() for NodePool on node " << Node::getInstance().getRank() << std::endl;
  }
}


tarch::parallel::NodePool::JobRequestMessageAnswer tarch::parallel::NodePool::waitForJob() {
  logTraceIn( "waitForJob()" );

  assertion2( _isAlive, Node::getInstance().getRank(), "forgot to call restart?" );
  assertion( !Node::getInstance().isGlobalMaster() );

  _masterNode = -1;

  #ifdef Parallel
  tarch::parallel::messages::JobRequestMessage message;
  message.send(Node::getInstance().getGlobalMasterRank(),_jobManagementTag, true, SendAndReceiveLoadBalancingMessagesBlocking);

  MPI_Status   status;
  tarch::parallel::messages::ActivationMessage answer;
  int result = MPI_Recv(
    &answer, 1,
    tarch::parallel::messages::ActivationMessage::Datatype,
    Node::getInstance().getGlobalMasterRank(), _jobManagementTag,
    tarch::parallel::Node::getInstance().getCommunicator(),
    &status
  );
  if ( result != MPI_SUCCESS ) {
    logError(
      "waitForJob()",
      "failed to start to receive tarch::parallel::messages::ActivationMessage from master node: "
      << tarch::parallel::MPIReturnValueToString(result)
    );
  }

  if ( answer.getNewMaster() == JobRequestMessageAnswerValues::Terminate ) {
    logDebug("waitForJob()", "node received termination signal");
    _isAlive = false;
    logTraceOutWith1Argument( "waitForJob()", "terminate" );
    return JobRequestMessageAnswerValues::Terminate;
  }
  else if ( answer.getNewMaster() == JobRequestMessageAnswerValues::RunAllNodes ) {
    logDebug("waitForJob()", "node received run code on all nodes signal. Will wake up for global step and then ask for new job again");
    _isAlive = true;
    logTraceOutWith1Argument( "waitForJob()", "run global step" );
    return JobRequestMessageAnswerValues::RunAllNodes;
  }
  else {
    _masterNode = answer.getNewMaster();
    assertion1(_masterNode>=0, _masterNode);
    logTraceOutWith1Argument( "waitForJob()", _masterNode );
    return _masterNode;
  }
  #else
  logTraceOutWith1Argument( "waitForJob()", _masterNode );
  return _masterNode;
  #endif
}


void tarch::parallel::NodePool::terminate() {
  if (Node::getInstance().isGlobalMaster()) {
    #ifdef Parallel
    assertion1WithExplanation( _strategy!=0, Node::getInstance().getRank(), "node pool restart missing?" );
    assertion1( Node::getInstance().isGlobalMaster(), Node::getInstance().getRank() );
    assertion1( _isAlive, Node::getInstance().getRank() );
    #endif

    logTraceIn("terminate()" );

    _isAlive = false;

    #ifdef Parallel
    while ( _strategy->hasIdleNode(NodePoolStrategy::AnyMaster) ) {
      int rank = _strategy->removeNextIdleNode();
      tarch::parallel::messages::ActivationMessage answerMessage( JobRequestMessageAnswerValues::Terminate );
      answerMessage.send( rank, _jobManagementTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
    }

    if (_strategy->getNumberOfRegisteredNodes()>0) {
      logInfo(
        "terminate()",
        "there are still " << _strategy->getNumberOfRegisteredNodes() <<
        " registered nodes alive and not idle"
      );
      logInfo( "terminate()", _strategy->toString() );
    }

    clock_t      timeOutWarning   = Node::getInstance().getDeadlockWarningTimeStamp();
    clock_t      timeOutShutdown  = Node::getInstance().getDeadlockTimeOutTimeStamp();
    bool         triggeredTimeoutWarning = false;

    assertion1( _strategy!=0, Node::getInstance().getRank() );
    while ( _strategy->getNumberOfRegisteredNodes()>0 ) {
      Node::getInstance().receiveDanglingMessages();

      // deadlock aspect
      if ( Node::getInstance().isTimeOutWarningEnabled() && (clock()>timeOutWarning) && (!triggeredTimeoutWarning)) {
        Node::getInstance().writeTimeOutWarning( "tarch::parallel::NodePool", "terminate()", -1, _jobManagementTag, 1);
        triggeredTimeoutWarning = true;
      }
      if ( Node::getInstance().isTimeOutDeadlockEnabled() && (clock()>timeOutShutdown)) {
        Node::getInstance().triggerDeadlockTimeOut( "tarch::parallel::NodePool", "terminate()", -1, _jobManagementTag, 1 );
      }
    }
    #endif
    logTraceOut( "terminate()" );
  }
}


int tarch::parallel::NodePool::getFreeNode(int forMaster) {
  assertion1( _isAlive, Node::getInstance().getRank() );
  assertion1( _strategy!=0, Node::getInstance().getRank() );

  logTraceInWith1Argument( "getFreeNode(int)", forMaster );

  int result;
  if ( _strategy->hasIdleNode(forMaster) ) {
    result = _strategy->reserveNode(forMaster);
  }
  else {
    result = NoFreeNodesMessage;
  }

  logTraceOutWith1Argument( "getFreeNode(int)", result );
  return result;
}


int tarch::parallel::NodePool::reserveFreeNodeForServer() {
  assertion1( _isAlive, Node::getInstance().getRank() );

  logTraceIn( "reserveFreeNodeForServer()");

  int activatedNode = getFreeNode( Node::getInstance().getGlobalMasterRank());

  #ifdef Parallel
  if (activatedNode!=NoFreeNodesMessage) {
    tarch::parallel::messages::ActivationMessage message( Node::getInstance().getGlobalMasterRank() );
    message.send( activatedNode, _jobManagementTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
  }
  #endif

  logTraceOutWith1Argument( "reserveFreeNodeForServer()", activatedNode);

  return activatedNode;
}


int tarch::parallel::NodePool::reserveFreeNode() {
  if ( Node::getInstance().isGlobalMaster() ) {
    assertion2( _masterNode == -1, _masterNode, Node::getInstance().getRank() );
  	assertion1( _isAlive, Node::getInstance().getRank() );
  	receiveDanglingMessages();
  	return reserveFreeNodeForServer();
  }
  else {
    assertion1( _isAlive, Node::getInstance().getRank() );
    return reserveFreeNodeForClient();
  }
}


int tarch::parallel::NodePool::reserveFreeNodeForClient() {
  assertion1( _isAlive, Node::getInstance().getRank() );
  assertion1( !Node::getInstance().isGlobalMaster(), Node::getInstance().getRank() );

  logTraceIn( "reserveFreeNodeForClient()" );

  #ifdef Parallel
  tarch::parallel::messages::WorkerRequestMessage queryMessage;
  queryMessage.send(Node::getInstance().getGlobalMasterRank(),_jobServicesTag, true, SendAndReceiveLoadBalancingMessagesBlocking);

  tarch::parallel::messages::NodePoolAnswerMessage answer;
  answer.receive(Node::getInstance().getGlobalMasterRank(),_jobServicesTag, true, SendAndReceiveLoadBalancingMessagesBlocking );

  const int result = answer.getNewWorker();
  #else
  const int result = -1;
  #endif

  logTraceOutWith1Argument( "reserveFreeNodeForClient()", result );

  return result;
}


tarch::parallel::NodePool::NodePool( const NodePool& pool ) {
  assertionMsg( false, "copy not allowed" );
}


tarch::parallel::NodePool& tarch::parallel::NodePool::operator=( const tarch::parallel::NodePool& pool ) {
  assertionMsg( false, "copy not allowed" );
  return *this;
}


void tarch::parallel::NodePool::receiveDanglingMessages() {
  if ( Node::getInstance().isGlobalMaster() ) {
    replyToRegistrationMessages();
    replyToJobRequestMessages();
    replyToWorkerRequestMessages();
  }
}


int tarch::parallel::NodePool::getMasterRank() const {
  assertion1WithExplanation(
    !Node::getInstance().isGlobalMaster(),
    Node::getInstance().getRank(),
    "You may not call getMasterRank() on the global master (typically rank 0). \nUse isGlobalMaster() to check before whether operation may be called."
  );
  return _masterNode;
}


void tarch::parallel::NodePool::emptyReceiveBuffers() {
  emptyRegisterMessageReceiveBuffer();
  emptyJobRequestMessageBuffer();
  emptyWorkerRequestMessageBuffer();
}


void tarch::parallel::NodePool::replyToRegistrationMessages() {
  assertion1( _strategy!=0, Node::getInstance().getRank() );

  #ifdef Parallel
  logTraceInWith1Argument( "replyToRegistrationMessages()", tarch::parallel::messages::RegisterAtNodePoolMessage::isMessageInQueue(_registrationTag, true) );

  while ( tarch::parallel::messages::RegisterAtNodePoolMessage::isMessageInQueue(_registrationTag, true) ) {
    tarch::parallel::messages::RegisterAtNodePoolMessage message;
    message.receive( MPI_ANY_SOURCE, _registrationTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
    logDebug(  "replyToRegistrationMessages()", "got registration from rank " << message.getSenderRank() );
    _strategy->addNode( message );
    assertion1( !_strategy->isIdleNode(message.getSenderRank()), message.toString() );
  }

  logTraceOut( "replyToRegistrationMessages()" );
  #endif
}


void tarch::parallel::NodePool::replyToJobRequestMessages() {
  logTraceIn( "replyToJobRequestMessage() ");
  assertion1( _strategy!=0, Node::getInstance().getRank() );

  #ifdef Parallel
  while ( tarch::parallel::messages::JobRequestMessage::isMessageInQueue(_jobManagementTag, true) ) {
    tarch::parallel::messages::JobRequestMessage queryMessage;
    queryMessage.receive( MPI_ANY_SOURCE, _jobManagementTag, true, SendAndReceiveLoadBalancingMessagesBlocking );

    assertion1( queryMessage.getSenderRank() !=Node::getInstance().getGlobalMasterRank(), Node::getInstance().getRank() );

    if ( !_strategy->isRegisteredNode(queryMessage.getSenderRank()) ) {
      logWarning(
        "replyToJobRequestMessages()",
        "node pool does not contain entry for rank " << queryMessage.getSenderRank()
         << ". Message from rank " << queryMessage.getSenderRank()
         << " might have overtaken registration message. Waiting for registration"
      );

      while ( !_strategy->isRegisteredNode(queryMessage.getSenderRank()) ) {
        replyToRegistrationMessages();
      }

      logDebug( "replyToJobRequestMessages()", "registration from " << queryMessage.getSenderRank() << " finally arrived" );
    }

    if ( !_isAlive ) {
      _strategy->setNodeIdle( queryMessage.getSenderRank() );
      int rank = _strategy->removeNextIdleNode();
      assertionEquals1( rank, queryMessage.getSenderRank(), Node::getInstance().getRank() );
      tarch::parallel::messages::ActivationMessage answerMessage( JobRequestMessageAnswerValues::Terminate );
      answerMessage.send( rank, _jobManagementTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
    }
    else {
      _strategy->setNodeIdle( queryMessage.getSenderRank() );
    }
  }
  #endif
  logTraceOut( "replyToJobRequestMessage() ");
}


void tarch::parallel::NodePool::replyToWorkerRequestMessages() {
  #ifdef Parallel
  assertion1( _strategy!=0 , Node::getInstance().getRank() );

  logTraceInWith1Argument( "replyToWorkerRequestMessages()", tarch::parallel::messages::WorkerRequestMessage::isMessageInQueue(_jobServicesTag, true) );

  static NodePoolStrategy::RequestQueue queue;

  if (tarch::parallel::messages::WorkerRequestMessage::isMessageInQueue(_jobServicesTag, true)) {
    //NOTE: Take care of recursive calls.
    _strategy->fillWorkerRequestQueue(queue);

    while ( !queue.empty() ) {
      tarch::parallel::messages::WorkerRequestMessage nextRequestToAnswer = _strategy->extractElementFromRequestQueue(queue);
      if ( _isAlive &&  _strategy->hasIdleNode(nextRequestToAnswer.getSenderRank()) ) {
        int activatedNode = _strategy->reserveNode(nextRequestToAnswer.getSenderRank());

        tarch::parallel::messages::NodePoolAnswerMessage answerMessage( activatedNode );
        answerMessage.send( nextRequestToAnswer.getSenderRank(), _jobServicesTag, true, SendAndReceiveLoadBalancingMessagesBlocking );

        tarch::parallel::messages::ActivationMessage activationMessage( nextRequestToAnswer.getSenderRank() );
        activationMessage.send( activatedNode, _jobManagementTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
      }
      else {
        tarch::parallel::messages::NodePoolAnswerMessage answerMessage( NoFreeNodesMessage );
        answerMessage.send( nextRequestToAnswer.getSenderRank(), _jobServicesTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
      }
      _strategy->fillWorkerRequestQueue(queue);
    }
  }

  logTraceOut( "replyToWorkerRequestMessages()" );
  #endif
}


void tarch::parallel::NodePool::emptyRegisterMessageReceiveBuffer() {
  #ifdef Parallel
  while ( tarch::parallel::messages::RegisterAtNodePoolMessage::isMessageInQueue(_registrationTag, true) ) {
    tarch::parallel::messages::RegisterAtNodePoolMessage message;
    message.receive( MPI_ANY_SOURCE, _registrationTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
  }
  #endif
}


void tarch::parallel::NodePool::emptyJobRequestMessageBuffer() {
  #ifdef Parallel
  while ( tarch::parallel::messages::JobRequestMessage::isMessageInQueue(_jobManagementTag, true) ) {
    tarch::parallel::messages::JobRequestMessage message;
    message.receive( MPI_ANY_SOURCE, _jobManagementTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
  }
  #endif
}


void tarch::parallel::NodePool::emptyWorkerRequestMessageBuffer() {
  #ifdef Parallel
  while ( tarch::parallel::messages::WorkerRequestMessage::isMessageInQueue(_jobServicesTag, true) ) {
    tarch::parallel::messages::WorkerRequestMessage message;
    message.receive( MPI_ANY_SOURCE, _jobServicesTag, true, SendAndReceiveLoadBalancingMessagesBlocking );
  }
  #endif
}


void tarch::parallel::NodePool::activateIdleNodes() {
  #ifdef Parallel
  assertion1( Node::getInstance().isGlobalMaster(), Node::getInstance().getRank() );
  assertion1( _isAlive, Node::getInstance().getRank() );
  assertion1( _strategy!=0, Node::getInstance().getRank() );

  logTraceIn( "activateIdleNodes(int)" );

  tarch::parallel::messages::ActivationMessage message( JobRequestMessageAnswerValues::RunAllNodes );

  for (int rank=1; rank<Node::getInstance().getNumberOfNodes(); rank++) {
    if (_strategy->isIdleNode(rank)) {
      _strategy->reserveParticularNode(rank);
      message.send(rank,getTagForForkMessages(), true, SendAndReceiveLoadBalancingMessagesBlocking);
    }
  }
  logTraceOut( "activateIdleNodes(int)" );
  #endif
}
