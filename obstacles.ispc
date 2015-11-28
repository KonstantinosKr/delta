/*
The MIT License (MIT)

Copyright (c) 2015 Tomasz Koziara

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

#include "algo.h"

/* update obstacles */
export void obstacles (uniform int threads, uniform int obsnum, uniform int trirng[],
  uniform iREAL point[], uniform iREAL angular[], uniform iREAL linear[],
  uniform int trinum, uniform iREAL * uniform tri[3][3], uniform iREAL step)
{
  uniform iREAL DL[9], y[3];
  uniform iREAL * uniform x = point,
                * uniform o = angular,
		* uniform v = linear;

  for (uniform int i = 0; i < obsnum; i ++, x += 3, o += 6, v += 6)
  {
    y[0] = x[0] + step*v[0];
    y[1] = x[1] + step*v[1];
    y[2] = x[2] + step*v[2];

    expmap (step*o[0], step*o[1], step*o[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);

    foreach (j = trirng[2*i] ... trirng[2*i+1])
    {
      iREAL z[3];

      z[0] = tri[0][0][j] - x[0];
      z[1] = tri[0][1][j] - x[1];
      z[2] = tri[0][2][j] - x[2];

      tri[0][0][j] = DL[0]*z[0]+DL[3]*z[1]+DL[6]*z[2] + y[0];
      tri[0][1][j] = DL[1]*z[0]+DL[4]*z[1]+DL[7]*z[2] + y[1];
      tri[0][2][j] = DL[2]*z[0]+DL[5]*z[1]+DL[8]*z[2] + y[2];

      z[0] = tri[1][0][j] - x[0];
      z[1] = tri[1][1][j] - x[1];
      z[2] = tri[1][2][j] - x[2];

      tri[1][0][j] = DL[0]*z[0]+DL[3]*z[1]+DL[6]*z[2] + y[0];
      tri[1][1][j] = DL[1]*z[0]+DL[4]*z[1]+DL[7]*z[2] + y[1];
      tri[1][2][j] = DL[2]*z[0]+DL[5]*z[1]+DL[8]*z[2] + y[2];

      z[0] = tri[2][0][j] - x[0];
      z[1] = tri[2][1][j] - x[1];
      z[2] = tri[2][2][j] - x[2];

      tri[2][0][j] = DL[0]*z[0]+DL[3]*z[1]+DL[6]*z[2] + y[0];
      tri[2][1][j] = DL[1]*z[0]+DL[4]*z[1]+DL[7]*z[2] + y[1];
      tri[2][2][j] = DL[2]*z[0]+DL[5]*z[1]+DL[8]*z[2] + y[2];
    }

    x[0] = y[0];
    x[1] = y[1];
    x[2] = y[2];
  }
}
