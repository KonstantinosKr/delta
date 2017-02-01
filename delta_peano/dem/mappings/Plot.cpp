#include "dem/mappings/Plot.h"
#include "dem/mappings/Collision.h"


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::Plot::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

/**
 * This is the only routine that we actually use.
 */
peano::MappingSpecification   dem::mappings::Plot::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::Serial,true);
}

peano::MappingSpecification   dem::mappings::Plot::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}
peano::MappingSpecification   dem::mappings::Plot::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::Plot::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::Plot::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::Plot::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log  dem::mappings::Plot::_log( "dem::mappings::Plot" );
int                  dem::mappings::Plot::_snapshotCounter( 0 );

void dem::mappings::Plot::beginIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  _writer       = new Writer();

  _vertexWriter = _writer->createVertexWriter();
  _cellWriter   = _writer->createCellWriter();

  _type                  = _writer->createCellDataWriter( "type(particle-centre=0,triangle=1,collision-point=2,link=3,collision-distance=4)", 1 );
  _level                 = _writer->createCellDataWriter( "level", 1 );
  _faceVertexAssociation = _writer->createCellDataWriter( "face-vertex-association", 1 );

  _velocitiesAndNormals  = _writer->createVertexDataWriter( "velocities-and-contact-normals", DIMENSIONS );
  _frictionNormals 		 = _writer->createVertexDataWriter( "friction-normals", DIMENSIONS );
  _particleVelocity		 = _writer->createVertexDataWriter( "particle-velocity", 1);
  _particleAngular		 = _writer->createVertexDataWriter( "particle-angular", DIMENSIONS);
  _particleDiameter  	 = _writer->createVertexDataWriter( "particle-radius", 1);
  _particleEpsilon  	 = _writer->createVertexDataWriter( "particle-epsilon", 1);
  _particleInfluence     = _writer->createVertexDataWriter( "particle-influence", 1);
  _vertexColoring 		 = _writer->createVertexDataWriter( "particle-coloring", 1);

  _vertexCounter         = 0;
  _particleCounter       = 0;
  _collisionPointCounter = 0;

  tarch::logging::CommandLineLogger::getInstance().setLogFormat("",true,false,false,true,true,"particle.log");

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::Plot::endIteration( dem::State&  solverState)
{
  logTraceInWith1Argument( "endIteration(State)", solverState );

  assertion( Collision::_collisionsOfNextTraversal.empty() );
  assertion( solverState.getNumberOfContactPoints()==0 || !Collision::_activeCollisions.empty() );

  for (std::map<int, std::vector<Collision::Collisions> >::const_iterator p = Collision::_activeCollisions.begin();
      p != Collision::_activeCollisions.end(); p++)
  {
	for (std::vector<Collision::Collisions>::const_iterator pp = p->second.begin(); pp != p->second.end(); pp++)
    {
	    for ( std::vector<delta::collision::contactpoint>::const_iterator ppp = pp->_contactPoints.begin();
        ppp != pp->_contactPoints.end(); ppp++)
      { //printf("Contacts:%d, between:%d and %d\n", contacts++, p->first, pp->_copyOfPartnerParticle._persistentRecords._globalParticleNumber);
    	tarch::la::Vector<3,double> v;
        v = ppp->x[0], ppp->x[1], ppp->x[2];
        int contactPointVertexIndex = _vertexWriter->plotVertex( v );
        int contactPointIndex       = _cellWriter->plotPoint(contactPointVertexIndex);

        v = ppp->normal[0], ppp->normal[1], ppp->normal[2];
        _velocitiesAndNormals->plotVertex(contactPointVertexIndex,v);

        if(ppp->frictionType>2)
        {
        	v = ppp->friction[0], ppp->friction[1], ppp->friction[2];
        	_frictionNormals->plotVertex(contactPointVertexIndex, v);
        }else
        {
        	_frictionNormals->plotVertex(contactPointVertexIndex, 0);
        }
        _particleVelocity->plotVertex(contactPointVertexIndex,0);
        _particleAngular->plotVertex(contactPointVertexIndex,0);
        _particleDiameter->plotVertex(contactPointVertexIndex,0);
        _particleEpsilon->plotVertex(contactPointVertexIndex,0);
        _particleInfluence->plotVertex(contactPointVertexIndex,0);
        _vertexColoring->plotVertex(contactPointVertexIndex,p->first);

        _type->plotCell(contactPointIndex,2);
        _level->plotCell(contactPointIndex,-1);
        _faceVertexAssociation->plotCell(contactPointIndex,-1);

        v = ppp->P[0], ppp->P[1], ppp->P[2];
        contactPointVertexIndex = _vertexWriter->plotVertex( v );
        contactPointIndex       = _cellWriter->plotPoint(contactPointVertexIndex);
        _velocitiesAndNormals->plotVertex(contactPointVertexIndex,0);
        _frictionNormals->plotVertex(contactPointVertexIndex, 0);
        _particleVelocity->plotVertex(contactPointVertexIndex,0);
        _particleAngular->plotVertex(contactPointVertexIndex,0);

        _particleDiameter->plotVertex(contactPointVertexIndex,0);
        _particleEpsilon->plotVertex(contactPointVertexIndex,0);
        _particleInfluence->plotVertex(contactPointVertexIndex,0);
        _vertexColoring->plotVertex(contactPointVertexIndex,0);

        _type->plotCell(contactPointIndex,4);
        _level->plotCell(contactPointIndex,-1);
        _faceVertexAssociation->plotCell(contactPointIndex,-1);

        v = ppp->Q[0], ppp->Q[1], ppp->Q[2];
        contactPointVertexIndex = _vertexWriter->plotVertex( v );
        contactPointIndex       = _cellWriter->plotPoint(contactPointVertexIndex);
        _velocitiesAndNormals->plotVertex(contactPointVertexIndex,0);
        _frictionNormals->plotVertex(contactPointVertexIndex, 0);
        _particleVelocity->plotVertex(contactPointVertexIndex,0);
        _particleAngular->plotVertex(contactPointVertexIndex,0);

        _particleDiameter->plotVertex(contactPointVertexIndex,0);
        _particleEpsilon->plotVertex(contactPointVertexIndex,0);
        _particleInfluence->plotVertex(contactPointVertexIndex,0);
        _vertexColoring->plotVertex(contactPointVertexIndex,0);

        _type->plotCell(contactPointIndex,4);
        _level->plotCell(contactPointIndex,-1);
        _faceVertexAssociation->plotCell(contactPointIndex,-1);

        _collisionPointCounter++;
      }
    }
  }

  _vertexWriter->close();
  _cellWriter->close();
  _type->close();
  _level->close();
  _faceVertexAssociation->close();

  _velocitiesAndNormals->close();
  _frictionNormals->close();
  _particleVelocity->close();
  _particleAngular->close();
  _particleDiameter->close();
  _particleEpsilon->close();
  _particleInfluence->close();
  _vertexColoring->close();

  std::ostringstream snapshotFileName;
  snapshotFileName << "geometry"
                   #ifdef Parallel
                   << "-rank-" << tarch::parallel::Node::getInstance().getRank()
                   #endif
                   << "-" << _snapshotCounter
                   << ".vtk";
  _writer->writeToFile( snapshotFileName.str() );

  _snapshotCounter++;


  tarch::logging::CommandLineLogger::getInstance().closeOutputStreamAndReopenNewOne();

  delete _vertexWriter;
  delete _cellWriter;
  delete _writer;
  delete _type;
  delete _level;
  delete _faceVertexAssociation;

  delete _frictionNormals;
  delete _velocitiesAndNormals;
  delete _particleVelocity;
  delete _particleAngular;
  delete _particleDiameter;
  delete _particleEpsilon;
  delete _particleInfluence;
  delete _vertexColoring;

  #ifdef Asserts
  logInfo( "endIteration(State)", "particles written=" << _particleCounter << ", collision points written=" << (_collisionPointCounter/2) << ", file=" << snapshotFileName.str() );
  #endif

  logTraceOutWith1Argument( "endIteration(State)", solverState);
}

