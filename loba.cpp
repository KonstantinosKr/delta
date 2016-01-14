#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <float.h>
#include "error.h"
#include "loba.h"
#include "tmr.h"

struct zoltan_args
{
  unsigned int n;
  iREAL *p[3];
  unsigned int *id;
};

/* number of objects for balacing */
static int obj_count (struct zoltan_args *args, int *ierr)
{
  *ierr = ZOLTAN_OK;

#if 0
  return args->n > 0 ? args->n : 1;
#else
  return args->n;
#endif
}

/* list of object identifiers for load balancing */
static void obj_list (struct zoltan_args *args, int num_gid_entries, int num_lid_entries,
  ZOLTAN_ID_PTR global_ids, ZOLTAN_ID_PTR local_ids, int wgt_dim, float *obj_wgts, int *ierr)
{
  unsigned int i;
  
  for (i = 0; i < args->n; i ++)
  {
    global_ids [i * num_gid_entries] = args->id[i];
    local_ids [i * num_lid_entries] = i;
    obj_wgts [i * wgt_dim] = 1.0;
  }

#if 0
  if (i == 0) /* XXX: Zoltan workaround */
  {
    global_ids [0] = UINT_MAX;
    obj_wgts [0] = 1.0;
  }
#endif

  *ierr = ZOLTAN_OK;
}

/* number of spatial dimensions */
static int dimensions (struct zoltan_args *args, int *ierr)
{
  *ierr = ZOLTAN_OK;
  return 3;
}

/* list of object points exploited during load balancing */
static void obj_points (struct zoltan_args *args, int num_gid_entries, int num_lid_entries, int num_obj,
  ZOLTAN_ID_PTR global_ids, ZOLTAN_ID_PTR local_ids, int num_dim, double *geom_vec, int *ierr)
{
  double *v;
  int i, j;

#if 0
  if (num_obj == 1 && global_ids [0] == UINT_MAX) /* XXX: Zoltan workaround */
  {
    geom_vec[0] = 0;
    geom_vec[1] = 0;
    geom_vec[2] = 0;
  }
  else
#endif
  for (i = 0; i < num_obj; i ++)
  {
    j = local_ids [i * num_lid_entries];
    v = &geom_vec [i * num_dim];

    v[0] = args->p[0][j];
    v[1] = args->p[1][j];
    v[2] = args->p[2][j];
  }

  *ierr = ZOLTAN_OK;
}

/* create load balancer */
struct loba* loba_create (enum algo al)
{
  struct loba *lb;

  ERRMEM (lb = (struct loba*) malloc (sizeof (struct loba)));

  switch (al)
  {
  case ZOLTAN_RCB:
  {
    /* create Zoltan object */
    ASSERT (lb->zoltan = Zoltan_Create (MPI_COMM_WORLD), "Zoltan initialisation failed");

    /* set general parameters */
    Zoltan_Set_Param (lb->zoltan, "DEBUG_LEVEL", "0");
    Zoltan_Set_Param (lb->zoltan, "DEBUG_MEMORY", "0");
    Zoltan_Set_Param (lb->zoltan, "NUM_GID_ENTRIES", "1");
    Zoltan_Set_Param (lb->zoltan, "NUM_LID_ENTRIES", "1");
    Zoltan_Set_Param (lb->zoltan, "OBJ_WEIGHT_DIM", "1");
    //parts
    //Zoltan_Set_Param (lb->zoltan, "NUM_LOCAL_PARTS", "4");

    /* load balancing parameters */
    Zoltan_Set_Param (lb->zoltan, "LB_METHOD", "RCB");
    Zoltan_Set_Param (lb->zoltan, "IMBALANCE_TOL", "1.3");
    Zoltan_Set_Param (lb->zoltan, "AUTO_MIGRATE", "FALSE");
    Zoltan_Set_Param (lb->zoltan, "RETURN_LISTS", "IMPORT AND EXPORT");
    //Zoltan_Set_Param (lb->zoltan, "RETURN_LISTS", "EXPORT");
    
    /* RCB parameters */
    Zoltan_Set_Param (lb->zoltan, "RCB_OVERALLOC", "1.3");
    Zoltan_Set_Param (lb->zoltan, "RCB_REUSE", "TRUE");
    Zoltan_Set_Param (lb->zoltan, "RCB_OUTPUT_LEVEL", "0");
    Zoltan_Set_Param (lb->zoltan, "CHECK_GEOM", "1");
    Zoltan_Set_Param (lb->zoltan, "KEEP_CUTS", "TRUE");
    Zoltan_Set_Param (lb->zoltan, "REDUCE_DIMENSIONS", "0");
  }
  break;
  case ZOLTAN_RIB:
  {
    /* TODO */
  }
break;
  }

