/*
 The MIT License (MIT)

 Copyright (c) 26 May 2016 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "triangle.h"

#include <stdlib.h>
#include <assert.h>
#include <cmath>

void delta::primitives::triangle::bisectTriangle(
		double A[3],
		double B[3],
		double C[3],
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	//UNOPTIMIZED
	/*double D[3], E[3], F[3];

	//BC = base
	//A = head
	//middlepoint = halfpoint of base

	double middlepoint[3];

	middlepoint[0] = (C[0] + B[0])/2;
	middlepoint[1] = (C[1] + B[1])/2;
	middlepoint[2] = (C[2] + B[2])/2;

	//Triangle one - A B Middlepoint
	D[0] = A[0];
	D[1] = A[1];
	D[2] = A[2];

	E[0] = B[0];
	E[1] = B[1];
	E[2] = B[2];

	F[0] = middlepoint[0];
	F[1] = middlepoint[1];
	F[2] = middlepoint[2];

	//Triangle two - A C Middlepoint
	B[0] = C[0];
	B[1] = C[1];
	B[2] = C[2];

	C[0] = middlepoint[0];
	C[1] = middlepoint[1];
	C[2] = middlepoint[2];
	////////////////////////////////
	xCoordinates.push_back(D[0]);
	xCoordinates.push_back(E[0]);
	xCoordinates.push_back(F[0]);

	yCoordinates.push_back(D[1]);
	yCoordinates.push_back(E[1]);
	yCoordinates.push_back(F[1]);

	zCoordinates.push_back(D[2]);
	zCoordinates.push_back(E[2]);
	zCoordinates.push_back(F[2]);
	////////////////////////////////

	xCoordinates.push_back(A[0]);
	xCoordinates.push_back(B[0]);
	xCoordinates.push_back(C[0]);

	yCoordinates.push_back(A[1]);
	yCoordinates.push_back(B[1]);
	yCoordinates.push_back(C[1]);

	zCoordinates.push_back(A[2]);
	zCoordinates.push_back(B[2]);
	zCoordinates.push_back(C[2]);*/

	//OPTIMIZED
	////////////////////////////////
	xCoordinates.push_back(A[0]);
	xCoordinates.push_back(B[0]);
	xCoordinates.push_back((C[0] + B[0])*0.5);

	yCoordinates.push_back(A[1]);
	yCoordinates.push_back(B[1]);
	yCoordinates.push_back((C[1] + B[1])*0.5);

	zCoordinates.push_back(A[2]);
	zCoordinates.push_back(B[2]);
	zCoordinates.push_back((C[2] + B[2])*0.5);
	////////////////////////////////

	xCoordinates.push_back(A[0]);
	xCoordinates.push_back(C[0]);
	xCoordinates.push_back((C[0] + B[0])*0.5);

	yCoordinates.push_back(A[1]);
	yCoordinates.push_back(C[1]);
	yCoordinates.push_back((C[1] + B[1])*0.5);

	zCoordinates.push_back(A[2]);
	zCoordinates.push_back(C[2]);
	zCoordinates.push_back((C[2] + B[2])*0.5);
}

void delta::primitives::triangle::meshDenseMultiplier(
		int  multiplier,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{

	std::vector<double>  xNewCoordinates;
	std::vector<double>  yNewCoordinates;
	std::vector<double>  zNewCoordinates;

	for(unsigned i=0;i<xCoordinates.size();i+=3)
	{
		double A[3], B[3], C[3];
		A[0] = xCoordinates[i];
		A[1] = yCoordinates[i];
		A[2] = zCoordinates[i];

		B[0] = xCoordinates[i+1];
		B[1] = yCoordinates[i+1];
		B[2] = zCoordinates[i+1];

		C[0] = xCoordinates[i+2];
		C[1] = yCoordinates[i+2];
		C[2] = zCoordinates[i+2];

		bisectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
	}
	xCoordinates = xNewCoordinates;
	yCoordinates = yNewCoordinates;
	zCoordinates = zNewCoordinates;
	if(multiplier>1) meshDenseMultiplier(multiplier-1, xCoordinates, yCoordinates, zCoordinates);
}

int delta::primitives::triangle::meshQuadSect(int quadsectTimes,
    std::vector<std::vector<double>>&  xCoordinatesVec,
    std::vector<std::vector<double>>&  yCoordinatesVec,
    std::vector<std::vector<double>>&  zCoordinatesVec, std::vector<std::array<double, 3>>& centroid)
{
  std::array<double, 3> minpoint = delta::primitives::properties::minBoundaryVertex(xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0]);
  std::array<double, 3> maxpoint = delta::primitives::properties::maxBoundaryVertex(xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0]);

  double xw = maxpoint[0]-minpoint[0]; double yw = maxpoint[1]-minpoint[1]; double zw = maxpoint[2]-minpoint[2];
  std::array<double, 3> midpoint = {minpoint[0]+xw/2, minpoint[1]+yw/2, minpoint[2]+zw/2};

  /*std::vector<double> xCoordinatesBox, yCoordinatesBox, zCoordinatesBox;
  double centre[3] = {midpoint[0], midpoint[1], midpoint[2]};
  delta::primitives::surface::generateBoundBox(centre, minpoint, maxpoint, xCoordinatesBox, yCoordinatesBox, zCoordinatesBox);

  for(int i=0; i<xCoordinatesBox.size(); i++)
  {
    xCoordinatesVec[0].push_back(xCoordinatesBox[i]);
    yCoordinatesVec[0].push_back(yCoordinatesBox[i]);
    zCoordinatesVec[0].push_back(zCoordinatesBox[i]);
  }*/

  /////////////////////////////////////////////////////////////////////////

  return quadsect(quadsectTimes, 0, minpoint, midpoint, maxpoint, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);;
}

