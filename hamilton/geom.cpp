#include "geom.h"
#include "stdio.h"

void getCentroid(int pid, int range1, int range2, iREAL *t[6][3], iREAL *centroid[6])
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
  
  centroid[3][pid] = cx;
  centroid[4][pid] = cy;
  centroid[5][pid] = cz;
}

void translate_enviroment(int i, int pid, iREAL *t[6][3], iREAL *p[6])
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
  
  t[3][0][i] = t[3][0][i] + p[0][pid];
  t[3][1][i] = t[3][1][i] + p[1][pid];
  t[3][2][i] = t[3][2][i] + p[2][pid];
  
  t[4][0][i] = t[4][0][i] + p[0][pid];
  t[4][1][i] = t[4][1][i] + p[1][pid];
  t[4][2][i] = t[4][2][i] + p[2][pid];
  
  t[5][0][i] = t[5][0][i] + p[0][pid];
  t[5][1][i] = t[5][1][i] + p[1][pid];
  t[5][2][i] = t[5][2][i] + p[2][pid];
}

void normalize(int nt, iREAL *t[6][3], iREAL mint, iREAL maxt)
{
    //range -255 to 255
    iREAL inv_range = 510.0/(maxt-mint);
    
    for(int i=0; i < nt; i++)
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

void nonsphericalparticle(iREAL eps, iREAL radius, int pointsize, int &nt, int nb,
                          iREAL *t[6][3], int tid[], int pid[], iREAL *position[6], iREAL &mint, iREAL &maxt)
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
  int idx = nt;
  nt += pointlength;
  
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
    
    //REFERENTIAL POINT A
    t[3][0][i] = point[0][counter];
    t[3][1][i] = point[1][counter];
    t[3][2][i] = point[2][counter];
    
    counter++;
    //SPATIAL POINT B
    t[1][0][i] = point[0][counter];
    t[1][1][i] = point[1][counter];
    t[1][2][i] = point[2][counter];
    
    //REFENTIAL POINT B
    t[4][0][i] = point[0][counter];
    t[4][1][i] = point[1][counter];
    t[4][2][i] = point[2][counter];
    
    counter++;
    //SPATIAL POINT C
    t[2][0][i] = point[0][counter];
    t[2][1][i] = point[1][counter];
    t[2][2][i] = point[2][counter];
    
    //SPATIAL POINT C
    t[5][0][i] = point[0][counter];
    t[5][1][i] = point[1][counter];
    t[5][2][i] = point[2][counter];
    
    counter++;
    
    tid[i] = i;
    pid[i] = nb;
  }
  
  getCentroid(nb, idx, idx+n, t, position);
  
  mint = min;
  maxt = max;
}


