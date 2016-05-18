
#include "multigrid/mappings/RefinementCriterion.h"
#include "multigrid/mappings/CreateGrid.h"
#include "multigrid/VertexOperations.h"


#include "peano/grid/aspects/VertexStateAnalysis.h"
#include "peano/utils/Loop.h"


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   multigrid::mappings::RefinementCriterion::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,false);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::RefinementCriterion::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::RefinementCriterion::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::RefinementCriterion::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::RefinementCriterion::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::RefinementCriterion::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::MappingSpecification   multigrid::mappings::RefinementCriterion::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces);
}


tarch::logging::Log  multigrid::mappings::RefinementCriterion::_log( "multigrid::mappings::RefinementCriterion" );
double               multigrid::mappings::RefinementCriterion::_convergenceThreshold(1e-2);


multigrid::mappings::RefinementCriterion::RefinementCriterion():
  _refinementCriterion(
    0.1,                   // refinementPercentage,
    0.0,                   // deletePercentage,
    0.5,                   // minimumMeshSize,
    0.5                    // maximumMeshSize
  ) {
}


void multigrid::mappings::RefinementCriterion::beginIteration(
  multigrid::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  switch (multigrid::mappings::CreateGrid::_scenario) {
    case multigrid::mappings::CreateGrid::Poisson2:
    case multigrid::mappings::CreateGrid::AdaptivePoisson2:
      _refinementCriterion.setMinMaxMeshWidth( 1.0/3.0, 0.5 );
      break;
    case multigrid::mappings::CreateGrid::Poisson3:
    case multigrid::mappings::CreateGrid::AdaptivePoisson3:
      _refinementCriterion.setMinMaxMeshWidth( 1.0/3.0/3.0, 0.5 );
      break;
    case multigrid::mappings::CreateGrid::Poisson4:
    case multigrid::mappings::CreateGrid::AdaptivePoisson4:
      _refinementCriterion.setMinMaxMeshWidth( 1.0/3.0/3.0/3.0, 0.5 );
      break;
    case multigrid::mappings::CreateGrid::Poisson5:
    case multigrid::mappings::CreateGrid::AdaptivePoisson5:
      _refinementCriterion.setMinMaxMeshWidth( 1.0/3.0/3.0/3.0/3.0, 0.5 );
      break;
    default:
      assertionMsg( false, "not supported yet" );
  }

  _refinementCriterion.clearMeasurements(0.0);

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}


void multigrid::mappings::RefinementCriterion::touchVertexFirstTime(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  VertexOperations::writeLinearSurplus(fineGridVertex,0.0);

  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}




void multigrid::mappings::RefinementCriterion::enterCell(
      multigrid::Cell&                 fineGridCell,
      multigrid::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  VertexOperations::writeLinearSurplus(
    fineGridVerticesEnumerator,
    fineGridVertices,
    _refinementCriterion.getNewLinearSurplus(
      VertexOperations::readU(fineGridVerticesEnumerator,fineGridVertices),
      VertexOperations::readLinearSurplus(fineGridVerticesEnumerator,fineGridVertices)
    )
  );


  if (
    fineGridCell.isRefined()
    &&
    peano::grid::aspects::VertexStateAnalysis::doesOneVertexCarryRefinementFlag(
      fineGridVertices, fineGridVerticesEnumerator, Vertex::Records::Unrefined
    )
  ) {
    bool isOneVertexABoundaryVertex = false;
    dfor2(k)
      isOneVertexABoundaryVertex |= fineGridVertices[ fineGridVerticesEnumerator(k) ].isBoundary();
    enddforx
    if (isOneVertexABoundaryVertex) {
      dfor2(k)
        if (fineGridVertices[ fineGridVerticesEnumerator(k) ].getRefinementControl()==Vertex::Records::Unrefined) {
          fineGridVertices[ fineGridVerticesEnumerator(k) ].refine();
        }
      enddforx
    }
  }

  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}


