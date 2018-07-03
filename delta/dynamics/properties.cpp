/*
 * properties.cpp
 *
 *  Created on: 17 Jun 2017
 *      Author: konstantinos
 */

#include "delta/dynamics/properties.h"

iREAL delta::dynamics::properties::getKineticRotationalEnergy(
    iREAL 	velocity[3],
    iREAL 	angular[3],
    iREAL 	inertia[9],
    iREAL 	mass)
{
  iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
  iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);

  return rotation+kinetic;
}

iREAL delta::dynamics::properties::getKineticEnergy(
    iREAL 	velocity[3],
    iREAL 	mass)
{
  iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);
  return kinetic;
}

iREAL delta::dynamics::properties::getRotationalEnergy(
    iREAL 	angular[3],
    iREAL 	inertia[9])
{
  iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
  return rotation;
}


