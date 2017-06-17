#include "properties.h"

void delta::geometry::properties::moveMeshFromPositionToOrigin(
    double center[3],
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	for(unsigned i=0;i<xCoordinates.size();i++)
	{
		xCoordinates[i] = xCoordinates[i]-center[0];
		yCoordinates[i] = yCoordinates[i]-center[1];
		zCoordinates[i] = zCoordinates[i]-center[2];
	}
}

void delta::geometry::properties::moveMeshFromOriginToPosition(
    double center[3],
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	for(unsigned i=0;i<xCoordinates.size();i++)
	{
		xCoordinates[i] = (xCoordinates[i])+center[0];
		yCoordinates[i] = (yCoordinates[i])+center[1];
		zCoordinates[i] = (zCoordinates[i])+center[2];
	}
}

void delta::geometry::properties::scaleXYZ(
    double scale,
    double position[3],
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
  delta::geometry::properties::moveMeshFromPositionToOrigin(position, xCoordinates, yCoordinates, zCoordinates);
	for(unsigned i=0;i<xCoordinates.size();i++)
	{
		xCoordinates[i] = xCoordinates[i]*scale;
		yCoordinates[i] = yCoordinates[i]*scale;
		zCoordinates[i] = zCoordinates[i]*scale;
	}
	delta::geometry::properties::moveMeshFromOriginToPosition(position, xCoordinates, yCoordinates, zCoordinates);
}

void delta::geometry::properties::rotateX(
    double alphaX,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	const double pi = std::acos(-1);
	for (unsigned i=0;i<xCoordinates.size(); i++)
	{
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
}

void delta::geometry::properties::rotateY(
    double alphaY,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	const double pi = std::acos(-1);
	for (unsigned i=0;i<xCoordinates.size(); i++) {
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
}

void delta::geometry::properties::rotateZ(
    double alphaZ,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	const double pi = std::acos(-1);
	for (unsigned i=0;i<xCoordinates.size(); i++) {
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
}

double delta::geometry::properties::computeDistanceAB(
    std::array<double, 3> A,
    std::array<double, 3> B)
{
	return std::sqrt(((B[0]-A[0])*(B[0]-A[0]))+((B[1]-A[1])*(B[1]-A[1]))+((B[2]-A[2])*(B[2]-A[2])));
}

double delta::geometry::properties::getXYZWidth(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	double xw = delta::geometry::properties::getXw(xCoordinates, yCoordinates, zCoordinates);
	double yw = delta::geometry::properties::getXw(xCoordinates, yCoordinates, zCoordinates);
	double zw = delta::geometry::properties::getXw(xCoordinates, yCoordinates, zCoordinates);

	double tmp = xw > yw ? xw : yw;
	double width = tmp>zw ? tmp : zw;

	return width;
}

double delta::geometry::properties::getXZWidth(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	double xw = delta::geometry::properties::getXw(xCoordinates, yCoordinates, zCoordinates);
	double zw = delta::geometry::properties::getXw(xCoordinates, yCoordinates, zCoordinates);
	double width = xw>zw ? xw : zw;

	return width;
}

double delta::geometry::properties::getXw(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{

	std::array<double, 3> min = delta::geometry::properties::getMinBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);
	std::array<double, 3> max = delta::geometry::properties::getMaxBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);

	return std::abs(min[0] - max[0]);
}

double delta::geometry::properties::getYw(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{

	std::array<double, 3> min = delta::geometry::properties::getMinBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);
	std::array<double, 3> max = delta::geometry::properties::getMaxBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);

	return std::abs(min[1] - max[1]);
}

double delta::geometry::properties::getZw(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	std::array<double, 3> min = delta::geometry::properties::getMinBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);
	std::array<double, 3> max = delta::geometry::properties::getMaxBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);

	return std::abs(min[2] - max[2]);
}

std::array<double, 3> delta::geometry::properties::getMinBoundaryVertex(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	std::array<double, 3> vertex;

	vertex[0] = getMinXAxis(xCoordinates);
	vertex[1] = getMinYAxis(yCoordinates);
	vertex[2] = getMinZAxis(zCoordinates);

	return vertex;
}

std::array<double, 3> delta::geometry::properties::getMaxBoundaryVertex(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	std::array<double, 3> vertex;

	vertex[0] = getMaxXAxis(xCoordinates);
	vertex[1] = getMaxYAxis(yCoordinates);
	vertex[2] = getMaxZAxis(zCoordinates);

	return vertex;
}

