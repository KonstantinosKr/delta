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


#include <vector>
#include <math.h>


namespace delta {
  namespace primitives {

    void centerOfMass(
      const std::vector<double>&  xCoordinates,
      const std::vector<double>&  yCoordinates,
      double&                     mass,
      double&                     centreOfMassX,
      double&                     centreOfMassY
    );

    void centerOfMass(
      std::vector<double>&  xCoordinates,
      std::vector<double>&  yCoordinates,
      std::vector<double>&  zCoordinates,
      double&                     centreOfMassX,
      double&                     centreOfMassY,
      double&                     centreOfMassZ,
	  double&                     refcentreOfMassX,
	  double&                     refcentreOfMassY,
	  double&                     refcentreOfMassZ
    );

    double computeHMin(
      const std::vector<double>&  xCoordinates,
      const std::vector<double>&  yCoordinates,
      const std::vector<double>&  zCoordinates
    );
  }
}