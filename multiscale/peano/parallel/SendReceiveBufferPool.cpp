#include "peano/parallel/SendReceiveBufferPool.h"


#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"
#include "tarch/Assertions.h"
#include "tarch/timing/Watch.h"
#include "tarch/multicore/Lock.h"
#include "tarch/multicore/RecursiveLock.h"


#include "tarch/multicore/Jobs.h"

#include "peano/datatraversal/TaskSet.h"


#include <atomic>
#include <chrono>
#include <thread>

#include "tarch/services/ServiceFactory.h"
registerService(peano::parallel::SendReceiveBufferPool)


tarch::logging::Log                                                              peano::parallel::SendReceiveBufferPool::_log( "peano::parallel::SendReceiveBufferPool" );



#ifdef Parallel
peano::parallel::SendReceiveBufferPool::SendReceiveBufferPool():
  #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
  _backgroundThread(nullptr),
  #endif
  _iterationManagementTag(MPI_ANY_TAG),
  _iterationDataTag(MPI_ANY_TAG),
  _bufferSize(0),
  _mode(SendAndDeploy) {
  _iterationManagementTag = tarch::parallel::Node::getInstance().reserveFreeTag("SendReceiveBufferPool[it-mgmt]");
  _iterationDataTag       = tarch::parallel::Node::getInstance().reserveFreeTag("SendReceiveBufferPool[it-data]");

  // I originally wanted to spawn a thread immediately here. However, that
  // seems not to work as the job environment might not have been up (it
  // seems some stuff here's initialised statically). Therefore, I lazy
  // kick off the thread in receiveDanglingMessages().
/*
  #ifdef MPIUsesItsOwnThread
  _backgroundThread = new BackgroundThread();
  peano::datatraversal::TaskSet spawnTask(_backgroundThread,peano::datatraversal::TaskSet::TaskType::LongRunningBackground);
  #endif
*/
}
#else
peano::parallel::SendReceiveBufferPool::SendReceiveBufferPool():
  _iterationManagementTag(-1),
  _iterationDataTag(-1),
  _bufferSize(0),
  _mode(SendAndDeploy) {
}
#endif


void peano::parallel::SendReceiveBufferPool::terminateBackgroundThread() {
  #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
  tarch::multicore::Lock lock( _backgroundThreadSemaphore );
  if (_backgroundThread != nullptr) {
    _backgroundThread->terminate();
  }
  #endif
}


void peano::parallel::SendReceiveBufferPool::disconnectBackgroundThread() {
  #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
  tarch::multicore::Lock lock( _backgroundThreadSemaphore );
  _backgroundThread = nullptr;
  #endif
}


peano::parallel::SendReceiveBufferPool::~SendReceiveBufferPool() {
  #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
  terminateBackgroundThread();
  disconnectBackgroundThread();
  #endif

  // Don't use the semaphore here as the pool is static and thus might be shut
  // down after the shared memory environment is already down
  //
  // tarch::multicore::RecursiveLock lock( tarch::services::Service::receiveDanglingMessagesSemaphore );

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
  receiveDanglingMessagesIfAvailable();
}


bool peano::parallel::SendReceiveBufferPool::receiveDanglingMessagesIfAvailable() {
  SCOREP_USER_REGION("peano::parallel::SendReceiveBufferPool::receiveDanglingMessagesIfAvailable()", SCOREP_USER_REGION_TYPE_FUNCTION)

  tarch::multicore::RecursiveLock lock( tarch::services::Service::receiveDanglingMessagesSemaphore );

  bool result = false;
  for (std::map<int,SendReceiveBuffer*>::iterator p = _map.begin(); p!=_map.end(); p++ ) {
    logDebug( "receiveDanglingMessagesIfAvailable()", "receive data from rank " << p->first << " in mode " << toString(_mode) );
    result |= p->second->receivePageIfAvailable();
  }
  return result;
}


void peano::parallel::SendReceiveBufferPool::terminate() {
  terminateBackgroundThread();
  disconnectBackgroundThread();

  tarch::multicore::RecursiveLock lock( tarch::services::Service::receiveDanglingMessagesSemaphore );

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

  #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
  terminateBackgroundThread();
  disconnectBackgroundThread();
  #endif
}


