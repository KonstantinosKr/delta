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
dem::mappings::CreateGrid::Scenario  dem::mappings::CreateGrid::_scenario;
double                               dem::mappings::CreateGrid::_maxH;
double                               dem::mappings::CreateGrid::_minParticleDiam;
double                               dem::mappings::CreateGrid::_maxParticleDiam;
dem::mappings::CreateGrid::GridType  dem::mappings::CreateGrid::_gridType;
double								               dem::mappings::CreateGrid::_epsilon;
int 								                 dem::mappings::CreateGrid::_noPointsPerParticle;
std::vector<std::array<double, 3>>   dem::mappings::CreateGrid::_particleGrid;

void dem::mappings::CreateGrid::setScenario(Scenario scenario,
                                            double maxH, double particleDiamMin, double particleDiamMax,
											                      GridType gridType, int noPointsPerGranulate)
{
	_scenario        		  = scenario;
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

	logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::CreateGrid::dropParticles(
  dem::Vertex&                                 fineGridVertex,
  dem::Vertex * const                          coarseGridVertices,
  const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
  const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  assertion1( !fineGridVertex.isHangingNode(), fineGridVertex.toString() );

  if (peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(fineGridPositionOfVertex))
  {
    Vertex& coarseGridVertexAtSamePosition = coarseGridVertices[coarseGridVerticesEnumerator(peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex))];

    if (!coarseGridVertexAtSamePosition.isOutside())
    {
      int i=0;
      while (i<coarseGridVertexAtSamePosition.getNumberOfParticles())
      {
        if (coarseGridVertexAtSamePosition.getParticle(i).getDiameter()<coarseGridVerticesEnumerator.getCellSize()(0) && !coarseGridVertexAtSamePosition.getParticle(i).getIsObstacle())
        {
          logDebug( "dropParticle()",
          "dropped particle " << coarseGridVertexAtSamePosition.getParticle(i).toString() <<
          " from " << peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(fineGridPositionOfVertex) <<
          " into " << fineGridPositionOfVertex <<
          ", i.e. from " << coarseGridVertexAtSamePosition.toString() <<
          " into " << fineGridVertex.toString());
          fineGridVertex.appendParticle( coarseGridVertexAtSamePosition.getParticle(i) );
          coarseGridVertexAtSamePosition.releaseParticle(i);
        } else {
          i++;
        }
      }
    }
  }
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

  if(_gridType == dem::mappings::CreateGrid::RegularGrid)
  dropParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex);

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

  if(_gridType == dem::mappings::CreateGrid::RegularGrid)
  dropParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex);

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
  if(_scenario == sla ||
     _scenario == nuclearArray)
  {
    //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::GOLD;
    centreAsArray[1] = 0.5;
    iREAL position[] = {centreAsArray[0], centreAsArray[1], centreAsArray[2]};
    double floorHeight = 0.05;

    int particleid = makeBox(vertex, position, 1, floorHeight, 0, 0, 0, eps, material, true, true);
    dem::mappings::CreateGrid::setVScheme(vertex,  particleid, dem::mappings::CreateGrid::moveLeft);
    if(_scenario == sla)
    {
      /*nuclear single layer array; experiment of seismic shakes concept: drop all components in coarse grid then reassign vertex to refined grid*/
      delta::world::assembly::makeLoadNuclearGeometry(centreAsArray, _particleGrid, _componentGrid, _rad, _minParticleDiam, _maxParticleDiam);
    } else if(_scenario == nuclearArray)
    {
      delta::world::assembly::makeFullBrickFBGrid(centreAsArray, 1, 10, _particleGrid, _componentGrid, _rad, _minParticleDiam, _maxParticleDiam);
    }
    return;
  }

  if(_scenario == hopperUniform ||
      _scenario == hopperUniform1k ||
      _scenario == hopperUniform10k ||
      _scenario == hopperUniform100k ||
      _scenario == hopperNonUniform ||
      _scenario == hopperNonUniform1k ||
      _scenario == hopperNonUniform10k ||
      _scenario == hopperNonUniform100k)
  {

    ////////HOPPER////////////////////////////////////////////////////////////////////////////////////////////
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::GOLD;
    double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.05;
    //HOPPER DIAGONAL:0.382926
    makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, eps, material, false, true);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////FLOOR////////////////floor DIAGONAL:0.344674///////////////////////////////////////////
    iREAL position[] = {centreAsArray[0], 0.35, centreAsArray[2]};
    double height = 0.05; double width = 0.35;
    makeBox(vertex, position, width, height, 0, 0, 0, eps, material, true, true);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //make coarseEnviromentGridArray
    //////////PARTICLE GRID///////////////////////////////////////////////////////////////////////////////////////
    iREAL xzcuts = 0; iREAL ycuts = 0;
    if(_scenario == hopperUniform ||
       _scenario == hopperNonUniform)
    {
      xzcuts = 10; ycuts = 1;
    }
    else if(_scenario == hopperUniform1k ||
            _scenario == hopperNonUniform1k)
    {
      xzcuts = 10; ycuts = 10;
    }
    else if(_scenario == hopperUniform10k ||
            _scenario == hopperNonUniform10k)
    {
      xzcuts = 30; ycuts = 12;
    }
    else if(_scenario == hopperUniform100k ||
            _scenario == hopperNonUniform100k)
    {
      xzcuts = 41.0; ycuts = 60;
    }

    double totalMass = 0.05; material = delta::collision::material::MaterialType::WOOD;

    iREAL minArraylengthX = (double)_hopperWidth - eps * 6;
    iREAL minArraylengthY = (double)_hopperHeight - eps * 6;

    iREAL subcellx = ((double)minArraylengthX/(double)xzcuts) - eps*4;
    iREAL margin = ((double)minArraylengthX/(double)xzcuts)/2.0;

    //iREAL minParticleDiameter = ((double)_hopperWidth/(double)xzcuts)-(margin*2.0);
    //printf("minParDiameter:%.10f\n", minParticleDiameter);

    position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
    _particleGrid = delta::world::assembly::getGridArrayList(position, xzcuts, ycuts, _hopperWidth);

    if(_scenario == hopperUniform ||
       _scenario == hopperUniform1k ||
       _scenario == hopperUniform10k ||
       _scenario == hopperUniform100k)
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
    else if(_scenario == hopperNonUniform ||
            _scenario == hopperNonUniform1k ||
            _scenario == hopperNonUniform10k ||
            _scenario == hopperNonUniform100k)
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
    return;
  }

  if(_scenario == frictionStatic)
   {
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::WOOD;
    double height = 0.05; double width = 0.35;
    makeBox(vertex, centreAsArray, width, height, 0,0,0, eps, material, true, true);

    double radius = 0.01; double eps = radius*0.2;

    int newParticleNumber;
    if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
    {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, material, true, false);
    } else {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + ((height)/1.5)-width/2, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeBox(vertex, position, width, height, 0,0,0, eps, material, true, false);
    }
    setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::crashY);
    return;
  } else if(_scenario == frictionSlide)
  {
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::WOOD;
    double height = 0.05; double width = 0.35;
    makeBox(vertex, centreAsArray, width, height, 0,0,0, eps, material, true, true);

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
    return;
  } else if(_scenario == frictionRoll)
  {
    delta::collision::material::MaterialType material = delta::collision::material::MaterialType::WOOD;
    double height = 0.05; double width = 0.35;
    makeBox(vertex, centreAsArray, width, height, 0,0,0.1, eps, material, true, true);

    double radius = 0.01; double eps = radius*0.2;

    int newParticleNumber;
    if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
    {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius/2 + eps*2, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, material, true, false);
    } else {
      iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius, centreAsArray[2]};
      newParticleNumber = dem::mappings::CreateGrid::makeBox(vertex, position, width, height, 0,0,0, eps, material, true, false);
    }
    setVScheme(vertex, newParticleNumber, dem::mappings::CreateGrid::slideXRotation);
    return;
  } else if(_scenario == TwoParticlesCrash)
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
    return;
  }
  return;
}

