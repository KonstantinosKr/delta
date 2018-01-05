#include "dem/mappings/AdoptGrid.h"
#include "peano/utils/Loop.h"
#include "tarch/logging/Log.h"

/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::AdoptGrid::communicationSpecification() const {
  return peano::CommunicationSpecification(
    peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,
    peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,
    false);
}

/**
 * Just invokes refine if necessary. It also deletes the accumulated attributes.
 */
peano::MappingSpecification   dem::mappings::AdoptGrid::touchVertexFirstTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

/**
 * Restrict data to coarser grid
 */
peano::MappingSpecification   dem::mappings::AdoptGrid::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::AdoptGrid::enterCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::AdoptGrid::leaveCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::AdoptGrid::ascendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::AdoptGrid::descendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log dem::mappings::AdoptGrid::_log( "dem::mappings::AdoptGrid" );
tarch::multicore::BooleanSemaphore  dem::mappings::AdoptGrid::_AdoptSemaphore;

void dem::mappings::dropParticles(
  dem::Vertex&                                 fineGridVertex,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex,
  double                                       fineGridH
) {
  assertion1( !fineGridVertex.isHangingNode(), fineGridVertex.toString() );

  if(peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex))
  {
    Vertex& coarseGridVertexAtSamePosition = coarseGridVertices[coarseGridVerticesEnumerator(
        peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex) )];

    if(!coarseGridVertexAtSamePosition.isOutside())
    {
      for(int i=0; i<coarseGridVertexAtSamePosition.getNumberOfParticles(); i++)
      {
        //if(!coarseGridVertexAtSamePosition.getParticle(i).getIsObstacle())
        if(fineGridH < coarseGridVertexAtSamePosition.getParticle(i).getDiameter() < coarseGridVerticesEnumerator.getCellSize()(0))
        {
          //printf("ID:%i COARSE CELL SIZE:%f FINE CELL SIZE:%f DIAMETER:%f\n", coarseGridVertexAtSamePosition.getParticle(i).getGlobalParticleId(), coarseGridVerticesEnumerator.getCellSize()(0), fineGridH, coarseGridVertexAtSamePosition.getParticle(i).getDiameter());

          fineGridVertex.appendParticle(coarseGridVertexAtSamePosition.getParticle(i));
          coarseGridVertexAtSamePosition.releaseParticle(i);

          /*std::cout << "dropParticle(): GLOBALID: "
                    << coarseGridVertexAtSamePosition.getParticle(i).getGlobalParticleId() << " LOCALID: "
                    << coarseGridVertexAtSamePosition.getParticle(i).getLocalParticleId() << " from "
                    << peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex) << " into "
                    << fineGridPositionOfVertex << std::endl;*/
        }
      }
    }
  }
}

void dem::mappings::AdoptGrid::touchVertexFirstTime(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    if(fineGridVertex.getParticle(i).getDiameter() < fineGridH(0)/3.0 && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined)
    {
      //logInfo( "touchVertexFirstTime(...)", "refine " << fineGridVertex.toString() );
      fineGridVertex.refine();
    }
  }

  fineGridVertex.clearGridRefinementAnalysisData();

  if(fineGridVertex.getRefinementControl() == Vertex::Records::Unrefined)
  {
    fineGridVertex.setVetoNumber(fineGridVertex.getNumberOfParticles());
  }

  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

void dem::mappings::restrictCoarseningVetoToCoarseGrid(
  dem::Vertex&                                 fineGridVertex,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  dfor2(k)
    bool influences = true;
    for (int d=0; d<DIMENSIONS; d++)
    {
      influences &= ( (k(d)==0 && fineGridPositionOfVertex(d)<3) || (k(d)==1 && fineGridPositionOfVertex(d)>0) );
    }
    if(influences)
    {
      coarseGridVertices[ coarseGridVerticesEnumerator(k) ].restrictParticleResponsibilityData( fineGridVertex );
    }
  enddforx
}

void dem::mappings::AdoptGrid::touchVertexLastTime(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  dropParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex, fineGridH(0));

  restrictCoarseningVetoToCoarseGrid(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex);

  fineGridVertex.eraseIfParticleDistributionPermits(false);

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}

void dem::mappings::liftAllParticles(
  const dem::Vertex&                            fineGridVertex,
  dem::Vertex * const                           coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator
) {
  int i=0;
  while (i<fineGridVertex.getNumberOfParticles())
  {
    tarch::la::Vector<DIMENSIONS,int> destinationVertex;
    for (int d=0; d<DIMENSIONS; d++)
    {
      destinationVertex(d) = fineGridVertex.getParticle(i).getCentre(d) < coarseGridVerticesEnumerator.getCellCenter()(d) ? 0 : 1;
    }
    coarseGridVertices[ coarseGridVerticesEnumerator(destinationVertex) ].appendParticle( fineGridVertex.getParticle(i) );
    //printf("lifted particle:%i\n", fineGridVertex.getParticle(i).getGlobalParticleId());

    i++;
  }
}

