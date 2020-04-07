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
#include <complex>
#include <limits>
#include <iostream>

#include <delta/contact/contactpoint.h>

namespace delta {
  namespace contact {
	namespace detection {
	  std::vector<contactpoint> sphere(
		const iREAL   xCoordinatesOfPointsOfGeometryA,
		const iREAL   yCoordinatesOfPointsOfGeometryA,
		const iREAL   zCoordinatesOfPointsOfGeometryA,
		const iREAL   diameterA,
		const iREAL   epsilonA,
		const bool    frictionA,
		const int 	  particleA,

		const iREAL   xCoordinatesOfPointsOfGeometryB,
		const iREAL   yCoordinatesOfPointsOfGeometryB,
		const iREAL   zCoordinatesOfPointsOfGeometryB,
		const iREAL   diameterB,
		const iREAL   epsilonB,
		const bool    frictionB,
		const int 	  particleB
		);

	  std::vector<contactpoint> sphereWithBarrierAB(
		const iREAL   xCoordinatesOfPointsOfGeometryA,
		const iREAL   yCoordinatesOfPointsOfGeometryA,
		const iREAL   zCoordinatesOfPointsOfGeometryA,
		const iREAL   diameterA,
		const iREAL   epsilonA,
		const bool    frictionA,
		const int	  	particleA,

		const iREAL   *xCoordinatesOfPointsOfGeometryB,
		const iREAL   *yCoordinatesOfPointsOfGeometryB,
		const iREAL   *zCoordinatesOfPointsOfGeometryB,
		const int	  	numberOfTrianglesOfGeometryB,
		const iREAL   epsilonB,
		const bool 	  frictionB,
		const int 	  particleB
		);

	  std::vector<contactpoint> sphereWithBarrierBA(
		const iREAL   xCoordinatesOfPointsOfGeometryA,
		const iREAL   yCoordinatesOfPointsOfGeometryA,
		const iREAL   zCoordinatesOfPointsOfGeometryA,
		const iREAL   diameterA,
		const iREAL   epsilonA,
		const bool    frictionA,
		const int 	  particleA,

		const iREAL   *xCoordinatesOfPointsOfGeometryB,
		const iREAL   *yCoordinatesOfPointsOfGeometryB,
		const iREAL   *zCoordinatesOfPointsOfGeometryB,
		const int     numberOfTrianglesOfGeometryB,
		const iREAL   epsilonB,
		const bool    frictionB,
		const int 	  particleB
		);

	  bool isSphereOverlayInContact(
		const iREAL   xCoordinatesOfPointsOfGeometryA,
		const iREAL   yCoordinatesOfPointsOfGeometryA,
		const iREAL   zCoordinatesOfPointsOfGeometryA,
		const iREAL   radiusA,

		const iREAL   xCoordinatesOfPointsOfGeometryB,
		const iREAL   yCoordinatesOfPointsOfGeometryB,
		const iREAL   zCoordinatesOfPointsOfGeometryB,
		const iREAL   radiusB
	  );
	}
  }
}
