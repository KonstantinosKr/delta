/*
 The MIT License (MIT)
 
 Copyright (c) 2015 Konstantinos Krestenitis
 
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

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "delta.h"

//method 0 = bf
//method 1 = penalty
//method 2 = hybrid
//s1 and e1 mean start of section 1 and end of section 1, same for s2,e2 and nt size nts1, nts2
void delta (int method, iREAL epsilon, 
            unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2, 
            iREAL *t[3][3], unsigned long long int *ncontacts)
{
  *ncontacts = 0;//use as counter
  iREAL a[3], b[3], c[3];
  
  iREAL *p[3], *q[3];
  for(int i = 0; i<3; i++)
  {
    p[i] = (iREAL *) malloc ((e2-s2)*sizeof(iREAL));
    q[i] = (iREAL *) malloc ((e2-s2)*sizeof(iREAL));
  }

  //Set triangle 1 points A,B,C
  //#pragma force inline recursive
  //#pragma omp parallel for schedule(dynamic,1) collapse(2)
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
    
    if(method == 0)
    {
      bf (s2, e2, a, b, c, t[0], t[1], t[2], p, q);
    }
    else if(method == 1)
    {
      penalty (s2, e2, a, b, c, t[0], t[1], t[2], p, q);
    }
    else if(method == 2)
    {
      hybrid (s2, e2, a, b, c, t[0], t[1], t[2], p, q);
    }
    for(unsigned int j=s2;j<e2;j++) 
    {
      iREAL dist = sqrt(pow((q[0][j]-p[0][j]),2)+pow((q[1][j]-p[1][j]),2)+pow((q[2][j]-p[1][j]),2));
      
      if(dist < epsilon*2)
      {
        iREAL midpt[3];
        midpt[0] = (p[0][j]+q[0][j])/2; //x
        midpt[1] = (p[1][j]+q[1][j])/2; //y
        midpt[2] = (p[2][j]+q[2][j])/2; //z
                  
        iREAL depth = (epsilon*2)-dist;
        iREAL mul = sqrt(pow(q[0][j]-p[0][j], 2)+pow(q[1][j] - p[1][j], 2)+pow(q[2][j]-p[2][j], 2));
        iREAL normal[3];
        normal[0] = ((q[0][j] - p[0][j])/mul)*depth;
        normal[1] = ((q[1][j] - p[1][j])/mul)*depth;
        normal[2] = ((q[2][j] - p[2][j])/mul)*depth;
        *ncontacts++;
      }
    }
  }

  for(int i = 0; i<3; i++)
  {
    free(p[i]);
    free(q[i]);
  }
}

