/*
 * scenarios.h
 *
 *  Created on: 19 Apr 2018
 *      Author: konstantinos
 */

#ifndef WORLD_SCENARIOS_H_
#define WORLD_SCENARIOS_H_

#include<vector>
#include<array>
#include<delta/geometry/Object.h>
#include<delta/geometry/mesh/Mesh.h>
#include<delta/geometry/material.h>

namespace delta {
  namespace world {
	namespace scenarios {

	  void hopper(
		  std::vector<delta::geometry::Object> &coarse,
		  std::vector<delta::geometry::Object> &insitu,
		  iREAL centre[3],
		  int xzcuts,
		  int ycuts,
		  bool uniform,
		  bool isSphere,
		  int meshDensity,
		  iREAL epsilon);

	  void turbine(
	  	std::vector<delta::geometry::Object>& 	coarseObjects,
	  	bool 									isSphere,
	  	int 										meshDensity,
	  	iREAL 									epsilon);

	  void friction(
		  int 									scenario,
		  bool 									isSphere,
		  iREAL 									centre[3],
		  int 									meshDensity,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object> &	coarse);

	  void nuclear(
		  int 									scenario,
		  bool 									isSphere,
		  iREAL 									centre[3],
		  int 									meshDensity,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object> &	coarse,
		  std::vector<delta::geometry::Object> &	fine);

	  void freeFall(
		  int 									scenario,
		  bool 									isSphere,
		  iREAL 									centre[3],
		  int 									meshDensity,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object> &	fine);

	  void twoParticlesCrash(
		  std::vector<delta::geometry::Object>& 	coarseObject,
		  bool 									isSphere,
		  int 									meshDensity,
		  iREAL 									epsilon);

	  void rotateParticle(
		  std::vector<delta::geometry::Object>& 	coarseObject,
		  bool 									isSphere,
		  int 									meshDensity,
		  iREAL 									epsilon);

	} /* namespace scenarios */
  } /* namespace world */
} /* namespace delta */

#endif /* WORLD_SCENARIOS_H_ */
