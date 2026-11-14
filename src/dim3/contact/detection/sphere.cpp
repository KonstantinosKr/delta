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

#include "sphere.h"
#include "point.h"
#include "core/algo.h"

namespace {

/* Unit normal of the triangle from its winding alone: n = (t2-t1) x (t3-t1).
 *
 * Deliberately NOT flipped toward the sphere. Flipping makes the wall push from
 * either side, which reads well for a container but destroys the signed
 * distance: the projection then becomes |distance|, depth peaks at the surface
 * and falls again inside, and the contact normal reverses sign on penetration.
 * With a fixed winding the projection is genuinely signed, depth grows
 * monotonically as the sphere sinks, and "which side" is decided by the caller. */
bool triangleNormal(const iREAL* t1, const iREAL* t2, const iREAL* t3, iREAL n[3])
{
  const iREAL u[3] = {t2[0]-t1[0], t2[1]-t1[1], t2[2]-t1[2]};
  const iREAL v[3] = {t3[0]-t1[0], t3[1]-t1[1], t3[2]-t1[2]};

  n[0] = u[1]*v[2] - u[2]*v[1];
  n[1] = u[2]*v[0] - u[0]*v[2];
  n[2] = u[0]*v[1] - u[1]*v[0];

  const iREAL len = std::sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
  if (!(len > 0.0)) return false;

  n[0] /= len; n[1] /= len; n[2] /= len;
  return true;
}

/* Distance from P to the triangle plane, positive on the side the normal points
 * to (i.e. toward P). */
iREAL planeDistance(const iREAL* t1, const iREAL* n, const iREAL* P)
{
  return (P[0]-t1[0])*n[0] + (P[1]-t1[1])*n[1] + (P[2]-t1[2])*n[2];
}

/* Sphere against a FINITE triangle.
 *
 * Fills `cn` (unit contact normal, pointing from the triangle toward the sphere
 * centre), `Q` (closest point on the triangle) and `gap` (signed: negative when
 * the sphere overlaps). Returns false when the sphere is behind the triangle or
 * too far to be resolved.
 *
 * A plane-only test (the previous version) turns every triangle into an
 * infinite plane: a sphere above a slanted wall's top edge, or beyond its side
 * edge, is still "in contact" with the wall's plane and gets pushed by a face
 * it is not touching. For the hopper that ejects spheres sideways through the
 * front/back walls. So: when the centre projects inside the triangle use the
 * plane distance; otherwise measure against the closest edge/vertex. */
bool sphereTriangleContact(iREAL* TP1, iREAL* TP2, iREAL* TP3,
                           iREAL* P, iREAL radius, iREAL epsTotal,
                           iREAL cn[3], iREAL Q[3], iREAL& gap)
{
  iREAL n[3];
  if (!triangleNormal(TP1, TP2, TP3, n)) return false;

  const iREAL planeDist = planeDistance(TP1, n, P);
  if (planeDist < 0.0) return false;          /* back face */

  const iREAL qDist = delta::contact::detection::pt(TP1, TP2, TP3, P, Q);
  if (!(qDist > 0.0)) return false;

  /* qDist == planeDist only when Q is the perpendicular projection, i.e. the
   * centre is over the triangle's interior. Otherwise Q lies on an edge or a
   * vertex and the gap is the distance to that feature. */
  const iREAL tol = 1e-12 * std::max(1.0, std::fabs(planeDist));
  if (qDist <= planeDist + tol)
  {
    cn[0] = n[0]; cn[1] = n[1]; cn[2] = n[2];
    gap = planeDist - radius;
  }
  else
  {
    cn[0] = (P[0]-Q[0]) / qDist;
    cn[1] = (P[1]-Q[1]) / qDist;
    cn[2] = (P[2]-Q[2]) / qDist;
    gap = qDist - radius;
  }

  const iREAL depth = epsTotal - gap;
  return depth >= 0.0;
}

}  // namespace

