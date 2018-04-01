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

#include <delta/geometry/operators/triangle.h>

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
		double gridH,
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

		if(getTriangleLength(A,B,C) >= gridH && meshRefinement > 0) {
	    biSideSectTriangle(A, B, C, xNewCoordinates, yNewCoordinates, zNewCoordinates);
		} else
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
	meshDenser(meshRefinement-1, gridH, xCoordinates, yCoordinates, zCoordinates);
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
    std::vector<std::vector<iREAL>>&  zCoordinatesMultiLevel, std::vector<std::array<iREAL, 3>>& centroid)
{
  std::array<iREAL, 3> minpoint = delta::geometry::properties::getMinBoundaryVertex(xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0]);
  std::array<iREAL, 3> maxpoint = delta::geometry::properties::getMaxBoundaryVertex(xCoordinatesMultiLevel[0], yCoordinatesMultiLevel[0], zCoordinatesMultiLevel[0]);

  iREAL xw = maxpoint[0]-minpoint[0]; iREAL yw = maxpoint[1]-minpoint[1]; iREAL zw = maxpoint[2]-minpoint[2];
  std::array<iREAL, 3> midpoint = {minpoint[0]+xw/2, minpoint[1]+yw/2, minpoint[2]+zw/2};

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

  return octSect(octSectTimes, 0, minpoint, midpoint, maxpoint, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);;
}

int delta::geometry::operators::triangle::octSect(
    int level,
    int index,
    std::array<iREAL, 3> minpoint,
    std::array<iREAL, 3> midpoint,
    std::array<iREAL, 3> maxpoint,
    std::vector<std::vector<iREAL>>&  xCoordinatesMultiLevel,
    std::vector<std::vector<iREAL>>&  yCoordinatesMultiLevel,
    std::vector<std::vector<iREAL>>&  zCoordinatesMultiLevel,
    std::vector<std::array<iREAL, 3>>& centroid)
{

  if(level < 1) return index;
  level--;

  xCoordinatesMultiLevel.resize(xCoordinatesMultiLevel.size()+8);
  yCoordinatesMultiLevel.resize(yCoordinatesMultiLevel.size()+8);
  zCoordinatesMultiLevel.resize(zCoordinatesMultiLevel.size()+8);
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

  iREAL xw = maxpoint[0]-minpoint[0];//source/parent width
  iREAL yw = maxpoint[1]-minpoint[1];
  iREAL zw = maxpoint[2]-minpoint[2];

  //////////////////No1 -A- BACK LEFT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointA = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]};
  std::array<iREAL, 3> maxpointA = {midpoint[0], maxpoint[1], midpoint[2]};

  std::array<iREAL, 3> minpointAA = {minpoint[0], minpoint[1], minpoint[2]};
  std::array<iREAL, 3> maxpointAA = {midpoint[0], maxpoint[1]-(yw/2), midpoint[2]};

  //////////////////No2 -B- FRONT LEFT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointB = {minpoint[0], minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointB = {midpoint[0], midpoint[1]+(yw/2), midpoint[2]+(zw/2)};

  std::array<iREAL, 3> minpointBB = {minpoint[0], minpoint[1], minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointBB = {midpoint[0], midpoint[1], midpoint[2]+(zw/2)};

  //////////////////No3 -C- BACK RIGHT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointC = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]};
  std::array<iREAL, 3> maxpointC = {maxpoint[0], maxpoint[1], maxpoint[2]-(zw/2)};

  std::array<iREAL, 3> minpointCC = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]};
  std::array<iREAL, 3> maxpointCC = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]-(zw/2)};

  /////////////////No4 -D- FRONT RIGHT BOX////////////////////////////////////////
  std::array<iREAL, 3> minpointD = {minpoint[0]+(xw/2), minpoint[1]+(yw/2), minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointD = maxpoint;

  std::array<iREAL, 3> minpointDD = {minpoint[0]+(xw/2), minpoint[1], minpoint[2]+(zw/2)};
  std::array<iREAL, 3> maxpointDD = {maxpoint[0], maxpoint[1]-(yw/2), maxpoint[2]};

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


  index = octSect(level, index, minpointA, centroid[indexA], maxpointA, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);
  index = octSect(level, index, minpointAA, centroid[indexAA], maxpointAA, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);
  index = octSect(level, index, minpointB, centroid[indexB], maxpointB, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);
  index = octSect(level, index, minpointBB, centroid[indexBB], maxpointBB, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);
  index = octSect(level, index, minpointC, centroid[indexC], maxpointC, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);
  index = octSect(level, index, minpointCC, centroid[indexCC], maxpointCC, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);
  index = octSect(level, index, minpointD, centroid[indexD], maxpointD, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);
  index = octSect(level, index, minpointDD, centroid[indexDD], maxpointDD, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);

  return index;
}

double delta::geometry::operators::triangle::getTriangleLength(iREAL A[3], iREAL B[3], iREAL C[3])
{
  std::vector<iREAL>  xCoordinates;
  std::vector<iREAL>  yCoordinates;
  std::vector<iREAL>  zCoordinates;

  xCoordinates.push_back(A[0]);
  xCoordinates.push_back(B[0]);
  xCoordinates.push_back(C[0]);

  yCoordinates.push_back(A[1]);
  yCoordinates.push_back(B[1]);
  yCoordinates.push_back(C[1]);

  zCoordinates.push_back(A[2]);
  zCoordinates.push_back(B[2]);
  zCoordinates.push_back(C[2]);

  return delta::geometry::properties::getXYZWidth(xCoordinates, yCoordinates, zCoordinates);
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

    iREAL x,y,z;

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
