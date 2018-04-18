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

	Mesh(std::vector<iREAL> xCoordinates,
		std::vector<iREAL> yCoordinates,
		std::vector<iREAL> zCoordinates);

	Mesh(std::vector<delta::geometry::mesh::Triangle> _triangles);

	void flatten (
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	void replace (
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	void moveMeshFromPositionToOrigin(iREAL center[3]);
	void moveMeshFromOriginToPosition(iREAL center[3]);
	void scaleXYZ(iREAL scale, iREAL position[3]);
	void rotateX(iREAL alphaX);
	void rotateY(iREAL alphaY);
	void rotateZ(iREAL alphaZ);

	iREAL computeDistanceAB(
	    std::array<iREAL, 3> A,
	    std::array<iREAL, 3> B);

	std::vector<iREAL> 		getxCoordinates();
	std::vector<iREAL> 		getyCoordinates();
	std::vector<iREAL> 		getzCoordinates();
	iREAL 					getXYZWidth();
	iREAL 					getXZWidth();
	iREAL 					getXw();
	iREAL 					getYw();
	iREAL 					getZw();
	iREAL 					getMaxXAxis();
	iREAL 					getMaxYAxis();
	iREAL 					getMaxZAxis();
	iREAL 					getMinXAxis();
	iREAL 					getMinYAxis();
	iREAL 					getMinZAxis();
	iREAL 					getHMin();
	std::array<iREAL, 3> 	getMinBoundaryVertex();
	std::array<iREAL, 3> 	getMaxBoundaryVertex();

	iREAL computeDiagonal();

	std::vector<std::array<int, 3>> 		getTriangleFaces();
	std::vector<std::array<iREAL, 3>> 	getUniqueVertices();

	void getCenterOfGeometry(
	  iREAL centreOfGeometry[3]);

	void getCenterOfMass(
	  iREAL&               centreOfMassX,
	  iREAL&               centreOfMassY,
	  iREAL&               centreOfMassZ,
	  iREAL&               refcentreOfMassX,
	  iREAL&               refcentreOfMassY,
	  iREAL&               refcentreOfMassZ
	);

	void explode(iREAL length);
	void exploded(iREAL length);

    iREAL computeVolume();

	void computeInertia(
		delta::geometry::material::MaterialType material,
		iREAL& mass,
		iREAL center[3],
		iREAL inertia[9]);

	iREAL computeMass(
	    delta::geometry::material::MaterialType material);

	void computeInverseInertia(
	    iREAL inertia[9],
	    iREAL inverse[9],
	    bool isObject);

	virtual ~Mesh();
  private:
	void compressFromVectors(
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	std::vector<delta::geometry::mesh::Triangle> _triangles;

	std::vector<std::array<int, 3>> 				_triangleFaces;
	std::vector<std::array<iREAL, 3>>		 	_uniqueVertices;

    std::vector<iREAL>   						_xCoordinates;
    std::vector<iREAL>   						_yCoordinates;
    std::vector<iREAL>   						_zCoordinates;

    iREAL simplex_J (iREAL *a, iREAL *b, iREAL *c, iREAL *d);
	#define simplex_1(J, a, b, c, d) ((J)/6.)

};

#endif /* GEOMETRY_MESH_MESH_H_ */
