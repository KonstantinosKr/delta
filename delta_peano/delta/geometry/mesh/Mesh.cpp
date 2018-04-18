/*
 * mesh.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include <delta/geometry/mesh/Mesh.h>
#include <functional>
#include <iostream>

delta::geometry::mesh::Mesh::Mesh()
{

}

delta::geometry::mesh::Mesh::Mesh(
	std::vector<std::array<int, 3>> 				triangleFaces,
	std::vector<std::array<iREAL, 3>> 			uniqueVertices)
{
  for(int i=0; i<triangleFaces.size(); i++)
  {
	std::array<iREAL, 3> A = uniqueVertices[triangleFaces[i][0]];
	std::array<iREAL, 3> B = uniqueVertices[triangleFaces[i][1]];
	std::array<iREAL, 3> C = uniqueVertices[triangleFaces[i][2]];

	delta::geometry::mesh::Triangle *triangle =
		new delta::geometry::mesh::Triangle( A[0], A[1], A[2],
											B[0], B[1], 	B[2],
											C[0], C[1], C[2]);
	_triangles.push_back(*triangle);

	_xCoordinates.push_back(A[0]);
	_yCoordinates.push_back(A[1]);
	_zCoordinates.push_back(A[2]);

	_xCoordinates.push_back(B[0]);
	_yCoordinates.push_back(B[1]);
	_zCoordinates.push_back(B[2]);

	_xCoordinates.push_back(C[0]);
	_yCoordinates.push_back(C[1]);
	_zCoordinates.push_back(C[2]);
  }

  compressFromVectors(_xCoordinates, _yCoordinates, _zCoordinates);
}

delta::geometry::mesh::Mesh::Mesh(
	std::vector<iREAL> xCoordinates,
	std::vector<iREAL> yCoordinates,
	std::vector<iREAL> zCoordinates)
{
  _xCoordinates.clear();
  _yCoordinates.clear();
  _zCoordinates.clear();

  for(int i=0; i<xCoordinates.size(); i+=3)
  {
	delta::geometry::mesh::Triangle *triangle =
		new delta::geometry::mesh::Triangle( xCoordinates[i], 	yCoordinates[i], 	zCoordinates[i],
											xCoordinates[i+1], 	yCoordinates[i+1], 	zCoordinates[i+1],
											xCoordinates[i+2], 	yCoordinates[i+2], 	zCoordinates[i+2]);
	_triangles.push_back(*triangle);

	_xCoordinates.push_back(xCoordinates[i]);
	_yCoordinates.push_back(yCoordinates[i]);
	_zCoordinates.push_back(zCoordinates[i]);

	_xCoordinates.push_back(xCoordinates[i+1]);
	_yCoordinates.push_back(yCoordinates[i+1]);
	_zCoordinates.push_back(zCoordinates[i+1]);

	_xCoordinates.push_back(xCoordinates[i+2]);
	_yCoordinates.push_back(yCoordinates[i+2]);
	_zCoordinates.push_back(zCoordinates[i+2]);
  }

  compressFromVectors(_xCoordinates, _yCoordinates, _zCoordinates);
}

delta::geometry::mesh::Mesh::Mesh(std::vector<delta::geometry::mesh::Triangle> triangles)
{
  _triangles.insert(_triangles.begin(), triangles.begin(), triangles.end());

  _xCoordinates.clear();
  _yCoordinates.clear();
  _zCoordinates.clear();

  for(int i=0; i<triangles.size(); i++)
  {
	iREAL xA = triangles[i].getVertexA()[0];
	iREAL yA = triangles[i].getVertexA()[1];
	iREAL zA = triangles[i].getVertexA()[2];

	iREAL xB = triangles[i].getVertexB()[0];
	iREAL yB = triangles[i].getVertexB()[1];
	iREAL zB = triangles[i].getVertexB()[2];

	iREAL xC = triangles[i].getVertexC()[0];
	iREAL yC = triangles[i].getVertexC()[1];
	iREAL zC = triangles[i].getVertexC()[2];

	_xCoordinates.push_back(xA);
	_yCoordinates.push_back(yA);
	_zCoordinates.push_back(zA);

	_xCoordinates.push_back(xB);
	_yCoordinates.push_back(yB);
	_zCoordinates.push_back(zB);

	_xCoordinates.push_back(xC);
	_yCoordinates.push_back(yC);
	_zCoordinates.push_back(zC);
  }

  compressFromVectors(_xCoordinates, _yCoordinates, _zCoordinates);
}

void delta::geometry::mesh::Mesh::compressFromVectors(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  //_triangleFaces.clear();
  //_uniqueVertices.clear();

  std::hash<std::string> v_hash;
  std::map<unsigned int, std::array<iREAL,3>> vertices;

  for(int i=0; i<_xCoordinates.size(); i+=3)
  {
	iREAL xA = xCoordinates[i];
	iREAL yA = yCoordinates[i];
	iREAL zA = zCoordinates[i];

	iREAL xB = xCoordinates[i+1];
	iREAL yB = yCoordinates[i+1];
	iREAL zB = zCoordinates[i+1];

	iREAL xC = xCoordinates[i+2];
	iREAL yC = yCoordinates[i+2];
	iREAL zC = zCoordinates[i+2];

	std::ostringstream ssA;
	ssA << xA << yA << zA;
	std::string A(ssA.str());
	unsigned int uniqueIDA = v_hash(A);

	if(vertices.count(uniqueIDA) == 0)
	{
	  std::array<iREAL, 3> v = {xA, yA, zA};
	  vertices[uniqueIDA] = v;
	  _uniqueVertices.push_back(vertices[uniqueIDA]);
	}

	std::ostringstream ssB;
	ssB << xB << yB << zB;
	std::string B(ssB.str());
	unsigned int uniqueIDB = v_hash(B);

	if(vertices.count(uniqueIDB) == 0)
	{
	  std::array<iREAL, 3> v = {xB, yB, zB};
	  vertices[uniqueIDB] = v;
	  _uniqueVertices.push_back(vertices[uniqueIDB]);
	}

	std::ostringstream ssC;
	ssC << xC << yC << zC;
	std::string C(ssC.str());
	unsigned int uniqueIDC = v_hash(C);

	if(vertices.count(uniqueIDC) == 0)
	{
	  std::array<iREAL, 3> v = {xC, yC, zC};
	  vertices[uniqueIDC] = v;
	  _uniqueVertices.push_back(vertices[uniqueIDC]);
	}

	int va=0,vb=0,vc=0;

	//search for vertices to construct triangleface
	for(int i=0; i<_uniqueVertices.size(); i++)
	{
	  //printf("entered loop: %i\n", i);
	  std::ostringstream ssr;
	  ssr << _uniqueVertices[i][0] << _uniqueVertices[i][1] << _uniqueVertices[i][2];
	  std::string tt(ssr.str());
	  unsigned int id = v_hash(tt);

	  if(id == uniqueIDA)
	  {
		va = i;
		//printf("entered A : %i\n", i);
		//std::cout << _uniqueVertices[i][0] << " " << _uniqueVertices[i][1] << " " << _uniqueVertices[i][2] << std::endl;
	  }
	  if(id == uniqueIDB)
	  {
		vb = i;
		//printf("entered B : %i\n", i);
		//std::cout << _uniqueVertices[i][0] << " " << _uniqueVertices[i][1] << " " << _uniqueVertices[i][2] << std::endl;
	  }
	  if(id == uniqueIDC)
	  {
		vc = i;
		//printf("entered C : %i\n", i);
		//std::cout << _uniqueVertices[i][0] << " " << _uniqueVertices[i][1] << " " << _uniqueVertices[i][2] << std::endl;
	  }
	}
	std::array<int, 3> loc = {va, vb, vc};

	_triangleFaces.push_back(loc);
  }
}

void delta::geometry::mesh::Mesh::flatten (
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  for(int i=0; i<_triangles.size(); i++)
  {
	xCoordinates.push_back(_triangles[i].getVertexA()[0]);
	xCoordinates.push_back(_triangles[i].getVertexB()[1]);
	xCoordinates.push_back(_triangles[i].getVertexC()[2]);

	yCoordinates.push_back(_triangles[i].getVertexA()[0]);
	yCoordinates.push_back(_triangles[i].getVertexB()[1]);
	yCoordinates.push_back(_triangles[i].getVertexC()[2]);

	zCoordinates.push_back(_triangles[i].getVertexA()[0]);
	zCoordinates.push_back(_triangles[i].getVertexB()[1]);
	zCoordinates.push_back(_triangles[i].getVertexC()[2]);
  }
}

void delta::geometry::mesh::Mesh::replace (
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  _triangles.clear();
  for(int i=0; i<xCoordinates.size(); i+=3)
  {
	delta::geometry::mesh::Triangle *triangle =
		new delta::geometry::mesh::Triangle( xCoordinates[i], 	yCoordinates[i], 	zCoordinates[i],
											xCoordinates[i+1], 	yCoordinates[i+1], 	zCoordinates[i+1],
											xCoordinates[i+2], 	yCoordinates[i+2], 	zCoordinates[i+2]);
	_triangles.push_back(*triangle);
  }
}

std::vector<iREAL> delta::geometry::mesh::Mesh::getxCoordinates()
{
  return _xCoordinates;
}

std::vector<iREAL> delta::geometry::mesh::Mesh::getyCoordinates()
{
  return _yCoordinates;
}

std::vector<iREAL> delta::geometry::mesh::Mesh::getzCoordinates()
{
  return _zCoordinates;
}

void delta::geometry::mesh::Mesh::moveMeshFromPositionToOrigin(iREAL center[3])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  for(unsigned i=0;i<xCoordinates.size();i++)
  {
	  xCoordinates[i] = xCoordinates[i]-center[0];
	  yCoordinates[i] = yCoordinates[i]-center[1];
	  zCoordinates[i] = zCoordinates[i]-center[2];
  }

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::moveMeshFromOriginToPosition(iREAL center[3])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  for(unsigned i=0;i<xCoordinates.size();i++)
  {
	  xCoordinates[i] = (xCoordinates[i])+center[0];
	  yCoordinates[i] = (yCoordinates[i])+center[1];
	  zCoordinates[i] = (zCoordinates[i])+center[2];
  }

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::scaleXYZ(
    iREAL scale,
    iREAL position[3])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::mesh::Mesh::moveMeshFromPositionToOrigin(position);
  for(unsigned i=0;i<xCoordinates.size();i++)
  {
	  xCoordinates[i] = xCoordinates[i]*scale;
	  yCoordinates[i] = yCoordinates[i]*scale;
	  zCoordinates[i] = zCoordinates[i]*scale;
  }
  delta::geometry::mesh::Mesh::moveMeshFromOriginToPosition(position);

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::rotateX(iREAL alphaX)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  const iREAL pi = std::acos(-1);
  for (unsigned i=0;i<xCoordinates.size(); i++)
  {
	  iREAL x = xCoordinates[i];
	  iREAL y = yCoordinates[i];
	  iREAL z = zCoordinates[i];

	  iREAL M[] = {
		 1.0,                 0.0,                   0.0,
		 0.0,  std::cos(2*pi*alphaX),  std::sin(2*pi*alphaX),
		 0.0, -std::sin(2*pi*alphaX),  std::cos(2*pi*alphaX)
	  };

	  xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
	  yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
	  zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::rotateY(iREAL alphaY)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  const iREAL pi = std::acos(-1);
  for (unsigned i=0;i<xCoordinates.size(); i++) {
	  iREAL x = xCoordinates[i];
	  iREAL y = yCoordinates[i];
	  iREAL z = zCoordinates[i];

	  iREAL M[] = {
		std::cos(2*pi*alphaY),  0.0, std::sin(2*pi*alphaY),
		0.0,                    1.0,                   0.0,
	   -std::sin(2*pi*alphaY),  0.0, std::cos(2*pi*alphaY)
	  };

	  xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
	  yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
	  zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::rotateZ(iREAL alphaZ)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  const iREAL pi = std::acos(-1);
  for (unsigned i=0;i<xCoordinates.size(); i++) {
	  iREAL x = xCoordinates[i];
	  iREAL y = yCoordinates[i];
	  iREAL z = zCoordinates[i];

	  iREAL M[] = {
		std::cos(2*pi*alphaZ),  std::sin(2*pi*alphaZ),  0.0,
	   -std::sin(2*pi*alphaZ),  std::cos(2*pi*alphaZ),  0.0,
						 0.0,                   0.0,  1.0
	  };

	  xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
	  yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
	  zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

iREAL delta::geometry::mesh::Mesh::computeDistanceAB(
    std::array<iREAL, 3> A,
    std::array<iREAL, 3> B)
{
  return std::sqrt(((B[0]-A[0])*(B[0]-A[0]))+((B[1]-A[1])*(B[1]-A[1]))+((B[2]-A[2])*(B[2]-A[2])));
}

iREAL delta::geometry::mesh::Mesh::getXYZWidth()
{
  iREAL xw = delta::geometry::mesh::Mesh::getXw();
  iREAL yw = delta::geometry::mesh::Mesh::getYw();
  iREAL zw = delta::geometry::mesh::Mesh::getZw();

  iREAL tmp = xw > yw ? xw : yw;
  iREAL width = tmp>zw ? tmp : zw;

  return width;
}

iREAL delta::geometry::mesh::Mesh::getXZWidth()
{
  iREAL xw = delta::geometry::mesh::Mesh::getXw();
  iREAL zw = delta::geometry::mesh::Mesh::getZw();
  iREAL width = xw>zw ? xw : zw;

  return width;
}

iREAL delta::geometry::mesh::Mesh::getXw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Mesh::getMinBoundaryVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Mesh::getMaxBoundaryVertex();

  return std::abs(min[0] - max[0]);
}

iREAL delta::geometry::mesh::Mesh::getYw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Mesh::getMinBoundaryVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Mesh::getMaxBoundaryVertex();

  return std::abs(min[1] - max[1]);
}

iREAL delta::geometry::mesh::Mesh::getZw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Mesh::getMinBoundaryVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Mesh::getMaxBoundaryVertex();

  return std::abs(min[2] - max[2]);
}

std::array<iREAL, 3> delta::geometry::mesh::Mesh::getMinBoundaryVertex()
{
  std::array<iREAL, 3> vertex;

  vertex[0] = getMinXAxis();
  vertex[1] = getMinYAxis();
  vertex[2] = getMinZAxis();

  return vertex;
}

std::array<iREAL, 3> delta::geometry::mesh::Mesh::getMaxBoundaryVertex()
{
  std::array<iREAL, 3> vertex;

  vertex[0] = getMaxXAxis();
  vertex[1] = getMaxYAxis();
  vertex[2] = getMaxZAxis();

  return vertex;
}

iREAL delta::geometry::mesh::Mesh::computeDiagonal()
{
  std::array<iREAL, 3> minPoint, maxPoint;

  minPoint = getMinBoundaryVertex();
  maxPoint = getMaxBoundaryVertex();

  return computeDistanceAB(minPoint,maxPoint);
}

iREAL delta::geometry::mesh::Mesh::getMaxXAxis()
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

iREAL delta::geometry::mesh::Mesh::getMaxYAxis()
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

iREAL delta::geometry::mesh::Mesh::getMaxZAxis()
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

iREAL delta::geometry::mesh::Mesh::getMinXAxis()
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

iREAL delta::geometry::mesh::Mesh::getMinYAxis()
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

iREAL delta::geometry::mesh::Mesh::getMinZAxis()
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

void delta::geometry::mesh::Mesh::getCenterOfGeometry(
	iREAL centreOfGeometry[3])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  centreOfGeometry[0] = 0.0;
  centreOfGeometry[1] = 0.0;
  centreOfGeometry[2] = 0.0;

  unsigned nVertices = xCoordinates.size();

  for(unsigned i=0;i<nVertices;i++)
  {
	  centreOfGeometry[0] += (xCoordinates[i]);
	  centreOfGeometry[1] += (yCoordinates[i]);
	  centreOfGeometry[2] += (zCoordinates[i]);
  }

  centreOfGeometry[0] = centreOfGeometry[0]/(nVertices);
  centreOfGeometry[1] = centreOfGeometry[1]/(nVertices);
  centreOfGeometry[2] = centreOfGeometry[2]/(nVertices);
}

void delta::geometry::mesh::Mesh::getCenterOfMass(
  iREAL&               centreOfMassX,
  iREAL&               centreOfMassY,
  iREAL&               centreOfMassZ,
  iREAL&               refcentreOfMassX,
  iREAL&               refcentreOfMassY,
  iREAL&               refcentreOfMassZ
) {

  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  centreOfMassX = 0.0;
  centreOfMassY = 0.0;
  centreOfMassZ = 0.0;

  unsigned nVertices = xCoordinates.size();

  for(unsigned i=0;i<nVertices;i++)
  {
    centreOfMassX += xCoordinates[i];
    centreOfMassY += yCoordinates[i];
    centreOfMassZ += zCoordinates[i];
  }

  centreOfMassX = centreOfMassX/(nVertices*3);
  centreOfMassY = centreOfMassY/(nVertices*3);
  centreOfMassZ = centreOfMassZ/(nVertices*3);

  refcentreOfMassX = centreOfMassX;
  refcentreOfMassY = centreOfMassY;
  refcentreOfMassZ = centreOfMassZ;
}

void delta::geometry::mesh::Mesh::explode(
  iREAL length
) {

  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  std::vector<iREAL> exCoordinates, eyCoordinates, ezCoordinates;

  for(unsigned i=0;i<xCoordinates.size();i+=3)
  {
    iREAL A[3], B[3], C[3];
    A[0] = xCoordinates[i];
    A[1] = yCoordinates[i];
    A[2] = zCoordinates[i];

    B[0] = xCoordinates[i+1];
    B[1] = yCoordinates[i+1];
    B[2] = zCoordinates[i+1];

    C[0] = xCoordinates[i+2];
    C[1] = yCoordinates[i+2];
    C[2] = zCoordinates[i+2];

    iREAL V[3], W[3], N[3];
    V[0] = B[0] - A[0];
    V[1] = B[1] - A[1];
    V[2] = B[2] - A[2];

    W[0] = C[0] - B[0];
    W[1] = C[1] - B[1];
    W[2] = C[2] - B[2];

    N[0] = (V[1]*W[2])-(V[2]*W[1]);
    N[1] = (V[2]*W[0])-(V[0]*W[2]);
    N[2] = (V[0]*W[1])-(V[1]*W[0]);

    iREAL mag = std::sqrt((N[0]*N[0])+(N[1]*N[1])+(N[2]*N[2]));
    N[0] = N[0]/mag;
    N[1] = N[1]/mag;
    N[2] = N[2]/mag;

    exCoordinates.push_back(xCoordinates[i] + length * N[0]);
    eyCoordinates.push_back(yCoordinates[i] + length * N[1]);
    ezCoordinates.push_back(zCoordinates[i] + length * N[2]);

    exCoordinates.push_back(xCoordinates[i+1] + length * N[0]);
    eyCoordinates.push_back(yCoordinates[i+1] + length * N[1]);
    ezCoordinates.push_back(zCoordinates[i+1] + length * N[2]);

    exCoordinates.push_back(xCoordinates[i+2] + length * N[0]);
    eyCoordinates.push_back(yCoordinates[i+2] + length * N[1]);
    ezCoordinates.push_back(zCoordinates[i+2] + length * N[2]);
  }

  xCoordinates.insert(xCoordinates.end(), exCoordinates.begin(), exCoordinates.end());
  yCoordinates.insert(yCoordinates.end(), eyCoordinates.begin(), eyCoordinates.end());
  zCoordinates.insert(zCoordinates.end(), ezCoordinates.begin(), ezCoordinates.end());
}

void delta::geometry::mesh::Mesh::exploded(
  iREAL length
) {

  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  std::vector<iREAL> exCoordinates, eyCoordinates, ezCoordinates;

  for(unsigned i=0;i<xCoordinates.size();i+=3)
  {
    iREAL A[3], B[3], C[3];
    A[0] = xCoordinates[i];
    A[1] = yCoordinates[i];
    A[2] = zCoordinates[i];

    B[0] = xCoordinates[i+1];
    B[1] = yCoordinates[i+1];
    B[2] = zCoordinates[i+1];

    C[0] = xCoordinates[i+2];
    C[1] = yCoordinates[i+2];
    C[2] = zCoordinates[i+2];

    iREAL V[3], W[3], N[3];
    V[0] = B[0] - A[0];
    V[1] = B[1] - A[1];
    V[2] = B[2] - A[2];

    W[0] = C[0] - B[0];
    W[1] = C[1] - B[1];
    W[2] = C[2] - B[2];

    N[0] = (V[1]*W[2])-(V[2]*W[1]);
    N[1] = (V[2]*W[0])-(V[0]*W[2]);
    N[2] = (V[0]*W[1])-(V[1]*W[0]);

    iREAL mag = std::sqrt((N[0]*N[0])+(N[1]*N[1])+(N[2]*N[2]));
    N[0] = N[0]/mag;
    N[1] = N[1]/mag;
    N[2] = N[2]/mag;

    xCoordinates[i] = xCoordinates[i] + length * N[0];
    yCoordinates[i] = yCoordinates[i] + length * N[1];
    zCoordinates[i] = zCoordinates[i] + length * N[2];

    xCoordinates[i+1] = xCoordinates[i+1] + length * N[0];
    yCoordinates[i+1] = yCoordinates[i+1] + length * N[1];
    zCoordinates[i+1] = zCoordinates[i+1] + length * N[2];

    xCoordinates[i+2] = xCoordinates[i+2] + length * N[0];
    yCoordinates[i+2] = yCoordinates[i+2] + length * N[1];
    zCoordinates[i+2] = zCoordinates[i+2] + length * N[2];
  }
}

iREAL delta::geometry::mesh::Mesh::getHMin()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL min = 1E99;

  for(unsigned i=0; i<xCoordinates.size(); i+=3)
  {
	iREAL A[3], B[3], C[3];
	A[0] = xCoordinates[i];
	A[1] = yCoordinates[i];
	A[2] = zCoordinates[i];

	B[0] = xCoordinates[i+1];
	B[1] = yCoordinates[i+1];
	B[2] = zCoordinates[i+1];

	C[0] = xCoordinates[i+2];
	C[1] = yCoordinates[i+2];
	C[2] = zCoordinates[i+2];

	iREAL AB = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1])+(A[2]-B[2])*(A[2]-B[2]));
	iREAL BC = sqrt((B[0]-C[0])*(B[0]-C[0])+(B[1]-C[1])*(B[1]-C[1])+(B[2]-C[2])*(B[2]-C[2]));
	iREAL CA = sqrt((C[0]-A[0])*(C[0]-A[0])+(C[1]-A[1])*(C[1]-A[1])+(C[2]-A[2])*(C[2]-A[2]));

	if (std::min(std::min(AB, BC), CA) < min)
	{
		min = std::min(std::min(AB, BC), CA);
	}
  }
  if(min == 1E99) min = 0.0;
  return min;
}

/*
 * gets the inertia using simplex integration from solfec
 */
