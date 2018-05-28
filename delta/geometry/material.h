/*
 * material.h
 *
 *  Created on: 23 Mar 2017
 *      Author: konstantinos
 */

#ifndef DELTA_GEOMETRY_MATERIAL_H_
#define DELTA_GEOMETRY_MATERIAL_H_

#include <map>

namespace delta {
  namespace geometry {
    namespace material {
      enum class MaterialType: int{
         GOLD=1,
         GRAPHITE=2,
         WOOD=3
       };


      /*  enum class MaterialDensity: int{
          GOLD=20000,
          GRAPHITE=1000,
          WOOD=1000
        };*/

      enum class MaterialInteraction: char{
        WOODWOOD,//6
        GOLDGOLD,//2
        GRAPHITEGRAPHITE,//4
        WOODGOLD,//4
        GRAPHITEWOOD,
        GRAPHITEGOLD
      };

      static const std::map<delta::geometry::material::MaterialType, iREAL> materialToDensitymap(
    	  		  {{delta::geometry::material::MaterialType::WOOD, 20000},
    	  		  {delta::geometry::material::MaterialType::GRAPHITE, 1000},
    	  		  {delta::geometry::material::MaterialType::GOLD, 1000}});

      /*
      static struct InterfaceTri{
          iREAL SFRICTION;
          iREAL DAMPER;
      }tri;

      static struct InterfaceSph{
          iREAL SFRICTION;
          iREAL ROLLFRICTION;
          iREAL DAMPER;
      }sph;*/

//      static std::map<int, InterfaceSph> interfaceSphmap;
//      static std::map<int, InterfaceTri> interfaceTrimap;

      int getInterfaceType(int materialA, int materialB);

      extern void materialInit();
      int getCollisionInterface();
    }
  }
}

#endif /* DELTA_GEOMETRY_MATERIAL_H_ */
