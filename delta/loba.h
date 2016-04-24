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


#ifndef __HEADER_FILE__
#define __HEADER_FILE__

#include <zoltan.h>
#include "contact.h"

enum algo
{
  ZOLTAN_RCB,
  ZOLTAN_RIB
};

struct loba
{
  struct Zoltan_Struct *zoltan;
  enum algo al;
};

// create load balancer
struct loba* loba_create (enum algo al);

// balance points up to tolerance; output migration ranks
void loba_balance (struct loba *lb, int n, iREAL *p[3], int *id, iREAL tol,
                    int *num_import, int **import_procs, int **import_to_part, 
		                int *num_export, int **export_procs, int **export_to_part, 
                    ZOLTAN_ID_PTR *import_global_ids, ZOLTAN_ID_PTR *import_local_ids, 
                    ZOLTAN_ID_PTR *export_global_ids, ZOLTAN_ID_PTR *export_local_ids); 

void loba_getAdjacent(struct loba *lb, int myrank, int *neighborhood, int *nNeighbors);

void loba_getGhosts(struct loba *lb, int myrank, int nNeighbors, int nt, iREAL *t[6][3], int *tid, int *pid, 
                    int *ghostlocalTID, int *ghostTID, int *ghostPID, int *nGhosts, 
                    int *nGhostNeighbors, int *ghostNeighborhood, 
                    int *ghostTIDNeighbors[], int *ghostTIDcrosses);

// find ranks overlapped by the [lo,hi] box
void loba_query (struct loba *lb, int node, double lo[3], double hi[3], int *ranks, int *nranks, int *parts, int *nparts);

// find ranks overlapped by the [lo,hi] box
void loba_query (struct loba *lb, int node, double lo[3], double hi[3], int *ranks, int *nranks);

// find rank owning the point
void loba_query (struct loba *lb, double point[3], int *rank);

void loba_getbox (struct loba *lb, int part, double lo[3], double hi[3]);

// free load balancer
void loba_destroy (struct loba *lb);

#endif