void delta::geometry::mesh::Mesh::computeInertia(
		delta::geometry::material::MaterialType material,
		iREAL& mass,
		iREAL center[3],
		iREAL inertia[9])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL me, sx, sy, sz, euler[9], a[3], b[3], c[3], J;

  iREAL zero[3];
  zero[0] = 0;
  zero[1] = 0;
  zero[2] = 0;

  me = sx = sy = sz = 0.0;
  euler[0] = euler[1] = euler[2] =
  euler[3] = euler[4] = euler[5] =
  euler[6] = euler[7] = euler[8] = 0.0;

  iREAL rho = iREAL(delta::geometry::material::materialToDensitymap.find(material)->second);

  for (unsigned i=0;i<xCoordinates.size(); i+=3)
  {
    a[0] = xCoordinates[i];
    a[1] = yCoordinates[i];
    a[2] = zCoordinates[i];

    b[0] = xCoordinates[i+1];
    b[1] = yCoordinates[i+1];
    b[2] = zCoordinates[i+1];

    c[0] = xCoordinates[i+2];
    c[1] = yCoordinates[i+2];
    c[2] = zCoordinates[i+2];

    J = rho * simplex_J (zero, a, b, c);

    me += simplex_1 (J, zero, a, b, c);

    sx += simplex_x (J, zero, a, b, c);
    sy += simplex_y (J, zero, a, b, c);
    sz += simplex_z (J, zero, a, b, c);
    euler[0] += simplex_xx (J, zero, a, b, c);
    euler[3] += simplex_xy (J, zero, a, b, c);
    euler[4] += simplex_yy (J, zero, a, b, c);
    euler[6] += simplex_xz (J, zero, a, b, c);
    euler[7] += simplex_yz (J, zero, a, b, c);
    euler[8] += simplex_zz (J, zero, a, b, c);
  }

  mass = me;
  //printf("mass:%f\n", mass);
