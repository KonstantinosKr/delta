/*
 * properties.cpp
 *
 *  Created on: 17 Jun 2017
 *      Author: konstantinos
 */

#include "delta/dynamics/properties.h"

double delta::dynamics::properties::getKineticRotationalEnergy(double velocity[3], double angular[3], double inertia[9], double mass){
  iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
  iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);

  return rotation+kinetic;
}

double delta::dynamics::properties::getKineticEnergy(double velocity[3], double mass){
  iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);
  return kinetic;
}

double delta::dynamics::properties::getRotationalEnergy(double angular[3], double inertia[9]){
  iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
  return rotation;
}


