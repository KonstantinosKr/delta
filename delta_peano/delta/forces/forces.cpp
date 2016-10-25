#include "forces.h"

#define SPRING 1E6
#define DAMPER 0.1
#define FRICTION 0.3
#define FRICTIONROLLING 0.0005

double delta::forces::springDashpot(iREAL normal[3], iREAL depth, iREAL relativeVelocity[3], iREAL massA, iREAL massB, iREAL f[3])
{
  iREAL ma = (1/massA) + (1/massB);
  
  //iREAL damp = log(0.7)*sqrt(1/(log(0.7)*log(0.7)+(3.14*3.14)));
  //printf("damp:%f\n", damp)

  iREAL damping = DAMPER * 2 * sqrt(SPRING*ma);

  iREAL velocity = (relativeVelocity[0]*normal[0]) + (relativeVelocity[1]*normal[1]) + (relativeVelocity[2]*normal[2]);

  iREAL force = SPRING*depth + damping*velocity;

  f[0] = force*normal[0];
  f[1] = force*normal[1];
  f[2] = force*normal[2];

  //printf("Velocity:%f DEPTH:%f normal: %f %f %f FORCE:%f force: %f %f %f vel:%f %f %f\n", velocity, depth, normal[0], normal[1], normal[2], force, f[0], f[1], f[2], relativeVelocity[0], relativeVelocity[1], relativeVelocity[2]);
  return force;
}

void delta::forces::friction(iREAL normal[3], iREAL vi[3], iREAL force, iREAL friction[3])
{
  iREAL vt[3];

  vt[0] = vi[0] - normal[0]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));
  vt[1] = vi[1] - normal[1]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));
  vt[2] = vi[2] - normal[2]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));

  friction[0] =  -vt[0]*FRICTION*force;
  friction[1] =  -vt[1]*FRICTION*force;
  friction[2] =  -vt[2]*FRICTION*force;
}

void delta::forces::getContactForces(
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
	bool  isSphere)
{

    for(unsigned int k = 0; k<conpnt.size(); k++)
    {
        iREAL z[3], vi[3], vj[3], vij[3];
        
        //contact point - position i
        z[0] = conpnt[k].x[0]-positionA[0];
        z[1] = conpnt[k].x[1]-positionA[1];
        z[2] = conpnt[k].x[2]-positionA[2];
        
        //cross product - relative angular i to contact point plus linear i
        vi[0] = angularVelocityA[1]*z[2]-angularVelocityA[2]*z[1] + linearVelocityA[0];
        vi[1] = angularVelocityA[2]*z[0]-angularVelocityA[0]*z[2] + linearVelocityA[1];
        vi[2] = angularVelocityA[0]*z[1]-angularVelocityA[1]*z[0] + linearVelocityA[2];
        
        //contact point - position j
        z[0] = conpnt[k].x[0]-positionB[0];
        z[1] = conpnt[k].x[1]-positionB[1];
        z[2] = conpnt[k].x[2]-positionB[2];
        
        //cross product - relative angular j to contact point plus linear j
        vj[0] = angularVelocityB[1]*z[2]-angularVelocityB[2]*z[1] + linearVelocityB[0];
        vj[1] = angularVelocityB[2]*z[0]-angularVelocityB[0]*z[2] + linearVelocityB[1];
        vj[2] = angularVelocityB[0]*z[1]-angularVelocityB[1]*z[0] + linearVelocityB[2];
        
        //relative velocities
        vij[0] = vj[0] - vi[0];
        vij[1] = vj[1] - vi[1];
        vij[2] = vj[2] - vi[2];

        iREAL f[3], friction[3];

        iREAL forc = delta::forces::springDashpot(conpnt[k].normal, conpnt[k].depth, vij, massA, massB, f);

        friction[0] = 0.0;
        friction[1] = 0.0;
        friction[2] = 0.0;

        if(conpnt[k].frictionType > 2)
        	delta::forces::friction(conpnt[k].normal, vi, forc, friction);

        f[0] = f[0] + friction[0];
        f[1] = f[1] + friction[1];
        f[2] = f[2] + friction[2];

        //accumulate force
        force[0] += f[0];
        force[1] += f[1];
        force[2] += f[2];

        iREAL arm[3];
        //contact-position = arm
        arm[0] = conpnt[k].x[0]-positionA[0];
        arm[1] = conpnt[k].x[1]-positionA[1];
        arm[2] = conpnt[k].x[2]-positionA[2];

        //cross product accumulate torque
        torque[0] += arm[1]*(f[2]) - arm[2]*(f[1]);
        torque[1] += arm[2]*(f[0]) - arm[0]*(f[2]);
        torque[2] += arm[0]*(f[1]) - arm[1]*(f[0]);

        if(isSphere)
        {
			//relative angular velocities
			vij[0] = angularVelocityA[0] - angularVelocityB[0];
			vij[1] = angularVelocityA[1] - angularVelocityB[1];
			vij[2] = angularVelocityA[2] - angularVelocityB[2];

			iREAL w = std::abs(sqrt(vij[0]*vij[0]+vij[1]*vij[1]+vij[2]*vij[2]));
			//printf("W:%f | wij: %f %f %f\n", w, vij[0], vij[1], vij[2]);

			if(w>0.0)
			{
				torque[0] += -(vij[0]/w)*FRICTIONROLLING*forc;
				torque[1] += -(vij[1]/w)*FRICTIONROLLING*forc;
				torque[2] += -(vij[2]/w)*FRICTIONROLLING*forc;
			}
        }
    }
}
