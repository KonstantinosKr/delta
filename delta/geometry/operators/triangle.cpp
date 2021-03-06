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

void delta::geometry::operators::triangle::bisectTriangle(
		iREAL A[3],
		iREAL B[3],
		iREAL C[3],
		std::vector<iREAL>&  xCoordinates,
		std::vector<iREAL>&  yCoordinates,
		std::vector<iREAL>&  zCoordinates)
{
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

void delta::geometry::operators::triangle::triSectTriangle(
    iREAL A[3],
    iREAL B[3],
    iREAL C[3],
    std::vector<iREAL>&  xCoordinates,
    std::vector<iREAL>&  yCoordinates,
    std::vector<iREAL>&  zCoordinates)
{
  iREAL O[3];

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

void delta::geometry::operators::triangle::fiveSectTriangle(
    iREAL A[3],
    iREAL B[3],
    iREAL C[3],
    std::vector<iREAL>&  xCoordinates,
    std::vector<iREAL>&  yCoordinates,
    std::vector<iREAL>&  zCoordinates)
{
  iREAL O[3];

  O[0] = A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0;
  O[1] = A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0;
  O[2] = A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0;

  iREAL AB[3], CB[3], AC[3];

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

void delta::geometry::operators::triangle::biSideSectTriangle(
    iREAL A[3],
    iREAL B[3],
    iREAL C[3],
    std::vector<iREAL>&  xCoordinates,
    std::vector<iREAL>&  yCoordinates,
    std::vector<iREAL>&  zCoordinates)
{
  //iREAL O[3] = {A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0,  A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0, A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0};

  iREAL alpha[3], beta[3], gamma[3];
  alpha[0] = (A[0] + B[0])/2.0;
  alpha[1] = (A[1] + B[1])/2.0;
  alpha[2] = (A[2] + B[2])/2.0;

  beta[0] = (A[0] + C[0])/2.0;
  beta[1] = (A[1] + C[1])/2.0;
  beta[2] = (A[2] + C[2])/2.0;

  gamma[0] = (B[0] + C[0])/2.0;
  gamma[1] = (B[1] + C[1])/2.0;
  gamma[2] = (B[2] + C[2])/2.0;

  //////////ONE////////////////
  //beta A alpha
  xCoordinates.push_back(beta[0]);
  xCoordinates.push_back(A[0]);
  xCoordinates.push_back(alpha[0]);

  yCoordinates.push_back(beta[1]);
  yCoordinates.push_back(A[1]);
  yCoordinates.push_back(alpha[1]);

  zCoordinates.push_back(beta[2]);
  zCoordinates.push_back(A[2]);
  zCoordinates.push_back(alpha[2]);

  /////////TWO/////////////////
  //gamma alpha B
  xCoordinates.push_back(gamma[0]);
  xCoordinates.push_back(alpha[0]);
  xCoordinates.push_back(B[0]);

  yCoordinates.push_back(gamma[1]);
  yCoordinates.push_back(alpha[1]);
  yCoordinates.push_back(B[1]);

  zCoordinates.push_back(gamma[2]);
  zCoordinates.push_back(alpha[2]);
  zCoordinates.push_back(B[2]);

  ////////THREE////////////////
  //gamma beta alpha
  xCoordinates.push_back(gamma[0]);
  xCoordinates.push_back(beta[0]);
  xCoordinates.push_back(alpha[0]);

  yCoordinates.push_back(gamma[1]);
  yCoordinates.push_back(beta[1]);
  yCoordinates.push_back(alpha[1]);

  zCoordinates.push_back(gamma[2]);
  zCoordinates.push_back(beta[2]);
  zCoordinates.push_back(alpha[2]);

  ////////FOUR/////////////////
  //C beta gamma
  xCoordinates.push_back(C[0]);
  xCoordinates.push_back(beta[0]);
  xCoordinates.push_back(gamma[0]);

  yCoordinates.push_back(C[1]);
  yCoordinates.push_back(beta[1]);
  yCoordinates.push_back(gamma[1]);

  zCoordinates.push_back(C[2]);
  zCoordinates.push_back(beta[2]);
  zCoordinates.push_back(gamma[2]);
}

void delta::geometry::operators::triangle::meshDenser(
	int meshRefinement,
	iREAL gridH,
	std::vector<iREAL>&  xCoordinates,
	std::vector<iREAL>&  yCoordinates,
	std::vector<iREAL>&  zCoordinates)
{

  if(meshRefinement < 1) return;
	std::vector<iREAL>  xNewCoordinates;
	std::vector<iREAL>  yNewCoordinates;
	std::vector<iREAL>  zNewCoordinates;

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

	  delta::geometry::mesh::Triangle triangle = delta::geometry::mesh::Triangle(A,B,C);

	  if(triangle.getTriangleLength() >= gridH && meshRefinement > 0)
	  {
	    biSideSectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
	  }
	  else
	  {
		xNewCoordinates.push_back(A[0]);
		xNewCoordinates.push_back(B[0]);
		xNewCoordinates.push_back(C[0]);

		yNewCoordinates.push_back(A[1]);
		yNewCoordinates.push_back(B[1]);
		yNewCoordinates.push_back(C[1]);

		zNewCoordinates.push_back(A[2]);
		zNewCoordinates.push_back(B[2]);
		zNewCoordinates.push_back(C[2]);
	  }
	}

	xCoordinates = xNewCoordinates;
	yCoordinates = yNewCoordinates;
	zCoordinates = zNewCoordinates;
	delta::geometry::operators::triangle::meshDenser(meshRefinement-1, gridH, xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::operators::triangle::meshDenser(
    int meshRefinement,
    std::vector<iREAL>&  xCoordinates,
    std::vector<iREAL>&  yCoordinates,
    std::vector<iREAL>&  zCoordinates)
{
  if(meshRefinement < 1) return;
  std::vector<iREAL>  xNewCoordinates;
  std::vector<iREAL>  yNewCoordinates;
  std::vector<iREAL>  zNewCoordinates;

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

    biSideSectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
    //triSectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
  }

  xCoordinates = xNewCoordinates;
  yCoordinates = yNewCoordinates;
  zCoordinates = zNewCoordinates;
  meshDenser(meshRefinement-1, xCoordinates, yCoordinates, zCoordinates);
}

int delta::geometry::operators::triangle::octSectParticle(
    int octSectTimes,
    std::vector<std::vector<iREAL>>&  xCoordinatesMultiLevel,
    std::vector<std::vector<iREAL>>&  yCoordinatesMultiLevel,
    std::vector<std::vector<iREAL>>&  zCoordinatesMultiLevel,
	std::vector<std::array<iREAL, 3>>& centroid)
{
  std::array<iREAL, 3> minpoint = delta::geometry::mesh::Mesh(xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0]).computeBoundaryMinVertex();
  std::array<iREAL, 3> maxpoint = delta::geometry::mesh::Mesh(xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0]).computeBoundaryMaxVertex();

  iREAL xw = maxpoint[0]-minpoint[0]; iREAL yw = maxpoint[1]-minpoint[1]; iREAL zw = maxpoint[2]-minpoint[2];

  ///////////VISUALISATION OF ROOT OCT-section CUBE///////////////////////
  /////////////////////////////////////////////////////////////////////////
  /*std::vector<iREAL> xCoordinatesBox, yCoordinatesBox, zCoordinatesBox;
  iREAL centre[3] = {midpoint[0], midpoint[1], midpoint[2]};
  delta::geometry::surface::generateBoundBox(centre, minpoint, maxpoint, xCoordinatesBox, yCoordinatesBox, zCoordinatesBox);

  for(int i=0; i<xCoordinatesBox.size(); i++)
  {
    xCoordinatesVec[0].push_back(xCoordinatesBox[i]);
    yCoordinatesVec[0].push_back(yCoordinatesBox[i]);
    zCoordinatesVec[0].push_back(zCoordinatesBox[i]);
  }*/
  ///////////END VISUALISATION/////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  return octSect(octSectTimes, 0, minpoint, maxpoint, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);;
}

int delta::geometry::operators::triangle::octSect(
    int targetLevelOfRefinement,
    int index,
    std::array<iREAL, 3> minpoint,
    std::array<iREAL, 3> maxpoint,
    std::vector<std::vector<iREAL>>&  xCoordinatesMultiLevel,
    std::vector<std::vector<iREAL>>&  yCoordinatesMultiLevel,
    std::vector<std::vector<iREAL>>&  zCoordinatesMultiLevel)
{

  if(targetLevelOfRefinement < 1) return index;
  targetLevelOfRefinement--;

  xCoordinatesMultiLevel.resize(xCoordinatesMultiLevel.size()+8);
  yCoordinatesMultiLevel.resize(yCoordinatesMultiLevel.size()+8);
  zCoordinatesMultiLevel.resize(zCoordinatesMultiLevel.size()+8);

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

  //compute coarse level dimensions widths
  iREAL xw = maxpoint[0]-minpoint[0];//source/parent width
  iREAL yw = maxpoint[1]-minpoint[1];
  iREAL zw = maxpoint[2]-minpoint[2];

  std::array<iREAL, 3> midpoint = {minpoint[0]+xw/2, minpoint[1]+yw/2, minpoint[2]+zw/2};

  //get fine level bounding boxes
  //////////////////No1 -A- BACK LEFT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointA = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]};
  std::array<iREAL, 3> maxpointA = {midpoint[0], maxpoint[1], midpoint[2]};

  std::array<iREAL, 3> minpointAA = {minpoint[0], minpoint[1], minpoint[2]};
  std::array<iREAL, 3> maxpointAA = {midpoint[0], maxpoint[1]-(yw/2), midpoint[2]};

  //get fine level bounding boxes
  //////////////////No2 -B- FRONT LEFT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointB = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointB = {midpoint[0], midpoint[1]+(yw/2), midpoint[2]+(zw/2)};

  std::array<iREAL, 3> minpointBB = {minpoint[0], minpoint[1], minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointBB = {midpoint[0], midpoint[1], midpoint[2]+(zw/2)};

  //get fine level bounding boxes
  //////////////////No3 -C- BACK RIGHT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointC = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]};
  std::array<iREAL, 3> maxpointC = {maxpoint[0], maxpoint[1], maxpoint[2]-(zw/2)};

  std::array<iREAL, 3> minpointCC = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]};
  std::array<iREAL, 3> maxpointCC = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]-(zw/2)};

  //get fine level bounding boxes
  /////////////////No4 -D- FRONT RIGHT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointD = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointD = maxpoint;

  std::array<iREAL, 3> minpointDD = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointDD = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]};
  ////////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////////
  getTrianglesInBoundingBox(minpointA, maxpointA,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexA], yCoordinatesMultiLevel[indexA], zCoordinatesMultiLevel[indexA]);

  getTrianglesInBoundingBox(minpointAA, maxpointAA,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexAA], yCoordinatesMultiLevel[indexAA], zCoordinatesMultiLevel[indexAA]);

  getTrianglesInBoundingBox(minpointB, maxpointB,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexB], yCoordinatesMultiLevel[indexB], zCoordinatesMultiLevel[indexB]);

  getTrianglesInBoundingBox(minpointBB, maxpointBB,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexBB], yCoordinatesMultiLevel[indexBB], zCoordinatesMultiLevel[indexBB]);

  getTrianglesInBoundingBox(minpointC, maxpointC,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexC], yCoordinatesMultiLevel[indexC], zCoordinatesMultiLevel[indexC]);

  getTrianglesInBoundingBox(minpointCC, maxpointCC,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexCC], yCoordinatesMultiLevel[indexCC], zCoordinatesMultiLevel[indexCC]);

  getTrianglesInBoundingBox(minpointD, maxpointD,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexD], yCoordinatesMultiLevel[indexD], zCoordinatesMultiLevel[indexD]);

  getTrianglesInBoundingBox(minpointDD, maxpointDD,
                            xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0],
                            xCoordinatesMultiLevel[indexDD], yCoordinatesMultiLevel[indexDD], zCoordinatesMultiLevel[indexDD]);


  //////////////BOUNDING BOX VISUALISATION///////////////////////////////////////////////
  /*
  iREAL centreA[3], centreAA[3], centreB[3], centreBB[3], centreC[3], centreCC[3], centreD[3], centreDD[3];

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

  std::vector<iREAL> xCoordinatesA, yCoordinatesA, zCoordinatesA;
  std::vector<iREAL> xCoordinatesAA, yCoordinatesAA, zCoordinatesAA;
  std::vector<iREAL> xCoordinatesB, yCoordinatesB, zCoordinatesB;
  std::vector<iREAL> xCoordinatesBB, yCoordinatesBB, zCoordinatesBB;
  std::vector<iREAL> xCoordinatesC, yCoordinatesC, zCoordinatesC;
  std::vector<iREAL> xCoordinatesCC, yCoordinatesCC, zCoordinatesCC;
  std::vector<iREAL> xCoordinatesD, yCoordinatesD, zCoordinatesD;
  std::vector<iREAL> xCoordinatesDD, yCoordinatesDD, zCoordinatesDD;

  delta::geometry::surface::generateBoundBox(centreA, minpointA, maxpointA, xCoordinatesA, yCoordinatesA, zCoordinatesA);
  delta::geometry::surface::generateBoundBox(centreAA, minpointAA, maxpointAA, xCoordinatesAA, yCoordinatesAA, zCoordinatesAA);
  delta::geometry::surface::generateBoundBox(centreB, minpointB, maxpointB, xCoordinatesB, yCoordinatesB, zCoordinatesB);
  delta::geometry::surface::generateBoundBox(centreBB, minpointBB, maxpointBB, xCoordinatesBB, yCoordinatesBB, zCoordinatesBB);
  delta::geometry::surface::generateBoundBox(centreC, minpointC, maxpointC, xCoordinatesC, yCoordinatesC, zCoordinatesC);
  delta::geometry::surface::generateBoundBox(centreCC, minpointCC, maxpointCC, xCoordinatesCC, yCoordinatesCC, zCoordinatesCC);
  delta::geometry::surface::generateBoundBox(centreD, minpointD, maxpointD, xCoordinatesD, yCoordinatesD, zCoordinatesD);
  delta::geometry::surface::generateBoundBox(centreDD, minpointDD, maxpointDD, xCoordinatesDD, yCoordinatesDD, zCoordinatesDD);


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
  //////////////END BOUNDING BOX VISUALISATION///////////////////////////////////////////////


  index = octSect(targetLevelOfRefinement, index, minpointA, maxpointA, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);
  index = octSect(targetLevelOfRefinement, index, minpointAA, maxpointAA, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);
  index = octSect(targetLevelOfRefinement, index, minpointB, maxpointB, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);
  index = octSect(targetLevelOfRefinement, index, minpointBB, maxpointBB, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);
  index = octSect(targetLevelOfRefinement, index, minpointC, maxpointC, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);
  index = octSect(targetLevelOfRefinement, index, minpointCC, maxpointCC, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);
  index = octSect(targetLevelOfRefinement, index, minpointD, maxpointD, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);
  index = octSect(targetLevelOfRefinement, index, minpointDD, maxpointDD, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel);

  return index;
}

void delta::geometry::operators::triangle::getTrianglesInBoundingBox(
    std::array<iREAL, 3> minpoint,
    std::array<iREAL, 3> maxpoint,
    std::vector<iREAL>&  xCoordinatesRoot,
    std::vector<iREAL>&  yCoordinatesRoot,
    std::vector<iREAL>&  zCoordinatesRoot,
    std::vector<iREAL>&  xCoordinatesBounded,
    std::vector<iREAL>&  yCoordinatesBounded,
    std::vector<iREAL>&  zCoordinatesBounded)
{
  for(unsigned i=0; i<xCoordinatesRoot.size(); i+=3)
  {
    iREAL A[3], B[3], C[3];

    A[0] = xCoordinatesRoot[i];
    A[1] = yCoordinatesRoot[i];
    A[2] = zCoordinatesRoot[i];

    B[0] = xCoordinatesRoot[i+1];
    B[1] = yCoordinatesRoot[i+1];
    B[2] = zCoordinatesRoot[i+1];

    C[0] = xCoordinatesRoot[i+2];
    C[1] = yCoordinatesRoot[i+2];
    C[2] = zCoordinatesRoot[i+2];

    //triangle middle point
    iREAL x = A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0;
    iREAL y = A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0;
    iREAL z = A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0;

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

void delta::geometry::operators::triangle::decomposeMeshByOctsectionIntoGroupsOfTriangleParticles(
    int 												octSectTimes,
    delta::geometry::mesh::Mesh 						mesh,
    delta::geometry::material::MaterialType 			material,
    bool 											isFriction,
    bool 											isObstacle,
	iREAL 											epsilon,
    std::vector<delta::world::structure::Object>&		fineObjects,
	int&												numberOfParticles,
	int&												numberOfObstacles)
{
  std::vector<std::array<iREAL, 3>> centroid;

  iREAL centerOfMass[3];
  iREAL inertia[9];
  iREAL inverse[9];
  iREAL mass;

  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  mesh.flatten(xCoordinates, yCoordinates, zCoordinates);

  if(octSectTimes && xCoordinates.size() > 0)
  {
    std::vector<std::vector<iREAL>> xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel;

    xCoordinatesMultiLevel.resize(1); yCoordinatesMultiLevel.resize(1); zCoordinatesMultiLevel.resize(1);

    mesh.computeInertia(material, mass, centerOfMass, inertia);
    mesh.computeInverseInertia(inertia, inverse, isObstacle);

    centroid.resize(1);
    xCoordinatesMultiLevel.resize(1); yCoordinatesMultiLevel.resize(1); zCoordinatesMultiLevel.resize(1);

    centroid[0][0] = centerOfMass[0];
    centroid[0][1] = centerOfMass[1];
    centroid[0][2] = centerOfMass[2];


    for(unsigned i=0; i<xCoordinates.size(); i++)
    {//coarse triangle push
      xCoordinatesMultiLevel[0].push_back(xCoordinates[i]);
      yCoordinatesMultiLevel[0].push_back(yCoordinates[i]);
      zCoordinatesMultiLevel[0].push_back(zCoordinates[i]);
    }
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    int numOfSubParticles = delta::geometry::operators::triangle::octSectParticle(octSectTimes, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);

    ////////LOOP ALL subdivisions of quadtree and create particles
    for(unsigned i=numOfSubParticles; i>(octSectTimes-1)*8; i--)
    {
      for(unsigned j=0; j<xCoordinatesMultiLevel[i].size(); j++)
      {
        xCoordinates.push_back(xCoordinatesMultiLevel[i][j]);
        yCoordinates.push_back(yCoordinatesMultiLevel[i][j]);
        zCoordinates.push_back(zCoordinatesMultiLevel[i][j]);
      }

      delta::world::structure::Object obj(
    	  "mesh", 0, centroid[0], material, isObstacle, isFriction, true, epsilon, {0,0,0}, {0,0,0});

      obj.setMesh(
          {xCoordinates[0], xCoordinates[1], xCoordinates[2]},
          {yCoordinates[0], yCoordinates[1], yCoordinates[2]},
          {zCoordinates[0], zCoordinates[1], zCoordinates[2]});

      obj.setMass(mass);
      obj.setInertia(inertia);
      obj.setInverse(inverse);
      obj.setCentreOfMass(centerOfMass);

      fineObjects.push_back(obj);

      //_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
      xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
    }
    ////////END LOOP
  } else {
    //delta::world::object::Object::Object obj("mesh", 0, centerOfMass, material, isObstacle, isFriction);
	delta::world::structure::Object obj(
		"mesh", 0, centroid[0], material, isObstacle, isFriction, true, epsilon, {0,0,0}, {0,0,0});

    obj.setMesh(
        {xCoordinates[0], xCoordinates[1], xCoordinates[2]},
        {yCoordinates[0], yCoordinates[1], yCoordinates[2]},
        {zCoordinates[0], zCoordinates[1], zCoordinates[2]});

    obj.setMass(mass);
    obj.setInertia(inertia);
    obj.setInverse(inverse);
    obj.setCentreOfMass(centerOfMass);

    fineObjects.push_back(obj);

    //_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
  }
  numberOfParticles++;
  if(isObstacle) numberOfObstacles++;
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
}

int delta::geometry::operators::triangle::decomposeMeshIntoTriangleParticles(
    delta::geometry::mesh::Mesh 						mesh,
    delta::geometry::material::MaterialType 			material,
    bool 											isObstacle,
    bool 											isFriction,
	iREAL 											epsilon,
    std::vector<delta::world::structure::Object>&		fineObjects
)
{
  iREAL mass;
  iREAL centerOfMass[3];
  iREAL inertia[9];
  iREAL inverse[9];

  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  mesh.flatten(xCoordinates, yCoordinates, zCoordinates);

  mesh.computeInertia(material, mass, centerOfMass, inertia);
  mesh.computeInverseInertia(inertia, inverse, isObstacle);

  for(unsigned i=0; i<xCoordinates.size(); i+=3)
  {
    iREAL A[3] = {xCoordinates[i], yCoordinates[i], zCoordinates[i]};
    iREAL B[3] = {xCoordinates[i+1], yCoordinates[i+1], zCoordinates[i+1]};
    iREAL C[3] = {xCoordinates[i+2], yCoordinates[i+2], zCoordinates[i+2]};

    iREAL O[3] = {A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0,  A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0, A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0};

    std::vector<iREAL> subxCoordinates;
    std::vector<iREAL> subyCoordinates;
    std::vector<iREAL> subzCoordinates;

    subxCoordinates.push_back(A[0]);
    subxCoordinates.push_back(B[0]);
    subxCoordinates.push_back(C[0]);

    subyCoordinates.push_back(A[1]);
    subyCoordinates.push_back(B[1]);
    subyCoordinates.push_back(C[1]);

    subzCoordinates.push_back(A[2]);
    subzCoordinates.push_back(B[2]);
    subzCoordinates.push_back(C[2]);

    std::array<iREAL, 3> Oarray = {O[0], O[1], O[2]};

    delta::world::structure::Object obj(
    	"mesh", 0, Oarray, material, isObstacle, isFriction, true, epsilon, {0,0,0}, {0,0,0});

    obj.setMesh(
        {subxCoordinates[0], subxCoordinates[1], subxCoordinates[2]},
        {subyCoordinates[0], subyCoordinates[1], subyCoordinates[2]},
        {subzCoordinates[0], subzCoordinates[1], subzCoordinates[2]});

    obj.setMass(mass);
    obj.setInertia(inertia);
    obj.setInverse(inverse);
    obj.setCentreOfMass(centerOfMass);

    fineObjects.push_back(obj);
  }

  return xCoordinates.size()/3;
}
