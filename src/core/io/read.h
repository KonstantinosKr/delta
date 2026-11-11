/*
 * read.h
 *
 *  Created on: 8 Feb 2017
 *      Author: konstantinos
 */

#ifndef DELTA_IO_READ_H_
#define DELTA_IO_READ_H_


#include <string>
#include <iostream>
#include <vector>
#include <array>

#include <cstdlib>
#include <fstream>
#include <cmath>

#include "../../geometry/structure/Mesh.h"
#include "../../world/scenario/scenarios.h"


namespace delta {
	namespace core {
	  namespace io {

	  	  //Every scenario asset lives in one directory pinned at configure time
	  	  //(DELTA_INPUT_DIR), so the binaries are runnable from any cwd instead of
	  	  //only from build/. The literal is the fallback for hand-rolled builds.
	  	  #ifndef DELTA_INPUT_DIR
	  	  #define DELTA_INPUT_DIR "../input"
	  	  #endif

	  	  inline std::string inputPath(std::string fileName)
	  	  {
	  	    return std::string(DELTA_INPUT_DIR) + "/" + fileName;
	  	  }

	  	  /*
	  	   * SURFACE_MATERIALS block: the contact law the file asks for.
	  	   *
	  	   * The serial contact model still uses the compiled-in SSPRING/SDAMPER
	  	   * (contact/forces/forces.cpp), so these are reported to the caller
	  	   * instead of being parsed and dropped. Making them authoritative is a
	  	   * physics change and needs a fresh baseline.
	  	   */
	  	  struct SurfaceMaterial
	  	  {
	  	    iREAL friction = 0.0;
	  	    iREAL cohesion = 0.0;
	  	    iREAL spring = 0.0;
	  	    iREAL dashpot = 0.0;
	  	  };

	  	  struct BulkMaterial
	  	  {
	  	    std::string label;
	  	    std::string model;
	  	    iREAL young = 0.0;
	  	    iREAL poisson = 0.0;
	  	    iREAL density = 0.0;
	  	  };

	  	  /*
	  	   * Everything the file declares that is not per-body geometry.
	  	   */
	  	  struct Scenario
	  	  {
	  	    std::array<iREAL, 3> gravity = {{0.0, 0.0, 0.0}};
	  	    SurfaceMaterial surface;
	  	    BulkMaterial bulk;
	  	    int bodyCount = 0;
	  	  };

	  	  delta::geometry::mesh::Mesh* readVTKGeometry(char* fileName);

	  	  delta::geometry::mesh::Mesh* readPartGeometry(std::string fileName);

	  	  //Geometry goes into `objects`; the returned struct carries the scene data
	  	  //(gravity, materials) that used to be parsed and thrown away.
	  	  Scenario readmbfcp(std::string filename, std::vector<delta::world::structure::Object>& objects, iREAL epsilon);
	  }
	}
}

#endif /* DELTA_CORE_READ_H_ */
