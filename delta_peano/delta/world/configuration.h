/*
 * configuration.h
 *
 *  Created on: 2 Apr 2018
 *      Author: konstantinos
 */

#include <vector>
#include <array>
#include "stdlib.h"
#include <string>

#include "delta/geometry/material.h"
#include "delta/geometry/properties.h"
#include "delta/geometry/primitive/granulate.h"
#include "delta/geometry/primitive/cube.h"
#include "delta/geometry/body/graphite.h"

#include "delta/geometry/Object.h"

namespace delta {
  namespace world {
	   iREAL getDiscritization(iREAL length, int number);

	   std::vector<std::array<iREAL, 3>> array1d(
		  iREAL position[3],
		  iREAL xAxisLength,
		  int partsNo);

	   std::vector<std::array<iREAL, 3>> array2d(
		  iREAL position[3],
		  iREAL xyAxisLength,
		  int partsNo);

	   std::vector<std::array<iREAL, 3>> array3d(
		  iREAL position[3],
		  iREAL xyzAxisLength,
		  int partsNo);

	   std::vector<std::array<iREAL, 3>> array3d(
		  iREAL position[3],
		  iREAL xyzAxisLength,
		  int partsXYZNo,
		  iREAL yAxisLength,
		  int partsYNo);

	   void uniSphereRadius(
		  iREAL totalMass,
		  int index,
		  std::vector<delta::geometry::Object>& _insitufineObjects);

	   void uniMeshGeometry(
		  iREAL totalMass,
		  int noPointsPerParticle,
		  std::vector<delta::geometry::Object> &_insitufineObjects,
		  int index);

	   void uniCubeGeometry(
		  iREAL totalMass,
		  delta::geometry::material::MaterialType material,
		  int noPointsPerParticle,
		  std::vector<iREAL>  &rad,
		  std::vector<std::array<iREAL, 3>> &particleGrid,
		  std::vector<std::string> &componentGrid,
		  std::vector<std::vector<iREAL>>  &xCoordinatesArray,
		  std::vector<std::vector<iREAL>>  &yCoordinatesArray,
		  std::vector<std::vector<iREAL>>  &zCoordinatesArray,
		  int index);

	   void nonUniSphereRadius(
		  iREAL totalMass,
		  int index,
		  iREAL subcellx,
		  std::vector<delta::geometry::Object>& _insitufineObjects);

	   void nonUniMeshGeometry(
		  iREAL totalMass,
		  delta::geometry::material::MaterialType material,
		  iREAL subcellx,
		  int noPointsPerParticle,
		  std::vector<iREAL>  &rad,
		  std::vector<std::array<iREAL, 3>> &particleGrid,
		  std::vector<std::string> &componentGrid,
		  std::vector<std::vector<iREAL>>  &xCoordinatesArray,
		  std::vector<std::vector<iREAL>>  &yCoordinatesArray,
		  std::vector<std::vector<iREAL>>  &zCoordinatesArray,
		  int index);

	   void collapseUniformGrid(
		iREAL position[3],
		std::vector<std::array<iREAL, 3>>& grid,
		int xzcuts,
		int ycuts,
		iREAL elementWidth,
		iREAL elementHeight,
		iREAL epsilon);

	   std::vector<std::array<iREAL, 3>> getGridArrayList(
		iREAL position[3],
		int xzcuts,
		int ycuts,
		iREAL width);

	   void uniformlyDistributedTotalMass(
		iREAL totalMass,
		int index,
		bool isSphereOrNone,
		int noPointsPerParticle,
		std::vector<delta::geometry::Object>& _insitufineObjects);

	   void nonUniformlyDistributedTotalMass(
		iREAL totalMass,
		int index,
		iREAL isSphereOrNone,
		iREAL subcellx,
		int _noPointsPerParticle,
		std::vector<delta::geometry::Object>& _insitufineObjects);

	   void loadNuclearGeometry(
		iREAL position[3],
		iREAL width,
		int layers,
		std::vector<delta::geometry::Object>& _insitufineObjects);

	   void makeBrickGrid(
		iREAL position[3],
		iREAL arrayXZlength,
		int   xzElements,
		iREAL arrayYlength,
		int   yElements,
		std::vector<delta::geometry::Object>& _insitufineObjects);
  }
}

