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

void delta::collision::cleanHybridStatistics() {
  numberOfPenaltyFails = 0;
  numberOfBatchFails = 0;
  //int batchSize = 0;
  //iREAL batchError = 0;
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

iREAL delta::collision::getBatchError() {
  return batchError;
}

std::vector<delta::collision::contactpoint> delta::collision::hybridWithPerTriangleFallBack(
  int      numberOfTrianglesOfGeometryA,
  iREAL*   xCoordinatesOfPointsOfGeometryA,
  iREAL*   yCoordinatesOfPointsOfGeometryA,
  iREAL*   zCoordinatesOfPointsOfGeometryA,
  iREAL    epsilonA,
  bool     frictionA,
  int 		 particleA,

  int      numberOfTrianglesOfGeometryB,
  iREAL*   xCoordinatesOfPointsOfGeometryB,
  iREAL*   yCoordinatesOfPointsOfGeometryB,
  iREAL*   zCoordinatesOfPointsOfGeometryB,
  iREAL    epsilonB,
  bool     frictionB,
  int	     particleB)
{
  __attribute__ ((aligned(byteAlignment))) iREAL MaxErrorOfPenalty = (epsilonA+epsilonB)/16;
  __attribute__ ((aligned(byteAlignment))) iREAL epsilonMargin = (epsilonA+epsilonB);
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
    __attribute__ ((aligned(byteAlignment))) iREAL xPA[100000], yPA[100000], zPA[100000], xPB[100000], yPB[100000], zPB[100000], d[100000];
    __attribute__ ((aligned(byteAlignment))) bool failed[100000] = {0};

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
      #pragma simd
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
              xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB], MaxErrorOfPenalty, failed[iB]);

      d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) iREAL dd = 1E99;

    #if defined(__INTEL_COMPILER)
      #pragma simd
    #else
      #pragma omp simd
    #endif
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
      if (d[iB] <= epsilonMargin && d[iB] <= dd)
      {
        nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        dd    = d[iB];
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
  int      numberOfTrianglesOfGeometryA,
  iREAL*   xCoordinatesOfPointsOfGeometryA,
  iREAL*   yCoordinatesOfPointsOfGeometryA,
  iREAL*   zCoordinatesOfPointsOfGeometryA,
  iREAL    epsilonA,
  bool     frictionA,
  int      particleA,

  int      numberOfTrianglesOfGeometryB,
  iREAL*   xCoordinatesOfPointsOfGeometryB,
  iREAL*   yCoordinatesOfPointsOfGeometryB,
  iREAL*   zCoordinatesOfPointsOfGeometryB,
  iREAL    epsilonB,
  bool     frictionB,
  int      particleB)
{
  __attribute__ ((aligned(byteAlignment))) std::vector<contactpoint> result;
  __attribute__ ((aligned(byteAlignment))) iREAL epsilonMargin = (epsilonA+epsilonB);
  __attribute__ ((aligned(byteAlignment))) const iREAL MaxErrorOfPenalty = (epsilonA+epsilonB)/16;

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
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) iREAL xPA[100000], yPA[100000], zPA[100000], xPB[100000], yPB[100000], zPB[100000], d[100000];
    __attribute__ ((aligned(byteAlignment))) bool failed[100000] = {0};
    __attribute__ ((aligned(byteAlignment))) iREAL dd = 1E99;

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
      #pragma simd
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
                xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB], MaxErrorOfPenalty, failed[iB]);

        d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    bool fail = false; int counter=0;
    #if defined(__INTEL_COMPILER)
      #pragma simd reduction(+:counter)
    #else 
      #pragma omp simd
    #endif
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if(failed[iB]) counter++;
    }

    iREAL localbatchError = counter * MaxErrorOfPenalty;
    if(localbatchError > ((iREAL)MaxErrorOfPenalty*(iREAL)numberOfTrianglesOfGeometryB)/32)
    {
      fail = true;
    }

    // If the maximum error in the whole computation exceeds our limit, we fall
    // back to brute force. For this, we reset nearestContactPoint and
    // minDistance and then run the interior loop over iB again with bf().
    if (fail)
    {
      #if defined(__INTEL_COMPILER)
        #pragma forceinline recursive
        #pragma simd
      #else
        #pragma omp simd
      #endif
      for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
      {
        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB]);

        d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
      }
    }

    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if (d[iB] <= epsilonMargin && d[iB] <= dd)
      {
        nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        dd    = d[iB];
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

std::vector<delta::collision::contactpoint> delta::collision::hybridBatchStat(
  int      numberOfTrianglesOfGeometryA,
  iREAL*   xCoordinatesOfPointsOfGeometryA,
  iREAL*   yCoordinatesOfPointsOfGeometryA,
  iREAL*   zCoordinatesOfPointsOfGeometryA,
  iREAL    epsilonA,
  bool     frictionA,
  int      particleA,

  int      numberOfTrianglesOfGeometryB,
  iREAL*   xCoordinatesOfPointsOfGeometryB,
  iREAL*   yCoordinatesOfPointsOfGeometryB,
  iREAL*   zCoordinatesOfPointsOfGeometryB,
  iREAL    epsilonB,
  bool     frictionB,
  int      particleB)
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
    __attribute__ ((aligned(byteAlignment))) iREAL epsilonMargin = (epsilonA+epsilonB);
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) const iREAL MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;
    __attribute__ ((aligned(byteAlignment))) iREAL dd = 1E99;

    __attribute__ ((aligned(byteAlignment))) iREAL xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], d[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000];

    #ifdef ompTriangle
      #pragma omp critical
    #endif
    {
      batchSize += numberOfTrianglesOfGeometryB;
      batchError += MaxErrorOfPenaltyMethod;
    }

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
      #pragma simd
    #else
      #pragma omp simd
    #endif
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

        d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    bool fail = false; int counter=0;
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if(failed[iB])
      {
        counter++;
      }
    }

    iREAL localbatchError = counter * MaxErrorOfPenaltyMethod;

    #ifdef ompTriangle
      #pragma omp critical
    #endif
    {
    numberOfPenaltyFails += counter;
    }
    if(localbatchError > ((iREAL)MaxErrorOfPenaltyMethod*(iREAL)numberOfTrianglesOfGeometryB)/32)
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
      #if defined(__INTEL_COMPILER)
        #pragma forceinline recursive
        #pragma simd
      #else
	#pragma omp simd
      #endif
      for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
      {
        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB]);

        d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
      }
    }

    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if (d[iB] <= epsilonMargin && d[iB] <= dd)
      {
        nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        dd    = d[iB];
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

std::vector<delta::collision::contactpoint> delta::collision::hybridTriangleStat(
  int      numberOfTrianglesOfGeometryA,
  iREAL*   xCoordinatesOfPointsOfGeometryA,
  iREAL*   yCoordinatesOfPointsOfGeometryA,
  iREAL*   zCoordinatesOfPointsOfGeometryA,
  iREAL    epsilonA,
  bool     frictionA,
  int      particleA,

  int      numberOfTrianglesOfGeometryB,
  iREAL*   xCoordinatesOfPointsOfGeometryB,
  iREAL*   yCoordinatesOfPointsOfGeometryB,
  iREAL*   zCoordinatesOfPointsOfGeometryB,
  iREAL    epsilonB,
  bool     frictionB,
  int      particleB)
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
    __attribute__ ((aligned(byteAlignment))) iREAL epsilonMargin = (epsilonA+epsilonB);
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) const iREAL MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;

    __attribute__ ((aligned(byteAlignment))) iREAL xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], d[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000];
    __attribute__ ((aligned(byteAlignment))) iREAL dd = 1E99;

    #ifdef ompTriangle
      #pragma omp critical
    #endif
    {
      batchSize += numberOfTrianglesOfGeometryB;
      batchError += MaxErrorOfPenaltyMethod;
    }

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
      #pragma simd
    #else
      #pragma omp simd
    #endif
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

        d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
    }

    bool fail = false; int counter=0;
    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if(failed[iB])
      {
        counter++;
      }
    }

    iREAL localbatchError = counter * MaxErrorOfPenaltyMethod;

    #ifdef ompTriangle
      #pragma omp critical
    #endif
    {
    numberOfPenaltyFails += counter;
    }
    if(localbatchError > ((iREAL)MaxErrorOfPenaltyMethod*(iREAL)numberOfTrianglesOfGeometryB)/32)
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
      #if defined(__INTEL_COMPILER)
        #pragma forceinline recursive
        #pragma simd
      #else
  #pragma omp simd
      #endif
      for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
      {
        bf(xCoordinatesOfPointsOfGeometryA+(iA),
           yCoordinatesOfPointsOfGeometryA+(iA),
           zCoordinatesOfPointsOfGeometryA+(iA),
           xCoordinatesOfPointsOfGeometryB+(iB),
           yCoordinatesOfPointsOfGeometryB+(iB),
           zCoordinatesOfPointsOfGeometryB+(iB),
           xPA[iB], yPA[iB], zPA[iB], xPB[iB], yPB[iB], zPB[iB]);

        d[iB] = std::sqrt(((xPB[iB]-xPA[iB])*(xPB[iB]-xPA[iB]))+((yPB[iB]-yPA[iB])*(yPB[iB]-yPA[iB]))+((zPB[iB]-zPA[iB])*(zPB[iB]-zPA[iB])));
      }
    }

    for (int iB=0; iB<numberOfTrianglesOfGeometryB*3; iB+=3)
    {
      if (d[iB] <= epsilonMargin && d[iB] <= dd)
      {
        nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        dd    = d[iB];
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
