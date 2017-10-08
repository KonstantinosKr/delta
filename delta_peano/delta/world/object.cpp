
#include "object.h"
#include <stdio.h>

delta::world::object::object(
    std::string                   component,
    int                           particleID,
    std::array<double, 3>         centre,
    std::array<double, 3>         linearVelocity,
    std::array<double, 3>         angularVelocity,
    delta::geometry::material::MaterialType material,
    bool                          isObstacle,
    bool                          isFriction)
{

  this->_component = component;
  this->_particleID = particleID;

  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_linearVelocity[0] = linearVelocity[0];
  this->_linearVelocity[1] = linearVelocity[1];
  this->_linearVelocity[2] = linearVelocity[2];

  this->_angularVelocity[0] = angularVelocity[0];
  this->_angularVelocity[1] = angularVelocity[1];
  this->_angularVelocity[2] = angularVelocity[2];

  this->_material = material;
  this->_isObstacle = isObstacle;
  this->_isFriction = isFriction;
}

void delta::world::object::generateSphere(
    double                        rad)
{
  this->_rad = rad;
  this->_centreOfMass[0] = _centre[0];
  this->_centreOfMass[1] = _centre[1];
  this->_centreOfMass[2] = _centre[2];
}

void delta::world::object::generateMesh(
    double wx,
    double wy,
    double wz,
    double rx,
    double ry,
    double rz,
    int mesh,
    double rad)
{
  double mass;
  double centerOfMass[3];
  double inertia[9];
  double inverse[9];

  std::vector<double> xCoordinates;
  std::vector<double> yCoordinates;
  std::vector<double> zCoordinates;

  if(_component == "granulate")
  {
    iREAL position[3] = {_centre[0], _centre[1], _centre[2]};
    delta::geometry::granulates::generateParticle(position, (this->_rad*2), xCoordinates, yCoordinates, zCoordinates, mesh);
  } else if(_component == "cube")
  {
    iREAL position[3] = {_centre[0], _centre[1], _centre[2]};
    delta::geometry::cubes::generateHullCube(position, wx, wy, wz, rx, ry, rz, mesh, xCoordinates, yCoordinates, zCoordinates);
  }

  delta::geometry::properties::getInertia(xCoordinates, yCoordinates, zCoordinates, _material, mass, centerOfMass, inertia);
  delta::geometry::properties::getInverseInertia(inertia, inverse, _isObstacle);


  for(int i=0;i<xCoordinates.size(); i++)
  {
    this->_xCoordinates.push_back(xCoordinates[i]);
    this->_yCoordinates.push_back(yCoordinates[i]);
    this->_zCoordinates.push_back(zCoordinates[i]);
  }


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


std::string delta::world::object::getComponent()
{
  return _component;
}

std::vector<double> delta::world::object::getxCoordinates()
{
  return _xCoordinates;
}

std::vector<double> delta::world::object::getyCoordinates()
{
  return _yCoordinates;
}

std::vector<double> delta::world::object::getzCoordinates()
{
  return _zCoordinates;
}

int delta::world::object::getParticleID()
{
  return _particleID;
}

std::array<double, 3> delta::world::object::getCentre()
{
  return _centre;
}

double delta::world::object::getRad()
{
  return _rad;
}

double delta::world::object::getMass()
{
  return _mass;
}

delta::geometry::material::MaterialType delta::world::object::getMaterial()
{
  return _material;
}

bool delta::world::object::getIsObstacle()
{
  return _isObstacle;
}

bool delta::world::object::getIsFriction()
{
  return _isFriction;
}

std::array<double, 9> delta::world::object::getInertia()
{
  return _inertia;
}

std::array<double, 9> delta::world::object::getInverse()
{
  return _inverse;
}

std::array<double, 3> delta::world::object::getxyzDimensions()
{
  return _xyzDimensions;
}

std::array<double, 3> delta::world::object::getCentreOfMass()
{
  return _centreOfMass;
}

std::array<double, 3> delta::world::object::getLinearVelocity()
{
  return _linearVelocity;
}

std::array<double, 3> delta::world::object::getAngularVelocity()
{
  return _angularVelocity;
}

delta::world::object::~object() {

}

