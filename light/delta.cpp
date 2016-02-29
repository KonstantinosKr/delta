#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "tmr.h"
#include "input.h"
#include "output.h"
#include "contact.h"
#include "dynamics.h"
#include "forces.h"

int main (int argc, char **argv)
{

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
		t[0][i] = (iREAL *) malloc (size*sizeof(iREAL));
		t[1][i] = (iREAL *) malloc (size*sizeof(iREAL));
		t[2][i] = (iREAL *) malloc (size*sizeof(iREAL));
		t[3][i] = (iREAL *) malloc (size*sizeof(iREAL));
		t[4][i] = (iREAL *) malloc (size*sizeof(iREAL));
		t[5][i] = (iREAL *) malloc (size*sizeof(iREAL));
		
    linear[i] = (iREAL *) malloc (size*sizeof(iREAL));
    
    torque[i] = (iREAL *) malloc (size*sizeof(iREAL));
    force[i] = (iREAL *) malloc (size*sizeof(iREAL));

		p[i] = (iREAL *) malloc (size*sizeof(iREAL));
		q[i] = (iREAL *) malloc (size*sizeof(iREAL));
  }
	
	for (int i = 0; i < 6; i++)
	{
		angular[i] = (iREAL *) malloc (size*sizeof(iREAL));
    position[i] = (iREAL *) malloc (size*sizeof(iREAL)); 
	}
 
  for (int i = 0; i<9; i++)
  {
    inverse[i] = (iREAL *) malloc (size*sizeof(iREAL));
    inertia[i] = (iREAL *) malloc (size*sizeof(iREAL));
    rotation[i] = (iREAL *) malloc (size*sizeof(iREAL));
  }
	
  parmat = (int *) malloc (size*sizeof(int));
	
	tid = (int *) malloc (size*sizeof(int));
	pid = (int *) malloc (size*sizeof(int));

	mass = (iREAL *) malloc(size*sizeof(iREAL));

	for(int i=0;i<size;i++) tid[i] = INT_MAX; 
	
	init_enviroment(nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);  
	printf("NT:%i NB:%i\n", nt, nb);
  
  std::vector<contact> *conpnt = new std::vector<contact>[nb];
 
  /* perform time stepping */
  iREAL step = 1E-4, time; int timesteps=1; 
  
  //step = critical (nt, mass, pairnum, iparam);
  
  euler(nb, angular, linear, rotation, position, 0.5*step);//half step
   
  for(time = 1E-4; time < 1; time+=step)
  {
    printf("TIMESTEP: %i\n", timesteps); 
   
    printf("BODY1 XVelocity:%f\n", linear[2][0]);
    printf("BODY2 XVelocity:%f\n", linear[2][1]);
    contact_detection (0, nt, 0, nt, t, tid, pid, linear, p, q, conpnt);
		
    forces(conpnt, nb, position, angular, linear, mass, force, torque, gravity, parmat);
    
    dynamics(conpnt, nt, nb, t, pid, angular, linear, rotation, position, inertia, inverse, mass, force, torque, step);
   
    output_state(nt, t, timesteps);
    
    timesteps++;
  }
	printf("\nComputation Finished.\n");

  for (int i = 0; i < 3; i ++)
  {
    free (t[0][i]);
    free (t[1][i]);
    free (t[2][i]);
    free (linear[i]);
    free (p[i]);
    free (q[i]);
  }

  return 0;
}
