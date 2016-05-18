#include "dem/mappings/MoveParticles.h"

#include "peano/utils/Loop.h"

#include "tarch/la/Matrix.h"



/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::MoveParticles::communicationSpecification() {
  return peano::CommunicationSpecification(peano::CommunicationSpecification::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::SendDataAndStateAfterLastTouchVertexLastTime,false);
}


/**
 * Reflect
 */
peano::MappingSpecification   dem::mappings::MoveParticles::touchVertexLastTimeSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
/**
 * Move
 */
peano::MappingSpecification   dem::mappings::MoveParticles::touchVertexFirstTimeSpecification() { 
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::RunConcurrentlyOnFineGrid);
}
/**
 * Reassign
 */
peano::MappingSpecification   dem::mappings::MoveParticles::enterCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::WholeTree,peano::MappingSpecification::AvoidFineGridRaces);
}


peano::MappingSpecification   dem::mappings::MoveParticles::leaveCellSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces);
}
peano::MappingSpecification   dem::mappings::MoveParticles::ascendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
}
peano::MappingSpecification   dem::mappings::MoveParticles::descendSpecification() {
  return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces);
}


tarch::logging::Log                dem::mappings::MoveParticles::_log( "dem::mappings::MoveParticles" ); 


double    dem::mappings::MoveParticles::timeStepSize = 0.0;
double    dem::mappings::MoveParticles::gravity      = 0.0;


void dem::mappings::MoveParticles::moveAllParticlesAssociatedToVertex(
  dem::Vertex&               fineGridVertex
) {
  for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++) {
    // Each particle is assigned to exactly one vertex.
    records::Particle&  particle = fineGridVertex.getParticle(i);

    particle._persistentRecords._velocity(1) -= timeStepSize * gravity;

    particle._persistentRecords._centre(0)   += timeStepSize * particle._persistentRecords._velocity(0);
    particle._persistentRecords._centre(1)   += timeStepSize * particle._persistentRecords._velocity(1);

    particle._persistentRecords._centreOfMass(0)   += timeStepSize * particle._persistentRecords._velocity(0);
    particle._persistentRecords._centreOfMass(1)   += timeStepSize * particle._persistentRecords._velocity(1);

    double* x = fineGridVertex.getXCoordinates( i );
    double* y = fineGridVertex.getYCoordinates( i );
    #ifdef Dim3
    particle._persistentRecords._centre(2)       += timeStepSize * particle._persistentRecords._velocity(2);
    particle._persistentRecords._centreOfMass(2) += timeStepSize * particle._persistentRecords._velocity(2);

    double* z = fineGridVertex.getZCoordinates( i );
    #endif

    for (int j=0; j<particle._persistentRecords._numberOfTriangles*DIMENSIONS; j++) {
      #ifdef Dim2
      x[j] += timeStepSize * particle._persistentRecords._velocity(0);
      y[j] += timeStepSize * particle._persistentRecords._velocity(1);
      #else
      x[j] += timeStepSize * particle._persistentRecords._velocity(0);
      y[j] += timeStepSize * particle._persistentRecords._velocity(1);
      z[j] += timeStepSize * particle._persistentRecords._velocity(2);
      #endif
    }


    // rotation
    const double alpha = 0.01 * timeStepSize * particle._persistentRecords._angularVelocity(0);
    const double beta  = 0.01 * timeStepSize * particle._persistentRecords._angularVelocity(1);
    const double gamma = 0.01 * timeStepSize * particle._persistentRecords._angularVelocity(2);


    tarch::la::Matrix<3,3,double> rotationMatrix;
    rotationMatrix =
      cos(beta) * cos(gamma), cos(gamma) * sin(alpha) * sin(beta) - cos(alpha) * sin(gamma), cos(alpha)*cos(gamma)*sin(beta) + sin(alpha)*sin(gamma),
      cos(beta)*sin(gamma), cos(alpha)*cos(gamma)+sin(alpha)*sin(beta)*sin(gamma),-cos(gamma)*sin(alpha)+cos(alpha)*sin(beta)*sin(gamma),
      -sin(beta), cos(beta)*sin(alpha),cos(alpha)*cos(beta);


    rotationMatrix = 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1;


    for (int j=0; j<particle._persistentRecords._numberOfTriangles*DIMENSIONS; j++) {
      #ifdef Dim2
        @todo implement one day if we really need it
      x[j] += timeStepSize * particle._persistentRecords._velocity(0);
      y[j] += timeStepSize * particle._persistentRecords._velocity(1);
      #else
      tarch::la::Vector<3,double> relativePosition;
      relativePosition = x[j]-particle._persistentRecords._centreOfMass(0),
                         y[j]-particle._persistentRecords._centreOfMass(1),
                         z[j]-particle._persistentRecords._centreOfMass(2);

      assertionNumericalEquals5( x[j], particle._persistentRecords._centreOfMass(0)  + (relativePosition)(0), particle._persistentRecords._centreOfMass, relativePosition, x, y, z);
      assertionNumericalEquals5( y[j], particle._persistentRecords._centreOfMass(1)  + (relativePosition)(1), particle._persistentRecords._centreOfMass, relativePosition, x, y, z);
      assertionNumericalEquals5( z[j], particle._persistentRecords._centreOfMass(2)  + (relativePosition)(2), particle._persistentRecords._centreOfMass, relativePosition, x, y, z);
/*

      x[j] = particle._persistentRecords._centreOfMass(0)  + (rotationMatrix * relativePosition)(0);
      y[j] = particle._persistentRecords._centreOfMass(1)  + (rotationMatrix * relativePosition)(1);
      z[j] = particle._persistentRecords._centreOfMass(2)  + (rotationMatrix * relativePosition)(2);
*/
      #endif
    }
  }
}


