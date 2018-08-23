/*
 * point.cpp
 *
 *  Created on: 19 Jun 2018
 *      Author: konstantinos
 */

#include "delta/contact/detection/point.h"
#include "delta/core/algo.h"

iREAL delta::contact::detection::pt(iREAL TP1[3], iREAL TP2[3], iREAL TP3[3], iREAL cPoint[3], iREAL tq[3])
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

std::vector<delta::contact::contactpoint> delta::contact::detection::pointToGeometry(
iREAL   xCoordinatesOfPointOfGeometryA,
iREAL   yCoordinatesOfPointOfGeometryA,
iREAL   zCoordinatesOfPointOfGeometryA,
int 		particleA,
iREAL   epsilonA,

const iREAL   *xCoordinatesOfPointsOfGeometryB,
const iREAL   *yCoordinatesOfPointsOfGeometryB,
const iREAL   *zCoordinatesOfPointsOfGeometryB,
int   			numberOfTrianglesOfGeometryB,
int				particleB,
iREAL   			epsilonB)
{
  /*
  std::vector<delta::contact::contactpoint> result;

  #pragma code_align(byteAlignment)
  #pragma omp simd
  for(int i=0; i<numberOfTrianglesOfGeometryB*3; i+=3)
  {
	iREAL P[3], Q[3];

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

	P[0] = xCoordinatesOfPointOfGeometryA;
	P[1] = yCoordinatesOfPointOfGeometryA;
	P[2] = zCoordinatesOfPointOfGeometryA;

	iREAL distance = delta::contact::detection::pt(TP1, TP2, TP3, P, Q);

	if(distance <= (epsilonA+epsilonB))
	{
	  contactpoint newContactPoint(P[0], P[1], P[2], epsilonA, particleA, TP1, TP2, TP3, Q[0], Q[1], Q[2], epsilonB, particleB, false);
	  result.push_back( newContactPoint );
	}
  }
  */

  std::vector<delta::contact::contactpoint> result;

  #pragma code_align(byteAlignment)
  #pragma omp simd
  for(int i=0; i<numberOfTrianglesOfGeometryB*3; i+=3)
  {
	iREAL P[3], Q[3];

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

	P[0] = xCoordinatesOfPointOfGeometryA;
	P[1] = yCoordinatesOfPointOfGeometryA;
	P[2] = zCoordinatesOfPointOfGeometryA;

	iREAL distance = delta::contact::detection::pt(TP1, TP2, TP3, P, Q);

	if(distance <= (epsilonA+epsilonB))
	{
	  contactpoint newContactPoint(P[0], P[1], P[2], epsilonA, particleA, TP1, TP2, TP3, Q[0], Q[1], Q[2], epsilonB, particleB, false);
	  result.push_back( newContactPoint );
	}
  }
  return result;
}

