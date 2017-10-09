/*
 * assembly.cpp
 *
 *  Created on: 13 Feb 2017
 *      Author: user
 */

#include "assembly.h"


iREAL delta::world::assembly::getDiscritization(
    iREAL length,
    int number)
{
	return length/number;
}

std::vector<std::array<iREAL, 3>> delta::world::assembly::array1d(
    iREAL position[3],
    iREAL xAxisLength,
    int partsNo)
{
	std::vector<std::array<iREAL, 3>> array;
	std::array<iREAL, 3> point = {0.0, 0.0, 0.0};

	iREAL length = getDiscritization(xAxisLength, partsNo);

	point[0] = position[0];
	point[1] = position[1];
	point[2] = position[2];
	array.push_back(point);

	for(int i=0;i<partsNo-1;i++)
	{
		point[0] += length;
		array.push_back(point);
	}
	return array;
}

std::vector<std::array<iREAL, 3>> delta::world::assembly::array2d(
    iREAL position[3],
    iREAL xyAxisLength,
    int partsNo)
{
	std::vector<std::array<iREAL, 3>> array;

	iREAL length = getDiscritization(xyAxisLength, partsNo);

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<iREAL, 3>> tmp = delta::world::assembly::array1d(position, xyAxisLength, partsNo);
		for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[2] += length;
	}

	return array;
}

std::vector<std::array<iREAL, 3>> delta::world::assembly::array3d(
    iREAL position[3],
    iREAL xyzAxisLength,
    int partsNo)
{
	std::vector<std::array<iREAL, 3>> array;

	iREAL length = getDiscritization(xyzAxisLength, partsNo);

  iREAL resetx = position[0];
	iREAL resetz = position[2];

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<iREAL, 3>> tmp = delta::world::assembly::array2d(position, xyzAxisLength, partsNo);
		for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[0] = resetx;
		position[1] += length;
		position[2] = resetz;
		if(position[1] > 1.0)
			break;
	}

	return array;
}

std::vector<std::array<iREAL, 3>> delta::world::assembly::array3d(
    iREAL position[3],
    iREAL xyzAxisLength,
    int partsXYZNo,
    iREAL yAxisLength,
    int partsYNo)
{
	std::vector<std::array<iREAL, 3>> array;

	iREAL lengthY =  getDiscritization(yAxisLength, partsYNo);

	for(int i=0;i<partsYNo;i++)
	{
		std::vector<std::array<iREAL, 3>> tmp = delta::world::assembly::array2d(position, xyzAxisLength, partsXYZNo);
		for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[1] += lengthY;
		position[2] -= xyzAxisLength;
		if(position[1] > 1.0)
			break;
	}

	return array;
}

void delta::world::assembly::collapseUniformGrid(iREAL position[3], std::vector<std::array<iREAL, 3>>& grid, int xzcuts, int ycuts, iREAL elementWidth, iREAL elementHeight, iREAL epsilon)
{
  int xAxisLoc = 0;
  int zAxisLoc = 0;
  int yAxisLoc = 0;

  elementWidth += epsilon*2;
  elementHeight += epsilon*2;

  double dx = (grid[1][0] - grid[0][0]) - elementWidth;
  double dy = (grid[xzcuts*xzcuts][1] - grid[xzcuts*xzcuts-1][1]) - elementHeight;
  double dz = dx;

  ////////////////////////////////////////
  //collapse from left to right on X AXIS
  ////////////////////////////////////////
  for(int i=0; i<grid.size(); i++)
  {
    if(xAxisLoc > 0)
    {
      //printf("%f\n", dx);
      grid[i][0] -= dx;
    }
    if(yAxisLoc > 0)
    {
      //printf("%i\n", yAxisLoc);
      //grid[i][1] -= dy;
    }
    if(zAxisLoc > 0)
    {
      grid[i][2] += dz;
    }

    xAxisLoc++;
    /////////////////////////////
    if(xAxisLoc >= xzcuts)
    {
      xAxisLoc = 0;
      zAxisLoc++;
    }
    if(zAxisLoc >= xzcuts)
    {
      zAxisLoc = 0;
    }
    if(xAxisLoc == 0 && zAxisLoc == 0)
    {
      yAxisLoc++;
    }
    //////////////////////////
  }

}

