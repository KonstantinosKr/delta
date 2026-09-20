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
#include "../../core/algo.h"

namespace {

/*
 * Unit direction from a sphere centre P to the closest point Q on a triangle.
 *
 * The barrier detectors used to divide (Q-P) by (distance + radius), which is
 * |P-Q| plus rounding only while |P-Q| != radius. For a sphere exactly touching
 * the surface (|P-Q| == radius, e.g. centre 0.98 with radius 0.02 against a wall
 * at x = 1) that denominator cancels to zero, the surface point P + radius*n
 * goes infinite and every contact of the pair turns into NaN. Dividing by |P-Q|
 * itself gives the same direction and stays finite; a zero direction is returned
 * when the centre lies on the triangle, where the pair has no direction at all.
 */
void surfaceNormal(const iREAL* P, const iREAL* Q, iREAL* normal)
{
  iREAL dx = Q[0]-P[0];
  iREAL dy = Q[1]-P[1];
  iREAL dz = Q[2]-P[2];
  iREAL length = std::sqrt((dx*dx)+(dy*dy)+(dz*dz));

  normal[0] = (length > 0.0) ? dx/length : 0.0;
  normal[1] = (length > 0.0) ? dy/length : 0.0;
  normal[2] = (length > 0.0) ? dz/length : 0.0;
}

}

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

  iREAL distance = std::sqrt(((xCoordinatesOfPointsOfGeometryB-xCoordinatesOfPointsOfGeometryA)*(xCoordinatesOfPointsOfGeometryB-xCoordinatesOfPointsOfGeometryA))+((yCoordinatesOfPointsOfGeometryB-yCoordinatesOfPointsOfGeometryA)*(yCoordinatesOfPointsOfGeometryB-yCoordinatesOfPointsOfGeometryA))+((zCoordinatesOfPointsOfGeometryB-zCoordinatesOfPointsOfGeometryA)*(zCoordinatesOfPointsOfGeometryB-zCoordinatesOfPointsOfGeometryA)));
  iREAL xd = xCoordinatesOfPointsOfGeometryB - xCoordinatesOfPointsOfGeometryA;
  iREAL yd = yCoordinatesOfPointsOfGeometryB - yCoordinatesOfPointsOfGeometryA;
  iREAL zd = zCoordinatesOfPointsOfGeometryB - zCoordinatesOfPointsOfGeometryA;

  iREAL xnormal = (1.0/distance) * xd;
  iREAL ynormal = (1.0/distance) * yd;
  iREAL znormal = (1.0/distance) * zd;

  iREAL xPA = xCoordinatesOfPointsOfGeometryA + ((diameterA/2) * xnormal);
  iREAL yPA = yCoordinatesOfPointsOfGeometryA + ((diameterA/2) * ynormal);
  iREAL zPA = zCoordinatesOfPointsOfGeometryA + ((diameterA/2) * znormal);

  iREAL xPB = xCoordinatesOfPointsOfGeometryB - ((diameterB/2) * xnormal);
  iREAL yPB = yCoordinatesOfPointsOfGeometryB - ((diameterB/2) * ynormal);
  iREAL zPB = zCoordinatesOfPointsOfGeometryB - ((diameterB/2) * znormal);

  contactpoint newContactPoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA & frictionB);

  if(newContactPoint.getDistance() <= (epsilonA+epsilonB))
  {
	result.push_back( newContactPoint );
  }
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

	//Q is filled with the closest point of the triangle; the sphere surface point
	//P + radius*normal lands on it for an exact touch.
	delta::contact::detection::pt(TP1, TP2, TP3, P, Q);

	iREAL normal[3];
	surfaceNormal(P, Q, normal);

	xPA = P[0] + ((diameterA/2.0) * normal[0]);
	yPA = P[1] + ((diameterA/2.0) * normal[1]);
	zPA = P[2] + ((diameterA/2.0) * normal[2]);

	xPB = Q[0];
	yPB = Q[1];
	zPB = Q[2];

	contactpoint newContactPoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA & frictionB);

	if(newContactPoint.getDistance() <= (epsilonA+epsilonB))
	{
	  result.push_back( newContactPoint );
	}
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

	//Q is filled with the closest point of the triangle; the sphere surface point
	//P + radius*normal lands on it for an exact touch.
	delta::contact::detection::pt(TP1, TP2, TP3, P, Q);

	iREAL normal[3];
	surfaceNormal(P, Q, normal);

	xPA = P[0] + ((diameterB/2.0) * normal[0]);
	yPA = P[1] + ((diameterB/2.0) * normal[1]);
	zPA = P[2] + ((diameterB/2.0) * normal[2]);

	xPB = Q[0];
	yPB = Q[1];
	zPB = Q[2];

	contactpoint newContactPoint(xPB, yPB, zPB, epsilonB, particleB, xPA, yPA, zPA, epsilonA, particleA, frictionA & frictionB);
	if (newContactPoint.getDistance() <= (epsilonA+epsilonB))
	{
	  result.push_back( newContactPoint );
	}
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
