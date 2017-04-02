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

	for(int i = 0; i<xCoordinates.size();i+=3)
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
