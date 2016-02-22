#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "tmr.h"
#include "loba.h"
#include "contact.h"
#include <vector>
#include <mpi.h>
#include <zoltan.h>

void migrate(int &nt, iREAL *t[6][3], iREAL *v[3],
                        iREAL *angular[6], int *parmat,
                        int *tid, int *pid,  
                        int num_import, int *import_procs, int *import_to_part, 
                        int num_export, int *export_procs, int *export_to_part,
                        ZOLTAN_ID_PTR import_global_ids, ZOLTAN_ID_PTR import_local_ids,
                        ZOLTAN_ID_PTR export_global_ids, ZOLTAN_ID_PTR export_local_ids);

void migrateGhosts(struct loba *lb, int myrank, int nt, iREAL *t[6][3], 
                      iREAL *linear[3], iREAL *angular[6], int *parmat,
                      iREAL step, iREAL *p[3], iREAL *q[3], 
                      int tid[], int pid[], std::vector<contact> conpnt[], 
                      iREAL *timer1, iREAL *timer2, iREAL *timer3);

