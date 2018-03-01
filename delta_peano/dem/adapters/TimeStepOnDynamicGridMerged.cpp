#include "dem/adapters/TimeStepOnDynamicGridMerged.h"


peano::CommunicationSpecification   dem::adapters::TimeStepOnDynamicGridMerged::communicationSpecification() const {
  return peano::CommunicationSpecification::getMinimalSpecification()
    &  _map2AdoptGridMerged.communicationSpecification()

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepOnDynamicGridMerged::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2AdoptGridMerged.touchVertexLastTimeSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepOnDynamicGridMerged::touchVertexFirstTimeSpecification(int level) const { 
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2AdoptGridMerged.touchVertexFirstTimeSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepOnDynamicGridMerged::enterCellSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2AdoptGridMerged.enterCellSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepOnDynamicGridMerged::leaveCellSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2AdoptGridMerged.leaveCellSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepOnDynamicGridMerged::ascendSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2AdoptGridMerged.ascendSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepOnDynamicGridMerged::descendSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2AdoptGridMerged.descendSpecification(level)

  ;
}


dem::adapters::TimeStepOnDynamicGridMerged::TimeStepOnDynamicGridMerged() {
}


dem::adapters::TimeStepOnDynamicGridMerged::~TimeStepOnDynamicGridMerged() {
}


#if defined(SharedMemoryParallelisation)
dem::adapters::TimeStepOnDynamicGridMerged::TimeStepOnDynamicGridMerged(const TimeStepOnDynamicGridMerged&  masterThread):
  _map2AdoptGridMerged(masterThread._map2AdoptGridMerged) 

{
}


void dem::adapters::TimeStepOnDynamicGridMerged::mergeWithWorkerThread(const TimeStepOnDynamicGridMerged& workerThread) {
  _map2AdoptGridMerged.mergeWithWorkerThread(workerThread._map2AdoptGridMerged);

}
#endif


void dem::adapters::TimeStepOnDynamicGridMerged::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  _map2AdoptGridMerged.createHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );


}


