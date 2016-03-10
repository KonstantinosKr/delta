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

// migrate triangles "in-place" to new ranks 
void migrate (struct loba *lb, int &nt, int &nb, iREAL *t[6][3], int *parmat,
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

  iREAL *tbuffer[6];
  int *pid_buffer;int *parmat_buffer;
  
  int n = nt;
  
  if(n_export_unique_procs > 0)
  {
    int mul = n_export_unique_procs*n*3;
    tbuffer[0] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[1] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[2] = (iREAL *) malloc(mul*sizeof(iREAL)); 
    tbuffer[3] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[4] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[5] = (iREAL *) malloc(mul*sizeof(iREAL)); 
    
    mul = n_export_unique_procs*n;
    pid_buffer = (int *) malloc(mul*sizeof(int));
    parmat_buffer = (int *) malloc(mul*sizeof(int));
   // printf("RANK[%i]: n:%i allocated: %i n_export_unique_procs: %i\n", myrank, n, n_export_unique_procs, mul);
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
      
      tbuffer[3][mul+0] = t[3][0][send_idx[x][j]]; //point 0/A        
      tbuffer[3][mul+1] = t[3][1][send_idx[x][j]]; //point 0/A        
      tbuffer[3][mul+2] = t[3][2][send_idx[x][j]]; //point 0/A        
      
      tbuffer[4][mul+0] = t[4][0][send_idx[x][j]]; //point 1/B
      tbuffer[4][mul+1] = t[4][1][send_idx[x][j]]; //point 1/B
      tbuffer[4][mul+2] = t[4][2][send_idx[x][j]]; //point 1/B
      
      tbuffer[5][mul+0] = t[5][0][send_idx[x][j]]; //point 2/C
      tbuffer[5][mul+1] = t[5][1][send_idx[x][j]]; //point 2/C
      tbuffer[5][mul+2] = t[5][2][send_idx[x][j]]; //point 2/C
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
        
        t[3][0][export_local_ids[i]] = t[3][0][j];
        t[3][1][export_local_ids[i]] = t[3][1][j];
        t[3][2][export_local_ids[i]] = t[3][2][j];
          
        t[4][0][export_local_ids[i]] = t[4][0][j];
        t[4][1][export_local_ids[i]] = t[4][1][j];
        t[4][2][export_local_ids[i]] = t[4][2][j];
          
        t[5][0][export_local_ids[i]] = t[5][0][j];
        t[5][1][export_local_ids[i]] = t[5][1][j];
        t[5][2][export_local_ids[i]] = t[5][2][j];
          
        pv = j; //save cave location for next iteration, no need to start from end/back/nt again
        break; //break loop/search from 'last to first' and go to next id that was exported
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
  
  iREAL *trvbuffer[6];
  int *rcvpid_buffer; int *rvparmat_buffer;
  int size = 0; 

  int MPISENDS = 9;
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
    //int x = export_unique_procs[i];
    //printf("RANK:%i sent:%i\n", myrank, parpivot[x]);
    //printf("RANK:%i received:%i\n", myrank, parrcvpivot[x]);
  }
  
  if(n_import_unique_procs > 0)
  {
    trvbuffer[0] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[1] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[2] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL)); 
    trvbuffer[3] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[4] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[5] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL)); 

    rcvpid_buffer = (int *) malloc(n_import_unique_procs*size*sizeof(int));
    rvparmat_buffer = (int *) malloc(n_import_unique_procs*size*sizeof(int));
  }

  for(int i=0; i<n_import_unique_procs; i++)
  {
    int x = import_unique_procs[i];  
    //printf("RANK[%d]: receive from rank %d\n", myrank, x);
   
    MPI_Irecv(&trvbuffer[0][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
    MPI_Irecv(&trvbuffer[1][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 2, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
    MPI_Irecv(&trvbuffer[2][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 3, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
    MPI_Irecv(&trvbuffer[3][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 4, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+4]);
    MPI_Irecv(&trvbuffer[4][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 5, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+5]);
    MPI_Irecv(&trvbuffer[5][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 6, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+6]);
    
    MPI_Irecv(&rcvpid_buffer[(i*size)], rcvpivot[x], MPI_INT, x, 7, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+7]);
    MPI_Irecv(&rvparmat_buffer[(i*size)], rcvpivot[x], MPI_INT, x, 8, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+8]);
  }

  for(int i=0; i<n_export_unique_procs; i++)
  {
    int x = export_unique_procs[i]; 
    
    MPI_Isend(&tbuffer[0][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
    MPI_Isend(&tbuffer[1][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 2, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
    MPI_Isend(&tbuffer[2][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 3, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);  
    MPI_Isend(&tbuffer[3][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 4, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+4]);
    MPI_Isend(&tbuffer[4][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 5, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+5]);
    MPI_Isend(&tbuffer[5][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 6, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+6]);  
  
    MPI_Isend(&pid_buffer[idx[i]], pivot[x], MPI_INT, x, 7, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+7]);
    MPI_Isend(&parmat_buffer[idx[i]], pivot[x], MPI_INT, x, 8, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+8]);
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
    MPI_Wait(&myrvRequest[(i*MPISENDS)+8], MPI_STATUS_IGNORE);
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
        t[3][k][receive_idx] = trvbuffer[3][(i*size*3)+(j*3)+(k)];        
        t[4][k][receive_idx] = trvbuffer[4][(i*size*3)+(j*3)+(k)]; 
        t[5][k][receive_idx] = trvbuffer[5][(i*size*3)+(j*3)+(k)]; 
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
    MPI_Wait(&myRequest[(i*MPISENDS)+8], MPI_STATUS_IGNORE);
  }

  nt = nt + (num_import-num_export);
 
  if(n_export_unique_procs)
  {
    free(tbuffer[0]);
    free(tbuffer[1]);
    free(tbuffer[2]);
    free(tbuffer[3]);
    free(tbuffer[4]);
    free(tbuffer[5]);
    free(pid_buffer);
    free(parmat_buffer);
  }
  
 if(n_import_unique_procs)
 {
   free(trvbuffer[0]);
   free(trvbuffer[1]);
   free(trvbuffer[2]);
   free(trvbuffer[3]);
   free(trvbuffer[4]);
   free(trvbuffer[5]);
   free(rcvpid_buffer);   
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

// migrate triangles "in-place" to new ranks 
void init_migratePosition (struct loba *lb, int &nb, iREAL *linear[3],
                    iREAL *angular[6], iREAL *rotation[9], 
                    iREAL *position[6], iREAL *inertia[9], 
                    iREAL *inverse[9], iREAL mass[])
{
  int nproc, myrank;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
 
  //allocate memory for tmp buffers
  int **send_paridx = (int **) malloc(nproc*sizeof(int*));
  int *parrcvpivot = (int *) malloc(nproc*sizeof(int));
  int *parpivot = (int *) malloc(nproc*sizeof(int));
  int *paridx = (int *) malloc(nproc*sizeof(int));

  int found = 0;
  for(int i=0;i<nproc;i++)
  {
    if(i==myrank)continue;
    send_paridx[i] = (int *) malloc((nb)*sizeof(int));
    parrcvpivot[i] = 0;
    parpivot[i] = 0;
    paridx[i] = 0;
    if(myrank != i)
    for (int j = 0; j < nb; j++)//loop through export data/ids 
    {
      printf("nb:%i is in rank:%i\n", j, myrank);
      int x = parpivot[i];
      send_paridx[i][x] = j;
      parpivot[i]++;
      found = 1;
    }
    paridx[i+1] = paridx[i] + parpivot[i];
  }
  
  iREAL *vbuffer, *angbuffer;
  int *paridxbuffer;
  iREAL *rotationbuffer, *positionbuffer, *inertiabuffer, *inversebuffer, *massbuffer;
  int n = nb;
  if(found)
  {
    int mul = nproc*n;
    paridxbuffer = (int *) malloc(mul*sizeof(int)); 
    vbuffer = (iREAL *) malloc(mul*3*sizeof(iREAL));
    angbuffer = (iREAL *) malloc(mul*6*sizeof(iREAL));//6 elements thus x 2
    positionbuffer = (iREAL *) malloc(mul*6*sizeof(iREAL));
    massbuffer = (iREAL *) malloc(mul*sizeof(iREAL)); 
    
    rotationbuffer = (iREAL *) malloc(mul*9*sizeof(iREAL));
    inertiabuffer = (iREAL *) malloc(mul*9*sizeof(iREAL));
    inversebuffer = (iREAL *) malloc(mul*9*sizeof(iREAL));

    for(int i=0; i<nproc; i++)//n processes to prepare buffers for
    {
      if(i==myrank)continue;
      for(int j=0; j<parpivot[i]; j++)
      {
        int mul = (paridx[i]*3)+(j*3);
        vbuffer[mul+0] = linear[0][send_paridx[i][j]];
        vbuffer[mul+1] = linear[1][send_paridx[i][j]];
        vbuffer[mul+2] = linear[2][send_paridx[i][j]];
        
        mul = (paridx[i]*6)+(j*6);
        angbuffer[mul+0] = angular[0][send_paridx[i][j]];
        angbuffer[mul+1] = angular[1][send_paridx[i][j]];
        angbuffer[mul+2] = angular[2][send_paridx[i][j]];
        angbuffer[mul+3] = angular[3][send_paridx[i][j]];
        angbuffer[mul+4] = angular[4][send_paridx[i][j]];
        angbuffer[mul+5] = angular[5][send_paridx[i][j]];
        
        positionbuffer[mul+0] = position[0][send_paridx[i][j]];
        positionbuffer[mul+1] = position[1][send_paridx[i][j]];
        positionbuffer[mul+2] = position[2][send_paridx[i][j]];
        positionbuffer[mul+3] = position[3][send_paridx[i][j]];
        positionbuffer[mul+4] = position[4][send_paridx[i][j]];
        positionbuffer[mul+5] = position[5][send_paridx[i][j]];

        mul = (paridx[i]*9)+(j*9);
        rotationbuffer[mul+0] = rotation[0][send_paridx[i][j]];
        rotationbuffer[mul+1] = rotation[1][send_paridx[i][j]];
        rotationbuffer[mul+2] = rotation[2][send_paridx[i][j]];
        rotationbuffer[mul+3] = rotation[3][send_paridx[i][j]];
        rotationbuffer[mul+4] = rotation[4][send_paridx[i][j]];
        rotationbuffer[mul+5] = rotation[5][send_paridx[i][j]];
        rotationbuffer[mul+6] = rotation[6][send_paridx[i][j]];
        rotationbuffer[mul+7] = rotation[7][send_paridx[i][j]];
        rotationbuffer[mul+8] = rotation[8][send_paridx[i][j]];
        
        inertiabuffer[mul+0] = inertia[0][send_paridx[i][j]]; 
        inertiabuffer[mul+1] = inertia[1][send_paridx[i][j]]; 
        inertiabuffer[mul+2] = inertia[2][send_paridx[i][j]]; 
        inertiabuffer[mul+3] = inertia[3][send_paridx[i][j]]; 
        inertiabuffer[mul+4] = inertia[4][send_paridx[i][j]]; 
        inertiabuffer[mul+5] = inertia[5][send_paridx[i][j]]; 
        inertiabuffer[mul+6] = inertia[6][send_paridx[i][j]]; 
        inertiabuffer[mul+7] = inertia[7][send_paridx[i][j]]; 
        inertiabuffer[mul+8] = inertia[8][send_paridx[i][j]]; 

        inversebuffer[mul+0] = inverse[0][send_paridx[i][j]]; 
        inversebuffer[mul+1] = inverse[1][send_paridx[i][j]]; 
        inversebuffer[mul+2] = inverse[2][send_paridx[i][j]]; 
        inversebuffer[mul+3] = inverse[3][send_paridx[i][j]]; 
        inversebuffer[mul+4] = inverse[4][send_paridx[i][j]]; 
        inversebuffer[mul+5] = inverse[5][send_paridx[i][j]]; 
        inversebuffer[mul+6] = inverse[6][send_paridx[i][j]]; 
        inversebuffer[mul+7] = inverse[7][send_paridx[i][j]]; 
        inversebuffer[mul+8] = inverse[8][send_paridx[i][j]]; 
        
        mul = (paridx[i])+(j);
        paridxbuffer[mul] = send_paridx[i][j];
        massbuffer[mul] = mass[send_paridx[i][j]];
      }
    }
  }
  
  iREAL *vrvbuffer, *angrvbuffer;
  int *rcv_paridx;
  iREAL *rcvpositionbuffer, *rcvrotationbuffer, *rcvinertiabuffer, *rcvinversebuffer, *rcvmassbuffer; 
  int parsize = 0;  

  int MPISENDS = 9;
  MPI_Request *myRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends 
 
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Irecv(&parrcvpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Isend(&parpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
    if(parrcvpivot[i] > parsize) parsize = parrcvpivot[i];
  }
  
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
  }
  
  if(nproc > 0)
  {
    vrvbuffer = (iREAL *) malloc(nproc*parsize*3*sizeof(iREAL));
    angrvbuffer = (iREAL *) malloc(nproc*parsize*6*sizeof(iREAL));//six elements
    rcv_paridx = (int *) malloc(nproc*parsize*sizeof(int)); 
    
    rcvpositionbuffer = (iREAL *) malloc(nproc*parsize*6*sizeof(iREAL));
    rcvrotationbuffer = (iREAL *) malloc(nproc*parsize*9*sizeof(iREAL));
    rcvinertiabuffer = (iREAL *) malloc(nproc*parsize*9*sizeof(iREAL));
    rcvinversebuffer = (iREAL *) malloc(nproc*parsize*9*sizeof(iREAL));
    rcvmassbuffer = (iREAL *) malloc(nproc*parsize*sizeof(iREAL));
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Irecv(&rcv_paridx[(i*parsize)], parrcvpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
    MPI_Irecv(&vrvbuffer[(i*parsize*3)], parrcvpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
    MPI_Irecv(&angrvbuffer[(i*parsize*6)], parrcvpivot[i]*6, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
    MPI_Irecv(&rcvpositionbuffer[(i*parsize*6)], parrcvpivot[i]*6, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+4]); 
    MPI_Irecv(&rcvrotationbuffer[(i*parsize*9)], parrcvpivot[i]*9, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+5]);
    MPI_Irecv(&rcvinertiabuffer[(i*parsize*9)], parrcvpivot[i]*9, MPI_DOUBLE, i, 6, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+6]);
    MPI_Irecv(&rcvinversebuffer[(i*parsize*9)], parrcvpivot[i]*9, MPI_DOUBLE, i, 7, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+7]);
    MPI_Irecv(&rcvmassbuffer[(i*parsize)], parrcvpivot[i], MPI_DOUBLE, i, 8, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+8]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Isend(&paridxbuffer[paridx[i]], parpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
    MPI_Isend(&vbuffer[paridx[i]*3], parpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
    MPI_Isend(&angbuffer[paridx[i]*6], parpivot[i]*6, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);
    MPI_Isend(&positionbuffer[paridx[i]*6], parpivot[i]*6, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+4]);
    MPI_Isend(&rotationbuffer[paridx[i]*9], parpivot[i]*9, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+5]);
    MPI_Isend(&inertiabuffer[paridx[i]*9], parpivot[i]*9, MPI_DOUBLE, i, 6, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+6]);
    MPI_Isend(&inversebuffer[paridx[i]*9], parpivot[i]*9, MPI_DOUBLE, i, 7, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+7]);
    MPI_Isend(&massbuffer[paridx[i]], parpivot[i], MPI_DOUBLE, i, 8, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+8]);
  }
 
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+8], MPI_STATUS_IGNORE);
    //printf("RANK[%i]:received\n", myrank);

    for(int j=0; j<parrcvpivot[i]; j++)
    {     
      int xx = rcv_paridx[(i*parsize)+j];
      
      mass[xx] = rcvmassbuffer[(i*parsize)+j];
       
      linear[0][xx] = vrvbuffer[(i*parsize*3)+(j*3)+0];
      linear[1][xx] = vrvbuffer[(i*parsize*3)+(j*3)+1];
      linear[2][xx] = vrvbuffer[(i*parsize*3)+(j*3)+2];
      
      angular[0][xx] = angrvbuffer[(i*parsize*6)+(j*6)+0];
      angular[1][xx] = angrvbuffer[(i*parsize*6)+(j*6)+1];
      angular[2][xx] = angrvbuffer[(i*parsize*6)+(j*6)+2];
      angular[3][xx] = angrvbuffer[(i*parsize*6)+(j*6)+3];
      angular[4][xx] = angrvbuffer[(i*parsize*6)+(j*6)+4];
      angular[5][xx] = angrvbuffer[(i*parsize*6)+(j*6)+5];
    
      position[0][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+0];
      position[1][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+1];
      position[2][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+2];
      position[3][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+3];
      position[4][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+4];
      position[5][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+5];
      
      rotation[0][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+0];
      rotation[1][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+1];
      rotation[2][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+2];
      rotation[3][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+3];
      rotation[4][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+4];
      rotation[5][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+5];
      rotation[6][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+6];
      rotation[7][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+7];
      rotation[8][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+8];
      
      inertia[0][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+0];
      inertia[1][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+1];
      inertia[2][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+2];
      inertia[3][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+3];
      inertia[4][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+4];
      inertia[5][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+5];
      inertia[6][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+6];
      inertia[7][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+7];
      inertia[8][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+8];
      
      inverse[0][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+0];
      inverse[1][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+1];
      inverse[2][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+2];
      inverse[3][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+3];
      inverse[4][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+4];
      inverse[5][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+5];
      inverse[6][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+6];
      inverse[7][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+7];
      inverse[8][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+8];
      nb++;
    }
  }
  
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+8], MPI_STATUS_IGNORE);
  }

  if(found)
  {
    free(vbuffer);
    free(angbuffer);
  }
  
 free(vrvbuffer);
 free(angrvbuffer);
 
 for(int i=0; i<nproc;i++)
 {
   if(i==myrank)continue;
   free(send_paridx[i]);
 }
  
 free(myRequest);
 free(myrvRequest);
}

