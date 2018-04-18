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

		/*
		 *  Get Discretisation
		 *
		 *  Discretise length by cuts and
		 *  return sub cut lengths.
		 *
		 *  @param length
		 *  @param number
		 *  @returns iREAL
		 */
	   iREAL getDiscritization(
			   iREAL length,
			   int number);

		/*
		 *  Get One Dimensional Discretisation Positions
		 *
		 *  Discretise 1d length and return positions.
		 *
		 *  @param position 	: position of array
		 *  @param xAxisLength 	: length
		 *  @param partsNo		: segmentations
		 *  @returns vector of positions
		 */
	   std::vector<std::array<iREAL, 3>> array1d(
		  iREAL position[3],
		  iREAL xAxisLength,
		  int partsNo);

		/*
		 *  Get Two Dimensional Discretisation Positions
		 *
		 *  Discretise 2d length and return positions.
		 *
		 *  @param position 	: position of array
		 *  @param xAxisLength 	: length
		 *  @param partsNo		: segmentations
		 *  @returns vector of positions
		 */
	   std::vector<std::array<iREAL, 3>> array2d(
		  iREAL position[3],
		  iREAL xyAxisLength,
		  int partsNo);

		/*
		 *  Get Three Dimensional Discretisation Positions
		 *
		 *  Discretise 3d length and return positions.
		 *
		 *  @param position 	: position of array
		 *  @param xAxisLength 	: length
		 *  @param partsNo		: segmentations
		 *  @returns vector of positions
		 */
	   std::vector<std::array<iREAL, 3>> array3d(
		  iREAL position[3],
		  iREAL xyzAxisLength,
		  int partsNo);

		/*
		 *  Get Three Dimensional Discretisation Positions
		 *
		 *  Discretise 3d length and return positions.
		 *
		 *  @param position 		: position of array
		 *  @param xyzAxisLength 	: length along all axis
		 *  @param partsXYZNo		: segmentations on all axis
		 *  @param yAxisLength 		: length along y axis
		 *  @param partsYNo			: segmentations on y axis
		 *  @returns vector of positions
		 */
	   std::vector<std::array<iREAL, 3>> array3d(
		  iREAL position[3],
		  iREAL xyzAxisLength,
		  int partsXYZNo,
		  iREAL yAxisLength,
		  int partsYNo);

		/*
		 *  Uniform Spheres
		 *
		 *  Setup uniform distributed mass sphere setup
		 *
		 *  @param totalMass			: total mass of setup
		 *  @param index 				: index
		 *  @param _insitufineObjects	: _insitufineObjects
		 *  @returns void
		 */
	   void uniSphereRadius(
		  iREAL totalMass,
		  int index,
		  std::vector<delta::geometry::Object>& _insitufineObjects);

		/*
		 *  Uniform Mesh
		 *
		 *  Setup uniform distributed mass mesh setup
		 *
		 *  @param totalMass			: total mass of setup
		 *  @param noPointsPerParticle 	: mesh density
		 *  @param _insitufineObjects	: _insitufineObjects
		 *  @param index 				: index
		 *  @returns void
		 */
	   void uniMeshGeometry(
		  iREAL totalMass,
		  int noPointsPerParticle,
		  std::vector<delta::geometry::Object> &_insitufineObjects,
		  int index);

		/*
		 *  Uniform Cube
		 *
		 *  Setup uniform distributed cube setup
		 *
		 *  @param totalMass			: total mass of setup
		 *  @param material 			: mesh density
		 *  @param noPointsPerParticle	: _insitufineObjects
		 *  @param rad 					: radius
		 *  @param particleGrid			: particleGrid layout
		 *  @param componentGrid		: componentGrid layout
		 *  @param meshArray			: meshes
		 *  @param index				: index
		 *  @returns void
		 */
	   void uniCubeGeometry(
		  iREAL totalMass,
		  delta::geometry::material::MaterialType material,
		  int noPointsPerParticle,
		  std::vector<iREAL>  &rad,
		  std::vector<std::array<iREAL, 3>> &particleGrid,
		  std::vector<std::string> &componentGrid,
		  std::vector<delta::geometry::mesh::Mesh>  &meshArray,
		  int index);

		/*
		 *  NonUniform Sphere
		 *
		 *  Setup non uniform distributed mass sphere setup
		 *
		 *  @param totalMass			: total mass of setup
		 *  @param index 				: index
		 *  @param subcellx				: cell position length
		 *  @param _insitufineObjects	: radius
		 *  @returns void
		 */
	   void nonUniSphereRadius(
		  iREAL totalMass,
		  int index,
		  iREAL subcellx,
		  std::vector<delta::geometry::Object>& _insitufineObjects);

		/*
		 *  NonUniform Mesh
		 *
		 *  Setup non uniform distributed mass mesh setup
		 *
		 *  @param totalMass			: total mass of setup
		 *  @param material 			: material type
		 *  @param subcellx 			: cell position length
		 *  @param noPointsPerParticle	: _insitufineObjects
		 *  @param rad 					: radius
		 *  @param particleGrid			: particleGrid layout
		 *  @param componentGrid		: componentGrid layout
		 *  @param meshArray			: meshes
		 *  @param index				: index
		 *  @returns void
		 */
	   void nonUniMeshGeometry(
		  iREAL totalMass,
		  delta::geometry::material::MaterialType material,
		  iREAL subcellx,
		  int noPointsPerParticle,
		  std::vector<iREAL>  &rad,
		  std::vector<std::array<iREAL, 3>> &particleGrid,
		  std::vector<std::string> &componentGrid,
		  std::vector<delta::geometry::mesh::Mesh>  &meshArray,
		  int index);

		/*
		 *  Collapse Grid
		 *
		 *  Stitch grid elements so that they are adjucent.
		 *
		 *  @param position				: position of the layout
		 *  @param grid 				: grid layout
		 *  @param xzcuts 				: number of xz cuts
		 *  @param ycuts				: number of y cuts
		 *  @param elementWidth 		: width of the elements
		 *  @param elementHeight		: height of the elements
		 *  @param epsilon				: epsilon margin
		 *  @returns void
		 */
	   void collapseUniformGrid(
		iREAL position[3],
		std::vector<std::array<iREAL, 3>>& grid,
		int xzcuts,
		int ycuts,
		iREAL elementWidth,
		iREAL elementHeight,
		iREAL epsilon);

		/*
		 *  Make grid layout
		 *
		 *  Setup grid for grid deployment layout of particles
		 *
		 *  @param position				: position of the layout
		 *  @param xzcuts 				: number of xz cuts
		 *  @param ycuts				: number of y cuts
		 *  @param gridxyLength			: grid xyz length
		 *  @returns void
		 */
	   std::vector<std::array<iREAL, 3>> makeGridLayout(
		iREAL position[3],
		int xzcuts,
		int ycuts,
		iREAL gridxyLength);

		/*
		 *  NonUniform Distributed Total Mass
		 *
		 *  Setup non uniform distributed mass setup.
		 *
		 *  @param position				: position of the layout
		 *  @param xzcuts 				: number of xz cuts
		 *  @param ycuts				: number of y cuts
		 *  @param gridxyLength 		: grid xyz length
		 *  @param totalMass			: total mass given
		 *  @param hopperWidth			: width of the hopper
		 *  @param index				: index
		 *  @param epsilon				: epsilon margin
		 *  @param isSphereOrNone		: is it sphere
		 *  @param noPointsPerParticle	: mesh density
		 *  @param _insitufineObjects	: objects returned
		 *  @returns void
		 */
	   void uniformlyDistributedTotalMass(
		iREAL position[3],
		int xzcuts,
		int ycuts,
		iREAL gridxyLength,
		iREAL totalMass,
		iREAL hopperWidth,
		int index,
		iREAL epsilon,
		bool isSphereOrNone,
		int noPointsPerParticle,
		std::vector<delta::geometry::Object>& _insitufineObjects);

		/*
		 *  NonUniform Distributed Total Mass
		 *
		 *  Setup non uniform distributed mass setup.
		 *
		 *  @param position				: position of the layout
		 *  @param xzcuts 				: number of xz cuts
		 *  @param ycuts				: number of y cuts
		 *  @param gridxyLength 		: grid xyz length
		 *  @param totalMass			: total mass given
		 *  @param hopperWidth			: width of the hopper
		 *  @param index				: index
		 *  @param epsilon				: epsilon margin
		 *  @param isSphereOrNone		: is it sphere
		 *  @param subcellx				: cell size length at positions
		 *  @param noPointsPerParticle	: mesh density
		 *  @param _insitufineObjects	: objects returned
		 *  @returns void
		 */
	   void nonUniformlyDistributedTotalMass(
		iREAL position[3],
		int xzcuts,
		int ycuts,
		iREAL gridxyLength,
		iREAL totalMass,
		iREAL hopperWidth,
		int index,
		iREAL epsilon,
		iREAL isSphereOrNone,
		iREAL subcellx,
		int _noPointsPerParticle,
		std::vector<delta::geometry::Object>& _insitufineObjects);

		/*
		 *  Load Nuclear Geometry
		 *
		 *  load nuclear core geometry
		 *
		 *  @param position				: position of the layout
		 *  @param width 				: width of the geometry
		 *  @param layers				: number of y cuts
		 *  @param epsilon 				: grid xyz length
		 *  @param _insitufineObjects	: objects returned
		 *  @returns void
		 */
	   void loadNuclearGeometry(
		iREAL position[3],
		iREAL width,
		int layers,
		iREAL epsilon,
		std::vector<delta::geometry::Object>& _insitufineObjects);

		/*
		 *  Make Brick Grid
		 *
		 *  load nuclear core geometry into the grid
		 *
		 *  @param position				: position of the layout
		 *  @param arrayXZlength 		: width at xz axis
		 *  @param xzElements			: number of xz elements
		 *  @param arrayYlength 		: width at y axis
		 *  @param yElements			: number of y elements
		 *  @param epsilon				: epsilon margin
		 *  @param _insitufineObjects	: objects returned
		 *  @returns void
		 */
	   void makeBrickGrid(
		iREAL position[3],
		iREAL arrayXZlength,
		int   xzElements,
		iREAL arrayYlength,
		int   yElements,
		iREAL epsilon,
		std::vector<delta::geometry::Object>& _insitufineObjects);

		/*
		 *  Compute Boundary
		 *
		 *  compute boundaries of the computational domain
		 *
		 *  @param position				: position of the layout
		 *  @param width 				: width of the geometry
		 *  @param layers				: layers of decks
		 *  @param epsilon 				: epsilon margin
		 *  @param _insitufineObjects	: objects returned
		 *  @returns void
		 */
	   void computeBoundary(
	  	 std::vector<delta::geometry::Object>& coarseObjects,
	  	 std::vector<delta::geometry::Object>& fineObjects,
	  	 std::vector<delta::geometry::Object>& insitufineObjects,
	  	 iREAL& minParticleDiam,
	  	 iREAL& maxParticleDiam,
	  	 iREAL* minComputeDomain,
	  	 iREAL* maxComputeDomain);
  }
}
#endif
