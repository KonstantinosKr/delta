/*
 * mesh.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include "../structure/Mesh.h"

#include <vector>
#include <map>
#include <cmath>
#include <array>
#include <string>
#include <sstream>
#include <functional>
#include <limits>
#include <iostream>
#include <iomanip>
#include <functional>
#include <unordered_set>
#include "../structure/Triangle.h"

delta::geometry::mesh::Mesh::Mesh()
{

}

delta::geometry::mesh::Mesh::Mesh(
	std::vector<std::array<int, 3>> 		&triangleFaces,
	std::vector<std::array<iREAL, 3>> 	&uniqueVertices)
{
  _triangleFaces = triangleFaces;
  _uniqueVertices = uniqueVertices;

  delta::geometry::mesh::Mesh::flatten();
}

delta::geometry::mesh::Mesh::Mesh(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  _maxMeshSize = 0;
  _minMeshSize = 1E99;

  iREAL min = 1E99;
  iREAL max = 0;

  //#pragma omp parallel for
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

	///////////////////////////////////////////////////

	//#pragma omp parallel for
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

	iREAL hmin = std::min(std::min(AB, BC), CA);
	iREAL hmax = std::max(std::max(AB, BC), CA);

	if (hmin < min) min = hmin;
	if (hmax > max) max = hmax;

	_avgMeshSize += hmax - hmin;
  }

  if(min == 1E99) min = 0.0;

  _minMeshSize = min;
  _maxMeshSize = max;
  _avgMeshSize = _avgMeshSize / xCoordinates.size();

  compressFromVectors();
}

void delta::geometry::mesh::Mesh::compressFromVectors()
{
  _uniqueVertices.clear();
  _triangleFaces.clear();

  //#pragma omp parallel for
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

	std::map<unsigned int, std::array<iREAL,3>> 	hashToVerticesMap;
	std::map<unsigned int, unsigned int> 		hashToUniqueVertexPositionMap;

	std::hash<std::string> v_hash;

	std::ostringstream ssA;
	ssA << xA << yA << zA;
	std::string A(ssA.str());
	unsigned int uniqueIDA = v_hash(A);

	if(hashToVerticesMap.count(uniqueIDA) == 0)
	{
	  hashToVerticesMap[uniqueIDA] = {xA, yA, zA};
	  _uniqueVertices.push_back(hashToVerticesMap[uniqueIDA]);
	  hashToUniqueVertexPositionMap[uniqueIDA] = _uniqueVertices.size()-1;
	}

	std::ostringstream ssB;
	ssB << xB << yB << zB;
	std::string B(ssB.str());
	unsigned int uniqueIDB = v_hash(B);

	if(hashToVerticesMap.count(uniqueIDB) == 0)
	{
	  hashToVerticesMap[uniqueIDB] = {xB, yB, zB};
	  _uniqueVertices.push_back(hashToVerticesMap[uniqueIDB]);
	  hashToUniqueVertexPositionMap[uniqueIDB] = _uniqueVertices.size()-1;
	}

	std::ostringstream ssC;
	ssC << xC << yC << zC;
	std::string C(ssC.str());
	unsigned int uniqueIDC = v_hash(C);

	if(hashToVerticesMap.count(uniqueIDC) == 0)
	{
	  hashToVerticesMap[uniqueIDC] = {xC, yC, zC};
	  _uniqueVertices.push_back(hashToVerticesMap[uniqueIDC]);
	  hashToUniqueVertexPositionMap[uniqueIDC] = _uniqueVertices.size()-1;
	}

	int a = hashToUniqueVertexPositionMap[uniqueIDA];
	int b = hashToUniqueVertexPositionMap[uniqueIDB];
	int c = hashToUniqueVertexPositionMap[uniqueIDC];
	std::array<int, 3> loc = {a, b, c};

    //#pragma omp critical
	_triangleFaces.push_back(loc);
  }
//  printf("compressed vertices: %i\n", _uniqueVertices.size());
}

void delta::geometry::mesh::Mesh::compressFromVectors(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  std::hash<std::string> v_hash;
  std::map<unsigned int, std::array<iREAL,3>> vertices;

  //#pragma omp parallel for
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

	std::map<unsigned int, std::array<iREAL,3>> 	hashToVerticesMap;
	std::map<unsigned int, unsigned int> 		hashToVertexPositionMap;

	std::ostringstream ssA;
	ssA << xA << yA << zA;
	std::string A(ssA.str());
	unsigned int uniqueIDA = v_hash(A);

	if(hashToVerticesMap.count(uniqueIDA) == 0)
	{
	  hashToVerticesMap[uniqueIDA] = {xA, yA, zA};
	  _uniqueVertices.push_back(hashToVerticesMap[uniqueIDA]);
	  hashToVertexPositionMap[uniqueIDA] = _uniqueVertices.size()-1;
	}

	std::ostringstream ssB;
	ssB << xB << yB << zB;
	std::string B(ssB.str());
	unsigned int uniqueIDB = v_hash(B);

	if(hashToVerticesMap.count(uniqueIDB) == 0)
	{
	  hashToVerticesMap[uniqueIDB] = {xB, yB, zB};
	  _uniqueVertices.push_back(hashToVerticesMap[uniqueIDB]);
	  hashToVertexPositionMap[uniqueIDB] = _uniqueVertices.size()-1;
	}

	std::ostringstream ssC;
	ssC << xC << yC << zC;
	std::string C(ssC.str());
	unsigned int uniqueIDC = v_hash(C);

	if(hashToVerticesMap.count(uniqueIDC) == 0)
	{
	  hashToVerticesMap[uniqueIDC] = {xC, yC, zC};
	  _uniqueVertices.push_back(hashToVerticesMap[uniqueIDC]);
	  hashToVertexPositionMap[uniqueIDC] = _uniqueVertices.size()-1;
	}

	int a = hashToVertexPositionMap[uniqueIDA];
	int b = hashToVertexPositionMap[uniqueIDB];
	int c = hashToVertexPositionMap[uniqueIDC];
	std::array<int, 3> loc = {a, b, c};

    //#pragma omp critical
	_triangleFaces.push_back(loc);
  }
}

void delta::geometry::mesh::Mesh::flatten()
{
  _xCoordinates.clear();
  _yCoordinates.clear();
  _zCoordinates.clear();

  _maxMeshSize = 0;
  _minMeshSize = 1E99;

  iREAL min = 1E99;
  iREAL max = 0;

  //#pragma omp parallel for
  for(int i=0; i<_triangleFaces.size(); i++)
  {
    int idxA = _triangleFaces[i][0];
    int idxB = _triangleFaces[i][1];
    int idxC = _triangleFaces[i][2];

	std::array<iREAL, 3> A = _uniqueVertices[idxA];
	std::array<iREAL, 3> B = _uniqueVertices[idxB];
	std::array<iREAL, 3> C = _uniqueVertices[idxC];
	//std::cout << _triangleFaces[i][0] << " " << _triangleFaces[i][1] << " " << _triangleFaces[i][2] << std::endl;

	_xCoordinates.push_back(A[0]);
	_yCoordinates.push_back(A[1]);
	_zCoordinates.push_back(A[2]);

	_xCoordinates.push_back(B[0]);
	_yCoordinates.push_back(B[1]);
	_zCoordinates.push_back(B[2]);

	_xCoordinates.push_back(C[0]);
	_yCoordinates.push_back(C[1]);
	_zCoordinates.push_back(C[2]);

	iREAL AB = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1])+(A[2]-B[2])*(A[2]-B[2]));
	iREAL BC = sqrt((B[0]-C[0])*(B[0]-C[0])+(B[1]-C[1])*(B[1]-C[1])+(B[2]-C[2])*(B[2]-C[2]));
	iREAL CA = sqrt((C[0]-A[0])*(C[0]-A[0])+(C[1]-A[1])*(C[1]-A[1])+(C[2]-A[2])*(C[2]-A[2]));

	iREAL hmin = std::min(std::min(AB, BC), CA);
	iREAL hmax = std::max(std::max(AB, BC), CA);

	if (hmin < min) min = hmin;
	if (hmax > max) max = hmax;

	_avgMeshSize += hmax - hmin;
  }

  if(min == 1E99) min = 0.0;

  _minMeshSize = min;
  _maxMeshSize = max;
  _avgMeshSize = _avgMeshSize / _xCoordinates.size();
}

void delta::geometry::mesh::Mesh::flatten(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  _maxMeshSize = 0;
  _minMeshSize = 1E99;

  iREAL min = 1E99;
  iREAL max = 0;

  //#pragma omp parallel for
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

	iREAL AB = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1])+(A[2]-B[2])*(A[2]-B[2]));
	iREAL BC = sqrt((B[0]-C[0])*(B[0]-C[0])+(B[1]-C[1])*(B[1]-C[1])+(B[2]-C[2])*(B[2]-C[2]));
	iREAL CA = sqrt((C[0]-A[0])*(C[0]-A[0])+(C[1]-A[1])*(C[1]-A[1])+(C[2]-A[2])*(C[2]-A[2]));

	iREAL hmin = std::min(std::min(AB, BC), CA);
	iREAL hmax = std::max(std::max(AB, BC), CA);

	if (hmin < min) min = hmin;
	if (hmax > max) max = hmax;

	_avgMeshSize += hmax - hmin;
  }

  if(min == 1E99) min = 0.0;

  _minMeshSize = min;
  _maxMeshSize = max;
  _avgMeshSize = _avgMeshSize / _xCoordinates.size();
}

void delta::geometry::mesh::Mesh::replace (
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  _xCoordinates.clear();
  _yCoordinates.clear();
  _zCoordinates.clear();

  //#pragma omp parallel for
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

void delta::geometry::mesh::Mesh::moveMeshToPosition(iREAL center[3])
{
  iREAL currentPosition[3] = {0,0,0};
  computeCenterOfGeometry(currentPosition);
  delta::geometry::operators::mesh::moveMeshFromPositionToOrigin(_xCoordinates, _yCoordinates, _zCoordinates, currentPosition);
  delta::geometry::operators::mesh::moveMeshFromOriginToPosition(_xCoordinates, _yCoordinates, _zCoordinates, center);
}

void delta::geometry::mesh::Mesh::moveMeshFromPositionToOrigin(iREAL center[3])
{
  delta::geometry::operators::mesh::moveMeshFromPositionToOrigin(
	  _xCoordinates, _yCoordinates, _zCoordinates, center);
}

void delta::geometry::mesh::Mesh::moveMeshFromOriginToPosition(iREAL center[3])
{
  delta::geometry::operators::mesh::moveMeshFromOriginToPosition(
	  _xCoordinates, _yCoordinates, _zCoordinates, center);
}

void delta::geometry::mesh::Mesh::scaleXYZ(
    iREAL scale)
{
  iREAL currentPosition[3] = {0,0,0};
  computeCenterOfGeometry(currentPosition);
  delta::geometry::operators::mesh::moveMeshFromPositionToOrigin(_xCoordinates, _yCoordinates, _zCoordinates, currentPosition);
  delta::geometry::operators::mesh::scaleXYZ(_xCoordinates, _yCoordinates, _zCoordinates, scale, currentPosition);
}

void delta::geometry::mesh::Mesh::rotateX(iREAL alphaX)
{
  delta::geometry::operators::mesh::rotateX(
	  _xCoordinates, _yCoordinates, _zCoordinates, alphaX);
}

void delta::geometry::mesh::Mesh::rotateY(iREAL alphaY)
{
  delta::geometry::operators::mesh::rotateY(
	  _xCoordinates, _yCoordinates, _zCoordinates, alphaY);
}

void delta::geometry::mesh::Mesh::rotateZ(iREAL alphaZ)
{
  delta::geometry::operators::mesh::rotateZ(
	  _xCoordinates, _yCoordinates, _zCoordinates, alphaZ);
}

iREAL delta::geometry::mesh::Mesh::computeDistanceAB(
    std::array<iREAL, 3> A,
    std::array<iREAL, 3> B)
{
  return std::sqrt(((B[0]-A[0])*(B[0]-A[0]))+((B[1]-A[1])*(B[1]-A[1]))+((B[2]-A[2])*(B[2]-A[2])));
}

iREAL delta::geometry::mesh::Mesh::computeDiameter()
{
  return delta::geometry::mesh::Mesh::computeXYZWidth();
}

iREAL delta::geometry::mesh::Mesh::computeXYZWidth()
{
  iREAL xw = delta::geometry::mesh::Mesh::computeXw();
  iREAL yw = delta::geometry::mesh::Mesh::computeYw();
  iREAL zw = delta::geometry::mesh::Mesh::computeZw();

  iREAL tmp = xw > yw ? xw : yw;

  return tmp>zw ? tmp : zw;
}

iREAL delta::geometry::mesh::Mesh::computeXZWidth()
{
  iREAL xw = delta::geometry::mesh::Mesh::computeXw();
  iREAL zw = delta::geometry::mesh::Mesh::computeZw();

  return xw>zw ? xw : zw;
}

iREAL delta::geometry::mesh::Mesh::computeXw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Mesh::computeBoundaryMinVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Mesh::computeBoundaryMaxVertex();

  return std::abs(min[0] - max[0]);
}

iREAL delta::geometry::mesh::Mesh::computeYw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Mesh::computeBoundaryMinVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Mesh::computeBoundaryMaxVertex();

  return std::abs(min[1] - max[1]);
}

iREAL delta::geometry::mesh::Mesh::computeZw()
{
  std::array<iREAL, 3> min = delta::geometry::mesh::Mesh::computeBoundaryMinVertex();
  std::array<iREAL, 3> max = delta::geometry::mesh::Mesh::computeBoundaryMaxVertex();

  return std::abs(min[2] - max[2]);
}

std::array<iREAL, 3> delta::geometry::mesh::Mesh::computeBoundaryMinVertex()
{
  std::array<iREAL, 3> vertex = {computeMinXAxis(), computeMinYAxis(), computeMinZAxis()};
  return vertex;
}

std::array<iREAL, 3> delta::geometry::mesh::Mesh::computeBoundaryMaxVertex()
{
  std::array<iREAL, 3> vertex = {computeMaxXAxis(), computeMaxYAxis(), computeMaxZAxis()};

  return vertex;
}

iREAL delta::geometry::mesh::Mesh::computeDiagonal()
{
  std::array<iREAL, 3> minPoint, maxPoint;

  minPoint = computeBoundaryMinVertex();
  maxPoint = computeBoundaryMaxVertex();

  return computeDistanceAB(minPoint,maxPoint);
}

iREAL delta::geometry::mesh::Mesh::computeMaxXAxis()
{
  iREAL max = std::numeric_limits<iREAL>::min();

  for(unsigned i=0;i<_xCoordinates.size();i++)
  {
	if (_xCoordinates[i] > max) max = _xCoordinates[i];
  }
  return max;
}

iREAL delta::geometry::mesh::Mesh::computeMaxYAxis()
{
  iREAL max = std::numeric_limits<iREAL>::min();

  for(unsigned i=0;i<_yCoordinates.size();i++)
  {
	if (_yCoordinates[i] > max) max = _yCoordinates[i];
  }
  return max;
}

iREAL delta::geometry::mesh::Mesh::computeMaxZAxis()
{
  iREAL max = std::numeric_limits<iREAL>::min();

  for(unsigned i=0; i<_zCoordinates.size(); i++)
  {
	if (_zCoordinates[i] > max) max = _zCoordinates[i];
  }
  return max;
}

iREAL delta::geometry::mesh::Mesh::computeMinXAxis()
{
  iREAL min = std::numeric_limits<iREAL>::max();

  for(unsigned i=0;i<_xCoordinates.size();i++)
  {
	if (_xCoordinates[i] < min) min = _xCoordinates[i];
  }
  return min;
}

iREAL delta::geometry::mesh::Mesh::computeMinYAxis()
{
  iREAL min = std::numeric_limits<iREAL>::max();

  for(unsigned i=0; i<_yCoordinates.size(); i++)
  {
	if (_yCoordinates[i] < min) min = _yCoordinates[i];
  }
  return min;
}

iREAL delta::geometry::mesh::Mesh::computeMinZAxis()
{
  iREAL min = std::numeric_limits<iREAL>::max();

  for(unsigned i=0; i<_zCoordinates.size(); i++)
  {
	if (_zCoordinates[i] < min) min = _zCoordinates[i];
  }
  return min;
}

void delta::geometry::mesh::Mesh::computeCenterOfGeometry(
	iREAL centreOfGeometry[3])
{
  centreOfGeometry[0] = 0.0;
  centreOfGeometry[1] = 0.0;
  centreOfGeometry[2] = 0.0;

  unsigned nVertices = _xCoordinates.size();

  for(unsigned i=0;i<nVertices;i++)
  {
	centreOfGeometry[0] += (_xCoordinates[i]);
	centreOfGeometry[1] += (_yCoordinates[i]);
	centreOfGeometry[2] += (_zCoordinates[i]);
  }

  centreOfGeometry[0] = centreOfGeometry[0]/(nVertices);
  centreOfGeometry[1] = centreOfGeometry[1]/(nVertices);
  centreOfGeometry[2] = centreOfGeometry[2]/(nVertices);
}

void delta::geometry::mesh::Mesh::computeCenterOfMass(
  iREAL&               centreOfMassX,
  iREAL&               centreOfMassY,
  iREAL&               centreOfMassZ,
  iREAL&               refcentreOfMassX,
  iREAL&               refcentreOfMassY,
  iREAL&               refcentreOfMassZ
) {
  centreOfMassX = 0.0;
  centreOfMassY = 0.0;
  centreOfMassZ = 0.0;

  unsigned nVertices = _xCoordinates.size();

  //#pragma omp parallel for
  for(unsigned i=0;i<nVertices;i++)
  {
    centreOfMassX += _xCoordinates[i];
    centreOfMassY += _yCoordinates[i];
    centreOfMassZ += _zCoordinates[i];
  }

  centreOfMassX = centreOfMassX/(nVertices*3);
  centreOfMassY = centreOfMassY/(nVertices*3);
  centreOfMassZ = centreOfMassZ/(nVertices*3);

  refcentreOfMassX = centreOfMassX;
  refcentreOfMassY = centreOfMassY;
  refcentreOfMassZ = centreOfMassZ;
}

void delta::geometry::mesh::Mesh::computeExplode(
	  iREAL length
) {

  std::vector<iREAL> exCoordinates, eyCoordinates, ezCoordinates;

  //#pragma omp parallel for
  for(unsigned i=0; i<_xCoordinates.size(); i+=3)
  {
    iREAL A[3], B[3], C[3];
    A[0] = _xCoordinates[i];
    A[1] = _yCoordinates[i];
    A[2] = _zCoordinates[i];

    B[0] = _xCoordinates[i+1];
    B[1] = _yCoordinates[i+1];
    B[2] = _zCoordinates[i+1];

    C[0] = _xCoordinates[i+2];
    C[1] = _yCoordinates[i+2];
    C[2] = _zCoordinates[i+2];

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
    
   // #pragma omp critical
    {
        exCoordinates.push_back(_xCoordinates[i] + length * N[0]);
        eyCoordinates.push_back(_yCoordinates[i] + length * N[1]);
        ezCoordinates.push_back(_zCoordinates[i] + length * N[2]);

        exCoordinates.push_back(_xCoordinates[i+1] + length * N[0]);
        eyCoordinates.push_back(_yCoordinates[i+1] + length * N[1]);
        ezCoordinates.push_back(_zCoordinates[i+1] + length * N[2]);

        exCoordinates.push_back(_xCoordinates[i+2] + length * N[0]);
        eyCoordinates.push_back(_yCoordinates[i+2] + length * N[1]);
        ezCoordinates.push_back(_zCoordinates[i+2] + length * N[2]);
    }
  }

  _xCoordinates.insert(_xCoordinates.end(), exCoordinates.begin(), exCoordinates.end());
  _yCoordinates.insert(_yCoordinates.end(), eyCoordinates.begin(), eyCoordinates.end());
  _zCoordinates.insert(_zCoordinates.end(), ezCoordinates.begin(), ezCoordinates.end());
}

iREAL delta::geometry::mesh::Mesh::computeHMin()
{
  iREAL min = 1E99;

  //#pragma omp parallel for
  for(unsigned i=0; i<_xCoordinates.size(); i+=3)
  {
	iREAL A[3], B[3], C[3];
	A[0] = _xCoordinates[i];
	A[1] = _yCoordinates[i];
	A[2] = _zCoordinates[i];

	B[0] = _xCoordinates[i+1];
	B[1] = _yCoordinates[i+1];
	B[2] = _zCoordinates[i+1];

	C[0] = _xCoordinates[i+2];
	C[1] = _yCoordinates[i+2];
	C[2] = _zCoordinates[i+2];

	iREAL AB = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1])+(A[2]-B[2])*(A[2]-B[2]));
	iREAL BC = sqrt((B[0]-C[0])*(B[0]-C[0])+(B[1]-C[1])*(B[1]-C[1])+(B[2]-C[2])*(B[2]-C[2]));
	iREAL CA = sqrt((C[0]-A[0])*(C[0]-A[0])+(C[1]-A[1])*(C[1]-A[1])+(C[2]-A[2])*(C[2]-A[2]));

	if (std::min(std::min(AB, BC), CA) < min)
	{
      //#pragma omp critical
	  min = std::min(std::min(AB, BC), CA);
	}
  }

  if(min == 1E99) min = 0.0;
  return min;
}

/*
 *gets the inertia using simplex integration from solfec
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
  return delta::geometry::operators::physics::computeMass(
	  _xCoordinates, _yCoordinates, _zCoordinates, material);
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
  return delta::geometry::operators::physics::computeVolume(
	  _xCoordinates, _yCoordinates, _zCoordinates);
}

std::vector<std::array<int, 3>> delta::geometry::mesh::Mesh::getTriangleFaces()
{
  return _triangleFaces;
}

std::vector<std::array<iREAL, 3>> delta::geometry::mesh::Mesh::getUniqueVertices()
{
  return _uniqueVertices;
}

iREAL delta::geometry::mesh::Mesh::getMaxMeshSize()
{
  return _maxMeshSize;
}

iREAL delta::geometry::mesh::Mesh::getMinMeshSize()
{
  return _minMeshSize;
}

iREAL delta::geometry::mesh::Mesh::getAvgMeshSize()
{
  return _avgMeshSize;
}

std::array<iREAL, 3> delta::geometry::mesh::Mesh::getBoundaryMinVertex()
{
  return _minBoundary;
}

std::array<iREAL, 3> delta::geometry::mesh::Mesh::getBoundaryMaxVertex()
{
  return _maxBoundary;
}

void delta::geometry::mesh::Mesh::toString()
{
  for(int i=0; i<_xCoordinates.size(); i+=3)
  {
	std::cout << _xCoordinates[i] << _yCoordinates[i] << _zCoordinates[i];
	std::cout << _xCoordinates[i + 1] << _yCoordinates[i + 1] << _zCoordinates[i + 1];
	std::cout << _xCoordinates[i + 2] << _yCoordinates[i + 2] << _zCoordinates[i + 2];
  }
}

delta::geometry::mesh::Mesh::Mesh::~Mesh() {

}
