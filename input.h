#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "error.h"
#include "dynamics.h"
#include <time.h>
#include <math.h>
extern "C"{
#include "ext/hul/hul.h"
}
void init_enviroment(int *nt, int *nb, 
                    iREAL *t[6][3], iREAL *v[3], iREAL *angular[6], iREAL *inertia[9], iREAL *inverse[9], iREAL *rotation[9], 
                    iREAL *mass, iREAL *invm, int *parmat, int tid[], int pid[], iREAL *position[6], iREAL lo[3], iREAL hi[3]);

void gen_nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, int *nt, int bodyidx, int initidx, 
                              iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void load_points(int ptype, int *nt, int bodyID, int startIDX, 
                  iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void load_enviroment(int ptype[], int *nt, int nParticles, 
                    iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void normalize(int nt, iREAL *t[6][3], iREAL mint, iREAL maxt); 

// invert inertia properties 
void invert (int nt, iREAL * inertia[9], iREAL * inverse[9], iREAL mass[], iREAL invm[]);

