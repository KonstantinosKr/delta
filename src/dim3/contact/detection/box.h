/*
 * cube.h
 *
 *  Created on: 28 Aug 2018
 *      Author: konstantinos
 */

#ifndef DELTA_CONTACT_DETECTION_BOX_H_
#define DELTA_CONTACT_DETECTION_BOX_H_

#include <array>
#include <vector>

namespace delta {
  namespace contact {
	namespace detection {

		bool box(
			std::array<iREAL, 6> A,
			iREAL epsilonA,
			std::array<iREAL, 6> B,
			iREAL epsilonB);

		bool isInBox(
			iREAL x[3],
			std::array<iREAL,6> bbox);

	} /* namespace detection */
  } /* namespace contact */
} /* namespace delta */

#endif /* DELTA_CONTACT_DETECTION_BOX_H_ */
