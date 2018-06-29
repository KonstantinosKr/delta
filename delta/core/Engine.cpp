/*
 * Engine.cpp
 *
 *  Created on: 11 Jun 2018
 *      Author: konstantinos
 */

#include "delta/core/Engine.h"

delta::core::Engine::Engine()
{

}

delta::core::Engine::Engine(
	bool overlapCheck,
	bool plot,
	iREAL dt,
	bool gravity,
	delta::core::data::Structure data)
{
  _overlapCheck = overlapCheck;
  _state = delta::core::State::State();
  _plot = plot;
  _collisionModel = delta::core::Engine::CollisionModel::none;
  _dt = dt;
  _gravity = gravity;
  _data = data;

  std::array<iREAL, 6> boundary;
  boundary[0] = 0.0;
  boundary[1] = 0.0;
  boundary[2] = 0.0;

  boundary[3] = 1.0;
  boundary[4] = 1.0;
  boundary[5] = 1.0;

  _boundary = boundary;

  _state = delta::core::State(data.getNumberOfParticles(), data.getNumberOfObstacles(), dt);
}

delta::core::Engine::Engine(
	bool overlapCheck,
	bool plot,
	iREAL dt,
	bool gravity,
	CollisionModel collisionModel,
	delta::core::data::Structure data)
{
  _overlapCheck = overlapCheck;
  _state = delta::core::State::State();
  _plot = plot;
  _collisionModel = collisionModel;
  _dt = dt;
  _gravity = gravity;
  _data = data;

  std::array<iREAL, 6> boundary;
  boundary[0] = 0.0;
  boundary[1] = 0.0;
  boundary[2] = 0.0;

  boundary[3] = 1.0;
  boundary[4] = 1.0;
  boundary[5] = 1.0;

  _boundary = boundary;

  _state = delta::core::State(data.getNumberOfParticles(), data.getNumberOfObstacles(), dt);
}

delta::core::Engine::~Engine()
{

}

void delta::core::Engine::iterate()
{
  if(_plot)
  {
	delta::core::io::writeGeometryToVTK(_state.getIteration(), _boundary, _data.getAll());
  }

  //delta::core::Engine::contactDetection();
  //delta::core::Engine::deriveForces();
  delta::core::Engine::updatePosition();

  _state.update();
}

void delta::core::Engine::addCollision(
	std::vector<delta::contact::contactpoint>& 	newContactPoints,
	delta::core::data::ParticleRecord&           particleA,
	delta::core::data::ParticleRecord&           particleB,
	bool sphere
) {
/*
  //////////////START initial insertion of collision vectors into _collisionsOfNextTraversal<id, collision> map for next move update of particle A and B
  if( _collisionsOfNextTraversal.count(particleA.getGlobalParticleID())==0 ) {
	_collisionsOfNextTraversal.insert(std::pair<int,std::vector<Collisions>>(particleA.getGlobalParticleID(), std::vector<Collisions>()));
  }

  if( _collisionsOfNextTraversal.count(particleB.getGlobalParticleID())==0 ) {
	_collisionsOfNextTraversal.insert(std::pair<int,std::vector<Collisions>>(particleB.getGlobalParticleID(), std::vector<Collisions>()));
  }
  ///////////////END

  //START DATASET pointer
  Collisions* dataSetA = nullptr;
  Collisions* dataSetB = nullptr;
  //////////////END

  /////////////////START if already exist | find and assign reference collision list to dataA or dataB particle
  for (std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleA.getGlobalParticleID()].begin();
	 p!=_collisionsOfNextTraversal[particleA.getGlobalParticleID()].end();  p++)
  {
	if(p->_copyOfPartnerParticle.getGlobalParticleID()==particleB.getGlobalParticleID())
	{
	  dataSetA = &(*p);
	}
  }

  for(std::vector<Collisions>::iterator p=_collisionsOfNextTraversal[particleB.getGlobalParticleID()].begin();
	 p!=_collisionsOfNextTraversal[particleB.getGlobalParticleID()].end(); p++)
  {
	if(p->_copyOfPartnerParticle.getGlobalParticleID()==particleA.getGlobalParticleID())
	{
	  dataSetB = &(*p);
	}
  }
  //////////////END

  if((dataSetA!=nullptr && dataSetB!=nullptr)) {
	newContactPoints.clear();
	return;
  }
  //END

  //START if dataset A and B is empty
  if(dataSetA==nullptr)
  {
	//START push_back collisions object into corresponding both A and B particle index collision list
	_collisionsOfNextTraversal[particleA.getGlobalParticleID()].push_back( Collisions() );
	_collisionsOfNextTraversal[particleB.getGlobalParticleID()].push_back( Collisions() );
	//END push_back

	//START reference of vector to data A and B ready to used
	dataSetA = &(_collisionsOfNextTraversal[particleA.getGlobalParticleID()].back());
	dataSetB = &(_collisionsOfNextTraversal[particleB.getGlobalParticleID()].back());
	//END

	//START add copy of master and slave particles to sets (dual contact reference)
	dataSetA->_copyOfPartnerParticle = particleB;
	dataSetB->_copyOfPartnerParticle = particleA;
	//END
  }
  ////////END
*/
  /*
   * Problem here was:
   * Although all normals were pointing to opposite direction for each particle due to how we loop particles
   * A. at each vertex or grid initial contact direction is interchangeable depending on what is A and B
   * B. we always want to have the normal for particles to the position update direction (inner-direction)
   * C. we always want to ensure that normal of particle point away from obstacle
   * D. we don't care about normal of obstacle.
   */
/*
  dataSetA->_contactPoints.insert(dataSetA->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());

  for (std::vector<delta::contact::contactpoint>::iterator p = newContactPoints.begin(); p != newContactPoints.end(); p++)
  {
	//invert normal for particle B
	p->normal[0] = -1.0 * p->normal[0];
	p->normal[1] = -1.0 * p->normal[1];
	p->normal[2] = -1.0 * p->normal[2];
  }

  dataSetB->_contactPoints.insert(dataSetB->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());
  */
}

