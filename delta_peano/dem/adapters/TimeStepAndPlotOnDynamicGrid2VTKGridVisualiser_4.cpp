#include "dem/adapters/TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4.h"

#include <sstream>

#include "peano/utils/Loop.h"
#include "peano/grid/CellFlags.h"

#ifdef Parallel
#include "tarch/parallel/Node.h"
#endif


int dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::_snapshotCounter = 0;



peano::CommunicationSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::communicationSpecification() {
  return peano::CommunicationSpecification::getPessimisticSpecification();
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::Serial,true);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::Serial,true);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}


peano::MappingSpecification   dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}


std::map<tarch::la::Vector<DIMENSIONS,double> , int, tarch::la::VectorCompare<DIMENSIONS> >  dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::_vertex2IndexMap;


dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4():
  _vtkWriter(0),
  _vertexWriter(0),
  _cellWriter(0),
  _vertexTypeWriter(0),
  _vertexRefinementControlWriter(0),
  _vertexAdjacentCellsHeight(0),
  _cellStateWriter(0) {
}


dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::~TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4() {
}


#if defined(SharedMemoryParallelisation)
dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4(const TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4&  masterThread):
  _vtkWriter(masterThread._vtkWriter),
  _vertexWriter(masterThread._vertexWriter),
  _cellWriter(masterThread._cellWriter),
  _vertexTypeWriter(masterThread._vertexTypeWriter),
  _vertexRefinementControlWriter(masterThread._vertexRefinementControlWriter),
  _vertexAdjacentCellsHeight(masterThread._vertexAdjacentCellsHeight),
  _cellStateWriter(masterThread._cellStateWriter)
{
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::mergeWithWorkerThread(const TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4& workerThread) {
}
#endif





void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::plotVertex(
  const dem::Vertex&                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridX
) {
  if ( _vertex2IndexMap.find(fineGridX) == _vertex2IndexMap.end() ) {
    assertion( _vertexWriter                  != nullptr );
    assertion( _vertexTypeWriter              != nullptr );
    assertion( _vertexRefinementControlWriter != nullptr );
    assertion( _vertexAdjacentCellsHeight     != nullptr );
    
    #if defined(Dim2) || defined(Dim3)
    _vertex2IndexMap[fineGridX] = _vertexWriter->plotVertex(fineGridX);
    #else
    _vertex2IndexMap[fineGridX] = _vertexWriter->plotVertex(tarch::la::Vector<3,double>(fineGridX.data()));
    #endif
    const int boundaryFlag = fineGridVertex.isHangingNode() ? -1 : fineGridVertex.isBoundary() ? 1 : 0;
    _vertexTypeWriter->plotVertex             (_vertex2IndexMap[fineGridX], boundaryFlag);
    _vertexRefinementControlWriter->plotVertex(_vertex2IndexMap[fineGridX],fineGridVertex.getRefinementControl() );
    _vertexAdjacentCellsHeight->plotVertex    (_vertex2IndexMap[fineGridX],fineGridVertex.getAdjacentCellsHeightOfPreviousIteration() );
  }
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  plotVertex( fineGridVertex, fineGridX ); 
}



void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::destroyHangingVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::destroyCell(
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
void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::mergeWithNeighbour(
  dem::Vertex&  vertex,
  const dem::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::prepareSendToNeighbour(
      dem::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::prepareCopyToRemoteNode(
      dem::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::prepareCopyToRemoteNode(
      dem::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
      int                                           level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::mergeWithRemoteDataDueToForkOrJoin(
  dem::Vertex&  localVertex,
  const dem::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::mergeWithRemoteDataDueToForkOrJoin(
  dem::Cell&  localCell,
  const dem::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
}


bool dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::prepareSendToWorker(
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


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::prepareSendToMaster(
      dem::Cell&                       localCell,
      dem::Vertex *                    vertices,
      const peano::grid::VertexEnumerator&       verticesEnumerator, 
      const dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
      const dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::mergeWithMaster(
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


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::receiveDataFromMaster(
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


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::mergeWithWorker(
      dem::Cell&           localCell, 
      const dem::Cell&     receivedMasterCell,
      const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
      int                                          level
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::mergeWithWorker(
      dem::Vertex&        localVertex,
      const dem::Vertex&  receivedMasterVertex,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}
#endif


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::touchVertexFirstTime(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  if (
    fineGridVertex.getRefinementControl()==dem::Vertex::Records::Unrefined ||
    fineGridVertex.getRefinementControl()==dem::Vertex::Records::RefinementTriggered ||
    fineGridVertex.getRefinementControl()==dem::Vertex::Records::Erasing
  ) {
    plotVertex( fineGridVertex, fineGridX ); 
  }
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::touchVertexLastTime(
      dem::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::enterCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
  #ifdef Parallel
  if (fineGridCell.isLeaf() && !fineGridCell.isAssignedToRemoteRank()) {
  #else
  if (fineGridCell.isLeaf()) {
  #endif
    assertion( DIMENSIONS==2 || DIMENSIONS==3 );
    int vertexIndex[TWO_POWER_D];
     dfor2(i)
      tarch::la::Vector<DIMENSIONS,double> currentVertexPosition = fineGridVerticesEnumerator.getVertexPosition(i);
      assertion2 ( _vertex2IndexMap.find(currentVertexPosition) != _vertex2IndexMap.end(), currentVertexPosition, fineGridVertices[ fineGridVerticesEnumerator(i) ].toString() );
      vertexIndex[iScalar] = _vertex2IndexMap[currentVertexPosition];
    enddforx
  
    int cellIndex;
    if (DIMENSIONS==2) {
      cellIndex = _cellWriter->plotQuadrangle(vertexIndex);
    }
    if (DIMENSIONS==3) {
      cellIndex = _cellWriter->plotHexahedron(vertexIndex);
    }
    
    _cellStateWriter->plotCell(cellIndex,fineGridVerticesEnumerator.getCellFlags());
  }
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::beginIteration(
  dem::State&  solverState
) {
  assertion( _vtkWriter==0 );
  
  _vtkWriter = new UsedWriter();
  
  _vertexWriter     = _vtkWriter->createVertexWriter();
  _cellWriter       = _vtkWriter->createCellWriter();
  
  _vertexTypeWriter               = _vtkWriter->createVertexDataWriter(dem::Vertex::Records::getInsideOutsideDomainMapping()+"/Hanging=-1" ,1);
  _vertexRefinementControlWriter  = _vtkWriter->createVertexDataWriter(dem::Vertex::Records::getRefinementControlMapping() ,1);
  _vertexAdjacentCellsHeight      = _vtkWriter->createVertexDataWriter( peano::grid::getCellFlagsLegend(),1);

  _cellStateWriter                = _vtkWriter->createCellDataWriter( "cell-flag(>=-1=stationary,-1=parallel-boundary,<=-2=not-stationary" ,1);
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::endIteration(
  dem::State&  solverState
) {
  _vertexWriter->close();
  _cellWriter->close();
  
  _vertexTypeWriter->close();
  _vertexRefinementControlWriter->close();
  _vertexAdjacentCellsHeight->close();
  _cellStateWriter->close();
  
  delete _vertexWriter;
  delete _cellWriter;
  delete _vertexTypeWriter;
  delete _vertexRefinementControlWriter;
  delete _vertexAdjacentCellsHeight;
  delete _cellStateWriter;
  
  _vertexWriter                  = nullptr;
  _cellWriter                    = nullptr;
  _vertexTypeWriter              = nullptr;
  _vertexRefinementControlWriter = nullptr;
  _vertexAdjacentCellsHeight     = nullptr;
  _cellStateWriter               = nullptr;
  
  std::ostringstream snapshotFileName;
  snapshotFileName << "grid"
                   #ifdef Parallel
                   << "-rank-" << tarch::parallel::Node::getInstance().getRank()
                   #endif
                   << "-" << _snapshotCounter
                   << ".vtk";
  _vtkWriter->writeToFile( snapshotFileName.str() );
  
  _snapshotCounter++;                  
  
  _vertex2IndexMap.clear();
  
  delete _vtkWriter;
  _vtkWriter = nullptr;
}




void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::descend(
  dem::Cell * const          fineGridCells,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell
) {
}


void dem::adapters::TimeStepAndPlotOnDynamicGrid2VTKGridVisualiser_4::ascend(
  dem::Cell * const    fineGridCells,
  dem::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell
) {
}
