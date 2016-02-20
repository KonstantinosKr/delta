#include "forces.h"

void granular(iREAL n[3], iREAL vij[3], iREAL depth, int i, int j, iREAL mass[], iREAL *iparam[NINT], int ij, iREAL f[3])
{
  iREAL ma = 1.0 / (1/mass[i] + 1/mass[j]);
  
  iREAL kn = iparam[SPRING][ij];
  iREAL en = iparam[DAMPER][ij] * 2.0 * sqrt(kn*ma);
  iREAL vn = DOT(vij,n);
  iREAL fn = kn*depth + en*vn;
  //printf("kn:%f, en:%f, vn:%f, fn:%f depth:%f, vij[0]:%f vij[1]:%f vij[2]:%f\n", kn, en, vn, fn, depth, vij[0], vij[1], vij[2]); 
  f[0] = fn*n[0];
  f[1] = fn*n[1];
  f[2] = fn*n[2];
  //printf("CONTACT F[0]: %f, F[1]: %f, F[2]: %f\n", f[0], f[1], f[2]); 
}

/*
 void granular(iREAL normal[3], iREAL relativeVelocity[3], iREAL depth, int i, int j, iREAL mass[], iREAL force[3])
 {//contact normal has to be unit vector
 
    iREAL ma = 1.0 / ((1/mass[i]) + (1/mass[j]);
    iREAL SPRING = 1E6; //dashpot-spring parameter
    iREAL DAMPER = 1E2 * sqrt(kn*ma); //damping parameter
    iREAL vn = DOT(relativeVelocity,normal); //maybe not needed for linear contact
    iREAL fn = kn*depth + en*vn; //force magnitude
 
    force[0] = fn*normal[0];
    force[1] = fn*normal[1];
    force[2] = fn*normal[2];
 }
 */

/* return pairing index based on (i,j) pairing of colors */
int pairing (int nummat, int pairs[], int i, int j)
{
  return 0; /* default material */
}

void forces (std::vector<contact> conpnt[], int nb, iREAL * linear[3], iREAL mass[], int parmat[], iREAL * mparam[NMAT],
              int pairnum, int pairs[], int ikind[], iREAL * iparam[NINT])
{
  for(int i=0;i<nb;i++)
  {
    iREAL vi[3];
    vi[0] = linear[0][i];
    vi[1] = linear[1][i];
    vi[2] = linear[2][i];
    for(unsigned int k = 0; k<conpnt[i].size(); k++)
    {  
      iREAL p[3], n[3], vj[3], vij[3];

      p[0] = conpnt[i][k].point[0];
      p[1] = conpnt[i][k].point[1];
      p[2] = conpnt[i][k].point[2];

      n[0] = conpnt[i][k].normal[0];
      n[1] = conpnt[i][k].normal[1];
      n[2] = conpnt[i][k].normal[2];

      int j = conpnt[i][k].pid[1]; //get index from slave body contact

      vj[0] = linear[0][j];
      vj[1] = linear[1][j];
      vj[2] = linear[2][j];

      SUB (vj, vi, vij); // relative linear velocity

      int ij = pairing (pairnum, pairs, conpnt[i][k].color[0], conpnt[i][k].color[1]);//get material from colours
    
      iREAL f[3];

      switch (ikind[ij])
      {
        case GRANULAR:
          granular (n, vij, conpnt[i][k].depth, i, j, mass, iparam, ij, f);
          break;
        default:
          printf ("ERROR: invalid pairing kind");
          break;
      }
     
      if(conpnt[i][k].pid[0] == i)
      {
        conpnt[i][k].force[0] = f[0];
        conpnt[i][k].force[1] = f[1];
        conpnt[i][k].force[2] = f[2];
      }
      else
      {
        conpnt[i][k].force[0] = -f[0];
        conpnt[i][k].force[1] = -f[1];
        conpnt[i][k].force[2] = -f[2];
      }
    }
  }
}