double delta::geometry::properties::computeDiagonal(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	std::array<double, 3> minPoint, maxPoint;

	minPoint = getMinBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);
	maxPoint = getMaxBoundaryVertex(xCoordinates, yCoordinates, zCoordinates);

	return computeDistanceAB(minPoint,maxPoint);
}

double delta::geometry::properties::getMaxXAxis(
    std::vector<double>&  xCoordinates)
{
	double max = std::numeric_limits<double>::min();

	for(unsigned i=0;i<xCoordinates.size();i++)
	{
		if (max < xCoordinates[i]) max = xCoordinates[i];
	}
	return max;
}

double delta::geometry::properties::getMaxYAxis(
    std::vector<double>&  yCoordinates)
{
	double max = std::numeric_limits<double>::min();

	for(unsigned i=0;i<yCoordinates.size();i++)
	{
		if (max < yCoordinates[i]) max = yCoordinates[i];
	}
	return max;
}

double delta::geometry::properties::getMaxZAxis(
    std::vector<double>&  zCoordinates)
{
	double max = std::numeric_limits<double>::min();

	for(unsigned i=0;i<zCoordinates.size();i++)
	{
		if (max < zCoordinates[i]) max = zCoordinates[i];
	}
	return max;
}

double delta::geometry::properties::getMinXAxis(
    std::vector<double>&  xCoordinates)
{
	double min = std::numeric_limits<double>::max();

	for(unsigned i=0;i<xCoordinates.size();i++)
	{
		if (min > xCoordinates[i]) min = xCoordinates[i];
	}
	return min;
}

double delta::geometry::properties::getMinYAxis(
    std::vector<double>&  yCoordinates)
{
	double min = std::numeric_limits<double>::max();

	for(unsigned i=0;i<yCoordinates.size();i++)
	{
		if (min > yCoordinates[i]) min = yCoordinates[i];
	}
	return min;
}

double delta::geometry::properties::getMinZAxis(
    std::vector<double>&  zCoordinates)
{
	double min = std::numeric_limits<double>::max();

	for(unsigned i=0;i<zCoordinates.size();i++)
	{
		if (min > zCoordinates[i]) min = zCoordinates[i];
	}
	return min;
}

void delta::geometry::properties::centerOfGeometry(
    double centreOfGeometry[3],
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
	centreOfGeometry[0] = 0.0;
	centreOfGeometry[1] = 0.0;
	centreOfGeometry[2] = 0.0;

	unsigned nVertices = xCoordinates.size();

	for(unsigned i=0;i<nVertices;i++)
	{
		centreOfGeometry[0] += (xCoordinates[i]);
		centreOfGeometry[1] += (yCoordinates[i]);
		centreOfGeometry[2] += (zCoordinates[i]);
	}

	centreOfGeometry[0] = centreOfGeometry[0]/(nVertices);
	centreOfGeometry[1] = centreOfGeometry[1]/(nVertices);
	centreOfGeometry[2] = centreOfGeometry[2]/(nVertices);
}

void delta::geometry::properties::centerOfMass(
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates,
  double&               centreOfMassX,
  double&               centreOfMassY,
  double&               centreOfMassZ,
  double&               refcentreOfMassX,
  double&               refcentreOfMassY,
  double&               refcentreOfMassZ
) {

  centreOfMassX = 0.0;
  centreOfMassY = 0.0;
  centreOfMassZ = 0.0;
  
  unsigned nVertices = xCoordinates.size();

  for(unsigned i=0;i<nVertices;i++)
  {
    centreOfMassX += xCoordinates[i];
    centreOfMassY += yCoordinates[i];
    centreOfMassZ += zCoordinates[i];
  }

  centreOfMassX = centreOfMassX/(nVertices*3);
  centreOfMassY = centreOfMassY/(nVertices*3);
  centreOfMassZ = centreOfMassZ/(nVertices*3);

  refcentreOfMassX = centreOfMassX;
  refcentreOfMassY = centreOfMassY;
  refcentreOfMassZ = centreOfMassZ;
}

