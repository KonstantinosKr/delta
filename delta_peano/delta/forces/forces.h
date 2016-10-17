#include <vector>
#include "delta/collision/contactpoint.h"
#include "math.h"


namespace delta {
  namespace forces {

  	double springDashpot(iREAL normal[3], iREAL depth, iREAL relativeVelocity[3], iREAL massA, iREAL massB, iREAL f[3]);

  	void friction(iREAL normal[3], iREAL vi[3], iREAL forc, iREAL friction[3]);

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
  	void getContactForces(
      std::vector<delta::collision::contactpoint> &conpnt,
      iREAL positionA[3],
      iREAL angularVelocityA[3],
      iREAL linearVelocityA[3],
      iREAL massA,
      iREAL positionB[3],
      iREAL angularVelocityB[3],
      iREAL linearVelocityB[3],
      iREAL massB,
      iREAL force[3],
      iREAL torque[3],
	  bool  isSphere);
  }
}
