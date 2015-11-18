#include "migration.h"

/* migrate triangles "in-place" to new ranks */
void migrate_triangles (unsigned int size, unsigned int *nt, iREAL *t[3][3], iREAL *v[3],
                              unsigned int *tid, unsigned int *pid,  
                              int num_import, int *import_procs, int num_export, int *export_procs, 
                              ZOLTAN_ID_PTR import_global_ids, ZOLTAN_ID_PTR import_local_ids,
                              ZOLTAN_ID_PTR export_global_ids, ZOLTAN_ID_PTR export_local_ids)
{
  int nproc, myrank;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
  
  //allocate memory for tmp buffers
  unsigned int **send_idx = (unsigned int **) malloc(nproc*sizeof(unsigned int*));
  unsigned int *pivot = (unsigned int *) malloc(nproc*sizeof(unsigned int));
  unsigned int *rcvpivot = (unsigned int *) malloc(nproc*sizeof(unsigned int));
  int *export_unique_procs = (int*) malloc(nproc*sizeof(int));
  int *import_unique_procs = (int*) malloc(nproc*sizeof(int));

  for(int i=0;i<nproc;i++)
  {
    rcvpivot[i] = 0;
    pivot[i] = 0;
    send_idx[i] = (unsigned int *) malloc((*nt)*sizeof(unsigned int));
    export_unique_procs[i] = -1;
    import_unique_procs[i] = -1;
  }

  unsigned int n_export_unique_procs=0;//number of unique ids to export
  for (unsigned int i = 0; i < num_export; i++)//loop through export data/ids 
  {
    int proc = export_procs[i]; //proc is the export process for data id[i]
    
    //set send indices and pivot for buffers for each export process
    send_idx[proc][pivot[proc]] = export_local_ids[i];
    pivot[proc]++;
    
    tid[export_local_ids[i]] = UINT_MAX; //mark tid that will be exported thus deleted
    
    int exists = 0; //set to 0 to mean doesn't exist
    for(unsigned int j = 0; j < nproc; j++)
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

  iREAL *tbuffer[3], *vbuffer;
  unsigned int *pid_buffer;
  unsigned int n = *nt;
  
  if(n_export_unique_procs > 0)
  {
    unsigned int mul = n_export_unique_procs*n*3;
    tbuffer[0] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[1] = (iREAL *) malloc(mul*sizeof(iREAL));
    tbuffer[2] = (iREAL *) malloc(mul*sizeof(iREAL)); 
    vbuffer = (iREAL *) malloc(mul*sizeof(iREAL));
    mul = n_export_unique_procs*n;
    pid_buffer = (unsigned int *) malloc(mul*sizeof(unsigned int));
    //printf("RANK[%i]: n:%i allocated: %i n_export_unique_procs: %i\n", myrank, n, n_export_unique_procs, mul);
  }

  unsigned int *idx = (unsigned int *) malloc(nproc*sizeof(unsigned int));
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
    for(unsigned int j=0; j<pivot[x]; j++)//pivot gives n number of ids to loop through
    {
      unsigned int mul = (idx[i])+(j); 
      pid_buffer[(mul)+j] = pid[send_idx[x][j]];
      //pid_buffer[(i*n)+j] = pid[send_idx[x][j]];
      pid[send_idx[x][j]] = UINT_MAX;
    
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
    }
  }
  
  ///////////////////////////////////////////////
  //refine local arrays and ids (memory gaps)
  unsigned int pv = *nt-1;
  for(unsigned int i=0;i<num_export;i++)
  {//be cautious bug may be hidden here;
    for(unsigned int j=pv; j>export_local_ids[i]; j--)//from last towards first but only until gap of exported
    {
      if(tid[j] != UINT_MAX)//if not marked as to be exported switch fill gaps
      {
        tid[export_local_ids[i]] = tid[j]; //send from 'last to first' the tids to 'first to last' in tid array
        pid[export_local_ids[i]] = pid[j];
        tid[j] = UINT_MAX; //mark moved tid
        pid[j] = UINT_MAX;
        
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
        pv = j;//save cave location for next iteration, no need to start from end/back/nt again
        break;//break loop/search from 'last to first' and go to next id that was exported
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////
  
  unsigned int receive_idx=0;
  if(*nt > 0 && num_export > 0) 
  {
    receive_idx = *nt - num_export; //set to last id
  } else if(*nt >= 0 && num_export <= 0){
    receive_idx = *nt;
  }

  unsigned int n_import_unique_procs=0;
  for(unsigned int i=0; i < num_import; i++) //loop throught imports
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
  
  iREAL *trvbuffer[3], *vrvbuffer;
  unsigned int *rcvpid_buffer;
  size = 0;  

  MPI_Request *myRequest = (MPI_Request*) malloc(n_export_unique_procs*6*sizeof(MPI_Request));//4 sends
  MPI_Request *myrvRequest = (MPI_Request*) malloc(n_import_unique_procs*6*sizeof(MPI_Request));//4 sends 
 
  for(int i=0; i<n_import_unique_procs; i++)
  {
    int x = import_unique_procs[i];
    MPI_Irecv(&rcvpivot[x], 1, MPI_INT, x, 0, MPI_COMM_WORLD, &myrvRequest[(i*6)+0]);
  }

  for(int i=0; i<n_export_unique_procs; i++)
  {
    int x = export_unique_procs[i];
    MPI_Isend(&pivot[x], 1, MPI_INT, x, 0, MPI_COMM_WORLD, &myRequest[(i*6)+0]);
  }

  for(int i=0; i<n_import_unique_procs; i++)
  {
    int x = import_unique_procs[i];
    MPI_Wait(&myrvRequest[(i*6)+0], MPI_STATUS_IGNORE);
    if(rcvpivot[x] > size) size = rcvpivot[x];
  }
  
  for(int i=0; i<n_export_unique_procs; i++)
  {
    MPI_Wait(&myRequest[(i*6)+0], MPI_STATUS_IGNORE);
  }
  
  if(n_import_unique_procs > 0)
  {
    trvbuffer[0] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[1] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    trvbuffer[2] = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL)); 
    vrvbuffer = (iREAL *) malloc(n_import_unique_procs*size*3*sizeof(iREAL));
    rcvpid_buffer = (unsigned int *) malloc(n_import_unique_procs*size*sizeof(unsigned int));
  }

  for(int i=0; i<n_import_unique_procs; i++)
  {
    int x = import_unique_procs[i];  
    //printf("RANK[%d]: receive from rank %d\n", myrank, x);
   
    MPI_Irecv(&trvbuffer[0][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 2, MPI_COMM_WORLD, &myrvRequest[(i*6)+1]);
    MPI_Irecv(&trvbuffer[1][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 3, MPI_COMM_WORLD, &myrvRequest[(i*6)+2]);
    MPI_Irecv(&trvbuffer[2][(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 4, MPI_COMM_WORLD, &myrvRequest[(i*6)+3]);
    MPI_Irecv(&vrvbuffer[(i*size*3)], rcvpivot[x]*3, MPI_DOUBLE, x, 5, MPI_COMM_WORLD, &myrvRequest[(i*6)+4]);
    MPI_Irecv(&rcvpid_buffer[(i*size)], rcvpivot[x], MPI_INT, x, 6, MPI_COMM_WORLD, &myrvRequest[(i*6)+5]); 
  }

  for(int i=0; i<n_export_unique_procs; i++)
  {
    int x = export_unique_procs[i]; 
    //printf("RANK[%d]: send to rank %d\n", myrank, x);
    
    /*MPI_Isend(&tbuffer[0][(i*n*3)], pivot[x]*3, MPI_DOUBLE, x, 2, MPI_COMM_WORLD, &myRequest[(i*6)+1]);
    MPI_Isend(&tbuffer[1][(i*n*3)], pivot[x]*3, MPI_DOUBLE, x, 3, MPI_COMM_WORLD, &myRequest[(i*6)+2]);
    MPI_Isend(&tbuffer[2][(i*n*3)], pivot[x]*3, MPI_DOUBLE, x, 4, MPI_COMM_WORLD, &myRequest[(i*6)+3]);  
    MPI_Isend(&vbuffer[(i*n*3)], pivot[x]*3, MPI_DOUBLE, x, 5, MPI_COMM_WORLD, &myRequest[(i*6)+4]);
    MPI_Isend(&pid_buffer[(i*n)], pivot[x], MPI_INT, x, 6, MPI_COMM_WORLD, &myRequest[(i*6)+5]);
  */
    MPI_Isend(&tbuffer[0][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 2, MPI_COMM_WORLD, &myRequest[(i*6)+1]);
    MPI_Isend(&tbuffer[1][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 3, MPI_COMM_WORLD, &myRequest[(i*6)+2]);
    MPI_Isend(&tbuffer[2][idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 4, MPI_COMM_WORLD, &myRequest[(i*6)+3]);  
    MPI_Isend(&vbuffer[idx[i]*3], pivot[x]*3, MPI_DOUBLE, x, 5, MPI_COMM_WORLD, &myRequest[(i*6)+4]);
    MPI_Isend(&pid_buffer[idx[i]], pivot[x], MPI_INT, x, 6, MPI_COMM_WORLD, &myRequest[(i*6)+5]);
  }
 
  if(*nt > 0 && num_export > 0) 
  {
    receive_idx = *nt - num_export; //set to last id
  } else if(*nt >= 0 && num_export <= 0){
    receive_idx = *nt;
  }
 
  for(int i=0; i<n_import_unique_procs; i++)
  {
    MPI_Wait(&myrvRequest[(i*6)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*6)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*6)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*6)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myrvRequest[(i*6)+5], MPI_STATUS_IGNORE);
    //printf("RANK[%i]:received\n", myrank);

    int x = import_unique_procs[i];
    for(unsigned int j=0; j<rcvpivot[x]; j++)
    {
      pid[receive_idx] = rcvpid_buffer[(i*size)+j];
      for(int k=0;k<3;k++)
      {
        t[0][k][receive_idx] = trvbuffer[0][(i*size*3)+(j*3)+(k)];        
        t[1][k][receive_idx] = trvbuffer[1][(i*size*3)+(j*3)+(k)]; 
        t[2][k][receive_idx] = trvbuffer[2][(i*size*3)+(j*3)+(k)]; 
        
        v[k][receive_idx] = vrvbuffer[(i*size*3)+(j*3)+(k)];
      }
      receive_idx++;
    }
  }
  
  for(int i=0; i<n_export_unique_procs; i++)
  {
    MPI_Wait(&myRequest[(i*6)+1], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*6)+2], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*6)+3], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*6)+4], MPI_STATUS_IGNORE);
    MPI_Wait(&myRequest[(i*6)+5], MPI_STATUS_IGNORE);
  }

  *nt = *nt + (num_import-num_export);
 
  if(n_export_unique_procs)
  {
    free(tbuffer[0]);
    free(tbuffer[1]);
    free(tbuffer[2]);
    free(pid_buffer);
    free(vbuffer);
  }
  
   if(n_import_unique_procs)
   {
     free(trvbuffer[0]);
     free(trvbuffer[1]);
     free(trvbuffer[2]);
     free(rcvpid_buffer);   
     free(vrvbuffer);
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
