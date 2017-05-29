/*
 * assembly.cpp
 *
 *  Created on: 13 Feb 2017
 *      Author: user
 */

#include "assembly.h"


double delta::world::assembly::getxDiscritizationLength(double length, int number)
{
	return length/number;
}

std::vector<std::array<double, 3>> delta::world::assembly::array1d(double position[3], double xAxisLength, int partsNo)
{
	std::vector<std::array<double, 3>> array;
	std::array<double, 3> point = {0.0, 0.0, 0.0};

	double length = getxDiscritizationLength(xAxisLength, partsNo);

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

std::vector<std::array<double, 3>> delta::world::assembly::array2d(double position[3], double xyAxisLength, int partsNo)
{
	std::vector<std::array<double, 3>> array;

	double length = getxDiscritizationLength(xyAxisLength, partsNo);

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<double, 3>> tmp = delta::world::assembly::array1d(position, xyAxisLength, partsNo);
		for(std::vector<std::array<double, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[2] += length;
	}

	return array;
}

std::vector<std::array<double, 3>> delta::world::assembly::array3d(double position[3], double xyzAxisLength, int partsNo)
{
	std::vector<std::array<double, 3>> array;

	double length = getxDiscritizationLength(xyzAxisLength, partsNo);

  double resetx = position[0];
	double resetz = position[2];

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<double, 3>> tmp = delta::world::assembly::array2d(position, xyzAxisLength, partsNo);
		for(std::vector<std::array<double, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
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

std::vector<std::array<double, 3>> delta::world::assembly::array3d(double position[3], double xyzAxisLength, int partsXYZNo, double yAxisLength, int partsYNo)
{
	std::vector<std::array<double, 3>> array;

	double lengthY =  getxDiscritizationLength(yAxisLength, partsYNo);

	for(int i=0;i<partsYNo;i++)
	{
		std::vector<std::array<double, 3>> tmp = delta::world::assembly::array2d(position, xyzAxisLength, partsXYZNo);
		for(std::vector<std::array<double, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
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


std::vector<std::array<double, 3>> delta::world::assembly::getGridArrayList(double position[3], int xzcuts, int ycuts, double width)
{
  return delta::world::assembly::array3d(position, width, xzcuts, width, ycuts);
}

void delta::world::assembly::uniSphereRadius(double totalMass, delta::collision::material::MaterialType material, std::vector<double>  &rad,
                                              std::vector<std::array<double, 3>> &particleGrid, double &minParticleDiam, double &maxParticleDiam)
{
  double massPerParticle = totalMass/(double)particleGrid.size();
  double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  minParticleDiam = radius;
  maxParticleDiam = radius;

  for(int i=0; i<particleGrid.size(); i++)
  {
    rad.push_back(radius);
  }
}

void delta::world::assembly::uniMeshGeometry(double totalMass,
                                                delta::collision::material::MaterialType material,
                                                int noPointsPerParticle,
                                                std::vector<double>  &rad,
                                                std::vector<std::array<double, 3>> &particleGrid,
                                                std::vector<std::vector<double>>  &xCoordinatesArray,
                                                std::vector<std::vector<double>>  &yCoordinatesArray,
                                                std::vector<std::vector<double>>  &zCoordinatesArray)
{
  double massPerParticle = totalMass/(double)particleGrid.size();
  double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  double reMassTotal = 0;
  //double masssphere = 0;

  double position[3];
  for(auto i:particleGrid)
  {
    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
    delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, noPointsPerParticle);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, material);
    //double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
    //double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  }

  double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<double, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::primitives::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);

    //double mt = delta::primitives::properties::computeMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], delta::collision::material::MaterialType::WOOD);
    //double vt = delta::primitives::properties::computeVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    //double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);
}

void delta::world::assembly::nonUniSphereRadius(double totalMass,
    delta::collision::material::MaterialType material,
    double subcellx,
    std::vector<double>  &rad,
    std::vector<std::array<double, 3>> &particleGrid,
    double &minParticleDiam, double &maxParticleDiam)
{
  double massPerParticle = totalMass/(double)particleGrid.size();
  double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  double reMassTotal = 0;

  iREAL mindiam = radius/2;
  iREAL maxdiam = radius*2;

  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  for(int i=0; i<particleGrid.size(); i++)
  {
    double particleDiameter = mindiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(maxdiam-mindiam)));
    rad.push_back(particleDiameter/2);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(particleDiameter/2,3) * int(delta::collision::material::materialToDensitymap.find(material)->second); //volume * mass
  }

  //get total mass
  printf("TOTAL REMASS:%f\n", reMassTotal);

  double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(auto i:rad)
  {
    i = i* rescale;

    if(i*2<minParticleDiam)
      minParticleDiam = i*2;
    if(i*2>maxParticleDiam)
      maxParticleDiam = i*2;

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(i,3) * int(delta::collision::material::materialToDensitymap.find(material)->second); //volume * mass
  }
  printf("RESCALE:%f\n", rescale);
  printf("TOTAL REREMASS:%f\n", reMassTotal);

}

void delta::world::assembly::nonUniMeshGeometry(double totalMass,
                                                delta::collision::material::MaterialType material,
                                                double subcellx, int noPointsPerParticle,
                                                std::vector<double>  &rad,
                                                std::vector<std::array<double, 3>> &particleGrid,
                                                double &minParticleDiam, double &maxParticleDiam,
                                                std::vector<std::vector<double>>  &xCoordinatesArray,
                                                std::vector<std::vector<double>>  &yCoordinatesArray,
                                                std::vector<std::vector<double>>  &zCoordinatesArray)
{
  double massPerParticle = totalMass/(double)particleGrid.size();
  double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  double reMassTotal = 0;
  double masssphere = 0;

  iREAL position[3];
  //std::vector<double> rad;
  iREAL mindiam = radius*2/2;
  iREAL maxdiam = radius*2;
  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  for(auto i:particleGrid)
  {
    std::vector<double>  xCoordinates, yCoordinates, zCoordinates;

    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    double particleDiameter = mindiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(maxdiam-mindiam)));

    rad.push_back(particleDiameter/2);
    radius = particleDiameter/2;

    delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, noPointsPerParticle);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, material);
    //double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
    //double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  }

  double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  //reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<double, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::primitives::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);

    rad[j] = rad[j] * rescale;
    if(rad[j]<minParticleDiam)
      minParticleDiam = rad[j];
    if(rad[j]>maxParticleDiam)
      maxParticleDiam = rad[j];

    //double mt = delta::primitives::properties::computeMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], material);
    //double vt = delta::primitives::properties::computeVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    //double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);
}

