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

#include <delta/contact/detection/hybrid.h>

void delta::contact::detection::cleanHybridStatistics() {
  _numberOfPenaltyFails = 0;
  _numberOfBatchFails = 0;
  //int batchSize = 0;
  //iREAL batchError = 0;
}

int delta::contact::detection::getPenaltyFails() {
  return _numberOfPenaltyFails;
}

int delta::contact::detection::getBatchFails() {
  return _numberOfBatchFails;
}

int delta::contact::detection::getBatchSize() {
  return _batchSize;
}

iREAL delta::contact::detection::getBatchError() {
  return _batchError;
}

#if defined(SharedTBB) && defined(peanoCall)
std::vector<delta::contact::contactpoint> delta::contact::detection::hybridWithPerTriangleFallBack(
  const iREAL*    xCoordinatesOfPointsOfGeometryA,
  const iREAL*    yCoordinatesOfPointsOfGeometryA,
  const iREAL*    zCoordinatesOfPointsOfGeometryA,
  const int       numberOfPointsOfGeometryA,
  const iREAL     epsilonA,
  const bool      frictionA,
  const int 		  particleA,

  const iREAL*    xCoordinatesOfPointsOfGeometryB,
  const iREAL*    yCoordinatesOfPointsOfGeometryB,
  const iREAL*    zCoordinatesOfPointsOfGeometryB,
  const int       numberOfPointsOfGeometryB,
  const iREAL     epsilonB,
  const bool      frictionB,
  const int	      particleB,
  tarch::multicore::BooleanSemaphore &semaphore)
#else
std::vector<delta::contact::contactpoint> delta::contact::detection::hybridWithPerTriangleFallBack(
  const iREAL*    xCoordinatesOfPointsOfGeometryA,
  const iREAL*    yCoordinatesOfPointsOfGeometryA,
  const iREAL*    zCoordinatesOfPointsOfGeometryA,
  const int       numberOfPointsOfGeometryA,
  const iREAL     epsilonA,
  const bool      frictionA,
  const int 		  particleA,

  const iREAL*    xCoordinatesOfPointsOfGeometryB,
  const iREAL*    yCoordinatesOfPointsOfGeometryB,
  const iREAL*    zCoordinatesOfPointsOfGeometryB,
  const int       numberOfPointsOfGeometryB,
  const iREAL     epsilonB,
  const bool      frictionB,
  const int	      particleB)
#endif
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

  //convert to number of points
  int numberOfTrianglesA = numberOfPointsOfGeometryA * 3;
  int numberOfTrianglesB = numberOfPointsOfGeometryB * 3;

#if defined(SharedTBB) && defined(peanoCall)
  tarch::multicore::Lock lock(semaphore,false);
#endif

  #if defined(SharedTBB) && defined(peanoCall)
	  // Take care: grain size has to be positive even if loop degenerates
	  const int grainSize = numberOfTrianglesA;
	  tbb::parallel_for(
	   tbb::blocked_range<int>(0, numberOfTrianglesA, grainSize), [&](const tbb::blocked_range<int>& r)
	   {
		 for(std::vector<int>::size_type iA=0; iA<r.size(); iA+=3)
  #else
  #ifdef OMPTriangle
	#pragma omp parallel for shared(result) firstprivate(numberOfTrianglesA, numberOfTrianglesB, epsilonA, epsilonB, frictionA, frictionB, particleA, particleB, xCoordinatesOfPointsOfGeometryA, yCoordinatesOfPointsOfGeometryA, zCoordinatesOfPointsOfGeometryA, xCoordinatesOfPointsOfGeometryB, yCoordinatesOfPointsOfGeometryB, zCoordinatesOfPointsOfGeometryB)
  #endif
  for(int iA=0; iA<numberOfTrianglesB; iA+=3)
  #endif
  {
    __attribute__ ((aligned(byteAlignment))) iREAL xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], d[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000];

    #pragma omp simd
    for (int iB=0; iB<numberOfTrianglesB; iB+=3)
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

    // Now find out whether one check has failed
    bool oneHasFailed = false;
    for (int iB=0; iB<numberOfTrianglesB; iB+=3) {
      oneHasFailed |= failed[iB];
    }

    // Rerun whole batch if necessary
    if ( oneHasFailed ) {
	  #pragma omp simd
      for (int iB=0; iB<numberOfTrianglesB; iB+=3) {
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

    // Determine minimum distance of this batch
    __attribute__ ((aligned(byteAlignment))) iREAL minD = 1E99;
    for (int iB=0; iB<numberOfTrianglesB; iB+=3) {
      minD          = std::min( d[iB], minD );
    }

    // Grab the closest one and insert it into the result
    for (int iB=0; iB<numberOfTrianglesB; iB+=3) {
      if ( d[iB] <= minD && d[iB] < epsilonMargin) {
        delta::contact::contactpoint* nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
	#if defined(SharedTBB) && defined(peanoCall)
		lock.lock();
		result.push_back(*nearestContactPoint);
		lock.free();
	#else
		#ifdef OMPTriangle
		  #pragma omp critical
		#endif
		result.push_back(*nearestContactPoint);
	#endif
      }
    }
  #if defined(SharedTBB) && defined(peanoCall)
  }});
  #else
  }
  #endif

  return result;
}

