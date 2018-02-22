#include "dem/mappings/ReluctantlyAdoptGrid.h"
#include "dem/mappings/AdoptGrid.h"

#include "delta/collision/hybrid.h"
#include "delta/collision/sphere.h"
#include "delta/collision/bf.h"
#include "delta/collision/penalty.h"
#include "delta/collision/gjk.h"
#include "delta/collision/filter.h"
#include "delta/forces/forces.h"
#include "delta/dynamics/dynamics.h"
#include "peano/utils/Loop.h"
#include <unordered_map>

#include "tarch/multicore/Jobs.h"
#include "peano/datatraversal/TaskSet.h"

peano::CommunicationSpecification   dem::mappings::ReluctantlyAdoptGrid::communicationSpecification() const {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

/**
 * @see AdoptGrid::dropParticles() for an explanation.
 */
peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::touchVertexFirstTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::enterCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces,true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::leaveCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces,true);
}

/**
 * @see AdoptGrid::restrictCoarseningVetoToCoarseGrid() for an explanation.
 */
peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::ascendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGrid::descendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                   dem::mappings::ReluctantlyAdoptGrid::_log( "dem::mappings::ReluctantlyAdoptGrid" );
tarch::multicore::BooleanSemaphore    dem::mappings::ReluctantlyAdoptGrid::_ReluctantSemaphore;

dem::State dem::mappings::ReluctantlyAdoptGrid::_backgroundTaskState;

