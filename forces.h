#include "contact.h"
#include "math.h"
#include "material.h"

int granular_force(iREAL n[3], iREAL vij[3], iREAL oij[3], iREAL depth, int i, int j, iREAL mass[], iREAL invm[], iREAL *iparam[NINT], int ij, iREAL f[3]);

void forces (master_conpnt master[], slave_conpnt slave[],
  int nb, iREAL * position[3], iREAL * angular[6], iREAL * linear[3],
  iREAL mass[], iREAL invm[], int parmat[], iREAL * mparam[NMAT],
  int pairnum, int pairs[], int ikind[], iREAL * iparam[NINT]);

