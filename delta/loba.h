#ifndef __rcb__
#define __rcb__

#include <zoltan.h>
#include "contact.h"

enum algo
{
  ZOLTAN_RCB,
  ZOLTAN_RIB
};

struct loba
{
  struct Zoltan_Struct *zoltan; /* Zoltan load balancer */
  enum algo al;
};

/* create load balancer */
struct loba* loba_create (enum algo al);

/* balance points up to tolerance; output migration ranks */
void loba_balance (struct loba *lb, int n, iREAL *p[3], int *id, iREAL tol,
                    int *num_import, int **import_procs, int **import_to_part, 
		                int *num_export, int **export_procs, int **export_to_part, 
                    ZOLTAN_ID_PTR *import_global_ids, ZOLTAN_ID_PTR *import_local_ids, 
                    ZOLTAN_ID_PTR *export_global_ids, ZOLTAN_ID_PTR *export_local_ids); 

void loba_getAdjacent(struct loba *lb, int myrank, int *neighborhood, int *nNeighbors);

void loba_getGhosts(struct loba *lb, int myrank, int nNeighbors, int nt, iREAL *t[3][3], int *tid, int *pid, 
                    int *ghostTID, int *ghostPID, int *nGhosts, 
                    int *nGhostNeighbors, int *ghostNeighborhood, 
                    int *ghostTIDNeighbors[], int *ghostTIDcrosses);

void loba_migrateGhosts(struct loba *lb, int  myrank, int *nt, iREAL *t[3][3], 
                      iREAL *v[3], iREAL *angular[6], int *parmat,
                      iREAL dt, iREAL *p[3], iREAL *q[3], 
                      int *tid, int *pid, master_conpnt *con, 
                      iREAL *timer1, iREAL *timer2, iREAL *timer3);

/* find ranks overlapped by the [lo,hi] box */
void loba_query (struct loba *lb, int node, iREAL lo[3], iREAL hi[3], int *ranks, int *nranks);

void loba_getbox (struct loba *lb, int part, iREAL lo[3], iREAL hi[3]);

/* free load balancer */
void loba_destroy (struct loba *lb);

#endif
