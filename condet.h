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

#ifndef __condet__
#define __condet__

#define CONBUF 8 /* TODO: tune contact buffer size */

/* master contact points; global array is used because
 * constitutive data at contacts can be persistent */
struct master_conpnt
{
  uniform int master[CONBUF]; /* ellipsoid */
  uniform int slave[2][CONBUF]; /* particle, ellipsoid or -(triangle+1), unused */
  uniform int color[2][CONBUF];
  uniform iREAL point[3][CONBUF];
  uniform iREAL normal[3][CONBUF];
  uniform iREAL depth[CONBUF];
  uniform iREAL force[3][CONBUF];
  uniform int size;

  uniform master_conpnt * uniform next; /* local list */
  uniform int lock; /* list update lock */
};

/* slave contact points; they are created by
 * symmetrically coppying master contact points */
struct slave_conpnt
{
  uniform int master[2][CONBUF]; /* particle, ellipsoid */
  uniform iREAL point[3][CONBUF];
  uniform iREAL force[3][CONBUF];
  uniform int size;

  uniform slave_conpnt * uniform next; /* local list */
  uniform int lock; /* list update lock */
};

#endif
