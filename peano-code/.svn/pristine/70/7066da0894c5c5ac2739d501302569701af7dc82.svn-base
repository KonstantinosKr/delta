#include "peano/CommunicationSpecification.h"
#include "tarch/Assertions.h"
#include "tarch/logging/Log.h"


#include <sstream>


peano::CommunicationSpecification::CommunicationSpecification(ExchangeMasterWorkerData  exchangeMasterWorkerData_, ExchangeWorkerMasterData  exchangeWorkerMasterData_, bool controlHeapInKernel_ ):
  exchangeMasterWorkerData( exchangeMasterWorkerData_ ),
  exchangeWorkerMasterData( exchangeWorkerMasterData_ ),
  controlHeapInKernel(controlHeapInKernel_) {
}


std::string peano::CommunicationSpecification::toString() const {
  std::ostringstream msg;

  msg << "(exchange-master-worker-data_=";
  switch (exchangeMasterWorkerData) {
    case SendDataAndStateBeforeFirstTouchVertexFirstTime:
      msg << "SendDataAndStateBeforeFirstTouchVertexFirstTime";
      break;
    case SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime:
      msg << "SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime";
      break;
    case SendDataAndStateBeforeDescendIntoLocalSubtree:
      msg << "SendDataAndStateBeforeDescendIntoLocalSubtree";
      break;
    case MaskOutMasterWorkerDataAndStateExchange:
      msg << "MaskOutMasterWorkerDataAndStateExchange";
      break;
  }

  msg << ",exchange-worker-master-data=";
  switch (exchangeWorkerMasterData) {
    case SendDataAndStateAfterLastTouchVertexLastTime:
      msg << "SendDataAndStateAfterLastTouchVertexLastTime";
      break;
    case SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime:
      msg << "SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime";
      break;
    case SendDataAndStateAfterProcessingOfLocalSubtree:
      msg << "SendDataAndStateAfterProcessingOfLocalSubtree";
      break;
    case MaskOutWorkerMasterDataAndStateExchange:
      msg << "MaskOutWorkerMasterDataAndStateExchange";
      break;
  }

  msg << ",control-heap=" << controlHeapInKernel;

  msg << ")";

  return msg.str();
}


bool peano::CommunicationSpecification::shallKernelControlHeap() const {
  return controlHeapInKernel;
}


peano::CommunicationSpecification operator&(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs) {
  return peano::CommunicationSpecification::combine(lhs,rhs);
}



peano::CommunicationSpecification peano::CommunicationSpecification::combine(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs) {
  static tarch::logging::Log _log( "peano::CommunicationSpecification" );
  logTraceInWith2Arguments("operator&(...)", lhs.toString(), rhs.toString() );

  peano::CommunicationSpecification::ExchangeMasterWorkerData exchangeMasterWorkerData;
  peano::CommunicationSpecification::ExchangeWorkerMasterData exchangeWorkerMasterData;

  if ((rhs.exchangeMasterWorkerData==peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime) | (lhs.exchangeMasterWorkerData==peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime)) {
    exchangeMasterWorkerData = peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime;
  }
  else
  if ((rhs.exchangeMasterWorkerData==peano::CommunicationSpecification::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime) | (lhs.exchangeMasterWorkerData==peano::CommunicationSpecification::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime)) {
    exchangeMasterWorkerData = peano::CommunicationSpecification::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime;
  }
  else
  if ((rhs.exchangeMasterWorkerData==peano::CommunicationSpecification::SendDataAndStateBeforeDescendIntoLocalSubtree) | (lhs.exchangeMasterWorkerData==peano::CommunicationSpecification::SendDataAndStateBeforeDescendIntoLocalSubtree)) {
    exchangeMasterWorkerData = peano::CommunicationSpecification::SendDataAndStateBeforeDescendIntoLocalSubtree;
  }
  else {
    exchangeMasterWorkerData = peano::CommunicationSpecification::MaskOutMasterWorkerDataAndStateExchange;
  }


  if ((rhs.exchangeWorkerMasterData==peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime) | (lhs.exchangeWorkerMasterData==peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime)) {
    exchangeWorkerMasterData = peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime;
  }
  else
  if ((rhs.exchangeWorkerMasterData==peano::CommunicationSpecification::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime) | (lhs.exchangeWorkerMasterData==peano::CommunicationSpecification::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime)) {
    exchangeWorkerMasterData = peano::CommunicationSpecification::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime;
  }
  else
  if ((rhs.exchangeWorkerMasterData==peano::CommunicationSpecification::SendDataAndStateAfterProcessingOfLocalSubtree) | (lhs.exchangeWorkerMasterData==peano::CommunicationSpecification::SendDataAndStateAfterProcessingOfLocalSubtree)) {
    exchangeWorkerMasterData = peano::CommunicationSpecification::SendDataAndStateAfterProcessingOfLocalSubtree;
  }
  else {
    exchangeWorkerMasterData = peano::CommunicationSpecification::MaskOutWorkerMasterDataAndStateExchange;
  }

  #ifdef Asserts
  if (
    (lhs.controlHeapInKernel != rhs.controlHeapInKernel)
    &&
    (lhs!=getMinimalSpecification())
    &&
    (rhs!=getMinimalSpecification())
  ) {
    logWarning( "operator&(...)", "two communication specifications do not agree on heap control. lhs=" << lhs.toString() << ", rhs=" << rhs.toString() << " (minimial spec=" << getMinimalSpecification().toString() << ")" );
  }
  #endif

  const peano::CommunicationSpecification result(exchangeMasterWorkerData,exchangeWorkerMasterData, lhs.controlHeapInKernel | rhs.controlHeapInKernel);

  logTraceOutWith1Argument("operator&(...)",result.toString());
  return result;
}


