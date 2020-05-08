/*
 * pairwise.h
 *
 *  Created on: 12 Jun 2018
 *      Author: konstantinos
 */

#ifndef METRICS_PAIRWISE_H_
#define METRICS_PAIRWISE_H_

#include <vector>

#include "../../geometry/structure/Mesh.h"

namespace delta {
  namespace metrics {
	namespace pairwise {

	  void pointToMeshDistance(
		  iREAL xV,
		  iREAL yV,
		  iREAL zV,

		  std::vector<iREAL>& xCoordinates,
		  std::vector<iREAL>& yCoordinates,
		  std::vector<iREAL>& zCoordinates,

		  std::vector<iREAL>& xP,
		  std::vector<iREAL>& yP,
		  std::vector<iREAL>& zP,

		  std::vector<iREAL>& distance);

		iREAL pointToTriangleDistance(
			  iREAL xV,
			  iREAL yV,
			  iREAL zV,

			  iREAL& xA,
			  iREAL& yA,
			  iREAL& zA,

			  iREAL& xB,
			  iREAL& yB,
			  iREAL& zB,

			  iREAL& xC,
			  iREAL& yC,
			  iREAL& zC,

			  iREAL &xP,
			  iREAL &yP,
			  iREAL &zP);
	}
  } /* namespace metrics */
} /* namespace delta */

#endif /* DELTA_METRICS_PAIRWISE_H_ */
