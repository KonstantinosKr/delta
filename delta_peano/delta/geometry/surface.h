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

#ifndef DELTA_geometry_SURFACE_H_
#define DELTA_geometry_SURFACE_H_


#include <vector>
#include <array>

#include "delta/hull/hull.h"
#include "delta/hull/alg.h"
#include "delta/geometry/surface.h"

#include "properties.h"

namespace delta {
  namespace geometry {
    namespace surface {
      void generateBoundBox(
        double center[3],
        std::array<double, 3> minPoint,
        std::array<double, 3> maxPoint,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates
      );

      void generateSurface(
        double    center[3],
        double 	width,
        double	height,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates
      );

      void generateXYZBox(
          double center[3],
          double xw,
          double yw,
          double zw,
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates
      );

      void generateCuboid(
        double    center[3],
        double 	width,
        double    height,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates
      );
    }
  }
}



#endif /* DELTA_geometry_SURFACE_H_ */
