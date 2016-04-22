#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "dynamics.h"
#include <time.h>
#include <math.h>
#include "geom.h"

extern "C"{
#include "ext/hul/hul.h"
}

void condition_enviroment(int nb, iREAL lin[3], iREAL ang[3], int ma, 
                          iREAL *linear[3], iREAL *angular[6], 
                          iREAL *rotation[9], iREAL *mass, 
                          iREAL *inertia[9], iREAL *inverse[9], int *parmat);

void load_enviroment(int ptype[], int &nt, int nb, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL &mint, iREAL &maxt);

void init_enviroment(int scene, int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3]);

void load_vtk(int &nt, int nb, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL &mint, iREAL &maxt);

void twoParticleCollision(int &nt, int &nb, iREAL *t[6][3],
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3]);


void oneParticleVsWall(int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3]);


void chaos(int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3]);
