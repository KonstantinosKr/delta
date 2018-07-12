/*
 * MultiBodyObject.h
 *
 *  Created on: 6 Jul 2018
 *      Author: konstantinos
 */

#ifndef DELTA_GEOMETRY_MULTIBODYOBJECT_H_
#define DELTA_GEOMETRY_MULTIBODYOBJECT_H_

#include <delta/geometry/Object.h>
#include <array>
#include <string>
#include <vector>

namespace delta {
  namespace geometry {

	class MultiBodyObject {
	  public:
		MultiBodyObject(
			std::vector<Object> parts,
			std::string                   			component,
			int                           			particleID,
			std::array<iREAL, 3>         			centre,
			delta::geometry::material::MaterialType 	material,
			bool                          			isObstacle,
			bool                          			isFriction,
			bool                          			isConvex,
			iREAL 									epsilon,
			std::array<iREAL, 3> 					linear,
			std::array<iREAL, 3> 					angular
			);
		virtual ~MultiBodyObject();
	};

  } /* namespace geometry */
} /* namespace delta */

#endif /* DELTA_GEOMETRY_MULTIBODYOBJECT_H_ */
