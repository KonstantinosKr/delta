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

#ifndef HEADER_FILE
#define HEADER_FILE

#include "collision/bf.h"
#include "algo.h"
#include "stdio.h"
#include <vector>

struct contactpoint {
  int pid[2];
  int color[2];
  iREAL point[3];
  iREAL normal[3];
  iREAL depth;
  
  iREAL pp[3];
  iREAL qq[3];
  contactpoint(int pid[2], int color[2], iREAL point[3], iREAL normal[3], iREAL depth, iREAL p[3], iREAL q[3]);
};

namespace contact
{
	void detection (int s, int e, iREAL *t[6][3], int tid[], int pid[], std::vector<contactpoint> conpnt[]);
}
#endif
