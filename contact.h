#ifndef HEADER_FILE
#define HEADER_FILE

#include "bf_ispc.h"

/* calculate distances */
void contact_detection (unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2,  unsigned int size, iREAL *t[3][3], iREAL *p[3], iREAL *q[3], iREAL *distance);

#endif
