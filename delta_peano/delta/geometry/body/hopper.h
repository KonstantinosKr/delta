/*
 The MIT License (MIT)

 Copyright (c) 26 May 2016 Konstantinos Krestenitis

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

#ifndef DELTA_geometry_HOPPER_H_
#define DELTA_geometry_HOPPER_H_

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <assert.h>

#include "delta/geometry/operators/hull/hull.h"
#include "delta/geometry/operators/hull/alg.h"
#include "delta/geometry/properties.h"
#include "delta/geometry/operators/triangle.h"

namespace delta {
  namespace world {
	namespace body {
	  void generateInnerHopper(
			iREAL  center[3],
			iREAL  width,
			iREAL  height,
			iREAL  hatch,
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

	  void generateOuterHopper(
			iREAL  center[3],
			iREAL  width,
			iREAL  height,
			iREAL  hatch,
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

		void generateHopper(
			iREAL  center[3],
			iREAL  width,
			iREAL  thickness,
			iREAL  height,
			iREAL  hatch,
			int    meshRefinement,
			double gridH,
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);
	}
  }
}




#endif /* DELTA_geometry_HOPPER_H_ */
