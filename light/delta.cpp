#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "ext/tmr.h"
#include "io/input.h"
#include "io/output.h"
#include "contact.h"
#include "dynamics.h"
#include "forces/forces.h"
#include <iostream>
#include <vector>
#include <ctime>

void ui(int argc, char **argv, std::string &scenario, int &timesteps,
				iREAL &step, std::string  &collisionModel);

int main (int argc, char **argv)
{
	std::string scenario; int timesteps; iREAL step; std::string  collisionModel;

	ui(argc, argv, scenario, timesteps, step, collisionModel);

	logg::initiate();

  iREAL *angular[6]; // angular velocities (referential, spatial)
  iREAL *linear[3]; // linear velocities
  iREAL *rotation[9]; // rotation operators
  iREAL *position[6]; // mass center current and reference positions
  iREAL *inertia[9]; // inertia tensors
  iREAL *mass; // scalar mass
  iREAL *force[3]; // total spatial force
  iREAL *torque[3]; // total spatial torque
  iREAL *inverse[9]; // inverse inertia tensors
  
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
  
	int nb = 0;
  int nt = 0; // number of triangles
  int *pid; // particle identifier
  int *tid; // triangle identifiers
  
  iREAL lo[3] = {-500, -500, -500}; // lower corner
  iREAL hi[3] = {500, 500, 500}; // upper corner
  
  int size = 27000000; // memory buffer size
   
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
	
  input::init_enviroment(0, nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);

  logg::start(nt, nb);

  std::vector<contactpoint> *conpnt = new std::vector<contactpoint>[nb];
 
  //iREAL step = 1E-4; = critical (nt, mass, pairnum, iparam);
  
  dynamics::euler(nb, angular, linear, rotation, position, 0.5*step);//half step
   
  for(int time = 0; time < timesteps; time++)
  {
    contact::detection (0, nt, t, tid, pid, conpnt);
		
    forces::force(conpnt, nb, position, angular, linear, mass, force, torque, gravity, parmat);
    
    dynamics::update(conpnt, nt, nb, t, pid, angular, linear, rotation, position, inertia, inverse, mass, force, torque, step, lo, hi);

    logg::iteration(time);

    output::state(nt, t, time);
  }

  logg::end();

  for (int i = 0; i < 3; i ++)
  { free (t[0][i]);
    free (t[1][i]);
    free (t[2][i]);
    free (t[3][i]);
    free (t[4][i]);
    free (t[5][i]);
    free (linear[i]);
  }

  return 0;
}

void ui(int argc, char **argv, std::string &scenario, int &timesteps, iREAL &step, std::string  &collisionModel)
{
  printf("\nEXECUTING DELTA...\n\n");
  if(argc==1)
  {
  	printf("DELTA MASTER | PROVIDE ARGUEMENT: ./delta _#scenario_ "
  																									 "_#timesteps_ "
  																									 "_#step_ "
  																									 "_#method_\n"
  				"SCENARIOS\n"
  				"two-particle-crash\n"
  				"two-egg-crash\n"
  				"------------------------------\n"
  				"TIMESTEPS\n"
  				"100-100000\n"
  				"------------------------------\n"
  				"STEP\n"
  				"0.00001-1\n"
  				"------------------------------\n"
  				"METHOD\n"
  				"bf\n"
  				"penalty\n"
  				"hybrid\n"
  				"------------------------------\n");
    exit(0);
  }
  else
  {
  	scenario = argv[1];
  	timesteps = atoi(argv[2]);
  	step = atof(argv[3]);
  	collisionModel	= argv[4];
  }
}