//#ifdef STATS
  //printf("sx:%f sy:%f sz:%f\n", sx, sy, sz);
//#endif
  center[0] = (sx / me);
  center[1] = (sy / me);
  center[2] = (sz / me);
//#ifdef STATS
  //printf("c %f c %f c %f\n", center[0], center[1], center[2]);
//#endif

#ifdef STATS
  //printf("euler %f %f %f %f %f %f %f %f %f\n", euler[0], euler[1], euler[2], euler[3], euler[4], euler[5], euler[6], euler[7], euler[8]);
#endif
  euler[0] -= (2*sx - center[0]*me)*center[0];
  euler[4] -= (2*sy - center[1]*me)*center[1];
  euler[8] -= (2*sz - center[2]*me)*center[2];
  euler[3] -= center[0]*sy + center[1]*sx - center[0]*center[1]*me;
  euler[6] -= center[0]*sz + center[2]*sx - center[0]*center[2]*me;
  euler[7] -= center[1]*sz + center[2]*sy - center[1]*center[2]*me;
  euler[1] = euler[3];
  euler[2] = euler[6];
  euler[5] = euler[7];

  /* convert Euler tensor to the inertia tensor */
  iREAL trace = TRACE (euler);
  inertia[0] = trace - euler[0];
  inertia[4] = trace - euler[4];
  inertia[8] = trace - euler[8];
  inertia[1] = -euler[1];
  inertia[2] = -euler[2];
  inertia[3] = -euler[3];
  inertia[5] = -euler[5];
  inertia[6] = -euler[6];
  inertia[7] = -euler[7]; /* inertia = tr(euler)*one - euler */
