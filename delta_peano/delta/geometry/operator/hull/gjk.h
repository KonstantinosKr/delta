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
 * gjk.h:
 * distance between a pair of convex polyhedra according to the classical algortihm
 * by Gilbert et al. IEEE J. of Robotics and Automation, 4/2, 1988, pp. 193-203
 */

#ifndef __gjk__
#define __gjk__

/* (a,na) and (b,nb) are the two input tables of polyhedrons vertices;
 * 'p' and 'q' are the two outputed closest points, respectively in
 * polyhedron (a,na) and polyhedron (b,nb); the distance is returned */
double gjk (double *a, int na, double *b, int nb, double *p, double *q);

/* (a,na) and (c,r) are the input polyhedron and sphere; 'p' and 'q' are the two outputed
 * closest points, respectively in polyhedron (a,na) and sphere (c,r); the distance is returned */
double gjk_convex_sphere (double *a, int na, double *c, double r, double *p, double *q);

/* (a,na) and p are the input polyhedron and point; 'q' is the outputed
 * closest point on the polyhedron; the distance is returned */
double gjk_convex_point (double *a, int na, double *p, double *q);

/* (a,na) and (b,bsca, brot) are the input polyhedron and ellipsoid; 'p' and 'q' are the two outputed
 * closest points, respectively in polyhedron (a,na) and ellipsoid (b, bsca, brot); the distance is returned */
double gjk_convex_ellip (double *a, int na, double *b, double *bsca, double *brot, double *p, double *q);

/* (a,ra) and (b,rb) are the input spheres; * 'p' and 'q' are the two outputed closest points,
 * respectively in spheres (a,ra) and (b,rb); the distance is returned */
double gjk_sphere_sphere (double *a, double ra, double *b, double rb, double *p, double *q);

/* (a,ra) and (b,bsca,brot)) are the input sphere and ellipsoid; 'p' and 'q' are the two outputed
 * closest points, respectively in sphere (a,ra) and ellipsoid (b,bsca,brot); the distance is returned */
double gjk_sphere_ellip (double *a, double ra, double *b, double *bsca, double *brot, double *p, double *q);

/* (a,asca,arot) and (b,bsca,brot) are the two input ellipsoids; 'p' and 'q' are the two outputed
 * closest points, respectively in (a,asca,arot) and (b,bsca,brot); the distance is returned */
double gjk_ellip_ellip (double *a, double *asca, double *arot, double *b, double *bsca, double *brot, double *p, double *q);

/* (a,asca,arot) and p are the input ellipsoid and point; 'q' is the outputed
 * closest point on the ellipsoid; the distance is returned */
double gjk_ellip_point (double *a, double *asca, double *arot, double *p, double *q);

/* compute gap function betwen two primitives along the given unit normal;
 * the normal direction is assumed to be outward to the first primitive */
double gjk_convex_convex_gap (double *a, int na, double *b, int nb, double *normal);
double gjk_convex_sphere_gap (double *a, int na, double *b, double rb, double *normal);
double gjk_convex_ellip_gap (double *a, int na, double *b, double *bsca, double *brot, double *normal);
double gjk_sphere_sphere_gap (double *a, double ra, double *b, double rb, double *normal);
double gjk_sphere_ellip_gap (double *a, double ra, double *b, double *bsca, double *brot, double *normal);
double gjk_ellip_ellip_gap (double *a, double *asca, double *arot, double *b, double *bsca, double *brot, double *normal);

/* compute furthest or closest (near == 0 or 1) point 'p' of a primitive along given normal direction */
void gjk_ellip_support_point (double *a, double *sca, double *rot, double *normal, short near, double *p);

#endif
