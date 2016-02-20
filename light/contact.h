#ifndef HEADER_FILE
#define HEADER_FILE

#include "bf.h"
#include "algo.h"
#include <vector>

struct contact {
  int pid[2];
  int color[2];
  iREAL point[3];
  iREAL normal[3];
  iREAL depth;
  iREAL force[3];

  contact(int pid[2], int color[2], iREAL point[3], iREAL normal[3], iREAL depth);
};


/* calculate distances */
void contact_detection (int s1, int e1, int s2, int e2, iREAL *t[6][3], int *tid, int *pid, iREAL *linear[3], iREAL *p[3], iREAL *q[3], std::vector<contact> conpnt[]);


#endif
