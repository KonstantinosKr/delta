/*
 The MIT License (MIT)

 Copyright (c) 26 May 2016 Konstantinos Krestenitis

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

#ifndef DELTA_geometry_TRIANGLE_H_
#define DELTA_geometry_TRIANGLE_H_

#include <stdlib.h>
#include <assert.h>
#include <cmath>
#include <vector>
#include <array>
#include <delta/geometry/material.h>
#include <delta/geometry/Object.h>
#include <delta/geometry/primitive/surface.h>
#include <delta/geometry/operators/physics.h>
#include <delta/geometry/mesh/Mesh.h>

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace triangle {

		/*
		 *  Bisect triangle
		 *
		 *  Returns sub triangles by reference.
		 *
		 *
		 *  @param xCoordinates
		 *  @param yCoordinates
		 *  @param zCoordinates
		 *  @returns void
		 */
		void bisectTriangle(
			iREAL xT[3],
			iREAL yT[3],
			iREAL zT[3],
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

		/*
		 *  Trisect triangle
		 *
		 *  Returns sub triangles by reference.
		 *
		 *
		 *  @param xCoordinates
		 *  @param yCoordinates
		 *  @param zCoordinates
		 *  @returns void
		 */
		void triSectTriangle(
			iREAL A[3],
			iREAL B[3],
			iREAL C[3],
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

		/*
		 *  Bisect at sides triangle
		 *
		 *  Returns sub triangles by reference.
		 *
		 *
		 *  @param xCoordinates
		 *  @param yCoordinates
		 *  @param zCoordinates
		 *  @returns void
		 */
		void biSideSectTriangle(
			iREAL A[3],
			iREAL B[3],
			iREAL C[3],
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

		/*
		 *  Fivesect triangle
		 *
		 *  Returns sub triangles by reference.
		 *
		 *
		 *  @param xCoordinates
		 *  @param yCoordinates
		 *  @param zCoordinates
		 *  @returns void
		 */
		void fiveSectTriangle(
			iREAL A[3],
			iREAL B[3],
			iREAL C[3],
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

		/*
		 *  Refine mesh
		 *
		 *  Returns sub triangles by reference.
		 *
		 *
		 *	@param meshRefinement : levels of refinement
		 *	@param gridH : width of H
		 *  @param xCoordinates
		 *  @param yCoordinates
		 *  @param zCoordinates
		 *  @returns void
		 */
		void meshDenser(
			int meshRefinement,
			iREAL gridH,
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

		/*
		 *  Refine mesh
		 *
		 *  Returns sub triangles by reference.
		 *
		 *
		 *	@param meshRefinement : levels of refinement
		 *  @param xCoordinates
		 *  @param yCoordinates
		 *  @param zCoordinates
		 *  @returns void
		 */
		void meshDenser(
			int meshRefinement,
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

		/*
		 *  OctSect Mesh
		 *
		 *  Returns sub triangles by reference per sub section.
		 *
		 *
		 *	@param quadsectTimes : levels of sections
		 *  @param xCoordinatesMultiLevel
		 *  @param yCoordinatesMultiLevel
		 *  @param zCoordinatesMultiLevel
  		 *  @param centroid
		 *  @returns void
		 */
		int octSectParticle(
			int quadsectTimes,
			std::vector<std::vector<iREAL>>&  xCoordinatesMultiLevel,
			std::vector<std::vector<iREAL>>&  yCoordinatesMultiLevel,
			std::vector<std::vector<iREAL>>&  zCoordinatesMultiLevel,
			std::vector<std::array<iREAL, 3>>& centroid);

		/*
		 *  OctSect Mesh
		 *
		 *  Returns sub triangles by reference per sub section.
		 *
		 *
		 *	@param quadsectTimes : levels of sections
		 *  @param xCoordinatesMultiLevel
		 *  @param yCoordinatesMultiLevel
		 *  @param zCoordinatesMultiLevel
  		 *  @param centroid
		 *  @returns void
		 */
		int octSect(
			int level,
			int index,
			std::array<iREAL, 3> minpoint,
			std::array<iREAL, 3> midpoint,
			std::array<iREAL, 3> maxpoint,
			std::vector<std::vector<iREAL>>&  xCoordinatesMultiLevel,
			std::vector<std::vector<iREAL>>&  yCoordinatesMultiLevel,
			std::vector<std::vector<iREAL>>&  zCoordinatesMultiLevel,
			std::vector<std::array<iREAL, 3>>& centroid);

		/*
		 *  Get Triangle Length
		 *
		 *  Returns sub triangles by reference.
		 *
		 *
		 *	@param A
		 *  @param B
		 *  @param C
		 *  @param zCoordinatesMultiLevel
  		 *  @param centroid
		 *  @returns iREAL
		 */
		iREAL getTriangleLength(
			iREAL A[3],
			iREAL B[3],
			iREAL C[3]);

		/*
		 *  Get Triangles in Bounding Box
		 *
		 *  Returns sub triangles by reference enclosed by
		 *  bounding box.
		 *
		 *
		 *	@param minpoint : min vertex of bounding box
		 *  @param maxpoint : max vertex of bounding box
		 *  @param xCoordinatesRoot : Coordinates of mesh
		 *  @param yCoordinatesRoot
  		 *  @param zCoordinatesRoot
		 *  @param xCoordinatesBounded : Coordinates of enclosed triangles
  		 *  @param yCoordinatesBounded
  		 *  @param zCoordinatesBounded
		 *  @returns void
		 */
		void getTrianglesInBoundingBox(
			std::array<iREAL, 3> minpoint,
			std::array<iREAL, 3> maxpoint,
			std::vector<iREAL>&  xCoordinatesRoot,
			std::vector<iREAL>&  yCoordinatesRoot,
			std::vector<iREAL>&  zCoordinatesRoot,
			std::vector<iREAL>&  xCoordinatesBounded,
			std::vector<iREAL>&  yCoordinatesBounded,
			std::vector<iREAL>&  zCoordinatesBounded);

		/*
		 *  Decompose Mesh
		 *
		 *  Returns a vector with children meshes by reference.
		 *
		 *
		 *	@param octSectTimes : number of refinements
		 *  @param mesh : initial mesh
		 *  @param material
		 *  @param isFriction
  		 *  @param isObstacle
		 *  @param epsilon
		 *  @param fineObjects : generated meshes
  		 *  @param numberOfParticles
  		 *  @param numberOfObstacles
		 *  @returns void
		 */
		void decomposeMeshByOctsection(
		    int octSectTimes,
		    delta::geometry::mesh::Mesh mesh,
		    delta::geometry::material::MaterialType material,
		    bool isFriction,
		    bool isObstacle,
			iREAL epsilon,
		    std::vector<delta::geometry::Object> &fineObjects,
			int &numberOfParticles,
			int &numberOfObstacles);

		/*
		 *  Get Triangles in Bounding Box
		 *
		 *  Returns a vector of children objects by reference.
		 *
		 *
		 *	@param mesh : min vertex of bounding box
		 *  @param material : max vertex of bounding box
		 *  @param isObstacle : Coordinates of mesh
		 *  @param isFriction
  		 *  @param epsilon
		 *  @param fineObjects : Coordinates of enclosed triangles
		 *  @returns int
		 */
		int decomposeMeshIntoParticles(
		    delta::geometry::mesh::Mesh mesh,
		    delta::geometry::material::MaterialType material,
		    bool isObstacle,
		    bool isFriction,
			iREAL epsilon,
		    std::vector<delta::geometry::Object> &fineObjects);
	  }
    }
  }
}



#endif /* DELTA_geometry_TRIANGLE_H_ */
