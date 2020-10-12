#include <geometry/shape/cube.h>
#include <geometry/shape/granulate.h>
#include <geometry/shape/graphite.h>
#include <geometry/shape/hopper.h>
#include "../structure/Object.h"

#include <stdio.h>
#include "../../core/algo.h"

delta::world::structure::Object::Object()
{
}

delta::world::structure::Object::Object(
  std::string                   						component,
  int                           						particleID,
  std::array<iREAL, 3>         							centre,
  delta::geometry::material::MaterialType 	material,
  bool                          						isObstacle,
  bool                          						isFriction,
	bool                          						isConvex,
	iREAL 																		epsilon,
	std::array<iREAL, 3> 											linear,
	std::array<iREAL, 3> 											angular)
{
  _component = component;
  _globalParticleID = particleID;

  _centre[0] = centre[0];
  _centre[1] = centre[1];
  _centre[2] = centre[2];

  _material 		= material;
  _isObstacle 	= isObstacle;
  _isFriction		= isFriction;

  _linearVelocity[0] = linear[0];
  _linearVelocity[1] = linear[1];
  _linearVelocity[2] = linear[2];

  _angularVelocity[0] = angular[0];
  _angularVelocity[1] = angular[1];
  _angularVelocity[2] = angular[2];

  _epsilon 					= epsilon;
  _haloDiameter 		= (_diameter+epsilon*2) * 1.1;
  _diameter					= _rad/2;
  _rad 							= _diameter/2;

  _localParticleID 	= 0;
  _isConvex 				= isConvex;

  _mesh	= nullptr;
  _mass	= 0;

  _wx = 0;
  _wy = 0;
  _wz = 0;
}

delta::world::structure::Object::Object(
	std::string                   						component,
	int                           						particleID,
	delta::geometry::mesh::Mesh*							mesh,
	std::array<iREAL, 3>         							centre,
	delta::geometry::material::MaterialType 	material,
	bool                          						isObstacle,
	bool                          						isFriction,
	bool                          						isConvex,
	iREAL 																		epsilon,
	std::array<iREAL, 3> 											linear,
	std::array<iREAL, 3> 											angular)
{
  _component = component;
  _globalParticleID = particleID;
  _localParticleID = 0;

  _isConvex = isConvex;

  _centre[0] = centre[0];
  _centre[1] = centre[1];
  _centre[2] = centre[2];

  _material		= material;
  _isObstacle 	= isObstacle;
  _isFriction 	= isFriction;

  _epsilon = epsilon;

  _linearVelocity[0] = linear[0];
  _linearVelocity[1] = linear[1];
  _linearVelocity[2] = linear[2];

  _angularVelocity[0] = angular[0];
  _angularVelocity[1] = angular[1];
  _angularVelocity[2] = angular[2];

  _mesh	= mesh;

  iREAL po[3] = {centre[0], centre[1], centre[2]};
  mesh->moveMeshToPosition(po);

  iREAL mass, centerOfMass[3], inertia[9], inverse[9];

  mesh->computeInertia(_material, mass, centerOfMass, inertia);
  mesh->computeInverseInertia(inertia, inverse, _isObstacle);

  _inertia[0] = inertia[0];
  _inertia[1] = inertia[1];
  _inertia[2] = inertia[2];
  _inertia[3] = inertia[3];
  _inertia[4] = inertia[4];
  _inertia[5] = inertia[5];
  _inertia[6] = inertia[6];
  _inertia[7] = inertia[7];
  _inertia[8] = inertia[8];

  _inverse[0] = inertia[0];
  _inverse[1] = inertia[1];
  _inverse[2] = inertia[2];
  _inverse[3] = inertia[3];
  _inverse[4] = inertia[4];
  _inverse[5] = inertia[5];
  _inverse[6] = inertia[6];
  _inverse[7] = inertia[7];
  _inverse[8] = inertia[8];

  _centreOfMass[0] = centerOfMass[0];
  _centreOfMass[1] = centerOfMass[1];
  _centreOfMass[2] = centerOfMass[2];

  _diameter				= mesh->computeDiameter();
  _rad						= _diameter/2;
  _haloDiameter 	= (_diameter+epsilon*2) * 1.1;
  _mass						= mass;

  _minBoundBox 	= mesh->getBoundaryMinVertex();
  _maxBoundBox 	= mesh->getBoundaryMinVertex();

  //dimensions
  _wx = 0;
  _wy = 0;
  _wz = 0;
}

