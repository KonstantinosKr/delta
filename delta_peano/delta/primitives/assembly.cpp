/*
 * assembly.cpp
 *
 *  Created on: 13 Feb 2017
 *      Author: user
 */

#include "assembly.h"


double delta::primitives::getDiscritizationLength(double length, int number)
{
	return length/number;
}

std::vector<std::array<double, 3>> delta::primitives::array1d(double xAxisLength, int partsNo, double initY)
{
	std::vector<std::array<double, 3>> array;
	std::array<double, 3> point = {0.0, 0.0, 0.0};

	double xBoxlength = getDiscritizationLength(xAxisLength, partsNo);
	double halfXBoxlength = xBoxlength/2;
	point[0] = halfXBoxlength;
	point[1] = initY;

	array.push_back(point);

	for(int i=0;i<partsNo-1;i++)
	{
		point[0] += halfXBoxlength;

		array.push_back(point);
	}
	return array;
}

std::vector<std::array<double, 3>> delta::primitives::array2d(double xyAxisLength, int partsNo, double initY)
{
	std::vector<std::array<double, 3>> array;
	std::array<double, 3> point = {0.0, 0.0, 0.0};

	double xyBoxlength = getDiscritizationLength(xyAxisLength, partsNo);
	double halfXYBoxlength = xyBoxlength/2;
	point[0] = halfXYBoxlength;
	point[1] = initY;
	point[2] = halfXYBoxlength;

	for(int i=0;i<partsNo-1;i++)
	{
		std::vector<std::array<double, 3>>delta::primitives::array1d(xyAxisLength, partsNo, initY);
		point[2] += halfXYBoxlength;
		array.push_back(point);
	}

	return array;
}

std::vector<std::array<double, 3>> delta::primitives::array3d(double xyzAxisLength, int partsNo, double initY)
{
	std::vector<std::array<double, 3>> array;
	std::array<double, 3> point = {0.0, 0.0, 0.0};

	double xyzBoxlength = getDiscritizationLength(xyzAxisLength, partsNo);
	double halfXYZBoxlength = xyzBoxlength/2;
	point[0] = halfXYZBoxlength;
	point[1] = initY;
	point[2] = halfXYZBoxlength;

	for(int i=0;i<partsNo-1;i++)
	{
		delta::primitives::array2d(xyzAxisLength, partsNo, initY);
		point[1] += halfXYZBoxlength;
		array.push_back(point);
	}

	return array;
}
