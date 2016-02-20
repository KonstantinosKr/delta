#include "forces.h"
#include "stdio.h"

int granular_force(iREAL n[3], iREAL vij[3], iREAL oij[3], iREAL depth, int i, int j, iREAL mass[], iREAL *iparam[NINT], int ij, iREAL f[3])
{
  iREAL ma = 1.0 / ((1/mass[i]) + (1/mass[j]));

  iREAL kn = iparam[SPRING][ij];
  iREAL en = iparam[DAMPER][ij] * sqrt(kn*ma);
  iREAL vn = DOT(vij,n);
  iREAL fn = kn*depth + en*vn;
  
  printf("kn:%f, en:%f, vn:%f, fn:%f depth:%f, vij[0]:%f vij[1]:%f vij[2]:%f\n", kn, en, vn, fn, depth, vij[0], vij[1], vij[2]); 
  f[0] = fn*n[0];
  f[1] = fn*n[1];
  f[2] = fn*n[2];
  printf("CONTACT F[0]: %f, F[1]: %f, F[2]: %f\n", f[0], f[1], f[2]); 
 
  /* TODO */
  return depth < 0.0 ? 1 : 0;
}

/* return pairing index based on (i,j) pairing of colors */
int pairing (int nummat, int pairs[], int i, int j)
{
  return 0; 
}

void forces (std::vector<contact> conpnt[],
  int nb, int pid[], iREAL * position[3], iREAL * angular[6], iREAL * linear[3],
  iREAL mass[], iREAL *force[3], iREAL *torque[3], iREAL gravity[3], int parmat[], iREAL * mparam[NMAT],
  int pairnum, int pairs[], int ikind[], iREAL * iparam[NINT])
{

  for (int i = 0; i < nb; i++)
  {
    iREAL oi[3], v[3], x[3];

    oi[0] = angular[3][i];
    oi[1] = angular[4][i];
    oi[2] = angular[5][i];

    v[0] = linear[0][i];
    v[1] = linear[1][i];
    v[2] = linear[2][i];

    x[0] = position[0][i];
    x[1] = position[1][i];
    x[2] = position[2][i];
      


    /* update contact forces */
    for(unsigned int k = 0; k<conpnt[i].size(); k++)
    {
      iREAL p[3], n[3], z[3], vi[3], vj[3], oj[3], vij[3], oij[3];

      p[0] = conpnt[i][k].point[0];
      p[1] = conpnt[i][k].point[1];
      p[2] = conpnt[i][k].point[2];

      z[0] = p[0]-x[0];
      z[1] = p[1]-x[1];
      z[2] = p[2]-x[2];

      vi[0] = oi[1]*z[2]-oi[2]*z[1] + v[0];
      vi[1] = oi[2]*z[0]-oi[0]*z[2] + v[1];
      vi[2] = oi[0]*z[1]-oi[1]*z[0] + v[2];

      int j = conpnt[i][k].pid[1]; //get index from slave body contact

      z[0] = p[0]-position[0][j];
      z[1] = p[1]-position[1][j];
      z[2] = p[2]-position[2][j];

      oj[0] = angular[3][j];
      oj[1] = angular[4][j];
      oj[2] = angular[5][j];

      vj[0] = oj[1]*z[2]-oj[2]*z[1] + linear[0][j];
      vj[1] = oj[2]*z[0]-oj[0]*z[2] + linear[1][j];
      vj[2] = oj[0]*z[1]-oj[1]*z[0] + linear[2][j];

      SUB (vj, vi, vij); // relative linear velocity
      SUB (oj, oi, oij); // relative angular velocity

      int ij = pairing (pairnum, pairs, conpnt[i][k].color[0], conpnt[i][k].color[1]);//get material from colours
    
      iREAL f[3];

      switch (ikind[ij])
      {
        case GRANULAR:
          granular_force (conpnt[i][k].normal, vij, oij, conpnt[i][k].depth, i, j, mass, iparam, ij, f);
          break;
        case BONDED:
          /* TODO */
          break;
        case UFORCE:
          /* TODO */
          break;
        default:
          printf ("ERROR: invalid pairing kind");
          break;
      }
      
      iREAL a[3];
    
      a[0] = conpnt[i][j].point[0]-x[0];//boundary
      a[1] = conpnt[i][j].point[1]-x[1];
      a[2] = conpnt[i][j].point[2]-x[2];
      
      //master force
      force[0][i] += f[0];
      force[1][i] += f[1];
      force[2][i] += f[2];

      torque[0][i] += a[1]*f[2] - a[2]*f[1];//cross product
      torque[1][i] += a[2]*f[0] - a[0]*f[2];
      torque[2][i] += a[0]*f[1] - a[1]*f[0];

      //add force to slaves
      
      f[0] = -f[0];
      f[1] = -f[1];
      f[2] = -f[2];

      force[0][j] += f[0];
      force[1][j] += f[1];
      force[2][j] += f[2];
      
      a[0] = conpnt[i][j].point[0]-position[0][j];//boundary
      a[1] = conpnt[i][j].point[1]-position[1][j];
      a[2] = conpnt[i][j].point[2]-position[2][j];

      torque[0][j] += a[1]*f[2] - a[2]*f[1];//cross product
      torque[1][j] += a[2]*f[0] - a[0]*f[2];
      torque[2][j] += a[0]*f[1] - a[1]*f[0];
    }
  }

  for(int i=0;i<nb;i++)
  {
    force[0][i] = force[0][i] + mass[i] * gravity[0];
    force[1][i] = force[1][i] + mass[i] * gravity[1];
    force[2][i] = force[2][i] + mass[i] * gravity[2];
    printf("Total Force of body: %i is: %f %f %f\n", i, force[0][i], force[1][i], force[2][i]);
    printf("Total Torque of body: %i is: %f %f %f\n", i, torque[0][i], torque[1][i], torque[2][i]);
    
    std::vector<contact>().swap(conpnt[i]);
  }
}


