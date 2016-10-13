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


namespace delta {
  namespace collision {
    /**
     * Hybrid contact detection
     *
     * The hybrid contact detection algorithm first runs the penalty contact
     * detection for a fixed number of sweeps. It then checks for convergence.
     * If it does not converge, it falls back to the brute force method.
     *
     * In a second step, the operation runs through all contact points (nearest
     * neighbours) and takes those that are closer than epsilon. All other
     * nearest neighbour pairs are neglected. The remaining points make the
     * algorithm befill the result data structure.
     *
     * The hybrid configuration parameters all are held as compile time
     * parameters. This is not that elegant from a software point of view.
     * However, it allows the code to optimise aggressively at compile time.
     *
     * @return Set of contact points.
     */
    std::vector<contactpoint> hybridWithPerTriangleFallBack(
      int       numberOfTrianglesOfGeometryA,
      double*   xCoordinatesOfPointsOfGeometryA,
      double*   yCoordinatesOfPointsOfGeometryA,
      double*   zCoordinatesOfPointsOfGeometryA,
	  double    epsilonA,
	  int       materialA,

      int       numberOfTrianglesOfGeometryB,
      double*   xCoordinatesOfPointsOfGeometryB,
      double*   yCoordinatesOfPointsOfGeometryB,
      double*   zCoordinatesOfPointsOfGeometryB,
      double    epsilonB,
	  int       materialB
    );

    std::vector<contactpoint> hybridWithPerBatchFallBack(
      int       numberOfTrianglesOfGeometryA,
      double*   xCoordinatesOfPointsOfGeometryA,
      double*   yCoordinatesOfPointsOfGeometryA,
      double*   zCoordinatesOfPointsOfGeometryA,
	  double    epsilonA,
	  int       materialA,

      int       numberOfTrianglesOfGeometryB,
      double*   xCoordinatesOfPointsOfGeometryB,
      double*   yCoordinatesOfPointsOfGeometryB,
      double*   zCoordinatesOfPointsOfGeometryB,
      double    epsilonB,
	  int       materialB
    );

    std::vector<contactpoint> hybridWithPerTriangleFallBackStats(
      int       numberOfTrianglesOfGeometryA,
      double*   xCoordinatesOfPointsOfGeometryA,
      double*   yCoordinatesOfPointsOfGeometryA,
      double*   zCoordinatesOfPointsOfGeometryA,
	  double    epsilonA,
	  int       materialA,

      int       numberOfTrianglesOfGeometryB,
      double*   xCoordinatesOfPointsOfGeometryB,
      double*   yCoordinatesOfPointsOfGeometryB,
      double*   zCoordinatesOfPointsOfGeometryB,
      double    epsilonB,
	  int       materialB
    );

    std::vector<contactpoint> hybridWithPerBatchFallBackStats(
      int       numberOfTrianglesOfGeometryA,
      double*   xCoordinatesOfPointsOfGeometryA,
      double*   yCoordinatesOfPointsOfGeometryA,
      double*   zCoordinatesOfPointsOfGeometryA,
	  double    epsilonA,
	  int       materialA,

      int       numberOfTrianglesOfGeometryB,
      double*   xCoordinatesOfPointsOfGeometryB,
      double*   yCoordinatesOfPointsOfGeometryB,
      double*   zCoordinatesOfPointsOfGeometryB,
      double    epsilonB,
	  int       materialB
    );
  }
}


