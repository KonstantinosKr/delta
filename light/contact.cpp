/*
 The MIT License (MIT)
 
 Copyright (c) 2016 Konstantinos Krestenitis
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "contact.h"
#include "math.h"

contact::contact(int pid[2], int color[2], iREAL point[3], iREAL normal[3], iREAL depth, iREAL p[3], iREAL q[3])
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
    
  this->pp[0] = p[0];
  this->pp[1] = p[1];
  this->pp[2] = p[2];
  
  this->qq[0] = q[0];
  this->qq[1] = q[1];
  this->qq[2] = q[2];
  
  this->depth = depth;
}

//all-to-all range
void contact_detection (int s, int e, iREAL *t[6][3], int tid[], int pid[], 
                        iREAL *linear[3], iREAL *p[3], iREAL *q[3], 
                        std::vector<contact> conpnt[])
{
  iREAL a[3], b[3], c[3];

  for(int i=s;i<e;i++)
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
   
    bf (i+1, e, a, b, c, t[0], t[1], t[2], p, q);//use tasks 
    
    iREAL margin = 15E-2;
    
    for(int j=i+1;j<e;j++) //careful; range can overflow due to ghosts particles
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
        
        normal[0] = ((q[0][j] - p[0][j])/depth);// depth for inclusion to normal
        normal[1] = ((q[1][j] - p[1][j])/depth);
        normal[2] = ((q[2][j] - p[2][j])/depth);
        
        int found=0;
        for(unsigned int ii=0; ii<conpnt[pid[i]].size(); ii++)
        {
          if(conpnt[pid[i]][ii].pp[0] == p[0][j] ||conpnt[pid[i]][ii].pp[1] == p[1][j] ||conpnt[pid[i]][ii].pp[2] == p[2][j] || conpnt[pid[i]][ii].qq[0] == q[0][j] ||conpnt[pid[i]][ii].qq[1] == q[1][j] || conpnt[pid[i]][ii].qq[2] == q[2][j]) 
          {
            found = 1;
          }
        }
       
        if(found!=1)
        {
          printf("PARTICLE A:%i T:%i is in CONTACT WITH PARTICLE B:%i T:%i, DEPTH:%f DIST:%f\n", pid[i], tid[i], pid[j], tid[j], depth, dist);
          int color[2], id[2];
          id[0] = pid[i];
          id[1] = pid[j];
          
          color[0] = 0;
          color[1] = 0;
          iREAL pp[3]; iREAL qq[3];
          pp[0] = p[0][j];
          pp[1] = p[1][j];
          pp[2] = p[2][j];

          qq[0] = q[0][j];
          qq[1] = q[1][j];
          qq[2] = q[2][j];

          contact point(id, color, midpt, normal, depth, pp, qq);
          conpnt[pid[i]].push_back(point);
       printf("contact point of body: %i is: %f %f %f\n", j,  conpnt[pid[i]][0].point[0], conpnt[pid[i]][0].point[1], conpnt[pid[i]][0].point[2]); 
        }
      }
    }
  }
}

//two ranges
void contact_detection (int s1, int e1, int s2, int e2, iREAL *t[6][3], int tid[], int pid[], iREAL *linear[3], iREAL *p[3], iREAL *q[3], std::vector<contact> conpnt[])
{
  iREAL a[3], b[3], c[3];

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
    
    for(int j=s2;j<e2;j++) //careful; range can overflow due to ghosts particles
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
        
        normal[0] = ((q[0][j] - p[0][j])/depth);// depth for inclusion to normal
        normal[1] = ((q[1][j] - p[1][j])/depth);
        normal[2] = ((q[2][j] - p[2][j])/depth);
        
        int found=0;
        for(unsigned int ii=0; ii<conpnt[pid[i]].size(); ii++)
        {
          if(conpnt[pid[i]][ii].pp[0] == p[0][j] ||conpnt[pid[i]][ii].pp[1] == p[1][j] ||conpnt[pid[i]][ii].pp[2] == p[2][j] || conpnt[pid[i]][ii].qq[0] == q[0][j] ||conpnt[pid[i]][ii].qq[1] == q[1][j] || conpnt[pid[i]][ii].qq[2] == q[2][j]) 
          {
            found = 1;
          }
        }
       
        if(found!=1)
        {
        printf("PARTICLE A:%i T:%i is in CONTACT WITH GHOST PARTICLE B:%i T:%i\n", pid[i], tid[i], pid[j], tid[j]);
          int color[2], id[2];
          id[0] = pid[i];
          id[1] = pid[j];
          
          color[0] = 0;
          color[1] = 0;
          iREAL pp[3]; iREAL qq[3];
          pp[0] = p[0][j];
          pp[1] = p[1][j];
          pp[2] = p[2][j];

          qq[0] = q[0][j];
          qq[1] = q[1][j];
          qq[2] = q[2][j];

          contact point(id, color, midpt, normal, depth, pp, qq);
          conpnt[pid[i]].push_back(point);
        }
      }
    }
  }
}
