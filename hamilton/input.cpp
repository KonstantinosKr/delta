#include "input.h" 
#include <float.h>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>


void init_enviroment(int scenario, int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3])
{
  srand48(time(NULL));
  switch(scenario)
  {
    case 0:
    {
      twoParticleCollision(nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);
      break;
    }
    case 1:
    {
      eggCollision(nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);
      break;
    }
    case 2:
    {
      oneParticleVsWall(nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);
      break;
    }
    case 3:
    {
      chaos(nt, nb, t, linear, angular, inertia, inverse, rotation, mass, parmat, tid, pid, position, lo, hi);
    }
  }
}

void condition_enviroment(int nb, iREAL lin[3], iREAL ang[3], int ma, 
                          iREAL *linear[3], iREAL *angular[6], 
                          iREAL *rotation[9], iREAL *mass, 
                          iREAL *inertia[9], iREAL *inverse[9], int *parmat)
{
  int i = nb;
  //iREAL rand = drand48();//random pull velocity
  
  //v[0][i] = 500 * rand;
  //v[1][i] = 500 * rand;
  //v[2][i] = 500 * rand;
  linear[0][i] = lin[0];
  linear[1][i] = lin[1];
  linear[2][i] = lin[2];

  //iREAL randomrot = 5 *rand;
  angular[0][i] = ang[0];
  angular[1][i] = ang[1];
  angular[2][i] = ang[2];
  angular[3][i] = ang[0]; 
  angular[4][i] = ang[1];
  angular[5][i] = ang[2];
 
  mass[i] = ma;
  parmat[i] = 0;
  
  //double volume = 
  //mass[i] = volume*mparam[DENSITY][material];

  inertia[0][i] = 10*mass[i];
  inertia[1][i] = 0;
  inertia[2][i] = 0;
  inertia[3][i] = 0;
  inertia[4][i] = 10*mass[i];
  inertia[5][i] = 0;
  inertia[6][i] = 0;
  inertia[7][i] = 0;
  inertia[8][i] = 10*mass[i];

  rotation[0][i] = 1;
  rotation[1][i] = 0;
  rotation[2][i] = 0;
  rotation[3][i] = 0;
  rotation[4][i] = 1;
  rotation[5][i] = 0;
  rotation[6][i] = 0;
  rotation[7][i] = 0;
  rotation[8][i] = 1;
  
  // invert inertia properties 
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
}

