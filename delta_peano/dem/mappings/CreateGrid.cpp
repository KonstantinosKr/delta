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

/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
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
double								               dem::mappings::CreateGrid::_epsilon;
int 								                 dem::mappings::CreateGrid::_noPointsPerParticle;
std::vector<std::array<double, 3>>   dem::mappings::CreateGrid::_particleGrid;
std::vector<double>                  dem::mappings::CreateGrid::_radArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_xCoordinatesArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_yCoordinatesArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_zCoordinatesArray;
std::vector<std::string>             dem::mappings::CreateGrid::_componentGrid;
double                               dem::mappings::CreateGrid::_centreAsArray[3];
bool                                 dem::mappings::CreateGrid::_isSphere;

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
		dem::State&  solverState
) {
	logTraceInWith1Argument( "beginIteration(State)", solverState );

  dem::ParticleHeap::getInstance().setName( "particle-heap" );
  dem::DEMDoubleHeap::getInstance().setName( "geometry-heap" );

	logInfo( "beginIteration()", "maxH=" << _maxH );

	srand (time(NULL));

	_numberOfParticles = 0;
	_numberOfTriangles = 0;
	_numberOfObstacles = 0;

  solverState.setPrescribedMinimumMeshWidth(_minParticleDiam);
  solverState.setPrescribedMaximumMeshWidth(_maxParticleDiam);

  _isSphere = (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere ||
               dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::none);

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

      if(fineGridH(0) >= 0.33)
      {
        fineGridVertex.refine();
      }
    }
  }

	fineGridVertex.init();

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

      if(fineGridH(0) >= 0.33)
      {
        fineGridVertex.refine();
      }
    }
  }

	fineGridVertex.init();

	logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::setVScheme(
    dem::Vertex&  vertex,
    int particleNumber,
    VScheme velocity)
{
  switch (velocity)
  {
    case none:
      vertex.getParticle(particleNumber)._persistentRecords._velocity(0) = 0;
      vertex.getParticle(particleNumber)._persistentRecords._velocity(1) = 0;
      vertex.getParticle(particleNumber)._persistentRecords._velocity(2) = 0;

      vertex.getParticle(particleNumber)._persistentRecords._angular(0) = 0;
      vertex.getParticle(particleNumber)._persistentRecords._angular(1) = 0;
      vertex.getParticle(particleNumber)._persistentRecords._angular(2) = 0;
      break;
    case moveLeft:
      vertex.getParticle(particleNumber)._persistentRecords._velocity(0) = -1.0;
      vertex.getParticle(particleNumber)._persistentRecords._velocity(1) = 0;
      vertex.getParticle(particleNumber)._persistentRecords._velocity(2) = 0;

      vertex.getParticle(particleNumber)._persistentRecords._angular(0) = 0;
      vertex.getParticle(particleNumber)._persistentRecords._angular(1) = 0;
      vertex.getParticle(particleNumber)._persistentRecords._angular(2) = 0;
      break;
    case randomLinear:
      vertex.getParticle(particleNumber)._persistentRecords._velocity =
        2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
        2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
        2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0;
      break;
    case randomLinearAngular:
      vertex.getParticle(particleNumber)._persistentRecords._velocity =
        2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
        2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
        2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0;

      vertex.getParticle(particleNumber)._persistentRecords._angular =
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX);
      break;
    case crashY:
      vertex.getParticle(particleNumber)._persistentRecords._velocity(1) = -0.5;
      break;
    case crashXY:
      vertex.getParticle(particleNumber)._persistentRecords._velocity(0) = 0.5;
      vertex.getParticle(particleNumber)._persistentRecords._velocity(1) = -0.5;
      break;
    case crashXYRotation:
      vertex.getParticle(particleNumber)._persistentRecords._velocity(0) = 0.5;
      vertex.getParticle(particleNumber)._persistentRecords._velocity(1) = -0.5;

      vertex.getParticle(particleNumber)._persistentRecords._angular(0) = 5.0;
      vertex.getParticle(particleNumber)._persistentRecords._angular(1) = 5.0;
      break;
    case slideX:
      vertex.getParticle(particleNumber)._persistentRecords._velocity(0) = 0.5;
      break;
    case slideXRotation:
      vertex.getParticle(particleNumber)._persistentRecords._angular(0) = 5.0;
      break;
  }
}

