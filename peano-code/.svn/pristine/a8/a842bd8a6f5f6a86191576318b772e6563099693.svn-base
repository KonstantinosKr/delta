#include "myproject/mappings/RefineDynamically.h"
#include "myproject/VertexOperations.h"
#include "tarch/la/Matrix.h"
#include "peano/utils/Loop.h"


peano::CommunicationSpecification   myproject::mappings::RefineDynamically::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,false);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   myproject::mappings::RefineDynamically::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   myproject::mappings::RefineDynamically::touchVertexFirstTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   myproject::mappings::RefineDynamically::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   myproject::mappings::RefineDynamically::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   myproject::mappings::RefineDynamically::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   myproject::mappings::RefineDynamically::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


tarch::logging::Log                myproject::mappings::RefineDynamically::_log( "myproject::mappings::RefineDynamically" );


myproject::mappings::RefineDynamically::RefineDynamically() {
  logTraceIn( "RefineDynamically()" );
  // @todo Insert your code here
  logTraceOut( "RefineDynamically()" );
}


myproject::mappings::RefineDynamically::~RefineDynamically() {
  logTraceIn( "~RefineDynamically()" );
  // @todo Insert your code here
  logTraceOut( "~RefineDynamically()" );
}


#if defined(SharedMemoryParallelisation)
myproject::mappings::RefineDynamically::RefineDynamically(const RefineDynamically&  masterThread) {
  logTraceIn( "RefineDynamically(RefineDynamically)" );
  // @todo Insert your code here
  logTraceOut( "RefineDynamically(RefineDynamically)" );
}


void myproject::mappings::RefineDynamically::mergeWithWorkerThread(const RefineDynamically& workerThread) {
  logTraceIn( "mergeWithWorkerThread(RefineDynamically)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(RefineDynamically)" );
}
#endif


void myproject::mappings::RefineDynamically::createHangingVertex(
      myproject::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      myproject::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      myproject::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
}


