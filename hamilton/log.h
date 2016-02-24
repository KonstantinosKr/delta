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

//LOG DATA HEADER
void log(int optionA, int optionB, struct loba *lb, int myrank, 
              int nt, int nb, iREAL *t[6][3], int timesteps,
              int pid[], int tid[], iREAL gravity[3], 
              iREAL force[], iREAL torque[], iREAL p[3], iREAL q[3],
              iREAL linear[3], iREAL angular[6], iREAL position[6], iREAL mass[], 
              iREAL tTimer1[], iREAL tTimer2[], 
              iREAL tTimer3[], iREAL tTimer4[], 
              int num_import, int num_export);

void performance_log(iREAL minsubtotal, iREAL maxsubtotal, iREAL avgsubtotal, 
                      iREAL minbal, iREAL maxbal, iREAL avgbal, 
                      iREAL minmig, iREAL maxmig, iREAL avgmig, 
                      iREAL minde, iREAL maxde, iREAL avgde, 
                      iREAL mindt1, iREAL maxdt1, iREAL avgdt1, 
                      iREAL mindt2, iREAL maxdt2, iREAL avgdt2, 
                      iREAL mindt3, iREAL maxdt3, iREAL avgdt3); 

void log_balance();

void log_migration();

void log_contact();

void log_ghost();

void log_force();

void log_dynamic();

void log_statistics();


