#include "contact.h"
#include "math.h"

void forces(unsigned int *tid, unsigned int *pid, master_conpnt *con);

void granular_force(iREAL n[3], iREAL vij[3], iREAL oij[3], iREAL depth, int i, int j, iREAL mass[], iREAL invm[], int ij, iREAL f[3]);
