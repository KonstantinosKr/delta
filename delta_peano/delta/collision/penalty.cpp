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


#include "penalty.h"

namespace {
  //__attribute__ ((aligned(byteAlignment))) const int MaxNumberOfNewtonIterations = 8;

  std::vector<int> numberOfNewtonIterations(MaxNumberOfNewtonIterations+1);
}

void delta::collision::cleanPenaltyStatistics() {
  #if defined(__INTEL_COMPILER)
    #pragma omp simd
  #else
    #pragma omp simd
  #endif
  for(int i=0; i<MaxNumberOfNewtonIterations+1; i++) {
    numberOfNewtonIterations[i] = 0;
  }
}

std::vector<int> delta::collision::getPenaltyStatistics() {
  return numberOfNewtonIterations;
}

std::vector<delta::collision::contactpoint> delta::collision::penaltyStat(
  int       numberOfTrianglesOfGeometryA,
  const iREAL*   xCoordinatesOfPointsOfGeometryA,
  const iREAL*   yCoordinatesOfPointsOfGeometryA,
  const iREAL*   zCoordinatesOfPointsOfGeometryA,
  iREAL    epsilonA,
  bool      frictionA,
  int 		  particleA,

  int       numberOfTrianglesOfGeometryB,
  const iREAL*   xCoordinatesOfPointsOfGeometryB,
  const iREAL*   yCoordinatesOfPointsOfGeometryB,
  const iREAL*   zCoordinatesOfPointsOfGeometryB,
  iREAL    epsilonB,
  bool      frictionB,
  int 		  particleB)
{
  const iREAL MaxError = (epsilonA+epsilonB) / 16.0;

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
  for (int iA = 0; iA<numberOfTrianglesOfGeometryA*3; iA+=3)
  {
    __attribute__ ((aligned(byteAlignment))) iREAL	epsilonMargin = (epsilonA+epsilonB);
    contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) iREAL dd = 1E99;

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
      #pragma omp simd
    #else
      #pragma omp simd
    #endif
    for (int iB = 0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      __attribute__ ((aligned(byteAlignment))) iREAL xPA;
      __attribute__ ((aligned(byteAlignment))) iREAL yPA;
      __attribute__ ((aligned(byteAlignment))) iREAL zPA;
      __attribute__ ((aligned(byteAlignment))) iREAL xPB;
      __attribute__ ((aligned(byteAlignment))) iREAL yPB;
      __attribute__ ((aligned(byteAlignment))) iREAL zPB;

      __attribute__ ((aligned(byteAlignment))) int numberOfNewtonIterationsRequired  = 0;

      penalty(xCoordinatesOfPointsOfGeometryA+(iA),
              yCoordinatesOfPointsOfGeometryA+(iA),
              zCoordinatesOfPointsOfGeometryA+(iA),
              xCoordinatesOfPointsOfGeometryB+(iB),
              yCoordinatesOfPointsOfGeometryB+(iB),
              zCoordinatesOfPointsOfGeometryB+(iB),
              xPA, yPA, zPA, xPB, yPB, zPB,
              MaxError,
              numberOfNewtonIterationsRequired);

      numberOfNewtonIterations[numberOfNewtonIterationsRequired]++;

      iREAL d = std::sqrt(((xPB-xPA)*(xPB-xPA))+((yPB-yPA)*(yPB-yPA))+((zPB-zPA)*(zPB-zPA)));
      if (d <= epsilonMargin && d <= dd)
      {
        nearestContactPoint = new contactpoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA && frictionB);;
        dd    = d;
      }
    }
    if (nearestContactPoint != nullptr)
    {
      #ifdef ompTriangle
        #pragma omp critical
      #endif
      result.push_back(*nearestContactPoint);
    }
  }
  return result;
}

