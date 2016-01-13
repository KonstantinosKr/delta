#include "dynamics.h"


void gen_velocities (iREAL lo[3], iREAL hi[3], unsigned int nt, iREAL * v[3])
{
    for(unsigned int i = 0; i < nt; i++)
    {
      v[0][i] = hi[0] * drand48();
      v[1][i] = hi[1] * drand48();
      v[2][i] = hi[2] * drand48();
    }
}

void dynamics (master_conpnt master[], slave_conpnt slave[],
  int parnum, iREAL * angular[6], iREAL * v[3],
  iREAL * rotation[9], iREAL * position[6],
  iREAL * inertia[9], iREAL * inverse[9],
  iREAL mass[], iREAL invm[], iREAL * force[3],
  iREAL * torque[3], iREAL gravity[3], iREAL step)
{
  int idx = 0;
  for(master_conpnt *i = master; i!=NULL; i= i->next)
  {
    //filter out multiple contact points
    for(int j = 0; j<i->size; j++)
    {
      idx = i->masterid[j];
      
      int counter = 0;
      int idxx = 0;
       
      for(master_conpnt *ii = master; ii!=NULL; ii= ii->next)
      {
        if (idx == ii->masterid[idxx])
        {
          counter = counter + 1;
        }
      }
    }
  }
}

// dynamics task 
/*void dynamics (master_conpnt master[], slave_conpnt slave[],
  int parnum, iREAL * angular[6], iREAL * linear[3],
  iREAL * rotation[9], iREAL * position[6],
  iREAL * inertia[9], iREAL * inverse[9],
  iREAL mass[], iREAL invm[], iREAL * force[3],
  iREAL * torque[3], iREAL gravity[3], iREAL step)
{
  iREAL half = 0.5*step;

  for (int i = 0; i < parnum; i ++) // force accumulation
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

  for (int i = 0; i<parnum; i++) // time integration 
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
}*/

/*
// invert inertia properties 
void invert (int span, iREAL * inertia[9], iREAL * inverse[9], iREAL mass[], iREAL invm[])
{
  int start = 0;
  int end = span;

  for(int i = start; i<end;i++)
  {
    iREAL a[9], x[9], det;

    for (int j = 0; j < 9; j ++)
    {
      a[j] = inertia[j][i];
    }
    
    INVERT (a, x, det);

    for (int j = 0; j < 9; j ++)
    {
      inverse[j][i] = x[j];
    }

    invm[i] = 1.0 / mass[i];
  }
}

// estimate critical time step 
iREAL critical (int parnum, iREAL mass[], int pairnum, iREAL * iparam[NINT])
{
  iREAL kmax, emax, mmin, omax, step;
  iREAL k, e, m;

  k = 0.0;
  e = 0.0;

  for(int i = 0; i<pairnum;i++)
  {
    k = max(iparam[SPRING][i],k);
    e = max(iparam[DAMPER][i],e);
  }

  kmax = reduce_max(k);
  emax = reduce_max(e);

  m = 1E99;

  for(int i = 0; i<parnum;i++)
  {
    m = min(mass[i],m);
  }

  mmin = m;

  omax = sqrt (kmax/mmin);

  //http://www.dynasupport.com/tutorial/ls-dyna-users-guide/time-integration 
  step  = (2.0/omax)*(sqrt(1.0+emax*emax) - emax);

  return step;
}

// Euler task 
void euler_task (int span, int parnum, iREAL * angular[6],
  iREAL * linear[3], iREAL * rotation[9], iREAL * position[3], iREAL step)
{
  int start = 0;
  int end = span;

  for(int i = start; i<end;i++)
  {
    iREAL O[3], L1[9], DL[9], L2[9], o[3];

    O[0] = step*angular[0][i];
    O[1] = step*angular[1][i];
    O[2] = step*angular[2][i];

    expmap (O[0], O[1], O[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);

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
    position[1][i] += step * linear[0][i];
    position[2][i] += step * linear[0][i];
  }
}
*/

void integrate (iREAL step, iREAL lo[3], iREAL hi[3], unsigned int nt, iREAL * t[3][3], iREAL * v[3])
{
    for(unsigned int i = 0; i < nt; i++)
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

//id: body/particle id,  
void translate(unsigned int id)
{
}
