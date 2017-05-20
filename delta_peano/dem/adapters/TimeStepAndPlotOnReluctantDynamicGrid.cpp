#include "dem/adapters/TimeStepAndPlotOnReluctantDynamicGrid.h"


peano::CommunicationSpecification   dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::communicationSpecification() const {
  return peano::CommunicationSpecification::getMinimalSpecification()
    &  _map2Collision.communicationSpecification()
    &  _map2MoveParticles.communicationSpecification()
    &  _map2ReluctantlyAdoptGrid.communicationSpecification()
    &  _map2Plot.communicationSpecification()
    &  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.communicationSpecification()

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2Collision.touchVertexLastTimeSpecification(level)
    &  _map2MoveParticles.touchVertexLastTimeSpecification(level)
    &  _map2ReluctantlyAdoptGrid.touchVertexLastTimeSpecification(level)
    &  _map2Plot.touchVertexLastTimeSpecification(level)
    &  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.touchVertexLastTimeSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::touchVertexFirstTimeSpecification(int level) const { 
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2Collision.touchVertexFirstTimeSpecification(level)
    &  _map2MoveParticles.touchVertexFirstTimeSpecification(level)
    &  _map2ReluctantlyAdoptGrid.touchVertexFirstTimeSpecification(level)
    &  _map2Plot.touchVertexFirstTimeSpecification(level)
    &  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.touchVertexFirstTimeSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::enterCellSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2Collision.enterCellSpecification(level)
    &  _map2MoveParticles.enterCellSpecification(level)
    &  _map2ReluctantlyAdoptGrid.enterCellSpecification(level)
    &  _map2Plot.enterCellSpecification(level)
    &  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.enterCellSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::leaveCellSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2Collision.leaveCellSpecification(level)
    &  _map2MoveParticles.leaveCellSpecification(level)
    &  _map2ReluctantlyAdoptGrid.leaveCellSpecification(level)
    &  _map2Plot.leaveCellSpecification(level)
    &  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.leaveCellSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::ascendSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2Collision.ascendSpecification(level)
    &  _map2MoveParticles.ascendSpecification(level)
    &  _map2ReluctantlyAdoptGrid.ascendSpecification(level)
    &  _map2Plot.ascendSpecification(level)
    &  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.ascendSpecification(level)

  ;
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::descendSpecification(int level) const {
  return peano::MappingSpecification::getMinimalSpecification()
    &  _map2Collision.descendSpecification(level)
    &  _map2MoveParticles.descendSpecification(level)
    &  _map2ReluctantlyAdoptGrid.descendSpecification(level)
    &  _map2Plot.descendSpecification(level)
    &  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.descendSpecification(level)

  ;
}


dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::TimeStepAndPlotOnReluctantDynamicGrid() {
}


dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::~TimeStepAndPlotOnReluctantDynamicGrid() {
}


#if defined(SharedMemoryParallelisation)
dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::TimeStepAndPlotOnReluctantDynamicGrid(const TimeStepAndPlotOnReluctantDynamicGrid&  masterThread):
  _map2Collision(masterThread._map2Collision) , 
  _map2MoveParticles(masterThread._map2MoveParticles) , 
  _map2ReluctantlyAdoptGrid(masterThread._map2ReluctantlyAdoptGrid) , 
  _map2Plot(masterThread._map2Plot) , 
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4(masterThread._map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4) 

{
}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::mergeWithWorkerThread(const TimeStepAndPlotOnReluctantDynamicGrid& workerThread) {
  _map2Collision.mergeWithWorkerThread(workerThread._map2Collision);
  _map2MoveParticles.mergeWithWorkerThread(workerThread._map2MoveParticles);
  _map2ReluctantlyAdoptGrid.mergeWithWorkerThread(workerThread._map2ReluctantlyAdoptGrid);
  _map2Plot.mergeWithWorkerThread(workerThread._map2Plot);
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.mergeWithWorkerThread(workerThread._map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4);

}
#endif


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  _map2Collision.createHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2MoveParticles.createHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2ReluctantlyAdoptGrid.createHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2Plot.createHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.createHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );


}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::destroyHangingVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  _map2Collision.destroyHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2MoveParticles.destroyHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2ReluctantlyAdoptGrid.destroyHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2Plot.destroyHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.destroyHangingVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  _map2Collision.createInnerVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2MoveParticles.createInnerVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2ReluctantlyAdoptGrid.createInnerVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2Plot.createInnerVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.createInnerVertex(fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  _map2Collision.createBoundaryVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2MoveParticles.createBoundaryVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2ReluctantlyAdoptGrid.createBoundaryVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2Plot.createBoundaryVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.createBoundaryVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  _map2Collision.destroyVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2MoveParticles.destroyVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2ReluctantlyAdoptGrid.destroyVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2Plot.destroyVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.destroyVertex( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  _map2Collision.createCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2MoveParticles.createCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2ReluctantlyAdoptGrid.createCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2Plot.createCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.createCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::destroyCell(
      const dem::Cell&           fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  _map2Collision.destroyCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2MoveParticles.destroyCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2ReluctantlyAdoptGrid.destroyCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2Plot.destroyCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.destroyCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


#ifdef Parallel
void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::mergeWithNeighbour(
  dem::Vertex&  vertex,
  const dem::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
   _map2Collision.mergeWithNeighbour( vertex, neighbour, fromRank, fineGridX, fineGridH, level );
   _map2MoveParticles.mergeWithNeighbour( vertex, neighbour, fromRank, fineGridX, fineGridH, level );
   _map2ReluctantlyAdoptGrid.mergeWithNeighbour( vertex, neighbour, fromRank, fineGridX, fineGridH, level );
   _map2Plot.mergeWithNeighbour( vertex, neighbour, fromRank, fineGridX, fineGridH, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.mergeWithNeighbour( vertex, neighbour, fromRank, fineGridX, fineGridH, level );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::prepareSendToNeighbour(
  dem::Vertex&  vertex,
  int                                           toRank,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
   _map2Collision.prepareSendToNeighbour( vertex, toRank, x, h, level );
   _map2MoveParticles.prepareSendToNeighbour( vertex, toRank, x, h, level );
   _map2ReluctantlyAdoptGrid.prepareSendToNeighbour( vertex, toRank, x, h, level );
   _map2Plot.prepareSendToNeighbour( vertex, toRank, x, h, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.prepareSendToNeighbour( vertex, toRank, x, h, level );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::prepareCopyToRemoteNode(
  dem::Vertex&  localVertex,
  int                                           toRank,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
   _map2Collision.prepareCopyToRemoteNode( localVertex, toRank, x, h, level );
   _map2MoveParticles.prepareCopyToRemoteNode( localVertex, toRank, x, h, level );
   _map2ReluctantlyAdoptGrid.prepareCopyToRemoteNode( localVertex, toRank, x, h, level );
   _map2Plot.prepareCopyToRemoteNode( localVertex, toRank, x, h, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.prepareCopyToRemoteNode( localVertex, toRank, x, h, level );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::prepareCopyToRemoteNode(
  dem::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
   _map2Collision.prepareCopyToRemoteNode( localCell, toRank, x, h, level );
   _map2MoveParticles.prepareCopyToRemoteNode( localCell, toRank, x, h, level );
   _map2ReluctantlyAdoptGrid.prepareCopyToRemoteNode( localCell, toRank, x, h, level );
   _map2Plot.prepareCopyToRemoteNode( localCell, toRank, x, h, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.prepareCopyToRemoteNode( localCell, toRank, x, h, level );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::mergeWithRemoteDataDueToForkOrJoin(
  dem::Vertex&  localVertex,
  const dem::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
   _map2Collision.mergeWithRemoteDataDueToForkOrJoin( localVertex, masterOrWorkerVertex, fromRank, x, h, level );
   _map2MoveParticles.mergeWithRemoteDataDueToForkOrJoin( localVertex, masterOrWorkerVertex, fromRank, x, h, level );
   _map2ReluctantlyAdoptGrid.mergeWithRemoteDataDueToForkOrJoin( localVertex, masterOrWorkerVertex, fromRank, x, h, level );
   _map2Plot.mergeWithRemoteDataDueToForkOrJoin( localVertex, masterOrWorkerVertex, fromRank, x, h, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.mergeWithRemoteDataDueToForkOrJoin( localVertex, masterOrWorkerVertex, fromRank, x, h, level );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::mergeWithRemoteDataDueToForkOrJoin(
  dem::Cell&  localCell,
  const dem::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
   _map2Collision.mergeWithRemoteDataDueToForkOrJoin( localCell, masterOrWorkerCell, fromRank, x, h, level );
   _map2MoveParticles.mergeWithRemoteDataDueToForkOrJoin( localCell, masterOrWorkerCell, fromRank, x, h, level );
   _map2ReluctantlyAdoptGrid.mergeWithRemoteDataDueToForkOrJoin( localCell, masterOrWorkerCell, fromRank, x, h, level );
   _map2Plot.mergeWithRemoteDataDueToForkOrJoin( localCell, masterOrWorkerCell, fromRank, x, h, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.mergeWithRemoteDataDueToForkOrJoin( localCell, masterOrWorkerCell, fromRank, x, h, level );

}


bool dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::prepareSendToWorker(
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
   result |= _map2Collision.prepareSendToWorker( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker );
   result |= _map2MoveParticles.prepareSendToWorker( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker );
   result |= _map2ReluctantlyAdoptGrid.prepareSendToWorker( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker );
   result |= _map2Plot.prepareSendToWorker( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker );
   result |= _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.prepareSendToWorker( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker );

  return result;
}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::prepareSendToMaster(
  dem::Cell&                       localCell,
  dem::Vertex *                    vertices,
  const peano::grid::VertexEnumerator&       verticesEnumerator, 
  const dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  const dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
   _map2Collision.prepareSendToMaster( localCell, vertices, verticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
   _map2MoveParticles.prepareSendToMaster( localCell, vertices, verticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
   _map2ReluctantlyAdoptGrid.prepareSendToMaster( localCell, vertices, verticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
   _map2Plot.prepareSendToMaster( localCell, vertices, verticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.prepareSendToMaster( localCell, vertices, verticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::mergeWithMaster(
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
   _map2Collision.mergeWithMaster( workerGridCell, workerGridVertices, workerEnumerator, fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker, workerState, masterState );
   _map2MoveParticles.mergeWithMaster( workerGridCell, workerGridVertices, workerEnumerator, fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker, workerState, masterState );
   _map2ReluctantlyAdoptGrid.mergeWithMaster( workerGridCell, workerGridVertices, workerEnumerator, fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker, workerState, masterState );
   _map2Plot.mergeWithMaster( workerGridCell, workerGridVertices, workerEnumerator, fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker, workerState, masterState );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.mergeWithMaster( workerGridCell, workerGridVertices, workerEnumerator, fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell, worker, workerState, masterState );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::receiveDataFromMaster(
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
   _map2Collision.receiveDataFromMaster( receivedCell, receivedVertices, receivedVerticesEnumerator, receivedCoarseGridVertices, receivedCoarseGridVerticesEnumerator, receivedCoarseGridCell, workersCoarseGridVertices, workersCoarseGridVerticesEnumerator, workersCoarseGridCell, fineGridPositionOfCell );
   _map2MoveParticles.receiveDataFromMaster( receivedCell, receivedVertices, receivedVerticesEnumerator, receivedCoarseGridVertices, receivedCoarseGridVerticesEnumerator, receivedCoarseGridCell, workersCoarseGridVertices, workersCoarseGridVerticesEnumerator, workersCoarseGridCell, fineGridPositionOfCell );
   _map2ReluctantlyAdoptGrid.receiveDataFromMaster( receivedCell, receivedVertices, receivedVerticesEnumerator, receivedCoarseGridVertices, receivedCoarseGridVerticesEnumerator, receivedCoarseGridCell, workersCoarseGridVertices, workersCoarseGridVerticesEnumerator, workersCoarseGridCell, fineGridPositionOfCell );
   _map2Plot.receiveDataFromMaster( receivedCell, receivedVertices, receivedVerticesEnumerator, receivedCoarseGridVertices, receivedCoarseGridVerticesEnumerator, receivedCoarseGridCell, workersCoarseGridVertices, workersCoarseGridVerticesEnumerator, workersCoarseGridCell, fineGridPositionOfCell );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.receiveDataFromMaster( receivedCell, receivedVertices, receivedVerticesEnumerator, receivedCoarseGridVertices, receivedCoarseGridVerticesEnumerator, receivedCoarseGridCell, workersCoarseGridVertices, workersCoarseGridVerticesEnumerator, workersCoarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::mergeWithWorker(
  dem::Cell&           localCell, 
  const dem::Cell&     receivedMasterCell,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                          level
) {
   _map2Collision.mergeWithWorker( localCell, receivedMasterCell, cellCentre, cellSize, level );
   _map2MoveParticles.mergeWithWorker( localCell, receivedMasterCell, cellCentre, cellSize, level );
   _map2ReluctantlyAdoptGrid.mergeWithWorker( localCell, receivedMasterCell, cellCentre, cellSize, level );
   _map2Plot.mergeWithWorker( localCell, receivedMasterCell, cellCentre, cellSize, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.mergeWithWorker( localCell, receivedMasterCell, cellCentre, cellSize, level );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::mergeWithWorker(
  dem::Vertex&        localVertex,
  const dem::Vertex&  receivedMasterVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
   _map2Collision.mergeWithWorker( localVertex, receivedMasterVertex, x, h, level );
   _map2MoveParticles.mergeWithWorker( localVertex, receivedMasterVertex, x, h, level );
   _map2ReluctantlyAdoptGrid.mergeWithWorker( localVertex, receivedMasterVertex, x, h, level );
   _map2Plot.mergeWithWorker( localVertex, receivedMasterVertex, x, h, level );
   _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.mergeWithWorker( localVertex, receivedMasterVertex, x, h, level );

}
#endif


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::touchVertexFirstTime(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  _map2Collision.touchVertexFirstTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2MoveParticles.touchVertexFirstTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2ReluctantlyAdoptGrid.touchVertexFirstTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2Plot.touchVertexFirstTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.touchVertexFirstTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::touchVertexLastTime(
      dem::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  _map2Collision.touchVertexLastTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2MoveParticles.touchVertexLastTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2ReluctantlyAdoptGrid.touchVertexLastTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2Plot.touchVertexLastTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.touchVertexLastTime( fineGridVertex, fineGridX, fineGridH, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfVertex );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::enterCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  _map2Collision.enterCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2MoveParticles.enterCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2ReluctantlyAdoptGrid.enterCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2Plot.enterCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.enterCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
  _map2Collision.leaveCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2MoveParticles.leaveCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2ReluctantlyAdoptGrid.leaveCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2Plot.leaveCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.leaveCell( fineGridCell, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell, fineGridPositionOfCell );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::beginIteration(
  dem::State&  solverState
) {
  _map2Collision.beginIteration( solverState );
  _map2MoveParticles.beginIteration( solverState );
  _map2ReluctantlyAdoptGrid.beginIteration( solverState );
  _map2Plot.beginIteration( solverState );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.beginIteration( solverState );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::endIteration(
  dem::State&  solverState
) {
  _map2Collision.endIteration( solverState );
  _map2MoveParticles.endIteration( solverState );
  _map2ReluctantlyAdoptGrid.endIteration( solverState );
  _map2Plot.endIteration( solverState );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.endIteration( solverState );

}




void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::descend(
  dem::Cell * const          fineGridCells,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell
) {
  _map2Collision.descend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2MoveParticles.descend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2ReluctantlyAdoptGrid.descend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2Plot.descend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.descend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );

}


void dem::adapters::TimeStepAndPlotOnReluctantDynamicGrid::ascend(
  dem::Cell * const    fineGridCells,
  dem::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell
) {
  _map2Collision.ascend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2MoveParticles.ascend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2ReluctantlyAdoptGrid.ascend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2Plot.ascend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );
  _map2TimeStepAndPlotOnReluctantDynamicGrid2VTKGridVisualiser_4.ascend( fineGridCells, fineGridVertices, fineGridVerticesEnumerator, coarseGridVertices, coarseGridVerticesEnumerator, coarseGridCell );

}
