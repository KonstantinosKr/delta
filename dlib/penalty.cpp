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

#include "penalty.h"

void penalty (unsigned int it, unsigned int nt,
              iREAL a[3], iREAL b[3], iREAL c[3],
              iREAL * d[3], iREAL * e[3], iREAL * f[3],
              iREAL * p[3], iREAL * q[3])
{
  for(unsigned int i = it; i<nt; i++)
  {
    iREAL A[3], B[3], C[3], D[3], E[3], F[3], P[3], Q[3];
    
    A[0] = a[0];
    A[1] = a[1];
    A[2] = a[2];

    B[0] = b[0];
    B[1] = b[1];
    B[2] = b[2];

    C[0] = c[0];
    C[1] = c[1];
    C[2] = c[2];

    D[0] = d[0][i];
    D[1] = d[1][i];
    D[2] = d[2][i];

    E[0] = e[0][i];
    E[1] = e[1][i];
    E[2] = e[2][i];

    F[0] = f[0][i];
    F[1] = f[1][i];
    F[2] = f[2][i];


    iREAL BA[3], CA[3], ED[3], FD[3], hessian[16], x[4];
    
    SUB(B,A, BA);
    SUB(C,A, CA);
    SUB(E,D, ED);
    SUB(F,D, FD);
    
    hessian[0] = 2.*DOT(BA,BA);
    hessian[1] = 2.*DOT(CA,BA);
    hessian[2] = -2.*DOT(ED,BA);
    hessian[3] = -2.*DOT(FD,BA);
    
    hessian[4] = hessian[1]; //use symmetry
    hessian[5] = 2.*DOT(CA,CA);
    hessian[6] = -2.*DOT(ED,CA);
    hessian[7] = -2.*DOT(FD,CA);
    
    hessian[8] = hessian[2];
    hessian[9] = hessian[6];
    hessian[10] = 2.*DOT(ED,ED);
    hessian[11] = 2.*DOT(FD,ED);
    
    hessian[12] = hessian[3];
    hessian[13] = hessian[7];
    hessian[14] = hessian[11];
    hessian[15] = 2.*DOT(FD,FD);
    
    iREAL eps = 0.1;
    iREAL delta = (hessian[0]+hessian[5]+hessian[10]+hessian[15]) * eps;
    iREAL lambda = sqrt(0.0125*(hessian[0]+hessian[5]+hessian[10]+hessian[15]));
    iREAL r = lambda*pow(10,log(lambda)+14);
    
    //initial guess
    x[0] = 0.33;
    x[1] = 0.33;
    x[2] = 0.33;
    x[3] = 0.33;
    
    //Declare loop variables;
    iREAL dx[4], a[16], SUBXY[3], b[4], mx[6], dh[8], tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;
    
    //Newton loop
    for(int j=0;j<5;j++)
    {
        dh[0] = dh[2] = dh[4] = dh[6] = -1;
        dh[1] = dh[3] = dh[5] = dh[7] = 1;
        
        if(-x[0] <= 0)
        {
            dh[0] = mx[0] = 0;
        }else
        {
            mx[0] = -x[0];
        }
        if(-x[1] <= 0)
        {
            dh[2] = mx[1] = 0;
        }else
        {
            mx[1] = -x[1];
        }
        if(x[0]+x[1]-1 <= 0)
        {
            dh[1] =  dh[3] = mx[2] = 0;
        }else
        {
            mx[2] = x[0]+x[1]-1;
        }
        if(-x[2] <= 0)
        {
            dh[4] = mx[3] = 0;
        }else
        {
            mx[3] = -x[2];
        }
        if(-x[3] <= 0)
        {
            dh[6] = mx[4] = 0;
        }else
        {
            mx[4] = -x[3];
        }
        if(x[2]+x[3]-1 <= 0)
        {
            dh[5] = dh[7] = 0;
            
            mx[5] = 0;
        }else
        {
            mx[5] = x[2]+x[3]-1;
        }
        
        delta = i < 4 ? delta : r*delta;
        
        SUBXY[0] = (A[0]+(BA[0] * x[0])+(CA[0] * x[1])) - (D[0]+(ED[0] * x[2])+(FD[0] * x[3]));
        SUBXY[1] = (A[1]+(BA[1] * x[0])+(CA[1] * x[1])) - (D[1]+(ED[1] * x[2])+(FD[1] * x[3]));
        SUBXY[2] = (A[2]+(BA[2] * x[0])+(CA[2] * x[1])) - (D[2]+(ED[2] * x[2])+(FD[2] * x[3]));
        
        b[0] = 2*DOT(SUBXY,BA) + r * (dh[0] * mx[0] + dh[1] * mx[2]);
        a[0] = hessian[0] + r * (dh[0] * dh[0] + dh[1] * dh[1]) + delta;
        a[4] = hessian[4] + r * (dh[3] * dh[1]);
        tmp1 = (hessian[1] + r * (dh[1] * dh[3]))/a[0];
        a[13] = hessian[13] - hessian[12] * tmp1;
        a[9] = hessian[9] - hessian[8] * tmp1;
        a[5] = (hessian[5] + r * (dh[2] * dh[2] + dh[3] * dh[3]) + delta) - a[4] * tmp1;
        b[1] = (2*DOT(SUBXY,CA) + r * (dh[2] * mx[1] + dh[3] * mx[2])) - b[0] * tmp1;
        tmp2 = hessian[2]/a[0];
        tmp3 = hessian[3]/a[0];
        tmp4 = ((hessian[6]) - a[4] * tmp2)/a[5];
        a[14] = ((hessian[14] + r * (dh[7] * dh[5])) - hessian[12] * tmp2) - a[13] * tmp4;
        a[10] = ((hessian[10] + r * (dh[4] * dh[4] + dh[5] * dh[5]) + delta) - hessian[8] * tmp2) - a[9] * tmp4;
        b[2] = ((-2*DOT(SUBXY,ED) + r * (dh[4] * mx[3] + dh[5] * mx[5])) - b[0] * tmp2) - b[1] * tmp4;
        tmp5 = (hessian[7] - a[4] * tmp3)/a[5];
        tmp6 = (((hessian[11] + r * (dh[5] * dh[7])) - hessian[8] * tmp3) - a[9] * tmp5)/a[10];
        
        dx[3] = ((((-2*DOT(SUBXY,FD) + r * (dh[6] * mx[4] + dh[7] * mx[5])) - b[2] * tmp6) - b[0] * tmp3) - b[1] * tmp5) / ((((hessian[15] + r * (dh[6] * dh[6] + dh[7] * dh[7]) + delta) - hessian[12] * tmp3) - a[13] * tmp5) - a[14] * tmp6);
        dx[2] = (b[2] - (a[14] * dx[3])) / a[10];
        dx[1] = (b[1] - (a[9] * dx[2] + a[13] * dx[3])) / a[5];
        dx[0] = (b[0] - (a[4] * dx[1] + hessian[8] * dx[2] + hessian[12] * dx[3])) / a[0];
        
        x[0] = x[0] - dx[0];
        x[1] = x[1] - dx[1];
        x[2] = x[2] - dx[2];
        x[3] = x[3] - dx[3];
    }
    
    P[0] = A[0]+(BA[0] * x[0])+(CA[0] * x[1]);
    P[1] = A[1]+(BA[1] * x[0])+(CA[1] * x[1]);
    P[2] = A[2]+(BA[2] * x[0])+(CA[2] * x[1]);
    
    Q[0] = D[0]+(ED[0] * x[2])+(FD[0] * x[3]);
    Q[1] = D[1]+(ED[1] * x[2])+(FD[1] * x[3]);
    Q[2] = D[2]+(ED[2] * x[2])+(FD[2] * x[3]);
    /*
    SUBXY[0] = (P[0]) - (Q[0]);
    SUBXY[1] = (P[1]) - (Q[1]);
    SUBXY[2] = (P[2]) - (Q[2]);
    iREAL distance = sqrt(DOT(SUBXY,SUBXY));
    */
    p[0][i] = P[0];
    p[1][i] = P[1];
    p[2][i] = P[2];

    q[0][i] = Q[0];
    q[1][i] = Q[1];
    q[2][i] = Q[2];
  }
}

