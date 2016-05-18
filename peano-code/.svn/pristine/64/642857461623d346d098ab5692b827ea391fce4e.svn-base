#include "multigrid/mappings/CreateGrid.h"



/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   multigrid::mappings::CreateGrid::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,false);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::CreateGrid::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::CreateGrid::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::CreateGrid::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::CreateGrid::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::CreateGrid::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::CreateGrid::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


tarch::logging::Log                multigrid::mappings::CreateGrid::_log( "multigrid::mappings::CreateGrid" ); 



multigrid::mappings::CreateGrid::Scenario  multigrid::mappings::CreateGrid::_scenario;



void multigrid::mappings::CreateGrid::refineInitialGrid(
  multigrid::Vertex&               fineGridVertex,
  int                              vertexLevel,
  bool                             isBoundary
) {
  if ( _scenario==Poisson2 && vertexLevel<=2 ) {
    fineGridVertex.refine();
  }
  if ( _scenario==Poisson3 && vertexLevel<=3 ) {
    fineGridVertex.refine();
  }
  if ( _scenario==Poisson4 && vertexLevel<=4 ) {
    fineGridVertex.refine();
  }
  if ( _scenario==Poisson5 && vertexLevel<=5 ) {
    fineGridVertex.refine();
  }

  if ( _scenario==AdaptivePoisson2 && vertexLevel<=2 && isBoundary) {
    fineGridVertex.refine();
  }
  if ( _scenario==AdaptivePoisson2 && vertexLevel<=2 && !isBoundary) {
    fineGridVertex.refine();
  }
  if ( _scenario==AdaptivePoisson3 && vertexLevel<=3 && isBoundary) {
    fineGridVertex.refine();
  }
  if ( _scenario==AdaptivePoisson3 && vertexLevel<=2 && !isBoundary) {
    fineGridVertex.refine();
  }
  if ( _scenario==AdaptivePoisson4 && vertexLevel<=4 && isBoundary) {
    fineGridVertex.refine();
  }
  if ( _scenario==AdaptivePoisson4 && vertexLevel<=2 && !isBoundary) {
    fineGridVertex.refine();
  }
  if ( _scenario==AdaptivePoisson5 && vertexLevel<=5 && isBoundary) {
    fineGridVertex.refine();
  }
  if ( _scenario==AdaptivePoisson5 && vertexLevel<=2 && !isBoundary) {
    fineGridVertex.refine();
  }
}


void multigrid::mappings::CreateGrid::createInnerVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  refineInitialGrid(fineGridVertex,coarseGridVerticesEnumerator.getLevel()+1, false);

  if (
    _scenario==Poisson2         || _scenario==Poisson3         || _scenario==Poisson4         || _scenario==Poisson5
    ||
    _scenario==AdaptivePoisson2 || _scenario==AdaptivePoisson3 || _scenario==AdaptivePoisson4 || _scenario==AdaptivePoisson5
  ) {
    double f = DIMENSIONS * tarch::la::PI * tarch::la::PI;
    for (int d=0; d<DIMENSIONS; d++) {
      f *= std::sin( tarch::la::PI * fineGridX(d) );
    }
    fineGridVertex.initInnerVertex(f,fineGridH);
  }
  else {
    assertionMsg( false, "not implemented yet" );
  }

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}


void multigrid::mappings::CreateGrid::createBoundaryVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );


  // set boolean constant to true if you wanna start with the finest mesh along the boundary
  //refineInitialGrid(fineGridVertex,coarseGridVerticesEnumerator.getLevel()+1, true);
  refineInitialGrid(fineGridVertex,coarseGridVerticesEnumerator.getLevel()+1, false);

  if (
    _scenario==Poisson2         || _scenario==Poisson3         || _scenario==Poisson4         || _scenario==Poisson5
    ||
    _scenario==AdaptivePoisson2 || _scenario==AdaptivePoisson3 || _scenario==AdaptivePoisson4 || _scenario==AdaptivePoisson5
  ) {
    fineGridVertex.initDirichletVertex(0.0);
  }
  else {
    assertionMsg( false, "not implemented yet" );
  }

  fineGridVertex.clearF();

  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}