std::vector<delta::collision::contactpoint> delta::collision::penalty(
  int             numberOfTrianglesOfGeometryA,
  const iREAL*    xCoordinatesOfPointsOfGeometryA,
  const iREAL*    yCoordinatesOfPointsOfGeometryA,
  const iREAL*    zCoordinatesOfPointsOfGeometryA,
  const iREAL     epsilonA,
  bool            frictionA,
  int	  	          particleA,

  int             numberOfTrianglesOfGeometryB,
  const iREAL*    xCoordinatesOfPointsOfGeometryB,
  const iREAL*    yCoordinatesOfPointsOfGeometryB,
  const iREAL*    zCoordinatesOfPointsOfGeometryB,
  const iREAL     epsilonB,
  bool            frictionB,
  int		          particleB
) {
  __attribute__ ((aligned(byteAlignment)))  const iREAL MaxError = (epsilonA+epsilonB) / 16.0;

  std::vector<contactpoint> __attribute__ ((aligned(byteAlignment))) result;

  #if defined(__INTEL_COMPILER)
  __assume_aligned(xCoordinatesOfPointsOfGeometryA, byteAlignment);
  __assume_aligned(yCoordinatesOfPointsOfGeometryA, byteAlignment);
  __assume_aligned(zCoordinatesOfPointsOfGeometryA, byteAlignment);

  __assume_aligned(xCoordinatesOfPointsOfGeometryB, byteAlignment);
  __assume_aligned(yCoordinatesOfPointsOfGeometryB, byteAlignment);
  __assume_aligned(zCoordinatesOfPointsOfGeometryB, byteAlignment);
  #endif

  #ifdef ompTriangle
    #pragma omp parallel for schedule(static) shared(result) firstprivate(numberOfTrianglesOfGeometryA, numberOfTrianglesOfGeometryB, epsilonA, epsilonB, frictionA, frictionB, particleA, particleB, xCoordinatesOfPointsOfGeometryA, yCoordinatesOfPointsOfGeometryA, zCoordinatesOfPointsOfGeometryA, xCoordinatesOfPointsOfGeometryB, yCoordinatesOfPointsOfGeometryB, zCoordinatesOfPointsOfGeometryB)
  #endif
  for (int iA=0; iA<numberOfTrianglesOfGeometryA*3; iA+=3)
  {
    __attribute__ ((aligned(byteAlignment))) bool failed = 0;
    __attribute__ ((aligned(byteAlignment))) iREAL xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], d[10000];

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
      #pragma omp simd
    #else
      #pragma omp simd
    #endif
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      penalty(xCoordinatesOfPointsOfGeometryA+(iA),
              yCoordinatesOfPointsOfGeometryA+(iA),
              zCoordinatesOfPointsOfGeometryA+(iA),
              xCoordinatesOfPointsOfGeometryB+(iB),
              yCoordinatesOfPointsOfGeometryB+(iB),
              zCoordinatesOfPointsOfGeometryB+(iB),
              xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB],
              MaxError, failed);

      d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    __attribute__ ((aligned(byteAlignment))) iREAL epsilonMargin = (epsilonA+epsilonB);
    contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) iREAL dd = 1E99;

    __attribute__ ((aligned(byteAlignment))) iREAL minD = 1E99;
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3) {
      minD          = std::min( d[iB], minD );
    }

    // Grab the closest one and insert it into the result
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3) {
      if ( d[iB] <= minD && d[iB] < epsilonMargin) {
        delta::collision::contactpoint* nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        #ifdef ompTriangle
          #pragma omp critical
        #endif
        result.push_back(*nearestContactPoint);
      }
    }
  }
  return result;
}

/**
 * penalty method for nonlinear constrained optimization of convex problem
 *
 * Penalty is the iterative Newton solver method that solves the optimization problem
 * of finding the distance between two triangles in 3D.
 *
 * This operation always runs a fixed number of Newton iterations.
 *
 * @author Konstantinos Krestenitis
 *
 * @param numberOfNewtonIterations  Number of Newton iterations to be ran.
 *            The routine always runs exactly this number of iterations no
 *            matter whether the iterations do converge or not.
 * @param error This is an out parameter where the iteration tells the
 *            invoking code what the final error has been.
 *
 * @param xCoordinatesOfTriangle[(iA*3)+3] x coordinates of triangle A
 * @param yCoordinatesOfTriangle[(iA*3)+3] y coordinates of triangle A
 * @param zCoordinatesOfTriangle[(iA*3)+3] z coordinates of triangle A
 * @param xCoordinatesOfTriangleB[3] x coordinates of triangle B
 * @param yCoordinatesOfTriangleB[3] y coordinates of triangle B
 * @param zCoordinatesOfTriangleB[3] z coordinates of triangle B
 * @param xPA x coordinate of Point P on triangle A that define closest distance between triangle A-B
 * @param xPA y coordinate of Point P on triangle A that define closest distance between triangle A-B
 * @param xPA z coordinate of Point P on triangle A that define closest distance between triangle A-B
 * @param xPA x coordinate of Point Q on triangle B that define closest distance between triangle B-A
 * @param xPA y coordinate of Point Q on triangle B that define closest distance between triangle B-A
 * @param xPA z coordinate of Point Q on triangle B that define closest distance between triangle B-A
 *
 */