void wall(iREAL lo[3], iREAL hi[3], int &nt, int nb, iREAL *t[6][3], int *tid, int *pid, iREAL *position[6])
{
    iREAL leftUI[3], leftUO[3], leftDI[3], leftDO[3];
    iREAL rightUI[3], rightUO[3], rightDI[3], rightDO[3];
    
    //left down inner
    leftDI[0] = lo[0];
    leftDI[1] = lo[1];
    leftDI[2] = lo[2];
    
    leftDO[0] = lo[0];
    leftDO[1] = lo[1];
    leftDO[2] = hi[2];
    
    leftUI[0] = lo[0];
    leftUI[1] = hi[1];
    leftUI[2] = lo[2];
    
    leftUO[0] = lo[0];
    leftUO[1] = hi[1];
    leftUO[2] = hi[2];
    
    rightDI[0] = hi[0];
    rightDI[1] = lo[1];
    rightDI[2] = lo[2];
    
    rightDO[0] = hi[0];
    rightDO[1] = lo[1];
    rightDO[2] = hi[2];
    
    rightUI[0] = hi[0];
    rightUI[1] = hi[1];
    rightUI[2] = lo[2];
    
    rightUO[0] = hi[0];
    rightUO[1] = hi[1];
    rightUO[2] = hi[2];
    
    //for each plane put two triangles
    //plane left
    t[0][0][nt] = leftDI[0];
    t[0][1][nt] = leftDI[1];
    t[0][2][nt] = leftDI[2];
    
    t[1][0][nt] = leftDO[0];
    t[1][1][nt] = leftDO[1];
    t[1][2][nt] = leftDO[2];
    
    t[2][0][nt] = leftUI[0];
    t[2][1][nt] = leftUI[1];
    t[2][2][nt] = leftUI[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;
    
    t[0][0][nt] = leftDO[0];
    t[0][1][nt] = leftDO[1];
    t[0][2][nt] = leftDO[2];
    
    t[1][0][nt] = leftUO[0];
    t[1][1][nt] = leftUO[1];
    t[1][2][nt] = leftUO[2];
    
    t[2][0][nt] = leftUI[0];
    t[2][1][nt] = leftUI[1];
    t[2][2][nt] = leftUI[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;

    //plane right
    t[0][0][nt] = rightDI[0];
    t[0][1][nt] = rightDI[1];
    t[0][2][nt] = rightDI[2];
    
    t[1][0][nt] = rightDO[0];
    t[1][1][nt] = rightDO[1];
    t[1][2][nt] = rightDO[2];
    
    t[2][0][nt] = rightUI[0];
    t[2][1][nt] = rightUI[1];
    t[2][2][nt] = rightUI[2];
 
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;
    
    t[0][0][nt] = rightDO[0];
    t[0][1][nt] = rightDO[1];
    t[0][2][nt] = rightDO[2];
    
    t[1][0][nt] = rightUO[0];
    t[1][1][nt] = rightUO[1];
    t[1][2][nt] = rightUO[2];
    
    t[2][0][nt] = rightUI[0];
    t[2][1][nt] = rightUI[1];
    t[2][2][nt] = rightUI[2];

    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;

    //plane front
    t[0][0][nt] = leftDO[0];
    t[0][1][nt] = leftDO[1];
    t[0][2][nt] = leftDO[2];
    
    t[1][0][nt] = rightDO[0];
    t[1][1][nt] = rightDO[1];
    t[1][2][nt] = rightDO[2];
    
    t[2][0][nt] = rightUO[0];
    t[2][1][nt] = rightUO[1];
    t[2][2][nt] = rightUO[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;
    
    t[0][0][nt] = rightUO[0];
    t[0][1][nt] = rightUO[1];
    t[0][2][nt] = rightUO[2];
    
    t[1][0][nt] = leftDO[0];
    t[1][1][nt] = leftDO[1];
    t[1][2][nt] = leftDO[2];
    
    t[2][0][nt] = leftUO[0];
    t[2][1][nt] = leftUO[1];
    t[2][2][nt] = leftUO[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;

    //plane inner
    t[0][0][nt] = leftDI[0];
    t[0][1][nt] = leftDI[1];
    t[0][2][nt] = leftDI[2];
    
    t[1][0][nt] = rightDI[0];
    t[1][1][nt] = rightDI[1];
    t[1][2][nt] = rightDI[2];
    
    t[2][0][nt] = rightUI[0];
    t[2][1][nt] = rightUI[1];
    t[2][2][nt] = rightUI[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;
    
    t[0][0][nt] = rightUI[0];
    t[0][1][nt] = rightUI[1];
    t[0][2][nt] = rightUI[2];
    
    t[1][0][nt] = leftDI[0];
    t[1][1][nt] = leftDI[1];
    t[1][2][nt] = leftDI[2];
    
    t[2][0][nt] = leftUI[0];
    t[2][1][nt] = leftUI[1];
    t[2][2][nt] = leftUI[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;

    //plane down
    t[0][0][nt] = leftDI[0];
    t[0][1][nt] = leftDI[1];
    t[0][2][nt] = leftDI[2];
    
    t[1][0][nt] = rightDI[0];
    t[1][1][nt] = rightDI[1];
    t[1][2][nt] = rightDI[2];
    
    t[2][0][nt] = rightDO[0];
    t[2][1][nt] = rightDO[1];
    t[2][2][nt] = rightDO[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;
    
    t[0][0][nt] = rightDO[0];
    t[0][1][nt] = rightDO[1];
    t[0][2][nt] = rightDO[2];
    
    t[1][0][nt] = leftDO[0];
    t[1][1][nt] = leftDO[1];
    t[1][2][nt] = leftDO[2];
    
    t[2][0][nt] = leftDI[0];
    t[2][1][nt] = leftDI[1];
    t[2][2][nt] = leftDI[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;
    
    //plane up
    t[0][0][nt] = leftUI[0];
    t[0][1][nt] = leftUI[1];
    t[0][2][nt] = leftUI[2];
    
    t[1][0][nt] = rightUI[0];
    t[1][1][nt] = rightUI[1];
    t[1][2][nt] = rightUI[2];
    
    t[2][0][nt] = rightUO[0];
    t[2][1][nt] = rightUO[1];
    t[2][2][nt] = rightUO[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;
    
    t[0][0][nt] = rightUO[0];
    t[0][1][nt] = rightUO[1];
    t[0][2][nt] = rightUO[2];
    
    t[1][0][nt] = leftUO[0];
    t[1][1][nt] = leftUO[1];
    t[1][2][nt] = leftUO[2];
    
    t[2][0][nt] = leftUI[0];
    t[2][1][nt] = leftUI[1];
    t[2][2][nt] = leftUI[2];
    
    t[3][0][nt] = t[0][0][nt];
    t[3][1][nt] = t[0][1][nt];
    t[3][2][nt] = t[0][2][nt];
    
    t[4][0][nt] = t[1][0][nt];
    t[4][1][nt] = t[1][1][nt];
    t[4][2][nt] = t[1][2][nt];
    
    t[5][0][nt] = t[2][0][nt];
    t[5][1][nt] = t[2][1][nt];
    t[5][2][nt] = t[2][2][nt];
    tid[nt] = nt;
    pid[nt] = nb;
    nt++;

    getCentroid(nb, nt-13, nt, t, position);
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