void multigrid::mappings::RefinementCriterion::touchVertexLastTime(
  multigrid::Vertex&                           fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
  multigrid::Vertex * const                    coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  multigrid::Cell&                             coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  if ( fineGridVertex.isInside() ) {
    const tarch::la::Vector<TWO_POWER_D_TIMES_D,double > coarseGridLinearSurplus =
      VertexOperations::readLinearSurplus(coarseGridVerticesEnumerator, coarseGridVertices)
      +
      _refinementCriterion.getLinearSurplusContributionFromFineGrid(
        VertexOperations::readLinearSurplus( fineGridVertex ),
        fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined,
        fineGridPositionOfVertex
      );

    VertexOperations::writeLinearSurplus( coarseGridVerticesEnumerator, coarseGridVertices, coarseGridLinearSurplus );

    switch (
      _refinementCriterion.analyse(
        VertexOperations::readLinearSurplus(fineGridVertex),
        fineGridVertex.getRefinementControl()==Vertex::Records::Refined,
        fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined,
        fineGridH
      )
    ) {
      case matrixfree::adaptivitycriteria::LinearSurplusRefinementCriterion::Refine:
        if ( tarch::la::abs( fineGridVertex.getResidual() )<_convergenceThreshold) {
          fineGridVertex.refine();
        }
        else {
          logDebug( "touchVertexLastTime(...)", "skip refinement as vertex has not converged yet. r=" << fineGridVertex.getR() << ", |r|=" << tarch::la::abs(fineGridVertex.getR()) );
        }
        break;
      case matrixfree::adaptivitycriteria::LinearSurplusRefinementCriterion::Delete:
        //fineGridVertex.erase();
        break;
      case matrixfree::adaptivitycriteria::LinearSurplusRefinementCriterion::NoAction:
        break;
    }
  }

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}


void multigrid::mappings::RefinementCriterion::createInnerVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  // We would like to write the following here:
  //
  // fineGridVertex.setU(
  //   _multigrid.getDLinearInterpolatedValue(
  //     VertexOperations::readU( coarseGridVerticesEnumerator, coarseGridVertices ),
  //     fineGridPositionOfVertex
  //   )
  // );
  //
  // This does not work as we have no instance of the multigrid solver here at
  // hands. Rather than introducing yet another multigrid solver instance here,
  // we thus implement the createInnerVertex in the smoother mapping.

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}



//
//   Nop
// ========
//
void multigrid::mappings::RefinementCriterion::endIteration(
  multigrid::State&  solverState
) {
}


multigrid::mappings::RefinementCriterion::~RefinementCriterion() {
}


#if defined(SharedMemoryParallelisation)
multigrid::mappings::RefinementCriterion::RefinementCriterion(const RefinementCriterion&  masterThread) {
  logTraceIn( "RefinementCriterion(RefinementCriterion)" );
  // @todo Insert your code here
  logTraceOut( "RefinementCriterion(RefinementCriterion)" );
}


void multigrid::mappings::RefinementCriterion::mergeWithWorkerThread(const RefinementCriterion& workerThread) {
  logTraceIn( "mergeWithWorkerThread(RefinementCriterion)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(RefinementCriterion)" );
}
#endif


void multigrid::mappings::RefinementCriterion::createHangingVertex(
      multigrid::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      multigrid::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      multigrid::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}


void multigrid::mappings::RefinementCriterion::destroyHangingVertex(
      const multigrid::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      multigrid::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      multigrid::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void multigrid::mappings::RefinementCriterion::createBoundaryVertex(
      multigrid::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      multigrid::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      multigrid::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void multigrid::mappings::RefinementCriterion::destroyVertex(
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


void multigrid::mappings::RefinementCriterion::createCell(
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


void multigrid::mappings::RefinementCriterion::destroyCell(
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
void multigrid::mappings::RefinementCriterion::mergeWithNeighbour(
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

void multigrid::mappings::RefinementCriterion::prepareSendToNeighbour(
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

void multigrid::mappings::RefinementCriterion::prepareCopyToRemoteNode(
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

void multigrid::mappings::RefinementCriterion::prepareCopyToRemoteNode(
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

void multigrid::mappings::RefinementCriterion::mergeWithRemoteDataDueToForkOrJoin(
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

void multigrid::mappings::RefinementCriterion::mergeWithRemoteDataDueToForkOrJoin(
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

bool multigrid::mappings::RefinementCriterion::prepareSendToWorker(
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

void multigrid::mappings::RefinementCriterion::prepareSendToMaster(
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


void multigrid::mappings::RefinementCriterion::mergeWithMaster(
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


void multigrid::mappings::RefinementCriterion::receiveDataFromMaster(
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


void multigrid::mappings::RefinementCriterion::mergeWithWorker(
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


void multigrid::mappings::RefinementCriterion::mergeWithWorker(
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



void multigrid::mappings::RefinementCriterion::leaveCell(
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


void multigrid::mappings::RefinementCriterion::descend(
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


void multigrid::mappings::RefinementCriterion::ascend(
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