// migrate triangles "in-place" to new ranks 
void migratePosition (struct loba *lb, int &nb, iREAL *linear[3],
                    iREAL *angular[6], iREAL *rotation[9], 
                    iREAL *position[6], iREAL *inertia[9], 
                    iREAL *inverse[9])
{
  int nproc, myrank;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
 
  //allocate memory for tmp buffers
  int **send_paridx = (int **) malloc(nproc*sizeof(int*));
  int *parrcvpivot = (int *) malloc(nproc*sizeof(int));
  int *parpivot = (int *) malloc(nproc*sizeof(int));
  int *paridx = (int *) malloc(nproc*sizeof(int));

  int found = 0;
  for(int i=0;i<nproc;i++)
  {
    if(myrank == i) continue;
    send_paridx[i] = (int *) malloc((nb)*sizeof(int));
    parrcvpivot[i] = 0;
    parpivot[i] = 0;
    paridx[i] = 0;//if there is bug look this line
    
    for (int j = 0; j < nb; j++)//loop through export data/ids 
    {
      iREAL point[3];
      point[0] = position[0][j];
      point[1] = position[1][j];
      point[2] = position[2][j];

      int rnk;
      loba_query (lb, point, &rnk);
      if(myrank == rnk)
      {
        int x = parpivot[i];
        send_paridx[i][x] = j;
        parpivot[i]++;
        //printf("RANK:%i is SENDING to RANK:%i\n", myrank, i);
        found = 1;
      }
    }
    paridx[i+1] = paridx[i] + parpivot[i];
  }
  
  iREAL *vbuffer, *angbuffer;
  int *paridxbuffer;
  iREAL *rotationbuffer, *positionbuffer, *inertiabuffer, *inversebuffer;
  int n = nb*10000;
  if(found)
  {
    int mul = nproc*n;
    vbuffer = (iREAL *) malloc(mul*3*sizeof(iREAL));
    angbuffer = (iREAL *) malloc(mul*6*sizeof(iREAL));//6 elements thus x 2
    positionbuffer = (iREAL *) malloc(mul*6*sizeof(iREAL));
    paridxbuffer = (int *) malloc(mul*sizeof(int)); 
    
    rotationbuffer = (iREAL *) malloc(mul*9*sizeof(iREAL));
    inertiabuffer = (iREAL *) malloc(mul*9*sizeof(iREAL));
    inversebuffer = (iREAL *) malloc(mul*9*sizeof(iREAL));

    for(int i=0; i<nproc; i++)//n processes to prepare buffers for
    {
      if(myrank == i) continue;
      for(int j=0; j<parpivot[i]; j++)
      {
        int mul = (paridx[i]*3)+(j*3);
        vbuffer[mul+0] = linear[0][send_paridx[i][j]];
        vbuffer[mul+1] = linear[1][send_paridx[i][j]];
        vbuffer[mul+2] = linear[2][send_paridx[i][j]];
       
        mul = (paridx[i]*6)+(j*6);
        angbuffer[mul+0] = angular[0][send_paridx[i][j]];
        angbuffer[mul+1] = angular[1][send_paridx[i][j]];
        angbuffer[mul+2] = angular[2][send_paridx[i][j]];
        angbuffer[mul+3] = angular[3][send_paridx[i][j]];
        angbuffer[mul+4] = angular[4][send_paridx[i][j]];
        angbuffer[mul+5] = angular[5][send_paridx[i][j]];
        
        positionbuffer[mul+0] = position[0][send_paridx[i][j]];
        positionbuffer[mul+1] = position[1][send_paridx[i][j]];
        positionbuffer[mul+2] = position[2][send_paridx[i][j]];
        positionbuffer[mul+3] = position[3][send_paridx[i][j]];
        positionbuffer[mul+4] = position[4][send_paridx[i][j]];
        positionbuffer[mul+5] = position[5][send_paridx[i][j]];

        mul = (paridx[i]*9)+(j*9);
        rotationbuffer[mul+0] = rotation[0][send_paridx[i][j]];
        rotationbuffer[mul+1] = rotation[1][send_paridx[i][j]];
        rotationbuffer[mul+2] = rotation[2][send_paridx[i][j]];
        rotationbuffer[mul+3] = rotation[3][send_paridx[i][j]];
        rotationbuffer[mul+4] = rotation[4][send_paridx[i][j]];
        rotationbuffer[mul+5] = rotation[5][send_paridx[i][j]];
        rotationbuffer[mul+6] = rotation[6][send_paridx[i][j]];
        rotationbuffer[mul+7] = rotation[7][send_paridx[i][j]];
        rotationbuffer[mul+8] = rotation[8][send_paridx[i][j]];
        
        inertiabuffer[mul+0] = inertia[0][send_paridx[i][j]]; 
        inertiabuffer[mul+1] = inertia[1][send_paridx[i][j]]; 
        inertiabuffer[mul+2] = inertia[2][send_paridx[i][j]]; 
        inertiabuffer[mul+3] = inertia[3][send_paridx[i][j]]; 
        inertiabuffer[mul+4] = inertia[4][send_paridx[i][j]]; 
        inertiabuffer[mul+5] = inertia[5][send_paridx[i][j]]; 
        inertiabuffer[mul+6] = inertia[6][send_paridx[i][j]]; 
        inertiabuffer[mul+7] = inertia[7][send_paridx[i][j]]; 
        inertiabuffer[mul+8] = inertia[8][send_paridx[i][j]]; 

        inversebuffer[mul+0] = inverse[0][send_paridx[i][j]]; 
        inversebuffer[mul+1] = inverse[1][send_paridx[i][j]]; 
        inversebuffer[mul+2] = inverse[2][send_paridx[i][j]]; 
        inversebuffer[mul+3] = inverse[3][send_paridx[i][j]]; 
        inversebuffer[mul+4] = inverse[4][send_paridx[i][j]]; 
        inversebuffer[mul+5] = inverse[5][send_paridx[i][j]]; 
        inversebuffer[mul+6] = inverse[6][send_paridx[i][j]]; 
        inversebuffer[mul+7] = inverse[7][send_paridx[i][j]]; 
        inversebuffer[mul+8] = inverse[8][send_paridx[i][j]]; 
        
        mul = (paridx[i])+(j);
        paridxbuffer[mul] = send_paridx[i][j];
      }
    }
  }
  
  iREAL *vrvbuffer, *angrvbuffer;
  int *rcv_paridx;
  iREAL *rcvpositionbuffer, *rcvrotationbuffer, *rcvinertiabuffer, *rcvinversebuffer; 
  int parsize = 0;  

  int MPISENDS = 8;
  MPI_Request *myRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends 
 
  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Irecv(&parrcvpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Isend(&parpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
    if(parrcvpivot[i] > parsize) parsize = parrcvpivot[i];
  }
  
  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
  }
  
  if(nproc > 0)
  {
    vrvbuffer = (iREAL *) malloc(nproc*parsize*3*sizeof(iREAL));
    angrvbuffer = (iREAL *) malloc(nproc*parsize*6*sizeof(iREAL));//six elements
    rcv_paridx = (int *) malloc(nproc*parsize*sizeof(int)); 
    
    rcvpositionbuffer = (iREAL *) malloc(nproc*parsize*6*sizeof(iREAL));
    rcvrotationbuffer = (iREAL *) malloc(nproc*parsize*9*sizeof(iREAL));
    rcvinertiabuffer = (iREAL *) malloc(nproc*parsize*9*sizeof(iREAL));
    rcvinversebuffer = (iREAL *) malloc(nproc*parsize*9*sizeof(iREAL));
  }

  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Irecv(&rcv_paridx[(i*parsize)], parrcvpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
    MPI_Irecv(&vrvbuffer[(i*parsize*3)], parrcvpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
    MPI_Irecv(&angrvbuffer[(i*parsize*6)], parrcvpivot[i]*6, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
    MPI_Irecv(&rcvpositionbuffer[(i*parsize*6)], parrcvpivot[i]*6, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+4]); 
    MPI_Irecv(&rcvrotationbuffer[(i*parsize*9)], parrcvpivot[i]*9, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+5]);
    MPI_Irecv(&rcvinertiabuffer[(i*parsize*9)], parrcvpivot[i]*9, MPI_DOUBLE, i, 6, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+6]);
    MPI_Irecv(&rcvinversebuffer[(i*parsize*9)], parrcvpivot[i]*9, MPI_DOUBLE, i, 7, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+7]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Isend(&paridxbuffer[paridx[i]], parpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
    MPI_Isend(&vbuffer[paridx[i]*3], parpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
    MPI_Isend(&angbuffer[paridx[i]*6], parpivot[i]*6, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);
    MPI_Isend(&positionbuffer[paridx[i]*6], parpivot[i]*6, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+4]);
    MPI_Isend(&rotationbuffer[paridx[i]*9], parpivot[i]*9, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+5]);
    MPI_Isend(&inertiabuffer[paridx[i]*9], parpivot[i]*9, MPI_DOUBLE, i, 6, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+6]);
    MPI_Isend(&inversebuffer[paridx[i]*9], parpivot[i]*9, MPI_DOUBLE, i, 7, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+7]);
  }
 
  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
    //printf("RANK[%i]:received\n", myrank);

    for(int j=0; j<parrcvpivot[i]; j++)
    {     
      int xx = rcv_paridx[(i*parsize)+j];
      linear[0][xx] = vrvbuffer[(i*parsize*3)+(j*3)+0];
      linear[1][xx] = vrvbuffer[(i*parsize*3)+(j*3)+1];
      linear[2][xx] = vrvbuffer[(i*parsize*3)+(j*3)+2];
      
      angular[0][xx] = angrvbuffer[(i*parsize*6)+(j*6)+0];
      angular[1][xx] = angrvbuffer[(i*parsize*6)+(j*6)+1];
      angular[2][xx] = angrvbuffer[(i*parsize*6)+(j*6)+2];
      angular[3][xx] = angrvbuffer[(i*parsize*6)+(j*6)+3];
      angular[4][xx] = angrvbuffer[(i*parsize*6)+(j*6)+4];
      angular[5][xx] = angrvbuffer[(i*parsize*6)+(j*6)+5];
    
      position[0][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+0];
      position[1][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+1];
      position[2][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+2];
      position[3][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+3];
      position[4][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+4];
      position[5][xx] = rcvpositionbuffer[(i*parsize*6)+(j*6)+5];
      
      rotation[0][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+0];
      rotation[1][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+1];
      rotation[2][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+2];
      rotation[3][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+3];
      rotation[4][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+4];
      rotation[5][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+5];
      rotation[6][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+6];
      rotation[7][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+7];
      rotation[8][xx] = rcvrotationbuffer[(i*parsize*9)+(j*9)+8];
      
      inertia[0][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+0];
      inertia[1][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+1];
      inertia[2][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+2];
      inertia[3][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+3];
      inertia[4][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+4];
      inertia[5][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+5];
      inertia[6][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+6];
      inertia[7][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+7];
      inertia[8][xx] = rcvinertiabuffer[(i*parsize*9)+(j*9)+8];
      
      inverse[0][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+0];
      inverse[1][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+1];
      inverse[2][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+2];
      inverse[3][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+3];
      inverse[4][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+4];
      inverse[5][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+5];
      inverse[6][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+6];
      inverse[7][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+7];
      inverse[8][xx] = rcvinversebuffer[(i*parsize*9)+(j*9)+8];
    }
  }
  
  for(int i=0; i<nproc; i++)
  {
    if(myrank == i) continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+5], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+6], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+7], MPI_STATUS_IGNORE);
  }

  if(found)
  {
    free(vbuffer);
    free(angbuffer);
  }
  
 free(vrvbuffer);
 free(angrvbuffer);
 
 for(int i=0; i<nproc;i++)
 {
   if(i == myrank) continue;
   free(send_paridx[i]);
 }
  
 free(myRequest);
 free(myrvRequest);
}

