#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <zoltan.h>

void migrate_triangles (unsigned int size, unsigned int *nt, iREAL *t[3][3], iREAL *v[3],
                              iREAL *angular[6], int *parmat,
                              unsigned int *tid, unsigned int *pid,  
                              int num_import, int *import_procs, int *import_to_part, 
                              int num_export, int *export_procs, int *export_to_part,
                              ZOLTAN_ID_PTR import_global_ids, ZOLTAN_ID_PTR import_local_ids,
                              ZOLTAN_ID_PTR export_global_ids, ZOLTAN_ID_PTR export_local_ids);