#if defined(__INTEL_COMPILER)
  #if defined(ompParticle) || defined(ompTriangle)
  #pragma omp declare simd
  #pragma omp declare simd linear(xCoordinatesOfTriangleA:3) linear(yCoordinatesOfTriangleA:3) linear(zCoordinatesOfTriangleA:3) linear(xCoordinatesOfTriangleB:3) linear(yCoordinatesOfTriangleB:3) linear(zCoordinatesOfTriangleB:3) nomask notinbranch
  #endif
#endif
extern void delta::collision::penalty(
  const iREAL   *xCoordinatesOfTriangleA,
  const iREAL   *yCoordinatesOfTriangleA,
  const iREAL   *zCoordinatesOfTriangleA,
  const iREAL   *xCoordinatesOfTriangleB,
  const iREAL   *yCoordinatesOfTriangleB,
  const iREAL   *zCoordinatesOfTriangleB,
  iREAL&  xPA,
  iREAL&  yPA,
  iREAL&  zPA,
  iREAL&  xPB,
  iREAL&  yPB,
  iREAL&  zPB,
  iREAL MaxErrorOfPenaltyMethod,
  bool& failed)
{

   __attribute__ ((aligned(byteAlignment))) iREAL BA[3];
   __attribute__ ((aligned(byteAlignment))) iREAL CA[3];
   __attribute__ ((aligned(byteAlignment))) iREAL ED[3];
   __attribute__ ((aligned(byteAlignment))) iREAL FD[3];
   __attribute__ ((aligned(byteAlignment))) iREAL hessian[16];
   __attribute__ ((aligned(byteAlignment))) iREAL x[4];

   BA[0] = xCoordinatesOfTriangleA[1] - xCoordinatesOfTriangleA[0];
   BA[1] = yCoordinatesOfTriangleA[1] - yCoordinatesOfTriangleA[0];
   BA[2] = zCoordinatesOfTriangleA[1] - zCoordinatesOfTriangleA[0];

   CA[0] = xCoordinatesOfTriangleA[2] - xCoordinatesOfTriangleA[0];
   CA[1] = yCoordinatesOfTriangleA[2] - yCoordinatesOfTriangleA[0];
   CA[2] = zCoordinatesOfTriangleA[2] - zCoordinatesOfTriangleA[0];

   ED[0] = xCoordinatesOfTriangleB[1] - xCoordinatesOfTriangleB[0];
   ED[1] = yCoordinatesOfTriangleB[1] - yCoordinatesOfTriangleB[0];
   ED[2] = zCoordinatesOfTriangleB[1] - zCoordinatesOfTriangleB[0];

   FD[0] = xCoordinatesOfTriangleB[2] - xCoordinatesOfTriangleB[0];
   FD[1] = yCoordinatesOfTriangleB[2] - yCoordinatesOfTriangleB[0];
   FD[2] = zCoordinatesOfTriangleB[2] - zCoordinatesOfTriangleB[0];

   hessian[0] = 2.*DOT(BA,BA);
   hessian[1] = 2.*DOT(CA,BA);
   hessian[2] = -2.*DOT(ED,BA);
   hessian[3] = -2.*DOT(FD,BA);

   hessian[4] = hessian[1]; //use symmetry
   hessian[5] = 2.*DOT(CA,CA);
   hessian[6] = -2.*DOT(ED,CA);
   hessian[7] = -2.*DOT(FD,CA);

   hessian[8] = hessian[2];
   hessian[9] = hessian[6];
   hessian[10] = 2.*DOT(ED,ED);
   hessian[11] = 2.*DOT(FD,ED);

   hessian[12] = hessian[3];
   hessian[13] = hessian[7];
   hessian[14] = hessian[11];
   hessian[15] = 2.*DOT(FD,FD);

   __attribute__ ((aligned(byteAlignment))) iREAL eps = 1E-2;
   __attribute__ ((aligned(byteAlignment))) iREAL delta = (hessian[0]+hessian[5]+hessian[10]+hessian[15]) * eps;
   __attribute__ ((aligned(byteAlignment))) iREAL lambda = sqrt(0.0125*(hessian[0]+hessian[5]+hessian[10]+hessian[15]));
   __attribute__ ((aligned(byteAlignment))) iREAL r = lambda*1E5;

   //initial guess
   x[0] = 0.33;
   x[1] = 0.33;
   x[2] = 0.33;
   x[3] = 0.33;

  //Newton loop
  for(int i=0;i<4 ;i++)
  {
   //Declare loop variables;
    __attribute__ ((aligned(byteAlignment))) iREAL dx[4];
    __attribute__ ((aligned(byteAlignment))) iREAL a[16];
    __attribute__ ((aligned(byteAlignment))) iREAL SUBXY[3];
    __attribute__ ((aligned(byteAlignment))) iREAL b[4];
    __attribute__ ((aligned(byteAlignment))) iREAL dh[8];
    __attribute__ ((aligned(byteAlignment))) iREAL mx[6];
    __attribute__ ((aligned(byteAlignment))) iREAL tmp1, tmp2,tmp3, tmp4, tmp5, tmp6;

    dh[0] = (-x[0] <= 0) ? 0.0 : -1;
    mx[0] = (-x[0] <= 0) ? 0.0 : -x[0];

    dh[2] = (-x[1] <= 0) ? 0.0 : -1;
    mx[1] = (-x[1] <= 0) ? 0.0 : -x[1];

    dh[1] = dh[3] = (x[0]+x[1]-1 <= 0) ? 0.0 : 1;
    mx[2] = (x[0]+x[1]-1 <= 0) ? 0.0 : x[0]+x[1]-1;

    dh[4] = (-x[2] <= 0) ? 0.0 : -1;
    mx[3] = (-x[2] <= 0) ? 0.0 : -x[2];

    dh[6] = (-x[3] <= 0) ? 0.0 : -1;
    mx[4] = (-x[3] <= 0) ? 0.0 : -x[3];

    dh[5] = dh[7] = (x[2]+x[3]-1 <= 0) ? 0.0 : 1;
    mx[5] = (x[2]+x[3]-1 <=0) ? 0.0 : x[2]+x[3]-1;

    SUBXY[0] = (xCoordinatesOfTriangleA[0]+(BA[0] * x[0])+(CA[0] * x[1])) - (xCoordinatesOfTriangleB[0]+(ED[0] * x[2])+(FD[0] * x[3]));
    SUBXY[1] = (yCoordinatesOfTriangleA[0]+(BA[1] * x[0])+(CA[1] * x[1])) - (yCoordinatesOfTriangleB[0]+(ED[1] * x[2])+(FD[1] * x[3]));
    SUBXY[2] = (zCoordinatesOfTriangleA[0]+(BA[2] * x[0])+(CA[2] * x[1])) - (zCoordinatesOfTriangleB[0]+(ED[2] * x[2])+(FD[2] * x[3]));

    b[0] = 2*DOT(SUBXY,BA) + r * (dh[0] * mx[0] + dh[1] * mx[2]);
    a[0] = hessian[0] + r * (dh[0] * dh[0] + dh[1] * dh[1]) + delta;
    a[4] = hessian[4] + r * (dh[3] * dh[1]);
    tmp1 = (hessian[1] + r * (dh[1] * dh[3]))/a[0];
    a[13] = hessian[13] - hessian[12] * tmp1;
    a[9] = hessian[9] - hessian[8] * tmp1;
    a[5] = (hessian[5] + r * (dh[2] * dh[2] + dh[3] * dh[3]) + delta) - a[4] * tmp1;
    b[1] = (2*DOT(SUBXY,CA) + r * (dh[2] * mx[1] + dh[3] * mx[2])) - b[0] * tmp1;
    tmp2 = hessian[2]/a[0];
    tmp3 = hessian[3]/a[0];
    tmp4 = ((hessian[6]) - a[4] * tmp2)/a[5];
    a[14] = ((hessian[14] + r * (dh[7] * dh[5])) - hessian[12] * tmp2) - a[13] * tmp4;
    a[10] = ((hessian[10] + r * (dh[4] * dh[4] + dh[5] * dh[5]) + delta) - hessian[8] * tmp2) - a[9] * tmp4;
    b[2] = ((-2*DOT(SUBXY,ED) + r * (dh[4] * mx[3] + dh[5] * mx[5])) - b[0] * tmp2) - b[1] * tmp4;
    tmp5 = (hessian[7] - a[4] * tmp3)/a[5];
    tmp6 = (((hessian[11] + r * (dh[5] * dh[7])) - hessian[8] * tmp3) - a[9] * tmp5)/a[10];

    dx[3] = ((((-2*DOT(SUBXY,FD) + r * (dh[6] * mx[4] + dh[7] * mx[5])) - b[2] * tmp6) - b[0] * tmp3) - b[1] * tmp5) / ((((hessian[15] + r * (dh[6] * dh[6] + dh[7] * dh[7]) + delta) - hessian[12] * tmp3) - a[13] * tmp5) - a[14] * tmp6);
    dx[2] = (b[2] - (a[14] * dx[3])) / a[10];
    dx[1] = (b[1] - (a[9] * dx[2] + a[13] * dx[3])) / a[5];
    dx[0] = (b[0] - (a[4] * dx[1] + hessian[8] * dx[2] + hessian[12] * dx[3])) / a[0];

    delta = 1E4*delta;

    failed = (DOT4(dx,dx)/DOT4(x,x)>MaxErrorOfPenaltyMethod*MaxErrorOfPenaltyMethod) ? 1 : 0;

    x[0] -= dx[0];
    x[1] -= dx[1];
    x[2] -= dx[2];
    x[3] -= dx[3];
  }

  xPA = xCoordinatesOfTriangleA[0]+(BA[0] * x[0])+(CA[0] * x[1]);
  yPA = yCoordinatesOfTriangleA[0]+(BA[1] * x[0])+(CA[1] * x[1]);
  zPA = zCoordinatesOfTriangleA[0]+(BA[2] * x[0])+(CA[2] * x[1]);

  xPB = xCoordinatesOfTriangleB[0]+(ED[0] * x[2])+(FD[0] * x[3]);
  yPB = yCoordinatesOfTriangleB[0]+(ED[1] * x[2])+(FD[1] * x[3]);
  zPB = zCoordinatesOfTriangleB[0]+(ED[2] * x[2])+(FD[2] * x[3]);
}
 
