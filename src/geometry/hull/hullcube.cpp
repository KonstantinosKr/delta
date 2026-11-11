#include "../../geometry/hull/hullcube.h"
#include "../operators/hull/hull.h"
#include "../operators/hull/alg.h"
#include "../operators/triangle.h"

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <vector>

delta::geometry::mesh::Mesh *delta::geometry::hull::generateHullCube(
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
delta::geometry::mesh::Mesh *delta::geometry::hull::generateHullCube(
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