#include "graphite.h"

#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

void delta::primitives::generateBrick(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
  //////////VTK format////////////

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());

  char filename[100];
  strncpy(filename, "input/bricksmall.vtk", 100);
  FILE *fp1 = fopen(filename, "r+");

  if( fp1 == NULL )
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  char ch, word[100];
  double *point[3];

  do
  {
    ch = fscanf(fp1,"%s",word);
    if(strcmp(word, "POINTS")==0)
    {
      ch = fscanf(fp1,"%s",word);
      int n = atol(word);

      point[0] = new double[n];
      point[1] = new double[n];
      point[2] = new double[n];

      ch = fscanf(fp1,"%s",word);

      for(int i=0;i<n;i++)
      {
        fscanf(fp1, "%lf", &point[0][i]);
        fscanf(fp1, "%lf", &point[1][i]);
        fscanf(fp1, "%lf", &point[2][i]);
      }
    }

    if(strcmp(word, "CELLS")==0 || strcmp(word, "POLYGONS") == 0)
    {
      ch = fscanf(fp1,"%s",word);
      int numberOfTriangles = atol(word);
      ch = fscanf(fp1,"%s",word);

      xCoordinates.resize( numberOfTriangles*3 );
      yCoordinates.resize( numberOfTriangles*3 );
      zCoordinates.resize( numberOfTriangles*3 );

      for(int i=0;i<numberOfTriangles*3;i+=3)
      {
        ch = fscanf(fp1,"%s",word);
        ch = fscanf(fp1,"%s",word);

        int index = atol(word);
        xCoordinates[i] = ((point[0][index]));
        yCoordinates[i] = ((point[1][index]));
        zCoordinates[i] = ((point[2][index]));

        ch = fscanf(fp1,"%s",word);
        index = atol(word);
        xCoordinates[i+1] = ((point[0][index]));
        yCoordinates[i+1] = ((point[1][index]));
        zCoordinates[i+1] = ((point[2][index]));

        ch = fscanf(fp1,"%s",word);
        index = atol(word);
        xCoordinates[i+2] = ((point[0][index]));
        yCoordinates[i+2] = ((point[1][index]));
        zCoordinates[i+2] = ((point[2][index]));
      }
    }
  } while (ch != EOF);
  fclose(fp1);

  for(int i=0;i<xCoordinates.size();i+=3)
  {
    xCoordinates[i] = (xCoordinates[i]*h)+center[0];
    yCoordinates[i] = (yCoordinates[i]*h)+center[1];
    zCoordinates[i] = (zCoordinates[i]*h)+center[2];

    xCoordinates[i+1] = (xCoordinates[i+1]*h)+center[0];
    yCoordinates[i+1] = (yCoordinates[i+1]*h)+center[1];
    zCoordinates[i+1] = (zCoordinates[i+1]*h)+center[2];

    xCoordinates[i+2] = (xCoordinates[i+2]*h)+center[0];
    yCoordinates[i+2] = (yCoordinates[i+2]*h)+center[1];
    zCoordinates[i+2] = (zCoordinates[i+2]*h)+center[2];
  }
}

void delta::primitives::generateKey(
  double center[3], double h,
  std::vector<double>&  xCoordinates,
  std::vector<double>&  yCoordinates,
  std::vector<double>&  zCoordinates)
{
  //////////VTK format////////////

  assert(xCoordinates.empty());
  assert(yCoordinates.empty());
  assert(zCoordinates.empty());

  char filename[100];
  strncpy(filename, "input/keysmall.vtk", 100);
  FILE *fp1 = fopen(filename, "r+");

  if( fp1 == NULL )
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  char ch, word[100];
  double *point[3];

  do
  {
    ch = fscanf(fp1,"%s",word);
    if(strcmp(word, "POINTS")==0)
    {
      ch = fscanf(fp1,"%s",word);
      int n = atol(word);

      point[0] = new double[n];
      point[1] = new double[n];
      point[2] = new double[n];

      ch = fscanf(fp1,"%s",word);

      for(int i=0;i<n;i++)
      {
        fscanf(fp1, "%lf", &point[0][i]);
        fscanf(fp1, "%lf", &point[1][i]);
        fscanf(fp1, "%lf", &point[2][i]);
      }
    }

    if(strcmp(word, "CELLS")==0 || strcmp(word, "POLYGONS") == 0)
    {
      ch = fscanf(fp1,"%s",word);
      int numberOfTriangles = atol(word);
      ch = fscanf(fp1,"%s",word);

      xCoordinates.resize( numberOfTriangles*3 );
      yCoordinates.resize( numberOfTriangles*3 );
      zCoordinates.resize( numberOfTriangles*3 );

      for(int i=0;i<numberOfTriangles*3;i+=3)
      {
        ch = fscanf(fp1,"%s",word);
        ch = fscanf(fp1,"%s",word);

        int index = atol(word);
        xCoordinates[i] = ((point[0][index]));
        yCoordinates[i] = ((point[1][index]));
        zCoordinates[i] = ((point[2][index]));

        ch = fscanf(fp1,"%s",word);
        index = atol(word);
        xCoordinates[i+1] = ((point[0][index]));
        yCoordinates[i+1] = ((point[1][index]));
        zCoordinates[i+1] = ((point[2][index]));

        ch = fscanf(fp1,"%s",word);
        index = atol(word);
        xCoordinates[i+2] = ((point[0][index]));
        yCoordinates[i+2] = ((point[1][index]));
        zCoordinates[i+2] = ((point[2][index]));
      }
    }
  } while (ch != EOF);
  fclose(fp1);

  for(int i=0;i<xCoordinates.size();i+=3)
  {
    xCoordinates[i] = (xCoordinates[i]*h)+center[0];
    yCoordinates[i] = (yCoordinates[i]*h)+center[1];
    zCoordinates[i] = (zCoordinates[i]*h)+center[2];

    xCoordinates[i+1] = (xCoordinates[i+1]*h)+center[0];
    yCoordinates[i+1] = (yCoordinates[i+1]*h)+center[1];
    zCoordinates[i+1] = (zCoordinates[i+1]*h)+center[2];

    xCoordinates[i+2] = (xCoordinates[i+2]*h)+center[0];
    yCoordinates[i+2] = (yCoordinates[i+2]*h)+center[1];
    zCoordinates[i+2] = (zCoordinates[i+2]*h)+center[2];
  }
}
