/*
 The MIT License (MIT)

 Copyright (c) 2015 Konstantinos Krestenitis

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

#ifndef _DELTA_COLLISION_CONTACTPOINT_H_
#define _DELTA_COLLISION_CONTACTPOINT_H_

#include <string>
#include <cmath>

namespace delta {
  namespace collision {
    struct contactpoint;
  }
}

struct delta::collision::contactpoint {
  iREAL x[3];
  iREAL normal[3];
  iREAL depth;
  iREAL P[3];
  iREAL Q[3];
  iREAL friction[3];
  int frictionType;
  double epsilonTotal;

  contactpoint();
  contactpoint(const contactpoint& copy);

  /**
   * This constructor is given two points on two triangles that are close to
   * each other. The points are PA and PB. The operation determines the
   * contact point at x,y,z (which is half the distance between the two
   * points PA and PB) and the corresponding normal xN,yN,zN.
   *
   * Please note that the field particleNumber is not set.
   */
  contactpoint(
    const double&  xPA,
    const double&  yPA,
    const double&  zPA,
	const double&  epsilonA,

    const double&  xPB,
    const double&  yPB,
    const double&  zPB,
	const double&  epsilonB,
	const int&     type
  );

  /**
   * Get distance from contact point to next triangle.
   */
  double getDistance() const;

  std::string toString() const;
};

#endif
