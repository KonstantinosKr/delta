#include "Object.h"

#include <delta/geometry/primitive/granulate.h>
#include <delta/geometry/primitive/cube.h>
#include <delta/geometry/defined/graphite.h>
#include <delta/geometry/defined/hopper.h>
#include "delta/geometry/mesh/mesh.h"

delta::geometry::Object::Object()
{
  this->_component = "particle";
  this->_particleID = 0;

  this->_centre[0] = 0.0;
  this->_centre[1] = 0.0;
  this->_centre[2] = 0.0;

  this->_material = delta::geometry::material::MaterialType::WOOD;
  this->_isObstacle = false;
  this->_isFriction = false;

  this->_linearVelocity[0] = 0.0;
  this->_linearVelocity[1] = 0.0;
  this->_linearVelocity[2] = 0.0;

  this->_angularVelocity[0] = 0.0;
  this->_angularVelocity[1] = 0.0;
  this->_angularVelocity[2] = 0.0;

  this->_rad = 0.0;

  this->_wx = 0;
  this->_wy = 0;
  this->_wz = 0;

  this->_rx = 0;
  this->_ry = 0;
  this->_rz = 0;

  this->_mass = 0.0;
  this->_epsilon = 0.0;
}

delta::geometry::Object::Object(
    std::string                   			component,
    int                           			particleID,
    std::array<double, 3>         			centre,
    delta::geometry::material::MaterialType 	material,
    bool                          			isObstacle,
    bool                          			isFriction,
	iREAL 									epsilon,
	std::array<double, 3> linear,
	std::array<double, 3> angular)
{
  this->_component = component;
  this->_particleID = particleID;

  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_material = material;
  this->_isObstacle = isObstacle;
  this->_isFriction = isFriction;

  this->_linearVelocity[0] = linear[0];
  this->_linearVelocity[1] = linear[1];
  this->_linearVelocity[2] = linear[2];

  this->_angularVelocity[0] = angular[0];
  this->_angularVelocity[1] = angular[1];
  this->_angularVelocity[2] = angular[2];

  this->_rad = 0.0;

  this->_wx = 0;
  this->_wy = 0;
  this->_wz = 0;

  this->_rx = 0;
  this->_ry = 0;
  this->_rz = 0;

  this->_mass = 0.0;
  this->_epsilon = epsilon;
}

void delta::geometry::Object::generateSphere(
    double                        rad)
{
  this->_rad = rad;
  this->_centreOfMass[0] = _centre[0];
  this->_centreOfMass[1] = _centre[1];
  this->_centreOfMass[2] = _centre[2];
}

void delta::geometry::Object::generateMesh(
    double wx,
    double wy,
    double wz,
    double rx,
    double ry,
    double rz,
    int mesh,
    double rad)
{
  iREAL position[3] = {_centre[0], _centre[1], _centre[2]};

  delta::geometry::mesh::Mesh *geometry;
  if(_component == "granulate")
  {
	geometry = delta::geometry::primitive::granulate::generateParticle(position, rad*2, mesh);
  } else if(_component == "granulate-load")
  {
	geometry = delta::geometry::primitive::granulate::loadParticle(position, (rad*2));
  } else if(_component == "cube")
  {
	geometry = delta::geometry::primitive::cube::generateHullCube(position, wx, wy, wz, rx, ry, rz, mesh);
  }
  else if(_component == "hopper")
  {
    double _hopperHatch = 0.05; double _hopperThickness = 0.005; int refinement = 0;
    geometry = delta::geometry::defined::generateHopper(position, wx, _hopperThickness, wy, _hopperHatch, refinement, 0.01);
  } else if(_component == "FB")
  {
	geometry = delta::geometry::defined::generateBrickFB(position, rad);
  } else if(_component == "mesh")
  {

  }

  double mass, centerOfMass[3], inertia[9], inverse[9];

  geometry->computeInertia(_material, mass, centerOfMass, inertia);
  geometry->computeInverseInertia(inertia, inverse, _isObstacle);

  std::vector<double> xCoordinates;
  std::vector<double> yCoordinates;
  std::vector<double> zCoordinates;

  geometry->flatten(xCoordinates, yCoordinates, zCoordinates);

  //printf("number of triangles:%i\n", xCoordinates.size()/3);

  for(unsigned i=0;i<xCoordinates.size(); i++)
  {
    _xCoordinates.push_back(xCoordinates[i]);
    _yCoordinates.push_back(yCoordinates[i]);
    _zCoordinates.push_back(zCoordinates[i]);
  }

  _mesh = geometry;

  _inertia[0] = inertia[0];
  _inertia[1] = inertia[1];
  _inertia[2] = inertia[2];
  _inertia[3] = inertia[3];
  _inertia[4] = inertia[4];
  _inertia[5] = inertia[5];
  _inertia[6] = inertia[6];
  _inertia[7] = inertia[7];
  _inertia[8] = inertia[8];

  _inverse[0] = inertia[0];
  _inverse[1] = inertia[1];
  _inverse[2] = inertia[2];
  _inverse[3] = inertia[3];
  _inverse[4] = inertia[4];
  _inverse[5] = inertia[5];
  _inverse[6] = inertia[6];
  _inverse[7] = inertia[7];
  _inverse[8] = inertia[8];

  _centreOfMass[0] = centerOfMass[0];
  _centreOfMass[1] = centerOfMass[1];
  _centreOfMass[2] = centerOfMass[2];

  _mass = mass;
  _rad = rad;
}