std::vector<std::array<iREAL, 3>> delta::world::assembly::getGridArrayList(
    iREAL position[3],
    int xzcuts,
    int ycuts,
    iREAL width)
{
  return delta::world::assembly::array3d(position, width, xzcuts, width, ycuts);
}

void delta::world::assembly::uniform (
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    bool isSphereOrNone,
    int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<std::vector<iREAL>>  &xCoordinatesArray,
    std::vector<std::vector<iREAL>>  &yCoordinatesArray,
    std::vector<std::vector<iREAL>>  &zCoordinatesArray,
	int index)
{
  if(isSphereOrNone)
  {
    delta::world::assembly::uniSphereRadius(
         totalMass,
         material,
         rad,
         particleGrid,
         componentGrid,
         index);
  } else {
    /*delta::world::assembly::uniMeshGeometry(
        totalMass,
        material,
        noPointsPerParticle,
        rad,
        particleGrid,
        componentGrid,
        xCoordinatesArray,
        yCoordinatesArray,
        zCoordinatesArray);*/

    delta::world::assembly::uniCubeGeometry(
        totalMass,
        material,
        noPointsPerParticle,
        rad,
        particleGrid,
        componentGrid,
        xCoordinatesArray,
        yCoordinatesArray,
        zCoordinatesArray,
		index);
  }
}

void delta::world::assembly::uniform(
      iREAL totalMass,
      int index,
      bool isSphereOrNone,
      int noPointsPerParticle,
      std::vector<delta::world::object> &_insitufineObjects)
{

  if(isSphereOrNone)
  {
    delta::world::assembly::uniSphereRadius(totalMass, index, _insitufineObjects);
  } else {
    /*delta::world::assembly::uniMeshGeometry(
        totalMass,
        material,
        noPointsPerParticle,
        rad,
        particleGrid,
        componentGrid,
        xCoordinatesArray,
        yCoordinatesArray,
        zCoordinatesArray);*/

    /*
    delta::world::assembly::uniCubeGeometry(
        totalMass,
        material,
        noPointsPerParticle,
        rad,
        particleGrid,
        componentGrid,
        xCoordinatesArray,
        yCoordinatesArray,
        zCoordinatesArray,
        index);
    */
  }
}

void delta::world::assembly::nonuniform (
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    bool isSphereOrNone,
    iREAL subcellx,
    int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<std::vector<iREAL>>  &xCoordinatesArray,
    std::vector<std::vector<iREAL>>  &yCoordinatesArray,
    std::vector<std::vector<iREAL>>  &zCoordinatesArray,
	int index)
{
  if(isSphereOrNone)
  {
    delta::world::assembly::nonUniSphereRadius(
        totalMass,
        material,
        subcellx,
        rad,
        particleGrid,
        componentGrid,
        index);
  } else {
    delta::world::assembly::nonUniMeshGeometry(
        totalMass,
        material,
        subcellx,
        noPointsPerParticle,
        rad,
        particleGrid,
        componentGrid,
        xCoordinatesArray,
        yCoordinatesArray,
        zCoordinatesArray,
        index);
     }
}


void delta::world::assembly::nonuniform(
          iREAL totalMass,
          int index,
          iREAL isSphereOrNone,
          iREAL subcellx,
          int _noPointsPerParticle,
          std::vector<delta::world::object> &_insitufineObjects)
{
  if(isSphereOrNone)
  {
    delta::world::assembly::nonUniSphereRadius(totalMass, index, subcellx, _insitufineObjects);
  } else {
    /*
    delta::world::assembly::nonUniMeshGeometry(
        totalMass,
        subcellx,
        noPointsPerParticle);*/
     }
}

void delta::world::assembly::uniSphereRadius(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    int index)
{
  iREAL massPerParticle = totalMass/(iREAL)particleGrid.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  for(int i=index; i<particleGrid.size(); i++)
  {
    rad.push_back(radius);
    componentGrid.push_back("sphere");
  }
}