void delta::geometry::properties::explode(
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates,
  double length
) {

  std::vector<double> exCoordinates, eyCoordinates, ezCoordinates;

  for(unsigned i=0;i<xCoordinates.size();i+=3)
  {
    iREAL A[3], B[3], C[3];
    A[0] = xCoordinates[i];
    A[1] = yCoordinates[i];
    A[2] = zCoordinates[i];

    B[0] = xCoordinates[i+1];
    B[1] = yCoordinates[i+1];
    B[2] = zCoordinates[i+1];

    C[0] = xCoordinates[i+2];
    C[1] = yCoordinates[i+2];
    C[2] = zCoordinates[i+2];

    iREAL V[3], W[3], N[3];
    V[0] = B[0] - A[0];
    V[1] = B[1] - A[1];
    V[2] = B[2] - A[2];

    W[0] = C[0] - B[0];
    W[1] = C[1] - B[1];
    W[2] = C[2] - B[2];

    N[0] = (V[1]*W[2])-(V[2]*W[1]);
    N[1] = (V[2]*W[0])-(V[0]*W[2]);
    N[2] = (V[0]*W[1])-(V[1]*W[0]);

    iREAL mag = std::sqrt((N[0]*N[0])+(N[1]*N[1])+(N[2]*N[2]));
    N[0] = N[0]/mag;
    N[1] = N[1]/mag;
    N[2] = N[2]/mag;

    exCoordinates.push_back(xCoordinates[i] + length * N[0]);
    eyCoordinates.push_back(yCoordinates[i] + length * N[1]);
    ezCoordinates.push_back(zCoordinates[i] + length * N[2]);

    exCoordinates.push_back(xCoordinates[i+1] + length * N[0]);
    eyCoordinates.push_back(yCoordinates[i+1] + length * N[1]);
    ezCoordinates.push_back(zCoordinates[i+1] + length * N[2]);

    exCoordinates.push_back(xCoordinates[i+2] + length * N[0]);
    eyCoordinates.push_back(yCoordinates[i+2] + length * N[1]);
    ezCoordinates.push_back(zCoordinates[i+2] + length * N[2]);
  }

  xCoordinates.insert(xCoordinates.end(), exCoordinates.begin(), exCoordinates.end());
  yCoordinates.insert(yCoordinates.end(), eyCoordinates.begin(), eyCoordinates.end());
  zCoordinates.insert(zCoordinates.end(), ezCoordinates.begin(), ezCoordinates.end());
}

void delta::geometry::properties::exploded(
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates,
  double length
) {

  std::vector<double> exCoordinates, eyCoordinates, ezCoordinates;

  for(unsigned i=0;i<xCoordinates.size();i+=3)
  {
    iREAL A[3], B[3], C[3];
    A[0] = xCoordinates[i];
    A[1] = yCoordinates[i];
    A[2] = zCoordinates[i];

    B[0] = xCoordinates[i+1];
    B[1] = yCoordinates[i+1];
    B[2] = zCoordinates[i+1];

    C[0] = xCoordinates[i+2];
    C[1] = yCoordinates[i+2];
    C[2] = zCoordinates[i+2];

    iREAL V[3], W[3], N[3];
    V[0] = B[0] - A[0];
    V[1] = B[1] - A[1];
    V[2] = B[2] - A[2];

    W[0] = C[0] - B[0];
    W[1] = C[1] - B[1];
    W[2] = C[2] - B[2];

    N[0] = (V[1]*W[2])-(V[2]*W[1]);
    N[1] = (V[2]*W[0])-(V[0]*W[2]);
    N[2] = (V[0]*W[1])-(V[1]*W[0]);

    iREAL mag = std::sqrt((N[0]*N[0])+(N[1]*N[1])+(N[2]*N[2]));
    N[0] = N[0]/mag;
    N[1] = N[1]/mag;
    N[2] = N[2]/mag;

    xCoordinates[i] = xCoordinates[i] + length * N[0];
    yCoordinates[i] = yCoordinates[i] + length * N[1];
    zCoordinates[i] = zCoordinates[i] + length * N[2];

    xCoordinates[i+1] = xCoordinates[i+1] + length * N[0];
    yCoordinates[i+1] = yCoordinates[i+1] + length * N[1];
    zCoordinates[i+1] = zCoordinates[i+1] + length * N[2];

    xCoordinates[i+2] = xCoordinates[i+2] + length * N[0];
    yCoordinates[i+2] = yCoordinates[i+2] + length * N[1];
    zCoordinates[i+2] = zCoordinates[i+2] + length * N[2];
  }
}

