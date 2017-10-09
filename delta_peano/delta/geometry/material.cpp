/*
 * material.cpp
 *
 *  Created on: 23 Mar 2017
 *      Author: konstantinos
 */

#include "material.h"

//std::map<delta::geometry::material::MaterialType, delta::geometry::material::MaterialDensity> delta::geometry::material::materialToDensitymap;

void delta::geometry::material::materialInit()
{

/*
  InterfaceTri woodwoodTri = {0.8, 400};
  InterfaceTri goldgoldTri = {0.8, 400};
  InterfaceTri graphitegraphiteTri = {0.8, 400};
  InterfaceTri woodgoldTri = {0.8, 400};
  InterfaceTri graphitewoodTri = {0.8, 400};
  InterfaceTri graphitegoldTri = {0.8, 400};

  InterfaceSph woodwoodSph = {0.8, 0.05, 0.005};
  InterfaceSph goldgoldSph = {0.8, 0.5, 0.005};
  InterfaceSph graphitegraphiteSph = {0.8, 0.5, 0.005};
  InterfaceSph woodgoldSph = {0.8, 0.05, 0.005};
  InterfaceSph graphitewoodSph = {0.8, 0.5, 0.005};
  InterfaceSph graphitegoldSph = {0.8, 0.5, 0.005};

  interfaceTrimap.insert(std::pair<int,InterfaceTri>(getInterfaceType(
                      int(MaterialType::WOOD), int(MaterialType::WOOD)), woodwoodTri));
  interfaceTrimap.insert(std::pair<int,InterfaceTri>(getInterfaceType(
                      int(MaterialType::GOLD), int(MaterialType::GOLD)), goldgoldTri));
  interfaceTrimap.insert(std::pair<int,InterfaceTri>(getInterfaceType(
                      int(MaterialType::GRAPHITE), int(MaterialType::GRAPHITE)), graphitegraphiteTri));
  interfaceTrimap.insert(std::pair<int,InterfaceTri>(getInterfaceType(
                      int(MaterialType::WOOD), int(MaterialType::GOLD)), woodgoldTri));
  interfaceTrimap.insert(std::pair<int,InterfaceTri>(getInterfaceType(
                      int(MaterialType::GRAPHITE), int(MaterialType::WOOD)), graphitewoodTri));
  interfaceTrimap.insert(std::pair<int,InterfaceTri>(getInterfaceType(
                      int(MaterialType::GRAPHITE), int(MaterialType::GOLD)), graphitegoldTri));

  interfaceSphmap.insert(std::pair<int,InterfaceSph>(getInterfaceType(
                      int(MaterialType::WOOD), int(MaterialType::WOOD)), woodwoodSph));
  interfaceSphmap.insert(std::pair<int,InterfaceSph>(getInterfaceType(
                      int(MaterialType::GOLD), int(MaterialType::GOLD)), goldgoldSph));
  interfaceSphmap.insert(std::pair<int,InterfaceSph>(getInterfaceType(
                      int(MaterialType::GRAPHITE), int(MaterialType::GRAPHITE)), graphitegraphiteSph));
  interfaceSphmap.insert(std::pair<int,InterfaceSph>(getInterfaceType(
                      int(MaterialType::WOOD), int(MaterialType::GOLD)), woodgoldSph));
  interfaceSphmap.insert(std::pair<int,InterfaceSph>(getInterfaceType(
                      int(MaterialType::GRAPHITE), int(MaterialType::WOOD)), graphitewoodSph));
  interfaceSphmap.insert(std::pair<int,InterfaceSph>(getInterfaceType(
                      int(MaterialType::GRAPHITE), int(MaterialType::GOLD)), graphitegoldSph));

  densitymap.insert(std::pair<int, int>(int(MaterialType::WOOD), 1000));
  densitymap.insert(std::pair<int, int>(int(MaterialType::GOLD), 20000));
  densitymap.insert(std::pair<int, int>(int(MaterialType::GRAPHITE), 20000));

  printf("RHOMAT:%i \n", delta::collision::material::densitymap[int(delta::collision::material::MaterialType::WOOD)]);

  delta::collision::material::InterfaceTri interface = delta::collision::material::interfaceTrimap[int(delta::collision::material::getInterfaceType(1, 1))];
  printf("friction:%f \n", interface.SFRICTION);*/
}

int delta::geometry::material::getInterfaceType(int materialA, int materialB)
{
  return materialA*materialB;
}


