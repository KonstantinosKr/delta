/*
 The MIT License (MIT)

 Copyright (c) 2015 Konstantinos Krestenitis

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


#include <vector>

#include <cmath>
#include <assert.h>
#include <stdlib.h>

#include "delta/geometry/operator/hull/hull.h"
#include "delta/geometry/operator/hull/alg.h"
#include "delta/geometry/primitive/triangle.h"

namespace delta {
  namespace geometry {
	namespace primitive {
	  namespace cube {
		/**
		 * Generates a square
		 *
		 * @param center
		 */
		 void generateSquare(
		   iREAL center[2],
		   iREAL h,
		   iREAL rotationAngle,
		   std::vector<iREAL>&  xCoordinates,
		   std::vector<iREAL>&  yCoordinates
		 );

		 void generateCube(
		   iREAL    center[3],
		   iREAL    h,
		   iREAL    alphaX,
		   iREAL    alphaY,
		   iREAL    alphaZ,
		   std::vector<iREAL>&  xCoordinates,
		   std::vector<iREAL>&  yCoordinates,
		   std::vector<iREAL>&  zCoordinates
		 );

		 void generateCube(
		   iREAL center[3],
		   iREAL x,
		   iREAL y,
		   iREAL z,
		   iREAL alphaX,
		   iREAL alphaY,
		   iREAL alphaZ,
		   std::vector<iREAL>&  xCoordinates,
		   std::vector<iREAL>&  yCoordinates,
		   std::vector<iREAL>&  zCoordinates
		 );

		 void generateHullCube(
		   iREAL  center[3],
		   iREAL  diagonal,
		   std::vector<iREAL>&  xCoordinates,
		   std::vector<iREAL>&  yCoordinates,
		   std::vector<iREAL>&  zCoordinates);

		 void generateHullCube(
			   iREAL  center[3],
			   iREAL x,
			   iREAL y,
			   iREAL z,
			   iREAL alphaX,
			   iREAL alphaY,
			   iREAL alphaZ,
			   int    meshmultiplier,
			   std::vector<iREAL>&  xCoordinates,
			   std::vector<iREAL>&  yCoordinates,
			   std::vector<iREAL>&  zCoordinates);
	  }
	}
  }
}
