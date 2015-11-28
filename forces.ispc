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

#include "material.h"
#include "condet.h"
#include "algo.h"

/* clear slave contact points */
task void clear_slaves (uniform int span, uniform slave_conpnt slave[], uniform int parnum)
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? parnum: start+span;

  for (uniform int i = start; i < end; i ++)
  {
    uniform slave_conpnt * uniform ptr = slave[i].next;
    while (ptr)
    {
      uniform slave_conpnt * uniform next = ptr->next;
      delete ptr;
      ptr = next;
    }

    slave[i].size = 0;
    slave[i].next = NULL;
  }
}

/* allocate new slave contact point that can be written to */
static uniform slave_conpnt * uniform newcon (uniform slave_conpnt * uniform slave, uniform int taskindex, uniform int *k)
{
  while (slave->lock != taskindex)
    atomic_compare_exchange_global (&slave->lock, -1, taskindex); /* lock access */

  uniform slave_conpnt * uniform con = slave;

  while (con->size == CONBUF) con = con->next; /* rewind to the end */

  *k = con->size ++;

  if (con->size == CONBUF)
  {
    uniform slave_conpnt * uniform ptr = uniform new uniform slave_conpnt;
    ptr->size = 0;
    ptr->next = NULL;
    con->next = ptr; /* append new item at the end */
  }

  slave->lock = -1; /* unlock */

  return con;
}

/* return pairing index based on (i,j) pairing of colors */
static inline int pairing (uniform int nummat, uniform int pairs[], int i, int j)
{
  int p[2] = {i, j}, start = 1, end = nummat;

  if (i > j)
  {
    p[0] = j;
    p[1] = i;
  }

  while (start < end)
  {
    int mid = ((end-start)>>1) + start;
    if (p[0] > pairs[2*mid]) start = mid;
    else if(p[0] == pairs[2*mid])
    {
      if (p[1] > pairs[2*mid+1]) start = mid;
      else if (p[1] == pairs[2*mid+1]) return mid;
      else end = mid;
    }
    else end = mid;
  }

  return 0; /* default material */
}

/* granular material force */
inline static int granural_force (iREAL n[3], iREAL vij[3], iREAL oij[3], iREAL depth, uniform int i, int j,
  uniform iREAL mass[], uniform iREAL invm[], uniform iREAL * uniform iparam[NINT], int ij, iREAL f[3])
{
  iREAL ma;

  if (j >= 0) ma = 1.0 / (invm[i] + invm[j]);
  else ma = mass[i];

  iREAL kn = iparam[SPRING][ij];
  iREAL en = iparam[DAMPER][ij] * 2.0 * sqrt(kn*ma);
  iREAL vn = DOT(vij,n);
  iREAL fn = kn*depth + en*vn;

  f[0] = fn*n[0];
  f[1] = fn*n[1];
  f[2] = fn*n[2];

  /* TODO */

  return depth < 0.0 ? 1 : 0;
}

