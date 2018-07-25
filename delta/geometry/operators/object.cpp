/*
 * object.cpp
 *
 *  Created on: 24 Jul 2018
 *      Author: konstantinos
 */

#include "object.h"

namespace delta {
  namespace geometry {
	namespace operators {
	  namespace object {

		void computeBoundary(
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

		  for(unsigned i=0; i<objects.size(); i++)
		  {
			iREAL ominx = objects[i].getMesh().computeMinXAxis();
			iREAL ominy = objects[i].getMesh().computeMinYAxis();
			iREAL ominz = objects[i].getMesh().computeMinZAxis();
			if(ominx < minx) minx = ominx;
			if(ominy < miny) miny = ominy;
			if(ominz < minz) minz = ominz;

			iREAL omaxx = objects[i].getMesh().computeMaxXAxis();
			iREAL omaxy = objects[i].getMesh().computeMaxYAxis();
			iREAL omaxz = objects[i].getMesh().computeMaxZAxis();
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
