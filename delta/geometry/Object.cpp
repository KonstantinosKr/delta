#include <delta/geometry/Object.h>
#include <stdio.h>
#include <delta/core/algo.h>
#include <delta/geometry/primitive/granulate.h>
#include <delta/geometry/primitive/cube.h>
#include <delta/geometry/hardcoded/graphite.h>
#include <delta/geometry/hardcoded/hopper.h>

delta::geometry::Object::Object()
{
}

delta::geometry::Object::Object(
    std::string                   			component,
    int                           			particleID,
    std::array<iREAL, 3>         			centre,
    delta::geometry::material::MaterialType 	material,
    bool                          			isObstacle,
    bool                          			isFriction,
	iREAL 									epsilon,
	std::array<iREAL, 3> 					linear,
	std::array<iREAL, 3> 					angular)
{
  this->_component = component;
  this->_globalParticleID = particleID;

  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_material = material;
  this->_isObstacle = isObstacle;
  this->_isFriction = isFriction;

  this->_linearVelocity[0] = linear[0];
  this->_linearVelocity[1] = linear[1];
  this->_linearVelocity[2] = linear[2];

  this->_angularVelocity[0] = angular[0];
  this->_angularVelocity[1] = angular[1];
  this->_angularVelocity[2] = angular[2];

  this->_epsilon = epsilon;
}

delta::geometry::Object::Object(
std::string                   			component,
int                           			particleID,
delta::geometry::mesh::Mesh*				mesh,
std::array<iREAL, 3>         			centre,
delta::geometry::material::MaterialType 	material,
bool                          			isObstacle,
bool                          			isFriction,
iREAL 									epsilon,
std::array<iREAL, 3> 					linear,
std::array<iREAL, 3> 					angular)
{
  this->_component = component;
  this->_globalParticleID = particleID;
  this->_localParticleID = 0;

  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_material = material;
  this->_isObstacle = isObstacle;
  this->_isFriction = isFriction;

  this->_epsilon = epsilon;

  this->_linearVelocity[0] = linear[0];
  this->_linearVelocity[1] = linear[1];
  this->_linearVelocity[2] = linear[2];

  this->_angularVelocity[0] = angular[0];
  this->_angularVelocity[1] = angular[1];
  this->_angularVelocity[2] = angular[2];

  this->_rad				= 	0;
  this->_haloDiameter 	= 	0;
  this->_diameter		=	0;
  this->_mass			=	0;
  this->_epsilon			=	0;

  this-> _mesh			= 	mesh;

  std::array<iREAL, 3> 	_refAngularVelocity;

  std::array<iREAL, 3> 	_centreOfMass;
  std::array<iREAL, 3> 	_refCentreOfMass;

  std::array<iREAL, 9> 	_inertia;
  std::array<iREAL, 9> 	_inverse;
  std::array<iREAL, 9> 	_orientation;

  //dimensions
  _wx = 0;
  _wy = 0;
  _wz = 0;
}


delta::geometry::Object::Object(
std::string                   			component,
iREAL									rad,
int                           			particleID,
std::array<iREAL, 3>         			centre,
delta::geometry::material::MaterialType 	material,
bool                          			isObstacle,
bool                          			isFriction,
iREAL 									epsilon,
std::array<iREAL, 3> 					linear,
std::array<iREAL, 3> 					angular)
{
  this->_component 			= component;
  this->_rad					= rad;
  this->_globalParticleID 	= particleID;
  this->_localParticleID 	= 0;

  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_centreOfMass[0] = _centre[0];
  this->_centreOfMass[1] = _centre[1];
  this->_centreOfMass[2] = _centre[2];

  this->_material 		= material;
  this->_isObstacle 		= isObstacle;
  this->_isFriction 		= isFriction;

  this->_epsilon 		= epsilon;

  this->_linearVelocity[0] = linear[0];
  this->_linearVelocity[1] = linear[1];
  this->_linearVelocity[2] = linear[2];

  this->_angularVelocity[0] = angular[0];
  this->_angularVelocity[1] = angular[1];
  this->_angularVelocity[2] = angular[2];

  this->_haloDiameter 	= 	0;
  this->_diameter		=	0;
  this->_mass			=	0;

  this-> _mesh			= 	nullptr;

  //dimensions
  _wx = 0;
  _wy = 0;
  _wz = 0;

}

