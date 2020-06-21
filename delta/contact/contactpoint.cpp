#include "contactpoint.h"

delta::contact::contactpoint::contactpoint(){}

delta::contact::contactpoint::contactpoint(const contactpoint& copy) {
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

  friction = copy.friction;
  epsilonTotal = copy.epsilonTotal;

  master = copy.master;
  slave = copy.slave;
  penetration = copy.penetration;
}

#if defined(OMPProcess) || defined(OMPTriangle)
#pragma omp declare simd notinbranch
#endif
delta::contact::contactpoint::contactpoint(
  const iREAL&  xPA,
  const iREAL&  yPA,
  const iREAL&  zPA,
  const iREAL&  epsilonA,
  const int 	masterID,

  iREAL TP1[3],
  iREAL TP2[3],
  iREAL TP3[3],
  const iREAL&  xQB,
  const iREAL&  yQB,
  const iREAL&  zQB,
  const iREAL&  epsilonB,
  const int 	slaveID,
  const bool&	type
) {
  x[0] = (xPA+xQB)/2.0;
  x[1] = (yPA+yQB)/2.0;
  x[2] = (zPA+zQB)/2.0;

  P[0] = xPA;
  P[1] = yPA;
  P[2] = zPA;

  Q[0] = xQB;
  Q[1] = yQB;
  Q[2] = zQB;

  epsilonTotal = epsilonA+epsilonB;

  depth = (epsilonTotal - getDistance());

  //face vector of triangle
  iREAL V[3], W[3], N[3];
  V[0] = TP2[0] - TP1[0];
  V[1] = TP2[1] - TP1[1];
  V[2] = TP2[2] - TP1[2];
                              
  W[0] = TP3[0] - TP2[0];
  W[1] = TP3[1] - TP2[1];
  W[2] = TP3[2] - TP2[2];
                                                   
  N[0] = (V[1]*W[2])-(V[2]*W[1]);
  N[1] = (V[2]*W[0])-(V[0]*W[2]);
  N[2] = (V[0]*W[1])-(V[1]*W[0]);
                                                                        
  iREAL mag = std::sqrt((N[0]*N[0])+(N[1]*N[1])+(N[2]*N[2]));

  iREAL np[3];
  np[0] = N[0]/mag;
  np[1] = N[1]/mag;
  np[2] = N[2]/mag;

  normal[0] = ((P[0]-Q[0])/getDistance());
  normal[1] = ((P[1]-Q[1])/getDistance());
  normal[2] = ((P[2]-Q[2])/getDistance());

  //direction is less than 0 then vectors point opposite direction (contact inside of body)
  //direction is more than 0 then vecotrs point same direction (contact outside of body)
  iREAL direction = DOT(np, normal);

  if(direction < 0.0) 
  {
    penetration = -1;
    //printf("direction:%f\n", direction);
  } 
  else
  {
    penetration = 1;
  }

  friction = type;

  master = masterID;
  slave = slaveID;
}

#if defined(OMPProcess) || defined(OMPTriangle)
#pragma omp declare simd notinbranch
#endif
delta::contact::contactpoint::contactpoint(
  const iREAL&  xPA,
  const iREAL&  yPA,
  const iREAL&  zPA,
  const iREAL&  epsilonA,
  const int 		masterID,

  const iREAL&  xQB,
  const iREAL&  yQB,
  const iREAL&  zQB,
  const iREAL&  epsilonB,
  const int 	slaveID,
  const bool&	type
) {
  x[0] = (xPA+xQB)/2.0;
  x[1] = (yPA+yQB)/2.0;
  x[2] = (zPA+zQB)/2.0;

  P[0] = xPA;
  P[1] = yPA;
  P[2] = zPA;

  Q[0] = xQB;
  Q[1] = yQB;
  Q[2] = zQB;

  epsilonTotal = epsilonA+epsilonB;

  depth = (epsilonTotal - getDistance());

  normal[0] = ((xPA-xQB)/getDistance());
  normal[1] = ((yPA-yQB)/getDistance());
  normal[2] = ((zPA-zQB)/getDistance());

  friction = type;

  master = masterID;
  slave = slaveID;
  penetration = 1;
}

iREAL delta::contact::contactpoint::getDistance() const {
	return std::sqrt(((Q[0]-P[0])*(Q[0]-P[0]))+((Q[1]-P[1])*(Q[1]-P[1]))+((Q[2]-P[2])*(Q[2]-P[2])));
}

std::string delta::contact::contactpoint::toString() const {
  std::ostringstream msg;
  msg << "(" << x[0] << "," << x[1] << "," << x[2] << "),(" << normal[0] << "," << normal[1] << "," << normal[2] << ")";
  return msg.str();
}
