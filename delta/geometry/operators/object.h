/*
 * object.h
 *
 *  Created on: 24 Jul 2018
 *      Author: konstantinos
 */

#ifndef DELTA_GEOMETRY_OPERATORS_OBJECT_H_
#define DELTA_GEOMETRY_OPERATORS_OBJECT_H_

#include <delta/geometry/Object.h>
#include <vector>
#include <array>
#include <limits>

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace object {

		/*
		 *  Compute Boundary
		 *
		 *  compute boundaries of the computational domain
		 *
		 *  @param position				: position of the layout
		 *  @param width 				: width of the geometry
		 *  @param layers				: layers of decks
		 *  @param epsilon 				: epsilon margin
		 *  @param objects				: objects returned
		 *  @returns void
		 */
		void computeBoundary(
			 std::vector<delta::geometry::Object>& objects,
			 iREAL& minParticleDiam,
			 iREAL& maxParticleDiam,
			 iREAL* minComputeDomain,
			 iREAL* maxComputeDomain);
	  } /* namespace Object */
	} /* namespace operators */
  } /* namespace geometry */
} /* namespace delta */

#endif /* DELTA_GEOMETRY_OPERATORS_OBJECT_H_ */
