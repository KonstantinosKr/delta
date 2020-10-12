/*
 * write.h
 *
 *  Created on: 2 Apr 2017
 *      Author: konstantinos
 */

#ifndef DELTA_IO_WRITE_H_
#define DELTA_IO_WRITE_H_

#include <vector>
#include <string>
#include "stdio.h"
#include "string.h"


#include "../../core/data/ParticleRecord.h"
#include "../../geometry/structure/Mesh.h"
#include "../../world/structure/Object.h"

namespace delta {
  namespace core {
	namespace io {

	  void writeGeometryToVTK(
			  std::vector<delta::world::structure::Object>& 	vectorGeometries,
			  std::array<iREAL, 6> 								boundary);

	  void writeGeometryToVTK(
			  std::string										path,
			  int 												step,
			  std::array<iREAL, 6> 								boundary,
			  std::vector<delta::core::data::ParticleRecord>& 	vectorGeometries);

	  void writeGeometryToVTK(
			  std::string										path,
			  int 												step,
			  std::vector<delta::core::data::ParticleRecord>& 	geometries);

	  void writeGridGeometryToVTK(
			  std::string								path,
			  int 										step,
			  std::vector<std::array<iREAL, 6>> 		boundary);

	  void writeScenarioSpecification(std::string 	fileName);
	}
  }
}

#endif /* DELTA_CORE_WRITE_H_ */
