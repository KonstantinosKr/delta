#ifndef HEADER_FILE
#define HEADER_FILE

#include "algo.h"
#include "bf.h"
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
void contact_detection (int s, int e, 
                        iREAL *t[3][3], int *tid, int *pid, iREAL *v[3], 
                        iREAL *p[3], iREAL *q[3], std::vector<contact> conpnt[]);
#endif