std::string delta::geometry::Object::getComponent()
{
  return _component;
}

void delta::geometry::Object::setParticleID(int id)
{
  _particleID = id;
}

std::vector<double> delta::geometry::Object::getxCoordinates()
{
  return _xCoordinates;
}

std::vector<double> delta::geometry::Object::getyCoordinates()
{
  return _yCoordinates;
}

std::vector<double> delta::geometry::Object::getzCoordinates()
{
  return _zCoordinates;
}

void delta::geometry::Object::setMesh(
    std::vector<double> xCoordinates,
    std::vector<double> yCoordinates,
    std::vector<double> zCoordinates)
{
  _xCoordinates.clear();
  _yCoordinates.clear();
  _zCoordinates.clear();

  for(unsigned i=0; i< xCoordinates.size(); i++)
  {
    _xCoordinates.push_back(xCoordinates[i]);
    _yCoordinates.push_back(yCoordinates[i]);
    _zCoordinates.push_back(zCoordinates[i]);
  }

  _mesh = new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}

delta::geometry::mesh::Mesh delta::geometry::Object::getMesh()
{
  return *_mesh;
}

void delta::geometry::Object::setMesh(
    delta::geometry::mesh::Mesh& mesh)
{
  _xCoordinates.clear();
  _yCoordinates.clear();
  _zCoordinates.clear();

  _xCoordinates = mesh.getxCoordinates();
  _yCoordinates = mesh.getyCoordinates();
  _zCoordinates = mesh.getzCoordinates();

  _mesh = &mesh;
}

int delta::geometry::Object::getParticleID()
{
  return _particleID;
}

std::array<double, 3> delta::geometry::Object::getCentre()
{
  return _centre;
}

void delta::geometry::Object::setCentre(double centre[3])
{
  this->_centre[0] = centre[0];
  this->_centre[1] = centre[1];
  this->_centre[2] = centre[2];

  this->_centreOfMass[0] = _centre[0];
  this->_centreOfMass[1] = _centre[1];
  this->_centreOfMass[2] = _centre[2];
}

double delta::geometry::Object::getEpsilon()
{
  return _epsilon;
}

void delta::geometry::Object::setEpsilon(double epsilon)
{
  _epsilon = epsilon;
}

double delta::geometry::Object::getRad()
{
  return _rad;
}

void delta::geometry::Object::setRad(double rad)
{
  _rad = rad;
}

double delta::geometry::Object::getMass()
{
  return _mass;
}

void delta::geometry::Object::setMass(double mass)
{
  _mass = mass;
}

delta::geometry::material::MaterialType delta::geometry::Object::getMaterial()
{
  return _material;
}

bool delta::geometry::Object::getIsObstacle()
{
  return _isObstacle;
}

bool delta::geometry::Object::getIsFriction()
{
  return _isFriction;
}

std::array<double, 9> delta::geometry::Object::getInertia()
{
  return _inertia;
}

void delta::geometry::Object::setInertia(double inertia[9])
{
  _inertia[0] = inertia[0];
  _inertia[1] = inertia[1];
  _inertia[2] = inertia[2];
  _inertia[3] = inertia[3];
  _inertia[4] = inertia[4];
  _inertia[5] = inertia[5];
  _inertia[6] = inertia[6];
  _inertia[7] = inertia[7];
  _inertia[8] = inertia[8];
}

std::array<double, 9> delta::geometry::Object::getInverse()
{
  return _inverse;
}

