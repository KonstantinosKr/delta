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

contactpoint::contactpoint(int pid[2], int color[2], iREAL point[3], iREAL normal[3], iREAL depth, iREAL p[3], iREAL q[3])
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
void contact::detection (int s, int e, iREAL *t[6][3], int tid[], int pid[], iREAL *linear[3], std::vector<contactpoint> conpnt[])
{
  iREAL Ax[3], Ay[3], Az[3], Bx[3], By[3], Bz[3];
  
  for(int i=s;i<e;i++)
  {
    Ax[0] = t[0][0][i];
    Ax[1] = t[1][0][i];
    Ax[2] = t[2][0][i];
    
    By[0] = t[0][1][i];
    By[1] = t[1][1][i];
    By[2] = t[2][1][i];
    
    Bz[0] = t[0][2][i];
    Bz[1] = t[1][2][i];
    Bz[2] = t[2][2][i];
    
    for(int j=i+1;j<e;j++) //careful; range can overflow due to ghosts particles
    {
      if(pid[i] == pid[j])continue;
      
      Bx[0] = t[0][0][j];
      Bx[1] = t[1][0][j];
      Bx[2] = t[2][0][j];
      
      By[0] = t[0][1][j];
      By[1] = t[1][1][j];
      By[2] = t[2][1][j];
      
      Bz[0] = t[0][2][j];
      Bz[1] = t[1][2][j];
      Bz[2] = t[2][2][j];
      
      iREAL xPA, yPA, zPA;
      iREAL xPB, yPB, zPB;
      
      bf(Ax, Ay, Az, Bx, By, Bz, xPA, yPA, zPA, xPB, yPB, zPB);
      
      iREAL dist = sqrt(pow((xPB-xPA),2)+pow((yPB-yPA),2)+pow((zPB-zPA),2));
      
      iREAL margin = 15e-2;
      
      if(dist < margin)
      {
        int found=0;
        for(unsigned int ii=0; ii<conpnt[pid[i]].size(); ii++)
        {
          if(conpnt[pid[i]][ii].pp[0] == xPA ||conpnt[pid[i]][ii].pp[1] == yPA ||conpnt[pid[i]][ii].pp[2] == zPA || conpnt[pid[i]][ii].qq[0] == xPB ||conpnt[pid[i]][ii].qq[1] == yPB || conpnt[pid[i]][ii].qq[2] == zPB)
          {
            found = 1;
          }
        }
        
        if(found!=1)
        {
          iREAL midpt[3], normal[3];
          
          midpt[0] = (xPA+xPB)/2; //x
          midpt[1] = (yPA+yPB)/2; //y
          midpt[2] = (zPA+zPB)/2; //z
          
          iREAL depth = margin-dist;
          
          normal[0] = ((xPB - xPA)/depth);// depth for inclusion to normal
          normal[1] = ((yPB - yPA)/depth);
          normal[2] = ((zPB - zPA)/depth);
          printf("PARTICLE A:%i T:%i is in CONTACT WITH PARTICLE B:%i T:%i DEPTH:%f DIST:%f\n", pid[i], tid[i], pid[j], tid[j], depth, dist);
          int color[2], id[2];
          id[0] = pid[i];
          id[1] = pid[j];
          
          color[0] = 0;
          color[1] = 0;
          iREAL pp[3]; iREAL qq[3];
          pp[0] = xPA;
          pp[1] = yPA;
          pp[2] = zPA;
          
          qq[0] = xPB;
          qq[1] = yPB;
          qq[2] = zPB;
          
          contactpoint point(id, color, midpt, normal, depth, pp, qq);
          conpnt[pid[i]].push_back(point);
        }
      }
    }
  }
}

//two ranges
void contact::detection (int s1, int e1, int s2, int e2, iREAL *t[6][3], int tid[], int pid[], iREAL *linear[3], std::vector<contactpoint> conpnt[])
{
  iREAL Ax[3], Ay[3], Az[3], Bx[3], By[3], Bz[3];
  
  for(int i=s1;i<e1;i++)
  {
    Ax[0] = t[0][0][i];
    Ax[1] = t[1][0][i];
    Ax[2] = t[2][0][i];
    
    By[0] = t[0][1][i];
    By[1] = t[1][1][i];
    By[2] = t[2][1][i];
    
    Bz[0] = t[0][2][i];
    Bz[1] = t[1][2][i];
    Bz[2] = t[2][2][i];
    
    for(int j=s2; j<e2; j++) //careful; range can overflow due to ghosts particles
    {
      if(pid[i] == pid[j])continue;
      
      Bx[0] = t[0][0][j];
      Bx[1] = t[1][0][j];
      Bx[2] = t[2][0][j];
      
      By[0] = t[0][1][j];
      By[1] = t[1][1][j];
      By[2] = t[2][1][j];
      
      Bz[0] = t[0][2][j];
      Bz[1] = t[1][2][j];
      Bz[2] = t[2][2][j];
      
      iREAL xPA, yPA, zPA;
      iREAL xPB, yPB, zPB;
      
      bf(Ax, Ay, Az, Bx, By, Bz, xPA, yPA, zPA, xPB, yPB, zPB);
      
      iREAL dist = sqrt(pow((xPB-xPA),2)+pow((yPB-yPA),2)+pow((zPB-zPA),2));
      
      iREAL margin = 15E-2;
      
      if(dist < margin)
      {
        int found = 0;
        for(unsigned int ii=0; ii<conpnt[pid[i]].size(); ii++)
        {
          if(conpnt[pid[i]][ii].pp[0] == xPA ||conpnt[pid[i]][ii].pp[1] == yPA ||conpnt[pid[i]][ii].pp[2] == zPA || conpnt[pid[i]][ii].qq[0] == xPB ||conpnt[pid[i]][ii].qq[1] == yPB || conpnt[pid[i]][ii].qq[2] == zPB)
          {
            found = 1;
          }
        }
        
        if(found!=1)
        {
          //printf("PARTICLE A:%i T:%i is in CONTACT WITH GHOST PARTICLE B:%i T:%i\n", pid[i], tid[i], pid[j], tid[j]);
          iREAL midpt[3], normal[3];
          
          midpt[0] = (xPA+xPB)/2; //x
          midpt[1] = (yPA+yPB)/2; //y
          midpt[2] = (zPA+zPB)/2; //z
          
          iREAL depth = margin-dist;
          
          normal[0] = ((xPB - xPA)/depth);// depth for inclusion to normal
          normal[1] = ((yPB - yPA)/depth);
          normal[2] = ((zPB - zPA)/depth);
          
          int color[2], id[2];
          id[0] = pid[i];
          id[1] = pid[j];
          
          color[0] = 0;
          color[1] = 0;
          
          iREAL pp[3]; iREAL qq[3];
          pp[0] = xPA;
          pp[1] = yPA;
          pp[2] = zPA;
          
          qq[0] = xPB;
          qq[1] = yPB;
          qq[2] = zPB;
          
          contactpoint point(id, color, midpt, normal, depth, pp, qq);
          conpnt[pid[i]].push_back(point);
        }
      }
    }
  }
}
