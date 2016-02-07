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

/* free global array of master contact points */
void master_free (master_conpnt * con, int size)
{
  for (int i = 0; i < size; i ++)
  {
    master_conpnt * ptr = con[i].next;
    while (ptr)
    {
      master_conpnt * next = ptr->next;
      delete ptr;
      ptr = next;
    }
  }
 
  delete con;
}

/* free global array of slave contact points */
void slave_free (slave_conpnt * con, int size)
{
  for (int i = 0; i < size; i ++)
  {
    slave_conpnt * ptr = con[i].next;
    while (ptr)
    {
      slave_conpnt * next = ptr->next;
      delete ptr;
      ptr = next;
    }
  } 
  delete con;
}

//s1 and e1 mean start of section 1 and end of section 1, same for s2,e2 and nt size nts1, nts2
void contact_detection (unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2, 
                        iREAL *t[6][3], unsigned int *tid, unsigned int *pid, iREAL *v[3], 
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
     
    iREAL margin = 10E-2;
    
    for(unsigned int j=s2;j<e2;j++) //careful; range can overflow due to ghosts particles
    {
      iREAL dist = sqrt(pow((q[0][j]-p[0][j]),2)+pow((q[1][j]-p[1][j]),2)+pow((q[2][j]-p[1][j]),2));
      //if there is margin overlap or contact point is stored
      if(dist < margin  && (pid[i] != pid[j]))
      {//contact found, //if not same particle body //get min distance contact
        iREAL midpt[3], normal[3];
        
        midpt[0] = (p[0][j]+q[0][j])/2; //x
        midpt[1] = (p[1][j]+q[1][j])/2; //y
        midpt[2] = (p[2][j]+q[2][j])/2; //z
    
        iREAL depth = margin-dist;
        printf("DIST:%f DEPTH:%f JJ:%i\n", dist, depth, j);   
        //iREAL mul = 1/sqrt(pow(midpt[0],2)+pow(midpt[1],2)+pow(midpt[2],2));
        iREAL mul = sqrt(pow(q[0][j]-p[0][j], 2)+pow(q[1][j] - p[1][j], 2)+pow(q[2][j]-p[2][j], 2));
        normal[0] = ((q[0][j] - p[0][j])/mul);//*depth for inclusion to normal
        normal[1] = ((q[1][j] - p[1][j])/mul);
        normal[2] = ((q[2][j] - p[2][j])/mul);
        
        int found = 0;
        
        master_conpnt * conpiv = &con[pid[i]];   
        //check if contact point is already in the list
        for(master_conpnt *iter = conpiv; iter!=NULL; iter=iter->next)
        {
          for(int jj=0;jj<iter->size;jj++)
          {
            if(!found && (unsigned int)iter->slave[1][jj]==tid[j] && (unsigned int)iter->master[i] == tid[i])//con slave equal slave processed 
            {
              printf("POINT ALREADY IN THE LIST\n");
              found = 1;//contact exist, need to update
              
              //update existing contact
              iter->point[0][jj] = midpt[0];
              iter->point[1][jj] = midpt[1];
              iter->point[2][jj] = midpt[2];

              iter->normal[0][jj] = normal[0];
              iter->normal[1][jj] = normal[1];
              iter->normal[2][jj] = normal[2];
              
              iter->depth[jj] = depth;
              break;
            }
          }
        }
        /*
        if(found)
          printf("Old MARGIN Penetration\n");
        else
          printf("New Margin Penetration\n");
        printf("Body %i is in Contact with Body %i\n", pid[i], pid[j]);
        printf("Triangle %i is in Contact with Triangle %i\n", tid[i], tid[j]);
        */
        if(!found)//not found
        {//append to list
          printf("APPENDED TO THE LIST\n");
          int idx;
          conpiv = newcon (conpiv, &idx);

          conpiv->master[idx] = tid[i];
          conpiv->slave[0][idx] = pid[j];
          conpiv->slave[1][idx] = tid[j];

          //store contact point;
          conpiv->point[0][idx] = midpt[0];
          conpiv->point[1][idx] = midpt[1];
          conpiv->point[2][idx] = midpt[2];

          conpiv->normal[0][idx] = normal[0];
          conpiv->normal[1][idx] = normal[1];
          conpiv->normal[2][idx] = normal[2];

          conpiv->color[0][idx] = 0;
          conpiv->color[1][idx] = 0;

          conpiv->depth[idx] = depth;
        }
      }
    }
  }
}

