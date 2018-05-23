/*
 * mesh.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include <delta/geometry/mesh/Mesh.h>
#include <delta/geometry/operators/physics.h>
#include <delta/geometry/operators/mesh.h>
#include <functional>
#include <limits>
#include <iostream>

delta::geometry::mesh::Mesh::Mesh()
{

}

delta::geometry::mesh::Mesh::Mesh(
	std::vector<std::array<int, 3>> 		triangleFaces,
	std::vector<std::array<iREAL, 3>> 		uniqueVertices)
{
  this->_triangleFaces = triangleFaces;
  this->_uniqueVertices = uniqueVertices;

  delta::geometry::mesh::Mesh::flatten();
}

delta::geometry::mesh::Mesh::Mesh(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  #pragma omp parallel for
  for(int i=0; i<xCoordinates.size(); i+=3)
  {
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

  compressFromVectors();
}

void delta::geometry::mesh::Mesh::compressFromVectors()
{
  std::hash<std::string> v_hash;
  std::map<unsigned int, std::array<iREAL,3>> vertices;

  #pragma omp parallel for
  for(int i=0; i<_xCoordinates.size(); i+=3)
  {
	iREAL xA = _xCoordinates[i];
	iREAL yA = _yCoordinates[i];
	iREAL zA = _zCoordinates[i];

	iREAL xB = _xCoordinates[i+1];
	iREAL yB = _yCoordinates[i+1];
	iREAL zB = _zCoordinates[i+1];

	iREAL xC = _xCoordinates[i+2];
	iREAL yC = _yCoordinates[i+2];
	iREAL zC = _zCoordinates[i+2];

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
    #pragma omp atomic
	_triangleFaces.push_back(loc);
  }
}

void delta::geometry::mesh::Mesh::compressFromVectors(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  std::hash<std::string> v_hash;
  std::map<unsigned int, std::array<iREAL,3>> vertices;

  #pragma omp parallel for
  for(int i=0; i<xCoordinates.size(); i+=3)
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
    #pragma omp atomic
	_triangleFaces.push_back(loc);
  }
}

void delta::geometry::mesh::Mesh::flatten()
{
  #pragma omp parallel for
  for(int i=0; i<_triangleFaces.size(); i++)
  {
    int idxA = _triangleFaces[i][0]; 
    int idxB = _triangleFaces[i][1]; 
    int idxC = _triangleFaces[i][2]; 
	std::array<iREAL, 3> A = _uniqueVertices[idxA];
	std::array<iREAL, 3> B = _uniqueVertices[idxB];
	std::array<iREAL, 3> C = _uniqueVertices[idxC];

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
}

void delta::geometry::mesh::Mesh::flatten(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  #pragma omp parallel for
  for(int i=0; i<_triangleFaces.size(); i++)
  {
    int idxA = _triangleFaces[i][0]; 
    int idxB = _triangleFaces[i][1]; 
    int idxC = _triangleFaces[i][2]; 
	std::array<iREAL, 3> A = _uniqueVertices[idxA];
	std::array<iREAL, 3> B = _uniqueVertices[idxB];
	std::array<iREAL, 3> C = _uniqueVertices[idxC];

	xCoordinates.push_back(A[0]);
	yCoordinates.push_back(A[1]);
	zCoordinates.push_back(A[2]);

	xCoordinates.push_back(B[0]);
	yCoordinates.push_back(B[1]);
	zCoordinates.push_back(B[2]);

	xCoordinates.push_back(C[0]);
	yCoordinates.push_back(C[1]);
	zCoordinates.push_back(C[2]);
  }
}

void delta::geometry::mesh::Mesh::replace (
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  _xCoordinates.clear();
  _yCoordinates.clear();
  _zCoordinates.clear();

  #pragma omp parallel for
  for(int i=0; i<xCoordinates.size(); i+=3)
  {
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
}

std::vector<iREAL> delta::geometry::mesh::Mesh::getXCoordinatesAsVector()
{
  return _xCoordinates;
}

std::vector<iREAL> delta::geometry::mesh::Mesh::getYCoordinatesAsVector()
{
  return _yCoordinates;
}

std::vector<iREAL> delta::geometry::mesh::Mesh::getZCoordinatesAsVector()
{
  return _zCoordinates;
}
iREAL* delta::geometry::mesh::Mesh::getXCoordinates()
{
  return _xCoordinates.data();
}

iREAL* delta::geometry::mesh::Mesh::getYCoordinates()
{
  return _yCoordinates.data();
}

iREAL* delta::geometry::mesh::Mesh::getZCoordinates()
{
  return _zCoordinates.data();
}

void delta::geometry::mesh::Mesh::moveMeshFromPositionToOrigin(iREAL center[3])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::operators::mesh::moveMeshFromPositionToOrigin(xCoordinates, yCoordinates, zCoordinates, center);

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::moveMeshFromOriginToPosition(iREAL center[3])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::operators::mesh::moveMeshFromOriginToPosition(xCoordinates, yCoordinates, zCoordinates, center);

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

  delta::geometry::operators::mesh::scaleXYZ(xCoordinates, yCoordinates, zCoordinates, scale, position);

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::rotateX(iREAL alphaX)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::operators::mesh::rotateX(xCoordinates, yCoordinates, zCoordinates, alphaX);

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::rotateY(iREAL alphaY)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::operators::mesh::rotateY(xCoordinates, yCoordinates, zCoordinates, alphaY);

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::mesh::Mesh::rotateZ(iREAL alphaZ)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->flatten(xCoordinates, yCoordinates, zCoordinates);

  delta::geometry::operators::mesh::rotateZ(xCoordinates, yCoordinates, zCoordinates, alphaZ);

  this->replace(xCoordinates, yCoordinates, zCoordinates);
}

iREAL delta::geometry::mesh::Mesh::computeDistanceAB(
    std::array<iREAL, 3> A,
    std::array<iREAL, 3> B)
{
  return std::sqrt(((B[0]-A[0])*(B[0]-A[0]))+((B[1]-A[1])*(B[1]-A[1]))+((B[2]-A[2])*(B[2]-A[2])));
}

iREAL delta::geometry::mesh::Mesh::getDiameter()
{
  iREAL width = delta::geometry::mesh::Mesh::getXYZWidth();

  return width;
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

  #pragma omp parallel for
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

  #pragma omp parallel for
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
    
    #pragma omp atomic
    {
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
    
  #pragma omp paralle for
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

  #pragma omp parallel for
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
      #pragma omp atomic
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
  delta::geometry::operators::physics::computeInertia(
		_xCoordinates,
		_yCoordinates,
		_zCoordinates,
		material,
		mass,
		center,
		inertia);
}

iREAL delta::geometry::mesh::Mesh::computeMass(
    delta::geometry::material::MaterialType material)
{
  return delta::geometry::operators::physics::computeMass(_xCoordinates, _yCoordinates, _zCoordinates, material);
}

void delta::geometry::mesh::Mesh::computeInverseInertia(
    iREAL inertia[9],
    iREAL inverse[9],
    bool isObject)
{
  delta::geometry::operators::physics::computeInverseInertia(inertia, inverse, isObject);
}

iREAL delta::geometry::mesh::Mesh::computeVolume()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  flatten(xCoordinates, yCoordinates, zCoordinates);
  return delta::geometry::operators::physics::computeVolume(xCoordinates, yCoordinates, zCoordinates);
}

std::vector<std::array<int, 3>> delta::geometry::mesh::Mesh::getTriangleFaces()
{
  return _triangleFaces;
}

std::vector<std::array<iREAL, 3>> delta::geometry::mesh::Mesh::getUniqueVertices()
{
  return _uniqueVertices;
}

delta::geometry::mesh::Mesh::Mesh::~Mesh() {

}
