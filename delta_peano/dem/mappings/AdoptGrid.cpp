#include "dem/mappings/AdoptGrid.h"
#include "peano/utils/Loop.h"
#include "tarch/logging/Log.h"

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

peano::MappingSpecification   dem::mappings::AdoptGrid::enterCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::AdoptGrid::leaveCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}

/**
 * Restrict data to coarser grid
 */
peano::MappingSpecification   dem::mappings::AdoptGrid::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::AdoptGrid::ascendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::AdoptGrid::descendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log dem::mappings::AdoptGrid::_log( "dem::mappings::AdoptGrid" );
tarch::multicore::BooleanSemaphore  		dem::mappings::AdoptGrid::_AdoptSemaphore;
iREAL 									dem::mappings::AdoptGrid::_refinementCoefficient;
iREAL 									dem::mappings::AdoptGrid::_coarsenCoefficient;

void dem::mappings::dropParticles(
  dem::Vertex&                                 fineGridVertex,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex,
  iREAL                                       fineGridH
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

void dem::mappings::liftAllParticles(
  const dem::Vertex&                            fineGridVertex,
  dem::Vertex * const                           coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator
) {

  for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    tarch::la::Vector<DIMENSIONS,int> destinationVertex;
    for (int d=0; d<DIMENSIONS; d++)
    {
      destinationVertex(d) = fineGridVertex.getParticle(i).getCentre(d) < coarseGridVerticesEnumerator.getCellCenter()(d) ? 0 : 1;
    }
    coarseGridVertices[ coarseGridVerticesEnumerator(destinationVertex) ].appendParticle( fineGridVertex.getParticle(i) );
    printf("lifted particle:%i\n", fineGridVertex.getParticle(i).getGlobalParticleId());
  }
}

void dem::mappings::propagageCoarseningFlagToCoarseGrid(
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
      coarseGridVertices[ coarseGridVerticesEnumerator(k) ].propagageCoarseningFlagToCoarseGrid( fineGridVertex );
    }
  enddforx
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

void dem::mappings::AdoptGrid::beginIteration(
  dem::State&  solverState
) {
}

void dem::mappings::AdoptGrid::endIteration(
  dem::State&  solverState
) {
}

void dem::mappings::AdoptGrid::touchVertexFirstTime(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    fineGridVertex.setNumberOfParticlesInUnrefinedVertex(fineGridVertex.getNumberOfParticles() * dem::mappings::AdoptGrid::_refinementCoefficient);
    if(fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined)
    {
      if(fineGridVertex.getParticle(i).getDiameter() < fineGridH(0)/3.0)
      {
        //logInfo( "touchVertexFirstTime(...)", "refine " << fineGridVertex.toString() );
        fineGridVertex.refine();
      }
    }
  }

  if(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() > 0.0)
  {
    fineGridVertex.setNumberOfParticlesInUnrefinedVertex(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() * dem::mappings::AdoptGrid::_coarsenCoefficient);
  }

  /*
  for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    if(fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined)
    {
      if(fineGridVertex.getParticle(i).getDiameter() < fineGridH(0)/3.0)
      {
        fineGridVertex.refine();
        fineGridVertex.setNumberOfParticlesInUnrefinedVertex(1);
        //logInfo( "touchVertexFirstTime(...)", "refine " << fineGridVertex.toString() );
      }
    }
  }*/


/*
  if(fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined)
  {
    if(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() > 1E-6)
    {
      printf("before sub %f\n", fineGridVertex.getNumberOfParticlesInUnrefinedVertex());

      fineGridVertex.setNumberOfParticlesInUnrefinedVertex(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() * _coarsenCoefficient);

      printf("after sub %f\n", fineGridVertex.getNumberOfParticlesInUnrefinedVertex());
    } else if(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() > 1E-6 && fineGridVertex.getNumberOfParticles() > 0)
    {
      printf("before more %f\n", fineGridVertex.getNumberOfParticlesInUnrefinedVertex());
      if(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() < 2.0)
      {
        fineGridVertex.setNumberOfParticlesInUnrefinedVertex(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() * _refinementCoefficient);
      }
      printf("more %f\n", fineGridVertex.getNumberOfParticlesInUnrefinedVertex());
    }
  }
*/
  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

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

void dem::mappings::AdoptGrid::touchVertexLastTime(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  dropParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex, fineGridH(0));

  propagageCoarseningFlagToCoarseGrid(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex);

  //printf("LASTTOUCH: %f\n", fineGridVertex.getNumberOfParticlesInUnrefinedVertex());
  fineGridVertex.eraseIfParticleDistributionPermits(false, fineGridVertex.getNumberOfParticles());

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::destroyHangingVertex(
  const dem::Vertex&                            fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridH,
  dem::Vertex * const                           coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&                                    coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

<<<<<<< Updated upstream
  //tarch::multicore::Lock lock(_AdoptSemaphore);
  //liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  //lock.free();
  //fineGridVertex.destroy();
=======
  tarch::multicore::Lock lock(_AdoptSemaphore);
  liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  lock.free();
>>>>>>> Stashed changes

  logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::createInnerVertex(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::AdoptGrid::destroyVertex(
      const dem::Vertex&   								fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&         fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&         fineGridH,
      dem::Vertex * const  								coarseGridVertices,
      const peano::grid::VertexEnumerator&          		coarseGridVerticesEnumerator,
      dem::Cell&           								coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&           fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  //tarch::multicore::Lock lock(_AdoptSemaphore);
  //liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  //lock.free();

<<<<<<< Updated upstream
  //assertion( fineGridVertex.getNumberOfParticles()==0 );
  //fineGridVertex.destroy();
=======
  assertion( fineGridVertex.getNumberOfParticles()==0 );
>>>>>>> Stashed changes

  logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

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
  const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridH,
  int                                           level
) {
  logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
  // @todo Insert your code here
  logTraceOut( "mergeWithNeighbour(...)" );
}

void dem::mappings::AdoptGrid::prepareSendToNeighbour(
  dem::Vertex&  vertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
      const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
      int                                           level
) {
  logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
  // @todo Insert your code here
  logTraceOut( "prepareSendToNeighbour(...)" );
}

void dem::mappings::AdoptGrid::prepareCopyToRemoteNode(
  dem::Vertex&  localVertex,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
      const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
      int                                           level
) {
  logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
  // @todo Insert your code here
  logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::AdoptGrid::prepareCopyToRemoteNode(
  dem::Cell&  localCell,
      int                                           toRank,
      const tarch::la::Vector<DIMENSIONS,iREAL>&   cellCentre,
      const tarch::la::Vector<DIMENSIONS,iREAL>&   cellSize,
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
  const tarch::la::Vector<DIMENSIONS,iREAL>&  x,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  h,
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
  const tarch::la::Vector<DIMENSIONS,iREAL>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  cellSize,
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
  const tarch::la::Vector<DIMENSIONS,iREAL>&  cellCentre,
  const tarch::la::Vector<DIMENSIONS,iREAL>&  cellSize,
  int                                          level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}

void dem::mappings::AdoptGrid::mergeWithWorker(
  dem::Vertex&        localVertex,
  const dem::Vertex&  receivedMasterVertex,
  const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
  const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
  int                                           level
) {
  logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
  // @todo Insert your code here
  logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif

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
