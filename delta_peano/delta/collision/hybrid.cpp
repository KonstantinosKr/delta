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

void delta::collision::cleanHybridStatistics() {
  numberOfPenaltyFails = 0;
  numberOfBatchFails = 0;
  int batchSize = 0;
  double batchError = 0;
}

int delta::collision::getPenaltyFails() {
  return numberOfPenaltyFails;
}

int delta::collision::getBatchFails() {
  return numberOfBatchFails;
}

int delta::collision::getBatchSize() {
  return batchSize;
}

double delta::collision::getBatchError() {
  return batchError;
}

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
  __attribute__ ((aligned(byteAlignment))) double MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;
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
    __attribute__ ((aligned(byteAlignment))) double xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], dd[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000];

    #pragma forceinline recursive
    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      failed[iB] = 0;
      penalty(xCoordinatesOfPointsOfGeometryA+(iA),
              yCoordinatesOfPointsOfGeometryA+(iA),
              zCoordinatesOfPointsOfGeometryA+(iA),
              xCoordinatesOfPointsOfGeometryB+(iB),
              yCoordinatesOfPointsOfGeometryB+(iB),
              zCoordinatesOfPointsOfGeometryB+(iB),
              xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB], MaxErrorOfPenaltyMethod, failed[iB]);

      dd[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    __attribute__ ((aligned(byteAlignment))) double shortestDistance = (epsilonA+epsilonB);
    contactpoint *nearestContactPoint = nullptr;

    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if(failed[iB])
      {
        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB]);
      }
      if (dd[iB]<=shortestDistance)
      {
        nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        shortestDistance    = dd[iB];
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
  int       particleA,

  int       numberOfTrianglesOfGeometryB,
  double*   xCoordinatesOfPointsOfGeometryB,
  double*   yCoordinatesOfPointsOfGeometryB,
  double*   zCoordinatesOfPointsOfGeometryB,
  double    epsilonB,
  bool      frictionB,
  int       particleB)
{
  __attribute__ ((aligned(byteAlignment))) std::vector<contactpoint> result;

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
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) const double MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;

    __attribute__ ((aligned(byteAlignment))) double xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], dd[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000] = {0};

    #pragma forceinline recursive
    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
        penalty(xCoordinatesOfPointsOfGeometryA+(iA),
                yCoordinatesOfPointsOfGeometryA+(iA),
                zCoordinatesOfPointsOfGeometryA+(iA),
                xCoordinatesOfPointsOfGeometryB+(iB),
                yCoordinatesOfPointsOfGeometryB+(iB),
                zCoordinatesOfPointsOfGeometryB+(iB),
                xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB], MaxErrorOfPenaltyMethod, failed[iB]);

        dd[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    bool fail = false; int counter=0;
    #pragma simd reduction(+:counter)
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if(failed[iB])
      {
        counter++;
      }
    }

    double localbatchError = counter * MaxErrorOfPenaltyMethod;

    if(localbatchError > ((double)MaxErrorOfPenaltyMethod*(double)numberOfTrianglesOfGeometryB)/32)
    {
      fail = true;
    }

    // If the maximum error in the whole computation exceeds our limit, we fall
    // back to brute force. For this, we reset nearestContactPoint and
    // minDistance and then run the interior loop over iB again with bf().
    if (fail)
    {
      #pragma forceinline recursive
      #pragma simd
      for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
      {
        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB]);

        dd[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
      }
    }

    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if (dd[iB]<=shortestDistance)
      {
        nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        shortestDistance    = dd[iB];
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


std::vector<delta::collision::contactpoint> delta::collision::hybridStat(
  int       numberOfTrianglesOfGeometryA,
  double*   xCoordinatesOfPointsOfGeometryA,
  double*   yCoordinatesOfPointsOfGeometryA,
  double*   zCoordinatesOfPointsOfGeometryA,
  double    epsilonA,
  bool      frictionA,
  int       particleA,

  int       numberOfTrianglesOfGeometryB,
  double*   xCoordinatesOfPointsOfGeometryB,
  double*   yCoordinatesOfPointsOfGeometryB,
  double*   zCoordinatesOfPointsOfGeometryB,
  double    epsilonB,
  bool      frictionB,
  int       particleB)
{
  __attribute__ ((aligned(byteAlignment))) std::vector<contactpoint> result;

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
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) const double MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;

    __attribute__ ((aligned(byteAlignment))) double xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], dd[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000];

    #ifdef ompTriangle
      #pragma omp critical
    #endif
    {
      batchSize += numberOfTrianglesOfGeometryB;
      batchError += MaxErrorOfPenaltyMethod;
    }

    #pragma forceinline recursive
    #pragma simd
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
        failed[iB] = 0;
        penalty(xCoordinatesOfPointsOfGeometryA+(iA),
                yCoordinatesOfPointsOfGeometryA+(iA),
                zCoordinatesOfPointsOfGeometryA+(iA),
                xCoordinatesOfPointsOfGeometryB+(iB),
                yCoordinatesOfPointsOfGeometryB+(iB),
                zCoordinatesOfPointsOfGeometryB+(iB),
                xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB], MaxErrorOfPenaltyMethod, failed[iB]);

        dd[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    bool fail = false; int counter=0;
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if(failed[iB])
      {
        counter++;
      }
    }

    double localbatchError = counter * MaxErrorOfPenaltyMethod;

    #ifdef ompTriangle
      #pragma omp critical
    #endif
    {
    numberOfPenaltyFails += counter;
    }
    if(localbatchError > ((double)MaxErrorOfPenaltyMethod*(double)numberOfTrianglesOfGeometryB)/32)
    {
      fail = true;
      #ifdef ompTriangle
        #pragma omp critical
      #endif
      numberOfBatchFails += 1;
    }

    // If the maximum error in the whole computation exceeds our limit, we fall
    // back to brute force. For this, we reset nearestContactPoint and
    // minDistance and then run the interior loop over iB again with bf().
    if(fail)
    {
      #pragma forceinline recursive
      #pragma simd
      for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
      {
        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB]);

        dd[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
      }
    }

    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if (dd[iB]<=shortestDistance)
      {
        nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        shortestDistance    = dd[iB];
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