double delta::geometry::properties::getHMin(
  const std::vector<double>&  xCoordinates,
  const std::vector<double>&  yCoordinates,
  const std::vector<double>&  zCoordinates
)
{
  double min = 1E99;

  for(unsigned i=0; i<xCoordinates.size(); i+=3)
  {
	double A[3], B[3], C[3];
	A[0] = xCoordinates[i];
	A[1] = yCoordinates[i];
	A[2] = zCoordinates[i];

	B[0] = xCoordinates[i+1];
	B[1] = yCoordinates[i+1];
	B[2] = zCoordinates[i+1];

	C[0] = xCoordinates[i+2];
	C[1] = yCoordinates[i+2];
	C[2] = zCoordinates[i+2];

	double AB = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1])+(A[2]-B[2])*(A[2]-B[2]));
	double BC = sqrt((B[0]-C[0])*(B[0]-C[0])+(B[1]-C[1])*(B[1]-C[1])+(B[2]-C[2])*(B[2]-C[2]));
	double CA = sqrt((C[0]-A[0])*(C[0]-A[0])+(C[1]-A[1])*(C[1]-A[1])+(C[2]-A[2])*(C[2]-A[2]));

	if (std::min(std::min(AB, BC), CA) < min)
	{
		min = std::min(std::min(AB, BC), CA);
	}
  }
  if(min == 1E99) min = 0.0;
  return min;
}

double delta::geometry::properties::simplex_J (
    double *a, double *b, double *c, double *d)
{
  double q [9], J;

  q [0] = b [0] - a [0];
  q [1] = c [0] - a [0];
  q [2] = d [0] - a [0];
  q [3] = b [1] - a [1];
  q [4] = c [1] - a [1];
  q [5] = d [1] - a [1];
  q [6] = b [2] - a [2];
  q [7] = c [2] - a [2];
  q [8] = d [2] - a [2];

  J = q [0]*q [4]*q [8] + q [3]*q [7]*q [2] + q [6]*q [1]*q [5] -
      q [6]*q [4]*q [2] - q [0]*q [7]*q [5] - q [3]*q [1]*q [8];

  return J;
}
//https://en.wikipedia.org/wiki/Trace_%28linear_algebra%29
#define TRACE(A) ((A)[0] + (A)[4] + (A)[8])

/*
 * gets the inertia using simplex integration from solfec
 */
