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

void delta::sys::readVTKGeometry(char* fileinput,
							  std::vector<double>&  xCoordinates,
							  std::vector<double>&  yCoordinates,
							  std::vector<double>&  zCoordinates)
{
	//////////VTK format////////////

	char filename[100];
	strncpy(filename, fileinput, 100);
	FILE *fp1 = fopen(filename, "r+");

	if( fp1 == NULL )
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	char ch, word[100];
	double *point[3];

	do
	{
		ch = fscanf(fp1,"%s",word);
		if(strcmp(word, "POINTS")==0)
		{
		  ch = fscanf(fp1,"%s",word);
		  int n = atol(word);

		  point[0] = new double[n];
		  point[1] = new double[n];
		  point[2] = new double[n];

		  ch = fscanf(fp1,"%s",word);

		  for(int i=0;i<n;i++)
		  {
			fscanf(fp1, "%lf", &point[0][i]);
			fscanf(fp1, "%lf", &point[1][i]);
			fscanf(fp1, "%lf", &point[2][i]);
		  }
		}

		if(strcmp(word, "CELLS")==0 || strcmp(word, "POLYGONS") == 0)
		{
		  ch = fscanf(fp1,"%s",word);
		  int numberOfTriangles = atol(word);
		  ch = fscanf(fp1,"%s",word);

		  xCoordinates.resize( numberOfTriangles*3 );
		  yCoordinates.resize( numberOfTriangles*3 );
		  zCoordinates.resize( numberOfTriangles*3 );

		  for(int i=0;i<numberOfTriangles*3;i+=3)
		  {
			ch = fscanf(fp1,"%s",word);
			ch = fscanf(fp1,"%s",word);

			int index = atol(word);
			xCoordinates[i] = ((point[0][index]));
			yCoordinates[i] = ((point[1][index]));
			zCoordinates[i] = ((point[2][index]));

			ch = fscanf(fp1,"%s",word);
			index = atol(word);
			xCoordinates[i+1] = ((point[0][index]));
			yCoordinates[i+1] = ((point[1][index]));
			zCoordinates[i+1] = ((point[2][index]));

			ch = fscanf(fp1,"%s",word);
			index = atol(word);
			xCoordinates[i+2] = ((point[0][index]));
			yCoordinates[i+2] = ((point[1][index]));
			zCoordinates[i+2] = ((point[2][index]));
		  }
		}
	} while (ch != EOF);

	fclose(fp1);
}