int delta::primitives::triangle::quadsect(int level, int index, std::array<double, 3> minpoint, std::array<double, 3> midpoint, std::array<double, 3> maxpoint,
    std::vector<std::vector<double>>&  xCoordinatesVec,
    std::vector<std::vector<double>>&  yCoordinatesVec,
    std::vector<std::vector<double>>&  zCoordinatesVec, std::vector<std::array<double, 3>>& centroid)
{

  if(level < 1) return index;
  level--;

  xCoordinatesVec.resize(xCoordinatesVec.size()+4);
  yCoordinatesVec.resize(yCoordinatesVec.size()+4);
  zCoordinatesVec.resize(zCoordinatesVec.size()+4);
  centroid.resize(centroid.size()+4);

  index++;
  int indexA = index;
  index++;
  int indexB = index;
  index++;
  int indexC = index;
  index++;
  int indexD = index;

  ////////////////////////////////////////////////////////////////////////////////

  double xw = maxpoint[0]-minpoint[0];//source/parent width
  double yw = maxpoint[1]-minpoint[1];
  double zw = maxpoint[2]-minpoint[2];

  //////////////////No1 -A- BACK LEFT BOX////////////////////////////////////////
  std::array<double, 3> minpointA = minpoint;
  std::array<double, 3> maxpointA = {midpoint[0], maxpoint[1], midpoint[2]};

  //////////////////No2 -B- FRONT LEFT BOX////////////////////////////////////////
  std::array<double, 3> minpointB = {minpoint[0], minpoint[1], minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointB = {midpoint[0], midpoint[1]+(yw/2), midpoint[2]+(zw/2)};

  //////////////////No3 -C- BACK RIGHT BOX////////////////////////////////////////
  std::array<double, 3> minpointC = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]};
  std::array<double, 3> maxpointC = {maxpoint[0], maxpoint[1], maxpoint[2]-(zw/2)};

  /////////////////No4 -D- FRONT RIGHT BOX////////////////////////////////////////
  std::array<double, 3> minpointD = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointD = maxpoint;

  ////////////////////////////////////////////////////////////////////////////////
  getTrianglesInBoundingBox(minpointA, maxpointA,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexA], yCoordinatesVec[indexA], zCoordinatesVec[indexA]);

  getTrianglesInBoundingBox(minpointB, maxpointB,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexB], yCoordinatesVec[indexB], zCoordinatesVec[indexB]);

  getTrianglesInBoundingBox(minpointC, maxpointC,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexC], yCoordinatesVec[indexC], zCoordinatesVec[indexC]);

  getTrianglesInBoundingBox(minpointD, maxpointD,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexD], yCoordinatesVec[indexD], zCoordinatesVec[indexD]);
  ////////////////////////////////////////////////////////////////////////////////

  centroid[indexA][0] = minpointA[0]+(maxpointA[0]-minpointA[0])/2;
  centroid[indexA][1] = minpointA[1]+(maxpointA[1]-minpointA[1])/2;
  centroid[indexA][2] = minpointA[2]+(maxpointA[2]-minpointA[2])/2;

  centroid[indexB][0] = minpointB[0]+(maxpointB[0]-minpointB[0])/2;
  centroid[indexB][1] = minpointB[1]+(maxpointB[1]-minpointB[1])/2;
  centroid[indexB][2] = minpointB[2]+(maxpointB[2]-minpointB[2])/2;

  centroid[indexC][0] = minpointC[0]+((maxpointC[0]-minpointC[0])/2);
  centroid[indexC][1] = minpointC[1]+((maxpointC[1]-minpointC[1])/2);
  centroid[indexC][2] = minpointC[2]+((maxpointC[2]-minpointC[2])/2);

  centroid[indexD][0] = minpointD[0]+((maxpointD[0]-minpointD[0])/2);
  centroid[indexD][1] = minpointD[1]+((maxpointD[1]-minpointD[1])/2);
  centroid[indexD][2] = minpointD[2]+((maxpointD[2]-minpointD[2])/2);

  /////////////////////////////////////////////////////////////////////////////////////

  double centreA[3], centreB[3], centreC[3], centreD[3];
  centreA[0] = centroid[indexA][0];
  centreA[1] = centroid[indexA][1];
  centreA[2] = centroid[indexA][2];

  centreB[0] = centroid[indexB][0];
  centreB[1] = centroid[indexB][1];
  centreB[2] = centroid[indexB][2];

  centreC[0] = centroid[indexC][0];
  centreC[1] = centroid[indexC][1];
  centreC[2] = centroid[indexC][2];

  centreD[0] = centroid[indexD][0];
  centreD[1] = centroid[indexD][1];
  centreD[2] = centroid[indexD][2];

  delta::primitives::surface::generateBoundBox(centreA, minpointA, maxpointA, xCoordinatesVec[indexA], yCoordinatesVec[indexA], zCoordinatesVec[indexA]);
  delta::primitives::surface::generateBoundBox(centreB, minpointB, maxpointB, xCoordinatesVec[indexB], yCoordinatesVec[indexB], zCoordinatesVec[indexB]);
  delta::primitives::surface::generateBoundBox(centreC, minpointC, maxpointC, xCoordinatesVec[indexC], yCoordinatesVec[indexC], zCoordinatesVec[indexC]);
  delta::primitives::surface::generateBoundBox(centreD, minpointD, maxpointD, xCoordinatesVec[indexD], yCoordinatesVec[indexD], zCoordinatesVec[indexD]);

  index = quadsect(level, index, minpointA, centroid[indexA], maxpointA, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = quadsect(level, index, minpointB, centroid[indexB], maxpointB, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = quadsect(level, index, minpointC, centroid[indexC], maxpointC, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = quadsect(level, index, minpointD, centroid[indexD], maxpointD, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);

  return index;
}

void delta::primitives::triangle::getTrianglesInBoundingBox(std::array<double, 3> minpoint, std::array<double, 3> maxpoint,
    std::vector<double>&  xCoordinatesRoot,
    std::vector<double>&  yCoordinatesRoot,
    std::vector<double>&  zCoordinatesRoot,
    std::vector<double>&  xCoordinatesBounded,
    std::vector<double>&  yCoordinatesBounded,
    std::vector<double>&  zCoordinatesBounded)
{
  for(int i=0; i<xCoordinatesRoot.size(); i+=3)
  {
    double A[3], B[3], C[3];

    A[0] = xCoordinatesRoot[i];
    A[1] = yCoordinatesRoot[i];
    A[2] = zCoordinatesRoot[i];

    B[0] = xCoordinatesRoot[i+1];
    B[1] = yCoordinatesRoot[i+1];
    B[2] = zCoordinatesRoot[i+1];

    C[0] = xCoordinatesRoot[i+2];
    C[1] = yCoordinatesRoot[i+2];
    C[2] = zCoordinatesRoot[i+2];

    double x,y,z;

    x = A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0;
    y = A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0;
    z = A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0;

    //printf("minz:%f, maxz:%f\n", minpoint[2], maxpoint[2]);
    if((x <= maxpoint[0] && x >= minpoint[0]) &&
       (y <= maxpoint[1] && y >= minpoint[1]) &&
       (z <= maxpoint[2] && z >= minpoint[2]))
    {
      xCoordinatesBounded.push_back(xCoordinatesRoot[i]);
      yCoordinatesBounded.push_back(yCoordinatesRoot[i]);
      zCoordinatesBounded.push_back(zCoordinatesRoot[i]);

      xCoordinatesBounded.push_back(xCoordinatesRoot[i+1]);
      yCoordinatesBounded.push_back(yCoordinatesRoot[i+1]);
      zCoordinatesBounded.push_back(zCoordinatesRoot[i+1]);

      xCoordinatesBounded.push_back(xCoordinatesRoot[i+2]);
      yCoordinatesBounded.push_back(yCoordinatesRoot[i+2]);
      zCoordinatesBounded.push_back(zCoordinatesRoot[i+2]);
    }
  }
}
