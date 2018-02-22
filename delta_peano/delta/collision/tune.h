
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

#include <vector>
#include "delta/collision/contactpoint.h"
#include <limits>
#include <float.h>
#include "delta/core/algo.h"
#include "delta/collision/bf.h"

namespace delta {
  namespace collision {
/*
    struct triangleSetStat {
      iREAL T1A[3], T1B[3], T1C[3];
      iREAL T2A[3], T2B[3], T2C[3];

      std::vector<double> epsilonHistory;

      std::vector<double> rpointDifference;
      std::vector<double> rdistanceDifference;

      std::vector<double> pointFail;
      std::vector<double> distanceFail;
    };

    void computeHistogram(std::vector<triangleSetStat> particleTriangleSet);
  */
    void penaltyTune(
      int             numberOfTrianglesOfGeometryA,
      const iREAL*    xCoordinatesOfPointsOfGeometryA,
      const iREAL*    yCoordinatesOfPointsOfGeometryA,
      const iREAL*    zCoordinatesOfPointsOfGeometryA,
      iREAL           epsilonA,
      bool            frictionA,
      int             particleA,

      int             numberOfTrianglesOfGeometryB,
      const iREAL*    xCoordinatesOfPointsOfGeometryB,
      const iREAL*    yCoordinatesOfPointsOfGeometryB,
      const iREAL*    zCoordinatesOfPointsOfGeometryB,
      iREAL           epsilonB,
      bool            frictionB,
      int             particleB);

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
