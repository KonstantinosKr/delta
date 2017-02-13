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

//Intact Poly Brick
void delta::primitives::generateBrickFB(
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

	delta::primitives::rotateX(-0.25, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::scaleXYZ(h, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation X-wise
void delta::primitives::generateKeyXK(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::sys::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::primitives::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::primitives::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::scaleXYZ(h, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation Y-wise
void delta::primitives::generateKeyYK(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::sys::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::primitives::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::primitives::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::scaleXYZ(h, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}
