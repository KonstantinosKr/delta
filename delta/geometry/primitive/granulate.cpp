#include <delta/geometry/primitive/granulate.h>

void delta::geometry::primitive::granulate::generateParticle(
  iREAL  center[2],
  iREAL  h,
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates
) {
  const iREAL pi = std::acos(-1);

  const int numberOfTriangles = rand() % 9 + 3;

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());

  xCoordinates.resize(numberOfTriangles*2);
  yCoordinates.resize(numberOfTriangles*2);

  const iREAL MaxConvexity = 0.6;

  for (int i=0; i<numberOfTriangles; i++) {
    iREAL localRadius = h/2.0 * (static_cast<iREAL>( rand() ) / static_cast<iREAL>(RAND_MAX) * MaxConvexity + 1.0 - MaxConvexity);

    xCoordinates[2*i] = center[0] + localRadius * std::cos(2.0*pi * (1.0/numberOfTriangles*i) );
    yCoordinates[2*i] = center[1] + localRadius * std::sin(2.0*pi * (1.0/numberOfTriangles*i) );

    xCoordinates[2*i-1>=0 ? 2*i-1 : 2*numberOfTriangles-1] = center[0] + localRadius * std::cos(2.0*pi * (1.0/numberOfTriangles*i) );
    yCoordinates[2*i-1>=0 ? 2*i-1 : 2*numberOfTriangles-1] = center[1] + localRadius * std::sin(2.0*pi * (1.0/numberOfTriangles*i) );
  }
}

delta::geometry::mesh::Mesh *delta::geometry::primitive::granulate::loadParticle(
  iREAL  center[3],
  iREAL  h
) {
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  char fileinput[100] = "input/rock.vtk";

  delta::geometry::mesh::Mesh *mesh =  delta::core::readSingleVTKGeometry(fileinput);

  iREAL diagonal = mesh->computeDiagonal();
  mesh->scaleXYZ(h/1.0/diagonal, center);

  mesh->replace(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

delta::geometry::mesh::Mesh *delta::geometry::primitive::granulate::generateParticle(
  iREAL  center[3],
  iREAL  h,
  int noPointsPerParticle)
{
  unsigned int mul=1E8;

  iREAL diameter = h; //diameter -> make it slightly smaller

  int pointsize = noPointsPerParticle; //number of points for point cloud

  iREAL eps = 0.25; //0.25 eps is the roundness degree

//  srand48(time(NULL));

  iREAL v[100000][3];
  for(int i = 0; i<pointsize; i++) //create point cloud and do delaunay hull triangulation
  {
    iREAL rng1 = 0;
    iREAL rng2 = 360;
    iREAL phi = (rng2-rng1) * (drand48()) + rng1;
    iREAL theta = (rng2-rng1) * (drand48()) + rng1;
    iREAL myradius = (((drand48()*eps)+1.0-eps) * (diameter/2));

    v[i][0] = (myradius*sin(phi) * cos(theta))*mul;
    v[i][1] = (myradius*sin(phi) * sin(theta))*mul;
    v[i][2] = (myradius*cos(phi))*mul;
  }

  delta::hull::GEOMETRIC_EPSILON = 1e-10;
  delta::hull::TRI* tr = NULL;
  int pointlength = 0;
  tr = delta::hull::hull((iREAL *)v, pointsize, &pointlength);

  /*
  assertion(tr!=NULL);
  assertion(pointlength>3);
  assertion(tr==NULL || pointlength>3);
  */

  int numberOfTriangles = pointlength;

  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

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

  /*
  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());*/

  return new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}

