#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <math.h>
#include "geom.h"
#include "algo.h"

void init_env(int &nt, int &nb, iREAL *t[3][3], iREAL *linear[3], iREAL *mass, int *parmat, int tid[], int pid[], iREAL *position[3], iREAL lo[3], iREAL hi[3]);

void load_vtk(int &nt, int &nb, int IDX, iREAL *t[3][3], int tid[], int pid[], iREAL *position[3], iREAL *mint, iREAL *maxt);

void load_env(int ptype[], int &nt, int &nb, iREAL *t[3][3], int tid[], int pid[], iREAL *position[3], iREAL *mint, iREAL *maxt);

void trans_env(int i, int pid, iREAL *t[3][3], iREAL *p[3]);

void cond_env(int nb, iREAL *linear[3], iREAL *mass, int *parmat);

