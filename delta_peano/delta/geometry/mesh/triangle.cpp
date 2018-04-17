/*
 * triangles.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include <delta/geometry/mesh/Triangle.h>

delta::geometry::mesh::Triangle::Triangle(
	std::array<iREAL, 3> A,
	std::array<iREAL, 3> B,
	std::array<iREAL, 3> C)
{
  this->_A = A;
  this->_B = B;
  this->_C = C;
}

delta::geometry::mesh::Triangle::Triangle(
	iREAL A[3],
	iREAL B[3],
	iREAL C[3])
{
  this->_A = std::array<iREAL, 3>{A[0], A[1], A[2]};
  this->_B = std::array<iREAL, 3>{B[0], B[1], B[2]};
  this->_C = std::array<iREAL, 3>{C[0], C[1], C[2]};
}

delta::geometry::mesh::Triangle::Triangle(
	iREAL Ax,
	iREAL Ay,
	iREAL Az,

	iREAL Bx,
	iREAL By,
	iREAL Bz,

	iREAL Cx,
	iREAL Cy,
	iREAL Cz)
{
  this->_A = std::array<iREAL, 3>{Ax, Ay, Az};
  this->_B = std::array<iREAL, 3>{Bx, By, Bz};
  this->_C = std::array<iREAL, 3>{Cx, Cy, Cz};
}

std::array<iREAL, 3> delta::geometry::mesh::Triangle::getVertexA()
{
  return _A;
}

std::array<iREAL, 3> delta::geometry::mesh::Triangle::getVertexB()
{
  return _B;
}

std::array<iREAL, 3> delta::geometry::mesh::Triangle::getVertexC()
{
  return _C;
}

void delta::geometry::mesh::Triangle::flatten (
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
	xCoordinates.push_back(this->_A[0]);
	xCoordinates.push_back(this->_B[0]);
	xCoordinates.push_back(this->_C[0]);

	yCoordinates.push_back(this->_A[1]);
	yCoordinates.push_back(this->_B[1]);
	yCoordinates.push_back(this->_C[1]);

	zCoordinates.push_back(this->_A[2]);
	zCoordinates.push_back(this->_B[2]);
	zCoordinates.push_back(this->_C[2]);
}

iREAL delta::geometry::mesh::Triangle::getTriangleLength()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL xw = delta::geometry::mesh::Triangle::getXw();
  iREAL yw = delta::geometry::mesh::Triangle::getYw();
  iREAL zw = delta::geometry::mesh::Triangle::getZw();

  iREAL tmp = xw > yw ? xw : yw;
  iREAL width = tmp>zw ? tmp : zw;

  return getXYZWidth();
}

iREAL delta::geometry::mesh::Triangle::getXYZWidth()
{
  iREAL xw = delta::geometry::mesh::Triangle::getXw();
  iREAL yw = delta::geometry::mesh::Triangle::getYw();
  iREAL zw = delta::geometry::mesh::Triangle::getZw();

  iREAL tmp = xw > yw ? xw : yw;
  iREAL width = tmp>zw ? tmp : zw;

  return width;
}

iREAL delta::geometry::mesh::Triangle::getXZWidth()
{
  iREAL xw = delta::geometry::mesh::Triangle::getXw();
  iREAL zw = delta::geometry::mesh::Triangle::getZw();
  iREAL width = xw>zw ? xw : zw;

  return width;
}

iREAL delta::geometry::mesh::Triangle::getXw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Triangle::getMinBoundaryVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Triangle::getMaxBoundaryVertex();

  return std::abs(min[0] - max[0]);
}

iREAL delta::geometry::mesh::Triangle::getYw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Triangle::getMinBoundaryVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Triangle::getMaxBoundaryVertex();

  return std::abs(min[1] - max[1]);
}

iREAL delta::geometry::mesh::Triangle::getZw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Triangle::getMinBoundaryVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Triangle::getMaxBoundaryVertex();

  return std::abs(min[2] - max[2]);
}

std::array<iREAL, 3> delta::geometry::mesh::Triangle::getMinBoundaryVertex()
{
  std::array<iREAL, 3> vertex;

  vertex[0] = getMinXAxis();
  vertex[1] = getMinYAxis();
  vertex[2] = getMinZAxis();

  return vertex;
}

std::array<iREAL, 3> delta::geometry::mesh::Triangle::getMaxBoundaryVertex()
{
  std::array<iREAL, 3> vertex;

  vertex[0] = getMaxXAxis();
  vertex[1] = getMaxYAxis();
  vertex[2] = getMaxZAxis();

  return vertex;
}

iREAL delta::geometry::mesh::Triangle::getMaxXAxis()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL max = std::numeric_limits<iREAL>::min();

  for(unsigned i=0;i<xCoordinates.size();i++)
  {
	  if (max < xCoordinates[i]) max = xCoordinates[i];
  }
  return max;
}

iREAL delta::geometry::mesh::Triangle::getMaxYAxis()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL max = std::numeric_limits<iREAL>::min();

  for(unsigned i=0;i<yCoordinates.size();i++)
  {
	  if (max < yCoordinates[i]) max = yCoordinates[i];
  }
  return max;
}

iREAL delta::geometry::mesh::Triangle::getMaxZAxis()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL max = std::numeric_limits<iREAL>::min();

  for(unsigned i=0;i<zCoordinates.size();i++)
  {
	  if (max < zCoordinates[i]) max = zCoordinates[i];
  }
  return max;
}

iREAL delta::geometry::mesh::Triangle::getMinXAxis()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL min = std::numeric_limits<iREAL>::max();

  for(unsigned i=0;i<xCoordinates.size();i++)
  {
	  if (min > xCoordinates[i]) min = xCoordinates[i];
  }
  return min;
}

iREAL delta::geometry::mesh::Triangle::getMinYAxis()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL min = std::numeric_limits<iREAL>::max();

  for(unsigned i=0;i<yCoordinates.size();i++)
  {
	  if (min > yCoordinates[i]) min = yCoordinates[i];
  }
  return min;
}

iREAL delta::geometry::mesh::Triangle::getMinZAxis()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL min = std::numeric_limits<iREAL>::max();

  for(unsigned i=0;i<zCoordinates.size();i++)
  {
	  if (min > zCoordinates[i]) min = zCoordinates[i];
  }
  return min;
}

delta::geometry::mesh::Triangle::~Triangle() {

}