void delta::world::assembly::makeLoadNuclearGeometry(double position[3],
                                                    std::vector<std::array<double, 3>> &particleGrid,
                                                    std::vector<std::string> &componentGrid,
                                                    std::vector<double> &radius,
                                                    double &minParticleDiam,
                                                    double &maxParticleDiam)
{
  std::vector<double> xCoordinates, yCoordinates, zCoordinates;

  //measurements
  iREAL pos[] = {0,0,0};
  delta::primitives::graphite::generateBrickFB(pos, 1, xCoordinates, yCoordinates, zCoordinates);
  double width = delta::primitives::properties::computeXZWidth(xCoordinates, yCoordinates, zCoordinates);
  double height = delta::primitives::properties::computeYw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

  //read nuclear graphite schematics
  std::vector<std::vector<std::string>> compoGrid;
  delta::core::parseModelGridSchematics("input/nuclear_core", compoGrid, componentGrid);

  ///place components of 2d array structure
  int elements = 46;
  double length = 1;
  double xwCell = delta::world::assembly::getxDiscritizationLength(length, elements);
  double halfxwCell = xwCell/2;

  double scaleDownPercentage = 0;
  if(xwCell < width)
  {//brick is bigger than grid
    double excess = width - xwCell;
    scaleDownPercentage = ((width - excess)/width);
    printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scaleDownPercentage, width, xwCell, excess);
  }

  position[0] = halfxwCell; position[1] += ((height/2)*scaleDownPercentage); position[2] = halfxwCell;
  particleGrid = delta::world::assembly::array2d(position, length, elements);
  for(unsigned i=0;i<particleGrid.size();i++) radius.push_back(0);
}

void delta::world::assembly::makeFullBrickFBGrid(double position[3],
    double length,
    double elements,
    std::vector<std::array<double, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<double> &radius,
    double &minParticleDiam,
    double &maxParticleDiam)
{
  std::vector<double>  xCoordinates, yCoordinates, zCoordinates;

  //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
  //measurements
  double pos[3]; pos[0] = pos[1] = pos[2] = 0;
  delta::primitives::graphite::generateBrickFB(pos, 1, xCoordinates, yCoordinates, zCoordinates);
  double width = delta::primitives::properties::computeXZWidth(xCoordinates, yCoordinates, zCoordinates);
  double height = delta::primitives::properties::computeYw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///place components of 2d array structure
  double xwCell = delta::world::assembly::getxDiscritizationLength(length, elements);
  double halfxwCell = xwCell/2;

  double scalePercentage;
  if(xwCell < width)
  {//brick is bigger than grid
    double excess = width - xwCell;
    scalePercentage = ((width - excess)/width);
    //printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  } else {
    double excess = xwCell - width;
    scalePercentage = 1+((xwCell - excess)/xwCell);
    //printf("SCALE UP: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  }

  width *= scalePercentage;
  height *= scalePercentage;

  position[0] = (position[0]-length/2)+halfxwCell; position[1] += (height/2); position[2] = (position[2]-length/2)+halfxwCell;
  particleGrid = delta::world::assembly::array3d(position, length, elements);
  for(unsigned i=0; i<particleGrid.size(); i++) {
    radius.push_back(scalePercentage);
    componentGrid.push_back("FB");
  }
}
