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


std::vector<delta::collision::contactpoint> delta::collision::sphere(
  iREAL   xCoordinatesOfPointsOfGeometryA,
  iREAL   yCoordinatesOfPointsOfGeometryA,
  iREAL   zCoordinatesOfPointsOfGeometryA,
  iREAL   diameterA,
  iREAL   epsilonA,
  bool     frictionA,
  int 	   particleA,

  iREAL   xCoordinatesOfPointsOfGeometryB,
  iREAL   yCoordinatesOfPointsOfGeometryB,
  iREAL   zCoordinatesOfPointsOfGeometryB,
  iREAL   diameterB,
  iREAL   epsilonB,
  bool     frictionB,
  int 	   particleB)
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


iREAL pt(iREAL TP1[3], iREAL TP2[3], iREAL TP3[3], iREAL cPoint[3], iREAL tq[3])
{
    //191 flops
    iREAL E0[3];
    E0[0] = TP2[0] - TP1[0];
    E0[1] = TP2[1] - TP1[1];
    E0[2] = TP2[2] - TP1[2];

    iREAL E1[3];
    E1[0] = TP3[0] - TP1[0];
    E1[1] = TP3[1] - TP1[1];
    E1[2] = TP3[2] - TP1[2];

    iREAL D[3];
    D[0] = TP1[0] - cPoint[0];
    D[1] = TP1[1] - cPoint[1];
    D[2] = TP1[2] - cPoint[2];

    iREAL a = DOT(E0,E0);
    iREAL b = DOT(E0,E1);
    iREAL c = DOT(E1,E1);
    iREAL d = DOT(E0,D);
    iREAL e = DOT(E1,D);
    iREAL f = DOT(D,D);

    iREAL det = a*c - b*b; //% do we have to use abs here?
    iREAL s   = b*e - c*d;
    iREAL t   = b*d - a*e;

    iREAL sqrDistance=0;

    if ((s+t) <= det){
        if (s < 0){
            if (t < 0){
                //region4
                if (d < 0){
                    t = 0;
                    if (-d >= a){
                        s = 1;
                        sqrDistance = a + 2*d + f;
                    }else {
                        s = -d/a;
                        sqrDistance = d*s + f;
                    }
                }else {
                    s = 0;
                    if (e >= 0){
                        t = 0;
                        sqrDistance = f;
                    }else{
                        if (-e >= c){
                            t = 1;
                            sqrDistance = c + 2*e + f;
                        } else {
                            t = -e/c;
                            sqrDistance = e*t + f;
                        }
                    }
                } //end of region 4
            }else {
                // region 3
                s = 0;
                if (e >= 0){
                    t = 0;
                    sqrDistance = f;
                }else {
                    if (-e >= c){
                        t = 1;
                        sqrDistance = c + 2*e +f;
                    }else {
                        t = -e/c;
                        sqrDistance = e*t + f;
                    }
                }
            } //end of region 3
        }else {
            if (t < 0){
                // region 5
                t = 0;
                if (d >= 0){
                    s = 0;
                    sqrDistance = f;
                }else {
                    if (-d >= a){
                        s = 1;
                        sqrDistance = a + 2*d + f;// GF 20101013 fixed typo d*s ->2*d
                    }else {
                        s = -d/a;
                        sqrDistance = d*s + f;
                    }
                }
            }else {
                // region 0
                iREAL invDet = 1/det;
                s = s*invDet;
                t = t*invDet;
                sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
            }
        }
    }else {
        if (s < 0){
            // region 2
            iREAL tmp0 = b + d;
            iREAL tmp1 = c + e;
            if (tmp1 > tmp0){ // minimum on edge s+t=1
                iREAL numer = tmp1 - tmp0;
                iREAL denom = a - 2*b + c;
                if (numer >= denom){
                    s = 1;
                    t = 0;
                    sqrDistance = a + 2*d + f; // GF 20101014 fixed typo 2*b -> 2*d
                }else {
                    s = numer/denom;
                    t = 1-s;
                    sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
                }
            }else {         // minimum on edge s=0
                s = 0;
                if (tmp1 <= 0) {
                    t = 1;
                    sqrDistance = c + 2*e + f;
                }else {
                    if (e >= 0){
                        t = 0;
                        sqrDistance = f;
                    }else {
                        t = -e/c;
                        sqrDistance = e*t + f;
                    }
                }
            } //end of region 2
        }else {
            if (t < 0) {
                //region6
                iREAL tmp0 = b + e;
                iREAL tmp1 = a + d;
                if (tmp1 > tmp0){
                    iREAL numer = tmp1 - tmp0;
                    iREAL denom = a-2*b+c;
                    if (numer >= denom){
                        t = 1;
                        s = 0;
                        sqrDistance = c + 2*e + f;
                    }else {
                        t = numer/denom;
                        s = 1 - t;
                        sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
                    }
                }else {
                    t = 0;
                    if (tmp1 <= 0){
                        s = 1;
                        sqrDistance = a + 2*d + f;
                    }else {
                        if (d >= 0) {
                            s = 0;
                            sqrDistance = f;
                        }else {
                            s = -d/a;
                            sqrDistance = d*s + f;
                        }
                    }
                }
                //end of region 6
            }else {
                // region 1
                iREAL numer = c + e - b - d;
                if (numer <= 0){
                    s = 0;
                    t = 1;
                    sqrDistance = c + 2*e + f;
                }else {
                    iREAL denom = a - 2*b + c;
                    if (numer >= denom){
                        s = 1;
                        t = 0;
                        sqrDistance = a + 2*d + f;
                    }else {
                        s = numer/denom;
                        t = 1-s;
                        sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
                    }
                } //end of region 1
            }
        }
    }

    // account for numerical round-off error
    if (sqrDistance < 0){
        sqrDistance = 0;
    }

    tq[0] = TP1[0] + (E1[0] * t) + (E0[0] * s);
    tq[1] = TP1[1] + (E1[1] * t) + (E0[1] * s);
    tq[2] = TP1[2] + (E1[2] * t) + (E0[2] * s);

    return sqrt(sqrDistance);
}


