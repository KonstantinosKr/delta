/*
 * triangles.h
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_MESH_TRIANGLE_H_
#define GEOMETRY_MESH_TRIANGLE_H_

#include "delta/geometry/mesh/vertex.h"
#include <map>
#include <vector>
#include <array>
#include <cmath>

namespace delta {
  namespace geometry {
	namespace mesh {
	  class Triangle;
	}
  }
}

class delta::geometry::mesh::Triangle {
  public:
	Triangle(
		delta::geometry::mesh::Vertex &A,
		delta::geometry::mesh::Vertex &B,
		delta::geometry::mesh::Vertex &C);

	Triangle(
		iREAL A[3],
		iREAL B[3],
		iREAL C[3]);

	Triangle(
		iREAL Ax,
		iREAL Ay,
		iREAL Az,

		iREAL Bx,
		iREAL By,
		iREAL Bz,

		iREAL Cx,
		iREAL Cy,
		iREAL Cz);

	delta::geometry::mesh::Vertex getVertexA();
	delta::geometry::mesh::Vertex getVertexB();
	delta::geometry::mesh::Vertex getVertexC();

	void flatten (
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	double getTriangleLength();

	iREAL getXYZWidth();
	iREAL getXZWidth();

	iREAL getXw();
	iREAL getYw();
	iREAL getZw();

	std::array<iREAL, 3> getMinBoundaryVertex();
	std::array<iREAL, 3> getMaxBoundaryVertex();

	iREAL getMaxXAxis();
	iREAL getMaxYAxis();
	iREAL getMaxZAxis();

	iREAL getMinXAxis();
	iREAL getMinYAxis();
	iREAL getMinZAxis();

	virtual ~Triangle();

  private:
	delta::geometry::mesh::Vertex *_A, *_B, *_C;

};

#endif /* GEOMETRY_MESH_TRIANGLE_H_ */