#if defined(SharedTBB) && defined(peanoCall)
std::vector<delta::contact::contactpoint> delta::contact::detection::hybridWithPerBatchFallBack(
  const iREAL*    xCoordinatesOfPointsOfGeometryA,
  const iREAL*    yCoordinatesOfPointsOfGeometryA,
  const iREAL*    zCoordinatesOfPointsOfGeometryA,
  const int       numberOfPointsOfGeometryA,
  const iREAL     epsilonA,
  const bool      frictionA,
  const int       particleA,

  const iREAL*    xCoordinatesOfPointsOfGeometryB,
  const iREAL*    yCoordinatesOfPointsOfGeometryB,
  const iREAL*    zCoordinatesOfPointsOfGeometryB,
  const int       numberOfPointsOfGeometryB,
  const iREAL     epsilonB,
  const bool      frictionB,
  const int       particleB,
  tarch::multicore::BooleanSemaphore &semaphore)
#else
std::vector<delta::contact::contactpoint> delta::contact::detection::hybridWithPerBatchFallBack(
  const iREAL*    xCoordinatesOfPointsOfGeometryA,
  const iREAL*    yCoordinatesOfPointsOfGeometryA,
  const iREAL*    zCoordinatesOfPointsOfGeometryA,
  const int       numberOfPointsOfGeometryA,
  const iREAL     epsilonA,
  const bool      frictionA,
  const int       particleA,

  const iREAL*    xCoordinatesOfPointsOfGeometryB,
  const iREAL*    yCoordinatesOfPointsOfGeometryB,
  const iREAL*    zCoordinatesOfPointsOfGeometryB,
  const int       numberOfPointsOfGeometryB,
  const iREAL     epsilonB,
  const bool      frictionB,
  const int       particleB)
#endif
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

  int numberOfTrianglesA = numberOfPointsOfGeometryA * 3;
  int numberOfTrianglesB = numberOfPointsOfGeometryB * 3;

#if defined(SharedTBB) && defined(peanoCall)
  tarch::multicore::Lock lock(semaphore,false);
