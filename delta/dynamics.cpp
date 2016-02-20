#include "dynamics.h"
#include "stdio.h"


void gen_velocities (iREAL lo[3], iREAL hi[3], int nt, iREAL * v[3])
{
    for(int i = 0; i < nt; i++)
    {
        v[0][i] = hi[0] * drand48();
        v[1][i] = hi[1] * drand48();
        v[2][i] = hi[2] * drand48();
    }
}

// dynamics task
void dynamics (master_conpnt master[], slave_conpnt slave[],
               int nb, iREAL * angular[6], iREAL * linear[3],
               iREAL * rotation[9], iREAL * position[6],
               iREAL * inertia[9], iREAL * inverse[9],
               iREAL mass[], iREAL invm[], iREAL * force[3],
               iREAL * torque[3], iREAL gravity[3], iREAL step)
{
    iREAL half = 0.5*step;
    
    /* symmetrical copy into slave contact points */
    for(int i = 0; i<nb;i++)
    {
        for (master_conpnt * con = &master[i]; con; con = con->next)
        {
            for (int j = 0; j < con->size; j ++)
            {
                slave_conpnt *ptr;
                int k=0;
                
                ptr = newcon (&slave[con->slave[0][j]], &k);
                
                ptr->master[0][k] = i;
                ptr->master[1][k] = con->master[j];
                ptr->point[0][k] = con->point[0][j];
                ptr->point[1][k] = con->point[1][j];
                ptr->point[2][k] = con->point[2][j];
                ptr->force[0][k] = -con->force[0][j];
                ptr->force[1][k] = -con->force[1][j];
                ptr->force[2][k] = -con->force[2][j];
            }
        }
    }
    
    for (int i = 0; i < nb; i ++) // force accumulation
    {
        iREAL f[3], a[3], fs[3], ts[3];
        iREAL po[3], ma;
        
        po[0] = position[0][i];
        po[1] = position[1][i];
        po[2] = position[2][i];
        
        ma = mass[i];
        
        SET (fs, 0.0);
        SET (ts, 0.0);
        
        for (master_conpnt * m = &master[i]; m; m = m->next)
        {
            for(int j = 0; j<m->size;j++)
            {
                f[0] = m->force[0][j];
                f[1] = m->force[1][j];
                f[2] = m->force[2][j];
                
                a[0] = m->point[0][j]-po[0];
                a[1] = m->point[1][j]-po[1];
                a[2] = m->point[2][j]-po[2];
                
                ACC (f, fs);
                PRODUCTADD (a, f, ts);
            }
        }
        
        for (slave_conpnt * s = &slave[i]; s; s = s->next)
        {
            for(int j = 0;j<s->size;j++)
            {
                f[0] = s->force[0][j];
                f[1] = s->force[1][j];
                f[2] = s->force[2][j];
                
                a[0] = s->point[0][j]-po[0];
                a[1] = s->point[1][j]-po[1];
                a[2] = s->point[2][j]-po[2];
                
                ACC (f, fs);
                PRODUCTADD (a, f, ts);
            }
        }
        
        force[0][i] = fs[0] + ma * gravity[0];
        force[1][i] = fs[1] + ma * gravity[1];
        force[2][i] = fs[2] + ma * gravity[2];
        
        torque[0][i] = ts[0];
        torque[1][i] = ts[1];
        torque[2][i] = ts[2];
    }
    
    for (int i = 0; i<nb; i++) // time integration
    {
        iREAL O[3], o[3], v[3], L1[9], J[9], I[9], im, f[3], t[3], T[3], DL[9], L2[9], A[3], B[3];
        
        O[0] = angular[0][i];
        O[1] = angular[1][i];
        O[2] = angular[2][i];
        
        v[0] = linear[0][i];
        v[1] = linear[1][i];
        v[2] = linear[2][i];
        
        L1[0] = rotation[0][i];
        L1[1] = rotation[1][i];
        L1[2] = rotation[2][i];
        L1[3] = rotation[3][i];
        L1[4] = rotation[4][i];
        L1[5] = rotation[5][i];
        L1[6] = rotation[6][i];
        L1[7] = rotation[7][i];
        L1[8] = rotation[8][i];
        
        J[0] = inertia[0][i];
        J[1] = inertia[1][i];
        J[2] = inertia[2][i];
        J[3] = inertia[3][i];
        J[4] = inertia[4][i];
        J[5] = inertia[5][i];
        J[6] = inertia[6][i];
        J[7] = inertia[7][i];
        J[8] = inertia[8][i];
        
        I[0] = inverse[0][i];
        I[1] = inverse[1][i];
        I[2] = inverse[2][i];
        I[3] = inverse[3][i];
        I[4] = inverse[4][i];
        I[5] = inverse[5][i];
        I[6] = inverse[6][i];
        I[7] = inverse[7][i];
        I[8] = inverse[8][i];
        
        im = invm[i];
        
        f[0] = force[0][i];
        f[1] = force[1][i];
        f[2] = force[2][i];
        
        t[0] = torque[0][i];
        t[1] = torque[1][i];
        t[2] = torque[2][i];
        
        TVMUL (L1, t, T);
        
        expmap (-half*O[0], -half*O[1], -half*O[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);
        
        NVMUL (J, O, A);
        NVMUL (DL, A, B);
        ADDMUL (B, half, T, B);
        NVMUL (I, B, A); // O(t+h/2)
        
        NVMUL (J, A, B);
        PRODUCTSUB (A, B, T); // T - O(t+h/2) x J O(t+h/2)
        
        SCALE (T, step);
        NVADDMUL (O, I, T, O); // O(t+h)
        
        im *= step;
        ADDMUL (v, im, f, v); // v(t+h)
        
        expmap (step*O[0], step*O[1], step*O[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);
        
        NNMUL (L1, DL, L2);
        
        rotation[0][i] = L2[0];
        rotation[1][i] = L2[1];
        rotation[2][i] = L2[2];
        rotation[3][i] = L2[3];
        rotation[4][i] = L2[4];
        rotation[5][i] = L2[5];
        rotation[6][i] = L2[6];
        rotation[7][i] = L2[7];
        rotation[8][i] = L2[8];
        
        NVMUL (L2, O, o);
        
        position[0][i] += step*v[0];
        position[1][i] += step*v[1];
        position[2][i] += step*v[2];
        
        angular[0][i] = O[0];
        angular[1][i] = O[1];
        angular[2][i] = O[2];
        
        angular[3][i] = o[0];
        angular[4][i] = o[1];
        angular[5][i] = o[2];
        
        linear[0][i] = v[0];
        linear[1][i] = v[1];
        linear[2][i] = v[2];
    }
}

void shapes (int nb, int nt, iREAL lo[3], iREAL hi[3], int pid[], iREAL * t[6][3], iREAL *v[3], iREAL * rotation[9], iREAL * position[6])
{
    for (int i = 0; i<nt; i++)
    {
        iREAL L[9], X[3], x[3], C[3], c[3];
        
        int j = pid[i];
        L[0] = rotation[0][j];
        L[1] = rotation[1][j];
        L[2] = rotation[2][j];
        L[3] = rotation[3][j];
        L[4] = rotation[4][j];
        L[5] = rotation[5][j];
        L[6] = rotation[6][j];
        L[7] = rotation[7][j];
        L[8] = rotation[8][j];
        
        X[0] = position[3][j];
        X[1] = position[4][j];
        X[2] = position[5][j];
        
        x[0] = position[0][j];
        x[1] = position[1][j];
        x[2] = position[2][j];
        
        //point A REFERENCIAL
        C[0] = t[3][0][i];
        C[1] = t[3][1][i];
        C[2] = t[3][2][i];
        
        SCC (X, C);
        NVADDMUL (x, L, C, c);
        
        //point A SPATIAL
        t[0][0][i] = c[0];
        t[0][1][i] = c[1];
        t[0][2][i] = c[2];
        
        //point B REFERENTIAL
        C[0] = t[4][0][i];
        C[1] = t[4][1][i];
        C[2] = t[4][2][i];
        
        SCC (X, C);
        NVADDMUL (x, L, C, c);
        
        //point B SPATIAL
        t[1][0][i] = c[0];
        t[1][1][i] = c[1];
        t[1][2][i] = c[2];
        
        //point C REFERENCIAL
        C[0] = t[5][0][i];
        C[1] = t[5][1][i];
        C[2] = t[5][2][i];
        
        SCC (X, C);
        NVADDMUL (x, L, C, c);
        
        //point C SPATIAL
        t[2][0][i] = c[0];
        t[2][1][i] = c[1];
        t[2][2][i] = c[2];
        
        if (t[0][0][i] < lo[0]) v[0][j] *= -1;
        if (t[0][1][i] < lo[1]) v[1][j] *= -1;
        if (t[0][2][i] < lo[2]) v[2][j] *= -1;
        if (t[0][0][i] > hi[0]) v[0][j] *= -1;
        if (t[0][1][i] > hi[1]) v[1][j] *= -1;
        if (t[0][2][i] > hi[2]) v[2][j] *= -1;
        
        if (t[1][0][i] < lo[0]) v[0][j] *= -1;
        if (t[1][1][i] < lo[1]) v[1][j] *= -1;
        if (t[1][2][i] < lo[2]) v[2][j] *= -1;
        if (t[1][0][i] > hi[0]) v[0][j] *= -1;
        if (t[1][1][i] > hi[1]) v[1][j] *= -1;
        if (t[1][2][i] > hi[2]) v[2][j] *= -1;
        
        if (t[2][0][i] < lo[0]) v[0][j] *= -1;
        if (t[2][1][i] < lo[1]) v[1][j] *= -1;
        if (t[2][2][i] < lo[2]) v[2][j] *= -1;
        if (t[2][0][i] > hi[0]) v[0][j] *= -1;
        if (t[2][1][i] > hi[1]) v[1][j] *= -1;
        if (t[2][2][i] > hi[2]) v[2][j] *= -1;
    }
}

// Euler task
void euler(int nb, iREAL * angular[6], iREAL * linear[3], iREAL * rotation[9], iREAL * position[3], iREAL step)
{
    for(int i = 0; i<nb;i++)
    {
        iREAL O[3], L1[9], DL[9], L2[9], o[3];
        
        O[0] = angular[0][i];
        O[1] = angular[1][i];
        O[2] = angular[2][i];
        
        expmap (step*O[0], step*O[1], step*O[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);
        
        L1[0] = rotation[0][i];
        L1[1] = rotation[1][i];
        L1[2] = rotation[2][i];
        L1[3] = rotation[3][i];
        L1[4] = rotation[4][i];
        L1[5] = rotation[5][i];
        L1[6] = rotation[6][i];
        L1[7] = rotation[7][i];
        L1[8] = rotation[8][i];
        
        NNMUL (L1, DL, L2);
        
        rotation[0][i] = L2[0];
        rotation[1][i] = L2[1];
        rotation[2][i] = L2[2];
        rotation[3][i] = L2[3];
        rotation[4][i] = L2[4];
        rotation[5][i] = L2[5];
        rotation[6][i] = L2[6];
        rotation[7][i] = L2[7];
        rotation[8][i] = L2[8];
        
        NVMUL (L2, O, o);
        
        angular[3][i] = o[0];
        angular[4][i] = o[1];
        angular[5][i] = o[2];
        
        position[0][i] += step * linear[0][i];
        position[1][i] += step * linear[1][i];
        position[2][i] += step * linear[2][i];
    }
}


void integrate (iREAL step, iREAL lo[3], iREAL hi[3], int nt, iREAL * t[3][3], iREAL * v[3])
{
    for(int i = 0; i < nt; i++)
    {
        t[0][0][i] += step*v[0][i];
        t[0][1][i] += step*v[1][i];
        t[0][2][i] += step*v[2][i];
        t[1][0][i] += step*v[0][i];
        t[1][1][i] += step*v[1][i];
        t[1][2][i] += step*v[2][i];
        t[2][0][i] += step*v[0][i];
        t[2][1][i] += step*v[1][i];
        t[2][2][i] += step*v[2][i];
        
        if (t[0][0][i] < lo[0]) v[0][i] *= -1;
        if (t[0][1][i] < lo[1]) v[1][i] *= -1;
        if (t[0][2][i] < lo[2]) v[2][i] *= -1;
        if (t[0][0][i] > hi[0]) v[0][i] *= -1;
        if (t[0][1][i] > hi[1]) v[1][i] *= -1;
        if (t[0][2][i] > hi[2]) v[2][i] *= -1;
        
        if (t[1][0][i] < lo[0]) v[0][i] *= -1;
        if (t[1][1][i] < lo[1]) v[1][i] *= -1;
        if (t[1][2][i] < lo[2]) v[2][i] *= -1;
        if (t[1][0][i] > hi[0]) v[0][i] *= -1;
        if (t[1][1][i] > hi[1]) v[1][i] *= -1;
        if (t[1][2][i] > hi[2]) v[2][i] *= -1;
        
        if (t[2][0][i] < lo[0]) v[0][i] *= -1;
        if (t[2][1][i] < lo[1]) v[1][i] *= -1;
        if (t[2][2][i] < lo[2]) v[2][i] *= -1;
        if (t[2][0][i] > hi[0]) v[0][i] *= -1;
        if (t[2][1][i] > hi[1]) v[1][i] *= -1;
        if (t[2][2][i] > hi[2]) v[2][i] *= -1;
    }
}


/* vectorizable exponential map */
void expmap (iREAL Omega1, iREAL Omega2, iREAL Omega3,
             iREAL &Lambda1, iREAL &Lambda2, iREAL &Lambda3,
			          iREAL &Lambda4, iREAL &Lambda5, iREAL &Lambda6,
			          iREAL &Lambda7, iREAL &Lambda8, iREAL &Lambda9)
{
    iREAL angsq, sx, cx, v0, v1, v2, v01, v02, v12, s0, s1, s2;
    
    v0 = Omega1 * Omega1;
    v1 = Omega2 * Omega2;
    v2 = Omega3 * Omega3;
    
    angsq = v0 + v1 + v2;
    
    if (angsq < 3.0461741978671E-02) // use Taylor expansion if |Omega| < 10 deg
    {
        sx = 1.0 +
        (-1.666666666666667E-1 +
         (8.333333333333333E-3 +
          (-1.984126984126984E-4 +
           (2.755731922398589E-6 +
            (-2.505210838544172E-8 +
             1.605904383682161E-10 * angsq
             )*angsq
            )*angsq
           )*angsq
          )*angsq
         )*angsq;
        cx = 0.5 +
        (-4.166666666666667E-2 +
         (1.388888888888889E-3 +
          (-2.480158730158730E-5 +
           (2.755731922398589E-7 +
            (-2.087675698786810E-9 +
             1.147074559772972E-11 * angsq
             )*angsq
            )*angsq
           )*angsq
          )*angsq
         )*angsq;
    }
    else
    {
        iREAL t, s, c;
        t = angsq;
        angsq = sqrt (angsq);
        s = sin (angsq);
        c = cos (angsq);
        sx = s / angsq;
        cx = (1.0 - c) / t;
    }
    
    v01 = Omega1 * Omega2;
    v02 = Omega2 * Omega3;
    v12 = Omega2 * Omega3;
    s0 = sx * Omega1;
    s1 = sx * Omega2;
    s2 = sx * Omega3;
    
    Lambda1 = -cx*(v2+v1);
    Lambda2 = cx*v01;
    Lambda3 = cx*v02;
    Lambda4 = Lambda2;
    Lambda5 = -cx*(v2+v0);
    Lambda6 = cx*v12;
    Lambda7 = Lambda3;
    Lambda8 = Lambda6;
    Lambda9 = -cx*(v1+v0);
    Lambda1 += 1.0;
    Lambda2 += s2;
    Lambda3 -= s1;
    Lambda4 -= s2;
    Lambda5 += 1.0;
    Lambda6 += s0;
    Lambda7 += s1;
    Lambda8 -= s0;
    Lambda9 += 1.0;
}
