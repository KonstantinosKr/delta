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

struct bd
{
  iREAL position[3]; 
  iREAL linear[3];
  iREAL angular[3];
  iREAL inertia[9];
  iREAL inverse[9];
  iREAL invm;
  iREAL mass;

  int parmat;
};

void init_enviroment(unsigned int *nt, unsigned int *nb, bd *b, 
    iREAL *t[3][3], iREAL *v[3], unsigned int tid[], unsigned int pid[], iREAL lo[3], iREAL hi[3]);

void gen_nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, unsigned int *nt, bd *b, unsigned int bodyidx, unsigned int initidx, 
                            iREAL *t[3][3], unsigned int tid[], unsigned int pid[], iREAL *mint, iREAL *maxt);

void load_points(int ptype, unsigned int *nt, unsigned int nb, bd *b, unsigned int startIDX, 
                iREAL *t[3][3], unsigned int tid[], unsigned int pid[], iREAL *mint, iREAL *maxt);

void load_enviroment(int ptype[], unsigned int *nt, unsigned int nb, bd *b, 
                  iREAL *t[3][3], unsigned int tid[], unsigned int pid[], iREAL *mint, iREAL *maxt);

void normalize(unsigned int nt, iREAL *t[3][3], iREAL mint, iREAL maxt); 
