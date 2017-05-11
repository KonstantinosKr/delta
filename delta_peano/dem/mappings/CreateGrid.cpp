#include "dem/mappings/CreateGrid.h"

#include "delta/primitives/cubes.h"
#include "delta/primitives/granulates.h"
#include "delta/primitives/properties.h"
#include "delta/primitives/hopper.h"
#include "delta/primitives/blender.h"
#include <delta/primitives/graphite.h>
#include <delta/primitives/surface.h>
#include <delta/primitives/assembly.h>

#include "peano/grid/aspects/VertexStateAnalysis.h"

#include <string>
#include <iostream>
#include <cmath>
#include <ctime>

#define epsilon 0.002

/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::CreateGrid::communicationSpecification() {
	return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexLastTimeSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexFirstTimeSpecification() { 
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::enterCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::leaveCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::ascendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::descendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                  dem::mappings::CreateGrid::_log( "dem::mappings::CreateGrid" );
dem::mappings::CreateGrid::Scenario  dem::mappings::CreateGrid::_scenario;
dem::mappings::CreateGrid::VScheme 	 dem::mappings::CreateGrid::_velocityScheme;
double                               dem::mappings::CreateGrid::_maxH;
double                               dem::mappings::CreateGrid::_minParticleDiam;
double                               dem::mappings::CreateGrid::_maxParticleDiam;
dem::mappings::CreateGrid::GridType  dem::mappings::CreateGrid::_gridType;
double								               dem::mappings::CreateGrid::_epsilon;
int 								                 dem::mappings::CreateGrid::_noPointsPerParticle;

void dem::mappings::CreateGrid::setScenario(Scenario scenario, VScheme velocityScheme,
                                            double maxH, double particleDiamMin, double particleDiamMax,
											                      GridType gridType, int noPointsPerGranulate)
{
	_scenario        		  = scenario;
	_velocityScheme  		  = velocityScheme;
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
        } else
        {
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

void dem::mappings::CreateGrid::setVScheme(dem::Vertex&  vertex, int particleNumber)
{
  switch (_velocityScheme)
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

	/*
	 * root cell enviroment creation (mainly for obstacles) or large scenario setups (nuclear SLA)
	 */
	if (coarseGridCell.isRoot())
	{
		dem::Vertex&  vertex  = fineGridVertices[fineGridVerticesEnumerator(0)];

		double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
                              fineGridVerticesEnumerator.getCellCenter()(1),
                              fineGridVerticesEnumerator.getCellCenter()(2)};

    if(_scenario == sla)
    {
      /*
       *nuclear single layer array;
       * experiment of seismic shakes concept: drop all components in coarse grid then reassign vertex to refined grid
       */

      //////FLOOR//////////////////////////////////////////////////////////////////////////////////////////////////
      centreAsArray[1] = 0.5;

      iREAL position[] = {centreAsArray[0], centreAsArray[1], centreAsArray[2]};
      double floorHeight = 0.05;
      int particleid = makeFloor(vertex, position, 1, floorHeight, 0, 0, 0, delta::collision::material::MaterialType::GOLD, true, true);

      dem::mappings::CreateGrid::_velocityScheme = moveLeft;
      dem::mappings::CreateGrid::setVScheme(vertex,  particleid);
      dem::mappings::CreateGrid::_velocityScheme = none;
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      position[0] = centreAsArray[0]; position[1] = centreAsArray[1]+(floorHeight/2); position[2] = centreAsArray[2];
      dem::mappings::CreateGrid::makeLoadNuclearGeometry(vertex, position);
      return;
    } else if(_scenario == nuclearArray)
		{
      /* nuclear single layer array;
       * experiment of seismic shakes:concept: drop all components in coarse grid then reassign vertex to refined grid */

      ////FLOOR/////////////////////////////////////////////////////////////////////////////////////////////////////
      centreAsArray[1] = 0.5;

      iREAL position[] = {centreAsArray[0], centreAsArray[1], centreAsArray[2]};
      double floorHeight = 0.05;
      int particleid = makeFloor(vertex, position, 1, floorHeight, 0, 0, 0, delta::collision::material::MaterialType::GOLD, true, true);
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      position[0] = centreAsArray[0]; position[1] = centreAsArray[1]+(floorHeight/2); position[2] = centreAsArray[2];
      dem::mappings::CreateGrid::makeFullBrickFBGrid(vertex, position, 1, 10);
      return;
		} else if(_scenario == hopperUniform)
		{
      ////////HOPPER////////////////////////////////////////////////////////////////////////////////////////////
      double _hopperWidth = 0.20;	double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
      //HOPPER DIAGONAL:0.382926
      makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      ////////FLOOR////////////////floor DIAGONAL:0.344674///////////////////////////////////////////
      iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, position, width, height, 0, 0, 0, delta::collision::material::MaterialType::GOLD, true, true);
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      //////////PARTICLE GRID///////////////////////////////////////////////////////////////////////////////////////
      iREAL xzcuts = 10; iREAL ycuts = 1;
      iREAL margin = ((double)_hopperWidth/(double)xzcuts)/2.0;
      double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

      //iREAL minParticleDiameter = ((double)_hopperWidth/(double)xzcuts)-(margin*2.0);
      //printf("minParDiameter:%.10f\n", minParticleDiameter);

      position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
      makeUniParticleGrid(vertex, position, xzcuts, ycuts, _hopperWidth, totalMass);
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      return;
		} else if(_scenario == hopperUniform1k)
    {
      /////////HOPPER//////////////////////////////////////////////////////////////////////////
      /* uniform hopper experiment;
       * particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom */
      //./dem-3d-release-vec 0.3 0.3 0.3 hopperUniformSphere 7000 reluctant-grid 0.0000005 every-batch 10 true sphere 50 | tee log400.log
      double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
      makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
      //////////////////////////////////////////////////////////////////////////////////////////

      ////////FLOOR////////////////////////////////////////////////////////////////////////////
      iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, position, width, height, 0,0,0, delta::collision::material::MaterialType::GOLD, true, true);
      ///////////////////////////////////////////////////////////////////////////////////////////

      //////////PARTICLE GRID///////////////////////////////////////////////////////////////////////////////////////
      iREAL xzcuts = 10; iREAL ycuts = 10;
      iREAL margin = ((double)_hopperWidth/(double)xzcuts)/2.0;
      //iREAL minParticleDiameter = ((double)_hopperWidth/(double)xcuts)-(margin*2.0);
      double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

      position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
      makeUniParticleGrid(vertex, position, xzcuts, ycuts, _hopperWidth, totalMass);
      //////////////////////////////////////////////////////////////////////////////////////////
      return;
    } else if(_scenario == hopperUniform10k)
    {
      //////HOPPER////////////////////////////////////////////////////////////////////////////
      /* uniform hopper experiment;
       * particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom */
      //./dem-3d-release-vec 0.3 0.3 0.3 hopperUniformSphere 7000 reluctant-grid 0.0000005 every-batch 10 true sphere 50 | tee log400.log
      double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
      makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
      //////////////////////////////////////////////////////////////////////////////////////////


      //////FLOOR///////////////////////////////////////////////////////////////////
      /**flooring creation*/
      iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, position, width, height, 0,0,0, delta::collision::material::MaterialType::GOLD, true, true);
      //////////////////////////////////////////////////////////////////////////////////////////

      ////PARTICLE GRID/////////////////////////////////////////////////////////////////////////
      iREAL xzcuts = 10; iREAL ycuts = 100;
      iREAL margin = ((double)_hopperWidth/(double)xzcuts)/2.0;
      //iREAL minParticleDiameter = ((double)_hopperWidth/(double)xzcuts)-(margin*2.0);
      double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

      position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
      makeUniParticleGrid(vertex, position, xzcuts, ycuts, _hopperWidth, totalMass);
      ////////////////////////////////////////////////////////////////////////////////////
      return;
    } else if(_scenario == hopperUniform50k)
    {
      ////HOPPER//////////////////////////////////////////////////////////////////////////////
      double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
      makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
      //////////////////////////////////////////////////////////////////////////////////////////

      ///FLOOR///////////////////////////////////////////////////////////////////////////
      iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, position, width, height, 0,0,0, delta::collision::material::MaterialType::GOLD, true, true);
      /////////////////////////////////////////////////////////////////////////////////////////

      ///PARTICLE GRID////////////////////////////////////////////////////////////////////////////////
      iREAL xzcuts = 10; iREAL ycuts = 500;
      iREAL margin = ((double)_hopperWidth/(double)xzcuts)/2.0;
      //iREAL minParticleDiameter = ((double)_hopperWidth/(double)xzcuts)-(margin*2.0);
      double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

      position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
      makeUniParticleGrid(vertex, position, xzcuts, ycuts, _hopperWidth, totalMass);
      //////////////////////////////////////////////////////////////////////////////////////////
      return;
    } else if(_scenario == hopperNonUniform)
		{
      /////HOPPER////////////////////////////////////////////////////////////////////////
      /*hopper experiment; particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom*/
      double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
      makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
      //////////////////////////////////////////////////////////////////////////////////////////

      /////FLOOR////////////////////////////////////////////////////////////////////////////////
      iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, position, width, height, 0,0,0, delta::collision::material::MaterialType::GOLD, true, true);
      //////////////////////////////////////////////////////////////////////////////////////////

      //////PARTICLE GRID//////////////////////////////////////////////////////////////////////
      iREAL xzcuts = 10.0; iREAL ycuts = 1.0;
      iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
      iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

      iREAL subcellx = ((double)minArraylengthX/(double)xzcuts) - _epsilon*4;

      iREAL margin = ((double)minArraylengthX/(double)xzcuts)/2.0;
      printf("maxDiameter:%f\n", subcellx);
      double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

      position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
      makeNonUniParticleGrid(vertex, position, xzcuts, ycuts, subcellx, _hopperWidth, totalMass);
      //////////////////////////////////////////////////////////////////////////////////////////
      return;
    } else if(_scenario == hopperNonUniform1k)
    {
      //////HOPPER////////////////////////////////////////////////////////////////////////////
      /*hopper experiment; particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom*/
      double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
      makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
      //////////////////////////////////////////////////////////////////////////////////////////

      //////FLOOR/////////////////////////////////////////////////////////////////////////
      iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, position, width, height, 0,0,0, delta::collision::material::MaterialType::GOLD, true, true);
      //////////////////////////////////////////////////////////////////////////////////////////

      ////////PARTICLE GRID//////////////////////////////////////////////////////////////////////////
      iREAL xzcuts = 10.0; iREAL ycuts = 10.0;
      iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
      iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

      iREAL subcellx = ((double)minArraylengthX/(double)xzcuts) - _epsilon*4;
      //iREAL subcelly = ((double)minArraylengthY/(double)ycuts) - _epsilon*4;

      iREAL margin = ((double)minArraylengthX/(double)xzcuts)/2.0;
      //printf("maxDiameter:%f\n", subcellx);
      double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

      position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
      makeNonUniParticleGrid(vertex, position, xzcuts, ycuts, subcellx, _hopperWidth, totalMass);
      //////////////////////////////////////////////////////////////////////////////////////////
      return;
     } else if(_scenario == hopperNonUniform10k)
     {
       ////////HOPPER//////////////////////////////////////////////////////////////////////
       double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
       makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
       //////////////////////////////////////////////////////////////////////////////////////////

       ///////FLOOR//////////////////////////////////////////////////////////////////////////
       iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
       double height = 0.05; double width = 0.35;
       makeFloor(vertex, position, width, height, 0,0,0, delta::collision::material::MaterialType::GOLD, true, true);
       //////////////////////////////////////////////////////////////////////////////////////////

       ///////PARTICLE GRID//////////////////////////////////////////////////////////////////////////
       iREAL xzcuts = 10.0; iREAL ycuts = 100.0;
       iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
       iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

       iREAL subcellx = ((double)minArraylengthX/(double)xzcuts) - _epsilon*4;
       //iREAL subcelly = ((double)minArraylengthY/(double)ycuts) - _epsilon*4;

       iREAL margin = ((double)minArraylengthX/(double)xzcuts)/2.0;
       printf("maxDiameter:%f\n", subcellx);
       double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

       position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
       makeNonUniParticleGrid(vertex, position, xzcuts, ycuts, subcellx, _hopperWidth, totalMass);
       //////////////////////////////////////////////////////////////////////////////////////////
       return;
     }  else if(_scenario == hopperNonUniform50k)
     {
       ///////HOPPER///////////////////////////////////////////////////////////////////////////
       /*hopper experiment; particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom*/
       double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;
       makeHopper(vertex, centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, delta::collision::material::MaterialType::GOLD, false, true);
       //////////////////////////////////////////////////////////////////////////////////////////

       ///////FLOOR//////////////////////////////////////////////////////////////////////////
       iREAL position[] = {centreAsArray[0], 0.3, centreAsArray[2]};
       double height = 0.05; double width = 0.35;
       makeFloor(vertex, position, width, height, 0,0,0, delta::collision::material::MaterialType::GOLD, true, true);
       //////////////////////////////////////////////////////////////////////////////////////////

       ///////PARTICLE GRID///////////////////////////////////////////////////////////////////////////
       iREAL xzcuts = 10.0; iREAL ycuts = 500.0;
       iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
       iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

       iREAL subcellx = ((double)minArraylengthX/(double)xzcuts) - _epsilon*4;
       //iREAL subcelly = ((double)minArraylengthY/(double)ycuts) - _epsilon*4;

       iREAL margin = ((double)minArraylengthX/(double)xzcuts)/2.0;
       printf("maxDiameter:%f\n", subcellx);
       double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

       position[0] = (centreAsArray[0] - _hopperWidth/2) + margin; position[1] = centreAsArray[1] + _hopperHeight/2; position[2] = (centreAsArray[2] - _hopperWidth/2) + margin;
       makeNonUniParticleGrid(vertex, position, xzcuts, ycuts, subcellx, _hopperWidth, totalMass);
       //////////////////////////////////////////////////////////////////////////////////////////
       return;
     } else if(_scenario == frictionStatic)
     {
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, centreAsArray, width, height, 0,0,0, delta::collision::material::MaterialType::WOOD, true, true);

      double radius = 0.01; double eps = radius*0.2;

      int newParticleNumber;
      if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
      {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height, centreAsArray[2]};
        newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, delta::collision::material::MaterialType::WOOD, true, false);
      } else {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + ((height)/1.5)-width/2, centreAsArray[2]};
        newParticleNumber = dem::mappings::CreateGrid::makeBox(vertex, position, width, height, eps, delta::collision::material::MaterialType::WOOD, true, false);
      }
      setVScheme(vertex, newParticleNumber);
      return;
    } else if(_scenario == frictionSlide)
    {
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, centreAsArray, width, height, 0,0,0, delta::collision::material::MaterialType::WOOD, true, true);

      double radius = 0.01; double eps = radius*0.2;

      int newParticleNumber;
      if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
      {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius + eps*2, centreAsArray[2]};
        newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, delta::collision::material::MaterialType::WOOD, true, false);
      } else {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius + eps, centreAsArray[2]};
        newParticleNumber = dem::mappings::CreateGrid::makeLoadedNonSpherical(vertex, position, radius, eps, delta::collision::material::MaterialType::WOOD, true, false);
      }
      setVScheme(vertex, newParticleNumber);
      return;
    } else if(_scenario == frictionRoll)
    {
      double height = 0.05; double width = 0.35;
      makeFloor(vertex, centreAsArray, width, height, 0,0,0.1, delta::collision::material::MaterialType::WOOD, true, true);

      double radius = 0.01; double eps = radius*0.2;

      int newParticleNumber;
      if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
      {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius/2 + eps*2, centreAsArray[2]};
        newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, radius, eps, delta::collision::material::MaterialType::WOOD, true, false);
      } else {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius, centreAsArray[2]};
        newParticleNumber = dem::mappings::CreateGrid::makeBox(vertex, position, width, height, eps, delta::collision::material::MaterialType::WOOD, true, false);
      }
      setVScheme(vertex, newParticleNumber);
      return;
    } else if(_scenario == TwoParticlesCrash)
    {
      if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
      {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + _maxParticleDiam*2, centreAsArray[2]};
        int newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, _maxParticleDiam, _epsilon, delta::collision::material::MaterialType::WOOD, false, false);
        vertex.getParticle(newParticleNumber)._persistentRecords._velocity = -0.5;

        position[0] = centreAsArray[0]; position[1] = centreAsArray[1] - _maxParticleDiam*2; position[2] = centreAsArray[2];
        newParticleNumber = dem::mappings::CreateGrid::makeSphere(vertex, position, _maxParticleDiam, _epsilon, delta::collision::material::MaterialType::WOOD, false, false);
        vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0.5;
      } else {
        iREAL position[] = {centreAsArray[0], centreAsArray[1] + _maxParticleDiam*2, centreAsArray[2]};
        int newParticleNumber = dem::mappings::CreateGrid::makeNonSpherical(vertex, position, _maxParticleDiam, _epsilon, delta::collision::material::MaterialType::WOOD, false, false);
        vertex.getParticle(newParticleNumber)._persistentRecords._velocity = -0.5;

        position[0] = centreAsArray[0]; position[1] = centreAsArray[1] - _maxParticleDiam*2; position[2] = centreAsArray[2];
        newParticleNumber = dem::mappings::CreateGrid::makeNonSpherical(vertex, position, _maxParticleDiam, _epsilon, delta::collision::material::MaterialType::WOOD, false, false);
        vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0.5;
      }
      return;
    }
	}

	if(_scenario != BlackHoleWithRandomOrientedCubes  ||
     _scenario != BlackHoleWithCubes                ||
	   _scenario != TwoParticlesCrash                 ||
     _scenario != RandomWithGranulates              ||
     _scenario != RandomWithCubes                   ||
     _scenario != freefall)
  {
    return;
  }

	if(peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices, fineGridVerticesEnumerator,Vertex::Records::Unrefined) &&
	    !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices,fineGridVerticesEnumerator))
	{
		int particlesInCellPerAxis = std::floor(fineGridVerticesEnumerator.getCellSize()(0) / _maxParticleDiam);

		printf("cellsize:%f particlesInCellPerAxis:%i maxparticlediam:%f\n", fineGridVerticesEnumerator.getCellSize()(0), particlesInCellPerAxis, _maxParticleDiam);
		if(particlesInCellPerAxis==0)
		{
			particlesInCellPerAxis = 1;
			_maxParticleDiam = fineGridVerticesEnumerator.getCellSize()(0);
			_minParticleDiam = std::min(_minParticleDiam,_maxParticleDiam);

			logWarning( "createCell(...)", "particle size has been too small for coarsest prescribed grid. Reduce particle size to "
			            << std::min(_minParticleDiam,_maxParticleDiam) << "-" << fineGridVerticesEnumerator.getCellSize()(0) );
		} else {
			logDebug( "createCell", "particlesInCellPerAxis="<< particlesInCellPerAxis );
		}

		dfor(k,particlesInCellPerAxis)
		{
      dem::Vertex&  vertex = fineGridVertices[fineGridVerticesEnumerator(0)];

		  const tarch::la::Vector<DIMENSIONS,double> centre = fineGridVerticesEnumerator.getVertexPosition() + _maxParticleDiam * (k.convertScalar<double>()+0.5);
      double centreAsArray[] = {centre(0),centre(1),centre(2)};

			double particleDiameter = (_minParticleDiam + (_maxParticleDiam-_minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) ) / std::sqrt( DIMENSIONS );

			// We need these temporary guys if we use an aligned vector from Peano.
			// Peano's aligned vector and the default std::vector are not compatible.
			std::vector<double>  xCoordinates;
			std::vector<double>  yCoordinates;
			std::vector<double>  zCoordinates;

			if (_scenario == BlackHoleWithRandomOrientedCubes)
			{
				delta::primitives::cubes::generateCube(centreAsArray, particleDiameter,
                                                static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
                                                static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
                                                static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
                                                xCoordinates, yCoordinates, zCoordinates);
			}
			else if (_scenario == BlackHoleWithCubes)
			{
				delta::primitives::cubes::generateCube(centreAsArray, particleDiameter,
                                                0.0, // around x-axis 1/8
                                                1.0/8.0, // around y-axis 1/8
                                                1.0/8.0, // around z-axis 1/8
                                                xCoordinates, yCoordinates, zCoordinates );
			}
			else if (_scenario == RandomWithGranulates || _scenario == RandomWithCubes || _scenario == freefall)
			{
				if (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
				{
					delta::primitives::cubes::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
				} else {
					delta::primitives::granulates::generateParticle(centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
				}
			}

			int newParticleNumber = vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
									                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
      dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, false);

			if(particleDiameter<_minParticleDiam)
			  _minParticleDiam = particleDiameter;
			if(particleDiameter>_maxParticleDiam)
			  _maxParticleDiam = particleDiameter;
			#ifdef STATSPARTICLE
			  logWarning( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter << " and id: " << particleId);
			#endif

			/*Set velocities for particle filling for scenario*/
			setVScheme(vertex, newParticleNumber);
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

void dem::mappings::CreateGrid::uniMesh(std::vector<std::array<double, 3>>&  nPositions,
                                        double  radius,
                                        double totalMass,
                                        std::vector<std::vector<double>>  &xCoordinatesArray,
                                        std::vector<std::vector<double>>  &yCoordinatesArray,
                                        std::vector<std::vector<double>>  &zCoordinatesArray)
{

  double reMassTotal = 0;
  //double masssphere = 0;
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  double position[3];
  for(auto i:nPositions)
  {
    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
    //double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
    //double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  }

  double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<double, 3> ar = nPositions[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::primitives::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);

    //double mt = delta::primitives::properties::computeMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], delta::collision::material::MaterialType::WOOD);
    //double vt = delta::primitives::properties::computeVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    //double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);
}


void dem::mappings::CreateGrid::nonUniMesh(std::vector<std::array<double, 3>>&  nPositions,
                                        double  radius,
                                        double totalMass,
                                        double subcellx,
                                        std::vector<std::vector<double>>  &xCoordinatesArray,
                                        std::vector<std::vector<double>>  &yCoordinatesArray,
                                        std::vector<std::vector<double>>  &zCoordinatesArray)
{
  double reMassTotal = 0;
  double masssphere = 0;

  iREAL position[3];
  for(auto i:nPositions)
  {
    std::vector<double>  xCoordinates;
    std::vector<double>  yCoordinates;
    std::vector<double>  zCoordinates;

    position[0] =  position[1] = i[1]; position[2] = i[2];
    _minParticleDiam = subcellx/2;
    _maxParticleDiam = subcellx;
    double particleDiameter = _minParticleDiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(_maxParticleDiam-_minParticleDiam)));

    radius = particleDiameter/2;
    delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
    //double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
    //double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  }

  double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  //reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<double, 3> ar = nPositions[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::primitives::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);

    double mt = delta::primitives::properties::computeMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], delta::collision::material::MaterialType::WOOD);
    double vt = delta::primitives::properties::computeVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

}

void dem::mappings::CreateGrid::nonUniSphere(int  N, double totalMass, double subcellx, std::vector<double>  &rad)
{
  //double massPerParticle = totalMass/(double)N.size();
  //double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

  double reMassTotal = 0;

  for(int i=0; i<N; i++)
  {
    _minParticleDiam = subcellx/2;
    _maxParticleDiam = subcellx;
    double particleDiameter = _minParticleDiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(_maxParticleDiam-_minParticleDiam)));
    rad.push_back(particleDiameter/2);
    reMassTotal += (4.0/3.0) * 3.14 * std::pow(particleDiameter/2,3) * int(delta::collision::material::MaterialDensity::WOOD); //volume * mass
  }

  //get total mass
  //printf("TOTAL REMASS:%f\n", reMassTotal);

  double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(auto i:rad)
  {
    i *= rescale;
    reMassTotal += (4.0/3.0) * 3.14 * std::pow(i,3) * int(delta::collision::material::MaterialDensity::WOOD); //volume * mass
  }
  //printf("RESCALE:%f\n", rescale);
  //printf("TOTAL REREMASS:%f\n", reMassTotal);
}

