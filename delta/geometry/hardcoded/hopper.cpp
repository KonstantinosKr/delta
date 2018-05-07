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

#include <delta/geometry/hardcoded/hopper.h>

void delta::geometry::hardcoded::generateInnerHopper(
		iREAL  center[3],
		iREAL 	width,
		iREAL  height,
		iREAL 	hatch,
		std::vector<iREAL>&  xCoordinates,
		std::vector<iREAL>&  yCoordinates,
		std::vector<iREAL>&  zCoordinates
)
{
	const int numberOfTriangles = 12;

	xCoordinates.resize( numberOfTriangles*3 );
	yCoordinates.resize( numberOfTriangles*3 );
	zCoordinates.resize( numberOfTriangles*3 );

	//FRONT HOPPER FACE
	iREAL A[3], B[3], C[3], D[3], E[3];
	//A->D - **UPPER LEFT FRONT POINT** ID: 0
	//B->D - **LOWER LEFT FRONT POINT** ID: 1
	A[0] = center[0] - width/2;
	A[1] = center[1] + height/2;
	A[2] = center[2] + width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - height/2; // lower hatch front face
	B[2] = center[2] + hatch/2; //

	C[0] = center[0] + hatch/2; //
	C[1] = B[1]; 				// lower hatch front face
	C[2] = center[2] + hatch/2; //

	D[0] = center[0] + width/2;
	D[1] = A[1];
	D[2] = A[2];

	E[0] = C[0] - hatch/2;//
	E[1] = A[1];//upper hatch front face
	E[2] = A[2];//

	//CREATE 3 TRIANGLES FOR FRONT FACE
	//T1A
	xCoordinates[0] = A[0];
	yCoordinates[0] = A[1];
	zCoordinates[0] = A[2];
	//T1B
	xCoordinates[1] = B[0];
	yCoordinates[1] = B[1];
	zCoordinates[1] = B[2];
	//T1C
	xCoordinates[2] = E[0];
	yCoordinates[2] = E[1];
	zCoordinates[2] = E[2];

	//T2A
	xCoordinates[3] = B[0];
	yCoordinates[3] = B[1];
	zCoordinates[3] = B[2];
	//T2B
	xCoordinates[4] = C[0];
	yCoordinates[4] = C[1];
	zCoordinates[4] = C[2];
	//T2C
	xCoordinates[5] = E[0];
	yCoordinates[5] = E[1];
	zCoordinates[5] = E[2];

	//T3A
	xCoordinates[6] = D[0];
	yCoordinates[6] = D[1];
	zCoordinates[6] = D[2];
	//T3B
	xCoordinates[7] = E[0];
	yCoordinates[7] = E[1];
	zCoordinates[7] = E[2];
	//T3C
	xCoordinates[8] = C[0];
	yCoordinates[8] = C[1];
	zCoordinates[8] = C[2];

	////////////////////////////////////////////////////////////
	//FRONT BACK FACE
	//A->A - **UPPER LEFT BACK POINT** ID:10
	//B->A - **LOWER LEFT BACK POINT** ID:9
	A[0] = center[0] - width/2;
	A[1] = center[1] + height/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - height/2; // lower hatch back face
	B[2] = center[2] - hatch/2; //

	C[0] = center[0] + hatch/2; //
	C[1] = center[1] - height/2; 				// lower hatch back face
	C[2] = center[2] - hatch/2; //

	D[0] = center[0] + width/2;
	D[1] = center[1] + height/2;
	D[2] = center[2] - width/2;

	E[0] = C[0] - hatch/2; //
	E[1] = A[1];//upper hatch back face
	E[2] = A[2];//

	//CREATE 3 TRIANGLES FOR BACK FACE
	//T1A
	xCoordinates[9] = B[0];
	yCoordinates[9] = B[1];
	zCoordinates[9] = B[2];
	//T1B
	xCoordinates[10] = A[0];
	yCoordinates[10] = A[1];
	zCoordinates[10] = A[2];
	//T1C
	xCoordinates[11] = E[0];
	yCoordinates[11] = E[1];
	zCoordinates[11] = E[2];

	//T2A
	xCoordinates[12] = C[0];
	yCoordinates[12] = C[1];
	zCoordinates[12] = C[2];
	//T2B
	xCoordinates[13] = B[0];
	yCoordinates[13] = B[1];
	zCoordinates[13] = B[2];
	//T2C
	xCoordinates[14] = E[0];
	yCoordinates[14] = E[1];
	zCoordinates[14] = E[2];

	//T3A
	xCoordinates[15] = C[0];
	yCoordinates[15] = C[1];
	zCoordinates[15] = C[2];
	//T3B
	xCoordinates[16] = E[0];
	yCoordinates[16] = E[1];
	zCoordinates[16] = E[2];
	//T3C
	xCoordinates[17] = D[0];
	yCoordinates[17] = D[1];
	zCoordinates[17] = D[2];

	//////////////////////////////////////////////

	//LEFT HOPPER FACE
	A[0] = center[0] - width/2;
	A[1] = center[1] + height/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - height/2; // lower hatch left face
	B[2] = center[2] - hatch/2; //

	C[0] = center[0] - hatch/2; 				//
	C[1] = center[1] - height/2; 				// lower hatch left face
	C[2] = center[2] + hatch/2; //

	D[0] = center[0] - width/2;
	D[1] = center[1] + height/2;
	D[2] = center[2] + width/2;

	E[0] = D[0];//
	E[1] = A[1];// upper hatch left face
	E[2] = C[2] - hatch/2;//

	//CREATE 3 TRIANGLES FOR LEFT FACE
	//T1A
	xCoordinates[18] = A[0];
	yCoordinates[18] = A[1];
	zCoordinates[18] = A[2];
	//T1B
	xCoordinates[19] = B[0];
	yCoordinates[19] = B[1];
	zCoordinates[19] = B[2];
	//T1C
	xCoordinates[20] = E[0];
	yCoordinates[20] = E[1];
	zCoordinates[20] = E[2];

	//T2A
	xCoordinates[21] = B[0];
	yCoordinates[21] = B[1];
	zCoordinates[21] = B[2];
	//T2B
	xCoordinates[22] = C[0];
	yCoordinates[22] = C[1];
	zCoordinates[22] = C[2];
	//T2C
	xCoordinates[23] = E[0];
	yCoordinates[23] = E[1];
	zCoordinates[23] = E[2];

	//T3A
	xCoordinates[24] = E[0];
	yCoordinates[24] = E[1];
	zCoordinates[24] = E[2];
	//T3B
	xCoordinates[25] = C[0];
	yCoordinates[25] = C[1];
	zCoordinates[25] = C[2];
	//T3C
	xCoordinates[26] = D[0];
	yCoordinates[26] = D[1];
	zCoordinates[26] = D[2];

	//////////////////////////////////////////////

	//RIGHT HOPPER FACE
	//A->B - **UPPER RIGHT BACK POINT** ID:29
  //D->C - **UPPER RIGHT FRONT POINT** ID:34

	//B->B - **LOWER RIGHT BACK POINT** ID:28
  //C->C - **LOWER RIGHT FRONT POINT** ID:32
	A[0] = center[0] + width/2;
	A[1] = center[1] + height/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] + hatch/2; //
	B[1] = center[1] - height/2; // lower hatch left face
	B[2] = center[2] - hatch/2; //

	C[0] = center[0] + hatch/2; 				//
	C[1] = center[1] - height/2; 				// lower hatch left face
	C[2] = center[2] + hatch/2; //

	D[0] = center[0] + width/2;
	D[1] = center[1] + height/2;
	D[2] = center[2] + width/2;

	E[0] = D[0];//
	E[1] = A[1];// upper hatch left face
	E[2] = C[2] - hatch/2;//

	//CREATE 2 TRIANGLES FOR RIGHT FACE
	//T1A
	xCoordinates[27] = E[0];
	yCoordinates[27] = E[1];
	zCoordinates[27] = E[2];
	//T1B
	xCoordinates[28] = B[0];
	yCoordinates[28] = B[1];
	zCoordinates[28] = B[2];
	//T1C
	xCoordinates[29] = A[0];
	yCoordinates[29] = A[1];
	zCoordinates[29] = A[2];

	//T2A
	xCoordinates[30] = B[0];
	yCoordinates[30] = B[1];
	zCoordinates[30] = B[2];
	//T2B
	xCoordinates[31] = E[0];
	yCoordinates[31] = E[1];
	zCoordinates[31] = E[2];
	//T2C
	xCoordinates[32] = C[0];
	yCoordinates[32] = C[1];
	zCoordinates[32] = C[2];

	//T3A
	xCoordinates[33] = E[0];
	yCoordinates[33] = E[1];
	zCoordinates[33] = E[2];
	//T3B
	xCoordinates[34] = D[0];
	yCoordinates[34] = D[1];
	zCoordinates[34] = D[2];
	//T3C
	xCoordinates[35] = C[0];
	yCoordinates[35] = C[1];
	zCoordinates[35] = C[2];

	//delta::geometry::hopper::explode(xCoordinates, yCoordinates, zCoordinates, 0.1);
}

