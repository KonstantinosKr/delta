
#include "object.h"
#include <stdio.h>

delta::world::object::object(
    std::string                   component,
    int                           particleID,
    std::array<double, 3>         centre,
    delta::geometry::material::MaterialType material,
    bool                          isObstacle,
    bool                          isFriction)
{

  this->_component = component;
  this->_particleID = particleID;

  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_material = material;
  this->_isObstacle = isObstacle;
  this->_isFriction = isFriction;

  this->_linearVelocity[0] = 0.0;
  this->_linearVelocity[1] = 0.0;
  this->_linearVelocity[2] = 0.0;

  this->_angularVelocity[0] = 0.0;
  this->_angularVelocity[1] = 0.0;
  this->_angularVelocity[2] = 0.0;

  //this->_rad = 0.0001;
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

  iREAL position[3] = {_centre[0], _centre[1], _centre[2]};
  if(_component == "granulate")
  {
    delta::geometry::primitive::granulate::generateParticle(position, rad*2, xCoordinates, yCoordinates, zCoordinates, mesh);
  } else if(_component == "granulate-load")
  {
    delta::geometry::primitive::granulate::loadParticle(position, (rad*2), xCoordinates, yCoordinates, zCoordinates);
  } else if(_component == "cube")
  {
    delta::geometry::primitive::cube::generateHullCube(position, wx, wy, wz, rx, ry, rz, mesh, xCoordinates, yCoordinates, zCoordinates);
  }
  else if(_component == "hopper")
  {
    double _hopperHatch = 0.05; double _hopperThickness = 0.005; int refinement = 0;
    delta::geometry::primitive::hopper::generateHopper(position, wx, _hopperThickness, wy, _hopperHatch, refinement, 0.01, xCoordinates, yCoordinates, zCoordinates);
  } else if(_component == "FB")
  {
    delta::geometry::primitive::graphite::generateBrickFB(position, rad, xCoordinates, yCoordinates, zCoordinates);
  } else if(_component == "mesh")
  {

  }

  delta::geometry::properties::getInertia(xCoordinates, yCoordinates, zCoordinates, _material, mass, centerOfMass, inertia);
  delta::geometry::properties::getInverseInertia(inertia, inverse, _isObstacle);


  for(unsigned i=0;i<xCoordinates.size(); i++)
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

void delta::world::object::setParticleID(int id)
{
  _particleID = id;
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

void delta::world::object::setxyzCoordinates(
    std::vector<double> xCoordinates,
    std::vector<double> yCoordinates,
    std::vector<double> zCoordinates)
{
  for(unsigned i=0; i< xCoordinates.size(); i++)
  {
    _xCoordinates.push_back(xCoordinates[i]);
    _yCoordinates.push_back(yCoordinates[i]);
    _zCoordinates.push_back(zCoordinates[i]);
  }
}

int delta::world::object::getParticleID()
{
  return _particleID;
}

std::array<double, 3> delta::world::object::getCentre()
{
  return _centre;
}

void delta::world::object::setCentre(double centre[3])
{
  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_centreOfMass[0] = _centre[0];
  this->_centreOfMass[1] = _centre[1];
  this->_centreOfMass[2] = _centre[2];
}

double delta::world::object::getRad()
{
  return _rad;
}

void delta::world::object::setRad(double rad)
{
  _rad = rad;
}

double delta::world::object::getMass()
{
  return _mass;
}

void delta::world::object::setMass(double mass)
{
  _mass = mass;
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

void delta::world::object::setInertia(double inertia[9])
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

std::array<double, 9> delta::world::object::getInverse()
{
  return _inverse;
}

void delta::world::object::setInverse(double inverse[9])
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

std::array<double, 3> delta::world::object::getxyzDimensions()
{
  return _xyzDimensions;
}

std::array<double, 3> delta::world::object::getCentreOfMass()
{
  return _centreOfMass;
}

void delta::world::object::setCentreOfMass(double centreOfMass[3])
{
  _centreOfMass[0] = centreOfMass[0];
  _centreOfMass[1] = centreOfMass[1];
  _centreOfMass[2] = centreOfMass[2];
}

void delta::world::object::setLinearVelocity(std::array<double, 3>  linearVelocity)
{
  this->_linearVelocity[0] = linearVelocity[0];
  this->_linearVelocity[1] = linearVelocity[1];
  this->_linearVelocity[2] = linearVelocity[2];
}

void delta::world::object::setAngularVelocity(std::array<double, 3>  angularVelocity)
{
  this->_angularVelocity[0] = angularVelocity[0];
  this->_angularVelocity[1] = angularVelocity[1];
  this->_angularVelocity[2] = angularVelocity[2];
}

std::array<double, 3> delta::world::object::getLinearVelocity()
{
  return _linearVelocity;
}

std::array<double, 3> delta::world::object::getAngularVelocity()
{
  return _angularVelocity;
}

double delta::world::object::getMinX()
{
  iREAL minx = std::numeric_limits<double>::max();
  if(_xCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_xCoordinates.size(); i++)
    {
      if(_xCoordinates[i] < minx) minx = _xCoordinates[i];
    }
  } else {
    minx = _centreOfMass[0] - _rad;
  }
  return minx;
}

double delta::world::object::getMaxX()
{
  iREAL maxx = std::numeric_limits<double>::min();
  if(_xCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_xCoordinates.size(); i++)
    {
      if(_xCoordinates[i] > maxx) maxx = _xCoordinates[i];
    }
  } else {
    maxx = _centreOfMass[0] + _rad;
  }
  return maxx;
}

double delta::world::object::getMinY()
{
  iREAL miny = std::numeric_limits<double>::max();
  if(_yCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_yCoordinates.size(); i++)
    {
      if(_yCoordinates[i] < miny) miny = _yCoordinates[i];
    }
  } else {
    miny = _centreOfMass[0] - _rad;
  }
  return miny;
}

double delta::world::object::getMaxY()
{
  iREAL maxy = std::numeric_limits<double>::min();
  if(_yCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_yCoordinates.size(); i++)
    {
      if(_xCoordinates[i] > maxy) maxy = _xCoordinates[i];
    }
  } else {
    maxy = _centreOfMass[0] + _rad;
  }
  return maxy;
}

double delta::world::object::getMinZ()
{
  iREAL minz = std::numeric_limits<double>::max();
  if(_xCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_xCoordinates.size(); i++)
    {
      if(_xCoordinates[i] < minz) minz = _zCoordinates[i];
    }
  } else {
    minz = _centreOfMass[0] - _rad;
  }
  return minz;
}

double delta::world::object::getMaxZ()
{
  iREAL maxz = std::numeric_limits<double>::min();
  if(_zCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_yCoordinates.size(); i++)
    {
      if(_xCoordinates[i] > maxz) maxz = _xCoordinates[i];
    }
  } else {
    maxz = _centreOfMass[0] + _rad;
  }
  return maxz;
}

delta::world::object::~object() {

}