bool operator==(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs) {
  return (lhs.exchangeMasterWorkerData==rhs.exchangeMasterWorkerData)
      && (lhs.exchangeWorkerMasterData==rhs.exchangeWorkerMasterData)
      && (lhs.controlHeapInKernel==rhs.controlHeapInKernel);
}


bool operator!=(const peano::CommunicationSpecification& lhs, const peano::CommunicationSpecification& rhs) {
  return (lhs.exchangeMasterWorkerData!=rhs.exchangeMasterWorkerData)
      || (lhs.exchangeWorkerMasterData!=rhs.exchangeWorkerMasterData)
      || (lhs.controlHeapInKernel!=rhs.controlHeapInKernel);
}


peano::CommunicationSpecification peano::CommunicationSpecification::getMinimalSpecification(bool handleHeapInKernel) {
  return CommunicationSpecification(peano::CommunicationSpecification::MaskOutMasterWorkerDataAndStateExchange,peano::CommunicationSpecification::MaskOutWorkerMasterDataAndStateExchange,handleHeapInKernel);
}


peano::CommunicationSpecification peano::CommunicationSpecification::getPessimisticSpecification(bool handleHeapInKernel) {
  return CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,handleHeapInKernel);
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::sendStateBackToMaster() const {
  switch (exchangeWorkerMasterData) {
    case SendDataAndStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Late;
      break;
    case SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Late;
      break;
    case SendDataAndStateAfterProcessingOfLocalSubtree:
      return peano::CommunicationSpecification::Early;
      break;
    case MaskOutWorkerMasterDataAndStateExchange:
      return peano::CommunicationSpecification::Skip;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Late;
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::sendDataBackToMaster() const {
  switch (exchangeWorkerMasterData) {
    case SendDataAndStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Late;
      break;
    case SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Early;
      break;
    case SendDataAndStateAfterProcessingOfLocalSubtree:
      return peano::CommunicationSpecification::Early;
      break;
    case MaskOutWorkerMasterDataAndStateExchange:
      return peano::CommunicationSpecification::Skip;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Late;
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::receiveDataFromMaster(bool stateMayUseLazyStateAndDataReceives) const {
  switch (exchangeMasterWorkerData) {
    case SendDataAndStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Early;
      break;
    case SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Late;
      break;
    case SendDataAndStateBeforeDescendIntoLocalSubtree:
      return peano::CommunicationSpecification::Late;
      break;
    case MaskOutMasterWorkerDataAndStateExchange:
      return stateMayUseLazyStateAndDataReceives ? peano::CommunicationSpecification::Skip : peano::CommunicationSpecification::Early;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Early;
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::receiveStateFromMaster(bool stateMayUseLazyStateAndDataReceives) const {
  switch (exchangeMasterWorkerData) {
    case SendDataAndStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Early;
      break;
    case SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Early;
      break;
    case SendDataAndStateBeforeDescendIntoLocalSubtree:
      return stateMayUseLazyStateAndDataReceives? peano::CommunicationSpecification::Late : peano::CommunicationSpecification::Early;
      break;
    case MaskOutMasterWorkerDataAndStateExchange:
      return stateMayUseLazyStateAndDataReceives ? peano::CommunicationSpecification::Skip : peano::CommunicationSpecification::Early;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Early;
}
