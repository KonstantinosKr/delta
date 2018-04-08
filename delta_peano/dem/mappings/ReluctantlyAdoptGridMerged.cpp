#include "dem/mappings/ReluctantlyAdoptGridMerged.h"

#include <delta/contact/detection/bf.h>
#include <delta/contact/filter.h>
#include <delta/contact/detection/gjk.h>
#include <delta/contact/detection/hybrid.h>
#include <delta/contact/detection/penalty.h>
#include <delta/contact/detection/sphere.h>

#include "dem/mappings/AdoptGrid.h"
#include "dem/mappings/ReluctantlyAdoptGrid.h"
#include "dem/mappings/MoveParticles.h"

#include "delta/contact/forces/forces.h"
#include "delta/dynamics/dynamics.h"
#include "peano/utils/Loop.h"
#include <unordered_map>

#include "tarch/multicore/Jobs.h"
#include "peano/datatraversal/TaskSet.h"

peano::CommunicationSpecification   dem::mappings::ReluctantlyAdoptGridMerged::communicationSpecification() const {
  return peano::CommunicationSpecification(
      peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,
      peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,
      false
  );
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGridMerged::touchVertexFirstTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,
      dem::mappings::Collision::RunGridTraversalInParallel ?
      peano::MappingSpecification::RunConcurrentlyOnFineGrid :
      peano::MappingSpecification::Serial,
      true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGridMerged::enterCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,
      dem::mappings::Collision::RunGridTraversalInParallel ?
      peano::MappingSpecification::AvoidFineGridRaces :
      peano::MappingSpecification::Serial,
      true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGridMerged::leaveCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,
      dem::mappings::Collision::RunGridTraversalInParallel ?
      peano::MappingSpecification::AvoidFineGridRaces :
      peano::MappingSpecification::Serial,
      true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGridMerged::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,
      dem::mappings::Collision::RunGridTraversalInParallel ?
      peano::MappingSpecification::AvoidCoarseGridRaces :
      peano::MappingSpecification::Serial,
      true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGridMerged::ascendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::ReluctantlyAdoptGridMerged::descendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                dem::mappings::ReluctantlyAdoptGridMerged::_log( "dem::mappings::ReluctantlyAdoptGridMerged" ); 
tarch::multicore::BooleanSemaphore dem::mappings::ReluctantlyAdoptGridMerged::_ReluctantSemaphoreMerged;
dem::State                         dem::mappings::ReluctantlyAdoptGridMerged::_backgroundTaskState;

dem::mappings::ReluctantlyAdoptGridMerged::ReluctantlyAdoptGridMerged() {
  logTraceIn( "ReluctantlyAdoptGridMerged()" );
  logTraceOut( "ReluctantlyAdoptGridMerged()" );
}

dem::mappings::ReluctantlyAdoptGridMerged::~ReluctantlyAdoptGridMerged() {
  logTraceIn( "~ReluctantlyAdoptGridMerged()" );
  logTraceOut( "~ReluctantlyAdoptGridMerged()" );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::ReluctantlyAdoptGridMerged::ReluctantlyAdoptGridMerged(const ReluctantlyAdoptGridMerged&  masterThread) {
  logTraceIn( "ReluctantlyAdoptGridMerged(ReluctantlyAdoptGridMerged)" );

  _state.clearAccumulatedData();

  logTraceOut( "ReluctantlyAdoptGridMerged(ReluctantlyAdoptGridMerged)" );
}

void dem::mappings::ReluctantlyAdoptGridMerged::mergeWithWorkerThread(const ReluctantlyAdoptGridMerged& workerThread) {
  logTraceIn( "mergeWithWorkerThread(ReluctantlyAdoptGridMerged)" );

  _state.merge(workerThread._state);

  logTraceOut( "mergeWithWorkerThread(ReluctantlyAdoptGridMerged)" );
}
#endif

void dem::mappings::ReluctantlyAdoptGridMerged::beginIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  _state = solverState;
  _backgroundTaskState = solverState;
  _state.clearAccumulatedData();
  _backgroundTaskState.clearAccumulatedData();

  assertion( dem::mappings::Collision::_collisionsOfNextTraversal.empty() );

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)
  delta::contact::detection::cleanPenaltyStatistics();

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::HybridStat)
  delta::contact::detection::cleanHybridStatistics();

  while (tarch::multicore::jobs::getNumberOfWaitingBackgroundJobs()>0) {
    tarch::multicore::jobs::processBackgroundJobs();
  }

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::ReluctantlyAdoptGridMerged::endIteration(
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
    std::vector<int> penaltyStatistics = delta::contact::detection::getPenaltyStatistics();
    for (int i=0; i<static_cast<int>(penaltyStatistics.size()); i++)
    {
      logInfo( "endIteration(State)", i << " Newton iterations: " << penaltyStatistics[i] );
    }
  }

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::HybridStat)
  {
    logInfo( "endIteration(State)", std::endl
                                 << "Penalty Fails: " << delta::contact::detection::getPenaltyFails() << " PenaltyFail avg: " << (iREAL)delta::contact::detection::getPenaltyFails()/(iREAL)delta::contact::detection::getBatchSize() << std::endl
                                 << "Batch Size: " << delta::contact::detection::getBatchSize() << std::endl
                                 << "Batch Fails: " << delta::contact::detection::getBatchFails() << " BatchFail avg: " << (iREAL)delta::contact::detection::getBatchFails()/(iREAL)delta::contact::detection::getBatchSize() << std::endl
                                 << "BatchError avg: " << (iREAL)delta::contact::detection::getBatchError()/(iREAL)delta::contact::detection::getBatchSize());
  }

  logTraceOutWith1Argument( "endIteration(State)", solverState);
}

