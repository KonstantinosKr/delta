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

#include "hybrid.h"
#include "penalty.h"
#include "bf.h"

std::vector<delta::collision::contactpoint> delta::collision::hybridWithPerTriangleFallBack(
  int       numberOfTrianglesOfGeometryA,
  double*   xCoordinatesOfPointsOfGeometryA,
  double*   yCoordinatesOfPointsOfGeometryA,
  double*   zCoordinatesOfPointsOfGeometryA,
  double    epsilonA,
  bool      frictionA,
  int 		  particleA,

  int       numberOfTrianglesOfGeometryB,
  double*   xCoordinatesOfPointsOfGeometryB,
  double*   yCoordinatesOfPointsOfGeometryB,
  double*   zCoordinatesOfPointsOfGeometryB,
  double    epsilonB,
  bool      frictionB,
  int	      particleB)
{
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
    #pragma omp parallel for schedule(static)
  #endif
  for (int iA=0; iA<numberOfTrianglesOfGeometryA*3; iA+=3)
  {
    __attribute__ ((aligned(byteAlignment))) double shortestDistance = (epsilonA+epsilonB);
    contactpoint *nearestContactPoint = nullptr;

    #pragma forceinline recursive
    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      __attribute__ ((aligned(byteAlignment))) double xPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double xPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPB;// __attribute__ ((aligned(byteAlignment)));
      const double MaxErrorOfPenaltyMethod __attribute__ ((aligned(byteAlignment))) = (epsilonA+epsilonB)/16;
      const int MaxNewtonIterations __attribute__ ((aligned(byteAlignment))) = 8;
      bool failed = 0;

      penalty(xCoordinatesOfPointsOfGeometryA+(iA),
              yCoordinatesOfPointsOfGeometryA+(iA),
              zCoordinatesOfPointsOfGeometryA+(iA),
              xCoordinatesOfPointsOfGeometryB+(iB),
              yCoordinatesOfPointsOfGeometryB+(iB),
              zCoordinatesOfPointsOfGeometryB+(iB),
              xPA, yPA, zPA, xPB, yPB, zPB, MaxErrorOfPenaltyMethod, failed);

      if (failed) 
      {
    	  bf(
          xCoordinatesOfPointsOfGeometryA+(iA),
          yCoordinatesOfPointsOfGeometryA+(iA),
          zCoordinatesOfPointsOfGeometryA+(iA),
          xCoordinatesOfPointsOfGeometryB+(iB),
          yCoordinatesOfPointsOfGeometryB+(iB),
          zCoordinatesOfPointsOfGeometryB+(iB),
          xPA, yPA, zPA, xPB, yPB, zPB
        );
      }

      double d = std::sqrt(((xPB-xPA)*(xPB-xPA))+((yPB-yPA)*(yPB-yPA))+((zPB-zPA)*(zPB-zPA)));
      if (d<=shortestDistance)
      {
        nearestContactPoint = new contactpoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA && frictionB);
        shortestDistance    = d;
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

std::vector<delta::collision::contactpoint> delta::collision::hybridWithPerTriangleFallBackStats(
  int       numberOfTrianglesOfGeometryA,
  double*   xCoordinatesOfPointsOfGeometryA,
  double*   yCoordinatesOfPointsOfGeometryA,
  double*   zCoordinatesOfPointsOfGeometryA,
  double    epsilonA,
  bool      frictionA,
  int 		  particleA,

  int       numberOfTrianglesOfGeometryB,
  double*   xCoordinatesOfPointsOfGeometryB,
  double*   yCoordinatesOfPointsOfGeometryB,
  double*   zCoordinatesOfPointsOfGeometryB,
  double    epsilonB,
  bool      frictionB,
  int 		  particleB)
{
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
    #pragma omp parallel for schedule(static)
  #endif
  for (int iA=0; iA<numberOfTrianglesOfGeometryA; iA+=3)
  {
    __attribute__ ((aligned(byteAlignment))) double shortestDistance = (epsilonA+epsilonB);
    contactpoint *nearestContactPoint = nullptr;

    #pragma forceinline recursive
    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      __attribute__ ((aligned(byteAlignment))) double xPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double xPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPB;// __attribute__ ((aligned(byteAlignment)));
      const double MaxErrorOfPenaltyMethod __attribute__ ((aligned(byteAlignment))) = (epsilonA+epsilonB)/16;
      const int MaxNewtonIterations __attribute__ ((aligned(byteAlignment))) = 8;
      bool failed = 0;

      penalty(xCoordinatesOfPointsOfGeometryA+(iA),
              yCoordinatesOfPointsOfGeometryA+(iA),
              zCoordinatesOfPointsOfGeometryA+(iA),
              xCoordinatesOfPointsOfGeometryB+(iB),
              yCoordinatesOfPointsOfGeometryB+(iB),
              zCoordinatesOfPointsOfGeometryB+(iB),
              xPA, yPA, zPA, xPB, yPB, zPB, MaxErrorOfPenaltyMethod, failed);

      if (failed)
      {
    	  bf(
          xCoordinatesOfPointsOfGeometryA+(iA),
          yCoordinatesOfPointsOfGeometryA+(iA),
          zCoordinatesOfPointsOfGeometryA+(iA),
          xCoordinatesOfPointsOfGeometryB+(iB),
          yCoordinatesOfPointsOfGeometryB+(iB),
          zCoordinatesOfPointsOfGeometryB+(iB),
          xPA, yPA, zPA, xPB, yPB, zPB
        );
      }

      double d = std::sqrt(((xPB-xPA)*(xPB-xPA))+((yPB-yPA)*(yPB-yPA))+((zPB-zPA)*(zPB-zPA)));
      if (d<=shortestDistance)
      {
        nearestContactPoint = new contactpoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA && frictionB);
        shortestDistance    = d;
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

std::vector<delta::collision::contactpoint> delta::collision::hybridWithPerBatchFallBack(
  int       numberOfTrianglesOfGeometryA,
  double*   xCoordinatesOfPointsOfGeometryA,
  double*   yCoordinatesOfPointsOfGeometryA,
  double*   zCoordinatesOfPointsOfGeometryA,
  double    epsilonA,
  bool      frictionA,
  int 		  particleA,

  int       numberOfTrianglesOfGeometryB,
  double*   xCoordinatesOfPointsOfGeometryB,
  double*   yCoordinatesOfPointsOfGeometryB,
  double*   zCoordinatesOfPointsOfGeometryB,
  double    epsilonB,
  bool      frictionB,
  int	      particleB)
{
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
    #pragma omp parallel for schedule(static)
  #endif
  for (int iA=0; iA<numberOfTrianglesOfGeometryA*3; iA+=3)
  {
    __attribute__ ((aligned(byteAlignment))) double shortestDistance = (epsilonA+epsilonB);
    contactpoint *nearestContactPoint = nullptr;

    bool failed = 0;
    
    #pragma forceinline recursive
    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      __attribute__ ((aligned(byteAlignment))) double xPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double xPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPB;// __attribute__ ((aligned(byteAlignment)));

      const int MaxNewtonIterations __attribute__ ((aligned(byteAlignment))) = 4;
      const double MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;
        
      penalty(xCoordinatesOfPointsOfGeometryA+(iA),
              yCoordinatesOfPointsOfGeometryA+(iA),
              zCoordinatesOfPointsOfGeometryA+(iA),
              xCoordinatesOfPointsOfGeometryB+(iB),
              yCoordinatesOfPointsOfGeometryB+(iB),
              zCoordinatesOfPointsOfGeometryB+(iB),
              xPA, yPA, zPA, xPB, yPB, zPB,
              MaxErrorOfPenaltyMethod, failed);

      double d = std::sqrt(((xPB-xPA)*(xPB-xPA))+((yPB-yPA)*(yPB-yPA))+((zPB-zPA)*(zPB-zPA)));
      if (d<=shortestDistance)
      {
        nearestContactPoint = new contactpoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA && frictionB);
        shortestDistance    = d;
      }
    }

    // If the maximum error in the whole computation exceeds our limit, we fall
    // back to brute force. For this, we reset nearestContactPoint and
    // minDistance and then run the interior loop over iB again with bf().
    if (failed) 
    {
      nearestContactPoint = nullptr;
      shortestDistance    = (epsilonA+epsilonB);

      #pragma forceinline recursive
      #pragma simd
      for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
      {
        __attribute__ ((aligned(byteAlignment))) double xPA;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double yPA;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double zPA;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double xPB;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double yPB;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double zPB;// __attribute__ ((aligned(byteAlignment)));
        
        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA, yPA, zPA, xPB, yPB, zPB);

        double d = std::sqrt(((xPB-xPA)*(xPB-xPA))+((yPB-yPA)*(yPB-yPA))+((zPB-zPA)*(zPB-zPA)));
        if (d<=shortestDistance)
        {
          nearestContactPoint = new contactpoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA && frictionB);
          shortestDistance    = d;
        }
      }
    }
    if(nearestContactPoint != nullptr)
    {
      #ifdef ompTriangle
	      #pragma omp critical
      #endif
      result.push_back(*nearestContactPoint);
    }
  }         

  return result;
}

