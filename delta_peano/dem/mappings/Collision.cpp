#include "delta/collision/hybrid.h"
#include "delta/collision/sphere.h"
#include "delta/collision/bf.h"
#include "delta/collision/penalty.h"
#include "delta/collision/gjk.h"
#include "delta/collision/filter.h"
#include "delta/forces/forces.h"

#include "dem/mappings/Collision.h"
#include "dem/mappings/MoveParticles.h"

#include "peano/utils/Loop.h"

#include "delta/dynamics/dynamics.h"

peano::CommunicationSpecification   dem::mappings::Collision::communicationSpecification() {
	return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexFirstTimeSpecification() { 
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

peano::MappingSpecification   dem::mappings::Collision::enterCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexLastTimeSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

peano::MappingSpecification   dem::mappings::Collision::leaveCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}

peano::MappingSpecification   dem::mappings::Collision::ascendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

peano::MappingSpecification   dem::mappings::Collision::descendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                                                 dem::mappings::Collision::_log( "dem::mappings::Collision" );
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_activeCollisions;
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_collisionsOfNextTraversal;
dem::mappings::Collision::CollisionModel                            dem::mappings::Collision::_collisionModel;
bool																dem::mappings::Collision::_enableOverlapCheck;

void dem::mappings::Collision::beginIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "beginIteration(State)", solverState );

	_state = solverState;
	_state.clearAccumulatedData();

	assertion( _collisionsOfNextTraversal.empty() );

	if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)
	delta::collision::cleanPenaltyStatistics();

	logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::Collision::endIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "endIteration(State)", solverState );

	solverState.merge(_state);

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


	logTraceOutWith1Argument( "endIteration(State)", solverState);
}

