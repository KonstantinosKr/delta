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

#ifndef GEOMETRY_HARDCODED_HOPPER_H_
#define GEOMETRY_HARDCODED_HOPPER_H_

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <delta/geometry/operators/triangle.h>
#include "../structure/Mesh.h"

namespace delta {
  namespace geometry {
	namespace hardcoded {

	/*
	 *  Generate Inner Hopper
	 *
	 *  Creates inner Side of Hopper.
	 *  The function returns by reference all xyz
	 *  coordinates of the mesh.
	 *
	 *
	 *  @param center 			: center of geometry
	 *  @param width      		: width of hopper
	 *  @param height  			: height of hopper
	 *  @param hatch 			: hatch width
	 *  @param xCoordinates 	: x coordinates
	 *  @param yCoordinates 	: y coordinates
	 *  @param zCoordinates 	: z coordinates
	 *  @returns void
	 */
	  void generateInnerHopper(
			iREAL  center[3],
			iREAL  width,
			iREAL  height,
			iREAL  hatch,
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

	/*
	 *  Generate Inner Hopper
	 *
	 *  Creates outer Side of Hopper.
	 *  The function returns by reference all xyz
	 *  coordinates of the mesh.
	 *
	 *
	 *  @param center 			: center of geometry
	 *  @param width      		: width of hopper
	 *  @param height  			: height of hopper
	 *  @param hatch 			: hatch width
	 *  @param xCoordinates 	: x coordinates
	 *  @param yCoordinates 	: y coordinates
	 *  @param zCoordinates 	: z coordinates
	 *  @returns void
	 */
	  void generateOuterHopper(
			iREAL  center[3],
			iREAL  width,
			iREAL  height,
			iREAL  hatch,
			std::vector<iREAL>&  xCoordinates,
			std::vector<iREAL>&  yCoordinates,
			std::vector<iREAL>&  zCoordinates);

	/*
	 *  Generate Hopper
	 *
	 *  Creates Hopper structure and returns its mesh.
	 *
	 *
	 *  @param center 			: center of geometry
	 *  @param width      		: width of hopper
	 *  @param thickness		: thickness of the wall
	 *  @param height  			: height of hopper
	 *  @param hatch 			: hatch width
	 *  @param meshRefinement 	: depth of mesh refinement
	 *  @param gridH 			: h length
	 *  @returns mesh
	 */
	  delta::geometry::mesh::Mesh *generateHopper(
			iREAL  center[3],
			iREAL  width,
			iREAL  thickness,
			iREAL  height,
			iREAL  hatch,
			int    meshRefinement,
			iREAL  gridH
			);
	}
  }
}




#endif /* DELTA_geometry_HOPPER_H_ */