void myproject::mappings::RefineDynamically::destroyHangingVertex(
      const myproject::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void myproject::mappings::RefineDynamically::createInnerVertex(
      myproject::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  double interpolatedValue = 0.0;
  dfor2(k)
    double weight = 1.0;
    for (int d=0; d<DIMENSIONS; d++) {
      if (k(d)==0) {
        weight *= 1.0 - (fineGridPositionOfVertex(d))/3.0;
      }
      else {
        weight *= (fineGridPositionOfVertex(d))/3.0;
      }
    }
    interpolatedValue = weight * coarseGridVertices[ coarseGridVerticesEnumerator(k)].getU();
  enddforx

  VertexOperations::writeU( fineGridVertex, interpolatedValue );
  fineGridVertex.copyCurrentSolutionIntoOldSolution();

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}


void myproject::mappings::RefineDynamically::createBoundaryVertex(
      myproject::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void myproject::mappings::RefineDynamically::destroyVertex(
      const myproject::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void myproject::mappings::RefineDynamically::createCell(
      myproject::Cell&                 fineGridCell,
      myproject::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  fineGridCell.init( fineGridVerticesEnumerator.getCellCenter() );

  logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}


void myproject::mappings::RefineDynamically::destroyCell(
      const myproject::Cell&           fineGridCell,
      myproject::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void myproject::mappings::RefineDynamically::mergeWithNeighbour(
  myproject::Vertex&  vertex,
  const myproject::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
  logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
  // @todo Insert your code here
  logTraceOut( "mergeWithNeighbour(...)" );
}

void myproject::mappings::RefineDynamically::prepareSendToNeighbour(
  myproject::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
  logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
  // @todo Insert your code here
  logTraceOut( "prepareSendToNeighbour(...)" );
}

void myproject::mappings::RefineDynamically::prepareCopyToRemoteNode(
  myproject::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
  logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
  // @todo Insert your code here
  logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void myproject::mappings::RefineDynamically::prepareCopyToRemoteNode(
  myproject::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
      int                                           level
) {
  logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localCell, toRank, cellCentre, cellSize, level );
  // @todo Insert your code here
  logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void myproject::mappings::RefineDynamically::mergeWithRemoteDataDueToForkOrJoin(
  myproject::Vertex&  localVertex,
  const myproject::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
  logTraceInWith6Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localVertex, masterOrWorkerVertex, fromRank, x, h, level );
  // @todo Insert your code here
  logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

void myproject::mappings::RefineDynamically::mergeWithRemoteDataDueToForkOrJoin(
  myproject::Cell&  localCell,
  const myproject::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                       level
) {
  logTraceInWith3Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localCell, masterOrWorkerCell, fromRank );
  // @todo Insert your code here
  logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

bool myproject::mappings::RefineDynamically::prepareSendToWorker(
  myproject::Cell&                 fineGridCell,
  myproject::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  myproject::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  myproject::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker
) {
  logTraceIn( "prepareSendToWorker(...)" );
  // @todo Insert your code here
  logTraceOutWith1Argument( "prepareSendToWorker(...)", true );
  return true;
}

void myproject::mappings::RefineDynamically::prepareSendToMaster(
  myproject::Cell&                       localCell,
  myproject::Vertex *                    vertices,
  const peano::grid::VertexEnumerator&       verticesEnumerator, 
  const myproject::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  const myproject::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
  logTraceInWith2Arguments( "prepareSendToMaster(...)", localCell, verticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "prepareSendToMaster(...)" );
}


void myproject::mappings::RefineDynamically::mergeWithMaster(
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
  logTraceIn( "mergeWithMaster(...)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithMaster(...)" );
}


void myproject::mappings::RefineDynamically::receiveDataFromMaster(
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
  logTraceIn( "receiveDataFromMaster(...)" );
  // @todo Insert your code here
  logTraceOut( "receiveDataFromMaster(...)" );
}


void myproject::mappings::RefineDynamically::mergeWithWorker(
  myproject::Cell&           localCell, 
  const myproject::Cell&     receivedMasterCell,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                          level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}


void myproject::mappings::RefineDynamically::mergeWithWorker(
  myproject::Vertex&        localVertex,
  const myproject::Vertex&  receivedMasterVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif

void myproject::mappings::RefineDynamically::touchVertexFirstTime(
      myproject::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      myproject::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      myproject::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  VertexOperations::writeAveragedU( fineGridVertex, 0.0 );

  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}


void myproject::mappings::RefineDynamically::touchVertexLastTime(
      myproject::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  if (coarseGridVerticesEnumerator.getLevel()<5) {
    fineGridVertex.evaluateRefinementCiterion();
  }

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}


void myproject::mappings::RefineDynamically::enterCell(
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

  A =  0.0,  1.0/4.0,  1.0/4.0,  0.0,  1.0/4.0,  0.0,  0.0,  0.0,
      -1.0/4.0,  0.0,  0.0,  1.0/4.0,  0.0,  1.0/4.0,  0.0,  0.0,
      -1.0/4.0,  0.0,  0.0,  1.0/4.0,  0.0,  0.0,  1.0/4.0,  0.0,
       0.0, -1.0/4.0, -1.0/4.0,  0.0,  0.0,  0.0,  0.0,  1.0/4.0,
      -1.0/4.0,  0.0,  0.0,  0.0,  0.0,  1.0/4.0,  1.0/4.0,  0.0,
       0.0, -1.0/4.0,  0.0,  0.0, -1.0/4.0,  0.0,  0.0,  1.0/4.0,
       0.0,  0.0, -1.0/4.0,  0.0, -1.0/4.0,  0.0,  0.0,  1.0/4.0,
       0.0,  0.0,  0.0, -1.0/4.0,  0.0, -1.0/4.0, -1.0/4.0,  0.0;

  tarch::la::Vector<TWO_POWER_D,double> uOld = VertexOperations::readOldU(fineGridVerticesEnumerator,fineGridVertices);

  assertion(fineGridCell.getEpsilon()>0.0);

  const double h       = fineGridVerticesEnumerator.getCellSize()(0);
  const double scaling = 1.0/h;
  tarch::la::Vector<TWO_POWER_D,double> averageUpdate = scaling * A * uOld;

  VertexOperations::writeAveragedU(
    fineGridVerticesEnumerator,fineGridVertices,
    VertexOperations::readAveragedU(fineGridVerticesEnumerator,fineGridVertices) + averageUpdate
  );

  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}


void myproject::mappings::RefineDynamically::leaveCell(
      myproject::Cell&           fineGridCell,
      myproject::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      myproject::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      myproject::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
}


void myproject::mappings::RefineDynamically::beginIteration(
  myproject::State&  solverState
) {
}


void myproject::mappings::RefineDynamically::endIteration(
  myproject::State&  solverState
) {
}



void myproject::mappings::RefineDynamically::descend(
  myproject::Cell * const          fineGridCells,
  myproject::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  myproject::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  myproject::Cell&                 coarseGridCell
) {
}


void myproject::mappings::RefineDynamically::ascend(
  myproject::Cell * const    fineGridCells,
  myproject::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  myproject::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  myproject::Cell&           coarseGridCell
) {
}
