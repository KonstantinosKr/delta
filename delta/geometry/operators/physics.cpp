/*
 * properties.cpp
 *
 *  Created on: 6 Apr 2018
 *      Author: konstantinos
 */

#include <delta/geometry/operators/physics.h>

iREAL delta::geometry::operators::physics::computeVolume(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
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

/*
 * gets the inertia using simplex integration from solfec
 */
void delta::geometry::operators::physics::computeInertia(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates,
	delta::geometry::material::MaterialType material,
	iREAL& mass,
	iREAL center[3],
	iREAL inertia[9])
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

  iREAL rho = iREAL(delta::geometry::material::materialToDensitymap.find(material)->second);

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
  //printf("mass:%f\n", mass);
//#ifdef STATS
  //printf("sx:%f sy:%f sz:%f\n", sx, sy, sz);
//#endif
  center[0] = (sx / me);
  center[1] = (sy / me);
  center[2] = (sz / me);
//#ifdef STATS
  //printf("c %f c %f c %f\n", center[0], center[1], center[2]);
//#endif

#ifdef STATS
  //printf("euler %f %f %f %f %f %f %f %f %f\n", euler[0], euler[1], euler[2], euler[3], euler[4], euler[5], euler[6], euler[7], euler[8]);
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
  iREAL trace = TRACE (euler);
  inertia[0] = trace - euler[0];
  inertia[4] = trace - euler[4];
  inertia[8] = trace - euler[8];
  inertia[1] = -euler[1];
  inertia[2] = -euler[2];
  inertia[3] = -euler[3];
  inertia[5] = -euler[5];
  inertia[6] = -euler[6];
  inertia[7] = -euler[7]; /* inertia = tr(euler)*one - euler */
//#ifdef STATS
  //printf("Inertia %e %e %e %e %e %e %e %e %e\n", inertia[0], inertia[1], inertia[2], inertia[3], inertia[4], inertia[5], inertia[6], inertia[7], inertia[8]);
//#endif
}

iREAL delta::geometry::operators::physics::computeMass(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates,
    delta::geometry::material::MaterialType material)
{

  iREAL me=0, a[3], b[3], c[3], J;

  iREAL zero[3];
  zero[0] = 0;
  zero[1] = 0;
  zero[2] = 0;

  int rho= int(delta::geometry::material::materialToDensitymap.find(material)->second);

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

void delta::geometry::operators::physics::computeInverseInertia(
    iREAL inertia[9],
    iREAL inverse[9],
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
		inverse[j] = 0.0;
	}
  } else {
	for (int j = 0; j < 9; j++)
	{
	  inverse[j] = x[j];
	}
  }
}

iREAL delta::geometry::operators::physics::simplex_J (
    iREAL *a, iREAL *b, iREAL *c, iREAL *d)
{
  iREAL q [9], J;

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