delta::core::State delta::core::Engine::getState()
{
  return _state;
}

int delta::core::Engine::getNumberOfCollisions()
{
  return _activeCollisions.size();
}

void delta::core::Engine::contactDetection()
{
  /*for(unsigned j=0; j<_data.getSize(); j++)
  {
	delta::core::data::ParticleRecord particleA = _data.getObject(j);

	//printf("triangles %i\n", particleA.getNumberOfTriangles());

	for(unsigned k=0; k<_data.getSize(); k++)
	{
	  delta::core::data::ParticleRecord particleB = _data.getObject(k);

	  if(_overlapCheck)
	  {
		bool overlap = delta::contact::detection::isSphereOverlayInContact(
		particleA->_centre[0],
		particleA._centre[1],
		particleA._centre[2],
		particleA.getHaloDiameter(),

		particleB._centre[0],
		particleB._centre[1],
		particleB._centre[2],
		particleB.getHaloDiameter());

		if (!overlap) continue;
	  }

	  std::vector<delta::contact::contactpoint> newContactPoints;

	  switch (_collisionModel) {
		case CollisionModel::Sphere:
		if(
		  particleA.getIsObstacle() &&
		  !particleB.getIsObstacle()
		) {
		  newContactPoints = delta::contact::detection::sphereWithBarrierBA(
					particleB._centre[0],
					particleB._centre[1],
					particleB._centre[2],
					particleB.getDiameter(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleID(),

					particleA._xCoordinates.data(),
					particleA._yCoordinates.data(),
					particleA._zCoordinates.data(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleID());
		} else if (
		  !particleA.getIsObstacle() &&
		   particleB.getIsObstacle()
		) {
		  newContactPoints = delta::contact::detection::sphereWithBarrierAB(
					particleA._centre[0],
					particleA._centre[1],
					particleA._centre[2],
					particleA.getDiameter(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleID(),

					particleB._xCoordinates.data(),
					particleB._yCoordinates.data(),
					particleB._zCoordinates.data(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleID());
		}
		else {
		  newContactPoints = delta::contact::detection::sphere(
					particleA._centre[0],
					particleA._centre[1],
					particleA._centre[2],
					particleA.getDiameter(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleID(),

					particleB._centre[0],
					particleB._centre[1],
					particleB._centre[2],
					particleB.getDiameter(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleID());
		}
		break;
		case CollisionModel::BruteForce:

		newContactPoints = delta::contact::detection::bf(
					particleA._xCoordinates.data(),
					particleA._yCoordinates.data(),
					particleA._zCoordinates.data(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleID(),

					particleB._xCoordinates.data(),
					particleB._yCoordinates.data(),
					particleB._zCoordinates.data(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleID());

		break;
		case CollisionModel::Penalty:

		newContactPoints = delta::contact::detection::penalty(
					particleA._xCoordinates.data(),
					particleA._yCoordinates.data(),
					particleA._zCoordinates.data(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleID(),

					particleB._xCoordinates.data(),
					particleB._yCoordinates.data(),
					particleB._zCoordinates.data(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleID());

		break;
		case CollisionModel::HybridOnBatches:

		newContactPoints = delta::contact::detection::hybridWithPerBatchFallBack(
					particleA._xCoordinates.data(),
					particleA._yCoordinates.data(),
					particleA._zCoordinates.data(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleID(),

					particleB._xCoordinates.data(),
					particleB._yCoordinates.data(),
					particleB._zCoordinates.data(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleID());

		break;
		case CollisionModel::HybridOnTrianglePairs:

		newContactPoints = delta::contact::detection::hybridWithPerTriangleFallBack(
					particleA._xCoordinates.data(),
					particleA._yCoordinates.data(),
					particleA._zCoordinates.data(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleID(),

					particleB._xCoordinates.data(),
					particleB._yCoordinates.data(),
					particleB._zCoordinates.data(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleID());

		break;
		case CollisionModel::none:
		break;
	  }

	  if(!newContactPoints.empty()) {
		delta::core::Engine::addCollision(newContactPoints, particleA, particleB, _collisionModel == CollisionModel::Sphere);
	  }

	  _state.incNumberOfTriangleComparisons(particleA.getNumberOfTriangles() * particleB.getNumberOfTriangles());
	  _state.incNumberOfParticleComparisons(1);
	}
  }*/
}