//#ifdef STATS
  //printf("Inertia %e %e %e %e %e %e %e %e %e\n", inertia[0], inertia[1], inertia[2], inertia[3], inertia[4], inertia[5], inertia[6], inertia[7], inertia[8]);
//#endif
}

iREAL delta::geometry::mesh::Mesh::computeMass(
    delta::geometry::material::MaterialType material)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL me=0, a[3], b[3], c[3], J;

  iREAL zero[3];
  zero[0] = 0;
  zero[1] = 0;
  zero[2] = 0;

  int rho= int(delta::geometry::material::materialToDensitymap.find(material)->second);

  for (unsigned i=0;i<xCoordinates.size(); i+=3)
  {
    a[0] = xCoordinates[i];
    a[1] = yCoordinates[i];
    a[2] = zCoordinates[i];

    b[0] = xCoordinates[i+1];
    b[1] = yCoordinates[i+1];
    b[2] = zCoordinates[i+1];

    c[0] = xCoordinates[i+2];
    c[1] = yCoordinates[i+2];
    c[2] = zCoordinates[i+2];

    J = rho * simplex_J (zero, a, b, c);
    me += simplex_1 (J, zero, a, b, c);
  }

  return me;
}

void delta::geometry::mesh::Mesh::computeInverseInertia(
    iREAL inertia[9],
    iREAL inverse[9],
    bool isObject)
{
  iREAL det;

  iREAL a[9], x[9];

  for (int j = 0; j < 9; j++)
  {
    a[j] = inertia[j];
  }

  INVERT (a, x, det);
	//INVERT (inertia, inverse, det);

	//zero inverse because it is an object
	if(isObject)
	{
		for (int j = 0; j < 9; j++)
		{
			inverse[j] = 0.0;
		}
	} else {
		for (int j = 0; j < 9; j++)
		{
		  inverse[j] = x[j];
		}
	}
}

