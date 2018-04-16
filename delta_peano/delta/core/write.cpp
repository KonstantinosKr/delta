/*
 * write.cpp
 *
 *  Created on: 2 Apr 2017
 *      Author: konstantinos
 */

#include <delta/core/write.h>

write::write() {
  // TODO Auto-generated constructor stub
}

void write::writeGeometryToVTK(int step, int numVertices, std::vector<delta::geometry::Object> vectorGeometries, iREAL *t[6][3])
{
  //domain boundary
  iREAL lo[3], hi[3];
  lo[0] = 0.0; // lower corner
  lo[1] = 0.0; // lower corner
  lo[2] = 0.0; // lower corner

  hi[0] = 1.0; // upper corner
  hi[1] = 1.0; // upper corner
  hi[2] = 1.0; // upper corner

  char iter[100];
  sprintf(iter, "%u.vtk", step);
  char filename[100] = "output/geometry_"; //care or buffer overflow
  strcat(filename, iter);

  FILE *fp = fopen(filename, "w+");
  if( fp == NULL )
  {
	perror("Error while opening the file.\n");
	return;
  }

  int numVerticesTriangles = 0;
  int numberOftriangles = 0;
  for(int i=0; i<vectorGeometries.size(); i++)
  {
	numVerticesTriangles += vectorGeometries[i].getNumberOfTriangles()/3.0;
	numberOftriangles += vectorGeometries[i].getNumberOfTriangles();
  }

  int totalPoints = numVerticesTriangles + 8;
  int numberOfLines = 12;

  fprintf(fp,"# vtk DataFile Version 2.0\n"
			 "Output vtk file\n"
			 "ASCII\n\n"
			 "DATASET UNSTRUCTURED_GRID\n"
			 "POINTS %i double\n", totalPoints);

  int i;
  for(i = 0; i < numVertices; i++)
  {
	fprintf(fp,"%.5f %.5f %.5f\n"
			   "%.5f %.5f %.5f\n"
			   "%.5f %.5f %.5f\n", t[0][0][i], t[0][1][i], t[0][2][i],
								   t[1][0][i], t[1][1][i], t[1][2][i],
								   t[2][0][i], t[2][1][i], t[2][2][i]);
  }

  //BOUNDARY
  fprintf(fp, "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n", lo[0], lo[1], lo[2], lo[0], hi[1], lo[2], lo[0], hi[1], hi[2], lo[0], lo[1], hi[2]);

  fprintf(fp, "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n"
			  "%.5f %.5f %.5f\n", hi[0], hi[1], hi[2], hi[0], lo[1], hi[2], hi[0], lo[1], lo[2], hi[0], hi[1], lo[2]);
  //BOUNDARY

  fprintf(fp,"\nCELLS %i %i\n", numVerticesTriangles+numberOfLines, numVerticesTriangles+numVerticesTriangles*3+(numberOfLines*3));

  for(i = 0; i < numVerticesTriangles*3; i=i+3)
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
  int ii = i;
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


  //number of triangles + number of lines
  fprintf(fp,"\nCELL_TYPES %i\n", numVertices+numberOfLines);

  for(i = 0; i<numVertices; i++)
  {
	fprintf(fp,"5\n");
  }
  for(int j = 0; j < 12; j++)
  {
	fprintf(fp, "3\n");
  }

  fclose(fp);
}

void write::writeGeometry()
{

}

write::~write() {
  // TODO Auto-generated destructor stub
}

