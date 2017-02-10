/*
 * read.h
 *
 *  Created on: 8 Feb 2017
 *      Author: konstantinos
 */

#ifndef DELTA_SYS_READ_H_
#define DELTA_SYS_READ_H_

#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <fstream>


namespace delta {
	namespace sys {
		void parseModelGridSchematics(std::string inputfile, std::vector<std::vector<std::string>> &componentGrid);
	}
}

#endif /* DELTA_SYS_READ_H_ */