void delta::world::assembly::uniSphereRadius(
    iREAL totalMass,
    int index,
    std::vector<delta::world::object> &_insitufineObjects)
{

  if(!_insitufineObjects.size() > 0) return;

  delta::geometry::material::MaterialType material = _insitufineObjects[0].getMaterial();

  iREAL massPerParticle = totalMass/(iREAL)_insitufineObjects.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  for(int i=index; i<_insitufineObjects.size(); i++)
  {
    _insitufineObjects[i].generateSphere(radius);
  }
}

void delta::world::assembly::uniMeshGeometry(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<std::vector<iREAL>>  &xCoordinatesArray,
    std::vector<std::vector<iREAL>>  &yCoordinatesArray,
    std::vector<std::vector<iREAL>>  &zCoordinatesArray,
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

    std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;
    delta::geometry::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, noPointsPerParticle);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    iREAL mt = delta::geometry::properties::getMass(xCoordinates, yCoordinates, zCoordinates, material);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinates, yCoordinates, zCoordinates);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::geometry::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    componentGrid.push_back("granulate");
    rad.push_back(radius);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<iREAL, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::geometry::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);

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

void delta::world::assembly::uniCubeGeometry(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<std::vector<iREAL>>  &xCoordinatesArray,
    std::vector<std::vector<iREAL>>  &yCoordinatesArray,
    std::vector<std::vector<iREAL>>  &zCoordinatesArray,
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

    std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

    delta::geometry::cubes::generateHullCube(position, radius*2, radius*2, radius*2, 0, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    iREAL mt = delta::geometry::properties::getMass(xCoordinates, yCoordinates, zCoordinates, material);

    reMassTotal += mt;
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    componentGrid.push_back("granulate");
    rad.push_back(radius);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<iREAL, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::geometry::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
  }
}

