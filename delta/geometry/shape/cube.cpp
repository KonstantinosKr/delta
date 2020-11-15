#include <geometry/shape/cube.h>

void delta::geometry::primitive::cube::generateSquare(
  iREAL center[2],
  iREAL h,
  iREAL rotationAngle,
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates
) {
  assert(xCoordinates.empty());
  assert(yCoordinates.empty());

  const iREAL pi                = std::acos(-1);
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

delta::geometry::mesh::Mesh *delta::geometry::primitive::cube::generateCube(
  iREAL center[3],
  iREAL h,
  iREAL alphaX,
  iREAL alphaY,
  iREAL alphaZ
) {
  std::vector<iREAL>	xCoordinates;
  std::vector<iREAL>	yCoordinates;
  std::vector<iREAL>	zCoordinates;

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());

  const int numberOfTriangles = 12;

  const iREAL pi = std::acos(-1);

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
    iREAL x = xCoordinates[i];
    iREAL y = yCoordinates[i];
    iREAL z = zCoordinates[i];

    iREAL M[] = {
	   1.0,                 0.0,                   0.0,
	   0.0,  std::cos(2*pi*alphaX),  std::sin(2*pi*alphaX),
	   0.0, -std::sin(2*pi*alphaX),  std::cos(2*pi*alphaX)
    };

    xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
    yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
    zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  for (int i=0; i<36; i++) {
    iREAL x = xCoordinates[i];
    iREAL y = yCoordinates[i];
    iREAL z = zCoordinates[i];

    iREAL M[] = {
      std::cos(2*pi*alphaY),  0.0, std::sin(2*pi*alphaY),
      0.0,                    1.0,                   0.0,
     -std::sin(2*pi*alphaY),  0.0, std::cos(2*pi*alphaY)
    };

    xCoordinates[i] =   M[0] * x + M[1] * y + M[2] * z;
    yCoordinates[i] =   M[3] * x + M[4] * y + M[5] * z;
    zCoordinates[i] =   M[6] * x + M[7] * y + M[8] * z;
  }

  for (int i=0; i<36; i++) {
    iREAL x = xCoordinates[i];
    iREAL y = yCoordinates[i];
    iREAL z = zCoordinates[i];

    iREAL M[] = {
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

  delta::geometry::mesh::Mesh *mesh = new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::primitive::cube::generateCube(
  iREAL center[3],
  iREAL x,
  iREAL y,
  iREAL z,
  iREAL alphaX,
  iREAL alphaY,
  iREAL alphaZ
) {
  std::vector<iREAL>	xCoordinates;
  std::vector<iREAL>	yCoordinates;
  std::vector<iREAL>	zCoordinates;

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());

  const int numberOfTriangles = 12;

  const iREAL pi = std::acos(-1);

  xCoordinates.resize( numberOfTriangles*3 );
  yCoordinates.resize( numberOfTriangles*3 );
  zCoordinates.resize( numberOfTriangles*3 );

  xCoordinates[0] = - x*std::sqrt(1.0/12.0);
  yCoordinates[0] = - y*std::sqrt(1.0/12.0);
  zCoordinates[0] = - z*std::sqrt(1.0/12.0);

  xCoordinates[1] = + x*std::sqrt(1.0/12.0);
  yCoordinates[1] = - y*std::sqrt(1.0/12.0);
  zCoordinates[1] = - z*std::sqrt(1.0/12.0);

  xCoordinates[2] = - x*std::sqrt(1.0/12.0);
  yCoordinates[2] = + y*std::sqrt(1.0/12.0);
  zCoordinates[2] = - z*std::sqrt(1.0/12.0);

  xCoordinates[3] = + x*std::sqrt(1.0/12.0);
  yCoordinates[3] = - y*std::sqrt(1.0/12.0);
  zCoordinates[3] = - z*std::sqrt(1.0/12.0);

  xCoordinates[4] = - x*std::sqrt(1.0/12.0);
  yCoordinates[4] = + y*std::sqrt(1.0/12.0);
  zCoordinates[4] = - z*std::sqrt(1.0/12.0);

  xCoordinates[5] = + x*std::sqrt(1.0/12.0);
  yCoordinates[5] = + y*std::sqrt(1.0/12.0);
  zCoordinates[5] = - z*std::sqrt(1.0/12.0);

  xCoordinates[6] = - x*std::sqrt(1.0/12.0);
  yCoordinates[6] = + y*std::sqrt(1.0/12.0);
  zCoordinates[6] = - z*std::sqrt(1.0/12.0);

  xCoordinates[7] = + x*std::sqrt(1.0/12.0);
  yCoordinates[7] = + y*std::sqrt(1.0/12.0);
  zCoordinates[7] = - z*std::sqrt(1.0/12.0);

  xCoordinates[8] = - x*std::sqrt(1.0/12.0);
  yCoordinates[8] = + y*std::sqrt(1.0/12.0);
  zCoordinates[8] = + z*std::sqrt(1.0/12.0);

  xCoordinates[9] = - x*std::sqrt(1.0/12.0);
  yCoordinates[9] = + y*std::sqrt(1.0/12.0);
  zCoordinates[9] = + z*std::sqrt(1.0/12.0);

  xCoordinates[10] = + x*std::sqrt(1.0/12.0);
  yCoordinates[10] = + y*std::sqrt(1.0/12.0);
  zCoordinates[10] = - z*std::sqrt(1.0/12.0);

  xCoordinates[11] = + x*std::sqrt(1.0/12.0);
  yCoordinates[11] = + y*std::sqrt(1.0/12.0);
  zCoordinates[11] = + z*std::sqrt(1.0/12.0);

  xCoordinates[12] = + x*std::sqrt(1.0/12.0);
  yCoordinates[12] = - y*std::sqrt(1.0/12.0);
  zCoordinates[12] = - z*std::sqrt(1.0/12.0);

  xCoordinates[13] = + x*std::sqrt(1.0/12.0);
  yCoordinates[13] = + y*std::sqrt(1.0/12.0);
  zCoordinates[13] = - z*std::sqrt(1.0/12.0);

  xCoordinates[14] = + x*std::sqrt(1.0/12.0);
  yCoordinates[14] = + y*std::sqrt(1.0/12.0);
  zCoordinates[14] = + z*std::sqrt(1.0/12.0);

  xCoordinates[15] = + x*std::sqrt(1.0/12.0);
  yCoordinates[15] = + y*std::sqrt(1.0/12.0);
  zCoordinates[15] = + z*std::sqrt(1.0/12.0);

  xCoordinates[16] = + x*std::sqrt(1.0/12.0);
  yCoordinates[16] = - y*std::sqrt(1.0/12.0);
  zCoordinates[16] = - z*std::sqrt(1.0/12.0);

  xCoordinates[17] = + x*std::sqrt(1.0/12.0);
  yCoordinates[17] = - y*std::sqrt(1.0/12.0);
  zCoordinates[17] = + z*std::sqrt(1.0/12.0);

  // Now just translated other faces
  xCoordinates[18] = - x*std::sqrt(1.0/12.0);
  yCoordinates[18] = - y*std::sqrt(1.0/12.0);
  zCoordinates[18] = + z*std::sqrt(1.0/12.0);

  xCoordinates[19] = + x*std::sqrt(1.0/12.0);
  yCoordinates[19] = - y*std::sqrt(1.0/12.0);
  zCoordinates[19] = + z*std::sqrt(1.0/12.0);

  xCoordinates[20] = - x*std::sqrt(1.0/12.0);
  yCoordinates[20] = + y*std::sqrt(1.0/12.0);
  zCoordinates[20] = + z*std::sqrt(1.0/12.0);

  xCoordinates[21] = + x*std::sqrt(1.0/12.0);
  yCoordinates[21] = - y*std::sqrt(1.0/12.0);
  zCoordinates[21] = + z*std::sqrt(1.0/12.0);

  xCoordinates[22] = - x*std::sqrt(1.0/12.0);
  yCoordinates[22] = + y*std::sqrt(1.0/12.0);
  zCoordinates[22] = + z*std::sqrt(1.0/12.0);

  xCoordinates[23] = + x*std::sqrt(1.0/12.0);
  yCoordinates[23] = + y*std::sqrt(1.0/12.0);
  zCoordinates[23] = + z*std::sqrt(1.0/12.0);

  xCoordinates[24] = - x*std::sqrt(1.0/12.0);
  yCoordinates[24] = - y*std::sqrt(1.0/12.0);
  zCoordinates[24] = - z*std::sqrt(1.0/12.0);

  xCoordinates[25] = + x*std::sqrt(1.0/12.0);
  yCoordinates[25] = - y*std::sqrt(1.0/12.0);
  zCoordinates[25] = - z*std::sqrt(1.0/12.0);

  xCoordinates[26] = - x*std::sqrt(1.0/12.0);
  yCoordinates[26] = - y*std::sqrt(1.0/12.0);
  zCoordinates[26] = + z*std::sqrt(1.0/12.0);

  xCoordinates[27] = - x*std::sqrt(1.0/12.0);
  yCoordinates[27] = - y*std::sqrt(1.0/12.0);
  zCoordinates[27] = + z*std::sqrt(1.0/12.0);

  xCoordinates[28] = + x*std::sqrt(1.0/12.0);
  yCoordinates[28] = - y*std::sqrt(1.0/12.0);
  zCoordinates[28] = - z*std::sqrt(1.0/12.0);

  xCoordinates[29] = + x*std::sqrt(1.0/12.0);
  yCoordinates[29] = - y*std::sqrt(1.0/12.0);
  zCoordinates[29] = + z*std::sqrt(1.0/12.0);

  xCoordinates[30] = - x*std::sqrt(1.0/12.0);
  yCoordinates[30] = - y*std::sqrt(1.0/12.0);
  zCoordinates[30] = - z*std::sqrt(1.0/12.0);

  xCoordinates[31] = - x*std::sqrt(1.0/12.0);
  yCoordinates[31] = + y*std::sqrt(1.0/12.0);
  zCoordinates[31] = - z*std::sqrt(1.0/12.0);

  xCoordinates[32] = - x*std::sqrt(1.0/12.0);
  yCoordinates[32] = + y*std::sqrt(1.0/12.0);
  zCoordinates[32] = + z*std::sqrt(1.0/12.0);

  xCoordinates[33] = - x*std::sqrt(1.0/12.0);
  yCoordinates[33] = + y*std::sqrt(1.0/12.0);
  zCoordinates[33] = + z*std::sqrt(1.0/12.0);

  xCoordinates[34] = - x*std::sqrt(1.0/12.0);
  yCoordinates[34] = - y*std::sqrt(1.0/12.0);
  zCoordinates[34] = - z*std::sqrt(1.0/12.0);

  xCoordinates[35] = - x*std::sqrt(1.0/12.0);
  yCoordinates[35] = - y*std::sqrt(1.0/12.0);
  zCoordinates[35] = + z*std::sqrt(1.0/12.0);


  for (int i=0; i<36; i++) {
    iREAL xc = xCoordinates[i];
    iREAL yc = yCoordinates[i];
    iREAL zc = zCoordinates[i];

    iREAL M[] = {
     1.0,                 0.0,                   0.0,
     0.0,  std::cos(2*pi*alphaX),  std::sin(2*pi*alphaX),
     0.0, -std::sin(2*pi*alphaX),  std::cos(2*pi*alphaX)
    };

    xCoordinates[i] =   M[0] * xc + M[1] * yc + M[2] * zc;
    yCoordinates[i] =   M[3] * xc + M[4] * yc + M[5] * zc;
    zCoordinates[i] =   M[6] * xc + M[7] * yc + M[8] * zc;
  }

  for (int i=0; i<36; i++) {
    iREAL xc = xCoordinates[i];
    iREAL yc = yCoordinates[i];
    iREAL zc = zCoordinates[i];

    iREAL M[] = {
      std::cos(2*pi*alphaY),  0.0, std::sin(2*pi*alphaY),
      0.0,                    1.0,                   0.0,
     -std::sin(2*pi*alphaY),  0.0, std::cos(2*pi*alphaY)
    };

    xCoordinates[i] =   M[0] * xc + M[1] * yc + M[2] * zc;
    yCoordinates[i] =   M[3] * xc + M[4] * yc + M[5] * zc;
    zCoordinates[i] =   M[6] * xc + M[7] * yc + M[8] * zc;
  }

  for (int i=0; i<36; i++) {
    iREAL xc = xCoordinates[i];
    iREAL yc = yCoordinates[i];
    iREAL zc = zCoordinates[i];

    iREAL M[] = {
      std::cos(2*pi*alphaZ),  std::sin(2*pi*alphaZ),  0.0,
     -std::sin(2*pi*alphaZ),  std::cos(2*pi*alphaZ),  0.0,
                       0.0,                   0.0,  1.0
    };

    xCoordinates[i] =   M[0] * xc + M[1] * yc + M[2] * zc;
    yCoordinates[i] =   M[3] * xc + M[4] * yc + M[5] * zc;
    zCoordinates[i] =   M[6] * xc + M[7] * yc + M[8] * zc;
  }

  for (int i=0; i<36; i++) {
    xCoordinates[i] += center[0];
    yCoordinates[i] += center[1];
    zCoordinates[i] += center[2];
  }

  delta::geometry::mesh::Mesh *mesh = new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::primitive::cube::generateHullCube(
  iREAL  center[3],
  iREAL  diagonal)
{
  std::vector<iREAL>	xCoordinates;
  std::vector<iREAL>	yCoordinates;
  std::vector<iREAL>	zCoordinates;

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());

  unsigned int mul=1E8;

  int pointsize = 8;

  iREAL v[100000][3];

  iREAL h = std::sqrt(1.0/12.0)*diagonal;

  //0
  v[0][0] = (- h) * mul;
  v[0][1] = (- h) * mul;
  v[0][2] = (+ h) * mul;

  //1
  v[1][0] = (+ h) * mul;
  v[1][1] = (- h) * mul;
  v[1][2] = (+ h) * mul;

  //2
  v[2][0] = (- h) * mul;
  v[2][1] = (+ h) * mul;
  v[2][2] = (+ h) * mul;

  //3
  v[3][0] = (+ h) * mul;
  v[3][1] = (+ h) * mul;
  v[3][2] = (+ h) * mul;

  //4
  v[4][0] = (- h) * mul;
  v[4][1] = (- h) * mul;
  v[4][2] = (- h) * mul;

  //5
  v[5][0] = (+ h) * mul;
  v[5][1] = (- h) * mul;
  v[5][2] = (- h) * mul;

  //6
  v[6][0] = (- h) * mul;
  v[6][1] = (+ h) * mul;
  v[6][2] = (- h) * mul;

  //7
  v[7][0] = (+ h) * mul;
  v[7][1] = (+ h) * mul;
  v[7][2] = (- h) * mul;


  delta::hull::GEOMETRIC_EPSILON = 1e-10;
  delta::hull::TRI* tr = NULL;
  int pointlength = 0;
  tr = delta::hull::hull((iREAL *)v, pointsize, &pointlength);

  const int numberOfTriangles = pointlength;

  xCoordinates.resize(numberOfTriangles*3);
  yCoordinates.resize(numberOfTriangles*3);
  zCoordinates.resize(numberOfTriangles*3);

  int counter = 0;
  //#pragma omp parallel for
  for(delta::hull::TRI *t = tr, *e = t + pointlength; t < e; t ++)
  {//iterate through triangles and assign value
    xCoordinates[counter] = (t->ver [0][0]/(mul)) + center[0];
    yCoordinates[counter] = (t->ver [0][1]/(mul)) + center[1];
    zCoordinates[counter] = (t->ver [0][2]/(mul)) + center[2];
    
    //#pragma omp atomic
    counter++;

    xCoordinates[counter] = (t->ver [1][0]/(mul)) + center[0];
    yCoordinates[counter] = (t->ver [1][1]/(mul)) + center[1];
    zCoordinates[counter] = (t->ver [1][2]/(mul)) + center[2];
    
    //#pragma omp atomic
    counter++;

    xCoordinates[counter] = (t->ver [2][0]/(mul)) + center[0];
    yCoordinates[counter] = (t->ver [2][1]/(mul)) + center[1];
    zCoordinates[counter] = (t->ver [2][2]/(mul)) + center[2];
    
    //#pragma omp atomic
    counter++;
  }
  free(tr);

  delta::geometry::mesh::Mesh *mesh = new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::primitive::cube::generateHullCube(
        iREAL center[3],
        iREAL x,
        iREAL y,
        iREAL z,
        iREAL alphaX,
        iREAL alphaY,
        iREAL alphaZ,
        int   meshmultiplier)
{
  std::vector<iREAL>	xCoordinates;
  std::vector<iREAL>	yCoordinates;
  std::vector<iREAL>	zCoordinates;

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());

  unsigned int mul=1E8;

  int pointsize = 8;

  iREAL v[100000][3];

  x = std::sqrt(1.0/12.0) * x;
  y = std::sqrt(1.0/12.0) * y;
  z = std::sqrt(1.0/12.0) * z;

  //0
  v[0][0] = (- x) * mul;
  v[0][1] = (- y) * mul;
  v[0][2] = (+ z) * mul;

  //1
  v[1][0] = (+ x) * mul;
  v[1][1] = (- y) * mul;
  v[1][2] = (+ z) * mul;

  //2
  v[2][0] = (- x) * mul;
  v[2][1] = (+ y) * mul;
  v[2][2] = (+ z) * mul;

  //3
  v[3][0] = (+ x) * mul;
  v[3][1] = (+ y) * mul;
  v[3][2] = (+ z) * mul;

  //4
  v[4][0] = (- x) * mul;
  v[4][1] = (- y) * mul;
  v[4][2] = (- z) * mul;

  //5
  v[5][0] = (+ x) * mul;
  v[5][1] = (- y) * mul;
  v[5][2] = (- z) * mul;

  //6
  v[6][0] = (- x) * mul;
  v[6][1] = (+ y) * mul;
  v[6][2] = (- z) * mul;

  //7
  v[7][0] = (+ x) * mul;
  v[7][1] = (+ y) * mul;
  v[7][2] = (- z) * mul;


  delta::hull::GEOMETRIC_EPSILON = 1e-10;
  delta::hull::TRI* tr = NULL;
  int pointlength = 0;
  tr = delta::hull::hull((iREAL *)v, pointsize, &pointlength);

  const int numberOfTriangles = pointlength;

  xCoordinates.resize(numberOfTriangles*3);
  yCoordinates.resize(numberOfTriangles*3);
  zCoordinates.resize(numberOfTriangles*3);

  int counter = 0;
  //#pragma omp parallel for
  for(delta::hull::TRI *t = tr, *e = t + pointlength; t < e; t ++)
  {//iterate through triangles and assign value
    xCoordinates[counter] = (t->ver [0][0]/(mul));
    yCoordinates[counter] = (t->ver [0][1]/(mul));
    zCoordinates[counter] = (t->ver [0][2]/(mul));
    //#pragma omp atomic
    counter++;

    xCoordinates[counter] = (t->ver [1][0]/(mul));
    yCoordinates[counter] = (t->ver [1][1]/(mul));
    zCoordinates[counter] = (t->ver [1][2]/(mul));
    //#pragma omp atomic
    counter++;

    xCoordinates[counter] = (t->ver [2][0]/(mul));
    yCoordinates[counter] = (t->ver [2][1]/(mul));
    zCoordinates[counter] = (t->ver [2][2]/(mul));
    //#pragma omp atomic
    counter++;
  }
  free(tr);

  const iREAL pi = std::acos(-1);

  #ifdef OMPProcess
	#pragma omp parallel for
  #endif
  for (unsigned i=0;i<xCoordinates.size(); i++) {
    iREAL xc = xCoordinates[i];
    iREAL yc = yCoordinates[i];
    iREAL zc = zCoordinates[i];

    iREAL M[] = {
     1.0,                 0.0,                   0.0,
     0.0,  std::cos(2*pi*alphaX),  std::sin(2*pi*alphaX),
     0.0, -std::sin(2*pi*alphaX),  std::cos(2*pi*alphaX)
    };

    xCoordinates[i] =   M[0] * xc + M[1] * yc + M[2] * zc;
    yCoordinates[i] =   M[3] * xc + M[4] * yc + M[5] * zc;
    zCoordinates[i] =   M[6] * xc + M[7] * yc + M[8] * zc;
  }

  #ifdef OMPProcess
	#pragma omp parallel for
  #endif
  for (unsigned i=0;i<xCoordinates.size(); i++) {
    iREAL xc = xCoordinates[i];
    iREAL yc = yCoordinates[i];
    iREAL zc = zCoordinates[i];

    iREAL M[] = {
      std::cos(2*pi*alphaY),  0.0, std::sin(2*pi*alphaY),
      0.0,                    1.0,                   0.0,
     -std::sin(2*pi*alphaY),  0.0, std::cos(2*pi*alphaY)
    };

    xCoordinates[i] =   M[0] * xc + M[1] * yc + M[2] * zc;
    yCoordinates[i] =   M[3] * xc + M[4] * yc + M[5] * zc;
    zCoordinates[i] =   M[6] * xc + M[7] * yc + M[8] * zc;
  }

  #ifdef OMPProcess
	#pragma omp parallel for
  #endif
  for (unsigned i=0;i<xCoordinates.size(); i++) {
    iREAL xc = xCoordinates[i];
    iREAL yc = yCoordinates[i];
    iREAL zc = zCoordinates[i];

    iREAL M[] = {
      std::cos(2*pi*alphaZ),  std::sin(2*pi*alphaZ),  0.0,
     -std::sin(2*pi*alphaZ),  std::cos(2*pi*alphaZ),  0.0,
                       0.0,                   0.0,  1.0
    };

    xCoordinates[i] =   M[0] * xc + M[1] * yc + M[2] * zc;
    yCoordinates[i] =   M[3] * xc + M[4] * yc + M[5] * zc;
    zCoordinates[i] =   M[6] * xc + M[7] * yc + M[8] * zc;
  }

  #ifdef OMPProcess
	#pragma omp parallel for
  #endif
  for (unsigned i=0;i<xCoordinates.size(); i++) {
    xCoordinates[i] += center[0];
    yCoordinates[i] += center[1];
    zCoordinates[i] += center[2];
  }
  delta::geometry::operators::triangle::meshDenser(meshmultiplier, xCoordinates, yCoordinates, zCoordinates);

  return new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}
