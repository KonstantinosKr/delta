/*
 * read.cpp
 *
 *  Created on: 8 Feb 2017
 *      Author: konstantinos
 */

#include <delta/core/read.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/Exporter.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

std::vector<std::string> splitString(
  std::string input,
  std::string delimiter)
{
 std::vector<std::string> output;
 char *pch;

 char *str = strdup(input.c_str());

 pch = strtok (str, delimiter.c_str());

 while (pch != NULL)
 {
	output.push_back(pch);

	pch = strtok (NULL,  delimiter.c_str());
 }

 free(str);

 return output;
}

void delta::core::parseModelGridSchematics(
	std::string fileName,
	std::vector<std::vector<std::string>> &componentGrid,
	std::vector<std::string> &componentSeq)
{
  std::string line;
  //46 * 46
  std::ifstream myfile;
  myfile.open(fileName);

  if (myfile.is_open())
  {
	while (std::getline (myfile, line))
	{
	  std::vector<std::string> vstring = splitString(line, ",");
	  //std::cout << vstring[2] << "\n";

	  componentSeq.push_back(vstring[2]);

	  if(std::stoi(vstring[0]) == 46)
	  {
		  //std::cout << std::stoi(vstring[0]) << "\n";
		  componentGrid.push_back(componentSeq);
	  }
	}
	myfile.close();
  }
  else std::cout << "Unable to open file";
}

void delta::core::readSingleVTKGeometry(
	char* fileName,
	std::vector<iREAL>&  xCoordinates,
	std::vector<iREAL>&  yCoordinates,
	std::vector<iREAL>&  zCoordinates)
{
  //////////VTK format////////////

  char filename[100];
  strncpy(filename, fileName, 100);
  FILE *fp1 = fopen(filename, "r+");

  if( fp1 == NULL )
  {
	  perror("Error while opening the file.\n");
	  exit(EXIT_FAILURE);
  }

  char ch, word[100];
  iREAL *point[3];

  do
  {
	  ch = fscanf(fp1,"%s",word);
	  if(strcmp(word, "POINTS")==0)
	  {
		ch = fscanf(fp1,"%s",word);
		int n = atol(word);

		point[0] = new iREAL[n];
		point[1] = new iREAL[n];
		point[2] = new iREAL[n];

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
}

std::vector<delta::geometry::mesh::Mesh> delta::core::readSceneGeometry(std::string fileName)
{
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile( fileName,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

  bool n = scene->HasMeshes();
  int nn = scene->mNumMeshes;

  //printf("has %i meshes:%i\n", nn, n);

  std::vector<delta::geometry::mesh::Mesh> meshVector;

  for(uint m_i = 0; m_i < scene->mNumMeshes; m_i++)
  {
	std::vector<std::array<int, 3>> 		triangleFaces;
	std::vector<std::array<iREAL, 3>> 	uniqueVertices;

	const aiMesh* mesh = scene->mMeshes[m_i];

	std::vector<iREAL> g_vp;
	g_vp.reserve(3 * mesh->mNumVertices);

	//printf("read %i vertices\n", mesh->mNumVertices);
	//vertices
	for(uint v_i = 0; v_i < mesh->mNumVertices; v_i++)
	{
	  if(mesh->HasPositions())
	  {
		const aiVector3D* vp = &(mesh->mVertices[v_i]);
		g_vp.push_back(vp->x);
		g_vp.push_back(vp->y);
		g_vp.push_back(vp->z);

		std::array<iREAL, 3> vertex = {vp->x, vp->y, vp->z};
		uniqueVertices.push_back(vertex);

		//std::cout << vp->x << " " << vp->y << " " << vp->z << std::endl;
	  }
	}

	//printf("number of triangles: %i\n", mesh->mNumFaces);

	//only triangle faces
	for(uint f_i = 0; f_i < mesh->mNumFaces; f_i++)
	{
	  //only triangle faces
	  for(uint index = 0; index < mesh->mFaces[f_i].mNumIndices; index+=3)
	  {
		int idxA = mesh->mFaces[f_i].mIndices[index];
		int idxB = mesh->mFaces[f_i].mIndices[index+1];
		int idxC = mesh->mFaces[f_i].mIndices[index+2];
		std::array<int, 3> triangle = {idxA, idxB, idxC};

		/*
		std::cout << uniqueVertices[idxA][0] << " " << uniqueVertices[idxA][1] << " " << uniqueVertices[idxA][2] << std::endl;
		std::cout << uniqueVertices[idxB][0] << " " << uniqueVertices[idxB][1] << " " << uniqueVertices[idxB][2] << std::endl;
		std::cout << uniqueVertices[idxC][0] << " " << uniqueVertices[idxC][1] << " " << uniqueVertices[idxC][2] << std::endl;*/

		triangleFaces.push_back(triangle);
	  }
	}

	delta::geometry::mesh::Mesh *meshgeometry = new delta::geometry::mesh::Mesh(triangleFaces, uniqueVertices);
	meshVector.push_back(*meshgeometry);
  }

  return meshVector;
}
