/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Tomasz Koziara
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * cvi.h: intersection of two convex polyhedrons
 */

#include "tri.h"

#ifndef __DELTA_HULL_cvi__
#define __DELTA_HULL_cvi__

namespace delta {
  namespace hull {

  typedef enum {REGULARIZED,     /* filters out zero-volume intersections */
				NON_REGULARIZED} /* includes surface-to-surface zero-volume intersections */
			CVIKIND;

  /* compute intersection of two convex polyhedrons:
   * (va, nva) are vertices of polyhedron 'a' (3-vectors),
   * (pa, npa) are planes of polyhedron 'b' (6-vectors: normal, point),
   * similarly for polyhedron 'b'; the returned 'm' triangles bounding
   * the surface mesh of the intersection; the 'flg' member in TRI is set either
   * to a positive or to a negative index, depending on the origin of the triangle
   * in the plane of polyhedron 'a' (positive) or 'b' (negative); pointers in the
   * returned TRI table reference the memory placed in the same block;
   * the adjacency structure in the returned mesh is not set;
   * 'pv' if not NULL, points to the vertex memory (part of 'tri' memory block);
   * 'nv' if not NULL, is the number of vertices of the intersection convex */
  TRI* cvi (double *va, int nva, double *pa, int npa,
			double *vb, int nvb, double *pb, int npb,
		CVIKIND kind, int *m, double **pv, int *nv);
  }
}

#endif
