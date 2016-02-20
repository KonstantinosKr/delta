#include "input.h" 
#include <float.h>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>

void init_env(int &nt, int &nb, iREAL *t[3][3], iREAL *linear[3], iREAL *mass, int *parmat, int tid[], int pid[], iREAL *position[3], iREAL lo[3], iREAL hi[3])
{
  //Input Type
  //0: Triangulated Mesh
  //1: wall 
  //2: Sphere
   
  //non-spherical particles generation and loading
  nb = 2;
  int ptype[nb];
  for(int i = 0; i < nb; i++){ptype[i] = 0;}
  //ptype[*nb] = 6;
  //*nb +=1;

  iREAL mint, maxt;
  load_env(ptype, nt, nb, t, tid, pid, position, &mint, &maxt);
  
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
        
        //compute position to translate
        for(int j = 0; j < nt; j++)
        {
          if(pid[j] == idx)
          {
            trans_env(j, idx, t, position);
          }
        }
        idx++;
        if(idx > nb) break;
      }
      if(idx > nb) break;
    }
    if(idx > nb) break;
  }
  cond_env(nb, linear, mass, parmat);
}

void load_env(int ptype[], int &nt, int &nb, iREAL *t[3][3], int tid[], int pid[], iREAL *position[3], iREAL *mint, iREAL *maxt)
{
  int n = 0;
  nt = 0;
  srand48(time(NULL));
  for(int i = 0; i < nb; i++)
  {
    switch(ptype[i])
    {
      case 0:
      {//load particle from file
        load_vtk(n, i, nt, t, tid, pid, position, mint, maxt);
        break;
      }
      case 1:
      {//create point cloud and do delaunay hull triangulation
        //0.25 eps is the roundness degree, 5 is the radius, 50 are the point of the point cloud
        nonsphericalparticle(0.25, 2.5, 50, n, i, nt, t, tid, pid, position, mint, maxt);
        break;
      }
      case 2:
      {
        iREAL A[3], B[3], C[3], D[3];
        
        A[0] = -250;
        A[1] = 250;
        A[2] = -50;

        B[0] = 250;
        B[1] = 250;
        B[2] = -50;

        C[0] = -250;
        C[1] = -250;
        C[2] = -50;

        D[0] = 250;
        D[1] = -250;
        D[2] = -50;
        createWall(n, nt, nb, t, tid, pid, A, B, C, D);
        break;
      }
    }
    nt = n + nt;
    n = 0;
  }
}

void trans_env(int i, int pid, iREAL *t[3][3], iREAL *p[3])
{ 
  t[0][0][i] = t[0][0][i] + p[0][pid];
  t[0][1][i] = t[0][1][i] + p[1][pid];
  t[0][2][i] = t[0][2][i] + p[2][pid];
   
  t[1][0][i] = t[1][0][i] + p[0][pid];
  t[1][1][i] = t[1][1][i] + p[1][pid];
  t[1][2][i] = t[1][2][i] + p[2][pid];
     
  t[2][0][i] = t[2][0][i] + p[0][pid];
  t[2][1][i] = t[2][1][i] + p[1][pid];
  t[2][2][i] = t[2][2][i] + p[2][pid];
} 

void cond_env(int nb, iREAL *linear[3], iREAL *mass, int *parmat)
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

    mass[i] = 0.5*0.4;
    parmat[i] = 0;
    
    //invm[i] = 1.0 / mass[i];
  }
}

void load_vtk(int &nt, int &nb, int IDX, iREAL *t[3][3], int tid[], int pid[], iREAL *position[3], iREAL *mint, iREAL *maxt)
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
        unsigned int n = atol(word);
        //get points
        ch = fscanf(fp1,"%s",word);
        //printf("will read: %llu\n",n); 
        point[0] = (iREAL *)malloc (n*sizeof(iREAL));
        point[1] = (iREAL *)malloc (n*sizeof(iREAL));
        point[2] = (iREAL *)malloc (n*sizeof(iREAL));
        
        for(unsigned int i=0;i<n;i++)
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
        unsigned int n = atol(word);
        nt = n;
        ch = fscanf(fp1,"%s",word);
        printf(":::%u::\n",n);
        for(unsigned int i=IDX;i<IDX+n;i++)
        {
          ch = fscanf(fp1,"%s",word);
          ch = fscanf(fp1,"%s",word);
          
          unsigned int index = atol(word);
          t[0][0][i] = point[0][index];
          t[0][1][i] = point[1][index];
          t[0][2][i] = point[2][index];
          
          //printf("idx:%s T[0][0] = %f | T[0][1] = %f | T[0][2] = %f\n", word, t[0][0][i], t[0][1][i], t[0][2][i]);
          
          ch = fscanf(fp1,"%s",word);
          index = atol(word);
          t[1][0][i] = point[0][index];
          t[1][1][i] = point[1][index];
          t[1][2][i] = point[2][index];
          
          //printf("idx:%s T[1][0] = %f | T[1][1] = %f | T[1][2] = %f\n", word, t[1][0][i], t[1][1][i], t[1][2][i]);
          
          ch = fscanf(fp1,"%s",word);
          index = atol(word);
          t[2][0][i] = point[0][index];
          t[2][1][i] = point[1][index];
          t[2][2][i] = point[2][index];
          
          //printf("idx:%s T[2][0] = %f | T[2][1] = %f | T[2][2] = %f\n", word, t[2][0][i], t[2][1][i], t[2][2][i]);
          
          tid[i] = i;
          pid[i] = nb;
        }
        //getCentroid(nb, IDX, IDX+n, t, position);
      }
  } while (ch != EOF);
  *mint = min;
  *maxt = max;
  fclose(fp1);
}

