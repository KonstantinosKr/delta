/*
 The MIT License (MIT)

 Copyright (c) 6 Sep 2016 Konstantinos Krestenitis

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

#ifndef DELTA_geometry_GRAPHITE_H_
#define DELTA_geometry_GRAPHITE_H_

#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <vector>
#include <delta/core/read.h>
#include <delta/geometry/mesh/Mesh.h>

namespace delta {
  namespace geometry {
	namespace defined {
		delta::geometry::mesh::Mesh *generateBrickFB(
		 iREAL    center[3],
		 iREAL    h);

		delta::geometry::mesh::Mesh *generateBrickFB();

		delta::geometry::mesh::Mesh *generateBrickFB(
		 iREAL    center[3]);

		delta::geometry::mesh::Mesh *generateKeyXK(
		 iREAL    center[3],
		 iREAL    h);

		delta::geometry::mesh::Mesh *generateKeyYK(
		 iREAL    center[3],
		 iREAL    h);
	}
  }
}


#endif /* DELTA_geometry_GRAPHITE_H_ */
