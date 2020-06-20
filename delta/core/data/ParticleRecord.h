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
#include "../../geometry/material.h"
#include "../../core/data/OctTree.h"
#include "../../world/structure/Object.h"

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
		ParticleRecord(delta::world::structure::Object& object);

		int 	getGlobalParticleID();
		int 	getLocalParticleID();
		int 	getNumberOfTriangles();
		bool 	getIsObstacle();
		bool 	getIsFriction();
		bool 	getIsConvex();
		bool 	getIsMesh();

		iREAL getHaloDiameter();
		iREAL getDiameter();
		iREAL getEpsilon();
		iREAL getRad();
		iREAL getMass();
		iREAL getMaxMeshSize();
		iREAL getMinMeshSize();
		iREAL getAvgMeshSize();

		std::vector<iREAL> getClosestXCoordinatesTriangles(iREAL x[3]);
		std::vector<iREAL> getClosestYCoordinatesTriangles(iREAL x[3]);
		std::vector<iREAL> getClosestZCoordinatesTriangles(iREAL x[3]);

		delta::core::data::OctTree& getTree();
		void refineTree(iREAL maxMeshSize);

		void getSubsetOfMesh(
			iREAL x[3], 
			iREAL epsilon,
			std::vector<iREAL> &xCoordinatesPartial,
			std::vector<iREAL> &yCoordinatesPartial,
			std::vector<iREAL> &zCoordinatesPartial
		);

		std::array<iREAL, 9> getInertia();
		std::array<iREAL, 9> getInverse();
		std::array<iREAL, 6> getBbox();

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
		
		delta::geometry::material::MaterialType getMaterial();
		
		virtual ~ParticleRecord();

  private:
		int     _globalParticleID;
		int     _localParticleID;
		bool    _isObstacle;
		bool    _isFriction;
		bool 		_isMesh;
		bool    _isConvex;

		iREAL   _rad;
		iREAL		_haloDiameter;
		iREAL		_diameter;
		iREAL   _mass;
		iREAL 	_epsilon;
		iREAL		_maxMeshSize;
		iREAL 	_minMeshSize;
		iREAL 	_avgMeshSize;
		
		std::array<iREAL, 9> 	_inertia;
		std::array<iREAL, 9> 	_inverse;
		std::array<iREAL, 6> 	_bbox;

		delta::core::data::OctTree _tree;
		delta::geometry::material::MaterialType _material;
};

#endif /* DELTA_CORE_DATA_PARTICLERECORD_H_ */
