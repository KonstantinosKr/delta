#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "tmr.h"
#include "input.h"
#include "output.h"
#include "contact.h"
#include "dynamics.h"
#include "forces.h"
#include <iostream>
#include <vector>

int main (int argc, char **argv)
{
  //input("input/twoparticles.py");

  iREAL *angular[6]; /* angular velocities (referential, spatial) */
  iREAL *linear[3]; /* linear velocities */
  iREAL *rotation[9]; /* rotation operators */
  iREAL *position[6]; /* mass center current and reference positions */
  iREAL *inertia[9]; /* inertia tensors */
  iREAL *mass; /* scalar mass */
  iREAL *force[3]; /* total spatial force */
  iREAL *torque[3]; /* total spatial torque */
  iREAL *inverse[9]; /* inverse inertia tensors */
  
  int *parmat; //particle material
  
  material::ikind[0] = GRANULAR;
  material::ikind[1] = GRANULAR;
  material::ikind[2] = GRANULAR;
  
  //GRANULAR interaction type parameters 
  material::iparam[SPRING][GRANULAR] = 1;
  material::iparam[DAMPER][GRANULAR] = 1;
  material::iparam[FRISTAT][GRANULAR] = 0;
  material::iparam[FRIDYN][GRANULAR] = 0;
  material::iparam[FRIROL][GRANULAR] = 0;
  material::iparam[FRIDRIL][GRANULAR] = 0;
  material::iparam[KSKN][GRANULAR] = 0;
  material::iparam[LAMBDA][GRANULAR] = 0;
  material::iparam[YOUNG2][GRANULAR] = 0;
  material::iparam[KSKN2][GRANULAR] = 0;
  material::iparam[SIGC][GRANULAR] = 0;
  material::iparam[TAUC][GRANULAR] = 0;
  material::iparam[ALPHA][GRANULAR] = 0;

  iREAL gravity[3];
  gravity[0] = 0;
  gravity[1] = 0;
  gravity[2] = 0;
  
  iREAL *t[6][3]; /* triangles */
  
  iREAL *p[3],*q[3];//p and q points
  
	int nb;
  int nt = 0; /* number of triangles */
  int *pid; /*particle identifier */
  int *tid; /* triangle identifiers */
  
  iREAL lo[3] = {-500, -500, -500}; /* lower corner */
  iREAL hi[3] = {500, 500, 500}; /* upper corner */
  
  int size = 27000000; /* memory buffer size */
   
	for (int i = 0; i < 3; i ++)
	{ 
		t[0][i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
		t[1][i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
		t[2][i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
		t[3][i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
		t[4][i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
		t[5][i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
		
    linear[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
    
    torque[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
    force[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];

		p[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
		q[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
  }
	
	for (int i = 0; i < 6; i++)
	{
		angular[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
    position[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
	}
 
  for (int i = 0; i<9; i++)
  {
    inverse[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
    inertia[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
    rotation[i] = (iREAL *) new iREAL[size*sizeof(iREAL)];
  }
	
  parmat = (int *) new iREAL[size*sizeof(int)];
	
	tid = (int *) new iREAL[size*sizeof(int)];
	pid = (int *) new iREAL[size*sizeof(int)];

	mass = (iREAL *) new iREAL[size*sizeof(iREAL)];

	for(int i=0;i<size;i++) tid[i] = INT_MAX; 
	
	init_enviroment(0, nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);  
	printf("NT:%i NB:%i\n", nt, nb);
  
  std::vector<contactpoint> *conpnt = new std::vector<contactpoint>[nb];
 
  iREAL step = 1E-4; int timesteps=100;
  
  //step = critical (nt, mass, pairnum, iparam);
  
  dynamics::euler(nb, angular, linear, rotation, position, 0.5*step);//half step
   
  for(int time = 1; time < timesteps; time++)
  {
    //printf("TIMESTEP: %i\n", timesteps);
    //printf("BODY1 XVelocity:%f\n", linear[2][0]);
    //printf("BODY2 XVelocity:%f\n", linear[2][1]);
    contact::detection (0, nt, t, tid, pid, linear, conpnt);
		
    forces(conpnt, nb, position, angular, linear, mass, force, torque, gravity, parmat);
    
    dynamics::update(conpnt, nt, nb, t, pid, angular, linear, rotation, position, inertia, inverse, mass, force, torque, step, lo, hi);
   
    output::state(nt, t, time);
    if(time==270) break;
  }
	printf("\nComputation Finished.\n");

  for (int i = 0; i < 3; i ++)
  { free (t[0][i]);
    free (t[1][i]);
    free (t[2][i]);
    free (linear[i]);
    free (p[i]);
    free (q[i]);
  }

  return 0;
}

