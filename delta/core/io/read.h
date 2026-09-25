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

#include <cstdlib>
#include <fstream>
#include <cmath>

#include "../../geometry/structure/Mesh.h"
#include "../../world/scenario/scenarios.h"

#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>


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

	  	  void readVTKLegacy();

	  	  void parseModelGridSchematics(
				std::string 							fileName,
				std::vector<std::vector<std::string>> 	&componentGrid,
				std::vector<std::string>				&componentSeq);

	  	  delta::geometry::mesh::Mesh* readVTKGeometry(char* fileName);

	  	  void readScenarioSpecification(std::string fileName);

	  	  std::vector<delta::geometry::mesh::Mesh> readGeometry(std::string fileName);

	  	  delta::geometry::mesh::Mesh* readPartGeometry(std::string fileName);

	  	  //Returns the GRAVITY vector of the file (zero when the section is absent).
	  	  std::array<iREAL, 3> readmbfcp(std::string filename, std::vector<delta::world::structure::Object>& objects, iREAL epsilon);
	  }
	}
}

#endif /* DELTA_CORE_READ_H_ */