std::vector<delta::contact::contactpoint> delta::contact::detection::sphere(
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
  const int 	  particleB)
{
  std::vector<contactpoint> result;

  iREAL dx = xCoordinatesOfPointsOfGeometryB - xCoordinatesOfPointsOfGeometryA;
  iREAL dy = yCoordinatesOfPointsOfGeometryB - yCoordinatesOfPointsOfGeometryA;
  iREAL dz = zCoordinatesOfPointsOfGeometryB - zCoordinatesOfPointsOfGeometryA;
  iREAL distance = std::sqrt(dx*dx + dy*dy + dz*dz);
  if (!(distance > 0.0)) return result;      /* coincident centres */

  /* n points from B to A: the direction body A is pushed away from B. This is
   * the same convention the sphere-vs-triangle path stores for the sphere, so
   * the shared spring+dashpot law works for both contact kinds. */
  const iREAL n[3] = {-dx/distance, -dy/distance, -dz/distance};

  const iREAL radiusA = diameterA / 2.0;
  const iREAL radiusB = diameterB / 2.0;
  const iREAL gap = distance - (radiusA + radiusB);   /* signed: <0 overlapping */
  const iREAL depth = (epsilonA + epsilonB) - gap;    /* acceptance quantity */
  if (depth < 0.0) return result;

  /* Surface points: A's on the side facing B, B's on the side facing A. */
  const iREAL xPA = xCoordinatesOfPointsOfGeometryA - radiusA * n[0];
  const iREAL yPA = yCoordinatesOfPointsOfGeometryA - radiusA * n[1];
  const iREAL zPA = zCoordinatesOfPointsOfGeometryA - radiusA * n[2];
  const iREAL xPB = xCoordinatesOfPointsOfGeometryB + radiusB * n[0];
  const iREAL yPB = yCoordinatesOfPointsOfGeometryB + radiusB * n[1];
  const iREAL zPB = zCoordinatesOfPointsOfGeometryB + radiusB * n[2];

  contactpoint newContactPoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA & frictionB);

  /* The constructor cannot represent this contact: it derives depth as
   * epsilonTotal - |PA-QB| (the acceptance quantity, which peaks at first touch
   * and falls as the overlap grows) and its normal degenerates at exact touch.
   * Use the real overlap and the centre-to-centre normal instead. */
  newContactPoint.normal[0] = n[0];
  newContactPoint.normal[1] = n[1];
  newContactPoint.normal[2] = n[2];
  newContactPoint.depth      = (gap < 0.0) ? -gap : 0.0;

  result.push_back( newContactPoint );
  return result;
}

std::vector<delta::contact::contactpoint> delta::contact::detection::sphereWithBarrierAB(
  iREAL   xCoordinatesOfPointsOfGeometryA,
  iREAL   yCoordinatesOfPointsOfGeometryA,
  iREAL   zCoordinatesOfPointsOfGeometryA,
  iREAL   diameterA,
  iREAL   epsilonA,
  bool    frictionA,
  int 	  particleA,

  const iREAL   *xCoordinatesOfPointsOfGeometryB,
  const iREAL   *yCoordinatesOfPointsOfGeometryB,
  const iREAL   *zCoordinatesOfPointsOfGeometryB,
  int   			numberOfTrianglesOfGeometryB,
  iREAL   		epsilonB,
  bool    		frictionB,
  int 				particleB)
{
  std::vector<contactpoint> result;

  for(int i=0; i<numberOfTrianglesOfGeometryB*3; i+=3)
  {
	iREAL P[3], Q[3];
	iREAL xPA, yPA, zPA, xPB, yPB, zPB;

	iREAL TP1[3], TP2[3], TP3[3];
	TP1[0] = xCoordinatesOfPointsOfGeometryB[i];
	TP1[1] = yCoordinatesOfPointsOfGeometryB[i];
	TP1[2] = zCoordinatesOfPointsOfGeometryB[i];

	TP2[0] = xCoordinatesOfPointsOfGeometryB[i+1];
	TP2[1] = yCoordinatesOfPointsOfGeometryB[i+1];
	TP2[2] = zCoordinatesOfPointsOfGeometryB[i+1];

	TP3[0] = xCoordinatesOfPointsOfGeometryB[i+2];
	TP3[1] = yCoordinatesOfPointsOfGeometryB[i+2];
	TP3[2] = zCoordinatesOfPointsOfGeometryB[i+2];

	P[0] = xCoordinatesOfPointsOfGeometryA;
	P[1] = yCoordinatesOfPointsOfGeometryA;
	P[2] = zCoordinatesOfPointsOfGeometryA;

	const iREAL radius = diameterA / 2.0;
	iREAL cn[3], gap;
	if (!sphereTriangleContact(TP1, TP2, TP3, P, radius, epsilonA + epsilonB, cn, Q, gap)) continue;

	/* The sphere's surface point, on the side facing the wall. */
	xPA = P[0] - radius * cn[0];
	yPA = P[1] - radius * cn[1];
	zPA = P[2] - radius * cn[2];

	xPB = Q[0];
	yPB = Q[1];
	zPB = Q[2];

	contactpoint newContactPoint(xPA, yPA, zPA, epsilonA, particleA,
	                             xPB, yPB, zPB, epsilonB, particleB,
	                             frictionA & frictionB);

	/* Override what the constructor derived from the two points: it cannot
	 * represent a plane contact and collapses when the points coincide. The
	 * spring uses the ACTUAL overlap, zero at first touch, never a pre-load. */
	newContactPoint.normal[0] = cn[0];
	newContactPoint.normal[1] = cn[1];
	newContactPoint.normal[2] = cn[2];
	newContactPoint.depth      = (gap < 0.0) ? -gap : 0.0;

	result.push_back( newContactPoint );
  }
  return result;
}

