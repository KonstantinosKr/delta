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

#include "hopper.h"

#include <stdlib.h>
#include <assert.h>


void delta::primitives::generateHopper(
		double    center[3],
		double 	width,
		double  height,
		double 	hatch,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates
)
{
	assert(xCoordinates.empty());
	assert(yCoordinates.empty());
	assert(zCoordinates.empty());

	const int numberOfTriangles = 3*4;//3 triangles x 4 faces

	xCoordinates.resize( numberOfTriangles*3 );
	yCoordinates.resize( numberOfTriangles*3 );
	zCoordinates.resize( numberOfTriangles*3 );

	//FRONT HOPPER FACE
	double A[3], B[3], C[3], D[3], E[3];
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
	A[0] = center[0] - width/2;
	A[1] = center[1] + height/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - height/2; // lower hatch back face
	B[2] = center[2] - hatch/2; //

	C[0] = center[0] + hatch/2; //
	C[1] = B[1]; 				// lower hatch back face
	C[2] = center[2] - hatch/2; //

	D[0] = center[0] + width/2;
	D[1] = A[1];
	D[2] = A[2];

	E[0] = C[0] - hatch/2; //
	E[1] = A[1];//upper hatch back face
	E[2] = A[2];//

	//CREATE 3 TRIANGLES FOR BACK FACE
	//T1A
	xCoordinates[9] = A[0];
	yCoordinates[9] = A[1];
	zCoordinates[9] = A[2];
	//T1B
	xCoordinates[10] = B[0];
	yCoordinates[10] = B[1];
	zCoordinates[10] = B[2];
	//T1C
	xCoordinates[11] = E[0];
	yCoordinates[11] = E[1];
	zCoordinates[11] = E[2];

	//T2A
	xCoordinates[12] = B[0];
	yCoordinates[12] = B[1];
	zCoordinates[12] = B[2];
	//T2B
	xCoordinates[13] = C[0];
	yCoordinates[13] = C[1];
	zCoordinates[13] = C[2];
	//T2C
	xCoordinates[14] = E[0];
	yCoordinates[14] = E[1];
	zCoordinates[14] = E[2];

	//T3A
	xCoordinates[15] = D[0];
	yCoordinates[15] = D[1];
	zCoordinates[15] = D[2];
	//T3B
	xCoordinates[16] = E[0];
	yCoordinates[16] = E[1];
	zCoordinates[16] = E[2];
	//T3C
	xCoordinates[17] = C[0];
	yCoordinates[17] = C[1];
	zCoordinates[17] = C[2];

	//////////////////////////////////////////////

	//LEFT HOPPER FACE
	A[0] = center[0] - width/2;
	A[1] = center[1] + height/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - height/2; // lower hatch left face
	B[2] = center[2] - hatch/2; //

	C[0] = B[0]; 				//
	C[1] = B[1]; 				// lower hatch left face
	C[2] = center[2] + hatch/2; //

	D[0] = A[0];
	D[1] = A[1];
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
	xCoordinates[24] = D[0];
	yCoordinates[24] = D[1];
	zCoordinates[24] = D[2];
	//T3B
	xCoordinates[25] = C[0];
	yCoordinates[25] = C[1];
	zCoordinates[25] = C[2];
	//T3C
	xCoordinates[26] = E[0];
	yCoordinates[26] = E[1];
	zCoordinates[26] = E[2];

	//////////////////////////////////////////////

	//RIGHT HOPPER FACE
	A[0] = center[0] + width/2;
	A[1] = center[1] + height/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] + hatch/2; //
	B[1] = center[1] - height/2; // lower hatch left face
	B[2] = center[2] - hatch/2; //

	C[0] = B[0]; 				//
	C[1] = B[1]; 				// lower hatch left face
	C[2] = center[2] + hatch/2; //

	D[0] = A[0];
	D[1] = A[1];
	D[2] = center[2] + width/2;

	E[0] = D[0];//
	E[1] = A[1];// upper hatch left face
	E[2] = C[2] - hatch/2;//

	//CREATE 2 TRIANGLES FOR RIGHT FACE
	//T1A
	xCoordinates[27] = A[0];
	yCoordinates[27] = A[1];
	zCoordinates[27] = A[2];
	//T1B
	xCoordinates[28] = B[0];
	yCoordinates[28] = B[1];
	zCoordinates[28] = B[2];
	//T1C
	xCoordinates[29] = E[0];
	yCoordinates[29] = E[1];
	zCoordinates[29] = E[2];

	//T2A
	xCoordinates[30] = C[0];
	yCoordinates[30] = C[1];
	zCoordinates[30] = C[2];
	//T2B
	xCoordinates[31] = E[0];
	yCoordinates[31] = E[1];
	zCoordinates[31] = E[2];
	//T2C
	xCoordinates[32] = B[0];
	yCoordinates[32] = B[1];
	zCoordinates[32] = B[2];

	//T3A
	xCoordinates[33] = C[0];
	yCoordinates[33] = C[1];
	zCoordinates[33] = C[2];
	//T3B
	xCoordinates[34] = D[0];
	yCoordinates[34] = D[1];
	zCoordinates[34] = D[2];
	//T3C
	xCoordinates[35] = E[0];
	yCoordinates[35] = E[1];
	zCoordinates[35] = E[2];
}

