/*
 * gjk.h
 * Copyright (C) 2008, Tomasz Koziara (t.koziara AT gmail.com)
 * -------------------------------------------------------------------------------
 * distance between a pair of convex polyhedra according to the classical algortihm
 * by Gilbert et al. IEEE J. of Robotics and Automation, 4/2, 1988, pp. 193-203
 */

/* This file is part of Solfec.
 * Solfec is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Solfec is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Solfec. If not, see <http://www.gnu.org/licenses/>. */

#include <delta/contact/contactpoint.h>
#include <vector>
#include <limits>
#include <float.h>
#include "delta/core/algo.h"

namespace delta {
  namespace contact {
	namespace detection {
    std::vector<contactpoint> gjk(
	   int       numberOfTrianglesOfGeometryA,
	   iREAL*   xCoordinatesOfPointsOfGeometryA,
	   iREAL*   yCoordinatesOfPointsOfGeometryA,
	   iREAL*   zCoordinatesOfPointsOfGeometryA,
	   iREAL    epsilonA,
	   bool       frictionA,
	   int 	   particleA,

	   int       numberOfTrianglesOfGeometryB,
	   iREAL*   xCoordinatesOfPointsOfGeometryB,
	   iREAL*   yCoordinatesOfPointsOfGeometryB,
	   iREAL*   zCoordinatesOfPointsOfGeometryB,
	   iREAL    epsilonB,
	   bool       frictionB,
	   int 	    particleB
	  );

	  /* (a,na) and (b,nb) are the two input tables of polyhedrons vertices;
	   * 'p' and 'q' are the two outputed closest points, respectively in
	   * polyhedron (a,na) and polyhedron (b,nb); the distance is returned */
	  iREAL gjk (iREAL *a, int na, iREAL *b, int nb, iREAL *p, iREAL *q);
	}
  }
}
