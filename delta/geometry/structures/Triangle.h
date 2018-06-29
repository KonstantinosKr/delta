/*
 * triangles.h
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_STRUCTURES_TRIANGLE_H_
#define GEOMETRY_STRUCTURES_TRIANGLE_H_

#include <map>
#include <vector>
#include <array>
#include <cmath>
#include <limits>

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

	/*
	 *  Gets Vertex A
	 *
	 *  Returns the A vertex of the triangle
	 *
	 *  @param none
	 *  @returns std::array of three elements of type iREAL
	 */
	std::array<iREAL, 3> getVertexA();

	/*
	 *  Gets Vertex B
	 *
	 *  Returns the B vertex of the triangle
	 *
	 *  @param none
	 *  @returns std::array of three elements of type iREAL
	 */
	std::array<iREAL, 3> getVertexB();

	/*
	 *  Gets Vertex C
	 *
	 *  Returns the C vertex of the triangle
	 *
	 *  @param none
	 *  @returns std::array of three elements of type iREAL
	 */
	std::array<iREAL, 3> getVertexC();

	/*
	 *  Gets Minimum Boundary Vertex
	 *
	 *  Returns the minimum vertex that defines the bounding box of the triangle
	 *
	 *  @param none
	 *  @returns std::array of three elements of type iREAL
	 */
	std::array<iREAL, 3> getMinBoundaryVertex();

	/*
	 *  Gets Maximum Boundary Vertex
	 *
	 *  Returns the maximum vertex that defines the bounding box of the triangle
	 *
	 *  @param none
	 *  @returns std::array of three elements of type iREAL
	 */
	std::array<iREAL, 3> getMaxBoundaryVertex();

	/*
	 *  Gets Triangle Length
	 *
	 *  Returns maximum line segment length of triangle
	 *
	 *  @param none
	 *  @returns std::array of three elements of type iREAL
	 */
	iREAL getTriangleLength();

	/*
	 *  Gets Width X
	 *
	 *  Returns width of x axis coordinates
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getXw();

	/*
	 *  Gets Width Y
	 *
	 *  Returns width of y axis coordinates
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getYw();

	/*
	 *  Gets Width Y
	 *
	 *  Returns width of y axis coordinates
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getZw();

	/*
	 *  Gets Max X
	 *
	 *  Returns max x axis value
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getMaxXAxis();

	/*
	 *  Gets Max X
	 *
	 *  Returns max x axis value
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getMaxYAxis();

	/*
	 *  Gets Max X
	 *
	 *  Returns max x axis value
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getMaxZAxis();


	/*
	 *  Gets Max X
	 *
	 *  Returns max x axis value
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getMinXAxis();

	/*
	 *  Gets Max Y
	 *
	 *  Returns max y axis value
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getMinYAxis();

	/*
	 *  Gets Max Z
	 *
	 *  Returns max z axis value
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getMinZAxis();

	virtual ~Triangle();
  private:

	/*
	 *  Gets XYZ width
	 *
	 *  Returns width on xyz axis.
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getXYZWidth();

	/*
	 *  Gets XY width
	 *
	 *  Returns width on xy axis.
	 *
	 *  @param none
	 *  @returns type iREAL
	 */
	iREAL getXZWidth();

	std::array<iREAL, 3> _A, _B, _C;

};

#endif /* GEOMETRY_MESH_TRIANGLE_H_ */
