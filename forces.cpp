#include "forces.h"

void forces(unsigned int *tid, unsigned int *pid, master_conpnt *con)
{

  for(master_conpnt *i = con; i!=NULL; i= i->next;)
  {
    for(int j = 0; j < i->size; j++)
    {
      iREAL normal[3];
      normal[0] = i->normal[0][j];
      normal[1] = i->normal[1][j];
      normal[2] = i->normal[2][j];

      iREAL depth = i->depth[j];
      
      iREAL f[3];
      f[0] = 0;
      f[1] = 0;
      f[2] = 0;
      //granular_force(normal, vij, oij, depth, i, j, mass, invm, ij, f);
      
      i->force[j] = f[0];
      i->force[j] = f[1];
      i->force[j] = f[2];
    }
  }
}

void granular_force(iREAL n[3], iREAL vij[3], iREAL oij[3], iREAL depth, int i, int j, iREAL mass[], iREAL invm[], int ij, iREAL f[3])
{

}
