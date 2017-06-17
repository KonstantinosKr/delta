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
#include <delta/geometry/surface.h>
#include "properties.h"

namespace delta {
  namespace geometry {
    namespace triangle {
      void bisectTriangle(
          double xT[3],
          double yT[3],
          double zT[3],
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates);

      void triSectTriangle(
          double A[3],
          double B[3],
          double C[3],
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates);

      void fiveSectTriangle(
          double A[3],
          double B[3],
          double C[3],
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates);

      void meshDenser(
          int multiplier,
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates);

      int meshOctSect(int quadsectTimes,
          std::vector<std::vector<double>>&  xCoordinates,
          std::vector<std::vector<double>>&  yCoordinates,
          std::vector<std::vector<double>>&  zCoordinates, std::vector<std::array<double, 3>>& centroid);

      int octSect(int level, int index, std::array<double, 3> minpoint, std::array<double, 3> midpoint, std::array<double, 3> maxpoint,
          std::vector<std::vector<double>>&  xCoordinatesVec,
          std::vector<std::vector<double>>&  yCoordinatesVec,
          std::vector<std::vector<double>>&  zCoordinatesVec, std::vector<std::array<double, 3>>& centroid);

      void getTrianglesInBoundingBox(std::array<double, 3> minpoint, std::array<double, 3> maxpoint,
          std::vector<double>&  xCoordinatesRoot,
          std::vector<double>&  yCoordinatesRoot,
          std::vector<double>&  zCoordinatesRoot,
          std::vector<double>&  xCoordinatesBounded,
          std::vector<double>&  yCoordinatesBounded,
          std::vector<double>&  zCoordinatesBounded);
    }
  }
}



#endif /* DELTA_geometry_TRIANGLE_H_ */