void dem::mappings::AdoptGrid::destroyHangingVertex(
  const dem::Vertex&                            fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  dem::Vertex * const                           coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&                                    coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  tarch::multicore::Lock lock(_AdoptSemaphore);
  liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  lock.free();
  fineGridVertex.destroy();

  logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::createInnerVertex(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  assertion( fineGridVertex.getNumberOfParticles()==0 );
  fineGridVertex.destroy();

  logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

dem::mappings::AdoptGrid::AdoptGrid() {
  logTraceIn( "AdoptGrid()" );
  // @todo Insert your code here
  logTraceOut( "AdoptGrid()" );
}

dem::mappings::AdoptGrid::~AdoptGrid() {
  logTraceIn( "~AdoptGrid()" );
  // @todo Insert your code here
  logTraceOut( "~AdoptGrid()" );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::AdoptGrid::AdoptGrid(const AdoptGrid&  masterThread) {
  logTraceIn( "AdoptGrid(AdoptGrid)" );
  // @todo Insert your code here
  logTraceOut( "AdoptGrid(AdoptGrid)" );
}


void dem::mappings::AdoptGrid::mergeWithWorkerThread(const AdoptGrid& workerThread) {
  logTraceIn( "mergeWithWorkerThread(AdoptGrid)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(AdoptGrid)" );
}
#endif


void dem::mappings::AdoptGrid::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::AdoptGrid::destroyCell(
      const dem::Cell&           fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void dem::mappings::AdoptGrid::mergeWithNeighbour(
  dem::Vertex&  vertex,
  const dem::Vertex&  neighbour,
  int                                           fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  int                                           level
) {
  logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
  // @todo Insert your code here
  logTraceOut( "mergeWithNeighbour(...)" );
}

void dem::mappings::AdoptGrid::prepareSendToNeighbour(
  dem::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
  logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
  // @todo Insert your code here
  logTraceOut( "prepareSendToNeighbour(...)" );
}

void dem::mappings::AdoptGrid::prepareCopyToRemoteNode(
  dem::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   x,
      const tarch::la::Vector<DIMENSIONS,double>&   h,
      int                                           level
) {
  logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
  // @todo Insert your code here
  logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::AdoptGrid::prepareCopyToRemoteNode(
  dem::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
      int                                           level
) {
  logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localCell, toRank, cellCentre, cellSize, level );
  // @todo Insert your code here
  logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::AdoptGrid::mergeWithRemoteDataDueToForkOrJoin(
  dem::Vertex&  localVertex,
  const dem::Vertex&  masterOrWorkerVertex,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  x,
  const tarch::la::Vector<DIMENSIONS,double>&  h,
  int                                       level
) {
  logTraceInWith6Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localVertex, masterOrWorkerVertex, fromRank, x, h, level );
  // @todo Insert your code here
  logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

void dem::mappings::AdoptGrid::mergeWithRemoteDataDueToForkOrJoin(
  dem::Cell&  localCell,
  const dem::Cell&  masterOrWorkerCell,
  int                                       fromRank,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                       level
) {
  logTraceInWith3Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localCell, masterOrWorkerCell, fromRank );
  // @todo Insert your code here
  logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

bool dem::mappings::AdoptGrid::prepareSendToWorker(
  dem::Cell&                 fineGridCell,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker
) {
  logTraceIn( "prepareSendToWorker(...)" );
  // @todo Insert your code here
  logTraceOutWith1Argument( "prepareSendToWorker(...)", true );
  return true;
}

void dem::mappings::AdoptGrid::prepareSendToMaster(
  dem::Cell&                       localCell,
  dem::Vertex *                    vertices,
  const peano::grid::VertexEnumerator&       verticesEnumerator, 
  const dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  const dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
  logTraceInWith2Arguments( "prepareSendToMaster(...)", localCell, verticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "prepareSendToMaster(...)" );
}


void dem::mappings::AdoptGrid::mergeWithMaster(
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
  logTraceIn( "mergeWithMaster(...)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithMaster(...)" );
}


void dem::mappings::AdoptGrid::receiveDataFromMaster(
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
  logTraceIn( "receiveDataFromMaster(...)" );
  // @todo Insert your code here
  logTraceOut( "receiveDataFromMaster(...)" );
}


void dem::mappings::AdoptGrid::mergeWithWorker(
  dem::Cell&           localCell, 
  const dem::Cell&     receivedMasterCell,
  const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
  int                                          level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}


void dem::mappings::AdoptGrid::mergeWithWorker(
  dem::Vertex&        localVertex,
  const dem::Vertex&  receivedMasterVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   h,
  int                                           level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif


void dem::mappings::AdoptGrid::enterCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}

void dem::mappings::AdoptGrid::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
}

void dem::mappings::AdoptGrid::beginIteration(
  dem::State&  solverState
) {
}

void dem::mappings::AdoptGrid::endIteration(
  dem::State&  solverState
) {
}

void dem::mappings::AdoptGrid::descend(
  dem::Cell * const          fineGridCells,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell
) {
}


void dem::mappings::AdoptGrid::ascend(
  dem::Cell * const    fineGridCells,
  dem::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell
) {
}
