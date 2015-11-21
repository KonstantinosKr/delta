#include "input.h" 

void translate_enviroment(unsigned int tid, iREAL *t[3][3], iREAL p[3])
{
  t[0][0][tid] = t[0][0][tid] + p[0];
  t[0][1][tid] = t[0][1][tid] + p[1];
  t[0][2][tid] = t[0][2][tid] + p[2];

  t[1][0][tid] = t[1][0][tid] + p[0];
  t[1][1][tid] = t[1][1][tid] + p[1];
  t[1][2][tid] = t[1][2][tid] + p[2];

  t[2][0][tid] = t[2][0][tid] + p[0];
  t[2][1][tid] = t[2][1][tid] + p[1];
  t[2][2][tid] = t[2][2][tid] + p[2];
}

void condition_enviroment(unsigned int nt, unsigned int nParticles, iREAL *v[3], unsigned int pid[])
{
  unsigned int counter=0;
  for(int j = 0; j < nParticles; j++)
  {
    iREAL rand = drand48();//random pull velocity
    for(unsigned int i = counter; i < nt; i++)
    {
      if(pid[i] == j)
      {
        v[0][i] = 250 * rand;
        v[1][i] = 0;
        v[2][i] = 0;
        counter++;
      } else
      {
        break;
      }
    }
  }
}

void init_enviroment(unsigned int *nt, unsigned int *nParticles, iREAL *t[3][3], iREAL *v[3], unsigned int tid[], unsigned int pid[], iREAL lo[3], iREAL hi[3])
{
  //Input Type
  //0: Triangulated Mesh
  //1: Particle
  //2: Triangle
  //3: Sphere
  //4: Square
  //5: Hexahedron
  
    
  //load non-spherical meshes from vtk files
  /* *nParticles = 1000;
  int ptype[*nParticles];
  for(int i = 0; i < *nParticles; i++)
  {
    ptype[i] = 1;
  }*/
   
   
  //non-spherical particles generation and loading
  *nParticles = 1000;
  int ptype[*nParticles];
  for(int i = 0; i < *nParticles; i++){ptype[i] = 6;}
   
  iREAL mint, maxt;
  load_enviroment(ptype, nt, *nParticles, t, tid, pid, &mint, &maxt);
  printf("ntinit:%i\n",*nt); 
  iREAL velo[3] = {50, 50, 50};
  lo[0] = -250; // lower corner
  lo[1] = -250; // lower corner
  lo[2] = -250; // lower corner
  
  hi[0] = 250; // upper corner
  hi[1] = 250; // upper corner
  hi[2] = 250; // upper corner
  
  //gen_velocities(lo, velo, *nt, v);
  
  iREAL p[3];//position to be translated
  int radius = 10;

  int counter = 0;
  int idx = 0;
  for(int ii = lo[0]; ii < hi[0]; ii=ii+radius)
  {
    for(int jj = lo[1]; jj < hi[1]; jj=jj+radius)
    {
      for(int kk = lo[2]; kk < hi[2]; kk=kk+radius)
      {
        if(idx < *nParticles)
        {
          //computer position to translate
          for(int j = counter; j < *nt; j++)
          {
            if(pid[j] == idx)
            {
              p[0] = ii+(radius/2);
              p[1] = jj+(radius/2);
              p[2] = kk+(radius/2);
              translate_enviroment(j, t, p);
              counter++;
            }
          }
          idx++;
        }
      }
    }
  }
  condition_enviroment(*nt, *nParticles, v, pid);
}


void load_enviroment(int ptype[], unsigned int *nt, unsigned int nParticles, iREAL *t[3][3], unsigned int tid[], unsigned int pid[], iREAL *mint, iREAL *maxt)
{
  unsigned int n = 0;
  *nt = 0;
  srand48(time(NULL));
  for(unsigned int i = 0; i < nParticles; i++)
  {
    if(ptype[i] != 6)
    {//load particle from file
        load_points(ptype[i], &n, i, *nt, t, tid, pid, mint, maxt);
    } else
    {//create point cloud and do delaunay hull triangulation
        //0.25 eps is the roundness degree, 5 is the radius, 50 are the point of the point cloud
        gen_nonsphericalparticle(0.25, 5, 50, &n, i, *nt, t, tid, pid, mint, maxt);
    }
    printf("n:%i\n", n);
    *nt = n + *nt;
    n = 0;
  }
  printf("loadnt:%i\n", *nt);
}

