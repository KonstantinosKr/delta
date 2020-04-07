/*
 The MIT License (MIT)

 Copyright (c) 2018 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef SCENARIO_CONFIGURATION_H
#define SCENARIO_CONFIGURATION_H

#include <vector>
#include <array>
#include "stdlib.h"
#include <string>

#include <geometry/material.h>
#include <world/structure/Object.h>
#include <world/operators/arrayLayout.h>
#include <geometry/hardcoded/graphite.h>
#include <geometry/hardcoded/granulate.h>
#include <geometry/hardcoded/cube.h>

namespace delta {
  namespace world {
	namespace configuration {
		  /*
		   *  Uniform Spheres
		   *
		   *  Setup uniform distributed mass sphere setup
		   *
		   *  @param totalMass	: total mass of setup
		   *  @param index 		: index
		   *  @param objects		: objects
		   *  @returns void
		   */
		 void uniSphereRadius(
			iREAL 											totalMass,
			std::vector<delta::world::structure::Object>& 	objects);

		  /*
		   *  Uniform Mesh
		   *
		   *  Setup uniform distributed mass mesh setup
		   *
		   *  @param totalMass				: total mass of setup
		   *  @param noPointsPerParticle 	: mesh density
		   *  @param objects					: objects
		   *  @param index 					: index
		   *  @returns void
		   */
		 void uniMeshGeometry(
			iREAL 											totalMass,
			int 												noPointsPerParticle,
			std::vector<delta::world::structure::Object>&	objects);

		  /*
		   *  Uniform Cube
		   *
		   *  Setup uniform distributed cube setup
		   *
		   *  @param totalMass				: total mass of setup
		   *  @param meshDensity				: meshDensity
		   *  @param objects					: objects
		   *  @param index					: index
		   *  @returns void
		   */
		 void uniCubeGeometry(
		    iREAL 											totalMass,
		    int 												noPointsPerParticle,
		    std::vector<delta::world::structure::Object>&		objects);

		  /*
		   *  NonUniform Sphere
		   *
		   *  Setup non uniform distributed mass sphere setup
		   *
		   *  @param totalMass			: total mass of setup
		   *  @param index 				: index
		   *  @param subcellx			: cell position length
		   *  @param objects				: radius
		   *  @returns void
		   */
		 void nonUniSphereRadius(
			iREAL 											totalMass,
			iREAL 											subcellx,
			std::vector<delta::world::structure::Object>& 	objects);

		  /*
		   *  NonUniform Mesh
		   *
		   *  Setup non uniform distributed mass mesh setup
		   *
		   *  @param totalMass				: total mass of setup
		   *  @param subcellx 				: cell position length
		   *  @param meshDensity 			: mesh density
		   *  @param objects 				: the objects
		   *  @returns void
		   */
		 void nonUniMeshGeometry(
		    iREAL 											totalMass,
		    iREAL 											subcellx,
			int 												meshDensity,
			std::vector<delta::world::structure::Object>&	objects);

		  /*
		   *  NonUniform Mesh
		   *
		   *  Setup non uniform distributed mass mesh setup
		   *
		   *  @param totalMass				: total mass of setup
		   *  @param subcellx 				: cell position length
		   *  @param meshDensity 			: mesh density
		   *  @param objects 				: the objects
		   *  @returns void
		   */
		 void nonUniCubeGeometry(
		  iREAL totalMass,
		  iREAL subcellx,
		  int meshDensity,
		  std::vector<delta::world::structure::Object>&	objects);

		  /*
		   *  NonUniform Distributed Total Mass
		   *
		   *  Setup non uniform distributed mass setup.
		   *
		   *  @param position				: position of the layout
		   *  @param xzcuts 					: number of xz cuts
		   *  @param ycuts					: number of y cuts
		   *  @param gridxyLength 			: grid xyz length
		   *  @param totalMass				: total mass given
		   *  @param hopperWidth				: width of the hopper
		   *  @param index					: index
		   *  @param epsilon					: epsilon margin
		   *  @param isSphereOrNone			: is it sphere
		   *  @param meshDensity				: mesh density
		   *  @returns objects
		   */
		 void uniformlyDistributedTotalMass(
	      std::vector<delta::world::structure::Object>&	insitu,
		  iREAL 											position[3],
		  int 											xzcuts,
		  int 											ycuts,
		  iREAL 											gridxyLength,
		  iREAL 											totalMass,
		  iREAL 											hopperWidth,
		  int 											index,
		  iREAL 											epsilon,
		  bool 											isSphereOrNone,
		  int 											meshDensity);

		  /*
		   *  NonUniform Distributed Total Mass
		   *
		   *  Setup non uniform distributed mass setup.
		   *
		   *  @param position				: position of the layout
		   *  @param xzcuts 					: number of xz cuts
		   *  @param ycuts					: number of y cuts
		   *  @param gridxyLength 			: grid xyz length
		   *  @param totalMass				: total mass given
		   *  @param hopperWidth				: width of the hopper
		   *  @param index					: index
		   *  @param epsilon					: epsilon margin
		   *  @param isSphereOrNone			: is it sphere
		   *  @param subcellx				: cell size length at positions
		   *  @param meshDensity				: mesh density
		   *  @returns objects
		   */
		 void nonUniformlyDistributedTotalMass(
		  std::vector<delta::world::structure::Object>&	insitu,
		  iREAL 											position[3],
		  int 											xzcuts,
		  int 											ycuts,
		  iREAL 											gridxyLength,
		  iREAL 											totalMass,
		  iREAL 											hopperWidth,
		  int 											index,
		  iREAL 											epsilon,
		  iREAL 											isSphereOrNone,
		  iREAL 											subcellx,
		  int 											meshDensity);

		  /*
		   *  Load Nuclear Geometry
		   *
		   *  load nuclear core geometry
		   *
		   *  @param position			: position of the layout
		   *  @param width 				: width of the geometry
		   *  @param layers				: number of y cuts
		   *  @param epsilon 			: grid xyz length
		   *  @param objects				: objects returned
		   *  @returns void
		   */
		 void loadNuclearGeometry(
		  iREAL 											position[3],
		  iREAL 											width,
		  int 											layers,
		  iREAL 											epsilon,
		  std::vector<delta::world::structure::Object>& 	objects);

		  /*
		   *  Make Brick Grid
		   *
		   *  load nuclear core geometry into the grid
		   *
		   *  @param position			: position of the layout
		   *  @param arrayXZlength 		: width at xz axis
		   *  @param xzElements			: number of xz elements
		   *  @param arrayYlength 		: width at y axis
		   *  @param yElements			: number of y elements
		   *  @param epsilon				: epsilon margin
		   *  @param objects				: objects returned
		   *  @returns void
		   */
		 void makeBrickGrid(
		  iREAL 											position[3],
		  iREAL							 				arrayXZlength,
		  int   											xzElements,
		  iREAL 											arrayYlength,
		  int   											yElements,
		  iREAL 											epsilon,
		  std::vector<delta::world::structure::Object>& 	objects);
	}
  }
}
#endif