void load_vtk(int mesh, int &nt, int nb, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL &mint, iREAL &maxt)
{
  //////////VTK format////////////
  iREAL min = DBL_MAX;
  iREAL max = DBL_MIN;
  char filename[100];
  
   
  switch(mesh)
  {
    case 0:
    {
      strncpy(filename, "input/mesh/rock", 100);
      break;
    }
    case 1:
    {
      strncpy(filename, "input/mesh/egg", 100);
      break;
    }
  }
  char strtmp[100];
  sprintf(strtmp, "%i.vtk", nb);
  strcat(filename, strtmp);
  FILE *fp1 = fopen(filename, "r+");
  printf("%s\n", filename);

  if( fp1 == NULL )
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }
  
  char ch, word[500];
  iREAL *point[3];
  
  do 
  {
    ch = fscanf(fp1,"%s",word);
    if(strcmp(word, "POINTS")==0)
    {
      printf("found!\n");
      ch = fscanf(fp1,"%s",word);
      int n = atol(word);
      
      ch = fscanf(fp1,"%s",word);
      //printf("will read: %llu\n",n); 
      point[0] = (iREAL *)malloc (n*sizeof(iREAL));
      point[1] = (iREAL *)malloc (n*sizeof(iREAL));
      point[2] = (iREAL *)malloc (n*sizeof(iREAL));
      
      for(int i=0;i<n;i++)
      {
        fscanf(fp1, "%lf", &point[0][i]);
        fscanf(fp1, "%lf", &point[1][i]);
        fscanf(fp1, "%lf", &point[2][i]);
        //printf("POINT[0] = %f | POINT[1] = %f | POINT[2] = %f\n", point[0][i], point[1][i], point[2][i]);
        
        if(point[0][i] < min) 
        {
          min = point[0][i];
        }

        if(point[1][i] < min) 
        {
          min = point[1][i];
        }

        if(point[2][i] < min)
        {
          min = point[2][i];
        }

        /////////////////////
        
        if(point[0][i] > max) 
        {
          max = point[0][i];
        }

        if(point[1][i] > max) 
        {
          max = point[1][i];
        }

        if(point[2][i] > max)
        {
          max = point[2][i];
        }
      }
    }
    if(strcmp(word, "CELLS")==0 || strcmp(word, "POLYGONS") == 0)
    { 
      ch = fscanf(fp1,"%s",word);
      int n = atol(word);
      //nt = n;
      ch = fscanf(fp1,"%s",word);
      printf(":::%u::\n",n);
      for(int i=nt;i<nt+n;i++)
      {
        ch = fscanf(fp1,"%s",word);
        ch = fscanf(fp1,"%s",word);
        
        int index = atol(word);
        t[0][0][i] = point[0][index];
        t[0][1][i] = point[1][index];
        t[0][2][i] = point[2][index];
        
        //printf("idx:%s T[0][0] = %f | T[0][1] = %f | T[0][2] = %f\n", word, t[0][0][i], t[0][1][i], t[0][2][i]);

        //REFERENTIAL POINT A
        t[3][0][i] = point[0][index];
        t[3][1][i] = point[1][index];
        t[3][2][i] = point[2][index];

        ch = fscanf(fp1,"%s",word);
        index = atol(word);
        t[1][0][i] = point[0][index];
        t[1][1][i] = point[1][index];
        t[1][2][i] = point[2][index];
        
        //printf("idx:%s T[1][0] = %f | T[1][1] = %f | T[1][2] = %f\n", word, t[1][0][i], t[1][1][i], t[1][2][i]);
        
        //REFENTIAL POINT B
        t[4][0][i] = point[0][index];
        t[4][1][i] = point[1][index];
        t[4][2][i] = point[2][index];

        ch = fscanf(fp1,"%s",word);
        index = atol(word);
        t[2][0][i] = point[0][index];
        t[2][1][i] = point[1][index];
        t[2][2][i] = point[2][index];
        
        //printf("idx:%s T[2][0] = %f | T[2][1] = %f | T[2][2] = %f\n", word, t[2][0][i], t[2][1][i], t[2][2][i]);
        
        //SPATIAL POINT C
        t[5][0][i] = point[0][index];
        t[5][1][i] = point[1][index];
        t[5][2][i] = point[2][index];
        
        tid[i] = i;
        pid[i] = nb;
      }
      getCentroid(nb, nt, nt+n, t, position);
      nt+=n;
    }
  } while (ch != EOF);
  mint = min;
  maxt = max;
  fclose(fp1);
}

