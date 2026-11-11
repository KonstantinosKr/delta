#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "penalty_ispc.h"
#include "bf_ispc.h"
#include "algo.h"
#include "math.h"
#include <ctime>

#define nsize 10000000

using namespace ispc;

void rndtriangles(unsigned n, REAL length, REAL * a[3], REAL * b[3], REAL * c[3], REAL * d[3], REAL * e[3], REAL * f[3])
{
  int padding = 2*length;
  
  for(unsigned i=0;i<n;i++)
  {
    a[0][i] = (length * drand48());
    a[1][i] = (length * drand48());
    a[2][i] = (length * drand48());

    b[0][i] = (length * drand48());
    b[1][i] = (length * drand48());
    b[2][i] = (length * drand48());

    c[0][i] = (length * drand48());
    c[1][i] = (length * drand48());
    c[2][i] = (length * drand48());

    d[0][i] = (length * drand48() + padding);
    d[1][i] = (length * drand48() + padding);
    d[2][i] = (length * drand48() + padding);

    e[0][i] = (length * drand48() + padding);
    e[1][i] = (length * drand48() + padding);
    e[2][i] = (length * drand48() + padding);

    f[0][i] = (length * drand48() + padding);
    f[1][i] = (length * drand48() + padding);
    f[2][i] = (length * drand48() + padding);
  }
}

int main (int argc, char *argv[])
{
  static unsigned n= nsize;

  REAL *a[3];
  REAL *b[3];
  REAL *c[3];
  REAL *d[3];
  REAL *e[3];
  REAL *f[3];
  REAL *p1[3];
  REAL *q1[3];
    
    REAL *p2[3];
    REAL *q2[3];
  static REAL distance1[nsize];
  static REAL distance2[nsize];
    
  for(unsigned i=0;i<3;i++)
  {
      a[i]= ispc_allocate (n);
      b[i]= ispc_allocate (n);
      c[i]= ispc_allocate (n);
      d[i]= ispc_allocate (n);
      e[i]= ispc_allocate (n);
      f[i]= ispc_allocate (n);
      p1[i]= ispc_allocate (n);
      q1[i]= ispc_allocate (n);
      p2[i]= ispc_allocate (n);
      q2[i]= ispc_allocate (n);
  }
  rndtriangles(n, 10, a, b, c, d, e, f);
  
  ispc_pen (n, a, b, c, d, e, f, p1, q1, distance1);
  ispc_bf (n, a, b, c, d, e, f, p2, q2, distance2);
    
    FILE *fp1 = fopen("output.data", "w+");
    FILE *fp2 = fopen("input.data", "w+");
    
    fprintf(fp1,"ID, p1[0], p2[0], p1[1], p2[1], p1[2], p2[2], q1[0], q2[0], q1[1], q2[1], q1[2], q2[2], distancePen, distanceBF\n");
    fprintf(fp2,"ID, a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], d[0], d[1], d[2], e[0], e[1], e[2], f[0], f[1], f[2], distancePen, distanceBF\n");
    
    unsigned counter = 0;
    for(unsigned i = 0; i < n; i++)
    {
        fprintf(fp1,"%i, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f\n", \
                    counter, p1[0][i], p2[0][i], p1[1][i], p2[1][i], p1[2][i], p2[2][i], q1[0][i], q2[0][i], q1[1][i], q2[1][i], q1[2][i], q2[2][i], distance1[i], distance2[i]);
            
        fprintf(fp2,"%i, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.2f, %.2f\n", \
                    counter, a[0][i], a[1][i], a[2][i], b[0][i], b[1][i], b[2][i], c[0][i], c[1][i], c[2][i], d[0][i], d[1][i], d[2][i], e[0][i], e[1][i], e[2][i], f[0][i], f[1][i], f[2][i], distance1[i], distance2[i]);
            
        counter++;
    }
    fclose(fp1);
    fclose(fp2);
  return 0;
}