void dem::mappings::CreateGrid::deployCoarseEnviroment(
    dem::Vertex& vertex)
{
  if(_scenario[1] == nuclear)
  {
    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
    auto material = delta::geometry::material::MaterialType::GOLD;
    iREAL position[] = {_centreAsArray[0], _centreAsArray[1], _centreAsArray[2]};
    double height = 0.05; double width = 0.35;
    int particleid = deployBox(vertex, 0, 0, position, width, height, 0, 0, 0, _epsilon, material, true, true);
    dem::mappings::CreateGrid::setVScheme(vertex,  particleid, dem::mappings::CreateGrid::moveLeft);
    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////

    position[1] += height/2;
    if(_scenario[0] == sla)
    {
      delta::world::assembly::loadNuclearGeometry(position, width, 1, _particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam);
    } else if(_scenario[0] == dla)
    {
      delta::world::assembly::loadNuclearGeometry(position, width, 2, _particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam);
    }
    else if(_scenario[0] == nuclearDeck)
    {
      if(_scenario[2] == n1)
      {
        //nuclear deck 1s
        delta::world::assembly::makeBrickGrid(position, 0.10, 1, 0.1, 1, _particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam);
      }else if(_scenario[2] == n4)
      {
        //nuclear deck 4s
        delta::world::assembly::makeBrickGrid(position, 0.10, 2, 0.1, 1, _particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam);
      }else if(_scenario[2] == n32)
      {
        //nuclear deck 32s
        delta::world::assembly::makeBrickGrid(position, 0.15, 4, 0.1, 2, _particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam);
      }
      else if(_scenario[2] == n64)
      {
        //nuclear deck 64
        delta::world::assembly::makeBrickGrid(position, 0.15, 4, 0.4, 4, _particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam);
      }
      else if(_scenario[2] == n256)
      {
        //nuclear deck 256
        delta::world::assembly::makeBrickGrid(position, 0.15, 10, 0.08, 4, _particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam);
      }
    }
  } else if(_scenario[1] == hopper)
  {
    ////////HOPPER////////////////////////////////////////////////////////////////////////////////////////////
    auto material = delta::geometry::material::MaterialType::GOLD;
    double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.05; double _hopperThickness = 0.005;
    int particleId = deployHopper(vertex, 0, 0, _centreAsArray, _hopperWidth, _hopperThickness, _hopperHeight, _hopperHatch, _epsilon, material, false, true);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    _minComputeDomain[0] = _centreAsArray[0] - _hopperWidth/2;
    _minComputeDomain[1] = _centreAsArray[1];
    _minComputeDomain[2] = _centreAsArray[2] - _hopperWidth/2;

    _maxComputeDomain[0] = _centreAsArray[0] + _hopperWidth/2;
    _maxComputeDomain[1] = _centreAsArray[1] + _hopperHeight*2.5;
    _maxComputeDomain[2] = _centreAsArray[2] + _hopperWidth/2;

    ////////FLOOR///////////////////////////////////////////////////////////////////////////////////////////
    iREAL position[] = {_centreAsArray[0], 0.35, _centreAsArray[2]};
    double height = 0.05; double width = 0.35;
    particleId = deployBox(vertex, 0, 0, position, width, height, 0, 0, 0, _epsilon, material, true, true);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////PARTICLE GRID/////////////////////////////////////////////////////////////////////////////////
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

    iREAL margin = (_hopperThickness + _epsilon) * 4;

    //iREAL minParticleDiam = ((double)_hopperWidth/(double)xzcuts)-(margin*2.0);
    //printf("minParDiam:%.10f\n", minParticleDiam);

    //position is top of hopper
    position[0] = (_centreAsArray[0] - _hopperWidth/2);
    position[1] = _centreAsArray[1] + _hopperHeight/2;
    position[2] = (_centreAsArray[2] - _hopperWidth/2);

    //create xzy cuts above hopper, position starts at left lower inner corner
    _particleGrid = delta::world::assembly::getGridArrayList(position, xzcuts, ycuts, _hopperWidth);

    if(_scenario[2] == uniform)
    {
      delta::world::assembly::uniform(
          totalMass,
          material,
          _isSphere,
          _noPointsPerParticle,
          _radArray,
          _particleGrid,
          _componentGrid,
          _minParticleDiam,
          _maxParticleDiam,
          _xCoordinatesArray,
          _yCoordinatesArray,
          _zCoordinatesArray);
    }
    else if(_scenario[2] == nonuniform)
    {
      iREAL minArraylengthXZ = (double)_hopperWidth - _epsilon * 6;
      iREAL subcellx = ((double)minArraylengthXZ/(double)xzcuts) - _epsilon*4;
      delta::world::assembly::nonuniform (
          totalMass,
          material,
          _isSphere,
          subcellx,
          _noPointsPerParticle,
          _radArray,
          _particleGrid,
          _componentGrid,
          _minParticleDiam,
          _maxParticleDiam,
          _xCoordinatesArray,
          _yCoordinatesArray,
          _zCoordinatesArray);
    }

    //lift above max radii
    double maxRad = 0;
    for(int i=0; i<_radArray.size(); i++)
    {
      if(maxRad < _radArray[i]) maxRad = _radArray[i];
    }
    for(int i=0; i<_particleGrid.size(); i++)
    {
      _particleGrid[i][1] += maxRad+epsilon;
    }
  }
  else if(_scenario[1] == friction)
  {
    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
    auto material = delta::geometry::material::MaterialType::WOOD;
    double height = 0.05; double width = 0.35;
    deployBox(vertex, 0, 0, _centreAsArray, width, height, 0,0,0, _epsilon, material, true, true);
    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////

    double radius = 0.01; double eps = radius*0.2;
    if(_scenario[2] == sstatic)
    {
      int newParticleNumber;
      iREAL position[] = {_centreAsArray[0], _centreAsArray[1] + height, _centreAsArray[2]};
      if(_isSphere)
      {
        newParticleNumber = dem::mappings::CreateGrid::deploySphere(vertex, position, radius, eps, material, true, false);
      } else {
        newParticleNumber = dem::mappings::CreateGrid::deployBox(vertex, 0, 0, position, radius*2, radius*2, 0,0,0, eps, material, true, false);
      }
      setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::crashY);
    }
    else if(_scenario[2] == slide || _scenario[2] == roll)
    {
      int newParticleNumber;
      if(_isSphere)
      {
        iREAL position[] = {_centreAsArray[0], _centreAsArray[1] + height/2 + radius + eps*2, _centreAsArray[2]};
        newParticleNumber = dem::mappings::CreateGrid::deploySphere(vertex, position, radius, eps, material, true, false);
      } else {
        iREAL position[] = {_centreAsArray[0], _centreAsArray[1] + height/2 + radius + eps, _centreAsArray[2]};
        if(_scenario[2] == slide)
        {
          newParticleNumber = dem::mappings::CreateGrid::deployGranulateFromFile(vertex, position, radius, eps, material, true, false);
          setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::slideX);
        }
        else if(_scenario[2] == roll)
        {
          newParticleNumber = dem::mappings::CreateGrid::deployBox(vertex, 0, 0, position, radius*2, radius*2, 0,0,0, eps, material, true, false);
          setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::slideXRotation);
        }
      }
    }
  }
  else if(_scenario[0] == TwoParticlesCrash)
  {
    double radius = 0.005;
    auto material = delta::geometry::material::MaterialType::WOOD;
    iREAL position[] = {_centreAsArray[0], 0.8, _centreAsArray[2]};
    if(_isSphere)
    {
      int newParticleNumber = dem::mappings::CreateGrid::deploySphere(vertex, position, radius, _epsilon, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = -0.5;

      position[1] = 0.2;
      newParticleNumber = dem::mappings::CreateGrid::deploySphere(vertex, position, radius, _epsilon, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0.5;
    } else {
      int newParticleNumber = dem::mappings::CreateGrid::deployGranulate(vertex, position, radius, _epsilon, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = -0.5;

      position[1] = _centreAsArray[1] - radius*2;
      newParticleNumber = dem::mappings::CreateGrid::deployGranulate(vertex, position, radius, _epsilon, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0.5;
    }
  }
}

void dem::mappings::CreateGrid::deployFineEnviroment(
    dem::Vertex& vertex,
    double centreAsArray[3],
    double cellSize,
    delta::geometry::material::MaterialType material,
    double friction,
    double isObstacle)
{
  if(_scenario[0] == blackHoleWithRandomOrientedCubes ||
     _scenario[0] == freefallWithRandomOrientedCubes)
  {
    int particleid = deployBox(vertex, 0, 0, centreAsArray, _maxParticleDiam, _maxParticleDiam, static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        _epsilon, material, true, false);
    if(_scenario[0] == blackHoleWithRandomOrientedCubes)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario[0] == blackHoleWithCubes ||
          _scenario[0] == freefallWithCubes)
  {
    double particleDiameter = (_minParticleDiam + (_maxParticleDiam-_minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) / std::sqrt(DIMENSIONS);
    int particleid = deployBox(vertex, 0, 0, centreAsArray, particleDiameter/2, particleDiameter/2, 0, 1.0/8.0, 1.0/8.0, _epsilon, material, friction, isObstacle);

    if(_scenario[0] == blackHoleWithCubes)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario[0] == blackHoleWithGranulates ||
          _scenario[0] == freefallWithGranulates)
  {
    double particleDiameter = (_minParticleDiam + (_maxParticleDiam-_minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) / std::sqrt(DIMENSIONS);
    int particleid = dem::mappings::CreateGrid::deployGranulate(vertex, centreAsArray, particleDiameter/2, _epsilon, material, friction, isObstacle);
    if(_scenario[0] == blackHoleWithGranulates)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario[1] == nuclear || _scenario[1] == hopper)
  {
    double epsMulti = 0.2;//percentage of radius is epsilon
    dem::mappings::CreateGrid::deployParticleInsituSubGrid(vertex, centreAsArray, cellSize, epsMulti, material, friction, isObstacle);
  }

  #ifdef STATSPARTICLE
    logWarning( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter << " and id: " << particleId);
  #endif
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
	if (coarseGridCell.isRoot())
	{
		_centreAsArray[0] = fineGridVerticesEnumerator.getCellCenter()(0);
    _centreAsArray[1] = fineGridVerticesEnumerator.getCellCenter()(1),
    _centreAsArray[2] = fineGridVerticesEnumerator.getCellCenter()(2);

		deployCoarseEnviroment(vertex);
	}

	if(peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices, fineGridVerticesEnumerator,  Vertex::Records::Unrefined) &&
	  !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices,  fineGridVerticesEnumerator))
	{
		int particlesInCellPerAxis = std::floor(fineGridVerticesEnumerator.getCellSize()(0) / _maxParticleDiam);

		if(particlesInCellPerAxis==0)
		{
			particlesInCellPerAxis = 1;
			//_maxParticleDiam = fineGridVerticesEnumerator.getCellSize()(0); //_minParticleDiam = std::min(_minParticleDiam, _maxParticleDiam);
		}

		dfor(k,particlesInCellPerAxis)
		{
		  const auto centre = fineGridVerticesEnumerator.getVertexPosition() + _maxParticleDiam * (k.convertScalar<double>()+0.5);
      double centreAsArray[] = {centre(0),centre(1),centre(2)};

      auto material = delta::geometry::material::MaterialType::WOOD; bool friction = true; bool obstacle = false;
      deployFineEnviroment(vertex, centreAsArray, fineGridVerticesEnumerator.getCellSize()(0), material, friction, obstacle);
		}
	}

	logTraceOutWith1Argument( "createCell(...)", fineGridCell );
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
    int quadsect,
    int meshmultiplier,
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
  delta::geometry::hopper::generateHopper(position, _hopperWidth, _hopperThickness, _hopperHeight, _hopperHatch, meshmultiplier, xCoordinates, yCoordinates, zCoordinates);
  return createParticleObject(quadsect, vertex, position, eps, material, friction, isObstacle, xCoordinates, yCoordinates, zCoordinates);
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
    int quadsect,
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
  if(quadsect > 0)
  {
    double centerOfMass[3];
    double inertia[9];
    double inverse[9];
    double mass;
    delta::geometry::properties::getInertia(xCoordinates, yCoordinates, zCoordinates, material, mass, centerOfMass, inertia);
    delta::geometry::properties::getInverseInertia(inertia, inverse, isObstacle);

    std::vector<std::vector<double>> xCoordinatesVec, yCoordinatesVec, zCoordinatesVec;
    std::vector<std::array<double, 3>> centroid;

    centroid.resize(1);
    xCoordinatesVec.resize(1); yCoordinatesVec.resize(1); zCoordinatesVec.resize(1);

    centroid[0][0] = position[0];
    centroid[0][1] = position[1];
    centroid[0][2] = position[2];

    for(int i=0; i<xCoordinates.size(); i++)
    {
      xCoordinatesVec[0].push_back(xCoordinates[i]);
      yCoordinatesVec[0].push_back(yCoordinates[i]);
      zCoordinatesVec[0].push_back(zCoordinates[i]);
    }
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    int numOfSubParticles = delta::geometry::triangle::meshOctSect(quadsect, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);

    for(int i=numOfSubParticles; i>(quadsect-1)*8; i--)
    {
      for(int j=0; j<xCoordinatesVec[i].size(); j++)
      {
        xCoordinates.push_back(xCoordinatesVec[i][j]);
        yCoordinates.push_back(yCoordinatesVec[i][j]);
        zCoordinates.push_back(zCoordinatesVec[i][j]);
      }

      iREAL pos[3];
      pos[0] = centroid[i][0];
      pos[1] = centroid[i][1];
      pos[2] = centroid[i][2];
      //delta::geometry::properties::centerOfGeometry(pos, xCoordinates, yCoordinates, zCoordinates);
      newParticleNumber = vertex.createSubParticle(pos, xCoordinates, yCoordinates, zCoordinates, centerOfMass, inertia, inverse, mass, eps, friction, material, isObstacle, _numberOfParticles, i);

      _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
      xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
    }
  } else {
    newParticleNumber = vertex.createParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
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
  _numberOfParticles++;
  if(isObstacle) dem::mappings::CreateGrid::_numberOfObstacles++;
  return vertex.createSphereParticle(position, radius, eps, friction, material, isObstacle, _numberOfParticles);
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
  int newParticleNumber = vertex.createParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
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
  int newParticleNumber = vertex.createParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

void dem::mappings::CreateGrid::deployParticleInsituSubGrid(
    dem::Vertex&  vertex,
    double centreAsArray[3],
    double cellSize,
    double eps,
    delta::geometry::material::MaterialType material,
    double friction,
    double isObstacle)
{
  iREAL cellXLeftBoundary = centreAsArray[0] - cellSize/2, cellZLeftBoundary = centreAsArray[2] - cellSize/2;
  iREAL cellXRightBoundary= centreAsArray[0] + cellSize/2, cellZRightBoundary = centreAsArray[2] + cellSize/2;

  iREAL cellYUPBoundary = centreAsArray[1] + cellSize/2;
  iREAL cellYDWBoundary = centreAsArray[1] - cellSize/2;

  if((_numberOfParticles-_numberOfObstacles) <= _particleGrid.size())
  for(unsigned i=0; i<_particleGrid.size(); i++)
  {
    if((_particleGrid[i][0] >= cellXLeftBoundary && _particleGrid[i][0] <= cellXRightBoundary) &&
       (_particleGrid[i][1] >= cellYDWBoundary && _particleGrid[i][1] <= cellYUPBoundary) &&
       (_particleGrid[i][2] >= cellZLeftBoundary && _particleGrid[i][2] <= cellZRightBoundary))
    {
      iREAL position[] = {_particleGrid[i][0], _particleGrid[i][1], _particleGrid[i][2]};
      if(_componentGrid[i] == "sphere")
      {
        deploySphere(vertex, position, _radArray[i], _radArray[i]*eps, material, friction, isObstacle);
      } else if(_componentGrid[i] == "nonSpherical")
      {
        vertex.createParticle(position, _xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], _radArray[i]*eps, friction, material, isObstacle, _numberOfParticles, 0);
        dem::mappings::CreateGrid::addParticleToState(_xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], isObstacle);
      } else if(_componentGrid[i] == "FB")
      {
        std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
        delta::geometry::graphite::generateBrickFB(position, _radArray[i], xCoordinates, yCoordinates, zCoordinates);
        vertex.createParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
        dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
      }
    }
  }
}

void dem::mappings::CreateGrid::endIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "endIteration(State)", solverState );

	solverState.incNumberOfParticles(_numberOfParticles);
	solverState.incNumberOfObstacles(_numberOfObstacles);

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
	// @todo Insert your code here
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
	// @todo Insert your code here
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
	// @todo Insert your code here
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
	// @todo Insert your code here

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