void dem::mappings::MoveParticles::reassignParticles(
  dem::Vertex * const                        fineGridVertices,
  const peano::grid::VertexEnumerator&       fineGridVerticesEnumerator
) {
  int numberOfReassignments = 0;
  dfor2(k)
    int i=0;
    while (i<fineGridVertices[ fineGridVerticesEnumerator(k) ].getNumberOfParticles()) {
      records::Particle&  particle = fineGridVertices[ fineGridVerticesEnumerator(k) ].getParticle(i);

      tarch::la::Vector<DIMENSIONS,int> correctVertex;
      for (int d=0; d<DIMENSIONS; d++) {
        correctVertex(d) = particle._persistentRecords._centre(d) < fineGridVerticesEnumerator.getCellCenter()(d) ? 0 : 1;
      }

      if (correctVertex!=k) {
        fineGridVertices[ fineGridVerticesEnumerator(correctVertex) ].appendParticle(particle);
        logDebug( "reassignParticles(...)", "reassign particle " << particle.toString() << " to " << fineGridVertices[ fineGridVerticesEnumerator(correctVertex) ].toString() );
        fineGridVertices[ fineGridVerticesEnumerator(k) ].releaseParticle(i);
        numberOfReassignments++;
      }
      else {
        i++;
      }
    }
  enddforx
  _state.incNumberOfParticleReassignments(numberOfReassignments);
}


void dem::mappings::MoveParticles::reflectParticles(
  dem::Vertex&                               fineGridVertex
) {
  assertion(fineGridVertex.isBoundary());

  for (int i=0; i<fineGridVertex.getNumberOfParticles(); i++) {
    records::Particle&  particle = fineGridVertex.getParticle(i);

    for (int d=0; d<DIMENSIONS; d++) {
      if (particle._persistentRecords._centre(d)<0.0) {
        particle._persistentRecords._velocity(d) = std::abs(particle._persistentRecords._velocity(d));
      }
      if (particle._persistentRecords._centre(d)>1.0) {
        particle._persistentRecords._velocity(d) = -std::abs(particle._persistentRecords._velocity(d));
      }
    }
  }
}