void dem::mappings::Collision::addCollision(
		std::vector<delta::collision::contactpoint> newContactPoints,
		const records::Particle&                    particleA,
		const records::Particle&                    particleB,
		bool sphere
) {
	assertion( !newContactPoints.empty() );

	/*if(particleA.getGlobalParticleId() == 57 || particleB.getGlobalParticleId() == 57)
	{
		printf("COLLISION DETECTED\n");
	}*/
	//START initial insertion of collision vectors into _collisionsOfNextTraversal<id, collision> map for next move update of particle A and B
	if ( _collisionsOfNextTraversal.count(particleA._persistentRecords._globalParticleId)==0 ) {
		_collisionsOfNextTraversal.insert( std::pair<int,std::vector<Collisions>>( particleA._persistentRecords._globalParticleId, std::vector<Collisions>()));
	}

	if ( _collisionsOfNextTraversal.count(particleB._persistentRecords._globalParticleId)==0 ) {
		_collisionsOfNextTraversal.insert( std::pair<int,std::vector<Collisions>>( particleB._persistentRecords._globalParticleId, std::vector<Collisions>()));
	}
	///////////////END

	//START DATASET pointer
	Collisions* dataSetA = nullptr;
	Collisions* dataSetB = nullptr;
	//////////////END

	//START if already exist | find and assign reference collision list to dataA or dataB particle
	for (std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleId].begin();
		 p!=_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleId].end();  p++)
	{
		if (p->_copyOfPartnerParticle._persistentRecords._globalParticleId==particleB._persistentRecords._globalParticleId)
		{
			dataSetA = &(*p);
		}
	}

	for (std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleId].begin();
		 p!=_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleId].end(); p++)
	{
		if (p->_copyOfPartnerParticle._persistentRecords._globalParticleId==particleA._persistentRecords._globalParticleId)
		{
			dataSetB = &(*p);
		}
	}
	//////////////END

	///ASSERT we have data assigned to both data pointers
	assertion( (dataSetA==nullptr && dataSetB==nullptr) || (dataSetA!=nullptr && dataSetB!=nullptr) );
    //END

	//START if data A is empty
	if (dataSetA==nullptr)
	{
		//START push_back collisions object into corresponding both A and B particle index collision list
		_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleId].push_back( Collisions() );
		_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleId].push_back( Collisions() );
		//END push_back

		//START reference of vector to data A and B ready to used
		dataSetA = &(_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleId].back());
		dataSetB = &(_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleId].back());
		//END

		//START add copy of master and slave particles to sets (dual contact reference)
		dataSetA->_copyOfPartnerParticle = particleB;
		dataSetB->_copyOfPartnerParticle = particleA;
		//END
	}
	////////END

	if(sphere)
	{
		delta::collision::filterNewContacts(newContactPoints, particleA._persistentRecords._diameter/2, particleB._persistentRecords._diameter/2);

		#ifdef ompParticle
			#pragma omp critical
		#endif
		{
			delta::collision::filterOldContacts(dataSetA->_contactPoints, newContactPoints, particleA._persistentRecords._diameter/2, particleB._persistentRecords._diameter/2);
			delta::collision::filterOldContacts(dataSetB->_contactPoints, newContactPoints, particleA._persistentRecords._diameter/2, particleB._persistentRecords._diameter/2);
		}
	} else {	//filter multiple contacts for same area of mesh
		delta::collision::filterNewContacts(newContactPoints);
		#ifdef ompParticle
			#pragma omp critical
		#endif
		{
			delta::collision::filterOldContacts(dataSetA->_contactPoints, newContactPoints, std::min(particleA._persistentRecords._hMin, particleB._persistentRecords._hMin));
			delta::collision::filterOldContacts(dataSetB->_contactPoints, newContactPoints, std::min(particleA._persistentRecords._hMin, particleB._persistentRecords._hMin));
		}
	}

	/*
	 * Problem here was:
	 * Although all normals were pointing to opposite direction for each particle due to how we loop particles
	 * A. at each vertex or grid initial contact direction is interchangeable depending on what is A and B
	 * B. we always want to have the normal for particles to the position update direction (inner-direction)
	 * C. we always want to ensure that normal of particle point away from obstacle
	 * D. we don't care about normal of obstacle.
	 */
	#ifdef ompParticle
		#pragma omp critical
	#endif
	dataSetA->_contactPoints.insert( dataSetA->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end() );

	for (std::vector<delta::collision::contactpoint>::iterator p = newContactPoints.begin(); p != newContactPoints.end(); p++)
	{
		//invert normal for particle B
		p->normal[0] = -1.0 * p->normal[0];
		p->normal[1] = -1.0 * p->normal[1];
		p->normal[2] = -1.0 * p->normal[2];
	}

	#ifdef ompParticle
		#pragma omp critical
	#endif
	dataSetB->_contactPoints.insert( dataSetB->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end() );

	#ifdef ompParticle
		#pragma omp critical
	#endif
	_state.incNumberOfContactPoints(newContactPoints.size());
}

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

	for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++) //No need to loop over virtual particles here
	{
		records::Particle& currentParticle = fineGridVertex.getParticle(i);

		double force[3]  = {0.0,0.0,0.0};
		double torque[3] = {0.0,0.0,0.0};

		//if value doesn't exist in map - no collision - skip particle
		if(_activeCollisions.count(currentParticle._persistentRecords._globalParticleId)==0) {continue;}

		//collisions with other partner particles
		for (std::vector<Collisions>::iterator p = _activeCollisions[currentParticle._persistentRecords._globalParticleId].begin(); p != _activeCollisions[currentParticle._persistentRecords._globalParticleId].end(); p++)
		{
			double rforce[3]  = {0.0,0.0,0.0};
			double rtorque[3] = {0.0,0.0,0.0};

			#ifdef FORCESTATS
			logInfo( "touchVertexFirstTime(...)", std::endl << "#####FORCE-DATA#####" << std::endl
															<<"masterID=" << currentParticle.getGlobalParticleId() << ", slaveID=" << p->_copyOfPartnerParticle.getGlobalParticleId() << std::endl);
			#endif

			//for each partner get contacts
			delta::forces::getContactForces(
				p->_contactPoints,
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

			//printf("%d\n", counter);
			#ifdef FORCESTATS
			logInfo( "touchVertexFirstTime(...)", std::endl
					<< "#####TOTAL-FDATA#####" << std::endl
					<< "forceId=" << std::to_string(counter+currentParticle.getGlobalParticleId() + p->_copyOfPartnerParticle.getGlobalParticleId())
					<< ", masterParticleNo=" << currentParticle.getGlobalParticleId() << ", slaveParticleNo=" << p->_copyOfPartnerParticle.getGlobalParticleId() << std::endl
					<< "massA=" << currentParticle.getMass() << ", massB" << p->_copyOfPartnerParticle.getMass() << ", nulldata3=0" << std::endl
					<< "fX=" << rforce[0] << ", fY=" << rforce[1] << ", fZ=" << rforce[2] << std::endl
					<< "tX=" << rtorque[0] << ", tY=" << rtorque[1] << ", tZ=" << rtorque[2]);
			#endif
		}

		if(!currentParticle.getIsObstacle())
		{
			//FOR DEBUGGING ONE PARTICLE PURPOSES
			//here is the instability when force is high.
			/*if(currentParticle.getGlobalParticleId() == 56)
			{
				logInfo("touchVertexFirstTime(...)", std::endl
					<< "##FORCESUM##" << std::endl
					<< "master: " << currentParticle.getGlobalParticleId() << std::endl
					<< "force[0]: " << force[0] << " force[1]:  " << force[1] << " force[2]: " << force[2] << std::endl
					<< "linear[0]: "<< currentParticle._persistentRecords._velocity(0) << "linear[1]: " << currentParticle._persistentRecords._velocity(1) << "linear[2]: " << currentParticle._persistentRecords._velocity(0) << std::endl <<"############");
			}*/

			currentParticle._persistentRecords._velocity(0) += timeStepSize * (force[0] / currentParticle.getMass());
			currentParticle._persistentRecords._velocity(1) += timeStepSize * (force[1] / currentParticle.getMass());
			currentParticle._persistentRecords._velocity(2) += timeStepSize * (force[2] / currentParticle.getMass());

			delta::dynamics::updateAngular(&currentParticle._persistentRecords._angular(0),
											&currentParticle._persistentRecords._referentialAngular(0),
											&currentParticle._persistentRecords._orientation(0),
											&currentParticle._persistentRecords._inertia(0),
											&currentParticle._persistentRecords._inverse(0),
											currentParticle.getMass(),
											torque, timeStepSize);
		}
		_activeCollisions.erase(currentParticle._persistentRecords._globalParticleId);
	}

	fineGridVertex.clearInheritedCoarseGridParticles();// clear adaptivity/multilevel data

	dfor2(k)
		fineGridVertex.inheritCoarseGridParticles(coarseGridVertices[coarseGridVerticesEnumerator(k)]);
	enddforx

	// contact detection within cell
	#ifdef ompParticle
		#pragma omp parallel for
	#endif
	for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
	{ // No nead to loop over virtual particles here as well

		//printf("Number in the grid master:%d\n", fineGridVertex.getNumberOfParticles());
		for (int j=0; j<fineGridVertex.getNumberOfRealAndVirtualParticles(); j++)
		{
			//printf("Number in the grid slave:%d\n", fineGridVertex.getNumberOfRealAndVirtualParticles());
			//printf("I:%d J:%d\n", fineGridVertex.getParticle(i).getGlobalParticleNumber(), fineGridVertex.getParticle(j).getGlobalParticleNumber());

			if ((fineGridVertex.getParticle(i).getGlobalParticleId() == fineGridVertex.getParticle(j).getGlobalParticleId()) ||
				(fineGridVertex.getParticle(i).getIsObstacle() && fineGridVertex.getParticle(j).getIsObstacle()))
				 continue;

			if(_enableOverlapCheck)
			  if(delta::collision::isSphereOverlayInContact(
					fineGridVertex.getParticle(i).getCentre(0),
					fineGridVertex.getParticle(i).getCentre(1),
					fineGridVertex.getParticle(i).getCentre(2),
					fineGridVertex.getParticle(i).getInfluenceRadius(),

					fineGridVertex.getParticle(j).getCentre(0),
					fineGridVertex.getParticle(j).getCentre(1),
					fineGridVertex.getParticle(j).getCentre(2),
					fineGridVertex.getParticle(j).getInfluenceRadius())) continue;

			std::vector<delta::collision::contactpoint> newContactPoints;
			switch (_collisionModel)
			{
				case CollisionModel::Sphere:
				{
					bool penetration = false;
					if(fineGridVertex.getParticle(i).getIsObstacle() && !fineGridVertex.getParticle(j).getIsObstacle())
					{
						//printf("ENTERED fine grid sphere BA\n");
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
							fineGridVertex.getParticle(i).getGlobalParticleId(),
							penetration);

					} else if(!fineGridVertex.getParticle(i).getIsObstacle() && fineGridVertex.getParticle(j).getIsObstacle())
					{
						//printf("ENTERED fine grid sphere AB\n");
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
							fineGridVertex.getParticle(j).getGlobalParticleId(),
							penetration);
					} else
					{
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
							fineGridVertex.getParticle(j).getGlobalParticleId(),
							penetration);
					}
					break;
				}
				case CollisionModel::BruteForce:
				{
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
				case CollisionModel::Penalty:
				{
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
				case CollisionModel::PenaltyStat:
				{
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
				case CollisionModel::HybridOnBatches:
				{
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
				case CollisionModel::HybridOnTrianglePairsStats:
				{
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
				case CollisionModel::HybridOnBatchesStats:
				{
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
				case CollisionModel::HybridOnTrianglePairs:
				{
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
				case CollisionModel::GJK:
				{
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
			}

			if (!newContactPoints.empty())
				addCollision( newContactPoints, fineGridVertex.getParticle(i), fineGridVertex.getParticle(j) , dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere);
			#ifdef ompParticle
				#pragma omp critical
			#endif
			_state.incNumberOfTriangleComparisons( fineGridVertex.getNumberOfTriangles(i) * fineGridVertex.getNumberOfTriangles(j) );
		}
	}
	_state.incNumberOfParticleComparisons(fineGridVertex.getNumberOfParticles() * fineGridVertex.getNumberOfRealAndVirtualParticles())
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
	for (int i=0; i<vertexA.getNumberOfRealAndVirtualParticles(); i++)
	{
		for (int j=0; j<vertexB.getNumberOfRealAndVirtualParticles(); j++)
		{
			if(((vertexA.getParticle(i).getIsObstacle()) && (vertexB.getParticle(j).getIsObstacle())) ||
			   ((vertexA.getParticle(i).getGlobalParticleId()) == (vertexB.getParticle(j).getGlobalParticleId()))) continue;

			if(_enableOverlapCheck)
				if(!delta::collision::isSphereOverlayInContact(
						vertexA.getParticle(i).getCentre(0),
						vertexA.getParticle(i).getCentre(1),
						vertexA.getParticle(i).getCentre(2),
						vertexA.getParticle(i).getInfluenceRadius(),

						vertexB.getParticle(j).getCentre(0),
						vertexB.getParticle(j).getCentre(1),
						vertexB.getParticle(j).getCentre(2),
						vertexB.getParticle(j).getInfluenceRadius())) continue;

			std::vector<delta::collision::contactpoint> newContactPoints;
			switch (_collisionModel)
			{
				case CollisionModel::Sphere:
				{
					bool penetration = false;
					if(vertexA.getParticle(i).getIsObstacle() && !vertexB.getParticle(j).getIsObstacle())
					{
						//printf("ENTERED neighbor grid sphere BA\n");
						newContactPoints = delta::collision::sphereWithBarrierBA(
							vertexB.getParticle(j).getCentre(0),
							vertexB.getParticle(j).getCentre(1),
							vertexB.getParticle(j).getCentre(2),
							vertexB.getParticle(j).getDiameter(),
							vertexB.getParticle(j).getEpsilon(),
							vertexB.getParticle(j).getFriction(),
							vertexB.getParticle(j).getGlobalParticleId(),

							vertexA.getXCoordinates(i),
							vertexA.getYCoordinates(i),
							vertexA.getZCoordinates(i),
							vertexA.getNumberOfTriangles(i),
							vertexA.getParticle(i).getEpsilon(),
							vertexA.getParticle(i).getFriction(),
							vertexA.getParticle(i).getGlobalParticleId(),
							penetration);

					} else if(!vertexA.getParticle(i).getIsObstacle() && vertexB.getParticle(j).getIsObstacle())
					{
						//printf("ENTERED neighbor grid sphere AB\n");
						newContactPoints = delta::collision::sphereWithBarrierAB(
							vertexA.getParticle(i).getCentre(0),
							vertexA.getParticle(i).getCentre(1),
							vertexA.getParticle(i).getCentre(2),
							vertexA.getParticle(i).getDiameter(),
							vertexA.getParticle(i).getEpsilon(),
							vertexA.getParticle(i).getFriction(),
							vertexA.getParticle(i).getGlobalParticleId(),

							vertexB.getXCoordinates(j),
							vertexB.getYCoordinates(j),
							vertexB.getZCoordinates(j),
							vertexB.getNumberOfTriangles(j),
							vertexB.getParticle(j).getEpsilon(),
							vertexB.getParticle(j).getFriction(),
							vertexB.getParticle(j).getGlobalParticleId(),
							penetration);
					} else
					{
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
							vertexB.getParticle(j).getGlobalParticleId(),
							penetration);
					}
					if(penetration)
					{
						_state.informStatePenetration();
					}
					break;
				}
				case CollisionModel::BruteForce:
				{
					newContactPoints = delta::collision::bf(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
				case CollisionModel::Penalty:
				{
					newContactPoints = delta::collision::penalty(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
				case CollisionModel::PenaltyStat:
				{
					newContactPoints = delta::collision::penaltyStat(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
				case CollisionModel::HybridOnBatches:
				{
					newContactPoints = delta::collision::hybridWithPerBatchFallBack(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
				case CollisionModel::HybridOnTrianglePairs:
				{
					newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
				case CollisionModel::HybridOnBatchesStats:
				{
					newContactPoints = delta::collision::hybridWithPerBatchFallBack(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
				case CollisionModel::HybridOnTrianglePairsStats:
				{
					newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
				case CollisionModel::GJK:
				{
					newContactPoints = delta::collision::gjk(
						vertexA.getNumberOfTriangles(i),
						vertexA.getXCoordinates(i),
						vertexA.getYCoordinates(i),
						vertexA.getZCoordinates(i),
						vertexA.getParticle(i).getEpsilon(),
						vertexA.getParticle(i).getFriction(),
						vertexA.getParticle(i).getGlobalParticleId(),

						vertexB.getNumberOfTriangles(j),
						vertexB.getXCoordinates(j),
						vertexB.getYCoordinates(j),
						vertexB.getZCoordinates(j),
						vertexB.getParticle(j).getEpsilon(),
						vertexB.getParticle(j).getFriction(),
						vertexB.getParticle(j).getGlobalParticleId());
					break;
				}
			}

			if (!newContactPoints.empty())
				addCollision( newContactPoints, vertexA.getParticle(i), vertexB.getParticle(j), dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere);

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
	//phase 1
	// along faces
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(1) ]);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(2) ]);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);

	//phase 2
	//START front face: diagonal vertex comparison (if a front face vertex is boundary)
	if (fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() ||
		fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary())
	{
		//diagonal 1
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(3) ]);
		//diagonal 1

		//diagonal 2
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(2) ]);
		//diagonal 2
	}
	//END front face

	//phase 3
	//START back face: all vertices comparison (if a back face vertex is boundary)
	if (fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary() ||
		fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary())
	{
		//bottom line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		//bottom line

		//top line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(6) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		//top line

		//left line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		//left line

		//right line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(5) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		//right line

		//face diagonals
		//diagonal 1
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		//diagonal 1

		//diagonal 2
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(5) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		//diagonal 2
	}
	//END back face

	//phase 4
		//LEFT-RIGHT FACE
		//START left face vertex comparison (if a left face vertex is boundary)
		if (fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary())
		{
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		}
		//END left face

	//phase 5
		//START right face all vertices comparison (if a right face vertex is boundary)
		if (fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary())
		{
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(3) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
		}
		//END right face
		//END LEFT-RIGHT FACE


	//START faces bottom/top
	//phase 6
	//START face bottom vertices comparison (if a bottom vertex is boundary)
	if (fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() ||
		fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary())
	{
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
	}
	//END face bottom

	//phase 7
	//START face top all vertices comparison (if a top vertex is boundary)
	if (fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary() ||
		fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary())
	{
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(3) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(6) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
	}
	//END face top

	//phase 8
	//START diagonals on faces
		//front diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(3) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(2) ]);
		//front diagonals

		//bottom diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
		//bottom diagonals

		//left diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
		//left diagonals

		//right is missing???
	//END
	//phase 9
	//START intra-cell diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
	//END
	 */

	//phase 1
	// along faces
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(1) ]);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(2) ]);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);

	//phase 3
	//START back face: all vertices comparison (if a back face vertex is boundary)
	if (fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary() ||
		fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary())
	{
		//bottom line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		//bottom line

		//top line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(6) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		//top line

		//left line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		//left line

		//right line
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(5) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		//right line

		//face diagonals
		//diagonal 1
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		//diagonal 1

		//diagonal 2
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(5) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		//diagonal 2
	}
	//END back face

	//phase 4
		//LEFT-RIGHT FACE
		//START left face vertex comparison (if a left face vertex is boundary)
		if (fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary())
		{
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		}
		//END left face

	//phase 5
		//START right face all vertices comparison (if a right face vertex is boundary)
		if (fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary())
		{
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(5) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
			dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		}
		//END right face
		//END LEFT-RIGHT FACE


	//START faces bottom/top
	//phase 6
	//START face bottom vertices comparison (if a bottom vertex is boundary)
	if (fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() ||
		fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary())
	{
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(4) ], fineGridVertices[ fineGridVerticesEnumerator(3) ]);
	}
	//END face bottom

	//phase 7
	//START face top all vertices comparison (if a top vertex is boundary)
	if (fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary() ||
		fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary() || fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary())
	{
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(3) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(6) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
	}
	//END face top

	//phase 8
	//START diagonals on faces
		//front diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(3) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(2) ]);
		//front diagonals

		//bottom diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
		//bottom diagonals

		//left diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
		//left diagonals

		//right is missing???
	//END
	//phase 9
	//START intra-cell diagonals
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(0) ], fineGridVertices[ fineGridVerticesEnumerator(7) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(1) ], fineGridVertices[ fineGridVerticesEnumerator(6) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(2) ], fineGridVertices[ fineGridVerticesEnumerator(5) ]);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(fineGridVertices[ fineGridVerticesEnumerator(3) ], fineGridVertices[ fineGridVerticesEnumerator(4) ]);
	//END
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