iREAL delta::geometry::mesh::Mesh::computeVolume()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  flatten(xCoordinates, yCoordinates, zCoordinates);

  iREAL vol=0, a[3], b[3], c[3], J;

  iREAL zero[3];
  zero[0] = 0;
  zero[1] = 0;
  zero[2] = 0;

  for (unsigned i=0;i<xCoordinates.size(); i+=3)
  {
    a[0] = xCoordinates[i];
    a[1] = yCoordinates[i];
    a[2] = zCoordinates[i];

    b[0] = xCoordinates[i+1];
    b[1] = yCoordinates[i+1];
    b[2] = zCoordinates[i+1];

    c[0] = xCoordinates[i+2];
    c[1] = yCoordinates[i+2];
    c[2] = zCoordinates[i+2];

    J = simplex_J (zero, a, b, c);

    vol += simplex_1 (J, zero, a, b, c);
  }

  return vol;
}

iREAL delta::geometry::mesh::Mesh::simplex_J (
    iREAL *a, iREAL *b, iREAL *c, iREAL *d)
{
  iREAL q [9], J;

  q [0] = b [0] - a [0];
  q [1] = c [0] - a [0];
  q [2] = d [0] - a [0];
  q [3] = b [1] - a [1];
  q [4] = c [1] - a [1];
  q [5] = d [1] - a [1];
  q [6] = b [2] - a [2];
  q [7] = c [2] - a [2];
  q [8] = d [2] - a [2];

  J = q [0]*q [4]*q [8] + q [3]*q [7]*q [2] + q [6]*q [1]*q [5] -
      q [6]*q [4]*q [2] - q [0]*q [7]*q [5] - q [3]*q [1]*q [8];

  return J;
}
//https://en.wikipedia.org/wiki/Trace_%28linear_algebra%29
#define TRACE(A) ((A)[0] + (A)[4] + (A)[8])

std::vector<std::array<int, 3>> delta::geometry::mesh::Mesh::getTriangleFaces()
{

  for(int i=0; i<_triangleFaces.size(); i++)
  {
	std::array<iREAL, 3> A = _uniqueVertices[_triangleFaces[i][0]];
	std::array<iREAL, 3> B = _uniqueVertices[_triangleFaces[i][1]];
	std::array<iREAL, 3> C = _uniqueVertices[_triangleFaces[i][2]];

	//printf("%i\n", _triangleFaces[i][0]);

  }

  return _triangleFaces;
}

std::vector<std::array<iREAL, 3>> delta::geometry::mesh::Mesh::getUniqueVertices()
{
  //compressFromVectors(_xCoordinates, _yCoordinates, _zCoordinates);
  return _uniqueVertices;
}

delta::geometry::mesh::Mesh::Mesh::~Mesh() {

}
