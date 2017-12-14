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
#include <complex>
#include <limits>
#include <iostream>
#include "delta/core/algo.h"


namespace delta {
  namespace collision {

    /**
     * @see hybrid() function with the same arguments for default
     */
    std::vector<contactpoint> bf(
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
      int 	    particleB
    );


    /**
     * Brute force method for finite computation of the minimum distance
     * between two triangles A and B using the geometric comparisons segment to
     * segment and triangle to point. It returns two points PA and PB on
     * triangle A and B, respectively. These are the two points that are
     * closests to each other on the respective triangles.
     *
     * @param xPA Output parameter. x coordinate on triangle A.
     */
    #if defined(ompParticle) || defined(ompTriangle)
    #pragma omp declare simd
    #endif
    void bf(
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
      iREAL&  zPB
    );
  }
}
