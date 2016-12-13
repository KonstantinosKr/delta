#include "dem/repositories/RepositoryArrayStack.h"

#include "tarch/Assertions.h"
#include "tarch/timing/Watch.h"

#include "tarch/compiler/CompilerSpecificSettings.h"

#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"

#ifdef Parallel
#include "peano/parallel/SendReceiveBufferPool.h"
#include "peano/parallel/loadbalancing/Oracle.h"
#endif

#include "peano/datatraversal/autotuning/Oracle.h"

#include "tarch/compiler/CompilerSpecificSettings.h"

#if !defined(CompilerICC)
#include "peano/grid/Grid.cpph"
#endif


tarch::logging::Log dem::repositories::RepositoryArrayStack::_log( "dem::repositories::RepositoryArrayStack" );


dem::repositories::RepositoryArrayStack::RepositoryArrayStack(
  peano::geometry::Geometry&                   geometry,
  const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
  const tarch::la::Vector<DIMENSIONS,double>&  domainOffset,
  int                                          maximumSizeOfCellInOutStack,
  int                                          maximumSizeOfVertexInOutStack,
  int                                          maximumSizeOfVertexTemporaryStack
):
  _geometry(geometry),
  _cellStack(maximumSizeOfCellInOutStack),
  _vertexStack(maximumSizeOfVertexInOutStack, maximumSizeOfVertexTemporaryStack),
  _solverState(),
  _gridWithCreateGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithCreateGridAndPlot(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStep(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlot(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,domainOffset,_regularGridContainer,_traversalOrderOnTopLevel),

  _repositoryState() {
  logTraceIn( "RepositoryArrayStack(...)" );
  
  _repositoryState.setAction( dem::records::RepositoryState::Terminate );

  peano::datatraversal::autotuning::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #ifdef Parallel
  peano::parallel::loadbalancing::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #endif
  
  logTraceOut( "RepositoryArrayStack(...)" );
}



dem::repositories::RepositoryArrayStack::RepositoryArrayStack(
  peano::geometry::Geometry&                   geometry,
  int                                          maximumSizeOfCellInOutStack,
  int                                          maximumSizeOfVertexInOutStack,
  int                                          maximumSizeOfVertexTemporaryStack
):
  _geometry(geometry),
  _cellStack(maximumSizeOfCellInOutStack),
  _vertexStack(maximumSizeOfVertexInOutStack,maximumSizeOfVertexTemporaryStack),
  _solverState(),
  _gridWithCreateGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithCreateGridAndPlot(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStep(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlot(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),

  _repositoryState() {
  logTraceIn( "RepositoryArrayStack(Geometry&)" );
  
  _repositoryState.setAction( dem::records::RepositoryState::Terminate );

  peano::datatraversal::autotuning::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #ifdef Parallel
  peano::parallel::loadbalancing::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #endif
  
  logTraceOut( "RepositoryArrayStack(Geometry&)" );
}
    
   
dem::repositories::RepositoryArrayStack::~RepositoryArrayStack() {
  assertion( _repositoryState.getAction() == dem::records::RepositoryState::Terminate );
}


void dem::repositories::RepositoryArrayStack::restart(
  const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
  const tarch::la::Vector<DIMENSIONS,double>&  domainOffset,
  int                                          domainLevel,
  const tarch::la::Vector<DIMENSIONS,int>&     positionOfCentralElementWithRespectToCoarserRemoteLevel
) {
  logTraceInWith4Arguments( "restart(...)", domainSize, domainOffset, domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel );
  #ifdef Parallel
  assertion( !tarch::parallel::Node::getInstance().isGlobalMaster());
  #endif
  
  logInfo( "restart(...)", "start node for subdomain " << domainOffset << "x" << domainSize << " on level " << domainLevel << " with master " << tarch::parallel::NodePool::getInstance().getMasterRank() );
  
  assertion( _repositoryState.getAction() == dem::records::RepositoryState::Terminate );

  _vertexStack.clear();
  _cellStack.clear();

  _gridWithCreateGrid.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithCreateGridAndPlot.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStep.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepAndPlot.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepOnDynamicGrid.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepAndPlotOnDynamicGrid.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepOnReluctantDynamicGrid.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid.restart(domainSize,domainOffset,domainLevel,positionOfCentralElementWithRespectToCoarserRemoteLevel);

 
   _solverState.restart();
 
  logTraceOut( "restart(...)" );
}


void dem::repositories::RepositoryArrayStack::terminate() {
  logTraceIn( "terminate()" );
  
  _repositoryState.setAction( dem::records::RepositoryState::Terminate );
  
  #ifdef Parallel
  if (tarch::parallel::Node::getInstance().isGlobalMaster()) {
    tarch::parallel::NodePool::getInstance().broadcastToWorkingNodes(
      _repositoryState,
      peano::parallel::SendReceiveBufferPool::getInstance().getIterationManagementTag()
    );
  }
  peano::parallel::SendReceiveBufferPool::getInstance().terminate();
  #endif
  
  _gridWithCreateGrid.terminate();
  _gridWithCreateGridAndPlot.terminate();
  _gridWithTimeStep.terminate();
  _gridWithTimeStepAndPlot.terminate();
  _gridWithTimeStepOnDynamicGrid.terminate();
  _gridWithTimeStepAndPlotOnDynamicGrid.terminate();
  _gridWithTimeStepOnReluctantDynamicGrid.terminate();
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid.terminate();

 
  logTraceOut( "terminate()" );
}


dem::State& dem::repositories::RepositoryArrayStack::getState() {
  return _solverState;
}


const dem::State& dem::repositories::RepositoryArrayStack::getState() const {
  return _solverState;
}

   
void dem::repositories::RepositoryArrayStack::iterate(int numberOfIterations, bool exchangeBoundaryVertices) {
  tarch::timing::Watch watch( "dem::repositories::RepositoryArrayStack", "iterate(bool)", false);
  
  #ifdef Parallel
  if (tarch::parallel::Node::getInstance().isGlobalMaster()) {
    _repositoryState.setNumberOfIterations(numberOfIterations);
    _repositoryState.setExchangeBoundaryVertices(exchangeBoundaryVertices);
    tarch::parallel::NodePool::getInstance().broadcastToWorkingNodes(
      _repositoryState,
      peano::parallel::SendReceiveBufferPool::getInstance().getIterationManagementTag()
    );
  }
  else {
    assertionEquals( numberOfIterations, 1 );
    numberOfIterations = _repositoryState.getNumberOfIterations();
  }

  peano::parallel::SendReceiveBufferPool::getInstance().exchangeBoundaryVertices(_repositoryState.getExchangeBoundaryVertices());

  if ( numberOfIterations > 1 && _solverState.isInvolvedInJoinOrFork() ) {
    logWarning( "iterate()", "iterate invoked for multiple traversals though load balancing still does redistribute data" );
  }
  bool switchedLoadBalancingTemporarilyOff = false;
  if ( numberOfIterations > 1 && peano::parallel::loadbalancing::Oracle::getInstance().isLoadBalancingActivated() ) {
    switchedLoadBalancingTemporarilyOff = true;
    peano::parallel::loadbalancing::Oracle::getInstance().activateLoadBalancing(false);
  }

  peano::datatraversal::autotuning::Oracle::getInstance().switchToOracle(_repositoryState.getAction());

  peano::parallel::loadbalancing::Oracle::getInstance().switchToOracle(_repositoryState.getAction());
  
  _solverState.currentlyRunsMultipleIterations(_repositoryState.getNumberOfIterations()>1);
  #else
  peano::datatraversal::autotuning::Oracle::getInstance().switchToOracle(_repositoryState.getAction());
  #endif
  
  for (int i=0; i<numberOfIterations; i++) {
    switch ( _repositoryState.getAction()) {
      case dem::records::RepositoryState::UseAdapterCreateGrid: watch.startTimer(); _gridWithCreateGrid.iterate(); watch.stopTimer(); _measureCreateGridCPUTime.setValue( watch.getCPUTime() ); _measureCreateGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterCreateGridAndPlot: watch.startTimer(); _gridWithCreateGridAndPlot.iterate(); watch.stopTimer(); _measureCreateGridAndPlotCPUTime.setValue( watch.getCPUTime() ); _measureCreateGridAndPlotCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStep: watch.startTimer(); _gridWithTimeStep.iterate(); watch.stopTimer(); _measureTimeStepCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepAndPlot: watch.startTimer(); _gridWithTimeStepAndPlot.iterate(); watch.stopTimer(); _measureTimeStepAndPlotCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepAndPlotCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepOnDynamicGrid: watch.startTimer(); _gridWithTimeStepOnDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepOnDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepOnDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnDynamicGrid: watch.startTimer(); _gridWithTimeStepAndPlotOnDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepAndPlotOnDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepAndPlotOnDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepOnReluctantDynamicGrid: watch.startTimer(); _gridWithTimeStepOnReluctantDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepOnReluctantDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepOnReluctantDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnReluctantDynamicGrid: watch.startTimer(); _gridWithTimeStepAndPlotOnReluctantDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;

      case dem::records::RepositoryState::Terminate:
        assertionMsg( false, "this branch/state should never be reached" ); 
        break;
      case dem::records::RepositoryState::NumberOfAdapters:
        assertionMsg( false, "this branch/state should never be reached" ); 
        break;
      case dem::records::RepositoryState::RunOnAllNodes:
        assertionMsg( false, "this branch/state should never be reached" ); 
        break;
      case dem::records::RepositoryState::ReadCheckpoint:
        assertionMsg( false, "not implemented yet" );
        break;
      case dem::records::RepositoryState::WriteCheckpoint:
        assertionMsg( false, "not implemented yet" );
        break;
    }
    #ifdef Parallel
    if ( switchedLoadBalancingTemporarilyOff && i==numberOfIterations-1) {
      peano::parallel::loadbalancing::Oracle::getInstance().activateLoadBalancing(true);
    }
    #endif
  }
    
  #ifdef Parallel
  if (_solverState.isJoiningWithMaster()) {
    _repositoryState.setAction( dem::records::RepositoryState::Terminate );
  }
  #endif
}

 void dem::repositories::RepositoryArrayStack::switchToCreateGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterCreateGrid); }
 void dem::repositories::RepositoryArrayStack::switchToCreateGridAndPlot() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterCreateGridAndPlot); }
 void dem::repositories::RepositoryArrayStack::switchToTimeStep() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStep); }
 void dem::repositories::RepositoryArrayStack::switchToTimeStepAndPlot() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepAndPlot); }
 void dem::repositories::RepositoryArrayStack::switchToTimeStepOnDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepOnDynamicGrid); }
 void dem::repositories::RepositoryArrayStack::switchToTimeStepAndPlotOnDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnDynamicGrid); }
 void dem::repositories::RepositoryArrayStack::switchToTimeStepOnReluctantDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepOnReluctantDynamicGrid); }
 void dem::repositories::RepositoryArrayStack::switchToTimeStepAndPlotOnReluctantDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnReluctantDynamicGrid); }



 bool dem::repositories::RepositoryArrayStack::isActiveAdapterCreateGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterCreateGrid; }
 bool dem::repositories::RepositoryArrayStack::isActiveAdapterCreateGridAndPlot() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterCreateGridAndPlot; }
 bool dem::repositories::RepositoryArrayStack::isActiveAdapterTimeStep() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStep; }
 bool dem::repositories::RepositoryArrayStack::isActiveAdapterTimeStepAndPlot() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepAndPlot; }
 bool dem::repositories::RepositoryArrayStack::isActiveAdapterTimeStepOnDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepOnDynamicGrid; }
 bool dem::repositories::RepositoryArrayStack::isActiveAdapterTimeStepAndPlotOnDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnDynamicGrid; }
 bool dem::repositories::RepositoryArrayStack::isActiveAdapterTimeStepOnReluctantDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepOnReluctantDynamicGrid; }
 bool dem::repositories::RepositoryArrayStack::isActiveAdapterTimeStepAndPlotOnReluctantDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnReluctantDynamicGrid; }



