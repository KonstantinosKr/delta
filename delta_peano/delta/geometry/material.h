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

      enum class MaterialDensity: int{
        GOLD=20000,
        GRAPHITE=1000,
        WOOD=1000
      };

      enum class MaterialInteraction: char{
        WOODWOOD,//6
        GOLDGOLD,//2
        GRAPHITEGRAPHITE,//4
        WOODGOLD,//4
        GRAPHITEWOOD,
        GRAPHITEGOLD
      };

      const static std::map<delta::geometry::material::MaterialType, delta::geometry::material::MaterialDensity> materialToDensitymap =
      {{delta::geometry::material::MaterialType::WOOD, delta::geometry::material::MaterialDensity::WOOD},
      {delta::geometry::material::MaterialType::GRAPHITE, delta::geometry::material::MaterialDensity::GRAPHITE},
      {delta::geometry::material::MaterialType::GOLD, delta::geometry::material::MaterialDensity::GOLD}};

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

      void materialInit();
      int getCollisionInterface();
    }
  }
}

#endif /* DELTA_GEOMETRY_MATERIAL_H_ */
