#include "geom.h"

void refine(int s, int nt, iREAL *t[3][3], int *tid, int *pid, int times);

void getCentroid(int pid, int range1, int range2, iREAL *t[3][3], iREAL *centroid[6])
{
  iREAL cx=0;
  iREAL cy=0;
  iREAL cz=0;

  for(int i=range1;i<range2;i++)
  {
    cx = cx+t[0][0][i];
    cy = cy+t[0][1][i];
    cz = cz+t[0][2][i];
    
    cx = cx+t[1][0][i];
    cy = cy+t[1][1][i];
    cz = cz+t[1][2][i];
    
    cx = cx+t[2][0][i];
    cy = cy+t[2][1][i];
    cz = cz+t[2][2][i];
  }

  cx = cx/((range2-range1)*3);
  cy = cy/((range2-range1)*3);
  cz = cz/((range2-range1)*3);
    
  centroid[0][pid] = cx;
  centroid[1][pid] = cy;
  centroid[2][pid] = cz;
}

void nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, int &nt, int nb, int idx, 
                              iREAL *t[6][3], int tid[], int pid[], iREAL *position[3], iREAL *mint, iREAL *maxt)
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
  
  for(TRI *tri = tr, *e = tri + pointlength; tri < e; tri ++){counter++;}
  
  int n = counter*3;
  nt = pointlength;
  iREAL *point[3];
  
  point[0] = (iREAL *)malloc (n*sizeof(iREAL));
  point[1] = (iREAL *)malloc (n*sizeof(iREAL));
  point[2] = (iREAL *)malloc (n*sizeof(iREAL));
  iREAL min = DBL_MAX;
  iREAL max = DBL_MIN;
  
  counter = 0;
  for(TRI *tri = tr, *e = tri + pointlength; tri < e; tri ++)
  {
    point[0][counter] = tri->ver [0][0];
    point[1][counter] = tri->ver [0][1];
    point[2][counter] = tri->ver [0][2];
   
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
    point[0][counter] = tri->ver [1][0];
    point[1][counter] = tri->ver [1][1];
    point[2][counter] = tri->ver [1][2];
    
    
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
    point[0][counter] = tri->ver [2][0];
    point[1][counter] = tri->ver [2][1];
    point[2][counter] = tri->ver [2][2];
    
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
  
  counter = 0;
  for(int i=idx;i<idx+n;i++)
  {
    //SPATIAL POINT A
    t[0][0][i] = point[0][counter];
    t[0][1][i] = point[1][counter];
    t[0][2][i] = point[2][counter];
  
    counter++;
    //SPATIAL POINT B
    t[1][0][i] = point[0][counter];
    t[1][1][i] = point[1][counter];
    t[1][2][i] = point[2][counter];
    
    counter++;
    //SPATIAL POINT C
    t[2][0][i] = point[0][counter];
    t[2][1][i] = point[1][counter];
    t[2][2][i] = point[2][counter];
    
    counter++;
    
    tid[i] = i;
    pid[i] = nb;
  }
 
  getCentroid(nb, idx, idx+n, t, position);
  
  *mint = min;
  *maxt = max;
}

void createWall(int &n, int &nt, int nb, iREAL *t[3][3], int *tid, int *pid, iREAL A[3], iREAL B[3], iREAL C[3], iREAL D[3])
{
  //create rectangle with two triangles then refine
  //T1 = A,B | 
  t[0][0][nt] = C[0];  
  t[0][1][nt] = C[1]; 
  t[0][2][nt] = C[2];

  t[1][0][nt] = A[0];
  t[1][1][nt] = A[1];
  t[1][2][nt] = A[2];

  t[2][0][nt] = D[0]; 
  t[2][1][nt] = D[1];
  t[2][2][nt] = D[2];
  tid[nt] = nt;
  pid[nt] = nb+1;

  //T2
  t[0][0][nt+1] = A[0]; 
  t[0][1][nt+1] = A[1];
  t[0][2][nt+1] = A[2];

  t[1][0][nt+1] = B[0];
  t[1][1][nt+1] = B[1]; 
  t[1][2][nt+1] = B[2];

  t[2][0][nt+1] = D[0];
  t[2][1][nt+1] = D[1];
  t[2][2][nt+1] = D[2];
  tid[nt+1] = nt+1;
  pid[nt+1] = nb+1;
  n = 2;
}

