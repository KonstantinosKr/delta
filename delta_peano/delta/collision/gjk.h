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

#include <vector>
#include <limits>

#include "delta/collision/contactpoint.h"

#include "algo.h"

namespace delta {
  namespace collision {
    std::vector<contactpoint> gjk(
     int       numberOfTrianglesOfGeometryA,
     double*   xCoordinatesOfPointsOfGeometryA,
     double*   yCoordinatesOfPointsOfGeometryA,
     double*   zCoordinatesOfPointsOfGeometryA,
	 double    epsilonA,
	 bool       frictionA,
	 int 	   particleA,

     int       numberOfTrianglesOfGeometryB,
     double*   xCoordinatesOfPointsOfGeometryB,
     double*   yCoordinatesOfPointsOfGeometryB,
     double*   zCoordinatesOfPointsOfGeometryB,
     double    epsilonB,
	 bool       frictionB,
	 int 	    particleB
    );
    
    /* (a,na) and (b,nb) are the two input tables of polyhedrons vertices;
     * 'p' and 'q' are the two outputed closest points, respectively in
     * polyhedron (a,na) and polyhedron (b,nb); the distance is returned */
    double gjk (double *a, int na, double *b, int nb, double *p, double *q);
  }
}
