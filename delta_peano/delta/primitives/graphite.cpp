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
void delta::primitives::graphite::generateBrickFB(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/bricksmall.vtk";
	delta::core::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::primitives::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::primitives::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::properties::rotateX(-0.25, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::properties::scaleXYZ(h, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation X-wise
void delta::primitives::graphite::generateKeyXK(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::core::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::primitives::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::primitives::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::properties::scaleXYZ(h, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation Y-wise
void delta::primitives::graphite::generateKeyYK(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::core::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::primitives::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::primitives::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::properties::scaleXYZ(h, xCoordinates, yCoordinates, zCoordinates);

	delta::primitives::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}
