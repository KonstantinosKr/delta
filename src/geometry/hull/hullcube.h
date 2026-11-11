#ifndef DELTA_GEOMETRY_HULL_HULLCUBE_H_
#define DELTA_GEOMETRY_HULL_HULLCUBE_H_

#include "../../core/delta.h"

/*
 * A box built by handing its 8 corners to the Delaunay hull and taking the
 * triangulation back. The result is a cube, but not by construction: it is an
 * approximation of one, and it lives in hull/ for that reason. Use
 * primitive::cube::generateCube when an exact box is wanted.
 */
namespace delta {
  namespace geometry {
	namespace hull {

	  delta::geometry::mesh::Mesh *generateHullCube(
		iREAL center[3],
		iREAL diagonal);

	  delta::geometry::mesh::Mesh *generateHullCube(
		iREAL center[3],
		iREAL x,
		iREAL y,
		iREAL z,
		iREAL alphaX,
		iREAL alphaY,
		iREAL alphaZ,
		int   meshmultiplier);
	}
  }
}

#endif /* DELTA_GEOMETRY_HULL_HULLCUBE_H_ */
