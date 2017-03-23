#include <vector>
#include "delta/collision/contactpoint.h"
#include "delta/sys/delta.h"
#include "math.h"


namespace delta {
  namespace forces {

  	double spring(iREAL normal[3], iREAL conpnt[3], iREAL depth, iREAL relativeVelocity[3],
                  iREAL positionASpatial[3], iREAL positionBSpatial[3],
                  iREAL positionAReferential[3], iREAL positionBReferential[3],
                  iREAL massA, iREAL massB,
                  iREAL rotationA[9], iREAL rotationB[9],
                  iREAL inverseA[9], iREAL inverseB[9], iREAL f[3]);

  	double springSphere(iREAL normal[3], iREAL depth, iREAL relativeVelocity[3], iREAL massA, iREAL massB, iREAL f[3]);

  	void friction(iREAL normal[3], iREAL vi[3], iREAL forc, iREAL friction[3]);
  	void frictionSphere(iREAL normal[3], iREAL vi[3], iREAL forc, iREAL friction[3], int materialA, int materialB);

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
  }

}
