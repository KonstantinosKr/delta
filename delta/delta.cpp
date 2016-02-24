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

int ui(int myrank, int argc, char **argv);

int main (int argc, char **argv)
{
  int nprocs, myrank;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
 
  //user interface
  if(ui(myrank, argc, argv)) return 0;

  int *parmat; //particle material  

  int nt = 0; // number of triangles
  int nb = 0;
  int *pid; //particle identifier
  int *tid; // triangle identifiers
  iREAL *t[3][3]; // triangles
  iREAL *p[3],*q[3];//p and q points
  
  material material();
  iREAL *mass; // scalar mass
  iREAL *force[3]; // total spatial force
  iREAL *torque[3]; // total spatial torque
  
  iREAL *angular[6]; // angular velocities (referential, spatial)
  iREAL *linear[3]; // linear velocities
  iREAL *rotation[9]; // rotation operators
  iREAL *position[6]; // mass center current and reference positions
  iREAL *inertia[9]; // inertia tensors
  iREAL *inverse[9]; // inverse inertia tensors 
  
  iREAL lo[3] = {-500, -500, -500}; // lower corner
  iREAL hi[3] = {500, 500, 500}; // upper corner
    
  iREAL gravity[3] = {0.0, 100.0, 0.0};
  
  int size = 2700000; // memory buffer size
 
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

  std::vector<contact> *conpnt = (std::vector<contact> *) malloc(size*sizeof(std::vector<contact>));
  
  int num_import, num_export, *import_procs, *import_to_part, *export_procs, *export_to_part;
  ZOLTAN_ID_PTR import_global_ids, import_local_ids, export_global_ids, export_local_ids;

  // create load balancer 
  struct loba *lb = loba_create (ZOLTAN_RCB);
  
  TIMING tbalance[1000];
  TIMING tmigration[1000];
  TIMING tdataExchange[1000];
  TIMING tdynamics[1000];
  iREAL tTimer1[1000];
  iREAL tTimer2[1000];
  iREAL tTimer3[1000];
  //iREAL tTimer4[1000];

  iREAL timer1, timer2, timer3;
  timer1 = 0.0;
  timer2 = 0.0;
  timer3 = 0.0;

  if (myrank == 0)
  {
    init_enviroment(nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);
    printf("NT:%i, NB: %i\n", nt, nb);
    //euler(nb, angular, linear, rotation, position, 0.5*step);
  }
  
  iREAL step = 1E-3; int timesteps=0;

  for(iREAL time = step; time < 0.1; time+=step)
  {
    if(!myrank){printf("TIMESTEP: %i\n", timesteps);} 
    
    timerstart(&tbalance[timesteps]);
    loba_balance (lb, nt, t[0], tid, 1.1,
                  &num_import, &import_procs, &import_to_part, 
                  &num_export, &export_procs, &export_to_part, 
                  &import_global_ids, &import_local_ids, 
                  &export_global_ids, &export_local_ids);
    timerend (&tbalance[timesteps]);
  
    printf("RANK[%i]: load balance:%f\n", myrank, tbalance[timesteps].total);
   
    timerstart(&tmigration[timesteps]);
    migrate (nt, t, linear, angular, parmat, tid, pid, 
               num_import, import_procs, import_to_part, 
               num_export, export_procs, export_to_part, 
               import_global_ids, import_local_ids, 
               export_global_ids, export_local_ids);
    timerend (&tmigration[timesteps]);
   
    printf("RANK[%i]: migration:%f\n", myrank, tmigration[timesteps].total);
    
    timer1 = 0.0;
    timer2 = 0.0;
    timer3 = 0.0;
    
    timerstart (&tdataExchange[timesteps]);
    migrateGhosts(lb, myrank, nt, t, linear, angular, parmat, step, p, q, tid, pid, conpnt, &timer1, &timer2, &timer3);
    timerend (&tdataExchange[timesteps]);
    
    tTimer1[timesteps] = timer1;
    tTimer2[timesteps] = timer2;
    tTimer3[timesteps] = timer3;
    printf("RANK[%i]: data exchange:%f\n", myrank, tdataExchange[timesteps].total);
   
    //forces(conpnt, nb, position, angular, linear, mass, force, torque, gravity, parmat, mparam, pairnum, pairs, ikind, iparam);
    printf("RANK[%i]: contact forces: %f\n", myrank, 0.0);

    timerstart (&tdynamics[timesteps]);
    //dynamics(conpnt, nt, nb, t, pid, angular, linear, rotation, position, inertia, inverse, mass, force, torque, step);
    timerend (&tdynamics[timesteps]);
    printf("RANK[%i]: dynamics:%f\n", myrank, tdynamics[timesteps].total);
    
    output_state(lb, myrank, nt, t, timesteps);
    timesteps++;
  }

  iREAL subtotal = 0;
  iREAL bal = 0;
  iREAL mig = 0;
  iREAL de = 0;
  iREAL in = 0;
  iREAL dt1 = 0;
  iREAL dt2 = 0;
  iREAL dt3 = 0;
  
  for(int i = 0; i<timesteps;i++)
  {
    subtotal = subtotal + tbalance[i].total + tmigration[i].total + tdataExchange[i].total + tdynamics[i].total;
    bal = bal + tbalance[i].total;
    mig = mig + tmigration[i].total;
    de = de + tdataExchange[i].total;
    in = in + tdynamics[i].total;
    dt1 = dt1 + tTimer1[i];
    dt2 = dt2 + tTimer2[i];
    dt3 = dt3 + tTimer3[i];
  }  
  
  if(myrank != 0)
  {
    MPI_Send(&subtotal, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&bal, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&mig, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&de, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&dt1, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&dt2, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&dt3, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  else if (myrank == 0)
  {
    iREAL *rvsubtotal = (iREAL *) malloc(nprocs*sizeof(iREAL));
    iREAL *rvbal = (iREAL *) malloc(nprocs*sizeof(iREAL));
    iREAL *rvmig = (iREAL *) malloc(nprocs*sizeof(iREAL));
    iREAL *rvde = (iREAL *) malloc(nprocs*sizeof(iREAL));
    iREAL *rvdt1 = (iREAL *) malloc(nprocs*sizeof(iREAL));
    iREAL *rvdt2 = (iREAL *) malloc(nprocs*sizeof(iREAL));
    iREAL *rvdt3 = (iREAL *) malloc(nprocs*sizeof(iREAL));
    
    iREAL minsubtotal = DBL_MAX;
    iREAL minbal = DBL_MAX;
    iREAL minmig = DBL_MAX;
    iREAL minde = DBL_MAX;
    iREAL mindt1 = DBL_MAX;
    iREAL mindt2 = DBL_MAX;
    iREAL mindt3 = DBL_MAX;
    
    iREAL avgsubtotal = 0;
    iREAL avgbal = 0;
    iREAL avgmig = 0;
    iREAL avgde = 0;
    iREAL avgdt1 = 0;
    iREAL avgdt2 = 0;
    iREAL avgdt3 = 0;
    
    iREAL maxsubtotal = DBL_MIN;
    iREAL maxbal = DBL_MIN;
    iREAL maxmig = DBL_MIN;
    iREAL maxde = DBL_MIN;
    iREAL maxdt1 = DBL_MIN;
    iREAL maxdt2 = DBL_MIN;
    iREAL maxdt3 = DBL_MIN;

    for(int i = 1; i < nprocs;i++)
    {
      MPI_Recv(&rvsubtotal[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&rvbal[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&rvmig[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&rvde[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&rvdt1[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&rvdt2[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&rvdt3[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    }
    
    rvsubtotal[0] = subtotal;
    rvbal[0] = bal;
    rvmig[0] = mig;
    rvde[0] = de;
    rvdt1[0] = dt1;
    rvdt2[0] = dt2;
    rvdt3[0] = dt3;

    for(int i = 0; i < nprocs;i++)
    {
      if(rvsubtotal[i] < minsubtotal) minsubtotal = rvsubtotal[i];
      if(rvsubtotal[i] > maxsubtotal) maxsubtotal = rvsubtotal[i];
      avgsubtotal += rvsubtotal[i];

      if(rvbal[i] < minbal) minbal = rvbal[i];
      if(rvbal[i] > maxbal) maxbal = rvbal[i];
      avgbal += rvbal[i];

      if(rvmig[i] < minmig) minmig = rvmig[i];
      if(rvmig[i] > maxmig) maxmig = rvmig[i];
      avgmig += rvmig[i];
      
      if(rvde[i] < minde) minde = rvde[i];
      if(rvde[i] > maxde) maxde = rvde[i];
      avgde += rvde[i];

      if(rvdt1[i] < mindt1) mindt1 = rvdt1[i];
      if(rvdt1[i] > maxdt1) maxdt1 = rvdt1[i];
      avgdt1 += rvdt1[i];
      
      if(rvdt2[i] < mindt2) mindt2 = rvdt2[i];
      if(rvdt2[i] > maxdt2) maxdt2 = rvdt2[i];
      avgdt2 += rvdt2[i];

      if(rvdt3[i] < mindt3) mindt3 = rvdt3[i];
      if(rvdt3[i] > maxdt3) maxdt3 = rvdt3[i];
      avgdt3 += rvdt3[i];
    }

    avgsubtotal = avgsubtotal/nprocs;
    avgbal = avgbal/nprocs;
    avgmig = avgmig/nprocs;
    avgde = avgde/nprocs;
    avgdt1 = avgdt1/nprocs;
    avgdt2 = avgdt2/nprocs;
    avgdt3 = avgdt3/nprocs; 
   
    printf("TOTmin: %f, TOTmax: %f, TOTavg: %f\n"
           "BALmin: %f, BALmax: %f, BALavg: %f\n"
           "MIGmin: %f, MIGmax: %f, MIGavg: %f\n"
           "DTXmin: %f, DTXmax: %f, DTXavg: %f\n"
           "DT1min: %f, DT1max: %f, DT1avg: %f\n"
           "DT2min: %f, DT2max: %f, DT2avg: %f\n"
           "DT3min: %f, DT3max: %f, DT3avg: %f\n", 
          minsubtotal, maxsubtotal, avgsubtotal, 
          minbal, maxbal, avgbal, 
          minmig, maxmig, avgmig, 
          minde, maxde, avgde, 
          mindt1, maxdt1, avgdt1, 
          mindt2, maxdt2, avgdt2, 
          mindt3, maxdt3, avgdt3); 

    performance_log(minsubtotal, maxsubtotal, avgsubtotal, 
                    minbal, maxbal, avgbal, 
                    minmig, maxmig, avgmig, 
                    minde, maxde, avgde, 
                    mindt1, maxdt1, avgdt1, 
                    mindt2, maxdt2, avgdt2, 
                    mindt3, maxdt3, avgdt3); 
    printf("Log Writting Finished.\n");
  }
  //have to make sure all ranks finished
  MPI_Barrier(MPI_COMM_WORLD);
  if(!myrank)
  {
    printf("\nComputation Finished.\n");
    postProcessing(nprocs, size, timesteps);
    printf("Post-Processing Finished.\n");

  }

  // DESTROY
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
    free(p[i]);
    free(q[i]);
  }
  
  Zoltan_LB_Free_Data (&import_global_ids, &import_local_ids, 
                      &import_procs, &export_global_ids, 
                      &export_local_ids, &export_procs);
  MPI_Finalize ();
  
  return 0;
}

int ui(int myrank, int argc, char **argv)
{
  int execid = 0;
  if(!myrank)
  {
    printf("\nExecuting Project Delta\n");
    if(argc==1) 
      printf("Delta: initiating default setup.\n");
    else
      for(int i=1;i<argc;i++)
      {
        execid = 0;
        printf("Arg[%i]: %s\n", i, argv[i]);
      }
  }else
  {
    execid = 0;    
  }
  return execid;
}
