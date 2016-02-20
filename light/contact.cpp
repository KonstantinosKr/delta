#include "contact.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <omp.h>

contact::contact(int pid[2], int color[2], iREAL point[3], iREAL normal[3], iREAL depth)
{
  this->pid[0] = pid[0];
  this->pid[1] = pid[1];

  this->color[0] = color[0];
  this->color[1] = color[1];

  this->point[0] = point[0];
  this->point[1] = point[1];
  this->point[2] = point[2];

  this->normal[0] = normal[0];
  this->normal[1] = normal[1];
  this->normal[2] = normal[2];
    
  this->depth = depth;
}

//s1 and e1 mean start of section 1 and end of section 1, same for s2,e2 and nt size nts1, nts2
void contact_detection (int s1, int e1, int s2, int e2, iREAL *t[6][3], int *tid, int *pid, iREAL *linear[3], iREAL *p[3], iREAL *q[3], std::vector<contact> conpnt[])
{
  iREAL a[3], b[3], c[3];

  //Set triangle 1 points A,B,C
  //#pragma force inline recursive
  //#pragma omp parallel for schedule(dynamic,1) collapse(2)
  for(int i=s1;i<e1;i++)
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
    
    bf (s2, e2, a, b, c, t[0], t[1], t[2], p, q);//use tasks 
     
    iREAL margin = 15E-2;
    
    for(int j=i+1;j<e1;j++) //careful; range can overflow due to ghosts particles
    {
      if(pid[i] == pid[j])continue;
      iREAL dist = sqrt(pow((q[0][j]-p[0][j]),2)+pow((q[1][j]-p[1][j]),2)+pow((q[2][j]-p[2][j]),2));
      
      if(dist < margin)
      {
        iREAL midpt[3], normal[3];
        
        midpt[0] = (p[0][j]+q[0][j])/2; //x
        midpt[1] = (p[1][j]+q[1][j])/2; //y
        midpt[2] = (p[2][j]+q[2][j])/2; //z
    
        iREAL depth = margin-dist;
        
        //iREAL mul = 1/sqrt(pow(midpt[0],2)+pow(midpt[1],2)+pow(midpt[2],2));
        normal[0] = ((q[0][j] - p[0][j])/depth);//*depth for inclusion to normal
        normal[1] = ((q[1][j] - p[1][j])/depth);
        normal[2] = ((q[2][j] - p[2][j])/depth);
       
        int found=0;
        for(unsigned int ii=0; ii<conpnt[pid[i]].size(); ii++)
        {
          if(conpnt[pid[i]][ii].depth == depth) 
          {
            found = 1;
          }
        }
        if(found!=1)
        {
          printf("Body:%i - TID[%i]:%i is in Contact with Body:%i - TID[%i]: %i dist:%f\n", pid[i], i, tid[i], pid[j], j, tid[j], dist);
          int color[2], id[2];
          
          id[0] = pid[i];
          id[1] = pid[j];

          color[0] = 0;
          color[1] = 0;

          contact point(id, color, midpt, normal, depth);
          conpnt[pid[i]].push_back(point);
        }
      }
    }
  }
}