  lb->al = al;

  return lb;
}

/* balance points up to tolerance; output migration ranks */
void loba_balance (struct loba *lb, unsigned int n, iREAL *p[3], unsigned int *id, iREAL tol,
                    int *num_import, int **import_procs, int **import_to_part, 
		                int *num_export, int **export_procs, int **export_to_part, 
                    ZOLTAN_ID_PTR *import_global_ids, ZOLTAN_ID_PTR *import_local_ids, 
                    ZOLTAN_ID_PTR *export_global_ids, ZOLTAN_ID_PTR *export_local_ids) 
{
  switch (lb->al)
  {
    case ZOLTAN_RCB:
    {
      struct zoltan_args args = {n, {p[0], p[1], p[2]}, id};
      
      /* callbacks */
      Zoltan_Set_Fn (lb->zoltan, ZOLTAN_NUM_OBJ_FN_TYPE, (void (*)()) obj_count, &args);
      Zoltan_Set_Fn (lb->zoltan, ZOLTAN_OBJ_LIST_FN_TYPE, (void (*)()) obj_list, &args);
      Zoltan_Set_Fn (lb->zoltan, ZOLTAN_NUM_GEOM_FN_TYPE, (void (*)()) dimensions, &args);
      Zoltan_Set_Fn (lb->zoltan, ZOLTAN_GEOM_MULTI_FN_TYPE, (void (*)()) obj_points, &args);

      /* update imbalance */
      char str[128];
      snprintf (str, 128, "%g", tol);
      Zoltan_Set_Param (lb->zoltan, "IMBALANCE_TOL", str);
      
      int changes, num_gid_entries, num_lid_entries; /* TODO: do we need this outside? */
      
      /* update partitioning */
      ASSERT (Zoltan_LB_Balance (lb->zoltan, &changes, &num_gid_entries, &num_lid_entries,
        num_import, import_global_ids, import_local_ids, import_procs,
        num_export, export_global_ids, export_local_ids, export_procs) == ZOLTAN_OK, "Zoltan load balancing failed");
      
      /*ASSERT (Zoltan_LB_Partition (lb->zoltan, &changes, &num_gid_entries, &num_lid_entries,
        num_import, import_global_ids, import_local_ids, import_procs, import_to_part,
        num_export, export_global_ids, export_local_ids, export_procs, export_to_part) == ZOLTAN_OK, "Zoltan load balancing failed");*/
    }
    break;
    case ZOLTAN_RIB:
    {
    
    }
  }
}

/* find ranks overlapped by the [lo,hi] box */
void loba_query (struct loba *lb, int node, iREAL lo[3], iREAL hi[3], int *ranks, int *nranks)
{
  switch (lb->al)
  {
    case ZOLTAN_RCB:
    {
      Zoltan_LB_Box_Assign (lb->zoltan, lo[0], lo[1], lo[2], hi[0], hi[1], hi[2], ranks, nranks);
      break;
    }
    case ZOLTAN_RIB:
    {
    }
  }
}

