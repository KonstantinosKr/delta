/*
 * properties.h
 *
 *  Created on: 6 Apr 2018
 *      Author: konstantinos
 */

#include <delta/geometry/material.h>
#include <delta/core/algo.h>

#include <vector>

#ifndef GEOMETRY_OPERATORS_H_
#define GEOMETRY_OPERATORS_H_

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace physics {

		/*
		 *  Get Volume
		 *
		 *  return volume of geomety
		 *  @param none
		 *  @return iREAL
		 */
		iREAL computeVolume(
			std::vector<iREAL>& xCoordinates,
			std::vector<iREAL>& yCoordinates,
			std::vector<iREAL>& zCoordinates);

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
			std::vector<iREAL>& xCoordinates,
			std::vector<iREAL>& yCoordinates,
			std::vector<iREAL>& zCoordinates,
			delta::geometry::material::MaterialType material,
			iREAL& mass,
			iREAL center[3],
			iREAL inertia[9]);

		/*
		 *  Get Mass
		 *
		 *  Returns mass of mesh give a material.
		 *
		 *
		 *  @param xCoordinates
		 *  @param yCoordinates
		 *  @param zCoordinates
		 *  @param material
		 *  @returns iREAL
		 */
		iREAL computeMass(
			std::vector<iREAL>& xCoordinates,
			std::vector<iREAL>& yCoordinates,
			std::vector<iREAL>& zCoordinates,
			delta::geometry::material::MaterialType material);

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

		/*
		 * simplex integration from solfec
		 */
		iREAL simplex_J(
			iREAL *a, iREAL *b, iREAL *c, iREAL *d);
	  }
	}
  } /* namespace geometry */
} /* namespace delta */

#endif /* GEOMETRY_OPERATORS_H_ */
