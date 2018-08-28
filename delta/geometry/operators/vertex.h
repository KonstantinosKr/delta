/*
 * vertex.h
 *
 *  Created on: 24 Aug 2018
 *      Author: konstantinos
 */

#ifndef DELTA_GEOMETRY_OPERATORS_VERTEX_H_
#define DELTA_GEOMETRY_OPERATORS_VERTEX_H_

#include <vector>
#include <array>
#include <cmath>

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace vertex {

		iREAL computeXYZw(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);
		iREAL computeXZw(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);
		iREAL computeXw(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);
		iREAL computeYw(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);
		iREAL computeZw(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);

		std::array<iREAL, 3> computeBoundaryMinVertex(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);

		std::array<iREAL, 3> computeBoundaryMaxVertex(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);

		iREAL computeDiagonal(
			std::vector<iREAL> xCoordinates,
			std::vector<iREAL> yCoordinates,
			std::vector<iREAL> zCoordinates);

		iREAL computeDistanceAB(
		    std::array<iREAL, 3> A,
		    std::array<iREAL, 3> B);

		iREAL computeMin(
			  std::vector<iREAL> coordinates);

		iREAL computeMax(
			  std::vector<iREAL> coordinates);

	  }
	} /* namespace operators */
  } /* namespace geometry */
} /* namespace delta */

#endif /* DELTA_GEOMETRY_OPERATORS_VERTEX_H_ */
