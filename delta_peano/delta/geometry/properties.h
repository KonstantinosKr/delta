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
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <array>
#include <stdio.h>
#include "delta/geometry/material.h"

namespace delta {
  namespace geometry {
    namespace properties {

      void moveMeshFromPositionToOrigin(
          iREAL center[3],
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void moveMeshFromOriginToPosition(
          iREAL center[3],
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void scaleXYZ(
          iREAL scale,
          iREAL position[3],
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void rotateX(
          iREAL alphaX,
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void rotateY(
          iREAL alphaY,
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void rotateZ(
          iREAL alphaZ,
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      iREAL getXYZWidth(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      iREAL getXZWidth(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      iREAL getXw(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      iREAL getYw(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      iREAL getZw(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      std::array<iREAL, 3> getMinBoundaryVertex(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      std::array<iREAL, 3> getMaxBoundaryVertex(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      iREAL computeDiagonal(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      iREAL getMaxXAxis(std::vector<iREAL>&  xCoordinates);
      iREAL getMaxYAxis(std::vector<iREAL>&  yCoordinates);
      iREAL getMaxZAxis(std::vector<iREAL>&  zCoordinates);

      iREAL getMinXAxis(std::vector<iREAL>&  xCoordinates);
      iREAL getMinYAxis(std::vector<iREAL>&  yCoordinates);
      iREAL getMinZAxis(std::vector<iREAL>&  zCoordinates);

      iREAL computeDistanceAB(std::array<iREAL, 3> A, std::array<iREAL, 3> B);

      void centerOfMass(
        std::vector<iREAL>&  xCoordinates,
        std::vector<iREAL>&  yCoordinates,
        std::vector<iREAL>&  zCoordinates,
        iREAL&               centreOfMassX,
        iREAL&               centreOfMassY,
        iREAL&               centreOfMassZ,
        iREAL&               refcentreOfMassX,
        iREAL&               refcentreOfMassY,
        iREAL&               refcentreOfMassZ);

      void explode(
        std::vector<iREAL>&  xCoordinates,
        std::vector<iREAL>&  yCoordinates,
        std::vector<iREAL>&  zCoordinates,
        iREAL length);

      void exploded(
        std::vector<iREAL>&  xCoordinates,
        std::vector<iREAL>&  yCoordinates,
        std::vector<iREAL>&  zCoordinates,
        iREAL length);

      void centerOfGeometry(
        iREAL centreOfGeometry[3],
        std::vector<iREAL>&  xCoordinates,
        std::vector<iREAL>&  yCoordinates,
        std::vector<iREAL>&  zCoordinates);

      iREAL getHMin(
        const std::vector<iREAL>&  xCoordinates,
        const std::vector<iREAL>&  yCoordinates,
        const std::vector<iREAL>&  zCoordinates);

      void getInertia(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates,
          delta::geometry::material::MaterialType material,
          iREAL& mass,
          iREAL center[3],
          iREAL inertia[9]);

      iREAL getMass(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates,
          delta::geometry::material::MaterialType material);

      iREAL getVolume(
          std::vector<iREAL>&  xCoordinates,
          std::vector<iREAL>&  yCoordinates,
          std::vector<iREAL>&  zCoordinates);

      void getInverseInertia(iREAL inertia[9], iREAL inverse[9], bool isObject);

      iREAL simplex_J (iREAL *a, iREAL *b, iREAL *c, iREAL *d);

      #define simplex_1(J, a, b, c, d) ((J)/6.)

      #define simplex_x(J, a, b, c, d) (((J)/24.)*((a) [0] + (b) [0] + (c) [0] + (d) [0]))

      #define simplex_y(J, a, b, c, d) (((J)/24.)*((a) [1] + (b) [1] + (c) [1] + (d) [1]))

      #define simplex_z(J, a, b, c, d) (((J)/24.)*((a) [2] + (b) [2] + (c) [2] + (d) [2]))

      #define simplex_xx(J, a, b, c, d)\
      (((J)/120.)*(\
      2.*(a) [0]*(a) [0] + (a) [0]*(b) [0] + (a) [0]*(c) [0] + (a) [0]*(d) [0] +\
      (b) [0]*(a) [0] + 2.*(b) [0]*(b) [0] + (b) [0]*(c) [0] + (b) [0]*(d) [0] +\
      (c) [0]*(a) [0] + (c) [0]*(b) [0] + 2.*(c) [0]*(c) [0] + (c) [0]*(d) [0] +\
      (d) [0]*(a) [0] + (d) [0]*(b) [0] + (d) [0]*(c) [0] + 2.*(d) [0]*(d) [0] \
      ))

      #define simplex_xy(J, a, b, c, d)\
      (((J)/120.)*(\
      2.*(a) [0]*(a) [1] + (a) [0]*(b) [1] + (a) [0]*(c) [1] + (a) [0]*(d) [1] +\
      (b) [0]*(a) [1] + 2.*(b) [0]*(b) [1] + (b) [0]*(c) [1] + (b) [0]*(d) [1] +\
      (c) [0]*(a) [1] + (c) [0]*(b) [1] + 2.*(c) [0]*(c) [1] + (c) [0]*(d) [1] +\
      (d) [0]*(a) [1] + (d) [0]*(b) [1] + (d) [0]*(c) [1] + 2.*(d) [0]*(d) [1] \
      ))

      #define simplex_xz(J, a, b, c, d)\
      (((J)/120.)*(\
      2.*(a) [0]*(a) [2] + (a) [0]*(b) [2] + (a) [0]*(c) [2] + (a) [0]*(d) [2] +\
      (b) [0]*(a) [2] + 2.*(b) [0]*(b) [2] + (b) [0]*(c) [2] + (b) [0]*(d) [2] +\
      (c) [0]*(a) [2] + (c) [0]*(b) [2] + 2.*(c) [0]*(c) [2] + (c) [0]*(d) [2] +\
      (d) [0]*(a) [2] + (d) [0]*(b) [2] + (d) [0]*(c) [2] + 2.*(d) [0]*(d) [2] \
      ))

      #define simplex_yy(J, a, b, c, d)\
      (((J)/120.)*(\
      2.*(a) [1]*(a) [1] + (a) [1]*(b) [1] + (a) [1]*(c) [1] + (a) [1]*(d) [1] +\
      (b) [1]*(a) [1] + 2.*(b) [1]*(b) [1] + (b) [1]*(c) [1] + (b) [1]*(d) [1] +\
      (c) [1]*(a) [1] + (c) [1]*(b) [1] + 2.*(c) [1]*(c) [1] + (c) [1]*(d) [1] +\
      (d) [1]*(a) [1] + (d) [1]*(b) [1] + (d) [1]*(c) [1] + 2.*(d) [1]*(d) [1] \
      ))

      #define simplex_yz(J, a, b, c, d)\
      (((J)/120.)*(\
      2.*(a) [1]*(a) [2] + (a) [1]*(b) [2] + (a) [1]*(c) [2] + (a) [1]*(d) [2] +\
      (b) [1]*(a) [2] + 2.*(b) [1]*(b) [2] + (b) [1]*(c) [2] + (b) [1]*(d) [2] +\
      (c) [1]*(a) [2] + (c) [1]*(b) [2] + 2.*(c) [1]*(c) [2] + (c) [1]*(d) [2] +\
      (d) [1]*(a) [2] + (d) [1]*(b) [2] + (d) [1]*(c) [2] + 2.*(d) [1]*(d) [2] \
      ))

      #define simplex_zz(J, a, b, c, d)\
      (((J)/120.)*(\
      2.*(a) [2]*(a) [2] + (a) [2]*(b) [2] + (a) [2]*(c) [2] + (a) [2]*(d) [2] +\
      (b) [2]*(a) [2] + 2.*(b) [2]*(b) [2] + (b) [2]*(c) [2] + (b) [2]*(d) [2] +\
      (c) [2]*(a) [2] + (c) [2]*(b) [2] + 2.*(c) [2]*(c) [2] + (c) [2]*(d) [2] +\
      (d) [2]*(a) [2] + (d) [2]*(b) [2] + (d) [2]*(c) [2] + 2.*(d) [2]*(d) [2] \
      ))
    }
  }
}
