/*
 * assembly.cpp
 *
 *  Created on: 13 Feb 2017
 *      Author: user
 */

#include "assembly.h"


double delta::primitives::getxDiscritizationLength(double length, int number)
{
	return length/number;
}

std::vector<std::array<double, 3>> delta::primitives::array1d(double position[3], double xAxisLength, int partsNo)
{
	std::vector<std::array<double, 3>> array;
	std::array<double, 3> point = {0.0, 0.0, 0.0};

	double length = getxDiscritizationLength(xAxisLength, partsNo);

	point[0] = position[0];
	point[1] = position[1];
	point[2] = position[2];
	array.push_back(point);

	for(int i=0;i<partsNo-1;i++)
	{
		point[0] += length;
		array.push_back(point);
	}
	return array;
}

std::vector<std::array<double, 3>> delta::primitives::array2d(double position[3], double xyAxisLength, int partsNo)
{
	std::vector<std::array<double, 3>> array;

	double length = getxDiscritizationLength(xyAxisLength, partsNo);

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<double, 3>> tmp = delta::primitives::array1d(position, xyAxisLength, partsNo);
		for(std::vector<std::array<double, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[2] += length;
	}

	return array;
}

std::vector<std::array<double, 3>> delta::primitives::array3d(double position[3], double xyzAxisLength, int partsNo)
{
	std::vector<std::array<double, 3>> array;

	double length = getxDiscritizationLength(xyzAxisLength, partsNo);

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<double, 3>> tmp = delta::primitives::array2d(position, xyzAxisLength, partsNo);
		for(std::vector<std::array<double, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[0] = length/2;
		position[1] += length;
		position[2] = length/2;
		if(position[1] > 1.0)
			break;
	}

	return array;
}


std::vector<std::array<double, 3>> delta::primitives::array3d(double position[3], double xyzAxisLength, int partsXYZNo, double yAxisLength, int partsYNo)
{
	std::vector<std::array<double, 3>> array;

	double length = getxDiscritizationLength(xyzAxisLength, partsXYZNo);
	double lengthY =  getxDiscritizationLength(yAxisLength, partsYNo);

	for(int i=0;i<partsXYZNo;i++)
	{
		std::vector<std::array<double, 3>> tmp = delta::primitives::array2d(position, xyzAxisLength, partsXYZNo);
		for(std::vector<std::array<double, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[0] = length/2;
		position[1] += lengthY;
		position[2] = length/2;
		if(position[1] > 1.0)
			break;
	}

	return array;
}