void delta::core::Engine::deriveForces()
{
  for(int i=0; i<_data.getNumberOfParticles(); i++)
  {
	delta::core::data::ParticleRecord& particle = _data.getParticle(i);

	//if value doesn't exist in map - no collision - skip particle
	if(_activeCollisions.count(particle.getGlobalParticleID())==0) {continue;}

	iREAL force[3]  = {0.0,0.0,0.0};
	iREAL torque[3] = {0.0,0.0,0.0};

	//collisions with partner particles
	for(std::vector<Collisions>::iterator p = _activeCollisions[particle.getGlobalParticleID()].begin();
						p != _activeCollisions[particle.getGlobalParticleID()].end();
						p++)
	{
	  iREAL rforce[3]  = {0.0,0.0,0.0};
	  iREAL rtorque[3] = {0.0,0.0,0.0};

	  delta::contact::forces::getContactsForces(
					 p->_contactPoints,
					 particle._centreOfMass.data(),
					 particle._refCentreOfMass.data(),
					 particle._angularVelocity.data(),
					 particle._refAngularVelocity.data(),
					 particle._linearVelocity.data(),
					 particle.getMass(),
					 particle.getInverse().data(),
					 particle._orientation.data(),
					 int(particle.getMaterial()),
					 p->_copyOfPartnerParticle._centreOfMass.data(),
					 p->_copyOfPartnerParticle._refCentreOfMass.data(),
					 p->_copyOfPartnerParticle._angularVelocity.data(),
					 p->_copyOfPartnerParticle._refAngularVelocity.data(),
					 p->_copyOfPartnerParticle._linearVelocity.data(),
					 p->_copyOfPartnerParticle.getMass(),
					 p->_copyOfPartnerParticle.getInverse().data(),
					 p->_copyOfPartnerParticle._orientation.data(),
					 int(p->_copyOfPartnerParticle.getMaterial()),
					 rforce, rtorque,
					 (_collisionModel == CollisionModel::Sphere));

	  force[0] += rforce[0];
	  force[1] += rforce[1];
	  force[2] += rforce[2];

	  torque[0] += rtorque[0];
	  torque[1] += rtorque[1];
	  torque[2] += rtorque[2];
	}

	if(!particle.getIsObstacle())
	{
	  particle._linearVelocity[0] += _dt * (force[0] / particle.getMass());
	  particle._linearVelocity[1] += _dt * (force[1] / particle.getMass());
	  particle._linearVelocity[2] += _dt * (force[2] / particle.getMass());

	  delta::dynamics::updateAngular(particle._refAngularVelocity.data(),
									particle._orientation.data(),
									particle.getInertia().data(),
									particle.getInverse().data(),
									torque, _dt);
	}
  }
}

void delta::core::Engine::updatePosition()
{
  for(unsigned i=0; i<_data.getNumberOfParticles(); i++)
  {
	delta::core::data::ParticleRecord& particle = _data.getParticle(i);

	if(particle.getIsObstacle()) continue;

	particle._linearVelocity[1] += _dt*(int(_gravity)*-9.8); //pass as gravity gxgygz vector

	particle._centre[0] += _dt*particle._linearVelocity[0];
	particle._centre[1] += _dt*particle._linearVelocity[1];
	particle._centre[2] += _dt*particle._linearVelocity[2];

	particle._centreOfMass[0] += _dt*particle._linearVelocity[0];
	particle._centreOfMass[1] += _dt*particle._linearVelocity[1];
	particle._centreOfMass[2] += _dt*particle._linearVelocity[2];

	//printf("dt:%f , %f %f %f\n", dt, particle._centreOfMass[0], particle._centreOfMass[1], particle._centreOfMass[2]);

	delta::dynamics::updateRotationMatrix(particle._angularVelocity.data(),
										  particle._refAngularVelocity.data(),
										  particle._orientation.data(), _dt);

	#pragma omp parallel for
	for(int j=0; j<particle.getNumberOfTriangles()*3; j++)
	{

	  delta::dynamics::updateVertices(&particle._xCoordinates.data()[j],
									  &particle._yCoordinates.data()[j],
									  &particle._zCoordinates.data()[j],
									  &particle._refxCoordinates.data()[j],
									  &particle._refxCoordinates.data()[j],
									  &particle._refxCoordinates.data()[j],
									  particle._orientation.data(),
									  particle._centreOfMass.data(),
									  particle._refCentreOfMass.data());
	}
  }
}

std::vector<delta::core::data::ParticleRecord>& delta::core::Engine::getParticleRecords()
{
  return _data.getAll();
}