void delta::geometry::properties::getInertia(
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates,
		delta::collision::material::MaterialType material,
		double& mass,
		double center[3],
		double inertia[9])
{
  iREAL me, sx, sy, sz, euler[9], a[3], b[3], c[3], J;

  iREAL zero[3];
  zero[0] = 0;
  zero[1] = 0;
  zero[2] = 0;

  me = sx = sy = sz = 0.0;
  euler[0] = euler[1] = euler[2] =
  euler[3] = euler[4] = euler[5] =
  euler[6] = euler[7] = euler[8] = 0.0;

  int rho = int(delta::collision::material::materialToDensitymap.find(material)->second);

  for (unsigned i=0;i<xCoordinates.size(); i+=3)
  {
    a[0] = xCoordinates[i];
    a[1] = yCoordinates[i];
    a[2] = zCoordinates[i];

    b[0] = xCoordinates[i+1];
    b[1] = yCoordinates[i+1];
    b[2] = zCoordinates[i+1];

    c[0] = xCoordinates[i+2];
    c[1] = yCoordinates[i+2];
    c[2] = zCoordinates[i+2];

    J = rho * simplex_J (zero, a, b, c);

    me += simplex_1 (J, zero, a, b, c);

    sx += simplex_x (J, zero, a, b, c);
    sy += simplex_y (J, zero, a, b, c);
    sz += simplex_z (J, zero, a, b, c);
    euler[0] += simplex_xx (J, zero, a, b, c);
    euler[3] += simplex_xy (J, zero, a, b, c);
    euler[4] += simplex_yy (J, zero, a, b, c);
    euler[6] += simplex_xz (J, zero, a, b, c);
    euler[7] += simplex_yz (J, zero, a, b, c);
    euler[8] += simplex_zz (J, zero, a, b, c);
  }

  mass = me;
#ifdef STATS
  printf("mass: %f\n", mass);
  printf("sx:%f sy:%f sz:%f\n", sx, sy, sz);
#endif
  center[0] = (sx / me);
  center[1] = (sy / me);
  center[2] = (sz / me);
#ifdef STATS
  printf("c %f c %f c %f\n", center[0], center[1], center[2]);
#endif

#ifdef STATS
  printf("euler %f %f %f %f %f %f %f %f %f\n", euler[0], euler[1], euler[2], euler[3], euler[4], euler[5], euler[6], euler[7], euler[8]);
#endif
  euler[0] -= (2*sx - center[0]*me)*center[0];
  euler[4] -= (2*sy - center[1]*me)*center[1];
  euler[8] -= (2*sz - center[2]*me)*center[2];
  euler[3] -= center[0]*sy + center[1]*sx - center[0]*center[1]*me;
  euler[6] -= center[0]*sz + center[2]*sx - center[0]*center[2]*me;
  euler[7] -= center[1]*sz + center[2]*sy - center[1]*center[2]*me;
  euler[1] = euler[3];
  euler[2] = euler[6];
  euler[5] = euler[7];

  /* convert Euler tensor to the inertia tensor */
  double trace = TRACE (euler);
  inertia[0] = trace - euler[0];
  inertia[4] = trace - euler[4];
  inertia[8] = trace - euler[8];
  inertia[1] = -euler[1];
  inertia[2] = -euler[2];
  inertia[3] = -euler[3];
  inertia[5] = -euler[5];
  inertia[6] = -euler[6];
  inertia[7] = -euler[7]; /* inertia = tr(euler)*one - euler */
#ifdef STATS
  printf("Inertia %f %f %f %f %f %f %f %f %f\n", inertia[0], inertia[1], inertia[2], inertia[3], inertia[4], inertia[5], inertia[6], inertia[7], inertia[8]);
#endif
}

double delta::geometry::properties::getMass(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates,
    delta::collision::material::MaterialType material)
{
  iREAL me=0, a[3], b[3], c[3], J;

  iREAL zero[3];
  zero[0] = 0;
  zero[1] = 0;
  zero[2] = 0;

  int rho= int(delta::collision::material::materialToDensitymap.find(material)->second);

  for (unsigned i=0;i<xCoordinates.size(); i+=3)
  {
    a[0] = xCoordinates[i];
    a[1] = yCoordinates[i];
    a[2] = zCoordinates[i];

    b[0] = xCoordinates[i+1];
    b[1] = yCoordinates[i+1];
    b[2] = zCoordinates[i+1];

    c[0] = xCoordinates[i+2];
    c[1] = yCoordinates[i+2];
    c[2] = zCoordinates[i+2];

    J = rho * simplex_J (zero, a, b, c);
    me += simplex_1 (J, zero, a, b, c);
  }

  return me;
}

double delta::geometry::properties::getVolume(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates)
{
  iREAL vol=0, a[3], b[3], c[3], J;

  iREAL zero[3];
  zero[0] = 0;
  zero[1] = 0;
  zero[2] = 0;

  for (unsigned i=0;i<xCoordinates.size(); i+=3)
  {
    a[0] = xCoordinates[i];
    a[1] = yCoordinates[i];
    a[2] = zCoordinates[i];

    b[0] = xCoordinates[i+1];
    b[1] = yCoordinates[i+1];
    b[2] = zCoordinates[i+1];

    c[0] = xCoordinates[i+2];
    c[1] = yCoordinates[i+2];
    c[2] = zCoordinates[i+2];

    J = simplex_J (zero, a, b, c);

    vol += simplex_1 (J, zero, a, b, c);
  }

  return vol;
}

void delta::geometry::properties::getInverseInertia(
    double inertia[9],
    double inverse[9],
    bool isObject)
{
	iREAL det;

    iREAL a[9], x[9];

    for (int j = 0; j < 9; j++)
    {
      a[j] = inertia[j];
    }

    INVERT (a, x, det);
	//INVERT (inertia, inverse, det);

	//zero inverse because it is an object
	if(isObject)
	{
		for (int j = 0; j < 9; j++)
		{
			inverse[j] = 0;
		}
	} else {
		for (int j = 0; j < 9; j++)
		{
		  inverse[j] = x[j];
		}
	}
}

