/*
 The MIT License (MIT)

 Copyright (c) 20 Sep 2016 Konstantinos Krestenitis

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

#include "dynamics.h"
#include <vector>
#include "math.h"
/*
The MIT License (MIT)

Copyright (c) 2015 Tomasz Koziara

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

/* vectorizable exponential map */
void expmap (iREAL Omega1, iREAL Omega2, iREAL Omega3,
                iREAL &Lambda1, iREAL &Lambda2, iREAL &Lambda3,
			          iREAL &Lambda4, iREAL &Lambda5, iREAL &Lambda6,
			          iREAL &Lambda7, iREAL &Lambda8, iREAL &Lambda9)
{
  iREAL angsq, sx, cx, v0, v1, v2, v01, v02, v12, s0, s1, s2;

  v0 = Omega1 * Omega1;
  v1 = Omega2 * Omega2;
  v2 = Omega3 * Omega3;

  angsq = v0 + v1 + v2;

  if (angsq < 3.0461741978671E-02) // use Taylor expansion if |Omega| < 10 deg
  {
    sx = 1.0 +
    (-1.666666666666667E-1 +
    (8.333333333333333E-3 +
    (-1.984126984126984E-4 +
    (2.755731922398589E-6 +
    (-2.505210838544172E-8 +
     1.605904383682161E-10 * angsq
    )*angsq
    )*angsq
    )*angsq
    )*angsq
    )*angsq;
  cx = 0.5 +
    (-4.166666666666667E-2 +
    (1.388888888888889E-3 +
    (-2.480158730158730E-5 +
    (2.755731922398589E-7 +
    (-2.087675698786810E-9 +
     1.147074559772972E-11 * angsq
    )*angsq
    )*angsq
    )*angsq
    )*angsq
    )*angsq;
  }
  else
  {
    iREAL t, s, c;
    t = angsq;
    angsq = sqrt (angsq);
    s = sin (angsq);
    c = cos (angsq);
    sx = s / angsq;
    cx = (1.0 - c) / t;
  }

  v01 = Omega1 * Omega2;
  v02 = Omega2 * Omega3;
  v12 = Omega2 * Omega3;
  s0 = sx * Omega1;
  s1 = sx * Omega2;
  s2 = sx * Omega3;

  Lambda1 = -cx*(v2+v1);
  Lambda2 = cx*v01;
  Lambda3 = cx*v02;
  Lambda4 = Lambda2;
  Lambda5 = -cx*(v2+v0);
  Lambda6 = cx*v12;
  Lambda7 = Lambda3;
  Lambda8 = Lambda6;
  Lambda9 = -cx*(v1+v0);
  Lambda1 += 1.0;
  Lambda2 += s2;
  Lambda3 -= s1;
  Lambda4 -= s2;
  Lambda5 += 1.0;
  Lambda6 += s0;
  Lambda7 += s1;
  Lambda8 -= s0;
  Lambda9 += 1.0;
}

 void delta::dynamics::updateRotationMatrix(iREAL *angular, iREAL *refAngular, iREAL *rotation, iREAL step)
 {
	iREAL DL[9];
 	expmap (step*angular[0], step*angular[1], step*angular[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);

 	//NNMUL (rotation, DL, rotation);
 	rotation[0] = rotation[0]*DL[0]+rotation[3]*DL[1]+rotation[6]*DL[2];
 	rotation[1] = rotation[1]*DL[0]+rotation[4]*DL[1]+rotation[7]*DL[2];
 	rotation[2] = rotation[2]*DL[0]+rotation[5]*DL[1]+rotation[8]*DL[2];
 	rotation[3] = rotation[0]*DL[3]+rotation[3]*DL[4]+rotation[6]*DL[5];
 	rotation[4] = rotation[1]*DL[3]+rotation[4]*DL[4]+rotation[7]*DL[5];
 	rotation[5] = rotation[2]*DL[3]+rotation[5]*DL[4]+rotation[8]*DL[5];
 	rotation[6] = rotation[0]*DL[6]+rotation[3]*DL[7]+rotation[6]*DL[8];
 	rotation[7] = rotation[1]*DL[6]+rotation[4]*DL[7]+rotation[7]*DL[8];
 	rotation[8] = rotation[2]*DL[6]+rotation[5]*DL[7]+rotation[8]*DL[8];

 	//NVMUL (rotation, angular, refangular);
 	refAngular[0] = rotation[0]*angular[0]+rotation[3]*angular[1]+rotation[6]*angular[2];
 	refAngular[1] = rotation[1]*angular[0]+rotation[4]*angular[1]+rotation[7]*angular[2];
 	refAngular[2] = rotation[2]*angular[0]+rotation[5]*angular[1]+rotation[8]*angular[2];
 }

void delta::dynamics::updateAngular(iREAL *angular, iREAL *refAngular, iREAL *rotation, iREAL *inertia, iREAL *inverse, iREAL mass, iREAL *torque, iREAL step)
{
	iREAL half = 0.5*step;

	iREAL T[3];
	iREAL DL[9];
	iREAL A[3];
	iREAL B[3];

	////EQUATION (13) START
	///////////////////////
	T[0] = rotation[0]*torque[0]+rotation[1]*torque[1]+rotation[2]*torque[2];
	T[1] = rotation[3]*torque[0]+rotation[4]*torque[1]+rotation[5]*torque[2];
	T[2] = rotation[6]*torque[0]+rotation[7]*torque[1]+rotation[8]*torque[2];
	////EQUATION (13) END
	/////////////////////

	////EQUATION (14) START
	///////////////////////
	expmap (-half*angular[0], -half*angular[1], -half*angular[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);

	//NVMUL (inertia, angular, A);
	A[0] = inertia[0]*angular[0]+inertia[3]*angular[1]+inertia[6]*angular[2];
	A[1] = inertia[1]*angular[0]+inertia[4]*angular[1]+inertia[7]*angular[2];
	A[2] = inertia[2]*angular[0]+inertia[5]*angular[1]+inertia[8]*angular[2];

	//NVMUL (DL, A, B);
	B[0] = DL[0]*A[0]+DL[3]*A[1]+DL[6]*A[2];
	B[1] = DL[1]*A[0]+DL[4]*A[1]+DL[7]*A[2];
	B[2] = DL[2]*A[0]+DL[5]*A[1]+DL[8]*A[2];

	//ADDMUL (B, half, T, B);
	B[0] = B[0] + half*T[0];
	B[1] = B[1] + half*T[1];
	B[2] = B[2] + half*T[2];

	//NVMUL (inverse, B, A); // O(t+h/2)
	A[0] = inverse[0]*B[0]+inverse[3]*B[1]+inverse[6]*B[2];
	A[1] = inverse[1]*B[0]+inverse[4]*B[1]+inverse[7]*B[2];
	A[2] = inverse[2]*B[0]+inverse[5]*B[1]+inverse[8]*B[2];

	////EQUATION (14) END
	/////////////////////

	////EQUATION (15) START
	///////////////////////
	//NVMUL (inertia, A, B);
	B[0] = inertia[0]*A[0]+inertia[3]*A[1]+inertia[6]*A[2];
	B[1] = inertia[1]*A[0]+inertia[4]*A[1]+inertia[7]*A[2];
	B[2] = inertia[2]*A[0]+inertia[5]*A[1]+inertia[8]*A[2];

	//PRODUCTSUB (A, B, T); // T - O(t+h/2) x J O(t+h/2)
	T[0] -= A[1]*B[2] - A[2]*B[1];
	T[1] -= A[2]*B[0] - A[0]*B[2];
	T[2] -= A[0]*B[1] - A[1]*B[0];

	//SCALE (T, step);
	T[0] *= step;
	T[1] *= step;
	T[2] *= step;

	//NVADDMUL (angular, inverse, T, angular); // O(t+h)
	angular[0] = angular[0] + inverse[0]*T[0]+inverse[3]*T[1]+inverse[6]*T[2];
	angular[1] = angular[1] + inverse[1]*T[0]+inverse[4]*T[1]+inverse[7]*T[2];
	angular[2] = angular[2] + inverse[2]*T[0]+inverse[5]*T[1]+inverse[8]*T[2];
	////EQUATION (15) END
	/////////////////////
}


void delta::dynamics::updateVertices(iREAL *x, iREAL *y, iREAL *z, iREAL *refx, iREAL *refy, iREAL *refz, iREAL *rotation, iREAL *position, iREAL *refposition)
{

	iREAL C[3], c[3];

	//point A REFERENCIAL
	C[0] = *refx - refposition[0];
	C[1] = *refy - refposition[1];
	C[2] = *refz - refposition[2];

	//SCC (refposition, C);

	c[0] = position[0] + (rotation[0]*(C)[0]+rotation[3]*C[1]+rotation[6]*C[2]);
	c[1] = position[1] + (rotation[1]*(C)[0]+rotation[4]*C[1]+rotation[7]*C[2]);
	c[2] = position[2] + (rotation[2]*(C)[0]+rotation[5]*C[1]+rotation[8]*C[2]);

	//point A SPATIAL
	*x = c[0];
	*y = c[1];
	*z = c[2];
}

