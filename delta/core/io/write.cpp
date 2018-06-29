/*
 * write.cpp
 *
 *  Created on: 2 Apr 2017
 *      Author: konstantinos
 */

#include <delta/core/io/write.h>

void delta::core::io::writeGeometryToVTK(int step, std::array<iREAL, 6> boundary, std::vector<delta::geometry::Object>& vectorGeometries)
{
  //domain boundary
  iREAL lo[3], hi[3];
  lo[0] = boundary[0]; // lower corner
  lo[1] = boundary[1]; // lower corner
  lo[2] = boundary[2]; // lower corner

  hi[0] = boundary[3]; // upper corner
  hi[1] = boundary[4]; // upper corner
  hi[2] = boundary[5]; // upper corner

  char iter[100];
  sprintf(iter, "%u.vtk", step);
  char filename[100] = "geometry_"; //care or buffer overflow
  strcat(filename, iter);

  FILE *fp = fopen(filename, "w+");
  if( fp == NULL )
  {
	perror("Error while opening the file.\n");
	return;
  }

  int numVertices = 0;
  int numberOfFaces = 0;
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	numVertices += vectorGeometries[i].getMesh().getUniqueVertices().size();
	numberOfFaces += vectorGeometries[i].getMesh().getTriangleFaces().size();
  }

  int numberOfBoundaries = 1;
  numVertices += 8*numberOfBoundaries;

  fprintf(fp,"# vtk DataFile Version 2.0\n"
			 "Output vtk file\n"
			 "ASCII\n\n"
			 "DATASET UNSTRUCTURED_GRID\n"
			 "POINTS %i double\n", numVertices);

  std::vector<int> meshEndPivots;
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	for(int j = 0; j < vectorGeometries[i].getMesh().getUniqueVertices().size(); j++)
	{
	  fprintf(fp,"%.5f %.5f %.5f\n",
		  vectorGeometries[i].getMesh().getUniqueVertices()[j][0],
		  vectorGeometries[i].getMesh().getUniqueVertices()[j][1],
		  vectorGeometries[i].getMesh().getUniqueVertices()[j][2]);
	}
	meshEndPivots.push_back(vectorGeometries[i].getMesh().getUniqueVertices().size());
  }

  //BOUNDARY
  fprintf(fp, "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n", 	lo[0], lo[1], lo[2], //0: A
									lo[0], hi[1], lo[2], //1: B
									lo[0], hi[1], hi[2], //2: E
									lo[0], lo[1], hi[2]);//3: F

  fprintf(fp, "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n", 	hi[0], hi[1], hi[2], //4: H
									hi[0], lo[1], hi[2], //5: G
									hi[0], lo[1], lo[2], //6: D
									hi[0], hi[1], lo[2]);//7: C

  int numberOfLines = numberOfBoundaries*12;

  int cellPointers 	= numberOfFaces*4 + numberOfLines*3;
  int cellNumber 	= numberOfFaces + numberOfLines;

  fprintf(fp,"\nCELLS %i %i\n", cellNumber, cellPointers);

  int pivot = 0; //position of start index
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	for(int j = 0; j < vectorGeometries[i].getMesh().getTriangleFaces().size(); j++)
	{
	  unsigned int A = pivot + vectorGeometries[i].getMesh().getTriangleFaces()[j][0];
	  unsigned int B = pivot + vectorGeometries[i].getMesh().getTriangleFaces()[j][1];
	  unsigned int C = pivot + vectorGeometries[i].getMesh().getTriangleFaces()[j][2];

	  fprintf(fp,"3 %i %i %i\n", A, B, C);
	}
	pivot = meshEndPivots[i];
  }

  int meshEndPivot = meshEndPivots[meshEndPivots.size()-1];

  //AB | 0->1
  //AD | 0->3
  //AG | 0->6

  //EC | 4->2
  //EH | 4->7
  //EF | 4->5

  //BC | 1->C
  //BH | 1->H
  //CD | 2->3
  //DF | 3->5
  //GH | 6->7
  //GF | 6->5

  int lA = meshEndPivot + 0;
  int lB = meshEndPivot + 1;
  int lE = meshEndPivot + 2;
  int lF = meshEndPivot + 3;
  int lH = meshEndPivot + 4;
  int lG = meshEndPivot + 5;
  int lD = meshEndPivot + 6;
  int lC = meshEndPivot + 7;

  fprintf(fp, "2 %i %i\n", lA, lB);
  fprintf(fp, "2 %i %i\n", lA, lF);
  fprintf(fp, "2 %i %i\n", lA, lD);

  fprintf(fp, "2 %i %i\n", lB, lE);
  fprintf(fp, "2 %i %i\n", lB, lC);

  fprintf(fp, "2 %i %i\n", lE, lF);
  fprintf(fp, "2 %i %i\n", lF, lG);

  fprintf(fp, "2 %i %i\n", lD, lC);
  fprintf(fp, "2 %i %i\n", lD, lG);

  fprintf(fp, "2 %i %i\n", lH, lE);
  fprintf(fp, "2 %i %i\n", lH, lC);
  fprintf(fp, "2 %i %i\n", lH, lG);


  fprintf(fp,"\nCELL_TYPES %i\n", cellNumber);

  //write triangle faces
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	for(int j = 0; j < vectorGeometries[i].getMesh().getTriangleFaces().size(); j++)
	{
	  fprintf(fp,"5\n"); //triangle
	}
  }

  //write line faces
  for(int j = 0; j < numberOfLines; j++)
  {
	fprintf(fp, "3\n"); //lines
  }

  fclose(fp);
}

