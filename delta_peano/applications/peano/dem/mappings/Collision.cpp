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

peano::CommunicationSpecification   dem::mappings::Collision::communicationSpecification() {
	return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexFirstTimeSpecification() { 
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
peano::MappingSpecification   dem::mappings::Collision::enterCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}

peano::MappingSpecification   dem::mappings::Collision::touchVertexLastTimeSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
peano::MappingSpecification   dem::mappings::Collision::leaveCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}

peano::MappingSpecification   dem::mappings::Collision::ascendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
}
peano::MappingSpecification   dem::mappings::Collision::descendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
}


tarch::logging::Log                                                 dem::mappings::Collision::_log( "dem::mappings::Collision" );
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_activeCollisions;
std::map<int, std::vector<dem::mappings::Collision::Collisions> >   dem::mappings::Collision::_collisionsOfNextTraversal;
dem::mappings::Collision::CollisionModel                            dem::mappings::Collision::_collisionModel;
int 																dem::mappings::Collision::_isNearToPenetration;


void dem::mappings::Collision::beginIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "beginIteration(State)", solverState );

	_isNearToPenetration = 0;

	_state = solverState;
	_state.clearAccumulatedData();
	assertion( _collisionsOfNextTraversal.empty() );

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

	if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::PenaltyStat)//PenaltyStat enum
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
		const records::Particle&                    particleB
) {
	assertion( !newContactPoints.empty() );

	if ( _collisionsOfNextTraversal.count(particleA._persistentRecords._globalParticleNumber)==0 ) {
		_collisionsOfNextTraversal.insert( std::pair<int,std::vector<Collisions>>( particleA._persistentRecords._globalParticleNumber, std::vector<Collisions>() ) );
	}

	if ( _collisionsOfNextTraversal.count(particleB._persistentRecords._globalParticleNumber)==0 ) {
		_collisionsOfNextTraversal.insert( std::pair<int,std::vector<Collisions>>( particleB._persistentRecords._globalParticleNumber, std::vector<Collisions>() ) );
	}

	Collisions* dataSetA = nullptr;
	Collisions* dataSetB = nullptr;

	for (std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleNumber].begin();
		 p!=_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleNumber].end();  p++)
	{
		if (p->_copyOfPartnerParticle._persistentRecords._globalParticleNumber==particleB._persistentRecords._globalParticleNumber)
		{
			dataSetA = &(*p);
		}
	}

	for (std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleNumber].begin();
		 p!=_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleNumber].end(); p++)
	{
		if (p->_copyOfPartnerParticle._persistentRecords._globalParticleNumber==particleA._persistentRecords._globalParticleNumber)
		{
			dataSetB = &(*p);
		}
	}

	assertion( (dataSetA==nullptr && dataSetB==nullptr) || (dataSetA!=nullptr && dataSetB!=nullptr) );

	if (dataSetA==nullptr)
	{
		_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleNumber].push_back( Collisions() );
		_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleNumber].push_back( Collisions() );

		dataSetA = &(_collisionsOfNextTraversal[particleA._persistentRecords._globalParticleNumber].back());
		dataSetB = &(_collisionsOfNextTraversal[particleB._persistentRecords._globalParticleNumber].back());

		dataSetA->_copyOfPartnerParticle = particleB;
		dataSetB->_copyOfPartnerParticle = particleA;
	}

	delta::collision::filter(dataSetA->_contactPoints, newContactPoints, std::min(particleA._persistentRecords._hMin, particleB._persistentRecords._hMin));

	//for debugging/plotting only
	if(particleA._persistentRecords._globalParticleNumber == 0 || particleB._persistentRecords._globalParticleNumber == 0)
	for (std::vector<delta::collision::contactpoint>::iterator p = newContactPoints.begin(); p != newContactPoints.end(); p++)
	{
		  double linearVelocityA[3];
		  linearVelocityA[0] = particleA._persistentRecords._velocity(0);
		  linearVelocityA[1] = particleA._persistentRecords._velocity(1);
	      linearVelocityA[2] = particleA._persistentRecords._velocity(2);

	      double velocity = sqrt(linearVelocityA[0]*linearVelocityA[0]+linearVelocityA[1]*linearVelocityA[1]+linearVelocityA[2]*linearVelocityA[2]);

	      p->friction[0] = -(((linearVelocityA[0]/velocity)-(linearVelocityA[0]*p->normal[0]))/velocity);
	      p->friction[1] = -(((linearVelocityA[1]/velocity)-(linearVelocityA[1]*p->normal[1]))/velocity);
	      p->friction[2] = -(((linearVelocityA[2]/velocity)-(linearVelocityA[2]*p->normal[2]))/velocity);

	      p->isThereFriction = true;
	 }
	/*
	 * Problem here was:
	 * Although all normals were pointing to opposite direction for each particle due to how we loop particles
	 * A. at each vertex or grid initial contact direction is interchangeable depending on what is A and B
	 * B. we always want to have the normal for particles to the position update direction (inner-direction)
	 * C. we always want to ensure that normal of particle point away from obstacle
	 * D. we don't care about normal of obstacle.
	 */
	dataSetA->_contactPoints.insert( dataSetA->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end() );

	for (std::vector<delta::collision::contactpoint>::iterator p = newContactPoints.begin(); p != newContactPoints.end(); p++)
	{
		//invert normal for particle B
		p->normal[0] = -1.0 * p->normal[0];
		p->normal[1] = -1.0 * p->normal[1];
		p->normal[2] = -1.0 * p->normal[2];

		//check if depth is critical and adjust timestep
        if(p->depth <= (particleA._persistentRecords._epsilon+particleB._persistentRecords._epsilon)/2) _state.informStateThatTwoParticlesAreClose();
	}

	dataSetB->_contactPoints.insert( dataSetB->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end() );
	//logInfo( "addCollision(...)", "add collision for particles " << particleA._persistentRecords._globalParticleNumber << " and " << particleB._persistentRecords._globalParticleNumber);

	#ifdef ompParticle
		#pragma omp critical
	#endif
	_state.incNumberOfContactPoints(newContactPoints.size());
}

