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
  const int MaxNumberOfNewtonIterations = 32;

  std::vector<int>  numberOfNewtonIterations(MaxNumberOfNewtonIterations);
}



void delta::collision::cleanPenaltyStatistics() {
  for (int i=0; i<MaxNumberOfNewtonIterations; i++) {
    numberOfNewtonIterations[i] = 0;
  }
}


std::vector<int> delta::collision::getPenaltyStatistics() {
  return numberOfNewtonIterations;
}


std::vector<delta::collision::contactpoint> delta::collision::penalty(
  int       numberOfTrianglesOfGeometryA,
  double*   xCoordinatesOfPointsOfGeometryA,
  double*   yCoordinatesOfPointsOfGeometryA,
  double*   zCoordinatesOfPointsOfGeometryA,
  int       numberOfTrianglesOfGeometryB,
  double*   xCoordinatesOfPointsOfGeometryB,
  double*   yCoordinatesOfPointsOfGeometryB,
  double*   zCoordinatesOfPointsOfGeometryB,
  double    epsilon
) {
  const double MaxError = epsilon / 16.0;

  std::vector<contactpoint> result;

  #pragma vector aligned
  for (int iA=0; iA<numberOfTrianglesOfGeometryA; iA++) {
    double       minDistance = std::numeric_limits<double>::max();
    contactpoint nearestContactPoint;

    for (int iB=0; iB<numberOfTrianglesOfGeometryB; iB++) {
      double xPA;
      double yPA;
      double zPA;
      double xPB;
      double yPB;
      double zPB;

      int     numberOfNewtonIterationsRequired;

      penalty(
        xCoordinatesOfPointsOfGeometryA+(iA*3),
        yCoordinatesOfPointsOfGeometryA+(iA*3),
        zCoordinatesOfPointsOfGeometryA+(iA*3),
        xCoordinatesOfPointsOfGeometryB+(iB*3),
        yCoordinatesOfPointsOfGeometryB+(iB*3),
        zCoordinatesOfPointsOfGeometryB+(iB*3),
        xPA, yPA, zPA, xPB, yPB, zPB,
        MaxNumberOfNewtonIterations,
        MaxError,
        numberOfNewtonIterationsRequired
      );

      numberOfNewtonIterations[numberOfNewtonIterationsRequired]++;

      contactpoint newContactPoint(xPA, yPA, zPA, xPB, yPB, zPB);
      if (newContactPoint.getDistance()<minDistance) {
        nearestContactPoint = newContactPoint;
        minDistance         = newContactPoint.getDistance();
      }
    }
    if (nearestContactPoint.getDistance()<=epsilon) {
      result.push_back( nearestContactPoint );
    }
  }

  return result;
}