void delta::world::assembly::nonUniSphereRadius(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    iREAL subcellx,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
	int index)
{
  iREAL massPerParticle = totalMass/(iREAL)particleGrid.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;

  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;

  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  for(int i=0; i<particleGrid.size(); i++)
  {
    iREAL particleDiameter = mindiam + static_cast <iREAL>(rand()) / (static_cast <iREAL> (RAND_MAX/(maxdiam-mindiam)));
    rad.push_back(particleDiameter/2);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(particleDiameter/2,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }

  //get total mass
  //printf("TOTAL REMASS:%f\n", reMassTotal);

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(auto i:rad)
  {
    i = i* rescale;

    componentGrid.push_back("sphere");

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(i,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }
  //printf("RESCALE:%f\n", rescale);
  //printf("TOTAL REREMASS:%f\n", reMassTotal);
}


void delta::world::assembly::nonUniSphereRadius(
    iREAL totalMass,
    int index,
    iREAL subcellx,
    std::vector<delta::world::object> &_insitufineObjects)
{
  if(!_insitufineObjects.size() > 0) return;

  delta::geometry::material::MaterialType material = _insitufineObjects[0].getMaterial();

  iREAL massPerParticle = totalMass/(iREAL)_insitufineObjects.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;

  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;

  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  std::vector<double> rad;
  for(int i=index; i<_insitufineObjects.size(); i++)
  {
    iREAL particleDiameter = mindiam + static_cast <iREAL>(rand()) / (static_cast <iREAL> (RAND_MAX/(maxdiam-mindiam)));
    rad.push_back(particleDiameter/2);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(particleDiameter/2,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }

  //get total mass
  //printf("TOTAL REMASS:%f\n", reMassTotal);

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(int i=0; i<rad.size(); i++)
  {
    rad[i] = rad[i] * rescale;
    _insitufineObjects[index+i].generateSphere(rad[i]);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(i,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }
  //printf("RESCALE:%f\n", rescale);
  //printf("TOTAL REREMASS:%f\n", reMassTotal);
}

void delta::world::assembly::nonUniMeshGeometry(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    iREAL subcellx, int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<std::vector<iREAL>>  &xCoordinatesArray,
    std::vector<std::vector<iREAL>>  &yCoordinatesArray,
    std::vector<std::vector<iREAL>>  &zCoordinatesArray,
	int index)
{
  iREAL massPerParticle = totalMass/(iREAL)particleGrid.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  iREAL masssphere = 0;

  iREAL position[3];
  //std::vector<iREAL> rad;
  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;
  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  for(auto i:particleGrid)
  {
    std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    iREAL particleDiameter = mindiam + static_cast <iREAL>(rand()) / (static_cast <iREAL> (RAND_MAX/(maxdiam-mindiam)));

    rad.push_back(particleDiameter/2);
    radius = particleDiameter/2;

    delta::geometry::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, noPointsPerParticle);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    iREAL mt = delta::geometry::properties::getMass(xCoordinates, yCoordinates, zCoordinates, material);
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
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<iREAL, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::geometry::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);

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

void delta::world::assembly::loadNuclearGeometry(
    iREAL position[3],
    iREAL width,
    int layers,
    std::vector<delta::world::object>& _insitufineObjects)
{

  //_particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam

  //measurements
  std::vector<iREAL> xCoordinates, yCoordinates, zCoordinates;
  delta::geometry::graphite::generateBrickFB(xCoordinates, yCoordinates, zCoordinates);
  iREAL w = delta::geometry::properties::getXZWidth(xCoordinates, yCoordinates, zCoordinates);
  iREAL h = delta::geometry::properties::getYw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

  //read nuclear graphite schematics
  //std::vector<std::vector<std::string>> compoGrid;
  //delta::core::parseModelGridSchematics("input/nuclear_core", compoGrid, componentGrid);

  ///place components of 2d array structure
  int elements = 46;
  iREAL arrayXZlength = width/2;
  iREAL xzWCell = delta::world::assembly::getDiscritization(arrayXZlength, elements);

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
    std::vector<std::array<iREAL, 3>> tmp = delta::world::assembly::array2d(position, arrayXZlength, elements);

    for(int j=0; j<tmp.size(); j++)
    {
      std::array<double, 3> linear = {0.0, 0.0, 0.0};
      std::array<double, 3> angular = {0.0, 0.0, 0.0};
      auto material = delta::geometry::material::MaterialType::GRAPHITE;

      delta::world::object obj("FB", 0, tmp[i], linear, angular, material, false, false);
      _insitufineObjects.push_back(obj);
    }
    //std::cout << tmp.size() << " " << particleGrid.size() << std::endl;
  }

  for(unsigned i=0; i<_insitufineObjects.size(); i++) {
    _insitufineObjects[i].setRad(scalePercentage);
  }
}

void delta::world::assembly::makeBrickGrid(
    iREAL position[3],
    iREAL arrayXZlength,
    int   xzElements,
    iREAL arrayYlength,
    int   yElements,
    std::vector<delta::world::object>& _insitufineObjects)
{
  std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

  //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
  //measurements
  iREAL pos[3]; pos[0] = pos[1] = pos[2] = 0;
  delta::geometry::graphite::generateBrickFB(pos, xCoordinates, yCoordinates, zCoordinates);
  iREAL width = delta::geometry::properties::getXZWidth(xCoordinates, yCoordinates, zCoordinates);
  iREAL height = delta::geometry::properties::getYw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///place components of 2d array structure
  iREAL xzWCell = delta::world::assembly::getDiscritization(arrayXZlength, xzElements);

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

  std::vector<std::array<iREAL, 3>> particleGrid = delta::world::assembly::array3d(position, arrayXZlength, xzElements, arrayYlength, yElements);

  collapseUniformGrid(position, particleGrid, xzElements, yElements, width, height, 0);

  for(unsigned i=0; i<particleGrid.size(); i++)
  {
    std::array<double, 3> linear = {0.0, 0.0, 0.0};
    std::array<double, 3> angular = {0.0, 0.0, 0.0};
    auto material = delta::geometry::material::MaterialType::GRAPHITE;

    delta::world::object obj("FB", i, particleGrid[i], linear, angular, material, false, false);
    obj.setRad(scalePercentage);
    _insitufineObjects.push_back(obj);
  }
}
