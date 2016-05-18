#include "peano/parallel/SendReceiveBufferPool.h"


#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"
#include "tarch/Assertions.h"
#include "tarch/timing/Watch.h"
#include "tarch/multicore/Lock.h"

#include "peano/datatraversal/TaskSet.h"


#include "tarch/services/ServiceFactory.h"
registerService(peano::parallel::SendReceiveBufferPool)


tarch::logging::Log                                                              peano::parallel::SendReceiveBufferPool::_log( "peano::parallel::SendReceiveBufferPool" );


#ifdef Parallel
peano::parallel::SendReceiveBufferPool::SendReceiveBufferPool():
  _iterationManagementTag(MPI_ANY_TAG),
  _iterationDataTag(MPI_ANY_TAG),
  _bufferSize(0),
  _mode(SendAndDeploy) {
  _iterationManagementTag = tarch::parallel::Node::getInstance().reserveFreeTag("SendReceiveBufferPool[it-mgmt]");
  _iterationDataTag       = tarch::parallel::Node::getInstance().reserveFreeTag("SendReceiveBufferPool[it-data]");

  #if defined(SEND_RECEIVE_BUFFER_POOL_USES_BACKGROUND_THREAD_TO_RECEIVE_DATA)
  peano::datatraversal::TaskSet spawnTask(_backgroundThread);
  #endif
}
#else
peano::parallel::SendReceiveBufferPool::SendReceiveBufferPool():
  _iterationManagementTag(-1),
  _iterationDataTag(-1),
  _bufferSize(0),
  _mode(SendAndDeploy) {
}
#endif


peano::parallel::SendReceiveBufferPool::~SendReceiveBufferPool() {
  #if defined(SEND_RECEIVE_BUFFER_POOL_USES_BACKGROUND_THREAD_TO_RECEIVE_DATA)
  assertion1( _backgroundThread._state == BackgroundThread::Terminate, _backgroundThread.toString() );
  #endif

  for (std::map<int,SendReceiveBuffer*>::iterator p = _map.begin(); p!=_map.end(); p++ ) {
    std::cerr << "encountered open buffer for destination " << p->first << " on rank " << tarch::parallel::Node::getInstance().getRank() <<  ". Would be nicer to call terminate() on SendReceiveBufferPool." << std::endl;
    delete p->second;
  }
}


peano::parallel::SendReceiveBufferPool& peano::parallel::SendReceiveBufferPool::getInstance() {
  static peano::parallel::SendReceiveBufferPool singleton;
  return singleton;
}



std::string peano::parallel::SendReceiveBufferPool::toString( SendReceiveMode  mode) {
  switch (mode) {
    case SendAndDeploy:
      return "send-and-deploy";
    case DeployButDoNotSend:
      return "deploy-but-do-not-send";
    case SendButDoNotDeploy:
      return "send-but-do-not-deploy";
    case NeitherDeployNorSend:
      return "neither-deploy-nor-send";
  }

  return "undef";
}


int peano::parallel::SendReceiveBufferPool::getIterationManagementTag() const {
  #ifdef Parallel
  assertion( _iterationManagementTag!=MPI_ANY_TAG );
  #endif
  return _iterationManagementTag;
}


int peano::parallel::SendReceiveBufferPool::getIterationDataTag() const {
  #ifdef Parallel
  assertion( _iterationDataTag!=MPI_ANY_TAG );
  #endif
  return _iterationDataTag;
}


void peano::parallel::SendReceiveBufferPool::receiveDanglingMessages() {
  #if !defined(SEND_RECEIVE_BUFFER_POOL_USES_BACKGROUND_THREAD_TO_RECEIVE_DATA)
  receiveDanglingMessagesFromAllBuffersInPool();
  #endif
}


void peano::parallel::SendReceiveBufferPool::receiveDanglingMessagesFromAllBuffersInPool() {
  for (std::map<int,SendReceiveBuffer*>::iterator p = _map.begin(); p!=_map.end(); p++ ) {
    logDebug( "receiveDanglingMessagesFromAllBuffersInPool()", "receive data from rank " << p->first << " in mode " << toString(_mode) );
    p->second->receivePageIfAvailable();
  }
}


void peano::parallel::SendReceiveBufferPool::terminate() {
  #if defined(SEND_RECEIVE_BUFFER_POOL_USES_BACKGROUND_THREAD_TO_RECEIVE_DATA)
  _backgroundThread.switchState(BackgroundThread::Terminate);
  #endif

  for (std::map<int,SendReceiveBuffer*>::iterator p = _map.begin(); p!=_map.end(); p++ ) {
    assertion1(  p->first >= 0, tarch::parallel::Node::getInstance().getRank() );
    assertion1( _map.count(p->first) == 1, tarch::parallel::Node::getInstance().getRank() );
    assertionEquals2( p->second->getNumberOfReceivedMessages(), 0, p->first, tarch::parallel::Node::getInstance().getRank() );
    delete p->second;
  }

  _map.clear();
}


