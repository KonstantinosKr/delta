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

  quadsect(minpoint, midpoint, maxpoint, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);

  return 4*1;
}

void delta::primitives::triangle::quadsect(std::array<double, 3> minpoint, std::array<double, 3> midpoint, std::array<double, 3> maxpoint,
    std::vector<std::vector<double>>&  xCoordinatesVec,
    std::vector<std::vector<double>>&  yCoordinatesVec,
    std::vector<std::vector<double>>&  zCoordinatesVec, std::vector<std::array<double, 3>>& centroid)
{
  double centre[3];
  //std::vector<double> xCoordinatesBox, yCoordinatesBox, zCoordinatesBox;

  double xw = maxpoint[0]-minpoint[0];//source/parent width
  double yw = maxpoint[1]-minpoint[1];
  double zw = maxpoint[2]-minpoint[2];

  std::vector<double> xCoordinatesBox, yCoordinatesBox, zCoordinatesBox;

  //////////////////No1 -A- BACK LEFT BOX
  std::array<double, 3> minpointA = minpoint;
  std::array<double, 3> maxpointA = {midpoint[0], maxpoint[1], midpoint[1]};

  double xwA = maxpointA[0]-minpointA[0];
  double ywA = maxpointA[1]-minpointA[1];
  double zwA = maxpointA[2]-minpointA[2];

  std::array<double, 3> midpointA = {minpointA[0]+xwA/2, minpointA[1]+ywA/2, minpointA[2]+zwA/2};

  xCoordinatesVec.resize(xCoordinatesVec.size()+1);
  yCoordinatesVec.resize(yCoordinatesVec.size()+1);
  zCoordinatesVec.resize(zCoordinatesVec.size()+1);
  centroid.resize(centroid.size()+1);

  int index = xCoordinatesVec.size()-1;

  centroid[index][0] = midpointA[0];
  centroid[index][1] = midpointA[1];
  centroid[index][2] = midpointA[2];

  getTrianglesInBoundingBox(minpointA, maxpointA,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[index], yCoordinatesVec[index], zCoordinatesVec[index]);

  /*centre[0] = midpointA[0];
  centre[1] = midpointA[1];
  centre[2] = midpointA[2];
  delta::primitives::surface::generateBoundBox(centre, minpointA, maxpointA, xCoordinatesBox, yCoordinatesBox, zCoordinatesBox);

  for(int i=0; i<xCoordinatesBox.size(); i++)
  {
    xCoordinatesVec[index].push_back(xCoordinatesBox[i]);
    yCoordinatesVec[index].push_back(yCoordinatesBox[i]);
    zCoordinatesVec[index].push_back(zCoordinatesBox[i]);
  }*/

  //////////////////No2 -B- FRONT LEFT BOX
  std::array<double, 3> minpointB = {minpoint[0], minpoint[1], minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointB = {midpoint[0], midpoint[1]+(yw/2), midpoint[2]+(zw/2)};

  double xwB = maxpointB[0]-minpointB[0];
  double ywB = maxpointB[1]-minpointB[1];
  double zwB = maxpointB[2]-minpointB[2];

  std::array<double, 3> midpointB = {minpointB[0]+xwB/2, minpointB[1]+ywB/2, minpointB[2]+zwB/2};

  xCoordinatesVec.resize(xCoordinatesVec.size()+1);
  yCoordinatesVec.resize(yCoordinatesVec.size()+1);
  zCoordinatesVec.resize(zCoordinatesVec.size()+1);
  centroid.resize(centroid.size()+1);

  index = xCoordinatesVec.size()-1;

  centroid[index][0] = midpointB[0];
  centroid[index][1] = midpointB[1];
  centroid[index][2] = midpointB[2];

  getTrianglesInBoundingBox(minpointB, maxpointB,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[index], yCoordinatesVec[index], zCoordinatesVec[index]);

  /*
  centre[0] = midpointB[0];
  centre[1] = midpointB[1];
  centre[2] = midpointB[2];
  delta::primitives::surface::generateBoundBox(centre, minpointB, maxpointB, xCoordinatesBox, yCoordinatesBox, zCoordinatesBox);

  for(int i=0; i<xCoordinatesBox.size(); i++)
  {
    xCoordinatesVec[index].push_back(xCoordinatesBox[i]);
    yCoordinatesVec[index].push_back(yCoordinatesBox[i]);
    zCoordinatesVec[index].push_back(zCoordinatesBox[i]);
  }*/

  //////////////////No3 -C- BACK RIGHT BOX
  std::array<double, 3> minpointC = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]};
  std::array<double, 3> maxpointC = {maxpoint[0], maxpoint[1], maxpoint[2]-(zw/2)};

  double xwC = maxpointC[0]-minpointC[0];
  double ywC = maxpointC[1]-minpointC[1];
  double zwC = maxpointC[2]-minpointC[2];

  std::array<double, 3> midpointC = {minpointC[0]+xwC/2, minpointC[1]+(ywC/2), minpointC[2]+(zwC/2)};

  xCoordinatesVec.resize(xCoordinatesVec.size()+1);
  yCoordinatesVec.resize(yCoordinatesVec.size()+1);
  zCoordinatesVec.resize(zCoordinatesVec.size()+1);
  centroid.resize(centroid.size()+1);

  index = xCoordinatesVec.size()-1;

  centroid[index][0] = midpointC[0];
  centroid[index][1] = midpointC[1];
  centroid[index][2] = midpointC[2];

  getTrianglesInBoundingBox(minpointC, maxpointC,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[index], yCoordinatesVec[index], zCoordinatesVec[index]);

  /*centre[0] = midpointC[0];
  centre[1] = midpointC[1];
  centre[2] = midpointC[2];
  delta::primitives::surface::generateBoundBox(centre, minpointC, maxpointC, xCoordinatesBox, yCoordinatesBox, zCoordinatesBox);

  for(int i=0; i<xCoordinatesBox.size(); i++)
  {
    xCoordinatesVec[index].push_back(xCoordinatesBox[i]);
    yCoordinatesVec[index].push_back(yCoordinatesBox[i]);
    zCoordinatesVec[index].push_back(zCoordinatesBox[i]);
  }*/

  /////////////////No4 -D- FRONT RIGHT BOX
  std::array<double, 3> minpointD = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointD = maxpoint;

  double xwD = maxpointD[0]-minpointD[0];
  double ywD = maxpointD[1]-minpointD[1];
  double zwD = maxpointD[2]-minpointD[2];

  std::array<double, 3> midpointD = {minpointD[0]+(xwD/2), minpointD[1]+(ywD/2), minpointD[2]+(zwD/2)};

  xCoordinatesVec.resize(xCoordinatesVec.size()+1);
  yCoordinatesVec.resize(yCoordinatesVec.size()+1);
  zCoordinatesVec.resize(zCoordinatesVec.size()+1);
  centroid.resize(centroid.size()+1);

  index = xCoordinatesVec.size()-1;

  centroid[index][0] = midpointD[0];
  centroid[index][1] = midpointD[1];
  centroid[index][2] = midpointD[2];

  getTrianglesInBoundingBox(minpointD, maxpointD,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[index], yCoordinatesVec[index], zCoordinatesVec[index]);

  /*centre[0] = midpointD[0];
  centre[1] = midpointD[1];
  centre[2] = midpointD[2];
  delta::primitives::surface::generateBoundBox(centre, minpointD, maxpointD, xCoordinatesBox, yCoordinatesBox, zCoordinatesBox);

  for(int i=0; i<xCoordinatesBox.size(); i++)
  {
    xCoordinatesVec[index].push_back(xCoordinatesBox[i]);
    yCoordinatesVec[index].push_back(yCoordinatesBox[i]);
    zCoordinatesVec[index].push_back(zCoordinatesBox[i]);
  }*/
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
