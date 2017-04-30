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

void dem::mappings::CreateGrid::setVScheme(dem::Vertex&  vertex, int particleNumber)
{
  switch (_velocityScheme)
  {
    case noVScheme:
      vertex.getParticle(particleNumber)._persistentRecords._velocity(0) = 0;
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
		std::vector<double>  xCoordinates;
		std::vector<double>  yCoordinates;
		std::vector<double>  zCoordinates;

		dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];

		double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
                              fineGridVerticesEnumerator.getCellCenter()(1),
                              fineGridVerticesEnumerator.getCellCenter()(2)};

    if(_scenario == sla)
    {
				/*
				 *nuclear single layer array;
				 * experiment of seismic shakes concept: drop all components in coarse grid then reassign vertex to refined grid
				 */

				/*create floor*/
				centreAsArray[1] = 0.5;
				double floorHeight = 0.05;
        delta::primitives::cubes::generateHullCube(centreAsArray, 1, floorHeight, 1, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
				vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
										             _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS; _numberOfParticles++; _numberOfObstacles++;
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

				//measurements
				double position[] = {0,0,0};
				delta::primitives::graphite::generateBrickFB( position, 1, xCoordinates, yCoordinates, zCoordinates);
				double width = delta::primitives::properties::computeXZWidth(xCoordinates, yCoordinates, zCoordinates);
				double yw = delta::primitives::properties::computeXw(xCoordinates, yCoordinates, zCoordinates);
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

				//read nuclear graphite schematics
				std::vector<std::vector<std::string>> componentGrid; std::vector<std::string> components;
				delta::sys::parseModelGridSchematics("input/nuclear_core", componentGrid, components);

				///place components of 2d array structure
				int elements = 46;
				double length = 1;
				double xBoxlength = delta::primitives::assembly::getxDiscritizationLength(length, elements);
				double halfXBoxlength = xBoxlength/2;

				double scaleDownPercentage = 0;
				if(xBoxlength < width)
				{//brick is bigger than grid
					double excess = width - xBoxlength;
					scaleDownPercentage = ((width - excess)/width);
					printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scaleDownPercentage, width, xBoxlength, excess);
				}

				position[0] = halfXBoxlength;
				position[1] = (centreAsArray[1]+(floorHeight/2))+((yw/2)*scaleDownPercentage);
				position[2] = halfXBoxlength;

				std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array2d(position, length, elements);

				int counter = 0;
				for(auto i:tmp)
				{
					position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

					if(components[counter] == "FB")
					{
						//std::cout<< components[counter] << std::endl;
						delta::primitives::graphite::generateBrickFB( position, scaleDownPercentage, xCoordinates, yCoordinates, zCoordinates);
						vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
												             _epsilon, false, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

						_numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
						xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
					}
					counter++;
					//if(counter > 600)
					//	break;
				}

				return;
    } else if(_scenario == nuclearArray)
		{
				/* nuclear single layer array;
				 * experiment of seismic shakes:concept: drop all components in coarse grid then reassign vertex to refined grid */

				/*create floor*/
				centreAsArray[1] = 0.5;
				double floorHeight = 0.05;
        delta::primitives::cubes::generateHullCube(centreAsArray, 1, floorHeight, 1, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
				vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
										             _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS; _numberOfParticles++; _numberOfObstacles++;
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

				//measurements
				double position[3]; position[0] = position[1] = position[2] = 0;
				delta::primitives::graphite::generateBrickFB( position, 1, xCoordinates, yCoordinates, zCoordinates);
				double width = delta::primitives::properties::computeXZWidth(xCoordinates, yCoordinates, zCoordinates);
				double yw = delta::primitives::properties::computeXw(xCoordinates, yCoordinates, zCoordinates);
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

				///place components of 2d array structure
				int elements = 10;
				double length = 1;
				double xBoxlength = delta::primitives::assembly::getxDiscritizationLength(length, elements);
				double halfXBoxlength = xBoxlength/2;

				double scalePercentage;
				if(xBoxlength < width)
				{//brick is bigger than grid
					double excess = width - xBoxlength;
					scalePercentage = ((width - excess)/width);
					printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xBoxlength, excess);
				}else
				{
					double excess = xBoxlength - width;
					scalePercentage = 1+((xBoxlength - excess)/xBoxlength);
					printf("SCALE UP: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xBoxlength, excess);
				}

				position[0] = halfXBoxlength;
				position[1] = (centreAsArray[1]+(floorHeight/2))+((yw/2)*scalePercentage);
				position[2] = halfXBoxlength;

				std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, length, elements);
				for(auto i:tmp)
				{
					position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
					//delta::primitives::generateSurface( position, xBoxlength*0.98, xBoxlength*0.98, xCoordinates, yCoordinates, zCoordinates);
					delta::primitives::graphite::generateBrickFB(position, scalePercentage*0.9, xCoordinates, yCoordinates, zCoordinates);
					vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
											             _epsilon, false, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

					_numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
					xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
				}
				return;
		} else if(_scenario == hopperUniform)
		{
				/* uniform hopper experiment;
				 * particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom */
				//./dem-3d-release-vec 0.3 0.3 0.3 hopperUniformSphere 7000 reluctant-adaptive-grid 0.0000005 every-batch 10 1 sphere 50 | tee log400.log
				double _hopperWidth = 0.20;	double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

				//HOPPER DIAGONAL:0.382926
				/*delta::primitives::hopper::generateHopper(centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
				vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
																 _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

				_numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();*/
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//////////////////ARRAY
				iREAL xcuts = 10; iREAL ycuts = 1;
				iREAL margin = ((double)_hopperWidth/(double)xcuts)/2.0;
				iREAL minParticleDiameter = ((double)_hopperWidth/(double)xcuts)-(margin*2.0);
				//printf("minParDiameter:%.10f\n", minParticleDiameter);
        iREAL position[] = {(centreAsArray[0] - _hopperWidth/2) + margin, centreAsArray[1] + _hopperHeight/2, (centreAsArray[2] - _hopperWidth/2) + margin};
        std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, _hopperWidth, xcuts, _hopperWidth, ycuts);
        int N = tmp.size();

        double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
        double massPerParticle = totalMass/(double)N; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
        double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));
        /////////////////ARRAY


				//////////////////////SPHERE//////////////////////////////////////////////////////////////////////////////////////
        if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
        {
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            //delta::primitives::cubes::generateCube(position, (radius*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
            vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                 radius, _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++;_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
        }else
        {
          //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
          std::vector<std::vector<double>>  xCoordinatesArray;
          std::vector<std::vector<double>>  yCoordinatesArray;
          std::vector<std::vector<double>>  zCoordinatesArray;

          double reMassTotal = 0;
          double masssphere = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

            delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

            xCoordinatesArray.push_back(xCoordinates);
            yCoordinatesArray.push_back(yCoordinates);
            zCoordinatesArray.push_back(zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);
            reMassTotal += mt;
            masssphere += mt;
            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
          //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

          reMassTotal=0;
          for(unsigned j=0; j<xCoordinatesArray.size(); j++)
          {
            xCoordinates = xCoordinatesArray[j];
            yCoordinates = yCoordinatesArray[j];
            zCoordinates = zCoordinatesArray[j];

            std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);

            reMassTotal += mt;
            masssphere += ms;

            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinatesArray[j] = xCoordinates;
            yCoordinatesArray[j] = yCoordinates;
            zCoordinatesArray[j] = zCoordinates;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
          //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

          for(unsigned i=0; i<xCoordinatesArray.size(); i++)
          {
            std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
          }
        }

				///////////////////////**flooring creation*/ //floor DIAGONAL:0.344674///////////////////////////////////////////
				centreAsArray[1] = 0.3;
        /*double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
				vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
										             _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS; _numberOfParticles++; _numberOfObstacles++;
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();*/
				return;
		} else if(_scenario == hopperUniform1k)
    {
        /* uniform hopper experiment;
         * particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom */
        //./dem-3d-release-vec 0.3 0.3 0.3 hopperUniformSphere 7000 reluctant-adaptive-grid 0.0000005 every-batch 10 1 sphere 50 | tee log400.log
        double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

        delta::primitives::hopper::generateHopper(centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        //////////////////////////////////////////////////////////////////////////////////////////
        iREAL xcuts = 10; iREAL ycuts = 10;
        iREAL margin = ((double)_hopperWidth/(double)xcuts)/2.0;
        iREAL minParticleDiameter = ((double)_hopperWidth/(double)xcuts)-(margin*2.0);

        iREAL position[] = {(centreAsArray[0] - _hopperWidth/2) + margin, centreAsArray[1] + _hopperHeight/2, (centreAsArray[2] - _hopperWidth/2) + margin};
        std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, _hopperWidth, xcuts, _hopperWidth, ycuts);
        int N = tmp.size();

        double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

        double massPerParticle = totalMass/(double)N; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
        double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));
        //////////////////////////////////////////////////////////////////////////////////////////


        ////////SPHERE///////////////////////////////////////////////////////////////////////
        if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
        {
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                                     radius, _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++;_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
        }else
        {
          ////MESH/////////////////////////////////////////////////////////////////////////////
          std::vector<std::vector<double>>  xCoordinatesArray;
          std::vector<std::vector<double>>  yCoordinatesArray;
          std::vector<std::vector<double>>  zCoordinatesArray;

          double reMassTotal = 0;
          double masssphere = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

            delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

            xCoordinatesArray.push_back(xCoordinates);
            yCoordinatesArray.push_back(yCoordinates);
            zCoordinatesArray.push_back(zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);
            reMassTotal += mt;
            masssphere += mt;
            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
          //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

          reMassTotal=0;
          for(unsigned j=0; j<xCoordinatesArray.size(); j++)
          {
            xCoordinates = xCoordinatesArray[j];
            yCoordinates = yCoordinatesArray[j];
            zCoordinates = zCoordinatesArray[j];

            std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);

            reMassTotal += mt;
            masssphere += ms;

            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinatesArray[j] = xCoordinates;
            yCoordinatesArray[j] = yCoordinates;
            zCoordinatesArray[j] = zCoordinates;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
          //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

          for(unsigned i=0; i<xCoordinatesArray.size(); i++)
          {
            std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
          }

        }
        //////////////////////////////////////////////////////////////////////////////////////////

        /**flooring creation*/
        centreAsArray[1] = 0.3;
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

        _numberOfTriangles += xCoordinates.size()/DIMENSIONS; _numberOfParticles++; _numberOfObstacles++;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        return;
    } else if(_scenario == hopperUniform10k)
    {
        /* uniform hopper experiment;
         * particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom */
        //./dem-3d-release-vec 0.3 0.3 0.3 hopperUniformSphere 7000 reluctant-adaptive-grid 0.0000005 every-batch 10 1 sphere 50 | tee log400.log
        double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

        delta::primitives::hopper::generateHopper(centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        //////////////////////////////////////////////////////////////////////////////////////////
        iREAL xcuts = 10; iREAL ycuts = 100;
        iREAL margin = ((double)_hopperWidth/(double)xcuts)/2.0;
        iREAL minParticleDiameter = ((double)_hopperWidth/(double)xcuts)-(margin*2.0);

        iREAL position[] = {(centreAsArray[0] - _hopperWidth/2) + margin, centreAsArray[1] + _hopperHeight/2, (centreAsArray[2] - _hopperWidth/2) + margin};

        std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, _hopperWidth, xcuts, _hopperWidth, ycuts);
        int N = tmp.size();

        double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

        double massPerParticle = totalMass/(double)N; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
        double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));
        //////////////////////////////////////////////////////////////////////////////////////////


        ////////SPHERE///////////////////////////////////////////////////////////////////////
        if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
        {
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                                     radius, _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++;_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
        }else
        {
          ////MESH/////////////////////////////////////////////////////////////////////////////

          std::vector<std::vector<double>>  xCoordinatesArray;
          std::vector<std::vector<double>>  yCoordinatesArray;
          std::vector<std::vector<double>>  zCoordinatesArray;

          double reMassTotal = 0;
          double masssphere = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

            delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

            xCoordinatesArray.push_back(xCoordinates);
            yCoordinatesArray.push_back(yCoordinates);
            zCoordinatesArray.push_back(zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);
            reMassTotal += mt;
            masssphere += mt;
            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
          //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

          reMassTotal=0;
          for(unsigned j=0; j<xCoordinatesArray.size(); j++)
          {
            xCoordinates = xCoordinatesArray[j];
            yCoordinates = yCoordinatesArray[j];
            zCoordinates = zCoordinatesArray[j];

            std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);

            reMassTotal += mt;
            masssphere += ms;

            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinatesArray[j] = xCoordinates;
            yCoordinatesArray[j] = yCoordinates;
            zCoordinatesArray[j] = zCoordinates;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
          //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

          for(unsigned i=0; i<xCoordinatesArray.size(); i++)
          {
            std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
          }

        }
        //////////////////////////////////////////////////////////////////////////////////////////

        /**flooring creation*/
        centreAsArray[1] = 0.3;
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

        _numberOfTriangles += xCoordinates.size()/DIMENSIONS; _numberOfParticles++; _numberOfObstacles++;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        return;
    } else if(_scenario == hopperUniform50k)
    {
        /* uniform hopper experiment;
         * particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom */
        //./dem-3d-release-vec 0.3 0.3 0.3 hopperUniformSphere 7000 reluctant-adaptive-grid 0.0000005 every-batch 10 1 sphere 50 | tee log400.log
        double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

        delta::primitives::hopper::generateHopper(centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        //////////////////////////////////////////////////////////////////////////////////////////


        iREAL xcuts = 10; iREAL ycuts = 500;
        iREAL margin = ((double)_hopperWidth/(double)xcuts)/2.0;
        iREAL minParticleDiameter = ((double)_hopperWidth/(double)xcuts)-(margin*2.0);

        iREAL position[] = {(centreAsArray[0] - _hopperWidth/2) + margin, centreAsArray[1] + _hopperHeight/2, (centreAsArray[2] - _hopperWidth/2) + margin};

        std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, _hopperWidth, xcuts, _hopperWidth, ycuts);
        int N = tmp.size();

        double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg

        double massPerParticle = totalMass/(double)N;
        double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));
        //////////////////////////////////////////////////////////////////////////////////////////

        ////////SPHERE///////////////////////////////////////////////////////////////////////
        if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
        {

          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                                     radius, _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++;_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
        }else
        {
          ////MESH/////////////////////////////////////////////////////////////////////////////
          std::vector<std::vector<double>>  xCoordinatesArray;
          std::vector<std::vector<double>>  yCoordinatesArray;
          std::vector<std::vector<double>>  zCoordinatesArray;

          double reMassTotal = 0;
          double masssphere = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

            delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

            xCoordinatesArray.push_back(xCoordinates);
            yCoordinatesArray.push_back(yCoordinates);
            zCoordinatesArray.push_back(zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);
            reMassTotal += mt;
            masssphere += mt;
            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
          //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

          reMassTotal=0;
          for(unsigned j=0; j<xCoordinatesArray.size(); j++)
          {
            xCoordinates = xCoordinatesArray[j];
            yCoordinates = yCoordinatesArray[j];
            zCoordinates = zCoordinatesArray[j];

            std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::collision::material::MaterialDensity::WOOD);

            reMassTotal += mt;
            masssphere += ms;

            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinatesArray[j] = xCoordinates;
            yCoordinatesArray[j] = yCoordinates;
            zCoordinatesArray[j] = zCoordinates;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }
          //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

          for(unsigned i=0; i<xCoordinatesArray.size(); i++)
          {
            std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
          }

        }
        //////////////////////////////////////////////////////////////////////////////////////////

        /**flooring creation*/
        centreAsArray[1] = 0.3;
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

        _numberOfTriangles += xCoordinates.size()/DIMENSIONS; _numberOfParticles++; _numberOfObstacles++;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        return;
    } else if(_scenario == hopperNonUniform)
		{
				/*hopper experiment; particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom*/
				double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

				delta::primitives::hopper::generateHopper( centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
				vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
																 _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

				_numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

				//////////////////////////////////////////////////////////////////////////////////////////
        iREAL xcuts = 10.0; iREAL ycuts = 1.0;
        iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
        iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

        iREAL subcellx = ((double)minArraylengthX/(double)xcuts) - _epsilon*4;
        //iREAL subcelly = ((double)minArraylengthY/(double)ycuts) - _epsilon*4;

        iREAL margin = ((double)minArraylengthX/(double)xcuts)/2.0;
        printf("maxDiameter:%f\n", subcellx);

        iREAL position[] = {centreAsArray[0] - (minArraylengthX/2 - margin), centreAsArray[1] + minArraylengthY/2, centreAsArray[2] - (minArraylengthX/2 - margin)};
        std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, minArraylengthX, xcuts, minArraylengthX, ycuts);
        int N = tmp.size();
        double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
        //////////////////////////////////////////////////////////////////////////////////////////

        //////SPHERE//////////////////////////////////////////////////////////////////////
        if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
        {
          //double massPerParticle = totalMass/(double)N;
          //double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

          double reMassTotal = 0;
          std::vector<double> rad;
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

          int idx = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            //delta::primitives::cubes::generateCube(position, (rad[idx]*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
            vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                 rad[idx], rad[idx]*0.2, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
            idx++;
          }

				}	else {
				  //////MESH/////////////////////////////////////////////////////////////////////////
          double massPerParticle = totalMass/(double)N;
          double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

          std::vector<std::vector<double>>  xCoordinatesArray;
          std::vector<std::vector<double>>  yCoordinatesArray;
          std::vector<std::vector<double>>  zCoordinatesArray;

          double reMassTotal = 0;
          double masssphere = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            _minParticleDiam = subcellx/2;
            _maxParticleDiam = subcellx;
            double particleDiameter = _minParticleDiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(_maxParticleDiam-_minParticleDiam)));

            radius = particleDiameter/2;
            delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

            xCoordinatesArray.push_back(xCoordinates);
            yCoordinatesArray.push_back(yCoordinates);
            zCoordinatesArray.push_back(zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);
            reMassTotal += mt;
            masssphere += mt;
            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
          //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

          reMassTotal=0;
          for(unsigned j=0; j<xCoordinatesArray.size(); j++)
          {
            xCoordinates = xCoordinatesArray[j];
            yCoordinates = yCoordinatesArray[j];
            zCoordinates = zCoordinatesArray[j];

            std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);

            reMassTotal += mt;
            masssphere += ms;

            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinatesArray[j] = xCoordinates;
            yCoordinatesArray[j] = yCoordinates;
            zCoordinatesArray[j] = zCoordinates;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

          for(unsigned i=0; i<xCoordinatesArray.size(); i++)
          {
            std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
          }
				}

				//////////////////////////////////////////////////////////////////////////////////////////

				/*flooring creation*/
				centreAsArray[1] = 0.3;
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
				vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
										             _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;_numberOfParticles++; _numberOfObstacles++;
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
				return;
    } else if(_scenario == hopperNonUniform1k)
    {
        /*hopper experiment; particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom*/
        double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

        delta::primitives::hopper::generateHopper( centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        //////////////////////////////////////////////////////////////////////////////////////////
        iREAL xcuts = 10.0; iREAL ycuts = 10.0;
        iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
        iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

        iREAL subcellx = ((double)minArraylengthX/(double)xcuts) - _epsilon*4;
        //iREAL subcelly = ((double)minArraylengthY/(double)ycuts) - _epsilon*4;

        iREAL margin = ((double)minArraylengthX/(double)xcuts)/2.0;
        printf("maxDiameter:%f\n", subcellx);

        iREAL position[] = {centreAsArray[0] - (minArraylengthX/2 - margin), centreAsArray[1] + minArraylengthY/2, centreAsArray[2] - (minArraylengthX/2 - margin)};
        std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, minArraylengthX, xcuts, minArraylengthX, ycuts);
        int N = tmp.size();
        double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
        //////////////////////////////////////////////////////////////////////////////////////////

        //////SPHERE//////////////////////////////////////////////////////////////////////
        if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
        {
          //double massPerParticle = totalMass/(double)N;
          //double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

          double reMassTotal = 0;
          std::vector<double> rad;
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

          int idx = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            //delta::primitives::cubes::generateCube(position, (rad[idx]*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
            vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                 rad[idx], rad[idx]*0.2, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
            idx++;
          }

        } else {
          //////MESH/////////////////////////////////////////////////////////////////////////

          double massPerParticle = totalMass/(double)N;
          double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

          std::vector<std::vector<double>>  xCoordinatesArray;
          std::vector<std::vector<double>>  yCoordinatesArray;
          std::vector<std::vector<double>>  zCoordinatesArray;

          double reMassTotal = 0;
          double masssphere = 0;
          for(auto i:tmp)
          {
            position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
            _minParticleDiam = subcellx/2;
            _maxParticleDiam = subcellx;
            double particleDiameter = _minParticleDiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(_maxParticleDiam-_minParticleDiam)));

            radius = particleDiameter/2;
            delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

            xCoordinatesArray.push_back(xCoordinates);
            yCoordinatesArray.push_back(yCoordinates);
            zCoordinatesArray.push_back(zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);
            reMassTotal += mt;
            masssphere += mt;
            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
          //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

          reMassTotal=0;
          for(unsigned j=0; j<xCoordinatesArray.size(); j++)
          {
            xCoordinates = xCoordinatesArray[j];
            yCoordinates = yCoordinatesArray[j];
            zCoordinates = zCoordinatesArray[j];

            std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

            double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
            double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
            double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
            double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);

            reMassTotal += mt;
            masssphere += ms;

            //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
            xCoordinatesArray[j] = xCoordinates;
            yCoordinatesArray[j] = yCoordinates;
            zCoordinatesArray[j] = zCoordinates;
            xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
          }

          //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

          for(unsigned i=0; i<xCoordinatesArray.size(); i++)
          {
            std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
            vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
            _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
          }
        }

        //////////////////////////////////////////////////////////////////////////////////////////

        /*flooring creation*/
        centreAsArray[1] = 0.3;
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

        _numberOfTriangles += xCoordinates.size()/DIMENSIONS;_numberOfParticles++; _numberOfObstacles++;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        return;
     } else if(_scenario == hopperNonUniform10k)
     {
         /*hopper experiment; particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom*/
         double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

         delta::primitives::hopper::generateHopper( centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
         vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                  _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

         _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
         xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

         //////////////////////////////////////////////////////////////////////////////////////////
         iREAL xcuts = 10.0; iREAL ycuts = 100.0;
         iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
         iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

         iREAL subcellx = ((double)minArraylengthX/(double)xcuts) - _epsilon*4;
         //iREAL subcelly = ((double)minArraylengthY/(double)ycuts) - _epsilon*4;

         iREAL margin = ((double)minArraylengthX/(double)xcuts)/2.0;
         printf("maxDiameter:%f\n", subcellx);

         iREAL position[] = {centreAsArray[0] - (minArraylengthX/2 - margin), centreAsArray[1] + minArraylengthY/2, centreAsArray[2] - (minArraylengthX/2 - margin)};
         std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, minArraylengthX, xcuts, minArraylengthX, ycuts);
         int N = tmp.size();
         double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
         //////////////////////////////////////////////////////////////////////////////////////////

         //////SPHERE//////////////////////////////////////////////////////////////////////
         if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
         {
           //double massPerParticle = totalMass/(double)N;
           //double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

           double reMassTotal = 0;
           std::vector<double> rad;
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

           int idx = 0;
           for(auto i:tmp)
           {
             position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
             //delta::primitives::cubes::generateCube(position, (rad[idx]*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
             vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                  rad[idx], rad[idx]*0.2, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
             _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
             xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
             idx++;
           }

         } else {
           //////MESH/////////////////////////////////////////////////////////////////////////

           double massPerParticle = totalMass/(double)N;
           double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

           std::vector<std::vector<double>>  xCoordinatesArray;
           std::vector<std::vector<double>>  yCoordinatesArray;
           std::vector<std::vector<double>>  zCoordinatesArray;

           double reMassTotal = 0;
           double masssphere = 0;
           for(auto i:tmp)
           {
             position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
             _minParticleDiam = subcellx/2;
             _maxParticleDiam = subcellx;
             double particleDiameter = _minParticleDiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(_maxParticleDiam-_minParticleDiam)));

             radius = particleDiameter/2;
             delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

             xCoordinatesArray.push_back(xCoordinates);
             yCoordinatesArray.push_back(yCoordinates);
             zCoordinatesArray.push_back(zCoordinates);

             double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
             double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
             double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
             double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);
             reMassTotal += mt;
             masssphere += mt;
             //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
             xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
           }

           double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
           //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

           reMassTotal=0;
           for(unsigned j=0; j<xCoordinatesArray.size(); j++)
           {
             xCoordinates = xCoordinatesArray[j];
             yCoordinates = yCoordinatesArray[j];
             zCoordinates = zCoordinatesArray[j];

             std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
             delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

             double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
             double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
             double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
             double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);

             reMassTotal += mt;
             masssphere += ms;

             //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
             xCoordinatesArray[j] = xCoordinates;
             yCoordinatesArray[j] = yCoordinates;
             zCoordinatesArray[j] = zCoordinates;
             xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
           }

           //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

           for(unsigned i=0; i<xCoordinatesArray.size(); i++)
           {
             std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
             vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                      _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
             _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
           }
         }

         //////////////////////////////////////////////////////////////////////////////////////////

         /*flooring creation*/
         centreAsArray[1] = 0.3;
         double height = 0.05; double width = 0.35;
         delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
         vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                  _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

         _numberOfTriangles += xCoordinates.size()/DIMENSIONS;_numberOfParticles++; _numberOfObstacles++;
         xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
         return;
     }  else if(_scenario == hopperNonUniform50k)
     {
         /*hopper experiment; particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom*/
         double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5; double _hopperHatch = 0.10;

         delta::primitives::hopper::generateHopper( centreAsArray, _hopperWidth, _hopperHeight, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);
         vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                  _epsilon, true, delta::collision::material::MaterialType::GOLD, false, _numberOfParticles);

         _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
         xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

         //////////////////////////////////////////////////////////////////////////////////////////

         iREAL xcuts = 10.0; iREAL ycuts = 500.0;
         iREAL minArraylengthX = (double)_hopperWidth - _epsilon * 6;
         iREAL minArraylengthY = (double)_hopperHeight - _epsilon * 6;

         iREAL subcellx = ((double)minArraylengthX/(double)xcuts) - _epsilon*4;
         //iREAL subcelly = ((double)minArraylengthY/(double)ycuts) - _epsilon*4;

         iREAL margin = ((double)minArraylengthX/(double)xcuts)/2.0;
         printf("maxDiameter:%f\n", subcellx);

         iREAL position[] = {centreAsArray[0] - (minArraylengthX/2 - margin), centreAsArray[1] + minArraylengthY/2, centreAsArray[2] - (minArraylengthX/2 - margin)};
         std::vector<std::array<double, 3>> tmp = delta::primitives::assembly::array3d(position, minArraylengthX, xcuts, minArraylengthX, ycuts);
         int N = tmp.size();
         double totalMass = 0.05; //0.5 delta::collision::MaterialType::GOLD, 0.05 delta::collision::MaterialType::WOOD//kg
         //////////////////////////////////////////////////////////////////////////////////////////

         //////SPHERE//////////////////////////////////////////////////////////////////////
         if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
         {
           //double massPerParticle = totalMass/(double)N;
           //double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

           double reMassTotal = 0;
           std::vector<double> rad;
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

           int idx = 0;
           for(auto i:tmp)
           {
             position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
             //delta::primitives::cubes::generateCube(position, (rad[idx]*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
             vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                  rad[idx], rad[idx]*0.2, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
             _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
             xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
             idx++;
           }

         } else {
           //////MESH/////////////////////////////////////////////////////////////////////////

           double massPerParticle = totalMass/(double)N;
           double radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::collision::material::MaterialDensity::WOOD)), (1.0/3.0));

           std::vector<std::vector<double>>  xCoordinatesArray;
           std::vector<std::vector<double>>  yCoordinatesArray;
           std::vector<std::vector<double>>  zCoordinatesArray;

           double reMassTotal = 0;
           double masssphere = 0;
           for(auto i:tmp)
           {
             position[0] = i[0]; position[1] = i[1]; position[2] = i[2];
             _minParticleDiam = subcellx/2;
             _maxParticleDiam = subcellx;
             double particleDiameter = _minParticleDiam + static_cast <double>(rand()) / (static_cast <double> (RAND_MAX/(_maxParticleDiam-_minParticleDiam)));

             radius = particleDiameter/2;
             delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);

             xCoordinatesArray.push_back(xCoordinates);
             yCoordinatesArray.push_back(yCoordinates);
             zCoordinatesArray.push_back(zCoordinates);

             double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
             double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
             double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
             double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);
             reMassTotal += mt;
             masssphere += mt;
             //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
             xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
           }

           double rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
           //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

           reMassTotal=0;
           for(unsigned j=0; j<xCoordinatesArray.size(); j++)
           {
             xCoordinates = xCoordinatesArray[j];
             yCoordinates = yCoordinatesArray[j];
             zCoordinates = zCoordinatesArray[j];

             std::array<double, 3> ar = tmp[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
             delta::primitives::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

             double mt = delta::primitives::properties::computeMass(xCoordinates, yCoordinates, zCoordinates, delta::collision::material::MaterialType::WOOD);
             double vt = delta::primitives::properties::computeVolume(xCoordinates, yCoordinates, zCoordinates);
             double vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
             double ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::collision::material::MaterialDensity::WOOD);

             reMassTotal += mt;
             masssphere += ms;

             //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
             xCoordinatesArray[j] = xCoordinates;
             yCoordinatesArray[j] = yCoordinates;
             zCoordinatesArray[j] = zCoordinates;
             xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
           }

           //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);

           for(unsigned i=0; i<xCoordinatesArray.size(); i++)
           {
             std::array<double, 3> ar = tmp[i]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
             vertex.createNewParticle(position, xCoordinatesArray[i], yCoordinatesArray[i], zCoordinatesArray[i],
                                      _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);
             _numberOfParticles++; _numberOfTriangles += xCoordinatesArray[i].size()/DIMENSIONS;
           }
         }

         //////////////////////////////////////////////////////////////////////////////////////////

         /*flooring creation*/
         centreAsArray[1] = 0.3;
         double height = 0.05; double width = 0.35;
         delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
         vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                  _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

         _numberOfTriangles += xCoordinates.size()/DIMENSIONS;_numberOfParticles++; _numberOfObstacles++;
         xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
         return;
     } else if(_scenario == frictionStaticSphere)
     {
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

				_numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
				xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        double radius = 0.01;
        double eps = radius*0.2;

        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height, centreAsArray[2]};

        delta::primitives::cubes::generateCube(position, (radius*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        int newParticleNumber = vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                                 radius, eps, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        setVScheme(vertex, newParticleNumber);
				return;
			} else if(_scenario == frictionSlideSphere)
			{
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::GOLD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        double radius = 0.01;
        double eps = radius*0.2;

        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius + eps*2, centreAsArray[2]};

        delta::primitives::cubes::generateCube(position, (radius*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        int newParticleNumber = vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                                 radius, eps, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        setVScheme(vertex, newParticleNumber);
				return;
			} else if(_scenario == frictionRollSphere)
      {
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0.1, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        double radius = 0.01;
        double eps = radius*0.2;

        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius/2 + eps*2, centreAsArray[2]};

        delta::primitives::cubes::generateCube(position, (radius*2)*0.9, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        int newParticleNumber = vertex.createNewParticleSphereRadius(position, xCoordinates, yCoordinates, zCoordinates,
                                                                 radius, eps, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        setVScheme(vertex, newParticleNumber);
        return;
      } else if(_scenario == frictionStaticMesh)
      {
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        double radius = 0.01;
        double eps = radius*0.2;
        width = (radius*2)*0.9;

        //iREAL position[] = {centreAsArray[0], centreAsArray[1] + ((height)/1.5)+width/10, centreAsArray[2]};

        iREAL position[] = {centreAsArray[0], centreAsArray[1] + ((height)/1.5)-width/2, centreAsArray[2]};
        delta::primitives::cubes::generateHullCube(position, width, xCoordinates, yCoordinates, zCoordinates);
        //delta::primitives::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
        //delta::primitives::granulates::loadParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates);
        int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                                                     eps, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        setVScheme(vertex, newParticleNumber);
        return;
      } else if(_scenario == frictionSlideMesh)
      {
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        double radius = 0.01;
        double eps = radius*0.2;

        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius + eps, centreAsArray[2]};

        //delta::primitives::cubes::generateHullCube(position, (radius*2)*0.9, xCoordinates, yCoordinates, zCoordinates);
        //delta::primitives::granulates::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
        delta::primitives::granulates::loadParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates);
        int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                                                     eps, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        setVScheme(vertex, newParticleNumber);
        return;
      } else if(_scenario == frictionRollMesh)
      {
        double height = 0.05; double width = 0.35;
        delta::primitives::cubes::generateHullCube(centreAsArray, width, height, width, 0, 0, 0.1, xCoordinates, yCoordinates, zCoordinates);
        vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                 _epsilon, true, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfObstacles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        double radius = 0.01;
        double eps = radius*0.2;

        iREAL position[] = {centreAsArray[0], centreAsArray[1] + height/2 + radius, centreAsArray[2]};

        //delta::primitives::cubes::generateHullCube(position, (radius*2)*0.9, xCoordinates, yCoordinates, zCoordinates);
        //delta::primitives::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
        delta::primitives::granulates::loadParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates);
        int newParticleNumber = vertex.createNewParticle(position, xCoordinates, yCoordinates, zCoordinates,
                                                     eps, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        setVScheme(vertex, newParticleNumber);
        return;
      } else if(_scenario == TwoParticlesCrash)
      {
        centreAsArray[0] += _maxParticleDiam*2;
        delta::primitives::granulates::generateParticle(centreAsArray, _maxParticleDiam, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
        int newParticleNumber = vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        vertex.getParticle(newParticleNumber)._persistentRecords._velocity = -0.5;
        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

        centreAsArray[0] -= _maxParticleDiam*4;
        delta::primitives::granulates::generateParticle(centreAsArray, _maxParticleDiam, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
        newParticleNumber = vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
                                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

        vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0.5;
        _numberOfParticles++; _numberOfTriangles += xCoordinates.size()/DIMENSIONS;
        xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
        return;
      }
	}

	if(_scenario == sla                       ||
     _scenario == nuclearArray              ||
	   _scenario == TwoParticlesCrash         ||
     _scenario == hopperUniform             ||
     _scenario == hopperUniform1k           ||
     _scenario == hopperUniform10k          ||
     _scenario == hopperUniform50k          ||
     _scenario == hopperNonUniform          ||
     _scenario == hopperNonUniform1k        ||
     _scenario == hopperNonUniform10k       ||
     _scenario == hopperNonUniform50k       ||
     _scenario == frictionStaticSphere      ||
     _scenario == frictionSlideSphere       ||
     _scenario == frictionRollSphere        ||
     _scenario == frictionStaticMesh        ||
     _scenario == frictionSlideMesh         ||
     _scenario == frictionRollMesh)
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
			logWarning( "createCell(...)", "particle size has been too small for coarsest prescribed grid. Reduce particle size to " << std::min(_minParticleDiam,_maxParticleDiam) << "-" << fineGridVerticesEnumerator.getCellSize()(0) );
		} else {
			logDebug( "createCell", "particlesInCellPerAxis="<< particlesInCellPerAxis );
		}

		dfor(k,particlesInCellPerAxis)
		{
			const tarch::la::Vector<DIMENSIONS,double> centre = fineGridVerticesEnumerator.getVertexPosition() + _maxParticleDiam * (k.convertScalar<double>()+0.5);

			double particleDiameter = (_minParticleDiam + (_maxParticleDiam-_minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) ) / std::sqrt( DIMENSIONS );
			dem::Vertex&  vertex = fineGridVertices[fineGridVerticesEnumerator(0)];

			double centreAsArray[] = {centre(0),centre(1),centre(2)};

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
			/*else if(hopperUniformSphere1200)
			{
        double _hopperWidth = 0.20; double _hopperHeight = _hopperWidth/1.5;

	      double minx = centreAsArray[0]-_hopperWidth/2;
	      double maxx = centreAsArray[0]+_hopperWidth/2;
	      double miny = centreAsArray[1]-_hopperHeight/2;
	      double maxy = centreAsArray[1]+fineGridVerticesEnumerator.getCellSize()(0)/3;
	      double minz = centreAsArray[2]-_hopperWidth/2;
	      double maxz = centreAsArray[2]+_hopperWidth/2;

			  bool withinXrange = centreAsArray[0] > fineGridVerticesEnumerator.getCellSize()(0) && centreAsArray[0] < fineGridVerticesEnumerator.getCellSize()(0)*2;
			  bool withinYrange = centreAsArray[1] > fineGridVerticesEnumerator.getCellSize()(0) && centreAsArray[1] < fineGridVerticesEnumerator.getCellSize()(0)*2;
        bool withinZrange = centreAsArray[2] > fineGridVerticesEnumerator.getCellSize()(0) && centreAsArray[2] < fineGridVerticesEnumerator.getCellSize()(0)*2;

	      if(withinXrange && withinYrange && withinZrange)
	      {

	      }
        return;
			}*/

			int newParticleNumber = vertex.createNewParticle(centreAsArray, xCoordinates, yCoordinates, zCoordinates,
									                                     _epsilon, false, delta::collision::material::MaterialType::WOOD, true, _numberOfParticles);

			#ifdef STATSPARTICLE
			  logWarning( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter << " and id: " << particleId);
			#endif
			_numberOfTriangles += xCoordinates.size()/DIMENSIONS; _numberOfParticles++;

			/*Set velocities for particle filling for scenario*/
			setVScheme(vertex, newParticleNumber);
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
