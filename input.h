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
void init_enviroment(unsigned int *nt, unsigned int *nb, 
                    iREAL *t[6][3], iREAL *v[3], iREAL *angular[6], iREAL *inertia[9], iREAL *inverse[9], iREAL *rotation[9], 
                    iREAL *mass, iREAL *invm, int *parmat, unsigned int tid[], unsigned int pid[], iREAL *position[6], iREAL lo[3], iREAL hi[3]);

void gen_nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, unsigned int *nt, unsigned int bodyidx, unsigned int initidx, 
                            iREAL *t[6][3], unsigned int tid[], unsigned int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void load_points(int ptype, unsigned int *nt, unsigned int bodyID, unsigned int startIDX, 
                iREAL *t[6][3], unsigned int tid[], unsigned int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void load_enviroment(int ptype[], unsigned int *nt, unsigned int nParticles, 
                  iREAL *t[6][3], unsigned int tid[], unsigned int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void normalize(unsigned int nt, iREAL *t[6][3], iREAL mint, iREAL maxt); 

// invert inertia properties 
void invert (int nt, iREAL * inertia[9], iREAL * inverse[9], iREAL mass[], iREAL invm[]);

