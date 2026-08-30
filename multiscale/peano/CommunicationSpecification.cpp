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
    case ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime:
      msg << "SendDataAndStateBeforeFirstTouchVertexFirstTime";
      break;
    case ExchangeMasterWorkerData::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime:
      msg << "SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime";
      break;
    case ExchangeMasterWorkerData::SendDataAndStateBeforeDescendIntoLocalSubtree:
      msg << "SendDataAndStateBeforeDescendIntoLocalSubtree";
      break;
    case ExchangeMasterWorkerData::MaskOutMasterWorkerDataAndStateExchange:
      msg << "MaskOutMasterWorkerDataAndStateExchange";
      break;
  }

  msg << ",exchange-worker-master-data=";
  switch (exchangeWorkerMasterData) {
    case ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime:
      msg << "SendDataAndStateAfterLastTouchVertexLastTime";
      break;
    case ExchangeWorkerMasterData::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime:
      msg << "SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime";
      break;
    case ExchangeWorkerMasterData::SendDataAndStateAfterProcessingOfLocalSubtree:
      msg << "SendDataAndStateAfterProcessingOfLocalSubtree";
      break;
    case ExchangeWorkerMasterData::MaskOutWorkerMasterDataAndStateExchange:
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
  logTraceInWith2Arguments("combine(...)", lhs.toString(), rhs.toString() );

  peano::CommunicationSpecification::ExchangeMasterWorkerData exchangeMasterWorkerData;
  peano::CommunicationSpecification::ExchangeWorkerMasterData exchangeWorkerMasterData;

  if ((rhs.exchangeMasterWorkerData==peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime) | (lhs.exchangeMasterWorkerData==peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime)) {
    exchangeMasterWorkerData = peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime;
  }
  else
  if ((rhs.exchangeMasterWorkerData==peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime) | (lhs.exchangeMasterWorkerData==peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime)) {
    exchangeMasterWorkerData = peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime;
  }
  else
  if ((rhs.exchangeMasterWorkerData==peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeDescendIntoLocalSubtree) | (lhs.exchangeMasterWorkerData==peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeDescendIntoLocalSubtree)) {
    exchangeMasterWorkerData = peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeDescendIntoLocalSubtree;
  }
  else {
    exchangeMasterWorkerData = peano::CommunicationSpecification::ExchangeMasterWorkerData::MaskOutMasterWorkerDataAndStateExchange;
  }


  if ((rhs.exchangeWorkerMasterData==peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime) | (lhs.exchangeWorkerMasterData==peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime)) {
    exchangeWorkerMasterData = peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime;
  }
  else
  if ((rhs.exchangeWorkerMasterData==peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime) | (lhs.exchangeWorkerMasterData==peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime)) {
    exchangeWorkerMasterData = peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime;
  }
  else
  if ((rhs.exchangeWorkerMasterData==peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterProcessingOfLocalSubtree) | (lhs.exchangeWorkerMasterData==peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterProcessingOfLocalSubtree)) {
    exchangeWorkerMasterData = peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterProcessingOfLocalSubtree;
  }
  else {
    exchangeWorkerMasterData = peano::CommunicationSpecification::ExchangeWorkerMasterData::MaskOutWorkerMasterDataAndStateExchange;
  }

  #ifdef Asserts
  if (
    (lhs.controlHeapInKernel != rhs.controlHeapInKernel)
    &&
    (lhs!=getMinimalSpecification())
    &&
    (rhs!=getMinimalSpecification())
  ) {
    logWarning( "combine(...)", "two communication specifications do not agree on heap control. lhs=" << lhs.toString() << ", rhs=" << rhs.toString() << " (minimial spec=" << getMinimalSpecification().toString() << ")" );
  }
  #endif

  const peano::CommunicationSpecification result(exchangeMasterWorkerData,exchangeWorkerMasterData, lhs.controlHeapInKernel | rhs.controlHeapInKernel);

  logTraceOutWith1Argument("combine(...)",result.toString());
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
  return CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::MaskOutMasterWorkerDataAndStateExchange,peano::CommunicationSpecification::ExchangeWorkerMasterData::MaskOutWorkerMasterDataAndStateExchange,handleHeapInKernel);
}


peano::CommunicationSpecification peano::CommunicationSpecification::getPessimisticSpecification(bool handleHeapInKernel) {
  return CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,handleHeapInKernel);
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::sendStateBackToMaster() const {
  switch (exchangeWorkerMasterData) {
    case ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Action::Late;
      break;
    case ExchangeWorkerMasterData::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Action::Late;
      break;
    case ExchangeWorkerMasterData::SendDataAndStateAfterProcessingOfLocalSubtree:
      return peano::CommunicationSpecification::Action::Early;
      break;
    case ExchangeWorkerMasterData::MaskOutWorkerMasterDataAndStateExchange:
      return peano::CommunicationSpecification::Action::Skip;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Action::Late;
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::sendDataBackToMaster() const {
  switch (exchangeWorkerMasterData) {
    case ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Action::Late;
      break;
    case ExchangeWorkerMasterData::SendDataAfterProcessingOfLocalSubtreeSendStateAfterLastTouchVertexLastTime:
      return peano::CommunicationSpecification::Action::Early;
      break;
    case ExchangeWorkerMasterData::SendDataAndStateAfterProcessingOfLocalSubtree:
      return peano::CommunicationSpecification::Action::Early;
      break;
    case ExchangeWorkerMasterData::MaskOutWorkerMasterDataAndStateExchange:
      return peano::CommunicationSpecification::Action::Skip;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Action::Late;
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::receiveDataFromMaster(bool stateMayUseLazyStateAndDataReceives) const {
  switch (exchangeMasterWorkerData) {
    case ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Action::Early;
      break;
    case ExchangeMasterWorkerData::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Action::Late;
      break;
    case ExchangeMasterWorkerData::SendDataAndStateBeforeDescendIntoLocalSubtree:
      return peano::CommunicationSpecification::Action::Late;
      break;
    case ExchangeMasterWorkerData::MaskOutMasterWorkerDataAndStateExchange:
      return stateMayUseLazyStateAndDataReceives ? peano::CommunicationSpecification::Action::Skip : peano::CommunicationSpecification::Action::Early;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Action::Early;
}


peano::CommunicationSpecification::Action peano::CommunicationSpecification::receiveStateFromMaster(bool stateMayUseLazyStateAndDataReceives) const {
  switch (exchangeMasterWorkerData) {
    case ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Action::Early;
      break;
    case ExchangeMasterWorkerData::SendDataBeforeDescendIntoLocalSubtreeSendStateBeforeFirstTouchVertexFirstTime:
      return peano::CommunicationSpecification::Action::Early;
      break;
    case ExchangeMasterWorkerData::SendDataAndStateBeforeDescendIntoLocalSubtree:
      return stateMayUseLazyStateAndDataReceives? peano::CommunicationSpecification::Action::Late : peano::CommunicationSpecification::Action::Early;
      break;
    case ExchangeMasterWorkerData::MaskOutMasterWorkerDataAndStateExchange:
      return stateMayUseLazyStateAndDataReceives ? peano::CommunicationSpecification::Action::Skip : peano::CommunicationSpecification::Action::Early;
      break;
  }
  assertion(false);
  return peano::CommunicationSpecification::Action::Early;
}
