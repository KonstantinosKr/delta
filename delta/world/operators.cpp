/*
 * operators.cpp
 *
 *  Created on: 19 Apr 2018
 *      Author: konstantinos
 */

#include <delta/world/operators.h>
#include <limits>

void delta::world::operators::computeBoundary(
	 std::vector<delta::geometry::Object>& coarseObjects,
	 std::vector<delta::geometry::Object>& fineObjects,
	 std::vector<delta::geometry::Object>& objects,
	 iREAL& minParticleDiam,
	 iREAL& maxParticleDiam,
	 iREAL *minComputeDomain,
	 iREAL *maxComputeDomain)
{
  //COMPUTE MIN/MAX XYZ DOMAIN
  iREAL minx = std::numeric_limits<iREAL>::max(), miny = std::numeric_limits<iREAL>::max(), minz = std::numeric_limits<iREAL>::max();
  iREAL maxx = std::numeric_limits<iREAL>::min(), maxy = std::numeric_limits<iREAL>::min(), maxz = std::numeric_limits<iREAL>::min();

  iREAL minDiameter = std::numeric_limits<iREAL>::max();
  iREAL maxDiameter = std::numeric_limits<iREAL>::min();

  #pragma omp parallel for
  for(unsigned i=0; i<coarseObjects.size(); i++)
  {
    iREAL ominx = coarseObjects[i].getMesh().getMinXAxis();
    iREAL ominy = coarseObjects[i].getMesh().getMinYAxis();
    iREAL ominz = coarseObjects[i].getMesh().getMinZAxis();
    if(ominx < minx) minx = ominx;
    if(ominy < miny) miny = ominy;
    if(ominz < minz) minz = ominz;

    iREAL omaxx = coarseObjects[i].getMesh().getMaxXAxis();
    iREAL omaxy = coarseObjects[i].getMesh().getMaxYAxis();
    iREAL omaxz = coarseObjects[i].getMesh().getMaxZAxis();
    if(omaxx > maxx) maxx = omaxx;
    if(omaxy > maxy) maxy = omaxy;
    if(omaxz > maxz) maxz = omaxz;

    if(coarseObjects[i].getRad() * 2.0 < minDiameter)
    minDiameter = coarseObjects[i].getRad() * 2.0;
    if(coarseObjects[i].getRad() * 2.0 > maxDiameter)
    maxDiameter = coarseObjects[i].getRad() * 2.0;
  }

  #pragma omp parallel for
  for(unsigned i=0; i<objects.size(); i++)
  {
    iREAL ominx = objects[i].getMesh().getMinXAxis();
    iREAL ominy = objects[i].getMesh().getMinYAxis();
    iREAL ominz = objects[i].getMesh().getMinZAxis();
    if(ominx < minx) minx = ominx;
    if(ominy < miny) miny = ominy;
    if(ominz < minz) minz = ominz;

    iREAL omaxx = objects[i].getMesh().getMaxXAxis();
    iREAL omaxy = objects[i].getMesh().getMaxYAxis();
    iREAL omaxz = objects[i].getMesh().getMaxZAxis();
    if(omaxx > maxx) maxx = omaxx;
    if(omaxy > maxy) maxy = omaxy;
    if(omaxz > maxz) maxz = omaxz;

    if(objects[i].getRad() * 2.0 < minDiameter)
    minDiameter = objects[i].getRad() * 2.0;
    if(objects[i].getRad() * 2.0 > maxDiameter)
    maxDiameter = objects[i].getRad() * 2.0;
  }

  #pragma omp parallel for
  for(unsigned i=0; i<fineObjects.size(); i++)
  {
   iREAL ominx = fineObjects[i].getMesh().getMinXAxis();
   iREAL ominy = fineObjects[i].getMesh().getMinYAxis();
   iREAL ominz = fineObjects[i].getMesh().getMinZAxis();
   if(ominx < minx) minx = ominx;
   if(ominy < miny) miny = ominy;
   if(ominz < minz) minz = ominz;

   iREAL omaxx = fineObjects[i].getMesh().getMaxXAxis();
   iREAL omaxy = fineObjects[i].getMesh().getMaxYAxis();
   iREAL omaxz = fineObjects[i].getMesh().getMaxZAxis();
   if(omaxx > maxx) maxx = omaxx;
   if(omaxy > maxy) maxy = omaxy;
   if(omaxz > maxz) maxz = omaxz;

   if(fineObjects[i].getRad() * 2.0 < minDiameter)
   minDiameter = fineObjects[i].getRad() * 2.0;
   if(fineObjects[i].getRad() * 2.0 > maxDiameter)
   maxDiameter = fineObjects[i].getRad() * 2.0;
  }

  maxParticleDiam = maxDiameter;
  minParticleDiam = minDiameter;

  minComputeDomain[0] = minx;
  minComputeDomain[1] = miny;
  minComputeDomain[2] = minz;

  maxComputeDomain[0] = maxx;
  maxComputeDomain[1] = maxy;
  maxComputeDomain[2] = maxz;
}

