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

void delta::primitives::triangle::triSectTriangle(
    double A[3],
    double B[3],
    double C[3],
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
  double O[3];

  O[0] = A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0;
  O[1] = A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0;
  O[2] = A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0;

  //////////ONE////////////////
  xCoordinates.push_back(C[0]);
  xCoordinates.push_back(A[0]);
  xCoordinates.push_back(O[0]);

  yCoordinates.push_back(C[1]);
  yCoordinates.push_back(A[1]);
  yCoordinates.push_back(O[1]);

  zCoordinates.push_back(C[2]);
  zCoordinates.push_back(A[2]);
  zCoordinates.push_back(O[2]);

  /////////TWO////////////////
  xCoordinates.push_back(C[0]);
  xCoordinates.push_back(O[0]);
  xCoordinates.push_back(B[0]);

  yCoordinates.push_back(C[1]);
  yCoordinates.push_back(O[1]);
  yCoordinates.push_back(B[1]);

  zCoordinates.push_back(C[2]);
  zCoordinates.push_back(O[2]);
  zCoordinates.push_back(B[2]);

  ////////THREE////////////////
  xCoordinates.push_back(O[0]);
  xCoordinates.push_back(A[0]);
  xCoordinates.push_back(B[0]);

  yCoordinates.push_back(O[1]);
  yCoordinates.push_back(A[1]);
  yCoordinates.push_back(B[1]);

  zCoordinates.push_back(O[2]);
  zCoordinates.push_back(A[2]);
  zCoordinates.push_back(B[2]);
}

void delta::primitives::triangle::fiveSectTriangle(
    double A[3],
    double B[3],
    double C[3],
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
  double O[3];

  O[0] = A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0;
  O[1] = A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0;
  O[2] = A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0;

  double AB[3], CB[3], AC[3];

  AB[0] = (B[0]+A[0])/2;
  AB[1] = (B[1]+A[1])/2;
  AB[2] = (B[2]+A[2])/2;

  CB[0] = (B[0]+C[0])/2;
  CB[1] = (B[1]+C[1])/2;
  CB[2] = (B[2]+C[2])/2;

  AC[0] = (C[0]+A[0])/2;
  AC[1] = (C[1]+A[1])/2;
  AC[2] = (C[2]+A[2])/2;

  ////////ONE/////////////////
  xCoordinates.push_back(AC[0]);
  xCoordinates.push_back(A[0]);
  xCoordinates.push_back(O[0]);

  yCoordinates.push_back(AC[1]);
  yCoordinates.push_back(A[1]);
  yCoordinates.push_back(O[1]);

  zCoordinates.push_back(AC[2]);
  zCoordinates.push_back(A[2]);
  zCoordinates.push_back(O[2]);

  ///////TWO//////////////////
  xCoordinates.push_back(C[0]);
  xCoordinates.push_back(AC[0]);
  xCoordinates.push_back(O[0]);

  yCoordinates.push_back(C[1]);
  yCoordinates.push_back(AC[1]);
  yCoordinates.push_back(O[1]);

  zCoordinates.push_back(C[2]);
  zCoordinates.push_back(AC[2]);
  zCoordinates.push_back(O[2]);

  //////THREE//////////////////////
  xCoordinates.push_back(C[0]);
  xCoordinates.push_back(O[0]);
  xCoordinates.push_back(CB[0]);

  yCoordinates.push_back(C[1]);
  yCoordinates.push_back(O[1]);
  yCoordinates.push_back(CB[1]);

  zCoordinates.push_back(C[2]);
  zCoordinates.push_back(O[2]);
  zCoordinates.push_back(CB[2]);

  //////FOUR/////////////////////

  xCoordinates.push_back(CB[0]);
  xCoordinates.push_back(O[0]);
  xCoordinates.push_back(B[0]);

  yCoordinates.push_back(CB[1]);
  yCoordinates.push_back(O[1]);
  yCoordinates.push_back(B[1]);

  zCoordinates.push_back(CB[2]);
  zCoordinates.push_back(O[2]);
  zCoordinates.push_back(B[2]);

  /////FIVE//////////////////////

  xCoordinates.push_back(O[0]);
  xCoordinates.push_back(AB[0]);
  xCoordinates.push_back(B[0]);

  yCoordinates.push_back(O[1]);
  yCoordinates.push_back(AB[1]);
  yCoordinates.push_back(B[1]);

  zCoordinates.push_back(O[2]);
  zCoordinates.push_back(AB[2]);
  zCoordinates.push_back(B[2]);

  /////SIX//////////////////////
  xCoordinates.push_back(O[0]);
  xCoordinates.push_back(A[0]);
  xCoordinates.push_back(AB[0]);

  yCoordinates.push_back(O[1]);
  yCoordinates.push_back(A[1]);
  yCoordinates.push_back(AB[1]);

  zCoordinates.push_back(O[2]);
  zCoordinates.push_back(A[2]);
  zCoordinates.push_back(AB[2]);
}