peano::grid::Checkpoint<dem::Vertex, dem::Cell>* dem::repositories::RepositoryArrayStack::createEmptyCheckpoint() {
  return new peano::grid::Checkpoint<dem::Vertex, dem::Cell>();
} 


void dem::repositories::RepositoryArrayStack::writeCheckpoint(peano::grid::Checkpoint<dem::Vertex, dem::Cell> * const checkpoint) {
  _solverState.writeToCheckpoint( *checkpoint );
  _vertexStack.writeToCheckpoint( *checkpoint );
  _cellStack.writeToCheckpoint( *checkpoint );
} 


void dem::repositories::RepositoryArrayStack::setMaximumMemoryFootprintForTemporaryRegularGrids(double value) {
  _regularGridContainer.setMaximumMemoryFootprintForTemporaryRegularGrids(value);
}


void dem::repositories::RepositoryArrayStack::readCheckpoint( peano::grid::Checkpoint<dem::Vertex, dem::Cell> const * const checkpoint ) {
  assertionMsg( checkpoint->isValid(), "checkpoint has to be valid if you call this operation" );

  _solverState.readFromCheckpoint( *checkpoint );
  _vertexStack.readFromCheckpoint( *checkpoint );
  _cellStack.readFromCheckpoint( *checkpoint );
}


