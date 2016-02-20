#include "output.h"

void output_state(int nt, iREAL *t[6][3], int timesteps)
{
  iREAL lo[3], hi[3];	
  lo[0] = -250; // lower corner
  lo[1] = -250; // lower corner
  lo[2] = -250; // lower corner
  
  hi[0] = 250; // upper corner
  hi[1] = 250; // upper corner
  hi[2] = 250; // upper corner
	char iter[100];
	sprintf(iter, "%u.vtk", timesteps);
	char filename[100] = "output/output"; //care or buffer overflow
	strcat(filename, iter);
	printf("%s\n", filename);
		
	FILE *fp = fopen(filename, "w+");
	if( fp == NULL )
	{
			perror("Error while opening the file.\n");
			exit(EXIT_FAILURE);
	}
	
	fprintf(fp,"# vtk DataFile Version 2.0\nOutput vtk file\nASCII\n\nDATASET UNSTRUCTURED_GRID\nPOINTS %i float\n", (nt*3)+8);
		
	int i;
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

