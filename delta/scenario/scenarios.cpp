/*
 * scenarios.cpp
 *
 *  Created on: 19 Apr 2018
 *      Author: konstantinos
 */

#include <delta/scenario/scenarios.h>
#include <delta/scenario/configuration.h>
#include <delta/geometry/hardcoded/graphite.h>

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
  delta::geometry::Object objectHopper("hopper", 0, position, delta::geometry::material::MaterialType::WOOD, true, false, epsilon, {0,0,0}, {0,0,0});
  objectHopper.generateMesh(hopperWidth, hopperHeight, hopperWidth, 0, 0, 0, hopperWidth, meshDensity);
  coarse.push_back(objectHopper);
  hopperParticles = 0;
#else
  int refinement = 3;
  std::vector<iREAL> xCoordinates, yCoordinates, zCoordinates;
  delta::geometry::hopper::generateHopper(centre, hopperWidth, hopperThickness, hopperHeight, hopperHatch, refinement, _minParticleDiam, xCoordinates, yCoordinates, zCoordinates);
  hopperParticles = decomposeMeshIntoParticles(xCoordinates, yCoordinates, zCoordinates, material, isObstacle, isFriction, _insitufineObjects);
#endif


  ///////////////////////////////////////////////////////////////////////////////////////////
  ////////FLOOR//////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  iREAL height = 0.05; iREAL width = 0.32;
  position = {centre[0], 0.35, centre[2]};

  delta::geometry::Object objectFloor("cube", 0, position, delta::geometry::material::MaterialType::WOOD, true, true, epsilon, {0,0,0}, {0,0,0});
  objectFloor.generateMesh(width, height, width, 0, 0, 0, width, meshDensity);
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
  iREAL pos[3] = {(centre[0] - hopperWidth/2), centre[1] + hopperHeight/2, (centre[2] - hopperWidth/2)};

  if(uniform)
  {
	insitu = delta::world::configuration::uniformlyDistributedTotalMass(pos, xzcuts, ycuts, gridxyLength, totalMass, hopperWidth, hopperParticles, epsilon, isSphere, meshDensity);
  }
  else
  {
	iREAL gridCellLength = ((iREAL)gridxyLength/(iREAL)xzcuts) - epsilon*4;
	insitu = delta::world::configuration::nonUniformlyDistributedTotalMass(pos, xzcuts, ycuts, gridxyLength, totalMass, hopperWidth, hopperParticles, epsilon, isSphere, gridCellLength, meshDensity);
  }

  //////////////////////////////////////////////////////
  /// HOPPER FLOW SCENARIO /////////////////////////////
  //////////////////////////////////////////////////////
}

std::vector<delta::geometry::Object> delta::world::scenarios::rotateParticle(
	bool 	isSphere,
	int 		meshDensity,
	iREAL 	epsilon)
{
  //////////////////////////////////////////////////////
  /// Rotate SCENARIO
  //////////////////////////////////////////////////////

  std::vector<delta::geometry::Object> _coarseObjects;

  std::array<iREAL, 3> centreArray = {0.5, 0.5, 0.5};
  std::array<iREAL, 3> linear = {0.0, 0.0, 0.0};

  if(isSphere)
  {
	delta::geometry::Object objectA("sphere", 0.01, 0, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {1,0,0});
	_coarseObjects.push_back(objectA);
  } else {
	delta::geometry::Object objectA("granulate", 0, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {1,0,0});
	objectA.generateMesh(0,0,0,0,0,0,meshDensity, 0.01);
	_coarseObjects.push_back(objectA);
  }
  return _coarseObjects;
}

std::vector<delta::geometry::Object> delta::world::scenarios::twoParticlesCrash(
	bool 	isSphere,
	int 		meshDensity,
	iREAL 	epsilon)
{
  //////////////////////////////////////////////////////
  /// TWO PARTICLES CRASH SCENARIO
  //////////////////////////////////////////////////////

  std::vector<delta::geometry::Object> _coarseObjects;

  std::array<iREAL, 3> centreArray = {0.2, 0.2, 0.2};
  std::array<iREAL, 3> linear = {0.1, 0.1, 0.1};

  if(isSphere)
  {
	delta::geometry::Object objectA("sphere", 0.01, 0, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	_coarseObjects.push_back(objectA);

	centreArray[0] = 0.8;
	centreArray[1] = 0.8;
	centreArray[2] = 0.8;

	linear[0] = -0.1;
	linear[1] = -0.1;
	linear[2] = -0.1;

	delta::geometry::Object objectB("sphere", 0.1, 1, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	_coarseObjects.push_back(objectB);
  } else {
	iREAL rad = 0.01;

	centreArray[0] = 0.5-rad+epsilon;
	centreArray[1] = 0.5;
	centreArray[2] = 0.5;
	delta::geometry::Object objectA("granulate", 0, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	objectA.generateMesh(0,0,0,0,0,0,meshDensity, rad);

	rad = 0.1;

	centreArray[0] = 0.5+rad-epsilon*2;
	centreArray[1] = 0.5;
	centreArray[2] = 0.5;

	linear[0] = -0.1;
	linear[1] = -0.1;
	linear[2] = -0.1;

	delta::geometry::Object objectB("granulate", 1, centreArray, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	objectB.generateMesh(0,0,0,0,0,0,meshDensity, rad);

	_coarseObjects.push_back(objectA);
	_coarseObjects.push_back(objectB);
  }
  return _coarseObjects;

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
  delta::geometry::Object objectFloor("cube", 0, position, delta::geometry::material::MaterialType::WOOD, true, true, epsilon, {0,0,0}, {0,0,0});
  objectFloor.generateMesh(width, height, width, 0, 0, 0, rad, meshDensity);
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
		delta::geometry::Object objectA("cube", 1, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, {0,0,0}, {0,0,0});
		objectA.generateMesh(rad, rad, rad, 0, 0, 0, rad, meshDensity);
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
		delta::geometry::Object objectA("cube", 1, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, linear, {0,0,0});
	  objectA.generateMesh(rad, rad, rad, 0, 0, 0, rad, meshDensity);
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
		delta::geometry::Object objectA("cube", 1, position, delta::geometry::material::MaterialType::WOOD, false, true, epsilon, {0,0,0}, angular);
		objectA.generateMesh(rad, rad, rad, 0, 0, 0, rad, meshDensity);
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
	delta::geometry::Object objectA("cube", 0, position, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, linear, {0,0,0});
	objectA.generateMesh(rad,rad,rad,0,0,0,rad,meshDensity);
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

  delta::geometry::Object objectFloor("cube", 0, position, delta::geometry::material::MaterialType::GOLD, true, true, epsilon, {-1.0, 0, 0}, {0,0,0});
  objectFloor.generateMesh(width, height, width, 0, 0, 0, width, meshDensity);
  coarse.push_back(objectFloor);
  //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////

  /////////FINE GRID///////////////////////////////
  iREAL pos[3] = {width, position[1] + height/2, width};

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