void delta::primitives::triangle::meshDenser(
		int multiplier,
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

		fiveSectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
		//triSectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
		//bisectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
	}

	xCoordinates = xNewCoordinates;
	yCoordinates = yNewCoordinates;
	zCoordinates = zNewCoordinates;
	if(multiplier>1) meshDenser(multiplier-1, xCoordinates, yCoordinates, zCoordinates);
}

int delta::primitives::triangle::meshOctSect(int quadsectTimes,
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

  return octsect(quadsectTimes, 0, minpoint, midpoint, maxpoint, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);;
}

int delta::primitives::triangle::octsect(int level, int index, std::array<double, 3> minpoint, std::array<double, 3> midpoint, std::array<double, 3> maxpoint,
    std::vector<std::vector<double>>&  xCoordinatesVec,
    std::vector<std::vector<double>>&  yCoordinatesVec,
    std::vector<std::vector<double>>&  zCoordinatesVec, std::vector<std::array<double, 3>>& centroid)
{

  if(level < 1) return index;
  level--;

  xCoordinatesVec.resize(xCoordinatesVec.size()+8);
  yCoordinatesVec.resize(yCoordinatesVec.size()+8);
  zCoordinatesVec.resize(zCoordinatesVec.size()+8);
  centroid.resize(centroid.size()+8);

  index++;
  int indexA = index;
  index++;
  int indexAA = index;
  index++;
  int indexB = index;
  index++;
  int indexBB = index;
  index++;
  int indexC = index;
  index++;
  int indexCC = index;
  index++;
  int indexD = index;
  index++;
  int indexDD = index;
  ////////////////////////////////////////////////////////////////////////////////

  double xw = maxpoint[0]-minpoint[0];//source/parent width
  double yw = maxpoint[1]-minpoint[1];
  double zw = maxpoint[2]-minpoint[2];

  //////////////////No1 -A- BACK LEFT BOX////////////////////////////////////////
  std::array<double, 3> minpointA = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]};
  std::array<double, 3> maxpointA = {midpoint[0], maxpoint[1], midpoint[2]};

  std::array<double, 3> minpointAA = {minpoint[0], minpoint[1], minpoint[2]};
  std::array<double, 3> maxpointAA = {midpoint[0], maxpoint[1]-(yw/2), midpoint[2]};

  //////////////////No2 -B- FRONT LEFT BOX////////////////////////////////////////
  std::array<double, 3> minpointB = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointB = {midpoint[0], midpoint[1]+(yw/2), midpoint[2]+(zw/2)};

  std::array<double, 3> minpointBB = {minpoint[0], minpoint[1], minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointBB = {midpoint[0], midpoint[1], midpoint[2]+(zw/2)};

  //////////////////No3 -C- BACK RIGHT BOX////////////////////////////////////////
  std::array<double, 3> minpointC = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]};
  std::array<double, 3> maxpointC = {maxpoint[0], maxpoint[1], maxpoint[2]-(zw/2)};

  std::array<double, 3> minpointCC = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]};
  std::array<double, 3> maxpointCC = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]-(zw/2)};

  /////////////////No4 -D- FRONT RIGHT BOX////////////////////////////////////////
  std::array<double, 3> minpointD = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointD = maxpoint;

  std::array<double, 3> minpointDD = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]+(zw/2)};
  std::array<double, 3> maxpointDD = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]};

  ////////////////////////////////////////////////////////////////////////////////
  getTrianglesInBoundingBox(minpointA, maxpointA,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexA], yCoordinatesVec[indexA], zCoordinatesVec[indexA]);

  getTrianglesInBoundingBox(minpointAA, maxpointAA,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexAA], yCoordinatesVec[indexAA], zCoordinatesVec[indexAA]);

  getTrianglesInBoundingBox(minpointB, maxpointB,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexB], yCoordinatesVec[indexB], zCoordinatesVec[indexB]);

  getTrianglesInBoundingBox(minpointBB, maxpointBB,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexBB], yCoordinatesVec[indexBB], zCoordinatesVec[indexBB]);

  getTrianglesInBoundingBox(minpointC, maxpointC,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexC], yCoordinatesVec[indexC], zCoordinatesVec[indexC]);

  getTrianglesInBoundingBox(minpointCC, maxpointCC,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexCC], yCoordinatesVec[indexCC], zCoordinatesVec[indexCC]);

  getTrianglesInBoundingBox(minpointD, maxpointD,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexD], yCoordinatesVec[indexD], zCoordinatesVec[indexD]);

  getTrianglesInBoundingBox(minpointDD, maxpointDD,
                            xCoordinatesVec[0], yCoordinatesVec[0], zCoordinatesVec[0],
                            xCoordinatesVec[indexDD], yCoordinatesVec[indexDD], zCoordinatesVec[indexDD]);
  ////////////////////////////////////////////////////////////////////////////////

  centroid[indexA][0] = minpointA[0]+(maxpointA[0]-minpointA[0])/2;
  centroid[indexA][1] = minpointA[1]+(maxpointA[1]-minpointA[1])/2;
  centroid[indexA][2] = minpointA[2]+(maxpointA[2]-minpointA[2])/2;

  centroid[indexAA][0] = minpointAA[0]+(maxpointAA[0]-minpointAA[0])/2;
  centroid[indexAA][1] = minpointAA[1]+(maxpointAA[1]-minpointAA[1])/2;
  centroid[indexAA][2] = minpointAA[2]+(maxpointAA[2]-minpointAA[2])/2;

  centroid[indexB][0] = minpointB[0]+(maxpointB[0]-minpointB[0])/2;
  centroid[indexB][1] = minpointB[1]+(maxpointB[1]-minpointB[1])/2;
  centroid[indexB][2] = minpointB[2]+(maxpointB[2]-minpointB[2])/2;

  centroid[indexBB][0] = minpointBB[0]+(maxpointBB[0]-minpointBB[0])/2;
  centroid[indexBB][1] = minpointBB[1]+(maxpointBB[1]-minpointBB[1])/2;
  centroid[indexBB][2] = minpointBB[2]+(maxpointBB[2]-minpointBB[2])/2;

  centroid[indexC][0] = minpointC[0]+((maxpointC[0]-minpointC[0])/2);
  centroid[indexC][1] = minpointC[1]+((maxpointC[1]-minpointC[1])/2);
  centroid[indexC][2] = minpointC[2]+((maxpointC[2]-minpointC[2])/2);

  centroid[indexCC][0] = minpointCC[0]+((maxpointCC[0]-minpointCC[0])/2);
  centroid[indexCC][1] = minpointCC[1]+((maxpointCC[1]-minpointCC[1])/2);
  centroid[indexCC][2] = minpointCC[2]+((maxpointCC[2]-minpointCC[2])/2);

  centroid[indexD][0] = minpointD[0]+((maxpointD[0]-minpointD[0])/2);
  centroid[indexD][1] = minpointD[1]+((maxpointD[1]-minpointD[1])/2);
  centroid[indexD][2] = minpointD[2]+((maxpointD[2]-minpointD[2])/2);

  centroid[indexDD][0] = minpointDD[0]+((maxpointDD[0]-minpointDD[0])/2);
  centroid[indexDD][1] = minpointDD[1]+((maxpointDD[1]-minpointDD[1])/2);
  centroid[indexDD][2] = minpointDD[2]+((maxpointDD[2]-minpointDD[2])/2);

  /////////////////////////////////////////////////////////////////////////////////////
