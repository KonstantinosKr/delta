/*
 * read.h
 *
 *  Created on: 8 Feb 2017
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_READ_H_
#define DELTA_CORE_READ_H_

#include <string>
#include <iostream>
#include <vector>

#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <fstream>
#include "math.h"
#include <delta/geometry/mesh/Mesh.h>

namespace delta {
	namespace core {
		void parseModelGridSchematics(
			std::string 								fileName,
			std::vector<std::vector<std::string>> 	&componentGrid,
			std::vector<std::string> 				&componentSeq);

		delta::geometry::mesh::Mesh* readVTKGeometry(
			char* 				fileName);

		void readScenarioSpecification(std::string fileName);

		std::vector<delta::geometry::mesh::Mesh> readGeometry(
			std::string fileName);
	}
}

#endif /* DELTA_CORE_READ_H_ */
