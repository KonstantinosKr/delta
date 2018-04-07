/*
 * properties.h
 *
 *  Created on: 6 Apr 2018
 *      Author: konstantinos
 */

#include "delta/geometry/material.h"
#include "delta/core/algo.h"

#include <vector>

#ifndef GEOMETRY_OPERATORS_H_
#define GEOMETRY_OPERATORS_H_

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace physics {

		iREAL computeVolume(
			std::vector<iREAL>& xCoordinates,
			std::vector<iREAL>& yCoordinates,
			std::vector<iREAL>& zCoordinates);

		/*
		 * gets the inertia using simplex integration from solfec
		 */
		void computeInertia(
			std::vector<iREAL>& xCoordinates,
			std::vector<iREAL>& yCoordinates,
			std::vector<iREAL>& zCoordinates,
			delta::geometry::material::MaterialType material,
			iREAL& mass,
			iREAL center[3],
			iREAL inertia[9]);

		iREAL computeMass(
			std::vector<iREAL>& xCoordinates,
			std::vector<iREAL>& yCoordinates,
			std::vector<iREAL>& zCoordinates,
			delta::geometry::material::MaterialType material);

		void computeInverseInertia(
			iREAL inertia[9],
			iREAL inverse[9],
			bool isObject);

		iREAL simplex_J(
			iREAL *a, iREAL *b, iREAL *c, iREAL *d);
	  }
	}
  } /* namespace geometry */
} /* namespace delta */

#endif /* GEOMETRY_OPERATORS_H_ */
