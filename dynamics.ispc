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

#include "condet.h"
#include "algo.h"

/* dynamics task */
task void dynamics_task (uniform int span, uniform master_conpnt master[], uniform slave_conpnt slave[],
  uniform int parnum, uniform iREAL * uniform angular[6], uniform iREAL * uniform linear[3],
  uniform iREAL * uniform rotation[9], uniform iREAL * uniform position[6],
  uniform iREAL * uniform inertia[9], uniform iREAL * uniform inverse[9],
  uniform iREAL mass[], uniform iREAL invm[], uniform iREAL * uniform force[3],
  uniform iREAL * uniform torque[3], uniform iREAL gravity[3], uniform iREAL step)
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? parnum: start+span;
  uniform iREAL half = 0.5*step;

  for (uniform int i = start; i < end; i ++) /* force accumulation */
  {
    iREAL f[3], a[3], fs[3], ts[3];
    uniform iREAL po[3], ma;

    po[0] = position[0][i];
    po[1] = position[1][i];
    po[2] = position[2][i];

    ma = mass[i];

    SET (fs, 0.0);
    SET (ts, 0.0);

    for (uniform master_conpnt * uniform m = &master[i]; m; m = m->next)
    {
      foreach (j = 0 ... m->size)
      {
	f[0] = m->force[0][j];
	f[1] = m->force[1][j];
	f[2] = m->force[2][j];

	a[0] = m->point[0][j]-po[0];
	a[1] = m->point[1][j]-po[1];
	a[2] = m->point[2][j]-po[2];

	ACC (f, fs);
	PRODUCTADD (a, f, ts);
      }
    }

    for (uniform slave_conpnt * uniform s = &slave[i]; s; s = s->next)
    {
      foreach (j = 0 ... s->size)
      {
	f[0] = s->force[0][j];
	f[1] = s->force[1][j];
	f[2] = s->force[2][j];

	a[0] = s->point[0][j]-po[0];
	a[1] = s->point[1][j]-po[1];
	a[2] = s->point[2][j]-po[2];

	ACC (f, fs);
	PRODUCTADD (a, f, ts);
      }
    }

    force[0][i] = reduce_add (fs[0]) + ma * gravity[0];
    force[1][i] = reduce_add (fs[1]) + ma * gravity[1];
    force[2][i] = reduce_add (fs[2]) + ma * gravity[2];

    torque[0][i] = reduce_add (ts[0]);
    torque[1][i] = reduce_add (ts[1]);
    torque[2][i] = reduce_add (ts[2]);
  }

  foreach (i = start ... end) /* time integration */
  {
    iREAL O[3], o[3], v[3], L1[9], J[9], I[9], im, f[3], t[3], T[3], DL[9], L2[9], A[3], B[3];

    O[0] = angular[0][i];
    O[1] = angular[1][i];
    O[2] = angular[2][i];

    v[0] = linear[0][i];
    v[1] = linear[1][i];
    v[2] = linear[2][i];

    L1[0] = rotation[0][i];
    L1[1] = rotation[1][i];
    L1[2] = rotation[2][i];
    L1[3] = rotation[3][i];
    L1[4] = rotation[4][i];
    L1[5] = rotation[5][i];
    L1[6] = rotation[6][i];
    L1[7] = rotation[7][i];
    L1[8] = rotation[8][i];

    J[0] = inertia[0][i];
    J[1] = inertia[1][i];
    J[2] = inertia[2][i];
    J[3] = inertia[3][i];
    J[4] = inertia[4][i];
    J[5] = inertia[5][i];
    J[6] = inertia[6][i];
    J[7] = inertia[7][i];
    J[8] = inertia[8][i];

    I[0] = inverse[0][i];
    I[1] = inverse[1][i];
    I[2] = inverse[2][i];
    I[3] = inverse[3][i];
    I[4] = inverse[4][i];
    I[5] = inverse[5][i];
    I[6] = inverse[6][i];
    I[7] = inverse[7][i];
    I[8] = inverse[8][i];

    im = invm[i];

    f[0] = force[0][i];
    f[1] = force[1][i];
    f[2] = force[2][i];

    t[0] = torque[0][i];
    t[1] = torque[1][i];
    t[2] = torque[2][i];

    TVMUL (L1, t, T);

    expmap (-half*O[0], -half*O[1], -half*O[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);

    NVMUL (J, O, A);
    NVMUL (DL, A, B);
    ADDMUL (B, half, T, B);
    NVMUL (I, B, A); /* O(t+h/2) */

    NVMUL (J, A, B);
    PRODUCTSUB (A, B, T); /* T - O(t+h/2) x J O(t+h/2) */

    SCALE (T, step);
    NVADDMUL (O, I, T, O); /* O(t+h) */

    im *= step;
    ADDMUL (v, im, f, v); /* v(t+h) */

    expmap (step*O[0], step*O[1], step*O[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);

    NNMUL (L1, DL, L2);

    rotation[0][i] = L2[0];
    rotation[1][i] = L2[1];
    rotation[2][i] = L2[2];
    rotation[3][i] = L2[3];
    rotation[4][i] = L2[4];
    rotation[5][i] = L2[5];
    rotation[6][i] = L2[6];
    rotation[7][i] = L2[7];
    rotation[8][i] = L2[8];

    NVMUL (L2, O, o);

    position[0][i] += step*v[0];
    position[1][i] += step*v[1];
    position[2][i] += step*v[2];

    angular[0][i] = O[0];
    angular[1][i] = O[1];
    angular[2][i] = O[2];
    angular[3][i] = o[0];
    angular[4][i] = o[1];
    angular[5][i] = o[2];

    linear[0][i] = v[0];
    linear[1][i] = v[1];
    linear[2][i] = v[2];
  }
}

/* update dynamics */
export void dynamics (uniform int threads, uniform master_conpnt master[], uniform slave_conpnt slave[],
  uniform int parnum, uniform iREAL * uniform angular[6], uniform iREAL * uniform linear[3],
  uniform iREAL * uniform rotation[9], uniform iREAL * uniform position[6],
  uniform iREAL * uniform inertia[9], uniform iREAL * uniform inverse[9],
  uniform iREAL mass[], uniform iREAL invm[], uniform iREAL * uniform force[3],
  uniform iREAL * uniform torque[3], uniform iREAL gravity[3], uniform iREAL step)
{
  launch [threads] dynamics_task (parnum/threads, master, slave, parnum, angular, linear,
          rotation, position, inertia, inverse, mass, invm, force, torque, gravity, step);
}
