#include "multigrid/mappings/AdditiveMG.h"
#include "multigrid/mappings/JacobiSmoother.h"
#include "multigrid/VertexOperations.h"



/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   multigrid::mappings::AdditiveMG::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,false);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::AdditiveMG::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::AdditiveMG::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::AdditiveMG::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::AdditiveMG::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::AdditiveMG::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::AdditiveMG::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


tarch::logging::Log                multigrid::mappings::AdditiveMG::_log( "multigrid::mappings::AdditiveMG" ); 




void multigrid::mappings::AdditiveMG::beginIteration(
  multigrid::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  _state = solverState;
  _state.clearAccumulatedAttributes();

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}



void multigrid::mappings::AdditiveMG::createHangingVertex(
      multigrid::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      multigrid::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      multigrid::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.setU(
    _multigrid.getDLinearInterpolatedValue(
      VertexOperations::readU( coarseGridVerticesEnumerator, coarseGridVertices ),
      fineGridPositionOfVertex
    )
  );
  fineGridVertex.clearHierarchicalValues();

  logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}


void multigrid::mappings::AdditiveMG::createInnerVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.setU(
    _multigrid.getDLinearInterpolatedValue(
      VertexOperations::readU( coarseGridVerticesEnumerator, coarseGridVertices ),
      fineGridPositionOfVertex
    )
  );

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}


void multigrid::mappings::AdditiveMG::touchVertexFirstTime(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.clearHierarchicalValues();

  if ( fineGridVertex.isInside() ) {
    const tarch::la::Vector<TWO_POWER_D,double > u_3h  = VertexOperations::readU(coarseGridVerticesEnumerator,coarseGridVertices);
    const tarch::la::Vector<TWO_POWER_D,double > e_3h  = VertexOperations::readUUpdate(coarseGridVerticesEnumerator,coarseGridVertices);
    const double                                 Pu_3h = _multigrid.getDLinearInterpolatedValue(u_3h,fineGridPositionOfVertex);
    const double                                 Pe_3h = _multigrid.getDLinearInterpolatedValue(e_3h,fineGridPositionOfVertex);

    fineGridVertex.correctU(Pe_3h);  // the order of these two operations is important
    fineGridVertex.determineUHierarchical(Pu_3h);

    if (fineGridVertex.getRefinementControl()!=Vertex::Records::Unrefined) {
      fineGridVertex.clearF();
//      fineGridVertex.clearUUpdate();
    }
  }

  fineGridVertex.clearAccumulatedAttributes();

  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}


void multigrid::mappings::AdditiveMG::enterCell(
      multigrid::Cell&                 fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  const tarch::la::Vector<TWO_POWER_D,double> u    =
    VertexOperations::readU( fineGridVerticesEnumerator, fineGridVertices );
  const tarch::la::Vector<TWO_POWER_D,double> dOld    =
    VertexOperations::readD( fineGridVerticesEnumerator, fineGridVertices );
  const tarch::la::Vector<TWO_POWER_D,double> rOld =
    VertexOperations::readR( fineGridVerticesEnumerator, fineGridVertices );
  const matrixfree::stencil::ElementWiseAssemblyMatrix A =
    fineGridCell.getElementsAssemblyMatrix( fineGridVerticesEnumerator.getCellSize() );

  tarch::la::Vector<TWO_POWER_D,double> r = rOld - A * u;
  tarch::la::Vector<TWO_POWER_D,double> d = dOld + tarch::la::diag(A);

  VertexOperations::writeR( fineGridVerticesEnumerator, fineGridVertices, r );
  VertexOperations::writeD( fineGridVerticesEnumerator, fineGridVertices, d );

  const tarch::la::Vector<TWO_POWER_D,double> hierarchicalU    =
     VertexOperations::readHierarchicalU( fineGridVerticesEnumerator, fineGridVertices );
   const tarch::la::Vector<TWO_POWER_D,double> hierarchicalROld =
     VertexOperations::readHierarchicalR( fineGridVerticesEnumerator, fineGridVertices );

   tarch::la::Vector<TWO_POWER_D,double> hierarchicalR = hierarchicalROld - A * hierarchicalU;

   VertexOperations::writeHierarchicalR( fineGridVerticesEnumerator, fineGridVertices, hierarchicalR );

   logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}


