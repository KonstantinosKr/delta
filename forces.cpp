#include "forces.h"
#include "stdio.h"

/*
 *
 *WARNING!
 *
 *I may create gaps with gone that need to be filled with contact points that are located in con->next.
 */

int granular_force(iREAL n[3], iREAL vij[3], iREAL oij[3], iREAL depth, int i, int j, iREAL mass[], iREAL invm[], iREAL *iparam[NINT], int ij, iREAL f[3])
{
  iREAL ma; 
  
  if (j >= 0) ma = 1.0 / (invm[i] + invm[j]);
  else ma = mass[i];

  iREAL kn = iparam[SPRING][ij];
  iREAL en = iparam[DAMPER][ij] * 2.0 * sqrt(kn*ma);
  iREAL vn = DOT(vij,n);
  iREAL fn = kn*depth + en*vn;
  printf("CONTACT\n"); 
  f[0] = fn*n[0];
  f[1] = fn*n[1];
  f[2] = fn*n[2];
 
  /* TODO */
  int ret = 0;
  if(depth < 0.0)
  {
    ret = 1;//depth is smaller than 0 so remove contact
  }
  else
  {
    ret = 0;//depth is higher than 0 so keep contact
  }
  return depth < 0.0 ? 1 : 0;
}

/* return pairing index based on (i,j) pairing of colors */
int pairing (int nummat, int pairs[], int i, int j)
{
  int p[2] = {i, j}, start = 1, end = nummat;

  if (i > j)
  {
    p[0] = j;
    p[1] = i;
  }

  while (start < end)
  {
    int mid = ((end-start)>>1) + start;
    if (p[0] > pairs[2*mid]) start = mid;
    else if(p[0] == pairs[2*mid])
    {
      if (p[1] > pairs[2*mid+1]) start = mid;
      else if (p[1] == pairs[2*mid+1]) return mid;
      else end = mid;
    }
    else end = mid;
  }

  return 0; /* default material */
}

void forces (master_conpnt master[], slave_conpnt slave[],
  int nb, iREAL * position[3], iREAL * angular[6], iREAL * linear[3],
  iREAL mass[], iREAL invm[], int parmat[], iREAL * mparam[NMAT],
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
    for (master_conpnt * con = &master[i]; con; con = con->next)
    {
      int gone[CONBUF];

      for(int k = 0; k<con->size; k++)
      {
        iREAL p[3], n[3], z[3], vi[3], vj[3], oj[3], vij[3], oij[3];

        p[0] = con->point[0][k];
        p[1] = con->point[1][k];
        p[2] = con->point[2][k];

        n[0] = con->normal[0][k];
        n[1] = con->normal[1][k];
        n[2] = con->normal[2][k];

        z[0] = p[0]-x[0];
        z[1] = p[1]-x[1];
        z[2] = p[2]-x[2];

        vi[0] = oi[1]*z[2]-oi[2]*z[1] + v[0];
        vi[1] = oi[2]*z[0]-oi[0]*z[2] + v[1];
        vi[2] = oi[0]*z[1]-oi[1]*z[0] + v[2];

        int j = con->slave[0][k]; //get index from slave body/contact

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

        int ij = pairing (pairnum, pairs, con->color[0][k], con->color[1][k]);//get material from colours
      
        iREAL f[3];

        switch (ikind[ij])
        {
          case GRANULAR:
            gone[k] = granular_force (n, vij, oij, con->depth[k], i, j, mass, invm, iparam, ij, f);
            break;
          case BONDED:
            /* TODO */
            break;
          case UFORCE:
            /* TODO */
            break;
          default:
            //printf ("ERROR: invalid pairing kind");
            break;
        }
        
        con->force[0][k] = f[0];
        con->force[1][k] = f[1];
        con->force[2][k] = f[2];
      }
      
      int ngone = 0;

      for (int k = 0; k < con->size; k ++)
      {//fill gaps
        if (gone[k] != 0)//gone is 1 so remove contact point.
        {
          int j = k+1; //go to next contact id
          while (j < con->size && gone[j] != 0) j ++;//get j id of last contact
          if (j < con->size)//loop through the contacts
          {//replace with last to fill the gone contact point
            con->master[k] = con->master[j];
            con->slave[0][k] = con->slave[0][j];
            con->slave[1][k] = con->slave[1][j];
            con->color[0][k] = con->color[0][j];
            con->color[1][k] = con->color[1][j];
            con->point[0][k] = con->point[0][j];
            con->point[1][k] = con->point[1][j];
            con->point[2][k] = con->point[2][j];
            con->normal[0][k] = con->normal[0][j];
            con->normal[1][k] = con->normal[1][j];
            con->normal[2][k] = con->normal[2][j];
            con->depth[k] = con->depth[j];
            con->force[0][k] = con->force[0][j];
            con->force[1][k] = con->force[1][j];
            con->force[2][k] = con->force[2][j];

            gone[j] = -1; // not to be used again 
          }

        if (gone[k] > 0) ngone ++;
        }
      }

      con->size -= ngone; //reduce size of contact points to size-removed/gone
    }
    
    master_conpnt * con = master[i].next; //may be con = master[i] instead of master[i].next
    while (con && con->next) // delete empty items
    {
      master_conpnt * next = con->next;

      if (next->size == 0)
      {
        con->next = next->next;
        delete next;
      }

      con = con->next;
    }//this deletes the last element contact from the list

    /* symmetrical copy into slave contact points */
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
}