std::vector<delta::collision::contactpoint> delta::collision::hybridWithPerBatchFallBackStats(
  int       numberOfTrianglesOfGeometryA,
  double*   xCoordinatesOfPointsOfGeometryA,
  double*   yCoordinatesOfPointsOfGeometryA,
  double*   zCoordinatesOfPointsOfGeometryA,
  double    epsilonA,
  bool      frictionA,
  int	      particleA,

  int       numberOfTrianglesOfGeometryB,
  double*   xCoordinatesOfPointsOfGeometryB,
  double*   yCoordinatesOfPointsOfGeometryB,
  double*   zCoordinatesOfPointsOfGeometryB,
  double    epsilonB,
  bool      frictionB,
  int 	  	particleB)
{
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
    #pragma omp parallel for schedule(static)
  #endif
  for (int iA=0; iA<numberOfTrianglesOfGeometryA*3; iA++)
  {
    __attribute__ ((aligned(byteAlignment))) double shortestDistance = (epsilonA+epsilonB);

    contactpoint *nearestContactPoint = nullptr;

    bool failed = 0;

    #pragma forceinline recursive
    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      __attribute__ ((aligned(byteAlignment))) double xPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPA;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double xPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double yPB;// __attribute__ ((aligned(byteAlignment)));
      __attribute__ ((aligned(byteAlignment))) double zPB;// __attribute__ ((aligned(byteAlignment)));

    	const int MaxNewtonIterations __attribute__ ((aligned(byteAlignment))) = 4;
    	const double MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;

      penalty(xCoordinatesOfPointsOfGeometryA+(iA),
              yCoordinatesOfPointsOfGeometryA+(iA),
              zCoordinatesOfPointsOfGeometryA+(iA),
              xCoordinatesOfPointsOfGeometryB+(iB),
              yCoordinatesOfPointsOfGeometryB+(iB),
              zCoordinatesOfPointsOfGeometryB+(iB),
              xPA, yPA, zPA, xPB, yPB, zPB,
              MaxErrorOfPenaltyMethod, failed);

      double d = std::sqrt(((xPB-xPA)*(xPB-xPA))+((yPB-yPA)*(yPB-yPA))+((zPB-zPA)*(zPB-zPA)));
      if (d<=shortestDistance)
      {
        nearestContactPoint = new contactpoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA && frictionB);
        shortestDistance    = d;
      }
    }

    if (failed)
    {
      nearestContactPoint = nullptr;
      shortestDistance    = (epsilonA+epsilonB);

      #pragma forceinline recursive
      #pragma simd
      for (int iB=0; iB<numberOfTrianglesOfGeometryB; iB+=3)
      {
        __attribute__ ((aligned(byteAlignment))) double xPA;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double yPA;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double zPA;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double xPB;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double yPB;// __attribute__ ((aligned(byteAlignment)));
        __attribute__ ((aligned(byteAlignment))) double zPB;// __attribute__ ((aligned(byteAlignment)));

        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA, yPA, zPA, xPB, yPB, zPB);

        double d = std::sqrt(((xPB-xPA)*(xPB-xPA))+((yPB-yPA)*(yPB-yPA))+((zPB-zPA)*(zPB-zPA)));
        if (d<=shortestDistance)
        {
          nearestContactPoint = new contactpoint(xPA, yPA, zPA, epsilonA, particleA, xPB, yPB, zPB, epsilonB, particleB, frictionA && frictionB);;
          shortestDistance    = d;

        }
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
