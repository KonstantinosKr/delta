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

#ifndef GEOMETRY_SHAPE_GRANULATE_H_
#define GEOMETRY_SHAPE_GRANULATE_H_

#include <vector>
#include <cmath>
#include <assert.h>
#include <stdlib.h>
#include "core/io/read.h"
#include "dim3/geometry/operators/hull/hull.h"
#include "dim3/geometry/operators/hull/alg.h"
#include "dim3/geometry/structure/Mesh.h"

namespace delta {
  namespace geometry {
	namespace primitive {
	  namespace granulate {

		/*
		 *  Generate Particle
		 *
		 *  Creates cube using hull algorithm.
		 *
		 *  @param center : center of geometry
		 *  @param h : radius
		 *  @param xCoordinates
  		 *  @param yCoordinates
		 *  @returns mesh
		 */
		void generateParticle(
		  iREAL    				center[2],
		  iREAL    				h,
		  std::vector<iREAL>&  	xCoordinates,
		  std::vector<iREAL>&  	yCoordinates
		);

		/*
		 * Deterministic variant of the 2D generator. The no-seed overload uses
		 * the process-wide rand() state, so it depends on call order.
		 */
		void generateParticle(
		  iREAL    				center[2],
		  iREAL    				h,
		  std::vector<iREAL>&  	xCoordinates,
		  std::vector<iREAL>&  	yCoordinates,
		  unsigned 				seed
		);

		/*
		 *  Generate Particle
		 *
		 *  Creates granulate using hull algorithm.
		 *
		 *  @param center : center of geometry
		 *  @param h : radius
		 *  @returns mesh
		 */
		delta::geometry::mesh::Mesh *loadParticle(
		  iREAL  				center[3],
		  iREAL  				h
		);

		/*
		 *  Generate Particle
		 *
		 *  Creates granulate using hull algorithm.
		 *
		 *  @param center : center of geometry
		 *  @param h : radius
		 *  @param noPointsPerParticle : mesh multiplier
		 *  @returns mesh
		 */
		delta::geometry::mesh::Mesh *generateParticle(
		  iREAL    				center[3],
		  iREAL    				h,
		  int 					noPointsPerParticle);

		/*
		 * Deterministic variant: the point cloud comes from a generator owned
		 * by this call, so the same seed gives the same granulate on every run
		 * and on every machine.
		 *
		 * Use this when a scenario has to be reproducible. The overload above
		 * consumes the process-wide drand48() stream instead, which means the
		 * shape depends on whatever ran before it - that behaviour is kept
		 * because existing scenario runs depend on it.
		 */
		delta::geometry::mesh::Mesh *generateParticle(
		  iREAL    				center[3],
		  iREAL    				h,
		  int 					noPointsPerParticle,
		  unsigned 				seed);

		/* Shared implementation behind the two overloads above. */
		delta::geometry::mesh::Mesh *generateParticle(
		  iREAL    				center[3],
		  iREAL    				h,
		  int 					noPointsPerParticle,
		  unsigned 				seed,
		  bool 					seeded);
	  }
	}
  }
}
#endif