void delta::core::io::writeGeometryToVTK(
	int step,
	std::array<iREAL, 6> boundary,
	std::vector<delta::core::data::ParticleRecord>& geometries)
{

  std::vector<delta::geometry::mesh::Mesh> vectorGeometries;

  for(int i=0; i<geometries.size(); i++)
  {
	delta::geometry::mesh::Mesh mesh(geometries[i]._xCoordinates, geometries[i]._yCoordinates, geometries[i]._zCoordinates);
	vectorGeometries.push_back(mesh);
  }

  //domain boundary
  iREAL lo[3], hi[3];
  lo[0] = boundary[0]; // lower corner
  lo[1] = boundary[1]; // lower corner
  lo[2] = boundary[2]; // lower corner

  hi[0] = boundary[3]; // upper corner
  hi[1] = boundary[4]; // upper corner
  hi[2] = boundary[5]; // upper corner

  char iter[100];
  sprintf(iter, "%u.vtk", step);
  char filename[100] = "geometry_"; //care or buffer overflow
  strcat(filename, iter);

  FILE *fp = fopen(filename, "w+");
  if( fp == NULL )
  {
	perror("Error while opening the file.\n");
	return;
  }

  int numVertices = 0;
  int numberOfFaces = 0;
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	numVertices += vectorGeometries[i].getUniqueVertices().size();
	numberOfFaces += vectorGeometries[i].getTriangleFaces().size();
  }
  printf("unique vertices %i\n", numVertices);
  printf("faces %i\n", numberOfFaces);

  int numberOfBoundaries = 1;
  numVertices += 8*numberOfBoundaries;

  fprintf(fp,"# vtk DataFile Version 2.0\n"
			 "Output vtk file\n"
			 "ASCII\n\n"
			 "DATASET UNSTRUCTURED_GRID\n"
			 "POINTS %i double\n", numVertices);

  std::vector<int> meshEndPivots;
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	for(int j = 0; j < vectorGeometries[i].getUniqueVertices().size(); j++)
	{
	  fprintf(fp,"%.5f %.5f %.5f\n",
		  vectorGeometries[i].getUniqueVertices()[j][0],
		  vectorGeometries[i].getUniqueVertices()[j][1],
		  vectorGeometries[i].getUniqueVertices()[j][2]);
	}
	meshEndPivots.push_back(vectorGeometries[i].getUniqueVertices().size());
  }

  //BOUNDARY
  fprintf(fp, "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n", 	lo[0], lo[1], lo[2], //0: A
									lo[0], hi[1], lo[2], //1: B
									lo[0], hi[1], hi[2], //2: E
									lo[0], lo[1], hi[2]);//3: F

  fprintf(fp, "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n", 	hi[0], hi[1], hi[2], //4: H
									hi[0], lo[1], hi[2], //5: G
									hi[0], lo[1], lo[2], //6: D
									hi[0], hi[1], lo[2]);//7: C

  int numberOfLines = numberOfBoundaries*12;

  int cellPointers 	= numberOfFaces*4 + numberOfLines*3;
  int cellNumber 	= numberOfFaces + numberOfLines;

  fprintf(fp,"\nCELLS %i %i\n", cellNumber, cellPointers);

  int pivot = 0; //position of start index
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	for(int j = 0; j < vectorGeometries[i].getTriangleFaces().size(); j++)
	{
	  unsigned int A = pivot + vectorGeometries[i].getTriangleFaces()[j][0];
	  unsigned int B = pivot + vectorGeometries[i].getTriangleFaces()[j][1];
	  unsigned int C = pivot + vectorGeometries[i].getTriangleFaces()[j][2];

	  fprintf(fp,"3 %i %i %i\n", A, B, C);
	}
	pivot = meshEndPivots[i];
  }

  int meshEndPivot = meshEndPivots[meshEndPivots.size()-1];

  //AB | 0->1
  //AD | 0->3
  //AG | 0->6

  //EC | 4->2
  //EH | 4->7
  //EF | 4->5

  //BC | 1->C
  //BH | 1->H
  //CD | 2->3
  //DF | 3->5
  //GH | 6->7
  //GF | 6->5

  int lA = meshEndPivot + 0;
  int lB = meshEndPivot + 1;
  int lE = meshEndPivot + 2;
  int lF = meshEndPivot + 3;
  int lH = meshEndPivot + 4;
  int lG = meshEndPivot + 5;
  int lD = meshEndPivot + 6;
  int lC = meshEndPivot + 7;

  fprintf(fp, "2 %i %i\n", lA, lB);
  fprintf(fp, "2 %i %i\n", lA, lF);
  fprintf(fp, "2 %i %i\n", lA, lD);

  fprintf(fp, "2 %i %i\n", lB, lE);
  fprintf(fp, "2 %i %i\n", lB, lC);

  fprintf(fp, "2 %i %i\n", lE, lF);
  fprintf(fp, "2 %i %i\n", lF, lG);

  fprintf(fp, "2 %i %i\n", lD, lC);
  fprintf(fp, "2 %i %i\n", lD, lG);

  fprintf(fp, "2 %i %i\n", lH, lE);
  fprintf(fp, "2 %i %i\n", lH, lC);
  fprintf(fp, "2 %i %i\n", lH, lG);


  fprintf(fp,"\nCELL_TYPES %i\n", cellNumber);

  //write triangle faces
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	for(int j = 0; j < vectorGeometries[i].getTriangleFaces().size(); j++)
	{
	  fprintf(fp,"5\n"); //triangle
	}
  }

  //write line faces
  for(int j = 0; j < numberOfLines; j++)
  {
	fprintf(fp, "3\n"); //lines
  }

  fclose(fp);
}

void delta::core::io::writeScenarioSpecification(std::string fileName)
{

}

