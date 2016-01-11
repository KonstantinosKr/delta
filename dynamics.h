#include <stdlib.h>
#include "contact.h"
#include "algo.h"

void gen_velocities (iREAL lo[3], iREAL hi[3], unsigned int nt, iREAL * v[3]);
void integrate (iREAL step, iREAL lo[3], iREAL hi[3], unsigned int nt, iREAL * t[3][3], iREAL * v[3]);

void dynamics (master_conpnt master[], slave_conpnt slave[],
  int nt, iREAL * angular[6], iREAL * v[3],
  iREAL * rotation[9], iREAL * position[6],
  iREAL * inertia[9], iREAL * inverse[9],
  iREAL mass[], iREAL invm[], iREAL * force[3],
  iREAL * torque[3], iREAL gravity[3], iREAL step);