void delta::collision::penalty(
   double   xCoordinatesOfTriangleA[3],
   double   yCoordinatesOfTriangleA[3],
   double   zCoordinatesOfTriangleA[3],
   double   xCoordinatesOfTriangleB[3],
   double   yCoordinatesOfTriangleB[3],
   double   zCoordinatesOfTriangleB[3],
   double&  xPA,
   double&  yPA,
   double&  zPA,
   double&  xPB,
   double&  yPB,
   double&  zPB,
   int numberOfNewtonIterations,
   double& error)
 {
   iREAL BA[3], CA[3], ED[3], FD[3], hessian[16], x[4];
   
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
   
   iREAL eps = 1E-2;
   iREAL delta = (hessian[0]+hessian[5]+hessian[10]+hessian[15]) * eps;
   iREAL lambda = sqrt(0.0125*(hessian[0]+hessian[5]+hessian[10]+hessian[15]));
   iREAL r = lambda*pow(10,log10(lambda)+8);
   
   // #if iREAL==double
   //iREAL tol = DBL_EPSILON;
   //#else
   //iREAL tol = FLT_EPSILON;
   //#endif

   //initial guess
   x[0] = 0.33;
   x[1] = 0.33;
   x[2] = 0.33;
   x[3] = 0.33;
   
   //Declare loop variables;
   iREAL dx[4], a[16], SUBXY[3], b[4], mx[6], dh[8], tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;
   
   //Newton loop
   for(int i=0;i<numberOfNewtonIterations;i++)
   {
     dh[0] = dh[2] = dh[4] = dh[6] = -1;
     dh[1] = dh[3] = dh[5] = dh[7] = 1;
     
     if(-x[0] <= 0)
     {
     dh[0] = mx[0] = 0;
     }else
     {
     mx[0] = -x[0];
     }
     if(-x[1] <= 0)
     {
     dh[2] = mx[1] = 0;
     }else
     {
     mx[1] = -x[1];
     }
     if(x[0]+x[1]-1 <= 0)
     {
     dh[1] =  dh[3] = mx[2] = 0;
     }else
     {
     mx[2] = x[0]+x[1]-1;
     }
     if(-x[2] <= 0)
     {
     dh[4] = mx[3] = 0;
     }else
     {
     mx[3] = -x[2];
     }
     if(-x[3] <= 0)
     {
     dh[6] = mx[4] = 0;
     }else
     {
     mx[4] = -x[3];
     }
     if(x[2]+x[3]-1 <= 0)
     {
     dh[5] = dh[7] = 0;
     
     mx[5] = 0;
     }else
     {
     mx[5] = x[2]+x[3]-1;
     }
     
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
     
     error = sqrt(DOT4(dx,dx)/DOT4(x,x));
     
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

void delta::collision::penalty(
	  double   xCoordinatesOfTriangleA[3],
	  double   yCoordinatesOfTriangleA[3],
	  double   zCoordinatesOfTriangleA[3],
	  double   xCoordinatesOfTriangleB[3],
	  double   yCoordinatesOfTriangleB[3],
	  double   zCoordinatesOfTriangleB[3],
    double&  xPA,
    double&  yPA,
    double&  zPA,
    double&  xPB,
    double&  yPB,
    double&  zPB,
    int      maxNumberOfNewtonIterations,
    double   maxError,
    int&     numberOfNewtonIterationsRequired
    )
 {
   iREAL BA[3], CA[3], ED[3], FD[3], hessian[16], x[4];

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

   iREAL eps = 1E-2;
   iREAL delta = (hessian[0]+hessian[5]+hessian[10]+hessian[15]) * eps;
   iREAL lambda = sqrt(0.0125*(hessian[0]+hessian[5]+hessian[10]+hessian[15]));
   iREAL r = lambda*pow(10,log10(lambda)+8);

   //#if iREAL==double
   //iREAL tol = DBL_EPSILON;
   //#else
   //iREAL tol = FLT_EPSILON;
   //#endif

   //initial guess
   x[0] = 0.33;
   x[1] = 0.33;
   x[2] = 0.33;
   x[3] = 0.33;

   //Declare loop variables;
   iREAL dx[4], a[16], SUBXY[3], b[4], mx[6], dh[8], tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

   //Newton loop
   for(int i=0;i<maxNumberOfNewtonIterations;i++)
   {
     dh[0] = dh[2] = dh[4] = dh[6] = -1;
     dh[1] = dh[3] = dh[5] = dh[7] = 1;

     if(-x[0] <= 0)
     {
     dh[0] = mx[0] = 0;
     }else
     {
     mx[0] = -x[0];
     }
     if(-x[1] <= 0)
     {
     dh[2] = mx[1] = 0;
     }else
     {
     mx[1] = -x[1];
     }
     if(x[0]+x[1]-1 <= 0)
     {
     dh[1] =  dh[3] = mx[2] = 0;
     }else
     {
     mx[2] = x[0]+x[1]-1;
     }
     if(-x[2] <= 0)
     {
     dh[4] = mx[3] = 0;
     }else
     {
     mx[3] = -x[2];
     }
     if(-x[3] <= 0)
     {
     dh[6] = mx[4] = 0;
     }else
     {
     mx[4] = -x[3];
     }
     if(x[2]+x[3]-1 <= 0)
     {
     dh[5] = dh[7] = 0;

     mx[5] = 0;
     }else
     {
     mx[5] = x[2]+x[3]-1;
     }

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

     iREAL error = sqrt(DOT4(dx,dx)/DOT4(x,x));
     if(error < maxError)
     {
    	 numberOfNewtonIterationsRequired = i+1;
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
