/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Tomasz Koziara
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * alg.c: basic operations on scalars, vectors, matrices ...
 */

#include "alg.h"

double GEOMETRIC_EPSILON = 1E-6;

/* lexicographical point comparison accounting for the GEOMETRIC_EPSILON */
int POINTS_COMPARE (double *a, double *b)
{
  if (LT (a[0], b[0])) return -1;
  else if (EQ (a[0], b[0]))
  {
    if (LT (a[1], b[1])) return -1;
    else if (EQ (a[1], b[1]))
    {
      if (LT (a[2], b[2])) return -1;
      else if (EQ (a[2], b[2])) return 0;
    }
  }

  return 1;
}