int dem::mappings::CreateGrid::makeHopper(dem::Vertex&  vertex, double position[3], double _hopperWidth, double _hopperHeight, double _hopperHatch,
                                           delta::collision::material::MaterialType material, bool friction, bool isObstacle)
{
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  delta::primitives::hopper::generateHopper(position, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                           dem::mappings::CreateGrid::_epsilon, isObstacle, material, friction, _numberOfParticles);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

int dem::mappings::CreateGrid::makeFloor(dem::Vertex&  vertex, double position[3], double width, double height, double rx, double ry, double rz,
                                          delta::collision::material::MaterialType material, bool friction, bool isObstacle)
{
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  delta::primitives::cubes::generateHullCube(position, width, height, width, rx, ry, rz, xCoordinates, yCoordinates, zCoordinates);
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                           dem::mappings::CreateGrid::_epsilon, isObstacle, material, friction, _numberOfParticles);

  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

int dem::mappings::CreateGrid::makeSphere(dem::Vertex&  vertex, double position[3], double radius, double eps,
                                          delta::collision::material::MaterialType material, bool friction, bool isObstacle)
{
  return vertex.createNewParticleSphere(position, radius, eps, isObstacle, material, friction, _numberOfParticles);
  _numberOfParticles++;
  if(isObstacle) dem::mappings::CreateGrid::_numberOfObstacles++;
}

int dem::mappings::CreateGrid::makeBox(dem::Vertex&  vertex, double position[3], double width, double height, double eps,
                                          delta::collision::material::MaterialType material, bool friction, bool isObstacle)
{
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  delta::primitives::cubes::generateHullCube(position, width, xCoordinates, yCoordinates, zCoordinates);
  //delta::primitives::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
  //delta::primitives::granulates::loadParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates);
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                                               eps, isObstacle, material, friction, _numberOfParticles);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

int dem::mappings::CreateGrid::makeLoadedNonSpherical(dem::Vertex&  vertex, double position[3], double radius, double eps,
                                                      delta::collision::material::MaterialType material, bool friction, bool isObstacle)
{
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  delta::primitives::granulates::loadParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates);
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                                               eps, isObstacle, material, friction, _numberOfParticles);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}


