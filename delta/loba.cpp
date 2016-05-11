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
#include <float.h>
#include "loba.h"
#include "tmr.h"

struct zoltan_args
{
  int n;
  iREAL *p[3];
  int *id;
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
  for (int i = 0; i < args->n; i ++)
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
  struct loba *lb = new struct loba;

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

// balance points up to tolerance; output migration ranks
void loba_balance (struct loba *lb, int n, iREAL *p[3], int *id, iREAL tol,
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
      
      int changes, num_gid_entries, num_lid_entries; // TODO: do we need this outside? 
      
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
void loba_query (struct loba *lb, int node, double lo[3], double hi[3], int *ranks, int *nranks, int *parts, int *nparts)
{
  switch (lb->al)
  {
    case ZOLTAN_RCB:
    {
      Zoltan_LB_Box_PP_Assign (lb->zoltan, lo[0], lo[1], lo[2], hi[0], hi[1], hi[2], ranks, nranks, parts, nparts);
      break;
    }
    case ZOLTAN_RIB:
    {
    }
  }
}

/* find ranks overlapped by the [lo,hi] box */
void loba_query (struct loba *lb, int node, double lo[3], double hi[3], int *ranks, int *nranks)
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

// find rank owning the point
void loba_query (struct loba *lb, double point[3], int *rank)
{
  switch (lb->al)
  {
    case ZOLTAN_RCB:
    {
      Zoltan_LB_Point_Assign (lb->zoltan, point, rank);
      break;
    }
    case ZOLTAN_RIB:
    {
    }
  }
}

void loba_getAdjacent(struct loba *lb, int myrank, int *neighborhood, int *nNeighbors)
{
  double mylo[3], myhi[3], lo[3], hi[3];

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
 
void loba_getbox (struct loba *lb, int part, double lo[3], double hi[3])
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
          lo[j] = -350;
        } 
        
        if(hi[j] > FLT_MAX)
        {
          hi[j] = 350;
        } 
      }
      
      break;
    }
    case ZOLTAN_RIB:
    {
    }
  }
}

//get triangle tids that overlap into neighbors
void loba_getGhosts(struct loba *lb, int myrank, int nNeighbors, int nt, iREAL *t[6][3], int *tid, int *pid, 
                    int *ghostlocalTID, int *ghostTID, int *ghostPID, int *nGhosts, 
                    int *nGhostNeighbors, int *ghostNeighborhood, 
                    int *ghostTIDNeighbors[], int *ghostTIDcrosses)
{
  double lo[3], hi[3];
  int idx = 0; int uniqueRanks = 0;
  int *ranks = new int[nNeighbors];
  //int *parts = (int*) malloc(nNeighbors*sizeof(int)); 
  
  for(int i=0;i<nt;i++)
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
    //int nparts = 0;
    
    //loba_query (lb, myrank, lo, hi, ranks, &nranks, parts, &nparts);
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
      ghostlocalTID[idx] = i;
      ghostTID[idx] = tid[i];
      ghostPID[idx] = pid[i];
      idx++;
    }
  }
  *nGhosts = idx;
  *nGhostNeighbors = uniqueRanks;
  //free(ranks);
  //free(parts);
}

/* free load balancer */
void loba_destroy (struct loba *lb)
{
  if (lb->zoltan) Zoltan_Destroy (&lb->zoltan);
  free (lb);
}
