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
#include <delta/geometry/body/graphite.h>


#include "delta/core/read.h"
#include "delta/geometry/properties.h"

//Intact Poly Brick
void delta::geometry::body::generateBrickFB(
  iREAL center[3], iREAL h,
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates,
  std::vector<iREAL>&  zCoordinates)
{
	char fileinput[100] = "input/bricksmall.vtk";
	delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	iREAL centerOfGeometry[3];
	delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::rotateX(-0.25, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::scaleXYZ(h, centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::body::generateBrickFB(
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates,
  std::vector<iREAL>&  zCoordinates)
{
  char fileinput[100] = "input/bricksmall.vtk";
  delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);
  iREAL center[] = {0,0,0};
  iREAL centerOfGeometry[3];
  delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
  delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::properties::rotateX(-0.25, xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::body::generateBrickFB(
  iREAL center[3],
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates,
  std::vector<iREAL>&  zCoordinates)
{
  char fileinput[100] = "input/bricksmall.vtk";
  delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

  iREAL centerOfGeometry[3];
  delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
  delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
  delta::geometry::properties::rotateX(-0.25, xCoordinates, yCoordinates, zCoordinates);
  delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation X-wise
void delta::geometry::body::generateKeyXK(
  iREAL center[3], iREAL h,
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates,
  std::vector<iREAL>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	iREAL centerOfGeometry[3];
	delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::scaleXYZ(h, centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}

//Loose keys - orientation Y-wise
void delta::geometry::body::generateKeyYK(
  iREAL center[3], iREAL h,
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates,
  std::vector<iREAL>&  zCoordinates)
{
	char fileinput[100] = "input/keysmall.vtk";
	delta::core::readSingleVTKGeometry(fileinput, xCoordinates, yCoordinates, zCoordinates);

	iREAL centerOfGeometry[3];
	delta::geometry::properties::centerOfGeometry(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);
	delta::geometry::properties::moveMeshFromPositionToOrigin(centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::scaleXYZ(h, centerOfGeometry, xCoordinates, yCoordinates, zCoordinates);

	delta::geometry::properties::moveMeshFromOriginToPosition(center, xCoordinates, yCoordinates, zCoordinates);
}