//sphere object
delta::world::structure::Object::Object(
	std::string                   						component,
	iREAL																			rad,
	int                           						particleID,
	std::array<iREAL, 3>         							centre,
	delta::geometry::material::MaterialType 	material,
	bool                          						isObstacle,
	bool                          						isFriction,
	bool                          						isConvex,
	iREAL 																		epsilon,
	std::array<iREAL, 3> 											linear,
	std::array<iREAL, 3> 											angular)
{
  _component 					= component;
  _rad								= rad;
  _globalParticleID 	= particleID;
  _localParticleID 		= 0;

  _centre[0] 	= centre[0];
  _centre[1] 	= centre[1];
  _centre[2] 	= centre[2];

  _centreOfMass[0] = _centre[0];
  _centreOfMass[1] = _centre[1];
  _centreOfMass[2] = _centre[2];

  _material				= material;
  _isObstacle 		= isObstacle;
  _isFriction 		= isFriction;
  _isFriction 		= isConvex;

  _epsilon			= epsilon;

  _linearVelocity[0] = linear[0];
  _linearVelocity[1] = linear[1];
  _linearVelocity[2] = linear[2];

  _angularVelocity[0] = angular[0];
  _angularVelocity[1] = angular[1];
  _angularVelocity[2] = angular[2];

  _haloDiameter 	= (_diameter+epsilon*2) * 1.1;
  _diameter				= rad*2;
  _mass						= 0;
  _isConvex 			= isConvex;
  _mesh						= nullptr;

  _minBoundBox 	= {centre[0] - _rad, centre[1] - _rad, centre[2] - _rad};
  _maxBoundBox 	= {centre[0] + _rad, centre[1] + _rad, centre[2] + _rad};

  //dimensions
  _wx = 0;
  _wy = 0;
  _wz = 0;
}

std::string delta::world::structure::Object::getComponent()
{
  return _component;
}

void delta::world::structure::Object::setParticleID(int id)
{
  _globalParticleID = id;
}

void delta::world::structure::Object::setMesh(
	  std::vector<iREAL> xCoordinates,
	  std::vector<iREAL> yCoordinates,
	  std::vector<iREAL> zCoordinates)
{
  _mesh = new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}

delta::geometry::mesh::Mesh delta::world::structure::Object::getMesh()
{
  return *_mesh;
}

bool delta::world::structure::Object::hasMesh()
{
  if (_mesh == nullptr) 
	{
		return false;
	}
	return true;
}

void delta::world::structure::Object::setMesh(
	  delta::geometry::mesh::Mesh& mesh)
{
  _mesh = &mesh;

  iREAL mass, centerOfMass[3], inertia[9], inverse[9];

  _mesh->computeInertia(_material, mass, centerOfMass, inertia);
  _mesh->computeInverseInertia(inertia, inverse, _isObstacle);

  _inertia[0] = inertia[0];
  _inertia[1] = inertia[1];
  _inertia[2] = inertia[2];
  _inertia[3] = inertia[3];
  _inertia[4] = inertia[4];
  _inertia[5] = inertia[5];
  _inertia[6] = inertia[6];
  _inertia[7] = inertia[7];
  _inertia[8] = inertia[8];

  _inverse[0] = inertia[0];
  _inverse[1] = inertia[1];
  _inverse[2] = inertia[2];
  _inverse[3] = inertia[3];
  _inverse[4] = inertia[4];
  _inverse[5] = inertia[5];
  _inverse[6] = inertia[6];
  _inverse[7] = inertia[7];
  _inverse[8] = inertia[8];

  _centreOfMass[0] = centerOfMass[0];
  _centreOfMass[1] = centerOfMass[1];
  _centreOfMass[2] = centerOfMass[2];

  _mass = mass;
}

iREAL delta::world::structure::Object::getHaloDiameter()
{
  return _haloDiameter;
}

int delta::world::structure::Object::getNumberOfTriangles()
{
  return (int)_mesh->getTriangleFaces().size();
}

int delta::world::structure::Object::getGlobalParticleId()
{
  return _globalParticleID;
}

int delta::world::structure::Object::getLocalParticleId()
{
  return _localParticleID;
}

iREAL delta::world::structure::Object::getDiameter()
{
  return _rad*2;
}

std::array<iREAL, 3> delta::world::structure::Object::getCentre()
{
  return _centre;
}

void delta::world::structure::Object::setCentre(iREAL centre[3])
{
  if(_mesh != nullptr)
  {
	_mesh->moveMeshToPosition(centre);
  }

  _centre[0] = centre[0];
  _centre[1] = centre[1];
  _centre[2] = centre[2];

  _centreOfMass[0] = _centre[0];
  _centreOfMass[1] = _centre[1];
  _centreOfMass[2] = _centre[2];
}

iREAL delta::world::structure::Object::getEpsilon()
{
  return _epsilon;
}

void delta::world::structure::Object::setEpsilon(iREAL epsilon)
{
  _epsilon = epsilon;
}

