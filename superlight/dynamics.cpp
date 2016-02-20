#include "dynamics.h"
#include "stdio.h"

void dynamics (std::vector<contact> conpnt[], int nt, int nb, iREAL *t[3][3], int *pid, iREAL * linear[3], iREAL * position[3],
              iREAL mass[], iREAL * force[3], iREAL gravity[3], iREAL step)
{
  for (int i = 0; i < nb; i++) // force accumulation
  {
    iREAL f[3], a[3], fs[3], ts[3];
    iREAL po[3], ma;

    po[0] = position[0][i];
    po[1] = position[1][i];
    po[2] = position[2][i];
    
    ma = mass[i];

    SET (fs, 0.0);
    SET (ts, 0.0);
    
    for(unsigned int j = 0; j<conpnt[i].size();j++)
    {
      f[0] = conpnt[i][j].force[0];
      f[1] = conpnt[i][j].force[1];
      f[2] = conpnt[i][j].force[2];

      a[0] = conpnt[i][j].point[0]-po[0];
      a[1] = conpnt[i][j].point[1]-po[1];
      a[2] = conpnt[i][j].point[2]-po[2];

      ACC (f, fs);
      PRODUCTADD (a, f, ts);
    }
    
    force[0][i] = fs[0] + ma * gravity[0];
    force[1][i] = fs[1] + ma * gravity[1];
    force[2][i] = fs[2] + ma * gravity[2];

    printf("Total Force of body: %i is: %f %f %f\n", i, force[0][i], force[1][i], force[2][i]);
    
    std::vector<contact>().swap(conpnt[i]);

    //time integration
    iREAL im = (1/mass[i])*step;

    linear[0][i] = linear[0][i]+force[0][i]*im;
    linear[1][i] = linear[1][i]+force[1][i]*im;
    linear[2][i] = linear[2][i]+force[2][i]*im;
    
    printf("Body:%i force:%f %f %f\n", i, force[0][i], force[1][i], force[2][i]);

    position[0][i] += step*linear[0][i];
    position[1][i] += step*linear[1][i];
    position[2][i] += step*linear[2][i];
  }
  
  for (int i = 0; i<nt; i++)
  {
    int j = pid[i];
    
    t[0][0][i] += linear[0][j] *step;
    t[0][1][i] += linear[1][j] *step;
    t[0][2][i] += linear[2][j] *step;
    
    t[0][0][i] += linear[0][j] *step;
    t[0][1][i] += linear[1][j] *step;
    t[0][2][i] += linear[2][j] *step;
    
    t[1][0][i] += linear[0][j] *step;
    t[1][1][i] += linear[1][j] *step;
    t[1][2][i] += linear[2][j] *step;
    
    t[1][0][i] += linear[0][j] *step;
    t[1][1][i] += linear[1][j] *step;
    t[1][2][i] += linear[2][j] *step;
    
    t[2][0][i] += linear[0][j] *step;
    t[2][1][i] += linear[1][j] *step;
    t[2][2][i] += linear[2][j] *step;
    
    t[2][0][i] += linear[0][j] *step;
    t[2][1][i] += linear[1][j] *step;
    t[2][2][i] += linear[2][j] *step;
  }
}

