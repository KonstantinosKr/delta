#include "delta/collision/contactpoint.h"

#include <limits>
#include <cmath>
#include <sstream>

delta::collision::contactpoint::contactpoint(){}

delta::collision::contactpoint::contactpoint(const contactpoint& copy) {
  x[0] = copy.x[0];
  x[1] = copy.x[1];
  x[2] = copy.x[2];

  normal[0] = copy.normal[0];
  normal[1] = copy.normal[1];
  normal[2] = copy.normal[2];

  depth = copy.depth;

  P[0] = copy.P[0];
  P[1] = copy.P[1];
  P[2] = copy.P[2];

  Q[0] = copy.Q[0];
  Q[1] = copy.Q[1];
  Q[2] = copy.Q[2];

  frictionVector[0] = copy.frictionVector[0];
  frictionVector[1] = copy.frictionVector[1];
  frictionVector[2] = copy.frictionVector[2];

  friction = copy.friction;
  epsilonTotal = copy.epsilonTotal;
}

#if ompTriangle || ompParticle
#pragma omp declare simd notinbranch
#endif
delta::collision::contactpoint::contactpoint(
  const double&  xPA,
  const double&  yPA,
  const double&  zPA,
  const double&  epsilonA,

  const double&  xPB,
  const double&  yPB,
  const double&  zPB,
  const double&  epsilonB,
  const bool&	 type
) {

  x[0] = (xPA+xPB)/2.0;
  x[1] = (yPA+yPB)/2.0;
  x[2] = (zPA+zPB)/2.0;

  P[0] = xPA;
  P[1] = yPA;
  P[2] = zPA;

  Q[0] = xPB;
  Q[1] = yPB;
  Q[2] = zPB;

  epsilonTotal = epsilonA+epsilonB;

  depth = (epsilonTotal - getDistance());

  normal[0] = ((xPA-xPB)/getDistance());
  normal[1] = ((yPA-yPB)/getDistance());
  normal[2] = ((zPA-zPB)/getDistance());

  friction = type;
}

#if ompTriangle || ompParticle
#pragma omp declare simd notinbranch
#endif
double delta::collision::contactpoint::getDistance() const {
  return std::sqrt(((Q[0]-P[0])*(Q[0]-P[0]))+((Q[1]-P[1])*(Q[1]-P[1]))+((Q[2]-P[2])*(Q[2]-P[2])));
}

std::string delta::collision::contactpoint::toString() const {
  std::ostringstream msg;
  msg << "(" << x[0] << "," << x[1] << "," << x[2] << "),(" << normal[0] << "," << normal[1] << "," << normal[2] << ")";
  return msg.str();
}
