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

#include "surface.h"

#include <stdlib.h>
#include <assert.h>

void delta::primitives::generateSurface(
		double center[3],
		double width,
		double height,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates
)
{
	const int numberOfTriangles = 12;

	xCoordinates.resize( numberOfTriangles*3 );
	yCoordinates.resize( numberOfTriangles*3 );
	zCoordinates.resize( numberOfTriangles*3 );

	//CREATE FIRST TRIANGLE
	//T1A
	xCoordinates[0] = center[0]-width/2;
	yCoordinates[0] = center[1]-height/2;
	zCoordinates[0] = center[2]-width/2;
	//T1B
	xCoordinates[1] = center[0]-width/2;
	yCoordinates[1] = center[1]-height/2;
	zCoordinates[1] = center[2]+width/2;
	//T1C
	xCoordinates[2] = center[0]+width/2;
	yCoordinates[2] = center[1]-height/2;
	zCoordinates[2] = center[2]+width/2;

	//T2A
	xCoordinates[3] = center[0]-width/2;
	yCoordinates[3] = center[1]-height/2;
	zCoordinates[3] = center[2]-width/2;
	//T2B
	xCoordinates[4] = center[0]+width/2;
	yCoordinates[4] = center[1]-height/2;
	zCoordinates[4] = center[2]-width/2;
	//T2C
	xCoordinates[5] = center[0]+width/2;
	yCoordinates[5] = center[1]-height/2;
	zCoordinates[5] = center[2]+width/2;

	//////////////top////////////////

	xCoordinates[6] = center[0]-width/2;
	yCoordinates[6] = center[1]+height/2;
	zCoordinates[6] = center[2]-width/2;
	//T1B
	xCoordinates[7] = center[0]-width/2;
	yCoordinates[7] = center[1]+height/2;
	zCoordinates[7] = center[2]+width/2;
	//T1C
	xCoordinates[8] = center[0]+width/2;
	yCoordinates[8] = center[1]+height/2;
	zCoordinates[8] = center[2]+width/2;

	//T2A
	xCoordinates[9] = center[0]-width/2;
	yCoordinates[9] = center[1]+height/2;
	zCoordinates[9] = center[2]-width/2;
	//T2B
	xCoordinates[10] = center[0]+width/2;
	yCoordinates[10] = center[1]+height/2;
	zCoordinates[10] = center[2]-width/2;
	//T2C
	xCoordinates[11] = center[0]+width/2;
	yCoordinates[11] = center[1]+height/2;
	zCoordinates[11] = center[2]+width/2;

	/////////////FACE front////////////////

	//T1A
	xCoordinates[12] = center[0]-width/2;
	yCoordinates[12] = center[1]+height/2;
	zCoordinates[12] = center[2]+width/2;
	//T1B
	xCoordinates[13] = center[0]+width/2;
	yCoordinates[13] = center[1]+height/2;
	zCoordinates[13] = center[2]+width/2;
	//T1C
	xCoordinates[14] = center[0]+width/2;
	yCoordinates[14] = center[1]-height/2;
	zCoordinates[14] = center[2]+width/2;

	//T2A
	xCoordinates[15] = center[0]-width/2;
	yCoordinates[15] = center[1]+height/2;
	zCoordinates[15] = center[2]+width/2;
	//T2B
	xCoordinates[16] = center[0]-width/2;
	yCoordinates[16] = center[1]-height/2;
	zCoordinates[16] = center[2]+width/2;
	//T2C
	xCoordinates[17] = center[0]+width/2;
	yCoordinates[17] = center[1]-height/2;
	zCoordinates[17] = center[2]+width/2;

	/////////////FACE back////////////////

	//T1A
	xCoordinates[18] = center[0]-width/2;
	yCoordinates[18] = center[1]+height/2;
	zCoordinates[18] = center[2]-width/2;
	//T1B
	xCoordinates[19] = center[0]+width/2;
	yCoordinates[19] = center[1]+height/2;
	zCoordinates[19] = center[2]-width/2;
	//T1C
	xCoordinates[20] = center[0]+width/2;
	yCoordinates[20] = center[1]-height/2;
	zCoordinates[20] = center[2]-width/2;

	//T2A
	xCoordinates[21] = center[0]-width/2;
	yCoordinates[21] = center[1]+height/2;
	zCoordinates[21] = center[2]-width/2;
	//T2B
	xCoordinates[22] = center[0]-width/2;
	yCoordinates[22] = center[1]-height/2;
	zCoordinates[22] = center[2]-width/2;
	//T2C
	xCoordinates[23] = center[0]+width/2;
	yCoordinates[23] = center[1]-height/2;
	zCoordinates[23] = center[2]-width/2;

	///////////FACE right////////////////

	//T1A
	xCoordinates[24] = center[0]+width/2;
	yCoordinates[24] = center[1]+height/2;
	zCoordinates[24] = center[2]-width/2;
	//T1B
	xCoordinates[25] = center[0]+width/2;
	yCoordinates[25] = center[1]-height/2;
	zCoordinates[25] = center[2]-width/2;
	//T1C
	xCoordinates[26] = center[0]+width/2;
	yCoordinates[26] = center[1]+height/2;
	zCoordinates[26] = center[2]+width/2;

	//T2A
	xCoordinates[27] = center[0]+width/2;
	yCoordinates[27] = center[1]+height/2;
	zCoordinates[27] = center[2]+width/2;
	//T2B
	xCoordinates[28] = center[0]+width/2;
	yCoordinates[28] = center[1]-height/2;
	zCoordinates[28] = center[2]+width/2;
	//T2C
	xCoordinates[29] = center[0]+width/2;
	yCoordinates[29] = center[1]-height/2;
	zCoordinates[29] = center[2]-width/2;

	///////////FACE left////////////////

	//T1A
	xCoordinates[30] = center[0]-width/2;
	yCoordinates[30] = center[1]+height/2;
	zCoordinates[30] = center[2]-width/2;
	//T1B
	xCoordinates[31] = center[0]-width/2;
	yCoordinates[31] = center[1]-height/2;
	zCoordinates[31] = center[2]-width/2;
	//T1C
	xCoordinates[32] = center[0]-width/2;
	yCoordinates[32] = center[1]+height/2;
	zCoordinates[32] = center[2]+width/2;

	//T2A
	xCoordinates[33] = center[0]-width/2;
	yCoordinates[33] = center[1]+height/2;
	zCoordinates[33] = center[2]+width/2;
	//T2B
	xCoordinates[34] = center[0]-width/2;
	yCoordinates[34] = center[1]-height/2;
	zCoordinates[34] = center[2]+width/2;
	//T2C
	xCoordinates[35] = center[0]-width/2;
	yCoordinates[35] = center[1]-height/2;
	zCoordinates[35] = center[2]-width/2;

}


