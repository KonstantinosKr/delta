#include "contact.h"
#include "math.h"
#include "material.h"

int granular_force(iREAL n[3], iREAL vij[3], iREAL oij[3], iREAL depth, int i, int j, iREAL mass[], iREAL *iparam[NINT], int ij, iREAL f[3]);

void forces (std::vector<contact> conpnt[],
  int nb, int pid[], iREAL * position[3], iREAL * angular[6], iREAL * linear[3],
  iREAL mass[], iREAL *force[3], iREAL *torque[3], iREAL gravity[3], int parmat[], iREAL * mparam[NMAT],
  int pairnum, int pairs[], int ikind[], iREAL * iparam[NINT]);

