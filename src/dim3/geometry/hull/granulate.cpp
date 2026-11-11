#include <dim3/geometry/hull/granulate.h>

#include <random>

namespace {

/*
 * Uniform draw in [0,1). Two adapters so the seeded path and the legacy
 * process-wide rand() path can share one implementation.
 */
struct SeededUnit
{
  std::mt19937 rng;
  explicit SeededUnit(unsigned seed) : rng(seed) {}
  iREAL operator()() { return static_cast<iREAL>(rng()) / (static_cast<iREAL>(std::mt19937::max()) + 1.0); }
};

struct LegacyUnit
{
  iREAL operator()() { return static_cast<iREAL>(rand()) / static_cast<iREAL>(RAND_MAX); }
};

template <typename Unit>
void generateParticle2D(iREAL center[2], iREAL h,
                        std::vector<iREAL>& xCoordinates,
                        std::vector<iREAL>& yCoordinates,
                        Unit& unit,
                        int numberOfTriangles)
{
  const iREAL pi = std::acos(-1);

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());

  xCoordinates.resize(numberOfTriangles*2);
  yCoordinates.resize(numberOfTriangles*2);

  const iREAL MaxConvexity = 0.6;

  for (int i=0; i<numberOfTriangles; i++) {
    iREAL localRadius = h/2.0 * (unit() * MaxConvexity + 1.0 - MaxConvexity);

    xCoordinates[2*i] = center[0] + localRadius * std::cos(2.0*pi * (1.0/numberOfTriangles*i) );
    yCoordinates[2*i] = center[1] + localRadius * std::sin(2.0*pi * (1.0/numberOfTriangles*i) );

    yCoordinates[2*i-1>=0 ? 2*i-1 : 2*numberOfTriangles-1] = center[1] + localRadius * std::sin(2.0*pi * (1.0/numberOfTriangles*i) );
    xCoordinates[2*i-1>=0 ? 2*i-1 : 2*numberOfTriangles-1] = center[0] + localRadius * std::cos(2.0*pi * (1.0/numberOfTriangles*i) );
  }
}

}  // namespace

void delta::geometry::primitive::granulate::generateParticle(
  iREAL  center[2],
  iREAL  h,
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates
) {
  /* No seed: keep the process-wide rand() stream, so existing runs reproduce
   * exactly as before. */
  LegacyUnit unit;
  generateParticle2D(center, h, xCoordinates, yCoordinates, unit, rand() % 9 + 3);
}

void delta::geometry::primitive::granulate::generateParticle(
  iREAL  center[2],
  iREAL  h,
  std::vector<iREAL>&  xCoordinates,
  std::vector<iREAL>&  yCoordinates,
  unsigned seed
) {
  SeededUnit unit(seed);
  /* Consume the shape draw from the seeded stream too, not from rand(). */
  const int numberOfTriangles = static_cast<int>(unit() * 9.0) + 3;
  generateParticle2D(center, h, xCoordinates, yCoordinates, unit, numberOfTriangles);
}

//reposition particle
delta::geometry::mesh::Mesh *delta::geometry::primitive::granulate::loadParticle(
  iREAL  center[3],
  iREAL  h
) {
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  char fileinput[100] = "input/rock.vtk";

  delta::geometry::mesh::Mesh *mesh =  delta::core::io::readVTKGeometry(fileinput);

  iREAL diagonal = mesh->computeDiagonal();
  mesh->scaleXYZ(h/1.0/diagonal);

  mesh->replace(xCoordinates, yCoordinates, zCoordinates);

  return mesh;
}

/* Legacy overload: keeps consuming the process-wide drand48() stream, so an
 * unmodified scenario produces the same granulates it always did. */
delta::geometry::mesh::Mesh *delta::geometry::primitive::granulate::generateParticle(
  iREAL  center[3],
  iREAL  h,
  int noPointsPerParticle)
{
  return generateParticle(center, h, noPointsPerParticle, 0u, false);
}

delta::geometry::mesh::Mesh *delta::geometry::primitive::granulate::generateParticle(
  iREAL  center[3],
  iREAL  h,
  int noPointsPerParticle,
  unsigned seed)
{
  return generateParticle(center, h, noPointsPerParticle, seed, true);
}

/* Shared implementation. seeded=false uses drand48(), seeded=true uses a
 * mt19937 owned by this call. */
delta::geometry::mesh::Mesh *delta::geometry::primitive::granulate::generateParticle(
  iREAL  center[3],
  iREAL  h,
  int noPointsPerParticle,
  unsigned seed,
  bool seeded)
{
  unsigned int mul=1E8;

  iREAL diameter = h; //diameter -> make it slightly smaller

  int pointsize = noPointsPerParticle; //number of points for point cloud

  iREAL eps = 0.25; //0.25 eps is the roundness degree

  std::mt19937 rng(seed);
  /* unit() is [0,1): the seeded path draws from rng, the legacy path from the
   * same drand48() calls it always used. */
  auto unit = [seeded, &rng]() -> iREAL {
    if(!seeded) {return drand48();}
    return static_cast<iREAL>(rng()) / (static_cast<iREAL>(std::mt19937::max()) + 1.0);
  };

  iREAL v[100000][3];

  //#pragma omp parallel for
  for(int i = 0; i<pointsize; i++) //create point cloud and do delaunay hull triangulation
  {
    iREAL rng1 = 0;
    iREAL rng2 = 360;
    iREAL phi = (rng2-rng1) * unit() + rng1;
    iREAL theta = (rng2-rng1) * unit() + rng1;
    iREAL myradius = (((unit()*eps)+1.0-eps) * (diameter/2));

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

  /*
  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());*/

  return new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}