int dem::mappings::CreateGrid::makeNonSpherical(dem::Vertex&  vertex, double position[3], double radius, double eps,
                                                      delta::collision::material::MaterialType material, bool friction, bool isObstacle)
{
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
  int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                                               eps, isObstacle, material, friction, _numberOfParticles);
  dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, isObstacle);
  return newParticleNumber;
}

void dem::mappings::CreateGrid::makeUniParticleGrid(dem::Vertex&  vertex, double position[3], int xzcuts, int ycuts, double width, double totalMass)
{
  //////////////////ARRAY
  std::vector<std::array<double, 3>> N = delta::primitives::assembly::array3d(position, width, xzcuts, width, ycuts);

  double massPerParticle = totalMass/(double)N.size(); //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
  double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));
  /////////////////ARRAY

  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  //////////////////////SPHERE//////////////////////////////////////////////////////////////////////////////////////
  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
  {
    for(auto i:N)
    {
      //delta::primitives::cubes::generateCube(position, (radius*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
      vertex.createNewParticleSphere({i[0], i[1], i[2]}, radius, _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
      dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, false);
    }
  }else
  {
    //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
    std::vector<std::vector<double>>  xCoordinatesArray, yCoordinatesArray, zCoordinatesArray;
    dem::mappings::CreateGrid::uniMesh(N, radius, totalMass, xCoordinatesArray, yCoordinatesArray, zCoordinatesArray);

    for(unsigned i=0; i<N.size(); i++)
    {
      vertex.createNewParticle({N[i][0], N[i][1], N[i][2]}, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                               _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
      dem::mappings::CreateGrid::addParticleToState(xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i], false);
    }
  }
}

