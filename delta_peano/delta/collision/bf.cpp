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

#include "bf.h"
#include <complex>
#include <limits>
#include <iostream>

std::vector<delta::collision::contactpoint> delta::collision::bf(
    int       numberOfTrianglesOfGeometryA,
    double*   xCoordinatesOfPointsOfGeometryA,
    double*   yCoordinatesOfPointsOfGeometryA,
    double*   zCoordinatesOfPointsOfGeometryA,
	double    epsilonA,
	int       materialA,

	int       numberOfTrianglesOfGeometryB,
    double*   xCoordinatesOfPointsOfGeometryB,
    double*   yCoordinatesOfPointsOfGeometryB,
    double*   zCoordinatesOfPointsOfGeometryB,
    double    epsilonB,
	int       materialB
    ) {
  std::vector<contactpoint> result;

#if defined(__INTEL_COMPILER)
  __assume_aligned(xCoordinatesOfPointsOfGeometryA, byteAlignment);
  __assume_aligned(yCoordinatesOfPointsOfGeometryA, byteAlignment);
  __assume_aligned(zCoordinatesOfPointsOfGeometryA, byteAlignment);

  __assume_aligned(xCoordinatesOfPointsOfGeometryB, byteAlignment);
  __assume_aligned(yCoordinatesOfPointsOfGeometryB, byteAlignment);
  __assume_aligned(zCoordinatesOfPointsOfGeometryB, byteAlignment);
#endif

#ifdef ompTriangle
	#pragma omp parallel for 
#endif
  for (int iA =0; iA<numberOfTrianglesOfGeometryA*3; iA+=3)
  {
	double minDistance = std::numeric_limits<double>::max();
	contactpoint nearestContactPoint;

	#pragma simd  
	for (int iB = 0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
	{
		double xPA __attribute__ ((aligned(byteAlignment)));
		double yPA __attribute__ ((aligned(byteAlignment)));
		double zPA __attribute__ ((aligned(byteAlignment)));
		double xPB __attribute__ ((aligned(byteAlignment)));
		double yPB __attribute__ ((aligned(byteAlignment)));
		double zPB __attribute__ ((aligned(byteAlignment)));
		  
		bf( xCoordinatesOfPointsOfGeometryA+(iA),
		    yCoordinatesOfPointsOfGeometryA+(iA),
		    zCoordinatesOfPointsOfGeometryA+(iA),
		    xCoordinatesOfPointsOfGeometryB+(iB),
		    yCoordinatesOfPointsOfGeometryB+(iB),
		    zCoordinatesOfPointsOfGeometryB+(iB),
		    xPA, yPA, zPA, xPB, yPB, zPB);

		contactpoint newContactPoint(xPA, yPA, zPA, epsilonA, xPB, yPB, zPB, epsilonB, materialA+materialB);
		if (newContactPoint.getDistance()<minDistance) {
		  	  nearestContactPoint = newContactPoint;
			  minDistance         = newContactPoint.getDistance();
		}
	}
	if (nearestContactPoint.getDistance()<=(epsilonA+epsilonB)) {
	#ifdef ompTriangle
		#pragma omp critical
	#endif
		result.push_back( nearestContactPoint );
	}
  }

  return result;
}

#define DOT(a, b)\
((a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2])

void delta::collision::bf(
                          double   xCoordinatesOfPointsOfGeometryA[3],
                          double   yCoordinatesOfPointsOfGeometryA[3],
                          double   zCoordinatesOfPointsOfGeometryA[3],
                          double   xCoordinatesOfPointsOfGeometryB[3],
                          double   yCoordinatesOfPointsOfGeometryB[3],
                          double   zCoordinatesOfPointsOfGeometryB[3],
                          double&  xPA,
                          double&  yPA,
                          double&  zPA,
                          double&  xPB,
                          double&  yPB,
                          double&  zPB
                          ) {
  
  iREAL u[3], v[3], nn[3][2], w[3], w0[3][6], dir[3][6], pointArray[3][6];
  
  u[0] = xCoordinatesOfPointsOfGeometryB[1] - xCoordinatesOfPointsOfGeometryB[0];
  u[1] = yCoordinatesOfPointsOfGeometryB[1] - yCoordinatesOfPointsOfGeometryB[0];
  u[2] = zCoordinatesOfPointsOfGeometryB[1] - zCoordinatesOfPointsOfGeometryB[0];
  
  v[0] = xCoordinatesOfPointsOfGeometryB[2] - xCoordinatesOfPointsOfGeometryB[1];
  v[1] = yCoordinatesOfPointsOfGeometryB[2] - yCoordinatesOfPointsOfGeometryB[1];
  v[2] = zCoordinatesOfPointsOfGeometryB[2] - zCoordinatesOfPointsOfGeometryB[1];
  
  nn[0][0] = u[1]*v[2] - u[2]*v[1];
  nn[1][0] = u[2]*v[0] - u[0]*v[2];
  nn[2][0] = u[0]*v[1] - u[1]*v[0];
  if(nn[0][0]==0 && nn[1][0]==0 && nn[2][0]==0){return;}
  dir[0][0] = xCoordinatesOfPointsOfGeometryA[1] - xCoordinatesOfPointsOfGeometryA[0];
  dir[1][0] = yCoordinatesOfPointsOfGeometryA[1] - yCoordinatesOfPointsOfGeometryA[0];
  dir[2][0] = zCoordinatesOfPointsOfGeometryA[1] - zCoordinatesOfPointsOfGeometryA[0];
  
  w0[0][0] = xCoordinatesOfPointsOfGeometryA[0] - xCoordinatesOfPointsOfGeometryB[0];
  w0[1][0] = yCoordinatesOfPointsOfGeometryA[0] - yCoordinatesOfPointsOfGeometryB[0];
  w0[2][0] = zCoordinatesOfPointsOfGeometryA[0] - zCoordinatesOfPointsOfGeometryB[0];
  
  dir[0][1] = xCoordinatesOfPointsOfGeometryA[2] - xCoordinatesOfPointsOfGeometryA[1];
  dir[1][1] = yCoordinatesOfPointsOfGeometryA[2] - yCoordinatesOfPointsOfGeometryA[1];
  dir[2][1] = zCoordinatesOfPointsOfGeometryA[2] - zCoordinatesOfPointsOfGeometryA[1];
  
  w0[0][1] = xCoordinatesOfPointsOfGeometryA[1] - xCoordinatesOfPointsOfGeometryB[0];
  w0[1][1] = yCoordinatesOfPointsOfGeometryA[1] - yCoordinatesOfPointsOfGeometryB[0];
  w0[2][1] = zCoordinatesOfPointsOfGeometryA[1] - zCoordinatesOfPointsOfGeometryB[0];
  
  dir[0][2] = xCoordinatesOfPointsOfGeometryA[0] - xCoordinatesOfPointsOfGeometryA[2];
  dir[1][2] = yCoordinatesOfPointsOfGeometryA[0] - yCoordinatesOfPointsOfGeometryA[2];
  dir[2][2] = zCoordinatesOfPointsOfGeometryA[0] - zCoordinatesOfPointsOfGeometryA[2];
  
  w0[0][2] = xCoordinatesOfPointsOfGeometryA[2] - xCoordinatesOfPointsOfGeometryB[0];
  w0[1][2] = yCoordinatesOfPointsOfGeometryA[2] - yCoordinatesOfPointsOfGeometryB[0];
  w0[2][2] = zCoordinatesOfPointsOfGeometryA[2] - zCoordinatesOfPointsOfGeometryB[0];
  
  //T2.1 - T1;
  u[0] = xCoordinatesOfPointsOfGeometryA[1] - xCoordinatesOfPointsOfGeometryA[0];
  u[1] = yCoordinatesOfPointsOfGeometryA[1] - yCoordinatesOfPointsOfGeometryA[0];
  u[2] = zCoordinatesOfPointsOfGeometryA[1] - zCoordinatesOfPointsOfGeometryA[0];
  
  v[0] = xCoordinatesOfPointsOfGeometryA[2] - xCoordinatesOfPointsOfGeometryA[0];
  v[1] = yCoordinatesOfPointsOfGeometryA[2] - yCoordinatesOfPointsOfGeometryA[0];
  v[2] = zCoordinatesOfPointsOfGeometryA[2] - zCoordinatesOfPointsOfGeometryA[0];
  
  nn[0][1] = u[1]*v[2] - u[2]*v[1];
  nn[1][1] = u[2]*v[0] - u[0]*v[2];
  nn[2][1] = u[0]*v[1] - u[1]*v[0];
  if(nn[0][0]==0 && nn[1][1]==0 && nn[2][1]==0) {
    return;
  }
  
  dir[0][3] = xCoordinatesOfPointsOfGeometryB[1] - xCoordinatesOfPointsOfGeometryB[0];
  dir[1][3] = yCoordinatesOfPointsOfGeometryB[1] - yCoordinatesOfPointsOfGeometryB[0];
  dir[2][3] = zCoordinatesOfPointsOfGeometryB[1] - zCoordinatesOfPointsOfGeometryB[0];
  
  w0[0][3] = xCoordinatesOfPointsOfGeometryB[0] - xCoordinatesOfPointsOfGeometryA[0];
  w0[1][3] = yCoordinatesOfPointsOfGeometryB[0] - yCoordinatesOfPointsOfGeometryA[0];
  w0[2][3] = zCoordinatesOfPointsOfGeometryB[0] - zCoordinatesOfPointsOfGeometryA[0];
  
  dir[0][4] = xCoordinatesOfPointsOfGeometryB[2] - xCoordinatesOfPointsOfGeometryB[1];
  dir[1][4] = yCoordinatesOfPointsOfGeometryB[2] - yCoordinatesOfPointsOfGeometryB[1];
  dir[2][4] = zCoordinatesOfPointsOfGeometryB[2] - zCoordinatesOfPointsOfGeometryB[1];
  
  w0[0][4] = xCoordinatesOfPointsOfGeometryB[1] - xCoordinatesOfPointsOfGeometryA[0];
  w0[1][4] = yCoordinatesOfPointsOfGeometryB[1] - yCoordinatesOfPointsOfGeometryA[0];
  w0[2][4] = zCoordinatesOfPointsOfGeometryB[1] - zCoordinatesOfPointsOfGeometryA[0];
  
  dir[0][5] = xCoordinatesOfPointsOfGeometryB[0] - xCoordinatesOfPointsOfGeometryB[2];
  dir[1][5] = yCoordinatesOfPointsOfGeometryB[0] - yCoordinatesOfPointsOfGeometryB[2];
  dir[2][5] = zCoordinatesOfPointsOfGeometryB[0] - zCoordinatesOfPointsOfGeometryB[2];
  
  w0[0][5] = xCoordinatesOfPointsOfGeometryB[2] - xCoordinatesOfPointsOfGeometryA[0];
  w0[1][5] = yCoordinatesOfPointsOfGeometryB[2] - yCoordinatesOfPointsOfGeometryA[0];
  w0[2][5] = zCoordinatesOfPointsOfGeometryB[2] - zCoordinatesOfPointsOfGeometryA[0];
  
  for(int j=0;j<6;j++)
  {
    iREAL a,b;
    if(j<3)
    {
      a = -DOT(nn[0], w0[0]);
      b= DOT(nn[0],dir[0]);
    }else {
      a = -DOT(nn[1], w0[0]);
      b= DOT(nn[1],dir[0]);
    }
    if (abs(b) < 1E-30 && a==0) {
      break; //segment in triangle plane
    }
    
    iREAL r = a / b;
    
    xPA = pointArray[0][j] + r * dir[0][j];
    yPA = pointArray[1][j] + r * dir[1][j];
    zPA = pointArray[2][j] + r * dir[2][j];
    
    xPB = xPA;
    yPB = yPA;
    zPB = zPA;
    
    if (r < 0.0 || r > 1.0 || r != r) { break;}
    
    if(j<3){
      w[0] = xPA - xCoordinatesOfPointsOfGeometryB[0];
      w[1] = yPA - yCoordinatesOfPointsOfGeometryB[0];
      w[2] = zPA - zCoordinatesOfPointsOfGeometryB[0];
    }else{
      w[0] = xPA - xCoordinatesOfPointsOfGeometryA[0];
      w[1] = yPA - yCoordinatesOfPointsOfGeometryA[0];
      w[2] = zPA - zCoordinatesOfPointsOfGeometryA[0];
    }
    
    iREAL D = DOT(u,v)*DOT(u,v) - DOT(u,u)*DOT(v,v);
    iREAL s = (DOT(u,v)*DOT(w,v) - DOT(v,v)*DOT(w,u))/D;
    if (s<0.0 || s>1.0) {
      break;
    }
    
    iREAL t = (DOT(u,v)*DOT(w,u) - DOT(u,u)*DOT(w,v))/D;
    if (t<0.0 || (s+t)>1.0)
    {
      break;
    }
  }
  
  iREAL a[9], b[9], c[9], d[9], e[9], f[9], p1[3], p2[3], p3[3], p4[3], p5[3], p6[3];
  
  p1[0] = xCoordinatesOfPointsOfGeometryA[0];
  p1[1] = yCoordinatesOfPointsOfGeometryA[0];
  p1[2] = zCoordinatesOfPointsOfGeometryA[0];
  
  p2[0] = xCoordinatesOfPointsOfGeometryA[1];
  p2[1] = yCoordinatesOfPointsOfGeometryA[1];
  p2[2] = zCoordinatesOfPointsOfGeometryA[1];
  
  p3[0] = xCoordinatesOfPointsOfGeometryA[2];
  p3[1] = yCoordinatesOfPointsOfGeometryA[2];
  p3[2] = zCoordinatesOfPointsOfGeometryA[2];
  
  p4[0] = xCoordinatesOfPointsOfGeometryB[0];
  p4[1] = yCoordinatesOfPointsOfGeometryB[0];
  p4[2] = zCoordinatesOfPointsOfGeometryB[0];
  
  p5[0] = xCoordinatesOfPointsOfGeometryB[1];
  p5[1] = yCoordinatesOfPointsOfGeometryB[1];
  p5[2] = zCoordinatesOfPointsOfGeometryB[1];
  
  p6[0] = xCoordinatesOfPointsOfGeometryB[2];
  p6[1] = yCoordinatesOfPointsOfGeometryB[2];
  p6[2] = zCoordinatesOfPointsOfGeometryB[2];
  
  //ab-de, ab-df, ab-ef | bc-de, bc-df, bc-ef | ca-de, ca-df, ca-ef.
  //12-45, 12-46, 12-56 | 23-45, 23-46, 23-56 | 31-45, 31-46, 31-56.
  
  //T1.1 - T2
  u[0] = (p1[0] - p2[0]);
  u[1] = (p1[1] - p2[1]);
  u[2] = (p1[2] - p2[2]);
  
  
  v[0] = (p4[0] - p5[0]);
  v[1] = (p4[1] - p5[1]);
  v[2] = (p4[2] - p5[2]);
  
  
  w[0] = (p2[0] - p5[0]);
  w[1] = (p2[1] - p5[1]);
  w[2] = (p2[2] - p5[2]);
  
  a[0] = DOT(u,u);
  b[0] = DOT(u,v);
  c[0] = DOT(v,v);
  d[0] = DOT(u,w);
  e[0] = DOT(v,w);
  
  v[0] = (p5[0] - p6[0]);
  v[1] = (p5[1] - p6[1]);
  v[2] = (p5[2] - p6[2]);
  
  w[0] = (p2[0] - p6[0]);
  w[1] = (p2[1] - p6[1]);
  w[2] = (p2[2] - p6[2]);
  
  a[1] = a[0];
  b[1] = DOT(u,v);
  c[1] = DOT(v,v);
  d[1] = DOT(u,w);
  e[1] = DOT(v,w);
  
  v[0] = (p6[0] - p4[0]);
  v[1] = (p6[1] - p4[1]);
  v[2] = (p6[2] - p4[2]);
  
  
  w[0] = (p2[0] - p4[0]);
  w[1] = (p2[1] - p4[1]);
  w[2] = (p2[2] - p4[2]);
  
  a[2] = a[0];
  b[2] = DOT(u,v);
  c[2] = DOT(v,v);
  d[2] = DOT(u,w);
  e[2] = DOT(v,w);
  
  //T1.2 - T2
  u[0] = (p2[0] - p3[0]);
  u[1] = (p2[1] - p3[1]);
  u[2] = (p2[2] - p3[2]);
  
  
  v[0] = (p4[0] - p5[0]);
  v[1] = (p4[1] - p5[1]);
  v[2] = (p4[2] - p5[2]);
  
  w[0] = (p3[0] - p5[0]);
  w[1] = (p3[1] - p5[1]);
  w[2] = (p3[2] - p5[2]);
  
  a[3] = DOT(u,u);
  b[3] = DOT(u,v);
  c[3] = DOT(v,v);
  d[3] = DOT(u,w);
  e[3] = DOT(v,w);
  
  v[0] = (p5[0] - p6[0]);
  v[1] = (p5[1] - p6[1]);
  v[2] = (p5[2] - p6[2]);
  
  
  w[0] = (p3[0] - p6[0]);
  w[1] = (p3[1] - p6[1]);
  w[2] = (p3[2] - p6[2]);
  
  a[4] = a[3];
  b[4] = DOT(u,v);
  c[4] = DOT(v,v);
  d[4] = DOT(u,w);
  e[4] = DOT(v,w);
  
  v[0] = (p6[0] - p4[0]);
  v[1] = (p6[1] - p4[1]);
  v[2] = (p6[2] - p4[2]);
  
  w[0] = (p3[0] - p4[0]);
  w[1] = (p3[1] - p4[1]);
  w[2] = (p3[2] - p4[2]);
  
  a[5] = a[3];
  b[5] = DOT(u,v);
  c[5] = DOT(v,v);
  d[5] = DOT(u,w);
  e[5] = DOT(v,w);
  
  //T1.3 - T2
  
  u[0] = (p3[0] - p1[0]);
  u[1] = (p3[1] - p1[1]);
  u[2] = (p3[2] - p1[2]);
  
  v[0] = (p4[0] - p5[0]);
  v[1] = (p4[1] - p5[1]);
  v[2] = (p4[2] - p5[2]);
  
  w[0] = (p1[0] - p5[0]);
  w[1] = (p1[1] - p5[1]);
  w[2] = (p1[2] - p5[2]);
  
  a[6] = DOT(u,u);
  b[6] = DOT(u,v);
  c[6] = DOT(v,v);
  d[6] = DOT(u,w);
  e[6] = DOT(v,w);
  
  v[0] = (p5[0] - p6[0]);
  v[1] = (p5[1] - p6[1]);
  v[2] = (p5[2] - p6[2]);
  
  w[0] = (p1[0] - p6[0]);
  w[1] = (p1[1] - p6[1]);
  w[2] = (p1[2] - p6[2]);
  
  a[7] = a[6];
  b[7] = DOT(u,v);
  c[7] = DOT(v,v);
  d[7] = DOT(u,w);
  e[7] = DOT(v,w);
  
  v[0] = (p6[0] - p4[0]);
  v[1] = (p6[1] - p4[1]);
  v[2] = (p6[2] - p4[2]);
  
  w[0] = (p1[0] - p4[0]);
  w[1] = (p1[1] - p4[1]);
  w[2] = (p1[2] - p4[2]);
  
  a[8] = a[6];
  b[8] = DOT(u,v);
  c[8] = DOT(v,v);
  d[8] = DOT(u,w);
  e[8] = DOT(v,w);
  
  
  iREAL ssmin = 1E+30, ttc=0, ssc=0;
  int ssid = 0;
  
  for( int j=0; j<9;j++)
  {
    iREAL D = (a[j]*c[j] - b[j]*b[j]);
    
    iREAL sD = D;
    iREAL tD = D;
    iREAL sN = 0, tN = 0;
    
    iREAL SMALL_NUM = 1E-30;
    
    // compute the line parameters of the two closest points
    if (D < SMALL_NUM)
    {//the lines are almost parallel
      sN = 0.0;       //force using point P0 on segment S1
      sD = 1.0;       //to prevent possible division by 0.0 later
      tN = e[j];
      tD = c[j];
    } else {               //get the closest points on the infinite lines
      sN = (b[j]*e[j] - c[j]*d[j]);
      tN = (a[j]*e[j] - b[j]*d[j]);
      
      if (sN < 0.0){   //sc < 0 => the s=0 edge is visible
        sN = 0.0;
        tN = e[j];
        tD = c[j];
      } else if (sN > sD){//sc > 1 => the s=1 edge is visible
        sN = sD;
        tN = e[j] + b[j];
        tD = c[j];
      }
    }
    
    if (tN < 0.0){     //tc < 0 => the t=0 edge is visible
      tN = 0.0; //recompute sc for this edge
      if (-d[j] < 0.0){
        sN = 0.0;
      }else if (-d[j] > a[j]){
        sN = sD;
      } else {
        sN = -d[j];
        sD = a[j];
      }
    } else if (tN > tD){       //tc > 1 => the t=1 edge is visible
      tN = tD;  //recompute sc for this edge
      if ((-d[j] + b[j]) < 0.0){
        sN = 0;
      }else if ((-d[j] + b[j]) > a[j]){
        sN = sD;
      } else {
        sN = (-d[j] + b[j]);
        sD = a[j];
      }
    }
    
    iREAL sc, tc; //do the division to get sc and tc
    if(abs(sN) < SMALL_NUM){
      sc = 0.0;
    }else{
      sc = sN / sD;
    }
    
    if(abs(tN) < SMALL_NUM){
      tc = 0.0;
    }else{
      tc = tN / tD;
    }
    
    iREAL dP[3], dist;
    
    if(j==0)
    {
      dP[0] = (p2[0] - p5[0]) + (sc*(p1[0] - p2[0])) - (tc*(p4[0] - p5[0]));
      dP[1] = (p2[1] - p5[1]) + (sc*(p1[1] - p2[1])) - (tc*(p4[1] - p5[1]));
      dP[2] = (p2[2] - p5[2]) + (sc*(p1[2] - p2[2])) - (tc*(p4[2] - p5[2]));
    }else if(j==1)
    {
      dP[0] = (p2[0] - p6[0]) + (sc*(p1[0] - p2[0])) - (tc*(p5[0] - p6[0]));
      dP[1] = (p2[1] - p6[1]) + (sc*(p1[1] - p2[1])) - (tc*(p5[1] - p6[1]));
      dP[2] = (p2[2] - p6[2]) + (sc*(p1[2] - p2[2])) - (tc*(p5[2] - p6[2]));
    }else if(j==2)
    {
      dP[0] = (p2[0] - p4[0]) + (sc*(p1[0] - p2[0])) - (tc*(p6[0] - p4[0]));
      dP[1] = (p2[1] - p4[1]) + (sc*(p1[1] - p2[1])) - (tc*(p6[1] - p4[1]));
      dP[2] = (p2[2] - p4[2]) + (sc*(p1[2] - p2[2])) - (tc*(p6[2] - p4[2]));
    }else if(j==3)
    {
      dP[0] = (p3[0] - p5[0]) + (sc*(p2[0] - p3[0])) - (tc*(p4[0] - p5[0]));
      dP[1] = (p3[1] - p5[1]) + (sc*(p2[1] - p3[1])) - (tc*(p4[1] - p5[1]));
      dP[2] = (p3[2] - p5[2]) + (sc*(p2[2] - p3[2])) - (tc*(p4[2] - p5[2]));
    }else if(j==4)
    {
      dP[0] = (p3[0] - p6[0]) + (sc*(p2[0] - p3[0])) - (tc*(p5[0] - p6[0]));
      dP[1] = (p3[1] - p6[1]) + (sc*(p2[1] - p3[1])) - (tc*(p5[1] - p6[1]));
      dP[2] = (p3[2] - p6[2]) + (sc*(p2[2] - p3[2])) - (tc*(p5[2] - p6[2]));
    }else if(j==5)
    {
      dP[0] = (p3[0] - p4[0]) + (sc*(p2[0] - p3[0])) - (tc*(p6[0] - p4[0]));
      dP[1] = (p3[1] - p4[1]) + (sc*(p2[1] - p3[1])) - (tc*(p6[1] - p4[1]));
      dP[2] = (p3[2] - p4[2]) + (sc*(p2[2] - p3[2])) - (tc*(p6[2] - p4[2]));
    }else if(j==6)
    {
      dP[0] = (p1[0] - p5[0]) + (sc*(p3[0] - p1[0])) - (tc*(p4[0] - p5[0]));
      dP[1] = (p1[1] - p5[1]) + (sc*(p3[1] - p1[1])) - (tc*(p4[1] - p5[1]));
      dP[2] = (p1[2] - p5[2]) + (sc*(p3[2] - p1[2])) - (tc*(p4[2] - p5[2]));
    }else if(j==7)
    {
      dP[0] = (p1[0] - p6[0]) + (sc*(p3[0] - p1[0])) - (tc*(p5[0] - p6[0]));
      dP[1] = (p1[1] - p6[1]) + (sc*(p3[1] - p1[1])) - (tc*(p5[1] - p6[1]));
      dP[2] = (p1[2] - p6[2]) + (sc*(p3[2] - p1[2])) - (tc*(p5[2] - p6[2]));
    }else
    {
      dP[0] = (p1[0] - p4[0]) + (sc*(p3[0] - p1[0])) - (tc*(p6[0] - p4[0]));
      dP[1] = (p1[1] - p4[1]) + (sc*(p3[1] - p1[1])) - (tc*(p6[1] - p4[1]));
      dP[2] = (p1[2] - p4[2]) + (sc*(p3[2] - p1[2])) - (tc*(p6[2] - p4[2]));
    }
    
    dist = sqrt(dP[0]*dP[0]+dP[1]*dP[1]+dP[2]*dP[2]);
    
    if(dist<ssmin) {
      ssmin = dist;
      ttc = tc;
      ssc = sc;
      ssid = j;
    }
  }
  
  switch(ssid)
  {
    case 0:
      //T1.1 - T2
      u[0] = (p1[0] - p2[0]);
      u[1] = (p1[1] - p2[1]);
      u[2] = (p1[2] - p2[2]);
      
      v[0] = (p4[0] - p5[0]);
      v[1] = (p4[1] - p5[1]);
      v[2] = (p4[2] - p5[2]);
      
      xPA = p2[0] + (u[0] * ssc);
      yPA = p2[1] + (u[1] * ssc);
      zPA = p2[2] + (u[2] * ssc);
      
      xPB = p5[0] + (v[0] * ttc);
      yPB = p5[1] + (v[1] * ttc);
      zPB = p5[2] + (v[2] * ttc);
      break;
    case 1:
      u[0] = (p1[0] - p2[0]);
      u[1] = (p1[1] - p2[1]);
      u[2] = (p1[2] - p2[2]);
      
      v[0] = (p5[0] - p6[0]);
      v[1] = (p5[1] - p6[1]);
      v[2] = (p5[2] - p6[2]);
      
      xPA = p2[0] + (u[0] * ssc);
      yPA = p2[1] + (u[1] * ssc);
      zPA = p2[2] + (u[2] * ssc);
      
      xPB = p6[0] + (v[0] * ttc);
      yPB = p6[1] + (v[1] * ttc);
      zPB = p6[2] + (v[2] * ttc);
      break;
    case 2:
      u[0] = (p1[0] - p2[0]);
      u[1] = (p1[1] - p2[1]);
      u[2] = (p1[2] - p2[2]);
      
      v[0] = (p6[0] - p4[0]);
      v[1] = (p6[1] - p4[1]);
      v[2] = (p6[2] - p4[2]);
      
      xPA = p2[0] + (u[0] * ssc);
      yPA = p2[1] + (u[1] * ssc);
      zPA = p2[2] + (u[2] * ssc);
      
      xPB = p4[0] + (v[0] * ttc);
      yPB = p4[1] + (v[1] * ttc);
      zPB = p4[2] + (v[2] * ttc);
      break;
    case 3:
      //T1.2 - T2
      u[0] = (p2[0] - p3[0]);
      u[1] = (p2[1] - p3[1]);
      u[2] = (p2[2] - p3[2]);
      
      v[0] = (p4[0] - p5[0]);
      v[1] = (p4[1] - p5[1]);
      v[2] = (p4[2] - p5[2]);
      
      xPA = p3[0] + (u[0] * ssc);
      yPA = p3[1] + (u[1] * ssc);
      zPA = p3[2] + (u[2] * ssc);
      
      xPB = p5[0] + (v[0] * ttc);
      yPB = p5[1] + (v[1] * ttc);
      zPB = p5[2] + (v[2] * ttc);
      break;
    case 4:
      u[0] = (p2[0] - p3[0]);
      u[1] = (p2[1] - p3[1]);
      u[2] = (p2[2] - p3[2]);
      
      v[0] = (p5[0] - p6[0]);
      v[1] = (p5[1] - p6[1]);
      v[2] = (p5[2] - p6[2]);
      
      xPA = p3[0] + (u[0] * ssc);
      yPA = p3[1] + (u[1] * ssc);
      zPA = p3[2] + (u[2] * ssc);
      
      xPB = p6[0] + (v[0] * ttc);
      yPB = p6[1] + (v[1] * ttc);
      zPB = p6[2] + (v[2] * ttc);
      break;
    case 5:
      u[0] = (p2[0] - p3[0]);
      u[1] = (p2[1] - p3[1]);
      u[2] = (p2[2] - p3[2]);
      
      v[0] = (p6[0] - p4[0]);
      v[1] = (p6[1] - p4[1]);
      v[2] = (p6[2] - p4[2]);
      
      xPA = p3[0] + (u[0] * ssc);
      yPA = p3[1] + (u[1] * ssc);
      zPA = p3[2] + (u[2] * ssc);
      
      xPB = p4[0] + (v[0] * ttc);
      yPB = p4[1] + (v[1] * ttc);
      zPB = p4[2] + (v[2] * ttc);
      break;
    case 6:
      //T1.3 - T2
      u[0] = (p3[0] - p1[0]);
      u[1] = (p3[1] - p1[1]);
      u[2] = (p3[2] - p1[2]);
      
      v[0] = (p4[0] - p5[0]);
      v[1] = (p4[1] - p5[1]);
      v[2] = (p4[2] - p5[2]);
      
      xPA = p1[0] + (u[0] * ssc);
      yPA = p1[1] + (u[1] * ssc);
      zPA = p1[2] + (u[2] * ssc);
      
      xPB = p5[0] + (v[0] * ttc);
      yPB = p5[1] + (v[1] * ttc);
      zPB = p5[2] + (v[2] * ttc);
      break;
    case 7:
      u[0] = (p3[0] - p1[0]);
      u[1] = (p3[1] - p1[1]);
      u[2] = (p3[2] - p1[2]);
      
      v[0] = (p5[0] - p6[0]);
      v[1] = (p5[1] - p6[1]);
      v[2] = (p5[2] - p6[2]);
      
      xPA = p1[0] + (u[0] * ssc);
      yPA = p1[1] + (u[1] * ssc);
      zPA = p1[2] + (u[2] * ssc);
      
      xPB = p6[0] + (v[0] * ttc);
      yPB = p6[1] + (v[1] * ttc);
      zPB = p6[2] + (v[2] * ttc);
      break;
    case 8:
      u[0] = (p3[0] - p1[0]);
      u[1] = (p3[1] - p1[1]);
      u[2] = (p3[2] - p1[2]);
      
      v[0] = (p6[0] - p4[0]);
      v[1] = (p6[1] - p4[1]);
      v[2] = (p6[2] - p4[2]);
      
      xPA = p1[0] + (u[0] * ssc);
      yPA = p1[1] + (u[1] * ssc);
      zPA = p1[2] + (u[2] * ssc);
      
      xPB = p4[0] + (v[0] * ttc);
      yPB = p4[1] + (v[1] * ttc);
      zPB = p4[2] + (v[2] * ttc);
      break;
  }
  
  a[0] = (p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1]) + (p2[2] - p1[2])*(p2[2] - p1[2]);
  b[0] = (p2[0] - p1[0])*(p3[0] - p1[0]) + (p2[1] - p1[1])*(p3[1] - p1[1]) + (p2[2] - p1[2])*(p3[2] - p1[2]);
  c[0] = (p3[0] - p1[0])*(p3[0] - p1[0]) + (p3[1] - p1[1])*(p3[1] - p1[1]) + (p3[2] - p1[2])*(p3[2] - p1[2]);
  d[0] = (p2[0] - p1[0])*(p1[0] - p4[0]) + (p2[1] - p1[1])*(p1[1] - p4[1]) + (p2[2] - p1[2])*(p1[2] - p4[2]);
  e[0] = (p3[0] - p1[0])*(p1[0] - p4[0]) + (p3[1] - p1[1])*(p1[1] - p4[1]) + (p3[2] - p1[2])*(p1[2] - p4[2]);
  f[0] = (p1[0] - p4[0])*(p1[0] - p4[0]) + (p1[1] - p4[1])*(p1[1] - p4[1]) + (p1[2] - p4[2])*(p1[2] - p4[2]);
  
  a[1] = (p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1]) + (p2[2] - p1[2])*(p2[2] - p1[2]);
  b[1] = (p2[0] - p1[0])*(p3[0] - p1[0]) + (p2[1] - p1[1])*(p3[1] - p1[1]) + (p2[2] - p1[2])*(p3[2] - p1[2]);
  c[1] = (p3[0] - p1[0])*(p3[0] - p1[0]) + (p3[1] - p1[1])*(p3[1] - p1[1]) + (p3[2] - p1[2])*(p3[2] - p1[2]);
  d[1] = (p2[0] - p1[0])*(p1[0] - p5[0]) + (p2[1] - p1[1])*(p1[1] - p5[1]) + (p2[2] - p1[2])*(p1[2] - p5[2]);
  e[1] = (p3[0] - p1[0])*(p1[0] - p5[0]) + (p3[1] - p1[1])*(p1[1] - p5[1]) + (p3[2] - p1[2])*(p1[2] - p5[2]);
  f[1] = (p1[0] - p5[0])*(p1[0] - p5[0]) + (p1[1] - p5[1])*(p1[1] - p5[1]) + (p1[2] - p5[2])*(p1[2] - p5[2]);
  
  a[2] = (p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1]) + (p2[2] - p1[2])*(p2[2] - p1[2]);
  b[2] = (p2[0] - p1[0])*(p3[0] - p1[0]) + (p2[1] - p1[1])*(p3[1] - p1[1]) + (p2[2] - p1[2])*(p3[2] - p1[2]);
  c[2] = (p3[0] - p1[0])*(p3[0] - p1[0]) + (p3[1] - p1[1])*(p3[1] - p1[1]) + (p3[2] - p1[2])*(p3[2] - p1[2]);
  d[2] = (p2[0] - p1[0])*(p1[0] - p6[0]) + (p2[1] - p1[1])*(p1[1] - p6[1]) + (p2[2] - p1[2])*(p1[2] - p6[2]);
  e[2] = (p3[0] - p1[0])*(p1[0] - p6[0]) + (p3[1] - p1[1])*(p1[1] - p6[1]) + (p3[2] - p1[2])*(p1[2] - p6[2]);
  f[2] = (p1[0] - p6[0])*(p1[0] - p6[0]) + (p1[1] - p6[1])*(p1[1] - p6[1]) + (p1[2] - p6[2])*(p1[2] - p6[2]);
  
  ///
  
  a[3] = (p5[0] - p4[0])*(p5[0] - p4[0]) + (p5[1] - p4[1])*(p5[1] - p4[1]) + (p5[2] - p4[2])*(p5[2] - p4[2]);
  b[3] = (p5[0] - p4[0])*(p6[0] - p4[0]) + (p5[1] - p4[1])*(p6[1] - p4[1]) + (p5[2] - p4[2])*(p6[2] - p4[2]);
  c[3] = (p6[0] - p4[0])*(p6[0] - p4[0]) + (p6[1] - p4[1])*(p6[1] - p4[1]) + (p6[2] - p4[2])*(p6[2] - p4[2]);
  d[3] = (p5[0] - p4[0])*(p4[0] - p1[0]) + (p5[1] - p4[1])*(p4[1] - p1[1]) + (p5[2] - p4[2])*(p4[2] - p1[2]);
  e[3] = (p6[0] - p4[0])*(p4[0] - p1[0]) + (p6[1] - p4[1])*(p4[1] - p1[1]) + (p6[2] - p4[2])*(p4[2] - p1[2]);
  f[3] = (p4[0] - p1[0])*(p4[0] - p1[0]) + (p4[1] - p1[1])*(p4[1] - p1[1]) + (p4[2] - p1[2])*(p4[2] - p1[2]);
  
  a[4] = (p5[0] - p4[0])*(p5[0] - p4[0]) + (p5[1] - p4[1])*(p5[1] - p4[1]) + (p5[2] - p4[2])*(p5[2] - p4[2]);
  b[4] = (p5[0] - p4[0])*(p6[0] - p4[0]) + (p5[1] - p4[1])*(p6[1] - p4[1]) + (p5[2] - p4[2])*(p6[2] - p4[2]);
  c[4] = (p6[0] - p4[0])*(p6[0] - p4[0]) + (p6[1] - p4[1])*(p6[1] - p4[1]) + (p6[2] - p4[2])*(p6[2] - p4[2]);
  d[4] = (p5[0] - p4[0])*(p4[0] - p2[0]) + (p5[1] - p4[1])*(p4[1] - p2[1]) + (p5[2] - p4[2])*(p4[2] - p2[2]);
  e[4] = (p6[0] - p4[0])*(p4[0] - p2[0]) + (p6[1] - p4[1])*(p4[1] - p2[1]) + (p6[2] - p4[2])*(p4[2] - p2[2]);
  f[4] = (p4[0] - p2[0])*(p4[0] - p2[0]) + (p4[1] - p2[1])*(p4[1] - p2[1]) + (p4[2] - p2[2])*(p4[2] - p2[2]);
  
  a[5] = (p5[0] - p4[0])*(p5[0] - p4[0]) + (p5[1] - p4[1])*(p5[1] - p4[1]) + (p5[2] - p4[2])*(p5[2] - p4[2]);
  b[5] = (p5[0] - p4[0])*(p6[0] - p4[0]) + (p5[1] - p4[1])*(p6[1] - p4[1]) + (p5[2] - p4[2])*(p6[2] - p4[2]);
  c[5] = (p6[0] - p4[0])*(p6[0] - p4[0]) + (p6[1] - p4[1])*(p6[1] - p4[1]) + (p6[2] - p4[2])*(p6[2] - p4[2]);
  d[5] = (p5[0] - p4[0])*(p4[0] - p3[0]) + (p5[1] - p4[1])*(p4[1] - p3[1]) + (p5[2] - p4[2])*(p4[2] - p3[2]);
  e[5] = (p6[0] - p4[0])*(p4[0] - p3[0]) + (p6[1] - p4[1])*(p4[1] - p3[1]) + (p6[2] - p4[2])*(p4[2] - p3[2]);
  f[5] = (p4[0] - p3[0])*(p4[0] - p3[0]) + (p4[1] - p3[1])*(p4[1] - p3[1]) + (p4[2] - p3[2])*(p4[2] - p3[2]);
  
  iREAL ss = 0, tt = 0;
  iREAL ptmin = 1E30;
  int id=0;
  for( int j=0; j<6;j++)
  {
    iREAL det = a[j]*c[j] - b[j]*b[j];
    iREAL s   = b[j]*e[j] - c[j]*d[j];
    iREAL t   = b[j]*d[j] - a[j]*e[j];
    
    iREAL sqrDistance=0;
    
    if ((s+t) <= det)
    {
      if (s < 0){
        if (t < 0){
          //region4
          if (d[j] < 0){
            t = 0;
            if (-d[j] >= a[j]){
              s = 1;
              sqrDistance = a[j] + 2*d[j] + f[j];
            }else {
              s = -d[j]/a[j];
              sqrDistance = d[j]*s + f[j];
            }
          }else {
            s = 0;
            if (e[j] >= 0){
              t = 0;
              sqrDistance = f[j];
            }else{
              if (-e[j] >= c[j]){
                t = 1;
                sqrDistance = c[j] + 2*e[j] + f[j];
              } else {
                t = -e[j]/c[j];
                sqrDistance = e[j]*t + f[j];
              }
            }
          } //end of region 4
        }else {
          // region 3
          s = 0;
          if (e[j] >= 0){
            t = 0;
            sqrDistance = f[j];
          }else {
            if (-e[j] >= c[j]){
              t = 1;
              sqrDistance = c[j] + 2*e[j] +f[j];
            }else {
              t = -e[j]/c[j];
              sqrDistance = e[j]*t + f[j];
            }
          }
        } //end of region 3
      }else {
        if (t < 0){
          // region 5
          t = 0;
          if (d[j] >= 0){
            s = 0;
            sqrDistance = f[j];
          }else {
            if (-d[j] >= a[j]){
              s = 1;
              sqrDistance = a[j] + 2*d[j] + f[j];// GF 20101013 fixed typo d*s ->2*d
            }else {
              s = -d[j]/a[j];
              sqrDistance = d[j]*s + f[j];
            }
          }
        }else {
          // region 0
          iREAL invDet = 1/det;
          s = s*invDet;
          t = t*invDet;
          sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
        }
      }
    }
    else
    {
      if (s < 0){
        // region 2
        iREAL tmp0 = b[j] + d[j];
        iREAL tmp1 = c[j] + e[j];
        if (tmp1 > tmp0){ // minimum on edge s+t=1
          iREAL numer = tmp1 - tmp0;
          iREAL denom = a[j] - 2*b[j] + c[j];
          if (numer >= denom){
            s = 1;
            t = 0;
            sqrDistance = a[j] + 2*d[j] + f[j]; // GF 20101014 fixed typo 2*b -> 2*d
          }else {
            s = numer/denom;
            t = 1-s;
            sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
          }
        }else {         // minimum on edge s=0
          s = 0;
          if (tmp1 <= 0) {
            t = 1;
            sqrDistance = c[j] + 2*e[j] + f[j];
          }else {
            if (e[j] >= 0){
              t = 0;
              sqrDistance = f[j];
            }else {
              t = -e[j]/c[j];
              sqrDistance = e[j]*t + f[j];
            }
          }
        } //end of region 2
      }else {
        if (t < 0) {
          //region6
          iREAL tmp0 = b[j] + e[j];
          iREAL tmp1 = a[j] + d[j];
          if (tmp1 > tmp0){
            iREAL numer = tmp1 - tmp0;
            iREAL denom = a[j]-2*b[j]+c[j];
            if (numer >= denom){
              t = 1;
              s = 0;
              sqrDistance = c[j] + 2*e[j] + f[j];
            }else {
              t = numer/denom;
              s = 1 - t;
              sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
            }
          }else {
            t = 0;
            if (tmp1 <= 0){
              s = 1;
              sqrDistance = a[j] + 2*d[j] + f[j];
            }else {
              if (d[j] >= 0) {
                s = 0;
                sqrDistance = f[j];
              }else {
                s = -d[j]/a[j];
                sqrDistance = d[j]*s + f[j];
              }
            }
          }
          //end of region 6
        }else {
          // region 1
          iREAL numer = c[j] + e[j] - b[j] - d[j];
          if (numer <= 0){
            s = 0;
            t = 1;
            sqrDistance = c[j] + 2*e[j] + f[j];
          }else {
            iREAL denom = a[j] - 2*b[j] + c[j];
            if (numer >= denom){
              s = 1;
              t = 0;
              sqrDistance = a[j] + 2*d[j] + f[j];
            }else {
              s = numer/denom;
              t = 1-s;
              sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
            }
          } //end of region 1
        }
      }
    }
    
    // account for numerical round-off error
    if (sqrDistance < 0){sqrDistance = 0;}
    
    iREAL dist = sqrt(sqrDistance);
    if(dist<ptmin)
    {
      ptmin = dist;
      ss = s;
      tt = t;
      id = j;
    }
  }
  if(ptmin > ssmin) {return;}
  
  switch(id)
  {
    case 0:
      xPA = p1[0] + ((p3[0] - p1[0]) * tt) + ((p2[0] - p1[0]) * ss);
      yPA = p1[1] + ((p3[1] - p1[1]) * tt) + ((p2[1] - p1[1]) * ss);
      zPA = p1[2] + ((p3[2] - p1[2]) * tt) + ((p2[2] - p1[2]) * ss);
      
      xPB = p4[0];
      yPB = p4[1];
      zPB = p4[2];
      break;
    case 1:
      xPA = p1[0] + ((p3[0] - p1[0]) * tt) + ((p2[0] - p1[0]) * ss);
      yPA = p1[1] + ((p3[1] - p1[1]) * tt) + ((p2[1] - p1[1]) * ss);
      zPA = p1[2] + ((p3[2] - p1[2]) * tt) + ((p2[2] - p1[2]) * ss);
      
      xPB = p5[0];
      yPB = p5[1];
      zPB = p5[2];
      break;
    case 2:
      xPA = p1[0] + ((p3[0] - p1[0]) * tt) + ((p2[0] - p1[0]) * ss);
      yPA = p1[1] + ((p3[1] - p1[1]) * tt) + ((p2[1] - p1[1]) * ss);
      zPA = p1[2] + ((p3[2] - p1[2]) * tt) + ((p2[2] - p1[2]) * ss);
      
      xPB = p6[0];
      yPB = p6[1];
      zPB = p6[2];
      break;
    case 3:
      xPA = p1[0];
      yPA = p1[1];
      zPA = p1[2];
      
      xPB = p4[0] + ((p6[0] - p4[0]) * tt) + ((p5[0] - p4[0]) * ss);
      yPB = p4[1] + ((p6[1] - p4[1]) * tt) + ((p5[1] - p4[1]) * ss);
      zPB = p4[2] + ((p6[2] - p4[2]) * tt) + ((p5[2] - p4[2]) * ss);
      break;
    case 4:
      xPA = p2[0];
      yPA = p2[1];
      zPA = p2[2];
      
      xPB = p4[0] + ((p6[0] - p4[0]) * tt) + ((p5[0] - p4[0]) * ss);
      yPB = p4[1] + ((p6[1] - p4[1]) * tt) + ((p5[1] - p4[1]) * ss);
      zPB = p4[2] + ((p6[2] - p4[2]) * tt) + ((p5[2] - p4[2]) * ss);
      break;
    case 5:
      xPA = p3[0];
      yPA = p3[1];
      zPA = p3[2];
      
      xPB = p4[0] + ((p6[0] - p4[0]) * tt) + ((p5[0] - p4[0]) * ss);
      yPB = p4[1] + ((p6[1] - p4[1]) * tt) + ((p5[1] - p4[1]) * ss);
      zPB = p4[2] + ((p6[2] - p4[2]) * tt) + ((p5[2] - p4[2]) * ss);
      break;
  }
}
