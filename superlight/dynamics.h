
#include <stdlib.h>
#include "algo.h"
#include "math.h"
#include "contact.h"
#include "material.h"


void dynamics (std::vector<contact> conpnt[],
              int nt, int nb, iREAL *t[3][3], int *pid, iREAL * linear[3], iREAL * position[3],
              iREAL mass[], iREAL * force[3], iREAL gravity[3], iREAL step);

