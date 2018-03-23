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


#include <delta/contact/detection/bf.h>
#include <delta/contact/contactpoint.h>
#include <delta/contact/detection/penalty.h>
#include <vector>
#include "peano/utils/Loop.h"
#include "tarch/multicore/Lock.h"
#include "tarch/multicore/BooleanSemaphore.h"

namespace delta {
  namespace contact {
	namespace detection {
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
		int             numberOfTrianglesOfGeometryA,
		const iREAL*    xCoordinatesOfPointsOfGeometryA,
		const iREAL*    yCoordinatesOfPointsOfGeometryA,
		const iREAL*    zCoordinatesOfPointsOfGeometryA,
		iREAL           epsilonA,
		bool            frictionA,
		int 	          particleA,

		int             numberOfTrianglesOfGeometryB,
		const iREAL*    xCoordinatesOfPointsOfGeometryB,
		const iREAL*    yCoordinatesOfPointsOfGeometryB,
		const iREAL*    zCoordinatesOfPointsOfGeometryB,
		iREAL           epsilonB,
		bool            frictionB,
		int 	          particleB,
		tarch::multicore::BooleanSemaphore &semaphore);

	  std::vector<contactpoint> hybridWithPerBatchFallBack(
		int             numberOfTrianglesOfGeometryA,
		const iREAL*    xCoordinatesOfPointsOfGeometryA,
		const iREAL*    yCoordinatesOfPointsOfGeometryA,
		const iREAL*    zCoordinatesOfPointsOfGeometryA,
		iREAL           epsilonA,
		bool            frictionA,
		int 	          particleA,

		int             numberOfTrianglesOfGeometryB,
		const iREAL*    xCoordinatesOfPointsOfGeometryB,
		const iREAL*    yCoordinatesOfPointsOfGeometryB,
		const iREAL*    zCoordinatesOfPointsOfGeometryB,
		iREAL           epsilonB,
		bool            frictionB,
		int 	          particleB,
		tarch::multicore::BooleanSemaphore &semaphore);


	  std::vector<contactpoint> hybridBatchStat(
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

	  std::vector<contactpoint> hybridTriangleStat(
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

		static int numberOfPenaltyFails;
		static int numberOfBatchFails;
		static int batchSize;
		static iREAL batchError;

		void cleanHybridStatistics();
		int getPenaltyFails();
		int getBatchFails();
		int getBatchSize();
		iREAL getBatchError();
	}
  }
}