/*
  double centreA[3], centreAA[3], centreB[3], centreBB[3], centreC[3], centreCC[3], centreD[3], centreDD[3];

  centreA[0] = centroid[indexA][0];
  centreA[1] = centroid[indexA][1];
  centreA[2] = centroid[indexA][2];

  centreAA[0] = centroid[indexAA][0];
  centreAA[1] = centroid[indexAA][1];
  centreAA[2] = centroid[indexAA][2];

  centreB[0] = centroid[indexB][0];
  centreB[1] = centroid[indexB][1];
  centreB[2] = centroid[indexB][2];

  centreBB[0] = centroid[indexBB][0];
  centreBB[1] = centroid[indexBB][1];
  centreBB[2] = centroid[indexBB][2];

  centreC[0] = centroid[indexC][0];
  centreC[1] = centroid[indexC][1];
  centreC[2] = centroid[indexC][2];

  centreCC[0] = centroid[indexCC][0];
  centreCC[1] = centroid[indexCC][1];
  centreCC[2] = centroid[indexCC][2];

  centreD[0] = centroid[indexD][0];
  centreD[1] = centroid[indexD][1];
  centreD[2] = centroid[indexD][2];

  centreDD[0] = centroid[indexDD][0];
  centreDD[1] = centroid[indexDD][1];
  centreDD[2] = centroid[indexDD][2];

  std::vector<double> xCoordinatesA, yCoordinatesA, zCoordinatesA;
  std::vector<double> xCoordinatesAA, yCoordinatesAA, zCoordinatesAA;
  std::vector<double> xCoordinatesB, yCoordinatesB, zCoordinatesB;
  std::vector<double> xCoordinatesBB, yCoordinatesBB, zCoordinatesBB;
  std::vector<double> xCoordinatesC, yCoordinatesC, zCoordinatesC;
  std::vector<double> xCoordinatesCC, yCoordinatesCC, zCoordinatesCC;
  std::vector<double> xCoordinatesD, yCoordinatesD, zCoordinatesD;
  std::vector<double> xCoordinatesDD, yCoordinatesDD, zCoordinatesDD;

  delta::primitives::surface::generateBoundBox(centreA, minpointA, maxpointA, xCoordinatesA, yCoordinatesA, zCoordinatesA);
  delta::primitives::surface::generateBoundBox(centreAA, minpointAA, maxpointAA, xCoordinatesAA, yCoordinatesAA, zCoordinatesAA);
  delta::primitives::surface::generateBoundBox(centreB, minpointB, maxpointB, xCoordinatesB, yCoordinatesB, zCoordinatesB);
  delta::primitives::surface::generateBoundBox(centreBB, minpointBB, maxpointBB, xCoordinatesBB, yCoordinatesBB, zCoordinatesBB);
  delta::primitives::surface::generateBoundBox(centreC, minpointC, maxpointC, xCoordinatesC, yCoordinatesC, zCoordinatesC);
  delta::primitives::surface::generateBoundBox(centreCC, minpointCC, maxpointCC, xCoordinatesCC, yCoordinatesCC, zCoordinatesCC);
  delta::primitives::surface::generateBoundBox(centreD, minpointD, maxpointD, xCoordinatesD, yCoordinatesD, zCoordinatesD);
  delta::primitives::surface::generateBoundBox(centreDD, minpointDD, maxpointDD, xCoordinatesDD, yCoordinatesDD, zCoordinatesDD);

  for(int i=0; i<xCoordinatesA.size(); i++)
  {
    xCoordinatesVec[indexA].push_back(xCoordinatesA[i]);
    yCoordinatesVec[indexA].push_back(yCoordinatesA[i]);
    zCoordinatesVec[indexA].push_back(zCoordinatesA[i]);

    xCoordinatesVec[indexAA].push_back(xCoordinatesAA[i]);
    yCoordinatesVec[indexAA].push_back(yCoordinatesAA[i]);
    zCoordinatesVec[indexAA].push_back(zCoordinatesAA[i]);

    xCoordinatesVec[indexB].push_back(xCoordinatesB[i]);
    yCoordinatesVec[indexB].push_back(yCoordinatesB[i]);
    zCoordinatesVec[indexB].push_back(zCoordinatesB[i]);

    xCoordinatesVec[indexBB].push_back(xCoordinatesBB[i]);
    yCoordinatesVec[indexBB].push_back(yCoordinatesBB[i]);
    zCoordinatesVec[indexBB].push_back(zCoordinatesBB[i]);

    xCoordinatesVec[indexC].push_back(xCoordinatesC[i]);
    yCoordinatesVec[indexC].push_back(yCoordinatesC[i]);
    zCoordinatesVec[indexC].push_back(zCoordinatesC[i]);

    xCoordinatesVec[indexCC].push_back(xCoordinatesCC[i]);
    yCoordinatesVec[indexCC].push_back(yCoordinatesCC[i]);
    zCoordinatesVec[indexCC].push_back(zCoordinatesCC[i]);

    xCoordinatesVec[indexD].push_back(xCoordinatesD[i]);
    yCoordinatesVec[indexD].push_back(yCoordinatesD[i]);
    zCoordinatesVec[indexD].push_back(zCoordinatesD[i]);

    xCoordinatesVec[indexDD].push_back(xCoordinatesDD[i]);
    yCoordinatesVec[indexDD].push_back(yCoordinatesDD[i]);
    zCoordinatesVec[indexDD].push_back(zCoordinatesDD[i]);
  }*/

  index = octsect(level, index, minpointA, centroid[indexA], maxpointA, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = octsect(level, index, minpointAA, centroid[indexAA], maxpointAA, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = octsect(level, index, minpointB, centroid[indexB], maxpointB, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = octsect(level, index, minpointBB, centroid[indexBB], maxpointBB, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = octsect(level, index, minpointC, centroid[indexC], maxpointC, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = octsect(level, index, minpointCC, centroid[indexCC], maxpointCC, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = octsect(level, index, minpointD, centroid[indexD], maxpointD, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);
  index = octsect(level, index, minpointDD, centroid[indexDD], maxpointDD, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);

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
