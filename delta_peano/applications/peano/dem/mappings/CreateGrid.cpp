#include "dem/mappings/CreateGrid.h"

#include "delta/primitives/cubes.h"
#include "delta/primitives/granulates.h"
#include "delta/primitives/properties.h"
#include "delta/primitives/hopper.h"
#include "delta/primitives/blender.h"

#include "peano/grid/aspects/VertexStateAnalysis.h"


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
double 								 dem::mappings::CreateGrid::_hopperWidth;
double 								 dem::mappings::CreateGrid::_hopperHatch;
dem::mappings::CreateGrid::GridType  dem::mappings::CreateGrid::_gridType;

void dem::mappings::CreateGrid::setScenario(Scenario scenario, double maxH, double particleDiamMax, double particleDiamMin, GridType gridType ) {
	_scenario        = scenario;
	_maxH            = maxH;
	_particleDiamMax = particleDiamMax;
	_particleDiamMin = particleDiamMin;
	_gridType        = gridType;
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

	if (
			fineGridH(0)>_maxH
			&&
			fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined
			&&
			_gridType!=NoGrid
	) {
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

	if (
			fineGridH(0)>_maxH
			&&
			fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined
			&&
			_gridType!=NoGrid
	) {
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
		const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

	bool UseCubesAndSquares        = false;
	bool UseAlignedCubesAndSquares = false;
	bool PlaceParticleInCell       = true;
	bool randomAlignedDomain = false;
	bool particlesInHopper = false;
	bool twoParticleCrash = false;
	bool freefallcase = false;

	switch (_scenario) {
	case BlackHoleWithAlignedCubes:
		UseAlignedCubesAndSquares = true;
		break;
	case BlackHoleWithCubes:
		UseCubesAndSquares = true;
		break;
	case BlackHole:
		break;
	case RandomWithAlignedCubes:
		UseAlignedCubesAndSquares = true;
		break;
	case RandomWithCubes:
		UseCubesAndSquares = true;
		break;
	case Random:
		randomAlignedDomain = true;
		break;
	case TwoParticlesCrash:
		PlaceParticleInCell = tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(0), 2.0/3.0, 1e-4 )
		| tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(TWO_POWER_D-1), 1.0/3.0, 1e-4);
		twoParticleCrash = true;
		break;
	case freefall:
		freefallcase = true;
		break;
	case hopper:
		if (coarseGridCell.isRoot() )
		{
			std::vector<double>  xCoordinates;
			std::vector<double>  yCoordinates;
			std::vector<double>  zCoordinates;

			dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];
			const int     newParticleNumber = vertex.createNewParticle(fineGridVerticesEnumerator.getVertexPosition());

			double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0), fineGridVerticesEnumerator.getCellCenter()(1), fineGridVerticesEnumerator.getCellCenter()(2)};
			delta::primitives::generateHopper( centreAsArray, _hopperWidth, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);

			for (int i=0; i<static_cast<int>(xCoordinates.size()); i++) {
				vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
				vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
				vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );
			}

			_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

			vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
			vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = _hopperWidth;
			vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
			vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = 0;
			assertion( tarch::la::greater(vertex.getParticle(newParticleNumber)._persistentRecords._hMin,0.0) );

			delta::primitives::computeMass(
					xCoordinates,yCoordinates,zCoordinates,
					vertex.getParticle(newParticleNumber)._persistentRecords._mass,
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(0),
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(1),
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(2));

			return;
		}
		particlesInHopper = true;
		break;
	}

	if (PlaceParticleInCell &&
		peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices,fineGridVerticesEnumerator,Vertex::Records::Unrefined)
		&& !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices,fineGridVerticesEnumerator))
	{
		int particlesInCellPerAxis = std::floor(fineGridVerticesEnumerator.getCellSize()(0) / _particleDiamMax);

		if (particlesInCellPerAxis==0)
		{
			particlesInCellPerAxis = 1;
			_particleDiamMax = fineGridVerticesEnumerator.getCellSize()(0);
			_particleDiamMin = std::min(_particleDiamMin,_particleDiamMax);
			logWarning( "createCell(...)", "particle size has been too small for coarsest prescribed grid. Reduce particle size to " << _particleDiamMin << "-" << _particleDiamMax );
		}else
		{
			logDebug( "createCell", "particlesInCellPerAxis="<< particlesInCellPerAxis );
		}

		dfor(k,particlesInCellPerAxis)
		{
			const tarch::la::Vector<DIMENSIONS,double> centre = fineGridVerticesEnumerator.getVertexPosition() + _particleDiamMax * (k.convertScalar<double>()+0.5);

			double particleDiameter = (_particleDiamMin + (_particleDiamMax-_particleDiamMin) * (static_cast<double>( rand() ) / static_cast<double>(RAND_MAX)) ) / std::sqrt( DIMENSIONS );

			logDebug( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter );

			dem::Vertex&  vertex            = fineGridVertices[ fineGridVerticesEnumerator(0) ];

			double centreAsArray[] = {centre(0),centre(1),centre(2)};

			int  newParticleNumber;

			// We need these temporary guys if we use an aligned vector from Peano.
			// Peano's aligned vector and the default std::vector are not compatible.
			std::vector<double>  xCoordinates;
			std::vector<double>  yCoordinates;
			std::vector<double>  zCoordinates;

			if (UseCubesAndSquares)
			{
				newParticleNumber = vertex.createNewParticle( centre );
				delta::primitives::generateCube(
						centreAsArray,
						particleDiameter,
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						xCoordinates,
						yCoordinates,
						zCoordinates
				);
				assertionEquals( xCoordinates.size(), 6*2*3 );
				assertionEquals( xCoordinates.size(), yCoordinates.size() );
				assertionEquals( xCoordinates.size(), zCoordinates.size() );
			}
			else if (UseAlignedCubesAndSquares)
			{
				newParticleNumber = vertex.createNewParticle( centre );
				delta::primitives::generateCube(
						centreAsArray,
						particleDiameter,
						0.0, // around x-axis 1/8
						1.0/8.0, // around y-axis 1/8
						1.0/8.0, // around z-axis 1/8
						xCoordinates,
						yCoordinates,
						zCoordinates
				);
				assertionEquals( xCoordinates.size(), 6*2*3 );
				assertionEquals( xCoordinates.size(), yCoordinates.size() );
				assertionEquals( xCoordinates.size(), zCoordinates.size() );
			}
			else if (twoParticleCrash)
			{
				newParticleNumber = vertex.createNewParticle( centre );
				assertion( particleDiameter>0.0 );
				delta::primitives::generateParticle(
						centreAsArray,
						particleDiameter,
						xCoordinates,
						yCoordinates,
						zCoordinates
				);

				assertion2( xCoordinates.size()>0, centreAsArray, particleDiameter );
				assertionEquals2( xCoordinates.size(), yCoordinates.size(), centreAsArray, particleDiameter );
				assertionEquals2( xCoordinates.size(), zCoordinates.size(), centreAsArray, particleDiameter );
			}
			else if (randomAlignedDomain)
			{
				newParticleNumber = vertex.createNewParticle( centre );
				assertion( particleDiameter>0.0 );
				delta::primitives::generateParticle(
						centreAsArray,
						particleDiameter,
						xCoordinates,
						yCoordinates,
						zCoordinates
				);

				assertion2( xCoordinates.size()>0, centreAsArray, particleDiameter );
				assertionEquals2( xCoordinates.size(), yCoordinates.size(), centreAsArray, particleDiameter );
				assertionEquals2( xCoordinates.size(), zCoordinates.size(), centreAsArray, particleDiameter );
			}
			else if(particlesInHopper || freefallcase)
			{
				double coarseCenterHopperMarginUpper = 0.5 + (_hopperWidth/2);
				double coarseCenterHopperMarginLower = 0.5 - (_hopperWidth/2);
				if(centreAsArray[0] < coarseCenterHopperMarginUpper && centreAsArray[0] > coarseCenterHopperMarginLower &&
				   centreAsArray[1] < coarseCenterHopperMarginUpper+0.2 && centreAsArray[1] > coarseCenterHopperMarginUpper &&
				   centreAsArray[2] < coarseCenterHopperMarginUpper && centreAsArray[2] > coarseCenterHopperMarginLower)
				{
					newParticleNumber = vertex.createNewParticle( centre );
					assertion( particleDiameter>0.0 );
					delta::primitives::generateParticle(
							centreAsArray,
							particleDiameter,
							xCoordinates,
							yCoordinates,
							zCoordinates
					);

					assertion2( xCoordinates.size()>0, centreAsArray, particleDiameter );
					assertionEquals2( xCoordinates.size(), yCoordinates.size(), centreAsArray, particleDiameter );
					assertionEquals2( xCoordinates.size(), zCoordinates.size(), centreAsArray, particleDiameter );
				}
				else
				{
					return;
				}
			}

			for (int i=0; i<static_cast<int>(xCoordinates.size()); i++)
			{
				vertex.getXCoordinatesAsVector(newParticleNumber).push_back( xCoordinates[i] );
				vertex.getYCoordinatesAsVector(newParticleNumber).push_back( yCoordinates[i] );
				vertex.getZCoordinatesAsVector(newParticleNumber).push_back( zCoordinates[i] );
			}

			// check alignment
			logDebug( "createCell()", "x[0]="
					<< (&vertex.getXCoordinates(newParticleNumber)[0])
					<< "\tmod 8=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[0] % 8)
					<< "\tmod 16=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[0] % 16)
					<< "\tmod 32=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[0] % 32)
					<< "\tmod 64=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[0] % 64)
			);
			logDebug( "createCell()", "x[1]="
					<< (&vertex.getXCoordinates(newParticleNumber)[1])
					<< "\tmod 8=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[1] % 8)
					<< "\tmod 16=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[1] % 16)
					<< "\tmod 32=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[1] % 32)
					<< "\tmod 64=" << ((long int)&vertex.getXCoordinates(newParticleNumber)[1] % 64)
			);

			_numberOfParticles ++;
			_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

			vertex.getParticle(newParticleNumber)._persistentRecords._numberOfTriangles    = vertex.getXCoordinatesAsVector(newParticleNumber).size()/DIMENSIONS;
			vertex.getParticle(newParticleNumber)._persistentRecords._diameter             = particleDiameter * 1.5;
			vertex.getParticle(newParticleNumber)._persistentRecords._radius               = particleDiameter/2;
			vertex.getParticle(newParticleNumber)._persistentRecords._epsilon 			   = 5E-3;

			vertex.getParticle(newParticleNumber)._persistentRecords._hMin                 = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
			vertex.getParticle(newParticleNumber)._persistentRecords._globalParticleNumber = _numberOfParticles;
			assertion( tarch::la::greater(vertex.getParticle(newParticleNumber)._persistentRecords._hMin,0.0) );

			delta::primitives::computeMass(
					xCoordinates,yCoordinates,zCoordinates,
					vertex.getParticle(newParticleNumber)._persistentRecords._mass,
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(0),
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(1),
					vertex.getParticle(newParticleNumber)._persistentRecords._centreOfMass(2));

			switch (_scenario) {
			case BlackHoleWithAlignedCubes:
			case BlackHoleWithCubes:
			case BlackHole:
			case TwoParticlesCrash:
				vertex.getParticle(newParticleNumber)._persistentRecords._velocity = tarch::la::Vector<DIMENSIONS,double>(0.5) - fineGridVerticesEnumerator.getVertexPosition(k);
				vertex.getParticle(newParticleNumber)._persistentRecords._angularVelocity = tarch::la::Vector<DIMENSIONS,double>(0.0);
			break;
			case RandomWithAlignedCubes:
			case RandomWithCubes:
			case Random:
				vertex.getParticle(newParticleNumber)._persistentRecords._velocity =
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0;

				vertex.getParticle(newParticleNumber)._persistentRecords._angularVelocity =
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX);
			break;
			case hopper:
				//vertex.getParticle(newParticleNumber)._persistentRecords._velocity = 9.81;

				//vertex.getParticle(newParticleNumber)._persistentRecords._angularVelocity = 9.81;

				//gravity force initial condition.
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

	logInfo( "endIteration(State)", "created " << _numberOfParticles << " particles with " << _numberOfTriangles << " triangles" );

	logTraceOutWith1Argument( "endIteration(State)", solverState);
}

//
//   NOP
// =======
//
dem::mappings::CreateGrid::CreateGrid() {
	logTraceIn( "CreateGrid()" );
	// @todo Insert your code here
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

	logTraceOut( "CreateGrid(CreateGrid)" );
}


void dem::mappings::CreateGrid::mergeWithWorkerThread(const CreateGrid& workerThread) {
	logTraceIn( "mergeWithWorkerThread(CreateGrid)" );

	_numberOfParticles += workerThread._numberOfParticles;
	_numberOfTriangles += workerThread._numberOfTriangles;

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
