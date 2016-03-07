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

#include <stdlib.h>
#include "contact.h"
#include "algo.h"
#include "math.h"
#include "material.h"
#include "loba.h"
#include "migration.h"

void integrate (iREAL step, iREAL lo[3], iREAL hi[3], int nt, iREAL * t[3][3], iREAL * v[3]);

void euler(int nb, iREAL * angular[6], iREAL * linear[3], iREAL * rotation[9], iREAL * position[6], iREAL step);

void dynamics (struct loba *lb, int myrank, std::vector<contact> conpnt[],
              int nt, int nb, iREAL *t[6][3], int pid[], iREAL *angular[6], iREAL *linear[3],
              iREAL *rotation[9], iREAL *position[6],
              iREAL *inertia[9], iREAL *inverse[9],
              iREAL mass[], iREAL *force[3],
              iREAL *torque[3], iREAL step, iREAL lo[3], iREAL hi[3]);

void expmap (iREAL Omega1, iREAL Omega2, iREAL Omega3,
             iREAL &Lambda1, iREAL &Lambda2, iREAL &Lambda3,
			          iREAL &Lambda4, iREAL &Lambda5, iREAL &Lambda6,
			          iREAL &Lambda7, iREAL &Lambda8, iREAL &Lambda9);
