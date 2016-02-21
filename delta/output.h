#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "loba.h"
#include <zoltan.h>

void output_state(struct loba *lb, int myrank, int nt, iREAL *t[6][3], int timesteps);

void output_log(int optionA, int optionB, struct loba *lb, int myrank, 
              int nt, int nb, iREAL *t[6][3], int timesteps,
              int pid[], int tid[], iREAL gravity[3], 
              iREAL force[], iREAL torque[], iREAL p[3], iREAL q[3],
              iREAL linear[3], iREAL angular[6], iREAL position[6], iREAL mass[], 
              iREAL tTimer1[], iREAL tTimer2[], 
              iREAL tTimer3[], iREAL tTimer4[], 
              int num_import, int num_export);


void output_performance_log();

void output_balance_log();

void output_migration_log();

void output_contact_log();

void output_ghost_log();

void output_force_log();

void output_dynamic_log();

void output_statistics_log();

void postProcessing(int nranks, int size, int timesteps);

