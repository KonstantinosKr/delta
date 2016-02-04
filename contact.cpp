#include "contact.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <omp.h>

using namespace ispc;

/* allocate new master contact point that can be written to */
master_conpnt * newcon (master_conpnt * master, int *k)
{
  master_conpnt * con = master; 
  while (con->size == CONBUF && con->next != NULL) con = con->next; // find available item or rewind to end
   
  if (con->size < CONBUF)
  {
    *k = con->size++;
  }
  else
  {
    master_conpnt * ptr = new master_conpnt;
    ptr->size = 0;
    ptr->next = NULL;
    con->next = ptr; // append new item at the end 
    con = ptr; // return new item 
    *k = 0;
  }
  return con;
}

/* allocate new slave contact point that can be written to */
slave_conpnt * newcon (slave_conpnt * slave, int *k)
{
  slave_conpnt * con = slave;
 
  while (con->size == CONBUF) con = con->next; /* rewind to the end */
 
  *k = con->size ++; 
  if (con->size == CONBUF)
  {
    slave_conpnt * ptr = new slave_conpnt;
    ptr->size = 0;
    ptr->next = NULL;
    con->next = ptr; /* append new item at the end */
  } 
  
  return con;
}

//s1 and e1 mean start of section 1 and end of section 1, same for s2,e2 and nt size nts1, nts2
void contact_detection (unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2, 
                        iREAL *t[6][3], unsigned int *tid, unsigned int *pid, iREAL *v[3], iREAL dt, 
                        iREAL *p[3], iREAL *q[3], master_conpnt *con)
{
  //unsigned int nts1 = e1-s1;
  //unsigned int nts2 = e2-s2;
  
  iREAL a[3], b[3], c[3];

  omp_set_num_threads(4);//don't know much to set this
  
  //Set triangle 1 points A,B,C
  //#pragma force inline recursive
  //#pragma omp parallel for schedule(dynamic,1) collapse(2)
  for(unsigned int i=s1;i<e1;i++)
  { 
    a[0] = t[0][0][i];
    a[1] = t[0][1][i];
    a[2] = t[0][2][i];
    
    b[0] = t[1][0][i];
    b[1] = t[1][1][i];
    b[2] = t[1][2][i];
    
    c[0] = t[2][0][i];
    c[1] = t[2][1][i];
    c[2] = t[2][2][i];
    
    ispc_bf (s2, e2, a, b, c, t[0], t[1], t[2], p, q);//use tasks 
     
    iREAL marginT1 = 1E-3;
    iREAL marginT2 = 1E-3;
    iREAL margin = marginT1+marginT2;
    
    master_conpnt * conpiv = &con[pid[i]];   
    for(master_conpnt *iter = conpiv; iter!=0; iter=iter->next)
    {//update existing contact points
      for(int j=0;j<iter->size;j++)
      {
        int index = 0;
        index = iter->slave[0][j];
        iREAL dist = sqrt(pow((q[0][index]-p[0][index]),2)+pow((q[1][index]-p[1][index]),2)+pow((q[2][index]-p[1][index]),2));
        iREAL midpt[3];
        midpt[0] = (p[0][index]+q[0][index])/2; //x
        midpt[1] = (p[1][index]+q[1][index])/2; //y
        midpt[2] = (p[2][index]+q[2][index])/2; //z
    
        iREAL depth = dist - margin;
    
        iREAL mul = 1/sqrt(pow(midpt[0],2)+pow(midpt[1],2)+pow(midpt[2],2));
        iREAL normal[3];
        normal[0] = mul*midpt[0];
        normal[1] = mul*midpt[1];
        normal[2] = mul*midpt[2];
        
        iter->point[0][j] = mul*midpt[0];
        iter->point[1][j] = mul*midpt[1];
        iter->point[2][j] = mul*midpt[2];

        iter->normal[0][j] = normal[0];
        iter->normal[1][j] = normal[1];
        iter->normal[2][j] = normal[2];

        iter->depth[j] = depth;
      }
    }

    for(unsigned int j=s2;j<e2;j++) //careful; range can overflow due to ghosts particles
    {
      iREAL dist = sqrt(pow((q[0][j]-p[0][j]),2)+pow((q[1][j]-p[1][j]),2)+pow((q[2][j]-p[1][j]),2));
      //if there is margin overlap or contact point is stored
      if(dist < margin && dist != 0 && (pid[i] != pid[j]))
      {//contact found, //if not same particle body //get min distance contact
        iREAL midpt[3];
        midpt[0] = (p[0][j]+q[0][j])/2; //x
        midpt[1] = (p[1][j]+q[1][j])/2; //y
        midpt[2] = (p[2][j]+q[2][j])/2; //z
    
        iREAL depth = dist - margin;
    
        iREAL mul = 1/sqrt(pow(midpt[0],2)+pow(midpt[1],2)+pow(midpt[2],2));
        iREAL normal[3];
        normal[0] = mul*midpt[0];
        normal[1] = mul*midpt[1];
        normal[2] = mul*midpt[2];
        
        int idx = conpiv->size%CONBUF;//CONBUF set to 8;
        if(idx == 0 && conpiv->size > 0 )
        {
          if(conpiv->next == 0) //conpiv->next makes sure that no next contact point exist from previous contacts
          {
            conpiv->next = (master_conpnt*) malloc(sizeof(master_conpnt));
            conpiv = conpiv->next;
          }
          else
          {
            while(conpiv->next != 0)
            {
              conpiv = conpiv->next;
            }
            //take to first with gap
            idx = conpiv->size%CONBUF;
          }
        }

        conpiv->master[idx] = tid[i];
        conpiv->slave[0][idx] = tid[j];
        
        //store contact point;
        conpiv->point[0][idx] = mul*midpt[0];
        conpiv->point[1][idx] = mul*midpt[1];
        conpiv->point[2][idx] = mul*midpt[2];

        conpiv->normal[0][idx] = normal[0];
        conpiv->normal[1][idx] = normal[1];
        conpiv->normal[2][idx] = normal[2];

        conpiv->depth[idx] = depth;
        
        conpiv->size++; 
      }
    }
  }
}



