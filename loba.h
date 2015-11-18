#ifndef __rcb__
#define __rcb__

#include <zoltan.h>

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
void loba_balance (struct loba *lb, unsigned int n, iREAL *p[3], unsigned int *id, iREAL tol,
                    int *num_import, int **import_procs, 
		    int *num_export, int **export_procs, 
                    ZOLTAN_ID_PTR *import_global_ids, ZOLTAN_ID_PTR *import_local_ids, 
                    ZOLTAN_ID_PTR *export_global_ids, ZOLTAN_ID_PTR *export_local_ids);

void loba_getAdjacent(struct loba *lb, int myrank, int *neighborhood, int *nNeighbors);

void loba_migrateGhosts(struct loba *lb, int  myrank, unsigned int *nt, iREAL *t[3][3], iREAL *v[3], iREAL *p[3], iREAL *q[3], iREAL *distance, unsigned int *tid, unsigned int *pid, iREAL *timer1, iREAL *timer2, iREAL *timer3);

/* find ranks overlapped by the [lo,hi] box */
void loba_query (struct loba *lb, int node, iREAL lo[3], iREAL hi[3], int *ranks, int *nranks);

void loba_getbox (struct loba *lb, int part, iREAL lo[3], iREAL hi[3]);

/* free load balancer */
void loba_destroy (struct loba *lb);

#endif
