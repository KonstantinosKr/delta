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

	const int numberOfTriangles = 2;

	xCoordinates.resize( numberOfTriangles*3 );
	yCoordinates.resize( numberOfTriangles*3 );
	zCoordinates.resize( numberOfTriangles*3 );

	//CREATE FIRST TRIANGLE
	//T1A
	xCoordinates[0] = center[0]-width/2;
	yCoordinates[0] = center[1];
	zCoordinates[0] = center[2]-width/2;
	//T1B
	xCoordinates[1] = center[0]-width/2;
	yCoordinates[1] = center[1];
	zCoordinates[1] = center[2]+width/2;
	//T1C
	xCoordinates[2] = center[0]+width/2;
	yCoordinates[2] = center[1];
	zCoordinates[2] = center[2]+width/2;

	//T2A
	xCoordinates[3] = center[0]-width/2;
	yCoordinates[3] = center[1];
	zCoordinates[3] = center[2]-width/2;
	//T2B
	xCoordinates[4] = center[0]+width/2;
	yCoordinates[4] = center[1];
	zCoordinates[4] = center[2]-width/2;
	//T2C
	xCoordinates[5] = center[0]+width/2;
	yCoordinates[5] = center[1];
	zCoordinates[5] = center[2]+width/2;
}


