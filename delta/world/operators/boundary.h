/*
 The MIT License (MIT)

 Copyright (c) 2018 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef WORLD_OPERATORS_H_
#define WORLD_OPERATORS_H_

#include <delta/geometry/Object.h>
#include <vector>
#include <array>
#include <limits>

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