iREAL sphere_ellipsoid (iREAL p[3], iREAL r, iREAL center[3],
  iREAL radii[3], iREAL orient[9], iREAL point[3], iREAL normal[3], int j)
{
  return 0.0;
}

/* ellipsoid-ellipsoid contact */
iREAL ellipsoid_ellipsoid (iREAL p1[3], iREAL r1[3], iREAL o1[9],
  iREAL p2[3], iREAL r2[3], iREAL o2[9], iREAL point[3], iREAL normal[3], int j)
{
  return 0.0;
}

/* triangle-ellipsoid contact */
iREAL triangle_ellipsoid (iREAL ax, iREAL ay, iREAL az,
  iREAL bx, iREAL by, iREAL bz, iREAL cx, iREAL cy, iREAL cz,
  iREAL p[3], iREAL r[3], iREAL o[9], iREAL point[3], iREAL normal[3], int j)
{
  /* TODO */
  return 0.0;
}

#define clamp(a,b,c) ((a) < (b) ? (b) : (a) > (c) ? (c) : (a))
/* triangle-sphere contact update based on http://www.gamedev.net/topic/552906-closest-point-on-triangle */
inline static iREAL triangle_sphere (iREAL ax, iREAL ay, iREAL az, iREAL bx, iREAL by, iREAL bz, iREAL cx, iREAL cy, iREAL cz,
  iREAL px, iREAL py, iREAL pz, iREAL r, iREAL point[3][CONBUF], iREAL normal[3][CONBUF], int j)
{
  iREAL edge0[3] = {bx-ax, by-ay, bz-az};
  iREAL edge1[3] = {cx-ax, cy-ay, cz-az};
  iREAL v0[3] = {ax-px, ay-py, az-pz};

  iREAL a = DOT(edge0,edge0);
  iREAL b = DOT(edge0,edge1);
  iREAL c = DOT(edge1,edge1);
  iREAL d = DOT(edge0,v0);
  iREAL e = DOT(edge1,v0);

  iREAL det = a*c - b*b;
  iREAL s = b*e - c*d;
  iREAL t = b*d - a*e;

  if (s + t < det)
  {
    if (s < 0.f)
    {
      if (t < 0.f)
      {
	if (d < 0.f)
	{
	  s = clamp(-d/a, 0.f, 1.f);
	  t = 0.f;
	}
	else
	{
	  s = 0.f;
	  t = clamp(-e/c, 0.f, 1.f);
	}
      }
      else
      {
        s = 0.f;
        t = clamp(-e/c, 0.f, 1.f);
      }
    }
    else if (t < 0.f)
    {
      s = clamp(-d/a, 0.f, 1.f);
      t = 0.f;
    }
    else
    {
      iREAL invDet = 1.f / det;
      s *= invDet;
      t *= invDet;
    }
  }
  else
  {
    if (s < 0.f)
    {
      iREAL tmp0 = b+d;
      iREAL tmp1 = c+e;
      if (tmp1 > tmp0)
      {
        iREAL numer = tmp1 - tmp0;
        iREAL denom = a-2*b+c;
        s = clamp(numer/denom, 0.f, 1.f);
        t = 1-s;
      }
      else
      {
        t = clamp(-e/c, 0.f, 1.f);
        s = 0.f;
      }
    }
    else if (t < 0.f)
    {
      if (a+d > b+e)
      {
        iREAL numer = c+e-b-d;
        iREAL denom = a-2*b+c;
        s = clamp(numer/denom, 0.f, 1.f);
        t = 1-s;
      }
      else
      {
        s = clamp(-e/c, 0.f, 1.f);
        t = 0.f;
      }
    }
    else
    {
      iREAL numer = c+e-b-d;
      iREAL denom = a-2*b+c;
      s = clamp(numer/denom, 0.f, 1.f);
      t = 1.f - s;
    }
  }

  point[0][j] = ax + s*edge0[0] + t*edge1[0];
  point[1][j] = ay + s*edge0[1] + t*edge1[1];
  point[2][j] = az + s*edge0[2] + t*edge1[2];
  iREAL q[3] = {px-point[0][j], py-point[1][j], pz-point[2][j]};
  iREAL len = LEN (q);
  iREAL ilen = 1.0/len;
  normal[0][j] = ilen*q[0];
  normal[1][j] = ilen*q[1];
  normal[2][j] = ilen*q[2];
  return r - len;
}

