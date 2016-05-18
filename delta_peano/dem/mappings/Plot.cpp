#include "dem/mappings/Plot.h"
#include "dem/mappings/Collision.h"
#include "delta/collision/bf.h"


/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::Plot::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,false);
}


/**
 * This is the only routine that we actually use.
 */
peano::MappingSpecification   dem::mappings::Plot::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::Serial);
}


peano::MappingSpecification   dem::mappings::Plot::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
peano::MappingSpecification   dem::mappings::Plot::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}
peano::MappingSpecification   dem::mappings::Plot::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}
peano::MappingSpecification   dem::mappings::Plot::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
}
peano::MappingSpecification   dem::mappings::Plot::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
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

  _faceVertexAssociation = _writer->createCellDataWriter( "face-vertex-association", 1 );
  _type                  = _writer->createCellDataWriter( "type(particle-centre=0,triangle=1,collision-point=2,link=3)", 1 );
  _velocitiesAndNormals  = _writer->createVertexDataWriter( "velocities-and-contact-normals", DIMENSIONS );
  _level                 = _writer->createCellDataWriter( "level", 1 );

  _vertexCounter         = 0;
  _particleCounter       = 0;
  _collisionPointCounter = 0;

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}


void dem::mappings::Plot::endIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "endIteration(State)", solverState );

  assertion( Collision::_collisionsOfNextTraversal.empty() );
  assertion( solverState.getNumberOfContactPoints()==0 || !Collision::_activeCollisions.empty() );

  for (
      std::map<int, std::vector<Collision::Collisions> >::const_iterator p = Collision::_activeCollisions.begin();
      p != Collision::_activeCollisions.end();
      p++
  ) {
    for (
      std::vector<Collision::Collisions>::const_iterator pp = p->second.begin();
      pp != p->second.end();
      pp++
    ) {
      for (
        std::vector<delta::collision::contactpoint>::const_iterator ppp = pp->_contactPoints.begin();
        ppp != pp->_contactPoints.end();
        ppp++
      ) {
        tarch::la::Vector<3,double> v;
        v = ppp->x[0], ppp->x[1], ppp->x[2];
        int contactPointVertexIndex = _vertexWriter->plotVertex( v );
        int contactPointIndex       = _cellWriter->plotPoint(contactPointVertexIndex);

        v = ppp->normal[0], ppp->normal[1], ppp->normal[2];
        _velocitiesAndNormals->plotVertex(contactPointVertexIndex,v);

        _faceVertexAssociation->plotCell(contactPointIndex,-1);
        _type->plotCell(contactPointIndex,2);
        _level->plotCell(contactPointIndex,-1);

        _collisionPointCounter++;
      }
    }
  }

  _vertexWriter->close();
  _cellWriter->close();
  _faceVertexAssociation->close();
  _type->close();
  _velocitiesAndNormals->close();
  _level->close();

  std::ostringstream snapshotFileName;
  snapshotFileName << "geometry"
                   #ifdef Parallel
                   << "-rank-" << tarch::parallel::Node::getInstance().getRank()
                   #endif
                   << "-" << _snapshotCounter
                   << ".vtk";
  _writer->writeToFile( snapshotFileName.str() );

  _snapshotCounter++;

  delete _vertexWriter;
  delete _cellWriter;
  delete _writer;
  delete _faceVertexAssociation;
  delete _type;
  delete _velocitiesAndNormals;
  delete _level;

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
  _velocitiesAndNormals->plotVertex(particleVertexLink[0],0);

  logDebug( "touchVertexLastTime(...)", "vertex holds " << fineGridVertex.getNumberOfParticles() << " particles" );
  for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++) {
    _particleCounter++;
    records::Particle&  particle = fineGridVertex.getParticle(i);

    particleVertexLink[1]            = _vertexWriter->plotVertex( particle._persistentRecords._centre );
    _velocitiesAndNormals->plotVertex(particleVertexLink[1],0);

    int lineFromParticleToHostVertex = _cellWriter->plotLine(particleVertexLink);
    _faceVertexAssociation->plotCell(lineFromParticleToHostVertex,_vertexCounter);
    _type->plotCell(lineFromParticleToHostVertex,3);
    _level->plotCell(lineFromParticleToHostVertex,coarseGridVerticesEnumerator.getLevel()+1);

    int particleCentre = _cellWriter->plotPoint(particleVertexLink[1]);
    _type->plotCell(particleCentre,0);
    _level->plotCell(particleCentre,coarseGridVerticesEnumerator.getLevel()+1);

    for (int j=0; j<particle._persistentRecords._numberOfTriangles; j++) {
      #ifdef Dim2
      const double* x = fineGridVertex.getXCoordinates( i );
      const double* y = fineGridVertex.getYCoordinates( i );
      int                         vertexIndex[2];
      tarch::la::Vector<2,double> p;
      p = x[j*2+0], y[j*2+0];
      vertexIndex[0] = _vertexWriter->plotVertex( p );
      p = x[j*2+1], y[j*2+1];
      vertexIndex[1] = _vertexWriter->plotVertex( p );
      int faceIndex = _cellWriter->plotLine(vertexIndex);

      _velocitiesAndNormals->plotVertex(vertexIndex[0],particle._persistentRecords._velocity);
      _velocitiesAndNormals->plotVertex(vertexIndex[1],particle._persistentRecords._velocity);
      #else
      const double* x = fineGridVertex.getXCoordinates( i );
      const double* y = fineGridVertex.getYCoordinates( i );
      const double* z = fineGridVertex.getZCoordinates( i );
      int                         vertexIndex[3];
      tarch::la::Vector<3,double> p;
      p = x[j*3+0], y[j*3+0], z[j*3+0];
      assertionEquals( p(0), p(0) );
      assertionEquals( p(1), p(1) );
      assertionEquals( p(2), p(2) );
      vertexIndex[0] = _vertexWriter->plotVertex( p );
      p = x[j*3+1], y[j*3+1], z[j*3+1];
      assertionEquals( p(0), p(0) );
      assertionEquals( p(1), p(1) );
      assertionEquals( p(2), p(2) );
      vertexIndex[1] = _vertexWriter->plotVertex( p );
      p = x[j*3+2], y[j*3+2], z[j*3+2];
      assertionEquals( p(0), p(0) );
      assertionEquals( p(1), p(1) );
      assertionEquals( p(2), p(2) );
      vertexIndex[2] = _vertexWriter->plotVertex( p );
      int faceIndex = _cellWriter->plotTriangle(vertexIndex);

      _velocitiesAndNormals->plotVertex(vertexIndex[0],particle._persistentRecords._velocity);
      _velocitiesAndNormals->plotVertex(vertexIndex[1],particle._persistentRecords._velocity);
      _velocitiesAndNormals->plotVertex(vertexIndex[2],particle._persistentRecords._velocity);
      #endif

      _faceVertexAssociation->plotCell(faceIndex,_vertexCounter);
      _type->plotCell(faceIndex,1);
      _level->plotCell(faceIndex,coarseGridVerticesEnumerator.getLevel()+1);
    }
  }


  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}



//
//   NOP
// =======
//
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


#if defined(SharedMemoryParallelisation)
dem::mappings::Plot::Plot(const Plot&  masterThread) {
  logTraceIn( "Plot(Plot)" );
  // @todo Insert your code here
  logTraceOut( "Plot(Plot)" );
}


void dem::mappings::Plot::mergeWithWorkerThread(const Plot& workerThread) {
  logTraceIn( "mergeWithWorkerThread(Plot)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(Plot)" );
}
#endif


void dem::mappings::Plot::createHangingVertex(
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


void dem::mappings::Plot::destroyHangingVertex(
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


void dem::mappings::Plot::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}


void dem::mappings::Plot::createBoundaryVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
  // @todo Insert your code here
  logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}


void dem::mappings::Plot::destroyVertex(
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


void dem::mappings::Plot::createCell(
      dem::Cell&                 fineGridCell,
      dem::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
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
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
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
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker
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
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
  int                                                                  worker,
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


void dem::mappings::Plot::enterCell(
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


void dem::mappings::Plot::leaveCell(
      dem::Cell&           fineGridCell,
      dem::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfCell
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