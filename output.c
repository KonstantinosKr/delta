#include "output.h"

void output_state(struct loba *lb, int myrank, unsigned int nt, iREAL *t[3][3], iREAL *v[3], unsigned int timesteps)
{
    iREAL lo[3], hi[3]; 
    loba_getbox (lb, myrank, lo, hi);//get local subdomain boundary box
    
    char iter[100];
    sprintf(iter, "%u_%i.vtk", timesteps, myrank);
    char filename[100] = "output/mpi/output"; //care or buffer overflow
    strcat(filename, iter);
    printf("%s\n", filename);
      
    FILE *fp = fopen(filename, "w+");
    if( fp == NULL )
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp,"# vtk DataFile Version 2.0\nOutput vtk file\nASCII\n\nDATASET UNSTRUCTURED_GRID\nPOINTS %i float\n", (nt*3)+8);
      
    unsigned int i;
    for(i = 0; i < nt; i++)
    {
      fprintf(fp,"%.5f %.5f %.5f\n%.5f %.5f %.5f\n%.5f %.5f %.5f\n", t[0][0][i], t[0][1][i], t[0][2][i], t[1][0][i], t[1][1][i], t[1][2][i], t[2][0][i], t[2][1][i], t[2][2][i]);
    }
 
    fprintf(fp, "%.5f %.5f %.5f\n%.5f %.5f %.5f\n%.5f %.5f %.5f\n%.5f %.5f %.5f\n", lo[0], lo[1], lo[2], lo[0], hi[1], lo[2], lo[0], hi[1], hi[2], lo[0], lo[1], hi[2]); 

    fprintf(fp, "%.5f %.5f %.5f\n%.5f %.5f %.5f\n%.5f %.5f %.5f\n%.5f %.5f %.5f\n", hi[0], hi[1], hi[2], hi[0], lo[1], hi[2], hi[0], lo[1], lo[2], hi[0], hi[1], lo[2]); 
    
    fprintf(fp,"\nCELLS %i %i\n", nt+12, nt+nt*3+(12*3));
    for(i = 0; i < nt*3; i=i+3)
    {
        fprintf(fp,"3 %i %i %i\n", i, i+1, i+2);
    }
    
    //left
    //1. lo,lo,lo
    //2. lo,hi,lo
    //3. lo,hi,hi
    //4. lo,lo,hi
    
    //link:
    //1. -> 2.
    //2. -> 3.
    //3. -> 4.
  
    //right
    //5. hi,hi,hi
    //6. hi,lo,hi
    //7. hi,lo,lo
    //8. hi,hi,lo
    
    //link:
    //5. -> 6.
    //6. -> 7.
    //7. -> 8.
   
    //interlink:
    //1. -> 6.
    //2. -> 5.
    //3. -> 7.
    //4. -> 8.
    //
    unsigned int ii = i;
    for(unsigned int j = 0; j < 3; j++)
    {
      fprintf(fp, "2 %i %i\n", i, i+1);
      fprintf(fp, "2 %i %i\n", i+4, i+4+1);
      i = i+1;
    }
    fprintf(fp, "2 %i %i\n", i, ii);
    fprintf(fp, "2 %i %i\n", i+4, ii+4);
    
    fprintf(fp, "2 %i %i\n", ii, ii+6);
    fprintf(fp, "2 %i %i\n", ii+1, ii+7);
    fprintf(fp, "2 %i %i\n", ii+2, ii+4);
    fprintf(fp, "2 %i %i\n", ii+3, ii+5);
    
    fprintf(fp,"\nCELL_TYPES %i\n", nt+12);
    for(i = 0; i<nt; i++)
    {
        fprintf(fp,"5\n");
    }
    for(int j = 0; j < 12; j++)
    {
      fprintf(fp, "3\n");
    }

    fclose(fp);
}

