/*
 * operators.h
 *
 *  Created on: 19 Apr 2018
 *      Author: konstantinos
 */

#ifndef WORLD_OPERATORS_H_
#define WORLD_OPERATORS_H_

#include <delta/geometry/Object.h>
#include <vector>
#include <array>

namespace delta {
namespace world {
namespace operators {

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
	 std::vector<delta::geometry::Object>& coarseObjects,
	 std::vector<delta::geometry::Object>& fineObjects,
	 std::vector<delta::geometry::Object>& insitufineObjects,
	 iREAL& minParticleDiam,
	 iREAL& maxParticleDiam,
	 iREAL* minComputeDomain,
	 iREAL* maxComputeDomain);

} /* namespace operators */
} /* namespace world */
} /* namespace delta */

#endif /* WORLD_OPERATORS_H_ */
