/*
 * properties.h
 *
 *  Created on: 17 Jun 2017
 *      Author: konstantinos
 */

#ifndef DELTA_DYNAMICS_PROPERTIES_H_
#define DELTA_DYNAMICS_PROPERTIES_H_


namespace delta {
  namespace dynamics {
     namespace properties {
       iREAL getKineticRotationalEnergy(
           iREAL velocity[3],
           iREAL angular[3],
           iREAL inertia[9],
           iREAL mass);

       iREAL getKineticEnergy(
           iREAL velocity[3],
           iREAL mass);

       iREAL getRotationalEnergy(
           iREAL angular[3],
           iREAL inertia[9]);
     }
  }
}


#endif /* DELTA_DYNAMICS_PROPERTIES_H_ */