void dem::adapters::TimeStepOnDynamicGridMerged::destroyHangingVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  _map2AdoptGridMerged.destroyHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepOnDynamicGridMerged::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  _map2AdoptGridMerged.createInnerVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepOnDynamicGridMerged::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  _map2AdoptGridMerged.createBoundaryVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepOnDynamicGridMerged::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  _map2AdoptGridMerged.destroyVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepOnDynamicGridMerged::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  _map2AdoptGridMerged.createCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepOnDynamicGridMerged::destroyCell(
      const dem::Cell&           fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  _map2AdoptGridMerged.destroyCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


#ifdef Parallel
void dem::adapters::TimeStepOnDynamicGridMerged::mergeWithNeighbour(
  dem::Vertex&  vertex,
  const dem::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
   _map2AdoptGridMerged.mergeWithNeighbour( vertex, neighbour, fromRank, fineGridX, fineGridH, level );

}


void dem::adapters::TimeStepOnDynamicGridMerged::prepareSendToNeighbour(
  dem::Vertex&  vertex,
  int                                           toRank,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
   _map2AdoptGridMerged.prepareSendToNeighbour( vertex, toRank, x, h, level );

}


void dem::adapters::TimeStepOnDynamicGridMerged::prepareCopyToRemoteNode(
  dem::Vertex&  localVertex,
  int                                           toRank,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
   _map2AdoptGridMerged.prepareCopyToRemoteNode( localVertex, toRank, x, h, level );

}


void dem::adapters::TimeStepOnDynamicGridMerged::prepareCopyToRemoteNode(
  dem::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
   _map2AdoptGridMerged.prepareCopyToRemoteNode( localCell, toRank, x, h, level );

}


void dem::adapters::TimeStepOnDynamicGridMerged::mergeWithRemoteDataDueToForkOrJoin(
  dem::Vertex&  localVertex,
  const dem::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
   _map2AdoptGridMerged.mergeWithRemoteDataDueToForkOrJoin( localVertex, masterOrWorkerVertex, fromRank, x, h, level );

}


void dem::adapters::TimeStepOnDynamicGridMerged::mergeWithRemoteDataDueToForkOrJoin(
  dem::Cell&  localCell,
  const dem::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
   _map2AdoptGridMerged.mergeWithRemoteDataDueToForkOrJoin( localCell, masterOrWorkerCell, fromRank, x, h, level );

}


bool dem::adapters::TimeStepOnDynamicGridMerged::prepareSendToWorker(
  dem::Cell&                 fineGridCell,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker
) {
  bool result = false;
   result |= _map2AdoptGridMerged.prepareSendToWorker( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker );

  return result;
}


void dem::adapters::TimeStepOnDynamicGridMerged::prepareSendToMaster(
  dem::Cell&                       localCell,
  dem::Vertex *                    vertices,
  const peano::grid::VertexEnumerator&       verticesEnumerator, 
  const dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  const dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
   _map2AdoptGridMerged.prepareSendToMaster( localCell, vertices, verticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepOnDynamicGridMerged::mergeWithMaster(
  const dem::Cell&           workerGridCell,
  dem::Vertex * const        workerGridVertices,
  const peano::grid::VertexEnumerator& workerEnumerator,
  dem::Cell&                 fineGridCell,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker,
    const dem::State&          workerState,
  dem::State&                masterState
) {
   _map2AdoptGridMerged.mergeWithMaster( workerGridCell, workerGridVertices, workerEnumerator, fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker, workerState, masterState );

}


void dem::adapters::TimeStepOnDynamicGridMerged::receiveDataFromMaster(
      dem::Cell&                        receivedCell, 
      dem::Vertex *                     receivedVertices,
      const peano::grid::VertexEnumerator&        receivedVerticesEnumerator,
      dem::Vertex * const               receivedCoarseGridVertices,
      const peano::grid::VertexEnumerator&        receivedCoarseGridVerticesEnumerator,
      dem::Cell&                        receivedCoarseGridCell,
      dem::Vertex * const               workersCoarseGridVertices,
      const peano::grid::VertexEnumerator&        workersCoarseGridVerticesEnumerator,
      dem::Cell&                        workersCoarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&    fineGridPositionOfCell
) {
   _map2AdoptGridMerged.receiveDataFromMaster( receivedCell, receivedVertices, receivedVerticesEnumerator, receivedCoarseGridVertices, receivedCoarseGridVerticesEnumerator, receivedCoarseGridCell, workersCoarseGridVertices, workersCoarseGridVerticesEnumerator, workersCoarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepOnDynamicGridMerged::mergeWithWorker(
  dem::Cell&           localCell, 
  const dem::Cell&     receivedMasterCell,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                          level
) {
   _map2AdoptGridMerged.mergeWithWorker( localCell, receivedMasterCell, cellCentre, cellSize, level );

}


void dem::adapters::TimeStepOnDynamicGridMerged::mergeWithWorker(
  dem::Vertex&        localVertex,
  const dem::Vertex&  receivedMasterVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
   _map2AdoptGridMerged.mergeWithWorker( localVertex, receivedMasterVertex, x, h, level );

}
#endif


void dem::adapters::TimeStepOnDynamicGridMerged::touchVertexFirstTime(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  _map2AdoptGridMerged.touchVertexFirstTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepOnDynamicGridMerged::touchVertexLastTime(
      dem::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  _map2AdoptGridMerged.touchVertexLastTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepOnDynamicGridMerged::enterCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  _map2AdoptGridMerged.enterCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepOnDynamicGridMerged::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
  _map2AdoptGridMerged.leaveCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepOnDynamicGridMerged::beginIteration(
  dem::State&  solverState
) {
  _map2AdoptGridMerged.beginIteration( solverState );

}


void dem::adapters::TimeStepOnDynamicGridMerged::endIteration(
  dem::State&  solverState
) {
  _map2AdoptGridMerged.endIteration( solverState );

}




void dem::adapters::TimeStepOnDynamicGridMerged::descend(
  dem::Cell * const          fineGridCells,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell
) {
  _map2AdoptGridMerged.descend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );

}


void dem::adapters::TimeStepOnDynamicGridMerged::ascend(
  dem::Cell * const    fineGridCells,
  dem::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell
) {
  _map2AdoptGridMerged.ascend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );

}