void delta::geometry::Object::generateMesh(
		iREAL wx,
		iREAL wy,
		iREAL wz,
		iREAL rx,
		iREAL ry,
		iREAL rz,
		int 	  mesh,
		iREAL rad)
{
  iREAL position[3] = {_centre[0], _centre[1], _centre[2]};

  delta::geometry::mesh::Mesh *geometry;
  if(_component == "granulate")
  {
	geometry = delta::geometry::primitive::granulate::generateParticle(position, rad*2, mesh);
  } else if(_component == "granulate-load")
  {
	geometry = delta::geometry::primitive::granulate::loadParticle(position, (rad*2));
  } else if(_component == "cube")
  {
	geometry = delta::geometry::primitive::cube::generateHullCube(position, wx, wy, wz, rx, ry, rz, mesh);
  }
  else if(_component == "hopper")
  {
    iREAL _hopperHatch = 0.05; iREAL _hopperThickness = 0.005; int refinement = 0;
    geometry = delta::geometry::hardcoded::generateHopper(position, wx, _hopperThickness, wy, _hopperHatch, refinement, 0.01);
  } else if(_component == "FB")
  {
	geometry = delta::geometry::hardcoded::generateBrickFB(position, rad);
  }

  iREAL mass, centerOfMass[3], inertia[9], inverse[9];

  geometry->computeInertia(_material, mass, centerOfMass, inertia);
  geometry->computeInverseInertia(inertia, inverse, _isObstacle);

  _mesh = geometry;

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
  _rad = rad;
}

std::string delta::geometry::Object::getComponent()
{
  return _component;
}

void delta::geometry::Object::setParticleID(int id)
{
  _globalParticleID = id;
}

void delta::geometry::Object::setMesh(
    std::vector<iREAL> xCoordinates,
    std::vector<iREAL> yCoordinates,
    std::vector<iREAL> zCoordinates)
{
  _mesh = new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}

delta::geometry::mesh::Mesh delta::geometry::Object::getMesh()
{
  if(_component != "mesh")
  {
	//printf("error getMesh called while particle is sphere\n");
  }

  return *_mesh;
}

void delta::geometry::Object::setMesh(
    delta::geometry::mesh::Mesh& mesh)
{
  _mesh = &mesh;
}

iREAL delta::geometry::Object::getHaloDiameter()
{
  return _haloDiameter;
}

int delta::geometry::Object::getNumberOfTriangles()
{
  return (int)_mesh->getTriangleFaces().size();
}

int delta::geometry::Object::getGlobalParticleId()
{
  return _globalParticleID;
}

int delta::geometry::Object::getLocalParticleId()
{
  return _localParticleID;
}

iREAL delta::geometry::Object::getDiameter()
{
  return _rad*2;
}

std::array<iREAL, 3> delta::geometry::Object::getCentre()
{
  return _centre;
}

void delta::geometry::Object::setCentre(iREAL centre[3])
{
  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_centreOfMass[0] = _centre[0];
  this->_centreOfMass[1] = _centre[1];
  this->_centreOfMass[2] = _centre[2];
}

iREAL delta::geometry::Object::getEpsilon()
{
  return _epsilon;
}

void delta::geometry::Object::setEpsilon(iREAL epsilon)
{
  _epsilon = epsilon;
}

iREAL delta::geometry::Object::getRad()
{
  return _rad;
}

void delta::geometry::Object::setRad(iREAL rad)
{
  _rad = rad;
}

iREAL delta::geometry::Object::getMass()
{
  return _mass;
}

void delta::geometry::Object::setMass(iREAL mass)
{
  _mass = mass;
}

delta::geometry::material::MaterialType delta::geometry::Object::getMaterial()
{
  return _material;
}

bool delta::geometry::Object::getIsObstacle()
{
  return _isObstacle;
}

