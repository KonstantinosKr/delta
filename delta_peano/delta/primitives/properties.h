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
#include "delta/hull/alg.h"
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <array>
#include "delta/collision/material.h"

namespace delta {
  namespace primitives {
    namespace properties {

    void moveMeshFromPositionToOrigin(
        double center[3],
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

    void moveMeshFromOriginToPosition(
        double center[3],
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

    void scaleXYZ(double scale,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

    void scaleXYZ(double scale,
        double position[3],
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

    void rotateX(double alphaX,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

    void rotateY(double alphaY,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

    void rotateZ(double alphaZ,
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

    double computeXYZWidth(std::vector<double>&  xCoordinates,
              std::vector<double>&  yCoordinates,
              std::vector<double>&  zCoordinates);

    double computeXZWidth(std::vector<double>&  xCoordinates,
              std::vector<double>&  yCoordinates,
              std::vector<double>&  zCoordinates);

    double computeXw(std::vector<double>&  xCoordinates,
            std::vector<double>&  yCoordinates,
            std::vector<double>&  zCoordinates);

    double computeYw(std::vector<double>&  xCoordinates,
            std::vector<double>&  yCoordinates,
            std::vector<double>&  zCoordinates);

    double computeZw(std::vector<double>&  xCoordinates,
            std::vector<double>&  yCoordinates,
            std::vector<double>&  zCoordinates);

    std::array<double, 3> minBoundaryVertex(std::vector<double>&  xCoordinates,
                    std::vector<double>&  yCoordinates,
                    std::vector<double>&  zCoordinates);

    std::array<double, 3> maxBoundaryVertex(std::vector<double>&  xCoordinates,
                    std::vector<double>&  yCoordinates,
                    std::vector<double>&  zCoordinates);

    double computeDiagonal(std::vector<double>&  xCoordinates,
                     std::vector<double>&  yCoordinates,
                     std::vector<double>&  zCoordinates);

    double computeMaxXWidth(std::vector<double>&  xCoordinates);
    double computeMaxYWidth(std::vector<double>&  yCoordinates);
    double computeMaxZWidth(std::vector<double>&  zCoordinates);

    double getMaxXAxis(std::vector<double>&  xCoordinates);
    double getMaxYAxis(std::vector<double>&  yCoordinates);
    double getMaxZAxis(std::vector<double>&  zCoordinates);

    double getMinXAxis(std::vector<double>&  xCoordinates);
    double getMinYAxis(std::vector<double>&  yCoordinates);
    double getMinZAxis(std::vector<double>&  zCoordinates);

    double computeDistanceAB(std::array<double, 3> A, std::array<double, 3> B);

      void centerOfMass(
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates,
        double&               centreOfMassX,
        double&               centreOfMassY,
        double&               centreOfMassZ,
        double&               refcentreOfMassX,
        double&               refcentreOfMassY,
        double&               refcentreOfMassZ);

      void explode(
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates,
        double length);

      void exploded(
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates,
        double length);

      void centerOfGeometry(
        double centreOfGeometry[3],
        std::vector<double>&  xCoordinates,
        std::vector<double>&  yCoordinates,
        std::vector<double>&  zCoordinates);

      double computeHMin(
          const std::vector<double>&  xCoordinates,
          const std::vector<double>&  yCoordinates,
          const std::vector<double>&  zCoordinates);

      void computeInertia(
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates,
          delta::collision::material::MaterialType material,
          double& mass,
          double center[3],
          double inertia[9]);

      double computeMass(
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates,
          delta::collision::material::MaterialType material);

      double computeVolume(
          std::vector<double>&  xCoordinates,
          std::vector<double>&  yCoordinates,
          std::vector<double>&  zCoordinates);

      void computeInverseInertia(double inertia[9], double inverse[9], bool isObject);

      double simplex_J (double *a, double *b, double *c, double *d);

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

      double getKineticRotationalEnergy(double velocity[3], double angular[3], double inertia[9], double mass);

      double getKineticEnergy(double velocity[3], double mass);

      double getRotationalEnergy(double angular[3], double inertia[9]);
    }
  }
}
