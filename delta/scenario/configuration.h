/*
 * configuration.h
 *
 *  Created on: 2 Apr 2018
 *      Author: konstantinos
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <vector>
#include <array>
#include "stdlib.h"
#include <string>

#include "delta/geometry/material.h"
#include "delta/geometry/Object.h"

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
			iREAL totalMass,
			int 	index,
			std::vector<delta::geometry::Object>& objects);

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
			iREAL 									totalMass,
			int 										noPointsPerParticle,
			std::vector<delta::geometry::Object> &	objects,
			int 										index);

		  /*
		   *  Uniform Cube
		   *
		   *  Setup uniform distributed cube setup
		   *
		   *  @param totalMass				: total mass of setup
		   *  @param material 				: mesh density
		   *  @param meshDensity				: objects
		   *  @param rad 					: radius
		   *  @param particleGrid			: particleGrid layout
		   *  @param componentGrid			: componentGrid layout
		   *  @param meshArray				: meshes
		   *  @param index					: index
		   *  @returns void
		   */
		 void uniCubeGeometry(
			iREAL 										totalMass,
			delta::geometry::material::MaterialType 		material,
			int 											meshDensity,
			std::vector<iREAL>  &						rad,
			std::vector<std::array<iREAL, 3>> &			particleGrid,
			std::vector<std::string> &					componentGrid,
			std::vector<delta::geometry::mesh::Mesh>  &	meshArray,
			int 											index);

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
			iREAL 									totalMass,
			int 										index,
			iREAL 									subcellx,
			std::vector<delta::geometry::Object>& 	objects);

		  /*
		   *  NonUniform Mesh
		   *
		   *  Setup non uniform distributed mass mesh setup
		   *
		   *  @param totalMass				: total mass of setup
		   *  @param material 				: material type
		   *  @param subcellx 				: cell position length
		   *  @param meshDensity				: objects
		   *  @param rad 					: radius
		   *  @param particleGrid			: particleGrid layout
		   *  @param componentGrid			: componentGrid layout
		   *  @param meshArray				: meshes
		   *  @param index					: index
		   *  @returns void
		   */
		 void nonUniMeshGeometry(
		    iREAL totalMass,
		    delta::geometry::material::MaterialType material,
		    iREAL subcellx,
			int meshDensity,
			std::vector<delta::geometry::Object> &objects);

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
	      std::vector<delta::geometry::Object>&	insitu,
		  iREAL 									position[3],
		  int 									xzcuts,
		  int 									ycuts,
		  iREAL 									gridxyLength,
		  iREAL 									totalMass,
		  iREAL 									hopperWidth,
		  int 									index,
		  iREAL 									epsilon,
		  bool 									isSphereOrNone,
		  int 									meshDensity);

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
		  std::vector<delta::geometry::Object>&	insitu,
		  iREAL 									position[3],
		  int 									xzcuts,
		  int 									ycuts,
		  iREAL 									gridxyLength,
		  iREAL 									totalMass,
		  iREAL 									hopperWidth,
		  int 									index,
		  iREAL 									epsilon,
		  iREAL 									isSphereOrNone,
		  iREAL 									subcellx,
		  int 									meshDensity);

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
		  iREAL 									position[3],
		  iREAL 									width,
		  int 									layers,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object>& 	objects);

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
		  iREAL 									position[3],
		  iREAL							 		arrayXZlength,
		  int   									xzElements,
		  iREAL 									arrayYlength,
		  int   									yElements,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object>& 	objects);
	}
  }
}
#endif