void dem::mappings::Plot::touchVertexLastTime(
  dem::Vertex&                                  fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
  dem::Vertex * const                           coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&                                    coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  _vertexCounter++;

  int particleVertexLink[2];
  particleVertexLink[0] = _vertexWriter->plotVertex( fineGridX );

  _frictionNormals->plotVertex(particleVertexLink[0],0);
  _velocitiesAndNormals->plotVertex(particleVertexLink[0],0);
  _particleVelocity->plotVertex(particleVertexLink[0],0);
  _particleAngular->plotVertex(particleVertexLink[0],0);
  _particleDiameter->plotVertex(particleVertexLink[0],0);
  _particleEpsilon->plotVertex(particleVertexLink[0],0);
  _particleInfluence->plotVertex(particleVertexLink[0],0);
  _vertexColoring->plotVertex(particleVertexLink[0],0);

  logDebug( "touchVertexLastTime(...)", "vertex holds " << fineGridVertex.getNumberOfParticles() << " particles" );

  for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    _particleCounter++;
    records::Particle&  particle = fineGridVertex.getParticle(i);

    assertion1( particle._persistentRecords._centreOfMass(0)==particle._persistentRecords._centreOfMass(0), particle.toString() );

    particleVertexLink[1] = _vertexWriter->plotVertex( particle._persistentRecords._centre );

    if(!particle._persistentRecords._isObstacle)
    {
    	//it will only accept diameter not radius to plot the sphere thus multiply epsilon by 2
        tarch::la::Vector<3,double> v;
        double mag = std::sqrt(particle._persistentRecords._velocity(0)*particle._persistentRecords._velocity(0)+particle._persistentRecords._velocity(1)*particle._persistentRecords._velocity(1)+particle._persistentRecords._velocity(2)*particle._persistentRecords._velocity(2));
        //double magAng = std::sqrt(particle._persistentRecords._velocity(0)*particle._persistentRecords._velocity(0)+particle._persistentRecords._velocity(1)*particle._persistentRecords._velocity(1)+particle._persistentRecords._velocity(2)*particle._persistentRecords._velocity(2));
        v = particle._persistentRecords._velocity(0), particle._persistentRecords._velocity(1), particle._persistentRecords._velocity(2);
    	_velocitiesAndNormals->plotVertex(particleVertexLink[1],v);
    	_particleVelocity->plotVertex(particleVertexLink[1],mag);
    	v = particle._persistentRecords._angular(0), particle._persistentRecords._angular(1), particle._persistentRecords._angular(2);
    	_particleAngular->plotVertex(particleVertexLink[1],v);
    	//printf("%f %f %f\n", particle._persistentRecords._angular(0), particle._persistentRecords._angular(1), particle._persistentRecords._angular(2));
    	_particleDiameter->plotVertex(particleVertexLink[1], particle._persistentRecords._diameter);
    	_particleEpsilon->plotVertex(particleVertexLink[1], particle._persistentRecords._diameter+(particle._persistentRecords._epsilon*2));
    	_particleInfluence->plotVertex(particleVertexLink[1], particle._persistentRecords._influenceRadius*2);
    } else
    {
    	_velocitiesAndNormals->plotVertex(particleVertexLink[1],0);
    	_particleVelocity->plotVertex(particleVertexLink[1],0);
    	_particleAngular->plotVertex(particleVertexLink[1],0);
    	_particleDiameter->plotVertex(particleVertexLink[1], 0);
    	_particleEpsilon->plotVertex(particleVertexLink[1], 0);
    	_particleInfluence->plotVertex(particleVertexLink[1], 0);
    }

    _frictionNormals->plotVertex(particleVertexLink[1],0);
	_vertexColoring->plotVertex(particleVertexLink[1], particle._persistentRecords._globalParticleId);

    int lineFromParticleToHostVertex = _cellWriter->plotLine(particleVertexLink);
    _type->plotCell(lineFromParticleToHostVertex,3);
    _level->plotCell(lineFromParticleToHostVertex,coarseGridVerticesEnumerator.getLevel()+1);
    _faceVertexAssociation->plotCell(lineFromParticleToHostVertex,_vertexCounter);

    int particleCentre = _cellWriter->plotPoint(particleVertexLink[1]);
    _type->plotCell(particleCentre,0);
    _level->plotCell(particleCentre,coarseGridVerticesEnumerator.getLevel()+1);
    _faceVertexAssociation->plotCell(particleCentre,_vertexCounter);

    for (int j=0; j<particle._persistentRecords._numberOfTriangles; j++)
    {
      double* x = fineGridVertex.getXCoordinates(i);
      double* y = fineGridVertex.getYCoordinates(i);
      double* z = fineGridVertex.getZCoordinates(i);

      int vertexIndex[3];
      tarch::la::Vector<3,double> p;

      p = x[j*3+0], y[j*3+0], z[j*3+0];
      assertionEquals( p(0), p(0) );
      assertionEquals( p(1), p(1) );
      assertionEquals( p(2), p(2) );
      vertexIndex[0] = _vertexWriter->plotVertex(p);

      p = x[j*3+1], y[j*3+1], z[j*3+1];
      assertionEquals( p(0), p(0) );
      assertionEquals( p(1), p(1) );
      assertionEquals( p(2), p(2) );
      vertexIndex[1] = _vertexWriter->plotVertex(p);

      p = x[j*3+2], y[j*3+2], z[j*3+2];
      assertionEquals( p(0), p(0) );
      assertionEquals( p(1), p(1) );
      assertionEquals( p(2), p(2) );
      vertexIndex[2] = _vertexWriter->plotVertex(p);

      _velocitiesAndNormals->plotVertex(vertexIndex[0], 0);
      _velocitiesAndNormals->plotVertex(vertexIndex[1], 0);
      _velocitiesAndNormals->plotVertex(vertexIndex[2], 0);

      _frictionNormals->plotVertex(vertexIndex[0], 0);
      _frictionNormals->plotVertex(vertexIndex[1], 0);
      _frictionNormals->plotVertex(vertexIndex[2], 0);

	  _particleVelocity->plotVertex(vertexIndex[0], 0);
	  _particleVelocity->plotVertex(vertexIndex[1], 0);
	  _particleVelocity->plotVertex(vertexIndex[2], 0);

	  _particleAngular->plotVertex(vertexIndex[0], 0);
	  _particleAngular->plotVertex(vertexIndex[1], 0);
	  _particleAngular->plotVertex(vertexIndex[2], 0);

      _particleDiameter->plotVertex(vertexIndex[0], 0);
      _particleDiameter->plotVertex(vertexIndex[1], 0);
      _particleDiameter->plotVertex(vertexIndex[2], 0);

      _particleEpsilon->plotVertex(vertexIndex[0], 0);
      _particleEpsilon->plotVertex(vertexIndex[1], 0);
      _particleEpsilon->plotVertex(vertexIndex[2], 0);

  	  _particleInfluence->plotVertex(vertexIndex[0], 0);
  	  _particleInfluence->plotVertex(vertexIndex[1], 0);
	  _particleInfluence->plotVertex(vertexIndex[2], 0);

      _vertexColoring->plotVertex(vertexIndex[0], 0);
      _vertexColoring->plotVertex(vertexIndex[1], 0);
      _vertexColoring->plotVertex(vertexIndex[2], 0);

	  int faceIndex = _cellWriter->plotTriangle(vertexIndex);
      _type->plotCell(faceIndex,1);
      _level->plotCell(faceIndex,coarseGridVerticesEnumerator.getLevel()+1);
      _faceVertexAssociation->plotCell(faceIndex,_vertexCounter);
    }

    logInfo("runAsMaster(...)",   "partiId=" << particle._persistentRecords._globalParticleId  <<", mass___=" << particle._persistentRecords._mass << ", diamete=" << particle._persistentRecords._diameter << std::endl
			  	  	  	  	   << "influRa=" << particle._persistentRecords._influenceRadius <<", epsilon=" << particle._persistentRecords._epsilon << ", hMin___=" << particle._persistentRecords._hMin << std::endl
							   << "noOfTri=" << particle._persistentRecords._numberOfTriangles <<", isObsta=" << particle._persistentRecords._isObstacle << ", materia=" << particle._persistentRecords._material << std::endl
    					       << "linearX=" << particle._persistentRecords._velocity(0) <<", linearY=" << particle._persistentRecords._velocity(1) << ", linearZ=" << particle._persistentRecords._velocity(2) << std::endl
							   << "angulaX=" << particle._persistentRecords._angular(0) <<", angulaY=" << particle._persistentRecords._angular(1) << ", angulaZ=" << particle._persistentRecords._angular(2) << std::endl<< "angulaX=" << particle._persistentRecords._angular(0) <<", angulaY=" << particle._persistentRecords._angular(1) << ", angulaZ=" << particle._persistentRecords._angular(2) << std::endl
							   << "rangulX=" << particle._persistentRecords._referentialAngular(0) <<", rangulY=" << particle._persistentRecords._referentialAngular(1) << ", rangulZ=" << particle._persistentRecords._referentialAngular(2) << std::endl
							   << "centreX=" << particle._persistentRecords._centre(0) <<", centreY=" << particle._persistentRecords._centre(1) << ", centreZ=" << particle._persistentRecords._centre(2) << std::endl
							   << "cOfMasX=" << particle._persistentRecords._centreOfMass(0) <<", cOfMasY=" << particle._persistentRecords._centreOfMass(1) << ", cOfMasZ=" << particle._persistentRecords._centreOfMass(2) << std::endl
							   << "rcOfMaX=" << particle._persistentRecords._referentialCentreOfMass(0) <<", rcOfMaY=" << particle._persistentRecords._referentialCentreOfMass(1) << ", rcOfMaZ=" << particle._persistentRecords._referentialCentreOfMass(2) << std::endl
							   << "iner[0]=" << particle._persistentRecords._inertia(0) <<", iner[1]=" << particle._persistentRecords._inertia(1) << ", iner[2]=" << particle._persistentRecords._inertia(2) << std::endl
							   << "iner[3]=" << particle._persistentRecords._inertia(3) <<", iner[4]=" << particle._persistentRecords._inertia(4) << ", iner[5]=" << particle._persistentRecords._inertia(5) << std::endl
							   << "iner[6]=" << particle._persistentRecords._inertia(6) <<", iner[7]=" << particle._persistentRecords._inertia(7) << ", iner[8]=" << particle._persistentRecords._inertia(8) << std::endl
							   << "inve[0]=" << particle._persistentRecords._inverse(0) <<", inve[1]=" << particle._persistentRecords._inverse(1) << ", inve[2]=" << particle._persistentRecords._inverse(2) << std::endl
							   << "inve[3]=" << particle._persistentRecords._inverse(3) <<", inve[4]=" << particle._persistentRecords._inverse(4) << ", inve[5]=" << particle._persistentRecords._inverse(5) << std::endl
							   << "inve[6]=" << particle._persistentRecords._inverse(6) <<", inve[7]=" << particle._persistentRecords._inverse(7) << ", inve[8]=" << particle._persistentRecords._inverse(8) << std::endl
							   << "orie[0]=" << particle._persistentRecords._orientation(0) <<", orie[1]=" << particle._persistentRecords._orientation(1) << ", orie[2]=" << particle._persistentRecords._orientation(2) << std::endl
							   << "orie[3]=" << particle._persistentRecords._orientation(3) <<", orie[4]=" << particle._persistentRecords._orientation(4) << ", orie[5]=" << particle._persistentRecords._orientation(5) << std::endl
							   << "orie[6]=" << particle._persistentRecords._orientation(6) <<", orie[7]=" << particle._persistentRecords._orientation(7) << ", orie[8]=" << particle._persistentRecords._orientation(8) );
  }
  //logInfo
  //logWarning
  //logError
  //logDebug
  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::Plot::Plot(const Plot&  masterThread):
  _writer(masterThread._writer),
  _vertexWriter(masterThread._vertexWriter),
  _cellWriter(masterThread._cellWriter),

  _type(masterThread._type),
  _level(masterThread._level),
  _faceVertexAssociation(masterThread._faceVertexAssociation),

  _velocitiesAndNormals(masterThread._velocitiesAndNormals),
  _frictionNormals(masterThread._frictionNormals),
  _particleVelocity(masterThread._particleVelocity),
  _particleAngular(masterThread._particleAngular),
  _particleEpsilon(masterThread._particleEpsilon),
  _particleDiameter(masterThread._particleDiameter),
  _particleInfluence(masterThread._particleInfluence),
  _vertexColoring(masterThread._vertexColoring),

  _vertexCounter(masterThread._vertexCounter),
  _particleCounter(masterThread._particleCounter),
  _collisionPointCounter(masterThread._collisionPointCounter)
{}


