#include <stdlib.h>

void gen_velocities (iREAL lo[3], iREAL hi[3], unsigned int nt, iREAL * v[3]);
void integrate (iREAL step, iREAL lo[3], iREAL hi[3], unsigned int nt, iREAL * t[3][3], iREAL * v[3]);
