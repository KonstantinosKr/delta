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

		/*
		* Get Rotational Kinetic Energy
		*
		* Returns the rotational energy of a particle.
		*
		*
		* @param velocity is the velocity of the particle
    	* @param angular is the angular velocity
    	* @param inertia is the inertia of the particle
		* @param mass
		* @return energy
		*/
       iREAL getKineticRotationalEnergy(
           iREAL velocity[3],
           iREAL angular[3],
           iREAL inertia[9],
           iREAL mass);

    	/*
    	* Get Kinetic Energy
    	*
    	* Returns the linear velocity kinetic energy of a particle.
    	*
    	*
		* @param velocity is the velocity of the particle
		* @param mass
    	* @return energy
    	*/
       iREAL getKineticEnergy(
           iREAL velocity[3],
           iREAL mass);

    	/*
    	* Get Rotational Energy
    	*
    	* Returns the rotational energy of a particle.
    	*
    	*
    	* @param angular is the angular velocity
    	* @param inertia is the inertia of the particle
    	* @return energy
    	*/
       iREAL getRotationalEnergy(
           iREAL angular[3],
           iREAL inertia[9]);
     }
  }
}


#endif /* DELTA_DYNAMICS_PROPERTIES_H_ */
