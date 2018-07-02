/*
 * ParticleRecord.cpp
 *
 *  Created on: 27 Jun 2018
 *      Author: konstantinos
 */

#include "delta/core/data/ParticleRecord.h"

delta::core::data::ParticleRecord::ParticleRecord(delta::geometry::Object& object)
{
  this->_linearVelocity = object.getLinearVelocity();
  this->_angularVelocity = object.getAngularVelocity();
  this->_refAngularVelocity = object.getAngularVelocity();

  this->_centre = object.getCentre();
  this->_centreOfMass = object.getCentreOfMass();
  this->_refCentreOfMass = object.getCentreOfMass();

  this->_orientation[0] = 1.0;
  this->_orientation[1] = 0;
  this->_orientation[2] = 0;
  this->_orientation[3] = 0;
  this->_orientation[4] = 1.0;
  this->_orientation[5] = 0;
  this->_orientation[6] = 0;
  this->_orientation[7] = 0;
  this->_orientation[8] = 1.0;

  this->_material = object.getMaterial();

  object.computeInertia(_material, this->_mass, _centreOfMass.data(), this->_inertia.data());

/*  printf("%f %f %f\n%f %f %f\n%f %f %f\n",
	  this->_inertia[0],
		  this->_inertia[1],
		  this->_inertia[2],
		  this->_inertia[3],
		  this->_inertia[4],
		  this->_inertia[5],
		  this->_inertia[6],
		  this->_inertia[7],
		  this->_inertia[8]);*/

  object.computeInverseInertia(this->_inertia.data(), this->_inverse.data(), object.getIsObstacle());
/*
  printf("%f %f %f\n%f %f %f\n%f %f %f\n",
	  this->_inverse[0],
		  this->_inverse[1],
		  this->_inverse[2],
		  this->_inverse[3],
		  this->_inverse[4],
		  this->_inverse[5],
		  this->_inverse[6],
		  this->_inverse[7],
		  this->_inverse[8]);*/

  this->_xCoordinates = object.getMesh().getXCoordinatesAsVector();
  this->_yCoordinates = object.getMesh().getYCoordinatesAsVector();
  this->_zCoordinates = object.getMesh().getZCoordinatesAsVector();

  this->_refxCoordinates = object.getMesh().getXCoordinatesAsVector();
  this->_refyCoordinates = object.getMesh().getYCoordinatesAsVector();
  this->_refzCoordinates = object.getMesh().getZCoordinatesAsVector();

  this->_globalParticleID = object.getGlobalParticleId();
  this->_localParticleID = object.getLocalParticleId();

  this->_rad = object.getRad();
  this->_haloDiameter = object.getHaloDiameter();
  this->_diameter = object.getDiameter();
  this->_epsilon = object.getEpsilon();

  this->_isObstacle = object.getIsObstacle();
  this->	_isFriction = object.getIsFriction();
  this->_isConvex = object.getIsConvex();
}

int delta::core::data::ParticleRecord::getGlobalParticleID()
{
  return _globalParticleID;
}

int delta::core::data::ParticleRecord::getLocalParticleID()
{
  return _localParticleID;
}

iREAL delta::core::data::ParticleRecord::getHaloDiameter()
{
  return _haloDiameter;
}

iREAL delta::core::data::ParticleRecord::getRad()
{
  return _rad;
}

iREAL delta::core::data::ParticleRecord::getDiameter()
{
  return _diameter;
}

iREAL delta::core::data::ParticleRecord::getMass()
{
  return _mass;
}

iREAL delta::core::data::ParticleRecord::getEpsilon()
{
  return _epsilon;
}

delta::geometry::material::MaterialType delta::core::data::ParticleRecord::getMaterial()
{
  return _material;
}

bool delta::core::data::ParticleRecord::getIsObstacle()
{
  return _isObstacle;
}

bool delta::core::data::ParticleRecord::getIsFriction()
{
  return _isFriction;
}

bool delta::core::data::ParticleRecord::getIsConvex()
{
  return _isConvex;
}

std::array<iREAL, 9> delta::core::data::ParticleRecord::getInertia()
{
  return _inertia;
}

std::array<iREAL, 9> delta::core::data::ParticleRecord::getInverse()
{
  return _inverse;
}

int delta::core::data::ParticleRecord::getNumberOfTriangles()
{
  return _xCoordinates.size()/3;
}

delta::core::data::ParticleRecord::~ParticleRecord() {
  // TODO Auto-generated destructor stub
}