/* forces task */
task void forces_task (uniform int span, uniform master_conpnt master[], uniform slave_conpnt slave[],
  uniform int parnum, uniform iREAL * uniform angular[6], uniform iREAL * uniform linear[3],
  uniform iREAL * uniform rotation[9], uniform iREAL * uniform position[3], uniform iREAL * uniform inertia[9],
  uniform iREAL * uniform inverse[9], uniform iREAL mass[], uniform iREAL invm[], uniform iREAL obspnt[],
  uniform iREAL obslin[], uniform iREAL obsang[], uniform int parmat[], uniform iREAL * uniform mparam[NMAT],
  uniform int pairnum, uniform int pairs[], uniform int ikind[], uniform iREAL * uniform iparam[NINT])
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? parnum: start+span;

  for (uniform int i = start; i < end; i ++)
  {
    uniform iREAL oi[3], v[3], x[3];

    oi[0] = angular[3][i];
    oi[1] = angular[4][i];
    oi[2] = angular[5][i];

    v[0] = linear[0][i];
    v[1] = linear[1][i];
    v[2] = linear[2][i];

    x[0] = position[0][i];
    x[1] = position[1][i];
    x[2] = position[2][i];

    /* update contact forces */
    for (uniform master_conpnt * uniform con = &master[i]; con; con = con->next)
    {
      uniform int gone[CONBUF];

      foreach (k = 0 ... con->size)
      {
	iREAL p[3], n[3], z[3], vi[3], vj[3], oj[3], vij[3], oij[3];

	p[0] = con->point[0][k];
	p[1] = con->point[1][k];
	p[2] = con->point[2][k];

	n[0] = con->normal[0][k];
	n[1] = con->normal[1][k];
	n[2] = con->normal[2][k];

	z[0] = p[0]-x[0];
	z[1] = p[1]-x[1];
	z[2] = p[2]-x[2];

	vi[0] = oi[1]*z[2]-oi[2]*z[1] + v[0];
	vi[1] = oi[2]*z[0]-oi[0]*z[2] + v[1];
	vi[2] = oi[0]*z[1]-oi[1]*z[0] + v[2];

	int j = con->slave[0][k];

	if (j >= 0) /* particle-particle */
	{
	  z[0] = p[0]-position[0][j];
	  z[1] = p[1]-position[1][j];
	  z[2] = p[2]-position[2][j];

	  oj[0] = angular[3][j];
	  oj[1] = angular[4][j];
	  oj[2] = angular[5][j];

	  vj[0] = oj[1]*z[2]-oj[2]*z[1] + linear[0][j];
	  vj[1] = oj[2]*z[0]-oj[0]*z[2] + linear[1][j];
	  vj[2] = oj[0]*z[1]-oj[1]*z[0] + linear[2][j];
	}
	else /* particle-obstacle */
	{
	  int l = -j-2; /* obstacle index, see condet.ispc:drop_triangle and input.cpp:OBSTACLE */

	  if (l >= 0) /* moving obstacle */
	  {
	    z[0] = p[0]-obspnt[3*l];
	    z[1] = p[1]-obspnt[3*l+1];
	    z[2] = p[2]-obspnt[3*l+2];

	    oj[0] = obsang[3*l];
	    oj[1] = obsang[3*l+1];
	    oj[2] = obsang[3*l+2];

	    vj[0] = oj[1]*z[2]-oj[2]*z[1] + obslin[3*l];
	    vj[1] = oj[2]*z[0]-oj[0]*z[2] + obslin[3*l+1];
	    vj[2] = oj[0]*z[1]-oj[1]*z[0] + obslin[3*l+2];
	  }
	  else /* static boundary */
	  {
	    SET (oj, 0.0);
	    SET (vj, 0.0);
	  }
	}

	SUB (vj, vi, vij); /* relative linear velocity */
	SUB (oj, oi, oij); /* relative angular velocity */

	int ij = pairing (pairnum, pairs, con->color[0][k], con->color[1][k]);

	iREAL f[3];

	switch (ikind[ij])
	{
	case GRANULAR:
	  gone[k] = granural_force (n, vij, oij, con->depth[k], i, j, mass, invm, iparam, ij, f);
	break;
	case BONDED:
	  assert (0);
	  /* TODO */
	break;
	case UFORCE:
	  assert (0);
	  /* TODO */
	break;
	default:
	  print ("ERROR: invalid pairing kind");
	  assert (0);
	break;
	}

	con->force[0][k] = f[0];
	con->force[1][k] = f[1];
	con->force[2][k] = f[2];
      }

      uniform int ngone = 0;

      for (uniform int k = 0; k < con->size; k ++)
      {
	if (gone[k] != 0)
	{
	  uniform int j = k+1;

	  while (j < con->size && gone[j] != 0) j ++;

	  if (j < con->size)
	  {
	    con->master[k] = con->master[j];
	    con->slave[0][k] = con->slave[0][j];
	    con->slave[1][k] = con->slave[1][j];
	    con->color[0][k] = con->color[0][j];
	    con->color[1][k] = con->color[1][j];
	    con->point[0][k] = con->point[0][j];
	    con->point[1][k] = con->point[1][j];
	    con->point[2][k] = con->point[2][j];
	    con->normal[0][k] = con->normal[0][j];
	    con->normal[1][k] = con->normal[1][j];
	    con->normal[2][k] = con->normal[2][j];
	    con->depth[k] = con->depth[j];
	    con->force[0][k] = con->force[0][j];
	    con->force[1][k] = con->force[1][j];
	    con->force[2][k] = con->force[2][j];

	    gone[j] = -1; /* not to be used again */
	  }

	  if (gone[k] > 0) ngone ++;
	}
      }

      con->size -= ngone;
    }

    uniform master_conpnt * uniform con = master[i].next;

    while (con && con->next) /* delete empty items */
    {
      uniform master_conpnt * uniform next = con->next;

      if (next->size == 0)
      {
	con->next = next->next;

	delete next;
      }

      con = con->next;
    }

    /* symmetrical copy into slave contact points */
    for (uniform master_conpnt * uniform con = &master[i]; con; con = con->next)
    {
      for (uniform int j = 0; j < con->size; j ++)
      {
	uniform slave_conpnt *ptr;
	uniform int k;

	if (con->slave[0][j] >= 0) /* particle-particle contact */
	{
	  ptr = newcon (&slave[con->slave[0][j]], taskIndex, &k);

	  ptr->master[0][k] = i;
	  ptr->master[1][k] = con->master[j];
	  ptr->point[0][k] = con->point[0][j];
	  ptr->point[1][k] = con->point[1][j];
	  ptr->point[2][k] = con->point[2][j];
	  ptr->force[0][k] = -con->force[0][j];
	  ptr->force[1][k] = -con->force[1][j];
	  ptr->force[2][k] = -con->force[2][j];
	}
      }
    }
  }
}

/* update forces */
export void forces (uniform int threads, uniform master_conpnt master[], uniform slave_conpnt slave[],
  uniform int parnum, uniform iREAL * uniform angular[6], uniform iREAL * uniform linear[3],
  uniform iREAL * uniform rotation[9], uniform iREAL * uniform position[3], uniform iREAL * uniform inertia[9],
  uniform iREAL * uniform inverse[9], uniform iREAL mass[], uniform iREAL invm[], uniform iREAL obspnt[],
  uniform iREAL obslin[], uniform iREAL obsang[], uniform int parmat[], uniform iREAL * uniform mparam[NMAT],
  uniform int pairnum, uniform int pairs[], uniform int ikind[], uniform iREAL * uniform iparam[NINT])
{
  launch [threads] clear_slaves (parnum/threads, slave, parnum);
  sync;

  launch [threads] forces_task (parnum/threads, master, slave, parnum, angular, linear, rotation, position, inertia, inverse,
                                mass, invm, obspnt, obslin, obsang, parmat, mparam, pairnum, pairs, ikind, iparam);
}
