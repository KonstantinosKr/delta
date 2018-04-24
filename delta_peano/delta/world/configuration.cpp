/*
 * configuration.cpp
 *
 *  Created on: 2 Apr 2018
 *      Author: konstantinos
 */

#include <delta/geometry/defined/graphite.h>
#include <delta/world/arrayLayout.h>
#include "delta/world/configuration.h"

#include "delta/geometry/primitive/granulate.h"
#include "delta/geometry/primitive/cube.h"

 std::vector<delta::geometry::Object> delta::world::configuration::uniformlyDistributedTotalMass(
	  iREAL 		position[3],
	  int 		xzcuts,
	  int 		ycuts,
	  iREAL 		gridxyLength,
      iREAL 		totalMass,
	  iREAL 		hopperWidth,
      int 		index,
	  iREAL 		epsilon,
      bool 		isSphereOrNone,
      int 		noPointsPerParticle
      )
{
   std::vector<delta::geometry::Object> objects;
   //create xzy cuts above hopper, position starts at left lower inner corner
   std::vector<std::array<iREAL, 3>> grid = delta::world::layout::makeGridLayout(position, xzcuts, ycuts, gridxyLength);

   iREAL xmin = 1; iREAL xmax = 0;
   for(unsigned i=0; i<grid.size(); i++)
   {
     std::array<iREAL, 3> p = {grid[i][0], grid[i][1], grid[i][2]};

     delta::geometry::Object particles(isSphereOrNone ? "sphere": "granulate", 0, p, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, {0, -1, 0}, {0,0,0});

     objects.push_back(particles);

     if(p[0] < xmin) xmin = p[0];
     if(p[0] > xmax) xmax = p[0];
   }

   //adjustment of xz dimension
   gridxyLength = xmax - xmin;
   //iREAL dx = (hopperWidth - gridxyLength)/2;
   iREAL dx = (hopperWidth - gridxyLength)/2;
   //printf("length1:%f\n", subGridLength);
   //printf("length2:%f\n", _hopperWidth-margin*2);

   for(unsigned i=0; i<objects.size(); i++)
   {
     std::array<iREAL, 3> position = objects[i].getCentre();
     position[0] += dx;  position[2] += dx;
     iREAL tmp[3] = {position[0], position[1], position[2]};
     objects[i].setCentre(tmp);
   }

  if(isSphereOrNone)
  {
    delta::world::configuration::uniSphereRadius(totalMass, index, objects);
  } else {
    delta::world::configuration::uniMeshGeometry(totalMass, noPointsPerParticle, objects, index);

    /*
    delta::world::uniCubeGeometry(
        totalMass,
        material,
        noPointsPerParticle,
        rad,
        particleGrid,
        componentGrid,
        messArray,
        index);
    */
  }

  //////////////////////////////////////////////////////
  /////MIN AND MAX RADIUS//////////////////////////////
  //////////////////////////////////////////////////////
  iREAL maxRad = 0.0;
  iREAL minRad = 1.00;

  for(unsigned i=index; i<objects.size(); i++)
  {
    if(maxRad <= objects[i].getRad()) maxRad = objects[i].getRad();
    if(minRad >= objects[i].getRad()) minRad = objects[i].getRad();
  }


  //lift above max radii
  for(unsigned i=index; i<objects.size(); i++)
  {
    std::array<iREAL, 3> pos = objects[i].getCentre();
    iREAL p[3] = {pos[0], pos[1] + maxRad+epsilon, pos[2]};
    objects[i].setCentre(p);

    auto yCoordinates = objects[i].getMesh().getYCoordinatesAsVector();

    if(yCoordinates.size() >= 0)
    {
      for(unsigned i=0; i<yCoordinates.size(); i++)
      {
        yCoordinates[i] += maxRad+epsilon;
      }
    }
  }

  return objects;
}

 std::vector<delta::geometry::Object> delta::world::configuration::nonUniformlyDistributedTotalMass(
		  iREAL position[3],
		  int xzcuts,
		  int ycuts,
		  iREAL gridxyLength,
		  iREAL totalMass,
		  iREAL hopperWidth,
		  int index,
		  iREAL epsilon,
          iREAL isSphereOrNone,
          iREAL subcellx,
          int _noPointsPerParticle)
{
   std::vector<delta::geometry::Object> objects;
   //create xzy cuts above hopper, position starts at left lower inner corner
   std::vector<std::array<iREAL, 3>> grid = delta::world::layout::makeGridLayout(position, xzcuts, ycuts, gridxyLength);

   delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;
   bool isObstacle = false;
   bool isFriction = false;

   iREAL xmin = 1; iREAL xmax = 0;
   for(unsigned i=0; i<grid.size(); i++)
   {
     std::array<iREAL, 3> p = {grid[i][0], grid[i][1], grid[i][2]};

     delta::geometry::Object particles(isSphereOrNone ? "sphere": "granulate", 0, p, delta::geometry::material::MaterialType::WOOD, isObstacle, isFriction, epsilon, {0, -1, 0}, {0,0,0});

     objects.push_back(particles);

     if(p[0] < xmin) xmin = p[0];
     if(p[0] > xmax) xmax = p[0];
   }

   //adjustment of xz dimension
   gridxyLength = xmax - xmin;
   //iREAL dx = (hopperWidth - gridxyLength)/2;
   iREAL dx = (hopperWidth - gridxyLength)/2;
   //printf("length1:%f\n", subGridLength);
   //printf("length2:%f\n", _hopperWidth-margin*2);

   for(unsigned i=0; i<objects.size(); i++)
   {
     std::array<iREAL, 3> position = objects[i].getCentre();
     position[0] += dx;  position[2] += dx;
     iREAL tmp[3] = {position[0], position[1], position[2]};
     objects[i].setCentre(tmp);
   }

  if(isSphereOrNone)
  {
    delta::world::configuration::nonUniSphereRadius(totalMass, index, subcellx, objects);
  } else {

	/*
    delta::world::nonUniMeshGeometry(
        totalMass,
        subcellx,
        _noPointsPerParticle);*/

   }

  //////////////////////////////////////////////////////
  /////MIN AND MAX RADIUS//////////////////////////////
  //////////////////////////////////////////////////////
  iREAL maxRad = 0.0;
  iREAL minRad = 1.00;

  for(unsigned i=index; i<objects.size(); i++)
  {
    if(maxRad <= objects[i].getRad()) maxRad = objects[i].getRad();
    if(minRad >= objects[i].getRad()) minRad = objects[i].getRad();
  }


  //lift above max radii
  for(unsigned i=index; i<objects.size(); i++)
  {
    std::array<iREAL, 3> pos = objects[i].getCentre();
    iREAL p[3] = {pos[0], pos[1] + maxRad+epsilon, pos[2]};
    objects[i].setCentre(p);

    auto yCoordinates = objects[i].getMesh().getYCoordinatesAsVector();

    if(yCoordinates.size() >= 0)
    {
      for(unsigned i=0; i<yCoordinates.size(); i++)
      {
        yCoordinates[i] += maxRad+epsilon;
      }
    }
  }
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////

  return objects;
}

 void delta::world::configuration::uniSphereRadius(
    iREAL totalMass,
    int index,
    std::vector<delta::geometry::Object> &objects)
{

  if((!objects.size()) > 0) return;

  delta::geometry::material::MaterialType material = objects[0].getMaterial();

  iREAL massPerParticle = totalMass/(iREAL)objects.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  for(unsigned i=index; i<objects.size(); i++)
  {
    objects[i].setRad(radius);
  }
}

 void delta::world::configuration::uniMeshGeometry(
    iREAL totalMass,
    int noPointsPerParticle,
    std::vector<delta::geometry::Object> &objects,
    int index)
{
  iREAL massPerParticle = totalMass/(iREAL)objects.size();
  delta::geometry::material::MaterialType material = objects[0].getMaterial();

  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  //iREAL masssphere = 0;

  iREAL position[3];
  for(unsigned i=index; i<objects.size(); i++)
  {
    objects[i].setParticleID(index);
    objects[i].generateMesh(0,0,0,0,0,0,noPointsPerParticle, radius);
    iREAL mt = objects[i].getMass();

    //iREAL vt = delta::geometry::properties::getVolume(xCoordinates, yCoordinates, zCoordinates);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(material);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  reMassTotal=0;
  for(unsigned j=0; j<objects.size(); j++)
  {
    std::array<iREAL, 3> ar = objects[j].getCentre(); position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];

    std::vector<iREAL> xCoordinates = objects[j].getMesh().getXCoordinatesAsVector();
    std::vector<iREAL> yCoordinates = objects[j].getMesh().getYCoordinatesAsVector();
    std::vector<iREAL> zCoordinates = objects[j].getMesh().getZCoordinatesAsVector();

    objects[j].getMesh().scaleXYZ(rescale, position);

    //iREAL mt = delta::geometry::properties::getMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], delta::geometry::material::MaterialType::WOOD);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::geometry::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);
}

 void delta::world::configuration::uniCubeGeometry(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<delta::geometry::mesh::Mesh>  &meshArray,
	int index)
{
  iREAL massPerParticle = totalMass/(iREAL)particleGrid.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  //iREAL masssphere = 0;

  iREAL position[3];
  for(auto i:particleGrid)
  {
    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    delta::geometry::mesh::Mesh *mesh = delta::geometry::primitive::cube::generateHullCube(position, radius*2, radius*2, radius*2, 0, 0, 0, 0);
    meshArray.push_back(*mesh);

    iREAL mt = mesh->computeMass(material);

    reMassTotal += mt;

    componentGrid.push_back("granulate");
    rad.push_back(radius);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(unsigned j=0; j<meshArray.size(); j++)
  {
    std::array<iREAL, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::geometry::mesh::Mesh mesh = meshArray[j];
    mesh.scaleXYZ(rescale, position);
    meshArray[j] = mesh;
  }
}

 void delta::world::configuration::nonUniSphereRadius(
    iREAL totalMass,
    int index,
    iREAL subcellx,
    std::vector<delta::geometry::Object> &objects)
{
  if((!objects.size()) > 0) return;

  delta::geometry::material::MaterialType material = objects[0].getMaterial();

  iREAL massPerParticle = totalMass/(iREAL)objects.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;

  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;

  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  std::vector<iREAL> rad;
  for(unsigned i=index; i<objects.size(); i++)
  {
    iREAL particleDiameter = mindiam + (iREAL)(rand()) / ((iREAL) (RAND_MAX/(maxdiam-mindiam)));
    rad.push_back(particleDiameter/2);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(particleDiameter/2,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }

  //get total mass
  //printf("TOTAL REMASS:%f\n", reMassTotal);

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(unsigned i=0; i<rad.size(); i++)
  {
    rad[i] = rad[i] * rescale;
    objects[index+i].setRad(rad[i]);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(i,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }
  //printf("RESCALE:%f\n", rescale);
  //printf("TOTAL REREMASS:%f\n", reMassTotal);
}

 void delta::world::configuration::nonUniMeshGeometry(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    iREAL subcellx, int meshDensity,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<delta::geometry::mesh::Mesh>  &meshArray,
	int index)
{
  iREAL massPerParticle = totalMass/(iREAL)particleGrid.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  //iREAL masssphere = 0;

  iREAL position[3];
  //std::vector<iREAL> rad;
  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;
  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  std::vector<iREAL> rad;

  for(auto i:particleGrid)
  {
    std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    iREAL particleDiameter = mindiam + (iREAL)(rand()) / ((iREAL) (RAND_MAX/(maxdiam-mindiam)));

    rad.push_back(particleDiameter/2);
    radius = particleDiameter/2;

    delta::geometry::mesh::Mesh *mesh =
    	  delta::geometry::primitive::granulate::generateParticle(position, (radius*2), meshDensity);
    meshArray.push_back(*mesh);

    iREAL mt = mesh->computeMass(material);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinates, yCoordinates, zCoordinates);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::geometry::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    componentGrid.push_back("nonSpherical");
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  //reMassTotal=0;
  for(unsigned j=0; j<meshArray.size(); j++)
  {
    std::array<iREAL, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];

    delta::geometry::mesh::Mesh mesh = meshArray[j];
    mesh.scaleXYZ(rescale, position);

    rad[j] = rad[j] * rescale;

    //iREAL mt = delta::geometry::properties::getMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], material);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::geometry::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);
}

 void delta::world::configuration::loadNuclearGeometry(
    iREAL position[3],
    iREAL width,
    int layers,
	iREAL epsilon,
	std::vector<delta::geometry::Object>& objects)
{

  //_particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam

  //measurements
  delta::geometry::mesh::Mesh *mesh = delta::geometry::defined::generateBrickFB();
  iREAL w = mesh->getXZWidth();
  iREAL h = mesh->getYw();

  //read nuclear graphite schematics
  //std::vector<std::vector<std::string>> compoGrid;
  //delta::core::parseModelGridSchematics("input/nuclear_core", compoGrid, componentGrid);

  ///place components of 2d array structure
  int elements = 46;
  iREAL arrayXZlength = width/2;
  iREAL xzWCell = delta::world::layout::getDiscritization(arrayXZlength, elements);

  iREAL scalePercentage = 0;
  if(w > xzWCell)//brick is bigger than grid
  {
    iREAL excess = w - xzWCell;
    scalePercentage = ((w-excess)/w);
    printf("SCALE DOWN:%f w:%f wperc:%f excess:%f boxWidth:%f\n", scalePercentage, w, w/100, xzWCell, excess);
  }

  position[0] -= arrayXZlength/2;
  position[1] -= (h*scalePercentage);
  position[2] -= arrayXZlength/2;

  for(int i=0; i<layers; i++)
  {
    position[1] += (h*scalePercentage)*i;
    std::vector<std::array<iREAL, 3>> tmp = delta::world::layout::array2d(position, arrayXZlength, elements);

    for(unsigned j=0; j<tmp.size(); j++)
    {
      auto material = delta::geometry::material::MaterialType::GRAPHITE;

      delta::geometry::Object obj("FB", 0, tmp[i], material, false, false, epsilon, {0,0,0}, {0,0,0});
      objects.push_back(obj);
    }
    //std::cout << tmp.size() << " " << particleGrid.size() << std::endl;
  }

  for(unsigned i=0; i<objects.size(); i++) {
    objects[i].setRad(scalePercentage);
  }
}

 void delta::world::configuration::makeBrickGrid(
    iREAL position[3],
    iREAL arrayXZlength,
    int   xzElements,
    iREAL arrayYlength,
    int   yElements,
	iREAL epsilon,
    std::vector<delta::geometry::Object>& objects)
{
  std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

  //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
  //measurements
  delta::geometry::mesh::Mesh *mesh = delta::geometry::defined::generateBrickFB();
  iREAL width = mesh->getXZWidth();
  iREAL height = mesh->getYw();
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///place components of 2d array structure
  iREAL xzWCell = delta::world::layout::getDiscritization(arrayXZlength, xzElements);

  iREAL scalePercentage = 0;
  if(xzWCell < width)
  {//brick is bigger than grid
    iREAL excess = width - xzWCell;
    scalePercentage = ((width - excess)/width);
    //printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  }
  else if (xzWCell < width)
  {
    iREAL excess = width - xzWCell;
    scalePercentage = ((width - excess)/width);
  }
  else if (xzWCell < height)
  {
    iREAL excess = height - xzWCell;
    scalePercentage = ((height - excess)/height);
  } else {
    iREAL excess = xzWCell - height;
    scalePercentage = 1+((xzWCell - excess)/xzWCell);
    //printf("SCALE UP: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  }

  width *= scalePercentage;
  height *= scalePercentage;

  position[0] = (position[0]-arrayXZlength/2)+xzWCell/2;
  position[1] = position[1]+(height/2);
  position[2] = (position[2]-arrayXZlength/2)+xzWCell/2;

  std::vector<std::array<iREAL, 3>> particleGrid = delta::world::layout::array3d(position, arrayXZlength, xzElements, arrayYlength, yElements);

  delta::world::layout::collapseUniformGrid(position, particleGrid, xzElements, yElements, width, height, 0);

  for(unsigned i=0; i<particleGrid.size(); i++)
  {
    auto material = delta::geometry::material::MaterialType::GRAPHITE;

    delta::geometry::Object obj("FB", i, particleGrid[i], material, false, false, epsilon, {0,0,0}, {0,0,0});
    obj.setRad(scalePercentage);
    objects.push_back(obj);
  }
}
