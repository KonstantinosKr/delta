/*
 * material.h
 *
 *  Created on: 23 Mar 2017
 *      Author: konstantinos
 */

#ifndef DELTA_COLLISION_MATERIAL_H_
#define DELTA_COLLISION_MATERIAL_H_

#include <map>
#include <string>
//#include <pair>

namespace delta {
  namespace collision {
      static std::map<std::string, int> _materials;
      void initMaterial();
  }
}

#endif /* DELTA_COLLISION_MATERIAL_H_ */