/* update existing contact points */
void update_existing (int nb, unsigned int nt, master_conpnt *master, iREAL * t[3][3], unsigned int *tid, unsigned int *pid, iREAL *p[3], iREAL *q[3])
{
  for(int zz=0;zz<nb;zz++)
  {
    for(master_conpnt *iter = &master[zz]; iter!=NULL; iter=iter->next)
    {//update existing contact points - adhensive contact
      for(int k=0;k<iter->size;k++)
      {
        //if(iter->depth > 0) continue;//only look for distance > margins //adhensive contact update
///TODO FILTERING HERE;
        int i = iter->master[k];//master id
        int j = iter->slave[1][k];//slave id
        
        //search and find it exist
        int ii; int jj; int foundslave = 0; int foundmaster = 0;
        
        for(ii=0;ii<nt;ii++)//find real indices
        {
          if(tid[ii] == i)
          {
            foundmaster = 1;
            break;
          }
        }
        for(jj=0;jj<nt;jj++)
        {
          if(tid[jj] == j)
          {
            foundslave = 1;
            break;
          }
        }
        
        if(!foundmaster) printf("SOMETHING WEIRD HAPPENED");//BUG CHECKPOINT
         
        if(foundslave==0 || foundmaster==0)//master or slave is not in domain, so delete contact point
        {//not found - slave has migrated to another rank
          //delete the contact
         
          printf("deleting contact because contact is gone\n");
          int x = k+1; //go to next contact id
          while (x < iter->size) x ++;//get j id of last contact
          if (x < iter->size)
          {//replace with last to fill the gone contact point
            iter->master[k] = iter->master[x];
            iter->slave[0][k] = iter->slave[0][x];
            iter->slave[1][k] = iter->slave[1][x];
            
            iter->color[0][k] = iter->color[0][x];
            iter->color[1][k] = iter->color[1][x];
            
            iter->point[0][k] = iter->point[0][x];
            iter->point[1][k] = iter->point[1][x];
            iter->point[2][k] = iter->point[2][x];
            
            iter->normal[0][k] = iter->normal[0][x];
            iter->normal[1][k] = iter->normal[1][x];
            iter->normal[2][k] = iter->normal[2][x];
            
            iter->depth[k] = iter->depth[x];
          }
          iter->size = iter->size-1;
          
          
          while (iter && iter->next) // delete empty items
          {
            master_conpnt * next = iter->next;

            if (next->size == 0)
            {
              iter->next = next->next;
              delete next;
            }

            iter = iter->next;
          }
        }else if(foundmaster && foundslave)
        { //if slave found - has not migrated to another rank
          //update distance 
          
          printf("update distance because contact is adhensive. DEPTH:%f JJ:%i\n", iter->depth[k], jj);
          
          iREAL a[3], b[3], c[3];
          a[0] = t[0][0][ii];
          a[1] = t[0][1][ii];
          a[2] = t[0][2][ii];
          
          b[0] = t[1][0][ii];
          b[1] = t[1][1][ii];
          b[2] = t[1][2][ii];
          
          c[0] = t[2][0][ii];
          c[1] = t[2][1][ii];
          c[2] = t[2][2][ii];

          ispc_bf (jj, jj+1, a, b, c, t[0], t[1], t[2], p, q);//use tasks 
          //-not valid statement - //already calculated in previous stage of simulation, just retrieve value
          iREAL dist = sqrt(pow((q[0][jj]-p[0][jj]),2)+pow((q[1][jj]-p[1][jj]),2)+pow((q[2][jj]-p[1][jj]),2));
           
          iREAL midpt[3], normal[3];
          midpt[0] = (p[0][jj]+q[0][jj])/2; //x
          midpt[1] = (p[1][jj]+q[1][jj])/2; //y
          midpt[2] = (p[2][jj]+q[2][jj])/2; //z
      
          iREAL margin = 10E-2;
          iREAL depth = margin-dist;
          
          printf("UPDATE: depth:%f dist:%f\n", depth, dist);
        
          iREAL mul = sqrt(pow(q[0][jj]-p[0][jj], 2)+pow(q[1][jj] - p[1][jj], 2)+pow(q[2][jj]-p[2][jj], 2));
          normal[0] = ((q[0][jj] - p[0][jj])/mul);//*depth for inclusion to normal
          normal[1] = ((q[1][jj] - p[1][jj])/mul);
          normal[2] = ((q[2][jj] - p[2][jj])/mul);
          
          iter->point[0][k] = midpt[0];
          iter->point[1][k] = midpt[1];
          iter->point[2][k] = midpt[2];

          iter->normal[0][k] = normal[0];
          iter->normal[1][k] = normal[1];
          iter->normal[2][k] = normal[2];

          iter->depth[k] = depth;
        }
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