void peano::parallel::SendReceiveBufferPool::restart() {
  assertion1( _map.empty(), tarch::parallel::Node::getInstance().getRank() );
}


void peano::parallel::SendReceiveBufferPool::releaseMessages() {
  logTraceInWith1Argument( "releaseMessages()", toString(_mode) );

  #if defined(SEND_RECEIVE_BUFFER_POOL_USES_BACKGROUND_THREAD_TO_RECEIVE_DATA)
  _backgroundThread.switchState(BackgroundThread::Suspend);
  #endif

  for ( std::map<int,SendReceiveBuffer*>::const_reverse_iterator p = _map.rbegin(); p != _map.rend(); p++ ) {
    p->second->releaseSentMessages();
  }
  for ( std::map<int,SendReceiveBuffer*>::const_reverse_iterator p = _map.rbegin(); p != _map.rend(); p++ ) {
    p->second->releaseReceivedMessages(true);
  }

  #if defined(SEND_RECEIVE_BUFFER_POOL_USES_BACKGROUND_THREAD_TO_RECEIVE_DATA)
  _backgroundThread.switchState(BackgroundThread::ReceiveDataInBackground);
  #endif

  switch (_mode) {
    case SendAndDeploy:
      break;
    case DeployButDoNotSend:
      _mode = NeitherDeployNorSend;
      break;
    case SendButDoNotDeploy:
      _mode = SendAndDeploy;
      break;
    case NeitherDeployNorSend:
      break;
  }

  logTraceOutWith1Argument( "releaseMessages()", toString(_mode) );
}


void peano::parallel::SendReceiveBufferPool::setBufferSize( int bufferSize ) {
  #ifdef Parallel
  assertion1( _map.empty(), tarch::parallel::Node::getInstance().getRank() );
  assertion2( bufferSize>0, bufferSize, tarch::parallel::Node::getInstance().getRank() );

  _bufferSize = bufferSize;
  #endif
}


tarch::multicore::BooleanSemaphore                               peano::parallel::SendReceiveBufferPool::BackgroundThread::_semaphore;
peano::parallel::SendReceiveBufferPool::BackgroundThread::State  peano::parallel::SendReceiveBufferPool::BackgroundThread::_state(Suspend);


void peano::parallel::SendReceiveBufferPool::BackgroundThread::operator()() {
  #if !defined(SEND_RECEIVE_BUFFER_POOL_USES_BACKGROUND_THREAD_TO_RECEIVE_DATA)
  assertionMsg( false, "not never enter this operator" );
  #endif

  bool terminate = false;

  while (!terminate) {
    tarch::multicore::Lock lock(_semaphore);
    switch (_state) {
      case ReceiveDataInBackground:
        SendReceiveBufferPool::getInstance().receiveDanglingMessagesFromAllBuffersInPool();
        break;
      case Suspend:
        break;
      case Terminate:
        terminate = true;
        break;
    }
    lock.free();

    tarch::multicore::BooleanSemaphore::sendTaskToBack();
  }
  tarch::multicore::BooleanSemaphore::continuedWithTask();
}


std::string peano::parallel::SendReceiveBufferPool::BackgroundThread::toString() const {
  switch (_state) {
    case ReceiveDataInBackground:
      return "receive-data-in-background";
    case Suspend:
      return "suspend";
    case Terminate:
      return "terminate";
  }

  return "<undef>";
}


void peano::parallel::SendReceiveBufferPool::BackgroundThread::switchState(State newState ) {
  logTraceInWith1Argument( "switchState(State)", toString() );
  tarch::multicore::Lock lock(_semaphore);

  assertion1( _state != BackgroundThread::Terminate, toString() );

  _state = newState;
  logTraceOutWith1Argument( "switchState(State)", toString() );
}


void peano::parallel::SendReceiveBufferPool::exchangeBoundaryVertices(bool value) {
  logTraceInWith2Arguments( "exchangeBoundaryVertices(bool)", toString(_mode), value );
  switch (_mode) {
    case SendAndDeploy:
      if (value) {
        _mode = SendAndDeploy;
      }
      else {
        _mode = DeployButDoNotSend;
      }
      break;
    case DeployButDoNotSend:
      assertionMsg( false, "mode should not be set in-between two iterations" );
      break;
    case SendButDoNotDeploy:
      assertionMsg( false, "mode should not be set in-between two iterations" );
      break;
    case NeitherDeployNorSend:
      if (value) {
        _mode = SendButDoNotDeploy;
      }
      else {
        _mode = NeitherDeployNorSend;
      }
      break;
  }
  logTraceOutWith1Argument( "exchangeBoundaryVertices(bool)", toString(_mode) );
}


bool peano::parallel::SendReceiveBufferPool::deploysValidData() const {
  return _mode==SendAndDeploy || _mode==DeployButDoNotSend;
}
