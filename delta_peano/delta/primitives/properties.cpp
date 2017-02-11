#include "delta/primitives/properties.h"
#include <stdio.h>

void delta::primitives::moveMeshFromPositionToOrigin(double center[3],
										  std::vector<double>&  xCoordinates,
										  std::vector<double>&  yCoordinates,
										  std::vector<double>&  zCoordinates)
{
	for(int i=0;i<xCoordinates.size();i++)
	{
		xCoordinates[i] = xCoordinates[i]-center[0];
		yCoordinates[i] = yCoordinates[i]-center[1];
		zCoordinates[i] = zCoordinates[i]-center[2];
	}
}

void delta::primitives::moveMeshFromOriginToPosition(double center[3],
										  std::vector<double>&  xCoordinates,
										  std::vector<double>&  yCoordinates,
										  std::vector<double>&  zCoordinates)
{
	for(int i=0;i<xCoordinates.size();i+=3)
	{
		xCoordinates[i] = (xCoordinates[i])+center[0];
		yCoordinates[i] = (yCoordinates[i])+center[1];
		zCoordinates[i] = (zCoordinates[i])+center[2];

		xCoordinates[i+1] = (xCoordinates[i+1])+center[0];
		yCoordinates[i+1] = (yCoordinates[i+1])+center[1];
		zCoordinates[i+1] = (zCoordinates[i+1])+center[2];

		xCoordinates[i+2] = (xCoordinates[i+2])+center[0];
		yCoordinates[i+2] = (yCoordinates[i+2])+center[1];
		zCoordinates[i+2] = (zCoordinates[i+2])+center[2];
	}
}

void delta::primitives::scaleXYZ(double scale,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	for(int i=0;i<xCoordinates.size();i++)
	{
		xCoordinates[i] = xCoordinates[i]*scale;
		yCoordinates[i] = yCoordinates[i]*scale;
		zCoordinates[i] = zCoordinates[i]*scale;
	}
}

void delta::primitives::rotateX(double alphaX,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	const double pi = std::acos(-1);
	for (int i=0; i<xCoordinates.size(); i++)
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

void delta::primitives::rotateY(double alphaY,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	const double pi = std::acos(-1);
	for (int i=0; i<xCoordinates.size(); i++) {
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

void delta::primitives::rotateZ(double alphaZ,
		std::vector<double>&  xCoordinates,
		std::vector<double>&  yCoordinates,
		std::vector<double>&  zCoordinates)
{
	const double pi = std::acos(-1);
	for (int i=0; i<xCoordinates.size(); i++) {
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

double delta::primitives::computeDistanceAB(double A[3], double B[3])
{
	return std::sqrt(((B-A)*(B-A))+((B-A)*(B-A))+((B-A)*(B-A)));
}

double delta::primitives::computeMaxWidth(std::vector<double>&  xCoordinates,
										std::vector<double>&  yCoordinates,
										std::vector<double>&  zCoordinates)
{
	double max = -1E99;

	for(int i=0;i<xCoordinates.size();i++)
	{
		double A[3];

		A[0] = xCoordinates[i];
		A[1] = yCoordinates[i];
		A[2] = zCoordinates[i];

		for(int j=i+1;j<xCoordinates.size();j++)
		{
			double B[3];
			B[0] = xCoordinates[j];
			B[1] = yCoordinates[j];
			B[2] = zCoordinates[j];

			double result = computeDistanceAB(A,B);
			if (max < result) max = result;
		}
	}
	return max;
}

double delta::primitives::computeMaxXWidth(std::vector<double>&  xCoordinates)
{
	double max = -1E99;

	for(int i=0;i<xCoordinates.size();i++)
	{
		double A = xCoordinates[i];

		for(int j=i+1;j<xCoordinates.size();j++)
		{
			double B = xCoordinates[j];
			double distance = std::abs(B - A);
			if (max < distance) max = distance;
		}
	}
	return max;
}

double delta::primitives::computeMaxYWidth(std::vector<double>&  yCoordinates)
{
	double max = -1E99;

	for(int i=0;i<yCoordinates.size();i++)
	{
		double A = yCoordinates[i];

		for(int j=i+1;j<yCoordinates.size();j++)
		{
			double B = yCoordinates[j];
			double distance = std::abs(B - A);
			if (max < distance) max = distance;
		}
	}
	return max;
}

double delta::primitives::computeMaxZWidth(std::vector<double>&  zCoordinates)
{
	double max = -1E99;

	for(int i=0;i<zCoordinates.size();i++)
	{
		double A = zCoordinates[i];

		for(int j=i+1;j<zCoordinates.size();j++)
		{
			double B = zCoordinates[j];
			double distance = std::abs(B - A);
			if (max < distance) max = distance;
		}
	}
	return max;
}

void delta::primitives::centerOfGeometry(double 	centreOfGeometry[3],
										  std::vector<double>&  xCoordinates,
										  std::vector<double>&  yCoordinates,
										  std::vector<double>&  zCoordinates)
{
	centreOfGeometry[0] = 0.0;
	centreOfGeometry[1] = 0.0;
	centreOfGeometry[2] = 0.0;

	int nVertices = xCoordinates.size();

	for(int i=0;i<nVertices;i++)
	{
		centreOfGeometry[0] += xCoordinates[i];
		centreOfGeometry[1] += yCoordinates[i];
		centreOfGeometry[2] += zCoordinates[i];
	}

	centreOfGeometry[0] = centreOfGeometry[0]/(nVertices*3);
	centreOfGeometry[1] = centreOfGeometry[1]/(nVertices*3);
	centreOfGeometry[2] = centreOfGeometry[2]/(nVertices*3);
}

void delta::primitives::centerOfMass(
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
  
  int nVertices = xCoordinates.size();

  for(int i=0;i<nVertices;i++)
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

double delta::primitives::computeHMin(
  const std::vector<double>&  xCoordinates,
  const std::vector<double>&  yCoordinates,
  const std::vector<double>&  zCoordinates
)
{
  double min = 1E99;

  for(int i=0; i < xCoordinates.size(); i+=3)
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

double delta::primitives::simplex_J (double *a, double *b, double *c, double *d)
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
 * computes the inertia using simplex integration from solfec
 */
void delta::primitives::computeInertia(
		const std::vector<double>&  xCoordinates,
		const std::vector<double>&  yCoordinates,
		const std::vector<double>&  zCoordinates,
		double rho,
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

  for (int i = 0; i < xCoordinates.size(); i+=3)
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

void delta::primitives::computeInverseInertia(double inertia[9], double inverse[9], bool isObject)
{
	iREAL det;
	INVERT (inertia, inverse, det);

	//zero inverse because it is an object
	if(isObject)
	{
		for (int j = 0; j < 9; j ++)
		{
			inverse[j] = 0;
		}
	}
}

double delta::primitives::getKineticRotationalEnergy(double velocity[3], double angular[3], double inertia[9], double mass){
	iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
	iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);

	return rotation+kinetic;
}

double delta::primitives::getKineticEnergy(double velocity[3], double mass){
	iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);
	return kinetic;
}

double delta::primitives::getRotationalEnergy(double angular[3], double inertia[9]){
	iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
	return rotation;
}

