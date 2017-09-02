#include "delta/collision/hybrid.h"
#include "delta/collision/sphere.h"
#include "delta/collision/bf.h"
#include "delta/collision/penalty.h"
#include "delta/collision/gjk.h"
#include "delta/collision/filter.h"
#include "delta/forces/forces.h"
#include "delta/dynamics/dynamics.h"

#include "dem/mappings/Collision.h"
#include "dem/mappings/MoveParticles.h"

#include "peano/utils/Loop.h"


peano::CommunicationSpecification   dem::mappings::Collision::communicationSpecification() const {
	return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexFirstTimeSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces,true);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

peano::MappingSpecification   dem::mappings::Collision::enterCellSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

peano::MappingSpecification   dem::mappings::Collision::leaveCellSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}

peano::MappingSpecification   dem::mappings::Collision::ascendSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::Collision::descendSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                                                 dem::mappings::Collision::_log( "dem::mappings::Collision" );
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_activeCollisions;
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_collisionsOfNextTraversal;
dem::mappings::Collision::CollisionModel                            dem::mappings::Collision::_collisionModel;
bool																                                dem::mappings::Collision::_enableOverlapCheck;
tarch::multicore::BooleanSemaphore                                  dem::mappings::Collision::_mySemaphore;

void dem::mappings::Collision::beginIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "beginIteration(State)", solverState );

	_state = solverState;
	_state.clearAccumulatedData();

	assertion( _collisionsOfNextTraversal.empty() );

	if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)
	delta::collision::cleanPenaltyStatistics();

  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::HybridStat)
  delta::collision::cleanHybridStatistics();

	logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::Collision::endIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "endIteration(State)", solverState );

	solverState.merge(_state);

	_activeCollisions.clear();

	assertion( _activeCollisions.empty() );
	assertion( _state.getNumberOfContactPoints()==0 || !_collisionsOfNextTraversal.empty() );

	_activeCollisions.insert(_collisionsOfNextTraversal.begin(), _collisionsOfNextTraversal.end());

	assertion( _state.getNumberOfContactPoints()==0 || !_activeCollisions.empty() );
	_collisionsOfNextTraversal.clear();

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

void dem::mappings::Collision::addCollision(
		std::vector<delta::collision::contactpoint> newContactPoints,
		const records::Particle&                    particleA,
		const records::Particle&                    particleB,
		bool sphere
) {
	assertion( !newContactPoints.empty() );

	//////////////START initial insertion of collision vectors into _collisionsOfNextTraversal<id, collision> map for next move update of particle A and B
	if( _collisionsOfNextTraversal.count(particleA.getGlobalParticleId())==0 ) {
		_collisionsOfNextTraversal.insert(std::pair<int,std::vector<Collisions>>(particleA.getGlobalParticleId(), std::vector<Collisions>()));
	}

	if( _collisionsOfNextTraversal.count(particleB.getGlobalParticleId())==0 ) {
		_collisionsOfNextTraversal.insert(std::pair<int,std::vector<Collisions>>(particleB.getGlobalParticleId(), std::vector<Collisions>()));
	}
	///////////////END

	//START DATASET pointer
	Collisions* dataSetA = nullptr;
	Collisions* dataSetB = nullptr;
	//////////////END

	/////////////////START if already exist | find and assign reference collision list to dataA or dataB particle
	for (std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleA.getGlobalParticleId()].begin();
		 p!=_collisionsOfNextTraversal[particleA.getGlobalParticleId()].end();  p++)
	{
		if(p->_copyOfPartnerParticle.getGlobalParticleId()==particleB.getGlobalParticleId())
		{
			dataSetA = &(*p);
		}
	}

	for(std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleB.getGlobalParticleId()].begin();
		 p!=_collisionsOfNextTraversal[particleB.getGlobalParticleId()].end(); p++)
	{
		if(p->_copyOfPartnerParticle.getGlobalParticleId()==particleA.getGlobalParticleId())
		{
			dataSetB = &(*p);
		}
	}
	//////////////END

	if((dataSetA!=nullptr && dataSetB!=nullptr)) return;

	///ASSERT we have data assigned to both data pointers
	assertion( (dataSetA==nullptr && dataSetB==nullptr) || (dataSetA!=nullptr && dataSetB!=nullptr) );
  //END

	//START if dataset A and B is empty
	if(dataSetA==nullptr)
	{
		//START push_back collisions object into corresponding both A and B particle index collision list
		_collisionsOfNextTraversal[particleA.getGlobalParticleId()].push_back( Collisions() );
		_collisionsOfNextTraversal[particleB.getGlobalParticleId()].push_back( Collisions() );
		//END push_back

		//START reference of vector to data A and B ready to used
		dataSetA = &(_collisionsOfNextTraversal[particleA.getGlobalParticleId()].back());
		dataSetB = &(_collisionsOfNextTraversal[particleB.getGlobalParticleId()].back());
		//END

		//START add copy of master and slave particles to sets (dual contact reference)
		dataSetA->_copyOfPartnerParticle = particleB;
		dataSetB->_copyOfPartnerParticle = particleA;
		//END
	}
	////////END

  //delta::collision::filterNewContacts(newContactPoints);
	if(sphere)
	{
    //delta::collision::filterOldContacts(dataSetA->_contactPoints, newContactPoints);
    //delta::collision::filterOldContacts(dataSetB->_contactPoints, newContactPoints);
	} else {	//filter multiple contacts for same area of mesh
    //delta::collision::filterOldContacts(dataSetA->_contactPoints, newContactPoints, std::min(particleA.getHMin(), particleB.getHMin()));
    //delta::collision::filterOldContacts(dataSetB->_contactPoints, newContactPoints, std::min(particleA.getHMin(), particleB.getHMin()));
	}

	/*
	 * Problem here was:
	 * Although all normals were pointing to opposite direction for each particle due to how we loop particles
	 * A. at each vertex or grid initial contact direction is interchangeable depending on what is A and B
	 * B. we always want to have the normal for particles to the position update direction (inner-direction)
	 * C. we always want to ensure that normal of particle point away from obstacle
	 * D. we don't care about normal of obstacle.
	 */
	dataSetA->_contactPoints.insert(dataSetA->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());

	for (std::vector<delta::collision::contactpoint>::iterator p = newContactPoints.begin(); p != newContactPoints.end(); p++)
	{
		//invert normal for particle B
		p->normal[0] = -1.0 * p->normal[0];
		p->normal[1] = -1.0 * p->normal[1];
		p->normal[2] = -1.0 * p->normal[2];
	}

	dataSetB->_contactPoints.insert(dataSetB->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());

	_state.incNumberOfContactPoints(newContactPoints.size());
}