void migrateGhosts(struct loba *lb, int  myrank, int nt, int nb, iREAL *t[6][3], int *parmat,
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
  int *ghostlocalTID = (int*) malloc(nt*sizeof(int));
  int *ghostPID = (int*) malloc(nt*sizeof(int));
  int **ghostTIDNeighbors = (int**) malloc(nt*sizeof(int*));
  int *ghostTIDcrosses = (int*) malloc(nt*sizeof(int));
  int *ghostNeighborhood = (int*) malloc(nproc*sizeof(int));
  
  for(int i = 0;i<nproc;i++){ghostNeighborhood[i] = -1;}
  for(int i = 0;i<nt;i++){ghostTIDNeighbors[i] = (int*) malloc(nNeighbors*sizeof(int));}
  int nGhosts, nGhostNeighbors;
  
  //get triangle tids that overlap into neighbors
  loba_getGhosts(lb, myrank, nNeighbors, nt, t, tid, pid, 
                      ghostlocalTID, ghostTID, ghostPID, &nGhosts, 
                      &nGhostNeighbors, ghostNeighborhood, 
                      ghostTIDNeighbors, ghostTIDcrosses);
  printf("RANK[%i]: overlaps:%i\n", myrank, nGhosts);
  
  //allocate memory for buffers
  int *pivot, *tid_buffer, *pid_buffer, *rcvpivot, *rcvtid_buffer, *rcvpid_buffer;
  iREAL *tbuffer[6]; 
  iREAL *trvbuffer[6];
  int *parmat_buffer, *rvparmat_buffer;

  tbuffer[0] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[1] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[2] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[3] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[4] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  tbuffer[5] = (iREAL *) malloc(nNeighbors*nGhosts*3*sizeof(iREAL));
  
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
      int id = ghostlocalTID[i];
      int j = ghostTIDNeighbors[i][ii];
      int jj = ghostNeighborhood[j];
      //printf("RANK[%i]: J:%i JJ:%i\n", myrank, j, jj);
      int xx = pivot[jj];
      
      parmat_buffer[(jj*nGhosts)+xx] = parmat[oltid];
      tid_buffer[(jj*nGhosts)+xx] = oltid;
      pid_buffer[(jj*nGhosts)+xx] = olpid;
      
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+0] = t[0][0][id]; //point 0
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+1] = t[0][1][id]; //point 0
      tbuffer[0][(jj*nGhosts*3)+(xx*3)+2] = t[0][2][id]; //point 0
      
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+0] = t[1][0][id]; //point 1
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+1] = t[1][1][id]; //point 1
      tbuffer[1][(jj*nGhosts*3)+(xx*3)+2] = t[1][2][id]; //point 1
      
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+0] = t[2][0][id]; //point 2
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+1] = t[2][1][id]; //point 2
      tbuffer[2][(jj*nGhosts*3)+(xx*3)+2] = t[2][2][id]; //point 2
      
      tbuffer[3][(jj*nGhosts*3)+(xx*3)+0] = t[3][0][id]; //point 0
      tbuffer[3][(jj*nGhosts*3)+(xx*3)+1] = t[3][1][id]; //point 0
      tbuffer[3][(jj*nGhosts*3)+(xx*3)+2] = t[3][2][id]; //point 0
      
      tbuffer[4][(jj*nGhosts*3)+(xx*3)+0] = t[4][0][id]; //point 1
      tbuffer[4][(jj*nGhosts*3)+(xx*3)+1] = t[4][1][id]; //point 1
      tbuffer[4][(jj*nGhosts*3)+(xx*3)+2] = t[4][2][id]; //point 1
      
      tbuffer[5][(jj*nGhosts*3)+(xx*3)+0] = t[5][0][id]; //point 2
      tbuffer[5][(jj*nGhosts*3)+(xx*3)+1] = t[5][1][id]; //point 2
      tbuffer[5][(jj*nGhosts*3)+(xx*3)+2] = t[5][2][id]; //point 2
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
  trvbuffer[3] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  trvbuffer[4] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  trvbuffer[5] = (iREAL *) malloc(nNeighbors*n*3*sizeof(iREAL));
  
  rvparmat_buffer = (int *) malloc(nNeighbors*n*sizeof(int));
  rcvtid_buffer = (int *) malloc(nNeighbors*n*sizeof(int));
  rcvpid_buffer = (int *) malloc(nNeighbors*n*sizeof(int));
 
  int MPISENDS = 9;
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
      MPI_Irecv(&trvbuffer[3][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+4]);
      MPI_Irecv(&trvbuffer[4][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+5]);
      MPI_Irecv(&trvbuffer[5][(i*n*3)], rcvpivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+6]);
      
      MPI_Irecv(&rcvpid_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+7]);
      MPI_Irecv(&rvparmat_buffer[i*n], rcvpivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+8]); 
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
      MPI_Isend(&tbuffer[3][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+4]);
      MPI_Isend(&tbuffer[4][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+5]);
      MPI_Isend(&tbuffer[5][(i*nGhosts*3)], pivot[i]*3, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+6]);
      
      MPI_Isend(&pid_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+7]);
      MPI_Isend(&rvparmat_buffer[i*nGhosts], pivot[i], MPI_INT, proc, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+8]);
    }
  }
  
  timerend(&t1);
  *timer1 = t1.total;

  iREAL *v[3];
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
      MPI_Wait(&myrvRequest[(i*MPISENDS)+8], MPI_STATUS_IGNORE);
      
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
          
          t[3][k][receive_idx] = trvbuffer[3][(i*n*3)+(j*3)+(k)];
          t[4][k][receive_idx] = trvbuffer[4][(i*n*3)+(j*3)+(k)];
          t[5][k][receive_idx] = trvbuffer[5][(i*n*3)+(j*3)+(k)];
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
      MPI_Wait(&myRequest[(i*MPISENDS)+8], MPI_STATUS_IGNORE);
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

