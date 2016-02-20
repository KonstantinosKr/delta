#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "tmr.h"
#include "input.h"
#include "output.h"
#include "dynamics.h"
#include "forces.h"
#include <vector>

using namespace std;

int main (int argc, char **argv)
{
  //particle material
  int *parmat; 
  iREAL *mparam[NMAT]; // material parameters 
  
  //number of material parameters
  for(int i = 0; i<NMAT; i++)
  {mparam[i] = (iREAL *) malloc(1*sizeof(iREAL));}

  int pairnum = 1;
  int *pairs; // color pairs 
  int *ikind; // interaction kind
  iREAL *iparam[NINT]; //interaction parameters per interaction type
  
  pairs = (int *) malloc(pairnum*sizeof(pairnum));
  ikind = (int *) malloc(1*sizeof(int)); //number of interaction kinds/types
  ikind[0] = GRANULAR; //set first kind

  for(int i=0;i<NINT;i++){iparam[i] = (iREAL *) malloc(1*sizeof(iREAL));}

  //GRANULAR interaction type parameters 
  iparam[SPRING][GRANULAR] = 1E4;
  iparam[DAMPER][GRANULAR] = 1E2;
  iparam[FRISTAT][GRANULAR] = 0;
  iparam[FRIDYN][GRANULAR] = 0;
  iparam[FRIROL][GRANULAR] = 0;
  iparam[FRIDRIL][GRANULAR] = 0;
  iparam[KSKN][GRANULAR] = 0;
  iparam[LAMBDA][GRANULAR] = 0;
  iparam[YOUNG2][GRANULAR] = 0;
  iparam[KSKN2][GRANULAR] = 0;
  iparam[SIGC][GRANULAR] = 0;
  iparam[TAUC][GRANULAR] = 0;
  iparam[ALPHA][GRANULAR] = 0;

  iREAL *linear[3]; // linear velocities
  iREAL *position[6]; // mass center current and reference positions
  iREAL *mass; // scalar mass 
  iREAL *force[3]; // total spatial force
 
  iREAL gravity[3];
  gravity[0] = 0;
  gravity[1] = 0;
  gravity[2] = 0;
  
  iREAL *t[3][3]; // triangles
  iREAL *p[3],*q[3]; //p and q points
  
	int nb;
  int nt = 0; //number of triangles
  int *pid; //particle identifier
  int *tid; // triangle identifiers
  
  iREAL lo[3] = {-500, -500, -500}; // lower corner
  iREAL hi[3] = {500, 500, 500}; // upper corner
  
  int size = 27000000; // memory buffer size
   
	for (int i = 0; i < 3; i ++)
	{ 
		t[0][i] = (iREAL *) malloc (size*sizeof(iREAL));
		t[1][i] = (iREAL *) malloc (size*sizeof(iREAL));
		t[2][i] = (iREAL *) malloc (size*sizeof(iREAL));
		
    linear[i] = (iREAL *) malloc (size*sizeof(iREAL));
    
    force[i] = (iREAL *) malloc (size*sizeof(iREAL));

		p[i] = (iREAL *) malloc (size*sizeof(iREAL));
		q[i] = (iREAL *) malloc (size*sizeof(iREAL));
    position[i] = (iREAL *) malloc (size*sizeof(iREAL)); 
  }

  vector<contact> *conpnt = (vector<contact> *) malloc(size*sizeof(vector<contact>));

  parmat = (int *) malloc (size*sizeof(int));
	
	tid = (int *) malloc (size*sizeof(int));
	pid = (int *) malloc (size*sizeof(int));

	mass = (iREAL *) malloc(size*sizeof(iREAL));

	for(int i=0;i<size;i++) tid[i] = UINT_MAX; 
	
	init_env(nt, nb, t, linear, mass, parmat, tid, pid, position, lo, hi);  
	printf("NT:%i NB:%i\n", nt, nb);
 
  iREAL step = 1E-4, time; int timesteps=0; 
  
  for(time = 1E-4; time < 1; time+=step)
  {
    printf("TIMESTEP: %i\n", timesteps); 
   
    printf("BODY1 XVelocity:%f\n", linear[2][0]);
    printf("BODY2 XVelocity:%f\n", linear[2][1]);
    contact_detection (0, nt, t, tid, pid, linear, p, q, conpnt);
    
    forces(conpnt, nb, linear, mass, parmat, mparam, pairnum, pairs, ikind, iparam);
    
    printf("Forces derivation finished\n");

    dynamics(conpnt, nt, nb, t, pid, linear, position, mass, force, gravity, step);
   
    printf("Updated dynamics\n");

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