void delta::geometry::hardcoded::generateOuterHopper(
    iREAL  center[3],
    iREAL  width,
    iREAL  height,
    iREAL  hatch,
    std::vector<iREAL>&  xCoordinates,
    std::vector<iREAL>&  yCoordinates,
    std::vector<iREAL>&  zCoordinates)
{

  //UD - **UPPER LEFT FRONT POINT** ID: 0
  //UA - **UPPER LEFT BACK POINT** ID:10
  //UB - **UPPER RIGHT BACK POINT** ID:29
  //UC - **UPPER RIGHT FRONT POINT** ID:34

  //LD - **LOWER LEFT FRONT POINT** ID: 1
  //LA - **LOWER LEFT BACK POINT** ID:9
  //LB - **LOWER RIGHT BACK POINT** ID:28
  //LC - **LOWER RIGHT FRONT POINT** ID:32

  std::vector<iREAL> iUxCoordinates;
  std::vector<iREAL> iUyCoordinates;
  std::vector<iREAL> iUzCoordinates;

  std::vector<iREAL> iLxCoordinates;
  std::vector<iREAL> iLyCoordinates;
  std::vector<iREAL> iLzCoordinates;

  for(unsigned i=0;i<xCoordinates.size(); i++)
  {
    if(i==10 || i==29 || i==0 || i==34)
    {
      iUxCoordinates.push_back(xCoordinates[i]);
      iUyCoordinates.push_back(yCoordinates[i]);
      iUzCoordinates.push_back(zCoordinates[i]);
    } else if(i==9 || i==28 || i==32 || i==1)
    {
      iLxCoordinates.push_back(xCoordinates[i]);
      iLyCoordinates.push_back(yCoordinates[i]);
      iLzCoordinates.push_back(zCoordinates[i]);
    }
  }
  iREAL iUA[3], iUB[3], iUC[3], iUD[3];
  iREAL iLA[3], iLB[3], iLC[3], iLD[3];

  //UD - **UPPER LEFT FRONT POINT** ID: 0
  //UA - **UPPER LEFT BACK POINT** ID:1
  //UB - **UPPER RIGHT BACK POINT** ID:2
  //UC - **UPPER RIGHT FRONT POINT** ID:3

  //LD - **LOWER LEFT FRONT POINT** ID: 0
  //LA - **LOWER LEFT BACK POINT** ID:1
  //LB - **LOWER RIGHT BACK POINT** ID:2
  //LC - **LOWER RIGHT FRONT POINT** ID:3

  iUA[0] = iUxCoordinates[1];
  iUA[1] = iUyCoordinates[1];
  iUA[2] = iUzCoordinates[1];

  iUB[0] = iUxCoordinates[2];
  iUB[1] = iUyCoordinates[2];
  iUB[2] = iUzCoordinates[2];

  iUC[0] = iUxCoordinates[3];
  iUC[1] = iUyCoordinates[3];
  iUC[2] = iUzCoordinates[3];

  iUD[0] = iUxCoordinates[0];
  iUD[1] = iUyCoordinates[0];
  iUD[2] = iUzCoordinates[0];

  iLA[0] = iLxCoordinates[1];
  iLA[1] = iLyCoordinates[1];
  iLA[2] = iLzCoordinates[1];

  iLB[0] = iLxCoordinates[2];
  iLB[1] = iLyCoordinates[2];
  iLB[2] = iLzCoordinates[2];

  iLC[0] = iLxCoordinates[3];
  iLC[1] = iLyCoordinates[3];
  iLC[2] = iLzCoordinates[3];

  iLD[0] = iLxCoordinates[0];
  iLD[1] = iLyCoordinates[0];
  iLD[2] = iLzCoordinates[0];

  //UD - **UPPER LEFT FRONT POINT** ID: 0
  //UA - **UPPER LEFT BACK POINT** ID:10
  //UB - **UPPER RIGHT BACK POINT** ID:29
  //UC - **UPPER RIGHT FRONT POINT** ID:34

  //LD - **LOWER LEFT FRONT POINT** ID: 1
  //LA - **LOWER LEFT BACK POINT** ID:9
  //LB - **LOWER RIGHT BACK POINT** ID:28
  //LC - **LOWER RIGHT FRONT POINT** ID:32

  std::vector<iREAL> exCoordinates = xCoordinates;
  std::vector<iREAL> eyCoordinates = yCoordinates;
  std::vector<iREAL> ezCoordinates = zCoordinates;

  //delta::geometry::hopper::scaleXYZ(1.2, exCoordinates, eyCoordinates, ezCoordinates);
  delta::geometry::hardcoded::generateInnerHopper(center, width, height, hatch, exCoordinates, eyCoordinates, ezCoordinates);

  xCoordinates.insert(xCoordinates.end(), exCoordinates.begin(), exCoordinates.end());
  yCoordinates.insert(yCoordinates.end(), eyCoordinates.begin(), eyCoordinates.end());
  zCoordinates.insert(zCoordinates.end(), ezCoordinates.begin(), ezCoordinates.end());

  std::vector<iREAL> eUxCoordinates;
  std::vector<iREAL> eUyCoordinates;
  std::vector<iREAL> eUzCoordinates;

  std::vector<iREAL> eLxCoordinates;
  std::vector<iREAL> eLyCoordinates;
  std::vector<iREAL> eLzCoordinates;

  for(unsigned i=0;i<xCoordinates.size(); i++)
  {
    if(i==10 || i==29 || i==0 || i==34)
    {
      eUxCoordinates.push_back(exCoordinates[i]);
      eUyCoordinates.push_back(eyCoordinates[i]);
      eUzCoordinates.push_back(ezCoordinates[i]);
    } else if(i==9 || i==28 || i==32 || i==1)
    {
      eLxCoordinates.push_back(exCoordinates[i]);
      eLyCoordinates.push_back(eyCoordinates[i]);
      eLzCoordinates.push_back(ezCoordinates[i]);
    }
  }
  iREAL eUA[3], eUB[3], eUC[3], eUD[3];
  iREAL eLA[3], eLB[3], eLC[3], eLD[3];

  //UD - **UPPER LEFT FRONT POINT** ID: 0
  //UA - **UPPER LEFT BACK POINT** ID:1
  //UB - **UPPER RIGHT BACK POINT** ID:2
  //UC - **UPPER RIGHT FRONT POINT** ID:3

  //LD - **LOWER LEFT FRONT POINT** ID: 0
  //LA - **LOWER LEFT BACK POINT** ID:1
  //LB - **LOWER RIGHT BACK POINT** ID:2
  //LC - **LOWER RIGHT FRONT POINT** ID:3

  eUA[0] = eUxCoordinates[1];
  eUA[1] = eUyCoordinates[1];
  eUA[2] = eUzCoordinates[1];

  eUB[0] = eUxCoordinates[2];
  eUB[1] = eUyCoordinates[2];
  eUB[2] = eUzCoordinates[2];

  eUC[0] = eUxCoordinates[3];
  eUC[1] = eUyCoordinates[3];
  eUC[2] = eUzCoordinates[3];

  eUD[0] = eUxCoordinates[0];
  eUD[1] = eUyCoordinates[0];
  eUD[2] = eUzCoordinates[0];

  eLA[0] = eLxCoordinates[1];
  eLA[1] = eLyCoordinates[1];
  eLA[2] = eLzCoordinates[1];

  eLB[0] = eLxCoordinates[2];
  eLB[1] = eLyCoordinates[2];
  eLB[2] = eLzCoordinates[2];

  eLC[0] = eLxCoordinates[3];
  eLC[1] = eLyCoordinates[3];
  eLC[2] = eLzCoordinates[3];

  eLD[0] = eLxCoordinates[0];
  eLD[1] = eLyCoordinates[0];
  eLD[2] = eLzCoordinates[0];

  iREAL UA[3], UB[3], UC[3], UD[3], UE[3], UF[3], UG[3], UH[3], UI[3], UJ[3], UK[3], UL[3];

  UA[0] = eUA[0];
  UA[1] = eUA[1];
  UA[2] = eUA[2];

  UB[0] = iUA[0];
  UB[1] = iUA[1];
  UB[2] = iUA[2];

  UD[0] = iUB[0];
  UD[1] = iUB[1];
  UD[2] = iUB[2];

  UE[0] = eUB[0];
  UE[1] = eUB[1];
  UE[2] = eUB[2];

  UG[0] = iUC[0];
  UG[1] = iUC[1];
  UG[2] = iUC[2];

  UH[0] = eUC[0];
  UH[1] = eUC[1];
  UH[2] = eUC[2];

  UJ[0] = iUD[0];
  UJ[1] = iUD[1];
  UJ[2] = iUD[2];

  UK[0] = eUD[0];
  UK[1] = eUD[1];
  UK[2] = eUD[2];

  UC[0] = (UA[0] + UE[0])/2;
  UC[1] = (UA[1] + UE[1])/2;
  UC[2] = (UA[2] + UE[2])/2;

  UF[0] = (UE[0] + UH[0])/2;
  UF[1] = (UE[1] + UH[1])/2;
  UF[2] = (UE[2] + UH[2])/2;

  UI[0] = (UK[0] + UH[0])/2;
  UI[1] = (UK[1] + UH[1])/2;
  UI[2] = (UK[2] + UH[2])/2;

  UL[0] = (UA[0] + UK[0])/2;
  UL[1] = (UA[1] + UK[1])/2;
  UL[2] = (UA[2] + UK[2])/2;

  //UP BACK
  //ABC
  xCoordinates.push_back(UA[0]);
  yCoordinates.push_back(UA[1]);
  zCoordinates.push_back(UA[2]);

  xCoordinates.push_back(UB[0]);
  yCoordinates.push_back(UB[1]);
  zCoordinates.push_back(UB[2]);

  xCoordinates.push_back(UC[0]);
  yCoordinates.push_back(UC[1]);
  zCoordinates.push_back(UC[2]);

  //BCD
  xCoordinates.push_back(UC[0]);
  yCoordinates.push_back(UC[1]);
  zCoordinates.push_back(UC[2]);

  xCoordinates.push_back(UB[0]);
  yCoordinates.push_back(UB[1]);
  zCoordinates.push_back(UB[2]);

  xCoordinates.push_back(UD[0]);
  yCoordinates.push_back(UD[1]);
  zCoordinates.push_back(UD[2]);

  //CDE
  xCoordinates.push_back(UC[0]);
  yCoordinates.push_back(UC[1]);
  zCoordinates.push_back(UC[2]);

  xCoordinates.push_back(UD[0]);
  yCoordinates.push_back(UD[1]);
  zCoordinates.push_back(UD[2]);

  xCoordinates.push_back(UE[0]);
  yCoordinates.push_back(UE[1]);
  zCoordinates.push_back(UE[2]);


  //UP RIGHT
  //DEF
  xCoordinates.push_back(UE[0]);
  yCoordinates.push_back(UE[1]);
  zCoordinates.push_back(UE[2]);

  xCoordinates.push_back(UD[0]);
  yCoordinates.push_back(UD[1]);
  zCoordinates.push_back(UD[2]);

  xCoordinates.push_back(UF[0]);
  yCoordinates.push_back(UF[1]);
  zCoordinates.push_back(UF[2]);

  //DFG
  xCoordinates.push_back(UD[0]);
  yCoordinates.push_back(UD[1]);
  zCoordinates.push_back(UD[2]);

  xCoordinates.push_back(UG[0]);
  yCoordinates.push_back(UG[1]);
  zCoordinates.push_back(UG[2]);

  xCoordinates.push_back(UF[0]);
  yCoordinates.push_back(UF[1]);
  zCoordinates.push_back(UF[2]);

  //GFH
  xCoordinates.push_back(UG[0]);
  yCoordinates.push_back(UG[1]);
  zCoordinates.push_back(UG[2]);

  xCoordinates.push_back(UH[0]);
  yCoordinates.push_back(UH[1]);
  zCoordinates.push_back(UH[2]);

  xCoordinates.push_back(UF[0]);
  yCoordinates.push_back(UF[1]);
  zCoordinates.push_back(UF[2]);


  //UP FRONT
  //HGI
  xCoordinates.push_back(UH[0]);
  yCoordinates.push_back(UH[1]);
  zCoordinates.push_back(UH[2]);

  xCoordinates.push_back(UG[0]);
  yCoordinates.push_back(UG[1]);
  zCoordinates.push_back(UG[2]);

  xCoordinates.push_back(UI[0]);
  yCoordinates.push_back(UI[1]);
  zCoordinates.push_back(UI[2]);

  //IGJ
  xCoordinates.push_back(UI[0]);
  yCoordinates.push_back(UI[1]);
  zCoordinates.push_back(UI[2]);

  xCoordinates.push_back(UG[0]);
  yCoordinates.push_back(UG[1]);
  zCoordinates.push_back(UG[2]);

  xCoordinates.push_back(UJ[0]);
  yCoordinates.push_back(UJ[1]);
  zCoordinates.push_back(UJ[2]);

  //IJK
  xCoordinates.push_back(UI[0]);
  yCoordinates.push_back(UI[1]);
  zCoordinates.push_back(UI[2]);

  xCoordinates.push_back(UJ[0]);
  yCoordinates.push_back(UJ[1]);
  zCoordinates.push_back(UJ[2]);

  xCoordinates.push_back(UK[0]);
  yCoordinates.push_back(UK[1]);
  zCoordinates.push_back(UK[2]);

  //UP LEFT
  //KJL
  xCoordinates.push_back(UK[0]);
  yCoordinates.push_back(UK[1]);
  zCoordinates.push_back(UK[2]);

  xCoordinates.push_back(UJ[0]);
  yCoordinates.push_back(UJ[1]);
  zCoordinates.push_back(UJ[2]);

  xCoordinates.push_back(UL[0]);
  yCoordinates.push_back(UL[1]);
  zCoordinates.push_back(UL[2]);

  //JBL
  xCoordinates.push_back(UJ[0]);
  yCoordinates.push_back(UJ[1]);
  zCoordinates.push_back(UJ[2]);

  xCoordinates.push_back(UB[0]);
  yCoordinates.push_back(UB[1]);
  zCoordinates.push_back(UB[2]);

  xCoordinates.push_back(UL[0]);
  yCoordinates.push_back(UL[1]);
  zCoordinates.push_back(UL[2]);

  //LBA
  xCoordinates.push_back(UL[0]);
  yCoordinates.push_back(UL[1]);
  zCoordinates.push_back(UL[2]);

  xCoordinates.push_back(UB[0]);
  yCoordinates.push_back(UB[1]);
  zCoordinates.push_back(UB[2]);

  xCoordinates.push_back(UA[0]);
  yCoordinates.push_back(UA[1]);
  zCoordinates.push_back(UA[2]);


  iREAL LA[3], LB[3], LC[3], LD[3], LE[3], LF[3], LG[3], LH[3], LI[3], LJ[3], LK[3], LL[3];

  LA[0] = eLA[0];
  LA[1] = eLA[1];
  LA[2] = eLA[2];

  LB[0] = iLA[0];
  LB[1] = iLA[1];
  LB[2] = iLA[2];

  LD[0] = iLB[0];
  LD[1] = iLB[1];
  LD[2] = iLB[2];

  LE[0] = eLB[0];
  LE[1] = eLB[1];
  LE[2] = eLB[2];

  LG[0] = iLC[0];
  LG[1] = iLC[1];
  LG[2] = iLC[2];

  LH[0] = eLC[0];
  LH[1] = eLC[1];
  LH[2] = eLC[2];

  LJ[0] = iLD[0];
  LJ[1] = iLD[1];
  LJ[2] = iLD[2];

  LK[0] = eLD[0];
  LK[1] = eLD[1];
  LK[2] = eLD[2];

  LC[0] = (LA[0] + LE[0])/2;
  LC[1] = (LA[1] + LE[1])/2;
  LC[2] = (LA[2] + LE[2])/2;

  LF[0] = (LE[0] + LH[0])/2;
  LF[1] = (LE[1] + LH[1])/2;
  LF[2] = (LE[2] + LH[2])/2;

  LI[0] = (LK[0] + LH[0])/2;
  LI[1] = (LK[1] + LH[1])/2;
  LI[2] = (LK[2] + LH[2])/2;

  LL[0] = (LA[0] + LK[0])/2;
  LL[1] = (LA[1] + LK[1])/2;
  LL[2] = (LA[2] + LK[2])/2;

  //LP BACK
  //ABC
  xCoordinates.push_back(LC[0]);
  yCoordinates.push_back(LC[1]);
  zCoordinates.push_back(LC[2]);

  xCoordinates.push_back(LB[0]);
  yCoordinates.push_back(LB[1]);
  zCoordinates.push_back(LB[2]);

  xCoordinates.push_back(LA[0]);
  yCoordinates.push_back(LA[1]);
  zCoordinates.push_back(LA[2]);

  //BCD
  xCoordinates.push_back(LD[0]);
  yCoordinates.push_back(LD[1]);
  zCoordinates.push_back(LD[2]);

  xCoordinates.push_back(LB[0]);
  yCoordinates.push_back(LB[1]);
  zCoordinates.push_back(LB[2]);

  xCoordinates.push_back(LC[0]);
  yCoordinates.push_back(LC[1]);
  zCoordinates.push_back(LC[2]);

  //CDE
  xCoordinates.push_back(LE[0]);
  yCoordinates.push_back(LE[1]);
  zCoordinates.push_back(LE[2]);

  xCoordinates.push_back(LD[0]);
  yCoordinates.push_back(LD[1]);
  zCoordinates.push_back(LD[2]);

  xCoordinates.push_back(LC[0]);
  yCoordinates.push_back(LC[1]);
  zCoordinates.push_back(LC[2]);


  //LP RIGHT
  //DEF
  xCoordinates.push_back(LF[0]);
  yCoordinates.push_back(LF[1]);
  zCoordinates.push_back(LF[2]);

  xCoordinates.push_back(LD[0]);
  yCoordinates.push_back(LD[1]);
  zCoordinates.push_back(LD[2]);

  xCoordinates.push_back(LE[0]);
  yCoordinates.push_back(LE[1]);
  zCoordinates.push_back(LE[2]);

  //DFG
  xCoordinates.push_back(LF[0]);
  yCoordinates.push_back(LF[1]);
  zCoordinates.push_back(LF[2]);

  xCoordinates.push_back(LG[0]);
  yCoordinates.push_back(LG[1]);
  zCoordinates.push_back(LG[2]);

  xCoordinates.push_back(LD[0]);
  yCoordinates.push_back(LD[1]);
  zCoordinates.push_back(LD[2]);

  //GFH
  xCoordinates.push_back(LG[0]);
  yCoordinates.push_back(LG[1]);
  zCoordinates.push_back(LG[2]);

  xCoordinates.push_back(LF[0]);
  yCoordinates.push_back(LF[1]);
  zCoordinates.push_back(LF[2]);

  xCoordinates.push_back(LH[0]);
  yCoordinates.push_back(LH[1]);
  zCoordinates.push_back(LH[2]);


  //LP FRONT
  //HGI
  xCoordinates.push_back(LI[0]);
  yCoordinates.push_back(LI[1]);
  zCoordinates.push_back(LI[2]);

  xCoordinates.push_back(LG[0]);
  yCoordinates.push_back(LG[1]);
  zCoordinates.push_back(LG[2]);

  xCoordinates.push_back(LH[0]);
  yCoordinates.push_back(LH[1]);
  zCoordinates.push_back(LH[2]);

  //IGJ
  xCoordinates.push_back(LJ[0]);
  yCoordinates.push_back(LJ[1]);
  zCoordinates.push_back(LJ[2]);

  xCoordinates.push_back(LG[0]);
  yCoordinates.push_back(LG[1]);
  zCoordinates.push_back(LG[2]);

  xCoordinates.push_back(LI[0]);
  yCoordinates.push_back(LI[1]);
  zCoordinates.push_back(LI[2]);

  //IJK
  xCoordinates.push_back(LK[0]);
  yCoordinates.push_back(LK[1]);
  zCoordinates.push_back(LK[2]);

  xCoordinates.push_back(LJ[0]);
  yCoordinates.push_back(LJ[1]);
  zCoordinates.push_back(LJ[2]);

  xCoordinates.push_back(LI[0]);
  yCoordinates.push_back(LI[1]);
  zCoordinates.push_back(LI[2]);

  //LP LEFT
  //KJL
  xCoordinates.push_back(LL[0]);
  yCoordinates.push_back(LL[1]);
  zCoordinates.push_back(LL[2]);

  xCoordinates.push_back(LJ[0]);
  yCoordinates.push_back(LJ[1]);
  zCoordinates.push_back(LJ[2]);

  xCoordinates.push_back(LK[0]);
  yCoordinates.push_back(LK[1]);
  zCoordinates.push_back(LK[2]);

  //JBL
  xCoordinates.push_back(LJ[0]);
  yCoordinates.push_back(LJ[1]);
  zCoordinates.push_back(LJ[2]);

  xCoordinates.push_back(LL[0]);
  yCoordinates.push_back(LL[1]);
  zCoordinates.push_back(LL[2]);

  xCoordinates.push_back(LB[0]);
  yCoordinates.push_back(LB[1]);
  zCoordinates.push_back(LB[2]);

  //LBA
  xCoordinates.push_back(LA[0]);
  yCoordinates.push_back(LA[1]);
  zCoordinates.push_back(LA[2]);

  xCoordinates.push_back(LB[0]);
  yCoordinates.push_back(LB[1]);
  zCoordinates.push_back(LB[2]);

  xCoordinates.push_back(LL[0]);
  yCoordinates.push_back(LL[1]);
  zCoordinates.push_back(LL[2]);


  //CCW the triangles of inner hopper

  iREAL tmp[3];
  tmp[0] = xCoordinates[0];
  tmp[1] = yCoordinates[0];
  tmp[2] = zCoordinates[0];

  xCoordinates[0] = xCoordinates[2];
  yCoordinates[0] = yCoordinates[2];
  zCoordinates[0] = zCoordinates[2];

  xCoordinates[2] = tmp[0];
  yCoordinates[2] = tmp[1];
  zCoordinates[2] = tmp[2];

  tmp[0] = xCoordinates[3];
  tmp[1] = yCoordinates[3];
  tmp[2] = zCoordinates[3];

  xCoordinates[3] = xCoordinates[5];
  yCoordinates[3] = yCoordinates[5];
  zCoordinates[3] = zCoordinates[5];

  xCoordinates[5] = tmp[0];
  yCoordinates[5] = tmp[1];
  zCoordinates[5] = tmp[2];


  tmp[0] = xCoordinates[6];
  tmp[1] = yCoordinates[6];
  tmp[2] = zCoordinates[6];

  xCoordinates[6] = xCoordinates[8];
  yCoordinates[6] = yCoordinates[8];
  zCoordinates[6] = zCoordinates[8];

  xCoordinates[8] = tmp[0];
  yCoordinates[8] = tmp[1];
  zCoordinates[8] = tmp[2];


  tmp[0] = xCoordinates[9];
  tmp[1] = yCoordinates[9];
  tmp[2] = zCoordinates[9];

  xCoordinates[9] = xCoordinates[11];
  yCoordinates[9] = yCoordinates[11];
  zCoordinates[9] = zCoordinates[11];

  xCoordinates[11] = tmp[0];
  yCoordinates[11] = tmp[1];
  zCoordinates[11] = tmp[2];

  tmp[0] = xCoordinates[12];
  tmp[1] = yCoordinates[12];
  tmp[2] = zCoordinates[12];

  xCoordinates[12] = xCoordinates[14];
  yCoordinates[12] = yCoordinates[14];
  zCoordinates[12] = zCoordinates[14];

  xCoordinates[14] = tmp[0];
  yCoordinates[14] = tmp[1];
  zCoordinates[14] = tmp[2];

  tmp[0] = xCoordinates[15];
  tmp[1] = yCoordinates[15];
  tmp[2] = zCoordinates[15];

  xCoordinates[15] = xCoordinates[17];
  yCoordinates[15] = yCoordinates[17];
  zCoordinates[15] = zCoordinates[17];

  xCoordinates[17] = tmp[0];
  yCoordinates[17] = tmp[1];
  zCoordinates[17] = tmp[2];

  tmp[0] = xCoordinates[18];
  tmp[1] = yCoordinates[18];
  tmp[2] = zCoordinates[18];

  xCoordinates[18] = xCoordinates[20];
  yCoordinates[18] = yCoordinates[20];
  zCoordinates[18] = zCoordinates[20];

  xCoordinates[20] = tmp[0];
  yCoordinates[20] = tmp[1];
  zCoordinates[20] = tmp[2];

  tmp[0] = xCoordinates[23];
  tmp[1] = yCoordinates[23];
  tmp[2] = zCoordinates[23];

  xCoordinates[23] = xCoordinates[21];
  yCoordinates[23] = yCoordinates[21];
  zCoordinates[23] = zCoordinates[21];

  xCoordinates[21] = tmp[0];
  yCoordinates[21] = tmp[1];
  zCoordinates[21] = tmp[2];

  tmp[0] = xCoordinates[24];
  tmp[1] = yCoordinates[24];
  tmp[2] = zCoordinates[24];

  xCoordinates[24] = xCoordinates[26];
  yCoordinates[24] = yCoordinates[26];
  zCoordinates[24] = zCoordinates[26];

  xCoordinates[26] = tmp[0];
  yCoordinates[26] = tmp[1];
  zCoordinates[26] = tmp[2];

  tmp[0] = xCoordinates[28];
  tmp[1] = yCoordinates[28];
  tmp[2] = zCoordinates[28];

  xCoordinates[28] = xCoordinates[29];
  yCoordinates[28] = yCoordinates[29];
  zCoordinates[28] = zCoordinates[29];

  xCoordinates[29] = tmp[0];
  yCoordinates[29] = tmp[1];
  zCoordinates[29] = tmp[2];

  tmp[0] = xCoordinates[30];
  tmp[1] = yCoordinates[30];
  tmp[2] = zCoordinates[30];

  xCoordinates[30] = xCoordinates[32];
  yCoordinates[30] = yCoordinates[32];
  zCoordinates[30] = zCoordinates[32];

  xCoordinates[32] = tmp[0];
  yCoordinates[32] = tmp[1];
  zCoordinates[32] = tmp[2];

  tmp[0] = xCoordinates[33];
  tmp[1] = yCoordinates[33];
  tmp[2] = zCoordinates[33];

  xCoordinates[33] = xCoordinates[35];
  yCoordinates[33] = yCoordinates[35];
  zCoordinates[33] = zCoordinates[35];

  xCoordinates[35] = tmp[0];
  yCoordinates[35] = tmp[1];
  zCoordinates[35] = tmp[2];
}

delta::geometry::mesh::Mesh *delta::geometry::hardcoded::generateHopper(
    iREAL  center[3],
    iREAL  width,
    iREAL  thickness,
    iREAL  height,
    iREAL  hatch,
    int    meshRefinement,
    iREAL gridH
)
{
  std::vector<iREAL>	xCoordinates;
  std::vector<iREAL>	yCoordinates;
  std::vector<iREAL>	zCoordinates;
  delta::geometry::hardcoded::generateInnerHopper(center, width, height, hatch, xCoordinates, yCoordinates, zCoordinates);
  delta::geometry::hardcoded::generateOuterHopper(center, width+thickness, height, hatch+0.005, xCoordinates, yCoordinates, zCoordinates);
  delta::geometry::operators::triangle::meshDenser(meshRefinement, gridH, xCoordinates, yCoordinates, zCoordinates);

  return new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}

