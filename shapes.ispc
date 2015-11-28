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

/* shapes task */
task void shapes_task (uniform int span, uniform int ellnum, uniform int part[],
  uniform iREAL * uniform center[6], uniform iREAL * uniform radii[3], uniform iREAL * uniform orient[18],
  uniform iREAL * uniform rotation[9], uniform iREAL * uniform position[6])
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? ellnum : start+span;

  foreach (i = start ... end)
  {
    iREAL L[9], X[3], x[3], C[3], c[3];

    int j = part[i];

    L[0] = rotation[0][j];
    L[1] = rotation[1][j];
    L[2] = rotation[2][j];
    L[3] = rotation[3][j];
    L[4] = rotation[4][j];
    L[5] = rotation[5][j];
    L[6] = rotation[6][j];
    L[7] = rotation[7][j];
    L[8] = rotation[8][j];

    X[0] = position[3][j];
    X[1] = position[4][j];
    X[2] = position[5][j];

    x[0] = position[0][j];
    x[1] = position[1][j];
    x[2] = position[2][j];

    C[0] = center[3][i];
    C[1] = center[4][i];
    C[2] = center[5][i];

    SCC (X, C);
    NVADDMUL (x, L, C, c);

    center[0][i] = c[0];
    center[1][i] = c[1];
    center[2][i] = c[2];

    if (radii[1][j] > 0.) /* ellipsoid */
    {
      iREAL O[9], o[9];

      O[0] = orient[9][i];
      O[1] = orient[10][i];
      O[2] = orient[11][i];
      O[3] = orient[12][i];
      O[4] = orient[13][i];
      O[5] = orient[14][i];
      O[6] = orient[15][i];
      O[7] = orient[16][i];
      O[8] = orient[17][i];

      NNMUL (L, O, o);

      orient[0][i] = o[0];
      orient[1][i] = o[1];
      orient[2][i] = o[2];
      orient[3][i] = o[3];
      orient[4][i] = o[4];
      orient[5][i] = o[5];
      orient[6][i] = o[6];
      orient[7][i] = o[7];
      orient[8][i] = o[8];
    }
  }
}

/* update shapes */
export void shapes (uniform int threads, uniform int ellnum, uniform int part[],
  uniform iREAL * uniform center[6], uniform iREAL * uniform radii[3], uniform iREAL * uniform orient[18],
  uniform iREAL * uniform rotation[9], uniform iREAL * uniform position[6])
{
  launch [threads] shapes_task (ellnum/threads, ellnum, part, center, radii, orient, rotation, position);
}
