#include <stdlib.h>
#include "contact.h"
#include "algo.h"
#include "math.h"
#include "material.h"

iREAL critical (int nt, iREAL mass[], int pairnum, iREAL *iparam[NINT]);

void integrate (iREAL step, iREAL lo[3], iREAL hi[3], int nt, iREAL *t[3][3], iREAL *v[3]);

void euler(int nb, iREAL *angular[6], iREAL *linear[3], iREAL *rotation[9], iREAL *position[3], iREAL step);

void dynamics (std::vector<contact>conpnt[],
  int nt, int nb, iREAL *t[6][3], int pid[], iREAL *angular[6], iREAL *linear[3],
  iREAL *rotation[9], iREAL *position[6],
  iREAL *inertia[9], iREAL *inverse[9],
  iREAL mass[], iREAL *force[3],
  iREAL *torque[3], iREAL step);

void expmap (iREAL Omega1, iREAL Omega2, iREAL Omega3,
                iREAL &Lambda1, iREAL &Lambda2, iREAL &Lambda3,
			          iREAL &Lambda4, iREAL &Lambda5, iREAL &Lambda6,
			          iREAL &Lambda7, iREAL &Lambda8, iREAL &Lambda9);
