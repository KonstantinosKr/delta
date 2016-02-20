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

#ifndef __macros__
#define __macros__

/* textual assertion */
#define ASSERT(__test__, ...)\
  do {\
  if (!(__test__)) { fprintf (stderr, "%s: %d => ", __FILE__, __LINE__);\
    fprintf (stderr, __VA_ARGS__);\
    fprintf (stderr, "\n"); exit (1); } } while (0)

/* memory validity assertion */
#define ERRMEM(__pointer__) ASSERT (__pointer__, "Out of memory!");

#define ALG_PI 3.14159265358979323846 
#define ALG_SQR2 1.4142135623730951

#if iREAL==float
#define REAL_MAX 3.40282347E+38F
#else
#define REAL_MAX 1.7976931348623157E+308
#endif

#ifndef MIN
  #define MIN(v, w) ((v) < (w) ? (v) : (w))
#endif

#ifndef MAX
  #define MAX(v, w) ((v) > (w) ? (v) : (w))
#endif

#define ADDMUL(a, eps, b, c)\
{\
  (c) [0] = (a) [0] + (eps)*(b) [0];\
  (c) [1] = (a) [1] + (eps)*(b) [1];\
  (c) [2] = (a) [2] + (eps)*(b) [2];\
}

#define SUB(a, b, c)\
{\
  (c) [0] = (a) [0] - (b) [0];\
  (c) [1] = (a) [1] - (b) [1];\
  (c) [2] = (a) [2] - (b) [2];\
}

#define DOT(a, b)\
  ((a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2])

#define LEN(a) (sqrt (DOT (a, a)))

#define PRODUCT(a, b, c)\
{\
  (c) [0] = (a) [1]*(b) [2] - (a) [2]*(b) [1];\
  (c) [1] = (a) [2]*(b) [0] - (a) [0]*(b) [2];\
  (c) [2] = (a) [0]*(b) [1] - (a) [1]*(b) [0];\
}

#define PRODUCTADD(a, b, c)\
{\
  (c) [0] += (a) [1]*(b) [2] - (a) [2]*(b) [1];\
  (c) [1] += (a) [2]*(b) [0] - (a) [0]*(b) [2];\
  (c) [2] += (a) [0]*(b) [1] - (a) [1]*(b) [0];\
}

#define PRODUCTSUB(a, b, c)\
{\
  (c) [0] -= (a) [1]*(b) [2] - (a) [2]*(b) [1];\
  (c) [1] -= (a) [2]*(b) [0] - (a) [0]*(b) [2];\
  (c) [2] -= (a) [0]*(b) [1] - (a) [1]*(b) [0];\
}

#define REVERSE(a, b)\
{\
  (b) [0] = -(a) [0];\
  (b) [1] = -(a) [1];\
  (b) [2] = -(a) [2];\
}

#define SCALE(a, b)\
{\
  (a) [0] *= b;\
  (a) [1] *= b;\
  (a) [2] *= b;\
}

#define ACC(a, b)\
{\
  (b) [0] += (a) [0];\
  (b) [1] += (a) [1];\
  (b) [2] += (a) [2];\
}

#define SCC(a, b)\
{\
  (b) [0] -= (a) [0];\
  (b) [1] -= (a) [1];\
  (b) [2] -= (a) [2];\
}

#define SET(a, b)\
{\
  (a) [0] = b;\
  (a) [1] = b;\
  (a) [2] = b;\
}

#define TNSUB(A, B, C)\
{\
  (C) [0] = (A)[0] - (B)[0];\
  (C) [1] = (A)[3] - (B)[1];\
  (C) [2] = (A)[6] - (B)[2];\
  (C) [3] = (A)[1] - (B)[3];\
  (C) [4] = (A)[4] - (B)[4];\
  (C) [5] = (A)[7] - (B)[5];\
  (C) [6] = (A)[2] - (B)[6];\
  (C) [7] = (A)[5] - (B)[7];\
  (C) [8] = (A)[8] - (B)[8];\
}

#define TNADDMUL(A, EPS, B, C)\
{\
  (C) [0] = (A)[0] + (EPS)*(B)[0];\
  (C) [1] = (A)[3] + (EPS)*(B)[1];\
  (C) [2] = (A)[6] + (EPS)*(B)[2];\
  (C) [3] = (A)[1] + (EPS)*(B)[3];\
  (C) [4] = (A)[4] + (EPS)*(B)[4];\
  (C) [5] = (A)[7] + (EPS)*(B)[5];\
  (C) [6] = (A)[2] + (EPS)*(B)[6];\
  (C) [7] = (A)[5] + (EPS)*(B)[7];\
  (C) [8] = (A)[8] + (EPS)*(B)[8];\
}

