#include "input.h" 
#include <float.h>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>

void condition_enviroment(int nb, iREAL *linear[3], iREAL *angular[6], iREAL *rotation[9],
                        iREAL *mass, iREAL *inertia[9], iREAL *inverse[9], int *parmat)
{
  for(int i = 0; i < nb; i++)
  {
    //iREAL rand = drand48();//random pull velocity
    
    //v[0][i] = 500 * rand;
    //v[1][i] = 500 * rand;
    //v[2][i] = 500 * rand;
    if(i == 0)
    {
      linear[0][i] = 0;
      linear[1][i] = 0;
      linear[2][i] = 100;
    }else
    {
      linear[0][i] = 0;
      linear[1][i] = 0;
      linear[2][i] = 0;
    }

    //iREAL randomrot = 5 *rand;
    angular[0][i] = 0;
    angular[1][i] = 0;
    angular[2][i] = 0;
    angular[3][i] = 0; 
    angular[4][i] = 0;
    angular[5][i] = 0;
   
    mass[i] = 15;
    parmat[i] = 0;
    
    //double volume = (4./3.)*M_PI*rad*rad*rad;

    //mass[i] = volume*mparam[DENSITY][material];

    inertia[0][i] = 15*mass[i];
    inertia[1][i] = 0;
    inertia[2][i] = 0;
    inertia[3][i] = 0;
    inertia[4][i] = 15*mass[i];
    inertia[5][i] = 0;
    inertia[6][i] = 0;
    inertia[7][i] = 0;
    inertia[8][i] = 15*mass[i];

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
}

void init_enviroment(int &nt, int &nb, iREAL *t[6][3], iREAL *linear[3], iREAL *angular[6], iREAL *inertia[9], iREAL *inverse[9], iREAL *rotation[9], iREAL *mass, int *parmat, int tid[], int pid[], iREAL *position[6], iREAL lo[3], iREAL hi[3])
{
    
  //non-spherical particles generation and loading
  nb = 2;
  int ptype[nb];
  for(int i = 0; i < nb; i++){ptype[i] = 0;}
  
  iREAL mint, maxt;
  load_enviroment(ptype, nt, nb, t, tid, pid, position, &mint, &maxt);
  
  lo[0] = -250; // lower corner
  lo[1] = -250; // lower corner
  lo[2] = -250; // lower corner
  
  hi[0] = 250; // upper corner
  hi[1] = 250; // upper corner
  hi[2] = 250; // upper corner
  
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
        
        //compute position to translate
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

  condition_enviroment(nb, linear, angular, rotation, mass, inertia, inverse, parmat);
}

void load_enviroment(int ptype[], int &nt, int nb, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt)
{
  int n = 0;
  nt = 0;
  srand48(time(NULL));
  for(int i = 0; i < nb; i++)
  {
    switch(ptype[i])
    {
      case 0:
      {
        printf("entered\n");
        load_vtk(n, i, nt, t, tid, pid, position, mint, maxt);
        break;
      }
      case 1:
      {
        //create point cloud and do delaunay hull triangulation
        //0.25 eps is the roundness degree, 5 is the radius, 50 are the point of the point cloud
        nonsphericalparticle(0.25, 2.5, 50, n, i, nt, t, tid, pid, position, mint, maxt);
        break;
      }
    }
    nt = n + nt;
    n = 0;
  }
}

void load_vtk(int &nt, int nb, int idx, iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL *mint, iREAL *maxt)
{
  //////////VTK format////////////
  iREAL min = DBL_MAX;
  iREAL max = DBL_MIN;
  FILE *fp1;
  char filename[100] = "input/mesh/mesh";
  char strtmp[100];
  sprintf(strtmp, "%i.vtk", nb);
  strcat(filename, strtmp);
  fp1 = fopen(filename, "r+");
  printf("%s\n", filename);

  if( fp1 == NULL )
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }
  
  char ch, word[500];
  iREAL *point[3];
  
  do {
      ch = fscanf(fp1,"%s",word);
      if(strcmp(word, "POINTS")==0)
      {
        printf("found!\n");
        ch = fscanf(fp1,"%s",word);
        int n = atol(word);
        //get points
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
        nt = n;
        ch = fscanf(fp1,"%s",word);
        printf(":::%u::\n",n);
        for(int i=idx;i<idx+n;i++)
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
        getCentroid(nb, idx, idx+n, t, position);
      }
  } while (ch != EOF);
  *mint = min;
  *maxt = max;
  fclose(fp1);
}


