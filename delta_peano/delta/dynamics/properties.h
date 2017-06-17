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
       double getKineticRotationalEnergy(double velocity[3], double angular[3], double inertia[9], double mass);

       double getKineticEnergy(double velocity[3], double mass);

       double getRotationalEnergy(double angular[3], double inertia[9]);
     }
  }
}


#endif /* DELTA_DYNAMICS_PROPERTIES_H_ */