void peano::parallel::SendReceiveBufferPool::releaseMessages() {
  SCOREP_USER_REGION("peano::parallel::SendReceiveBufferPool::releaseMessages()", SCOREP_USER_REGION_TYPE_FUNCTION)

  logTraceInWith1Argument( "releaseMessages()", toString(_mode) );

  #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
  terminateBackgroundThread();
  #endif

  tarch::multicore::RecursiveLock lock( tarch::services::Service::receiveDanglingMessagesSemaphore );

  logInfo( "releaseMessages()", "release the sent messages" );
  std::map<int,SendReceiveBuffer*>::iterator p = _map.begin();
  while (  p != _map.end() ) {
    p->second->releaseSentMessages();
    if ( p->second->getNumberOfSentMessages()==0 ) {
      logInfo( "releaseMessages()", "no message have been sent out, so remove buffer" );
      p = _map.erase(p);
    }
    else {
      p++;
    }
  }

  logInfo( "releaseMessages()", "all sent messages released, release the received messages" );

  for ( std::map<int,SendReceiveBuffer*>::const_reverse_iterator p = _map.rbegin(); p != _map.rend(); p++ ) {
    p->second->releaseReceivedMessages(true);
  }

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

  lock.free();

  disconnectBackgroundThread();

  logTraceOutWith1Argument( "releaseMessages()", toString(_mode) );
}


void peano::parallel::SendReceiveBufferPool::setBufferSize( int bufferSize ) {
  #ifdef Parallel
  assertion1( _map.empty(), tarch::parallel::Node::getInstance().getRank() );
  assertion2( bufferSize>0, bufferSize, tarch::parallel::Node::getInstance().getRank() );

  _bufferSize = bufferSize;
  #endif
}


peano::parallel::SendReceiveBufferPool::BackgroundThread::BackgroundThread():
  _state(peano::parallel::SendReceiveBufferPool::BackgroundThread::State::Running) {
}


bool peano::parallel::SendReceiveBufferPool::BackgroundThread::operator()() {
  #if !defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
  assertionMsg( false, "not never enter this operator" );
  #endif

  bool result = true;

  static int counter = 0;
  static int CallsInBetweenTwoReceives = IprobeEveryKIterations;
  counter++;
  if (counter>CallsInBetweenTwoReceives) {
    counter = 0;

    tarch::multicore::Lock stateLock( _semaphore );
    State state = _state;
    stateLock.free();

    switch (state) {
      case State::Running:
        {
          if (!SendReceiveBufferPool::getInstance().receiveDanglingMessagesIfAvailable()) {
            CallsInBetweenTwoReceives++;
          }
          #if defined(MPIUsesItsOwnThread) and defined(MultipleThreadsMayTriggerMPICalls) and defined(SharedMemoryParallelisation)
          if (CallsInBetweenTwoReceives>2*IprobeEveryKIterations) {
            SendReceiveBufferPool::getInstance().terminateBackgroundThread();
            SendReceiveBufferPool::getInstance().disconnectBackgroundThread();
            
            logDebug( "operator()()", "decided to kill myself" );
          }
          #endif
          // A release fence prevents the memory reordering of any read or write which precedes it in program order with any write which follows it in program order.
          //std::atomic_thread_fence(std::memory_order_release);
        }
        break;
      case State::Terminate:
        #ifdef Asserts
        logInfo( "BackgroundThread::operator()", "terminating" );
        #endif
        result = false;
        break;
    }
  }

  return result;
}


std::string peano::parallel::SendReceiveBufferPool::BackgroundThread::toString() const {
  return toString(_state);
}


std::string peano::parallel::SendReceiveBufferPool::BackgroundThread::toString(State state) {
  switch (state) {
    case State::Running:
      return "running";
    case State::Terminate:
      return "terminate";
  }

  return "<undef>";
}


void peano::parallel::SendReceiveBufferPool::BackgroundThread::terminate() {
  logTraceInWith1Argument( "switchState(State)", toString() );

  tarch::multicore::Lock stateLock( _semaphore );
  _state = State::Terminate;

  logTraceOutWith1Argument( "switchState(State)", toString() );
}


peano::parallel::SendReceiveBufferPool::BackgroundThread::~BackgroundThread() {
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
        terminateBackgroundThread();
        disconnectBackgroundThread();
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


