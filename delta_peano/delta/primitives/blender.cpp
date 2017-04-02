/*
 The MIT License (MIT)

 Copyright (c) 15 Jun 2016 Konstantinos Krestenitis

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

#include "blender.h"

#include <stdlib.h>
#include <assert.h>

void delta::primitives::blender::generateBlender(
		double    center[3],
		double 	width,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates
)
{
	assert(xCoordinates.empty());
	assert(yCoordinates.empty());
	assert(zCoordinates.empty());

	const int numberOfTriangles = 2*6 + 4*5;//2 triangles x 6 faces + (4 rods * 5 faces)

	xCoordinates.resize( numberOfTriangles*3 );
	yCoordinates.resize( numberOfTriangles*3 );
	zCoordinates.resize( numberOfTriangles*3 );

	//FRONT HOPPER FACE
	double upperA[3], upperB[3], upperC[3], upperD[3];
	double lowerA[3], lowerB[3], lowerC[3], lowerD[3];

	upperA[0] = center[0] - width/20;
	upperA[1] = center[1] + width;
	upperA[2] = center[2] - width/20;

	upperB[0] = center[0] + width/20;
	upperB[1] = upperA[1];
	upperB[2] = upperA[2];

	upperC[0] = upperA[0];
	upperC[1] = upperA[1];
	upperC[2] = center[2] + width/20;

	upperD[0] = upperB[0];
	upperD[1] = upperB[1];
	upperD[2] = upperC[2];

	////////lower 4 points
	lowerA[0] = upperA[0];
	lowerA[1] = center[1] - width;
	lowerA[2] = center[2] - width/20;

	lowerB[0] = upperB[0];
	lowerB[1] = lowerA[1];
	lowerB[2] = lowerA[2];

	lowerC[0] = upperC[0];
	lowerC[1] = lowerA[1];
	lowerC[2] = upperC[2];

	lowerD[0] = upperB[0];
	lowerD[1] = lowerA[1];
	lowerD[2] = upperC[2];

	//CREATE 3 TRIANGLES FOR BACK FACE of rod
	//T1A
	xCoordinates[0] = upperA[0];
	yCoordinates[0] = upperA[1];
	zCoordinates[0] = upperA[2];
	//T1B
	xCoordinates[1] = upperB[0];
	yCoordinates[1] = upperB[1];
	zCoordinates[1] = upperB[2];
	//T1C
	xCoordinates[2] = lowerA[0];
	yCoordinates[2] = lowerA[1];
	zCoordinates[2] = lowerA[2];

	//T2A
	xCoordinates[3] = upperB[0];
	yCoordinates[3] = upperB[1];
	zCoordinates[3] = upperB[2];
	//T2B
	xCoordinates[4] = lowerB[0];
	yCoordinates[4] = lowerB[1];
	zCoordinates[4] = lowerB[2];
	//T2C
	xCoordinates[5] = lowerA[0];
	yCoordinates[5] = lowerA[1];
	zCoordinates[5] = lowerA[2];

	//CREATE 3 TRIANGLES FOR FRONT FACE of rod

	//T3A
	xCoordinates[6] = upperC[0];
	yCoordinates[6] = upperC[1];
	zCoordinates[6] = upperC[2];
	//T3B
	xCoordinates[7] = upperD[0];
	yCoordinates[7] = upperD[1];
	zCoordinates[7] = upperD[2];
	//T3C
	xCoordinates[8] = lowerC[0];
	yCoordinates[8] = lowerC[1];
	zCoordinates[8] = lowerC[2];

	//T3A
	xCoordinates[9] = upperD[0];
	yCoordinates[9] = upperD[1];
	zCoordinates[9] = upperD[2];
	//T3B
	xCoordinates[10] = lowerC[0];
	yCoordinates[10] = lowerC[1];
	zCoordinates[10] = lowerC[2];
	//T3C
	xCoordinates[11] = lowerD[0];
	yCoordinates[11] = lowerD[1];
	zCoordinates[11] = lowerD[2];

	//CREATE 3 TRIANGLES FOR LEFT FACE of rod

	//T3A
	xCoordinates[12] = upperA[0];
	yCoordinates[12] = upperA[1];
	zCoordinates[12] = upperA[2];
	//T3B
	xCoordinates[13] = upperC[0];
	yCoordinates[13] = upperC[1];
	zCoordinates[13] = upperC[2];
	//T3C
	xCoordinates[14] = lowerA[0];
	yCoordinates[14] = lowerA[1];
	zCoordinates[14] = lowerA[2];

	//T3A
	xCoordinates[15] = upperC[0];
	yCoordinates[15] = upperC[1];
	zCoordinates[15] = upperC[2];
	//T3B
	xCoordinates[16] = lowerA[0];
	yCoordinates[16] = lowerA[1];
	zCoordinates[16] = lowerA[2];
	//T3C
	xCoordinates[17] = lowerC[0];
	yCoordinates[17] = lowerC[1];
	zCoordinates[17] = lowerC[2];

	//CREATE 3 TRIANGLES FOR RIGHT FACE of rod
	//T3A
	xCoordinates[18] = upperB[0];
	yCoordinates[18] = upperB[1];
	zCoordinates[18] = upperB[2];
	//T3B
	xCoordinates[19] = lowerB[0];
	yCoordinates[19] = lowerB[1];
	zCoordinates[19] = lowerB[2];
	//T3C
	xCoordinates[20] = upperD[0];
	yCoordinates[20] = upperD[1];
	zCoordinates[20] = upperD[2];

	//T3A
	xCoordinates[21] = upperD[0];
	yCoordinates[21] = upperD[1];
	zCoordinates[21] = upperD[2];
	//T3B
	xCoordinates[22] = lowerD[0];
	yCoordinates[22] = lowerD[1];
	zCoordinates[22] = lowerD[2];
	//T3C
	xCoordinates[23] = lowerB[0];
	yCoordinates[23] = lowerB[1];
	zCoordinates[23] = lowerB[2];

	//CREATE 2 TRIANGLE TOP FACE
	//T3A
	xCoordinates[24] = upperA[0];
	yCoordinates[24] = upperA[1];
	zCoordinates[24] = upperA[2];
	//T3B
	xCoordinates[25] = upperB[0];
	yCoordinates[25] = upperB[1];
	zCoordinates[25] = upperB[2];
	//T3C
	xCoordinates[26] = upperC[0];
	yCoordinates[26] = upperC[1];
	zCoordinates[26] = upperC[2];

	//T3A
	xCoordinates[27] = upperC[0];
	yCoordinates[27] = upperC[1];
	zCoordinates[27] = upperC[2];
	//T3B
	xCoordinates[28] = upperD[0];
	yCoordinates[28] = upperD[1];
	zCoordinates[28] = upperD[2];
	//T3C
	xCoordinates[29] = upperB[0];
	yCoordinates[29] = upperB[1];
	zCoordinates[29] = upperB[2];

	//CREATE 2 TRIANGLE BOTTOM FACE
	//T3A
	xCoordinates[30] = lowerA[0];
	yCoordinates[30] = lowerA[1];
	zCoordinates[30] = lowerA[2];
	//T3B
	xCoordinates[31] = lowerB[0];
	yCoordinates[31] = lowerB[1];
	zCoordinates[31] = lowerB[2];
	//T3C
	xCoordinates[32] = lowerC[0];
	yCoordinates[32] = lowerC[1];
	zCoordinates[32] = lowerC[2];

	//T3A
	xCoordinates[33] = lowerC[0];
	yCoordinates[33] = lowerC[1];
	zCoordinates[33] = lowerC[2];
	//T3B
	xCoordinates[34] = lowerD[0];
	yCoordinates[34] = lowerD[1];
	zCoordinates[34] = lowerD[2];
	//T3C
	xCoordinates[35] = lowerB[0];
	yCoordinates[35] = lowerB[1];
	zCoordinates[35] = lowerB[2];


	//CREATE upper RIGHT hand rod
	double leftA[3], leftB[3], leftC[3], leftD[3];
	double rightA[3], rightB[3], rightC[3], rightD[3];

	leftA[0] = center[0] + width/20;
	leftA[1] = center[1] + width/20;
	leftA[2] = center[2] + width/20;

	leftB[0] = center[0] + width/20;
	leftB[1] = leftA[1];
	leftB[2] = center[2] - width/20;

	leftC[0] = center[0] + width/20;
	leftC[1] = center[1] - width/20;
	leftC[2] = center[2] + width/20;

	leftD[0] = center[0] + width/20;
	leftD[1] = center[1] - width/20;
	leftD[2] = center[2] - width/20;

	rightA[0] = center[0] + width;
	rightA[1] = center[1] + width/20;
	rightA[2] = center[2] + width/20;

    rightB[0] = center[0] + width;
    rightB[1] = center[1] + width/20;
    rightB[2] = center[2] - width/20;

    rightC[0] = center[0] + width;
    rightC[1] = center[1] - width/20;
    rightC[2] = center[2] + width/20;

    rightD[0] = center[0] + width;
    rightD[1] = center[1] - width/20;
    rightD[2] = center[2] - width/20;

	/////top 1
	xCoordinates[36] = rightA[0];
	yCoordinates[36] = rightA[1];
	zCoordinates[36] = rightA[2];
	//T3B
	xCoordinates[37] = rightB[0];
	yCoordinates[37] = rightB[1];
	zCoordinates[37] = rightB[2];
	//T3C
	xCoordinates[38] = rightC[0];
	yCoordinates[38] = rightC[1];
	zCoordinates[38] = rightC[2];

	xCoordinates[39] = rightC[0];
	yCoordinates[39] = rightC[1];
	zCoordinates[39] = rightC[2];
	//T3B
	xCoordinates[40] = rightB[0];
	yCoordinates[40] = rightB[1];
	zCoordinates[40] = rightB[2];
	//T3C
	xCoordinates[41] = rightD[0];
	yCoordinates[41] = rightD[1];
	zCoordinates[41] = rightD[2];


	//rod 1 front
	//T3A
	xCoordinates[42] = leftA[0];
	yCoordinates[42] = leftA[1];
	zCoordinates[42] = leftA[2];
	//T3B
	xCoordinates[43] = rightA[0];
	yCoordinates[43] = rightA[1];
	zCoordinates[43] = rightA[2];
	//T3C
	xCoordinates[44] = leftC[0];
	yCoordinates[44] = leftC[1];
	zCoordinates[44] = leftC[2];

	//T3A
	xCoordinates[45] = rightA[0];
	yCoordinates[45] = rightA[1];
	zCoordinates[45] = rightA[2];
	//T3B
	xCoordinates[46] = rightC[0];
	yCoordinates[46] = rightC[1];
	zCoordinates[46] = rightC[2];
	//T3C
	xCoordinates[47] = leftC[0];
	yCoordinates[47] = leftC[1];
	zCoordinates[47] = leftC[2];

	//rod 1 back
	//T3A
	xCoordinates[48] = leftB[0];
	yCoordinates[48] = leftB[1];
	zCoordinates[48] = leftB[2];
	//T3B
	xCoordinates[49] = rightB[0];
	yCoordinates[49] = rightB[1];
	zCoordinates[49] = rightB[2];
	//T3C
	xCoordinates[50] = leftD[0];
	yCoordinates[50] = leftD[1];
	zCoordinates[50] = leftD[2];

	//T3A
	xCoordinates[51] = rightB[0];
	yCoordinates[51] = rightB[1];
	zCoordinates[51] = rightB[2];
	//T3B
	xCoordinates[52] = rightD[0];
	yCoordinates[52] = rightD[1];
	zCoordinates[52] = rightD[2];
	//T3C
	xCoordinates[53] = leftD[0];
	yCoordinates[53] = leftD[1];
	zCoordinates[53] = leftD[2];

	//rod 1 top
	//T3A
	xCoordinates[54] = leftB[0];
	yCoordinates[54] = leftB[1];
	zCoordinates[54] = leftB[2];
	//T3B
	xCoordinates[55] = rightB[0];
	yCoordinates[55] = rightB[1];
	zCoordinates[55] = rightB[2];
	//T3C
	xCoordinates[56] = leftA[0];
	yCoordinates[56] = leftA[1];
	zCoordinates[56] = leftA[2];

	//T3A
	xCoordinates[57] = rightB[0];
	yCoordinates[57] = rightB[1];
	zCoordinates[57] = rightB[2];
	//T3B
	xCoordinates[58] = rightA[0];
	yCoordinates[58] = rightA[1];
	zCoordinates[58] = rightA[2];
	//T3C
	xCoordinates[59] = leftA[0];
	yCoordinates[59] = leftA[1];
	zCoordinates[59] = leftA[2];

	//rod 1 bottom
	//T3A
	xCoordinates[60] = leftD[0];
	yCoordinates[60] = leftD[1];
	zCoordinates[60] = leftD[2];
	//T3B
	xCoordinates[61] = rightD[0];
	yCoordinates[61] = rightD[1];
	zCoordinates[61] = rightD[2];
	//T3C
	xCoordinates[62] = leftC[0];
	yCoordinates[62] = leftC[1];
	zCoordinates[62] = leftC[2];

	//T3A
	xCoordinates[63] = rightD[0];
	yCoordinates[63] = rightD[1];
	zCoordinates[63] = rightD[2];
	//T3B
	xCoordinates[64] = rightC[0];
	yCoordinates[64] = rightC[1];
	zCoordinates[64] = rightC[2];
	//T3C
	xCoordinates[65] = leftC[0];
	yCoordinates[65] = leftC[1];
	zCoordinates[65] = leftC[2];


}