bool delta::geometry::Object::getIsFriction()
{
  return _isFriction;
}

std::array<iREAL, 9> delta::geometry::Object::getInertia()
{
  return _inertia;
}

void delta::geometry::Object::setInertia(iREAL inertia[9])
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

void delta::geometry::Object::setOrientation(iREAL orientation[9])
{
  _orientation[0] = orientation[0];
  _orientation[1] = orientation[1];
  _orientation[2] = orientation[2];
  _orientation[3] = orientation[3];
  _orientation[4] = orientation[4];
  _orientation[5] = orientation[5];
  _orientation[6] = orientation[6];
  _orientation[7] = orientation[7];
  _orientation[8] = orientation[8];
}

std::array<iREAL, 9>	delta::geometry::Object::getOrientation()
{
  return _orientation;
}

std::array<iREAL, 9> delta::geometry::Object::getInverse()
{
  return _inverse;
}

void delta::geometry::Object::setInverse(iREAL inverse[9])
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

std::array<iREAL, 3> delta::geometry::Object::getCentreOfMass()
{
  return _centreOfMass;
}

void delta::geometry::Object::setRefCentreOfMass(iREAL refCentreOfMass[3])
{
  _refCentreOfMass[0] = refCentreOfMass[0];
  _refCentreOfMass[1] = refCentreOfMass[1];
  _refCentreOfMass[2] = refCentreOfMass[2];
}

std::array<iREAL, 3> delta::geometry::Object::getRefCentreOfMass()
{
  return _refCentreOfMass;
}

void delta::geometry::Object::setCentreOfMass(iREAL centreOfMass[3])
{
  _centreOfMass[0] = centreOfMass[0];
  _centreOfMass[1] = centreOfMass[1];
  _centreOfMass[2] = centreOfMass[2];
}

void delta::geometry::Object::setLinearVelocity(std::array<iREAL, 3>  linearVelocity)
{
  this->_linearVelocity[0] = linearVelocity[0];
  this->_linearVelocity[1] = linearVelocity[1];
  this->_linearVelocity[2] = linearVelocity[2];
}

void delta::geometry::Object::setAngularVelocity(std::array<iREAL, 3>  angularVelocity)
{
  this->_angularVelocity[0] = angularVelocity[0];
  this->_angularVelocity[1] = angularVelocity[1];
  this->_angularVelocity[2] = angularVelocity[2];
}

void delta::geometry::Object::setRefAngularVelocity(std::array<iREAL, 3>  refAngularVelocity)
{
  this->_refAngularVelocity[0] = refAngularVelocity[0];
  this->_refAngularVelocity[1] = refAngularVelocity[1];
  this->_refAngularVelocity[2] = refAngularVelocity[2];
}

std::array<iREAL, 3> delta::geometry::Object::getLinearVelocity()
{
  return _linearVelocity;
}

std::array<iREAL, 3> delta::geometry::Object::getAngularVelocity()
{
  return _angularVelocity;
}

std::array<iREAL, 3> delta::geometry::Object::getRefAngularVelocity()
{
  return _refAngularVelocity;
}

iREAL delta::geometry::Object::computeVolume()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->getMesh().flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL vol = this->getMesh().computeVolume();

  return vol;
}

/*
 * gets the inertia using simplex integration from solfec
 */
void delta::geometry::Object::computeInertia(
		delta::geometry::material::MaterialType material,
		iREAL& mass,
		iREAL center[3],
		iREAL inertia[9])
{
  this->getMesh().computeInertia(material, mass, center, inertia);
  this->setInertia(inertia);
  this->setMass(mass);
}

iREAL delta::geometry::Object::computeMass(
    delta::geometry::material::MaterialType material)
{
  iREAL mass = this->getMesh().computeMass(material);

  this->setMass(mass);

  return mass;
}

void delta::geometry::Object::computeInverseInertia(
    iREAL inertia[9],
    iREAL inverse[9],
    bool isObject)
{
  delta::geometry::operators::physics::computeInverseInertia(inertia, inverse, isObject);

  this->setInverse(inverse);
}

delta::geometry::Object::~Object() {

}