void dem::mappings::CreateGrid::makeNonUniParticleGrid(dem::Vertex&  vertex, double position[3], int xzcuts, int ycuts, double subcellx, double width, double totalMass)
{
  //////////////////ARRAY
  std::vector<std::array<double, 3>> N = delta::primitives::assembly::array3d(position, width, xzcuts, width, ycuts);

  double massPerParticle = totalMass/(double)N.size(); //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
  double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));
  /////////////////ARRAY

  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  //////////////////////SPHERE//////////////////////////////////////////////////////////////////////////////////////
  if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
  {
    std::vector<double>  rad;
    dem::mappings::CreateGrid::nonUniSphere(N.size(), totalMass, subcellx, rad);

    for(unsigned i=0; i<N.size(); i++)
    {
      //delta::primitives::cubes::generateCube(position, (radius*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
      vertex.createNewParticleSphere({N[i][0], N[i][1], N[i][2]}, rad[i], _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
      dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, false);
    }
  }else
  {
    //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
    std::vector<std::vector<double>>  xCoordinatesArray, yCoordinatesArray, zCoordinatesArray;

    dem::mappings::CreateGrid::nonUniMesh(N, radius, totalMass, subcellx, xCoordinatesArray, yCoordinatesArray, zCoordinatesArray);

    for(unsigned i=0; i<N.size(); i++)
    {
      vertex.createNewParticle({N[i][0], N[i][1], N[i][2]}, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                               _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
      dem::mappings::CreateGrid::addParticleToState(xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i], false);
    }
  }
}