void dem::mappings::ReluctantlyAdoptGrid::beginIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  _state = solverState;
  _backgroundTaskState = solverState;
  _state.clearAccumulatedData();
  _backgroundTaskState.clearAccumulatedData();

  assertion( dem::mappings::Collision::_collisionsOfNextTraversal.empty() );

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)
  delta::collision::cleanPenaltyStatistics();

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::HybridStat)
  delta::collision::cleanHybridStatistics();


  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::ReluctantlyAdoptGrid::endIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "endIteration(State)", solverState );

  _state.merge(_backgroundTaskState);
  solverState.merge(_state);

  dem::mappings::Collision::_activeCollisions.clear();

  assertion( dem::mappings::Collision::_activeCollisions.empty() );
  assertion( _backgroundTaskState.getNumberOfContactPoints()==0 || ! dem::mappings::Collision::_collisionsOfNextTraversal.empty() );

  dem::mappings::Collision::_activeCollisions.insert(dem::mappings::Collision::_collisionsOfNextTraversal.begin(), dem::mappings::Collision::_collisionsOfNextTraversal.end());

  assertion( _backgroundTaskState.getNumberOfContactPoints()==0 || ! dem::mappings::Collision::_activeCollisions.empty() );
  dem::mappings::Collision::_collisionsOfNextTraversal.clear();

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)
  {
    std::vector<int> penaltyStatistics = delta::collision::getPenaltyStatistics();
    for (int i=0; i<static_cast<int>(penaltyStatistics.size()); i++)
    {
      logInfo( "endIteration(State)", i << " Newton iterations: " << penaltyStatistics[i] );
    }
  }

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::HybridStat)
  {
    logInfo( "endIteration(State)", std::endl
                                 << "Penalty Fails: " << delta::collision::getPenaltyFails() << " PenaltyFail avg: " << (double)delta::collision::getPenaltyFails()/(double)delta::collision::getBatchSize() << std::endl
                                 << "Batch Size: " << delta::collision::getBatchSize() << std::endl
                                 << "Batch Fails: " << delta::collision::getBatchFails() << " BatchFail avg: " << (double)delta::collision::getBatchFails()/(double)delta::collision::getBatchSize() << std::endl
                                 << "BatchError avg: " << (double)delta::collision::getBatchError()/(double)delta::collision::getBatchSize());
  }

  logTraceOutWith1Argument( "endIteration(State)", solverState);
}

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

  // @Refine only if there are more than one particles
  //     and at least one of the particle pairs approach each other.
  //     If all particles move away from each other, there's not need
  //     to refine.

  if(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() > 0.0)
  {
    fineGridVertex.setNumberOfParticlesInUnrefinedVertex(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() * 0.9);
  } else if(fineGridVertex.getRefinementControl() == Vertex::Records::Unrefined)
  {
    fineGridVertex.setNumberOfParticlesInUnrefinedVertex(fineGridVertex.getNumberOfParticles());
  }

  if(fineGridVertex.getNumberOfParticles() < 0.0) fineGridVertex.setNumberOfParticlesInUnrefinedVertex(0.0);

  double timeStepSize = _state.getTimeStepSize();

  for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    records::Particle& currentParticle = fineGridVertex.getParticle(i);

    //if value doesn't exist in map - no collision - skip particle
    if(dem::mappings::Collision::_activeCollisions.count(currentParticle.getGlobalParticleId())==0) {continue;}

    //double force[3]  = {0.0,gravity*currentParticle._persistentRecords.getMass()*(-10),0.0};
    double force[3]  = {0.0,0.0,0.0};
    double torque[3] = {0.0,0.0,0.0};

    //collisions with partner particles
    for(std::vector<dem::mappings::Collision::Collisions>::iterator p = dem::mappings::Collision::_activeCollisions[currentParticle.getGlobalParticleId()].begin();
                                          p != dem::mappings::Collision::_activeCollisions[currentParticle.getGlobalParticleId()].end();
                                          p++)
    {
      double rforce[3]  = {0.0,0.0,0.0};
      double rtorque[3] = {0.0,0.0,0.0};

      delta::forces::getContactsForces(p->_contactPoints,
                                       &(currentParticle._persistentRecords._centreOfMass(0)),
                                       &(currentParticle._persistentRecords._referentialCentreOfMass(0)),
                                       &(currentParticle._persistentRecords._angular(0)),
                                       &(currentParticle._persistentRecords._referentialAngular(0)),
                                       &(currentParticle._persistentRecords._velocity(0)),
                                       currentParticle.getMass(),
                                       &(currentParticle._persistentRecords._inverse(0)),
                                       &(currentParticle._persistentRecords._orientation(0)),
                                       currentParticle.getMaterial(),
                                       &(p->_copyOfPartnerParticle._persistentRecords._centreOfMass(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._referentialCentreOfMass(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._angular(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._referentialAngular(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._velocity(0)),
                                       p->_copyOfPartnerParticle.getMass(),
                                       &(p->_copyOfPartnerParticle._persistentRecords._inverse(0)),
                                       &(p->_copyOfPartnerParticle._persistentRecords._orientation(0)),
                                       p->_copyOfPartnerParticle.getMaterial(),
                                       rforce, rtorque,
                                       (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere));

      force[0] += rforce[0];
      force[1] += rforce[1];
      force[2] += rforce[2];

      torque[0] += rtorque[0];
      torque[1] += rtorque[1];
      torque[2] += rtorque[2];
    }

    if(!currentParticle.getIsObstacle())
    {
      currentParticle._persistentRecords._velocity(0) += timeStepSize * (force[0] / currentParticle.getMass());
      currentParticle._persistentRecords._velocity(1) += timeStepSize * (force[1] / currentParticle.getMass());
      currentParticle._persistentRecords._velocity(2) += timeStepSize * (force[2] / currentParticle.getMass());

      delta::dynamics::updateAngular(&currentParticle._persistentRecords._referentialAngular(0),
                                      &currentParticle._persistentRecords._orientation(0),
                                      &currentParticle._persistentRecords._inertia(0),
                                      &currentParticle._persistentRecords._inverse(0),
                                      currentParticle.getMass(), //why mass is passed here, remove if not used
                                      torque, timeStepSize);
    }
  }

  fineGridVertex.clearInheritedCoarseGridParticles();// clear adaptivity/multilevel data

  dfor2(k)
    fineGridVertex.inheritCoarseGridParticles(coarseGridVertices[coarseGridVerticesEnumerator(k)], fineGridX, fineGridH(0));
  enddforx


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

  propagageCoarseningFlagToCoarseGrid(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex);

  fineGridVertex.eraseIfParticleDistributionPermits(true);

  if (fineGridVertex.getNumberOfParticles()==0) return;

  #ifdef SharedTBB
  const int grainSize = (dem::mappings::Collision::RunParticleLoopInParallel||fineGridVertex.getNumberOfParticles()==0) ? 1 : fineGridVertex.getNumberOfParticles();
  tbb::parallel_for(
   tbb::blocked_range<int>(0,fineGridVertex.getNumberOfParticles(),grainSize),
   [&](const tbb::blocked_range<int>& r) {
    for (int i=r.begin(); i!=r.end(); i++) {
  #else
  for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++) {
  #endif
    for(int j=i+1; j<fineGridVertex.getNumberOfParticles(); j++)
    {
    if((fineGridVertex.getParticle(i).getGlobalParticleId() == fineGridVertex.getParticle(j).getGlobalParticleId()) ||
       (fineGridVertex.getParticle(i).getIsObstacle() && fineGridVertex.getParticle(j).getIsObstacle()))
      continue;

    if (dem::mappings::Collision::RunParticleComparisionsInBackground) {
      auto p0 = fineGridVertex.getParticle(i);
      auto p1 = fineGridVertex.getNumberOfTriangles(i);
      auto p2 = fineGridVertex.getXCoordinates(i);
      auto p3 = fineGridVertex.getYCoordinates(i);
      auto p4 = fineGridVertex.getZCoordinates(i);
      auto p5 = fineGridVertex.getParticle(j);
      auto p6 = fineGridVertex.getNumberOfTriangles(j);
      auto p7 = fineGridVertex.getXCoordinates(j);
      auto p8 = fineGridVertex.getYCoordinates(j);
      auto p9 = fineGridVertex.getZCoordinates(j);

      peano::datatraversal::TaskSet backgroundTask(
       [=] () {
          dem::mappings::Collision::collisionDetection(
            p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,
            &_backgroundTaskState,
            true
          );
          return false;
       },
       peano::datatraversal::TaskSet::TaskType::Background
      );

      logDebug( "collideParticlesOfTwoDifferentVertices(...)", "spawn background task" );
    }
    else {
      dem::mappings::Collision::collisionDetection(
        fineGridVertex.getParticle(i),
        fineGridVertex.getNumberOfTriangles(i),
        fineGridVertex.getXCoordinates(i),
        fineGridVertex.getYCoordinates(i),
        fineGridVertex.getZCoordinates(i),
        fineGridVertex.getParticle(j),
        fineGridVertex.getNumberOfTriangles(j),
        fineGridVertex.getXCoordinates(j),
        fineGridVertex.getYCoordinates(j),
        fineGridVertex.getZCoordinates(j),
        &_state,
        false
      );
    }
   }
  #ifdef SharedTBB
  }});
  #else
  }
  #endif

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
      const tarch::la::Vector<DIMENSIONS,double>&        fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&        fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&               coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&           fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGrid::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&           coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  tarch::multicore::Lock lock(_ReluctantSemaphore);
  liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  lock.free();

  assertion( fineGridVertex.getNumberOfParticles()==0 );
  fineGridVertex.destroy();

  logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
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

  dem::mappings::Collision::all_to_all(fineGridVertices, fineGridVerticesEnumerator, _state, _backgroundTaskState);

  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}

void dem::mappings::ReluctantlyAdoptGrid::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfCell
) {
  while (tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs()>0) {
    tarch::multicore::jobs::processBackgroundJobs();
  }

  if(_state.getTwoParticlesAreClose() <= 0.0 && _backgroundTaskState.getTwoParticlesAreClose() <= 0.0) return;

  double minDiameter  = std::numeric_limits<double>::max();
  int numberOfRealParticles = 0;
  int numberOfVirtualAndRealParticles = 0;

  dfor2(k) //get min diameter particle and count number of real particles
    numberOfRealParticles += fineGridVertices[fineGridVerticesEnumerator(k)].getNumberOfParticles();
    numberOfVirtualAndRealParticles += fineGridVertices[fineGridVerticesEnumerator(k)].getNumberOfRealAndVirtualParticles();

    for(int i=0; i<fineGridVertices[fineGridVerticesEnumerator(k)].getNumberOfParticles(); i++)
    {
      minDiameter = std::min(minDiameter, fineGridVertices[fineGridVerticesEnumerator(k)].getParticle(i).getDiameter());
    }
  enddforx

  // @We should refine if more than one virtual or real particle are in the cell and if at least one particle is real
  // @We should furthermore refine if and only if the at least one real particles approaches any other particles,
  if(numberOfRealParticles > 0 && numberOfVirtualAndRealParticles > 1)
  {
    dfor2(k)
      fineGridVertices[ fineGridVerticesEnumerator(k) ].setNumberOfParticlesInUnrefinedVertex(fineGridVertices[ fineGridVerticesEnumerator(k) ].getNumberOfParticles() * 2);

      if(!fineGridVertices[ fineGridVerticesEnumerator(k) ].isHangingNode())
      {
        if(minDiameter < fineGridVerticesEnumerator.getCellSize()(0)/3.0 && fineGridVertices[ fineGridVerticesEnumerator(k) ].getRefinementControl()==Vertex::Records::Unrefined)
        {
          fineGridVertices[ fineGridVerticesEnumerator(k) ].refine();
        }
      }
    enddforx
  }
}

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
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfCell
) {
}

void dem::mappings::ReluctantlyAdoptGrid::destroyCell(
      const dem::Cell&           fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfCell
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
