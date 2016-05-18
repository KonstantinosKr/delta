#include "peano/parallel/JoinDataBufferPool.h"
#include "peano/parallel/JoinDataBufferImplementation.h"

#include "tarch/Assertions.h"
#include "tarch/parallel/Node.h"


#include "tarch/services/ServiceFactory.h"
registerService(peano::parallel::JoinDataBufferPool)


#if defined(Debug) && (defined(Dim2) || defined(Dim3) || defined(Dim4))
const int peano::parallel::JoinDataBufferPool::OffsetForAdditionalCellLevelEncoding = TWO_POWER_TWO_POWER_D;
#endif


tarch::logging::Log peano::parallel::JoinDataBufferPool::_log( "peano::parallel::JoinDataBufferPool" );


peano::parallel::JoinDataBufferPool::JoinDataBufferPool():
  _map(),
  _bufferSize(1) {
  _vertexTag            = tarch::parallel::Node::getInstance().reserveFreeTag("JoinDataBufferPool[vertex]");
  _cellTag              = tarch::parallel::Node::getInstance().reserveFreeTag("JoinDataBufferPool[cell]");
  _cellMarkerTag        = tarch::parallel::Node::getInstance().reserveFreeTag("JoinDataBufferPool[cell-marker]");
}


peano::parallel::JoinDataBufferPool& peano::parallel::JoinDataBufferPool::getInstance() {
  static peano::parallel::JoinDataBufferPool singleton;
  return singleton;
}


peano::parallel::JoinDataBufferPool::BufferSet::BufferSet():
  _vertexBuffer(0),
  _cellBuffer(0),
  _cellMarkerBuffer(0) {
}


void peano::parallel::JoinDataBufferPool::receiveDanglingMessages() {
  for (std::map<int,BufferSet>::iterator p = _map.begin(); p!=_map.end(); p++ ) {
    if (p->second._vertexBuffer!=0 && p->second._vertexBuffer->isReceiveBuffer()) {
      p->second._vertexBuffer->receivePageIfAvailable();
    }
    if (p->second._cellBuffer!=0 && p->second._cellBuffer->isReceiveBuffer()) {
      p->second._cellBuffer->receivePageIfAvailable();
    }
    if (p->second._cellMarkerBuffer!=0 && p->second._cellMarkerBuffer->isReceiveBuffer()) {
      p->second._cellMarkerBuffer->receivePageIfAvailable();
    }
  }
}


void peano::parallel::JoinDataBufferPool::releaseMessages() {
  logTraceIn( "releaseMessages()" );
  for (std::map<int,BufferSet>::iterator p = _map.begin(); p!=_map.end(); p++ ) {
    if (p->second._vertexBuffer!=0) {
      if (!p->second._vertexBuffer->isReceiveBuffer()) {
        p->second._vertexBuffer->releaseMessages();
      }
    }
    if (p->second._cellBuffer!=0) {
      if (!p->second._cellBuffer->isReceiveBuffer()) {
        p->second._cellBuffer->releaseMessages();
      }
    }
    if (p->second._cellMarkerBuffer!=0) {
      if (!p->second._cellMarkerBuffer->isReceiveBuffer()) {
        p->second._cellMarkerBuffer->releaseMessages();
      }
    }

    if (
      (p->second._vertexBuffer!=0) &&
      (p->second._vertexBuffer->isEmpty())
    ) {
      assertion5(
        p->second._vertexBuffer->isEmpty(),
        p->second._vertexBuffer->toString(),
        p->second._cellBuffer->toString(),
        p->second._cellMarkerBuffer->toString(),
        p->first,
        tarch::parallel::Node::getInstance().getRank()
      );
      assertion5(
        p->second._cellBuffer->isEmpty(),
        p->second._vertexBuffer->toString(),
        p->second._cellBuffer->toString(),
        p->second._cellMarkerBuffer->toString(),
        p->first,
        tarch::parallel::Node::getInstance().getRank()
      );
      assertion5(
        p->second._cellMarkerBuffer->isEmpty(),
        p->second._vertexBuffer->toString(),
        p->second._cellBuffer->toString(),
        p->second._cellMarkerBuffer->toString(),
        p->first,
        tarch::parallel::Node::getInstance().getRank()
      );

      delete p->second._vertexBuffer;
      delete p->second._cellBuffer;
      delete p->second._cellMarkerBuffer;

      p->second._vertexBuffer = 0;
      p->second._cellBuffer = 0;
      p->second._cellMarkerBuffer = 0;

      logDebug( "releaseMessages()", "destroyed vertex, cell, and marker buffer for rank " << p->first << " on rank " << tarch::parallel::Node::getInstance().getRank() );
    }
  }
  logTraceOut( "releaseMessages()" );
}


