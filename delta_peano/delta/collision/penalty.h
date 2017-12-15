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

#include "delta/collision/contactpoint.h"
#include <limits>
#include <float.h>
#include "delta/core/algo.h"

#define MaxNumberOfNewtonIterations 16

namespace delta {
  namespace collision {
    void cleanPenaltyStatistics();

    /**
     * The penalty() operation that accepts whole batches of triangles, i.e.
     * triangulated bodies, keeps some statistics about the number of
     * comparisons. You can retrieve this information with this routine,
     * and clean the statistics with cleanPenaltyStatistics(). The whole
     * statistics stuff is not thread-safe.
     */
    std::vector<int> getPenaltyStatistics();


    /**
     *
     * @see hybrid() or bf() for a description of the remaining parameters.
     */
    std::vector<contactpoint> penaltyStat(
      int       numberOfTrianglesOfGeometryA,
      const iREAL*   xCoordinatesOfPointsOfGeometryA,
      const iREAL*   yCoordinatesOfPointsOfGeometryA,
      const iREAL*   zCoordinatesOfPointsOfGeometryA,
      iREAL    epsilonA,
      bool      frictionA,
      int 	    particleA,

      int       numberOfTrianglesOfGeometryB,
      const iREAL*   xCoordinatesOfPointsOfGeometryB,
      const iREAL*   yCoordinatesOfPointsOfGeometryB,
      const iREAL*   zCoordinatesOfPointsOfGeometryB,
      iREAL    epsilonB,
      bool      frictionB,
      int 	    particleB);

    std::vector<contactpoint> penalty(
      int       numberOfTrianglesOfGeometryA,
      const iREAL*   xCoordinatesOfPointsOfGeometryA,
      const iREAL*   yCoordinatesOfPointsOfGeometryA,
      const iREAL*   zCoordinatesOfPointsOfGeometryA,
      iREAL    epsilonA,
      bool      frictionA,
      int 	    particleA,

      int       numberOfTrianglesOfGeometryB,
      const iREAL*   xCoordinatesOfPointsOfGeometryB,
      const iREAL*   yCoordinatesOfPointsOfGeometryB,
      const iREAL*   zCoordinatesOfPointsOfGeometryB,
      iREAL    epsilonB,
      bool      frictionB,
      int 	    particleB);

    #if defined(ompParticle) || defined(ompTriangle)
      #pragma omp declare simd
      #pragma omp declare simd linear(xCoordinatesOfTriangleA:3) linear(yCoordinatesOfTriangleA:3) linear(zCoordinatesOfTriangleA:3) linear(xCoordinatesOfTriangleB:3) linear(yCoordinatesOfTriangleB:3) linear(zCoordinatesOfTriangleB:3) nomask notinbranch
    #endif
    extern void penalty(
      const iREAL   *xCoordinatesOfTriangleA,
       const iREAL   *yCoordinatesOfTriangleA,
       const iREAL   *zCoordinatesOfTriangleA,
       const iREAL   *xCoordinatesOfTriangleB,
       const iREAL   *yCoordinatesOfTriangleB,
       const iREAL   *zCoordinatesOfTriangleB,
       iREAL&  xPA,
       iREAL&  yPA,
       iREAL&  zPA,
       iREAL&  xPB,
       iREAL&  yPB,
       iREAL&  zPB,
       iREAL MaxErrorOfPenaltyMethod,
       bool& failed);

    /**
     * This is a second variant of the penalty method. Different to the one
     * above, this one does terminate as soon as the error underruns maxError
     * or the number of itertions exceeds maxNumberOfNewtonIterations.
     */
    void penalty(
      const iREAL   xCoordinatesOfTriangleA[],
      const iREAL   yCoordinatesOfTriangleA[],
      const iREAL   zCoordinatesOfTriangleA[],
      const iREAL   xCoordinatesOfTriangleB[],
      const iREAL   yCoordinatesOfTriangleB[],
      const iREAL   zCoordinatesOfTriangleB[],
      iREAL&  xPA,
      iREAL&  yPA,
      iREAL&  zPA,
      iREAL&  xPB,
      iREAL&  yPB,
      iREAL&  zPB,
      iREAL   maxError,
      int&     numberOfNewtonIterationsRequired);
  }
}
