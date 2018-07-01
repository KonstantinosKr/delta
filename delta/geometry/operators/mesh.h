/*
 * mesh.h
 *
 *  Created on: 10 May 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_OPERATORS_MESH_H_
#define GEOMETRY_OPERATORS_MESH_H_

#include <vector>
#include <cmath>

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace mesh {

	    void moveMeshFromPositionToOrigin(
		    std::vector<iREAL> &xCoordinates,
		    std::vector<iREAL> &yCoordinates,
		    std::vector<iREAL> &zCoordinates,
		    iREAL center[3]);

	    void moveMeshFromOriginToPosition(
		    std::vector<iREAL> &xCoordinates,
		    std::vector<iREAL> &yCoordinates,
		    std::vector<iREAL> &zCoordinates,
		    iREAL center[3]);

		void scaleXYZ(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
		    iREAL scale,
		    iREAL position[3]);

		void rotateX(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL alphaX);

		void rotateY(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL alphaY);

		void rotateZ(
			std::vector<iREAL> &xCoordinates,
			std::vector<iREAL> &yCoordinates,
			std::vector<iREAL> &zCoordinates,
			iREAL alphaZ);



		void moveMeshFromPositionToOrigin(
		    std::vector<iREAL> &points,
		    iREAL center[3]);

		void moveMeshFromOriginToPosition(
		    std::vector<iREAL> &points,
		    iREAL center[3]);

		void scaleXYZ(
		    std::vector<iREAL> &points,
		    iREAL scale,
		    iREAL position[3]);
	  }
	} /* namespace operators */
  } /* namespace geometry */
} /* namespace delta */

#endif /* GEOMETRY_OPERATORS_MESH_H_ */