#if defined(Debug) && (defined(Dim2) || defined(Dim3) || defined(Dim4))
int peano::parallel::JoinDataBufferPool::getCellLevelFromStream(int fromRank) {
  const int result = getRawCellMarkerFromStream(fromRank) / OffsetForAdditionalCellLevelEncoding;
  return result;
}
#endif


void peano::parallel::JoinDataBufferPool::createCellMarkerBufferManually(bool isReceiveBuffer, int fromOrToRank) {
  #ifdef Parallel
  if (_map[fromOrToRank]._cellMarkerBuffer==0) {
    typedef JoinDataBufferImplementation<int> BufferType;
    _map[fromOrToRank]._cellMarkerBuffer = new BufferType(isReceiveBuffer, _bufferSize, MPI_INT, fromOrToRank, _cellMarkerTag);
    logDebug( "sendVertex(...)", "created cell tag join buffer on rank " << tarch::parallel::Node::getInstance().getRank() << " for rank " << fromOrToRank << ", is receive buffer=" << isReceiveBuffer);
  }
  #endif
}


int peano::parallel::JoinDataBufferPool::getRawCellMarkerFromStream(int fromRank) {
  logTraceInWith1Argument( "getRawCellMarkerFromStream(int)", fromRank );
  assertion2( _map.count(fromRank) == 0 || _map[fromRank]._cellMarkerBuffer==0 || _map[fromRank]._cellMarkerBuffer->isReceiveBuffer(), fromRank, tarch::parallel::Node::getInstance().getRank() );

  createCellMarkerBufferManually(true,fromRank);

  typedef JoinDataBufferImplementation<int> BufferType;
  const int result = static_cast<BufferType*>(_map[fromRank]._cellMarkerBuffer)->getTopElement();
  logTraceOutWith1Argument( "getRawCellMarkerFromStream(int)", result );
  return result;
}


std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT> peano::parallel::JoinDataBufferPool::getCellMarkerFromStream(int fromRank) {
  int result = getRawCellMarkerFromStream(fromRank);

  #if defined(Debug) && (defined(Dim2) || defined(Dim3) || defined(Dim4))
  result -= getCellLevelFromStream(fromRank) * OffsetForAdditionalCellLevelEncoding;
  #endif

  return std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT>(result);
}


void peano::parallel::JoinDataBufferPool::removeVertexFromStream(int rank) {
  assertion2( _map.count(rank)>0, rank, tarch::parallel::Node::getInstance().getRank() );
  assertion2( _map[rank]._vertexBuffer!=0, rank, tarch::parallel::Node::getInstance().getRank() );
  assertion2( !_map[rank]._vertexBuffer->isEmpty(), rank, tarch::parallel::Node::getInstance().getRank() );
  _map[rank]._vertexBuffer->removeTopElementFromStream();
}


void peano::parallel::JoinDataBufferPool::removeCellFromStream(int rank) {
  assertion2( _map.count(rank)>0, rank, tarch::parallel::Node::getInstance().getRank() );
  assertion2( _map[rank]._cellBuffer!=0, rank, tarch::parallel::Node::getInstance().getRank() );
  assertion2( !_map[rank]._cellBuffer->isEmpty(), rank, tarch::parallel::Node::getInstance().getRank() );
  _map[rank]._cellBuffer->removeTopElementFromStream();
}


void peano::parallel::JoinDataBufferPool::removeCellMarkerFromStream(int rank, bool calledOnMasterJoiningWithItsWorker) {
  assertion2( _map.count(rank)>0, rank, tarch::parallel::Node::getInstance().getRank() );
  assertion2( _map[rank]._cellMarkerBuffer!=0, rank, tarch::parallel::Node::getInstance().getRank() );
  if (calledOnMasterJoiningWithItsWorker) {
    while (_map[rank]._cellMarkerBuffer->isEmpty() ) {
      receiveDanglingMessages();
    }
  }
  else {
    assertion2( !_map[rank]._cellMarkerBuffer->isEmpty(), rank, tarch::parallel::Node::getInstance().getRank() );
  }
  _map[rank]._cellMarkerBuffer->removeTopElementFromStream();
}


void peano::parallel::JoinDataBufferPool::setBufferSize( int bufferSize ) {
  #ifdef Parallel
  assertion1( _map.empty(), tarch::parallel::Node::getInstance().getRank() );
  assertion2( bufferSize>0, bufferSize, tarch::parallel::Node::getInstance().getRank() );

  _bufferSize = bufferSize;
  #endif
}
