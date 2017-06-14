#include "dem/mappings/CreateGrid.h"

#include "delta/primitives/cubes.h"
#include "delta/primitives/granulates.h"
#include "delta/primitives/properties.h"
#include "delta/primitives/hopper.h"
#include "delta/primitives/blender.h"
#include <delta/primitives/graphite.h>
#include <delta/primitives/surface.h>
#include <delta/world/assembly.h>

#include "peano/grid/aspects/VertexStateAnalysis.h"

#include <string>
#include <iostream>
#include <cmath>
#include <ctime>

#define epsilon 0.001

/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::CreateGrid::communicationSpecification() const {
	return peano::CommunicationSpecification(
	    peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,
	    peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexLastTimeSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
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
double                               dem::mappings::CreateGrid::_minParticleDiam;
double                               dem::mappings::CreateGrid::_maxParticleDiam;
dem::mappings::CreateGrid::GridType  dem::mappings::CreateGrid::_gridType;
double								               dem::mappings::CreateGrid::_epsilon;
int 								                 dem::mappings::CreateGrid::_noPointsPerParticle;
std::vector<std::array<double, 3>>   dem::mappings::CreateGrid::_particleGrid;
std::vector<double>                  dem::mappings::CreateGrid::_rad;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_xCoordinatesArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_yCoordinatesArray;
std::vector<std::vector<double>>     dem::mappings::CreateGrid::_zCoordinatesArray;
std::vector<std::string>             dem::mappings::CreateGrid::_componentGrid;
double                               dem::mappings::CreateGrid::_centreAsArray[3];

void dem::mappings::CreateGrid::setScenario(Scenario scenario[4],
                                            double maxH,
                                            double particleDiamMin, double particleDiamMax,
											                      GridType gridType, int noPointsPerGranulate)
{
	_scenario[0]        		  = scenario[0];
  _scenario[1]              = scenario[1];
  _scenario[2]              = scenario[2];
  _scenario[3]              = scenario[3];
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

	if (fineGridH(0)>_maxH && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined && _gridType!=NoGrid)
	{
		fineGridVertex.refine();
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

	if (fineGridH(0)>_maxH && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined && _gridType!=NoGrid)
	{
		fineGridVertex.refine();
	}

	fineGridVertex.init();

	logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::setVScheme(dem::Vertex&  vertex, int particleNumber, VScheme velocity)
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

void dem::mappings::CreateGrid::makeCoarseEnviroment(dem::Vertex& vertex, double centreAsArray[3], double eps, int noPointsPerParticle)
{
  if(_scenario[0] == sla ||
     _scenario[0] == nuclearArray)
  {
    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::GOLD;
    centreAsArray[1] = 0.5;
    iREAL position[] = {centreAsArray[0], centreAsArray[1], centreAsArray[2]};
    double floorHeight = 0.05;

    int particleid = makeBox(vertex, 1, 1, position, 1, floorHeight, 0, 0, 0, eps, material, true, true);
    dem::mappings::CreateGrid::setVScheme(vertex,  particleid, dem::mappings::CreateGrid::moveLeft);
    if(_scenario[0] == sla)
    {
      delta::world::assembly::makeLoadNuclearGeometry(centreAsArray, _particleGrid, _componentGrid, _rad, _minParticleDiam, _maxParticleDiam);
    } else if(_scenario[0] == nuclearArray)
    {
      delta::world::assembly::makeFullBrickFBGrid(centreAsArray, 1, 10, _particleGrid, _componentGrid, _rad, _minParticleDiam, _maxParticleDiam);
    }
  } else if(_scenario[1] == hopper)
  {
    ////////HOPPER////////////////////////////////////////////////////////////////////////////////////////////
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::GOLD;
    double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.05;
    //HOPPER DIAGONAL:0.382926
    int particleId = makeHopper(vertex, 1, 2, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, eps, material, false, true);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////FLOOR////////////////floor DIAGONAL:0.344674///////////////////////////////////////////
    iREAL position[] = {centreAsArray[0], 0.35, centreAsArray[2]};
    double height = 0.05; double width = 0.35;
    particleId = makeBox(vertex, 1, 1, position, width, height, 0, 0, 0, eps, material, true, true);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////PARTICLE GRID///////////////////////////////////////////////////////////////////////////////////////
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

    double totalMass = 0.05; material = delta::collision::material::MaterialType::WOOD;

    iREAL minArraylengthXZ = (double)_hopperWidth - eps * 6;

    iREAL subcellx = ((double)minArraylengthXZ/(double)xzcuts) - eps*4;
    iREAL margin = ((double)minArraylengthXZ/(double)xzcuts)/2.0;

    //iREAL minParticleDiameter = ((double)_hopperWidth/(double)xzcuts)-(margin*2.0);
    //printf("minParDiameter:%.10f\n", minParticleDiameter);

    position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
    _particleGrid = delta::world::assembly::getGridArrayList(position, xzcuts, ycuts, _hopperWidth);

    if(_scenario[2] == uniform)
    {
      if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere ||
         dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::none)
      {
        delta::world::assembly::uniSphereRadius(totalMass, material,
                                                _rad, _particleGrid, _componentGrid,
                                                _minParticleDiam, _maxParticleDiam);
      } else {
        delta::world::assembly::uniMeshGeometry(totalMass, material, _noPointsPerParticle,
                                                _rad, _particleGrid, _componentGrid,
                                                _minParticleDiam, _maxParticleDiam,
                                                _xCoordinatesArray, _yCoordinatesArray, _zCoordinatesArray);
      }
    }
    else if(_scenario[2] == nonuniform)
    {
      if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere ||
         dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::none)
      {
        delta::world::assembly::nonUniSphereRadius(totalMass, material, subcellx,
                                                  _rad, _particleGrid, _componentGrid,
                                                  _minParticleDiam, _maxParticleDiam);
      } else {
        delta::world::assembly::nonUniMeshGeometry(totalMass, material, subcellx, _noPointsPerParticle,
                                                  _rad, _particleGrid, _componentGrid,
                                                  _minParticleDiam, _maxParticleDiam,
                                                  _xCoordinatesArray, _yCoordinatesArray, _zCoordinatesArray);
      }
    }
  }
  else if(_scenario[0] == frictionStatic)
   {
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::WOOD;
    double height = 0.05; double width = 0.35;
    makeBox(vertex, 1, 1, centreAsArray, width, height, 0,0,0, eps, material, true, true);

    double radius = 0.01; double eps = radius*0.2;

    int newParticleNumber;
    if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
    {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, material, true, false);
    } else {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + ((height)/1.5)-width/2, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeBox(vertex, 1, 1, position, width, height, 0,0,0, eps, material, true, false);
    }
    setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::crashY);
  } else if(_scenario[0] == frictionSlide)
  {
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::WOOD;
    double height = 0.05; double width = 0.35;
    makeBox(vertex, 1, 1, centreAsArray, width, height, 0,0,0, eps, material, true, true);

    double radius = 0.01; double eps = radius*0.2;

    int newParticleNumber;
    if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
    {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius + eps*2, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, material, true, false);
    } else {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius + eps, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeLoadedNonSpherical(vertex, position, radius, eps, material, true, false);
    }
    setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::slideX);
  } else if(_scenario[0] == frictionRoll)
  {
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::WOOD;
    double height = 0.05; double width = 0.35;
    makeBox(vertex, 1, 1, centreAsArray, width, height, 0,0,0.1, eps, material, true, true);

    double radius = 0.01; double eps = radius*0.2;

    int newParticleNumber;
    if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
    {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius/2 + eps*2, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, material, true, false);
    } else {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeBox(vertex, 1, 1, position, width, height, 0,0,0, eps, material, true, false);
    }
    setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::slideXRotation);
  } else if(_scenario[0] == TwoParticlesCrash)
  {
    double radius = 0.05;
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::WOOD;
    if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
    {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + radius*2, centreAsArray[2]};
      int newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = -0.5;

      position[1] = centreAsArray[1] - radius*2;;
      newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0.5;
    } else {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + radius*2, centreAsArray[2]};
      int newParticleNumber = dem::mappings::CreateGrid::makeNonSpherical(vertex, position, radius, eps, noPointsPerParticle, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = -0.5;

      position[1] = centreAsArray[1] - radius*2;;
      newParticleNumber = dem::mappings::CreateGrid::makeNonSpherical(vertex, position, radius, eps, noPointsPerParticle, material, false, false);
      vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0.5;
    }
  }
}