void twoParticleCollision(int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3])
{
  nb = 2;
  int ptype[nb];
  for(int i = 0; i < nb; i++){ptype[i] = 1;}
  
  iREAL mint, maxt;
  for(int i = 0; i < nb; i++)
  {
    switch(ptype[i])
    {
      case 0:
      {
        //create point cloud and do delaunay hull triangulation
        //0.25 eps is the roundness degree, 5 is the radius, 50 are the point of the point cloud
        nonsphericalparticle(0.25, 2.5, 50, nt, i, t, tid, pid, position, mint, maxt);
        iREAL lin[3], ang[3]; iREAL ma;
        if(i == 0)
        {
          lin[2] = 100;
        }
        else 
        {
          lin[2] = 100;
        }
        lin[0] = lin[1] = 0;
        ang[0] = ang[1] = ang[2] = 0;
        ma = 1;
        condition_enviroment(i, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
        break;
      }
      case 1:
      {
        load_vtk(0, nt, i, t, tid, pid, position, mint, maxt);
        iREAL lin[3], ang[3]; iREAL ma;

        lin[1] = lin[2] = 0;
        if(i == 0)
        {
          lin[2] = 100;
        }
        else 
        {
          lin[2] = -100;
        }
        lin[0] = lin[1] = 0;
        ang[0] = ang[1] = ang[2] = 0;
        ma = 1;
        condition_enviroment(i, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
        break;
      }
    }
  }
  
  int radius = 10;

  int idx = 0; lo[0] =0; lo[1] = 0; lo[2] = 0;
  for(int ii = lo[0]; ii < hi[0]; ii=ii+radius)
  {
    for(int jj = lo[1]; jj < hi[1]; jj=jj+radius)
    {
      for(int kk = lo[2]; kk < hi[2]; kk=kk+radius)
      {
        position[0][idx] = ii+(radius/2);
        position[1][idx] = jj+(radius/2);
        position[2][idx] = kk+(radius/2);
        
        position[3][idx] = ii+(radius/2);
        position[4][idx] = jj+(radius/2);
        position[5][idx] = kk+(radius/2);
        
        for(int j = 0; j < nt; j++)
        {
          if(pid[j] == idx)
          {
            translate_enviroment(j, idx, t, position);
          }
        }
        idx++;
        if(idx > nb) break;
      }
      if(idx > nb) break;
    }
    if(idx > nb) break;
  }
}

void oneParticleVsWall(int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3])
{
  nb = 2;
  int ptype[nb];
  ptype[0] = 1;
  ptype[1] = 2;
  
  iREAL wlo[3], whi[3];
  wlo[0] = -10;
  wlo[1] = -10;
  wlo[2] = -10;

  whi[0] = 10;
  whi[1] = 100;
  whi[2] = 10;
    
  iREAL mint, maxt;
  for(int i = 1; i <= nb; i++)
  {
    switch(ptype[i])
    {
      case 0:
      {
        //create point cloud and do delaunay hull triangulation
        //0.25 eps is the roundness degree, 5 is the radius, 50 are the point of the point cloud
        nonsphericalparticle(0.25, 2.5, 50, nt, i, t, tid, pid, position, mint, maxt);
        iREAL lin[3], ang[3]; iREAL ma;

        lin[0] = 100;
        lin[1] = lin[2] = 0;
        ang[0] = ang[1] = ang[2] = 0;

        ma = 1;
        condition_enviroment(i, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
        break;
      }
      case 1:
      {
        load_vtk(0, nt, i, t, tid, pid, position, mint, maxt);
        iREAL lin[3], ang[3]; iREAL ma;

        lin[0] = 100; 
        lin[1] = lin[2] = 0;
        ang[0] = ang[1] = ang[2] = 0;
        ma = 1;
        condition_enviroment(i, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
        break;
      }
      case 2:
      {
        wall(wlo, whi, nt, i, t, tid, pid, position);
        iREAL lin[3], ang[3]; iREAL ma;

        lin[0] = lin[1] = lin[2] = 0;
        ang[0] = ang[1] = ang[2] = 0;
        ma = 500;
        condition_enviroment(i, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
        break;
      }
    }
  }
  
  position[0][0] = -15;
  position[1][0] = 0;
  position[2][0] = 0;
  
  position[3][0] = -15;
  position[4][0] = 0;
  position[5][0] = 0;
  
  for(int j = 0; j < nt; j++)
  {
    if(pid[j] == 0)
    {
      translate_enviroment(j, 0, t, position);
    }
  }
}

void twoParticleVsWall(int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3])
{

}

void chaos(int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3])
{
  nb = 20;
  int ptype[nb];
  for(int i = 0; i < nb; i++){ptype[i] = 0;}
  
  iREAL mint, maxt;
  for(int i = 0; i < nb; i++)
  {
    switch(ptype[i])
    {
      case 0:
      {
        //create point cloud and do delaunay hull triangulation
        //0.25 eps is the roundness degree, 5 is the radius, 50 are the point of the point cloud
        nonsphericalparticle(0.25, 2.5, 50, nt, i, t, tid, pid, position, mint, maxt);
        iREAL lin[3], ang[3]; iREAL ma;

        lin[0] = 100;
        lin[1] = lin[2] = 0;
        ang[0] = ang[1] = ang[2] = 0;
        ma = 1;
        condition_enviroment(i, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
        break;
      }
      case 1:
      {
        load_vtk(0, nt, i, t, tid, pid, position, mint, maxt);
        iREAL lin[3], ang[3]; iREAL ma;

        lin[0] = 100;
        lin[1] = lin[2] = 0;
        ang[0] = ang[1] = ang[2] = 0;
        ma = 1;
        condition_enviroment(i, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
        break;
      }
    }
  }
  
  int radius = 10;

  int idx = 0; lo[0] =0; lo[1] = 0; lo[2] = 0;
  for(int ii = lo[0]; ii < hi[0]; ii=ii+radius)
  {
    for(int jj = lo[1]; jj < hi[1]; jj=jj+radius)
    {
      for(int kk = lo[2]; kk < hi[2]; kk=kk+radius)
      {
        position[0][idx] = ii+(radius/2);
        position[1][idx] = jj+(radius/2);
        position[2][idx] = kk+(radius/2);
        
        position[3][idx] = ii+(radius/2);
        position[4][idx] = jj+(radius/2);
        position[5][idx] = kk+(radius/2);
        
        for(int j = 0; j < nt; j++)
        {
          if(pid[j] == idx)
          {
            translate_enviroment(j, idx, t, position);
          }
        }
        idx++;
        if(idx > nb) break;
      }
      if(idx > nb) break;
    }
    if(idx > nb) break;
  }
}

void eggCollision(int &nt, int &nb, iREAL *t[6][3], 
                    iREAL *linear[3], iREAL *angular[6], 
                    iREAL *inertia[9], iREAL *inverse[9], 
                    iREAL *rotation[9], iREAL *mass, 
                    int *parmat, int tid[], int pid[], 
                    iREAL *position[6], iREAL lo[3], iREAL hi[3])
{
  nb = 2;
  
  iREAL mint, maxt;
  load_vtk(0, nt, 0, t, tid, pid, position, mint, maxt);
  load_vtk(0, nt, 1, t, tid, pid, position, mint, maxt);
  iREAL lin[3], ang[3]; iREAL ma;

  lin[1] = lin[2] = 0;
  lin[2] = -100;
  lin[0] = lin[1] = 0;
  ang[0] = ang[1] = ang[2] = 0;
  ma = 1;
  condition_enviroment(0, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
  lin[2] = -100;
  lin[0] = lin[1] = 0;
  ang[0] = ang[1] = ang[2] = 0;
  ma = 1;
  
  condition_enviroment(1, lin, ang, ma, linear, angular, rotation, mass, inertia, inverse, parmat);
  
  int radius = 10;

  int idx = 0; lo[0] =0; lo[1] = 0; lo[2] = 0;
  for(int ii = lo[0]; ii < hi[0]; ii=ii+radius)
  {
    for(int jj = lo[1]; jj < hi[1]; jj=jj+radius)
    {
      for(int kk = lo[2]; kk < hi[2]; kk=kk+radius)
      {
        position[0][idx] = ii+(radius/2);
        position[1][idx] = jj+(radius/2);
        position[2][idx] = kk+(radius/2);
        
        position[3][idx] = ii+(radius/2);
        position[4][idx] = jj+(radius/2);
        position[5][idx] = kk+(radius/2);
        
        for(int j = 0; j < nt; j++)
        {
          if(pid[j] == idx)
          {
            translate_enviroment(j, idx, t, position);
          }
        }
        idx++;
        if(idx > nb) break;
      }
      if(idx > nb) break;
    }
    if(idx > nb) break;
  }
}
