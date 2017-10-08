#include "dem/mappings/CreateGrid.h"

#include "delta/geometry/cubes.h"
#include "delta/geometry/granulates.h"
#include "delta/geometry/properties.h"
#include "delta/geometry/hopper.h"
#include "delta/geometry/blender.h"
#include <delta/geometry/graphite.h>
#include <delta/geometry/surface.h>
#include <delta/world/assembly.h>

#include "peano/grid/aspects/VertexStateAnalysis.h"

#include <string>
#include <iostream>
#include <cmath>
#include <ctime>

#define epsilon 0.002

peano::CommunicationSpecification   dem::mappings::CreateGrid::communicationSpecification() const {
	return peano::CommunicationSpecification(
	    peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,
	    peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexLastTimeSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexFirstTimeSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::enterCellSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::leaveCellSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::ascendSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::descendSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                  dem::mappings::CreateGrid::_log( "dem::mappings::CreateGrid" );
dem::mappings::CreateGrid::Scenario  dem::mappings::CreateGrid::_scenario[4];
double                               dem::mappings::CreateGrid::_maxH;
double                               dem::mappings::CreateGrid::_minComputeDomain[3];
double                               dem::mappings::CreateGrid::_maxComputeDomain[3];
double                               dem::mappings::CreateGrid::_minParticleDiam;
double                               dem::mappings::CreateGrid::_maxParticleDiam;
dem::mappings::CreateGrid::GridType  dem::mappings::CreateGrid::_gridType;
double                               dem::mappings::CreateGrid::_centreAsArray[3];

double								                dem::mappings::CreateGrid::_epsilon;
int 								                  dem::mappings::CreateGrid::_noPointsPerParticle;
bool                                 dem::mappings::CreateGrid::_isSphere;
bool                                 dem::mappings::CreateGrid::_deployInsitu;

std::vector<delta::world::object>   dem::mappings::CreateGrid::_coarseObjects;
std::vector<delta::world::object>   dem::mappings::CreateGrid::_fineObjects;

std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_coarsePositionArray;
std::vector<std::string>                              dem::mappings::CreateGrid::_coarseComponentArray;
std::vector<delta::geometry::material::MaterialType>  dem::mappings::CreateGrid::_coarseMaterialArray;
std::vector<double>                                   dem::mappings::CreateGrid::_coarseRadiusArray;
std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_coarseXYZDimensionsArray;
std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_coarseLinearVelocityArray;
std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_coarseAngularVelocityArray;
std::vector<double>                                   dem::mappings::CreateGrid::_coarseisFrictionArray;
std::vector<double>                                   dem::mappings::CreateGrid::_coarseisObstacleArray;

std::vector<int>                                      dem::mappings::CreateGrid::_particleIDArray;
std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_particleGridArray;
std::vector<std::string>                              dem::mappings::CreateGrid::_componentGridArray;
std::vector<delta::geometry::material::MaterialType>  dem::mappings::CreateGrid::_materialArray;
std::vector<double>                                   dem::mappings::CreateGrid::_radArray;
std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_xyzDimensionsArray;
std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_linearVelocityArray;
std::vector<std::array<double, 3>>                    dem::mappings::CreateGrid::_angularVelocityArray;
std::vector<bool>                                     dem::mappings::CreateGrid::_isFrictionArray;
std::vector<bool>                                     dem::mappings::CreateGrid::_isObstacleArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_xCoordinatesArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_yCoordinatesArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_zCoordinatesArray;

std::vector<std::vector<double>>     dem::mappings::CreateGrid::_centreOfMassArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_inertiaArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_inverseArray;
std::vector<double>                  dem::mappings::CreateGrid::_massArray;


void dem::mappings::CreateGrid::setScenario(
    Scenario scenario[4],
    double maxH,
    double particleDiamMin,
    double particleDiamMax,
    GridType gridType,
    int noPointsPerGranulate)
{
	_scenario[0]        	= scenario[0];
  _scenario[1]          = scenario[1];
  _scenario[2]          = scenario[2];
  _scenario[3]          = scenario[3];
	_maxH            		  = maxH;
	_minParticleDiam 		  = particleDiamMin;
	_maxParticleDiam 		  = particleDiamMax;
	_gridType        		  = gridType;
	_epsilon 		 		      = epsilon;
	_noPointsPerParticle	= noPointsPerGranulate;
}

void dem::mappings::CreateGrid::beginIteration(
		dem::State&  solverState)
{
  logTraceInWith1Argument( "beginIteration(State)", solverState );
  dem::ParticleHeap::getInstance().setName( "particle-heap" );
  dem::DEMDoubleHeap::getInstance().setName( "geometry-heap" );

  logInfo( "beginIteration()", "maxH=" << _maxH );

  srand (time(NULL));

  _numberOfParticles = 0;
  _numberOfTriangles = 0;
  _numberOfObstacles = 0;

  //delta::world::object object;

  _isSphere = (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere ||
               dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::none);


  iREAL centre[3] = {0.5, 0.5, 0.5};


  _deployInsitu = false;

  //////////////////////////////////////////////////////
  /// NUCLEAR ARRAY
  //////////////////////////////////////////////////////
  if(_scenario[1] == nuclear)
  {
    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
    double height = 0.05; double width = 0.30;
    std::array<double, 3> xyzDimensions = {width, height, width};
    std::array<double, 3> position = {centre[0], centre[1], centre[2]};
    std::array<double, 3> linear = {-1.0, 0, 0};
    std::array<double, 3> angular = {0, 0, 0};
    delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::GOLD;
    bool isFriction = true;
    bool isObstacle = true;

    _coarseXYZDimensionsArray.push_back(xyzDimensions);
    _coarseMaterialArray.push_back(material);
    _coarseisFrictionArray.push_back(isFriction);
    _coarseisObstacleArray.push_back(isObstacle);
    _coarseRadiusArray.push_back(0);
    _coarseLinearVelocityArray.push_back(linear);
    _coarseAngularVelocityArray.push_back(angular);
    _coarsePositionArray.push_back(position);
    _coarseComponentArray.push_back("cube");

    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////

    //if(!assertCoarse()) return;

    _deployInsitu = true;
    /////////FINE GRID///////////////////////////////
    iREAL pos[3];
    pos[0] = _coarsePositionArray[0][0];
    pos[1] += _coarseXYZDimensionsArray[0][1]/2;
    pos[2] = _coarsePositionArray[0][2];
    if(_scenario[0] == sla)
    {
      delta::world::assembly::loadNuclearGeometry(pos, width, 1, _particleGridArray, _componentGridArray, _radArray, _minParticleDiam, _maxParticleDiam);
    } else if(_scenario[0] == dla)
    {
      delta::world::assembly::loadNuclearGeometry(pos, width, 2, _particleGridArray, _componentGridArray, _radArray, _minParticleDiam, _maxParticleDiam);
    }
    else if(_scenario[0] == nuclearDeck)
    {
      if(_scenario[2] == n1)
      {
        //nuclear deck 1s
        delta::world::assembly::makeBrickGrid(pos, 0.10, 1, 0.1, 1, _particleGridArray, _componentGridArray, _radArray, _minParticleDiam, _maxParticleDiam);
      }else if(_scenario[2] == n4)
      {
        //nuclear deck 4s
        delta::world::assembly::makeBrickGrid(pos, 0.10, 2, 0.1, 1, _particleGridArray, _componentGridArray, _radArray, _minParticleDiam, _maxParticleDiam);
      }else if(_scenario[2] == n32)
      {
        //nuclear deck 32s
        delta::world::assembly::makeBrickGrid(pos, 0.15, 4, 0.1, 2, _particleGridArray, _componentGridArray, _radArray, _minParticleDiam, _maxParticleDiam);
      }
      else if(_scenario[2] == n64)
      {
        //nuclear deck 64
        delta::world::assembly::makeBrickGrid(pos, 0.15, 4, 0.4, 4, _particleGridArray, _componentGridArray, _radArray, _minParticleDiam, _maxParticleDiam);
      }
      else if(_scenario[2] == n256)
      {
        //nuclear deck 256
        delta::world::assembly::makeBrickGrid(pos, 0.15, 10, 0.08, 4, _particleGridArray, _componentGridArray, _radArray, _minParticleDiam, _maxParticleDiam);
      }
    }
    for(int i=0; i<1000; i++)
    {
      _materialArray.push_back(delta::geometry::material::MaterialType::GOLD);
      _isFrictionArray.push_back(false);
      _isObstacleArray.push_back(false);

      std::array<double, 3> angular = {0, 0, 0};
      _angularVelocityArray.push_back(angular);

      std::array<double, 3> linear = {0, 0, 0};
      _linearVelocityArray.push_back(linear);
    }
  } else if(_scenario[1] == hopper)
  {
    //////////////////////////////////////////////////////
    /// HOPPER FLOW SCENARIO /////////////////////////////
    //////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////
    ////////HOPPER/////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    double _hopperHatch = 0.05; double _hopperThickness = 0.005;

    double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5;
    std::array<double, 3> xyzDimensions = {_hopperWidth, _hopperHeight, _hopperWidth};

    std::array<double, 3> linear = {0, 0, 0};
    std::array<double, 3> angular = {0, 0, 0};
    std::array<double, 3> position = {centre[0], centre[1], centre[2]};

    bool isObstacle = true;
    auto material = delta::geometry::material::MaterialType::WOOD;

    /*
    _coarseXYZDimensionsArray.push_back(xyzDimensions);
    _coarseMaterialArray.push_back(material);
    _coarseisFrictionArray.push_back(true);
    _coarseisObstacleArray.push_back(isObstacle);
    _coarseRadiusArray.push_back(0);

    _coarseLinearVelocityArray.push_back(linear);
    _coarseAngularVelocityArray.push_back(angular);

    _coarsePositionArray.push_back(position);
    _coarseComponentArray.push_back("hopper");//hopper creations
    */

    int refinement = 1;

    std::vector<double> xCoordinates, yCoordinates, zCoordinates;
    delta::geometry::hopper::generateHopper(centre, xyzDimensions[0], _hopperThickness, xyzDimensions[1], _hopperHatch, refinement, _maxH, xCoordinates, yCoordinates, zCoordinates);

    //decompose hopper into triangle particles
    double centerOfMass[3], inertia[9], inverse[9], mass;

    int particles = decomposeMeshIntoParticles(xCoordinates, yCoordinates, zCoordinates, material, isObstacle, mass, centerOfMass, inertia, inverse);

    for(int j=0; j<particles; j++)
    {
      _centreOfMassArray.push_back({centerOfMass[0],centerOfMass[1],centerOfMass[2]});
      _inertiaArray.push_back({inertia[0],inertia[1],inertia[2],inertia[3],inertia[4],inertia[5],inertia[6],inertia[7],inertia[8]});
      _inverseArray.push_back({inverse[0],inverse[1],inverse[2],inverse[3],inverse[4],inverse[5],inverse[6],inverse[7],inverse[8]});
      _massArray.push_back(mass);

      _componentGridArray.push_back("partialmesh");
      _isObstacleArray.push_back(isObstacle);
      _isFrictionArray.push_back(false);
      _materialArray.push_back(material);
      _radArray.push_back(0.0);

      _particleIDArray.push_back(_numberOfParticles);

      std::array<iREAL,3> velocity = {0,0,0};
      _linearVelocityArray.push_back(velocity);
      _angularVelocityArray.push_back(velocity);
    }

    int index = particles;

    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////

    _minComputeDomain[0] = centre[0] - xyzDimensions[0]/2;
    _minComputeDomain[1] = centre[1];
    _minComputeDomain[2] = centre[2] - xyzDimensions[2]/2;

    _maxComputeDomain[0] = centre[0] + xyzDimensions[0]/2;
    _maxComputeDomain[1] = centre[1] + xyzDimensions[1]*2.5;
    _maxComputeDomain[2] = centre[2] + xyzDimensions[2]/2;

    ///////////////////////////////////////////////////////////////////////////////////////////
    ////////FLOOR//////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    double height = 0.05; double width = 0.32;
    linear = {0, 0, 0};
    angular = {0, 0, 0};
    position = {centre[0], 0.35, centre[2]};
    material = delta::geometry::material::MaterialType::WOOD;
    bool isFriction = true;
    bool isObstacle = true;

    xyzDimensions = {width, height, width};

    /*
    _coarseXYZDimensionsArray.push_back(xyzDimensions);
    _coarseMaterialArray.push_back(material);
    _coarseisFrictionArray.push_back(isFriction);
    _coarseisObstacleArray.push_back(isObstacle);
    _coarseRadiusArray.push_back(0);
    _coarseLinearVelocityArray.push_back(linear);
    _coarseAngularVelocityArray.push_back(angular);
    _coarsePositionArray.push_back(position);
    _coarseComponentArray.push_back("cube");//floor creations
    */

    delta::world::object objectFloor(
        "cube", 0, position, linear, angular, material, isObstacle, isFriction);

    objectFloor.generateMesh(width, height, width, 0, 0, 0, width, _noPointsPerParticle);

    _coarseObjects.push_back(objectFloor);

    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////

	_deployInsitu = true;

	////////////////////////////////////////////////////////////////////
	//////////PARTICLE GRID/////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

    iREAL xzcuts = 0; iREAL ycuts = 0;
    if(_scenario[3] == n100)
    {
      xzcuts = 10; ycuts = 1;
    }
    else if(_scenario[3] == n1k)
    {
      xzcuts = 10; ycuts = 10;
    }
    else if(_scenario[3] == n10k)
    {
      xzcuts = 30; ycuts = 12;
    }
    else if(_scenario[3] == n100k)
    {
      xzcuts = 41.0; ycuts = 60;
    }
    else if(_scenario[3] == n500k)
    {
      xzcuts = 100.0; ycuts = 50;
    }

    double totalMass = 0.05; material = delta::geometry::material::MaterialType::WOOD;
    isObstacle = false;

    iREAL margin = (_hopperThickness + _epsilon) * 4;
    iREAL subGridLength = _hopperWidth-margin/2;

    //position is top of hopper
    iREAL pos[3] = {(centre[0] - _hopperWidth/2), centre[1] + _hopperHeight/2, (centre[2] - _hopperWidth/2)};

    //create xzy cuts above hopper, position starts at left lower inner corner
    std::vector<std::array<iREAL, 3>> grid = delta::world::assembly::getGridArrayList(pos, xzcuts, ycuts, subGridLength);

    _particleGridArray.insert(_particleGridArray.end(), grid.begin(), grid.end());

    //measure real length
    iREAL xmin = 1; iREAL xmax = 0;
    for(int i=index; i<_particleGridArray.size(); i++)
    {
      if(_particleGridArray[i][0] < xmin) xmin = _particleGridArray[i][0];
      if(_particleGridArray[i][0] > xmax) xmax = _particleGridArray[i][0];
    }

    subGridLength = xmax - xmin;
    iREAL dx = (_hopperWidth - subGridLength)/2;
    //printf("length1:%f\n", subGridLength);
    //printf("length2:%f\n", _hopperWidth-margin*2);

    for(int i=index; i<_particleGridArray.size(); i++)
    {
      _particleGridArray[i][0] += dx;
      _particleGridArray[i][2] += dx;
    }

    if(_scenario[2] == uniform)
    {
      delta::world::assembly::uniform(
          totalMass,
          material,
          _isSphere,
          _noPointsPerParticle,
          _radArray,
          _particleGridArray,
          _componentGridArray,
          _xCoordinatesArray,
          _yCoordinatesArray,
          _zCoordinatesArray,
          index);
    }
    else if(_scenario[2] == nonuniform)
    {
      iREAL subcellx = ((double)subGridLength/(double)xzcuts) - _epsilon*4;
      delta::world::assembly::nonuniform(
          totalMass,
          material,
          _isSphere,
          subcellx,
          _noPointsPerParticle,
          _radArray,
          _particleGridArray,
          _componentGridArray,
          _xCoordinatesArray,
          _yCoordinatesArray,
          _zCoordinatesArray,
          index);
    }

    //////////////////////////////////////////////////////
    //lift above max radii
    /////MIN AND MAX RADIUS//////////////////////////////
    //////////////////////////////////////////////////////
    double maxRad = 0.0;
    double minRad = 1.00;

    for(int i=index; i<_radArray.size(); i++)
    {
      //printf("%f\n", _radArray[i]);
      if(maxRad <= _radArray[i]) maxRad = _radArray[i];
      if(minRad >= _radArray[i]) minRad = _radArray[i];
    }

    _maxParticleDiam = maxRad;
    _minParticleDiam = minRad;
    ////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    for(int i=index; i<_particleGridArray.size(); i++)
    {
      _particleGridArray[i][1] += maxRad+epsilon;

      _materialArray.push_back(material);
      _isFrictionArray.push_back(true);
      _isObstacleArray.push_back(isObstacle);

      std::array<double, 3> angular = {0, 0, 0};
      _angularVelocityArray.push_back(angular);

      std::array<double, 3> linear = {0, 0, 0};
      _linearVelocityArray.push_back(linear);
    }

    if(_yCoordinatesArray.size() >= index)
    {
        for(int i=index; i<_yCoordinatesArray.size(); i++)
        {
          for(int j=index; j<_yCoordinatesArray[i].size(); j++)
          {
            _yCoordinatesArray[i][j] += maxRad+epsilon;
          }
    	    }
    	}

    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    //////////////////////////////////////////////////////
    /// HOPPER FLOW SCENARIO /////////////////////////////
    //////////////////////////////////////////////////////
  } else if(_scenario[1] == friction)
  {
    //////////////////////////////////////////////////////
    /// FRICTION SCENARIO
    //////////////////////////////////////////////////////

    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
    double height = 0.05; double width = 0.35;

    std::array<double, 3> xyzDimensions = {width, height, width};
    double rad = 0.0;
    delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;
    bool isFriction = true;
    bool isObstacle = true;
    std::array<double, 3> linear = {0, 0, 0};
    std::array<double, 3> angular = {0, 0, 0};
    std::array<double, 3> position = {centre[0], centre[1], centre[2]};

    /*
    _coarseXYZDimensionsArray.push_back(xyzDimensions);
    _coarseMaterialArray.push_back(material);
    _coarseisFrictionArray.push_back(isFriction);
    _coarseisObstacleArray.push_back(isObstacle);
    _coarseRadiusArray.push_back(rad);
    _coarseLinearVelocityArray.push_back(linear);
    _coarseAngularVelocityArray.push_back(angular);
    _coarsePositionArray.push_back(position);
    _coarseComponentArray.push_back("cube");*/
    ////////////////////////////////////////////////////////////////////////////////

    delta::world::object objectFloor(
        "cube", 0, position, linear, angular, material, isObstacle, isFriction);

    objectFloor.generateMesh(width, height, width, 0, 0, 0, rad, _noPointsPerParticle);

    _coarseObjects.push_back(objectFloor);

    ////////////////////////////////////////////////////////////////////////////////
    if(_scenario[2] == sstatic)
    {
    ///////////////////////
    delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;

    bool isFriction = true;
    bool isObstacle = false;
    double rad = 0.02;
    std::array<double, 3> position = {centre[0]+0.05, centre[1] + height, centre[2]};
    std::array<double, 3> linear = {0, 0, 0};
    std::array<double, 3> angular = {0, 0, 0};

    std::array<double, 3> xyzDimensions = {_coarseRadiusArray[0], _coarseRadiusArray[0], _coarseRadiusArray[0]};

    /*
    _coarseMaterialArray.push_back(material);
    _coarseisFrictionArray.push_back(isFriction);
    _coarseisObstacleArray.push_back(isObstacle);
    _coarseRadiusArray.push_back(rad);
    _coarseXYZDimensionsArray.push_back(xyzDimensions);
    _coarsePositionArray.push_back(position);
    _coarseLinearVelocityArray.push_back(linear);
    _coarseAngularVelocityArray.push_back(angular);*/

    if(_isSphere){
      //_coarseComponentArray.push_back("sphere");
      delta::world::object objectA(
          "cube", 1, position, linear, angular, material, isObstacle, isFriction);

      objectA.generateMesh(rad, rad, rad, 0, 0, 0, rad, _noPointsPerParticle);
      _coarseObjects.push_back(objectA);
    } else {
      //_coarseComponentArray.push_back("cube");
      delta::world::object objectA(
          "sphere", 1, position, linear, angular, material, isObstacle, isFriction);

      objectA.generateSphere(rad);
      _coarseObjects.push_back(objectA);
    }

    } else if(_scenario[2] == slide)
    {
      delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;
      bool isFriction = true;
      bool isObstacle = false;
      double rad = 0.02;

      std::array<double, 3> linear = {0.5, 0.0, 0.0};
      std::array<double, 3> angular = {0.0, 0.0, 0.0};
      std::array<double, 3> position = {_centreAsArray[0], _centreAsArray[1] + height/2 + _coarseRadiusArray[0] + _epsilon, _centreAsArray[2]};
      std::array<double, 3> xyzDimensions = {_coarseRadiusArray[0], _coarseRadiusArray[0], _coarseRadiusArray[0]};

      /*
      _coarseMaterialArray.push_back(material);
      _coarseisFrictionArray.push_back(isFriction);
      _coarseisObstacleArray.push_back(isObstacle);
      _coarseRadiusArray.push_back(rad);
      _coarseLinearVelocityArray.push_back(linear);
      _coarseAngularVelocityArray.push_back(angular);
      _coarsePositionArray.push_back(position);
      _coarseXYZDimensionsArray.push_back(xyzDimensions);*/

      if(_isSphere){
        //_coarseComponentArray.push_back("sphere");
        delta::world::object objectA(
            "cube", 1, position, linear, angular, material, isObstacle, isFriction);

        objectA.generateMesh(rad, rad, rad, 0, 0, 0, rad, _noPointsPerParticle);
        _coarseObjects.push_back(objectA);
      } else {
        //_coarseComponentArray.push_back("cube");
        delta::world::object objectA(
            "sphere", 1, position, linear, angular, material, isObstacle, isFriction);
        objectA.generateSphere(rad);

        _coarseObjects.push_back(objectA);
      }
    } else if(_scenario[2] == roll)
    {
      delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;
      std::array<double, 3> linear = {0, 0, 0};
      std::array<double, 3> angular = {5.0, 0, 0};
      std::array<double, 3> xyzDimensions = {_coarseRadiusArray[0], _coarseRadiusArray[0], _coarseRadiusArray[0]};
      std::array<double, 3> position = {centre[0], centre[1] + height/2 + centre[0] + _epsilon, _centreAsArray[2]};
      double rad = 0.02;

      bool isFriction = true;
      bool isObstacle = false;

      /*
      _coarseMaterialArray.push_back(material);
      _coarseisFrictionArray.push_back(isFriction);
      _coarseisObstacleArray.push_back(isObstacle);
      _coarseRadiusArray.push_back(rad);
      _coarseLinearVelocityArray.push_back(linear);
      _coarseAngularVelocityArray.push_back(angular);
      _coarseXYZDimensionsArray.push_back(xyzDimensions);
      _coarsePositionArray.push_back(position);*/

      if(_isSphere){
        //_coarseComponentArray.push_back("sphere");

        delta::world::object objectA(
            "cube", 1, position, linear, angular, material, isObstacle, isFriction);

        objectA.generateMesh(rad, rad, rad, 0, 0, 0, rad, _noPointsPerParticle);
        _coarseObjects.push_back(objectA);
      } else {
        //_coarseComponentArray.push_back("cube");

        delta::world::object objectA(
            "sphere", 1, position, linear, angular, material, isObstacle, isFriction);
        objectA.generateSphere(rad);

        _coarseObjects.push_back(objectA);
      }
    }
    //////////////////////////////////////////////////////
    /// END | FRICTION SCENARIO
    //////////////////////////////////////////////////////
  } else if(_scenario[0] == TwoParticlesCrash)
  {
    //////////////////////////////////////////////////////
    /// TWO PARTICLES CRASH SCENARIO
    //////////////////////////////////////////////////////

    auto material = delta::geometry::material::MaterialType::WOOD;
    double rad = 0.01;
    std::array<double, 3> centreArray = {centre[0], 0.9, centre[2]};
    std::array<double, 3> linear = {0, -1, 0};
    std::array<double, 3> angular = {0, 0, 0};

    if(_isSphere){
      delta::world::object objectA(
          "sphere", 0, centreArray, linear, angular, material, false, false);

      objectA.generateSphere(rad);

      centreArray[1] = 0.1;
      linear[1] = 1;

      delta::world::object objectB(
          "sphere", 1, centreArray, linear, angular, material, false, false);

      objectB.generateSphere(rad);

      _coarseObjects.push_back(objectA);
      _coarseObjects.push_back(objectB);
    } else {
      delta::world::object objectA(
          "granulate", 0, centreArray, linear, angular, material, false, false);

      objectA.generateMesh(0,0,0,0,0,0,rad,_noPointsPerParticle);

      centreArray[1] = 0.1;
      linear[1] = 1;

      delta::world::object objectB(
          "granulate", 1, centreArray, linear, angular, material, false, false);

      objectB.generateMesh(0,0,0,0,0,0,rad,_noPointsPerParticle);

      _coarseObjects.push_back(objectA);
      _coarseObjects.push_back(objectB);
    }

    //////////////////////////////////////////////////////
    /// END | TWO PARTICLES CRASH SCENARIO
    //////////////////////////////////////////////////////
  }
  else if(_scenario[0] == blackHoleWithCubes ||
          _scenario[0] == freefallWithCubes ||
          _scenario[0] == blackHoleWithGranulates ||
          _scenario[0] == freefallWithGranulates  ||
          _scenario[0] == blackHoleWithRandomOrientedCubes ||
          _scenario[0] == freefallWithRandomOrientedCubes)
  {
    //////////////////////////////////////////////////////
    /// FREEFALL AND BLACKHOLE SCENARIO
    //////////////////////////////////////////////////////

    //double particleDiameter = (_minParticleDiam + (_maxParticleDiam-_minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) / std::sqrt(DIMENSIONS);
    //int particleid = deployBox(vertex, 0, 0, _centreAsArray, particleDiameter/2, particleDiameter/2, 0, 1.0/8.0, 1.0/8.0, _epsilon, _materialArray[0], _isFrictionArray[0], _isObstacleArray[0]);

    delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;

    bool isFriction = false;
    bool isObstacle = false;
    double rad = 0.1;
    std::array<double, 3> linear = {0, 0, 0};
    std::array<double, 3> angular = {0, 0, 0};
    std::array<double, 3> position = {centre[0], 0.8, centre[2]};

    if(_scenario[0] == blackHoleWithCubes ||
       _scenario[0] == blackHoleWithGranulates  ||
       _scenario[0] == blackHoleWithRandomOrientedCubes)
    {
      linear[0] = static_cast<double>( rand() ) / static_cast<double>(RAND_MAX);
      linear[1] = static_cast<double>( rand() ) / static_cast<double>(RAND_MAX);
      linear[2] = static_cast<double>( rand() ) / static_cast<double>(RAND_MAX);
    }

    /*
    _materialArray.push_back(material);
    _isFrictionArray.push_back(isFriction);
    _isObstacleArray.push_back(isObstacle);
    _radArray.push_back(rad);
    _particleGridArray.push_back(position);
    _angularVelocityArray.push_back(angular);

    if(_isSphere){
      _componentGridArray.push_back("sphere");
    } else {
      _componentGridArray.push_back("cube");
    }

    std::array<double, 3> xyz = {_radArray[0], _radArray[0], _radArray[0]};
    _xyzDimensionsArray.push_back(xyz);

    _linearVelocityArray.push_back(linear);
    */

    //OBJECT ISSUE: here object particles number has to be incremental with deployment in non-insitu enviroment
    if(_isSphere){
      delta::world::object objectA(
          "cube", -1, position, linear, angular, material, false, false);

      _coarseObjects.push_back(objectA);
    } else {
      delta::world::object objectA(
          "cube", -1, position, linear, angular, material, false, false);

      _coarseObjects.push_back(objectA);
    }
    //////////////////////////////////////////////////////
    /// END | FREEFALL AND BLACKHOLE SCENARIO
    //////////////////////////////////////////////////////
  }

  //if(dem::mappings::CreateGrid::_gridType == NoGrid) _deployInsitu = false;

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::CreateGrid::createInnerVertex(
		dem::Vertex&                                  fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
		dem::Vertex * const                           coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&                                    coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

	fineGridVertex.init();
  dropParticles(fineGridVertex, coarseGridVertices, coarseGridVerticesEnumerator, fineGridPositionOfVertex, fineGridH(0));

  if(_gridType != NoGrid && fineGridH(0)>_maxH && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined)
  {
    if(_gridType == RegularGrid)
    {
      fineGridVertex.refine();
    }
    else if((_gridType == AdaptiveGrid || _gridType == ReluctantAdaptiveGrid))
    {
      if(fineGridX(0) >= _minComputeDomain[0] && fineGridX(0) <= _maxComputeDomain[0] &&
         fineGridX(1) >= _minComputeDomain[1] && fineGridX(1) <= _maxComputeDomain[1] &&
         fineGridX(2) >= _minComputeDomain[2] && fineGridX(2) <= _maxComputeDomain[2])
      {
        fineGridVertex.refine();
      }
      else if(fineGridH(0) >= 0.33)
      {
        fineGridVertex.refine();
      }
    }
  }

  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::createBoundaryVertex(
		dem::Vertex&                                 fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
		dem::Vertex * const                          coarseGridVertices,
		const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
		dem::Cell&                                   coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

	fineGridVertex.init();
  dropParticles(fineGridVertex, coarseGridVertices, coarseGridVerticesEnumerator, fineGridPositionOfVertex, fineGridH(0));

  if(_gridType != NoGrid && fineGridH(0)>_maxH && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined)
  {
    if(_gridType == RegularGrid)
    {
      fineGridVertex.refine();
    }
    else if((_gridType == AdaptiveGrid || _gridType == ReluctantAdaptiveGrid))
    {
      if(fineGridX(0) >= _minComputeDomain[0] && fineGridX(0) <= _maxComputeDomain[0] &&
         fineGridX(1) >= _minComputeDomain[1] && fineGridX(1) <= _maxComputeDomain[1] &&
         fineGridX(2) >= _minComputeDomain[2] && fineGridX(2) <= _maxComputeDomain[2])
      {
        fineGridVertex.refine();
      }
      else if(fineGridH(0) >= 0.33)
      {
        fineGridVertex.refine();
      }
    }
  }

	logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::createCell(
		dem::Cell&                                fineGridCell,
		dem::Vertex * const                       fineGridVertices,
		const peano::grid::VertexEnumerator&      fineGridVerticesEnumerator,
		dem::Vertex * const                       coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&                                coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell)
{
	logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  dem::Vertex&  vertex  = fineGridVertices[fineGridVerticesEnumerator(0)];

	if(_scenario[0] == nonescenario) return;

  _centreAsArray[0] = fineGridVerticesEnumerator.getCellCenter()(0);
  _centreAsArray[1] = fineGridVerticesEnumerator.getCellCenter()(1),
  _centreAsArray[2] = fineGridVerticesEnumerator.getCellCenter()(2);

	if (coarseGridCell.isRoot())
	{
		deployCoarseEnviroment(vertex);
	}

	if(peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(
	    fineGridVertices,
	    fineGridVerticesEnumerator,
	    Vertex::Records::Unrefined) &&
	  !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices, fineGridVerticesEnumerator))
	{

	  deployFineEnviroment(vertex, fineGridVerticesEnumerator.getCellSize()(0));

    dfor2(k) //size 2, dimension 3
      for(int i=0; i<fineGridVertices[fineGridVerticesEnumerator(k)].getNumberOfParticles(); i++)
      {
        records::Particle&  particle = fineGridVertices[fineGridVerticesEnumerator(k)].getParticle(i);
        tarch::la::Vector<DIMENSIONS,int> correctVertex;

        for(int d=0; d<DIMENSIONS; d++)
        {
          correctVertex(d) = particle._persistentRecords._centre(d) < fineGridVerticesEnumerator.getCellCenter()(d) ? 0 : 1;
        }

        if(correctVertex != k)
        {
          fineGridVertices[fineGridVerticesEnumerator(correctVertex)].appendParticle(particle);
          fineGridVertices[fineGridVerticesEnumerator(k)].releaseParticle(i);
        }
      }
    enddforx
	}
	logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::CreateGrid::deployCoarseEnviroment(
    dem::Vertex& vertex)
{
  for(int i=0; i<_coarseObjects.size(); i++)
  {
    delta::world::object obj = _coarseObjects[i];

    iREAL position[3] = {obj.getCentre()[0], obj.getCentre()[1], obj.getCentre()[2]};
    int newParticleNumber = dem::mappings::CreateGrid::deployObject(vertex, obj);

    if(newParticleNumber > -1)
    {
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = obj.getLinearVelocity()[0];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = obj.getLinearVelocity()[1];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = obj.getLinearVelocity()[2];

      vertex.getParticle(newParticleNumber)._persistentRecords._angular(0) = obj.getAngularVelocity()[0];
      vertex.getParticle(newParticleNumber)._persistentRecords._angular(1) = obj.getAngularVelocity()[1];
      vertex.getParticle(newParticleNumber)._persistentRecords._angular(2) = obj.getAngularVelocity()[2];

      vertex.getParticle(newParticleNumber)._persistentRecords._referentialAngular(0) = obj.getAngularVelocity()[0];
      vertex.getParticle(newParticleNumber)._persistentRecords._referentialAngular(1) = obj.getAngularVelocity()[1];
      vertex.getParticle(newParticleNumber)._persistentRecords._referentialAngular(2) = obj.getAngularVelocity()[2];
    }
  }
return;
  for(int i=0; i<_coarsePositionArray.size(); i++)
  {
    iREAL position[3] = {_coarsePositionArray[i][0], _coarsePositionArray[i][1], _coarsePositionArray[i][2]};

    int newParticleNumber = -1;

    if(_coarseComponentArray[i] == "sphere")
    {
      newParticleNumber = dem::mappings::CreateGrid::deploySphere(vertex, position, _coarseRadiusArray[i], _epsilon, _coarseMaterialArray[i], _coarseisFrictionArray[i], _coarseisObstacleArray[i]);
    } else if(_coarseComponentArray[i] == "granulate") {
      newParticleNumber = dem::mappings::CreateGrid::deployGranulate(vertex, position, _coarseRadiusArray[i], _epsilon, _coarseMaterialArray[i], _coarseisFrictionArray[i], _coarseisObstacleArray[i]);
    } else if(_coarseComponentArray[i] == "cube") {
      newParticleNumber = dem::mappings::CreateGrid::deployBox(vertex, 0, 1, position, _coarseXYZDimensionsArray[i][0], _coarseXYZDimensionsArray[i][1], 0,0,0, _epsilon, _coarseMaterialArray[i], _coarseisFrictionArray[i], _coarseisObstacleArray[i]);
    } else if(_coarseComponentArray[i] == "hopper") {
      double _hopperHatch = 0.05; double _hopperThickness = 0.005; int refinement = 0;

      std::vector<double> xCoordinates, yCoordinates, zCoordinates;
      delta::geometry::hopper::generateHopper(position, _coarseXYZDimensionsArray[i][0], _hopperThickness, _coarseXYZDimensionsArray[i][1], _hopperHatch, refinement, _maxH, xCoordinates, yCoordinates, zCoordinates);

      newParticleNumber = createParticleObject(0, vertex, position, _epsilon, _coarseMaterialArray[i], _coarseisFrictionArray[i], _coarseisObstacleArray[i], xCoordinates, yCoordinates, zCoordinates);
    }

    if(newParticleNumber > -1)
    {
      if(!_coarseLinearVelocityArray.size() > 0) return;
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = _coarseLinearVelocityArray[i][0];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = _coarseLinearVelocityArray[i][1];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = _coarseLinearVelocityArray[i][2];

      if(!_coarseAngularVelocityArray.size() > 0) return;
      vertex.getParticle(newParticleNumber)._persistentRecords._angular(0) = _coarseAngularVelocityArray[i][0];
      vertex.getParticle(newParticleNumber)._persistentRecords._angular(1) = _coarseAngularVelocityArray[i][1];
      vertex.getParticle(newParticleNumber)._persistentRecords._angular(2) = _coarseAngularVelocityArray[i][2];

      vertex.getParticle(newParticleNumber)._persistentRecords._referentialAngular(0) = _coarseAngularVelocityArray[i][0];
      vertex.getParticle(newParticleNumber)._persistentRecords._referentialAngular(1) = _coarseAngularVelocityArray[i][1];
      vertex.getParticle(newParticleNumber)._persistentRecords._referentialAngular(2) = _coarseAngularVelocityArray[i][2];
    }
  }
}

void dem::mappings::CreateGrid::deployFineEnviroment(
    dem::Vertex& vertex,
    double cellSize)
{

  if(_deployInsitu)
  {
    dem::mappings::CreateGrid::deployParticleInsituSubGrid(vertex, _centreAsArray, cellSize);
  } else {
    if(_componentGridArray.size() == 0) return;

    int newParticleNumber = -1;

    if(_componentGridArray[0] == "sphere")
    {
      newParticleNumber = dem::mappings::CreateGrid::deploySphere(vertex, _centreAsArray, _radArray[0], _epsilon, _materialArray[0], _isFrictionArray[0], _isObstacleArray[0]);
    } else if(_componentGridArray[0] == "cube")
    {
      newParticleNumber = dem::mappings::CreateGrid::deployBox(vertex, 0, 0, _centreAsArray, _xyzDimensionsArray[0][0], _xyzDimensionsArray[0][1], 0, 1.0/8.0, 1.0/8.0, _epsilon, _materialArray[0], _isFrictionArray[0], _isObstacleArray[0]);
      std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
      dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, _isObstacleArray[0]);
    } else if(_componentGridArray[0] == "granulate")
    {
      newParticleNumber = dem::mappings::CreateGrid::deployGranulate(vertex, _centreAsArray, _radArray[0], _epsilon, _materialArray[0], _isFrictionArray[0], _isObstacleArray[0]);
      std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
      dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, _isObstacleArray[0]);
    } else {
      return;
    }

    if(newParticleNumber > -1)
    {
      if(!_linearVelocityArray.size() > 0) return;
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = _linearVelocityArray[0][0];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = _linearVelocityArray[0][1];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = _linearVelocityArray[0][2];

      if(!_angularVelocityArray.size() > 0) return;
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = _angularVelocityArray[0][0];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = _angularVelocityArray[0][1];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = _angularVelocityArray[0][2];
    }
  }
  #ifdef STATSPARTICLE
    logWarning( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter << " and id: " << particleId);
  #endif
}

void dem::mappings::CreateGrid::addParticleToState(
    std::vector<double>&  xCoordinates,
    std::vector<double>&  yCoordinates,
    std::vector<double>&  zCoordinates, bool isObstacle)
{
  _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
  if(isObstacle) _numberOfObstacles++;
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
}

int dem::mappings::CreateGrid::deployHopper(
    dem::Vertex&  vertex,
    int octSectTimes,
    int meshRefinement,
    double position[3],
    double _hopperWidth,
    double _hopperThickness,
    double _hopperHeight,
    double _hopperHatch,
    double eps,
    delta::geometry::material::MaterialType material,
    bool friction,
    bool isObstacle)
{
  std::vector<double> xCoordinates, yCoordinates, zCoordinates;
  delta::geometry::hopper::generateHopper(position, _hopperWidth, _hopperThickness, _hopperHeight, _hopperHatch, meshRefinement, _maxH, xCoordinates, yCoordinates, zCoordinates);
  return createParticleObject(octSectTimes, vertex, position, eps, material, friction, isObstacle, xCoordinates, yCoordinates, zCoordinates);
}

int dem::mappings::CreateGrid::deployBox(
    dem::Vertex&  vertex,
    int quadsect,
    int meshmultiplier,
    double position[3],
    double width,
    double height,
    double rx,
    double ry,
    double rz,
    double eps,
    delta::geometry::material::MaterialType material,
    bool friction,
    bool isObstacle)
{
  std::vector<double> xCoordinates, yCoordinates, zCoordinates;
  delta::geometry::cubes::generateHullCube(position, width, height, width, rx, ry, rz, meshmultiplier, xCoordinates, yCoordinates, zCoordinates);
  return createParticleObject(quadsect, vertex, position, eps, material, friction, isObstacle, xCoordinates, yCoordinates, zCoordinates);
}

int dem::mappings::CreateGrid::createParticleObject(
    int octSectTimes,
    dem::Vertex& vertex,
    double position[3],
    double eps,
    delta::geometry::material::MaterialType material,
    bool friction, bool isObstacle,
    std::vector<double> xCoordinates,
    std::vector<double> yCoordinates,
    std::vector<double> zCoordinates)
{

  int newParticleNumber = 0;
  if(octSectTimes)
  {
    double centerOfMass[3];
    double inertia[9];
    double inverse[9];
    double mass;
    delta::geometry::properties::getInertia(xCoordinates, yCoordinates, zCoordinates, material, mass, centerOfMass, inertia);
    delta::geometry::properties::getInverseInertia(inertia, inverse, isObstacle);

    std::vector<std::vector<double>> xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel;
    std::vector<std::array<double, 3>> centroid;

    centroid.resize(1);
    xCoordinatesMultiLevel.resize(1); yCoordinatesMultiLevel.resize(1); zCoordinatesMultiLevel.resize(1);

    centroid[0][0] = position[0];
    centroid[0][1] = position[1];
    centroid[0][2] = position[2];

    for(int i=0; i<xCoordinates.size(); i++)
    {//coarse triangle push
      xCoordinatesMultiLevel[0].push_back(xCoordinates[i]);
      yCoordinatesMultiLevel[0].push_back(yCoordinates[i]);
      zCoordinatesMultiLevel[0].push_back(zCoordinates[i]);
    }
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    int numOfSubParticles = delta::geometry::triangle::octSectParticle(octSectTimes, xCoordinatesMultiLevel, yCoordinatesMultiLevel, zCoordinatesMultiLevel, centroid);

    ////////LOOP ALL subdivisions of quadtree and create particles
    for(int i=numOfSubParticles; i>(octSectTimes-1)*8; i--)
    {
      for(int j=0; j<xCoordinatesMultiLevel[i].size(); j++)
      {
        xCoordinates.push_back(xCoordinatesMultiLevel[i][j]);
        yCoordinates.push_back(yCoordinatesMultiLevel[i][j]);
        zCoordinates.push_back(zCoordinatesMultiLevel[i][j]);
      }

      iREAL pos[3] = {centroid[i][0], centroid[i][1], centroid[i][2]};

      newParticleNumber = vertex.createSubParticle(pos, xCoordinates, yCoordinates, zCoordinates, centerOfMass, inertia, inverse, mass, eps, friction, material, isObstacle, _numberOfParticles, i);

      _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
      xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
    }
    ////////END LOOP
  } else {
    newParticleNumber = vertex.createParticle(xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
    _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
  }
  _numberOfParticles++;
  if(isObstacle) _numberOfObstacles++;
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

  return newParticleNumber;
}

int dem::mappings::CreateGrid::deploySphere(
    dem::Vertex&  vertex,
    double position[3],
    double radius,
    double eps,
    delta::geometry::material::MaterialType material,
    bool friction,
    bool isObstacle)
{
  int particleNumber = vertex.createSphereParticle(position, radius, eps, friction, material, isObstacle, _numberOfParticles);
  _numberOfParticles++;
  if(isObstacle) _numberOfObstacles++;
  return particleNumber;
}

int dem::mappings::CreateGrid::deployObject(
    dem::Vertex&  vertex,
    delta::world::object object)
{
  int particleNumber = -1;

  if(object.getComponent() == "sphere")
  {
    iREAL position[3] = {object.getCentre()[0], object.getCentre()[1], object.getCentre()[2]};
    particleNumber = vertex.createSphereParticle(position, object.getRad(), _epsilon, object.getIsFriction(), object.getMaterial(), object.getIsObstacle(), _numberOfParticles);
  } else {
    std::vector<double> xCoordinates = object.getxCoordinates();
    std::vector<double> yCoordinates = object.getyCoordinates();
    std::vector<double> zCoordinates = object.getzCoordinates();
    particleNumber = vertex.createParticle(xCoordinates, yCoordinates, zCoordinates, _epsilon, object.getIsFriction(), object.getMaterial(), object.getIsObstacle(), _numberOfParticles, 0);
  }
  _numberOfParticles++;
  if(object.getIsObstacle()) _numberOfObstacles++;
  return particleNumber;
}

int dem::mappings::CreateGrid::deployGranulateFromFile(
    dem::Vertex&  vertex,
    double position[3],
    double radius,
    double eps,
    delta::geometry::material::MaterialType material,
    bool friction,
    bool isObstacle)
{
  std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
  delta::geometry::granulates::loadParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates);
  int newParticleNumber = vertex.createParticle(xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

int dem::mappings::CreateGrid::deployGranulate(
    dem::Vertex&  vertex,
    double position[3],
    double radius,
    double eps,
    delta::geometry::material::MaterialType material,
    bool friction,
    bool isObstacle)
{
  std::vector<double>  xCoordinates, yCoordinates, zCoordinates;

  delta::geometry::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
  int newParticleNumber = vertex.createParticle(xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

int dem::mappings::CreateGrid::decomposeMeshIntoParticles(
    std::vector<double> xCoordinates,
    std::vector<double> yCoordinates,
    std::vector<double> zCoordinates,
	delta::geometry::material::MaterialType material,
	bool isObstacle,
	double &mass,
	double centerOfMass[3],
	double inertia[9],
	double inverse[9])
{

  for(int i=0; i<xCoordinates.size(); i+=3)
  {
    iREAL A[3] = {xCoordinates[i], yCoordinates[i], zCoordinates[i]};
    iREAL B[3] = {xCoordinates[i+1], yCoordinates[i+1], zCoordinates[i+1]};
    iREAL C[3] = {xCoordinates[i+2], yCoordinates[i+2], zCoordinates[i+2]};

    iREAL O[3] = {A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0,  A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0, A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0};

    std::vector<double> subxCoordinates;
    std::vector<double> subyCoordinates;
    std::vector<double> subzCoordinates;

    subxCoordinates.push_back(A[0]);
    subxCoordinates.push_back(B[0]);
    subxCoordinates.push_back(C[0]);

    subyCoordinates.push_back(A[1]);
    subyCoordinates.push_back(B[1]);
    subyCoordinates.push_back(C[1]);

    subzCoordinates.push_back(A[2]);
    subzCoordinates.push_back(B[2]);
    subzCoordinates.push_back(C[2]);

    _xCoordinatesArray.push_back({subxCoordinates[0], subxCoordinates[1], subxCoordinates[2]});
    _yCoordinatesArray.push_back({subyCoordinates[0], subyCoordinates[1], subyCoordinates[2]});
    _zCoordinatesArray.push_back({subzCoordinates[0], subzCoordinates[1], subzCoordinates[2]});

    std::array<iREAL, 3> Oarray = {O[0], O[1], O[2]};
    _particleGridArray.push_back(Oarray);
  }

  delta::geometry::properties::getInertia(xCoordinates, yCoordinates, zCoordinates, material, mass, centerOfMass, inertia);
  delta::geometry::properties::getInverseInertia(inertia, inverse, isObstacle);

  return xCoordinates.size()/3;
}

void dem::mappings::CreateGrid::deployParticleInsituSubGrid(
    dem::Vertex&  vertex,
    double centreAsArray[3],
    double cellSize)
{
  iREAL cellXLeftBoundary = centreAsArray[0] - cellSize/2, cellZLeftBoundary = centreAsArray[2] - cellSize/2;
  iREAL cellXRightBoundary= centreAsArray[0] + cellSize/2, cellZRightBoundary = centreAsArray[2] + cellSize/2;

  iREAL cellYUPBoundary = centreAsArray[1] + cellSize/2;
  iREAL cellYDWBoundary = centreAsArray[1] - cellSize/2;

  if((_numberOfParticles-_numberOfObstacles) <= _particleGridArray.size())
  for(unsigned i=0; i<_particleGridArray.size(); i++)
  {
    if((_particleGridArray[i][0] >= cellXLeftBoundary && _particleGridArray[i][0] <= cellXRightBoundary) &&
       (_particleGridArray[i][1] >= cellYDWBoundary && _particleGridArray[i][1] <= cellYUPBoundary) &&
       (_particleGridArray[i][2] >= cellZLeftBoundary && _particleGridArray[i][2] <= cellZRightBoundary))
    {
      iREAL position[] = {_particleGridArray[i][0], _particleGridArray[i][1], _particleGridArray[i][2]};

      int newParticleNumber;
      if(_componentGridArray[i] == "sphere")
      {
        newParticleNumber = deploySphere(vertex, position, _radArray[i], _epsilon, _materialArray[i], _isFrictionArray[i], _isObstacleArray[i]);
      } else if(_componentGridArray[i] == "granulate" || _componentGridArray[i] == "cube")
      {
        newParticleNumber = vertex.createParticle(_xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], _epsilon, _isFrictionArray[i], _materialArray[i], _isObstacleArray[i], _numberOfParticles, 0);
        dem::mappings::CreateGrid::addParticleToState(_xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], _isObstacleArray[i]);
      } else if(_componentGridArray[i] == "FB")
      {
        std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
        delta::geometry::graphite::generateBrickFB(position, _radArray[i], xCoordinates, yCoordinates, zCoordinates);
        newParticleNumber = vertex.createParticle(xCoordinates, yCoordinates, zCoordinates, _epsilon, _isFrictionArray[i], _materialArray[i], _isObstacleArray[i], _numberOfParticles, 0);
        dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, _isObstacleArray[i]);
      } else if(_componentGridArray[i] == "partialmesh")
      {
        iREAL centreOfMass[3] = {_centreOfMassArray[i][0], _centreOfMassArray[i][1], _centreOfMassArray[i][2]};
        iREAL inertia[9] = {_inertiaArray[i][0], _inertiaArray[i][1], _inertiaArray[i][2], _inertiaArray[i][3], _inertiaArray[i][4], _inertiaArray[i][5], _inertiaArray[i][6], _inertiaArray[i][7], _inertiaArray[i][8]};
        iREAL inverse[9] = {_inverseArray[i][0], _inverseArray[i][1], _inverseArray[i][2], _inverseArray[i][3], _inverseArray[i][4], _inverseArray[i][5], _inverseArray[i][6], _inverseArray[i][7], _inverseArray[i][8]};

        iREAL O[3] = {_particleGridArray[i][0], _particleGridArray[i][1], _particleGridArray[i][2]};

        newParticleNumber = vertex.createSubParticle(O, _xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], centreOfMass, inertia, inverse, _massArray[i], _epsilon, _isFrictionArray[i], _materialArray[i], _isObstacleArray[i], _particleIDArray[i], i);

        _numberOfParticles++; _numberOfTriangles += _xCoordinatesArray[i].size()/DIMENSIONS;
        if(_isObstacleArray[i]) _numberOfObstacles++;
      }

      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = _linearVelocityArray[i][0];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = _linearVelocityArray[i][1];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = _linearVelocityArray[i][2];

      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = _angularVelocityArray[i][0];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = _angularVelocityArray[i][1];
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = _angularVelocityArray[i][2];
    }
  }
}

void dem::mappings::CreateGrid::endIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "endIteration(State)", solverState );

	solverState.incNumberOfParticles(_numberOfParticles);
	solverState.incNumberOfObstacles(_numberOfObstacles);

  solverState.setPrescribedMinimumMeshWidth(_minParticleDiam);
  solverState.setPrescribedMaximumMeshWidth(_maxParticleDiam);

	logInfo( "endIteration(State)", "created "
			<< _numberOfParticles << " particles with "
			<< _numberOfObstacles << " obstacles "
			<< _numberOfTriangles << " triangles");

	//delta::sys::Sys::saveScenario(_numberOfParticles, _numberOfObstacles);

	logTraceOutWith1Argument( "endIteration(State)", solverState);
}

dem::mappings::CreateGrid::CreateGrid() {
	logTraceIn( "CreateGrid()" );
	logTraceOut( "CreateGrid()" );
}

dem::mappings::CreateGrid::~CreateGrid() {
	logTraceIn( "~CreateGrid()" );
	// @todo Insert your code here
	logTraceOut( "~CreateGrid()" );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::CreateGrid::CreateGrid(const CreateGrid&  masterThread) {
	logTraceIn( "CreateGrid(CreateGrid)" );

	_numberOfParticles = 0;
	_numberOfTriangles = 0;
	_numberOfObstacles = 0;

	logTraceOut( "CreateGrid(CreateGrid)" );
}

void dem::mappings::CreateGrid::mergeWithWorkerThread(const CreateGrid& workerThread) {
	logTraceIn( "mergeWithWorkerThread(CreateGrid)" );

	_numberOfParticles += workerThread._numberOfParticles;
	_numberOfTriangles += workerThread._numberOfTriangles;
	_numberOfObstacles += workerThread._numberOfObstacles;

	logTraceOut( "mergeWithWorkerThread(CreateGrid)" );
}
#endif

void dem::mappings::CreateGrid::createHangingVertex(
		dem::Vertex&     fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
		dem::Vertex * const   coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&       coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

	logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::destroyHangingVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::destroyVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::destroyCell(
		const dem::Cell&           fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void dem::mappings::CreateGrid::mergeWithNeighbour(
		dem::Vertex&  vertex,
		const dem::Vertex&  neighbour,
		int                                           fromRank,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
		int                                           level
) {
	logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
	// @todo Insert your code here
	logTraceOut( "mergeWithNeighbour(...)" );
}

void dem::mappings::CreateGrid::prepareSendToNeighbour(
		dem::Vertex&  vertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,double>&   x,
		const tarch::la::Vector<DIMENSIONS,double>&   h,
		int                                           level
) {
	logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
	// @todo Insert your code here
	logTraceOut( "prepareSendToNeighbour(...)" );
}

void dem::mappings::CreateGrid::prepareCopyToRemoteNode(
		dem::Vertex&  localVertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,double>&   x,
		const tarch::la::Vector<DIMENSIONS,double>&   h,
		int                                           level
) {
	logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
	// @todo Insert your code here
	logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::CreateGrid::prepareCopyToRemoteNode(
		dem::Cell&  localCell,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
		const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
		int                                           level
) {
	logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localCell, toRank, cellCentre, cellSize, level );
	// @todo Insert your code here
	logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::CreateGrid::mergeWithRemoteDataDueToForkOrJoin(
		dem::Vertex&  localVertex,
		const dem::Vertex&  masterOrWorkerVertex,
		int                                       fromRank,
		const tarch::la::Vector<DIMENSIONS,double>&  x,
		const tarch::la::Vector<DIMENSIONS,double>&  h,
		int                                       level
) {
	logTraceInWith6Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localVertex, masterOrWorkerVertex, fromRank, x, h, level );
	// @todo Insert your code here
	logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

void dem::mappings::CreateGrid::mergeWithRemoteDataDueToForkOrJoin(
		dem::Cell&  localCell,
		const dem::Cell&  masterOrWorkerCell,
		int                                       fromRank,
		const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
		int                                       level
) {
	logTraceInWith3Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localCell, masterOrWorkerCell, fromRank );
	// @todo Insert your code here
	logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

bool dem::mappings::CreateGrid::prepareSendToWorker(
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
		int                                                                  worker
) {
	logTraceIn( "prepareSendToWorker(...)" );
	// @todo Insert your code here
	logTraceOutWith1Argument( "prepareSendToWorker(...)", true );
	return true;
}

void dem::mappings::CreateGrid::prepareSendToMaster(
		dem::Cell&                       localCell,
		dem::Vertex *                    vertices,
		const peano::grid::VertexEnumerator&       verticesEnumerator,
		const dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
		const dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
	logTraceInWith2Arguments( "prepareSendToMaster(...)", localCell, verticesEnumerator.toString() );
	// @todo Insert your code here
	logTraceOut( "prepareSendToMaster(...)" );
}

void dem::mappings::CreateGrid::mergeWithMaster(
		const dem::Cell&           workerGridCell,
		dem::Vertex * const        workerGridVertices,
		const peano::grid::VertexEnumerator& workerEnumerator,
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
		int                                                                  worker,
		const dem::State&          workerState,
		dem::State&                masterState
) {
	logTraceIn( "mergeWithMaster(...)" );
	// @todo Insert your code here
	logTraceOut( "mergeWithMaster(...)" );
}

void dem::mappings::CreateGrid::receiveDataFromMaster(
		dem::Cell&                        receivedCell,
		dem::Vertex *                     receivedVertices,
		const peano::grid::VertexEnumerator&        receivedVerticesEnumerator,
		dem::Vertex * const               receivedCoarseGridVertices,
		const peano::grid::VertexEnumerator&        receivedCoarseGridVerticesEnumerator,
		dem::Cell&                        receivedCoarseGridCell,
		dem::Vertex * const               workersCoarseGridVertices,
		const peano::grid::VertexEnumerator&        workersCoarseGridVerticesEnumerator,
		dem::Cell&                        workersCoarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&    fineGridPositionOfCell
) {
	logTraceIn( "receiveDataFromMaster(...)" );
	// @todo Insert your code here
	logTraceOut( "receiveDataFromMaster(...)" );
}

void dem::mappings::CreateGrid::mergeWithWorker(
		dem::Cell&           localCell,
		const dem::Cell&     receivedMasterCell,
		const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
		int                                          level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}

void dem::mappings::CreateGrid::mergeWithWorker(
		dem::Vertex&        localVertex,
		const dem::Vertex&  receivedMasterVertex,
		const tarch::la::Vector<DIMENSIONS,double>&   x,
		const tarch::la::Vector<DIMENSIONS,double>&   h,
		int                                           level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif

void dem::mappings::CreateGrid::touchVertexFirstTime(
		dem::Vertex&               fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::touchVertexLastTime(
		dem::Vertex&         fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  dropParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex, fineGridH(0));

	logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::enterCell(
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
	// @todo Insert your code here
	logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}

void dem::mappings::CreateGrid::leaveCell(
		dem::Cell&           fineGridCell,
		dem::Vertex * const  fineGridVertices,
		const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfCell
) {
}

void dem::mappings::CreateGrid::descend(
		dem::Cell * const          fineGridCells,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell
) {
}

void dem::mappings::CreateGrid::ascend(
		dem::Cell * const    fineGridCells,
		dem::Vertex * const  fineGridVertices,
		const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell
) {
}