iREAL delta::world::structure::Object::getRad()
{
  return _rad;
}

void delta::world::structure::Object::setRad(iREAL rad)
{
  _minBoundBox 	= {_centre[0] - _rad, _centre[1] - _rad, _centre[2] - _rad};
  _maxBoundBox 	= {_centre[0] + _rad, _centre[1] + _rad, _centre[2] + _rad};

  _rad = rad;
  _haloDiameter 	= (_diameter+_epsilon*2) * 1.1;
  _diameter		= rad*2;
}

iREAL delta::world::structure::Object::getMass()
{
  return _mass;
}

void delta::world::structure::Object::setMass(iREAL mass)
{
  _mass = mass;
}

delta::geometry::material::MaterialType delta::world::structure::Object::getMaterial()
{
  return _material;
}

bool delta::world::structure::Object::getIsObstacle()
{
  return _isObstacle;
}

bool delta::world::structure::Object::getIsFriction()
{
  return _isFriction;
}

std::array<iREAL, 9> delta::world::structure::Object::getInertia()
{
  return _inertia;
}

void delta::world::structure::Object::setInertia(iREAL inertia[9])
{
  _inertia[0] = inertia[0];
  _inertia[1] = inertia[1];
  _inertia[2] = inertia[2];
  _inertia[3] = inertia[3];
  _inertia[4] = inertia[4];
  _inertia[5] = inertia[5];
  _inertia[6] = inertia[6];
  _inertia[7] = inertia[7];
  _inertia[8] = inertia[8];
}

std::array<iREAL, 9> delta::world::structure::Object::getInverse()
{
  return _inverse;
}

void delta::world::structure::Object::setInverse(iREAL inverse[9])
{
  _inverse[0] = inverse[0];
  _inverse[1] = inverse[1];
  _inverse[2] = inverse[2];
  _inverse[3] = inverse[3];
  _inverse[4] = inverse[4];
  _inverse[5] = inverse[5];
  _inverse[6] = inverse[6];
  _inverse[7] = inverse[7];
  _inverse[8] = inverse[8];
}

std::array<iREAL, 3> delta::world::structure::Object::getCentreOfMass()
{
  return _centreOfMass;
}

void delta::world::structure::Object::setCentreOfMass(iREAL centreOfMass[3])
{
  _centreOfMass[0] = centreOfMass[0];
  _centreOfMass[1] = centreOfMass[1];
  _centreOfMass[2] = centreOfMass[2];
}

void delta::world::structure::Object::setLinearVelocity(std::array<iREAL, 3>  linearVelocity)
{
  _linearVelocity[0] = linearVelocity[0];
  _linearVelocity[1] = linearVelocity[1];
  _linearVelocity[2] = linearVelocity[2];
}

void delta::world::structure::Object::setAngularVelocity(std::array<iREAL, 3>  angularVelocity)
{
  _angularVelocity[0] = angularVelocity[0];
  _angularVelocity[1] = angularVelocity[1];
  _angularVelocity[2] = angularVelocity[2];
}

std::array<iREAL, 3> delta::world::structure::Object::getLinearVelocity()
{
  return _linearVelocity;
}

std::array<iREAL, 3> delta::world::structure::Object::getAngularVelocity()
{
  return _angularVelocity;
}

iREAL delta::world::structure::Object::computeVolume()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;
  getMesh().flatten(xCoordinates, yCoordinates, zCoordinates);

  return getMesh().computeVolume();
}

/*
 * gets the inertia using simplex integration from solfec
 */
void delta::world::structure::Object::computeInertia(
	delta::geometry::material::MaterialType material,
	iREAL& mass,
	iREAL center[3],
	iREAL inertia[9])
{
  getMesh().computeInertia(material, mass, center, inertia);
  setInertia(inertia);
  setMass(mass);
}

iREAL delta::world::structure::Object::computeMass(
		delta::geometry::material::MaterialType material)
{
  iREAL mass = getMesh().computeMass(material);
  setMass(mass);
  return mass;
}

void delta::world::structure::Object::computeInverseInertia(
	iREAL inertia[9],
	iREAL inverse[9],
	bool 	isObject)
{
  delta::geometry::operators::physics::computeInverseInertia(inertia, inverse, isObject);
  setInverse(inverse);
}

bool delta::world::structure::Object::getIsConvex()
{
  return _isConvex;
}

std::array<iREAL, 3> delta::world::structure::Object::getMinBoundaryVertex()
{
  return _minBoundBox;
}

std::array<iREAL, 3> delta::world::structure::Object::getMaxBoundaryVertex()
{
  return _maxBoundBox;
}

delta::world::structure::Object::~Object() {

}