void dem::mappings::CreateGrid::makeFineEnviroment(dem::Vertex& vertex,
    dem::Vertex * const                       coarseGridVertices,
    const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
    double centreAsArray[3],
    double cellSize,
    double eps,
    int noPointsPerParticle,
    delta::collision::material::MaterialType material,
    double friction,
    double isObstacle,
    double &minParticleDiam,
    double &maxParticleDiam)
{
  if(_scenario[0] == blackHoleWithRandomOrientedCubes ||
     _scenario[0] == freefallWithRandomOrientedCubes)
  {
    int particleid = makeBox(vertex, 1, 1, centreAsArray, maxParticleDiam, maxParticleDiam, static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        eps, material, true, false);
    if(_scenario[0] == blackHoleWithRandomOrientedCubes)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario[0] == blackHoleWithCubes ||
          _scenario[0] == freefallWithCubes)
  {
    //printf("%i\n", _numberOfParticles);
    double particleDiameter = (minParticleDiam + (maxParticleDiam-minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) / std::sqrt(DIMENSIONS);
    //int particleid = makeSphere(vertex, centreAsArray, particleDiameter/2, particleDiameter/2*eps, material, friction, isObstacle);
    int particleid = makeBox(vertex, 1, 1, centreAsArray, particleDiameter, particleDiameter, 0, 1.0/8.0, 1.0/8.0, eps, material, friction, isObstacle);

    if(_scenario[0] == blackHoleWithCubes)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario[0] == blackHoleWithGranulates ||
          _scenario[0] == freefallWithGranulates)
  {
    double particleDiameter = (minParticleDiam + (maxParticleDiam-minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) / std::sqrt(DIMENSIONS);
    int particleid = dem::mappings::CreateGrid::makeNonSpherical(vertex, centreAsArray, particleDiameter/2, eps, noPointsPerParticle, material, friction, isObstacle);
    if(_scenario[0] == blackHoleWithGranulates)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario[0] == sla ||
           _scenario[0] == nuclearArray ||
           _scenario[1] == hopper)
  {
    eps = 0.2;//percentage of radius is epsilon
    dem::mappings::CreateGrid::deployParticleInsituSubGrid(vertex, centreAsArray, cellSize, eps, material, friction, isObstacle);
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
	  printf("level:%i\n", fineGridVerticesEnumerator.getLevel());
		_centreAsArray[0] = fineGridVerticesEnumerator.getCellCenter()(0);
    _centreAsArray[1] = fineGridVerticesEnumerator.getCellCenter()(1),
    _centreAsArray[2] = fineGridVerticesEnumerator.getCellCenter()(2);

		makeCoarseEnviroment(vertex, _centreAsArray, _epsilon, _noPointsPerParticle);
	}

	if(peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices, fineGridVerticesEnumerator,  Vertex::Records::Unrefined) &&
	  !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices,  fineGridVerticesEnumerator))
	{
		int particlesInCellPerAxis = std::floor(fineGridVerticesEnumerator.getCellSize()(0) / _maxParticleDiam);

		if(particlesInCellPerAxis==0)
		{
			particlesInCellPerAxis = 1;
			//_maxParticleDiam = fineGridVerticesEnumerator.getCellSize()(0); //_minParticleDiam = std::min(_minParticleDiam, _maxParticleDiam);
			//logWarning( "createCell(...)", "particle size has been too small for coarsest prescribed grid. Reduce particle size to " << _minParticleDiam << "-" << fineGridVerticesEnumerator.getCellSize()(0) );
		}

		dfor(k,particlesInCellPerAxis)
		{
		  const auto centre = fineGridVerticesEnumerator.getVertexPosition() + _maxParticleDiam * (k.convertScalar<double>()+0.5);
      double centreAsArray[] = {centre(0),centre(1),centre(2)};

      auto material = delta::collision::material::MaterialType::WOOD;
      bool friction = true; bool obstacle = false;
      makeFineEnviroment(vertex, coarseGridVertices, coarseGridVerticesEnumerator, centreAsArray, fineGridVerticesEnumerator.getCellSize()(0), _epsilon, _noPointsPerParticle, material, friction, obstacle, _minParticleDiam, _maxParticleDiam);
		}
	}
	logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::CreateGrid::addParticleToState(std::vector<double>&  xCoordinates,
                                                   std::vector<double>&  yCoordinates,
                                                   std::vector<double>&  zCoordinates, bool isObstacle)
{
  _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
  if(isObstacle) _numberOfObstacles++;
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
}

int dem::mappings::CreateGrid::makeHopper(dem::Vertex&  vertex, int quadsect, int meshmultiplier,
                                          double position[3],
                                          double _hopperWidth,
                                          double _hopperHeight,
                                          double _hopperHatch,
                                          double eps,
                                          delta::collision::material::MaterialType material,
                                          bool friction,
                                          bool isObstacle)
{
  std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
  delta::primitives::hopper::generateHopper(position, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
  delta::primitives::triangle::meshDenser(2, xCoordinates, yCoordinates, zCoordinates);

  return createParticleObject(quadsect, vertex, position, eps, material, friction, isObstacle, xCoordinates, yCoordinates, zCoordinates);
}

int dem::mappings::CreateGrid::makeBox(dem::Vertex&  vertex, int quadsect, int meshmultiplier,
                                          double position[3],
                                          double width,
                                          double height,
                                          double rx,
                                          double ry,
                                          double rz,
                                          double eps,
                                          delta::collision::material::MaterialType material,
                                          bool friction,
                                          bool isObstacle)
{
  std::vector<double>  xCoordinates, yCoordinates, zCoordinates;
  delta::primitives::cubes::generateHullCube(position, width, height, width, rx, ry, rz, xCoordinates, yCoordinates, zCoordinates);

  delta::primitives::triangle::meshDenser(meshmultiplier, xCoordinates, yCoordinates, zCoordinates);
  return createParticleObject(quadsect, vertex, position, eps, material, friction, isObstacle, xCoordinates, yCoordinates, zCoordinates);
}

int dem::mappings::CreateGrid::createParticleObject(int quadsect, dem::Vertex&  vertex, double position[3],
    double eps, delta::collision::material::MaterialType material, bool friction, bool isObstacle,
    std::vector<double> xCoordinates, std::vector<double> yCoordinates, std::vector<double> zCoordinates)
{

  int newParticleNumber = 0;
  if(quadsect > 0)
  {
    double centerOfMass[3];
    double inertia[9];
    double inverse[9];
    double mass;
    double hMin = delta::primitives::properties::computeHMin(xCoordinates, yCoordinates, zCoordinates);
    double diameter = delta::primitives::properties::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
    delta::primitives::properties::computeInertia(xCoordinates, yCoordinates, zCoordinates, material, mass, centerOfMass, inertia);
    delta::primitives::properties::computeInverseInertia(inertia, inverse, isObstacle);

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

    int numOfSubParticles = delta::primitives::triangle::meshOctSect(quadsect, xCoordinatesVec, yCoordinatesVec, zCoordinatesVec, centroid);

    for(int i=numOfSubParticles; i>(quadsect-1)*8; i--)
    {
      for(int j=0; j<xCoordinatesVec[i].size(); j++)
      {
        xCoordinates.push_back(xCoordinatesVec[i][j]);
        yCoordinates.push_back(yCoordinatesVec[i][j]);
        zCoordinates.push_back(zCoordinatesVec[i][j]);
      }

      iREAL pos[] = {centroid[i][0], centroid[i][1], centroid[i][2]};
      double diameter = delta::primitives::properties::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
      newParticleNumber = vertex.createNewSubParticle(pos, xCoordinates, yCoordinates, zCoordinates, centerOfMass, inertia, inverse, mass, hMin, diameter, eps, friction, material, isObstacle, _numberOfParticles, i);

      _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
      if(isObstacle) _numberOfObstacles++;
      xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
    }
  } else {
    newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, 0);
  }
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);

  return newParticleNumber;
}