void dem::mappings::ReluctantlyAdoptGridMerged::touchVertexFirstTime(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );


  // @Refine only if there are more than one particles
  //     and at least one of the particle pairs approach each other.
  //     If all particles move away from each other, there's not need
  //     to refine.

  if(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() > 0.0)
  {
    fineGridVertex.setNumberOfParticlesInUnrefinedVertex(fineGridVertex.getNumberOfParticlesInUnrefinedVertex() * dem::mappings::ReluctantlyAdoptGrid::_coarsenCoefficientReluctant);
  }

  iREAL timeStepSize = _state.getTimeStepSize();

  for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    records::Particle& currentParticle = fineGridVertex.getParticle(i);

    //if value doesn't exist in map - no collision - skip particle
    if(dem::mappings::Collision::_activeCollisions.count(currentParticle.getGlobalParticleId())==0) {continue;}

    //iREAL force[3]  = {0.0,gravity*currentParticle._persistentRecords.getMass()*(-10),0.0};
    iREAL force[3]  = {0.0,0.0,0.0};
    iREAL torque[3] = {0.0,0.0,0.0};

    //collisions with partner particles
    for(std::vector<dem::mappings::Collision::Collisions>::iterator p = dem::mappings::Collision::_activeCollisions[currentParticle.getGlobalParticleId()].begin();
                                          p != dem::mappings::Collision::_activeCollisions[currentParticle.getGlobalParticleId()].end();
                                          p++)
    {
      iREAL rforce[3]  = {0.0,0.0,0.0};
      iREAL rtorque[3] = {0.0,0.0,0.0};

      delta::contact::forces::getContactsForces(p->_contactPoints,
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


  if (fineGridVertex.isBoundary())
  {
    dem::mappings::MoveParticles::reflectParticles(fineGridVertex);
  }

  dem::mappings::MoveParticles::moveAllParticlesAssociatedToVertex(fineGridVertex, _state);


  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGridMerged::enterCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  dem::mappings::Collision::all_to_all(fineGridVertices, fineGridVerticesEnumerator, _state, _backgroundTaskState);

  dem::mappings::MoveParticles::reassignParticles(fineGridVertices, fineGridVerticesEnumerator, _state);

  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}

void dem::mappings::ReluctantlyAdoptGridMerged::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "leaveCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  if(_state.getTwoParticlesAreClose() <= 0.0 && _backgroundTaskState.getTwoParticlesAreClose() <= 0.0) return;

  iREAL minDiameter  = std::numeric_limits<iREAL>::max();
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
      fineGridVertices[ fineGridVerticesEnumerator(k) ].setNumberOfParticlesInUnrefinedVertex(fineGridVertices[ fineGridVerticesEnumerator(k) ].getNumberOfParticles() * dem::mappings::ReluctantlyAdoptGrid::_refinementCoefficientReluctant);

      if(!fineGridVertices[ fineGridVerticesEnumerator(k) ].isHangingNode())
      {
        if(minDiameter < fineGridVerticesEnumerator.getCellSize()(0)/3.0 && fineGridVertices[ fineGridVerticesEnumerator(k) ].getRefinementControl()==Vertex::Records::Unrefined)
        {
          fineGridVertices[ fineGridVerticesEnumerator(k) ].refine();
        }
      }
    enddforx
  }
  logTraceOutWith1Argument( "leaveCell(...)", fineGridCell );
}