void postProcessing(int nranks, unsigned int size, unsigned int timesteps)
{
    double *point[3];
    int *cells[5];
    
    for(int i=0; i<3; i++)
    {
        point[i] = (double*) malloc(sizeof(double[size]));
    }
    
    for(int i=0; i<5; i++)
    {
        cells[i] = (int*) malloc(sizeof(int[size]));
    }
    
    for(int ii=0; ii<timesteps; ii++)
    {
        unsigned int nt = 0;
        unsigned int n = 0;
        unsigned int cellnt = 0;
        unsigned int celldx = 0;
        
        //readin vtks from every rank
        for(int j=0; j<nranks; j++)
        {
            char ch, word[100];
            char filename[100] = "output/mpi/output";
            char str[100];
            sprintf(str, "%i_%i.vtk", ii, j);
            strcat(filename, str);
            
            FILE *fp = fopen(filename, "r");
            if( fp == NULL )
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            
            do  {
                ch = fscanf(fp,"%s",word);
                if(strcmp(word, "POINTS")==0)
                {
                    ch = fscanf(fp,"%s",word);//n points
                    n = atoi(word);//n points saved
                    ch = fscanf(fp,"%s",word);//float or double read
                    
                    //loop through points
                    for(unsigned int i=nt;i<nt+n;i++)
                    {
                        ch = fscanf(fp, "%s", word);
                        point[0][i] = atof(word);
                        ch = fscanf(fp, "%s", word);
                        point[1][i] = atof(word);
                        ch = fscanf(fp, "%s", word);
                        point[2][i] = atof(word);
                        //printf("POINT[0] = %f | POINT[1] = %f | POINT[2] = %f\n", point[0][i], point[1][i], point[2][i]);
                    }
                }
                
                if(strcmp(word, "CELLS")==0)
                {
                    ch = fscanf(fp,"%s",word);
                    unsigned int celln = atoi(word);
                    ch = fscanf(fp,"%s",word);
                    celldx = atoi(word) + celldx;
                    for(unsigned int i=cellnt;i<cellnt+celln;i++)
                    {
                        ch = fscanf(fp,"%s",word);
                        if(strcmp(word, "3")==0)//triangle
                        {
                            cells[0][i] = 3;
                            ch = fscanf(fp, "%s", word);
                            cells[1][i] = atof(word) + nt;
                            ch = fscanf(fp, "%s", word);
                            cells[2][i] = atof(word) + nt;
                            ch = fscanf(fp, "%s", word);
                            cells[3][i] = atof(word) + nt;
                        }
                        else if(strcmp(word, "2")==0)//line
                        {
                            cells[0][i] = 2;
                            ch = fscanf(fp, "%s", word);
                            cells[1][i] = atof(word) + nt;
                            ch = fscanf(fp, "%s", word);
                            cells[2][i] = atof(word) + nt;
                        }
                    }
                    cellnt = cellnt + celln;
                    nt = nt + n;//set nt
                }
            } while (ch != EOF);
            fclose(fp);
        }
        
        // WRITE OUTPUT
        //
        char iter[50];
        sprintf(iter, "%i.vtk", ii);
        char filename[50] = "output/mergedmpi/output"; //care or buffer overflow
        strcat(filename, iter);
        printf("%s\n", filename);
        
        FILE *out = fopen(filename, "w+");
        
        fprintf(out,"# vtk DataFile Version 2.0\nOutput vtk file\nASCII\n\nDATASET UNSTRUCTURED_GRID\nPOINTS %i float\n", nt);
        
        for(unsigned int i = 0; i < nt; i++)
        {
            fprintf(out,"%.5f %.5f %.5f\n", point[0][i], point[1][i], point[2][i]);
        }
        
        fprintf(out,"\nCELLS %i %i\n", cellnt, celldx);
        for(unsigned int i = 0; i < cellnt; i++)
        {
            if(cells[0][i] == 3)
            {
                fprintf(out,"%i %i %i %i\n", cells[0][i], cells[1][i], cells[2][i], cells[3][i]);
            }
            else if(cells[0][i] == 2)
            {
                fprintf(out,"%i %i %i\n", cells[0][i], cells[1][i], cells[2][i]);
            }
        }
        
        fprintf(out, "\nCELL_TYPES %i\n", cellnt);
        for(unsigned int i = 0; i < cellnt; i++)
        {
            if(cells[0][i] == 3)
            {
                fprintf(out, "%i\n", 5);
            }
            else if (cells[0][i] == 2)
            {
                fprintf(out, "%i\n", 3);
            }
        }
        fclose(out);
    }
    
    free(point[0]);
    free(point[1]);
    free(point[2]);
    
    free(cells[0]);
    free(cells[1]);
    free(cells[2]);
    free(cells[3]);
    free(cells[4]);
}
