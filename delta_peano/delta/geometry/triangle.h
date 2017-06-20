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

#ifndef DELTA_geometry_TRIANGLE_H_
#define DELTA_geometry_TRIANGLE_H_


#include <vector>
#include <array>
#include <stdlib.h>
#include <assert.h>
#include <cmath>
#include <delta/geometry/surface.h>
#include "properties.h"

namespace delta {
  namespace geometry {
    namespace triangle {
      void bisectTriangle(
          iREAL xT[3],
          iREAL yT[3],
          iREAL zT[3],
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void triSectTriangle(
          iREAL A[3],
          iREAL B[3],
          iREAL C[3],
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void fiveSectTriangle(
          iREAL A[3],
          iREAL B[3],
          iREAL C[3],
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void meshDenser(
          int multiplier,
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      int meshOctSect(
          int quadsectTimes,
          std::vector<std::vector<iREAL>>&  xCoordinates,
          std::vector<std::vector<iREAL>>&  yCoordinates,
          std::vector<std::vector<iREAL>>&  zCoordinates,
          std::vector<std::array<iREAL, 3>>& centroid);

      int octSect(
          int level,
          int index,
          std::array<iREAL, 3> minpoint,
          std::array<iREAL, 3> midpoint,
          std::array<iREAL, 3> maxpoint,
          std::vector<std::vector<iREAL>>&  xCoordinatesVec,
          std::vector<std::vector<iREAL>>&  yCoordinatesVec,
          std::vector<std::vector<iREAL>>&  zCoordinatesVec,
          std::vector<std::array<iREAL, 3>>& centroid);

      void getTrianglesInBoundingBox(
          std::array<iREAL, 3> minpoint,
          std::array<iREAL, 3> maxpoint,
          std::vector<iREAL>&  xCoordinatesRoot,
          std::vector<iREAL>&  yCoordinatesRoot,
          std::vector<iREAL>&  zCoordinatesRoot,
          std::vector<iREAL>&  xCoordinatesBounded,
          std::vector<iREAL>&  yCoordinatesBounded,
          std::vector<iREAL>&  zCoordinatesBounded);
    }
  }
}



#endif /* DELTA_geometry_TRIANGLE_H_ */