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
#include <stdio.h>
#include <string.h>


#include "core/data/ParticleRecord.h"
#include "../geometry/structure/Mesh.h"
#include "../world/structure/Object.h"
#include "core/io/FrameStaging.h"


#include <string>

namespace delta {
  namespace core {
	namespace io {

	/* One body per record, drawn as its mesh triangles (or as a sphere when the
	 * record carries no mesh). */
	void writeParticleGeometryToVTK(
		std::string 									path,
		int 											step,
		std::vector<delta::core::data::ParticleRecord>& geometries);

	/* The space decomposition: one wireframe box per subdomain, drawn as
	 * VTK_LINE edges. Named for what it is rather than for the AMReX type the
	 * caller happens to build it from. */
	void writeSpaceDecompositionToVTK(
		std::string 						path,
		int 								step,
		std::vector<std::array<iREAL, 6>> 	boundary);

	}
  }
}

#endif /* DELTA_CORE_WRITE_H_ */