void delta::primitives::generateHopper(
		double    center[3],
		double 	width,
		double 	hatch,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates
)
{
	assert(xCoordinates.empty());
	assert(yCoordinates.empty());
	assert(zCoordinates.empty());

	const int numberOfTriangles = 3*4;//3 triangles x 4 faces

	xCoordinates.resize( numberOfTriangles*3 );
	yCoordinates.resize( numberOfTriangles*3 );
	zCoordinates.resize( numberOfTriangles*3 );

	//FRONT HOPPER FACE
	double A[3], B[3], C[3], D[3], E[3];
	A[0] = center[0] - width/2;
	A[1] = center[1] + width/2;
	A[2] = center[2] + width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - width/2; // lower hatch front face
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
	A[0] = center[0] - width/2;
	A[1] = center[1] + width/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - width/2; // lower hatch back face
	B[2] = center[2] - hatch/2; //

	C[0] = center[0] + hatch/2; //
	C[1] = B[1]; 				// lower hatch back face
	C[2] = center[2] - hatch/2; //

	D[0] = center[0] + width/2;
	D[1] = A[1];
	D[2] = A[2];

	E[0] = C[0] - hatch/2; //
	E[1] = A[1];//upper hatch back face
	E[2] = A[2];//

	//CREATE 3 TRIANGLES FOR BACK FACE
	//T1A
	xCoordinates[9] = A[0];
	yCoordinates[9] = A[1];
	zCoordinates[9] = A[2];
	//T1B
	xCoordinates[10] = B[0];
	yCoordinates[10] = B[1];
	zCoordinates[10] = B[2];
	//T1C
	xCoordinates[11] = E[0];
	yCoordinates[11] = E[1];
	zCoordinates[11] = E[2];

	//T2A
	xCoordinates[12] = B[0];
	yCoordinates[12] = B[1];
	zCoordinates[12] = B[2];
	//T2B
	xCoordinates[13] = C[0];
	yCoordinates[13] = C[1];
	zCoordinates[13] = C[2];
	//T2C
	xCoordinates[14] = E[0];
	yCoordinates[14] = E[1];
	zCoordinates[14] = E[2];

	//T3A
	xCoordinates[15] = D[0];
	yCoordinates[15] = D[1];
	zCoordinates[15] = D[2];
	//T3B
	xCoordinates[16] = E[0];
	yCoordinates[16] = E[1];
	zCoordinates[16] = E[2];
	//T3C
	xCoordinates[17] = C[0];
	yCoordinates[17] = C[1];
	zCoordinates[17] = C[2];

	//////////////////////////////////////////////

	//LEFT HOPPER FACE
	A[0] = center[0] - width/2;
	A[1] = center[1] + width/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] - hatch/2; //
	B[1] = center[1] - width/2; // lower hatch left face
	B[2] = center[2] - hatch/2; //

	C[0] = B[0]; 				//
	C[1] = B[1]; 				// lower hatch left face
	C[2] = center[2] + hatch/2; //

	D[0] = A[0];
	D[1] = A[1];
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
	xCoordinates[24] = D[0];
	yCoordinates[24] = D[1];
	zCoordinates[24] = D[2];
	//T3B
	xCoordinates[25] = C[0];
	yCoordinates[25] = C[1];
	zCoordinates[25] = C[2];
	//T3C
	xCoordinates[26] = E[0];
	yCoordinates[26] = E[1];
	zCoordinates[26] = E[2];

	//////////////////////////////////////////////

	//RIGHT HOPPER FACE
	A[0] = center[0] + width/2;
	A[1] = center[1] + width/2;
	A[2] = center[2] - width/2;

	B[0] = center[0] + hatch/2; //
	B[1] = center[1] - width/2; // lower hatch left face
	B[2] = center[2] - hatch/2; //

	C[0] = B[0]; 				//
	C[1] = B[1]; 				// lower hatch left face
	C[2] = center[2] + hatch/2; //

	D[0] = A[0];
	D[1] = A[1];
	D[2] = center[2] + width/2;

	E[0] = D[0];//
	E[1] = A[1];// upper hatch left face
	E[2] = C[2] - hatch/2;//

	//CREATE 2 TRIANGLES FOR RIGHT FACE
	//T1A
	xCoordinates[27] = A[0];
	yCoordinates[27] = A[1];
	zCoordinates[27] = A[2];
	//T1B
	xCoordinates[28] = B[0];
	yCoordinates[28] = B[1];
	zCoordinates[28] = B[2];
	//T1C
	xCoordinates[29] = E[0];
	yCoordinates[29] = E[1];
	zCoordinates[29] = E[2];

	//T2A
	xCoordinates[30] = C[0];
	yCoordinates[30] = C[1];
	zCoordinates[30] = C[2];
	//T2B
	xCoordinates[31] = E[0];
	yCoordinates[31] = E[1];
	zCoordinates[31] = E[2];
	//T2C
	xCoordinates[32] = B[0];
	yCoordinates[32] = B[1];
	zCoordinates[32] = B[2];

	//T3A
	xCoordinates[33] = C[0];
	yCoordinates[33] = C[1];
	zCoordinates[33] = C[2];
	//T3B
	xCoordinates[34] = D[0];
	yCoordinates[34] = D[1];
	zCoordinates[34] = D[2];
	//T3C
	xCoordinates[35] = E[0];
	yCoordinates[35] = E[1];
	zCoordinates[35] = E[2];
}