#define DOT(a, b)\
		((a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2])

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

	for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
	{ // No need to loop over virtual particles here as well
		records::Particle& currentParticle = fineGridVertex.getParticle(i);

		// derive forces from collisions of previous grid traversal
		if( _activeCollisions.count(currentParticle._persistentRecords._globalParticleNumber)>0 )
		{
			//logInfo( "touchVertexFirstTime(...)", "particle no " << currentParticle._persistentRecords._globalParticleNumber << " does collide with other particle(s) in " << _activeCollisions[currentParticle._persistentRecords._globalParticleNumber].size() << " contact points" );
			for (std::vector<Collisions>::iterator p = _activeCollisions[currentParticle._persistentRecords._globalParticleNumber].begin();
				 p != _activeCollisions[currentParticle._persistentRecords._globalParticleNumber].end(); p++)
			{
				double force[3]  = {0.0,0.0,0.0};
				double torque[3] = {0.0,0.0,0.0};

				 delta::forces::getContactForces(
					p->_contactPoints,
					&(currentParticle._persistentRecords._centreOfMass(0)),
					&(currentParticle._persistentRecords._angularVelocity(0)),
					&(currentParticle._persistentRecords._velocity(0)),
					currentParticle._persistentRecords._mass,
					&(p->_copyOfPartnerParticle._persistentRecords._centreOfMass(0)),
					&(p->_copyOfPartnerParticle._persistentRecords._angularVelocity(0)),
					&(p->_copyOfPartnerParticle._persistentRecords._velocity(0)),
					p->_copyOfPartnerParticle._persistentRecords._mass,
					force,
					torque);

				 	//logInfo( "touchVertexFirstTime(...)", "add force f=" << force[0] << ", " << force[1] << ", " << force[2] << " to particle no " << currentParticle._persistentRecords.getGlobalParticleNumber() );

				if (currentParticle._persistentRecords.getGlobalParticleNumber() != 0)
				{
					double timeStepSize = _state.getTimeStepSize();

					currentParticle._persistentRecords._angularVelocity(0) += timeStepSize * (force[0] / currentParticle._persistentRecords._mass);
					currentParticle._persistentRecords._angularVelocity(1) += timeStepSize * (force[1] / currentParticle._persistentRecords._mass);
					currentParticle._persistentRecords._angularVelocity(2) += timeStepSize * (force[2] / currentParticle._persistentRecords._mass);

					currentParticle._persistentRecords._velocity(0) += timeStepSize * (force[0] / currentParticle._persistentRecords._mass);
					currentParticle._persistentRecords._velocity(1) += timeStepSize * (force[1] / currentParticle._persistentRecords._mass);
					currentParticle._persistentRecords._velocity(2) += timeStepSize * (force[2] / currentParticle._persistentRecords._mass);

					//logInfo( "touchVertexFirstTime(...)", "add angvel av=" << currentParticle._persistentRecords._angularVelocity(0) << ", " << currentParticle._persistentRecords._angularVelocity(1) << ", " << currentParticle._persistentRecords._angularVelocity(2) << " to particle no " << currentParticle._persistentRecords.getGlobalParticleNumber() );
					//logInfo( "touchVertexFirstTime(...)", "add vel av=" << currentParticle._persistentRecords._velocity(0) << ", " << currentParticle._persistentRecords._velocity(1) << ", " << currentParticle._persistentRecords._velocity(2) << " to particle no " << currentParticle._persistentRecords.getGlobalParticleNumber() );
				}
			}
			_activeCollisions.erase(currentParticle._persistentRecords._globalParticleNumber);
		}
	}

	// clear adaptivity/mulitlevel data
	fineGridVertex.clearInheritedCoarseGridParticles();
	dfor2(k)
	fineGridVertex.inheritCoarseGridParticles(coarseGridVertices[ coarseGridVerticesEnumerator(k) ]);
	enddforx

	// contact detection within cell
	#ifdef ompParticle
		#pragma omp parallel for
	#endif
	for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
	{ // No nead to loop over virtual particles here as well
		for (int j=0; j<fineGridVertex.getNumberOfRealAndVirtualParticles(); j++)
		{
			if ( (fineGridVertex.getParticle(i).getVelocity(0)) != (fineGridVertex.getParticle(j).getVelocity(0)) )
			{
				std::vector<delta::collision::contactpoint> newContactPoints;
				switch (_collisionModel) {
				case CollisionModel::Sphere:
				{
					if(fineGridVertex.getParticle(i)._persistentRecords.getGlobalParticleNumber() <= 0 && fineGridVertex.getParticle(j)._persistentRecords.getGlobalParticleNumber() <= 0)
					{
						//printf("entered - Obstacle to Obstacle\n");
						//obstacle to obstacle
					} else if(fineGridVertex.getParticle(i)._persistentRecords.getGlobalParticleNumber() <= 0 && fineGridVertex.getParticle(j)._persistentRecords.getGlobalParticleNumber() > 0)
					{
						if(delta::collision::isSphereOverlayInContact(
								fineGridVertex.getParticle(i).getCentre(0),
								fineGridVertex.getParticle(i).getCentre(1),
								fineGridVertex.getParticle(i).getCentre(2),
								fineGridVertex.getParticle(i).getInfluenceRadius(),

								fineGridVertex.getParticle(j).getCentre(0),
								fineGridVertex.getParticle(j).getCentre(1),
								fineGridVertex.getParticle(j).getCentre(2),
								fineGridVertex.getParticle(j).getInfluenceRadius()))
						{
							newContactPoints = delta::collision::sphereWithBarrierBA(
								fineGridVertex.getParticle(j).getCentre(0),
								fineGridVertex.getParticle(j).getCentre(1),
								fineGridVertex.getParticle(j).getCentre(2),
								fineGridVertex.getParticle(j).getDiameter(),
								fineGridVertex.getParticle(j)._persistentRecords._epsilon,

								fineGridVertex.getXCoordinates( i ),
								fineGridVertex.getYCoordinates( i ),
								fineGridVertex.getZCoordinates( i ),
								fineGridVertex.getNumberOfTriangles( i ),
								fineGridVertex.getParticle(i)._persistentRecords._epsilon
							);
						}

					} else if(fineGridVertex.getParticle(i)._persistentRecords.getGlobalParticleNumber() > 0 && fineGridVertex.getParticle(j)._persistentRecords.getGlobalParticleNumber() <= 0)
					{
						if(delta::collision::isSphereOverlayInContact(
								fineGridVertex.getParticle(i).getCentre(0),
								fineGridVertex.getParticle(i).getCentre(1),
								fineGridVertex.getParticle(i).getCentre(2),
								fineGridVertex.getParticle(i).getInfluenceRadius(),

								fineGridVertex.getParticle(j).getCentre(0),
								fineGridVertex.getParticle(j).getCentre(1),
								fineGridVertex.getParticle(j).getCentre(2),
								fineGridVertex.getParticle(j).getInfluenceRadius()))
						{
							newContactPoints = delta::collision::sphereWithBarrierAB(
								fineGridVertex.getParticle(i).getCentre(0),
								fineGridVertex.getParticle(i).getCentre(1),
								fineGridVertex.getParticle(i).getCentre(2),
								fineGridVertex.getParticle(i).getDiameter(),
								fineGridVertex.getParticle(i)._persistentRecords._epsilon,

								fineGridVertex.getXCoordinates( j ),
								fineGridVertex.getYCoordinates( j ),
								fineGridVertex.getZCoordinates( j ),
								fineGridVertex.getNumberOfTriangles( j ),
								fineGridVertex.getParticle(j)._persistentRecords._epsilon
							);
						}
					} else
					{
						newContactPoints = delta::collision::sphere(
								fineGridVertex.getParticle(i).getCentre(0),
								fineGridVertex.getParticle(i).getCentre(1),
								fineGridVertex.getParticle(i).getCentre(2),
								fineGridVertex.getParticle(i).getDiameter(),
								fineGridVertex.getParticle(i)._persistentRecords._epsilon,
								fineGridVertex.getParticle(i)._persistentRecords._globalParticleNumber,

								fineGridVertex.getParticle(j).getCentre(0),
								fineGridVertex.getParticle(j).getCentre(1),
								fineGridVertex.getParticle(j).getCentre(2),
								fineGridVertex.getParticle(j).getDiameter(),
								fineGridVertex.getParticle(j)._persistentRecords._epsilon,
								fineGridVertex.getParticle(j)._persistentRecords._globalParticleNumber
						);
					}
					break;
				}
				case CollisionModel::BruteForce:
				{
					newContactPoints = delta::collision::bf(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getParticle(i)._persistentRecords._epsilon,
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
				case CollisionModel::Penalty:
				{
					newContactPoints = delta::collision::penalty(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getParticle(i)._persistentRecords._epsilon,
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
				case CollisionModel::PenaltyStat:
				{
					newContactPoints = delta::collision::penaltyStat(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getParticle(i)._persistentRecords._epsilon,
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
				case CollisionModel::HybridOnBatches:
				{
					newContactPoints = delta::collision::hybridWithPerBatchFallBack(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getParticle(i)._persistentRecords._epsilon,
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
				case CollisionModel::HybridOnTrianglePairsStats:
				{
					newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getParticle(i)._persistentRecords._epsilon,
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
				case CollisionModel::HybridOnBatchesStats:
				{
					newContactPoints = delta::collision::hybridWithPerBatchFallBack(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getParticle(i)._persistentRecords._epsilon,
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
				case CollisionModel::HybridOnTrianglePairs:
				{
					newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getParticle(i)._persistentRecords._epsilon,
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
				case CollisionModel::SphereHybridOnBatches:
				{
					if(delta::collision::isSphereOverlayInContact(
							fineGridVertex.getParticle(i).getCentre(0),
							fineGridVertex.getParticle(i).getCentre(1),
							fineGridVertex.getParticle(i).getCentre(2),
							fineGridVertex.getParticle(i).getInfluenceRadius(),

							fineGridVertex.getParticle(j).getCentre(0),
							fineGridVertex.getParticle(j).getCentre(1),
							fineGridVertex.getParticle(j).getCentre(2),
							fineGridVertex.getParticle(j).getInfluenceRadius()))
					{
						newContactPoints = delta::collision::hybridWithPerBatchFallBack(
								fineGridVertex.getNumberOfTriangles( i ),
								fineGridVertex.getXCoordinates( i ),
								fineGridVertex.getYCoordinates( i ),
								fineGridVertex.getZCoordinates( i ),
								fineGridVertex.getParticle(i)._persistentRecords._epsilon,
								fineGridVertex.getNumberOfTriangles( j ),
								fineGridVertex.getXCoordinates( j ),
								fineGridVertex.getYCoordinates( j ),
								fineGridVertex.getZCoordinates( j ),
								fineGridVertex.getParticle(j)._persistentRecords._epsilon
						);
					}
					break;
				}
				case CollisionModel::SphereHybridOnTrianglePairs:
				{
					if(delta::collision::isSphereOverlayInContact(
							fineGridVertex.getParticle(i).getCentre(0),
							fineGridVertex.getParticle(i).getCentre(1),
							fineGridVertex.getParticle(i).getCentre(2),
							fineGridVertex.getParticle(i).getInfluenceRadius(),

							fineGridVertex.getParticle(j).getCentre(0),
							fineGridVertex.getParticle(j).getCentre(1),
							fineGridVertex.getParticle(j).getCentre(2),
							fineGridVertex.getParticle(j).getInfluenceRadius()))
					{
						newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
								fineGridVertex.getNumberOfTriangles( i ),
								fineGridVertex.getXCoordinates( i ),
								fineGridVertex.getYCoordinates( i ),
								fineGridVertex.getZCoordinates( i ),
								fineGridVertex.getParticle(i)._persistentRecords._epsilon,
								fineGridVertex.getNumberOfTriangles( j ),
								fineGridVertex.getXCoordinates( j ),
								fineGridVertex.getYCoordinates( j ),
								fineGridVertex.getZCoordinates( j ),
								fineGridVertex.getParticle(j)._persistentRecords._epsilon
						);
					}
					break;
				}
				case CollisionModel::SphereBruteForce:
				{
					if(delta::collision::isSphereOverlayInContact(
							fineGridVertex.getParticle(i).getCentre(0),
							fineGridVertex.getParticle(i).getCentre(1),
							fineGridVertex.getParticle(i).getCentre(2),
							fineGridVertex.getParticle(i).getInfluenceRadius(),

							fineGridVertex.getParticle(j).getCentre(0),
							fineGridVertex.getParticle(j).getCentre(1),
							fineGridVertex.getParticle(j).getCentre(2),
							fineGridVertex.getParticle(j).getInfluenceRadius()))
					{
						newContactPoints = delta::collision::bf(
								fineGridVertex.getNumberOfTriangles( i ),
								fineGridVertex.getXCoordinates( i ),
								fineGridVertex.getYCoordinates( i ),
								fineGridVertex.getZCoordinates( i ),
								fineGridVertex.getParticle(i)._persistentRecords._epsilon,
								fineGridVertex.getNumberOfTriangles( j ),
								fineGridVertex.getXCoordinates( j ),
								fineGridVertex.getYCoordinates( j ),
								fineGridVertex.getZCoordinates( j ),
								fineGridVertex.getParticle(j)._persistentRecords._epsilon
						);
					}
					break;
				}
				case CollisionModel::SpherePenalty:
				{
					if(delta::collision::isSphereOverlayInContact(
							fineGridVertex.getParticle(i).getCentre(0),
							fineGridVertex.getParticle(i).getCentre(1),
							fineGridVertex.getParticle(i).getCentre(2),
							fineGridVertex.getParticle(i).getInfluenceRadius(),

							fineGridVertex.getParticle(j).getCentre(0),
							fineGridVertex.getParticle(j).getCentre(1),
							fineGridVertex.getParticle(j).getCentre(2),
							fineGridVertex.getParticle(j).getInfluenceRadius()))
					{
						newContactPoints = delta::collision::penalty(
								fineGridVertex.getNumberOfTriangles( i ),
								fineGridVertex.getXCoordinates( i ),
								fineGridVertex.getYCoordinates( i ),
								fineGridVertex.getZCoordinates( i ),
								fineGridVertex.getParticle(i)._persistentRecords._epsilon,
								fineGridVertex.getNumberOfTriangles( j ),
								fineGridVertex.getXCoordinates( j ),
								fineGridVertex.getYCoordinates( j ),
								fineGridVertex.getZCoordinates( j ),
								fineGridVertex.getParticle(j)._persistentRecords._epsilon
						);
					}
					break;
				}
				case CollisionModel::GJK:
				{
					newContactPoints = delta::collision::gjk(
							fineGridVertex.getNumberOfTriangles( i ),
							fineGridVertex.getXCoordinates( i ),
							fineGridVertex.getYCoordinates( i ),
							fineGridVertex.getZCoordinates( i ),
							fineGridVertex.getNumberOfTriangles( j ),
							fineGridVertex.getXCoordinates( j ),
							fineGridVertex.getYCoordinates( j ),
							fineGridVertex.getZCoordinates( j ),
							fineGridVertex.getParticle(j)._persistentRecords._epsilon
					);
					break;
				}
			}

			if (!newContactPoints.empty())
			{
				#ifdef ompParticle
					#pragma omp critical
				#endif
				addCollision( newContactPoints, fineGridVertex.getParticle(i), fineGridVertex.getParticle(j) );
			}
			#ifdef ompParticle
				#pragma omp critical
			#endif
			_state.incNumberOfTriangleComparisons( fineGridVertex.getNumberOfTriangles( i ) * fineGridVertex.getNumberOfTriangles( j ) );
			}
		}
	}

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
		//if(vertexA.getParticle(i)._persistentRecords._globalParticleNumber == 0)continue;
		for (int j=0; j<vertexB.getNumberOfRealAndVirtualParticles(); j++)
		{
			if((vertexA.getParticle(i)._persistentRecords.getGlobalParticleNumber() == 0) && (vertexB.getParticle(j)._persistentRecords.getGlobalParticleNumber()==0)) continue;
			// If we use adaptive grids, it happens that coarse particles are
			// assigned to both fine grid vertices. In this case, we have to
			// check explicitly to ensure that we do not compare a particle with
			// itself. Two particles are equal if they refer to the same vertex
			// set.
			if ( (vertexA.getParticle(i)._persistentRecords._globalParticleNumber) != (vertexB.getParticle(j)._persistentRecords._globalParticleNumber) )
			{
				//printf("VertexAOuterLoop:%d VertexBinnerLoop:%d\n", vertexA.getParticle(i)._persistentRecords._globalParticleNumber, vertexB.getParticle(j)._persistentRecords._globalParticleNumber);

				std::vector<delta::collision::contactpoint> newContactPoints;
				switch (_collisionModel)
				{
					case CollisionModel::Sphere:
					{
						if(vertexA.getParticle(i)._persistentRecords.getGlobalParticleNumber() <= 0 && vertexB.getParticle(j)._persistentRecords.getGlobalParticleNumber() <= 0)
						{
							//printf("entered - Obstacle to Obstacle\n");
							//obstacle to obstacle
						} else if(vertexA.getParticle(i)._persistentRecords.getGlobalParticleNumber() <= 0 && vertexB.getParticle(j)._persistentRecords.getGlobalParticleNumber() > 0)
						{
							if(delta::collision::isSphereOverlayInContact(
								vertexA.getParticle(i).getCentre(0),
								vertexA.getParticle(i).getCentre(1),
								vertexA.getParticle(i).getCentre(2),
								vertexA.getParticle(i).getInfluenceRadius(),

								vertexB.getParticle(j).getCentre(0),
								vertexB.getParticle(j).getCentre(1),
								vertexB.getParticle(j).getCentre(2),
								vertexB.getParticle(j).getInfluenceRadius()))
							{
								newContactPoints = delta::collision::sphereWithBarrierBA(
								vertexB.getParticle(j).getCentre(0),
								vertexB.getParticle(j).getCentre(1),
								vertexB.getParticle(j).getCentre(2),
								vertexB.getParticle(j).getDiameter(),
								vertexB.getParticle(j)._persistentRecords._epsilon,

								vertexA.getXCoordinates( i ),
								vertexA.getYCoordinates( i ),
								vertexA.getZCoordinates( i ),
								vertexA.getNumberOfTriangles( i ),
								vertexA.getParticle(i)._persistentRecords._epsilon
								);
							}

						} else if(vertexA.getParticle(i)._persistentRecords.getGlobalParticleNumber() > 0 && vertexB.getParticle(j)._persistentRecords.getGlobalParticleNumber() <= 0)
						{
							if(delta::collision::isSphereOverlayInContact(
								vertexA.getParticle(i).getCentre(0),
								vertexA.getParticle(i).getCentre(1),
								vertexA.getParticle(i).getCentre(2),
								vertexA.getParticle(i).getInfluenceRadius(),

								vertexB.getParticle(j).getCentre(0),
								vertexB.getParticle(j).getCentre(1),
								vertexB.getParticle(j).getCentre(2),
								vertexB.getParticle(j).getInfluenceRadius()))
							{
								newContactPoints = delta::collision::sphereWithBarrierAB(
								vertexA.getParticle(i).getCentre(0),
								vertexA.getParticle(i).getCentre(1),
								vertexA.getParticle(i).getCentre(2),
								vertexA.getParticle(i).getDiameter(),
								vertexA.getParticle(i)._persistentRecords._epsilon,

								vertexB.getXCoordinates( j ),
								vertexB.getYCoordinates( j ),
								vertexB.getZCoordinates( j ),
								vertexB.getNumberOfTriangles( j ),
								vertexB.getParticle(j)._persistentRecords._epsilon);
							}
						} else
						{
							newContactPoints = delta::collision::sphere(
								vertexA.getParticle(i).getCentre(0),
								vertexA.getParticle(i).getCentre(1),
								vertexA.getParticle(i).getCentre(2),
								vertexA.getParticle(i).getDiameter(),
								vertexA.getParticle(i)._persistentRecords._epsilon,
								vertexA.getParticle(i)._persistentRecords._globalParticleNumber,

								vertexB.getParticle(j).getCentre(0),
								vertexB.getParticle(j).getCentre(1),
								vertexB.getParticle(j).getCentre(2),
								vertexB.getParticle(j).getDiameter(),
								vertexB.getParticle(j)._persistentRecords._epsilon,
								vertexB.getParticle(j)._persistentRecords._globalParticleNumber);
						}
						break;
					}
					case CollisionModel::BruteForce:
					{
						newContactPoints = delta::collision::bf(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexA.getParticle(i)._persistentRecords._epsilon,
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon
						);
						break;
					}
					case CollisionModel::Penalty:
					{
						newContactPoints = delta::collision::penalty(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexA.getParticle(i)._persistentRecords._epsilon,
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon
						);
						break;
					}
					case CollisionModel::PenaltyStat:
					{
						newContactPoints = delta::collision::penaltyStat(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexA.getParticle(i)._persistentRecords._epsilon,
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon);
						break;
					}
					case CollisionModel::HybridOnBatches:
					{
						newContactPoints = delta::collision::hybridWithPerBatchFallBack(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexA.getParticle(i)._persistentRecords._epsilon,
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon);
						break;
					}
					case CollisionModel::HybridOnTrianglePairs:
					{
						newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexA.getParticle(i)._persistentRecords._epsilon,
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon);
						break;
					}
					case CollisionModel::HybridOnBatchesStats:
					{
						newContactPoints = delta::collision::hybridWithPerBatchFallBack(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexA.getParticle(i)._persistentRecords._epsilon,
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon);
						break;
					}
					case CollisionModel::HybridOnTrianglePairsStats:
					{
						newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexA.getParticle(i)._persistentRecords._epsilon,
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon);
						break;
					}
					case CollisionModel::SphereHybridOnBatches:
					{
						if(delta::collision::isSphereOverlayInContact(
							vertexA.getParticle(i).getCentre(0),
							vertexA.getParticle(i).getCentre(1),
							vertexA.getParticle(i).getCentre(2),
							vertexA.getParticle(i).getInfluenceRadius(),

							vertexB.getParticle(j).getCentre(0),
							vertexB.getParticle(j).getCentre(1),
							vertexB.getParticle(j).getCentre(2),
							vertexB.getParticle(j).getInfluenceRadius()))
						{
							newContactPoints = delta::collision::hybridWithPerBatchFallBack(
								vertexA.getNumberOfTriangles( i ),
								vertexA.getXCoordinates( i ),
								vertexA.getYCoordinates( i ),
								vertexA.getZCoordinates( i ),
								vertexA.getParticle(i)._persistentRecords._epsilon,
								vertexB.getNumberOfTriangles( j ),
								vertexB.getXCoordinates( j ),
								vertexB.getYCoordinates( j ),
								vertexB.getZCoordinates( j ),
								vertexB.getParticle(j)._persistentRecords._epsilon);
						}
						break;
					}
					case CollisionModel::SphereHybridOnTrianglePairs:
					{
						if(delta::collision::isSphereOverlayInContact(
							vertexA.getParticle(i).getCentre(0),
							vertexA.getParticle(i).getCentre(1),
							vertexA.getParticle(i).getCentre(2),
							vertexA.getParticle(i).getInfluenceRadius(),

							vertexB.getParticle(j).getCentre(0),
							vertexB.getParticle(j).getCentre(1),
							vertexB.getParticle(j).getCentre(2),
							vertexB.getParticle(j).getInfluenceRadius()))
						{
							newContactPoints = delta::collision::hybridWithPerTriangleFallBack(
								vertexA.getNumberOfTriangles( i ),
								vertexA.getXCoordinates( i ),
								vertexA.getYCoordinates( i ),
								vertexA.getZCoordinates( i ),
								vertexA.getParticle(i)._persistentRecords._epsilon,
								vertexB.getNumberOfTriangles( j ),
								vertexB.getXCoordinates( j ),
								vertexB.getYCoordinates( j ),
								vertexB.getZCoordinates( j ),
								vertexB.getParticle(j)._persistentRecords._epsilon);
						}
						break;
					}
					case CollisionModel::SphereBruteForce:
					{
						if(delta::collision::isSphereOverlayInContact(
							vertexA.getParticle(i).getCentre(0),
							vertexA.getParticle(i).getCentre(1),
							vertexA.getParticle(i).getCentre(2),
							vertexA.getParticle(i).getInfluenceRadius(),

							vertexB.getParticle(j).getCentre(0),
							vertexB.getParticle(j).getCentre(1),
							vertexB.getParticle(j).getCentre(2),
							vertexB.getParticle(j).getInfluenceRadius()))
						{
							newContactPoints = delta::collision::bf(
								vertexA.getNumberOfTriangles( i ),
								vertexA.getXCoordinates( i ),
								vertexA.getYCoordinates( i ),
								vertexA.getZCoordinates( i ),
								vertexA.getParticle(i)._persistentRecords._epsilon,
								vertexB.getNumberOfTriangles( j ),
								vertexB.getXCoordinates( j ),
								vertexB.getYCoordinates( j ),
								vertexB.getZCoordinates( j ),
								vertexB.getParticle(j)._persistentRecords._epsilon);
						}
						break;
					}
					case CollisionModel::SpherePenalty:
					{
						if(delta::collision::isSphereOverlayInContact(
							vertexA.getParticle(i).getCentre(0),
							vertexA.getParticle(i).getCentre(1),
							vertexA.getParticle(i).getCentre(2),
							vertexA.getParticle(i).getInfluenceRadius(),

							vertexB.getParticle(j).getCentre(0),
							vertexB.getParticle(j).getCentre(1),
							vertexB.getParticle(j).getCentre(2),
							vertexB.getParticle(j).getInfluenceRadius()))
						{
							newContactPoints = delta::collision::penalty(
								vertexA.getNumberOfTriangles( i ),
								vertexA.getXCoordinates( i ),
								vertexA.getYCoordinates( i ),
								vertexA.getZCoordinates( i ),
								vertexA.getParticle(i)._persistentRecords._epsilon,
								vertexB.getNumberOfTriangles( j ),
								vertexB.getXCoordinates( j ),
								vertexB.getYCoordinates( j ),
								vertexB.getZCoordinates( j ),
								vertexB.getParticle(j)._persistentRecords._epsilon);
						}
						break;
					}
					case CollisionModel::GJK:
					{
						newContactPoints = delta::collision::gjk(
							vertexA.getNumberOfTriangles( i ),
							vertexA.getXCoordinates( i ),
							vertexA.getYCoordinates( i ),
							vertexA.getZCoordinates( i ),
							vertexB.getNumberOfTriangles( j ),
							vertexB.getXCoordinates( j ),
							vertexB.getYCoordinates( j ),
							vertexB.getZCoordinates( j ),
							vertexB.getParticle(j)._persistentRecords._epsilon);
						break;
					}
				}

				if (!newContactPoints.empty())
				{
					#ifdef ompParticle
						#pragma omp critical
					#endif
					addCollision( newContactPoints, vertexA.getParticle(i), vertexB.getParticle(j) );
				}

					//printf("DifferentVertexContact:%d\n", newContactPoints.size());
					//printf("VertexANoParticles:%d VertexBNoParticles:%d\n", vertexA.getNumberOfRealAndVirtualParticles(), vertexB.getNumberOfRealAndVirtualParticles());
					//printf("VertexA:%d VertexB:%d\n", vertexA.getParticle(i)._persistentRecords._globalParticleNumber, vertexB.getParticle(j)._persistentRecords._globalParticleNumber);
					#ifdef ompParticle
						#pragma omp critical
					#endif
					_state.incNumberOfTriangleComparisons( vertexA.getNumberOfTriangles( i ) * vertexB.getNumberOfTriangles( j ) );
			}
		}
	}
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

	// along faces
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(0) ],
			fineGridVertices[ fineGridVerticesEnumerator(1) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(0) ],
			fineGridVertices[ fineGridVerticesEnumerator(2) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(0) ],
			fineGridVertices[ fineGridVerticesEnumerator(4) ]
	);

	// additional faces front/back
	if (
			fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary()

	) {
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(0) ],
				fineGridVertices[ fineGridVerticesEnumerator(3) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(1) ],
				fineGridVertices[ fineGridVerticesEnumerator(2) ]
		);
	}
	if (
			fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary()

	) {
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(4) ],
				fineGridVertices[ fineGridVerticesEnumerator(5) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(6) ],
				fineGridVertices[ fineGridVerticesEnumerator(7) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(4) ],
				fineGridVertices[ fineGridVerticesEnumerator(6) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(5) ],
				fineGridVertices[ fineGridVerticesEnumerator(7) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(4) ],
				fineGridVertices[ fineGridVerticesEnumerator(7) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(5) ],
				fineGridVertices[ fineGridVerticesEnumerator(6) ]
		);
	}

	// additional faces left/right
	if (
			fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary()

	) {
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(2) ],
				fineGridVertices[ fineGridVerticesEnumerator(5) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(3) ],
				fineGridVertices[ fineGridVerticesEnumerator(5) ]
		);
	}
	if (
			fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary()

	) {
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(1) ],
				fineGridVertices[ fineGridVerticesEnumerator(4) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(3) ],
				fineGridVertices[ fineGridVerticesEnumerator(6) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(1) ],
				fineGridVertices[ fineGridVerticesEnumerator(3) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(4) ],
				fineGridVertices[ fineGridVerticesEnumerator(6) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(1) ],
				fineGridVertices[ fineGridVerticesEnumerator(6) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(3) ],
				fineGridVertices[ fineGridVerticesEnumerator(4) ]
		);
	}

	// additional faces bottom/top
	if (
			fineGridVertices[fineGridVerticesEnumerator(0) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(1) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(4) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(5) ].isBoundary()

	) {
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(4) ],
				fineGridVertices[ fineGridVerticesEnumerator(5) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(1) ],
				fineGridVertices[ fineGridVerticesEnumerator(5) ]
		);
	}
	if (
			fineGridVertices[fineGridVerticesEnumerator(2) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(3) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(6) ].isBoundary() ||
			fineGridVertices[fineGridVerticesEnumerator(7) ].isBoundary()

	) {
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(2) ],
				fineGridVertices[ fineGridVerticesEnumerator(3) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(6) ],
				fineGridVertices[ fineGridVerticesEnumerator(7) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(2) ],
				fineGridVertices[ fineGridVerticesEnumerator(6) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(3) ],
				fineGridVertices[ fineGridVerticesEnumerator(7) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(3) ],
				fineGridVertices[ fineGridVerticesEnumerator(6) ]
		);
		dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
				fineGridVertices[ fineGridVerticesEnumerator(2) ],
				fineGridVertices[ fineGridVerticesEnumerator(7) ]
		);
	}

	// diagonals on faces
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(0) ],
			fineGridVertices[ fineGridVerticesEnumerator(3) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(1) ],
			fineGridVertices[ fineGridVerticesEnumerator(2) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(0) ],
			fineGridVertices[ fineGridVerticesEnumerator(5) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(1) ],
			fineGridVertices[ fineGridVerticesEnumerator(4) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(0) ],
			fineGridVertices[ fineGridVerticesEnumerator(6) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(2) ],
			fineGridVertices[ fineGridVerticesEnumerator(4) ]
	);

	// intra-cell diagonals
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(0) ],
			fineGridVertices[ fineGridVerticesEnumerator(7) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(1) ],
			fineGridVertices[ fineGridVerticesEnumerator(6) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(2) ],
			fineGridVertices[ fineGridVerticesEnumerator(5) ]
	);
	dem::mappings::Collision::collideParticlesOfTwoDifferentVertices(
			fineGridVertices[ fineGridVerticesEnumerator(3) ],
			fineGridVertices[ fineGridVerticesEnumerator(4) ]
	);

	logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}



//
//   NOP
// =======
//
dem::mappings::Collision::Collision() {
	logTraceIn( "Collision()" );
	// @todo Insert your code here
	logTraceOut( "Collision()" );
}


dem::mappings::Collision::~Collision() {
	logTraceIn( "~Collision()" );
	// @todo Insert your code here
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