std::vector<delta::collision::contactpoint> delta::collision::sphereWithBarrierAB(
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
  int   	numberOfTrianglesOfGeometryB,
  iREAL   epsilonB,
  bool    frictionB,
  int 		particleB)
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

		iREAL distance = pt(TP1, TP2, TP3, P, Q) - (diameterA/2.0);

		iREAL xnormal = (Q[0] - P[0])/(distance+(diameterA/2.0));
		iREAL ynormal = (Q[1] - P[1])/(distance+(diameterA/2.0));
		iREAL znormal = (Q[2] - P[2])/(distance+(diameterA/2.0));

		xPA = P[0] + ((diameterA/2.0) * xnormal);
		yPA = P[1] + ((diameterA/2.0) * ynormal);
		zPA = P[2] + ((diameterA/2.0) * znormal);

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

std::vector<delta::collision::contactpoint> delta::collision::sphereWithBarrierBA(
  iREAL   xCoordinatesOfPointsOfGeometryB,
  iREAL   yCoordinatesOfPointsOfGeometryB,
  iREAL   zCoordinatesOfPointsOfGeometryB,
  iREAL   diameterB,
  iREAL   epsilonB,
  bool     frictionB,
  int 	   particleB,

  const iREAL   *xCoordinatesOfPointsOfGeometryA,
  const iREAL   *yCoordinatesOfPointsOfGeometryA,
  const iREAL   *zCoordinatesOfPointsOfGeometryA,
  int   	numberOfTrianglesOfGeometryA,
  iREAL   epsilonA,
  bool    frictionA,
  int 	  particleA)
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

		iREAL distance = pt(TP1, TP2, TP3, P, Q) - (diameterB/2.0);

		iREAL xnormal = (Q[0] - P[0])/(distance+(diameterB/2.0));
		iREAL ynormal = (Q[1] - P[1])/(distance+(diameterB/2.0));
		iREAL znormal = (Q[2] - P[2])/(distance+(diameterB/2.0));

		xPA = P[0] + ((diameterB/2.0) * xnormal);
		yPA = P[1] + ((diameterB/2.0) * ynormal);
		zPA = P[2] + ((diameterB/2.0) * znormal);

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


bool delta::collision::isSphereOverlayInContact(
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
