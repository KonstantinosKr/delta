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
#include <fstream>


namespace delta {
	namespace core {
		void parseModelGridSchematics(std::string inputfile, std::vector<std::vector<std::string>> &componentGrid, std::vector<std::string> &componentSeq);
		void readVTKGeometry(char* fileinput,
						   std::vector<iREAL>&  xCoordinates,
						   std::vector<iREAL>&  yCoordinates,
						   std::vector<iREAL>&  zCoordinates);
	}
}

#endif /* DELTA_CORE_READ_H_ */
