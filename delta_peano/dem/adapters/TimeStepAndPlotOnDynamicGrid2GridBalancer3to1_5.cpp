#include "dem/adapters/TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5.h"

#include <sstream>

#include "peano/utils/Loop.h"
#include "peano/grid/CellFlags.h"

#ifdef Parallel
#include "tarch/parallel/Node.h"
#endif



peano::CommunicationSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::communicationSpecification() {
  return peano::CommunicationSpecification::getMinimalSpecification();
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::OnlyLeaves,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5() {
}


dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::~TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5() {
}


#if defined(SharedMemoryParallelisation)
dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5(const TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5&  masterThread) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::mergeWithWorkerThread(const TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5& workerThread) {
}
#endif



void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
}



void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::destroyHangingVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::destroyCell(
      const dem::Cell&           fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}


#ifdef Parallel
void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::mergeWithNeighbour(
  dem::Vertex&  vertex,
  const dem::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::prepareSendToNeighbour(
      dem::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::prepareCopyToRemoteNode(
      dem::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::prepareCopyToRemoteNode(
      dem::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
      int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::mergeWithRemoteDataDueToForkOrJoin(
  dem::Vertex&  localVertex,
  const dem::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::mergeWithRemoteDataDueToForkOrJoin(
  dem::Cell&  localCell,
  const dem::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
}


bool dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::prepareSendToWorker(
  dem::Cell&                 fineGridCell,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker
) {
  return false;
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::prepareSendToMaster(
      dem::Cell&                       localCell,
      dem::Vertex *                    vertices,
      const peano::grid::VertexEnumerator&       verticesEnumerator, 
      const dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
      const dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::mergeWithMaster(
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
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::receiveDataFromMaster(
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
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::mergeWithWorker(
      dem::Cell&           localCell, 
      const dem::Cell&     receivedMasterCell,
      const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
      int                                          level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::mergeWithWorker(
      dem::Vertex&        localVertex,
      const dem::Vertex&  receivedMasterVertex,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}
#endif


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::touchVertexFirstTime(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::touchVertexLastTime(
  dem::Vertex&         fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  if( 
    fineGridVertex.getRefinementControl() == dem::Vertex::Records::Refined
    || 
    fineGridVertex.getRefinementControl() == dem::Vertex::Records::RefinementTriggered
  ) {
    tarch::la::Vector<DIMENSIONS, int> coarseGridPositionOfVertex;
    for(int d = 0; d < DIMENSIONS; d++) {
      coarseGridPositionOfVertex(d) = fineGridPositionOfVertex(d) > 1 ? 1 : 0;
    }

    dem::Vertex& coarseVertex = coarseGridVertices[coarseGridVerticesEnumerator(coarseGridPositionOfVertex)];
    if(
      coarseVertex.getRefinementControl()== dem::Vertex::Records::Unrefined
      && 
      !coarseVertex.isHangingNode()
      && 
      !coarseVertex.isOutside()
    ) {
      coarseVertex.refine();
    }
  }
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::enterCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::beginIteration(
  dem::State&  solverState
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::endIteration(
  dem::State&  solverState
) {
}




void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::descend(
  dem::Cell * const          fineGridCells,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2GridBalancer3to1_5::ascend(
  dem::Cell * const    fineGridCells,
  dem::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell
) {
}
