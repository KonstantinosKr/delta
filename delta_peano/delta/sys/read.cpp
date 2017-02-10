/*
 * read.cpp
 *
 *  Created on: 8 Feb 2017
 *      Author: konstantinos
 */

#include "read.h"
#include "math.h"
#include <stdlib.h>

std::vector<std::string> splitString(std::string input, std::string delimiter)
{
 std::vector<std::string> output;
 char *pch;

 char *str = strdup(input.c_str());

 pch = strtok (str, delimiter.c_str());

 while (pch != NULL)
 {
	output.push_back(pch);

	pch = strtok (NULL,  delimiter.c_str());
 }

 free(str);

 return output;
}

void delta::sys::parseModelGridSchematics(std::string inputfile, std::vector<std::vector<std::string>> &componentGrid)
{
	std::string line;
	//46 * 46
	std::ifstream myfile;
	myfile.open(inputfile);

	if (myfile.is_open())
	{
		std::vector<std::string> componentX;
		while (std::getline (myfile, line))
		{
			std::vector<std::string> vstring = splitString(line, ",");
			//std::cout << vstring[0] << " " << vstring[1] << " " << vstring[2] << "\n";

			componentX.push_back(vstring[2]);

			if(std::stoi(vstring[0]) == 46) {
				//std::cout << std::stoi(vstring[0]) << "\n";
				componentGrid.push_back(componentX);
			}
		}

		myfile.close();
	}
	else std::cout << "Unable to open file";
}