void gen_nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, unsigned int *nt, unsigned int bodyidx, unsigned int initidx, iREAL *t[3][3], unsigned int tid[], unsigned int pid[], iREAL *mint, iREAL *maxt)
{
  iREAL v[100][3];
  for(int i = 0; i<pointsize; i++)
  {
    iREAL rng1 = 0;
    iREAL rng2 = 360;
    iREAL phi = (rng2-rng1) * (drand48()) + rng1;
    iREAL theta = (rng2-rng1) * (drand48()) + rng1;
    iREAL myradius = ((drand48()*eps)+1.0-eps) * radius;
            
    v[i][0] = myradius*sin(phi) * cos(theta);
    v[i][1] = myradius*sin(phi) * sin(theta);
    v[i][2] = myradius*cos(phi);
  }
    
    TRI* tr = NULL;
    free(tr);int pointlength = 0;
    tr = hull((iREAL *)v, pointsize, &pointlength);
    int counter = 0;
    
    for(TRI *t = tr, *e = t + pointlength; t < e; t ++){counter++;}
    
    unsigned int n = counter*3;
    *nt = pointlength;
    iREAL *point[3];
    
    point[0] = (iREAL *)malloc (n*sizeof(iREAL));
    point[1] = (iREAL *)malloc (n*sizeof(iREAL));
    point[2] = (iREAL *)malloc (n*sizeof(iREAL));
    iREAL min = DBL_MAX;
    iREAL max = DBL_MIN;
    
    counter = 0;
    for(TRI *t = tr, *e = t + pointlength; t < e; t ++)
    {
        iREAL p[3];
       
        point[0][counter] = t->ver [0][0];
        point[1][counter] = t->ver [0][1];
        point[2][counter] = t->ver [0][2];
       
        if(point[0][counter] < min)
        {
            min = point[0][counter];
        }
        
        if(point[1][counter] < min)
        {
            min = point[1][counter];
        }
        
        if(point[2][counter] < min)
        {
            min = point[2][counter];
        }
        
        /////////////////////
        
        if(point[0][counter] > max)
        {
            max = point[0][counter];
        }
        
        if(point[1][counter] > max)
        {
            max = point[1][counter];
        }
        
        if(point[2][counter] > max)
        {
            max = point[2][counter];
        }
        
        counter++;
        point[0][counter] = t->ver [1][0];
        point[1][counter] = t->ver [1][1];
        point[2][counter] = t->ver [1][2];
        
        
        if(point[0][counter] < min)
        {
            min = point[0][counter];
        }
        
        if(point[1][counter] < min)
        {
            min = point[1][counter];
        }
        
        if(point[2][counter] < min)
        {
            min = point[2][counter];
        }
        
        /////////////////////
        
        if(point[0][counter] > max)
        {
            max = point[0][counter];
        }
        
        if(point[1][counter] > max)
        {
            max = point[1][counter];
        }
        
        if(point[2][counter] > max)
        {
            max = point[2][counter];
        }
        
        counter++;
        point[0][counter] = t->ver [2][0];
        point[1][counter] = t->ver [2][1];
        point[2][counter] = t->ver [2][2];
        
        if(point[0][counter] < min)
        {
            min = point[0][counter];
        }
        
        if(point[1][counter] < min)
        {
            min = point[1][counter];
        }
        
        if(point[2][counter] < min)
        {
            min = point[2][counter];
        }
        
        /////////////////////
        
        if(point[0][counter] > max)
        {
            max = point[0][counter];
        }
        
        if(point[1][counter] > max)
        {
            max = point[1][counter];
        }
        
        if(point[2][counter] > max)
        {
            max = point[2][counter];
        }
        
        counter++;

    }
    
    printf(":::%u::\n",pointlength);
    counter = 0;
    for(unsigned int i=initidx;i<initidx+n;i++)
    {
        t[0][0][i] = point[0][counter];
        t[0][1][i] = point[1][counter];
        t[0][2][i] = point[2][counter];
        
        counter++;
        t[1][0][i] = point[0][counter];
        t[1][1][i] = point[1][counter];
        t[1][2][i] = point[2][counter];
        
        counter++;
        t[2][0][i] = point[0][counter];
        t[2][1][i] = point[1][counter];
        t[2][2][i] = point[2][counter];
        counter++;
        
        tid[i] = i;
        pid[i] = bodyidx;
    }
    *mint = min;
    *maxt = max;
}


void load_points(int ptype, unsigned int *nt, unsigned int bodyID, unsigned int startIDX, iREAL *t[3][3], unsigned int tid[], unsigned int pid[], iREAL *mint, iREAL *maxt)
{
  //////////VTK format////////////

  //Input Type
  //0: Triangulated Mesh
  //1: Particle
  //2: Triangle
  //3: Sphere
  //4: Square
  //5: Hexahedron
  //6: point cloud 
  iREAL min = DBL_MAX;
  iREAL max = DBL_MIN;
  FILE *fp1;
  if(ptype == 0)
  {
    char filename[100] = "input/mesh/mesh";
    char strtmp[100];
    sprintf(strtmp, "%i.vtk", bodyID);
    strcat(filename, strtmp);
    fp1 = fopen(filename, "r+");
    printf("%s\n", filename);
  } else if(ptype == 1)
  {
    char filename[100] = "input/particles/par_";
    char strtmp[100];
    sprintf(strtmp, "%i.vtk", bodyID);
    strcat(filename, strtmp);
    fp1 = fopen(filename, "r+");
    printf("%s\n", filename);
  } else if(ptype == 2)
  {
    fp1 = fopen("input/shapes/triangle.vtk", "r");
  } else if(ptype == 3)
  {
    fp1 = fopen("input/shapes/sphere.vtk", "r");
  } else if(ptype == 4)
  {
    fp1 = fopen("input/shapes/square.vtk", "r");
  } else if(ptype == 5)
  {
    fp1 = fopen("input/shapes/hexahedron.vtk", "r");
  }

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
          *nt = n;
          ch = fscanf(fp1,"%s",word);
          printf(":::%u::\n",n);
          for(unsigned int i=startIDX;i<startIDX+n;i++)
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
              pid[i] = bodyID;
          }
      }
  } while (ch != EOF);
  *mint = min;
  *maxt = max;
  fclose(fp1);
}

void normalize(unsigned int nt, iREAL *t[3][3], iREAL mint, iREAL maxt) 
{  
  //range -255 to 255
  iREAL inv_range = 510.0/(maxt-mint);

  for(unsigned int i=0; i < nt; i++)
  {
    for(int j=0; j < 3; j++)
    {
      for(int k=0; k < 3; k++)
      {
        t[j][k][i] = -255.0 + (t[j][k][i] - mint) * inv_range;
      }
    }
  }
}