int dem::mappings::CreateGrid::makeSphere(dem::Vertex&  vertex,
                                          double position[3],
                                          double radius,
                                          double eps,
                                          delta::collision::material::MaterialType material,
                                          bool friction,
                                          bool isObstacle)
{
  //printf("%i\n", _numberOfParticles);
  int localparticleNumber = 0;
  int particleid = vertex.createNewParticleSphere(position, radius, eps, friction, material, isObstacle, _numberOfParticles, localparticleNumber);
  _numberOfParticles++;
  if(isObstacle) dem::mappings::CreateGrid::_numberOfObstacles++;
  return particleid;
}

int dem::mappings::CreateGrid::makeLoadedNonSpherical(dem::Vertex&  vertex,
                                                      double position[3],
                                                      double radius,
                                                      double eps,
                                                      delta::collision::material::MaterialType material,
                                                      bool friction,
                                                      bool isObstacle)
{
  std::vector<double>  xCoordinates, yCoordinates, zCoordinates;

  delta::primitives::granulates::loadParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates);
  int localparticleNumber = 0;
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, localparticleNumber);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

int dem::mappings::CreateGrid::makeNonSpherical(dem::Vertex&  vertex,
                                                double position[3],
                                                double radius,
                                                double eps,
                                                int noPointsPerParticle,
                                                delta::collision::material::MaterialType material,
                                                bool friction,
                                                bool isObstacle)
{
  std::vector<double>  xCoordinates, yCoordinates, zCoordinates;

  delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, noPointsPerParticle);
  int localparticleNumber = 0;
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles, localparticleNumber);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