#endif

  #if defined(SharedTBB) && defined(peanoCall)
  // Take care: grain size has to be positive even if loop degenerates
  const int grainSize = numberOfTrianglesA;
  tbb::parallel_for(
	  tbb::blocked_range<int>(0, numberOfTrianglesA, grainSize), [&](const tbb::blocked_range<int>& r)
	  {
		for(std::vector<int>::size_type iA=0; iA<r.size(); iA+=3)
  #else
  #ifdef OMPTriangle
	#pragma omp parallel for shared(result) firstprivate(numberOfTrianglesA, numberOfTrianglesB, epsilonA, epsilonB, frictionA, frictionB, particleA, particleB, xCoordinatesOfPointsOfGeometryA, yCoordinatesOfPointsOfGeometryA, zCoordinatesOfPointsOfGeometryA, xCoordinatesOfPointsOfGeometryB, yCoordinatesOfPointsOfGeometryB, zCoordinatesOfPointsOfGeometryB)
  #endif
  for(int iA=0; iA<numberOfTrianglesA; iA+=3)
  #endif
  {
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) iREAL xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], d[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000] = {0};
    __attribute__ ((aligned(byteAlignment))) iREAL dd = epsilonA+epsilonB;

    #pragma omp simd
    for (int iB=0; iB<numberOfTrianglesB; iB+=3)
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
    #pragma omp simd reduction(+:counter)
    for (int iB=0; iB<numberOfTrianglesB; iB+=3)
    {
      if(failed[iB]) counter++;
    }

    iREAL localbatchError = counter * MaxErrorOfPenalty;
    if(localbatchError > ((iREAL)MaxErrorOfPenalty*(iREAL)numberOfTrianglesB)/32.0)
    {
      fail = true;
    }

    // If the maximum error in the whole computation exceeds our limit, we fall
    // back to brute force. For this, we reset nearestContactPoint and
    // minDistance and then run the interior loop over iB again with bf().
    if (fail)
    {
	  #pragma omp simd
      for (int iB=0; iB<numberOfTrianglesB; iB+=3)
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

    __attribute__ ((aligned(byteAlignment))) iREAL minD = 1E99;
    for (int iB=0; iB<numberOfTrianglesB; iB+=3) {
      minD          = std::min( d[iB], minD );
    }

    // Grab the closest one and insert it into the result
    for (int iB=0; iB<numberOfTrianglesB; iB+=3) {
      if ( d[iB] <= minD && d[iB] < epsilonMargin) {
        delta::contact::contactpoint* nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
		#if defined(SharedTBB) && defined(peanoCall)
			lock.lock();
			  result.push_back(*nearestContactPoint);
			lock.free();
		#else
			#ifdef OMPTriangle
			  #pragma omp critical
			#endif
			result.push_back(*nearestContactPoint);
		#endif
      }
    }
  #if defined(SharedTBB) && defined(peanoCall)
  }});
  #else
  }
  #endif
  return result;
}

std::vector<delta::contact::contactpoint> delta::contact::detection::hybridBatchStat(
  const iREAL*    xCoordinatesOfPointsOfGeometryA,
  const iREAL*    yCoordinatesOfPointsOfGeometryA,
  const iREAL*    zCoordinatesOfPointsOfGeometryA,
  const int       numberOfPointsOfGeometryA,
  const iREAL     epsilonA,
  const bool      frictionA,
  const int       particleA,

  const iREAL*    xCoordinatesOfPointsOfGeometryB,
  const iREAL*    yCoordinatesOfPointsOfGeometryB,
  const iREAL*    zCoordinatesOfPointsOfGeometryB,
  const int       numberOfPointsOfGeometryB,
  const iREAL     epsilonB,
  const bool      frictionB,
  const int       particleB)
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

  #ifdef OMPTriangle
    #pragma omp parallel for schedule(static)
  #endif
  for (int iA=0; iA<numberOfPointsOfGeometryA*3; iA+=3)
  {
    __attribute__ ((aligned(byteAlignment))) iREAL epsilonMargin = (epsilonA+epsilonB);
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) const iREAL MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;
    __attribute__ ((aligned(byteAlignment))) iREAL dd = epsilonA+epsilonB;

    __attribute__ ((aligned(byteAlignment))) iREAL xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], d[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000];

    #ifdef OMPTriangle
      #pragma omp critical
    #endif
    {
      _batchSize += numberOfPointsOfGeometryB;
      _batchError += MaxErrorOfPenaltyMethod;
    }

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
      #pragma omp simd
    #else
      #pragma omp simd
    #endif
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3)
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
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3)
    {
      if(failed[iB])
      {
        counter++;
      }
    }

    iREAL localbatchError = counter * MaxErrorOfPenaltyMethod;

    #ifdef OMPTriangle
      #pragma omp critical
    #endif
    {
      _numberOfPenaltyFails += counter;
    }
    if(localbatchError > ((iREAL)MaxErrorOfPenaltyMethod*(iREAL)numberOfPointsOfGeometryB)/32)
    {
      fail = true;
      #ifdef OMPTriangle
        #pragma omp critical
      #endif
      _numberOfBatchFails += 1;
    }

    // If the maximum error in the whole computation exceeds our limit, we fall
    // back to brute force. For this, we reset nearestContactPoint and
    // minDistance and then run the interior loop over iB again with bf().
    if(fail)
    {
      #if defined(__INTEL_COMPILER)
        #pragma forceinline recursive
        #pragma omp simd
      #else
	      #pragma omp simd
      #endif
      for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3)
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

    __attribute__ ((aligned(byteAlignment))) iREAL minD = 1E99;
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3) {
      minD          = std::min( d[iB], minD );
    }

    // Grab the closest one and insert it into the result
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3) {
      if ( d[iB] <= minD && d[iB] < epsilonMargin) {
        delta::contact::contactpoint* nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        #ifdef OMPTriangle
          #pragma omp critical
        #endif
        result.push_back(*nearestContactPoint);
      }
    }
  }
  return result;
}

