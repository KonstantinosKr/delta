#include "algo.h"

export void ispc_bf (uniform unsigned int n,
                         uniform iREAL uniform pp1[3], uniform iREAL uniform pp2[3], uniform iREAL uniform pp3[3],
                         uniform iREAL * uniform pp4[3], uniform iREAL * uniform pp5[3], uniform iREAL * uniform pp6[3],
                         uniform iREAL * uniform P[3], uniform iREAL * uniform Q[3]) 
{

	
  iREAL u[3], v[3], nn[3][2], w[3], w0[3][6], dir[3][6], pointArray[3][6];
   
  pointArray[0][0] = pp1[0];
  pointArray[1][0] = pp1[1];
  pointArray[2][0] = pp1[2];

  pointArray[0][1] = pp2[0];
  pointArray[1][1] = pp2[1];
  pointArray[2][1] = pp2[2];

  pointArray[0][2] = pp3[0];
  pointArray[1][2] = pp3[1];
  pointArray[2][2] = pp3[2];
  
  foreach(i=0 ... n)
  { 
    pointArray[0][3] = pp4[0][i];
    pointArray[1][3] = pp4[1][i];
    pointArray[2][3] = pp4[2][i];

    pointArray[0][4] = pp5[0][i];
    pointArray[1][4] = pp5[1][i];
    pointArray[2][4] = pp5[2][i];

    pointArray[0][5] = pp6[0][i];
    pointArray[1][5] = pp6[1][i];
    pointArray[2][5] = pp6[2][i];

    u[0] = pointArray[0][4] - pointArray[0][3];
    u[1] = pointArray[1][4] - pointArray[1][3];
    u[2] = pointArray[2][4] - pointArray[2][3];
    
    v[0] = pointArray[0][5] - pointArray[0][4];
    v[1] = pointArray[1][5] - pointArray[1][4];
    v[2] = pointArray[2][5] - pointArray[2][4];
    
    nn[0][0] = u[1]*v[2] - u[2]*v[1];
    nn[1][0] = u[2]*v[0] - u[0]*v[2];
    nn[2][0] = u[0]*v[1] - u[1]*v[0];
    if(nn[0][0]==0 && nn[1][0]==0 && nn[2][0]==0){
      continue;
    }
    dir[0][0] = pointArray[0][1] - pointArray[0][0];
    dir[1][0] = pointArray[1][1] - pointArray[1][0];
    dir[2][0] = pointArray[2][1] - pointArray[2][0];
    
    w0[0][0] = pointArray[0][0] - pointArray[0][3];
    w0[1][0] = pointArray[1][0] - pointArray[1][3];
    w0[2][0] = pointArray[2][0] - pointArray[2][3];

    dir[0][1] = pointArray[0][2] - pointArray[0][1];
    dir[1][1] = pointArray[1][2] - pointArray[1][1];
    dir[2][1] = pointArray[2][2] - pointArray[2][1];
    
    w0[0][1] = pointArray[0][1] - pointArray[0][3];
    w0[1][1] = pointArray[1][1] - pointArray[1][3];
    w0[2][1] = pointArray[2][1] - pointArray[2][3];

    dir[0][2] = pointArray[0][0] - pointArray[0][2];
    dir[1][2] = pointArray[1][0] - pointArray[1][2];
    dir[2][2] = pointArray[2][0] - pointArray[2][2];
    
    w0[0][2] = pointArray[0][2] - pointArray[0][3];
    w0[1][2] = pointArray[1][2] - pointArray[1][3];
    w0[2][2] = pointArray[2][2] - pointArray[2][3];

    //T2.1 - T1;
    u[0] = pointArray[0][1] - pointArray[0][0];
    u[1] = pointArray[1][1] - pointArray[1][0];
    u[2] = pointArray[2][1] - pointArray[2][0];
    
    v[0] = pointArray[0][2] - pointArray[0][0];
    v[1] = pointArray[1][2] - pointArray[1][0];
    v[2] = pointArray[2][2] - pointArray[2][0];
    
    nn[0][1] = u[1]*v[2] - u[2]*v[1];
    nn[1][1] = u[2]*v[0] - u[0]*v[2];
    nn[2][1] = u[0]*v[1] - u[1]*v[0];
    if(nn[0][0]==0 && nn[1][1]==0 && nn[2][1]==0) {
      continue;
    }

    dir[0][3] = pointArray[0][4] - pointArray[0][3];
    dir[1][3] = pointArray[1][4] - pointArray[1][3];
    dir[2][3] = pointArray[2][4] - pointArray[2][3];
    
    w0[0][3] = pointArray[0][3] - pointArray[0][0];
    w0[1][3] = pointArray[1][3] - pointArray[1][0];
    w0[2][3] = pointArray[2][3] - pointArray[2][0];

    dir[0][4] = pointArray[0][5] - pointArray[0][4];
    dir[1][4] = pointArray[1][5] - pointArray[1][4];
    dir[2][4] = pointArray[2][5] - pointArray[2][4];
    
    w0[0][4] = pointArray[0][4] - pointArray[0][0];
    w0[1][4] = pointArray[1][4] - pointArray[1][0];
    w0[2][4] = pointArray[2][4] - pointArray[2][0];

    dir[0][5] = pointArray[0][3] - pointArray[0][5];
    dir[1][5] = pointArray[1][3] - pointArray[1][5];
    dir[2][5] = pointArray[2][3] - pointArray[2][5];
    
    w0[0][5] = pointArray[0][5] - pointArray[0][0];
    w0[1][5] = pointArray[1][5] - pointArray[1][0];
    w0[2][5] = pointArray[2][5] - pointArray[2][0];
    
    for(uniform j=0;j<6;j++)
    {
      iREAL a,b;
      if(j<3)
      {
        a = -DOT(nn[0], w0[0]);
        b= DOT(nn[0],dir[0]);
      }else {
        a = -DOT(nn[1], w0[0]);
        b= DOT(nn[1],dir[0]);
      }
      if (abs(b) < 1E-30 && a==0) {
        //segment in triangle plane
        continue;
      }
      
      iREAL r = a / b;

      P[0][i] = pointArray[0][j] + r * dir[0][j];
      P[1][i] = pointArray[1][j] + r * dir[1][j];
      P[2][i] = pointArray[2][j] + r * dir[2][j];

      Q[0][i] = P[0][i];
      Q[1][i] = P[1][i];
      Q[2][i] = P[2][i];

      if (r < 0.0 || r > 1.0 || r != r) {
        break;
      }
      
      if(j<3){
        w[0] = P[0][i] - pointArray[0][3];
        w[1] = P[1][i] - pointArray[1][3];
        w[2] = P[2][i] - pointArray[2][3];
      }else{
        w[0] = P[0][i] - pointArray[0][0];
        w[1] = P[1][i] - pointArray[1][0];
        w[2] = P[2][i] - pointArray[2][0];
      }  
            
      iREAL D = DOT(u,v)*DOT(u,v) - DOT(u,u)*DOT(v,v);
      iREAL s = (DOT(u,v)*DOT(w,v) - DOT(v,v)*DOT(w,u))/D;
      if (s<0.0 || s>1.0) {
        break;
      }
      
      iREAL t = (DOT(u,v)*DOT(w,u) - DOT(u,u)*DOT(w,v))/D;
      if (t<0.0 || (s+t)>1.0) 
      {
        break;
      }
      
      //seg in triangle
      continue;
    }


      iREAL a[9], b[9], c[9], d[9], e[9], f[9], p1[3], p2[3], p3[3], p4[3], p5[3], p6[3];

      p1[0] = pp1[0];
      p1[1] = pp1[1];
      p1[2] = pp1[2];

      p2[0] = pp2[0];
      p2[1] = pp2[1];
      p2[2] = pp2[2];

      p3[0] = pp3[0];
      p3[1] = pp3[1];
      p3[2] = pp3[2];

      p4[0] = pp4[0][i];
      p4[1] = pp4[1][i];
      p4[2] = pp4[2][i];

      p5[0] = pp5[0][i];
      p5[1] = pp5[1][i];
      p5[2] = pp5[2][i];

      p6[0] = pp6[0][i];
      p6[1] = pp6[1][i];
      p6[2] = pp6[2][i];

      //ab-de, ab-df, ab-ef | bc-de, bc-df, bc-ef | ca-de, ca-df, ca-ef.
      //12-45, 12-46, 12-56 | 23-45, 23-46, 23-56 | 31-45, 31-46, 31-56.

      //T1.1 - T2
      iREAL u[3];
      u[0] = (p1[0] - p2[0]);
      u[1] = (p1[1] - p2[1]);
      u[2] = (p1[2] - p2[2]);
      
      iREAL v[3];
      v[0] = (p4[0] - p5[0]);
      v[1] = (p4[1] - p5[1]);
      v[2] = (p4[2] - p5[2]);
      
      iREAL w[3];
      w[0] = (p2[0] - p5[0]);
      w[1] = (p2[1] - p5[1]);
      w[2] = (p2[2] - p5[2]);
      
      a[0] = DOT(u,u);
      b[0] = DOT(u,v);
      c[0] = DOT(v,v);
      d[0] = DOT(u,w);
      e[0] = DOT(v,w);

      v[0] = (p5[0] - p6[0]);
      v[1] = (p5[1] - p6[1]);
      v[2] = (p5[2] - p6[2]);
      
      w[0] = (p2[0] - p6[0]);
      w[1] = (p2[1] - p6[1]);
      w[2] = (p2[2] - p6[2]);
      
      a[1] = a[0];
      b[1] = DOT(u,v);
      c[1] = DOT(v,v);
      d[1] = DOT(u,w);
      e[1] = DOT(v,w);

      v[0] = (p6[0] - p4[0]);
      v[1] = (p6[1] - p4[1]);
      v[2] = (p6[2] - p4[2]);
      

      w[0] = (p2[0] - p4[0]);
      w[1] = (p2[1] - p4[1]);
      w[2] = (p2[2] - p4[2]);
      
      a[2] = a[0];
      b[2] = DOT(u,v);
      c[2] = DOT(v,v);
      d[2] = DOT(u,w);
      e[2] = DOT(v,w);

      //T1.2 - T2
      u[0] = (p2[0] - p3[0]);
      u[1] = (p2[1] - p3[1]);
      u[2] = (p2[2] - p3[2]);
      

      v[0] = (p4[0] - p5[0]);
      v[1] = (p4[1] - p5[1]);
      v[2] = (p4[2] - p5[2]);
      
      w[0] = (p3[0] - p5[0]);
      w[1] = (p3[1] - p5[1]);
      w[2] = (p3[2] - p5[2]);
      
      a[3] = DOT(u,u);
      b[3] = DOT(u,v);
      c[3] = DOT(v,v);
      d[3] = DOT(u,w);
      e[3] = DOT(v,w);

      v[0] = (p5[0] - p6[0]);
      v[1] = (p5[1] - p6[1]);
      v[2] = (p5[2] - p6[2]);
      

      w[0] = (p3[0] - p6[0]);
      w[1] = (p3[1] - p6[1]);
      w[2] = (p3[2] - p6[2]);
      
      a[4] = a[3];
      b[4] = DOT(u,v);
      c[4] = DOT(v,v);
      d[4] = DOT(u,w);
      e[4] = DOT(v,w);

      v[0] = (p6[0] - p4[0]);
      v[1] = (p6[1] - p4[1]);
      v[2] = (p6[2] - p4[2]);

      w[0] = (p3[0] - p4[0]);
      w[1] = (p3[1] - p4[1]);
      w[2] = (p3[2] - p4[2]);
      
      a[5] = a[3];
      b[5] = DOT(u,v);
      c[5] = DOT(v,v);
      d[5] = DOT(u,w);
      e[5] = DOT(v,w);

      //T1.3 - T2

      u[0] = (p3[0] - p1[0]);
      u[1] = (p3[1] - p1[1]);
      u[2] = (p3[2] - p1[2]);
      
      v[0] = (p4[0] - p5[0]);
      v[1] = (p4[1] - p5[1]);
      v[2] = (p4[2] - p5[2]);
      
      w[0] = (p1[0] - p5[0]);
      w[1] = (p1[1] - p5[1]);
      w[2] = (p1[2] - p5[2]);
      
      a[6] = DOT(u,u);
      b[6] = DOT(u,v);
      c[6] = DOT(v,v);
      d[6] = DOT(u,w);
      e[6] = DOT(v,w);

      v[0] = (p5[0] - p6[0]);
      v[1] = (p5[1] - p6[1]);
      v[2] = (p5[2] - p6[2]);
      
      w[0] = (p1[0] - p6[0]);
      w[1] = (p1[1] - p6[1]);
      w[2] = (p1[2] - p6[2]);
      
      a[7] = a[6];
      b[7] = DOT(u,v);
      c[7] = DOT(v,v);
      d[7] = DOT(u,w);
      e[7] = DOT(v,w);

      v[0] = (p6[0] - p4[0]);
      v[1] = (p6[1] - p4[1]);
      v[2] = (p6[2] - p4[2]);
      
      w[0] = (p1[0] - p4[0]);
      w[1] = (p1[1] - p4[1]);
      w[2] = (p1[2] - p4[2]);
      
      a[8] = a[6];
      b[8] = DOT(u,v);
      c[8] = DOT(v,v);
      d[8] = DOT(u,w);
      e[8] = DOT(v,w);

      
      iREAL ssmin = 1E+30, ttc, ssc;
        
      cfor(uniform int j=0; j<9;j++)
      {
        iREAL D = (a[j]*c[j] - b[j]*b[j]);

        iREAL sD = D;
        iREAL tD = D;
        iREAL sN = 0, tN = 0; 
     
        iREAL SMALL_NUM = 1E-30;
        
        // compute the line parameters of the two closest points
        if (D < SMALL_NUM)
        {//the lines are almost parallel
          sN = 0.0;       //force using point P0 on segment S1
          sD = 1.0;       //to prevent possible division by 0.0 later
          tN = e[j];
          tD = c[j]; 
        } else {               //get the closest points on the infinite lines
          sN = (b[j]*e[j] - c[j]*d[j]);
          tN = (a[j]*e[j] - b[j]*d[j]);

          if (sN < 0.0){   //sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e[j];
            tD = c[j];
          } else if (sN > sD){//sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e[j] + b[j];
            tD = c[j];
          }
        }
            
        if (tN < 0.0){     //tc < 0 => the t=0 edge is visible
          tN = 0.0; //recompute sc for this edge
          if (-d[j] < 0.0){
            sN = 0.0;
          }else if (-d[j] > a[j]){
            sN = sD;
          } else {
            sN = -d[j];
            sD = a[j];
          }
        } else if (tN > tD){       //tc > 1 => the t=1 edge is visible
          tN = tD;  //recompute sc for this edge
          if ((-d[j] + b[j]) < 0.0){
            sN = 0;
          }else if ((-d[j] + b[j]) > a[j]){
            sN = sD;
          } else {
            sN = (-d[j] + b[j]);
            sD = a[j];
          }
        }

        iREAL sc, tc; //do the division to get sc and tc
        if(abs(sN) < SMALL_NUM){
          sc = 0.0;
        }else{
          sc = sN / sD;
        }
            
        if(abs(tN) < SMALL_NUM){
          tc = 0.0;
        }else{
          tc = tN / tD;
        } 

        iREAL dP[3], dist;

        if(j==0)
        {   
            dP[0] = (p2[0] - p5[0]) + (sc*(p1[0] - p2[0])) - (tc*(p4[0] - p5[0]));
            dP[1] = (p2[1] - p5[1]) + (sc*(p1[1] - p2[1])) - (tc*(p4[1] - p5[1]));
            dP[2] = (p2[2] - p5[2]) + (sc*(p1[2] - p2[2])) - (tc*(p4[2] - p5[2]));
        }else if(j==1)
        {
            dP[0] = (p2[0] - p6[0]) + (sc*(p1[0] - p2[0])) - (tc*(p5[0] - p6[0]));
            dP[1] = (p2[1] - p6[1]) + (sc*(p1[1] - p2[1])) - (tc*(p5[1] - p6[1]));
            dP[2] = (p2[2] - p6[2]) + (sc*(p1[2] - p2[2])) - (tc*(p5[2] - p6[2]));
        }else if(j==2)
        {
            dP[0] = (p2[0] - p4[0]) + (sc*(p1[0] - p2[0])) - (tc*(p6[0] - p4[0]));
            dP[1] = (p2[1] - p4[1]) + (sc*(p1[1] - p2[1])) - (tc*(p6[1] - p4[1]));
            dP[2] = (p2[2] - p4[2]) + (sc*(p1[2] - p2[2])) - (tc*(p6[2] - p4[2]));
        }else if(j==3)
        {
            dP[0] = (p3[0] - p5[0]) + (sc*(p2[0] - p3[0])) - (tc*(p4[0] - p5[0]));
            dP[1] = (p3[1] - p5[1]) + (sc*(p2[1] - p3[1])) - (tc*(p4[1] - p5[1]));
            dP[2] = (p3[2] - p5[2]) + (sc*(p2[2] - p3[2])) - (tc*(p4[2] - p5[2]));
        }else if(j==4)
        {
            dP[0] = (p3[0] - p6[0]) + (sc*(p2[0] - p3[0])) - (tc*(p5[0] - p6[0]));
            dP[1] = (p3[1] - p6[1]) + (sc*(p2[1] - p3[1])) - (tc*(p5[1] - p6[1]));
            dP[2] = (p3[2] - p6[2]) + (sc*(p2[2] - p3[2])) - (tc*(p5[2] - p6[2]));
        }else if(j==5)
        {
            dP[0] = (p3[0] - p4[0]) + (sc*(p2[0] - p3[0])) - (tc*(p6[0] - p4[0]));
            dP[1] = (p3[1] - p4[1]) + (sc*(p2[1] - p3[1])) - (tc*(p6[1] - p4[1]));
            dP[2] = (p3[2] - p4[2]) + (sc*(p2[2] - p3[2])) - (tc*(p6[2] - p4[2]));
        }else if(j==6)
        {
            dP[0] = (p1[0] - p5[0]) + (sc*(p3[0] - p1[0])) - (tc*(p4[0] - p5[0]));
            dP[1] = (p1[1] - p5[1]) + (sc*(p3[1] - p1[1])) - (tc*(p4[1] - p5[1]));
            dP[2] = (p1[2] - p5[2]) + (sc*(p3[2] - p1[2])) - (tc*(p4[2] - p5[2]));
        }else if(j==7)
        {
            dP[0] = (p1[0] - p6[0]) + (sc*(p3[0] - p1[0])) - (tc*(p5[0] - p6[0]));
            dP[1] = (p1[1] - p6[1]) + (sc*(p3[1] - p1[1])) - (tc*(p5[1] - p6[1]));
            dP[2] = (p1[2] - p6[2]) + (sc*(p3[2] - p1[2])) - (tc*(p5[2] - p6[2]));
        }else 
        {
            dP[0] = (p1[0] - p4[0]) + (sc*(p3[0] - p1[0])) - (tc*(p6[0] - p4[0]));
            dP[1] = (p1[1] - p4[1]) + (sc*(p3[1] - p1[1])) - (tc*(p6[1] - p4[1]));
            dP[2] = (p1[2] - p4[2]) + (sc*(p3[2] - p1[2])) - (tc*(p6[2] - p4[2]));
        }
        
        dist = sqrt(dP[0]*dP[0]+dP[1]*dP[1]+dP[2]*dP[2]);   

        if(dist<ssmin) {
          ssmin = dist;
          ttc = tc;
          ssc = sc;
        }
      }

      P[0][i] = p2[0] + (p1[0] - p2[0]) * ssc;
      P[1][i] = p2[1] + (p1[1] - p2[1]) * ssc;
      P[2][i] = p2[2] + (p1[2] - p2[2]) * ssc;
      
      Q[0][i] = p4[0] + (p4[0] - p5[0]) * ttc;
      Q[1][i] = p4[1] + (p4[0] - p5[0]) * ttc;
      Q[2][i] = p4[2] + (p4[0] - p5[0]) * ttc;


    a[0] = (p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1]) + (p2[2] - p1[2])*(p2[2] - p1[2]);
    b[0] = (p2[0] - p1[0])*(p3[0] - p1[0]) + (p2[1] - p1[1])*(p3[1] - p1[1]) + (p2[2] - p1[2])*(p3[2] - p1[2]);
    c[0] = (p3[0] - p1[0])*(p3[0] - p1[0]) + (p3[1] - p1[1])*(p3[1] - p1[1]) + (p3[2] - p1[2])*(p3[2] - p1[2]);
    d[0] = (p2[0] - p1[0])*(p1[0] - p4[0]) + (p2[1] - p1[1])*(p1[1] - p4[1]) + (p2[2] - p1[2])*(p1[2] - p4[2]);
    e[0] = (p3[0] - p1[0])*(p1[0] - p4[0]) + (p3[1] - p1[1])*(p1[1] - p4[1]) + (p3[2] - p1[2])*(p1[2] - p4[2]);
    f[0] = (p1[0] - p4[0])*(p1[0] - p4[0]) + (p1[1] - p4[1])*(p1[1] - p4[1]) + (p1[2] - p4[2])*(p1[2] - p4[2]);

    a[1] = (p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1]) + (p2[2] - p1[2])*(p2[2] - p1[2]);
    b[1] = (p2[0] - p1[0])*(p3[0] - p1[0]) + (p2[1] - p1[1])*(p3[1] - p1[1]) + (p2[2] - p1[2])*(p3[2] - p1[2]);
    c[1] = (p3[0] - p1[0])*(p3[0] - p1[0]) + (p3[1] - p1[1])*(p3[1] - p1[1]) + (p3[2] - p1[2])*(p3[2] - p1[2]);
    d[1] = (p2[0] - p1[0])*(p1[0] - p5[0]) + (p2[1] - p1[1])*(p1[1] - p5[1]) + (p2[2] - p1[2])*(p1[2] - p5[2]);
    e[1] = (p3[0] - p1[0])*(p1[0] - p5[0]) + (p3[1] - p1[1])*(p1[1] - p5[1]) + (p3[2] - p1[2])*(p1[2] - p5[2]);
    f[1] = (p1[0] - p5[0])*(p1[0] - p5[0]) + (p1[1] - p5[1])*(p1[1] - p5[1]) + (p1[2] - p5[2])*(p1[2] - p5[2]);

    a[2] = (p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1]) + (p2[2] - p1[2])*(p2[2] - p1[2]);
    b[2] = (p2[0] - p1[0])*(p3[0] - p1[0]) + (p2[1] - p1[1])*(p3[1] - p1[1]) + (p2[2] - p1[2])*(p3[2] - p1[2]);
    c[2] = (p3[0] - p1[0])*(p3[0] - p1[0]) + (p3[1] - p1[1])*(p3[1] - p1[1]) + (p3[2] - p1[2])*(p3[2] - p1[2]);
    d[2] = (p2[0] - p1[0])*(p1[0] - p6[0]) + (p2[1] - p1[1])*(p1[1] - p6[1]) + (p2[2] - p1[2])*(p1[2] - p6[2]);
    e[2] = (p3[0] - p1[0])*(p1[0] - p6[0]) + (p3[1] - p1[1])*(p1[1] - p6[1]) + (p3[2] - p1[2])*(p1[2] - p6[2]);
    f[2] = (p1[0] - p6[0])*(p1[0] - p6[0]) + (p1[1] - p6[1])*(p1[1] - p6[1]) + (p1[2] - p6[2])*(p1[2] - p6[2]);

    ///

    a[3] = (p5[0] - p4[0])*(p5[0] - p4[0]) + (p5[1] - p4[1])*(p5[1] - p4[1]) + (p5[2] - p4[2])*(p5[2] - p4[2]);
    b[3] = (p5[0] - p4[0])*(p6[0] - p4[0]) + (p5[1] - p4[1])*(p6[1] - p4[1]) + (p5[2] - p4[2])*(p6[2] - p4[2]);
    c[3] = (p6[0] - p4[0])*(p6[0] - p4[0]) + (p6[1] - p4[1])*(p6[1] - p4[1]) + (p6[2] - p4[2])*(p6[2] - p4[2]);
    d[3] = (p5[0] - p4[0])*(p4[0] - p1[0]) + (p5[1] - p4[1])*(p4[1] - p1[1]) + (p5[2] - p4[2])*(p4[2] - p1[2]);
    e[3] = (p6[0] - p4[0])*(p4[0] - p1[0]) + (p6[1] - p4[1])*(p4[1] - p1[1]) + (p6[2] - p4[2])*(p4[2] - p1[2]);
    f[3] = (p4[0] - p1[0])*(p4[0] - p1[0]) + (p4[1] - p1[1])*(p4[1] - p1[1]) + (p4[2] - p1[2])*(p4[2] - p1[2]);

    a[4] = (p5[0] - p4[0])*(p5[0] - p4[0]) + (p5[1] - p4[1])*(p5[1] - p4[1]) + (p5[2] - p4[2])*(p5[2] - p4[2]);
    b[4] = (p5[0] - p4[0])*(p6[0] - p4[0]) + (p5[1] - p4[1])*(p6[1] - p4[1]) + (p5[2] - p4[2])*(p6[2] - p4[2]);
    c[4] = (p6[0] - p4[0])*(p6[0] - p4[0]) + (p6[1] - p4[1])*(p6[1] - p4[1]) + (p6[2] - p4[2])*(p6[2] - p4[2]);
    d[4] = (p5[0] - p4[0])*(p4[0] - p2[0]) + (p5[1] - p4[1])*(p4[1] - p2[1]) + (p5[2] - p4[2])*(p4[2] - p2[2]);
    e[4] = (p6[0] - p4[0])*(p4[0] - p2[0]) + (p6[1] - p4[1])*(p4[1] - p2[1]) + (p6[2] - p4[2])*(p4[2] - p2[2]);
    f[4] = (p4[0] - p2[0])*(p4[0] - p2[0]) + (p4[1] - p2[1])*(p4[1] - p2[1]) + (p4[2] - p2[2])*(p4[2] - p2[2]);

    a[5] = (p5[0] - p4[0])*(p5[0] - p4[0]) + (p5[1] - p4[1])*(p5[1] - p4[1]) + (p5[2] - p4[2])*(p5[2] - p4[2]);
    b[5] = (p5[0] - p4[0])*(p6[0] - p4[0]) + (p5[1] - p4[1])*(p6[1] - p4[1]) + (p5[2] - p4[2])*(p6[2] - p4[2]);
    c[5] = (p6[0] - p4[0])*(p6[0] - p4[0]) + (p6[1] - p4[1])*(p6[1] - p4[1]) + (p6[2] - p4[2])*(p6[2] - p4[2]);
    d[5] = (p5[0] - p4[0])*(p4[0] - p3[0]) + (p5[1] - p4[1])*(p4[1] - p3[1]) + (p5[2] - p4[2])*(p4[2] - p3[2]);
    e[5] = (p6[0] - p4[0])*(p4[0] - p3[0]) + (p6[1] - p4[1])*(p4[1] - p3[1]) + (p6[2] - p4[2])*(p4[2] - p3[2]);
    f[5] = (p4[0] - p3[0])*(p4[0] - p3[0]) + (p4[1] - p3[1])*(p4[1] - p3[1]) + (p4[2] - p3[2])*(p4[2] - p3[2]);

    iREAL ss = 0, tt = 0;
    iREAL ptmin = 1E30;
    iREAL id=0;
    cfor(uniform int j=0; j<6;j++)
    {
      iREAL det = a[j]*c[j] - b[j]*b[j]; //do we have to use abs here?
      iREAL s   = b[j]*e[j] - c[j]*d[j];
      iREAL t   = b[j]*d[j] - a[j]*e[j];
      
      iREAL sqrDistance=0;
      
      if ((s+t) <= det)
      {
          if (s < 0){
              if (t < 0){
                  //region4
                  if (d[j] < 0){
                      t = 0;
                      if (-d[j] >= a[j]){
                          s = 1;
                          sqrDistance = a[j] + 2*d[j] + f[j];
                      }else {
                          s = -d[j]/a[j];
                          sqrDistance = d[j]*s + f[j];
                      }
                  }else {
                      s = 0;
                      if (e[j] >= 0){
                          t = 0;
                          sqrDistance = f[j];
                      }else{
                          if (-e[j] >= c[j]){
                              t = 1;
                              sqrDistance = c[j] + 2*e[j] + f[j];
                          } else {
                              t = -e[j]/c[j];
                              sqrDistance = e[j]*t + f[j];
                          }
                      }
                  } //end of region 4
              }else {
                  // region 3
                  s = 0;
                  if (e[j] >= 0){
                      t = 0;
                      sqrDistance = f[j];
                  }else {
                      if (-e[j] >= c[j]){
                          t = 1;
                          sqrDistance = c[j] + 2*e[j] +f[j];
                      }else {
                          t = -e[j]/c[j];
                          sqrDistance = e[j]*t + f[j];
                      }
                  }
              } //end of region 3
          }else {
              if (t < 0){
                  // region 5
                  t = 0;
                  if (d[j] >= 0){
                      s = 0;
                      sqrDistance = f[j];
                  }else {
                      if (-d[j] >= a[j]){
                          s = 1;
                          sqrDistance = a[j] + 2*d[j] + f[j];// GF 20101013 fixed typo d*s ->2*d
                      }else {
                          s = -d[j]/a[j];
                          sqrDistance = d[j]*s + f[j];
                      }
                  }
              }else {
                  // region 0
                  iREAL invDet = 1/det;
                  s = s*invDet;
                  t = t*invDet;
                  sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
              }
          }
      } 
      else 
      {
          if (s < 0){
              // region 2
              iREAL tmp0 = b[j] + d[j];
              iREAL tmp1 = c[j] + e[j];
              if (tmp1 > tmp0){ // minimum on edge s+t=1
                  iREAL numer = tmp1 - tmp0;
                  iREAL denom = a[j] - 2*b[j] + c[j];
                  if (numer >= denom){
                      s = 1;
                      t = 0;
                      sqrDistance = a[j] + 2*d[j] + f[j]; // GF 20101014 fixed typo 2*b -> 2*d
                  }else {
                      s = numer/denom;
                      t = 1-s;
                      sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
                  }
              }else {         // minimum on edge s=0
                  s = 0;
                  if (tmp1 <= 0) {
                      t = 1;
                      sqrDistance = c[j] + 2*e[j] + f[j];
                  }else {
                      if (e[j] >= 0){
                          t = 0;
                          sqrDistance = f[j];
                      }else {
                          t = -e[j]/c[j];
                          sqrDistance = e[j]*t + f[j];
                      }
                  }
              } //end of region 2
          }else {
              if (t < 0) {
                  //region6
                  iREAL tmp0 = b[j] + e[j];
                  iREAL tmp1 = a[j] + d[j];
                  if (tmp1 > tmp0){
                      iREAL numer = tmp1 - tmp0;
                      iREAL denom = a[j]-2*b[j]+c[j];
                      if (numer >= denom){
                          t = 1;
                          s = 0;
                          sqrDistance = c[j] + 2*e[j] + f[j];
                      }else {
                          t = numer/denom;
                          s = 1 - t;
                          sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
                      }
                  }else {
                      t = 0;
                      if (tmp1 <= 0){
                          s = 1;
                          sqrDistance = a[j] + 2*d[j] + f[j];
                      }else {
                          if (d[j] >= 0) {
                              s = 0;
                              sqrDistance = f[j];
                          }else {
                              s = -d[j]/a[j];
                              sqrDistance = d[j]*s + f[j];
                          }
                      }
                  }
                  //end of region 6
              }else {
                  // region 1
                  iREAL numer = c[j] + e[j] - b[j] - d[j];
                  if (numer <= 0){
                      s = 0;
                      t = 1;
                      sqrDistance = c[j] + 2*e[j] + f[j];
                  }else {
                      iREAL denom = a[j] - 2*b[j] + c[j];
                      if (numer >= denom){
                          s = 1;
                          t = 0;
                          sqrDistance = a[j] + 2*d[j] + f[j];
                      }else {
                          s = numer/denom;
                          t = 1-s;
                          sqrDistance = s*(a[j]*s + b[j]*t + 2*d[j]) + t*(b[j]*s + c[j]*t + 2*e[j]) + f[j];
                      }
                  } //end of region 1
              }
          }
      }
      
      // account for numerical round-off error
      if (sqrDistance < 0){sqrDistance = 0;}

      iREAL tq[3], dist;

      dist = sqrt(sqrDistance);
      if(dist<ptmin)
      {
        ptmin = dist;
        ss = s;
        tt = t; 
      }
    }
    if(ptmin > ssmin)
    {
       continue;
    }

    
    switch(id)
    {
      case 0:
        P[0][i] = p1[0] + ((p3[0] - p1[0]) * tt) + ((p2[0] - p1[0]) * ss);
        P[1][i] = p1[1] + ((p3[1] - p1[1]) * tt) + ((p2[1] - p1[1]) * ss);
        P[2][i] = p1[2] + ((p3[2] - p1[2]) * tt) + ((p2[2] - p1[2]) * ss);

        Q[0][i] = p4[0];
        Q[1][i] = p4[1];
        Q[2][i] = p4[2];
        break;
      case 1:
        P[0][i] = p1[0] + ((p3[0] - p1[0]) * tt) + ((p2[0] - p1[0]) * ss);
        P[1][i] = p1[1] + ((p3[1] - p1[1]) * tt) + ((p2[1] - p1[1]) * ss);
        P[2][i] = p1[2] + ((p3[2] - p1[2]) * tt) + ((p2[2] - p1[2]) * ss);

        Q[0][i] = p5[0];
        Q[1][i] = p5[1];
        Q[2][i] = p5[2];
      case 2:
        P[0][i] = p1[0] + ((p3[0] - p1[0]) * tt) + ((p2[0] - p1[0]) * ss);
        P[1][i] = p1[1] + ((p3[1] - p1[1]) * tt) + ((p2[1] - p1[1]) * ss);
        P[2][i] = p1[2] + ((p3[2] - p1[2]) * tt) + ((p2[2] - p1[2]) * ss);

        Q[0][i] = p6[0];
        Q[1][i] = p6[1];
        Q[2][i] = p6[2];
        break;
      case 3:
        P[0][i] = p1[0];
        P[1][i] = p1[1];
        P[2][i] = p1[2];

        Q[0][i] = p4[0] + ((p6[0] - p4[0]) * tt) + ((p4[0] - p4[0]) * ss);
        Q[1][i] = p4[1] + ((p6[1] - p4[1]) * tt) + ((p4[1] - p4[1]) * ss);
        Q[2][i] = p4[2] + ((p6[2] - p4[2]) * tt) + ((p5[2] - p4[2]) * ss);
        break;
      case 4:
        P[0][i] = p2[0];
        P[1][i] = p2[1];
        P[2][i] = p2[2];

        Q[0][i] = p4[0] + ((p6[0] - p4[0]) * tt) + ((p4[0] - p4[0]) * ss);
        Q[1][i] = p4[1] + ((p6[1] - p4[1]) * tt) + ((p4[1] - p4[1]) * ss);
        Q[2][i] = p4[2] + ((p6[2] - p4[2]) * tt) + ((p5[2] - p4[2]) * ss);
        break;
      case 5:
        P[0][i] = p3[0];
        P[1][i] = p3[1];
        P[2][i] = p3[2];

        Q[0][i] = p4[0] + ((p6[0] - p4[0]) * tt) + ((pp4[0] - pp4[0]) * ss);
        Q[1][i] = p4[1] + ((p6[1] - p4[1]) * tt) + ((pp4[1] - pp4[1]) * ss);
        Q[2][i] = p4[2] + ((p6[2] - p4[2]) * tt) + ((pp5[2] - pp4[2]) * ss);
        break;
    }
  }
}
