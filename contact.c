#include "contact.h"
#include "stdio.h"
#include "stdlib.h"
#include "algo.h"
#include "math.h"
//s1 and e1 mean start of section 1 and end of section 1, same for s2,e2 and nt size nts1, nts2
void contact_detection (unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2,  unsigned int size, iREAL *t[3][3], iREAL *p[3], iREAL *q[3], iREAL *distance)
{
  unsigned int nts1 = e1-s1;
  unsigned int nts2 = e2-s2;

  iREAL *d[3], *e[3], *f[3], *pp[3], *qq[3];
  
  //allocate memory
  for(int i=0;i<3;i++)
  {
    d[i] = (iREAL *) malloc(nts2 * sizeof(iREAL));
    e[i] = (iREAL *) malloc(nts2 * sizeof(iREAL));
    f[i] = (iREAL *) malloc(nts2 * sizeof(iREAL));
    pp[i] = (iREAL *) malloc(nts2 * sizeof(iREAL));
    qq[i] = (iREAL *) malloc(nts2 * sizeof(iREAL));
  }
  
  nts2 = 0; //use as counter
  //Set triangle 2 points D,E,F
  for(unsigned int i=s2;i<s2;i++)
  {
    d[0][nts2] = t[0][0][i];
    d[1][nts2] = t[0][1][i];
    d[2][nts2] = t[0][2][i];
    
    e[0][nts2] = t[1][0][i];
    e[1][nts2] = t[1][1][i];
    e[2][nts2] = t[1][2][i];
    
    f[0][nts2] = t[2][0][i];
    f[1][nts2] = t[2][1][i];
    f[2][nts2] = t[2][2][i];
    nts2++;
  }
  
  iREAL a[3], b[3], c[3];
  
  //Set triangle 1 points A,B,C
  for(unsigned int i=s1;i<s1;i++)
  { 
    a[0] = t[0][0][i];
    a[1] = t[0][1][i];
    a[2] = t[0][2][i];
    
    b[0] = t[1][0][i];
    b[1] = t[1][1][i];
    b[2] = t[1][2][i];
    
    c[0] = t[2][0][i];
    c[1] = t[2][1][i];
    c[2] = t[2][2][i];
    
    unsigned int nt = e2-s2;
    ispc_bf (nt, a, b, c, d, e, f, pp, qq);//use tasks 
    
    nt = 0;//use as counter
    for(unsigned int j=s2;j<e2;j++)
    {
      iREAL dist = sqrt(pow((qq[0][nt]-pp[0][nt]),2)+pow((qq[1][nt]-pp[1][nt]),2)+pow((qq[2][nt]-pp[1][nt]),2));
      if(dist < 1E-3)
      {
        p[0][j]= pp[0][nt]; 
        p[1][j]= pp[1][nt]; 
        p[2][j]= pp[2][nt]; 
        
        q[0][j] = qq[0][nt];
        q[1][j] = qq[1][nt];
        q[2][j] = qq[2][nt];

        distance[j] = dist;
      }
      nt++;
    }
  }


  for(int i=0;i<3;i++)
  {  
    free(d[i]); 
    free(e[i]);
    free(f[i]);
    free(pp[i]);
    free(qq[i]);
  }
}

