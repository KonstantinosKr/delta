#include "math.h"
#include "float.h"

extern "C"{
#include "ext/hul/hul.h"
}

void getCentroid(int pid, int range1, int range2, iREAL *t[3][3], iREAL *centroid[6]);

void nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, int &nt, int nb, int idx, iREAL *t[6][3], int tid[], int pid[], iREAL *position[3], iREAL *mint, iREAL *maxt);

void createWall(int &n, int &nt, int nb, iREAL *t[3][3], int *tid, int *pid, iREAL A[3], iREAL B[3], iREAL C[3], iREAL D[3]);


