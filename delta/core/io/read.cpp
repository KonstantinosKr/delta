 /*
 *  Created on: 8 Feb 2017
 *      Author: konstantinos
 */

#include "read.h"
#include <sstream>
#include <stdexcept>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/Exporter.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "../../geometry/structure/Mesh.h"

std::vector<std::string> splitString(
  std::string input,
  std::string delimiter)
{
	std::vector<std::string> output;
	char *str = strdup(input.c_str());
	char *pch = strtok (str, delimiter.c_str());

	while (pch != NULL)
	{
            output.push_back(pch);
            pch = strtok (NULL,  delimiter.c_str());
	}

	free(str);

	return output;
}

void delta::core::io::parseModelGridSchematics(
    std::string fileName,
    std::vector<std::vector<std::string>> &componentGrid,
    std::vector<std::string> &componentSeq)
{
  std::string line;
  //46 * 46
  std::ifstream myfile;
  myfile.open(fileName.c_str());

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

delta::geometry::mesh::Mesh *delta::core::io::readVTKGeometry(
	char* fileName)
{
  std::vector<iREAL> xCoordinates;
  std::vector<iREAL> yCoordinates;
  std::vector<iREAL> zCoordinates;

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

  return new delta::geometry::mesh::Mesh(xCoordinates, yCoordinates, zCoordinates);
}

void delta::core::io::readScenarioSpecification(std::string fileName)
{

}

std::vector<delta::geometry::mesh::Mesh> delta::core::io::readGeometry(std::string fileName)
{
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile( fileName,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

  if(scene == nullptr || scene->mNumMeshes == 0)
  {
    throw std::runtime_error("readGeometry: cannot load '" + fileName + "': " + importer.GetErrorString());
  }

  printf("Importing %i Meshes.\n", scene->mNumMeshes);

  std::vector<delta::geometry::mesh::Mesh> meshVector;

  for(uint m_i = 0; m_i < scene->mNumMeshes; m_i++)
  {
    std::vector<std::array<int, 3>> 		triangleFaces;
    std::vector<std::array<iREAL, 3>> 	uniqueVertices;

    const aiMesh* mesh = scene->mMeshes[m_i];

    std::vector<iREAL> g_vp;
    g_vp.reserve(3 * mesh->mNumVertices);

    //printf("Read %i vertices\n", mesh->mNumVertices);

    //vertices
    #pragma omp parallel for
    for(uint v_i = 0; v_i < mesh->mNumVertices; v_i++)
    {
      if(mesh->HasPositions())
      {
        const aiVector3D* vp = &(mesh->mVertices[v_i]);
        g_vp.push_back(vp->x);
        g_vp.push_back(vp->y);
        g_vp.push_back(vp->z);

        std::array<iREAL, 3> vertex = {vp->x, vp->y, vp->z};

        #pragma omp critical
        uniqueVertices.push_back(vertex);
        //std::cout << vp->x << " " << vp->y << " " << vp->z << std::endl;
      }
    }

    //printf("number of triangles: %i\n", mesh->mNumFaces);

    //only triangle faces
    //#pragma omp parallel for
    for(uint f_i = 0; f_i < mesh->mNumFaces; f_i++)
    {
      //only triangle faces
      //#pragma omp parallel for
      for(uint index = 0; index < mesh->mFaces[f_i].mNumIndices; index+=3)
      {
        int idxA = mesh->mFaces[f_i].mIndices[index];
        int idxB = mesh->mFaces[f_i].mIndices[index+1];
        int idxC = mesh->mFaces[f_i].mIndices[index+2];
        std::array<int, 3> triangle = {idxA, idxB, idxC};

        /*std::cout << uniqueVertices[idxA][0] << " " << uniqueVertices[idxA][1] << " " << uniqueVertices[idxA][2] << std::endl;
        std::cout << uniqueVertices[idxB][0] << " " << uniqueVertices[idxB][1] << " " << uniqueVertices[idxB][2] << std::endl;
        std::cout << uniqueVertices[idxC][0] << " " << uniqueVertices[idxC][1] << " " << uniqueVertices[idxC][2] << std::endl;*/

        //#pragma omp critical
        triangleFaces.push_back(triangle);
      }
    }

    //printf("Faces Size: %i\n", triangleFaces.size());
    for(int i=0; i<triangleFaces.size(); i++)
    {
      std::cout << triangleFaces[i][0] << " " << triangleFaces[i][1] << " " << triangleFaces[i][2] << std::endl;
    }
    delta::geometry::mesh::Mesh *meshgeometry = new delta::geometry::mesh::Mesh(triangleFaces, uniqueVertices);
    meshVector.push_back(*meshgeometry);
  }

  return meshVector;
}

delta::geometry::mesh::Mesh* delta::core::io::readPartGeometry(std::string fileName)
{
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile( fileName,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

  //A missing or unreadable file leaves `scene` null with the reason in the
  //importer's error string; dereferencing it is an instant segfault, so fail
  //loudly with the path instead.
  if(scene == nullptr || scene->mNumMeshes == 0)
  {
    throw std::runtime_error("readPartGeometry: cannot load '" + fileName + "': " + importer.GetErrorString());
  }

  std::vector<std::array<int, 3>> 	triangleFaces;
  std::vector<std::array<iREAL, 3>> 	uniqueVertices;

  const aiMesh* mesh = scene->mMeshes[0];

  std::vector<iREAL> g_vp;
  g_vp.reserve(3 * mesh->mNumVertices);

  //printf("Read %i vertices\n", mesh->mNumVertices);

  //vertices
  //#pragma omp parallel for
  for(uint v_i = 0; v_i < mesh->mNumVertices; v_i++)
  {
    if(mesh->HasPositions())
    {
      const aiVector3D* vp = &(mesh->mVertices[v_i]);
      g_vp.push_back(vp->x);
      g_vp.push_back(vp->y);
      g_vp.push_back(vp->z);

      std::array<iREAL, 3> vertex = {vp->x, vp->y, vp->z};

      //#pragma omp critical
      uniqueVertices.push_back(vertex);
      //std::cout << vp->x << " " << vp->y << " " << vp->z << std::endl;
    }
  }

  //printf("number of triangles: %i\n", mesh->mNumFaces);

  //only triangle faces
  //#pragma omp parallel for
  for(uint f_i = 0; f_i < mesh->mNumFaces; f_i++)
  {
    //only triangle faces
    //#pragma omp parallel for
    for(uint index = 0; index < mesh->mFaces[f_i].mNumIndices; index+=3)
    {
        int idxA = mesh->mFaces[f_i].mIndices[index];
        int idxB = mesh->mFaces[f_i].mIndices[index+1];
        int idxC = mesh->mFaces[f_i].mIndices[index+2];
        std::array<int, 3> triangle = {idxA, idxB, idxC};

        //#pragma omp critical
        triangleFaces.push_back(triangle);
    }
  }
  return new delta::geometry::mesh::Mesh(triangleFaces, uniqueVertices);
}

void delta::core::io::readVTKLegacy() {

    // Get all data from the file
    vtkSmartPointer<vtkUnstructuredGridReader> reader =
      vtkSmartPointer<vtkUnstructuredGridReader>::New();
    reader->SetFileName("../output/grid_0.vtk");
    reader->Update();

    // All of the standard data types can be checked and obtained like this:
    if(reader->IsFileUnstructuredGrid())
    {
        std::cout << "output is a unstructured grid" << std::endl;
        vtkUnstructuredGrid* output = reader->GetOutput();
        std::cout << "output has " << output->GetNumberOfPoints() << " points." << std::endl;
    }
}

/*
 *
 * 
SURFACE_MATERIALS:	1
SURF1:	ANY
SURF2:	ANY
MODEL:	SPRING_DASHPOT

FRICTION:	0
COHESION:	0
SPRING:	1e+06
DASHPOT:	-1

BULK_MATERIALS:	1
LABEL:	BULK_MATERIAL_0
MODEL:	KIRCHHOFF
YOUNG:	1e+09
POISSON:	0.25
DENSITY:	1000

GRAVITY:
CONSTANT:	0
CONSTANT:	0
CONSTANT:	-10
*/

namespace {

//The file pads numbers with tabs and variable-width spaces
//("CENTER:\t0.02  0.06  0.06"), so a value can only be read by skipping the key
//and letting the stream tokenize. Positional substr() misreads any other
//spacing silently.
std::string valueAfterColon(const std::string& line)
{
  std::string::size_type colon = line.find(':');
  return colon == std::string::npos ? std::string() : line.substr(colon + 1);
}

std::vector<iREAL> numbersAfterColon(const std::string& line, int count)
{
  std::istringstream stream(valueAfterColon(line));
  std::vector<iREAL> values(count);
  for(int i=0; i<count; i++)
  {
    if(!(stream >> values[i]))
    {
      throw std::logic_error("readmbfcp: malformed line '" + line + "'");
    }
  }
  return values;
}

iREAL numberAfterColon(const std::string& line)
{
  return numbersAfterColon(line, 1)[0];
}

bool startsWith(const std::string& line, const std::string& prefix)
{
  return line.rfind(prefix, 0) == 0;
}

/*
 * Every body paragraph ends with VELOCITY (linear xyz then angular xyz) and
 * FORCES. Both were read past and dropped, so every particle started at rest
 * no matter what the file said.
 */
std::array<std::array<iREAL, 3>, 2> readBodyInitialState(std::ifstream& file, int id)
{
  std::string line;

  getline(file, line);
  std::vector<iREAL> velocity = numbersAfterColon(line, 6);

  getline(file, line);
  int forces = static_cast<int>(numberAfterColon(line));
  //Every other single-integer key here is a count (BODIES, SPHERES, CONVEXES,
  //FACES...), so FORCES is read as one too. Its record grammar is not
  //documented anywhere in the tree, so refuse rather than guess at it.
  if(forces > 0)
  {
    throw std::logic_error("readmbfcp: body " + std::to_string(id) + " declares " +
                           std::to_string(forces) +
                           " FORCES records, whose grammar is not implemented");
  }

  return {{ {velocity[0], velocity[1], velocity[2]},
            {velocity[3], velocity[4], velocity[5]} }};
}

} // namespace

delta::core::io::Scenario delta::core::io::readmbfcp(
    std::string                                     filename,
    std::vector<delta::world::structure::Object>&   objects,
    iREAL                                           epsilon)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("readmbfcp: cannot open '" + filename + "'");
    }
    std::cout << "Opened: " << filename << std::endl;

    Scenario scenario;

    //BULK_MATERIALS/GRAVITY are parsed before the BODIES paragraphs, so both
    //values have to outlive the material block: density turns a sphere radius
    //into a mass, the rest is reported back to the caller.
    iREAL density = 0.0;
    int surfaceMaterials = 0;

    //read line by line
    for (std::string line; getline(file, line);)
    {
    	//skip empty lines, without echoing one per line: the hopper file has one
    	//between every body.
        if (line.empty())
        {
            continue;
        }

    	//read meta data - materials, etc
        ////////////////////////////////////////////////////
        /*
        SURFACE_MATERIALS:	1
        SURF1:	ANY
        SURF2:	ANY
        MODEL:	SPRING_DASHPOT
        FRICTION:	0
        COHESION:	0
        SPRING:	1e+06
        DASHPOT:	-1

        BULK_MATERIALS:	1
        LABEL:	BULK_MATERIAL_0
        MODEL:	KIRCHHOFF
        YOUNG:	1e+09
        POISSON:	0.25
        DENSITY:	1000

        GRAVITY:
        CONSTANT:	0
        CONSTANT:	0
        CONSTANT:	-10
        */
        if (line.find("SURFACE_MATERIALS:") != std::string::npos) {
            surfaceMaterials = static_cast<int>(numberAfterColon(line));
            getline(file, line); //SURF1
            getline(file, line); //SURF2
            getline(file, line); //MODEL
            getline(file, line);
            scenario.surface.friction = numberAfterColon(line);
            getline(file, line);
            scenario.surface.cohesion = numberAfterColon(line);
            getline(file, line);
            scenario.surface.spring = numberAfterColon(line);
            getline(file, line);
            scenario.surface.dashpot = numberAfterColon(line);
            //////////////////////////////////////////

            getline(file, line); //blank
            getline(file, line); //BULK_MATERIALS:
            getline(file, line);
            scenario.bulk.label = valueAfterColon(line);
            getline(file, line);
            scenario.bulk.model = valueAfterColon(line);
            getline(file, line);
            scenario.bulk.young = numberAfterColon(line);
            getline(file, line);
            scenario.bulk.poisson = numberAfterColon(line);
            getline(file, line);
            density = numberAfterColon(line);
            scenario.bulk.density = density;

            getline(file, line); //blank
            getline(file, line); //GRAVITY:
            getline(file, line);
            scenario.gravity[0] = numberAfterColon(line);
            getline(file, line);
            scenario.gravity[1] = numberAfterColon(line);
            getline(file, line);
            scenario.gravity[2] = numberAfterColon(line);
            //////////////////////////////////////
        }

        //extract number of bodies
        if (line.find("BODIES:") != std::string::npos) {
            scenario.bodyCount = static_cast<int>(numberAfterColon(line));
            std::cout << "# bodies: " << scenario.bodyCount << std::endl;
        }

        /*
        ID:	126
        LABEL:	(null)
        KINEMATICS:	OBSTACLE
        BULK_MATERIAL:	BULK_MATERIAL_0
        SHAPES:	1

        CONVEXES:	1
        VERTEXES:	6
        1  -0.1  0
        0  -0.1  1
        0  -0.1  0
        0  0  1
        0  0  0
        1  0  0
        FACES:	8
        3  0  3  6  2
        3  9  3  0  2
        3  6  3  9  2
        3  12  9  15  2
        3  6  9  12  2
        3  6  12  15  2
        3  15  0  6  2
        3  9  0  15  2
        VELOCITY:  0  0  0  0  0  0
        FORCES:	0

        ID:	1
        LABEL:	(null)
        KINEMATICS:	RIGID
        BULK_MATERIAL:	BULK_MATERIAL_0
        SHAPES:	1

        SPHERES:	0
        VELOCITY:  0  0  0  0  0  0
        FORCES:	0
        */

        //extract paragraph (group of lines split by \n\n

        //startsWith, not find: a body starts at "ID:" and "SURFID:" (which
        //contains "ID:") is not one.
        if (startsWith(line, "ID:")) {

            int id = static_cast<int>(numberAfterColon(line));

            getline(file, line); //LABEL
            getline(file, line); //KINEMATICS
            getline(file, line); //BULK_MATERIAL
            getline(file, line); //SHAPES
            getline(file, line); //SPHERES/CONVEXES

            if (line.find("SPHERES:") != std::string::npos) {
                getline(file, line);
                std::vector<iREAL> centre = numbersAfterColon(line, 3);

                getline(file, line);
                iREAL rad = numberAfterColon(line);

                getline(file, line); //SURFID
                int surfid = static_cast<int>(numberAfterColon(line));
                //The file's SURFID is 1-based over the SURFACE_MATERIALS table.
                if(surfid < 1 || surfid > surfaceMaterials)
                {
                    throw std::logic_error("readmbfcp: body " + std::to_string(id) +
                                           " has SURFID " + std::to_string(surfid) +
                                           " outside the declared SURFACE_MATERIALS table (" +
                                           std::to_string(surfaceMaterials) + ")");
                }

                std::array<std::array<iREAL, 3>, 2> initial = readBodyInitialState(file, id);
                std::array<iREAL, 3> centreArray = {centre[0], centre[1], centre[2]};

                delta::world::structure::Object object("sphere", rad, id, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, true, epsilon, initial[0], initial[1]);

                //deriveForces() divides by the mass, so the bulk density has to
                //settle it here (the ctor only knows the material).
                object.setMass(density * (4.0/3.0) * 3.14159265358979323846 * rad*rad*rad);

                objects.push_back(object);

            } else if(line.find("CONVEXES:") != std::string::npos) {
                //Obstacle (hopper wall): CONVEXES/VERTEXES/FACES paragraph.
                //FACES index the flat vertex coordinate buffer, hence the /3.
                getline(file, line);
                int vertices = static_cast<int>(numberAfterColon(line));

                std::vector<std::array<iREAL, 3>> uniqueVertices;
                for(int v=0; v<vertices; v++)
                {
                    getline(file, line);
                    std::istringstream coordinates(line);
                    iREAL px, py, pz;
                    coordinates >> px >> py >> pz;
                    uniqueVertices.push_back({px, py, pz});
                }

                getline(file, line);
                int faces = static_cast<int>(numberAfterColon(line));

                std::vector<std::array<int, 3>> triangleFaces;
                for(int f=0; f<faces; f++)
                {
                    getline(file, line);
                    std::istringstream face(line);
                    int verticesPerFace, a, b, c, materialID;
                    face >> verticesPerFace >> a >> b >> c >> materialID;
                    triangleFaces.push_back({a/3, b/3, c/3});
                }

                delta::geometry::mesh::Mesh* mesh = new delta::geometry::mesh::Mesh(triangleFaces, uniqueVertices);

                //The mesh ctor re-centres the mesh on the centre it is given;
                //passing the mesh's own centre keeps the wall where the file
                //put it. Mass/inertia come from the mesh volume, the obstacle
                //gets a zero inverse so it never moves.
                iREAL centre[3];
                mesh->computeCenterOfGeometry(centre);

                std::array<std::array<iREAL, 3>, 2> initial = readBodyInitialState(file, id);

                delta::world::structure::Object object("obstacle", id, mesh,
                    std::array<iREAL, 3>{centre[0], centre[1], centre[2]},
                    delta::geometry::material::MaterialType::WOOD,
                    true, false, true, epsilon, initial[0], initial[1]);

                objects.push_back(object);
            }
        }
    }

    return scenario;
}