void dem::mappings::ReluctantlyAdoptGridMerged::touchVertexLastTime(
      dem::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  dropParticles(fineGridVertex, coarseGridVertices, coarseGridVerticesEnumerator, fineGridPositionOfVertex, fineGridH(0));

  propagageCoarseningFlagToCoarseGrid(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex);

  fineGridVertex.eraseIfParticleDistributionPermits(true, fineGridVertex.getNumberOfParticles());

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
       [=] ()->bool {
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

void dem::mappings::ReluctantlyAdoptGridMerged::createHangingVertex(
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

void dem::mappings::ReluctantlyAdoptGridMerged::destroyHangingVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  tarch::multicore::Lock lock(_ReluctantSemaphoreMerged);
  liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  lock.free();

  fineGridVertex.destroy();

  logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGridMerged::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGridMerged::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGridMerged::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  tarch::multicore::Lock lock(_ReluctantSemaphoreMerged);
  liftAllParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator);
  lock.free();

  assertion( fineGridVertex.getNumberOfParticles()==0 );
  fineGridVertex.destroy();

  logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

void dem::mappings::ReluctantlyAdoptGridMerged::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::ReluctantlyAdoptGridMerged::destroyCell(
      const dem::Cell&           fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void dem::mappings::ReluctantlyAdoptGridMerged::mergeWithNeighbour(
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

void dem::mappings::ReluctantlyAdoptGridMerged::prepareSendToNeighbour(
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

void dem::mappings::ReluctantlyAdoptGridMerged::prepareCopyToRemoteNode(
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

void dem::mappings::ReluctantlyAdoptGridMerged::prepareCopyToRemoteNode(
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

void dem::mappings::ReluctantlyAdoptGridMerged::mergeWithRemoteDataDueToForkOrJoin(
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

void dem::mappings::ReluctantlyAdoptGridMerged::mergeWithRemoteDataDueToForkOrJoin(
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

bool dem::mappings::ReluctantlyAdoptGridMerged::prepareSendToWorker(
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

void dem::mappings::ReluctantlyAdoptGridMerged::prepareSendToMaster(
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

void dem::mappings::ReluctantlyAdoptGridMerged::mergeWithMaster(
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

void dem::mappings::ReluctantlyAdoptGridMerged::receiveDataFromMaster(
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

void dem::mappings::ReluctantlyAdoptGridMerged::mergeWithWorker(
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

void dem::mappings::ReluctantlyAdoptGridMerged::mergeWithWorker(
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

void dem::mappings::ReluctantlyAdoptGridMerged::descend(
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

void dem::mappings::ReluctantlyAdoptGridMerged::ascend(
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
