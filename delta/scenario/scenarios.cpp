/*
 * scenarios.cpp
 *
 *  Created on: 19 Apr 2018
 *      Author: konstantinos
 */

#include <delta/scenario/scenarios.h>
#include <delta/scenario/configuration.h>
#include <delta/geometry/hardcoded/graphite.h>
#include <delta/geometry/hardcoded/hopper.h>
#include <delta/geometry/hardcoded/cube.h>
#include <delta/geometry/hardcoded/granulate.h>

void delta::world::scenarios::hopper(
	std::vector<delta::geometry::Object> &coarse,
	std::vector<delta::geometry::Object> &insitu,
	iREAL centre[3],
	int xzcuts,
	int ycuts,
	bool uniform,
	bool isSphere,
	int meshDensity,
	iREAL epsilon)
{
  //////////////////////////////////////////////////////
  /// HOPPER FLOW SCENARIO /////////////////////////////
  //////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////
  ////////HOPPER/////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
  iREAL hopperHatch = 0.05; iREAL hopperThickness = 0.005;
  iREAL hopperWidth = 0.20; iREAL hopperHeight = hopperWidth/1.5;

  std::array<iREAL, 3> position = {centre[0], centre[1], centre[2]};
  ///////////////////////////////////////////////////////////////////////////////
  ////////HOPPER/////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

  int hopperParticles = 0;

#if true
  iREAL _hopperHatch = 0.05; iREAL _hopperThickness = 0.005; int refinement = 0;

  iREAL pos[3] = {position[0], position[1], position[2]};
  delta::geometry::mesh::Mesh *geometry = delta::geometry::hardcoded::generateHopper(pos, hopperWidth, _hopperThickness, hopperHeight, _hopperHatch, refinement, 0.01);
  delta::geometry::Object objectHopper("hopper", 0, geometry, position, delta::geometry::material::MaterialType::WOOD, true, false, epsilon, {0,0,0}, {0,0,0});
  coarse.push_back(objectHopper);
  hopperParticles = 0;
#else
  int refinement = 3;
  std::vector<iREAL> xCoordinates, yCoordinates, zCoordinates;
  delta::geometry::mesh::Mesh *geometry = delta::geometry::hardcoded::generateHopper(centre, hopperWidth, hopperThickness, hopperHeight, hopperHatch, refinement, _minParticleDiam);
  hopperParticles = geometry->decomposeMeshIntoParticles(material, isObstacle, isFriction, _insitufineObjects);
#endif


  ///////////////////////////////////////////////////////////////////////////////////////////
  ////////FLOOR//////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  iREAL height = 0.05; iREAL width = 0.32;
  position = {centre[0], 0.35, centre[2]};

  iREAL po[3] = {position[0], position[1], position[2]};
  delta::geometry::mesh::Mesh *cube = delta::geometry::primitive::cube::generateHullCube(po, width, height, width, 0, 0, 0, 0);
  delta::geometry::Object objectFloor("cube", 0, cube, position, delta::geometry::material::MaterialType::WOOD, true, true, epsilon, {0,0,0}, {0,0,0});

  coarse.push_back(objectFloor);
  ///////////////////////////////////////////////////////////////////////////////////////////
  /////////FLOOR/////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////
  //////////PARTICLE GRID/////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  iREAL totalMass = 0.05;

  iREAL margin = (hopperThickness + epsilon) * 4;
  iREAL gridxyLength = hopperWidth-margin/2;

  //position is top of hopper
  pos[0] = (centre[0] - hopperWidth/2);
  pos[1] = centre[1] + hopperHeight/2;
  pos[2] = (centre[2] - hopperWidth/2);

  if(uniform)
  {
	delta::world::configuration::uniformlyDistributedTotalMass(insitu, pos, xzcuts, ycuts, gridxyLength, totalMass, hopperWidth, hopperParticles, epsilon, isSphere, meshDensity);
  }
  else
  {
	iREAL gridCellLength = ((iREAL)gridxyLength/(iREAL)xzcuts) - epsilon*4;
	delta::world::configuration::nonUniformlyDistributedTotalMass(insitu, pos, xzcuts, ycuts, gridxyLength, totalMass, hopperWidth, hopperParticles, epsilon, isSphere, gridCellLength, meshDensity);
  }

  //////////////////////////////////////////////////////
  /// HOPPER FLOW SCENARIO /////////////////////////////
  //////////////////////////////////////////////////////
}

