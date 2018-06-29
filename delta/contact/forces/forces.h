#include <delta/contact/contactpoint.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>

#ifndef _DELTA_FORCES_H_
#define _DELTA_FORCES_H_

#define NNMUL(A, B, C)\
{\
 (C) [0] = (A)[0]*(B)[0]+(A)[3]*(B)[1]+(A)[6]*(B)[2];\
 (C) [1] = (A)[1]*(B)[0]+(A)[4]*(B)[1]+(A)[7]*(B)[2];\
 (C) [2] = (A)[2]*(B)[0]+(A)[5]*(B)[1]+(A)[8]*(B)[2];\
 (C) [3] = (A)[0]*(B)[3]+(A)[3]*(B)[4]+(A)[6]*(B)[5];\
 (C) [4] = (A)[1]*(B)[3]+(A)[4]*(B)[4]+(A)[7]*(B)[5];\
 (C) [5] = (A)[2]*(B)[3]+(A)[5]*(B)[4]+(A)[8]*(B)[5];\
 (C) [6] = (A)[0]*(B)[6]+(A)[3]*(B)[7]+(A)[6]*(B)[8];\
 (C) [7] = (A)[1]*(B)[6]+(A)[4]*(B)[7]+(A)[7]*(B)[8];\
 (C) [8] = (A)[2]*(B)[6]+(A)[5]*(B)[7]+(A)[8]*(B)[8];\
}

namespace delta {
  namespace contact {
	namespace forces {

		/*
		* Spring Force
		*
		* Returns the single force vector at the contact point between two interacting particles.
		*
		* @param normal is the contact normal
		* @param conpnt is the contact point
		* @param depth is the penetration depth
		* @param vij relative linear and angular velocities between the two particles
		* @param positionASpatial is the spatial position of particle A
		* @param positionBSpatial is the spatial position of particle B
		* @param positionAReferential is the referential position of particle A
		* @param positionBReferential is the referential position of particle B
		* @param massA is the mass of particle A
		* @param massB is the mass of particle B
		* @param rotationA is the rotational matrix of particle A
		* @param rotationB is the rotational matrix of particle A
		* @param inverseA is the inverse inertia of particle A
		* @param inverseB is the inverse inertia of particle B
		* @param f is the force vector at the normal
		* @param forc is the force magnitude
		* @return void
		*/
	  void spring(
		  iREAL normal[3],
		  iREAL conpnt[3],
		  iREAL depth,
		  iREAL vij[3],
		  iREAL positionASpatial[3],
		  iREAL positionBSpatial[3],
		  iREAL positionAReferential[3],
		  iREAL positionBReferential[3],

		  iREAL massA,
		  iREAL massB,
		  iREAL rotationA[9],
		  iREAL rotationB[9],
		  iREAL inverseA[9],
		  iREAL inverseB[9],
		  iREAL f[3],
		  iREAL &forc);


	  /*
	   * Friction
	   *
	   * Returns the friction vector at the contact point.
	   *
	   * @param normal is the contact normal
	   * @param vi is the relative linear velocity between the two spheres
	   * @param force is the applied force between particle A and B
	   * @param materialA is the material type of particle A
	   * @param materialB is the material type of particle B
	   * @param isSphere indicate whether the particles are spheres
	   * @return void
	   */
	  void friction(
		  iREAL normal[3],
		  iREAL vi[3],
		  iREAL force,
		  iREAL friction[3],
		  int materialA,
		  int materialB,
		  bool isSphere);

	  /*
	   * Spring Sphere
	   *
	   * Returns the force between two spheres
	   *
	   * @param normal is the contact normal
	   * @param depth is the penetration depth
	   * @param relativeVelocity is the relative velocity between the two spheres
	   * @param massA is the mass of particle A
	   * @param massB is the mass of particle B
	   * @param f is the vector force at the contact point
	   * @param force is the force magnitude at the contact point
	   * @return void
	   */
	  void springSphere(
		  iREAL normal[3],
		  iREAL depth,
		  iREAL relativeVelocity[3],
		  iREAL massA,
		  iREAL massB,
		  iREAL f[3],
		  iREAL &forc);

	  /**
	   * Get Contact Forces
	   *
	   * Returns the contact forces of all contacts between two interacting bodies
	   *
	   * @param angularA is a vector that indicates linear velocity from center of mass
 	   * @param refAngularA is a vector that indicates linear velocity from center of mass
	   * @param linearA is a vector that indicates angular velocity from center of mass
	   * @param massA is a floating number that indicates the mass
	   * @param inverseA is the inverse of the inertia
	   * @param rotationA is the orientation matrix
	   * @param materialA is the material type
	   *
	   * @param angularB is a vector that indicates linear velocity from center of mass
 	   * @param refAngularB is a vector that indicates linear velocity from center of mass
	   * @param linearB is a vector that indicates angular velocity from center of mass
	   * @param massB is a floating number that indicates the mass
	   * @param inverseB is the inverse of the inertia
	   * @param rotationB is the orientation matrix
	   * @param materialB is the material type
	   *
	   * @param-returned force is the total force returned as vector to indicate magnitude and direction
	   * @param torque is the total torque returned as vector to indicate torque
	   * @param isSphere indicate whether the objects are spheres
	   * @return void
	   *
	   */
	  void getContactsForces(
		std::vector<delta::contact::contactpoint> &conpnt,
		iREAL positionASpatial[3],
		iREAL positionAReferential[3],
		iREAL angularA[3],
		iREAL refAngularA[3],
		iREAL linearA[3],
		iREAL massA,
		iREAL inverseA[9],
		iREAL rotationA[9],
		int   materialA,

		iREAL positionB[3],
		iREAL positionBReferential[3],
		iREAL angularB[3],
		iREAL refAngularB[3],
		iREAL linearB[3],
		iREAL massB,
		iREAL inverseB[9],
		iREAL rotationB[9],
		int   materialB,

		iREAL force[3],
		iREAL torque[3],
		bool  isSphere);

	  /*
	   * dummy function of getContactsForces() used for plotting
	   */
	  void getContactForce(
		delta::contact::contactpoint conpnt,
		iREAL positionASpatial[3],
		iREAL positionAReferential[3],
		iREAL angularA[3],
		iREAL refAngularA[3],
		iREAL linearA[3],
		iREAL massA,
		iREAL inverseA[9],
		iREAL rotationA[9],
		int   materialA,
		iREAL positionB[3],
		iREAL positionBReferential[3],
		iREAL angularB[3],
		iREAL refAngularB[3],
		iREAL linearB[3],
		iREAL massB,
		iREAL inverseB[9],
		iREAL rotationB[9],
		int   materialB,
		iREAL force[3],
		iREAL torque[3],
		iREAL frict[3],
		bool  isSphere);
	}
  }
}


#endif
