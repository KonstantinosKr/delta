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
	CollisionModel collisionModel)
{
  _overlapCheck = overlapCheck;
  //_state = delta::core::State::State();
  _plot = plot;
  _collisionModel = collisionModel;
}

delta::core::Engine::~Engine()
{

}

void delta::core::Engine::iterate()
{
  std::array<iREAL, 6> boundary;
  boundary[0] = 0.0;
  boundary[1] = 0.0;
  boundary[2] = 0.0;

  boundary[3] = 1.0;
  boundary[4] = 1.0;
  boundary[5] = 1.0;


  if(_plot)
  {
	//delta::core::writeGeometryToVTK(_state.getIteration(), boundary, _data.getAll());
  }

   delta::core::Engine::contactDetection();
   //delta::core::Engine::deriveForces(dt);
   //delta::core::Engine::updatePosition(dt, _gravity);
}

void delta::core::Engine::addCollision(
	std::vector<delta::contact::contactpoint> & newContactPoints,
	delta::geometry::Object&                    particleA,
	delta::geometry::Object&                    particleB,
	bool sphere
) {

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

  if((dataSetA!=nullptr && dataSetB!=nullptr)) {
	newContactPoints.clear();
	return;
  }
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

  /*
   * Problem here was:
   * Although all normals were pointing to opposite direction for each particle due to how we loop particles
   * A. at each vertex or grid initial contact direction is interchangeable depending on what is A and B
   * B. we always want to have the normal for particles to the position update direction (inner-direction)
   * C. we always want to ensure that normal of particle point away from obstacle
   * D. we don't care about normal of obstacle.
   */

  dataSetA->_contactPoints.insert(dataSetA->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());

  for (std::vector<delta::contact::contactpoint>::iterator p = newContactPoints.begin(); p != newContactPoints.end(); p++)
  {
	//invert normal for particle B
	p->normal[0] = -1.0 * p->normal[0];
	p->normal[1] = -1.0 * p->normal[1];
	p->normal[2] = -1.0 * p->normal[2];
  }

  dataSetB->_contactPoints.insert(dataSetB->_contactPoints.end(), newContactPoints.begin(), newContactPoints.end());
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
  for(unsigned j=0; j<_data.getSize(); j++)
  {
	delta::geometry::Object particleA = _data.getObject(j);

	//printf("triangles %i\n", particleA.getNumberOfTriangles());

	for(unsigned k=0; k<_data.getSize(); k++)
	{
	  delta::geometry::Object particleB = _data.getObject(k);

	  if(_overlapCheck)
	  {
		bool overlap = delta::contact::detection::isSphereOverlayInContact(
		particleA.getCentre()[0],
		particleA.getCentre()[1],
		particleA.getCentre()[2],
		particleA.getHaloDiameter(),

		particleB.getCentre()[0],
		particleB.getCentre()[1],
		particleB.getCentre()[2],
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
					particleB.getCentre()[0],
					particleB.getCentre()[1],
					particleB.getCentre()[2],
					particleB.getDiameter(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleId(),

					particleA.getMesh().getXCoordinates(),
					particleA.getMesh().getYCoordinates(),
					particleA.getMesh().getZCoordinates(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleId());
		} else if (
		  !particleA.getIsObstacle() &&
		   particleB.getIsObstacle()
		) {
		  newContactPoints = delta::contact::detection::sphereWithBarrierAB(
					particleA.getCentre()[0],
					particleA.getCentre()[1],
					particleA.getCentre()[2],
					particleA.getDiameter(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleId(),

					particleB.getMesh().getXCoordinates(),
					particleB.getMesh().getYCoordinates(),
					particleB.getMesh().getZCoordinates(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleId());
		}
		else {
		  newContactPoints = delta::contact::detection::sphere(
					particleA.getCentre()[0],
					particleA.getCentre()[1],
					particleA.getCentre()[2],
					particleA.getDiameter(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleId(),

					particleB.getCentre()[0],
					particleB.getCentre()[1],
					particleB.getCentre()[2],
					particleB.getDiameter(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleId());
		}
		break;
		case CollisionModel::BruteForce:

		newContactPoints = delta::contact::detection::bf(
					particleA.getMesh().getXCoordinates(),
					particleA.getMesh().getYCoordinates(),
					particleA.getMesh().getZCoordinates(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleId(),

					particleB.getMesh().getXCoordinates(),
					particleB.getMesh().getYCoordinates(),
					particleB.getMesh().getZCoordinates(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleId());

		break;
		case CollisionModel::Penalty:

		newContactPoints = delta::contact::detection::penalty(
					particleA.getMesh().getXCoordinates(),
					particleA.getMesh().getYCoordinates(),
					particleA.getMesh().getZCoordinates(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleId(),

					particleB.getMesh().getXCoordinates(),
					particleB.getMesh().getYCoordinates(),
					particleB.getMesh().getZCoordinates(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleId());

		break;
		case CollisionModel::HybridOnBatches:

		newContactPoints = delta::contact::detection::hybridWithPerBatchFallBack(
					particleA.getMesh().getXCoordinates(),
					particleA.getMesh().getYCoordinates(),
					particleA.getMesh().getZCoordinates(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleId(),

					particleB.getMesh().getXCoordinates(),
					particleB.getMesh().getYCoordinates(),
					particleB.getMesh().getZCoordinates(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleId());

		break;
		case CollisionModel::HybridOnTrianglePairs:

		newContactPoints = delta::contact::detection::hybridWithPerTriangleFallBack(
					particleA.getMesh().getXCoordinates(),
					particleA.getMesh().getYCoordinates(),
					particleA.getMesh().getZCoordinates(),
					particleA.getNumberOfTriangles(),
					particleA.getEpsilon(),
					particleA.getIsFriction(),
					particleA.getGlobalParticleId(),

					particleB.getMesh().getXCoordinates(),
					particleB.getMesh().getYCoordinates(),
					particleB.getMesh().getZCoordinates(),
					particleB.getNumberOfTriangles(),
					particleB.getEpsilon(),
					particleB.getIsFriction(),
					particleB.getGlobalParticleId());

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
  }
}

void delta::core::Engine::deriveForces(iREAL dt)
{

  for(int i=0; i<_data.getSize(); i++)
  {
	delta::geometry::Object  particle = _data.getObject(i);

	//if value doesn't exist in map - no collision - skip particle
	if(_activeCollisions.count(particle.getGlobalParticleId())==0) {continue;}

	iREAL force[3]  = {0.0,0.0,0.0};
	iREAL torque[3] = {0.0,0.0,0.0};

	//collisions with partner particles
	for(std::vector<Collisions>::iterator p = _activeCollisions[particle.getGlobalParticleId()].begin();
						p != _activeCollisions[particle.getGlobalParticleId()].end();
						p++)
	{
	  iREAL rforce[3]  = {0.0,0.0,0.0};
	  iREAL rtorque[3] = {0.0,0.0,0.0};
/*
	  delta::contact::forces::getContactsForces(p->_contactPoints,
					 &(particle.getCentreOfMass()),
					 &(particle.getRefCentreOfMass()),
					 &(particle.getAngularVelocity()),
					 &(particle.getRefAngularVelocity()),
					 &(particle.getLinearVelocity()),
					 particle.getMass(),
					 &(particle.getInverse()),
					 &(particle.getOrientation()),
					 particle.getMaterial(),
					 &(p->_copyOfPartnerParticle.getCentreOfMass()),
					 &(p->_copyOfPartnerParticle.getRefCentreOfMass()),
					 &(p->_copyOfPartnerParticle.getAngularVelocity()),
					 &(p->_copyOfPartnerParticle.getRefAngularVelocity()),
					 &(p->_copyOfPartnerParticle.getLinearVelocity()),
					 p->_copyOfPartnerParticle.getMass(),
					 &(p->_copyOfPartnerParticle.getInverse()),
					 &(p->_copyOfPartnerParticle.getOrientation()),
					 p->_copyOfPartnerParticle.getMaterial(),
					 rforce, rtorque,
					 (_collisionModel == CollisionModel::Sphere));*/

	  force[0] += rforce[0];
	  force[1] += rforce[1];
	  force[2] += rforce[2];

	  torque[0] += rtorque[0];
	  torque[1] += rtorque[1];
	  torque[2] += rtorque[2];
	}

	if(!particle.getIsObstacle())
	{
	  particle.getLinearVelocity()[0] += dt * (force[0] / particle.getMass());
	  particle.getLinearVelocity()[1] += dt * (force[1] / particle.getMass());
	  particle.getLinearVelocity()[2] += dt * (force[2] / particle.getMass());
/*
	  delta::dynamics::updateAngular(particle.getRefAngularVelocity(),
									particle.getOrientation(),
									particle.getInertia(),
									particle.getInverse(),
									particle.getMass(),
									torque, dt);*/
	}
  }

}

void delta::core::Engine::updatePosition(iREAL dt, bool gravity)
{
	for(unsigned i=0; i<_data.getSize(); i++)
	{
	  delta::geometry::Object  particle = _data.getObject(i);

	  if(particle.getIsObstacle()) continue;
/*
	  particle._linearVelocity(1) += dt*(int(gravity)*-9.8); //pass as gravity gxgygz vector

	  particle._centre(0) += dt*particle._linearVelocity(0);
	  particle._centre(1) += dt*particle._linearVelocity(1);
	  particle._centre(2) += dt*particle._linearVelocity(2);

	  particle._centreOfMass(0) += dt*particle._linearVelocity(0);
	  particle._centreOfMass(1) += dt*particle._linearVelocity(1);
	  particle._centreOfMass(2) += dt*particle._linearVelocity(2);
*/
	  //delta::dynamics::updateRotationMatrix(&particle.getAngularVelocity(0),
		//									&particle.getRefAngularVelocity(0),
		//									&particle._orientation(0), dt);

	  iREAL* x = particle.getMesh().getXCoordinates();
	  iREAL* y = particle.getMesh().getYCoordinates();
	  iREAL* z = particle.getMesh().getZCoordinates();

	  //iREAL* refx = particle.getMesh().getXRefCoordinates();
	  //iREAL* refy = particle.getMesh().getYRefCoordinates(i);
	  //iREAL* refz = particle.getMesh().getZRefCoordinates(i);

	  /*for(int j=0; j<particle.getNumberOfTriangles()*3; j++)
	  {
		delta::dynamics::updateVertices(&x[j], &y[j], &z[j], &refx[j], &refy[j], &refz[j],
										&particle._orientation(0),
										&particle._centreOfMass(0),
										&particle._refCentreOfMass(0));
	  }*/
	}
}