void dem::mappings::CreateGrid::deployParticleInsituSubGrid(dem::Vertex&  vertex,
                                                        double centreAsArray[3],
                                                        double cellSize,
                                                        double eps,
                                                        delta::collision::material::MaterialType material,
                                                        double friction,
                                                        double isObstacle)
{
  iREAL cellXLeftBoundary = centreAsArray[0] - cellSize/2, cellZLeftBoundary = centreAsArray[2] - cellSize/2;
  iREAL cellXRightBoundary= centreAsArray[0] + cellSize/2, cellZRightBoundary = centreAsArray[2] + cellSize/2;

  iREAL cellYUPBoundary = centreAsArray[1] + cellSize/2;
  iREAL cellYDWBoundary = centreAsArray[1] - cellSize/2;

  for(unsigned i=0; i<_particleGrid.size(); i++)
  {
    if((_particleGrid[i][0] >= cellXLeftBoundary && _particleGrid[i][0] <= cellXRightBoundary) &&
       (_particleGrid[i][1] >= cellYDWBoundary && _particleGrid[i][1] <= cellYUPBoundary) &&
       (_particleGrid[i][2] >= cellZLeftBoundary && _particleGrid[i][2] <= cellZRightBoundary))
    {
      iREAL position[] = {_particleGrid[i][0], _particleGrid[i][1], _particleGrid[i][2]};
      if(_componentGrid[i] == "sphere")
      {
        makeSphere(vertex, position, _rad[i], _rad[i]*eps, material, friction, isObstacle);
      } else if(_componentGrid[i] == "nonSpherical")
      {
        int localparticleNumber = 0;
        vertex.createNewParticle(position, _xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], _rad[i]*eps, friction, material, isObstacle, _numberOfParticles, localparticleNumber);
        dem::mappings::CreateGrid::addParticleToState(_xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], isObstacle);
      } else if(_componentGrid[i] == "FB")
      {
        std::vector<double>  xCoordinates, yCoordinates, zCoordinates;

        delta::primitives::graphite::generateBrickFB(position, _rad[i]*0.9, xCoordinates, yCoordinates, zCoordinates);
        int localparticleNumber = 0;
        vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, _rad[i]*eps, friction, material, isObstacle, _numberOfParticles, localparticleNumber);
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
