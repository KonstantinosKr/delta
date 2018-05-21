#include "dem/mappings/CreateGrid.h"

#include "peano/grid/aspects/VertexStateAnalysis.h"

#include <string>
#include <iostream>
#include <cmath>
#include <ctime>
#include <delta/world/operators.h>
#include <delta/scenario/scenarios.h>

#define epsilon 0.001

peano::CommunicationSpecification   dem::mappings::CreateGrid::communicationSpecification() const {
	return peano::CommunicationSpecification(
	    peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,
	    peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexFirstTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexLastTimeSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::enterCellSpecification(int level) const {
	return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces,true);
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

tarch::logging::Log                   	dem::mappings::CreateGrid::_log( "dem::mappings::CreateGrid" );
dem::mappings::CreateGrid::Scenario   	dem::mappings::CreateGrid::_scenario[4];
dem::mappings::CreateGrid::GridType   	dem::mappings::CreateGrid::_gridType;
iREAL                                	dem::mappings::CreateGrid::_maxH;

iREAL                                	dem::mappings::CreateGrid::_minComputeDomain[3];
iREAL                                	dem::mappings::CreateGrid::_maxComputeDomain[3];
iREAL                                	dem::mappings::CreateGrid::_minParticleDiam;
iREAL                                	dem::mappings::CreateGrid::_maxParticleDiam;

iREAL								 	dem::mappings::CreateGrid::_epsilon;
int 								     	dem::mappings::CreateGrid::_noPointsPerParticle;
bool                                  	dem::mappings::CreateGrid::_isSphere;
bool                                  	dem::mappings::CreateGrid::_gravity;

std::vector<delta::geometry::Object>     dem::mappings::CreateGrid::_coarseObjects;
std::vector<delta::geometry::Object>     dem::mappings::CreateGrid::_insitufineObjects;
std::vector<delta::geometry::Object>     dem::mappings::CreateGrid::_fineObjects;

int                                   	dem::mappings::CreateGrid::_numberOfParticles;
int                                   	dem::mappings::CreateGrid::_numberOfTriangles;
int                                   	dem::mappings::CreateGrid::_numberOfObstacles;

void dem::mappings::CreateGrid::setScenario(
    Scenario scenario[4],
    iREAL maxH,
    GridType gridType,
    int noPointsPerGranulate)
{
	_scenario[0]          = scenario[0];
	_scenario[1]          = scenario[1];
	_scenario[2]          = scenario[2];
	_scenario[3]          = scenario[3];
	_maxH                 = maxH;
	_minParticleDiam      = _maxH;
	_maxParticleDiam      = _maxH;
	_gridType             = gridType;
	_epsilon              = epsilon;
	_noPointsPerParticle  = noPointsPerGranulate;
	_gravity					= true;
}

void dem::mappings::CreateGrid::deployEnviroment(
    dem::Vertex& vertex,
    iREAL cellSize,
    iREAL centreAsArray[3],
	bool isFine)
{
  iREAL cellXLeftBoundary = centreAsArray[0] - cellSize/2, cellZLeftBoundary = centreAsArray[2] - cellSize/2;
  iREAL cellXRightBoundary= centreAsArray[0] + cellSize/2, cellZRightBoundary = centreAsArray[2] + cellSize/2;

  iREAL cellYUPBoundary = centreAsArray[1] + cellSize/2;
  iREAL cellYDWBoundary = centreAsArray[1] - cellSize/2;

  if(isFine)
  {
	if((_numberOfParticles-_numberOfObstacles) <= _insitufineObjects.size())
	for(unsigned i=0; i<_insitufineObjects.size(); i++)
	{
	  delta::geometry::Object object = _insitufineObjects[i];
	  if((object.getCentre()[0] >= cellXLeftBoundary 	&& object.getCentre()[0] <= cellXRightBoundary) &&
		 (object.getCentre()[1] >= cellYDWBoundary 		&& object.getCentre()[1] <= cellYUPBoundary) &&
		 (object.getCentre()[2] >= cellZLeftBoundary 	&& object.getCentre()[2] <= cellZRightBoundary))
	  {
		//std::cout << vertex.toString();
		dem::mappings::CreateGrid::deployObject(vertex, object);
		//printf("particle fine: %i\n", _numberOfParticles);
	  }
	}

	//////////////////////////////////////////////////////////////
	//////////////PER CELL CENTER DEPLOYMENT//////////////////////
	//////////////////////////////////////////////////////////////
	//if((_numberOfParticles-_numberOfObstacles) <= _fineObjects.size())
	for(unsigned i=0; i<_fineObjects.size(); i++)
	{
	  delta::geometry::Object obj = _fineObjects[i];

	  iREAL position[3] = {centreAsArray[0], centreAsArray[1], centreAsArray[2]};
	  obj.setCentre(position);
	  dem::mappings::CreateGrid::deployObject(vertex, obj);
	}
  } else {

	  //std::vector<int> deleteCoarseObjects;

	  //if(_numberOfParticles <= _coarseObjects.size())
	  for(unsigned i=0; i<_coarseObjects.size(); i++)
	  {
		delta::geometry::Object object = _coarseObjects[i];
	    if((object.getCentre()[0] >= cellXLeftBoundary 	&& object.getCentre()[0] <= cellXRightBoundary) &&
	       (object.getCentre()[1] >= cellYDWBoundary 		&& object.getCentre()[1] <= cellYUPBoundary) &&
	       (object.getCentre()[2] >= cellZLeftBoundary 	&& object.getCentre()[2] <= cellZRightBoundary))
	    {

	      dem::mappings::CreateGrid::deployObject(vertex, object);
	      //deleteCoarseObjects.push_back(i);
	    }
	  }
	  /*
	  for(unsigned i=0; i<deleteCoarseObjects.size(); i++)
	  {
	    _coarseObjects.erase(_coarseObjects.begin()+deleteCoarseObjects[i]);
	  }*/
  }
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  #ifdef STATSPARTICLE
    logWarning( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter << " and id: " << particleId);
  #endif
}

void dem::mappings::CreateGrid::deployObject(
    dem::Vertex&  vertex,
	delta::geometry::Object object)
{
  int particleNumber = -1;

  if(object.getComponent() == "sphere")
  {
    particleNumber = vertex.createSphereParticle(object, _numberOfParticles);
  } else if(object.getComponent() == "mesh")
  {
	particleNumber = vertex.createSubParticle(object, _numberOfParticles, 0);
	_numberOfTriangles += object.getNumberOfTriangles();
  } else {
    particleNumber = vertex.createParticle(object, _numberOfParticles, 0);
	_numberOfTriangles += object.getNumberOfTriangles();
  }

  _numberOfParticles++;
  if(object.getIsObstacle()) _numberOfObstacles++;
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

void dem::mappings::CreateGrid::beginIteration(
		dem::State&  solverState)
{
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  dem::ParticleHeap::getInstance().setName( "particle-heap" );
  dem::DEMdoubleHeap::getInstance().setName( "geometry-heap" );

  logInfo( "beginIteration()", "maxH=" << _maxH );

  srand (time(NULL));

  _numberOfParticles = 0;
  _numberOfTriangles = 0;
  _numberOfObstacles = 0;

  _isSphere = (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere ||
               dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::none);

  iREAL centre[3] = {0.5, 0.5, 0.5};

  if(_scenario[1] == nuclear)
  {
	delta::world::scenarios::nuclear(1, _isSphere, centre, _noPointsPerParticle, _epsilon, _coarseObjects, _fineObjects);
  } else if(_scenario[1] == hopper)
  {
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

	bool uni = false;
	if(_scenario[2] == uniform) uni = true;
	delta::world::scenarios::hopper(
		_coarseObjects, _insitufineObjects,
		centre, xzcuts, ycuts, uni,
		_isSphere, _noPointsPerParticle, _epsilon);
  } else if(_scenario[0] == turbine)
  {
	delta::world::scenarios::turbine(_coarseObjects, _isSphere, _noPointsPerParticle, _epsilon);
	printf("passed\n");
  } else if(_scenario[1] == friction)
  {
	int sc = 0;
	if(_scenario[2] == sstatic)
	{
	  sc = 1;
	} else if(_scenario[2] == slide)
	{
	  sc = 2;
	} else if(_scenario[2] == roll)
	{
	  sc = 3;
	}
	delta::world::scenarios::friction(sc,  _isSphere, centre, _noPointsPerParticle, _epsilon, _coarseObjects);
  } else if(_scenario[0] == ParticleRotation)
  {
	delta::world::scenarios::rotateParticle(_coarseObjects, _isSphere, _noPointsPerParticle, _epsilon);
	_gravity = false;
  }
  else if(_scenario[0] == TwoParticlesCrash)
  {
  delta::world::scenarios::twoParticlesCrash(_coarseObjects, _isSphere, _noPointsPerParticle, _epsilon);
  _gravity = false;
  }
  else if(_scenario[0] == blackHoleWithCubes ||
          _scenario[0] == freefallWithCubes ||
          _scenario[0] == blackHoleWithRandomOrientedCubes ||
          _scenario[0] == freefallWithRandomOrientedCubes)
  {
	int scn = 0;
	if( _scenario[0] == blackHoleWithRandomOrientedCubes ||
        _scenario[0] == freefallWithRandomOrientedCubes)
	{
	  scn = 1;
	}
	delta::world::scenarios::freeFall(scn, _isSphere, centre, _noPointsPerParticle, _epsilon, _coarseObjects);
  }
  else if(_scenario[0] == nonescenario)
  {
    return;
  }

  dem::mappings::Collision::gravity	= _gravity==true ? 9.81 : 0.0;

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::CreateGrid::endIteration(
		dem::State&  solverState
)
{
  logTraceInWith1Argument( "endIteration(State)", solverState );

  if(!_isSphere)
  delta::world::operators::computeBoundary(
		 _coarseObjects,
		 _fineObjects,
		 _insitufineObjects,
		 _minParticleDiam,
		 _maxParticleDiam,
		 &_minComputeDomain[0],
		 &_maxComputeDomain[0]);

  solverState.incNumberOfParticles(_numberOfParticles);
  solverState.incNumberOfObstacles(_numberOfObstacles);

  solverState.setPrescribedMinimumMeshWidth(_minParticleDiam);
  solverState.setPrescribedMaximumMeshWidth(_maxParticleDiam);

  logInfo( "endIteration(State)", "created "
			<< _numberOfParticles << " particles with "
			<< _numberOfObstacles << " obstacles "
			<< _numberOfTriangles << " triangles");

  //delta::sys::Sys::saveScenario(_numberOfParticles, _numberOfObstacles);

  _coarseObjects.clear();
  _insitufineObjects.clear();
  _fineObjects.clear();

  logTraceOutWith1Argument( "endIteration(State)", solverState);
}

void dem::mappings::CreateGrid::touchVertexFirstTime(
		dem::Vertex&               fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                          fineGridH,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

	logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
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

  dfor2(k) //size 2, dimension 3
    for(unsigned i=0; i<fineGridVertices[fineGridVerticesEnumerator(k)].getNumberOfParticles(); i++)
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

void dem::mappings::CreateGrid::touchVertexLastTime(
		dem::Vertex&         fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  dropParticles(fineGridVertex,coarseGridVertices,coarseGridVerticesEnumerator,fineGridPositionOfVertex, fineGridH(0));

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::createInnerVertex(
		dem::Vertex&                                  fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridH,
		dem::Vertex * const                           coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&                                    coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  dropParticles(fineGridVertex, coarseGridVertices, coarseGridVerticesEnumerator, fineGridPositionOfVertex, fineGridH(0));
  bool spheremodel = (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere);

  if(_gridType != NoGrid && fineGridH(0) > _maxH && fineGridVertex.getRefinementControl() == Vertex::Records::Unrefined)
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
		const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  fineGridH,
		dem::Vertex * const                          coarseGridVertices,
		const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
		dem::Cell&                                   coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  fineGridVertex.init();

  dropParticles(fineGridVertex, coarseGridVertices, coarseGridVerticesEnumerator, fineGridPositionOfVertex, fineGridH(0));
  bool spheremodel = (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere);

  if(_gridType != NoGrid && fineGridH(0) > _maxH && fineGridVertex.getRefinementControl() == Vertex::Records::Unrefined)
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

  if(_scenario[0] == nonescenario) return;

  dem::Vertex&  vertex  = fineGridVertices[fineGridVerticesEnumerator(0)];
  //printf("cell created length: %f\n", fineGridVerticesEnumerator.getCellSize()(0));

  iREAL centreAsArray[3] = {fineGridVerticesEnumerator.getCellCenter()(0)-1E-4,
                            fineGridVerticesEnumerator.getCellCenter()(1)-1E-4,
                            fineGridVerticesEnumerator.getCellCenter()(2)-1E-4};

  if(coarseGridCell.isRoot())
  {
    deployEnviroment(vertex, fineGridVerticesEnumerator.getCellSize()(0), centreAsArray, false);
  }

  if(peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices, fineGridVerticesEnumerator, Vertex::Records::Unrefined) &&
     !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices, fineGridVerticesEnumerator))
  {
    deployEnviroment(vertex, fineGridVerticesEnumerator.getCellSize()(0), centreAsArray, true);
  }

  logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::CreateGrid::createHangingVertex(
		dem::Vertex&     fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                fineGridH,
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
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
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
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&                    fineGridH,
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
		const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridX,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   fineGridH,
		int                                           level
) {
	logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
	// @todo Insert your code here
	logTraceOut( "mergeWithNeighbour(...)" );
}

void dem::mappings::CreateGrid::prepareSendToNeighbour(
		dem::Vertex&  vertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
		int                                           level
) {
	logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
	// @todo Insert your code here
	logTraceOut( "prepareSendToNeighbour(...)" );
}

void dem::mappings::CreateGrid::prepareCopyToRemoteNode(
		dem::Vertex&  localVertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
		int                                           level
) {
	logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
	// @todo Insert your code here
	logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::CreateGrid::prepareCopyToRemoteNode(
		dem::Cell&  localCell,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   cellCentre,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   cellSize,
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
		const tarch::la::Vector<DIMENSIONS,iREAL>&  x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  h,
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
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellSize,
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
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,iREAL>&  cellSize,
		int                                          level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}

void dem::mappings::CreateGrid::mergeWithWorker(
		dem::Vertex&        localVertex,
		const dem::Vertex&  receivedMasterVertex,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   x,
		const tarch::la::Vector<DIMENSIONS,iREAL>&   h,
		int                                           level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif

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