void dem::mappings::CreateGrid::makeFullBrickFBGrid(dem::Vertex&  vertex, double position[3], double length, double elements)
{
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
  //measurements
  double pos[3]; pos[0] = pos[1] = pos[2] = 0;
  delta::primitives::graphite::generateBrickFB(pos, 1, xCoordinates, yCoordinates, zCoordinates);
  double width = delta::primitives::properties::computeXZWidth(xCoordinates, yCoordinates, zCoordinates);
  double height = delta::primitives::properties::computeYw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///place components of 2d array structure
  double xwCell = delta::primitives::assembly::getxDiscritizationLength(length, elements);
  double halfxwCell = xwCell/2;

  double scalePercentage;
  if(xwCell < width)
  {//brick is bigger than grid
    double excess = width - xwCell;
    scalePercentage = ((width - excess)/width);
    printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  }else
  {
    double excess = xwCell - width;
    scalePercentage = 1+((xwCell - excess)/xwCell);
    printf("SCALE UP: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  }

  position[0] = halfxwCell; position[1] += (height/2)*scalePercentage; position[2] = halfxwCell;
  std::vector<std::array<double, 3>> N = delta::primitives::assembly::array3d(position, length, elements);

  for(auto i:N)
  {
    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
    delta::primitives::graphite::generateBrickFB(position, scalePercentage*0.9, xCoordinates, yCoordinates, zCoordinates);
    vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                             _epsilon, false, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);
    dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, false);
  }
}

