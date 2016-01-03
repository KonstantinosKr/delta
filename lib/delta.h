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

#include "bf.h"
#include "penalty.h"
#include "hybrid.h"

/**
 * Delta library
 * 
 * Delta is a library for contact detection & dem simulation using triangular elements. 
 * The contact detection is executed by calling the delta method by 
 * providing a epsilon parameter that defines the overlapping margin between the particle surface.
 * The contact detection requires an all to all comparison of the particles O(n^2) thus for convinience 
 * two ranges can be specified to operate on the one long vectorized array of triangle points.
 *  
 * @author Konstantinos Krestenitis
 *
 * @param method  Which distance calculation shall be used (0=brute force, 1=penalty, 2=hybrid)
 * @param epsilon Parameter for distance detection, margin layer distance
 * @param s1 Parameter for starting range position 1 for array t
 * @param e1 Parameter for ending range position 1 for array t
 * @param s2 Parameter for starting range position 2 for array t
 * @param e2 Parameter for ending range position 2 for array t
 * @param t Vectorizable array of triangle point A, B, C; i.e. t[A][x][i]... 
 */
void delta (int method, iREAL epsilon,
            unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2, 
            iREAL *t[3][3], unsigned long long int *ncontacts);
