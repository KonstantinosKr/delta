/*
 * write.h
 *
 *  Created on: 2 Apr 2017
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_WRITE_H_
#define DELTA_CORE_WRITE_H_


#include "delta/geometry/Object.h"
#include <vector>
#include "stdio.h"
#include "string.h"

namespace delta {
  namespace core {
	void writeGeometryToVTK(int step, std::array<iREAL, 6> boundary, std::vector<delta::geometry::Object> vectorGeometries);
  }
}

#endif /* DELTA_CORE_WRITE_H_ */
