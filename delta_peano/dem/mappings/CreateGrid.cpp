#include <delta/primitives/graphite.h>
#include <delta/primitives/surface.h>
#include "dem/mappings/CreateGrid.h"

#include "delta/primitives/cubes.h"
#include "delta/primitives/granulates.h"
#include "delta/primitives/properties.h"
#include "delta/primitives/hopper.h"
#include "delta/primitives/blender.h"
#include "peano/grid/aspects/VertexStateAnalysis.h"

#include <string>

#define INVERT(F, INV, DET)\
if (((DET) =\
 ((F)[0]*((F)[4]*(F)[8]-(F)[5]*(F)[7])+\
  (F)[3]*((F)[2]*(F)[7]-(F)[1]*(F)[8])+\
 ((F)[1]*(F)[5]-(F)[2]*(F)[4])*(F)[6])) != 0.0)\
{\
  (DET) = 1.0 / (DET);\
  (INV) [0] = ((F)[4]*(F)[8]-(F)[5]*(F)[7])*(DET);\
  (INV) [1] = ((F)[2]*(F)[7]-(F)[1]*(F)[8])*(DET);\
  (INV) [2] = ((F)[1]*(F)[5]-(F)[2]*(F)[4])*(DET);\
  (INV) [3] = ((F)[5]*(F)[6]-(F)[3]*(F)[8])*(DET);\
  (INV) [4] = ((F)[0]*(F)[8]-(F)[2]*(F)[6])*(DET);\
  (INV) [5] = ((F)[2]*(F)[3]-(F)[0]*(F)[5])*(DET);\
  (INV) [6] = ((F)[3]*(F)[7]-(F)[4]*(F)[6])*(DET);\
  (INV) [7] = ((F)[1]*(F)[6]-(F)[0]*(F)[7])*(DET);\
  (INV) [8] = ((F)[0]*(F)[4]-(F)[1]*(F)[3])*(DET);\
  (DET) = 1.0 / (DET);\
}

/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::CreateGrid::communicationSpecification() {
	return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexLastTimeSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexFirstTimeSpecification() { 
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
peano::MappingSpecification   dem::mappings::CreateGrid::enterCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}
peano::MappingSpecification   dem::mappings::CreateGrid::leaveCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}
peano::MappingSpecification   dem::mappings::CreateGrid::ascendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
}
peano::MappingSpecification   dem::mappings::CreateGrid::descendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
}

tarch::logging::Log                  dem::mappings::CreateGrid::_log( "dem::mappings::CreateGrid" );
dem::mappings::CreateGrid::Scenario  dem::mappings::CreateGrid::_scenario;
double                               dem::mappings::CreateGrid::_maxH;
double                               dem::mappings::CreateGrid::_particleDiamMax;
double                               dem::mappings::CreateGrid::_particleDiamMin;
dem::mappings::CreateGrid::GridType  dem::mappings::CreateGrid::_gridType;
double								 dem::mappings::CreateGrid::_epsilon;
int 								 dem::mappings::CreateGrid::_noPointsPerParticle;

void dem::mappings::CreateGrid::setScenario(Scenario scenario, double maxH, double particleDiamMax, double particleDiamMin, GridType gridType, double epsilon, int noPointsPerParticle) {
	_scenario        = scenario;
	_maxH            = maxH;
	_particleDiamMax = particleDiamMax;
	_particleDiamMin = particleDiamMin;
	_gridType        = gridType;
	_epsilon 		 = epsilon;
	_noPointsPerParticle = noPointsPerParticle;
}

