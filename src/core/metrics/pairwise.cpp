/*
 * pairwise.cpp
 *
 *  Created on: 12 Jun 2018
 *      Author: konstantinos
 */

#include "pairwise.h"

void delta::metrics::pairwise::pointToMeshDistance(
	iREAL xV,
	iREAL yV,
	iREAL zV,

	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates,

	std::vector<iREAL>& xP,
	std::vector<iREAL>& yP,
	std::vector<iREAL>& zP,

	std::vector<iREAL>& distance)
{
  for(int i=0; i<xCoordinates.size(); i+=3)
  {
	iREAL xA = xCoordinates[i];
	iREAL yA = yCoordinates[i];
	iREAL zA = zCoordinates[i];

	iREAL xB = xCoordinates[i+1];
	iREAL yB = yCoordinates[i+1];
	iREAL zB = zCoordinates[i+1];

	iREAL xC = xCoordinates[i+2];
	iREAL yC = yCoordinates[i+2];
	iREAL zC = zCoordinates[i+2];

	iREAL xP,yP,zP;
	delta::metrics::pairwise::pointToTriangleDistance(xV, yV, zV, xA,yA,zA, xB,yB,zB, xC,yC,zC, xP,yP,zP);
  }
}

iREAL delta::metrics::pairwise::pointToTriangleDistance(
	iREAL xV,
	iREAL yV,
	iREAL zV,

	iREAL& xA,
	iREAL& yA,
	iREAL& zA,

	iREAL& xB,
	iREAL& yB,
	iREAL& zB,

	iREAL& xC,
	iREAL& yC,
	iREAL& zC,

	iREAL &xP,
	iREAL &yP,
	iREAL &zP)
{
  double TP1[3] = {xA, yA, zA};
  double TP2[3] = {xB, yB, zB};
  double TP3[3] = {xC, yC, zC};

  double E0[3];
  E0[0] = TP2[0] - TP1[0];
  E0[1] = TP2[1] - TP1[1];
  E0[2] = TP2[2] - TP1[2];

  double E1[3];
  E1[0] = TP3[0] - TP1[0];
  E1[1] = TP3[1] - TP1[1];
  E1[2] = TP3[2] - TP1[2];

  double D[3];
  D[0] = TP1[0] - xV;
  D[1] = TP1[1] - yV;
  D[2] = TP1[2] - zV;

  double a = DOT(E0,E0);
  double b = DOT(E0,E1);
  double c = DOT(E1,E1);
  double d = DOT(E0,D);
  double e = DOT(E1,D);
  double f = DOT(D,D);

  double det = a*c - b*b; //% do we have to use abs here?
  double s   = b*e - c*d;
  double t   = b*d - a*e;

  double sqrDistance=0;

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
			  double invDet = 1/det;
			  s = s*invDet;
			  t = t*invDet;
			  sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
		  }
	  }
  }else {
	  if (s < 0){
		  // region 2
		  double tmp0 = b + d;
		  double tmp1 = c + e;
		  if (tmp1 > tmp0){ // minimum on edge s+t=1
			  double numer = tmp1 - tmp0;
			  double denom = a - 2*b + c;
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
			  double tmp0 = b + e;
			  double tmp1 = a + d;
			  if (tmp1 > tmp0){
				  double numer = tmp1 - tmp0;
				  double denom = a-2*b+c;
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
			  double numer = c + e - b - d;
			  if (numer <= 0){
				  s = 0;
				  t = 1;
				  sqrDistance = c + 2*e + f;
			  }else {
				  double denom = a - 2*b + c;
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
  if (sqrDistance < 0){ sqrDistance = 0;}

  xP = TP1[0] + (E1[0] * t) + (E0[0] * s);
  yP = TP1[1] + (E1[1] * t) + (E0[1] * s);
  zP = TP1[2] + (E1[2] * t) + (E0[2] * s);

  return sqrt(sqrDistance);
}