void dem::mappings::CreateGrid::makeFineEnviroment(dem::Vertex& vertex,
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
  if(_scenario == blackHoleWithRandomOrientedCubes ||
     _scenario == freefallWithRandomOrientedCubes)
  {
    int particleid = makeBox(vertex, centreAsArray, maxParticleDiam, maxParticleDiam, static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
        eps, material, true, false);
    if(_scenario == blackHoleWithRandomOrientedCubes)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario == blackHoleWithCubes ||
          _scenario == freefallWithCubes)
  {
    double particleDiameter = (minParticleDiam + (maxParticleDiam-minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) / std::sqrt(DIMENSIONS);
    int particleid = makeBox(vertex, centreAsArray, particleDiameter, particleDiameter, 0, 1.0/8.0, 1.0/8.0, eps, material, friction, isObstacle);
    if(_scenario == blackHoleWithCubes)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario == blackHoleWithGranulates ||
          _scenario == freefallWithGranulates)
  {
    double particleDiameter = (minParticleDiam + (maxParticleDiam-minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX))) / std::sqrt(DIMENSIONS);
    int particleid = dem::mappings::CreateGrid::makeNonSpherical(vertex, centreAsArray, particleDiameter/2, eps, noPointsPerParticle, material, friction, isObstacle);
    if(_scenario == blackHoleWithGranulates)
      dem::mappings::CreateGrid::setVScheme(vertex, particleid, dem::mappings::CreateGrid::randomLinearAngular);
  }
  else if(_scenario == sla ||
           _scenario == nuclearArray ||
           _scenario == hopperUniform ||
           _scenario == hopperUniform1k ||
           _scenario == hopperUniform10k  ||
           _scenario == hopperUniform100k ||
           _scenario == hopperNonUniform ||
           _scenario == hopperNonUniform1k ||
           _scenario == hopperNonUniform10k  ||
           _scenario == hopperNonUniform100k)
  {
    eps = 0.1;//percentage of radius is epsilon
    dem::mappings::CreateGrid::deployParticleInsituSubGrid(vertex, centreAsArray, cellSize, eps, material, friction, isObstacle);
  }

  #ifdef STATSPARTICLE
    logWarning( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter << " and id: " << particleId);
  #endif
}