void delta::world::scenarios::turbine(
	std::vector<delta::geometry::Object>& 	coarseObjects,
	bool 									isSphere,
	int 										meshDensity,
	iREAL 									epsilon)
{
  std::vector<delta::geometry::mesh::Mesh> meshes = delta::core::readGeometry("input/turbine.stl");

  std::array<iREAL, 3> 					centre 	= {0.5, 0.5, 0.5};
  std::array<iREAL, 3> 					linear 	= {0.0, 0.0, 0.0};
  std::array<iREAL, 3> 					angular 	= {2.0, 0.0, 0.0};

  printf("passed\n");
  delta::geometry::Object * object = new  delta::geometry::Object("mesh", 0, &meshes[0], centre,
										  delta::geometry::material::MaterialType::WOOD,
										  false, false, epsilon, linear, angular);
  coarseObjects.push_back(*object);

/*
  centre 	= {0.5, 0.5, 0.5};
  linear 	= {0.0, 0.0, 0.0};
  angular 	= {-1.0, 0.0, 0.0};

  delta::geometry::mesh::Mesh copy = meshes[0];
  object = new  delta::geometry::Object("mesh", 1, &copy, centre,
										delta::geometry::material::MaterialType::WOOD,
										false, false, epsilon, linear, angular);
  coarseObjects.push_back(*object);*/
}

void delta::world::scenarios::rotateParticle(
	std::vector<delta::geometry::Object>& 	coarseObjects,
	bool 									isSphere,
	int 										meshDensity,
	iREAL 									epsilon)
{
  //////////////////////////////////////////////////////
  /// Rotate SCENARIO
  //////////////////////////////////////////////////////

  std::array<iREAL, 3> centreArray = {0.5, 0.5, 0.5};
  std::array<iREAL, 3> linear = {0.0, 0.0, 0.0};

  if(isSphere)
  {
	delta::geometry::Object objectA("sphere", 0.01, 0, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {1,0,0});
	coarseObjects.push_back(objectA);
  } else {
	iREAL pos[3] = {centreArray[0], centreArray[1], centreArray[2]};
	delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::granulate::generateParticle(pos, 0.01*2, meshDensity);
	delta::geometry::Object objectA("granulate", 0, geometry, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {1,0,0});
	coarseObjects.push_back(objectA);
  }
}

void delta::world::scenarios::twoParticlesCrash(
	std::vector<delta::geometry::Object>& 	coarseObjects,
	bool 									isSphere,
	int 										meshDensity,
	iREAL 									epsilon)
{
  //////////////////////////////////////////////////////
  /// TWO PARTICLES CRASH SCENARIO
  //////////////////////////////////////////////////////

  std::array<iREAL, 3> centreArray = {0.2, 0.2, 0.2};
  std::array<iREAL, 3> linear = {0.1, 0.1, 0.1};
printf("entered\n");
  if(isSphere)
  {
	delta::geometry::Object objectA("sphere", 0.01, 0, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	coarseObjects.push_back(objectA);

	centreArray[0] = 0.8;
	centreArray[1] = 0.8;
	centreArray[2] = 0.8;

	linear[0] = -0.1;
	linear[1] = -0.1;
	linear[2] = -0.1;

	delta::geometry::Object objectB("sphere", 0.1, 1, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	coarseObjects.push_back(objectB);
  } else {
	iREAL rad = 0.01;

	centreArray[0] = 0.5-rad+epsilon;
	centreArray[1] = 0.5;
	centreArray[2] = 0.5;

	iREAL pos[3] = {centreArray[0], centreArray[1], centreArray[2]};
	delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::granulate::generateParticle(pos, rad*2, meshDensity);
	delta::geometry::Object objectA("granulate", 0, geometry, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});

	rad = 0.1;

	centreArray[0] = 0.5+rad-epsilon*2;
	centreArray[1] = 0.5;
	centreArray[2] = 0.5;

	linear[0] = -0.1;
	linear[1] = -0.1;
	linear[2] = -0.1;

	iREAL po[3] = {centreArray[0], centreArray[1], centreArray[2]};
	geometry = delta::geometry::primitive::granulate::generateParticle(po, rad*2, meshDensity);
	delta::geometry::Object objectB("granulate", 1, geometry, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});

	coarseObjects.push_back(objectA);
	coarseObjects.push_back(objectB);

	printf("%i triangles\n", objectB.getNumberOfTriangles());
  }
}

