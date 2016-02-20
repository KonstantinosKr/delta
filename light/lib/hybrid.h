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

/**
 * Hybrid method for computation of the minimum distance between
 * triangles in 3D using the iterative penalty method initialy for a set 
 * number of batch (batch size can be tuned) and as fallback method based on an error
 * constant the brute force method is used. The method is executed using openmp level
 * balancing over threads.
 * 
 * @author Konstantinos Krestenitis
 *
 * @param it Start position for array of triangles
 * @param nt Total triangles in the array of triangles
 * @param a[3] Point A of triangle 1
 * @param b[3[ Point B of triangle 1
 * @param c[3] Point C of triangle 1
 * @param *d[3] Points D of triangle 2
 * @param *e[3] Points E of triangle 2
 * @param *f[3] Points F of triangle 2
 * @param *p[3] Points P on triangle 1 that define closest point on triangle 1
 * @param *q[3] Points Q on triangle 2 that define closest point on triangle 2
 *
 */
void hybrid (unsigned int it, unsigned int nt,
    iREAL a[3], iREAL b[3], iREAL c[3],
    iREAL *d[3], iREAL *e[3], iREAL *f[3],
    iREAL *p[3], iREAL *q[3]); 
