#include "dem/mappings/Plot.h"
#include "dem/mappings/Collision.h"


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::Plot::communicationSpecification() const {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}

peano::MappingSpecification   dem::mappings::Plot::touchVertexFirstTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}

/**
 * This is the only routine that we actually use.
 */
peano::MappingSpecification   dem::mappings::Plot::touchVertexLastTimeSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::Serial,true);
}
peano::MappingSpecification   dem::mappings::Plot::enterCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::Plot::leaveCellSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::Plot::ascendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::Plot::descendSpecification(int level) const {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log  dem::mappings::Plot::_log( "dem::mappings::Plot" );
int                  dem::mappings::Plot::_snapshotCounter( 0 );
int                  dem::mappings::Plot::_mini;
int                  dem::mappings::Plot::_maxi;

void dem::mappings::Plot::beginIteration(dem::State&  solverState)
{
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  _iterationNumber       = solverState.getTimeStep();

  if(_mini < _iterationNumber && _iterationNumber < _maxi)
  {
    //printf("ENTERED %i %i %i\n", _mini, _maxi, _iterationNumber);

    _writer       = new Writer();

    _vertexWriter = _writer->createVertexWriter();
    _cellWriter   = _writer->createCellWriter();

    _type                  = _writer->createCellDataWriter( "type(particle-centre=0,link=1,triangle=2,triangleEpsilon=3,collision-point=4,collision-distancePQ=5,triangle-normal=6)", 1 );
    _level                 = _writer->createCellDataWriter( "level", 1 );
    _faceVertexAssociation = _writer->createCellDataWriter( "face-vertex-association", 1 );

    _velocitiesAndNormals   = _writer->createVertexDataWriter( "velocities-and-contact-normals", DIMENSIONS );
    _frictionNormals 		    = _writer->createVertexDataWriter( "friction-normals", DIMENSIONS );
    _particleVelocity		 = _writer->createVertexDataWriter( "particle-velocity", 1);
    _particleAngular		 = _writer->createVertexDataWriter( "particle-angular", DIMENSIONS);
    _particleDiameter  	 = _writer->createVertexDataWriter( "particle-radius", 1);
    _particleEpsilon  	 = _writer->createVertexDataWriter( "particle-epsilon", 1);
    _particleInfluence   = _writer->createVertexDataWriter( "particle-influence", 1);
    _vertexColoring 		 = _writer->createVertexDataWriter( "particle-coloring", 1);

    _vertexCounter         = 0;
    _particleCounter       = 0;
    _collisionPointCounter = 0;

    // _trackParticle = _trackID >= 0 ? true : false;
  }
  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::Plot::endIteration( dem::State&  solverState)
{
  logTraceInWith1Argument( "endIteration(State)", solverState );

  //TODO print mesh epsilon by resizing mess and creating a double
  assertion( Collision::_collisionsOfNextTraversal.empty() );
  assertion( solverState.getNumberOfContactPoints()==0 || !Collision::_activeCollisions.empty() );

  _iterationNumber       = solverState.getTimeStep();

  if(_mini < _iterationNumber && _iterationNumber < _maxi)
  {
    //printf("ENTERED %i %i %i\n", _mini, _maxi, _iterationNumber);

    std::vector<std::pair<int,int>> particlePrinted;
    //loop every collision list of every particle
    for(auto p:Collision::_activeCollisions)
    {
      //loop every partner particle in list of collisions
      for(auto pp:p.second)
      {
        //loop every contact point
        for (auto ppp:pp._contactPoints)
        {
          tarch::la::Vector<3,double> v;
          v = ppp.x[0], ppp.x[1], ppp.x[2];
          int contactPointVertexIndex = _vertexWriter->plotVertex( v );
          int contactPointIndex       = _cellWriter->plotPoint(contactPointVertexIndex);

          //check if force normal points towards master
          v = ppp.normal[0], ppp.normal[1], ppp.normal[2];

          _velocitiesAndNormals->plotVertex(contactPointVertexIndex,v);

          double force[]  = {0.0,0.0,0.0};
          double torque[] = {0.0,0.0,0.0};
          double friction [] = {0.0, 0.0, 0.0};

          /* check if it is master so that normal points towards it
          bool isMaster = false;

          iREAL tmp[] = {((ppp.P[0]-ppp.Q[0])/ppp.getDistance()), ((ppp.P[1]-ppp.Q[1])/ppp.getDistance()), ((ppp.P[2]-ppp.Q[2])/ppp.getDistance())};
          if(tmp[0]==ppp.normal[0] && tmp[1] == ppp.normal[1] && tmp[2] == ppp.normal[2])
          {
            isMaster = true;
          }

          if(isMaster)
          {
            _velocitiesAndNormals->plotVertex(contactPointVertexIndex,v);
          } else {
            tarch::la::Vector<3, double> t = {0, 1, 0};
            _velocitiesAndNormals->plotVertex(contactPointVertexIndex,t);
          }*/

          if(ppp.friction)
          {
            v = friction[0], friction[1], friction[2];
            _frictionNormals->plotVertex(contactPointVertexIndex, v);
          } else {
            _frictionNormals->plotVertex(contactPointVertexIndex, 0);
          }

          _particleVelocity->plotVertex(contactPointVertexIndex,0);
          _particleAngular->plotVertex(contactPointVertexIndex,0);
          _particleDiameter->plotVertex(contactPointVertexIndex,0);
          _particleEpsilon->plotVertex(contactPointVertexIndex,0);
          _particleInfluence->plotVertex(contactPointVertexIndex,0);
          _vertexColoring->plotVertex(contactPointVertexIndex,p.first);

          _type->plotCell(contactPointIndex,4); //contact point
          _level->plotCell(contactPointIndex,-1);
          _faceVertexAssociation->plotCell(contactPointIndex,-1);

          v = ppp.P[0], ppp.P[1], ppp.P[2];
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

          _type->plotCell(contactPointIndex,5);  //min distance P
          _level->plotCell(contactPointIndex,-1);
          _faceVertexAssociation->plotCell(contactPointIndex,-1);

          v = ppp.Q[0], ppp.Q[1], ppp.Q[2];
          contactPointVertexIndex = _vertexWriter->plotVertex( v );
          contactPointIndex       = _cellWriter->plotPoint(contactPointVertexIndex);
          _velocitiesAndNormals->plotVertex(contactPointVertexIndex,0);
          _frictionNormals->plotVertex(contactPointVertexIndex, 0);
          _particleVelocity->plotVertex(contactPointVertexIndex,0);
          _particleAngular->plotVertex(contactPointVertexIndex,0);

          _particleDiameter->plotVertex(contactPointVertexIndex,0);
          _particleEpsilon->plotVertex(contactPointVertexIndex,0);
          _particleInfluence->plotVertex(contactPointVertexIndex,0);
          _vertexColoring->plotVertex(contactPointVertexIndex,1);

          _type->plotCell(contactPointIndex,5); //min distance Q
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
                     << "-" << _snapshotCounter;
    _writer->writeToFile( snapshotFileName.str() );
    #ifdef Asserts
    logInfo( "endIteration(State)", "particles written=" << _particleCounter << ", collision points written=" << (_collisionPointCounter/2) << ", file=" << snapshotFileName.str() );
    #endif
    _snapshotCounter++;

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
  }


  for(auto p:Collision::_activeCollisions)
  {
    for(auto pp:p.second)
    {
      int masterID = p.first, slaveID = pp._copyOfPartnerParticle.getGlobalParticleId();
      iREAL *centreOfMass, *referentialCentreOfMass, *angular, *referentialAngular, *velocity, *inverse, *orientation, mass, material;
      //find master data in collision data as slave
      for(auto iCollision:Collision::_activeCollisions)
      {
        for(auto j:iCollision.second)
        {
          if(j._copyOfPartnerParticle.getGlobalParticleId() == masterID)
          {
            centreOfMass = &j._copyOfPartnerParticle._persistentRecords._centreOfMass(0);
            referentialCentreOfMass = &j._copyOfPartnerParticle._persistentRecords._referentialCentreOfMass(0);
            angular = &j._copyOfPartnerParticle._persistentRecords._angular(0);
            referentialAngular = &j._copyOfPartnerParticle._persistentRecords._referentialAngular(0);
            velocity = &j._copyOfPartnerParticle._persistentRecords._velocity(0);
            inverse = &j._copyOfPartnerParticle._persistentRecords._inverse(0);
            orientation = &j._copyOfPartnerParticle._persistentRecords._orientation(0);
            mass = j._copyOfPartnerParticle.getMass();
            material = j._copyOfPartnerParticle.getMaterial();

            break;
          }
        }
      }

      //loop every contact point
      for (auto ppp:pp._contactPoints)
      {

        #ifdef CONTACTSTATS
        /*
        if(_trackParticle)
        for(int i = 0; i<_trackParticleID.size(); i++)
        {
          //iftrackplotting enabled and one or more id are enables then print their data
          if(_trackParticleID[i] == p.first)
          {

          }
        }
        */
        std::cout << "#####CONTACT-DATA#####" << std::endl
                  << "contactId=" << std::fixed << std::setprecision(10) << std::to_string(ppp.x[0]+ppp.x[1]+ppp.x[2]) << ", MasterId=" << std::fixed << std::setprecision(10) << ppp.master << ", SlaveId=" << std::fixed << std::setprecision(10) << ppp.slave << std::endl
                  << "friction=" << std::fixed << std::setprecision(10) << ppp.friction << std::endl
                  << "distance=" << std::fixed << std::setprecision(10) << ppp.getDistance() << ", depth=" << std::fixed << std::setprecision(10) << ppp.depth << ", epsilonTotal=" << std::fixed << std::setprecision(10) << ppp.epsilonTotal << std::endl;

        double rforce[3]  = {0.0,0.0,0.0};
        double rtorque[3] = {0.0,0.0,0.0};
        double rfriction[3] = {0.0,0.0,0.0};
        delta::forces::getContactForce(ppp,
                                      centreOfMass,
                                      referentialCentreOfMass,
                                      angular,
                                      referentialAngular,
                                      velocity,
                                      mass,
                                      inverse,
                                      orientation,
                                      material,

                                       &(pp._copyOfPartnerParticle._persistentRecords._centreOfMass(0)),
                                       &(pp._copyOfPartnerParticle._persistentRecords._referentialCentreOfMass(0)),
                                       &(pp._copyOfPartnerParticle._persistentRecords._angular(0)),
                                       &(pp._copyOfPartnerParticle._persistentRecords._referentialAngular(0)),
                                       &(pp._copyOfPartnerParticle._persistentRecords._velocity(0)),
                                       pp._copyOfPartnerParticle.getMass(),
                                       &(pp._copyOfPartnerParticle._persistentRecords._inverse(0)),
                                       &(pp._copyOfPartnerParticle._persistentRecords._orientation(0)),
                                       pp._copyOfPartnerParticle.getMaterial(),
                                       rforce, rtorque, rfriction,
                                       (dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere));

        std::cout  << "xX=" << std::fixed << std::setprecision(10) << ppp.x[0] <<", xY=" << std::fixed << std::setprecision(10) << ppp.x[1] << ", xZ=" << std::fixed << std::setprecision(10) << ppp.x[2] << std::endl
                   << "normalX=" << std::fixed << std::setprecision(10) << ppp.normal[0] <<", normalY=" << std::fixed << std::setprecision(10) << ppp.normal[1] << ", normalZ=" << std::fixed << std::setprecision(10) << ppp.normal[2] << std::endl
                   << "frictionX=" << std::fixed << std::setprecision(10) << rfriction[0] <<", frictionY=" << std::fixed << std::setprecision(10) << rfriction[1] << ", frictionZ=" << std::fixed << std::setprecision(10) << rfriction[2] << std::endl
                   << "fX=" << std::fixed << std::setprecision(10) << rforce[0] << ", fY=" << std::fixed << std::setprecision(10) << rforce[1] << ", fZ=" << std::fixed << std::setprecision(10) << rforce[2] << std::endl
                   << "tX=" << std::fixed << std::setprecision(10) << rtorque[0] << ", tY=" << std::fixed << std::setprecision(10) << rtorque[1] << ", tZ=" << std::fixed << std::setprecision(10) << rtorque[2] << std::endl
                   << "pX=" << std::fixed << std::setprecision(10) << ppp.P[0] <<", pY=" << std::fixed << std::setprecision(10) << ppp.P[1] << ", pZ=" << std::fixed << std::setprecision(10) << ppp.P[2] << std::endl
                   << "qX=" << std::fixed << std::setprecision(10) << ppp.Q[0] <<", qY=" << std::fixed << std::setprecision(10) << ppp.Q[1] << ", qZ=" << std::fixed << std::setprecision(10) << ppp.Q[2] << std::endl;
        #endif
      }
    }
  }

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

  if(_mini < _iterationNumber && _iterationNumber < _maxi)
  {
    //printf("ENTERED %i %i %i\n", _mini, _maxi, _iterationNumber);

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

    for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
    {
      _particleCounter++;
      records::Particle&  particle = fineGridVertex.getParticle(i);

      particleVertexLink[1] = _vertexWriter->plotVertex( particle.getCentre() );

      if(!particle.getIsObstacle())
      {
        //it will only accept diameter not radius to plot the sphere thus multiply epsilon by 2
        tarch::la::Vector<3,double> v = {particle._persistentRecords._velocity(0), particle._persistentRecords._velocity(1), particle._persistentRecords._velocity(2)};
        _velocitiesAndNormals->plotVertex(particleVertexLink[1],v);

        _particleVelocity->plotVertex(particleVertexLink[1],std::sqrt(
            particle._persistentRecords._velocity(0)*particle._persistentRecords._velocity(0)+
            particle._persistentRecords._velocity(1)*particle._persistentRecords._velocity(1)+
            particle._persistentRecords._velocity(2)*particle._persistentRecords._velocity(2)));

        v = {particle._persistentRecords._angular(0), particle._persistentRecords._angular(1), particle._persistentRecords._angular(2)};
        _particleAngular->plotVertex(particleVertexLink[1],v);

        _particleDiameter->plotVertex(particleVertexLink[1], particle.getDiameter());
        _particleEpsilon->plotVertex(particleVertexLink[1], particle.getDiameter()+(particle.getEpsilon()*2));
        _particleInfluence->plotVertex(particleVertexLink[1], particle.getHaloDiameter());
      } else {
        _velocitiesAndNormals->plotVertex(particleVertexLink[1],0);
        _particleVelocity->plotVertex(particleVertexLink[1],0);
        _particleAngular->plotVertex(particleVertexLink[1],0);
        _particleDiameter->plotVertex(particleVertexLink[1], 0);
        _particleEpsilon->plotVertex(particleVertexLink[1], 0);
        _particleInfluence->plotVertex(particleVertexLink[1], 0);
      }

      _frictionNormals->plotVertex(particleVertexLink[1],0);
      _vertexColoring->plotVertex(particleVertexLink[1], particle.getGlobalParticleId());

      int lineFromParticleToHostVertex = _cellWriter->plotLine(particleVertexLink);
      _type->plotCell(lineFromParticleToHostVertex,1); //link
      _level->plotCell(lineFromParticleToHostVertex,coarseGridVerticesEnumerator.getLevel()+1);
      _faceVertexAssociation->plotCell(lineFromParticleToHostVertex,_vertexCounter);

      int particleCentre = _cellWriter->plotPoint(particleVertexLink[1]);
      _type->plotCell(particleCentre,0); //particle center
      _level->plotCell(particleCentre,coarseGridVerticesEnumerator.getLevel()+1);
      _faceVertexAssociation->plotCell(particleCentre,_vertexCounter);

      double* x = fineGridVertex.getXCoordinates(i);
      double* y = fineGridVertex.getYCoordinates(i);
      double* z = fineGridVertex.getZCoordinates(i);

      std::vector<double> xCoordinatesWider, yCoordinatesWider, zCoordinatesWider;

      for(int k=0; k<particle.getNumberOfTriangles()*3; k++)
      {
        xCoordinatesWider.push_back(x[k]);
        yCoordinatesWider.push_back(y[k]);
        zCoordinatesWider.push_back(z[k]);
      }

      double center[3] = {particle.getCentre(0), particle.getCentre(1), particle.getCentre(2)};

      //double resizePercentage = (particle.getDiameter()+particle.getEpsilon())/ particle.getDiameter();
      //delta::geometry::properties::scaleXYZ(resizePercentage, center, xCoordinatesWider, yCoordinatesWider, zCoordinatesWider);

      delta::geometry::properties::explode(xCoordinatesWider, yCoordinatesWider, zCoordinatesWider, 5);

      for (int j=0; j<particle.getNumberOfTriangles(); j++)
      {
        int vertexIndex[3];
        tarch::la::Vector<3,double> p;

        p = {xCoordinatesWider[j*3+0], yCoordinatesWider[j*3+0], zCoordinatesWider[j*3+0]};
        vertexIndex[0] = _vertexWriter->plotVertex(p);

        p = {xCoordinatesWider[j*3+1], yCoordinatesWider[j*3+1], zCoordinatesWider[j*3+1]};
        vertexIndex[1] = _vertexWriter->plotVertex(p);

        p = {xCoordinatesWider[j*3+2], yCoordinatesWider[j*3+2], zCoordinatesWider[j*3+2]};
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

        _type->plotCell(faceIndex,3); //triangle
        _level->plotCell(faceIndex,coarseGridVerticesEnumerator.getLevel()+1);
        _faceVertexAssociation->plotCell(faceIndex,_vertexCounter);

        tarch::la::Vector<3,double> A;
        tarch::la::Vector<3,double> B;
        tarch::la::Vector<3,double> C;
        tarch::la::Vector<3,double> np;

        A = {xCoordinatesWider[j*3+0], yCoordinatesWider[j*3+0], zCoordinatesWider[j*3+0]};
        B = {xCoordinatesWider[j*3+1], yCoordinatesWider[j*3+1], zCoordinatesWider[j*3+1]};
        C = {xCoordinatesWider[j*3+2], yCoordinatesWider[j*3+2], zCoordinatesWider[j*3+2]};

        //A + (B − A) x a+(C−A)·b
        np[0] = A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0;
        np[1] = A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0;
        np[2] = A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0;

        int vertexPIndex = _vertexWriter->plotVertex(np);
        int dataPointIndex = _cellWriter->plotPoint(vertexPIndex);
        _type->plotCell(dataPointIndex,6); //mesh direction
        _level->plotCell(dataPointIndex,coarseGridVerticesEnumerator.getLevel()+1);
        _faceVertexAssociation->plotCell(dataPointIndex,_vertexCounter);

        iREAL V[3], W[3], N[3];
        V[0] = B[0] - A[0];
        V[1] = B[1] - A[1];
        V[2] = B[2] - A[2];

        W[0] = C[0] - B[0];
        W[1] = C[1] - B[1];
        W[2] = C[2] - B[2];

        N[0] = (V[1]*W[2])-(V[2]*W[1]);
        N[1] = (V[2]*W[0])-(V[0]*W[2]);
        N[2] = (V[0]*W[1])-(V[1]*W[0]);

        iREAL mag = std::sqrt((N[0]*N[0])+(N[1]*N[1])+(N[2]*N[2]));

        np = N[0]/mag, N[1]/mag, N[2]/mag;

        _velocitiesAndNormals->plotVertex(vertexPIndex, np);
        _frictionNormals->plotVertex(vertexPIndex, 0);
        _particleVelocity->plotVertex(vertexPIndex, 0);
        _particleAngular->plotVertex(vertexPIndex, 0);
        _particleDiameter->plotVertex(vertexPIndex, 0);
        _particleEpsilon->plotVertex(vertexPIndex, 0);
        _particleInfluence->plotVertex(vertexPIndex, 0);
        _vertexColoring->plotVertex(vertexPIndex, 0);
      }

      //original mesh
      for (int j=0; j<particle.getNumberOfTriangles(); j++)
      {
        int vertexIndex[3];
        tarch::la::Vector<3,double> p;

        p = {x[j*3+0], y[j*3+0], z[j*3+0]};
        vertexIndex[0] = _vertexWriter->plotVertex(p);

        p = {x[j*3+1], y[j*3+1], z[j*3+1]};
        vertexIndex[1] = _vertexWriter->plotVertex(p);

        p = {x[j*3+2], y[j*3+2], z[j*3+2]};
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

        _type->plotCell(faceIndex,2); //triangle
        _level->plotCell(faceIndex,coarseGridVerticesEnumerator.getLevel()+1);
        _faceVertexAssociation->plotCell(faceIndex,_vertexCounter);

        tarch::la::Vector<3,double> A;
        tarch::la::Vector<3,double> B;
        tarch::la::Vector<3,double> C;
        tarch::la::Vector<3,double> np;

        A = {x[j*3+0], y[j*3+0], z[j*3+0]};
        B = {x[j*3+1], y[j*3+1], z[j*3+1]};
        C = {x[j*3+2], y[j*3+2], z[j*3+2]};

        //A + (B − A) x a+(C−A)·b
        np[0] = A[0] + (B[0]-A[0]) * 1.0/3.0 + (C[0] - A[0]) * 1.0/3.0;
        np[1] = A[1] + (B[1]-A[1]) * 1.0/3.0 + (C[1] - A[1]) * 1.0/3.0;
        np[2] = A[2] + (B[2]-A[2]) * 1.0/3.0 + (C[2] - A[2]) * 1.0/3.0;

        int vertexPIndex = _vertexWriter->plotVertex(np);
        int dataPointIndex = _cellWriter->plotPoint(vertexPIndex);
        _type->plotCell(dataPointIndex,6);//triangle face direction
        _level->plotCell(dataPointIndex,coarseGridVerticesEnumerator.getLevel()+1);
        _faceVertexAssociation->plotCell(dataPointIndex,_vertexCounter);

        iREAL V[3], W[3], N[3];
        V[0] = B[0] - A[0];
        V[1] = B[1] - A[1];
        V[2] = B[2] - A[2];

        W[0] = C[0] - B[0];
        W[1] = C[1] - B[1];
        W[2] = C[2] - B[2];

        N[0] = (V[1]*W[2])-(V[2]*W[1]);
        N[1] = (V[2]*W[0])-(V[0]*W[2]);
        N[2] = (V[0]*W[1])-(V[1]*W[0]);

        iREAL mag = std::sqrt((N[0]*N[0])+(N[1]*N[1])+(N[2]*N[2]));

        np = N[0]/mag, N[1]/mag, N[2]/mag;

        _velocitiesAndNormals->plotVertex(vertexPIndex, np);
        _frictionNormals->plotVertex(vertexPIndex, 0);
        _particleVelocity->plotVertex(vertexPIndex, 0);
        _particleAngular->plotVertex(vertexPIndex, 0);
        _particleDiameter->plotVertex(vertexPIndex, 0);
        _particleEpsilon->plotVertex(vertexPIndex, 0);
        _particleInfluence->plotVertex(vertexPIndex, 0);
        _vertexColoring->plotVertex(vertexPIndex, 0);
      }

      /*if(_trackParticle && _trackID == particle.getGlobalParticleId())
      {
      }*/
    }
  }

  for(int i=0; i<fineGridVertex.getNumberOfParticles(); i++)
  {
    records::Particle&  particle = fineGridVertex.getParticle(i);
    #ifdef PARTICLESTATSFULL
    std::cout    << "#####PARTICLE-DATA#####" << std::endl
                 << "partiId=" << std::fixed << std::setprecision(10) << particle.getGlobalParticleId()  <<", mass=" << std::fixed << std::setprecision(10) << particle.getMass() << ", diameter=" << std::fixed << std::setprecision(10) << particle.getDiameter() << std::endl
                 << "influRa=" << std::fixed << std::setprecision(10) << particle.getInfluenceRadius() <<", epsilon=" << std::fixed << std::setprecision(10) << particle.getEpsilon() << ", hMin=" << std::fixed << std::setprecision(10) << particle.getHMin() << std::endl
                 << "noOfTri=" << std::fixed << std::setprecision(10) << particle.getNumberOfTriangles() <<", isObsta=" << std::fixed << std::setprecision(10) << particle._persistentRecords.getIsObstacle() << ", materia=" << std::fixed << std::setprecision(10) << particle.getMaterial() << std::endl
                 << "linearX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._velocity(0) <<", linearY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._velocity(1) << ", linearZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._velocity(2) << std::endl
                 << "angulaX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._angular(0) <<", angulaY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._angular(1) << ", angulaZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._angular(2) << std::endl
                 << "rangulX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._referentialAngular(0) <<", rangulY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._referentialAngular(1) << ", rangulZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._referentialAngular(2) << std::endl
                 << "centreX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centre(0) <<", centreY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centre(1) << ", centreZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centre(2) << std::endl
                 << "cOfMasX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centreOfMass(0) <<", cOfMasY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centreOfMass(1) << ", cOfMasZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centreOfMass(2) << std::endl
                 << "rcOfMaX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._referentialCentreOfMass(0) <<", rcOfMaY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._referentialCentreOfMass(1) << ", rcOfMaZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._referentialCentreOfMass(2) << std::endl
                 << "iner[0]=" << std::fixed << particle._persistentRecords._inertia(0) <<", iner[1]=" << particle._persistentRecords._inertia(1) << ", iner[2]=" << particle._persistentRecords._inertia(2) << std::endl
                 << "iner[3]=" << std::fixed << particle._persistentRecords._inertia(3) <<", iner[4]=" << particle._persistentRecords._inertia(4) << ", iner[5]=" << particle._persistentRecords._inertia(5) << std::endl
                 << "iner[6]=" << std::fixed << particle._persistentRecords._inertia(6) <<", iner[7]=" << particle._persistentRecords._inertia(7) << ", iner[8]=" << particle._persistentRecords._inertia(8) << std::endl
                 << "inve[0]=" << std::fixed << particle._persistentRecords._inverse(0) <<", inve[1]=" << particle._persistentRecords._inverse(1) << ", inve[2]=" << particle._persistentRecords._inverse(2) << std::endl
                 << "inve[3]=" << std::fixed << particle._persistentRecords._inverse(3) <<", inve[4]=" << particle._persistentRecords._inverse(4) << ", inve[5]=" << particle._persistentRecords._inverse(5) << std::endl
                 << "inve[6]=" << std::fixed << particle._persistentRecords._inverse(6) <<", inve[7]=" << particle._persistentRecords._inverse(7) << ", inve[8]=" << particle._persistentRecords._inverse(8) << std::endl
                 << "orie[0]=" << std::fixed << particle._persistentRecords._orientation(0) <<", orie[1]=" << particle._persistentRecords._orientation(1) << ", orie[2]=" << particle._persistentRecords._orientation(2) << std::endl
                 << "orie[3]=" << std::fixed << particle._persistentRecords._orientation(3) <<", orie[4]=" << particle._persistentRecords._orientation(4) << ", orie[5]=" << particle._persistentRecords._orientation(5) << std::endl
                 << "orie[6]=" << std::fixed << particle._persistentRecords._orientation(6) <<", orie[7]=" << particle._persistentRecords._orientation(7) << ", orie[8]=" << particle._persistentRecords._orientation(8) << std::endl;

    iREAL linE = 0.5 * particle.getMass()*(particle._persistentRecords._velocity(0)*particle._persistentRecords._velocity(0))+(particle._persistentRecords._velocity(1)*particle._persistentRecords._velocity(1))+(particle._persistentRecords._velocity(2)*particle._persistentRecords._velocity(2));
    iREAL rotE = 0.5 * particle._persistentRecords._inertia(0)*(particle._persistentRecords._angular(0)*particle._persistentRecords._angular(0))+0.5*particle._persistentRecords._inertia(4)*(particle._persistentRecords._angular(1)*particle._persistentRecords._angular(1))+0.5*particle._persistentRecords._inertia(8)*(particle._persistentRecords._angular(2)*particle._persistentRecords._angular(2));
    std::cout    << "#####ENERGY-DATA#####" << std::endl
                 << "linE=" << std::fixed << std::setprecision(10) << linE
                 << ", rotE=" << std::fixed << std::setprecision(10) << rotE
                 << ", totE=" << std::fixed << std::setprecision(10) << linE+rotE << std::endl;
    #elif defined(PARTICLESTATSMIN)
    std::cout    << "#####PARTICLE-DATA#####" << std::endl
                 << "particled="<< std::fixed << std::setprecision(10) << particle.getGlobalParticleId() << std::endl
                 << "linearX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._velocity(0) <<", linearY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._velocity(1) << ", linearZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._velocity(2) << std::endl
                 << "angulaX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._angular(0) <<", angulaY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._angular(1) << ", angulaZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._angular(2) << std::endl
                 << "cOfMasX=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centreOfMass(0) <<", cOfMasY=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centreOfMass(1) << ", cOfMasZ=" << std::fixed << std::setprecision(10) << particle._persistentRecords._centreOfMass(2) << std::endl;
    #endif
  }
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