std::vector<delta::contact::contactpoint> delta::contact::detection::hybridTriangleStat(
  const iREAL*    xCoordinatesOfPointsOfGeometryA,
  const iREAL*    yCoordinatesOfPointsOfGeometryA,
  const iREAL*    zCoordinatesOfPointsOfGeometryA,
  const int       numberOfPointsOfGeometryA,
  const iREAL     epsilonA,
  const bool      frictionA,
  const int       particleA,

  const iREAL*    xCoordinatesOfPointsOfGeometryB,
  const iREAL*    yCoordinatesOfPointsOfGeometryB,
  const iREAL*    zCoordinatesOfPointsOfGeometryB,
  const int       numberOfPointsOfGeometryB,
  const iREAL     epsilonB,
  const bool      frictionB,
  const int       particleB)
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

  #ifdef OMPTriangle
    #pragma omp parallel for schedule(static)
  #endif
  for (int iA=0; iA<numberOfPointsOfGeometryA*3; iA+=3)
  {
    __attribute__ ((aligned(byteAlignment))) iREAL epsilonMargin = (epsilonA+epsilonB);
    __attribute__ ((aligned(byteAlignment))) contactpoint *nearestContactPoint = nullptr;
    __attribute__ ((aligned(byteAlignment))) const iREAL MaxErrorOfPenaltyMethod = (epsilonA+epsilonB)/16;

    __attribute__ ((aligned(byteAlignment))) iREAL xPA[10000], yPA[10000], zPA[10000], xPB[10000], yPB[10000], zPB[10000], d[10000];
    __attribute__ ((aligned(byteAlignment))) bool failed[10000];
    __attribute__ ((aligned(byteAlignment))) iREAL dd = epsilonA+epsilonB;

    #ifdef OMPTriangle
      #pragma omp critical
    #endif
    {
      _batchSize += numberOfPointsOfGeometryB;
      _batchError += MaxErrorOfPenaltyMethod;
    }

    #if defined(__INTEL_COMPILER)
      #pragma forceinline recursive
    #endif
	#pragma omp simd
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3)
    {
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
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3)
    {
      if(failed[iB])
      {
        counter++;
      }
    }

    iREAL localbatchError = counter * MaxErrorOfPenaltyMethod;

    #ifdef OMPTriangle
      #pragma omp critical
    #endif
    {
      _numberOfPenaltyFails += counter;
    }

    if(localbatchError > ((iREAL)MaxErrorOfPenaltyMethod*(iREAL)numberOfPointsOfGeometryB)/32)
    {
      fail = true;
      #ifdef OMPTriangle
        #pragma omp critical
      #endif
      _numberOfBatchFails += 1;
    }

    // If the maximum error in the whole computation exceeds our limit, we fall
    // back to brute force. For this, we reset nearestContactPoint and
    // minDistance and then run the interior loop over iB again with bf().
    if(fail)
    {
      #if defined(__INTEL_COMPILER)
        #pragma forceinline recursive
      #endif
	  #pragma omp simd
      for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3)
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

    __attribute__ ((aligned(byteAlignment))) iREAL minD = 1E99;
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3) {
      minD          = std::min( d[iB], minD );
    }

    // Grab the closest one and insert it into the result
    for (int iB=0; iB<numberOfPointsOfGeometryB*3; iB+=3) {
      if ( d[iB] <= minD && d[iB] < epsilonMargin) {
        delta::contact::contactpoint* nearestContactPoint = new contactpoint(xPA[iB], yPA[iB], zPA[iB], epsilonA, particleA, xPB[iB], yPB[iB], zPB[iB], epsilonB, particleB, frictionA && frictionB);
        #ifdef OMPTriangle
          #pragma omp critical
        #endif
        result.push_back(*nearestContactPoint);
      }
    }
  }
  return result;
}
