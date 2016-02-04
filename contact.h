#ifndef HEADER_FILE
#define HEADER_FILE

#include "bf_ispc.h"
#include "algo.h"
#define CONBUF 8
#define LSIZE 96
/* master contact points; global array is used because
 * constitutive data at contacts can be persistent */


struct master_conpnt
{
  int master[CONBUF]; /* ellipsoid */
  int slave[2][CONBUF]; /* particle, ellipsoid or -(triangle+1), unused */
  int color[2][CONBUF];
  iREAL point[3][CONBUF];
  iREAL normal[3][CONBUF];
  iREAL depth[CONBUF];
  iREAL force[3][CONBUF];
  int size;

  struct master_conpnt * next; /* local list */
  int lock; /* list update lock */
};

/* slave contact points; they are created by
 * symmetrically coppying master contact points */
struct slave_conpnt
{
  int master[2][CONBUF]; /* particle, ellipsoid */
  iREAL point[3][CONBUF];
  iREAL force[3][CONBUF];
  int size;

  struct slave_conpnt * next; /* local list */
  int lock; /* list update lock */
};

/* calculate distances */
void contact_detection (unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2, 
                        iREAL *t[6][3], unsigned int *tid, unsigned int *pid, iREAL *v[3], iREAL dt, 
                        iREAL *p[3], iREAL *q[3], master_conpnt *con);

master_conpnt * newcon (master_conpnt * master, int *k);
slave_conpnt * newcon (slave_conpnt * slave, int *k);
#endif