void multigrid::mappings::CreateGrid::createCell(
      multigrid::Cell&                 fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  if (
    _scenario==Poisson2         || _scenario==Poisson3         || _scenario==Poisson4         || _scenario==Poisson5
    ||
    _scenario==AdaptivePoisson2 || _scenario==AdaptivePoisson3 || _scenario==AdaptivePoisson4 || _scenario==AdaptivePoisson5
  ) {
    fineGridCell.init(
      tarch::la::Vector<DIMENSIONS,double>(1.0), // epsilon
      tarch::la::Vector<DIMENSIONS,double>(0.0)  // v
    );
  }
  else {
    logError( "createCell(...)", "not supported yet" );
    assertion(false);
  }

  logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}



void multigrid::mappings::CreateGrid::createHangingVertex(
      multigrid::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      multigrid::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      multigrid::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.clearF();

  logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}



//
//   NOP
// =======
//
multigrid::mappings::CreateGrid::CreateGrid() {
  logTraceIn( "CreateGrid()" );
  // @todo Insert your code here
  logTraceOut( "CreateGrid()" );
}


multigrid::mappings::CreateGrid::~CreateGrid() {
  logTraceIn( "~CreateGrid()" );
  // @todo Insert your code here
  logTraceOut( "~CreateGrid()" );
}


#if defined(SharedMemoryParallelisation)
multigrid::mappings::CreateGrid::CreateGrid(const CreateGrid&  masterThread) {
  logTraceIn( "CreateGrid(CreateGrid)" );
  // @todo Insert your code here
  logTraceOut( "CreateGrid(CreateGrid)" );
}


void multigrid::mappings::CreateGrid::mergeWithWorkerThread(const CreateGrid& workerThread) {
  logTraceIn( "mergeWithWorkerThread(CreateGrid)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(CreateGrid)" );
}
#endif


void multigrid::mappings::CreateGrid::destroyHangingVertex(
      const multigrid::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}


void multigrid::mappings::CreateGrid::destroyVertex(
      const multigrid::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}


