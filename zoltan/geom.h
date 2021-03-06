#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>

extern "C"{
#include "ext/hul/hul.h"
}

namespace geometry
{
	void getCentroid(int pid, int range1, int range2, iREAL *t[6][3], iREAL *centroid[6]);

	void translate(int i, int pid, iREAL *t[6][3], iREAL *p[6]);

	void normalize(int nt, iREAL *t[6][3], iREAL mint, iREAL maxt);

	void nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, int &nt,
														int nb, iREAL *t[6][3], int tid[], int pid[],
														iREAL *position[6], iREAL &mint, iREAL &maxt);

	void sphere(iREAL point, iREAL radius, int &ns, int nb,
							iREAL *s[6][3], int sid[], int pid[],
							iREAL *position[6], iREAL &mint, iREAL &maxt);

	void cube(iREAL point, iREAL length, int &nt, int nb,
						iREAL *t[6][3], int tid[], int pid[],
						iREAL *position[6], iREAL &mint, iREAL &maxt);

	void hyperrectangle(iREAL lo[3], iREAL hi[3], int &nt, int nb,
											iREAL *t[6][3], int *tid, int *pid,
											iREAL *position[6]);
}