tarch::multicore::BooleanSemaphore  dem::mappings::CreateGrid::_CreateSemaphore;
std::vector<std::vector<double>> dem::mappings::CreateGrid::_xCoordinatesArray;
std::vector<std::vector<double>> dem::mappings::CreateGrid::_yCoordinatesArray;
std::vector<std::vector<double>> dem::mappings::CreateGrid::_zCoordinatesArray;
std::vector<double> dem::mappings::CreateGrid::_rad;
std::vector<std::string> dem::mappings::CreateGrid::_componentGrid;

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

	if(_scenario == nonescenario) return;
	if (coarseGridCell.isRoot())
	{
		dem::Vertex&  vertex  = fineGridVertices[fineGridVerticesEnumerator(0)];

		double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
                              fineGridVerticesEnumerator.getCellCenter()(1),
                              fineGridVerticesEnumerator.getCellCenter()(2)};

		makeCoarseEnviroment(vertex, centreAsArray, _epsilon, _noPointsPerParticle);
	}

	if(peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices, fineGridVerticesEnumerator,  Vertex::Records::Unrefined) &&
	    !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices,  fineGridVerticesEnumerator))
	{
		int particlesInCellPerAxis = std::floor(fineGridVerticesEnumerator.getCellSize()(0) / _maxParticleDiam);
    dem::Vertex&  vertex = fineGridVertices[fineGridVerticesEnumerator(0)];

		if(particlesInCellPerAxis==0)
		{
			particlesInCellPerAxis = 1;
			//_maxParticleDiam = fineGridVerticesEnumerator.getCellSize()(0);
			//_minParticleDiam = std::min(_minParticleDiam, _maxParticleDiam);
			//logWarning( "createCell(...)", "particle size has been too small for coarsest prescribed grid. Reduce particle size to " << _minParticleDiam << "-" << fineGridVerticesEnumerator.getCellSize()(0) );
		}

    //printf("cellsize:%f particlesInCellPerAxis:%i\n", fineGridVerticesEnumerator.getCellSize()(0), particlesInCellPerAxis);

		dfor(k,particlesInCellPerAxis)
		{
		  const auto centre = fineGridVerticesEnumerator.getVertexPosition() + _maxParticleDiam * (k.convertScalar<double>()+0.5);
      double centreAsArray[] = {centre(0),centre(1),centre(2)};

      auto material = delta::collision::material::MaterialType::WOOD;
      bool friction = true; bool obstacle = false;
      makeFineEnviroment(vertex, centreAsArray, fineGridVerticesEnumerator.getCellSize()(0), _epsilon, _noPointsPerParticle, material, friction, obstacle, _minParticleDiam, _maxParticleDiam);
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


int dem::mappings::CreateGrid::makeHopper(dem::Vertex&  vertex,
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
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

int dem::mappings::CreateGrid::makeBox(dem::Vertex&  vertex,
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
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, epsilon, friction, material, isObstacle, _numberOfParticles);
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
 int particleid = vertex.createNewParticleSphere(position, radius, eps, friction, material, isObstacle, _numberOfParticles);
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
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles);
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
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, eps, friction, material, isObstacle, _numberOfParticles);
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
        vertex.createNewParticle(position, _xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], _rad[i]*eps, friction, material, isObstacle, _numberOfParticles);
        dem::mappings::CreateGrid::addParticleToState(_xCoordinatesArray[i], _yCoordinatesArray[i], _zCoordinatesArray[i], isObstacle);
      } else if(_componentGrid[i] == "FB")
      {
        std::vector<double>  xCoordinates, yCoordinates, zCoordinates;

        delta::primitives::graphite::generateBrickFB(position, _rad[i]*0.9, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates, _rad[i]*eps, friction, material, isObstacle, _numberOfParticles);
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