void multigrid::mappings::CreateGrid::destroyCell(
      const multigrid::Cell&           fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void multigrid::mappings::CreateGrid::mergeWithNeighbour(
  multigrid::Vertex&  vertex,
  const multigrid::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
  logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
  // @todo Insert your code here
  logTraceOut( "mergeWithNeighbour(...)" );
}

void multigrid::mappings::CreateGrid::prepareSendToNeighbour(
  multigrid::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
  logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
  // @todo Insert your code here
  logTraceOut( "prepareSendToNeighbour(...)" );
}

void multigrid::mappings::CreateGrid::prepareCopyToRemoteNode(
  multigrid::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
  logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
  // @todo Insert your code here
  logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void multigrid::mappings::CreateGrid::prepareCopyToRemoteNode(
  multigrid::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
      int                                           level
) {
  logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localCell, toRank, cellCentre, cellSize, level );
  // @todo Insert your code here
  logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void multigrid::mappings::CreateGrid::mergeWithRemoteDataDueToForkOrJoin(
  multigrid::Vertex&  localVertex,
  const multigrid::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
  logTraceInWith6Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localVertex, masterOrWorkerVertex, fromRank, x, h, level );
  // @todo Insert your code here
  logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

void multigrid::mappings::CreateGrid::mergeWithRemoteDataDueToForkOrJoin(
  multigrid::Cell&  localCell,
  const multigrid::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                       level
) {
  logTraceInWith3Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localCell, masterOrWorkerCell, fromRank );
  // @todo Insert your code here
  logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

bool multigrid::mappings::CreateGrid::prepareSendToWorker(
  multigrid::Cell&                 fineGridCell,
  multigrid::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  multigrid::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  multigrid::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker
) {
  logTraceIn( "prepareSendToWorker(...)" );
  // @todo Insert your code here
  logTraceOutWith1Argument( "prepareSendToWorker(...)", true );
  return true;
}

void multigrid::mappings::CreateGrid::prepareSendToMaster(
  multigrid::Cell&                       localCell,
  multigrid::Vertex *                    vertices,
  const peano::grid::VertexEnumerator&       verticesEnumerator, 
  const multigrid::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  const multigrid::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
  logTraceInWith2Arguments( "prepareSendToMaster(...)", localCell, verticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "prepareSendToMaster(...)" );
}


void multigrid::mappings::CreateGrid::mergeWithMaster(
  const multigrid::Cell&           workerGridCell,
  multigrid::Vertex * const        workerGridVertices,
 const peano::grid::VertexEnumerator& workerEnumerator,
  multigrid::Cell&                 fineGridCell,
  multigrid::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  multigrid::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  multigrid::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker,
  const multigrid::State&          workerState,
  multigrid::State&                masterState
) {
  logTraceIn( "mergeWithMaster(...)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithMaster(...)" );
}


void multigrid::mappings::CreateGrid::receiveDataFromMaster(
      multigrid::Cell&                        receivedCell, 
      multigrid::Vertex *                     receivedVertices,
      const peano::grid::VertexEnumerator&        receivedVerticesEnumerator,
      multigrid::Vertex * const               receivedCoarseGridVertices,
      const peano::grid::VertexEnumerator&        receivedCoarseGridVerticesEnumerator,
      multigrid::Cell&                        receivedCoarseGridCell,
      multigrid::Vertex * const               workersCoarseGridVertices,
      const peano::grid::VertexEnumerator&        workersCoarseGridVerticesEnumerator,
      multigrid::Cell&                        workersCoarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&    fineGridPositionOfCell
) {
  logTraceIn( "receiveDataFromMaster(...)" );
  // @todo Insert your code here
  logTraceOut( "receiveDataFromMaster(...)" );
}


void multigrid::mappings::CreateGrid::mergeWithWorker(
  multigrid::Cell&           localCell, 
  const multigrid::Cell&     receivedMasterCell,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                          level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}


void multigrid::mappings::CreateGrid::mergeWithWorker(
  multigrid::Vertex&        localVertex,
  const multigrid::Vertex&  receivedMasterVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif

void multigrid::mappings::CreateGrid::touchVertexFirstTime(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}


void multigrid::mappings::CreateGrid::touchVertexLastTime(
      multigrid::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}


void multigrid::mappings::CreateGrid::enterCell(
      multigrid::Cell&                 fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}


void multigrid::mappings::CreateGrid::leaveCell(
      multigrid::Cell&           fineGridCell,
      multigrid::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "leaveCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "leaveCell(...)", fineGridCell );
}


void multigrid::mappings::CreateGrid::beginIteration(
  multigrid::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );
  // @todo Insert your code here
  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}


void multigrid::mappings::CreateGrid::endIteration(
  multigrid::State&  solverState
) {
  logTraceInWith1Argument( "endIteration(State)", solverState );
  // @todo Insert your code here
  logTraceOutWith1Argument( "endIteration(State)", solverState);
}



void multigrid::mappings::CreateGrid::descend(
  multigrid::Cell * const          fineGridCells,
  multigrid::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  multigrid::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  multigrid::Cell&                 coarseGridCell
) {
  logTraceInWith2Arguments( "descend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "descend(...)" );
}


void multigrid::mappings::CreateGrid::ascend(
  multigrid::Cell * const    fineGridCells,
  multigrid::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  multigrid::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  multigrid::Cell&           coarseGridCell
) {
  logTraceInWith2Arguments( "ascend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "ascend(...)" );
}