#ifdef Parallel
void dem::repositories::RepositoryArrayStack::runGlobalStep() {
  assertion(tarch::parallel::Node::getInstance().isGlobalMaster());

  dem::records::RepositoryState intermediateStateForWorkingNodes;
  intermediateStateForWorkingNodes.setAction( dem::records::RepositoryState::RunOnAllNodes );
  
  tarch::parallel::NodePool::getInstance().broadcastToWorkingNodes(
    intermediateStateForWorkingNodes,
    peano::parallel::SendReceiveBufferPool::getInstance().getIterationManagementTag()
  );
  tarch::parallel::NodePool::getInstance().activateIdleNodes();
}


dem::repositories::RepositoryArrayStack::ContinueCommand dem::repositories::RepositoryArrayStack::continueToIterate() {
  logTraceIn( "continueToIterate()" );

  assertion( !tarch::parallel::Node::getInstance().isGlobalMaster());

  ContinueCommand result;
  if ( _solverState.hasJoinedWithMaster() ) {
    result = Terminate;
  }
  else {
    int masterNode = tarch::parallel::Node::getInstance().getGlobalMasterRank();
    assertion( masterNode != -1 );

    _repositoryState.receive( masterNode, peano::parallel::SendReceiveBufferPool::getInstance().getIterationManagementTag(), true, ReceiveIterationControlMessagesBlocking );

    result = Continue;
    if (_repositoryState.getAction()==dem::records::RepositoryState::Terminate) {
      result = Terminate;
    } 
    if (_repositoryState.getAction()==dem::records::RepositoryState::RunOnAllNodes) {
      result = RunGlobalStep;
    } 
  }
   
  logTraceOutWith1Argument( "continueToIterate()", result );
  return result;
}
#endif


