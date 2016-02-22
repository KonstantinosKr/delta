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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "dynamics.h"
#include <time.h>
#include <math.h>
#include "geom.h"

extern "C"{
#include "ext/hul/hul.h"
}

void init_enviroment(int &nt, int &nb, iREAL *t[6][3], iREAL *linear[3], iREAL *angular[6], 
                      iREAL *inertia[9], iREAL *inverse[9], iREAL *rotation[9], 
                      iREAL *mass, int *parmat, int tid[], int pid[], iREAL *position[6], iREAL lo[3], iREAL hi[3]);

void nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, int &nt, int bodyidx, int initidx, 
                        iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void load_vtk(int &nt, int nb, int IDX, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void load_enviroment(int ptype[], int &nt, int nb, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt);

void normalize(int nt, iREAL *t[6][3], iREAL mint, iREAL maxt); 

// invert inertia properties 
void invert (int nt, iREAL * inertia[9], iREAL * inverse[9], iREAL mass[]);