double                dem::mappings::Collision::gravity = 0.0;

void dem::mappings::Collision::touchVertexFirstTime(
		dem::Vertex&                                 fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
		dem::Vertex * const                          coarseGridVertices,
		const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
		dem::Cell&                                   coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

	double timeStepSize = _state.getTimeStepSize();

	for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++) //No need to loop over virtual particles here
	{
		records::Particle& currentParticle = fineGridVertex.getParticle(i);

		//if value doesn't exist in map - no collision - skip particle
		//if(_activeCollisions.count(currentParticle.getGlobalParticleId())==0) {continue;}

    double force[3]  = {0.0,gravity*currentParticle._persistentRecords.getMass()*(-10),0.0};
    double torque[3] = {0.0,0.0,0.0};

		//collisions with partner particles
		for(std::vector<Collisions>::iterator p = _activeCollisions[currentParticle.getGlobalParticleId()].begin(); p != _activeCollisions[currentParticle.getGlobalParticleId()].end(); p++)
		{
		  //break;
			double rforce[3]  = {0.0,0.0,0.0};
			double rtorque[3] = {0.0,0.0,0.0};

			//for each partner contact get force
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
                                      currentParticle.getMass(),
                                      torque, timeStepSize);
		}
	}


	fineGridVertex.clearInheritedCoarseGridParticles();// clear adaptivity/multilevel data

	dfor2(k)
		fineGridVertex.inheritCoarseGridParticles(coarseGridVertices[coarseGridVerticesEnumerator(k)]);
	enddforx

	#ifdef ompParticle
		#pragma omp parallel for
	#endif
	for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++) // No need to loop over virtual particles here as well
	{
		//printf("Number in the grid master:%d\n", fineGridVertex.getNumberOfParticles());
    //printf("Number in the grid slave:%d\n", fineGridVertex.getNumberOfRealAndVirtualParticles());

		for(int j=0; j<fineGridVertex.getNumberOfRealAndVirtualParticles(); j++)
		{
      if((fineGridVertex.getParticle(i).getGlobalParticleId() == fineGridVertex.getParticle(j).getGlobalParticleId()) ||
         (fineGridVertex.getParticle(i).getIsObstacle() && fineGridVertex.getParticle(j).getIsObstacle()))
        continue;

      if(_enableOverlapCheck)
        if (delta::collision::isSphereOverlayInContact(
            fineGridVertex.getParticle(i).getCentre(0),
            fineGridVertex.getParticle(i).getCentre(1),
            fineGridVertex.getParticle(i).getCentre(2),
            fineGridVertex.getParticle(i).getInfluenceRadius(),

            fineGridVertex.getParticle(j).getCentre(0),
            fineGridVertex.getParticle(j).getCentre(1),
            fineGridVertex.getParticle(j).getCentre(2),
            fineGridVertex.getParticle(j).getInfluenceRadius()))
          continue;

      std::vector<delta::collision::contactpoint> newContactPoints;

      switch (_collisionModel)
      {
        case CollisionModel::Sphere: {
          bool penetration = false;
          if(fineGridVertex.getParticle(i).getIsObstacle() &&
            !fineGridVertex.getParticle(j).getIsObstacle()) {
            newContactPoints = delta::collision::sphereWithBarrierBA(
                fineGridVertex.getParticle(j).getCentre(0),
                fineGridVertex.getParticle(j).getCentre(1),
                fineGridVertex.getParticle(j).getCentre(2),
                fineGridVertex.getParticle(j).getDiameter(),
                fineGridVertex.getParticle(j).getEpsilon(),
                fineGridVertex.getParticle(j).getFriction(),
                fineGridVertex.getParticle(j).getGlobalParticleId(),

                fineGridVertex.getXCoordinates(i),
                fineGridVertex.getYCoordinates(i),
                fineGridVertex.getZCoordinates(i),
                fineGridVertex.getNumberOfTriangles(i),
                fineGridVertex.getParticle(i).getEpsilon(),
                fineGridVertex.getParticle(i).getFriction(),
                fineGridVertex.getParticle(i).getGlobalParticleId(), penetration);

          } else if (!fineGridVertex.getParticle(i).getIsObstacle() &&
                      fineGridVertex.getParticle(j).getIsObstacle()) {
            newContactPoints = delta::collision::sphereWithBarrierAB(
                fineGridVertex.getParticle(i).getCentre(0),
                fineGridVertex.getParticle(i).getCentre(1),
                fineGridVertex.getParticle(i).getCentre(2),
                fineGridVertex.getParticle(i).getDiameter(),
                fineGridVertex.getParticle(i).getEpsilon(),
                fineGridVertex.getParticle(i).getFriction(),
                fineGridVertex.getParticle(i).getGlobalParticleId(),

                fineGridVertex.getXCoordinates(j),
                fineGridVertex.getYCoordinates(j),
                fineGridVertex.getZCoordinates(j),
                fineGridVertex.getNumberOfTriangles(j),
                fineGridVertex.getParticle(j).getEpsilon(),
                fineGridVertex.getParticle(j).getFriction(),
                fineGridVertex.getParticle(j).getGlobalParticleId(), penetration);
          } else {
            newContactPoints = delta::collision::sphere(
                fineGridVertex.getParticle(i).getCentre(0),
                fineGridVertex.getParticle(i).getCentre(1),
                fineGridVertex.getParticle(i).getCentre(2),
                fineGridVertex.getParticle(i).getDiameter(),
                fineGridVertex.getParticle(i).getEpsilon(),
                fineGridVertex.getParticle(i).getFriction(),
                fineGridVertex.getParticle(i).getGlobalParticleId(),

                fineGridVertex.getParticle(j).getCentre(0),
                fineGridVertex.getParticle(j).getCentre(1),
                fineGridVertex.getParticle(j).getCentre(2),
                fineGridVertex.getParticle(j).getDiameter(),
                fineGridVertex.getParticle(j).getEpsilon(),
                fineGridVertex.getParticle(j).getFriction(),
                fineGridVertex.getParticle(j).getGlobalParticleId(), penetration);
          }
          break;
        }
        case CollisionModel::BruteForce: {
          newContactPoints = delta::collision::bf(
              fineGridVertex.getNumberOfTriangles(i),
              fineGridVertex.getXCoordinates(i),
              fineGridVertex.getYCoordinates(i),
              fineGridVertex.getZCoordinates(i),
              fineGridVertex.getParticle(i).getEpsilon(),
              fineGridVertex.getParticle(i).getFriction(),
              fineGridVertex.getParticle(i).getGlobalParticleId(),

              fineGridVertex.getNumberOfTriangles(j),
              fineGridVertex.getXCoordinates(j),
              fineGridVertex.getYCoordinates(j),
              fineGridVertex.getZCoordinates(j),
              fineGridVertex.getParticle(j).getEpsilon(),
              fineGridVertex.getParticle(j).getFriction(),
              fineGridVertex.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::Penalty: {
          newContactPoints = delta::collision::penalty(
              fineGridVertex.getNumberOfTriangles(i),
              fineGridVertex.getXCoordinates(i),
              fineGridVertex.getYCoordinates(i),
              fineGridVertex.getZCoordinates(i),
              fineGridVertex.getParticle(i).getEpsilon(),
              fineGridVertex.getParticle(i).getFriction(),
              fineGridVertex.getParticle(i).getGlobalParticleId(),

              fineGridVertex.getNumberOfTriangles(j),
              fineGridVertex.getXCoordinates(j),
              fineGridVertex.getYCoordinates(j),
              fineGridVertex.getZCoordinates(j),
              fineGridVertex.getParticle(j).getEpsilon(),
              fineGridVertex.getParticle(j).getFriction(),
              fineGridVertex.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::PenaltyStat: {
          newContactPoints = delta::collision::penaltyStat(
              fineGridVertex.getNumberOfTriangles(i),
              fineGridVertex.getXCoordinates(i),
              fineGridVertex.getYCoordinates(i),
              fineGridVertex.getZCoordinates(i),
              fineGridVertex.getParticle(i).getEpsilon(),
              fineGridVertex.getParticle(i).getFriction(),
              fineGridVertex.getParticle(i).getGlobalParticleId(),

              fineGridVertex.getNumberOfTriangles(j),
              fineGridVertex.getXCoordinates(j),
              fineGridVertex.getYCoordinates(j),
              fineGridVertex.getZCoordinates(j),
              fineGridVertex.getParticle(j).getEpsilon(),
              fineGridVertex.getParticle(j).getFriction(),
              fineGridVertex.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::HybridOnBatches: {
          newContactPoints = delta::collision::hybridWithPerBatchFallBack(
              fineGridVertex.getNumberOfTriangles(i),
              fineGridVertex.getXCoordinates(i),
              fineGridVertex.getYCoordinates(i),
              fineGridVertex.getZCoordinates(i),
              fineGridVertex.getParticle(i).getEpsilon(),
              fineGridVertex.getParticle(i).getFriction(),
              fineGridVertex.getParticle(i).getGlobalParticleId(),

              fineGridVertex.getNumberOfTriangles(j),
              fineGridVertex.getXCoordinates(j),
              fineGridVertex.getYCoordinates(j),
              fineGridVertex.getZCoordinates(j),
              fineGridVertex.getParticle(j).getEpsilon(),
              fineGridVertex.getParticle(j).getFriction(),
              fineGridVertex.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::HybridOnTrianglePairs: {
          newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
              fineGridVertex.getNumberOfTriangles(i),
              fineGridVertex.getXCoordinates(i),
              fineGridVertex.getYCoordinates(i),
              fineGridVertex.getZCoordinates(i),
              fineGridVertex.getParticle(i).getEpsilon(),
              fineGridVertex.getParticle(i).getFriction(),
              fineGridVertex.getParticle(i).getGlobalParticleId(),

              fineGridVertex.getNumberOfTriangles(j),
              fineGridVertex.getXCoordinates(j),
              fineGridVertex.getYCoordinates(j),
              fineGridVertex.getZCoordinates(j),
              fineGridVertex.getParticle(j).getEpsilon(),
              fineGridVertex.getParticle(j).getFriction(),
              fineGridVertex.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::HybridStat: {
          newContactPoints = delta::collision::hybridTriangleStat(
              fineGridVertex.getNumberOfTriangles(i),
              fineGridVertex.getXCoordinates(i),
              fineGridVertex.getYCoordinates(i),
              fineGridVertex.getZCoordinates(i),
              fineGridVertex.getParticle(i).getEpsilon(),
              fineGridVertex.getParticle(i).getFriction(),
              fineGridVertex.getParticle(i).getGlobalParticleId(),

              fineGridVertex.getNumberOfTriangles(j),
              fineGridVertex.getXCoordinates(j),
              fineGridVertex.getYCoordinates(j),
              fineGridVertex.getZCoordinates(j),
              fineGridVertex.getParticle(j).getEpsilon(),
              fineGridVertex.getParticle(j).getFriction(),
              fineGridVertex.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::GJK: {
          newContactPoints = delta::collision::gjk(
              fineGridVertex.getNumberOfTriangles(i),
              fineGridVertex.getXCoordinates(i),
              fineGridVertex.getYCoordinates(i),
              fineGridVertex.getZCoordinates(i),
              fineGridVertex.getNumberOfTriangles(j),
              fineGridVertex.getParticle(i).getEpsilon(),
              fineGridVertex.getParticle(i).getFriction(),
              fineGridVertex.getParticle(i).getGlobalParticleId(),

              fineGridVertex.getXCoordinates(j),
              fineGridVertex.getYCoordinates(j),
              fineGridVertex.getZCoordinates(j),
              fineGridVertex.getParticle(j).getEpsilon(),
              fineGridVertex.getParticle(j).getFriction(),
              fineGridVertex.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::none: {
          break;
        }
      }

			if(!newContactPoints.empty())
			{
        #ifdef ompParticle
          #pragma omp critical
        #endif
			  {//TODO deadlock two locks
			    tarch::multicore::Lock lock(_mySemaphore);
			    addCollision(newContactPoints, fineGridVertex.getParticle(i), fineGridVertex.getParticle(j) , dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere);
			    lock.free();
			  }
			}
			#ifdef ompParticle
				#pragma omp critical
			#endif
			{
			  _state.incNumberOfTriangleComparisons(fineGridVertex.getNumberOfTriangles(i) * fineGridVertex.getNumberOfTriangles(j));
			}
		}
	}

	_state.incNumberOfParticleComparisons(fineGridVertex.getNumberOfParticles() * fineGridVertex.getNumberOfRealAndVirtualParticles());
	logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

void dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
		dem::Vertex&  vertexA,
		dem::Vertex&  vertexB
) {
	logDebug( "collideParticlesOfTwoDifferentVertices(...)", vertexA.toString() << ", " << vertexA.getNumberOfRealAndVirtualParticles() );
	logDebug( "collideParticlesOfTwoDifferentVertices(...)", vertexB.toString() << ", " << vertexB.getNumberOfRealAndVirtualParticles() );

	#ifdef ompParticle
		#pragma omp parallel for
	#endif
	for(int i=0; i<vertexA.getNumberOfRealAndVirtualParticles(); i++)
	{
		for(int j=0; j<vertexB.getNumberOfRealAndVirtualParticles(); j++)
		{
			if((vertexA.getParticle(i).getIsObstacle() && vertexB.getParticle(j).getIsObstacle()) ||
			   (vertexA.getParticle(i).getGlobalParticleId() == vertexB.getParticle(j).getGlobalParticleId()))
			  continue;

			if(_enableOverlapCheck)
				if(!delta::collision::isSphereOverlayInContact(
						vertexA.getParticle(i).getCentre(0),
						vertexA.getParticle(i).getCentre(1),
						vertexA.getParticle(i).getCentre(2),
						vertexA.getParticle(i).getInfluenceRadius(),

						vertexB.getParticle(j).getCentre(0),
						vertexB.getParticle(j).getCentre(1),
						vertexB.getParticle(j).getCentre(2),
						vertexB.getParticle(j).getInfluenceRadius()))
				  continue;

			std::vector<delta::collision::contactpoint> newContactPoints;
			switch (_collisionModel)
			{
        case CollisionModel::Sphere: {
          bool penetration = false;
          if(vertexA.getParticle(i).getIsObstacle()
              && !vertexB.getParticle(j).getIsObstacle()) {
            //printf("ENTERED neighbor grid sphere BA\n");
            newContactPoints = delta::collision::sphereWithBarrierBA(
                vertexB.getParticle(j).getCentre(0),
                vertexB.getParticle(j).getCentre(1),
                vertexB.getParticle(j).getCentre(2),
                vertexB.getParticle(j).getDiameter(),
                vertexB.getParticle(j).getEpsilon(),
                vertexB.getParticle(j).getFriction(),
                vertexB.getParticle(j).getGlobalParticleId(),

                vertexA.getXCoordinates(i), vertexA.getYCoordinates(i),
                vertexA.getZCoordinates(i), vertexA.getNumberOfTriangles(i),
                vertexA.getParticle(i).getEpsilon(),
                vertexA.getParticle(i).getFriction(),
                vertexA.getParticle(i).getGlobalParticleId(), penetration);
          } else if(!vertexA.getParticle(i).getIsObstacle()
              && vertexB.getParticle(j).getIsObstacle()) {
            //printf("ENTERED neighbor grid sphere AB\n");
            newContactPoints = delta::collision::sphereWithBarrierAB(
                vertexA.getParticle(i).getCentre(0),
                vertexA.getParticle(i).getCentre(1),
                vertexA.getParticle(i).getCentre(2),
                vertexA.getParticle(i).getDiameter(),
                vertexA.getParticle(i).getEpsilon(),
                vertexA.getParticle(i).getFriction(),
                vertexA.getParticle(i).getGlobalParticleId(),

                vertexB.getXCoordinates(j), vertexB.getYCoordinates(j),
                vertexB.getZCoordinates(j), vertexB.getNumberOfTriangles(j),
                vertexB.getParticle(j).getEpsilon(),
                vertexB.getParticle(j).getFriction(),
                vertexB.getParticle(j).getGlobalParticleId(), penetration);
          } else {
            newContactPoints = delta::collision::sphere(
                vertexA.getParticle(i).getCentre(0),
                vertexA.getParticle(i).getCentre(1),
                vertexA.getParticle(i).getCentre(2),
                vertexA.getParticle(i).getDiameter(),
                vertexA.getParticle(i).getEpsilon(),
                vertexA.getParticle(i).getFriction(),
                vertexA.getParticle(i).getGlobalParticleId(),

                vertexB.getParticle(j).getCentre(0),
                vertexB.getParticle(j).getCentre(1),
                vertexB.getParticle(j).getCentre(2),
                vertexB.getParticle(j).getDiameter(),
                vertexB.getParticle(j).getEpsilon(),
                vertexB.getParticle(j).getFriction(),
                vertexB.getParticle(j).getGlobalParticleId(), penetration);
          }
          break;
        }
        case CollisionModel::BruteForce: {
          newContactPoints = delta::collision::bf(vertexA.getNumberOfTriangles(i),
              vertexA.getXCoordinates(i), vertexA.getYCoordinates(i),
              vertexA.getZCoordinates(i), vertexA.getParticle(i).getEpsilon(),
              vertexA.getParticle(i).getFriction(),
              vertexA.getParticle(i).getGlobalParticleId(),

              vertexB.getNumberOfTriangles(j), vertexB.getXCoordinates(j),
              vertexB.getYCoordinates(j), vertexB.getZCoordinates(j),
              vertexB.getParticle(j).getEpsilon(),
              vertexB.getParticle(j).getFriction(),
              vertexB.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::Penalty: {
          newContactPoints = delta::collision::penalty(
              vertexA.getNumberOfTriangles(i), vertexA.getXCoordinates(i),
              vertexA.getYCoordinates(i), vertexA.getZCoordinates(i),
              vertexA.getParticle(i).getEpsilon(),
              vertexA.getParticle(i).getFriction(),
              vertexA.getParticle(i).getGlobalParticleId(),

              vertexB.getNumberOfTriangles(j), vertexB.getXCoordinates(j),
              vertexB.getYCoordinates(j), vertexB.getZCoordinates(j),
              vertexB.getParticle(j).getEpsilon(),
              vertexB.getParticle(j).getFriction(),
              vertexB.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::PenaltyStat: {
          newContactPoints = delta::collision::penaltyStat(
              vertexA.getNumberOfTriangles(i), vertexA.getXCoordinates(i),
              vertexA.getYCoordinates(i), vertexA.getZCoordinates(i),
              vertexA.getParticle(i).getEpsilon(),
              vertexA.getParticle(i).getFriction(),
              vertexA.getParticle(i).getGlobalParticleId(),

              vertexB.getNumberOfTriangles(j), vertexB.getXCoordinates(j),
              vertexB.getYCoordinates(j), vertexB.getZCoordinates(j),
              vertexB.getParticle(j).getEpsilon(),
              vertexB.getParticle(j).getFriction(),
              vertexB.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::HybridOnBatches: {
          newContactPoints = delta::collision::hybridWithPerBatchFallBack(
              vertexA.getNumberOfTriangles(i), vertexA.getXCoordinates(i),
              vertexA.getYCoordinates(i), vertexA.getZCoordinates(i),
              vertexA.getParticle(i).getEpsilon(),
              vertexA.getParticle(i).getFriction(),
              vertexA.getParticle(i).getGlobalParticleId(),

              vertexB.getNumberOfTriangles(j), vertexB.getXCoordinates(j),
              vertexB.getYCoordinates(j), vertexB.getZCoordinates(j),
              vertexB.getParticle(j).getEpsilon(),
              vertexB.getParticle(j).getFriction(),
              vertexB.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::HybridOnTrianglePairs: {
          newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
              vertexA.getNumberOfTriangles(i), vertexA.getXCoordinates(i),
              vertexA.getYCoordinates(i), vertexA.getZCoordinates(i),
              vertexA.getParticle(i).getEpsilon(),
              vertexA.getParticle(i).getFriction(),
              vertexA.getParticle(i).getGlobalParticleId(),

              vertexB.getNumberOfTriangles(j), vertexB.getXCoordinates(j),
              vertexB.getYCoordinates(j), vertexB.getZCoordinates(j),
              vertexB.getParticle(j).getEpsilon(),
              vertexB.getParticle(j).getFriction(),
              vertexB.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::HybridStat: {
          newContactPoints = delta::collision::hybridTriangleStat(
              vertexA.getNumberOfTriangles(i), vertexA.getXCoordinates(i),
              vertexA.getYCoordinates(i), vertexA.getZCoordinates(i),
              vertexA.getParticle(i).getEpsilon(),
              vertexA.getParticle(i).getFriction(),
              vertexA.getParticle(i).getGlobalParticleId(),

              vertexB.getNumberOfTriangles(j), vertexB.getXCoordinates(j),
              vertexB.getYCoordinates(j), vertexB.getZCoordinates(j),
              vertexB.getParticle(j).getEpsilon(),
              vertexB.getParticle(j).getFriction(),
              vertexB.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::GJK: {
          newContactPoints = delta::collision::gjk(
              vertexA.getNumberOfTriangles(i), vertexA.getXCoordinates(i),
              vertexA.getYCoordinates(i), vertexA.getZCoordinates(i),
              vertexA.getParticle(i).getEpsilon(),
              vertexA.getParticle(i).getFriction(),
              vertexA.getParticle(i).getGlobalParticleId(),

              vertexB.getNumberOfTriangles(j), vertexB.getXCoordinates(j),
              vertexB.getYCoordinates(j), vertexB.getZCoordinates(j),
              vertexB.getParticle(j).getEpsilon(),
              vertexB.getParticle(j).getFriction(),
              vertexB.getParticle(j).getGlobalParticleId());
          break;
        }
        case CollisionModel::none: {
          break;
        }
			}

			if(!newContactPoints.empty())
			{
        #ifdef ompParticle
          #pragma omp critical
        #endif
        {
          tarch::multicore::Lock lock(_mySemaphore);
          addCollision( newContactPoints, vertexA.getParticle(i), vertexB.getParticle(j), dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere);
          lock.free();
        }
			}
			//printf("DifferentVertexContact:%d\n", newContactPoints.size());
			//printf("VertexANoParticles:%d VertexBNoParticles:%d\n", vertexA.getNumberOfRealAndVirtualParticles(), vertexB.getNumberOfRealAndVirtualParticles());
			//printf("VertexA:%d VertexB:%d\n", vertexA.getParticle(i).getGlobalParticleNumber(), vertexB.getParticle(j).getGlobalParticleNumber());
			#ifdef ompParticle
				#pragma omp critical
			#endif
			_state.incNumberOfTriangleComparisons(vertexA.getNumberOfTriangles(i) * vertexB.getNumberOfTriangles(j));
		}
	}
	_state.incNumberOfParticleComparisons(vertexA.getNumberOfRealAndVirtualParticles() * vertexB.getNumberOfRealAndVirtualParticles());
}

void dem::mappings::Collision::enterCell(
		dem::Cell&                                fineGridCell,
		dem::Vertex * const                       fineGridVertices,
		const peano::grid::VertexEnumerator&      fineGridVerticesEnumerator,
		dem::Vertex * const                       coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&                                coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

	/*
	if(
    fineGridVertices[fineGridVerticesEnumerator(0)].getNumberOfParticles() == 0 &&
    fineGridVertices[fineGridVerticesEnumerator(1)].getNumberOfParticles() == 0 &&
    fineGridVertices[fineGridVerticesEnumerator(2)].getNumberOfParticles() == 0 &&
    fineGridVertices[fineGridVerticesEnumerator(3)].getNumberOfParticles() == 0 &&
    fineGridVertices[fineGridVerticesEnumerator(4)].getNumberOfParticles() == 0 &&
    fineGridVertices[fineGridVerticesEnumerator(5)].getNumberOfParticles() == 0 &&
    fineGridVertices[fineGridVerticesEnumerator(6)].getNumberOfParticles() == 0 &&
    fineGridVertices[fineGridVerticesEnumerator(7)].getNumberOfParticles() == 0)return;*/

    //phase A
  #ifdef ompParticle
    #pragma omp parallel
    {
      #pragma omp single
      {
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(1)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(2)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(3)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(4)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(5)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(6)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(7)]);

        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[fineGridVerticesEnumerator(2)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[fineGridVerticesEnumerator(4)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[fineGridVerticesEnumerator(6)]);

        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(2)], fineGridVertices[fineGridVerticesEnumerator(5)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(2)], fineGridVertices[fineGridVerticesEnumerator(4)]);
        #pragma omp task
          dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(3)], fineGridVertices[fineGridVerticesEnumerator(4)]);
        #pragma omp taskwait
      }
    }
  #else
    //if(fineGridVertices[fineGridVerticesEnumerator(0)].getNumberOfParticles() > 0)
    {
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(1)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(2)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(3)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(4)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(5)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(6)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(0)], fineGridVertices[fineGridVerticesEnumerator(7)]);
    }

    //if(fineGridVertices[fineGridVerticesEnumerator(1)].getNumberOfParticles() > 0)
    {
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[fineGridVerticesEnumerator(2)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[fineGridVerticesEnumerator(4)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[fineGridVerticesEnumerator(6)]);
    }

    //if(fineGridVertices[fineGridVerticesEnumerator(2)].getNumberOfParticles() > 0)
    {
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(2)], fineGridVertices[fineGridVerticesEnumerator(5)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(2)], fineGridVertices[fineGridVerticesEnumerator(4)]);
    }
    dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(3)], fineGridVertices[fineGridVerticesEnumerator(4)]);
  #endif

    //phase B
    //back face is boundary
    bool backface = ((fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()) &&
                     (fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary())) &&

                    ((!fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && !fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary()) &&
                     (!fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary() && !fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()));

    bool backfaceLRUDHalo = ((fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()) &&
                             (fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary())) &&

                           ((fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary()) || //left halo
                            (fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()) || //right halo
                            (fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()) || //up halo
                            (fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary()));  //down halo

    bool topface = ((fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()) &&
                     (fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary())) &&

                   ((!fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && !fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary()) &&
                     (!fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && !fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()));

    bool topfaceLRUDHalo = ((fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()) &&
                            (fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary())) &&

                           ((fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary()) || //left halo
                            (fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()) || //right halo
                            (fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()) || //up halo
                            (fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary()));  //down halo

    bool rightface = ((fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()) &&
                      (fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary())) &&

                     ((!fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && !fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary()) &&
                      (!fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && !fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary()));

    bool rightfaceLRUDHalo = ((fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()) &&
                              (fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary())) &&

                             ((fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary()) || //left halo
                              (fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary()) || //right halo
                              (fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary()) || //up halo
                              (fineGridVertices[fineGridVerticesEnumerator(0)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary()));  //down halo

    //if(backface || backfaceLRUDHalo)
    {
      //face X
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4)], fineGridVertices[ fineGridVerticesEnumerator(7)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(5)], fineGridVertices[ fineGridVerticesEnumerator(6)]);
      //face L
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4)], fineGridVertices[ fineGridVerticesEnumerator(5)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4)], fineGridVertices[ fineGridVerticesEnumerator(6)]);
    }

    //face top vertex is boundary
    //if(topface || topfaceLRUDHalo)
    {
      //face X
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(2)], fineGridVertices[fineGridVerticesEnumerator(7)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(3)], fineGridVertices[fineGridVerticesEnumerator(6)]);
      //face L
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(2)], fineGridVertices[fineGridVerticesEnumerator(6)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(2)], fineGridVertices[fineGridVerticesEnumerator(3)]);
    }

    //right face is boundary
    //if(rightface || rightfaceLRUDHalo)
    {
      //face X
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[ fineGridVerticesEnumerator(7)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(3)], fineGridVertices[ fineGridVerticesEnumerator(5)]);
      //face L
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[ fineGridVerticesEnumerator(3)]);
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[fineGridVerticesEnumerator(1)], fineGridVertices[ fineGridVerticesEnumerator(5)]);
    }

    bool backtopEdge = ((fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()) &&
                        (fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary()) &&
                        (fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()));

    bool backrightEdge = ((fineGridVertices[fineGridVerticesEnumerator(4)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()) &&
                          (fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary()) &&
                          (fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()));

    bool toprightEdge = ((fineGridVertices[fineGridVerticesEnumerator(2)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(3)].isBoundary()) &&
                         (fineGridVertices[fineGridVerticesEnumerator(6)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(7)].isBoundary()) &&
                         (fineGridVertices[fineGridVerticesEnumerator(1)].isBoundary() && fineGridVertices[fineGridVerticesEnumerator(5)].isBoundary()));

    //if(backtopEdge)
    {
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(6)], fineGridVertices[ fineGridVerticesEnumerator(7)]);
    }

    //if(backrightEdge)
    {
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(5)], fineGridVertices[ fineGridVerticesEnumerator(7)]);
    }

    //if(toprightEdge)
    {
      dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3)], fineGridVertices[ fineGridVerticesEnumerator(7)]);
    }

	logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}

