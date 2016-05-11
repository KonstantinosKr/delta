/*
 The MIT License (MIT)
 
 Copyright (c) 2016 Konstantinos Krestenitis
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <limits.h>
#include <float.h>
#include "tmr.h"
#include "material.h"
#include "input.h"
#include "output.h"
#include "log.h"
#include "migration.h"
#include "loba.h"
#include "contact.h"
#include "forces.h"
#include "dynamics.h"

void ui(int argc, char **argv, std::string &scenario, int &timesteps,
				iREAL &step, std::string  &collisionModel);

int main (int argc, char **argv)
{
  int nprocs, myrank;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
 
	std::string scenario; int timesteps=10000; iREAL step=1E-4; std::string  collisionModel;

  if(!myrank)
  {
  	//ui(argc, argv, scenario, timesteps, step, collisionModel);
    logg::initiate();
  }

  MPI_Barrier(MPI_COMM_WORLD);

  int *parmat; //particle material  
  
  material::ikind[0] = GRANULAR;
  material::ikind[1] = GRANULAR;
  material::ikind[2] = GRANULAR;
  
  //GRANULAR interaction type parameters 
  material::iparam[SPRING][GRANULAR] = 1E2;
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

  int nt = 0; // number of triangles
  int nb = 0; // number of bodies
  int *pid; //particle identifier
  int *tid; // triangle identifiers
  iREAL *t[6][3]; // triangles
  
  iREAL *mass; // scalar mass
  iREAL *force[3]; // total spatial force
  iREAL *torque[3]; // total spatial torque
  
  iREAL *position[6]; // mass center current and reference positions
  iREAL *angular[6]; // angular velocities (referential, spatial)
  iREAL *linear[3]; // linear velocities
  iREAL *rotation[9]; // rotation operators
  iREAL *inertia[9]; // inertia tensors
  iREAL *inverse[9]; // inverse inertia tensors 
  
  iREAL lo[3] = {-500, -500, -500}; // lower corner
  iREAL hi[3] = {500, 500, 500}; // upper corner
    
  iREAL gravity[3] = {0.0, 0.0, 0.0};
  
  int size = 100000; // memory buffer size
 
  for (int i = 0; i < 3; i ++)
  {
    t[0][i] = new iREAL[size];
    t[1][i] = new iREAL[size];
    t[2][i] = new iREAL[size];
    t[3][i] = new iREAL[size];
    t[4][i] = new iREAL[size];
    t[5][i] = new iREAL[size];
    
    linear[i] = new iREAL[size];
    torque[i] = new iREAL[size];
    force[i] = new iREAL[size];
  }
  
  for (int i = 0; i < 6; i++)
  {
    angular[i] = new iREAL[size]; 
    position[i] = new iREAL[size];
  }
  
  for (int i = 0; i<9; i++)
  {
    inverse[i] = new iREAL[size];
    inertia[i] = new iREAL[size]; 
    rotation[i] = new iREAL[size];
  }
  
  parmat = new int[size]; 
  tid = new int[size]; 
  pid = new int[size];
  mass = new iREAL[size]; 

  for(int i=0;i<size;i++) 
  {
    tid[i] = INT_MAX;
    force[0][i] = 0;
    force[1][i] = 0;
    force[2][i] = 0;
    
    torque[0][i] = 0;
    torque[1][i] = 0;
    torque[2][i] = 0;
  }
  
  int num_import, num_export, *import_procs, *import_to_part, *export_procs, *export_to_part;
  ZOLTAN_ID_PTR import_global_ids, import_local_ids, export_global_ids, export_local_ids;

  struct loba *lb = loba_create(ZOLTAN_RCB);
  
  TIMING tbalance;
  TIMING tmigration;
  TIMING tdataExchange;
  TIMING tdynamics;

  iREAL timer1, timer2, timer3;

  if (myrank == 0)
  {
    input::init_enviroment(atoi(scenario.c_str()), nt, nb, t, linear, angular,
    											inertia, inverse, rotation, mass,
													parmat, tid, pid, position, lo, hi);

    logg::start(nt, nb);
    dynamics::euler(nb, angular, linear, rotation, position, 0.5*step);
  }
  
  init_migratePosition (lb, nb, linear, angular, rotation, position, inertia, inverse, mass);
  
  std::vector<contactpoint> *conpnt = new std::vector<contactpoint>[nb]; 
 
  logg::slaveInitiate(myrank, nb);
        
  for(int time = 0; time < timesteps; time++)
  {
    timerstart(&tbalance);
    loba_balance (lb, nt, t[0], tid, 1.1,
                  &num_import, &import_procs, &import_to_part, 
                  &num_export, &export_procs, &export_to_part, 
                  &import_global_ids, &import_local_ids, 
                  &export_global_ids, &export_local_ids);
    timerend (&tbalance);
  
    timerstart(&tmigration);
    migrate (lb, nt, nb, t, parmat, tid, pid, 
               num_import, import_procs, import_to_part, 
               num_export, export_procs, export_to_part, 
               import_global_ids, import_local_ids, 
               export_global_ids, export_local_ids);
    timerend (&tmigration); 
    
    timer1 = 0.0;
    timer2 = 0.0;
    timer3 = 0.0;
    
    timerstart (&tdataExchange);
    migrateGhosts(lb, myrank, nt, nb, t, parmat, step, tid, pid, conpnt, &timer1, &timer2, &timer3);
    timerend (&tdataExchange);
    
    forces::force(lb, myrank, conpnt, nb, position, angular, linear, mass, force, torque, gravity, parmat);
    
    timerstart (&tdynamics);
    dynamics::update(lb, myrank, conpnt, nt, nb, t, pid, angular, linear, rotation, position, inertia, inverse, mass, force, torque, step, lo, hi);
    timerend (&tdynamics);

    output::state(lb, myrank, nt, t, time);
    
    if(!myrank){logg::iteration(timesteps);}
  }


  MPI_Barrier(MPI_COMM_WORLD); 
  if(!myrank)
  {
  	logg::end();
    output::postProcessing(nprocs, size, timesteps);
    logg::postprocessing();
  }

  loba_destroy (lb);

  for (int i = 0; i < 3; i ++)
  {
    free(t[0][i]);
    free(t[1][i]);
    free(t[2][i]);
    free(t[3][i]);
    free(t[4][i]);
    free(t[5][i]);
    free(linear[i]);
    free(torque[i]);
    free(force[i]);
  }
  
  Zoltan_LB_Free_Data (&import_global_ids, &import_local_ids, 
                      &import_procs, &export_global_ids, 
                      &export_local_ids, &export_procs);
  MPI_Finalize ();
  
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
