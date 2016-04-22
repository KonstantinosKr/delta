#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>

extern "C"{
#include "ext/hul/hul.h"
}

namespace geometry{
	void getCentroid(int pid, int range1, int range2, iREAL *t[6][3], iREAL *centroid[6]);

	void translate_enviroment(int i, int pid, iREAL *t[6][3], iREAL *p[6]);

	void normalize(int nt, iREAL *t[6][3], iREAL mint, iREAL maxt);

	void nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, int &nt, int nb, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL &mint, iREAL &maxt);

	void wall(iREAL lo[3], iREAL hi[3], int &nt, int nb, iREAL *t[6][3], int *tid, int *pid, iREAL *position[6]);
}
