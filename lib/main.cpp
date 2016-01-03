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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "delta.h"

int main (int argc, char **argv)
{
  iREAL *t[3][3]; /* triangles */
  unsigned int nt = 2700; /* memory buffer size */
   
  for (int i = 0; i < 3; i ++)
  { 
    t[0][i] = (iREAL *) malloc (nt*sizeof(iREAL));
    t[1][i] = (iREAL *) malloc (nt*sizeof(iREAL));
    t[2][i] = (iREAL *) malloc (nt*sizeof(iREAL));
  }
 
  for(unsigned int i = 0; i<nt;i++)
  {
    for(int j = 0; j<3; j++)
    {
      t[0][j][i] = 0;
      t[1][j][i] = 0;
      t[2][j][i] = 0;
    }
  }

  unsigned long long int ncontacts = 0;
  iREAL margin = 1E-3;

  delta(2, margin, 0, nt, 0, nt, t, &ncontacts);

  return 0;
}
