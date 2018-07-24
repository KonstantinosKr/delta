/*
 * ParticleRecord.h
 *
 *  Created on: 27 Jun 2018
 *      Author: konstantinos
 */

#ifndef CORE_DATA_PARTICLERECORD_H_
#define CORE_DATA_PARTICLERECORD_H_

#include <array>
#include <string>
#include <vector>
#include <delta/geometry/material.h>
#include <delta/geometry/Object.h>
#include <delta/core/data/OctTree.h>

namespace delta {
  namespace core {
	namespace data {

	  class ParticleRecord;

	}
  }
} /* namespace delta */



class delta::core::data::ParticleRecord
{
  public:
	ParticleRecord(delta::geometry::Object& object);

	int getGlobalParticleID();
	int getLocalParticleID();
	iREAL getHaloDiameter();
	iREAL getRad();
	iREAL getDiameter();
	iREAL getMass();
	iREAL getEpsilon();
	delta::geometry::material::MaterialType getMaterial();
	bool getIsObstacle();
	bool getIsFriction();
	bool getIsConvex();
	std::array<iREAL, 9> getInertia();
	std::array<iREAL, 9> getInverse();
	int getNumberOfTriangles();

	std::vector<iREAL> getClosestXCoordinatesTriangles(iREAL x[3]);
	std::vector<iREAL> getClosestYCoordinatesTriangles(iREAL x[3]);
	std::vector<iREAL> getClosestZCoordinatesTriangles(iREAL x[3]);

	delta::core::data::OctTree& getTree();
	void refineTree(double maxMeshSize);

	void getSubsetOfMesh(
		  double x[3], double epsilon,
		  std::vector<iREAL> &xCoordinatesPartial,
		  std::vector<iREAL> &yCoordinatesPartial,
		  std::vector<iREAL> &zCoordinatesPartial);

	iREAL getMaxMeshSize();
	iREAL getMinMeshSize();
	iREAL getAvgMeshSize();

	virtual ~ParticleRecord();

	std::array<iREAL, 3> 	_linearVelocity;
	std::array<iREAL, 3> 	_angularVelocity;
	std::array<iREAL, 3> 	_refAngularVelocity;

	std::array<iREAL, 3> 	_centre;
	std::array<iREAL, 3> 	_centreOfMass;
	std::array<iREAL, 3> 	_refCentreOfMass;
	std::array<iREAL, 9> 	_orientation;

	std::vector<iREAL>   	_xCoordinates;
	std::vector<iREAL>   	_yCoordinates;
	std::vector<iREAL>   	_zCoordinates;

	std::vector<iREAL>   	_refxCoordinates;
	std::vector<iREAL>   	_refyCoordinates;
	std::vector<iREAL>   	_refzCoordinates;

  private:
	int                   	_globalParticleID;
	int                  	_localParticleID;

	iREAL                	_rad;
	iREAL				 	_haloDiameter;
	iREAL				 	_diameter;
	iREAL               		_mass;
	iREAL 				 	_epsilon;
	iREAL					_maxMeshSize;
	iREAL 					_minMeshSize;
	iREAL 					_avgMeshSize;

	delta::geometry::material::MaterialType _material;

	bool                  	_isObstacle;
	bool                  	_isFriction;
	bool                  	_isConvex;

	std::array<iREAL, 9> 	_inertia;
	std::array<iREAL, 9> 	_inverse;

	delta::core::data::OctTree _tree;
	std::array<iREAL,6> _bbox;
};

#endif /* DELTA_CORE_DATA_PARTICLERECORD_H_ */
