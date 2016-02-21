#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "loba.h"
#include <zoltan.h>

void output_state(struct loba *lb, int myrank, int nt, iREAL *t[6][3], int timesteps);
void postProcessing(int nranks, int size, int timesteps);