void loba_getAdjacent(struct loba *lb, int myrank, int *neighborhood, int *nNeighbors)
{
  iREAL mylo[3], myhi[3], lo[3], hi[3];

  loba_getbox(lb, myrank, mylo, myhi); 
  
  iREAL mypoint[8][3];
  iREAL point[8][3];

  mypoint[0][0] = mylo[0];
  mypoint[0][1] = mylo[1];
  mypoint[0][2] = mylo[2];

  mypoint[1][0] = mylo[0];
  mypoint[1][1] = myhi[1];
  mypoint[1][2] = mylo[2];

  mypoint[2][0] = mylo[0];
  mypoint[2][1] = myhi[1];
  mypoint[2][2] = myhi[2];

  mypoint[3][0] = mylo[0];
  mypoint[3][1] = mylo[1];
  mypoint[3][2] = myhi[2]; 

  mypoint[4][0] = myhi[0];
  mypoint[4][1] = myhi[1];
  mypoint[4][2] = myhi[2];

  mypoint[5][0] = myhi[0];
  mypoint[5][1] = mylo[1];
  mypoint[5][2] = myhi[2];

  mypoint[6][0] = myhi[0];
  mypoint[6][1] = mylo[1];
  mypoint[6][2] = mylo[2];

  mypoint[7][0] = myhi[0];
  mypoint[7][1] = myhi[1];
  mypoint[7][2] = mylo[2]; 
  
  int counter = 0;
  int nranks;
  MPI_Comm_size(MPI_COMM_WORLD, &nranks);
  for(int i=0; i<nranks; i++)
  {
    if(i == myrank) continue;
    loba_getbox(lb, i, lo, hi); 
    
    point[0][0] = lo[0];
    point[0][1] = lo[1];
    point[0][2] = lo[2];

    point[1][0] = lo[0];
    point[1][1] = hi[1];
    point[1][2] = lo[2];

    point[2][0] = lo[0];
    point[2][1] = hi[1];
    point[2][2] = hi[2];

    point[3][0] = lo[0];
    point[3][1] = lo[1];
    point[3][2] = hi[2]; 

    point[4][0] = hi[0];
    point[4][1] = hi[1];
    point[4][2] = hi[2];

    point[5][0] = hi[0];
    point[5][1] = lo[1];
    point[5][2] = hi[2];

    point[6][0] = hi[0];
    point[6][1] = lo[1];
    point[6][2] = lo[2];

    point[7][0] = hi[0];
    point[7][1] = hi[1];
    point[7][2] = lo[2]; 
    
    int isNeighbor = 0;
    for(int j=0; j<8; j++)
    {
      for(int jj=0; j<8; j++)
      {
        for(int z=0; z<3; z++)
        {
          for(int zz=0; zz<3; zz++)
          {
            if(point[j][z] == mypoint[jj][zz])
            {
              isNeighbor = 1;
            }
            if(mypoint[j][z] == point[jj][zz])
            {
              isNeighbor = 1;
            }
          }
        }
      }
    }
    if(isNeighbor == 1)
    {
      neighborhood[counter++] = i;
      isNeighbor = 0;
    }
  }
  *nNeighbors = counter;
}
 
void loba_getbox (struct loba *lb, int part, iREAL lo[3], iREAL hi[3])
{
  switch (lb->al)
  {
    case ZOLTAN_RCB:
    {  
      int ndim;
      Zoltan_RCB_Box(lb->zoltan, part, &ndim, &lo[0], &lo[1], &lo[2], &hi[0], &hi[1], &hi[2]);  
  
      for(int j = 0; j < 3; j++)
      {
        if(lo[j] < -FLT_MAX)
        {
          lo[j] = -250;
        } 
        
        if(hi[j] > FLT_MAX)
        {
          hi[j] = 250;
        } 
      }
      
      break;
    }
    case ZOLTAN_RIB:
    {
    }
  }
}

