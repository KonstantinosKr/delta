#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <iostream>
#define nranks 32
#define timesteps 100
#define size 1000000

int main()
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
      char filename[100] = "mpi/output";
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
    char iter[15];
    sprintf(iter, "%i.vtk", ii);
    char filename[50] = "mergedmpi/output"; //care or buffer overflow
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

  return 0;
}
