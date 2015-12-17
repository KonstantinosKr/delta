#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "delta.h"

int main (int argc, char **argv)
{
  iREAL *t[3][3]; /* triangles */
  unsigned int nt = 27000000; /* memory buffer size */
   
  for (int i = 0; i < 3; i ++)
  { 
    t[0][i] = (iREAL *) malloc (nt*sizeof(iREAL));
    t[1][i] = (iREAL *) malloc (nt*sizeof(iREAL));
    t[2][i] = (iREAL *) malloc (nt*sizeof(iREAL));
  }
  
  unsigned long long int ncontacts;
  iREAL margin = 1E-3;

  delta(0, margin, 0, nt, 0, nt, t, &ncontacts);

  return 0;
}
