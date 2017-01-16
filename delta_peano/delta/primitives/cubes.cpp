#include "cubes.h"

#include <cmath>
#include <stdlib.h>
#include <assert.h>


void delta::primitives::generateSquare(
  double center[2],
  double h,
  double rotationAngle,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates
) {
  assert(xCoordinates.empty());
  assert(yCoordinates.empty());

  const double pi                = std::acos(-1);
  const int    numberOfTriangles = 4;

  xCoordinates.resize( 2*numberOfTriangles );
  yCoordinates.resize( 2*numberOfTriangles );

  xCoordinates[7] = center[0] + h/2.0 * std::cos(2.0*pi * (0+rotationAngle) );
  yCoordinates[7] = center[1] + h/2.0 * std::sin(2.0*pi * (0+rotationAngle) );

  xCoordinates[0] = center[0] + h/2.0 * std::cos(2.0*pi * (0+rotationAngle) );
  yCoordinates[0] = center[1] + h/2.0 * std::sin(2.0*pi * (0+rotationAngle) );

  xCoordinates[1] = center[0] + h/2.0 * std::cos(2.0*pi * (0.25+rotationAngle) );
  yCoordinates[1] = center[1] + h/2.0 * std::sin(2.0*pi * (0.25+rotationAngle) );

  xCoordinates[2] = center[0] + h/2.0 * std::cos(2.0*pi * (0.25+rotationAngle) );
  yCoordinates[2] = center[1] + h/2.0 * std::sin(2.0*pi * (0.25+rotationAngle) );

  xCoordinates[3] = center[0] + h/2.0 * std::cos(2.0*pi * (0.50+rotationAngle) );
  yCoordinates[3] = center[1] + h/2.0 * std::sin(2.0*pi * (0.50+rotationAngle) );

  xCoordinates[4] = center[0] + h/2.0 * std::cos(2.0*pi * (0.50+rotationAngle) );
  yCoordinates[4] = center[1] + h/2.0 * std::sin(2.0*pi * (0.50+rotationAngle) );

  xCoordinates[5] = center[0] + h/2.0 * std::cos(2.0*pi * (0.75+rotationAngle) );
  yCoordinates[5] = center[1] + h/2.0 * std::sin(2.0*pi * (0.75+rotationAngle) );

  xCoordinates[6] = center[0] + h/2.0 * std::cos(2.0*pi * (0.75+rotationAngle) );
  yCoordinates[6] = center[1] + h/2.0 * std::sin(2.0*pi * (0.75+rotationAngle) );
}