void delta::world::scenarios::friction(
	int 										scenario,
	bool 									isSphere,
	iREAL 									centre[3],
	int 										meshDensity,
	iREAL 									epsilon,
	std::vector<delta::geometry::Object> &	coarse)
{
  //////////////////////////////////////////////////////
  /// FRICTION SCENARIO
  //////////////////////////////////////////////////////

  //////FLOOR///////////////////////////////////////////////////////////////////
  iREAL height = 0.05; iREAL width = 0.35; iREAL rad = 0.0;
  std::array<iREAL, 3> position = {centre[0], centre[1], centre[2]};

  ////////////////////////////////////////////////////////////////////////////////
  iREAL pos[3] = {position[0], position[1], position[2]};
  delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::cube::generateHullCube(pos, width, height, width, 0, 0, 0, 0);
  delta::geometry::Object objectFloor("cube", 0, geometry, position, delta::geometry::material::MaterialType::WOOD, true, true, epsilon, {0,0,0}, {0,0,0});
  coarse.push_back(objectFloor);
  ////////////////////////////////////////////////////////////////////////////////
  if(scenario == 1)
  {
	  ///////////////////////
	  iREAL rad = 0.02;
	  std::array<iREAL, 3> position = {centre[0]+0.05, centre[1] + height, centre[2]};

	  if(isSphere){
		delta::geometry::Object objectA("sphere", rad, 1, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, {0,0,0}, {0,0,0});
		coarse.push_back(objectA);
	  } else {
		iREAL pos[3] = {position[0], position[1], position[2]};
		delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::cube::generateHullCube(pos, rad, rad, rad, 0, 0, 0, 0);
		delta::geometry::Object objectA("cube", 1, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, {0,0,0}, {0,0,0});
		coarse.push_back(objectA);
	  }
  } else if(scenario == 2)
  {
	iREAL rad = 0.02;
	std::array<iREAL, 3> linear = {0.5, 0.0, 0.0};
	std::array<iREAL, 3> position = {centre[0], centre[1] + height/2 + epsilon, centre[2]};

	if(isSphere){
		delta::geometry::Object objectA("sphere", rad, 1, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, linear, {0,0,0});
	  coarse.push_back(objectA);
	} else {
	  iREAL pos[3] = {position[0], position[1], position[2]};
	  delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::cube::generateHullCube(pos, rad, rad, rad, 0, 0, 0, 0);
	  delta::geometry::Object objectA("cube", 1, geometry, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, linear, {0,0,0});
	  coarse.push_back(objectA);
	}
  } else if(scenario == 3)
  {
	std::array<iREAL, 3> angular = {5.0, 0, 0};
	std::array<iREAL, 3> position = {centre[0], centre[1] + height/2 + centre[0] + epsilon, centre[2]};
	iREAL rad = 0.02;

	if(isSphere){
	  delta::geometry::Object objectA("sphere", rad, 1, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, {0,0,0}, angular);
	  coarse.push_back(objectA);
	} else {
	  iREAL pos[3] = {position[0], position[1], position[2]};
	  delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::cube::generateHullCube(pos, rad, rad, rad, 0, 0, 0, 0);
	  delta::geometry::Object objectA("cube", 1, geometry, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, {0,0,0}, angular);
	  coarse.push_back(objectA);
	}
  }
  //////////////////////////////////////////////////////
  /// END | FRICTION SCENARIO
  //////////////////////////////////////////////////////
}

