/*
 * configuration.h
 *
 *  Created on: 2 Apr 2018
 *      Author: konstantinos
 */

#ifndef WORLD_CONFIGURATION_H_
#define WORLD_CONFIGURATION_H_

#include <vector>
#include <array>
#include "stdlib.h"
#include <string>

#include "delta/geometry/material.h"
#include "Object.h"

namespace delta {
  namespace world {
	  class configuration;
  }
}

class delta::world::configuration {

private:
  static iREAL getDiscritization(iREAL length, int number);

  static std::vector<std::array<iREAL, 3>> array1d(
	  iREAL position[3],
	  iREAL xAxisLength,
	  int partsNo);

  static std::vector<std::array<iREAL, 3>> array2d(
	  iREAL position[3],
	  iREAL xyAxisLength,
	  int partsNo);

  static std::vector<std::array<iREAL, 3>> array3d(
	  iREAL position[3],
	  iREAL xyzAxisLength,
	  int partsNo);

  static std::vector<std::array<iREAL, 3>> array3d(
	  iREAL position[3],
	  iREAL xyzAxisLength,
	  int partsXYZNo,
	  iREAL yAxisLength,
	  int partsYNo);

  //std::vector<delta::world::Object> _insitufineObjects;


  static void uniSphereRadius(
	iREAL totalMass,
	delta::geometry::material::MaterialType material,
	std::vector<iREAL>  &rad,
	std::vector<std::array<iREAL, 3>> &particleGrid,
	std::vector<std::string> &componentGrid,
	int index);

  static void uniSphereRadius(
	  iREAL totalMass,
	  int index,
	  std::vector<delta::world::Object>& _insitufineObjects);

  static void uniMeshGeometry(
	  iREAL totalMass,
	  int noPointsPerParticle,
	  std::vector<delta::world::Object> &_insitufineObjects,
	  int index);

  static void uniMeshGeometry(
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

  static void uniCubeGeometry(
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

  static void nonUniSphereRadius(
	  iREAL totalMass,
      delta::geometry::material::MaterialType material,
      iREAL subcellx,
      std::vector<iREAL>  &rad,
      std::vector<std::array<iREAL, 3>> &particleGrid,
      std::vector<std::string> &componentGrid,
      int index);

  static void nonUniSphereRadius(
      iREAL totalMass,
      int index,
      iREAL subcellx,
      std::vector<delta::world::Object>& _insitufineObjects);

  static void nonUniMeshGeometry(
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

public:
  configuration();
  virtual ~configuration();

  static std::vector<std::array<iREAL, 3>> getGridArrayList(
	iREAL position[3],
	int xzcuts,
	int ycuts,
	iREAL width);

  static void collapseUniformGrid(
	iREAL position[3],
	std::vector<std::array<iREAL, 3>>& grid,
	int xzcuts,
	int ycuts,
	iREAL elementWidth,
	iREAL elementHeight,
	iREAL epsilon);

  static void uniform(
	iREAL totalMass,
	int index,
	bool isSphereOrNone,
	int noPointsPerParticle,
	std::vector<delta::world::Object>& _insitufineObjects);

  static void nonuniform(
	iREAL totalMass,
	int index,
	iREAL isSphereOrNone,
	iREAL subcellx,
	int _noPointsPerParticle,
	std::vector<delta::world::Object>& _insitufineObjects);

  static void loadNuclearGeometry(iREAL position[3],
	  iREAL width,
	  int layers,
	  std::vector<delta::world::Object>& _insitufineObjects);

  static void makeBrickGrid(
	  iREAL position[3],
	  iREAL arrayXZlength,
	  int   xzElements,
	  iREAL arrayYlength,
	  int   yElements,
	  std::vector<delta::world::Object>& _insitufineObjects);
};

#endif /* WORLD_CONFIGURATION_H_ */
