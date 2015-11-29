#include "contact.h"
#include "stdio.h"
#include "stdlib.h"
#include "algo.h"
#include "math.h"
#include <omp.h>

using namespace ispc;
//s1 and e1 mean start of section 1 and end of section 1, same for s2,e2 and nt size nts1, nts2
void contact_detection (unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2, iREAL *t[3][3], iREAL *v[3], iREAL dt, iREAL *p[3], iREAL *q[3], iREAL *distance, unsigned long long int *ncontacts)
{
  unsigned int nts1 = e1-s1;
  unsigned int nts2 = e2-s2;
  
  *ncontacts = 0;//use as counter
  iREAL a[3], b[3], c[3];

  omp_set_num_threads(4);//don't know much to set this
  
  //Set triangle 1 points A,B,C
  #pragma force inline recursive
  #pragma omp parallel for schedule(dynamic,1) collapse(2)
  for(unsigned int i=s1;i<e1;i++)
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
    
    ispc_bf (s2, e2, a, b, c, t[0], t[1], t[2], p, q);//use tasks 
     
    iREAL marginT1 = 1E-3*dt;
    iREAL marginT2 = 1E-3*dt;
    iREAL margin = marginT1+marginT2
    for(unsigned int j=s2;j<e2;j++) //careful; range can overflow due to ghosts particles
    {
      iREAL dist = sqrt(pow((q[0][j]-p[0][j]),2)+pow((q[1][j]-p[1][j]),2)+pow((q[2][j]-p[1][j]),2));
      //if there is margin overlap
      if(dist < margin && dist != 0 && (pid[i] != pid[j]))
      {//contact found, //if not same particle body //get min distance contact
        distance[j] = dist;
     
        iREAL midpt[3];
        midpt[0] = (p[0]+q[0])/2; //x
        midpt[1] = (p[1]+q[1])/2; //y
        midpt[2] = (p[2]+q[2])/2; //z
    
        iREAL depth = dist - margin;
    
        iREAL normal[3];
        iREAL mul = 1/sqrt(pow(midpt[0],2)+pow(midpt[1],2)+pow(midpt[2],2));
        normal[0] = mul*midpt[0];
        normal[1] = mul*midpt[1];
        normal[2] = mul*midpt[2];
        *ncontacts++;

        //store contact point to tree
      }
    }
  }
}