std::vector<delta::contact::contactpoint> delta::contact::detection::sphereWithBarrierBA(
iREAL   xCoordinatesOfPointsOfGeometryB,
iREAL   yCoordinatesOfPointsOfGeometryB,
iREAL   zCoordinatesOfPointsOfGeometryB,
iREAL   diameterB,
iREAL   epsilonB,
bool    frictionB,
int 	    particleB,

const iREAL   *xCoordinatesOfPointsOfGeometryA,
const iREAL   *yCoordinatesOfPointsOfGeometryA,
const iREAL   *zCoordinatesOfPointsOfGeometryA,
int   				numberOfTrianglesOfGeometryA,
iREAL   			epsilonA,
bool    			frictionA,
int 	  			particleA)
{
  std::vector<contactpoint> result;

  for(int i=0; i<numberOfTrianglesOfGeometryA*3; i+=3)
  {
	iREAL P[3], Q[3];
	iREAL xPA, yPA, zPA, xPB, yPB, zPB;

	iREAL TP1[3], TP2[3], TP3[3];
	TP1[0] = xCoordinatesOfPointsOfGeometryA[i];
	TP1[1] = yCoordinatesOfPointsOfGeometryA[i];
	TP1[2] = zCoordinatesOfPointsOfGeometryA[i];

	TP2[0] = xCoordinatesOfPointsOfGeometryA[i+1];
	TP2[1] = yCoordinatesOfPointsOfGeometryA[i+1];
	TP2[2] = zCoordinatesOfPointsOfGeometryA[i+1];

	TP3[0] = xCoordinatesOfPointsOfGeometryA[i+2];
	TP3[1] = yCoordinatesOfPointsOfGeometryA[i+2];
	TP3[2] = zCoordinatesOfPointsOfGeometryA[i+2];

	P[0] = xCoordinatesOfPointsOfGeometryB;
	P[1] = yCoordinatesOfPointsOfGeometryB;
	P[2] = zCoordinatesOfPointsOfGeometryB;

	/* Same construction as sphereWithBarrierAB; see the comment there. This
	 * variant only differs in the argument order of the contactpoint, which
	 * records the sphere as the master. */
	const iREAL radius = diameterB / 2.0;
	iREAL cn[3], gap;
	if (!sphereTriangleContact(TP1, TP2, TP3, P, radius, epsilonA + epsilonB, cn, Q, gap)) continue;

	xPA = P[0] - radius * cn[0];
	yPA = P[1] - radius * cn[1];
	zPA = P[2] - radius * cn[2];

	xPB = Q[0];
	yPB = Q[1];
	zPB = Q[2];

	/* Master must be the SPHERE and slave the triangle, matching
	 * sphereWithBarrierAB. The old order paired the triangle's point with the
	 * sphere's id, so deriveForces applied the separating force to the wrong
	 * side and drove the sphere into the wall. */
	contactpoint newContactPoint(xPA, yPA, zPA, epsilonB, particleB,
	                             xPB, yPB, zPB, epsilonA, particleA,
	                             frictionA & frictionB);

	/* addCollision() negates the normal for the second party, and here the
	 * sphere IS that second party (the obstacle is A). Its stored copy must end
	 * up pointing away from the wall, so return -cn and let the negation flip it
	 * back to +cn. */
	newContactPoint.normal[0] = -cn[0];
	newContactPoint.normal[1] = -cn[1];
	newContactPoint.normal[2] = -cn[2];
	newContactPoint.depth      = (gap < 0.0) ? -gap : 0.0;

	result.push_back( newContactPoint );
  }
  return result;
}

bool delta::contact::detection::isSphereOverlayInContact(
  iREAL   xCoordinatesOfPointsOfGeometryA,
  iREAL   yCoordinatesOfPointsOfGeometryA,
  iREAL   zCoordinatesOfPointsOfGeometryA,
  iREAL   radiusA,

  iREAL   xCoordinatesOfPointsOfGeometryB,
  iREAL   yCoordinatesOfPointsOfGeometryB,
  iREAL   zCoordinatesOfPointsOfGeometryB,
  iREAL   radiusB)
{
  bool contact = 0;
  iREAL distance = std::sqrt(std::pow(xCoordinatesOfPointsOfGeometryB-xCoordinatesOfPointsOfGeometryA,2)+std::pow(yCoordinatesOfPointsOfGeometryB-yCoordinatesOfPointsOfGeometryA,2)+std::pow(zCoordinatesOfPointsOfGeometryB-zCoordinatesOfPointsOfGeometryA,2));
  if(distance <= radiusA+radiusB) contact = 1;
  return contact;
}