void dem::mappings::Plot::mergeWithWorkerThread(const Plot& workerThread) {}
#endif


dem::mappings::Plot::Plot() {
  logTraceIn( "Plot()" );
  // @todo Insert your code here
  logTraceOut( "Plot()" );
}

dem::mappings::Plot::~Plot() {
  logTraceIn( "~Plot()" );
  // @todo Insert your code here
  logTraceOut( "~Plot()" );
}

void dem::mappings::Plot::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}

void dem::mappings::Plot::destroyHangingVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::Plot::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&         fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&         fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::Plot::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&         fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&         fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::Plot::destroyVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

void dem::mappings::Plot::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::Plot::destroyCell(
      const dem::Cell&           fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void dem::mappings::Plot::mergeWithNeighbour(
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

void dem::mappings::Plot::prepareSendToNeighbour(
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

void dem::mappings::Plot::prepareCopyToRemoteNode(
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

void dem::mappings::Plot::prepareCopyToRemoteNode(
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

void dem::mappings::Plot::mergeWithRemoteDataDueToForkOrJoin(
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

void dem::mappings::Plot::mergeWithRemoteDataDueToForkOrJoin(
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

bool dem::mappings::Plot::prepareSendToWorker(
  dem::Cell&                 fineGridCell,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfCell,
  int                                                 worker
) {
  logTraceIn( "prepareSendToWorker(...)" );
  // @todo Insert your code here
  logTraceOutWith1Argument( "prepareSendToWorker(...)", true );
  return true;
}

void dem::mappings::Plot::prepareSendToMaster(
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


void dem::mappings::Plot::mergeWithMaster(
  const dem::Cell&           workerGridCell,
  dem::Vertex * const        workerGridVertices,
 const peano::grid::VertexEnumerator& workerEnumerator,
  dem::Cell&                 fineGridCell,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfCell,
  int                                                 worker,
  const dem::State&          workerState,
  dem::State&                masterState
) {
  logTraceIn( "mergeWithMaster(...)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithMaster(...)" );
}


void dem::mappings::Plot::receiveDataFromMaster(
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


void dem::mappings::Plot::mergeWithWorker(
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


void dem::mappings::Plot::mergeWithWorker(
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

void dem::mappings::Plot::touchVertexFirstTime(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&         fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&         fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}

void dem::mappings::Plot::enterCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&            fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}

void dem::mappings::Plot::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "leaveCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
  // @todo Insert your code here
  logTraceOutWith1Argument( "leaveCell(...)", fineGridCell );
}

void dem::mappings::Plot::descend(
  dem::Cell * const          fineGridCells,
  dem::Vertex * const        fineGridVertices,
  const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell
) {
  logTraceInWith2Arguments( "descend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "descend(...)" );
}

void dem::mappings::Plot::ascend(
  dem::Cell * const    fineGridCells,
  dem::Vertex * const  fineGridVertices,
  const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell
) {
  logTraceInWith2Arguments( "ascend(...)", coarseGridCell.toString(), coarseGridVerticesEnumerator.toString() );
  // @todo Insert your code here
  logTraceOut( "ascend(...)" );
}