//statistics
void delta::collision::penalty(
  const iREAL   xCoordinatesOfTriangleA[],
  const iREAL   yCoordinatesOfTriangleA[],
  const iREAL   zCoordinatesOfTriangleA[],
  const iREAL   xCoordinatesOfTriangleB[],
  const iREAL   yCoordinatesOfTriangleB[],
  const iREAL   zCoordinatesOfTriangleB[],
    iREAL&  xPA,
    iREAL&  yPA,
    iREAL&  zPA,
    iREAL&  xPB,
    iREAL&  yPB,
    iREAL&  zPB,
    iREAL   maxError,
    int&     numberOfNewtonIterationsRequired)
 {
  __attribute__ ((aligned(byteAlignment))) iREAL BA[3];
  __attribute__ ((aligned(byteAlignment))) iREAL CA[3];
  __attribute__ ((aligned(byteAlignment))) iREAL ED[3];
  __attribute__ ((aligned(byteAlignment))) iREAL FD[3];
  __attribute__ ((aligned(byteAlignment))) iREAL hessian[16];
  __attribute__ ((aligned(byteAlignment))) iREAL x[4];

  BA[0] = xCoordinatesOfTriangleA[1] - xCoordinatesOfTriangleA[0];
  BA[1] = yCoordinatesOfTriangleA[1] - yCoordinatesOfTriangleA[0];
  BA[2] = zCoordinatesOfTriangleA[1] - zCoordinatesOfTriangleA[0];

  CA[0] = xCoordinatesOfTriangleA[2] - xCoordinatesOfTriangleA[0];
  CA[1] = yCoordinatesOfTriangleA[2] - yCoordinatesOfTriangleA[0];
  CA[2] = zCoordinatesOfTriangleA[2] - zCoordinatesOfTriangleA[0];

  ED[0] = xCoordinatesOfTriangleB[1] - xCoordinatesOfTriangleB[0];
  ED[1] = yCoordinatesOfTriangleB[1] - yCoordinatesOfTriangleB[0];
  ED[2] = zCoordinatesOfTriangleB[1] - zCoordinatesOfTriangleB[0];

  FD[0] = xCoordinatesOfTriangleB[2] - xCoordinatesOfTriangleB[0];
  FD[1] = yCoordinatesOfTriangleB[2] - yCoordinatesOfTriangleB[0];
  FD[2] = zCoordinatesOfTriangleB[2] - zCoordinatesOfTriangleB[0];

  hessian[0] = 2.*DOT(BA,BA);
  hessian[1] = 2.*DOT(CA,BA);
  hessian[2] = -2.*DOT(ED,BA);
  hessian[3] = -2.*DOT(FD,BA);

  hessian[4] = hessian[1]; //use symmetry
  hessian[5] = 2.*DOT(CA,CA);
  hessian[6] = -2.*DOT(ED,CA);
  hessian[7] = -2.*DOT(FD,CA);

  hessian[8] = hessian[2];
  hessian[9] = hessian[6];
  hessian[10] = 2.*DOT(ED,ED);
  hessian[11] = 2.*DOT(FD,ED);

  hessian[12] = hessian[3];
  hessian[13] = hessian[7];
  hessian[14] = hessian[11];
  hessian[15] = 2.*DOT(FD,FD);

  __attribute__ ((aligned(byteAlignment))) iREAL eps = 1E-2;
  __attribute__ ((aligned(byteAlignment))) iREAL delta = (hessian[0]+hessian[5]+hessian[10]+hessian[15]) * eps;
  __attribute__ ((aligned(byteAlignment))) iREAL lambda = sqrt(0.0125*(hessian[0]+hessian[5]+hessian[10]+hessian[15]));
  __attribute__ ((aligned(byteAlignment))) iREAL r = lambda*1E5;

  //initial guess
  x[0] = 0.33;
  x[1] = 0.33;
  x[2] = 0.33;
  x[3] = 0.33;

   //Newton loop
  for(int i=0;i<MaxNumberOfNewtonIterations;i++)
  {
    //Declare loop variables;
    __attribute__ ((aligned(byteAlignment))) iREAL dx[4];
    __attribute__ ((aligned(byteAlignment))) iREAL a[16] ;
    __attribute__ ((aligned(byteAlignment))) iREAL SUBXY[3] ;
    __attribute__ ((aligned(byteAlignment))) iREAL b[4];
    __attribute__ ((aligned(byteAlignment))) iREAL dh[8];
    iREAL tmp1, tmp2,tmp3, tmp4, tmp5, tmp6, mx[6];

    dh[0] = (-x[0] <= 0) ? 0.0 : -1;
    mx[0] = (-x[0] <= 0) ? 0.0 : -x[0];

    dh[2] = (-x[1] <= 0) ? 0.0 : -1;
    mx[1] = (-x[1] <= 0) ? 0.0 : -x[1];

    dh[1] = dh[3] = (x[0]+x[1]-1 <= 0) ? 0.0 : 1;
    mx[2] = (x[0]+x[1]-1 <= 0) ? 0.0 : x[0]+x[1]-1;

    dh[4] = (-x[2] <= 0) ? 0.0 : -1;
    mx[3] = (-x[2] <= 0) ? 0.0 : -x[2];

    dh[6] = (-x[3] <= 0) ? 0.0 : -1;
    mx[4] = (-x[3] <= 0) ? 0.0 : -x[3];

    dh[5] = dh[7] = (x[2]+x[3]-1 <= 0) ? 0.0 : 1;
    mx[5] = (x[2]+x[3]-1 <=0) ? 0.0 : x[2]+x[3]-1;

    delta = i < 3 ? delta : 1E5*delta;

    SUBXY[0] = (xCoordinatesOfTriangleA[0]+(BA[0] * x[0])+(CA[0] * x[1])) - (xCoordinatesOfTriangleB[0]+(ED[0] * x[2])+(FD[0] * x[3]));
    SUBXY[1] = (yCoordinatesOfTriangleA[0]+(BA[1] * x[0])+(CA[1] * x[1])) - (yCoordinatesOfTriangleB[0]+(ED[1] * x[2])+(FD[1] * x[3]));
    SUBXY[2] = (zCoordinatesOfTriangleA[0]+(BA[2] * x[0])+(CA[2] * x[1])) - (zCoordinatesOfTriangleB[0]+(ED[2] * x[2])+(FD[2] * x[3]));

    b[0] = 2*DOT(SUBXY,BA) + r * (dh[0] * mx[0] + dh[1] * mx[2]);
    a[0] = hessian[0] + r * (dh[0] * dh[0] + dh[1] * dh[1]) + delta;
    a[4] = hessian[4] + r * (dh[3] * dh[1]);
    tmp1 = (hessian[1] + r * (dh[1] * dh[3]))/a[0];
    a[13] = hessian[13] - hessian[12] * tmp1;
    a[9] = hessian[9] - hessian[8] * tmp1;
    a[5] = (hessian[5] + r * (dh[2] * dh[2] + dh[3] * dh[3]) + delta) - a[4] * tmp1;
    b[1] = (2*DOT(SUBXY,CA) + r * (dh[2] * mx[1] + dh[3] * mx[2])) - b[0] * tmp1;
    tmp2 = hessian[2]/a[0];
    tmp3 = hessian[3]/a[0];
    tmp4 = ((hessian[6]) - a[4] * tmp2)/a[5];
    a[14] = ((hessian[14] + r * (dh[7] * dh[5])) - hessian[12] * tmp2) - a[13] * tmp4;
    a[10] = ((hessian[10] + r * (dh[4] * dh[4] + dh[5] * dh[5]) + delta) - hessian[8] * tmp2) - a[9] * tmp4;
    b[2] = ((-2*DOT(SUBXY,ED) + r * (dh[4] * mx[3] + dh[5] * mx[5])) - b[0] * tmp2) - b[1] * tmp4;
    tmp5 = (hessian[7] - a[4] * tmp3)/a[5];
    tmp6 = (((hessian[11] + r * (dh[5] * dh[7])) - hessian[8] * tmp3) - a[9] * tmp5)/a[10];

    dx[3] = ((((-2*DOT(SUBXY,FD) + r * (dh[6] * mx[4] + dh[7] * mx[5])) - b[2] * tmp6) - b[0] * tmp3) - b[1] * tmp5) / ((((hessian[15] + r * (dh[6] * dh[6] + dh[7] * dh[7]) + delta) - hessian[12] * tmp3) - a[13] * tmp5) - a[14] * tmp6);
    dx[2] = (b[2] - (a[14] * dx[3])) / a[10];
    dx[1] = (b[1] - (a[9] * dx[2] + a[13] * dx[3])) / a[5];
    dx[0] = (b[0] - (a[4] * dx[1] + hessian[8] * dx[2] + hessian[12] * dx[3])) / a[0];

    iREAL error = DOT4(dx,dx)/DOT4(x,x);

    if (error < maxError*maxError) {
      numberOfNewtonIterationsRequired = i;
      break;
    }

    x[0] = x[0] - dx[0];
    x[1] = x[1] - dx[1];
    x[2] = x[2] - dx[2];
    x[3] = x[3] - dx[3];
  }

  xPA = xCoordinatesOfTriangleA[0]+(BA[0] * x[0])+(CA[0] * x[1]);
  yPA = yCoordinatesOfTriangleA[0]+(BA[1] * x[0])+(CA[1] * x[1]);
  zPA = zCoordinatesOfTriangleA[0]+(BA[2] * x[0])+(CA[2] * x[1]);

  xPB = xCoordinatesOfTriangleB[0]+(ED[0] * x[2])+(FD[0] * x[3]);
  yPB = yCoordinatesOfTriangleB[0]+(ED[1] * x[2])+(FD[1] * x[3]);
  zPB = zCoordinatesOfTriangleB[0]+(ED[2] * x[2])+(FD[2] * x[3]);
}

