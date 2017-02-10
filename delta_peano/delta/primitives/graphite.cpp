#include "graphite.h"

#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

void delta::primitives::generateBrick(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/bricksmall.vtk";
	delta::sys::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::primitives::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::primitives::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::rotateX(90, xCoordinates, yCoordinates, zCoordinates);

	for(int i=0;i<xCoordinates.size();i+=3)
	{
		xCoordinates[i] = (xCoordinates[i]*h);
		yCoordinates[i] = (yCoordinates[i]*h);
		zCoordinates[i] = (zCoordinates[i]*h);

		xCoordinates[i+1] = (xCoordinates[i+1]*h);
		yCoordinates[i+1] = (yCoordinates[i+1]*h);
		zCoordinates[i+1] = (zCoordinates[i+1]*h);

		xCoordinates[i+2] = (xCoordinates[i+2]*h);
		yCoordinates[i+2] = (yCoordinates[i+2]*h);
		zCoordinates[i+2] = (zCoordinates[i+2]*h);
	}

	delta::primitives::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

void delta::primitives::generateKey(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	double rh = 0;
	char fileinput[100] = "input/keysmall.vtk";
	delta::sys::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::primitives::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::primitives::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	for(int i=0;i<xCoordinates.size();i+=3)
	{
		xCoordinates[i] = (xCoordinates[i]*h);
		yCoordinates[i] = (yCoordinates[i]*h);
		zCoordinates[i] = (zCoordinates[i]*h);

		xCoordinates[i+1] = (xCoordinates[i+1]*h);
		yCoordinates[i+1] = (yCoordinates[i+1]*h);
		zCoordinates[i+1] = (zCoordinates[i+1]*h);

		xCoordinates[i+2] = (xCoordinates[i+2]*h);
		yCoordinates[i+2] = (yCoordinates[i+2]*h);
		zCoordinates[i+2] = (zCoordinates[i+2]*h);
	}

	delta::primitives::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}
