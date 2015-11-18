#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "error.h"
#include "loba.h"
#include <zoltan.h>

void output_state(struct loba *lb, int myrank, unsigned int nt, iREAL *t[3][3], iREAL *v[3], unsigned int timesteps);
void postProcessing(int nranks, unsigned int size, unsigned int timesteps);