void dem::mappings::CreateGrid::makeLoadNuclearGeometry(dem::Vertex&  vertex, double position[3])
{
  std::vector<double>  xCoordinates;
  std::vector<double>  yCoordinates;
  std::vector<double>  zCoordinates;

  //measurements
  iREAL pos[] = {0,0,0};
  delta::primitives::graphite::generateBrickFB(pos, 1, xCoordinates, yCoordinates, zCoordinates);
  double width = delta::primitives::properties::computeXZWidth(xCoordinates, yCoordinates, zCoordinates);
  double yw = delta::primitives::properties::computeXw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

  //read nuclear graphite schematics
  std::vector<std::vector<std::string>> componentGrid; std::vector<std::string> components;
  delta::sys::parseModelGridSchematics("input/nuclear_core", componentGrid, components);

  ///place components of 2d array structure
  int elements = 46;
  double length = 1;
  double xwCell = delta::primitives::assembly::getxDiscritizationLength(length, elements);
  double halfxwCell = xwCell/2;

  double scaleDownPercentage = 0;
  if(xwCell < width)
  {//brick is bigger than grid
    double excess = width - xwCell;
    scaleDownPercentage = ((width - excess)/width);
    printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scaleDownPercentage, width, xwCell, excess);
  }

  position[0] = halfxwCell; position[1] += ((yw/2)*scaleDownPercentage); position[2] = halfxwCell;
  std::vector<std::array<double, 3>> N = delta::primitives::assembly::array2d(position, length, elements);

  for(unsigned i=0; i<N.size(); i++)
  {
    position[0] = N[i][0]; position[1] = N[i][1]; position[2] = N[i][2];

    if(components[i] == "FB")
    {
      delta::primitives::graphite::generateBrickFB(position, scaleDownPercentage, xCoordinates, yCoordinates, zCoordinates);
      vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                               _epsilon, false, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);
      dem::mappings::CreateGrid::addParticleToState(xCoordinates, yCoordinates, zCoordinates, false);
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