void delta::geometry::Object::setInverse(double inverse[9])
{
  _inverse[0] = inverse[0];
  _inverse[1] = inverse[1];
  _inverse[2] = inverse[2];
  _inverse[3] = inverse[3];
  _inverse[4] = inverse[4];
  _inverse[5] = inverse[5];
  _inverse[6] = inverse[6];
  _inverse[7] = inverse[7];
  _inverse[8] = inverse[8];
}

std::array<double, 3> delta::geometry::Object::getxyzDimensions()
{
  return _xyzDimensions;
}

std::array<double, 3> delta::geometry::Object::getCentreOfMass()
{
  return _centreOfMass;
}

void delta::geometry::Object::setCentreOfMass(double centreOfMass[3])
{
  _centreOfMass[0] = centreOfMass[0];
  _centreOfMass[1] = centreOfMass[1];
  _centreOfMass[2] = centreOfMass[2];
}

void delta::geometry::Object::setLinearVelocity(std::array<double, 3>  linearVelocity)
{
  this->_linearVelocity[0] = linearVelocity[0];
  this->_linearVelocity[1] = linearVelocity[1];
  this->_linearVelocity[2] = linearVelocity[2];
}

void delta::geometry::Object::setAngularVelocity(std::array<double, 3>  angularVelocity)
{
  this->_angularVelocity[0] = angularVelocity[0];
  this->_angularVelocity[1] = angularVelocity[1];
  this->_angularVelocity[2] = angularVelocity[2];
}

std::array<double, 3> delta::geometry::Object::getLinearVelocity()
{
  return _linearVelocity;
}

std::array<double, 3> delta::geometry::Object::getAngularVelocity()
{
  return _angularVelocity;
}

double delta::geometry::Object::getMinX()
{
  iREAL minx = std::numeric_limits<double>::max();
  if(_xCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_xCoordinates.size(); i++)
    {
      if(_xCoordinates[i] < minx) minx = _xCoordinates[i];
    }
  } else {
    minx = _centreOfMass[0] - _rad;
  }
  return minx;
}

double delta::geometry::Object::getMaxX()
{
  iREAL maxx = std::numeric_limits<double>::min();
  if(_xCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_xCoordinates.size(); i++)
    {
      if(_xCoordinates[i] > maxx) maxx = _xCoordinates[i];
    }
  } else {
    maxx = _centreOfMass[0] + _rad;
  }
  return maxx;
}

double delta::geometry::Object::getMinY()
{
  iREAL miny = std::numeric_limits<double>::max();
  if(_yCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_yCoordinates.size(); i++)
    {
      if(_yCoordinates[i] < miny) miny = _yCoordinates[i];
    }
  } else {
    miny = _centreOfMass[0] - _rad;
  }
  return miny;
}

double delta::geometry::Object::getMaxY()
{
  iREAL maxy = std::numeric_limits<double>::min();
  if(_yCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_yCoordinates.size(); i++)
    {
      if(_xCoordinates[i] > maxy) maxy = _xCoordinates[i];
    }
  } else {
    maxy = _centreOfMass[0] + _rad;
  }
  return maxy;
}

double delta::geometry::Object::getMinZ()
{
  iREAL minz = std::numeric_limits<double>::max();
  if(_xCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_xCoordinates.size(); i++)
    {
      if(_xCoordinates[i] < minz) minz = _zCoordinates[i];
    }
  } else {
    minz = _centreOfMass[0] - _rad;
  }
  return minz;
}

double delta::geometry::Object::getMaxZ()
{
  iREAL maxz = std::numeric_limits<double>::min();
  if(_zCoordinates.size() > 0)
  {
    for(unsigned i=0; i<_yCoordinates.size(); i++)
    {
      if(_xCoordinates[i] > maxz) maxz = _xCoordinates[i];
    }
  } else {
    maxz = _centreOfMass[0] + _rad;
  }
  return maxz;
}

iREAL delta::geometry::Object::computeVolume()
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->getMesh().flatten(xCoordinates, yCoordinates, zCoordinates);

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
void delta::geometry::Object::computeInertia(
		delta::geometry::material::MaterialType material,
		iREAL& mass,
		iREAL center[3],
		iREAL inertia[9])
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->getMesh().flatten(xCoordinates, yCoordinates, zCoordinates);

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

iREAL delta::geometry::Object::computeMass(
    delta::geometry::material::MaterialType material)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

  this->getMesh().flatten(xCoordinates, yCoordinates, zCoordinates);

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

void delta::geometry::Object::computeInverseInertia(
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

iREAL delta::geometry::Object::simplex_J (
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


delta::geometry::Object::~Object() {

}