void multigrid::mappings::AdditiveMG::touchVertexLastTime(
  multigrid::Vertex&         fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
  multigrid::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  multigrid::Cell&           coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  if (
    fineGridVertex.isInside()
  ) {
    fineGridVertex.clearUUpdate();
    fineGridVertex.performJacobiSmoothingStep(
     fineGridVertex.getDampingFactorForAdditiveCoarseGridCorrection(multigrid::mappings::JacobiSmoother::omega)
    );
    if (
      fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined
    ) {
      _state.notifyAboutFineGridVertexUpdate(
        fineGridVertex.getResidual(),
        fineGridVertex.getU(),
        fineGridH
      );
    }
    else {
      _state.incNumberOfStencilEvaluations();
    }

    if (
     peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(
       fineGridPositionOfVertex
     )
    ) {
      const peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex coarseGridPosition =
        peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex);
      coarseGridVertices[ coarseGridVerticesEnumerator(coarseGridPosition) ].inject(fineGridVertex);
    }

    const tarch::la::Vector<TWO_POWER_D, double > P = _multigrid.calculateP(fineGridPositionOfVertex);

    dfor2(k)
      // There is no need to exclude boundary points here (the rhs does not play
      // there a role anyway), but it makes the visualisation nicer.
      if (
        coarseGridVertices[ coarseGridVerticesEnumerator(k) ].getRefinementControl()==Vertex::Records::Refined
        &&
        coarseGridVertices[ coarseGridVerticesEnumerator(k) ].isInside()
      ) {
        coarseGridVertices[ coarseGridVerticesEnumerator(k) ].incF(  P(kScalar) * fineGridVertex.getHierarchicalResidual() );
      }
    enddforx
  }

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}


void multigrid::mappings::AdditiveMG::endIteration(
  multigrid::State&  solverState
) {
  logTraceInWith1Argument( "endIteration(State)", solverState );

  solverState.merge(_state);

  logTraceOutWith1Argument( "endIteration(State)", solverState);
}




//
//   NOP
// =======
//
multigrid::mappings::AdditiveMG::AdditiveMG() {
}


multigrid::mappings::AdditiveMG::~AdditiveMG() {
}


#if defined(SharedMemoryParallelisation)
multigrid::mappings::AdditiveMG::AdditiveMG(const AdditiveMG&  masterThread) {
  logTraceIn( "AdditiveMG(AdditiveMG)" );
  // @todo Insert your code here
  logTraceOut( "AdditiveMG(AdditiveMG)" );
}


void multigrid::mappings::AdditiveMG::mergeWithWorkerThread(const AdditiveMG& workerThread) {
  logTraceIn( "mergeWithWorkerThread(AdditiveMG)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(AdditiveMG)" );
}
#endif


void multigrid::mappings::AdditiveMG::destroyHangingVertex(
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


void multigrid::mappings::AdditiveMG::createBoundaryVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}


void multigrid::mappings::AdditiveMG::destroyVertex(
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


void multigrid::mappings::AdditiveMG::createCell(
      multigrid::Cell&                 fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}


void multigrid::mappings::AdditiveMG::destroyCell(
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
void multigrid::mappings::AdditiveMG::mergeWithNeighbour(
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

void multigrid::mappings::AdditiveMG::prepareSendToNeighbour(
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

void multigrid::mappings::AdditiveMG::prepareCopyToRemoteNode(
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

void multigrid::mappings::AdditiveMG::prepareCopyToRemoteNode(
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

void multigrid::mappings::AdditiveMG::mergeWithRemoteDataDueToForkOrJoin(
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

void multigrid::mappings::AdditiveMG::mergeWithRemoteDataDueToForkOrJoin(
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

bool multigrid::mappings::AdditiveMG::prepareSendToWorker(
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

void multigrid::mappings::AdditiveMG::prepareSendToMaster(
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


void multigrid::mappings::AdditiveMG::mergeWithMaster(
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


void multigrid::mappings::AdditiveMG::receiveDataFromMaster(
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


void multigrid::mappings::AdditiveMG::mergeWithWorker(
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


void multigrid::mappings::AdditiveMG::mergeWithWorker(
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


void multigrid::mappings::AdditiveMG::leaveCell(
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



void multigrid::mappings::AdditiveMG::descend(
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


void multigrid::mappings::AdditiveMG::ascend(
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
