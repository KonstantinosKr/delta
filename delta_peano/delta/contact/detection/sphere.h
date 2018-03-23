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

#include <delta/contact/contactpoint.h>
#include <vector>
#include <complex>
#include <limits>
#include <iostream>

#include "dem/mappings/Collision.h"
#include "delta/core/algo.h"
#include "dem/mappings/Collision.h"


namespace delta {
  namespace contact {
	namespace detection {

	  std::vector<contactpoint> sphere(
		iREAL   xCoordinatesOfPointsOfGeometryA,
		iREAL   yCoordinatesOfPointsOfGeometryA,
		iREAL   zCoordinatesOfPointsOfGeometryA,
		iREAL   diameterA,
		iREAL   epsilonA,
		bool    frictionA,
		int 	  	particleA,

		iREAL   xCoordinatesOfPointsOfGeometryB,
		iREAL   yCoordinatesOfPointsOfGeometryB,
		iREAL   zCoordinatesOfPointsOfGeometryB,
		iREAL   diameterB,
		iREAL   epsilonB,
		bool    frictionB,
		int 	  	particleB
		);

	  std::vector<contactpoint> sphereWithBarrierAB(
		iREAL   xCoordinatesOfPointsOfGeometryA,
		iREAL   yCoordinatesOfPointsOfGeometryA,
		iREAL   zCoordinatesOfPointsOfGeometryA,
		iREAL   diameterA,
		iREAL   epsilonA,
		bool    frictionA,
		int	  	particleA,

		const iREAL   *xCoordinatesOfPointsOfGeometryB,
		const iREAL   *yCoordinatesOfPointsOfGeometryB,
		const iREAL   *zCoordinatesOfPointsOfGeometryB,
		int				numberOfTrianglesOfGeometryB,
		iREAL    		epsilonB,
		bool 	  		frictionB,
		int 	    			particleB
		);

	  std::vector<contactpoint> sphereWithBarrierBA(
		iREAL   xCoordinatesOfPointsOfGeometryA,
		iREAL   yCoordinatesOfPointsOfGeometryA,
		iREAL   zCoordinatesOfPointsOfGeometryA,
		iREAL   diameterA,
		iREAL   epsilonA,
		bool    frictionA,
		int 	  	particleA,

		const iREAL   *xCoordinatesOfPointsOfGeometryB,
		const iREAL   *yCoordinatesOfPointsOfGeometryB,
		const iREAL   *zCoordinatesOfPointsOfGeometryB,
		int      		numberOfTrianglesOfGeometryB,
		iREAL    		epsilonB,
		bool     		frictionB,
		int 	    			particleB
		);

	  bool isSphereOverlayInContact(
		iREAL   xCoordinatesOfPointsOfGeometryA,
		iREAL   yCoordinatesOfPointsOfGeometryA,
		iREAL   zCoordinatesOfPointsOfGeometryA,
		iREAL   radiusA,

		iREAL   xCoordinatesOfPointsOfGeometryB,
		iREAL   yCoordinatesOfPointsOfGeometryB,
		iREAL   zCoordinatesOfPointsOfGeometryB,
		iREAL   radiusB
	  );
	}
  }
}
