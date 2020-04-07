/*
 * vertex.cpp
 *
 *  Created on: 24 Aug 2018
 *      Author: konstantinos
 */

#include "geometry/operators/vertex.h"


iREAL delta::geometry::operators::vertex::computeXYZw(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  iREAL xw = delta::geometry::operators::vertex::computeXw(xCoordinates, yCoordinates, zCoordinates);
  iREAL yw = delta::geometry::operators::vertex::computeYw(xCoordinates, yCoordinates, zCoordinates);
  iREAL zw = delta::geometry::operators::vertex::computeZw(xCoordinates, yCoordinates, zCoordinates);

  iREAL tmp = xw > yw ? xw : yw;

  return tmp>zw ? tmp : zw;
}

iREAL delta::geometry::operators::vertex::computeXZw(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  iREAL xw = delta::geometry::operators::vertex::computeXw(xCoordinates, yCoordinates, zCoordinates);
  iREAL zw = delta::geometry::operators::vertex::computeZw(xCoordinates, yCoordinates, zCoordinates);

  return xw>zw ? xw : zw;
}

iREAL delta::geometry::operators::vertex::computeXw(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  std::array<iREAL, 3> min = delta::geometry::operators::vertex::computeBoundaryMinVertex(xCoordinates, yCoordinates, zCoordinates);
  std::array<iREAL, 3> max = delta::geometry::operators::vertex::computeBoundaryMaxVertex(xCoordinates, yCoordinates, zCoordinates);

  return std::abs(min[0] - max[0]);
}

iREAL delta::geometry::operators::vertex::computeYw(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  std::array<iREAL, 3> min = delta::geometry::operators::vertex::computeBoundaryMinVertex(xCoordinates, yCoordinates, zCoordinates);
  std::array<iREAL, 3> max = delta::geometry::operators::vertex::computeBoundaryMaxVertex(xCoordinates, yCoordinates, zCoordinates);

  return std::abs(min[1] - max[1]);
}

iREAL delta::geometry::operators::vertex::computeZw(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  std::array<iREAL, 3> min = delta::geometry::operators::vertex::computeBoundaryMinVertex(xCoordinates, yCoordinates, zCoordinates);
  std::array<iREAL, 3> max = delta::geometry::operators::vertex::computeBoundaryMaxVertex(xCoordinates, yCoordinates, zCoordinates);

  return std::abs(min[2] - max[2]);
}

std::array<iREAL, 6> delta::geometry::operators::vertex::computeBbox(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  std::array<iREAL, 3> vertexMin = computeBoundaryMinVertex(xCoordinates, yCoordinates, zCoordinates);
  std::array<iREAL, 3> vertexMax = computeBoundaryMaxVertex(xCoordinates, yCoordinates, zCoordinates);

  std::array<iREAL, 6> bbox = {vertexMin[0], vertexMin[1], vertexMin[2],
							  vertexMax[0], vertexMax[1], vertexMax[2]};
  return bbox;
}

std::array<iREAL, 3> delta::geometry::operators::vertex::computeBoundaryMinVertex(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  std::array<iREAL, 3> vertex = {computeMin(xCoordinates),
								computeMin(yCoordinates),
								computeMin(zCoordinates)};
  return vertex;
}

std::array<iREAL, 3> delta::geometry::operators::vertex::computeBoundaryMaxVertex(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  std::array<iREAL, 3> vertex = {computeMax(xCoordinates),
								computeMax(yCoordinates),
								computeMax(zCoordinates)};

  return vertex;
}

iREAL delta::geometry::operators::vertex::computeDiagonal(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  std::array<iREAL, 3> minPoint, maxPoint;

  minPoint = computeBoundaryMinVertex(xCoordinates, yCoordinates, zCoordinates);
  maxPoint = computeBoundaryMaxVertex(xCoordinates, yCoordinates, zCoordinates);

  return computeDistanceAB(minPoint,maxPoint);
}

iREAL delta::geometry::operators::vertex::computeDistanceAB(
    std::array<iREAL, 3> A,
    std::array<iREAL, 3> B)
{
  return std::sqrt(((B[0]-A[0])*(B[0]-A[0]))+((B[1]-A[1])*(B[1]-A[1]))+((B[2]-A[2])*(B[2]-A[2])));
}

iREAL delta::geometry::operators::vertex::computeMin(
	  std::vector<iREAL> coordinates)
{
  iREAL min = std::numeric_limits<iREAL>::max();

  for(unsigned i=0; i<coordinates.size(); i++)
  {
	if (coordinates[i] < min) min = coordinates[i];
  }
  return min;
}

iREAL delta::geometry::operators::vertex::computeMax(
	  std::vector<iREAL> coordinates)
{
  iREAL max = std::numeric_limits<iREAL>::min();

  for(unsigned i=0; i<coordinates.size(); i++)
  {
	if (coordinates[i] > max) max = coordinates[i];
  }
  return max;
}
