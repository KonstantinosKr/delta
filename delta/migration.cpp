/*
 The MIT License (MIT)
 
 Copyright (c) 2016 Konstantinos Krestenitis
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "migration.h"

/* migrate triangles "in-place" to new ranks */
void migrate (int &nt, iREAL *t[6][3], iREAL *v[3],
                        iREAL *angular[6], int *parmat,
                        int *tid, int *pid,
                        int num_import, int *import_procs, int *import_to_part, 
                        int num_export, int *export_procs, int *export_to_part,
                        ZOLTAN_ID_PTR import_global_ids, ZOLTAN_ID_PTR import_local_ids,
                        ZOLTAN_ID_PTR export_global_ids, ZOLTAN_ID_PTR export_local_ids)
{
  int nproc, myrank;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
 
  //allocate memory for tmp buffers
  int **send_idx = (int **) malloc(nproc*sizeof(int*));
  int *pivot = (int *) malloc(nproc*sizeof(int));
  int *rcvpivot = (int *) malloc(nproc*sizeof(int));
  int *export_unique_procs = (int*) malloc(nproc*sizeof(int));
  int *import_unique_procs = (int*) malloc(nproc*sizeof(int));

  for(int i=0;i<nproc;i++)
  {
    rcvpivot[i] = 0;
    pivot[i] = 0;
    send_idx[i] = (int *) malloc((nt)*sizeof(int));
    export_unique_procs[i] = -1;
    import_unique_procs[i] = -1;
  }

  int n_export_unique_procs=0;//number of unique ids to export
  for (int i = 0; i < num_export; i++)//loop through export data/ids 
  {
    int proc = export_procs[i]; //proc is the export process for data id[i]
    
    //set send indices and pivot for buffers for each export process
    send_idx[proc][pivot[proc]] = export_local_ids[i];
    pivot[proc]++;
    
    tid[export_local_ids[i]] = INT_MAX; //mark tid that will be exported thus deleted
    
    int exists = 0; //set to 0 to mean doesn't exist
    for(int j = 0; j < nproc; j++)
    {
      if(proc == export_unique_procs[j])//search list of unique export for duplicates 
      {
        exists = 1; //if exist skip following code to not create a duplicate
        break;
      }
    }

    //if proc is not in export_unique_procs then list it
    if(exists == 0)
    {
      export_unique_procs[n_export_unique_procs] = proc; ///save proc id to array of unique export processes 
      n_export_unique_procs++;//increase number of unique export processes
      //sort unique procs increamentally
      if(n_export_unique_procs > 1)
      { //sort binary-style
        if(export_unique_procs[n_export_unique_procs-2] > export_unique_procs[n_export_unique_procs-1])
        {//swap
          unsigned int tmp = export_unique_procs[n_export_unique_procs-2];
          export_unique_procs[n_export_unique_procs-2] = export_unique_procs[n_export_unique_procs-1];
          export_unique_procs[n_export_unique_procs-1] = tmp;
        }
      }
    }
  }

  iREAL *tbuffer[3], *vbuffer, *angbuffer;
  int *pid_buffer;int *parmat_buffer;
  int n = nt;
  
  if(n_export_unique_procs > 0)
  {
    int mul = n_export_unique_procs*n*3;
    tbuffer[0] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[1] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[2] = (iREAL *) malloc(mul*sizeof(iREAL)); 
    vbuffer = (iREAL *) malloc(mul*sizeof(iREAL));
    angbuffer = (iREAL *) malloc(mul*2*sizeof(iREAL));//6 elements thus x 2
    mul = n_export_unique_procs*n;
    pid_buffer = (int *) malloc(mul*sizeof(int));
    parmat_buffer = (int *) malloc(mul*sizeof(int));
    //printf("RANK[%i]: n:%i allocated: %i n_export_unique_procs: %i\n", myrank, n, n_export_unique_procs, mul);
  }

  int *idx = (int *) malloc(nproc*sizeof(int));
  idx[0] = 0;
  for(int i=0; i<n_export_unique_procs; i++)
  {
    int x = export_unique_procs[i];
    idx[i+1] = idx[i] + pivot[x];
  } 
  
  //assign values to tmp export buffers
  for(int i=0; i<n_export_unique_procs; i++)//n processes to prepare buffers for
  {
    int x = export_unique_procs[i];
    for(int j=0; j<pivot[x]; j++)//pivot gives n number of ids to loop through
    {
      int mul = (idx[i])+(j); 
      pid_buffer[(mul)] = pid[send_idx[x][j]];
      parmat_buffer[(mul)] = parmat[send_idx[x][j]];
      pid[send_idx[x][j]] = INT_MAX;
    
      //printf("LOOKING:%i, equeals(i*size*3)+(j*3)+0: (i:%i * size:%i) + (j:%i * 3) + 0 \n", (i*n*3)+(j*3)+0, i, n, j); 
      //mul = (i*n*3)+(j*3); 
      mul = (idx[i]*3)+(j*3);
      tbuffer[0][mul+0] = t[0][0][send_idx[x][j]]; //point 0/A        
      tbuffer[0][mul+1] = t[0][1][send_idx[x][j]]; //point 0/A        
      tbuffer[0][mul+2] = t[0][2][send_idx[x][j]]; //point 0/A        
      
      tbuffer[1][mul+0] = t[1][0][send_idx[x][j]]; //point 1/B
      tbuffer[1][mul+1] = t[1][1][send_idx[x][j]]; //point 1/B
      tbuffer[1][mul+2] = t[1][2][send_idx[x][j]]; //point 1/B
      
      tbuffer[2][mul+0] = t[2][0][send_idx[x][j]]; //point 2/C
      tbuffer[2][mul+1] = t[2][1][send_idx[x][j]]; //point 2/C
      tbuffer[2][mul+2] = t[2][2][send_idx[x][j]]; //point 2/C
      
      vbuffer[mul+0] = v[0][send_idx[x][j]];
      vbuffer[mul+1] = v[1][send_idx[x][j]];
      vbuffer[mul+2] = v[2][send_idx[x][j]];

      angbuffer[mul+0] = angular[0][send_idx[x][j]];
      angbuffer[mul+1] = angular[1][send_idx[x][j]];
      angbuffer[mul+2] = angular[2][send_idx[x][j]];
      angbuffer[mul+3] = angular[3][send_idx[x][j]];
      angbuffer[mul+4] = angular[4][send_idx[x][j]];
      angbuffer[mul+5] = angular[5][send_idx[x][j]];
    }
  }
  
  ///////////////////////////////////////////////
  //refine local arrays and ids (memory gaps)
  int pv = nt-1;
  for(int i=0;i<num_export;i++)
  {//be cautious bug may be hidden here;
    for(unsigned int j=pv; j>export_local_ids[i]; j--)//from last towards first but only until gap of exported
    {
      if(tid[j] != INT_MAX)//if not marked as to be exported switch fill gaps
      {
        parmat[export_local_ids[i]] = parmat[j];

        tid[export_local_ids[i]] = tid[j]; //send from 'last to first' the tids to 'first to last' in tid array
        pid[export_local_ids[i]] = pid[j];
        tid[j] = INT_MAX; //mark moved tid
        pid[j] = INT_MAX;
        
        t[0][0][export_local_ids[i]] = t[0][0][j];
        t[0][1][export_local_ids[i]] = t[0][1][j];
        t[0][2][export_local_ids[i]] = t[0][2][j];
          
        t[1][0][export_local_ids[i]] = t[1][0][j];
        t[1][1][export_local_ids[i]] = t[1][1][j];
        t[1][2][export_local_ids[i]] = t[1][2][j];
          
        t[2][0][export_local_ids[i]] = t[2][0][j];
        t[2][1][export_local_ids[i]] = t[2][1][j];
        t[2][2][export_local_ids[i]] = t[2][2][j];
          
        v[0][export_local_ids[i]] = v[0][j];
        v[1][export_local_ids[i]] = v[1][j];
        v[2][export_local_ids[i]] = v[2][j];
        
        angular[0][export_local_ids[i]] = angular[0][j];
        angular[1][export_local_ids[i]] = angular[1][j];
        angular[2][export_local_ids[i]] = angular[2][j];
        angular[3][export_local_ids[i]] = angular[3][j];
        angular[4][export_local_ids[i]] = angular[4][j];
        angular[5][export_local_ids[i]] = angular[5][j];
        
        pv = j;//save cave location for next iteration, no need to start from end/back/nt again
        break;//break loop/search from 'last to first' and go to next id that was exported
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////
  
  int receive_idx=0;
  if(nt > 0 && num_export > 0) 
  {
    receive_idx = nt - num_export; //set to last id
  } else if(nt >= 0 && num_export <= 0){
    receive_idx = nt;
  }

  int n_import_unique_procs=0;
  for(int i=0; i < num_import; i++) //loop throught imports
  {
    int proc = import_procs[i]; //get process of import id i
    
    tid[receive_idx] = import_global_ids[i];//set import id to tid
    receive_idx++;//increase receive index
    
    int exists = 0; //set exists to unknown
    for(int j = 0; j < nproc; j++) //loop through until idx reached
    {
      if(proc == import_unique_procs[j])
      {
        exists = 1;
        break;
      }
    }

    if(exists == 0) //if doesn't exist or first id i
    {
      import_unique_procs[n_import_unique_procs] = proc; //assign proc to import unique process
      n_import_unique_procs++; //increase unique import process
      if(n_import_unique_procs > 1) //if number of import processes are bigger than 1
      { //sort
        if(import_unique_procs[n_import_unique_procs-2] > import_unique_procs[n_import_unique_procs-1])
        {//swap
          int tmp = import_unique_procs[n_import_unique_procs-2];
          import_unique_procs[n_import_unique_procs-2] = import_unique_procs[n_import_unique_procs-1];
          import_unique_procs[n_import_unique_procs-1] = tmp;
        }
      }
    }
  }
  
  iREAL *trvbuffer[3], *vrvbuffer, *angrvbuffer;
  int *rcvpid_buffer; int *rvparmat_buffer;
  int size = 0;  

  int MPISENDS = 8;
  MPI_Request *myRequest = (MPI_Request*) malloc(n_export_unique_procs*MPISENDS*sizeof(MPI_Request));//4 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(n_import_unique_procs*MPISENDS*sizeof(MPI_Request));//4 sends 
 
  for(int i=0; i<n_import_unique_procs; i++)
  {
    int x = import_unique_procs[i];
    MPI_Irecv(&rcvpivot[x], 1, MPI_INT, x, 0, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<n_export_unique_procs; i++)
  {
    int x = export_unique_procs[i];
    MPI_Isend(&pivot[x], 1, MPI_INT, x, 0, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<n_import_unique_procs; i++)
  {
    int x = import_unique_procs[i];
    MPI_Wait(&myrvRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
    if(rcvpivot[x] > size) size = rcvpivot[x];
  }
  
  for(int i=0; i<n_export_unique_procs; i++)
  {
    MPI_Wait(&myRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
  }
  
  if(n_import_unique_procs > 0)
  {
    trvbuffer[0] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[1] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[2] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL)); 
    vrvbuffer = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    angrvbuffer = (iREAL *) malloc(n_import_unique_procs*size*6*sizeof(iREAL));//six elements
    rcvpid_buffer = (int *) malloc(n_import_unique_procs*size*sizeof(int));
    rvparmat_buffer = (int *) malloc(n_import_unique_procs*size*sizeof(int));
  }


  for(int i=0; i<n_import_unique_procs; i++)
  {
    int x = import_unique_procs[i];  
    //printf("RANK[%d]: receive from rank %d\n", myrank, x);
   
    MPI_Irecv(&trvbuffer[0][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 2, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
    MPI_Irecv(&trvbuffer[1][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 3, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
    MPI_Irecv(&trvbuffer[2][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 4, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
    MPI_Irecv(&vrvbuffer[(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 5, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+4]);
    MPI_Irecv(&angrvbuffer[(i*size*6)], rcvpivot[x]*6, MPI_DOUBLE, x, 6, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+5]);
    MPI_Irecv(&rcvpid_buffer[(i*size)], rcvpivot[x], MPI_INT, x, 7, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+6]);
    MPI_Irecv(&rvparmat_buffer[(i*size)], rcvpivot[x], MPI_INT, x, 8, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+7]);
  }

  for(int i=0; i<n_export_unique_procs; i++)
  {
    int x = export_unique_procs[i]; 
    
    MPI_Isend(&tbuffer[0][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 2, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
    MPI_Isend(&tbuffer[1][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 3, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
    MPI_Isend(&tbuffer[2][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 4, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);  
    MPI_Isend(&vbuffer[idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 5, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+4]);
    MPI_Isend(&angbuffer[idx[i]*6], pivot[x]*6, MPI_DOUBLE, x, 6, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+5]);
    MPI_Isend(&pid_buffer[idx[i]], pivot[x], MPI_INT, x, 7, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+6]);
    MPI_Isend(&parmat_buffer[idx[i]], pivot[x], MPI_INT, x, 8, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+7]);
  }
 
  if(nt > 0 && num_export > 0) 
  {
    receive_idx = nt - num_export; //set to last id
  } else if(nt >= 0 && num_export <= 0){
    receive_idx = nt;
  }
 
  for(int i=0; i<n_import_unique_procs; i++)
  {
    MPI_Wait(&myrvRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
    //printf("RANK[%i]:received\n", myrank);

    int x = import_unique_procs[i];
    for(int j=0; j<rcvpivot[x]; j++)
    {
      pid[receive_idx] = rcvpid_buffer[(i*size)+j];
      parmat[receive_idx] = rvparmat_buffer[(i*size)+j];
      for(int k=0;k<3;k++)
      {
        t[0][k][receive_idx] = trvbuffer[0][(i*size*3)+(j*3)+(k)];        
        t[1][k][receive_idx] = trvbuffer[1][(i*size*3)+(j*3)+(k)]; 
        t[2][k][receive_idx] = trvbuffer[2][(i*size*3)+(j*3)+(k)]; 
        
        v[k][receive_idx] = vrvbuffer[(i*size*3)+(j*3)+(k)];
      }
      for(int k=0;k<6;k++)
      {
        angular[k][receive_idx] = angrvbuffer[(i*size*6)+(j*6)+(k)];
      }
      receive_idx++;
    }
  }
  
  for(int i=0; i<n_export_unique_procs; i++)
  {
    MPI_Wait(&myRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
  }

  nt = nt + (num_import-num_export);
 
  if(n_export_unique_procs)
  {
    free(tbuffer[0]);
    free(tbuffer[1]);
    free(tbuffer[2]);
    free(pid_buffer);
    free(vbuffer);
    free(angbuffer);
    free(parmat_buffer);
  }
  
   if(n_import_unique_procs)
   {
     free(trvbuffer[0]);
     free(trvbuffer[1]);
     free(trvbuffer[2]);
     free(rcvpid_buffer);   
     free(vrvbuffer);
     free(angrvbuffer);
     free(rvparmat_buffer);
   }
   
   for(int i=0; i<nproc;i++)
   {
     free(send_idx[i]);
   }
     
   free(idx);
   free(pivot);
   free(rcvpivot);
    
   free(export_unique_procs);
   free(import_unique_procs);
    
   free(myRequest);
   free(myrvRequest);
}

void migrateGhosts(struct loba *lb, int  myrank, int nt, iREAL *t[6][3], 
              iREAL *v[3], iREAL *angular[6], int *parmat,
              iREAL dt, iREAL *p[3], iREAL *q[3], 
              int tid[], int pid[], std::vector<contact> conpnt[], 
              iREAL *timer1, iREAL *timer2, iREAL *timer3)
{
  TIMING t1, t2, t3;
  
  timerstart(&t1);
  int nproc=0;
  int nNeighbors=0;

  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  int *neighborhood = (int *) malloc(nproc * sizeof(int));
  loba_getAdjacent(lb, myrank, neighborhood, &nNeighbors);
  
  int *ghostTID = (int*) malloc(nt*sizeof(int));
  int *ghostPID = (int*) malloc(nt*sizeof(int));
  int **ghostTIDNeighbors = (int**) malloc(nt*sizeof(int*));
  int *ghostTIDcrosses = (int*) malloc(nt*sizeof(int));
  int *ghostNeighborhood = (int*) malloc(nproc*sizeof(int));
  
  for(int i = 0;i<nproc;i++){ghostNeighborhood[i] = -1;}
  for(int i = 0;i<nt;i++){ghostTIDNeighbors[i] = (int*) malloc(nNeighbors*sizeof(int));}
  int nGhosts, nGhostNeighbors;
  
  //get triangle tids that overlap into neighbors
  loba_getGhosts(lb, myrank, nNeighbors, nt, t, tid, pid, 
                      ghostTID, ghostPID, &nGhosts, 
                      &nGhostNeighbors, ghostNeighborhood, 
                      ghostTIDNeighbors, ghostTIDcrosses);
  printf("RANK[%i]: overlaps:%i\n", myrank, nGhosts);
  
  //allocate memory for buffers
  int *pivot, *tid_buffer, *pid_buffer, *rcvpivot, *rcvtid_buffer, *rcvpid_buffer;
  iREAL *tbuffer[3], *vbuffer, *angbuffer;
  iREAL *trvbuffer[3], *vrvbuffer, *angrvbuffer;
  int *parmat_buffer, *rvparmat_buffer;

  tbuffer[0] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[1] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[2] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  vbuffer = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  angbuffer = (iREAL *) malloc(nNeighbors*nGhosts*6*sizeof(iREAL));//six elements thus x2.
  
  tid_buffer = (int *) malloc(nNeighbors*nGhosts*sizeof(int));
  pid_buffer = (int *) malloc(nNeighbors*nGhosts*sizeof(int));
  parmat_buffer = (int *) malloc(nNeighbors*nGhosts*sizeof(int));

  pivot = (int *) malloc(nNeighbors*sizeof(int));
  rcvpivot = (int *) malloc(nNeighbors*sizeof(int));
  
  for(int i = 0; i < nNeighbors; i++)
  { //set send indices and pivots for buffers
    pivot[i] = 0; //set pivot to zero
    rcvpivot[i] = 0;  
  }
  
  //prepare export buffers
  for (int i = 0; i<nGhosts; i++)
  {
    for(int ii = 0; ii<ghostTIDcrosses[i]; ii++)
    {
      int oltid = ghostTID[i];
      int olpid = ghostPID[i];
      int j = ghostTIDNeighbors[i][ii];
      int jj = ghostNeighborhood[j];
      //printf("RANK[%i]: J:%i JJ:%i\n", myrank, j, jj);
      int xx = pivot[jj];
      
      parmat_buffer[(jj*nGhosts)+xx] = parmat[oltid];
      tid_buffer[(jj*nGhosts)+xx] = oltid;
      pid_buffer[(jj*nGhosts)+xx] = olpid;
      
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+0] = t[0][0][oltid]; //point 0
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+1] = t[0][1][oltid]; //point 0
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+2] = t[0][2][oltid]; //point 0
      
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+0] = t[1][0][oltid]; //point 1
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+1] = t[1][1][oltid]; //point 1
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+2] = t[1][2][oltid]; //point 1
      
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+0] = t[2][0][oltid]; //point 2
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+1] = t[2][1][oltid]; //point 2
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+2] = t[2][2][oltid]; //point 2
      
      vbuffer[(jj*nGhosts*3)+(xx*3)+0] = v[0][oltid];
      vbuffer[(jj*nGhosts*3)+(xx*3)+1] = v[1][oltid];
      vbuffer[(jj*nGhosts*3)+(xx*3)+2] = v[2][oltid];

      angbuffer[(jj*nGhosts*6)+(xx*6)+0] = angular[0][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+1] = angular[1][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+2] = angular[2][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+3] = angular[3][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+4] = angular[4][oltid];
      angbuffer[(jj*nGhosts*6)+(xx*6)+5] = angular[5][oltid];
      
      pivot[jj]++;
    }
  }
  
  //blocking communication
  for(int i=0; i<nNeighbors; i++)
  {
    int proc = neighborhood[i];
    MPI_Send(&pivot[i], 1, MPI_INT, proc, 1, MPI_COMM_WORLD);
  }
 
  int max = 0;
  for(int i=0; i<nNeighbors; i++)
  {
    int proc = neighborhood[i];
    MPI_Recv(&rcvpivot[i], 1, MPI_INT, proc, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if(rcvpivot[i] > max) max = rcvpivot[i];
  }
  
  int n = max;
  trvbuffer[0] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  trvbuffer[1] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  trvbuffer[2] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  vrvbuffer = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  angrvbuffer = (iREAL *) malloc(nNeighbors*n*6*sizeof(iREAL));//six elements thus x2.
  
  rvparmat_buffer = (int *) malloc(nNeighbors*n*sizeof(int));
  rcvtid_buffer = (int *) malloc(nNeighbors*n*sizeof(int));
  rcvpid_buffer = (int *) malloc(nNeighbors*n*sizeof(int));
 
  int MPISENDS = 8;
  MPI_Request *myRequest = (MPI_Request*) malloc(nNeighbors*MPISENDS*sizeof(MPI_Request));//6 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(nNeighbors*MPISENDS*sizeof(MPI_Request));//6 sends
  
  for(int i=0; i<nNeighbors; i++)
  {
    int proc = neighborhood[i];
    if(rcvpivot[i] > 0)
    {//safe check
      MPI_Irecv(&rcvtid_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+0]);
      MPI_Irecv(&trvbuffer[0][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
      MPI_Irecv(&trvbuffer[1][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
      MPI_Irecv(&trvbuffer[2][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
      
      MPI_Irecv(&vrvbuffer[(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+4]);
      MPI_Irecv(&angrvbuffer[(i*n*6)], rcvpivot[i]*6, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+5]);

      MPI_Irecv(&rcvpid_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+6]);
      MPI_Irecv(&rvparmat_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+7]); 
    }
  }

  for(int i=0;i<nNeighbors;i++)
  {
    int proc = neighborhood[i];
    if(pivot[i] > 0)
    {//safe check
      MPI_Isend(&tid_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+0]);
      MPI_Isend(&tbuffer[0][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
      MPI_Isend(&tbuffer[1][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
      MPI_Isend(&tbuffer[2][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);
      
      MPI_Isend(&vbuffer[(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+4]);
      MPI_Isend(&angbuffer[(i*nGhosts*6)], pivot[i]*6, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+5]);

      MPI_Isend(&pid_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+6]);
      MPI_Isend(&rvparmat_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+7]);
    }
  }
  
  timerend(&t1);
  *timer1 = t1.total;
 
  timerstart(&t2);
  contact_detection (0, nt, t, tid, pid, v, p, q, conpnt);//local computation
  timerend(&t2);
  *timer2 = t2.total;
  
  TIMING t4;
  iREAL tassign=0;
  timerstart(&t3);
  int receive_idx = nt; //set to last id
  for(int i=0;i<nNeighbors;i++)
  {
    if(rcvpivot[i] > 0)
    {
      MPI_Wait(&myrvRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
      MPI_Wait(&myrvRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
      
      timerstart(&t4);
      for(int j=0;j<rcvpivot[i];j++)
      {
        tid[receive_idx] = rcvtid_buffer[(i*n)+j]; //tids to imported
        pid[receive_idx] = rcvpid_buffer[(i*n)+j];
        parmat[receive_idx] = rvparmat_buffer[(i*n)+j];
        for(int k=0;k<3;k++)
        {
          t[0][k][receive_idx] = trvbuffer[0][(i*n*3)+(j*3)+(k)];
          t[1][k][receive_idx] = trvbuffer[1][(i*n*3)+(j*3)+(k)];
          t[2][k][receive_idx] = trvbuffer[2][(i*n*3)+(j*3)+(k)];
          
          v[k][receive_idx] = vrvbuffer[(i*n*3)+(j*3)+(k)];
        }
        for(int k=0;k<6;k++)
        {
          angular[k][receive_idx] = angrvbuffer[(i*n*6)+(j*6)+(k)];
        }
        receive_idx++;
      }
      timerend(&t4);
      tassign = tassign+t4.total;
    }
      
    if(pivot[i] > 0)
    {//safe check
      MPI_Wait(&myRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
      MPI_Wait(&myRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
    }
  }
  timerend(&t3);
  *timer3 = t3.total;//doesn't include timer 4
  
  if(receive_idx > nt)
  {
    printf("Myrank[%i]: nt:%i, receive:%i\n", myrank, nt, receive_idx);
    //range s1-e1 is outter loop, s2-e2 is inner loop in the traversal
    contact_detection (0, nt, nt, receive_idx, t, tid, pid, v, p, q, conpnt);
  }
  
  for(int i=0; i<3; i++)
  {
    free(tbuffer[i]);
    free(trvbuffer[i]);
  }
  
  free(pivot);
  free(rcvpivot);
  
  free(tid_buffer);
  free(rcvtid_buffer);
  
  free(pid_buffer);
  free(rcvpid_buffer);
 
  free(parmat_buffer);
  free(rvparmat_buffer);

  free(vbuffer);
  free(vrvbuffer);

  free(angbuffer);
  free(angrvbuffer);

  free(myRequest);
  free(myrvRequest);
  
  free(neighborhood);
  free(ghostTID);
  free(ghostPID);
  free(ghostTIDcrosses);
  free(ghostNeighborhood);
  for(int i = 0;i<nt;i++)
  {
    free(ghostTIDNeighbors[i]);
  }
}