/* update existing contact points */
void update_existing (int span, int parnum, master_conpnt master,
                      iREAL * center[6], iREAL * radii[3],
                      iREAL * orient[18], iREAL * tri[3][3])
{
  for (master_conpnt * con = &master; con; con = con->next)
  {
    for(int k = 0; k<con->size; k++)
    {
      int i = con->master[k];
      int j = con->slave[1][k];

      if (radii[1][i] < 0.0) /* sphere- */
      {
        if (j < 0) /* sphere-triangle */
        {
          int u = -j-1;
          con->depth[k] = triangle_sphere (tri[0][0][u], tri[0][1][u], tri[0][2][u],
                                         tri[1][0][u], tri[1][1][u], tri[1][2][u],
                 tri[2][0][u], tri[2][1][u], tri[2][2][u],
                 center[0][i], center[1][i], center[2][i],
                 radii[0][i], con->point, con->normal, k);
        }
        else if (radii[1][j] < 0.0) /* sphere-sphere */
        {
          iREAL p[3], q[3], c[3], len, ilen;

          p[0] = center[0][i];
          p[1] = center[1][i];
          p[2] = center[2][i];
          c[0] = center[0][j];
          c[1] = center[1][j];
          c[2] = center[2][j];
          q[0] = p[0]-c[0];
          q[1] = p[1]-c[1];
          q[2] = p[2]-c[2];
          len = LEN(q);
          ilen = 1.0/len;
          con->point[0][k] = 0.5*(p[0]+c[0]);
          con->point[1][k] = 0.5*(p[1]+c[1]); 
          con->point[2][k] = 0.5*(p[2]+c[2]); 
          con->normal[0][k] = ilen*q[0];
          con->normal[1][k] = ilen*q[1];
          con->normal[2][k] = ilen*q[2];
          con->depth[k] = radii[0][i]+radii[0][j] - len;
        }
        else /* sphere-ellipsoid */
        {
          return;
        }
      }
      else /* ellipsoid- */
      {
        if (j < 0) /* ellipsoid-triangle */
        {
          return; /* TODO */
        }
        else if (radii[1][j] < 0.0) /* ellipsoid-sphere */
        {
          return; /* TODO */
        }
        else /* ellipsoid-ellipsoid */
        {
          return; /* TODO */
        }
      }
    }
  }
}

