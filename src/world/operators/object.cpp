/*
 * object.cpp
 *
 *  Created on: 24 Jul 2018
 *      Author: konstantinos
 */

#include "object.h"

namespace delta {
  namespace world {
	namespace operators {
	  namespace object {

		void computeBoundary(
		   std::vector<delta::world::structure::Object>& 		coarseObjects,
		   std::vector<delta::world::structure::Object>& 		fineObjects,
		   std::vector<delta::world::structure::Object>& 		objects,
		   iREAL& 											minParticleDiam,
		   iREAL& 											maxParticleDiam,
		   std::array<iREAL, 3>& 							minComputeDomain,
		   std::array<iREAL, 3>& 							maxComputeDomain)
		{
		  //COMPUTE MIN/MAX XYZ DOMAIN
		  iREAL minx = std::numeric_limits<iREAL>::max(),
			  miny = std::numeric_limits<iREAL>::max(),
			  minz = std::numeric_limits<iREAL>::max();

		  iREAL maxx = std::numeric_limits<iREAL>::min(),
			  maxy = std::numeric_limits<iREAL>::min(),
			  maxz = std::numeric_limits<iREAL>::min();

		  iREAL minDiameter = std::numeric_limits<iREAL>::max();
		  iREAL maxDiameter = std::numeric_limits<iREAL>::min();

		  for(unsigned i=0; i<coarseObjects.size(); i++)
		  {
			iREAL ominx = coarseObjects[i].getMinBoundaryVertex()[0];
			iREAL ominy = coarseObjects[i].getMinBoundaryVertex()[1];
			iREAL ominz = coarseObjects[i].getMinBoundaryVertex()[2];
			if(ominx < minx) minx = ominx;
			if(ominy < miny) miny = ominy;
			if(ominz < minz) minz = ominz;

			iREAL omaxx = coarseObjects[i].getMaxBoundaryVertex()[0];
			iREAL omaxy = coarseObjects[i].getMaxBoundaryVertex()[1];
			iREAL omaxz = coarseObjects[i].getMaxBoundaryVertex()[2];
			if(omaxx > maxx) maxx = omaxx;
			if(omaxy > maxy) maxy = omaxy;
			if(omaxz > maxz) maxz = omaxz;

			if(coarseObjects[i].getRad() * 2.0 < minDiameter)
			minDiameter = coarseObjects[i].getRad() * 2.0;
			if(coarseObjects[i].getRad() * 2.0 > maxDiameter)
			maxDiameter = coarseObjects[i].getRad() * 2.0;
		  }

		  for(unsigned i=0; i<objects.size(); i++)
		  {
			iREAL ominx = objects[i].getMinBoundaryVertex()[0];
			iREAL ominy = objects[i].getMinBoundaryVertex()[1];
			iREAL ominz = objects[i].getMinBoundaryVertex()[2];
			if(ominx < minx) minx = ominx;
			if(ominy < miny) miny = ominy;
			if(ominz < minz) minz = ominz;

			iREAL omaxx = objects[i].getMaxBoundaryVertex()[0];
			iREAL omaxy = objects[i].getMaxBoundaryVertex()[1];
			iREAL omaxz = objects[i].getMaxBoundaryVertex()[2];
			if(omaxx > maxx) maxx = omaxx;
			if(omaxy > maxy) maxy = omaxy;
			if(omaxz > maxz) maxz = omaxz;

			if(objects[i].getRad() * 2.0 < minDiameter)
			minDiameter = objects[i].getRad() * 2.0;
			if(objects[i].getRad() * 2.0 > maxDiameter)
			maxDiameter = objects[i].getRad() * 2.0;
		  }

		  for(unsigned i=0; i<fineObjects.size(); i++)
		  {
		   iREAL ominx = fineObjects[i].getMinBoundaryVertex()[0];
		   iREAL ominy = fineObjects[i].getMinBoundaryVertex()[1];
		   iREAL ominz = fineObjects[i].getMinBoundaryVertex()[2];
		   if(ominx < minx) minx = ominx;
		   if(ominy < miny) miny = ominy;
		   if(ominz < minz) minz = ominz;

		   iREAL omaxx = fineObjects[i].getMaxBoundaryVertex()[0];
		   iREAL omaxy = fineObjects[i].getMaxBoundaryVertex()[1];
		   iREAL omaxz = fineObjects[i].getMaxBoundaryVertex()[2];
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

		void computeBoundary(
		   std::vector<delta::world::structure::Object>& 	objects,
		   iREAL& 										minParticleDiam,
		   iREAL& 										maxParticleDiam,
		   std::array<iREAL, 3> 							minComputeDomain,
		   std::array<iREAL, 3> 							maxComputeDomain)
		{
		  //COMPUTE MIN/MAX XYZ DOMAIN
		  iREAL minx = std::numeric_limits<iREAL>::max(), miny = std::numeric_limits<iREAL>::max(), minz = std::numeric_limits<iREAL>::max();
		  iREAL maxx = std::numeric_limits<iREAL>::min(), maxy = std::numeric_limits<iREAL>::min(), maxz = std::numeric_limits<iREAL>::min();

		  iREAL minDiameter = std::numeric_limits<iREAL>::max();
		  iREAL maxDiameter = std::numeric_limits<iREAL>::min();

		  for(unsigned i=0; i<objects.size(); i++)
		  {
			iREAL ominx = objects[i].getMinBoundaryVertex()[0];
			iREAL ominy = objects[i].getMinBoundaryVertex()[1];
			iREAL ominz = objects[i].getMinBoundaryVertex()[2];
			if(ominx < minx) minx = ominx;
			if(ominy < miny) miny = ominy;
			if(ominz < minz) minz = ominz;

			iREAL omaxx = objects[i].getMaxBoundaryVertex()[0];
			iREAL omaxy = objects[i].getMaxBoundaryVertex()[1];
			iREAL omaxz = objects[i].getMaxBoundaryVertex()[2];
			if(omaxx > maxx) maxx = omaxx;
			if(omaxy > maxy) maxy = omaxy;
			if(omaxz > maxz) maxz = omaxz;

			if(objects[i].getRad() * 2.0 < minDiameter)
			minDiameter = objects[i].getRad() * 2.0;
			if(objects[i].getRad() * 2.0 > maxDiameter)
			maxDiameter = objects[i].getRad() * 2.0;
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

	  } /* namespace Object */
	} /* namespace operators */
  } /* namespace geometry */
} /* namespace delta */
