#include "myproject/mappings/TimeStep.h"
#include "myproject/VertexOperations.h"
#include "tarch/la/Matrix.h"


peano::CommunicationSpecification   myproject::mappings::TimeStep::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,false);
}
peano::MappingSpecification   myproject::mappings::TimeStep::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
peano::MappingSpecification   myproject::mappings::TimeStep::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
peano::MappingSpecification   myproject::mappings::TimeStep::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}
peano::MappingSpecification   myproject::mappings::TimeStep::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}
peano::MappingSpecification   myproject::mappings::TimeStep::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}
peano::MappingSpecification   myproject::mappings::TimeStep::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


tarch::logging::Log                myproject::mappings::TimeStep::_log( "myproject::mappings::TimeStep" ); 


myproject::mappings::TimeStep::TimeStep() {
}


myproject::mappings::TimeStep::~TimeStep() {
}


#if defined(SharedMemoryParallelisation)
myproject::mappings::TimeStep::TimeStep(const TimeStep&  masterThread) {
}


void myproject::mappings::TimeStep::mergeWithWorkerThread(const TimeStep& workerThread) {
}
#endif


void myproject::mappings::TimeStep::createHangingVertex(
      myproject::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      myproject::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      myproject::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
}


void myproject::mappings::TimeStep::destroyHangingVertex(
      const myproject::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void myproject::mappings::TimeStep::createInnerVertex(
      myproject::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void myproject::mappings::TimeStep::createBoundaryVertex(
      myproject::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void myproject::mappings::TimeStep::destroyVertex(
      const myproject::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void myproject::mappings::TimeStep::createCell(
      myproject::Cell&                 fineGridCell,
      myproject::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}


void myproject::mappings::TimeStep::destroyCell(
      const myproject::Cell&           fineGridCell,
      myproject::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}

#ifdef Parallel
void myproject::mappings::TimeStep::mergeWithNeighbour(
  myproject::Vertex&  vertex,
  const myproject::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
}

void myproject::mappings::TimeStep::prepareSendToNeighbour(
  myproject::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}

void myproject::mappings::TimeStep::prepareCopyToRemoteNode(
  myproject::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
}

void myproject::mappings::TimeStep::prepareCopyToRemoteNode(
  myproject::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
      int                                           level
) {
}

void myproject::mappings::TimeStep::mergeWithRemoteDataDueToForkOrJoin(
  myproject::Vertex&  localVertex,
  const myproject::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
}

void myproject::mappings::TimeStep::mergeWithRemoteDataDueToForkOrJoin(
  myproject::Cell&  localCell,
  const myproject::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                       level
) {
}

bool myproject::mappings::TimeStep::prepareSendToWorker(
  myproject::Cell&                 fineGridCell,
  myproject::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  myproject::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  myproject::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker
) {
  return true;
}

void myproject::mappings::TimeStep::prepareSendToMaster(
  myproject::Cell&                       localCell,
  myproject::Vertex *                    vertices,
  const peano::grid::VertexEnumerator&       verticesEnumerator, 
  const myproject::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  const myproject::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
}


void myproject::mappings::TimeStep::mergeWithMaster(
  const myproject::Cell&           workerGridCell,
  myproject::Vertex * const        workerGridVertices,
 const peano::grid::VertexEnumerator& workerEnumerator,
  myproject::Cell&                 fineGridCell,
  myproject::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  myproject::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  myproject::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker,
  const myproject::State&          workerState,
  myproject::State&                masterState
) {
}


void myproject::mappings::TimeStep::receiveDataFromMaster(
      myproject::Cell&                        receivedCell, 
      myproject::Vertex *                     receivedVertices,
      const peano::grid::VertexEnumerator&        receivedVerticesEnumerator,
      myproject::Vertex * const               receivedCoarseGridVertices,
      const peano::grid::VertexEnumerator&        receivedCoarseGridVerticesEnumerator,
      myproject::Cell&                        receivedCoarseGridCell,
      myproject::Vertex * const               workersCoarseGridVertices,
      const peano::grid::VertexEnumerator&        workersCoarseGridVerticesEnumerator,
      myproject::Cell&                        workersCoarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&    fineGridPositionOfCell
) {
}


void myproject::mappings::TimeStep::mergeWithWorker(
  myproject::Cell&           localCell, 
  const myproject::Cell&     receivedMasterCell,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                          level
) {
}


void myproject::mappings::TimeStep::mergeWithWorker(
  myproject::Vertex&        localVertex,
  const myproject::Vertex&  receivedMasterVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
}
#endif

void myproject::mappings::TimeStep::touchVertexFirstTime(
      myproject::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  if (fineGridVertex.isBoundary() && fineGridX(0)<1e-8) {
    VertexOperations::writeU( fineGridVertex, 1.0 );
  }
  else if (fineGridVertex.isBoundary()) {
    VertexOperations::writeU( fineGridVertex, 0.0 );
  }

  fineGridVertex.copyCurrentSolutionIntoOldSolution();

  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}


void myproject::mappings::TimeStep::touchVertexLastTime(
      myproject::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void myproject::mappings::TimeStep::enterCell(
      myproject::Cell&                 fineGridCell,
      myproject::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  tarch::la::Matrix<TWO_POWER_D,TWO_POWER_D,double> A;

  A =  6.0/8.0, -1.0/4.0, -1.0/4.0,      0.0, -1.0/4.0,      0.0,      0.0,      0.0,
      -1.0/4.0,  6.0/8.0,      0.0, -1.0/4.0,      0.0, -1.0/4.0,      0.0,      0.0,
      -1.0/4.0,      0.0,  6.0/8.0, -1.0/4.0,      0.0,      0.0, -1.0/4.0,      0.0,
           0.0, -1.0/4.0, -1.0/4.0,  6.0/8.0,      0.0,      0.0,      0.0, -1.0/4.0,
      -1.0/4.0,      0.0,      0.0,      0.0,  6.0/8.0, -1.0/4.0, -1.0/4.0,      0.0,
           0.0, -1.0/4.0,      0.0,      0.0, -1.0/4.0,  6.0/8.0,      0.0, -1.0/4.0,
           0.0,      0.0, -1.0/4.0,      0.0, -1.0/4.0,      0.0,  6.0/8.0, -1.0/4.0,
           0.0,      0.0,      0.0, -1.0/4.0,      0.0, -1.0/4.0, -1.0/4.0,  6.0/8.0;

  tarch::la::Vector<TWO_POWER_D,double> uOld = VertexOperations::readOldU(fineGridVerticesEnumerator,fineGridVertices);

  assertion(fineGridCell.getEpsilon()>0.0);
  assertion(_timeStepSize>0.0);

  const double h = fineGridVerticesEnumerator.getCellSize()(0);
  const double scaling = (-_timeStepSize * fineGridCell.getEpsilon()/ h / h);
  tarch::la::Vector<TWO_POWER_D,double> uUpdate = scaling * A * uOld;

  VertexOperations::writeU(
    fineGridVerticesEnumerator,fineGridVertices,
    VertexOperations::readU(fineGridVerticesEnumerator,fineGridVertices) + uUpdate
  );

  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}


void myproject::mappings::TimeStep::leaveCell(
      myproject::Cell&           fineGridCell,
      myproject::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
}


void myproject::mappings::TimeStep::beginIteration(
  myproject::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  _timeStepSize = solverState.getTimeStepSize();

  assertion2( _timeStepSize>0.0, _timeStepSize, solverState.toString() );

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}


void myproject::mappings::TimeStep::endIteration(
  myproject::State&  solverState
) {
}



void myproject::mappings::TimeStep::descend(
  myproject::Cell * const          fineGridCells,
  myproject::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  myproject::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  myproject::Cell&                 coarseGridCell
) {
}


void myproject::mappings::TimeStep::ascend(
  myproject::Cell * const    fineGridCells,
  myproject::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  myproject::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  myproject::Cell&           coarseGridCell
) {
}
