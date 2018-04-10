
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

#include <map>
#include <vector>
#include <limits>
#include <float.h>
#include "delta/core/algo.h"
#include <delta/contact/detection/bf.h>
#include <delta/contact/contactpoint.h>

namespace delta {
  namespace contact {
	namespace detection {
	/*
    struct triangleSetStat {
      iREAL T1A[3], T1B[3], T1C[3];
      iREAL T2A[3], T2B[3], T2C[3];

      std::vector<iREAL> epsilonHistory;

      std::vector<iREAL> rpointDifference;
      std::vector<iREAL> rdistanceDifference;

      std::vector<iREAL> pointFail;
      std::vector<iREAL> distanceFail;
    };

    void computeHistogram(std::vector<triangleSetStat> particleTriangleSet);
  */
    void penaltyTune(
      const iREAL*    xCoordinatesOfPointsOfGeometryA,
      const iREAL*    yCoordinatesOfPointsOfGeometryA,
      const iREAL*    zCoordinatesOfPointsOfGeometryA,
      const int       numberOfTrianglesOfGeometryA,
      const iREAL     epsilonA,
      const bool      frictionA,
      const int       particleA,

      const iREAL*    xCoordinatesOfPointsOfGeometryB,
      const iREAL*    yCoordinatesOfPointsOfGeometryB,
      const iREAL*    zCoordinatesOfPointsOfGeometryB,
      const int       numberOfTrianglesOfGeometryB,
      const iREAL     epsilonB,
      const bool      frictionB,
      const int       particleB);

    void penalty(
      const iREAL   e,
      const iREAL   r,
      const iREAL   xCoordinatesOfTriangleA[],
      const iREAL   yCoordinatesOfTriangleA[],
      const iREAL   zCoordinatesOfTriangleA[],
      const iREAL   xCoordinatesOfTriangleB[],
      const iREAL   yCoordinatesOfTriangleB[],
      const iREAL   zCoordinatesOfTriangleB[],
      iREAL&        xPA,
      iREAL&        yPA,
      iREAL&        zPA,
      iREAL&        xPB,
      iREAL&        yPB,
      iREAL&        zPB,
      iREAL         maxError,
      int&          maxNumberOfNewtonIterations,
      int&          numberOfNewtonIterationsRequired);
	}
  }
}