void dem::mappings::CreateGrid::beginIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "beginIteration(State)", solverState );

	dem::ParticleHeap::getInstance().setName( "particle-heap" );

	logInfo( "beginIteration()", "maxH=" << _maxH );

	srand (time(NULL));

	_numberOfParticles = 0;
	_numberOfTriangles = 0;
	_numberOfObstacles = 0;

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

	bool PlaceParticleInCell       = true;

	switch (_scenario)
	{
		case TwoParticlesCrash:
			PlaceParticleInCell = tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(0), 2.0/3.0, 1E-4 ) | tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(TWO_POWER_D-1), 1.0/3.0, 1E-4);
			//PlaceParticleInCell = tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(0), 2.0/3.0, 0.0 ) | tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(TWO_POWER_D-1), 2.0/3.0, 0.0);
			break;
		case flatwall:
			if (coarseGridCell.isRoot())
			{
				std::vector<double>  xCoordinates;
				std::vector<double>  yCoordinates;
				std::vector<double>  zCoordinates;

				dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];

				double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
										  fineGridVerticesEnumerator.getCellCenter()(1),
										  fineGridVerticesEnumerator.getCellCenter()(2)};

				const int	newParticleNumber = vertex.createNewParticle(centreAsArray);

				double _wallWidth = 0.3;

				_numberOfParticles++; _numberOfObstacles++;

				delta::primitives::generateSurface( centreAsArray, _wallWidth, xCoordinates, yCoordinates, zCoordinates);

				for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
				{
					vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
					vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
					vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );

					vertex.getXRefCoordinatesAsVector(newParticleNumber).push_back(xCoordinates[i]);
					vertex.getYRefCoordinatesAsVector(newParticleNumber).push_back(yCoordinates[i]);
					vertex.getZRefCoordinatesAsVector(newParticleNumber).push_back(zCoordinates[i]);
				}

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
				vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = _wallWidth;
				vertex.getParticle(newParticleNumber)._persistentRecords._influenceRadius      = _wallWidth * 1.8;
				vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = _epsilon;
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = 1;
				vertex.getParticle(newParticleNumber)._persistentRecords._material 			   = 1;
				return;
			}
			break;
		case icecube:
			if (coarseGridCell.isRoot() )
			{
				std::vector<double>  xCoordinates;
				std::vector<double>  yCoordinates;
				std::vector<double>  zCoordinates;

				dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];

				double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
										  fineGridVerticesEnumerator.getCellCenter()(1),
										  fineGridVerticesEnumerator.getCellCenter()(2)};

				const int	newParticleNumber = vertex.createNewParticle(centreAsArray);

				double _wallWidth = 0.3;
				delta::primitives::generateCube( centreAsArray, _wallWidth, _wallWidth, _wallWidth, _wallWidth, xCoordinates, yCoordinates, zCoordinates);

				for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
				{
					vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
					vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
					vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );

					vertex.getXRefCoordinatesAsVector(newParticleNumber).push_back(xCoordinates[i]);
					vertex.getYRefCoordinatesAsVector(newParticleNumber).push_back(yCoordinates[i]);
					vertex.getZRefCoordinatesAsVector(newParticleNumber).push_back(zCoordinates[i]);
				}

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
				vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = _wallWidth;
				vertex.getParticle(newParticleNumber)._persistentRecords._influenceRadius      = _wallWidth * 1.2;
				vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = _epsilon-0.0005;
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = 1; _numberOfParticles++; _numberOfObstacles++;
				vertex.getParticle(newParticleNumber)._persistentRecords._material 			   = 1;
				return;
			}
			break;
		case hopper:
			if (coarseGridCell.isRoot() )
			{
				std::vector<double>  xCoordinates;
				std::vector<double>  yCoordinates;
				std::vector<double>  zCoordinates;

				dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];

				double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
										  fineGridVerticesEnumerator.getCellCenter()(1),
										  fineGridVerticesEnumerator.getCellCenter()(2)};

				int	newParticleNumber = vertex.createNewParticle(centreAsArray);

				//hopper
				double _hopperWidth = 0.26;
				double _hopperHatch = 0.15;

				//hopper 300
				//double _hopperWidth = 0.26;
				//double _hopperHatch = 0.10;

				//hopper 1000
				//double _hopperWidth = 0.4;
				//double _hopperHatch = 0.10;

				_numberOfParticles++; _numberOfObstacles++;
				delta::primitives::generateHopper( centreAsArray, _hopperWidth, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);

				for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
				{
					vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
					vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
					vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );

					vertex.getXRefCoordinatesAsVector(newParticleNumber).push_back(xCoordinates[i]);
					vertex.getYRefCoordinatesAsVector(newParticleNumber).push_back(yCoordinates[i]);
					vertex.getZRefCoordinatesAsVector(newParticleNumber).push_back(zCoordinates[i]);
				}

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;

				vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = _hopperWidth*1.8;
				vertex.getParticle(newParticleNumber)._persistentRecords._influenceRadius      = _hopperWidth * 1.8;
				vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = _epsilon*1.5;
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = _numberOfParticles;
				vertex.getParticle(newParticleNumber)._persistentRecords._material 			   = 1;

				assertion( tarch::la::greater(vertex.getParticle(newParticleNumber)._persistentRecords._hMin,0.0) );

				xCoordinates.clear();
				yCoordinates.clear();
				zCoordinates.clear();

				newParticleNumber = vertex.createNewParticle(centreAsArray);

				centreAsArray[1] = 0;

				_numberOfParticles++; _numberOfObstacles++;
				delta::primitives::generateSurface( centreAsArray, 1, xCoordinates, yCoordinates, zCoordinates);

				for (int i=0; i<static_cast<int>(xCoordinates.size()); i++) {
					vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
					vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
					vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );

					vertex.getXRefCoordinatesAsVector(newParticleNumber).push_back(xCoordinates[i]);
					vertex.getYRefCoordinatesAsVector(newParticleNumber).push_back(yCoordinates[i]);
					vertex.getZRefCoordinatesAsVector(newParticleNumber).push_back(zCoordinates[i]);
				}

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
				vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = 1;
				vertex.getParticle(newParticleNumber)._persistentRecords._influenceRadius      = 1;
				vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = _epsilon*3;
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = _numberOfParticles;
				vertex.getParticle(newParticleNumber)._persistentRecords._material 			   = 2;

				vertex.getParticle(newParticleNumber)._persistentRecords._mass = 100;
				return;
			}
			break;
		case friction:
			if (coarseGridCell.isRoot())
			{
				std::vector<double>  xCoordinates;
				std::vector<double>  yCoordinates;
				std::vector<double>  zCoordinates;

				dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];

				double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
										  fineGridVerticesEnumerator.getCellCenter()(1),
										  fineGridVerticesEnumerator.getCellCenter()(2)};

				const int	newParticleNumber = vertex.createNewParticle(centreAsArray);

				_numberOfParticles++; _numberOfObstacles++;
				double _wallWidth = 1;
				delta::primitives::generateSurface( centreAsArray, _wallWidth, xCoordinates, yCoordinates, zCoordinates);

				for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
				{
					vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
					vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
					vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );

					vertex.getXRefCoordinatesAsVector(newParticleNumber).push_back(xCoordinates[i]);
					vertex.getYRefCoordinatesAsVector(newParticleNumber).push_back(yCoordinates[i]);
					vertex.getZRefCoordinatesAsVector(newParticleNumber).push_back(zCoordinates[i]);
				}

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
				vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = _wallWidth;
				vertex.getParticle(newParticleNumber)._persistentRecords._influenceRadius      = _wallWidth * 1.2;
				vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = _epsilon;
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = _numberOfParticles;
				vertex.getParticle(newParticleNumber)._persistentRecords._material 			   = 2;
				return;
			}
			break;
		case frictionSlide:
			if (coarseGridCell.isRoot() )
			{
				std::vector<double>  xCoordinates;
				std::vector<double>  yCoordinates;
				std::vector<double>  zCoordinates;

				dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];

				double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
										  fineGridVerticesEnumerator.getCellCenter()(1),
										  fineGridVerticesEnumerator.getCellCenter()(2)};

				const int     newParticleNumber = vertex.createNewParticle(centreAsArray);

				_numberOfParticles++; _numberOfObstacles++;
				double _wallWidth = 1;
				delta::primitives::generateSurface( centreAsArray, _wallWidth, xCoordinates, yCoordinates, zCoordinates);

				for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
				{
					vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
					vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
					vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );

					vertex.getXRefCoordinatesAsVector(newParticleNumber).push_back(xCoordinates[i]);
					vertex.getYRefCoordinatesAsVector(newParticleNumber).push_back(yCoordinates[i]);
					vertex.getZRefCoordinatesAsVector(newParticleNumber).push_back(zCoordinates[i]);
				}

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
				vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = _wallWidth;
				vertex.getParticle(newParticleNumber)._persistentRecords._influenceRadius      = _wallWidth * 1.8;
				vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = 0;
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = _numberOfParticles;
				vertex.getParticle(newParticleNumber)._persistentRecords._material 			   = 2;
				return;
			}
			break;
	}

	if (PlaceParticleInCell
		&& peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices,fineGridVerticesEnumerator,Vertex::Records::Unrefined)
		&& !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices,fineGridVerticesEnumerator))
	{
		int particlesInCellPerAxis = std::floor(fineGridVerticesEnumerator.getCellSize()(0) / _particleDiamMax);

		if (particlesInCellPerAxis==0)
		{
			particlesInCellPerAxis = 1;
			_particleDiamMax = fineGridVerticesEnumerator.getCellSize()(0);
			_particleDiamMin = std::min(_particleDiamMin,_particleDiamMax);
			logWarning( "createCell(...)", "particle size has been too small for coarsest prescribed grid. Reduce particle size to " << _particleDiamMin << "-" << _particleDiamMax );
		}else{logDebug( "createCell", "particlesInCellPerAxis="<< particlesInCellPerAxis );}

		dfor(k,particlesInCellPerAxis)
		{
			const tarch::la::Vector<DIMENSIONS,double> centre = fineGridVerticesEnumerator.getVertexPosition() + _particleDiamMax * (k.convertScalar<double>()+0.5);

			double particleDiameter = (_particleDiamMin + (_particleDiamMax-_particleDiamMin) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) ) / std::sqrt( DIMENSIONS );

			logDebug( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter );

			dem::Vertex&  vertex = fineGridVertices[ fineGridVerticesEnumerator(0) ];

			double centreAsArray[] = {centre(0),centre(1),centre(2)};

			int  newParticleNumber;

			// We need these temporary guys if we use an aligned vector from Peano.
			// Peano's aligned vector and the default std::vector are not compatible.
			std::vector<double>  xCoordinates;
			std::vector<double>  yCoordinates;
			std::vector<double>  zCoordinates;

			if (_scenario == BlackHoleWithAlignedCubes)
			{
				newParticleNumber = vertex.createNewParticle( centre );
				if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
				delta::primitives::generateCube( centreAsArray, particleDiameter,
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						xCoordinates, yCoordinates, zCoordinates );
			}
			else if (_scenario == BlackHoleWithCubes)
			{
				newParticleNumber = vertex.createNewParticle( centre );
				if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
				delta::primitives::generateCube( centreAsArray, particleDiameter,
						0.0, // around x-axis 1/8
						1.0/8.0, // around y-axis 1/8
						1.0/8.0, // around z-axis 1/8
						xCoordinates, yCoordinates, zCoordinates );
			}
			else if (_scenario == TwoParticlesCrash)
			{
				newParticleNumber = vertex.createNewParticle( centre );

				if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
				delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
			}
			else if (_scenario == Random)
			{
				newParticleNumber = vertex.createNewParticle( centre );

				if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
				delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
			}
			else if(_scenario == hopper)
			{
				//hopper - hopper300
				double _hopperWidth = 0.26;

				//hopper 1000
				//double _hopperWidth = 0.4;

				double coarseCenterHopperMarginUpper = 0.5 + (_hopperWidth/2);
				double coarseCenterHopperMarginLower = 0.5 - (_hopperWidth/2);
				if(centreAsArray[0] < coarseCenterHopperMarginUpper && centreAsArray[0] > coarseCenterHopperMarginLower &&
				   centreAsArray[1] < coarseCenterHopperMarginUpper+0.2 && centreAsArray[1] > coarseCenterHopperMarginUpper &&
				   centreAsArray[2] < coarseCenterHopperMarginUpper && centreAsArray[2] > coarseCenterHopperMarginLower)
				{
					newParticleNumber = vertex.createNewParticle( centre );
					if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
					{
						delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
					}
					else{
						delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
					}
					//delta::primitives::generateKey( centreAsArray,particleDiameter*18,xCoordinates,yCoordinates,zCoordinates );
				} else {return;}
			}
			else if(_scenario == freefall)
			{
				newParticleNumber = vertex.createNewParticle( centre );
				if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
				{
					delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
				}
				else{
					delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
				}
				//delta::primitives::generateBrick( centreAsArray,particleDiameter*4.5,xCoordinates,yCoordinates,zCoordinates );
				//delta::primitives::generateKey( centreAsArray,particleDiameter*18,xCoordinates,yCoordinates,zCoordinates );
			}
			else if(_scenario==flatwall)
			{
				double _wallWidth = 0.25;

				double coarseCenterHopperMarginUpper = 0.5 + (_wallWidth/2);
				double coarseCenterHopperMarginLower = 0.5 - (_wallWidth/2);
				if(centreAsArray[0] < coarseCenterHopperMarginUpper && centreAsArray[0] > coarseCenterHopperMarginLower &&
					centreAsArray[1] < coarseCenterHopperMarginUpper+0.2 && centreAsArray[1] > coarseCenterHopperMarginUpper &&
					centreAsArray[2] < coarseCenterHopperMarginUpper && centreAsArray[2] > coarseCenterHopperMarginLower)
				{
					newParticleNumber = vertex.createNewParticle( centre );
					if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
					delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
				} else {return;}
			}
			else if(_scenario == icecube)
			{

			}
			else if(_scenario == friction)
			{
				if(centreAsArray[0] < 0.45 && centreAsArray[0] > 0.30 &&
				   centreAsArray[1] < 1-0.30 && centreAsArray[1] > 1-0.45 &&
				   centreAsArray[2] < 1-0.30 && centreAsArray[2] > 1-0.45)
				{
					newParticleNumber = vertex.createNewParticle( centre );
					if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
					{
						delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
					}else
					{
						delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
					}
				} else {return;}
			}
			else if(_scenario == frictionSlide)
			{
				if(centreAsArray[0] < 0.45 && centreAsArray[0] > 0.30 &&
				   centreAsArray[1] < 0.5 && centreAsArray[1] > 0.35 &&
				   centreAsArray[2] < 1-0.30 && centreAsArray[2] > 1-0.45)
				{
					centreAsArray[1] = centreAsArray[1] + (0.15 - _epsilon*2);
					newParticleNumber = vertex.createNewParticle( centreAsArray );
					if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
					{
						delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
						delta::primitives::meshDenseMultiplier(1, xCoordinates, yCoordinates, zCoordinates);
					}else
					{
						delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
					}
				} else {return;}
			}

			for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
			{
				vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
				vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
				vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );

				vertex.getXRefCoordinatesAsVector(newParticleNumber).push_back(xCoordinates[i]);
				vertex.getYRefCoordinatesAsVector(newParticleNumber).push_back(yCoordinates[i]);
				vertex.getZRefCoordinatesAsVector(newParticleNumber).push_back(zCoordinates[i]);
			}

			/*
			if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
			delta::primitives::centerOfMass(xCoordinates,yCoordinates,zCoordinates,
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(0),
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(1),
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(2),
					vertex.getParticle(newParticleNumber)._persistentRecords._referentialCentreOfMass(0),
					vertex.getParticle(newParticleNumber)._persistentRecords._referentialCentreOfMass(1),
					vertex.getParticle(newParticleNumber)._persistentRecords._referentialCentreOfMass(2));*/

			_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
			_numberOfParticles++;

			vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
			vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = particleDiameter;
			vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = _epsilon;
			vertex.getParticle(newParticleNumber)._persistentRecords._influenceRadius 	   = (particleDiameter/2) + (vertex.getParticle(newParticleNumber)._persistentRecords._epsilon * 6);
			if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
			{
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin             = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
			}else
			{
				vertex.getParticle(newParticleNumber)._persistentRecords._hMin             = 0;
			}
			vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = _numberOfParticles;
			vertex.getParticle(newParticleNumber)._persistentRecords._material 			   = 1;

			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(0) = 0.0005*vertex.getParticle(newParticleNumber)._persistentRecords._mass;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(1) = 0;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(2) = 0;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(3) = 0;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(4) = 0.0005*vertex.getParticle(newParticleNumber)._persistentRecords._mass;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(5) = 0;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(6) = 0;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(7) = 0;
			vertex.getParticle(newParticleNumber)._persistentRecords._inertia(8) = 0.0005*vertex.getParticle(newParticleNumber)._persistentRecords._mass;

			// invert inertia properties
			iREAL a[9], x[9], det;
			for (int j = 0; j < 9; j ++){a[j] = vertex.getParticle(newParticleNumber)._persistentRecords._inertia(j);}
			INVERT (a, x, det);
			for (int j = 0; j < 9; j ++){vertex.getParticle(newParticleNumber)._persistentRecords._inverse(j) = x[j];}

			switch (_scenario)
			{
				case TwoParticlesCrash:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity = tarch::la::Vector<DIMENSIONS,double>(0.5) - fineGridVerticesEnumerator.getVertexPosition(k);
					vertex.getParticle(newParticleNumber)._persistentRecords._angular = tarch::la::Vector<DIMENSIONS,double>(0.0);
					break;
				case RandomWithAlignedCubes:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity =
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0;
					break;
				case Random:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity =
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0;

					vertex.getParticle(newParticleNumber)._persistentRecords._angular =
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX);
					break;
				case friction:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity = tarch::la::Vector<DIMENSIONS,double>(0.5) - fineGridVerticesEnumerator.getVertexPosition(k);
					break;
				case frictionSlide:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = 0.5;
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0;
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = 0;

					vertex.getParticle(newParticleNumber)._persistentRecords._angular(0) = 5;
					vertex.getParticle(newParticleNumber)._persistentRecords._angular(1) = 5;
					vertex.getParticle(newParticleNumber)._persistentRecords._angular(2) = 0;
					break;
			}
		}
	}
	logTraceOutWith1Argument( "createCell(...)", fineGridCell );
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
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
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