void refine(int s, int nt, iREAL *t[3][3], int *tid, int *pid, int times)
{
  for(int x = 0; x<times; x++)
  {
    for(int i = s; i<nt; i++)
    {
      iREAL A[3], B[3], C[3];
      A[0] = t[0][0][i];
      A[1] = t[0][1][i];
      A[2] = t[0][2][i];

      B[0] = t[1][0][i];
      B[1] = t[1][1][i];
      B[2] = t[1][2][i];
      
      C[0] = t[2][0][i];
      C[1] = t[2][1][i];
      C[2] = t[2][2][i];
    
      iREAL alpha[3], beta[3], gamma[3];
      iREAL one[3][3], two[3][3], three[3][3], four[3][3];

      alpha[0] = (A[0]+B[0])/2; 
      alpha[1] = (A[1]+B[1])/2;
      alpha[2] = (A[2]+B[2])/2;

      beta[0] = (A[0]+C[0])/2;
      beta[1] = (A[1]+C[1])/2;
      beta[2] = (A[2]+C[2])/2;

      gamma[0] = (C[0]+B[0])/2;
      gamma[1] = (C[1]+B[1])/2;
      gamma[2] = (C[2]+B[2])/2;

      //one
      one[0][0] = A[0];
      one[0][0] = A[1];
      one[0][0] = A[2];
      
      one[1][0] = alpha[0];
      one[1][1] = alpha[1];
      one[1][2] = alpha[2];
      
      one[2][0] = beta[0];
      one[2][1] = beta[1];
      one[2][2] = beta[2];

      //two
      two[0][0] = alpha[0]; 
      two[0][1] = alpha[1]; 
      two[0][2] = alpha[2]; 

      two[1][0] = beta[0]; 
      two[1][1] = beta[1]; 
      two[1][2] = beta[2]; 
      
      two[2][0] = gamma[0]; 
      two[2][1] = gamma[1]; 
      two[2][2] = gamma[2]; 
      
      //three
      three[0][0] = beta[0];
      three[0][1] = beta[1];
      three[0][2] = beta[2];
      
      three[1][0] = gamma[0];
      three[1][1] = gamma[1];
      three[1][2] = gamma[2];
      
      three[2][0] = C[0];
      three[2][1] = C[1];
      three[2][2] = C[2];
      
      //four
      four[0][0] = alpha[0];
      four[0][1] = alpha[1];
      four[0][2] = alpha[2];
      
      four[1][0] = beta[0];
      four[1][1] = beta[1];
      four[1][2] = beta[2];
      
      four[2][0] = gamma[0];
      four[2][1] = gamma[0];
      four[2][2] = gamma[2];
      
      //assing T-one
      t[0][0][i] = one[0][0];
      t[0][1][i] = one[0][1];
      t[0][2][i] = one[0][2];
      
      t[1][0][i] = one[1][0];
      t[1][1][i] = one[1][1];
      t[1][2][i] = one[1][2];
      
      t[2][0][i] = one[2][0];
      t[2][1][i] = one[2][1];
      t[2][2][i] = one[2][2];
      
      int id = nt*(x+1);
      //assing T-two
      tid[id+i] = id+i;
      pid[id+i] = pid[i];
      t[0][0][id+i] = two[0][0];
      t[0][1][id+i] = two[0][1];
      t[0][2][id+i] = two[0][2];
      
      t[1][0][id+i] = two[1][0];
      t[1][1][id+i] = two[1][1];
      t[1][2][id+i] = two[1][2];
      
      t[2][0][id+i] = two[2][0];
      t[2][1][id+i] = two[2][1];
      t[2][2][id+i] = two[2][2];

      //assing T-three
      tid[id+i+1] = id+i+1;
      pid[id+i+1] = pid[i];
      t[0][0][id+i+1] = three[0][0];
      t[0][1][id+i+1] = three[0][1];
      t[0][2][id+i+1] = three[0][2];
      
      t[1][0][id+i+1] = three[1][0];
      t[1][1][id+i+1] = three[1][1];
      t[1][2][id+i+1] = three[1][2];
      
      t[2][0][id+i+1] = three[2][0];
      t[2][1][id+i+1] = three[2][1];
      t[2][2][id+i+1] = three[2][2];

      //assing T-four
      tid[id+i+2] = id+i+2;
      pid[id+i+2] = pid[i];
      t[0][0][id+i+2] = four[0][0];
      t[0][1][id+i+2] = four[0][1];
      t[0][2][id+i+2] = four[0][2];
      
      t[1][0][id+i+2] = four[1][0];
      t[1][1][id+i+2] = four[1][1];
      t[1][2][id+i+2] = four[1][2];
      
      t[2][0][id+i+2] = four[2][0];
      t[2][1][id+i+2] = four[2][1];
      t[2][2][id+i+2] = four[2][2];
    }
    nt += nt;
  }
}