void delta::primitives::generateCube(
  double center[3], double h,
  double alphaX,
  double alphaY,
  double alphaZ,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates
) {
  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());

  const int numberOfTriangles = 12;

  const double pi = std::acos(-1);

  xCoordinates.resize( numberOfTriangles*3 );
  yCoordinates.resize( numberOfTriangles*3 );
  zCoordinates.resize( numberOfTriangles*3 );

  xCoordinates[0] = - h*std::sqrt(1.0/12.0);
  yCoordinates[0] = - h*std::sqrt(1.0/12.0);
  zCoordinates[0] = - h*std::sqrt(1.0/12.0);

  xCoordinates[1] = + h*std::sqrt(1.0/12.0);
  yCoordinates[1] = - h*std::sqrt(1.0/12.0);
  zCoordinates[1] = - h*std::sqrt(1.0/12.0);

  xCoordinates[2] = - h*std::sqrt(1.0/12.0);
  yCoordinates[2] = + h*std::sqrt(1.0/12.0);
  zCoordinates[2] = - h*std::sqrt(1.0/12.0);

  xCoordinates[3] = + h*std::sqrt(1.0/12.0);
  yCoordinates[3] = - h*std::sqrt(1.0/12.0);
  zCoordinates[3] = - h*std::sqrt(1.0/12.0);

  xCoordinates[4] = - h*std::sqrt(1.0/12.0);
  yCoordinates[4] = + h*std::sqrt(1.0/12.0);
  zCoordinates[4] = - h*std::sqrt(1.0/12.0);

  xCoordinates[5] = + h*std::sqrt(1.0/12.0);
  yCoordinates[5] = + h*std::sqrt(1.0/12.0);
  zCoordinates[5] = - h*std::sqrt(1.0/12.0);

  xCoordinates[6] = - h*std::sqrt(1.0/12.0);
  yCoordinates[6] = + h*std::sqrt(1.0/12.0);
  zCoordinates[6] = - h*std::sqrt(1.0/12.0);

  xCoordinates[7] = + h*std::sqrt(1.0/12.0);
  yCoordinates[7] = + h*std::sqrt(1.0/12.0);
  zCoordinates[7] = - h*std::sqrt(1.0/12.0);

  xCoordinates[8] = - h*std::sqrt(1.0/12.0);
  yCoordinates[8] = + h*std::sqrt(1.0/12.0);
  zCoordinates[8] = + h*std::sqrt(1.0/12.0);

  xCoordinates[9] = - h*std::sqrt(1.0/12.0);
  yCoordinates[9] = + h*std::sqrt(1.0/12.0);
  zCoordinates[9] = + h*std::sqrt(1.0/12.0);

  xCoordinates[10] = + h*std::sqrt(1.0/12.0);
  yCoordinates[10] = + h*std::sqrt(1.0/12.0);
  zCoordinates[10] = - h*std::sqrt(1.0/12.0);

  xCoordinates[11] = + h*std::sqrt(1.0/12.0);
  yCoordinates[11] = + h*std::sqrt(1.0/12.0);
  zCoordinates[11] = + h*std::sqrt(1.0/12.0);

  xCoordinates[12] = + h*std::sqrt(1.0/12.0);
  yCoordinates[12] = - h*std::sqrt(1.0/12.0);
  zCoordinates[12] = - h*std::sqrt(1.0/12.0);

  xCoordinates[13] = + h*std::sqrt(1.0/12.0);
  yCoordinates[13] = + h*std::sqrt(1.0/12.0);
  zCoordinates[13] = - h*std::sqrt(1.0/12.0);

  xCoordinates[14] = + h*std::sqrt(1.0/12.0);
  yCoordinates[14] = + h*std::sqrt(1.0/12.0);
  zCoordinates[14] = + h*std::sqrt(1.0/12.0);

  xCoordinates[15] = + h*std::sqrt(1.0/12.0);
  yCoordinates[15] = + h*std::sqrt(1.0/12.0);
  zCoordinates[15] = + h*std::sqrt(1.0/12.0);

  xCoordinates[16] = + h*std::sqrt(1.0/12.0);
  yCoordinates[16] = - h*std::sqrt(1.0/12.0);
  zCoordinates[16] = - h*std::sqrt(1.0/12.0);

  xCoordinates[17] = + h*std::sqrt(1.0/12.0);
  yCoordinates[17] = - h*std::sqrt(1.0/12.0);
  zCoordinates[17] = + h*std::sqrt(1.0/12.0);

  // Now just translated other faces
  xCoordinates[18] = - h*std::sqrt(1.0/12.0);
  yCoordinates[18] = - h*std::sqrt(1.0/12.0);
  zCoordinates[18] = + h*std::sqrt(1.0/12.0);

  xCoordinates[19] = + h*std::sqrt(1.0/12.0);
  yCoordinates[19] = - h*std::sqrt(1.0/12.0);
  zCoordinates[19] = + h*std::sqrt(1.0/12.0);

  xCoordinates[20] = - h*std::sqrt(1.0/12.0);
  yCoordinates[20] = + h*std::sqrt(1.0/12.0);
  zCoordinates[20] = + h*std::sqrt(1.0/12.0);

  xCoordinates[21] = + h*std::sqrt(1.0/12.0);
  yCoordinates[21] = - h*std::sqrt(1.0/12.0);
  zCoordinates[21] = + h*std::sqrt(1.0/12.0);

  xCoordinates[22] = - h*std::sqrt(1.0/12.0);
  yCoordinates[22] = + h*std::sqrt(1.0/12.0);
  zCoordinates[22] = + h*std::sqrt(1.0/12.0);

  xCoordinates[23] = + h*std::sqrt(1.0/12.0);
  yCoordinates[23] = + h*std::sqrt(1.0/12.0);
  zCoordinates[23] = + h*std::sqrt(1.0/12.0);

  xCoordinates[24] = - h*std::sqrt(1.0/12.0);
  yCoordinates[24] = - h*std::sqrt(1.0/12.0);
  zCoordinates[24] = - h*std::sqrt(1.0/12.0);

  xCoordinates[25] = + h*std::sqrt(1.0/12.0);
  yCoordinates[25] = - h*std::sqrt(1.0/12.0);
  zCoordinates[25] = - h*std::sqrt(1.0/12.0);

  xCoordinates[26] = - h*std::sqrt(1.0/12.0);
  yCoordinates[26] = - h*std::sqrt(1.0/12.0);
  zCoordinates[26] = + h*std::sqrt(1.0/12.0);

  xCoordinates[27] = - h*std::sqrt(1.0/12.0);
  yCoordinates[27] = - h*std::sqrt(1.0/12.0);
  zCoordinates[27] = + h*std::sqrt(1.0/12.0);

  xCoordinates[28] = + h*std::sqrt(1.0/12.0);
  yCoordinates[28] = - h*std::sqrt(1.0/12.0);
  zCoordinates[28] = - h*std::sqrt(1.0/12.0);

  xCoordinates[29] = + h*std::sqrt(1.0/12.0);
  yCoordinates[29] = - h*std::sqrt(1.0/12.0);
  zCoordinates[29] = + h*std::sqrt(1.0/12.0);

  xCoordinates[30] = - h*std::sqrt(1.0/12.0);
  yCoordinates[30] = - h*std::sqrt(1.0/12.0);
  zCoordinates[30] = - h*std::sqrt(1.0/12.0);

  xCoordinates[31] = - h*std::sqrt(1.0/12.0);
  yCoordinates[31] = + h*std::sqrt(1.0/12.0);
  zCoordinates[31] = - h*std::sqrt(1.0/12.0);

  xCoordinates[32] = - h*std::sqrt(1.0/12.0);
  yCoordinates[32] = + h*std::sqrt(1.0/12.0);
  zCoordinates[32] = + h*std::sqrt(1.0/12.0);

  xCoordinates[33] = - h*std::sqrt(1.0/12.0);
  yCoordinates[33] = + h*std::sqrt(1.0/12.0);
  zCoordinates[33] = + h*std::sqrt(1.0/12.0);

  xCoordinates[34] = - h*std::sqrt(1.0/12.0);
  yCoordinates[34] = - h*std::sqrt(1.0/12.0);
  zCoordinates[34] = - h*std::sqrt(1.0/12.0);

  xCoordinates[35] = - h*std::sqrt(1.0/12.0);
  yCoordinates[35] = - h*std::sqrt(1.0/12.0);
  zCoordinates[35] = + h*std::sqrt(1.0/12.0);


  for (int i=0; i<36; i++) {
    double x = xCoordinates[i];
    double y = yCoordinates[i];
    double z = zCoordinates[i];

    double M[] = {
	   1.0,                 0.0,                   0.0,
	   0.0,  std::cos(2*pi*alphaX),  std::sin(2*pi*alphaX),
	   0.0, -std::sin(2*pi*alphaX),  std::cos(2*pi*alphaX)
    };

    xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
    yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
    zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  for (int i=0; i<36; i++) {
    double x = xCoordinates[i];
    double y = yCoordinates[i];
    double z = zCoordinates[i];

    double M[] = {
      std::cos(2*pi*alphaY),  0.0, std::sin(2*pi*alphaY),
      0.0,                    1.0,                   0.0,
     -std::sin(2*pi*alphaY),  0.0, std::cos(2*pi*alphaY)
    };

    xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
    yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
    zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  for (int i=0; i<36; i++) {
    double x = xCoordinates[i];
    double y = yCoordinates[i];
    double z = zCoordinates[i];

    double M[] = {
      std::cos(2*pi*alphaZ),  std::sin(2*pi*alphaZ),  0.0,
     -std::sin(2*pi*alphaZ),  std::cos(2*pi*alphaZ),  0.0,
                       0.0,                   0.0,  1.0
    };

    xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
    yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
    zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  for (int i=0; i<36; i++) {
    xCoordinates[i] += center[0];
    yCoordinates[i] += center[1];
    zCoordinates[i] += center[2];
  }
}
