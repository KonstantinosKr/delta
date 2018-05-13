/*
 * mesh.h
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_MESH_MESH_H_
#define GEOMETRY_MESH_MESH_H_


#include <vector>
#include <map>
#include <cmath>
#include <array>
#include <string>
#include <sstream>
#include <delta/geometry/mesh/Triangle.h>
#include <delta/geometry/material.h>
#include <delta/geometry/operators/hull/alg.h>
#include <delta/core/algo.h>

namespace delta {
  namespace geometry {
	namespace mesh {
	  class Mesh;
	}
  }
}

class delta::geometry::mesh::Mesh {
  public:
	Mesh();

	Mesh(
		std::vector<std::array<int, 3>> 		triangleFaces,
		std::vector<std::array<iREAL, 3>> 	uniqueVertices);

	Mesh(
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	Mesh(std::vector<delta::geometry::mesh::Triangle> _triangles);

	/*
	 *  Flatten Data Structure
	 *
	 *  Returns the SoA data structure of the object oriented
	 *  structure of triangles.
	 *
	 *  @param xCoordinates : x axis elements array of mesh
	 *  @param yCoordinates : y axis elements array of mesh
	 *  @param zCoordinates : z axis elements array of mesh
	 *  @returns void but through parameters by reference
	 */
	void flatten (
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	/*
	 *  Replace Data Structure
	 *
	 *  Set SoA data structure with SoA passed.
	 *  This modifies the local data.
	 *
	 *
	 *  @param xCoordinates : x axis elements array of mesh
	 *  @param yCoordinates : y axis elements array of mesh
	 *  @param zCoordinates : z axis elements array of mesh
	 *  @returns void
	 */
	void replace (
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	/*
	 *  Move Mesh From Position To Origin
	 *
	 *  Set center of geometry to origin.
	 *  This modifies the local data.
	 *
	 *
	 *  @param centre : centre of geometry
	 *  @returns void
	 */
	void moveMeshFromPositionToOrigin(iREAL center[3]);

	/*
	 *  Move Mesh From Origin To Position
	 *
	 *  Set center of geometry to position.
	 *  This modifies the local data.
	 *
	 *
	 *  @param centre : centre of geometry
	 *  @returns void
	 */
	void moveMeshFromOriginToPosition(iREAL center[3]);

	/*
	 *  Scale XYZ
	 *
	 *  Resize by scale factor towards all axis directions.
	 *  This modifies the local data.
	 *
	 *
	 *  @param scale : centre of geometry
	 *  @returns void
	 */
	void scaleXYZ(iREAL scale, iREAL position[3]);

	/*
	 *  Rotate X
	 *
	 *  Rotate along X axis by value.
	 *  This modifies the local data.
	 *
	 *
	 *  @param alphaX : rotation value 0-1 that correspond to 0 to 360 degrees.
	 *  @returns void
	 */
	void rotateX(iREAL alphaX);

	/*
	 *  Rotate Y
	 *
	 *  Rotate along Y axis by value.
	 *  This modifies the local data.
	 *
	 *
	 *  @param alphaY : rotation value 0-1 that correspond to 0 to 360 degrees.
	 *  @returns void
	 */
	void rotateY(iREAL alphaY);

	/*
	 *  Rotate Z
	 *
	 *  Rotate along Z axis by value.
	 *  This modifies the local data.
	 *
	 *
	 *  @param alphaZ : rotation value 0-1 that correspond to 0 to 360 degrees.
	 *  @returns void
	 */
	void rotateZ(iREAL alphaZ);

	/*
	 *  Compute Diagonal
	 *
	 *  Returns max diagonal in mesh
	 *
	 *
	 *  @param none
	 *  @returns iREAL
	 */
	iREAL computeDiagonal();

	/*
	 *  Get X Coordinates
	 *
	 *  Returns X coordinates as SoA data structure.
	 *
	 *
	 *  @param none
	 *  @returns vector of iREAL values
	 */
	iREAL* 		getXCoordinates();

	/*
	 *  Get Y Coordinates
	 *
	 *  Returns Y coordinates as SoA data structure.
	 *
	 *
	 *  @param none
	 *  @returns vector of iREAL values
	 */
	iREAL* 		getYCoordinates();

	/*
	 *  Get Z Coordinates
	 *
	 *  Returns Z coordinates as SoA data structure.
	 *
	 *
	 *  @param none
	 *  @returns vector of iREAL values
	 */
	iREAL* 		getZCoordinates();

	/*
	 *  Get Width of the X Coordinates
	 *
	 *  Returns width of x coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getXw();

	/*
	 *  Get Width of the Y Coordinates
	 *
	 *  Returns width of Y coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getYw();

	/*
	 *  Get Width of the Z Coordinates
	 *
	 *  Returns width of Z coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getZw();

	/*
	 *  Get Max Value on X Axis
	 *
	 *  Returns max value on X coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getMaxXAxis();

	/*
	 *  Get Max Value on Y Axis
	 *
	 *  Returns max value on Y coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getMaxYAxis();

	/*
	 *  Get Max Value on Z Axis
	 *
	 *  Returns max value on Z coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getMaxZAxis();

	/*
	 *  Get Min Value on X Axis
	 *
	 *  Returns min value on X coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getMinXAxis();

	/*
	 *  Get Min Value on Y Axis
	 *
	 *  Returns min value on Y coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getMinYAxis();

	/*
	 *  Get Min Value on Z Axis
	 *
	 *  Returns min value on Z coordinates
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getMinZAxis();

	/*
	 *  Get H Min
	 *
	 *  Returns min value of line segment length of mesh
	 *
	 *
	 *  @param none
	 *  @returns iREAL value
	 */
	iREAL 					getHMin();

	/*
	 *  Get Min Boundary Vertex
	 *
	 *  Returns min vertex of mesh boundary
	 *
	 *
	 *  @param none
	 *  @returns xyz vertex
	 */
	std::array<iREAL, 3> 	getMinBoundaryVertex();

	/*
	 *  Get Max Boundary Vertex
	 *
	 *  Returns max vertex of mesh boundary
	 *
	 *
	 *  @param none
	 *  @returns xyz vertex
	 */
	std::array<iREAL, 3> 	getMaxBoundaryVertex();

	/*
	 *  Get XY Width
	 *
	 *  Returns width on XY coordinates.
	 *
	 *
	 *  @param none
	 *  @returns iREAL
	 */
	iREAL 					getXZWidth();

	/*
	 *  Get Triangle Faces
	 *
	 *  Returns vector of triangle faces as
	 *  an array of integer index pointers to unique vertices.
	 *  The returned index pointers point to unique vertices as
	 *  returned in "getUniqueVertices()" function and correspond
	 *  to each of the three points of the triangle face.
	 *
	 *
	 *  @param none
	 *  @returns vector of arrays of index pointers
	 */
	std::vector<std::array<int, 3>> 	getTriangleFaces();

	/*
	 *  Get Unique Vertices
	 *
	 *  Returns vector of mesh unique vertices.
	 *
	 *
	 *  @param none
	 *  @returns vector of unique xyz vertices
	 */
	std::vector<std::array<iREAL, 3>> 	getUniqueVertices();

	/*
	 *  Get Center of Geometry
	 *
	 *  Returns center of Geometry through parameter.
	 *
	 *
	 *  @param centreOfGeometry
	 *  @returns void
	 */
	void getCenterOfGeometry(
	  iREAL centreOfGeometry[3]);

	/*
	 *  Get Center of Mass
	 *
	 *  Returns spatial and referential center of mass by reference.
	 *
	 *
	 *  @param centreOfMassX
	 *  @param centreOfMassY
	 *  @param centreOfMassZ
	 *  @param refcentreOfMassX
	 *  @param refcentreOfMassY
	 *  @param refcentreOfMassZ
	 *
	 *  @returns void
	 */
	void getCenterOfMass(
	  iREAL&               centreOfMassX,
	  iREAL&               centreOfMassY,
	  iREAL&               centreOfMassZ,
	  iREAL&               refcentreOfMassX,
	  iREAL&               refcentreOfMassY,
	  iREAL&               refcentreOfMassZ
	);

	/*
	 *  Get X Coordinates
	 *
	 *  Returns vector of x axis coordinates of mess as SoA structure.
	 *
	 *
	 *  @param none
	 *  @returns vector
	 */
	std::vector<iREAL> getXCoordinatesAsVector();

	/*
	 *  Get X Coordinates
	 *
	 *  Returns vector of x axis coordinates of mess as SoA structure.
	 *
	 *
	 *  @param none
	 *  @returns vector
	 */
	std::vector<iREAL> getYCoordinatesAsVector();

	/*
	 *  Get X Coordinates
	 *
	 *  Returns vector of x axis coordinates of mess as SoA structure.
	 *
	 *
	 *  @param none
	 *  @returns vector
	 */
	std::vector<iREAL> getZCoordinatesAsVector();

	void explode(iREAL length);
	void exploded(iREAL length);

	/*
	 *  Get Mass
	 *
	 *  Returns mass using the passed material.
	 *
	 *
	 *  @param material
	 *  @returns iREAL
	 */
	iREAL computeMass(
	    delta::geometry::material::MaterialType material);

	/*
	 *  Get Volume
	 *
	 *  Returns volume.
	 *
	 *
	 *  @param none
	 *  @returns iREAL
	 */
    iREAL computeVolume();

	/*
	 *  Get Inertia Matrix
	 *
	 *  Returns inertia by reference.
	 *
	 *
	 *  @param material
  	 *  @param mass
	 *  @param center
	 *  @param inertia
	 *  @returns void
	 */
	void computeInertia(
		delta::geometry::material::MaterialType material,
		iREAL& mass,
		iREAL center[3],
		iREAL inertia[9]);

	/*
	 *  Get Inverse Inertia Matrix
	 *
	 *  Returns inverse inertia by reference.
	 *
	 *
	 *  @param inertia
	 *  @param inverse
	 *  @param isObject
	 *  @returns void
	 */
	void computeInverseInertia(
	    iREAL inertia[9],
	    iREAL inverse[9],
	    bool isObject);

	virtual ~Mesh();
  private:

	/*
	 *  Get XYZ Width
	 *
	 *  Returns width on XYZ coordinates.
	 *
	 *
	 *  @param none
	 *  @returns iREAL
	 */
	iREAL 					getXYZWidth();

	/*
	 *  Get Distance Between two Vertices
	 *
	 *  Returns distance
	 *
	 *
	 *  @param A vertex
  	 *  @param B vertex
	 *  @returns iREAL
	 */
	iREAL computeDistanceAB(
	    std::array<iREAL, 3> A,
	    std::array<iREAL, 3> B);

	/*
	 *  Compress from vectors
	 *
	 *  Compresses SoA data structure into triangle
	 *  faces pointer and unique vertices.
	 *  This modifies the local data.
	 *
	 *  @returns void
	 */
	void compressFromVectors();

	std::vector<delta::geometry::mesh::Triangle> _triangles;

	std::vector<std::array<int, 3>> 				_triangleFaces;
	std::vector<std::array<iREAL, 3>>		 	_uniqueVertices;

    std::vector<iREAL>   						_xCoordinates;
    std::vector<iREAL>   						_yCoordinates;
    std::vector<iREAL>   						_zCoordinates;

    //https://en.wikipedia.org/wiki/Trace_%28linear_algebra%29
    #define TRACE(A) ((A)[0] + (A)[4] + (A)[8])

	#define simplex_1(J, a, b, c, d) ((J)/6.)
};

#endif /* GEOMETRY_MESH_MESH_H_ */