void dem::mappings::MoveParticles::touchVertexFirstTime(
  dem::Vertex&               fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
  dem::Vertex * const        coarseGridVertices,
  const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
  dem::Cell&                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  moveAllParticlesAssociatedToVertex(fineGridVertex);

  logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}



void dem::mappings::MoveParticles::enterCell(
  dem::Cell&                                 fineGridCell,
  dem::Vertex * const                        fineGridVertices,
  const peano::grid::VertexEnumerator&       fineGridVerticesEnumerator,
  dem::Vertex * const                        coarseGridVertices,
  const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
  dem::Cell&                                 coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
  logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

  reassignParticles( fineGridVertices, fineGridVerticesEnumerator );

  logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}


void dem::mappings::MoveParticles::touchVertexLastTime(
  dem::Vertex&         fineGridVertex,
  const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
  const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
  dem::Vertex * const  coarseGridVertices,
  const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
  dem::Cell&           coarseGridCell,
  const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
  logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

  if (fineGridVertex.isBoundary()) {
    reflectParticles(fineGridVertex);
  }

  logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}


void dem::mappings::MoveParticles::beginIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "beginIteration(State)", solverState );

  _state = solverState;
  _state.clearAccumulatedData();

  logTraceOutWith1Argument( "beginIteration(State)", solverState);
}


void dem::mappings::MoveParticles::endIteration(
  dem::State&  solverState
) {
  logTraceInWith1Argument( "endIteration(State)", solverState );

  solverState.merge(_state);

  logTraceOutWith1Argument( "endIteration(State)", solverState);
}



//
//   NOP
// =======
//
dem::mappings::MoveParticles::MoveParticles() {
}


dem::mappings::MoveParticles::~MoveParticles() {
}


#if defined(SharedMemoryParallelisation)
dem::mappings::MoveParticles::MoveParticles(const MoveParticles&  masterThread) {
  logTraceIn( "MoveParticles(MoveParticles)" );
  // @todo Insert your code here
  logTraceOut( "MoveParticles(MoveParticles)" );
}


void dem::mappings::MoveParticles::mergeWithWorkerThread(const MoveParticles& workerThread) {
  logTraceIn( "mergeWithWorkerThread(MoveParticles)" );
  // @todo Insert your code here
  logTraceOut( "mergeWithWorkerThread(MoveParticles)" );
}
#endif


void dem::mappings::MoveParticles::createHangingVertex(
      dem::Vertex&     fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
      dem::Vertex * const   coarseGridVertices,
      const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
      dem::Cell&       coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
}


void dem::mappings::MoveParticles::destroyHangingVertex(
      const dem::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      dem::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      dem::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
}


void dem::mappings::MoveParticles::createInnerVertex(
      dem::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      dem::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      dem::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
}


void dem::mappings::MoveParticles::createBoundaryVertex(
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


void dem::mappings::MoveParticles::destroyVertex(
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


void dem::mappings::MoveParticles::createCell(
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


void dem::mappings::MoveParticles::destroyCell(
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
void dem::mappings::MoveParticles::mergeWithNeighbour(
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

void dem::mappings::MoveParticles::prepareSendToNeighbour(
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

void dem::mappings::MoveParticles::prepareCopyToRemoteNode(
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

void dem::mappings::MoveParticles::prepareCopyToRemoteNode(
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

void dem::mappings::MoveParticles::mergeWithRemoteDataDueToForkOrJoin(
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

void dem::mappings::MoveParticles::mergeWithRemoteDataDueToForkOrJoin(
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

bool dem::mappings::MoveParticles::prepareSendToWorker(
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

void dem::mappings::MoveParticles::prepareSendToMaster(
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


void dem::mappings::MoveParticles::mergeWithMaster(
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


void dem::mappings::MoveParticles::receiveDataFromMaster(
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


void dem::mappings::MoveParticles::mergeWithWorker(
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


void dem::mappings::MoveParticles::mergeWithWorker(
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


void dem::mappings::MoveParticles::leaveCell(
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


void dem::mappings::MoveParticles::descend(
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


void dem::mappings::MoveParticles::ascend(
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