void migrateForce(struct loba *lb, int myrank, int *rank, int *fpid, int nranks, iREAL *force[3], iREAL *torque[3])
{
  int nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
 
  //allocate memory for tmp buffers
  int **send_paridx = (int **) malloc(nproc*sizeof(int*));
  int *parrcvpivot = (int *) malloc(nproc*sizeof(int));
  int *parpivot = (int *) malloc(nproc*sizeof(int));
  int *paridx = (int *) malloc(nproc*sizeof(int));
  
  for(int i=0;i<nproc;i++)
  {
    send_paridx[i] = (int *) malloc((1000)*sizeof(int));
    parrcvpivot[i] = 0;
    parpivot[i] = 0;
    paridx[i] = 0;
  }

  for(int i=0;i<nranks;i++)
  {
    int proc = rank[i];
    send_paridx[proc][parpivot[proc]++] = fpid[i]; 
    //printf("RANK:%i IS SENDING TO RANK:%i PARTICLE:%i\n", myrank, proc, fpid[i]);
  }
  for(int i=0; i<nproc; i++)
  {
    if(i == myrank) continue;
    paridx[i+1] = paridx[i] + parpivot[i];
    //printf("PROC:%i -> %i\n", i, parpivot[i]);
  } 
  
  iREAL *fbuffer, *tbuffer;
  int *paridxbuffer;
  int mul = nproc*1000;
  fbuffer = (iREAL *) malloc(mul*3*sizeof(iREAL));
  tbuffer = (iREAL *) malloc(mul*3*sizeof(iREAL));
  paridxbuffer = (int *) malloc(mul*sizeof(int)); 
  
  for(int i=0; i<nproc; i++)//n processes to prepare buffers for
  {
    if(i == myrank) continue;
    for(int j=0; j<parpivot[i]; j++)
    {
      int mul = (paridx[i]*3)+(j*3);
      fbuffer[mul+0] = force[0][send_paridx[i][j]];
      fbuffer[mul+1] = force[1][send_paridx[i][j]];
      fbuffer[mul+2] = force[2][send_paridx[i][j]];
      
      tbuffer[mul+0] = torque[0][send_paridx[i][j]];
      tbuffer[mul+1] = torque[1][send_paridx[i][j]];
      tbuffer[mul+2] = torque[2][send_paridx[i][j]];
      //printf("PROC:%i SENDS %f\n", i, fbuffer[mul+0]); 
      mul = (paridx[i])+(j);
      paridxbuffer[mul] = send_paridx[i][j];
    }
  }
  
  iREAL *rcvfbuffer, *rcvtbuffer;
  int *rcv_paridx;
  int parsize = 0;  

  int MPISENDS = 4;
  MPI_Request *myRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends 
 
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Irecv(&parrcvpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Isend(&parpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
    if(parrcvpivot[i] > parsize) parsize = parrcvpivot[i];
  }
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
  }
  
  if(nproc > 0)
  {
    rcvfbuffer = (iREAL *) malloc(nproc*parsize*3*sizeof(iREAL));
    rcvtbuffer = (iREAL *) malloc(nproc*parsize*3*sizeof(iREAL));
    rcv_paridx = (int *) malloc(nproc*parsize*sizeof(int)); 
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Irecv(&rcv_paridx[(i*parsize)], parrcvpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
    MPI_Irecv(&rcvfbuffer[(i*parsize*3)], parrcvpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
    MPI_Irecv(&rcvtbuffer[(i*parsize*3)], parrcvpivot[i]*3, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Isend(&paridxbuffer[paridx[i]], parpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
    MPI_Isend(&fbuffer[paridx[i]*3], parpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
    MPI_Isend(&tbuffer[paridx[i]*3], parpivot[i]*3, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);
  }
 
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);

    for(int j=0; j<parrcvpivot[i]; j++)
    {     
      int xx = rcv_paridx[(i*parsize)+j];
      force[0][xx] += rcvfbuffer[(i*parsize*3)+(j*3)+0];
      force[1][xx] += rcvfbuffer[(i*parsize*3)+(j*3)+1];
      force[2][xx] += rcvfbuffer[(i*parsize*3)+(j*3)+2];
      
      torque[0][xx] += rcvtbuffer[(i*parsize*3)+(j*3)+0];
      torque[1][xx] += rcvtbuffer[(i*parsize*3)+(j*3)+1];
      torque[2][xx] += rcvtbuffer[(i*parsize*3)+(j*3)+2];
      printf("RANK:%i IS RECEIVING FROM RANK:%i PARTICLE:%i FORCE[0]:%f\n", myrank, i, xx, force[0][xx]);
    }
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
  }
 
 for(int i=0; i<nproc;i++)
 {
   if(i==myrank) continue;
   free(send_paridx[i]);
 }
  
 free(myRequest);
 free(myrvRequest);
}

void migrateForceGlobal(struct loba *lb, int myrank, int nb, iREAL *position[3], iREAL *force[3], iREAL *torque[3])
{
  int nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
 
  //allocate memory for tmp buffers
  int **send_paridx = (int **) malloc(nproc*sizeof(int*));
  int *parrcvpivot = (int *) malloc(nproc*sizeof(int));
  int *parpivot = (int *) malloc(nproc*sizeof(int));
  int *paridx = (int *) malloc(nproc*sizeof(int));
  
  for(int i=0;i<nproc;i++)
  {
    send_paridx[i] = (int *) malloc((1000)*sizeof(int));
    parrcvpivot[i] = 0;
    parpivot[i] = 0;
    paridx[i] = 0;
  }

  for(int i=0;i<nb;i++)
  {
    int qrank;
    iREAL x[3];
    x[0] = position[0][i];
    x[1] = position[1][i];
    x[2] = position[2][i];

    loba_query(lb, x, &qrank); 
    if(qrank == myrank)
    {
      for(int j=0;j<nproc;j++)
      {
        if(j == myrank) continue;
        send_paridx[j][parpivot[j]++] = i; 
        //printf("RANK:%i IS SENDING TO RANK:%i PARTICLE:%i\n", myrank, j, i);
      }
    }
  }
  
  for(int i=0; i<nproc; i++)
  {
    if(i == myrank) continue;
    paridx[i+1] = paridx[i] + parpivot[i];
  } 
  
  iREAL *fbuffer, *tbuffer;
  int *paridxbuffer;
  int mul = nproc*nb;
  fbuffer = (iREAL *) malloc(mul*3*sizeof(iREAL));
  tbuffer = (iREAL *) malloc(mul*3*sizeof(iREAL));
  paridxbuffer = (int *) malloc(mul*sizeof(int)); 
  
  for(int i=0; i<nproc; i++)//n processes to prepare buffers for
  {
    if(i == myrank) continue;
    for(int j=0; j<parpivot[i]; j++)
    {
      int mul = (paridx[i]*3)+(j*3);
      fbuffer[mul+0] = force[0][send_paridx[i][j]];
      fbuffer[mul+1] = force[1][send_paridx[i][j]];
      fbuffer[mul+2] = force[2][send_paridx[i][j]];
      
      tbuffer[mul+0] = torque[0][send_paridx[i][j]];
      tbuffer[mul+1] = torque[1][send_paridx[i][j]];
      tbuffer[mul+2] = torque[2][send_paridx[i][j]];
      //printf("PROC:%i SENDS %f\n", i, fbuffer[mul+0]); 
      mul = (paridx[i])+(j);
      paridxbuffer[mul] = send_paridx[i][j];
    }
  }
  
  iREAL *rcvfbuffer, *rcvtbuffer;
  int *rcv_paridx;
  int parsize = 0;  

  int MPISENDS = 4;
  MPI_Request *myRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(nproc*MPISENDS*sizeof(MPI_Request));//4 sends 
 
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Irecv(&parrcvpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Isend(&parpivot[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+0]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
    if(parrcvpivot[i] > parsize) parsize = parrcvpivot[i];
  }
  
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+0], MPI_STATUS_IGNORE);
  }
  
  if(nproc > 0)
  {
    rcvfbuffer = (iREAL *) malloc(nproc*parsize*3*sizeof(iREAL));
    rcvtbuffer = (iREAL *) malloc(nproc*parsize*3*sizeof(iREAL));
    rcv_paridx = (int *) malloc(nproc*parsize*sizeof(int)); 
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Irecv(&rcv_paridx[(i*parsize)], parrcvpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+1]);
    MPI_Irecv(&rcvfbuffer[(i*parsize*3)], parrcvpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+2]);
    MPI_Irecv(&rcvtbuffer[(i*parsize*3)], parrcvpivot[i]*3, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myrvRequest[(i*MPISENDS)+3]);
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Isend(&paridxbuffer[paridx[i]], parpivot[i], MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+1]);
    MPI_Isend(&fbuffer[paridx[i]*3], parpivot[i]*3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+2]);
    MPI_Isend(&tbuffer[paridx[i]*3], parpivot[i]*3, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &myRequest[(i*MPISENDS)+3]);
  }
 
  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myrvRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);

    for(int j=0; j<parrcvpivot[i]; j++)
    {     
      int xx = rcv_paridx[(i*parsize)+j];
      force[0][xx] = rcvfbuffer[(i*parsize*3)+(j*3)+0];
      force[1][xx] = rcvfbuffer[(i*parsize*3)+(j*3)+1];
      force[2][xx] = rcvfbuffer[(i*parsize*3)+(j*3)+2];
      
      torque[0][xx] = rcvtbuffer[(i*parsize*3)+(j*3)+0];
      torque[1][xx] = rcvtbuffer[(i*parsize*3)+(j*3)+1];
      torque[2][xx] = rcvtbuffer[(i*parsize*3)+(j*3)+2];
      //printf("RANK:%i IS RECEIVING FROM RANK:%i PARTICLE:%i FORCE[0]:%f\n", myrank, i, xx, force[0][xx]);
    }
  }

  for(int i=0; i<nproc; i++)
  {
    if(i==myrank)continue;
    MPI_Wait(&myRequest[(i*MPISENDS)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*MPISENDS)+3], MPI_STATUS_IGNORE);
  }
 
 for(int i=0; i<nproc;i++)
 {
   if(i==myrank)continue;
   free(send_paridx[i]);
 }
  
 free(myRequest);
 free(myrvRequest);
}
