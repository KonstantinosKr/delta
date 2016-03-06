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
#include <string.h>
#include <limits.h>
#include <float.h>
#include "tmr.h"
#include "log.h"
#include "loba.h"
#include "contact.h"
#include <vector>
#include <mpi.h>
#include <zoltan.h>

void migrate (struct loba *lb, int &nt, int &nb, iREAL *t[6][3], iREAL *linear[3],
              iREAL *angular[6], iREAL *rotation[9], iREAL *position[6], iREAL *inertia[9], iREAL *inverse[9], iREAL mass[], int *parmat,
              int *tid, int *pid,
              int num_import, int *import_procs, int *import_to_part, 
              int num_export, int *export_procs, int *export_to_part,
              ZOLTAN_ID_PTR import_global_ids, ZOLTAN_ID_PTR import_local_ids,
              ZOLTAN_ID_PTR export_global_ids, ZOLTAN_ID_PTR export_local_ids);

void migrateGhosts(struct loba *lb, int  myrank, int nt, int nb, iREAL *t[6][3], 
              iREAL *v[3], iREAL *angular[6], iREAL *rotation[9], iREAL *position[6], iREAL *inertia[9], iREAL *inverse[9], iREAL mass[], int *parmat,
              iREAL dt, iREAL *p[3], iREAL *q[3], 
              int tid[], int pid[], std::vector<contact> conpnt[], 
              iREAL *timer1, iREAL *timer2, iREAL *timer3);

void init_migratePosition (struct loba *lb, int &nb, iREAL *linear[3],
                    iREAL *angular[6], iREAL *rotation[9], 
                    iREAL *position[6], iREAL *inertia[9], 
                    iREAL *inverse[9], iREAL mass[]);

void migratePosition (struct loba *lb, int &nb, iREAL *linear[3],
                    iREAL *angular[6], iREAL *rotation[9], 
                    iREAL *position[6], iREAL *inertia[9], 
                    iREAL *inverse[9]);

void migrateForce(struct loba *lb, int myrank, int *rank, int nranks, iREAL *force[3], iREAL *torque[3]);

void migrateForce(struct loba *lb, int myrank, int *rank, int *fpid, int nranks, iREAL *force[3], iREAL *torque[3]);
