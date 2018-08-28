/*
 The MIT License (MIT)

 Copyright (c) 2018 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <delta/world/scenario/configuration.h>

 void delta::world::configuration::uniformlyDistributedTotalMass(
	  std::vector<delta::world::structure::Object>&		insitu,
	  iREAL 												position[3],
	  int 												xzcuts,
	  int 												ycuts,
	  iREAL 												gridxyLength,
      iREAL 												totalMass,
	  iREAL 												hopperWidth,
      int 												index,
	  iREAL 												epsilon,
      bool 												isSphereOrNone,
      int 												noPointsPerParticle
      )
{
   //create xzy cuts above hopper, position starts at left lower inner corner
   std::vector<std::array<iREAL, 3>> grid = delta::world::layout::makeGridLayout(position, xzcuts, ycuts, gridxyLength);

   delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;
   bool isObstacle = false;
   bool isFriction = false;
   bool isConvex = true;

   iREAL xmin = 1; iREAL xmax = 0;
   for(unsigned i=0; i<grid.size(); i++)
   {
     std::array<iREAL, 3> p = {grid[i][0], grid[i][1], grid[i][2]};

     delta::world::structure::Object particles(
    		 isSphereOrNone ? "sphere": "granulate",
    		 index+i, p, material, isObstacle, isFriction, isConvex, epsilon, {0, 0, 0}, {0,0,0});
     insitu.push_back(particles);

     if(p[0] < xmin) xmin = p[0];
     if(p[0] > xmax) xmax = p[0];
   }

   //adjustment of xz dimension
   gridxyLength = xmax - xmin;
   //iREAL dx = (hopperWidth - gridxyLength)/2;

   iREAL dx = (hopperWidth - gridxyLength)/2;

   //printf("length1:%f\n", subGridLength);
   //printf("length2:%f\n", _hopperWidth-margin*2);

   for(unsigned i=0; i<insitu.size(); i++)
   {
     std::array<iREAL, 3> position = insitu[i].getCentre();
     position[0] += dx;  position[2] += dx;
     iREAL tmp[3] = {position[0], position[1], position[2]};
     insitu[i].setCentre(tmp);
   }

  if(isSphereOrNone)
  {
    delta::world::configuration::uniSphereRadius(totalMass, insitu);
  } else {
    delta::world::configuration::uniMeshGeometry(totalMass, noPointsPerParticle, insitu);

    //delta::world::configuration::uniCubeGeometry(totalMass, noPointsPerParticle, insitu);
  }

  //////////////////////////////////////////////////////
  /////MIN AND MAX RADIUS//////////////////////////////
  //////////////////////////////////////////////////////
  iREAL maxRad = 0.0;
  iREAL minRad = 1.00;

  for(unsigned i=0; i<insitu.size(); i++)
  {
    if(maxRad <= insitu[i].getRad()) maxRad = insitu[i].getRad();
    if(minRad >= insitu[i].getRad()) minRad = insitu[i].getRad();
  }

  //lift above max radii
  for(unsigned i=0; i<insitu.size(); i++)
  {
    std::array<iREAL, 3> pos = insitu[i].getCentre();
    iREAL p[3] = {pos[0], pos[1] + maxRad+epsilon, pos[2]};

    insitu[i].setCentre(p);

    if(!isSphereOrNone)
    {
	  auto yCoordinates = insitu[i].getMesh().getYCoordinatesAsVector();

	  if(yCoordinates.size() >= 0)
	  {
		for(unsigned i=0; i<yCoordinates.size(); i++)
		{
		  yCoordinates[i] += maxRad+epsilon;
		}
	  }
    }
  }
}

void delta::world::configuration::nonUniformlyDistributedTotalMass(
    std::vector<delta::world::structure::Object>&		insitu,
    iREAL 											position[3],
    int 												xzcuts,
	int 												ycuts,
	iREAL 											gridxyLength,
	iREAL		 									totalMass,
	iREAL 											hopperWidth,
	int 												index,
	iREAL 											epsilon,
	iREAL 											isSphereOrNone,
	iREAL 											subcellx,
	int 												noPointsPerParticle)
{
   //create xzy cuts above hopper, position starts at left lower inner corner
   std::vector<std::array<iREAL, 3>> grid = delta::world::layout::makeGridLayout(position, xzcuts, ycuts, gridxyLength);

   delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;
   bool isObstacle = false;
   bool isFriction = false;
   bool isConvex = true;

   iREAL xmin = 1; iREAL xmax = 0;
   for(unsigned i=0; i<grid.size(); i++)
   {
     std::array<iREAL, 3> p = {grid[i][0], grid[i][1], grid[i][2]};

     delta::world::structure::Object particles(
    		 isSphereOrNone ? "sphere": "granulate",
    		 index+1, p, material, isObstacle, isFriction, isConvex, epsilon, {0,0,0}, {0,0,0});
     insitu.push_back(particles);

     if(p[0] < xmin) xmin = p[0];
     if(p[0] > xmax) xmax = p[0];
   }

   //adjustment of xz dimension
   gridxyLength = xmax - xmin;
   //iREAL dx = (hopperWidth - gridxyLength)/2;
   iREAL dx = (hopperWidth - gridxyLength)/2;
   //printf("length1:%f\n", subGridLength);
   //printf("length2:%f\n", _hopperWidth-margin*2);

   for(unsigned i=0; i<insitu.size(); i++)
   {
     std::array<iREAL, 3> position = insitu[i].getCentre();
     position[0] += dx;  position[2] += dx;
     iREAL tmp[3] = {position[0], position[1], position[2]};
     insitu[i].setCentre(tmp);
   }

  if(isSphereOrNone)
  {
    delta::world::configuration::nonUniSphereRadius(totalMass, subcellx, insitu);
  } else {
    delta::world::configuration::nonUniMeshGeometry(totalMass, subcellx, noPointsPerParticle, insitu);
  }

  //////////////////////////////////////////////////////
  /////MIN AND MAX RADIUS//////////////////////////////
  //////////////////////////////////////////////////////
  iREAL maxRad = 0.0;
  iREAL minRad = 1.00;

  for(unsigned i=0; i<insitu.size(); i++)
  {
    if(maxRad <= insitu[i].getRad()) maxRad = insitu[i].getRad();
    if(minRad >= insitu[i].getRad()) minRad = insitu[i].getRad();
  }

  //lift above max radii
  for(unsigned i=0; i<insitu.size(); i++)
  {
    std::array<iREAL, 3> pos = insitu[i].getCentre();
    iREAL p[3] = {pos[0], pos[1] + maxRad+epsilon, pos[2]};

    insitu[i].setCentre(p);

    if(!isSphereOrNone)
    {
	  auto yCoordinates = insitu[i].getMesh().getYCoordinatesAsVector();

	  if(yCoordinates.size() >= 0)
	  {
		for(unsigned i=0; i<yCoordinates.size(); i++)
		{
		  yCoordinates[i] += maxRad+epsilon;
		}
	  }
    }
  }
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////
}

 void delta::world::configuration::uniSphereRadius(
    iREAL 											totalMass,
    std::vector<delta::world::structure::Object>&		objects)
{
  if((!objects.size()) > 0) return;

  delta::geometry::material::MaterialType material = objects[0].getMaterial();

  iREAL massPerParticle = totalMass/(iREAL)objects.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  for(unsigned i=0; i<objects.size(); i++)
  {
	//printf("%f\n", radius);
    objects[i].setRad(radius);
  }
}

 void delta::world::configuration::uniMeshGeometry(
    iREAL 											totalMass,
    int 												noPointsPerParticle,
    std::vector<delta::world::structure::Object>&		objects)
{
  iREAL massPerParticle = totalMass/(iREAL)objects.size();
  delta::geometry::material::MaterialType material = objects[0].getMaterial();

  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));
  //printf("%f %i %f\n", radius, int(delta::geometry::material::materialToDensitymap.find(material)->second), massPerParticle);

  iREAL reMassTotal = 0;
  iREAL masssphere = 0;

  for(unsigned i=0; i<objects.size(); i++)
  {
    std::array<iREAL, 3> pos = objects[i].getCentre();
    iREAL position[3] = {pos[0], pos[1], pos[2]};

    delta::geometry::mesh::Mesh *mesh = delta::geometry::primitive::granulate::generateParticle(position, (radius*2), noPointsPerParticle);
	objects[i].setMesh(*mesh);

    iREAL mt = objects[i].getMass();

    //iREAL vt = objects[i].getMesh().computeVolume();
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(material);
    reMassTotal += mt;
    masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  reMassTotal=0;
  for(unsigned j=0; j<objects.size(); j++)
  {
    objects[j].getMesh().scaleXYZ(rescale);
    objects[j].setRad(objects[j].getRad()*rescale);

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
    iREAL 											totalMass,
    int 												noPointsPerParticle,
    std::vector<delta::world::structure::Object>&		objects)
{
  iREAL massPerParticle = totalMass/(iREAL)objects.size();

  delta::geometry::material::MaterialType material = objects[0].getMaterial();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  iREAL masssphere = 0;

  for(int i=0; i<objects.size(); i++)
  {
    std::array<iREAL, 3> pos = objects[i].getCentre();
    iREAL position[3] = {pos[0], pos[1], pos[2]};

    delta::geometry::mesh::Mesh *mesh = delta::geometry::primitive::cube::generateHullCube(position, radius*2, radius*2, radius*2, 0, 0, 0, 0);
    objects[i].setMesh(*mesh);

    iREAL mt = mesh->computeMass(material);

    reMassTotal += mt;
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(unsigned j=0; j<objects.size(); j++)
  {
    objects[j].getMesh().scaleXYZ(rescale);
  }
}

 void delta::world::configuration::nonUniSphereRadius(
    iREAL 											totalMass,
    iREAL 											subcellx,
    std::vector<delta::world::structure::Object>&		objects)
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
  for(unsigned i=0; i<objects.size(); i++)
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
    objects[0+i].setRad(rad[i]);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(i,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }
  //printf("RESCALE:%f\n", rescale);
  //printf("TOTAL REREMASS:%f\n", reMassTotal);
}

 void delta::world::configuration::nonUniMeshGeometry(
    iREAL 											totalMass,
    iREAL 											subcellx,
	int 												meshDensity,
	std::vector<delta::world::structure::Object>&	objects)
{
  iREAL massPerParticle = totalMass/(iREAL)objects.size();
  delta::geometry::material::MaterialType material = objects[0].getMaterial();

  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  //iREAL masssphere = 0;

  //std::vector<iREAL> rad;
  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;
  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  std::vector<iREAL> rad;

  for(int i=0; i<objects.size(); i++)
  {
    iREAL particleDiameter = mindiam + (iREAL)(rand()) / ((iREAL) (RAND_MAX/(maxdiam-mindiam)));

    rad.push_back(particleDiameter/2);
    radius = particleDiameter/2;

    iREAL position[3] = { objects[i].getCentre()[0], objects[i].getCentre()[1], objects[i].getCentre()[2]};

    delta::geometry::mesh::Mesh *mesh =
    	  delta::geometry::primitive::granulate::generateParticle(position, (radius*2), meshDensity);
	objects[i].setMesh(*mesh);

    iREAL mt = mesh->computeMass(material);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinates, yCoordinates, zCoordinates);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::geometry::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  //reMassTotal=0;
  for(unsigned j=0; j<objects.size(); j++)
  {
    objects[j].getMesh().scaleXYZ(rescale);

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

 void delta::world::configuration::nonUniCubeGeometry(
  iREAL 											totalMass,
  iREAL 											subcellx,
  int 											meshDensity,
  std::vector<delta::world::structure::Object>&	objects)
{
   iREAL massPerParticle = totalMass/(iREAL)objects.size();
   delta::geometry::material::MaterialType material = objects[0].getMaterial();

   iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

   iREAL reMassTotal = 0;
   //iREAL masssphere = 0;

   //std::vector<iREAL> rad;
   iREAL mindiam = radius;
   iREAL maxdiam = radius*2;
   if(radius*2 > subcellx)
     printf("ERROR:radius bigger than subcellx\n");

   std::vector<iREAL> rad;

   for(int i=0; i<objects.size(); i++)
   {
     iREAL particleDiameter = mindiam + (iREAL)(rand()) / ((iREAL) (RAND_MAX/(maxdiam-mindiam)));

     rad.push_back(particleDiameter/2);
     radius = particleDiameter/2;

     iREAL position[3] = { objects[i].getCentre()[0], objects[i].getCentre()[1], objects[i].getCentre()[2]};

    delta::geometry::mesh::Mesh *mesh = delta::geometry::primitive::cube::generateHullCube(position, radius*2, radius*2, radius*2, 0, 0, 0, 0);
    objects[i].setMesh(*mesh);

     iREAL mt = mesh->computeMass(material);
     //iREAL vt = delta::geometry::properties::getVolume(xCoordinates, yCoordinates, zCoordinates);
     //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
     //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::geometry::material::MaterialDensity::WOOD);
     reMassTotal += mt;
     //masssphere += mt;
     //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
   }

   iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
   //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

   //reMassTotal=0;
   for(unsigned j=0; j<objects.size(); j++)
   {
     objects[j].getMesh().scaleXYZ(rescale);

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
    iREAL 												position[3],
    iREAL 												width,
    int 													layers,
	iREAL 												epsilon,
	std::vector<delta::world::structure::Object>& 		objects)
{

  //_particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam

  //measurements
  delta::geometry::mesh::Mesh *mesh = delta::geometry::hardcoded::generateBrickFB();
  iREAL w = mesh->computeXZw();
  iREAL h = mesh->computeYw();

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

      delta::world::structure::Object obj(
    	  "FB", 0, tmp[i], material, false, false, true, epsilon, {0,0,0}, {0,0,0});
/*
	  iREAL pos[3] = {tmp[i][0], tmp[i][1], tmp[i][2]};
	  delta::geometry::mesh::Mesh *geometry = delta::geometry::hardcoded::generateBrickFB(pos, 0.1);
	  delta::geometry::Object obj("FB", 0, geometry, tmp[i], delta::geometry::material::MaterialType::WOOD, false, false, epsilon, {0,0,0}, {0,0,0});
*/
      objects.push_back(obj);
    }
    //std::cout << tmp.size() << " " << particleGrid.size() << std::endl;
  }

  for(unsigned i=0; i<objects.size(); i++) {
    objects[i].setRad(scalePercentage);
  }
}

 void delta::world::configuration::makeBrickGrid(
    iREAL 												position[3],
    iREAL 												arrayXZlength,
    int   												xzElements,
    iREAL 												arrayYlength,
    int   												yElements,
	iREAL 												epsilon,
    std::vector<delta::world::structure::Object>& 		objects)
{
  std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

  //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
  //measurements
  delta::geometry::mesh::Mesh *mesh = delta::geometry::hardcoded::generateBrickFB();
  iREAL width = mesh->computeXZw();
  iREAL height = mesh->computeYw();
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

    delta::geometry::mesh::Mesh *geometry = delta::geometry::hardcoded::generateBrickFB(position, scalePercentage);
    delta::world::structure::Object obj(
    	"FB", i, geometry, particleGrid[i], material, false, false, true, epsilon, {0,0,0}, {0,0,0});
    obj.setRad(scalePercentage);
    objects.push_back(obj);
  }
}
