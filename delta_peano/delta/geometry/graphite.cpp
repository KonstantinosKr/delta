#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <delta/geometry/graphite.h>
#include <float.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

//Intact Poly Brick
void delta::geometry::graphite::generateBrickFB(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/bricksmall.vtk";
	delta::core::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::rotateX(-0.25, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::scaleXYZ(h, centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation X-wise
void delta::geometry::graphite::generateKeyXK(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::core::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::scaleXYZ(h, centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation Y-wise
void delta::geometry::graphite::generateKeyYK(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::core::readVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	double centerOfGeometry[3];
	delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::scaleXYZ(h, centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}