dem::mappings::Collision::Collision() {
	logTraceIn( "Collision()" );

    _enableOverlapCheck = false;

	logTraceOut( "Collision()" );
}

dem::mappings::Collision::~Collision() {
	logTraceIn( "~Collision()" );
	logTraceOut( "~Collision()" );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::Collision::Collision(const Collision&  masterThread):
		  _state(masterThread._state) {
	_state.clearAccumulatedData();
}


void dem::mappings::Collision::mergeWithWorkerThread(const Collision& workerThread) {
	_state.merge( workerThread._state );
}
#endif

void dem::mappings::Collision::createHangingVertex(
		dem::Vertex&     fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
		dem::Vertex * const   coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&       coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::destroyHangingVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::createInnerVertex(
		dem::Vertex&               fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::createBoundaryVertex(
		dem::Vertex&               fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::destroyVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

void dem::mappings::Collision::createCell(
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

void dem::mappings::Collision::destroyCell(
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
void dem::mappings::Collision::mergeWithNeighbour(
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

void dem::mappings::Collision::prepareSendToNeighbour(
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

void dem::mappings::Collision::prepareCopyToRemoteNode(
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

void dem::mappings::Collision::prepareCopyToRemoteNode(
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

void dem::mappings::Collision::mergeWithRemoteDataDueToForkOrJoin(
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

void dem::mappings::Collision::mergeWithRemoteDataDueToForkOrJoin(
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

bool dem::mappings::Collision::prepareSendToWorker(
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

void dem::mappings::Collision::prepareSendToMaster(
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

void dem::mappings::Collision::mergeWithMaster(
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

void dem::mappings::Collision::receiveDataFromMaster(
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

void dem::mappings::Collision::mergeWithWorker(
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

void dem::mappings::Collision::mergeWithWorker(
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

void dem::mappings::Collision::touchVertexLastTime(
		dem::Vertex&         fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}

void dem::mappings::Collision::leaveCell(
		dem::Cell&           fineGridCell,
		dem::Vertex * const  fineGridVertices,
		const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
) {

}

void dem::mappings::Collision::descend(
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

void dem::mappings::Collision::ascend(
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
