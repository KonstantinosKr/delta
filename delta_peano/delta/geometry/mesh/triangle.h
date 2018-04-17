/*
 * triangles.h
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_MESH_TRIANGLE_H_
#define GEOMETRY_MESH_TRIANGLE_H_

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
		std::array<iREAL, 3> A,
		std::array<iREAL, 3> B,
		std::array<iREAL, 3> C);

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

	std::array<iREAL, 3> getVertexA();
	std::array<iREAL, 3> getVertexB();
	std::array<iREAL, 3> getVertexC();

	void flatten (
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	iREAL getTriangleLength();

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
	std::array<iREAL, 3> _A, _B, _C;

};

#endif /* GEOMETRY_MESH_TRIANGLE_H_ */