void dem::repositories::RepositoryArrayStack::logIterationStatistics(bool logAllAdapters) const {
  logInfo( "logIterationStatistics()", "|| adapter name \t || iterations \t || total CPU time [t]=s \t || average CPU time [t]=s \t || total user time [t]=s \t || average user time [t]=s  || CPU time properties  || user time properties " );  
   if (logAllAdapters || _measureCreateGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| CreateGrid \t |  " << _measureCreateGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureCreateGridCPUTime.getAccumulatedValue() << " \t |  " << _measureCreateGridCPUTime.getValue()  << " \t |  " << _measureCreateGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureCreateGridCalendarTime.getValue() << " \t |  " << _measureCreateGridCPUTime.toString() << " \t |  " << _measureCreateGridCalendarTime.toString() );
   if (logAllAdapters || _measureCreateGridAndPlotCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| CreateGridAndPlot \t |  " << _measureCreateGridAndPlotCPUTime.getNumberOfMeasurements() << " \t |  " << _measureCreateGridAndPlotCPUTime.getAccumulatedValue() << " \t |  " << _measureCreateGridAndPlotCPUTime.getValue()  << " \t |  " << _measureCreateGridAndPlotCalendarTime.getAccumulatedValue() << " \t |  " << _measureCreateGridAndPlotCalendarTime.getValue() << " \t |  " << _measureCreateGridAndPlotCPUTime.toString() << " \t |  " << _measureCreateGridAndPlotCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStep \t |  " << _measureTimeStepCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepCPUTime.getValue()  << " \t |  " << _measureTimeStepCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepCalendarTime.getValue() << " \t |  " << _measureTimeStepCPUTime.toString() << " \t |  " << _measureTimeStepCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepAndPlotCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepAndPlot \t |  " << _measureTimeStepAndPlotCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepAndPlotCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotCPUTime.getValue()  << " \t |  " << _measureTimeStepAndPlotCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotCalendarTime.getValue() << " \t |  " << _measureTimeStepAndPlotCPUTime.toString() << " \t |  " << _measureTimeStepAndPlotCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepOnDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepOnDynamicGrid \t |  " << _measureTimeStepOnDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepOnDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepOnDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepOnDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepOnDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepAndPlotOnDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepAndPlotOnDynamicGrid \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepOnReluctantDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepOnReluctantDynamicGrid \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepOnReluctantDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepAndPlotOnReluctantDynamicGrid \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.toString() );

}


void dem::repositories::RepositoryArrayStack::clearIterationStatistics() {
   _measureCreateGridCPUTime.erase();
   _measureCreateGridAndPlotCPUTime.erase();
   _measureTimeStepCPUTime.erase();
   _measureTimeStepAndPlotCPUTime.erase();
   _measureTimeStepOnDynamicGridCPUTime.erase();
   _measureTimeStepAndPlotOnDynamicGridCPUTime.erase();
   _measureTimeStepOnReluctantDynamicGridCPUTime.erase();
   _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.erase();

   _measureCreateGridCalendarTime.erase();
   _measureCreateGridAndPlotCalendarTime.erase();
   _measureTimeStepCalendarTime.erase();
   _measureTimeStepAndPlotCalendarTime.erase();
   _measureTimeStepOnDynamicGridCalendarTime.erase();
   _measureTimeStepAndPlotOnDynamicGridCalendarTime.erase();
   _measureTimeStepOnReluctantDynamicGridCalendarTime.erase();
   _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.erase();

}
