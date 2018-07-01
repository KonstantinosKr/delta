/*
 * write.h
 *
 *  Created on: 2 Apr 2017
 *      Author: konstantinos
 */

#ifndef DELTA_IO_WRITE_H_
#define DELTA_IO_WRITE_H_

#include "delta/geometry/Object.h"
#include "delta/core/data/ParticleRecord.h"
#include "delta/geometry/structures/Mesh.h"
#include <vector>
#include "stdio.h"
#include "string.h"

namespace delta {
  namespace core {
	namespace io {
	  void writeGeometryToVTK(std::array<iREAL, 6> boundary, std::vector<delta::geometry::Object>& vectorGeometries);
	  void writeGeometryToVTK(int step, std::array<iREAL, 6> boundary, std::vector<delta::core::data::ParticleRecord>& vectorGeometries);
	  void writeScenarioSpecification(std::string fileName);
	}
  }
}

#endif /* DELTA_CORE_WRITE_H_ */
