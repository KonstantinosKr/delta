#include "dem/mappings/ReluctantlyAdoptGrid.h"
#include "dem/mappings/AdoptGrid.h"

#include "peano/utils/Loop.h"
#include <unordered_map>


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::ReluctantlyAdoptGrid::communicationSpecification() const {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

/**
 * @see AdoptGrid::dropParticles() for an explanation.
 */
peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::touchVertexFirstTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces,true);
}

/**
 * @see AdoptGrid::restrictCoarseningVetoToCoarseGrid() for an explanation.
 */
peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::enterCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::leaveCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::ascendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::descendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                   dem::mappings::ReluctantlyAdoptGrid::_log( "dem::mappings::ReluctantlyAdoptGrid" );
tarch::multicore::BooleanSemaphore    dem::mappings::ReluctantlyAdoptGrid::_ReluctantSemaphore;

void dem::mappings::ReluctantlyAdoptGrid::touchVertexFirstTime(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  if (fineGridVertex.getNumberOfParticles()>1)
  {
    for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
    {
      if(fineGridVertex.getParticle(i).getDiameter()<fineGridH(0)/3.0 && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined)
      {
        logDebug( "touchVertexFirstTime(...)", "refine " << fineGridVertex.toString() );
        fineGridVertex.refine();
      }
    }
  }

  fineGridVertex.clearGridRefinementAnalysisData();

  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGrid::touchVertexLastTime(
  dem::Vertex&                                 fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  dem::Cell&                                   coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  dropParticles(fineGridVertex, coarseGridVertices, coarseGridVerticesEnumerator, fineGridPositionOfVertex, fineGridH(0));

  fineGridVertex.eraseIfParticleDistributionPermits();
  restrictCoarseningVetoToCoarseGrid(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex);

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGrid::createHangingVertex(
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

void dem::mappings::ReluctantlyAdoptGrid::destroyHangingVertex(
  const dem::Vertex&                            fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  dem::Vertex * const                           coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&                                    coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  tarch::multicore::Lock lock(_ReluctantSemaphore);
  liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  lock.free();

  fineGridVertex.destroy();

  logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGrid::createInnerVertex(
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

void dem::mappings::ReluctantlyAdoptGrid::createBoundaryVertex(
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

void dem::mappings::ReluctantlyAdoptGrid::destroyVertex(
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

bool dem::mappings::ReluctantlyAdoptGrid::isApproach(
    const records::Particle& particleA,
    const records::Particle& particleB)
{
  iREAL dt = _state.getTimeStepSize();
  iREAL pdt = dt + dt * 1.1;

  iREAL cA[3], cB[3];

  cA[0] = particleA.getCentre(0) + pdt*particleA.getVelocity(0);
  cA[1] = particleA.getCentre(1) + pdt*particleA.getVelocity(1);
  cA[2] = particleA.getCentre(2) + pdt*particleA.getVelocity(2);

  cB[0] = particleB.getCentre(0) + pdt*particleB.getVelocity(0);
  cB[1] = particleB.getCentre(1) + pdt*particleB.getVelocity(1);
  cB[2] = particleB.getCentre(2) + pdt*particleB.getVelocity(2);

  iREAL dAB[3], pdAB[3];

  pdAB[0] = cB[0] - cA[0];
  pdAB[1] = cB[1] - cA[1];
  pdAB[2] = cB[2] - cA[2];

  dAB[0] = particleB.getCentre(0) - particleA.getCentre(0);
  dAB[1] = particleB.getCentre(1) - particleA.getCentre(1);
  dAB[2] = particleB.getCentre(2) - particleA.getCentre(2);

  iREAL vA[3], vB[3];
  vA[0] = particleA.getVelocity(0);
  vA[1] = particleA.getVelocity(1);
  vA[2] = particleA.getVelocity(2);

  vB[0] = particleB.getVelocity(0);
  vB[1] = particleB.getVelocity(1);
  vB[2] = particleB.getVelocity(2);

  //velocity of B relative to A
  iREAL vBA = ((vB[0] * dAB[0]) + (vB[1] * dAB[1]) + (vB[2] * dAB[2])) -
              ((vA[0] * dAB[0]) + (vA[1] * dAB[1]) + (vA[2] * dAB[2]));

  iREAL pvBA = ((vB[0] * pdAB[0]) + (vB[1] * pdAB[1]) + (vB[2] * pdAB[2])) -
               ((vA[0] * pdAB[0]) + (vA[1] * pdAB[1]) + (vA[2] * pdAB[2]));

  //particles separate
  if (vBA > 0 && pvBA > 0) {
    return false;
  }

  return true;
}

bool dem::mappings::ReluctantlyAdoptGrid::ParticlesOfTwoDifferentVertices(
    dem::Vertex&  vertexA,
    dem::Vertex&  vertexB)
{
  bool approach = false;
  for(int i=0; i<vertexA.getNumberOfParticles(); i++)
  {
    for(int j=0; j<vertexB.getNumberOfParticles(); j++)
    {
      if((vertexA.getParticle(i).getIsObstacle() && vertexB.getParticle(j).getIsObstacle()) ||
         (vertexA.getParticle(i).getGlobalParticleId() == vertexB.getParticle(j).getGlobalParticleId()))
        continue;

      approach = isApproach(vertexA.getParticle(i), vertexB.getParticle(j));
    }
  }
  return approach;
}

void dem::mappings::ReluctantlyAdoptGrid::enterCell(
  dem::Cell&                                 fineGridCell,
  dem::Vertex * const                        fineGridVertices,
  const peano::grid::VertexEnumerator&       fineGridVerticesEnumerator,
  dem::Vertex * const                        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  dem::Cell&                                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  Vertex &v0 = fineGridVertices[fineGridVerticesEnumerator(0)];
  Vertex &v1 = fineGridVertices[fineGridVerticesEnumerator(1)];
  Vertex &v2 = fineGridVertices[fineGridVerticesEnumerator(2)];
  Vertex &v3 = fineGridVertices[fineGridVerticesEnumerator(3)];
  Vertex &v4 = fineGridVertices[fineGridVerticesEnumerator(4)];
  Vertex &v5 = fineGridVertices[fineGridVerticesEnumerator(5)];
  Vertex &v6 = fineGridVertices[fineGridVerticesEnumerator(6)];
  Vertex &v7 = fineGridVertices[fineGridVerticesEnumerator(7)];

  if(
    v0.getNumberOfParticles() == 0 &&
    v1.getNumberOfParticles() == 0 &&
    v2.getNumberOfParticles() == 0 &&
    v3.getNumberOfParticles() == 0 &&
    v4.getNumberOfParticles() == 0 &&
    v5.getNumberOfParticles() == 0 &&
    v6.getNumberOfParticles() == 0 &&
    v7.getNumberOfParticles() == 0)return;

  dem::Vertex vcentre = reduceVirtuals(v0, v1, v2, v3, v4, v5, v6, v7);

  bool approach = false;

  double minDiameter  = std::numeric_limits<double>::max();
  int numberOfParticles = 0;

  dfor2(k) //get min diameter particle and count number of real particles
    numberOfParticles += fineGridVertices[fineGridVerticesEnumerator(k)].getNumberOfParticles();

    for(int i=0; i<fineGridVertices[fineGridVerticesEnumerator(k)].getNumberOfParticles(); i++)
    {
      minDiameter = std::min(minDiameter, fineGridVertices[fineGridVerticesEnumerator(k)].getParticle(i).getDiameter());
    }
  enddforx

  //approach = isApproach();

  if(numberOfParticles >= 2 && minDiameter < fineGridVerticesEnumerator.getCellSize()(0)/3.0 && approach)
  {
    dfor2(k)
      if (!fineGridVertices[ fineGridVerticesEnumerator(k) ].isHangingNode() && fineGridVertices[ fineGridVerticesEnumerator(k) ].getRefinementControl()==Vertex::Records::Unrefined)
      {
        fineGridVertices[ fineGridVerticesEnumerator(k) ].refine();
      }
    enddforx
  }

  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}

//
//   NOP
// =======
//
dem::mappings::ReluctantlyAdoptGrid::ReluctantlyAdoptGrid() {
  logTraceIn( "ReluctantlyAdoptGrid()" );
  // @todo Insert your code here
  logTraceOut( "ReluctantlyAdoptGrid()" );
}

dem::mappings::ReluctantlyAdoptGrid::~ReluctantlyAdoptGrid() {
  logTraceIn( "~ReluctantlyAdoptGrid()" );
  // @todo Insert your code here
  logTraceOut( "~ReluctantlyAdoptGrid()" );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::ReluctantlyAdoptGrid::ReluctantlyAdoptGrid(const ReluctantlyAdoptGrid&  masterThread) {
  logTraceIn( "ReluctantlyAdoptGrid(ReluctantlyAdoptGrid)" );
  // @todo Insert your code here
  logTraceOut( "ReluctantlyAdoptGrid(ReluctantlyAdoptGrid)" );
}


void dem::mappings::ReluctantlyAdoptGrid::mergeWithWorkerThread(const ReluctantlyAdoptGrid& workerThread) {
  logTraceIn( "mergeWithWorkerThread(ReluctantlyAdoptGrid)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(ReluctantlyAdoptGrid)" );
}
#endif

void dem::mappings::ReluctantlyAdoptGrid::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
}

void dem::mappings::ReluctantlyAdoptGrid::destroyCell(
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
void dem::mappings::ReluctantlyAdoptGrid::mergeWithNeighbour(
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

void dem::mappings::ReluctantlyAdoptGrid::prepareSendToNeighbour(
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

void dem::mappings::ReluctantlyAdoptGrid::prepareCopyToRemoteNode(
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

void dem::mappings::ReluctantlyAdoptGrid::prepareCopyToRemoteNode(
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

void dem::mappings::ReluctantlyAdoptGrid::mergeWithRemoteDataDueToForkOrJoin(
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

void dem::mappings::ReluctantlyAdoptGrid::mergeWithRemoteDataDueToForkOrJoin(
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

bool dem::mappings::ReluctantlyAdoptGrid::prepareSendToWorker(
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

void dem::mappings::ReluctantlyAdoptGrid::prepareSendToMaster(
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


void dem::mappings::ReluctantlyAdoptGrid::mergeWithMaster(
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


void dem::mappings::ReluctantlyAdoptGrid::receiveDataFromMaster(
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


void dem::mappings::ReluctantlyAdoptGrid::mergeWithWorker(
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


void dem::mappings::ReluctantlyAdoptGrid::mergeWithWorker(
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


void dem::mappings::ReluctantlyAdoptGrid::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
}


void dem::mappings::ReluctantlyAdoptGrid::beginIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );
  // @todo Insert your code here
  _state = solverState;
  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}


void dem::mappings::ReluctantlyAdoptGrid::endIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "endIteration(State)", solverState );
  // @todo Insert your code here
  logTraceOutWith1Argument( "endIteration(State)", solverState);
}



void dem::mappings::ReluctantlyAdoptGrid::descend(
  dem::Cell * const          fineGridCells,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell
) {
  logTraceInWith2Arguments( "descend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "descend(...)" );
}


void dem::mappings::ReluctantlyAdoptGrid::ascend(
  dem::Cell * const    fineGridCells,
  dem::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell
) {
  logTraceInWith2Arguments( "ascend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "ascend(...)" );
}