void loba_migrateGhosts(struct loba *lb, int  myrank, unsigned int *nt, iREAL *t[3][3], 
                      iREAL *v[3], iREAL *angular[6], int *parmat,
                      iREAL dt, iREAL *p[3], iREAL *q[3], 
                      unsigned int *tid, unsigned int *pid, 
                      master_conpnt *con, unsigned int long long *ncontacts, 
                      iREAL *timer1, iREAL *timer2, iREAL *timer3)
{
  TIMING t1, t2, t3;
  
  timerstart(&t1);
  int nproc=0;
  int nNeighbors=0;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  int *neighborhood = (int *) malloc(nproc * sizeof(int));
  loba_getAdjacent(lb, myrank, neighborhood, &nNeighbors);
  
  unsigned int *ghostTID = (unsigned int*) malloc(*nt*sizeof(unsigned int));
  unsigned int *ghostPID = (unsigned int*) malloc(*nt*sizeof(unsigned int));
  unsigned int **ghostTIDNeighbors = (unsigned int**) malloc(*nt*sizeof(unsigned int*));
  unsigned int *ghostTIDcrosses = (unsigned int*) malloc(*nt*sizeof(unsigned int));
  int *ghostNeighborhood = (int*) malloc(nproc*sizeof(int));
  
  for(int i = 0;i<nproc;i++){ghostNeighborhood[i] = -1;}
  for(unsigned int i = 0;i<*nt;i++){ghostTIDNeighbors[i] = (unsigned int*) malloc(nNeighbors*sizeof(unsigned int));}
  unsigned int nGhosts, nGhostNeighbors;
  
  //get triangle tids that overlap into neighbors
  loba_getGhosts(lb, myrank, nNeighbors, *nt, t, tid, pid, 
                      ghostTID, ghostPID, &nGhosts, 
                      &nGhostNeighbors, ghostNeighborhood, 
                      ghostTIDNeighbors, ghostTIDcrosses);
  printf("RANK[%i]: overlaps:%i\n", myrank, nGhosts);
  
  //allocate memory for buffers
  unsigned int *pivot, *tid_buffer, *pid_buffer, *rcvpivot, *rcvtid_buffer, *rcvpid_buffer;
  iREAL *tbuffer[3], *vbuffer, *angbuffer;
  iREAL *trvbuffer[3], *vrvbuffer, *angrvbuffer;
  int *parmat_buffer, *rvparmat_buffer;

  tbuffer[0] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[1] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[2] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  vbuffer = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  angbuffer = (iREAL *) malloc(nNeighbors*nGhosts*6*sizeof(iREAL));//six elements thus x2.
  
  tid_buffer = (unsigned int *) malloc(nNeighbors*nGhosts*sizeof(unsigned int));
  pid_buffer = (unsigned int *) malloc(nNeighbors*nGhosts*sizeof(unsigned int));
  parmat_buffer = (int *) malloc(nNeighbors*nGhosts*sizeof(int));

  pivot = (unsigned int *) malloc(nNeighbors*sizeof(unsigned int));
  rcvpivot = (unsigned int *) malloc(nNeighbors*sizeof(unsigned int));
  
  for(int i = 0; i < nNeighbors; i++)
  { //set send indices and pivots for buffers
    pivot[i] = 0; //set pivot to zero
    rcvpivot[i] = 0;  
  }
  
  //prepare export buffers
  for (unsigned int i = 0; i<nGhosts; i++)
  {
    for(unsigned int ii = 0; ii<ghostTIDcrosses[i]; ii++)
    {
      int oltid = ghostTID[i];
      int olpid = ghostPID[i];
      int j = ghostTIDNeighbors[i][ii];
      int jj = ghostNeighborhood[j];
      //printf("RANK[%i]: J:%i JJ:%i\n", myrank, j, jj);
      int xx = pivot[jj];
      
      parmat_buffer[(jj*nGhosts)+xx] = parmat[oltid];
      tid_buffer[(jj*nGhosts)+xx] = oltid;
      pid_buffer[(jj*nGhosts)+xx] = olpid;
      
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+0] = t[0][0][oltid]; //point 0
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+1] = t[0][1][oltid]; //point 0
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+2] = t[0][2][oltid]; //point 0
      
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+0] = t[1][0][oltid]; //point 1
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+1] = t[1][1][oltid]; //point 1
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+2] = t[1][2][oltid]; //point 1
      
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+0] = t[2][0][oltid]; //point 2
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+1] = t[2][1][oltid]; //point 2
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+2] = t[2][2][oltid]; //point 2
      
      vbuffer[(jj*nGhosts*3)+(xx*3)+0] = v[0][oltid];
      vbuffer[(jj*nGhosts*3)+(xx*3)+1] = v[1][oltid];
      vbuffer[(jj*nGhosts*3)+(xx*3)+2] = v[2][oltid];

      angbuffer[(jj*nGhosts*6)+(xx*6)+0] = angular[0][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+1] = angular[1][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+2] = angular[2][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+3] = angular[3][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+4] = angular[4][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+5] = angular[5][oltid];
      
      pivot[jj]++;
    }
  }
  
  //blocking communication
  for(int i=0; i<nNeighbors; i++)
  {
    int proc = neighborhood[i];
    MPI_Send(&pivot[i], 1, MPI_INT, proc, 1, MPI_COMM_WORLD);
  }
 
  unsigned int max = 0;
  for(int i=0; i<nNeighbors; i++)
  {
    int proc = neighborhood[i];
    MPI_Recv(&rcvpivot[i], 1, MPI_INT, proc, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if(rcvpivot[i] > max) max = rcvpivot[i];
  }
  
  int n = max;
  trvbuffer[0] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  trvbuffer[1] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  trvbuffer[2] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  vrvbuffer = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  angrvbuffer = (iREAL *) malloc(nNeighbors*n*6*sizeof(iREAL));//six elements thus x2.
  
  rvparmat_buffer = (int *) malloc(nNeighbors*n*sizeof(int));
  rcvtid_buffer = (unsigned int *) malloc(nNeighbors*n*sizeof(unsigned int));
  rcvpid_buffer = (unsigned int *) malloc(nNeighbors*n*sizeof(unsigned int));
 
  int MPISENDS = 8;
  MPI_Request *myRequest = (MPI_Request*) malloc(nNeighbors*MPISENDS*sizeof(MPI_Request));//6 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(nNeighbors*MPISENDS*sizeof(MPI_Request));//6 sends
  
  for(int i=0; i<nNeighbors; i++)
  {
    int proc = neighborhood[i];
    if(rcvpivot[i] > 0)
    {//safe check
      MPI_Irecv(&rcvtid_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+0]);
      MPI_Irecv(&trvbuffer[0][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
      MPI_Irecv(&trvbuffer[1][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
      MPI_Irecv(&trvbuffer[2][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
      
      MPI_Irecv(&vrvbuffer[(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+4]);
      MPI_Irecv(&angrvbuffer[(i*n*6)], rcvpivot[i]*6, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+5]);

      MPI_Irecv(&rcvpid_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+6]);
      MPI_Irecv(&rvparmat_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+7]); 
    }
  }

  for(int i=0;i<nNeighbors;i++)
  {
    int proc = neighborhood[i];
    if(pivot[i] > 0)
    {//safe check
      MPI_Isend(&tid_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+0]);
      MPI_Isend(&tbuffer[0][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
      MPI_Isend(&tbuffer[1][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
      MPI_Isend(&tbuffer[2][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);
      
      MPI_Isend(&vbuffer[(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+4]);
      MPI_Isend(&angbuffer[(i*nGhosts*6)], pivot[i]*6, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+5]);

      MPI_Isend(&pid_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+6]);
      MPI_Isend(&rvparmat_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+7]);
    }
  }
  
  timerend(&t1);
  *timer1 = t1.total;
  
  timerstart(&t2);
  contact_detection (0, *nt, 0, *nt, t, tid, pid, v, dt, p, q, con, ncontacts);//local computation
  timerend(&t2);
  *timer2 = t2.total;
  
  TIMING t4;
  iREAL tassign=0;
  timerstart(&t3);
  unsigned int receive_idx = *nt; //set to last id
  for(int i=0;i<nNeighbors;i++)
  {
    if(rcvpivot[i] > 0)
    {
      MPI_Wait(&myrvRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
      
      timerstart(&t4);
      for(unsigned int j=0;j<rcvpivot[i];j++)
      {
        tid[receive_idx] = rcvtid_buffer[(i*n)+j]; //tids to imported
        pid[receive_idx] = rcvpid_buffer[(i*n)+j];
        parmat[receive_idx] = rvparmat_buffer[(i*n)+j];
        for(int k=0;k<3;k++)
        {
          t[0][k][receive_idx] = trvbuffer[0][(i*n*3)+(j*3)+(k)];
          t[1][k][receive_idx] = trvbuffer[1][(i*n*3)+(j*3)+(k)];
          t[2][k][receive_idx] = trvbuffer[2][(i*n*3)+(j*3)+(k)];
          
          v[k][receive_idx] = vrvbuffer[(i*n*3)+(j*3)+(k)];
        }
        for(int k=0;k<6;k++)
        {
          angular[k][receive_idx] = angrvbuffer[(i*n*6)+(j*6)+(k)];
        }
        receive_idx++;
      }
      timerend(&t4);
      tassign = tassign+t4.total;
    }
      
    if(pivot[i] > 0)
    {//safe check
      MPI_Wait(&myRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
    }
  }
  timerend(&t3);
  *timer3 = t3.total;//doesn't include timer 4
  
  if(receive_idx > *nt)
  {
    printf("Myrank[%i]: nt:%i, receive:%i\n", myrank, *nt, receive_idx);
    //range s1-e1 is outter loop, s2-e2 is inner loop in the traversal
    contact_detection (0, *nt, *nt, receive_idx, t, tid, pid, v, dt, p, q, con, ncontacts);
  }
  for(int i=0; i<3; i++)
  {
    free(tbuffer[i]);
    free(trvbuffer[i]);
  }
  
  free(pivot);
  free(rcvpivot);
  
  free(tid_buffer);
  free(rcvtid_buffer);
  
  free(pid_buffer);
  free(rcvpid_buffer);
 
  free(parmat_buffer);
  free(rvparmat_buffer);

  free(vbuffer);
  free(vrvbuffer);

  free(angbuffer);
  free(angrvbuffer);

  free(myRequest);
  free(myrvRequest);
  
  free(neighborhood);
  free(ghostTID);
  free(ghostPID);
  free(ghostTIDcrosses);
  free(ghostNeighborhood);
  for(unsigned int i = 0;i<*nt;i++)
  {
    free(ghostTIDNeighbors[i]);
  }
}

//get triangle tids that overlap into neighbors
void loba_getGhosts(struct loba *lb, int myrank, int nNeighbors, unsigned nt, iREAL *t[3][3], unsigned int *tid, unsigned int *pid, 
                    unsigned int *ghostTID, unsigned int *ghostPID, unsigned int *nGhosts, 
                    unsigned int *nGhostNeighbors, int *ghostNeighborhood, 
                    unsigned int *ghostTIDNeighbors[], unsigned int *ghostTIDcrosses)
{
  iREAL lo[3], hi[3];
  unsigned int idx = 0; unsigned int uniqueRanks = 0;
  int *ranks = (int*) malloc(nNeighbors*sizeof(int)); 
  for(unsigned int i=0;i<nt;i++)
  {
    iREAL xmin = FLT_MAX;
    iREAL ymin = FLT_MAX;
    iREAL zmin = FLT_MAX;
    iREAL xmax = -FLT_MAX;
    iREAL ymax = -FLT_MAX;
    iREAL zmax = -FLT_MAX;
    for(int ii=0;ii<3;ii++) //loop through A,B,C vertex
    {
      if(t[ii][0][i] < xmin) //if x > xmin
      {
        xmin = t[ii][0][i];
      }
      if(t[ii][1][i] < ymin) //if y > ymin
      {
        ymin = t[ii][1][i];
      }
      if(t[ii][2][i] < zmin) //if z > zmin
      {
        zmin = t[ii][2][i];
      }

      if(t[ii][0][i] > xmax) //if x > xmax
      {
        xmax = t[ii][0][i];
      }
      if(t[ii][1][i] > ymax) //if y > ymax
      {
        ymax = t[ii][1][i];
      }
      if(t[ii][2][i] > zmax) //if z > zmax
      {
        zmax = t[ii][2][i];
      }
    }

    lo[0] = xmin;
    lo[1] = ymin;
    lo[2] = zmin;

    hi[0] = xmax;
    hi[1] = ymax;
    hi[2] = zmax;
    int nranks = 0;
    loba_query (lb, myrank, lo, hi, ranks, &nranks);
    if(nranks > 1)
    {
      int counter = 0;
      for(int j=0;j<nranks;j++)
      {
        if(ranks[j] != myrank)
        {
          ghostTIDNeighbors[idx][counter] = ranks[j];
          counter++;
          //printf("RANK[%i]: ranks[j]:%i, neighborhood:%i uniqueRanks:%i\n", myrank, ranks[j], ghostNeighborhood[ranks[j]], uniqueRanks);
          if(ghostNeighborhood[ranks[j]] < 0)
          {
            ghostNeighborhood[ranks[j]] = uniqueRanks;
            //printf("RANK[%i]: uniqueRank:%i\n", myrank, ghostNeighborhood[ranks[j]]);
            uniqueRanks++;
          }
        }
      }
      ghostTIDcrosses[idx] = counter;
      ghostTID[idx] = tid[i];
      ghostPID[idx] = pid[i];
      idx++;
    }
  }
  *nGhosts = idx;
  *nGhostNeighbors = uniqueRanks;
  free(ranks);
}

/*
void loba_migrateGhosts(struct loba *lb, int  myrank, unsigned int *nt, iREAL *t[3][3], iREAL *v[3], iREAL dt, iREAL *p[3], iREAL *q[3], unsigned int *tid, unsigned int *pid, unsigned int long long *ncontacts, iREAL *timer1, iREAL *timer2, iREAL *timer3)
{
    TIMING t1, t2, t3;
 
    timerstart(&t1);
    int nproc=0;
    int nNeighbors=0;
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    int *neighborhood = (int *) malloc(nproc * sizeof(int));
    loba_getAdjacent(lb, myrank, neighborhood, &nNeighbors);

    unsigned int *pivot = (unsigned int*) malloc(sizeof(unsigned int)*nNeighbors);
    unsigned int *rcvpivot =  (unsigned int*) malloc(sizeof(unsigned int)*nNeighbors);
    unsigned int *idx = (unsigned int*) malloc(sizeof(unsigned int)*nNeighbors);

    MPI_Request *myRequest = (MPI_Request*) malloc(nNeighbors*15*sizeof(MPI_Request));//6 sends
    MPI_Request *myrvRequest = (MPI_Request*) malloc(nNeighbors*15*sizeof(MPI_Request));//6 sends
    
    idx[0] = *nt;
    unsigned int sum =0;
    for(int i=0; i<nNeighbors; i++)
    {
      int proc = neighborhood[i];
      MPI_Irecv(&rcvpivot[i], 1, MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[i*15]);
    }

    for(int i=0; i<nNeighbors; i++)
    {
      int proc = neighborhood[i];
      pivot[i] = *nt;
      MPI_Isend(&pivot[i], 1, MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[i*15]);
    }

    for(int i=0; i<nNeighbors; i++)
    {
      MPI_Wait(&myrvRequest[i*15], MPI_STATUS_IGNORE);
      if(i<nNeighbors-1)
      {
        idx[i+1] = idx[i] + rcvpivot[i];
      }
      sum = sum + rcvpivot[i];
    }
    
    for(int i=0; i<nNeighbors; i++)
    {
      MPI_Wait(&myRequest[i*15], MPI_STATUS_IGNORE);
    }

    for(int i=0; i<nNeighbors; i++)
    {
      int proc = neighborhood[i];
      if(rcvpivot[i] > 0)
      {//safe check
        MPI_Irecv(&tid[idx[i]], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+1]);

        MPI_Irecv(&t[0][0][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+2]);
        MPI_Irecv(&t[1][0][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+3]);
        MPI_Irecv(&t[2][0][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+4]);
        MPI_Irecv(&t[0][1][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+5]);
        MPI_Irecv(&t[1][1][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+6]);
        MPI_Irecv(&t[2][1][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+7]);
        MPI_Irecv(&t[0][2][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+8]);
        MPI_Irecv(&t[1][2][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+9]);
        MPI_Irecv(&t[2][2][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+10]);

        MPI_Irecv(&v[0][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+11]);
        MPI_Irecv(&v[1][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+12]);
        MPI_Irecv(&v[2][idx[i]], rcvpivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+13]);

        MPI_Irecv(&pid[idx[i]], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*15)+14]);
      }
    }

    for(int i=0;i<nNeighbors;i++)
    {
      int proc = neighborhood[i];
      if(pivot[i] > 0)
      {//safe check
        MPI_Isend(&tid[0], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+1]);
        
        MPI_Isend(&t[0][0][0], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+2]);
        MPI_Isend(&t[1][0][0], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+3]);
        MPI_Isend(&t[2][0][0], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+4]);
        MPI_Isend(&t[0][0][1], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+5]);
        MPI_Isend(&t[1][0][1], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+6]);
        MPI_Isend(&t[2][0][1], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+7]);
        MPI_Isend(&t[0][0][2], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+8]);
        MPI_Isend(&t[1][0][2], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+9]);
        MPI_Isend(&t[2][0][2], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+10]);
     	
        MPI_Isend(v[0], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+11]);
        MPI_Isend(v[1], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+12]);
        MPI_Isend(v[2], pivot[i], MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+13]);
        
        MPI_Isend(&pid[0], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*15)+14]);
      }
    }
  
    timerend(&t1);
    *timer1 = t1.total;

    timerstart(&t2); 
    contact_detection (0, *nt, 0, *nt, *nt, t, v, dt, p, q, ncontacts);//local computation
    timerend(&t2);    
    *timer2 = t2.total; 

    timerstart(&t3);
    for(int i=0;i<nNeighbors;i++)
    {
      if(rcvpivot[i] > 0)
      {
	MPI_Wait(&myrvRequest[(i*15)+1], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+2], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+3], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+4], MPI_STATUS_IGNORE);
     	  MPI_Wait(&myrvRequest[(i*15)+5], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+6], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+7], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+8], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+9], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+10], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+11], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+12], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+13], MPI_STATUS_IGNORE);
        MPI_Wait(&myrvRequest[(i*15)+14], MPI_STATUS_IGNORE);
      }
        
      if(pivot[i] > 0)
      {//safe check
      //printf("rank[%i]: pivot[%i]: %i\n", myrank, i, pivot[i]);
        MPI_Wait(&myRequest[(i*15)+1], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+2], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+3], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+4], MPI_STATUS_IGNORE);
    	  MPI_Wait(&myRequest[(i*15)+5], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+6], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+7], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+8], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+9], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+10], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+11], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+12], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+13], MPI_STATUS_IGNORE);
        MPI_Wait(&myRequest[(i*15)+14], MPI_STATUS_IGNORE);
      }
    }
    timerend(&t3);
    *timer3 = t3.total;

    //range s1-e1 is outter loop, s2-e2 is inner loop in the traversal
    contact_detection (0, *nt, *nt, sum, t, v, dt, p, q, ncontacts);
    
    free(pivot);
    free(rcvpivot);
    
    free(myRequest);
    free(myrvRequest);

    free(neighborhood);
}*/

/* free load balancer */
void loba_destroy (struct loba *lb)
{
  if (lb->zoltan) Zoltan_Destroy (&lb->zoltan);
  free (lb);
}
