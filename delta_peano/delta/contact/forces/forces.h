#include <delta/contact/contactpoint.h>
#include <vector>
#include "delta/geometry/material.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include "delta/core/delta.h"

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

	  void friction(
		  iREAL normal[3],
		  iREAL vi[3],
		  iREAL force,
		  iREAL friction[3],
		  int materialA,
		  int materialB,
		  bool isSphere);

	  void springSphere(
		  iREAL normal[3],
		  iREAL depth,
		  iREAL relativeVelocity[3],
		  iREAL massA,
		  iREAL massB,
		  iREAL f[3],
		  iREAL &forc);

	  /**
		@param angularVelocityA is a vector that indicates linear velocity from center of mass
		@param linearVelocityA is a vector that indicates angular velocity from center of mass
		@param massA is a floating number that indicates the mass
		@param-returned forceA is the total force returned as vector to indicate magnitude and direction
		@param torqueA is the total torque returned as vector to indicate torque

		@param angularVelocityB is a vector that indicates linear velocity from center of mass
		@param linearVelocityB is a vector that indicates angular velocity from center of mass
		@param massB is a floating number that indicates the mass
		@param-returned forceB is the total force returned as vector to indicate magnitude and opposite direction from A
		@param torqueB is the total torque returned as vector to indicate torque
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
