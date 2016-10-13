#include "granulates.h"


#include <cmath>
#include <assert.h>
#include <stdlib.h>


#include "delta/hull/hull.h"
#include "delta/hull/alg.h"


void delta::primitives::generateParticle(
  double  center[2],
  double  h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates
) {
  const double pi = std::acos(-1);

  const int numberOfTriangles = rand() % 9 + 3;

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());

  xCoordinates.resize(numberOfTriangles*2);
  yCoordinates.resize(numberOfTriangles*2);

  const double MaxConvexity = 0.6;

  for (int i=0; i<numberOfTriangles; i++) {
    double localRadius = h/2.0 * (static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) * MaxConvexity + 1.0 - MaxConvexity);

    xCoordinates[2*i] = center[0] + localRadius * std::cos(2.0*pi * (1.0/numberOfTriangles*i) );
    yCoordinates[2*i] = center[1] + localRadius * std::sin(2.0*pi * (1.0/numberOfTriangles*i) );

    xCoordinates[2*i-1>=0 ? 2*i-1 : 2*numberOfTriangles-1] = center[0] + localRadius * std::cos(2.0*pi * (1.0/numberOfTriangles*i) );
    yCoordinates[2*i-1>=0 ? 2*i-1 : 2*numberOfTriangles-1] = center[1] + localRadius * std::sin(2.0*pi * (1.0/numberOfTriangles*i) );
  }
}


void delta::primitives::generateParticle(
  double  center[3],
  double  h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates
) {
  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());
	
  unsigned int mul=1E8;

  double diameter = h * 0.9; //diameter -> make it slightly smaller

  int pointsize = 20; //number of points for point cloud

  double eps = 0.25; //0.25 eps is the roundness degree

//  srand48(time(NULL));

  double v[10000][3];
  for(int i = 0; i<pointsize; i++) //create point cloud and do delaunay hull triangulation
  {
    double rng1 = 0;
    double rng2 = 360;
    double phi = (rng2-rng1) * (drand48()) + rng1;
    double theta = (rng2-rng1) * (drand48()) + rng1;
    double myradius = (((drand48()*eps)+1.0-eps) * (diameter/2));

    v[i][0] = (myradius*sin(phi) * cos(theta))*mul;
    v[i][1] = (myradius*sin(phi) * sin(theta))*mul;
    v[i][2] = (myradius*cos(phi))*mul;
  }

  delta::hull::GEOMETRIC_EPSILON = 1e-10;
  delta::hull::TRI* tr = NULL;
  int pointlength = 0;
  tr = delta::hull::hull((double *)v, pointsize, &pointlength);

  /*
  assertion(tr!=NULL);
  assertion(pointlength>3);
  assertion(tr==NULL || pointlength>3);
  */

  const int numberOfTriangles = pointlength;

  xCoordinates.resize(numberOfTriangles*3);
  yCoordinates.resize(numberOfTriangles*3);
  zCoordinates.resize(numberOfTriangles*3);

  int counter = 0;
  for(delta::hull::TRI *t = tr, *e = t + pointlength; t < e; t ++)
  {//iterate through triangles and assign value
    xCoordinates[counter] = (t->ver [0][0]/(mul)) + center[0];
    yCoordinates[counter] = (t->ver [0][1]/(mul)) + center[1];
    zCoordinates[counter] = (t->ver [0][2]/(mul)) + center[2];

    counter++;

    xCoordinates[counter] = (t->ver [1][0]/(mul)) + center[0];
    yCoordinates[counter] = (t->ver [1][1]/(mul)) + center[1];
    zCoordinates[counter] = (t->ver [1][2]/(mul)) + center[2];

    counter++;

    xCoordinates[counter] = (t->ver [2][0]/(mul)) + center[0];
    yCoordinates[counter] = (t->ver [2][1]/(mul)) + center[1];
    zCoordinates[counter] = (t->ver [2][2]/(mul)) + center[2];

    counter++;
  }
  free(tr);
}