void delta::world::scenarios::freeFall(
	int 										scenario,
	bool 									isSphere,
	iREAL 									centre[3],
	int 										meshDensity,
	iREAL 									epsilon,
	std::vector<delta::geometry::Object> &	fine)
{
  //////////////////////////////////////////////////////
  /// FREEFALL AND BLACKHOLE SCENARIO
  //////////////////////////////////////////////////////

  //iREAL particleDiameter = (_minParticleDiam + (_maxParticleDiam-_minParticleDiam) * (static_cast<iREAL>(rand()) / static_cast<iREAL>(RAND_MAX))) / std::sqrt(DIMENSIONS);
  //int particleid = deployBox(vertex, 0, 0, _centreAsArray, particleDiameter/2, particleDiameter/2, 0, 1.0/8.0, 1.0/8.0, _epsilon, material, isFriction, isObstacle);

  iREAL rad = 0.01;
  std::array<iREAL, 3> linear = {0, 0, 0};
  std::array<iREAL, 3> position = {centre[0], 0.8, centre[2]};

  if(scenario == 1)
  {
	linear[0] = static_cast<iREAL>( rand() ) / static_cast<iREAL>(RAND_MAX);
	linear[1] = static_cast<iREAL>( rand() ) / static_cast<iREAL>(RAND_MAX);
	linear[2] = static_cast<iREAL>( rand() ) / static_cast<iREAL>(RAND_MAX);
  }

  //Object ISSUE: here Object particles number has to be incremental with deployment in non-insitu enviroment
  if(isSphere){
	delta::geometry::Object objectA("sphere", rad, 0, position, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	fine.push_back(objectA);
  } else {
	iREAL pos[3] = {position[0], position[1], position[2]};
	delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::cube::generateHullCube(pos, rad, rad, rad, 0, 0, 0, 0);
	delta::geometry::Object objectA("cube", 0, geometry, position, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	fine.push_back(objectA);
  }
  //////////////////////////////////////////////////////
  /// END | FREEFALL AND BLACKHOLE SCENARIO
  //////////////////////////////////////////////////////
}

void delta::world::scenarios::nuclear(
	int 										scenario,
	bool 									isSphere,
	iREAL 									centre[3],
	int 										meshDensity,
	iREAL 									epsilon,
	std::vector<delta::geometry::Object> &	coarse,
	std::vector<delta::geometry::Object> &	fine)
{

  //////////////////////////////////////////////////////
  /// NUCLEAR ARRAY
  //////////////////////////////////////////////////////
  //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
  iREAL height = 0.05; iREAL width = 0.30;
  std::array<iREAL, 3> position = {centre[0], centre[1], centre[2]};

  iREAL pos[3] = {position[0], position[1], position[2]};
  delta::geometry::mesh::Mesh *geometry = delta::geometry::primitive::cube::generateHullCube(pos, width, height, width, 0, 0, 0, 0);
  delta::geometry::Object objectFloor("cube", 0, geometry, position, delta::geometry::material::MaterialType::GOLD, true, true, epsilon, {-1.0, 0, 0}, {0,0,0});
  coarse.push_back(objectFloor);
  //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////

  /////////FINE GRID///////////////////////////////
  pos[0] = width;
  pos[1] = position[1] + height/2;
  pos[2] = width;

  if(scenario == -1)
  {
	delta::world::configuration::loadNuclearGeometry(pos, width, 1, epsilon, fine);
  } else if(scenario == 0)
  {
	delta::world::configuration::loadNuclearGeometry(pos, width, 2, epsilon, fine);
  }
  else if(scenario > 0)
  {
	if(scenario == 1)
	{
	  //nuclear deck 1s
	  delta::world::configuration::makeBrickGrid(pos, 0.10, 1, 0.1, 1, epsilon, fine);
	}else if(scenario == 4)
	{
	  //nuclear deck 4s
	  delta::world::configuration::makeBrickGrid(pos, 0.10, 2, 0.1, 1, epsilon, fine);
	}else if(scenario == 32)
	{
	  //nuclear deck 32s
	  delta::world::configuration::makeBrickGrid(pos, 0.15, 4, 0.1, 2, epsilon, fine);
	}
	else if(scenario == 64)
	{
	  //nuclear deck 64
	  delta::world::configuration::makeBrickGrid(pos, 0.15, 4, 0.4, 4, epsilon, fine);
	}
	else if(scenario == 256)
	{
	  //nuclear deck 256
	  delta::world::configuration::makeBrickGrid(pos, 0.15, 10, 0.08, 4, epsilon, fine);
	}
  }
}
