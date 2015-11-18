#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "input.h"
#include "output.h"
#include "loba.h"
#include "error.h"
#include "tribal_ispc.h"
#include "contact.h"
#include "motion.h"
#include "migration.h"
#include "tmr.h"

int main (int argc, char **argv)
{
  iREAL *t[3][3]; /* triangles */
  iREAL *v[3]; /* velocities */
  iREAL *distance; /*distance */
  iREAL *p[3],*q[3];//p and q points
  unsigned int nt = 0; /* number of triangles */
  unsigned int *pid; /*particle identifier */
  unsigned int *tid; /* triangle identifiers */
  iREAL lo[3] = {-500, -500, -500}; /* lower corner */
  iREAL hi[3] = {500, 500, 500}; /* upper corner */
  
  unsigned int size = 27000000; /* memory buffer size */
  int nprocs, myrank;

  /* init */ 
  MPI_Init (&argc, &argv);

  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   
  if (myrank == 0)
  {
    for (int i = 0; i < 3; i ++)
    { 
      t[0][i] = (iREAL *) malloc (size*sizeof(iREAL));
      t[1][i] = (iREAL *) malloc (size*sizeof(iREAL));
      t[2][i] = (iREAL *) malloc (size*sizeof(iREAL));
      v[i] = (iREAL *) malloc (size*sizeof(iREAL));
    
      p[i] = (iREAL *) malloc (size*sizeof(iREAL));
      q[i] = (iREAL *) malloc (size*sizeof(iREAL));
    }
    distance = (iREAL *) malloc (size*sizeof(iREAL));
    tid = (unsigned int *) malloc (size*sizeof(unsigned int));
    pid = (unsigned int *) malloc (size*sizeof(unsigned int));
    
    for(unsigned int i=0;i<size;i++) tid[i] = UINT_MAX; 
    
    unsigned int nparticles;
    init_enviroment(&nt, &nparticles, t, v, tid, pid, lo, hi);  
    printf("NT:%i\n", nt);
  }
  else
  {
    for (int i = 0; i < 3; i ++)
    {
      t[0][i] = (iREAL *) malloc (size*sizeof(iREAL));
      t[1][i] = (iREAL *) malloc (size*sizeof(iREAL));
      t[2][i] = (iREAL *) malloc (size*sizeof(iREAL));
      v[i] = (iREAL *) malloc (size*sizeof(iREAL));
      
      p[i] = (iREAL *) malloc (size*sizeof(iREAL));
      q[i] = (iREAL *) malloc (size*sizeof(iREAL));
    }
    distance = (iREAL*) malloc (size*sizeof(iREAL));
    tid = (unsigned int *) malloc (size*sizeof(unsigned int));
    pid = (unsigned int *) malloc (size*sizeof(unsigned int));
      
    for(unsigned int i=0;i<size;i++) tid[i] = UINT_MAX;
  }
  
  int num_import, num_export, *import_procs, *export_procs;
  ZOLTAN_ID_PTR import_global_ids, import_local_ids, export_global_ids, export_local_ids;
  
  /* create load balancer */
  struct loba *lb = loba_create (ZOLTAN_RCB);

  /* perform time stepping */
  iREAL step = 1E-3, time; unsigned int timesteps=0;
  
  TIMING tbalance[100];
  TIMING tmigration[100];
  TIMING tdataExchange[100];
  TIMING tintegration[100];
  iREAL tTimer1[100];
  iREAL tTimer2[100];
  iREAL tTimer3[100];
  iREAL tTimer4[100];

  iREAL timer1, timer2, timer3;
  timer1 = 0.0;
  timer2 = 0.0;
  timer3 = 0.0;
  
  //for (time = 0.0; time < 1.0; time += step)
  for(time = 0; time < 0.1; time+=step)
  //for(time = 0; time < 1; time++)
  {
    if(myrank == 0){printf("TIMESTEP: %i\n", timesteps);}
    
    timerstart(&tbalance[timesteps]);
    loba_balance (lb, nt, t[0], tid, 1.1,
                  &num_import, &import_procs, 
                  &num_export, &export_procs, 
                  &import_global_ids, &import_local_ids, 
                  &export_global_ids, &export_local_ids);
    timerend (&tbalance[timesteps]);
   
    printf("RANK[%i]: load balance:%f\n", myrank, tbalance[timesteps].total);
    
    timerstart(&tmigration[timesteps]);
    migrate_triangles (size, &nt, t, v, tid, pid, 
                        num_import, import_procs, 
                        num_export, export_procs, 
                        import_global_ids, import_local_ids, 
                        export_global_ids, export_local_ids);
    timerend (&tmigration[timesteps]);
    
    printf("RANK[%i]: migration:%f\n", myrank, tmigration[timesteps].total);
    
    timer1 = 0.0;
    timer2 = 0.0;
    timer3 = 0.0;
    
    timerstart (&tdataExchange[timesteps]);
    loba_migrateGhosts(lb, myrank, &nt, t, v, p, q, distance, tid, pid, &timer1, &timer2, &timer3);
    timerend (&tdataExchange[timesteps]);
   
    tTimer1[timesteps] = timer1;
    tTimer2[timesteps] = timer2;
    tTimer3[timesteps] = timer3;
 
    printf("RANK[%i]: data exchange:%f\n", myrank, tdataExchange[timesteps].total);
    
    timerstart (&tintegration[timesteps]);
    integrate (step, lo, hi, nt, t, v);
    timerend (&tintegration[timesteps]);
    
    printf("RANK[%i]: integration:%f\n", myrank, tintegration[timesteps].total);

//    output_state(lb, myrank, nt, t, v, timesteps);
    
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
    subtotal = subtotal + tbalance[i].total + tmigration[i].total + tdataExchange[i].total + tintegration[i].total;
    bal = bal + tbalance[i].total;
    mig = mig + tmigration[i].total;
    de = de + tdataExchange[i].total;
    in = in + tintegration[i].total;
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
   
    printf("TOTALmin: %f, TOTALmax: %f, TOTALavg: %f\nZ-BALmin: %f, Z-BALmax: %f, Z-BALavg: %f\nMIGRATIONmin: %f, MIGRATIONmax: %f, MIGRATIONavg: %f\nDTXmin: %f, DTXmax: %f, DTXavg: %f\nDT1min: %f, DT1max: %f, DT1avg: %f\nDT2min: %f, DT2max: %f, DT2avg: %f\nDT3min: %f, DT3max: %f, DT3avg: %f\n", minsubtotal, maxsubtotal, avgsubtotal, minbal, maxbal, avgbal, minmig, maxmig, avgmig, minde, maxde, avgde, mindt1, maxdt1, avgdt1, mindt2, maxdt2, avgdt2, mindt3, maxdt3, avgdt3); 
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if(myrank == 0)//have to make sure all ranks finished
  {
    printf("\nComputation Finished.\n");
    //postProcessing(nprocs, size, timesteps);
    //printf("Post-Processing Finished.\n");
  }

  /* finalise */
  loba_destroy (lb);

  for (int i = 0; i < 3; i ++)
  {
    free (t[0][i]);
    free (t[1][i]);
    free (t[2][i]);
    free (v[i]);
    free (p[i]);
    free (q[i]);
  }

  free (distance);
  
  Zoltan_LB_Free_Data (&import_global_ids, &import_local_ids, &import_procs, &export_global_ids, &export_local_ids, &export_procs);
  
  MPI_Finalize ();

  return 0;
}