#define NNMUL(A, B, C)\
{\
 (C) [0] = (A)[0]*(B)[0]+(A)[3]*(B)[1]+(A)[6]*(B)[2];\
 (C) [1] = (A)[1]*(B)[0]+(A)[4]*(B)[1]+(A)[7]*(B)[2];\
 (C) [2] = (A)[2]*(B)[0]+(A)[5]*(B)[1]+(A)[8]*(B)[2];\
 (C) [3] = (A)[0]*(B)[3]+(A)[3]*(B)[4]+(A)[6]*(B)[5];\
 (C) [4] = (A)[1]*(B)[3]+(A)[4]*(B)[4]+(A)[7]*(B)[5];\
 (C) [5] = (A)[2]*(B)[3]+(A)[5]*(B)[4]+(A)[8]*(B)[5];\
 (C) [6] = (A)[0]*(B)[6]+(A)[3]*(B)[7]+(A)[6]*(B)[8];\
 (C) [7] = (A)[1]*(B)[6]+(A)[4]*(B)[7]+(A)[7]*(B)[8];\
 (C) [8] = (A)[2]*(B)[6]+(A)[5]*(B)[7]+(A)[8]*(B)[8];\
}

#define NVMUL(A, B, C)\
{\
 (C) [0] = (A)[0]*(B)[0]+(A)[3]*(B)[1]+(A)[6]*(B)[2];\
 (C) [1] = (A)[1]*(B)[0]+(A)[4]*(B)[1]+(A)[7]*(B)[2];\
 (C) [2] = (A)[2]*(B)[0]+(A)[5]*(B)[1]+(A)[8]*(B)[2];\
}

#define TVMUL(A, B, C)\
{\
 (C) [0] = (A)[0]*(B)[0]+(A)[1]*(B)[1]+(A)[2]*(B)[2];\
 (C) [1] = (A)[3]*(B)[0]+(A)[4]*(B)[1]+(A)[5]*(B)[2];\
 (C) [2] = (A)[6]*(B)[0]+(A)[7]*(B)[1]+(A)[8]*(B)[2];\
}

#define NVADDMUL(C, A, B, D)\
{\
 (D) [0] = (C)[0] + ((A)[0]*(B)[0]+(A)[3]*(B)[1]+(A)[6]*(B)[2]);\
 (D) [1] = (C)[1] + ((A)[1]*(B)[0]+(A)[4]*(B)[1]+(A)[7]*(B)[2]);\
 (D) [2] = (C)[2] + ((A)[2]*(B)[0]+(A)[5]*(B)[1]+(A)[8]*(B)[2]);\
}

#define TVADDMUL(C, A, B, D)\
{\
 (D) [0] = (C)[0] + ((A)[0]*(B)[0]+(A)[1]*(B)[1]+(A)[2]*(B)[2]);\
 (D) [1] = (C)[1] + ((A)[3]*(B)[0]+(A)[4]*(B)[1]+(A)[5]*(B)[2]);\
 (D) [2] = (C)[2] + ((A)[6]*(B)[0]+(A)[7]*(B)[1]+(A)[8]*(B)[2]);\
}

#define DET(F) ((F)[0]*((F)[4]*(F)[8]-(F)[5]*(F)[7])+\
                (F)[3]*((F)[2]*(F)[7]-(F)[1]*(F)[8])+\
                (F)[6]*((F)[1]*(F)[5]-(F)[2]*(F)[4]))

#define INVERT(F, INV, DET)\
if (((DET) =\
 ((F)[0]*((F)[4]*(F)[8]-(F)[5]*(F)[7])+\
  (F)[3]*((F)[2]*(F)[7]-(F)[1]*(F)[8])+\
 ((F)[1]*(F)[5]-(F)[2]*(F)[4])*(F)[6])) != 0.0)\
{\
  (DET) = 1.0 / (DET);\
  (INV) [0] = ((F)[4]*(F)[8]-(F)[5]*(F)[7])*(DET);\
  (INV) [1] = ((F)[2]*(F)[7]-(F)[1]*(F)[8])*(DET);\
  (INV) [2] = ((F)[1]*(F)[5]-(F)[2]*(F)[4])*(DET);\
  (INV) [3] = ((F)[5]*(F)[6]-(F)[3]*(F)[8])*(DET);\
  (INV) [4] = ((F)[0]*(F)[8]-(F)[2]*(F)[6])*(DET);\
  (INV) [5] = ((F)[2]*(F)[3]-(F)[0]*(F)[5])*(DET);\
  (INV) [6] = ((F)[3]*(F)[7]-(F)[4]*(F)[6])*(DET);\
  (INV) [7] = ((F)[1]*(F)[6]-(F)[0]*(F)[7])*(DET);\
  (INV) [8] = ((F)[0]*(F)[4]-(F)[1]*(F)[3])*(DET);\
  (DET) = 1.0 / (DET);\
}

#endif
