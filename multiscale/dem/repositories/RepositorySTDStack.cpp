#include "dem/repositories/RepositorySTDStack.h"

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

#include "peano/performanceanalysis/ScorePMacros.h"

#if !defined(CompilerICC)
#include "peano/grid/Grid.cpph"
#endif


tarch::logging::Log dem::repositories::RepositorySTDStack::_log( "dem::repositories::RepositorySTDStack" );


dem::repositories::RepositorySTDStack::RepositorySTDStack(
  peano::geometry::Geometry&                   geometry,
  const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
  const tarch::la::Vector<DIMENSIONS,double>&  computationalDomainOffset
):
  _geometry(geometry),
  _cellStack(),
  _vertexStack(),
  _solverState(),
  _gridWithCreateGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithCreateGridAndPlot(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStep(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlot(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithCollision(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithMoveParticles(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithAdopt(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithAdoptReluctantly(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithFlopAdopt(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithPlotData(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnFlopDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnFlopDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,domainSize,computationalDomainOffset,_regularGridContainer,_traversalOrderOnTopLevel),

  _repositoryState() {
  logTraceIn( "RepositorySTDStack(...)" );
  _repositoryState.setAction( dem::records::RepositoryState::Terminate );

  peano::datatraversal::autotuning::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #ifdef Parallel
  peano::parallel::loadbalancing::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #endif
  
  logTraceOut( "RepositorySTDStack(...)" );
}



dem::repositories::RepositorySTDStack::RepositorySTDStack(
  peano::geometry::Geometry&                   geometry
):
  _geometry(geometry),
  _cellStack(),
  _vertexStack(),
  _solverState(),
  _gridWithCreateGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithCreateGridAndPlot(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStep(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlot(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithCollision(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithMoveParticles(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithAdopt(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithAdoptReluctantly(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithFlopAdopt(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithPlotData(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepOnFlopDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),
  _gridWithTimeStepAndPlotOnFlopDynamicGrid(_vertexStack,_cellStack,_geometry,_solverState,_regularGridContainer,_traversalOrderOnTopLevel),

  _repositoryState() {
  logTraceIn( "RepositorySTDStack(Geometry&)" );

  _repositoryState.setAction( dem::records::RepositoryState::Terminate );
  
  peano::datatraversal::autotuning::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #ifdef Parallel
  peano::parallel::loadbalancing::Oracle::getInstance().setNumberOfOracles(dem::records::RepositoryState::NumberOfAdapters);
  #endif
  
  logTraceOut( "RepositorySTDStack(Geometry&)" );
}
    
   
dem::repositories::RepositorySTDStack::~RepositorySTDStack() {
  assertionMsg( _repositoryState.getAction() == dem::records::RepositoryState::Terminate, "terminate() must be called before destroying repository." );
}


void dem::repositories::RepositorySTDStack::restart(
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

  _gridWithCreateGrid.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithCreateGridAndPlot.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStep.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepAndPlot.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithCollision.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithMoveParticles.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithAdopt.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithAdoptReluctantly.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithFlopAdopt.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithPlotData.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepOnDynamicGrid.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepAndPlotOnDynamicGrid.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepOnReluctantDynamicGrid.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepOnFlopDynamicGrid.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);
  _gridWithTimeStepAndPlotOnFlopDynamicGrid.restart(domainSize,domainOffset,domainLevel, positionOfCentralElementWithRespectToCoarserRemoteLevel);


  _solverState.restart();

  logTraceOut( "restart(...)" );
}


void dem::repositories::RepositorySTDStack::terminate() {
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
  _gridWithCollision.terminate();
  _gridWithMoveParticles.terminate();
  _gridWithAdopt.terminate();
  _gridWithAdoptReluctantly.terminate();
  _gridWithFlopAdopt.terminate();
  _gridWithPlotData.terminate();
  _gridWithTimeStepOnDynamicGrid.terminate();
  _gridWithTimeStepAndPlotOnDynamicGrid.terminate();
  _gridWithTimeStepOnReluctantDynamicGrid.terminate();
  _gridWithTimeStepAndPlotOnReluctantDynamicGrid.terminate();
  _gridWithTimeStepOnFlopDynamicGrid.terminate();
  _gridWithTimeStepAndPlotOnFlopDynamicGrid.terminate();


  logTraceOut( "terminate()" );
}


dem::State& dem::repositories::RepositorySTDStack::getState() {
  return _solverState;
}


const dem::State& dem::repositories::RepositorySTDStack::getState() const {
  return _solverState;
}


void dem::repositories::RepositorySTDStack::iterate(int numberOfIterations, bool exchangeBoundaryVertices) {
  SCOREP_USER_REGION( (std::string("dem::repositories::RepositorySTDStack::iterate() - ") + _repositoryState.toString( _repositoryState.getAction() )).c_str(), SCOREP_USER_REGION_TYPE_FUNCTION)

  tarch::timing::Watch watch( "dem::repositories::RepositorySTDStack", "iterate(bool)", false);
  
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
  #else
  peano::datatraversal::autotuning::Oracle::getInstance().switchToOracle(_repositoryState.getAction());
  #endif
  
  for (int i=0; i<numberOfIterations; i++) {
    _solverState.setBatchState(numberOfIterations, i );

    switch ( _repositoryState.getAction()) {
      case dem::records::RepositoryState::UseAdapterCreateGrid: watch.startTimer(); _gridWithCreateGrid.iterate(); watch.stopTimer(); _measureCreateGridCPUTime.setValue( watch.getCPUTime() ); _measureCreateGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterCreateGridAndPlot: watch.startTimer(); _gridWithCreateGridAndPlot.iterate(); watch.stopTimer(); _measureCreateGridAndPlotCPUTime.setValue( watch.getCPUTime() ); _measureCreateGridAndPlotCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStep: watch.startTimer(); _gridWithTimeStep.iterate(); watch.stopTimer(); _measureTimeStepCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepAndPlot: watch.startTimer(); _gridWithTimeStepAndPlot.iterate(); watch.stopTimer(); _measureTimeStepAndPlotCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepAndPlotCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterCollision: watch.startTimer(); _gridWithCollision.iterate(); watch.stopTimer(); _measureCollisionCPUTime.setValue( watch.getCPUTime() ); _measureCollisionCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterMoveParticles: watch.startTimer(); _gridWithMoveParticles.iterate(); watch.stopTimer(); _measureMoveParticlesCPUTime.setValue( watch.getCPUTime() ); _measureMoveParticlesCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterAdopt: watch.startTimer(); _gridWithAdopt.iterate(); watch.stopTimer(); _measureAdoptCPUTime.setValue( watch.getCPUTime() ); _measureAdoptCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterAdoptReluctantly: watch.startTimer(); _gridWithAdoptReluctantly.iterate(); watch.stopTimer(); _measureAdoptReluctantlyCPUTime.setValue( watch.getCPUTime() ); _measureAdoptReluctantlyCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterFlopAdopt: watch.startTimer(); _gridWithFlopAdopt.iterate(); watch.stopTimer(); _measureFlopAdoptCPUTime.setValue( watch.getCPUTime() ); _measureFlopAdoptCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterPlotData: watch.startTimer(); _gridWithPlotData.iterate(); watch.stopTimer(); _measurePlotDataCPUTime.setValue( watch.getCPUTime() ); _measurePlotDataCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepOnDynamicGrid: watch.startTimer(); _gridWithTimeStepOnDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepOnDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepOnDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnDynamicGrid: watch.startTimer(); _gridWithTimeStepAndPlotOnDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepAndPlotOnDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepAndPlotOnDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepOnReluctantDynamicGrid: watch.startTimer(); _gridWithTimeStepOnReluctantDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepOnReluctantDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepOnReluctantDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnReluctantDynamicGrid: watch.startTimer(); _gridWithTimeStepAndPlotOnReluctantDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepOnFlopDynamicGrid: watch.startTimer(); _gridWithTimeStepOnFlopDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepOnFlopDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepOnFlopDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;
      case dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnFlopDynamicGrid: watch.startTimer(); _gridWithTimeStepAndPlotOnFlopDynamicGrid.iterate(); watch.stopTimer(); _measureTimeStepAndPlotOnFlopDynamicGridCPUTime.setValue( watch.getCPUTime() ); _measureTimeStepAndPlotOnFlopDynamicGridCalendarTime.setValue( watch.getCalendarTime() ); break;

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

 void dem::repositories::RepositorySTDStack::switchToCreateGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterCreateGrid); }
 void dem::repositories::RepositorySTDStack::switchToCreateGridAndPlot() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterCreateGridAndPlot); }
 void dem::repositories::RepositorySTDStack::switchToTimeStep() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStep); }
 void dem::repositories::RepositorySTDStack::switchToTimeStepAndPlot() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepAndPlot); }
 void dem::repositories::RepositorySTDStack::switchToCollision() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterCollision); }
 void dem::repositories::RepositorySTDStack::switchToMoveParticles() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterMoveParticles); }
 void dem::repositories::RepositorySTDStack::switchToAdopt() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterAdopt); }
 void dem::repositories::RepositorySTDStack::switchToAdoptReluctantly() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterAdoptReluctantly); }
 void dem::repositories::RepositorySTDStack::switchToFlopAdopt() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterFlopAdopt); }
 void dem::repositories::RepositorySTDStack::switchToPlotData() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterPlotData); }
 void dem::repositories::RepositorySTDStack::switchToTimeStepOnDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepOnDynamicGrid); }
 void dem::repositories::RepositorySTDStack::switchToTimeStepAndPlotOnDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnDynamicGrid); }
 void dem::repositories::RepositorySTDStack::switchToTimeStepOnReluctantDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepOnReluctantDynamicGrid); }
 void dem::repositories::RepositorySTDStack::switchToTimeStepAndPlotOnReluctantDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnReluctantDynamicGrid); }
 void dem::repositories::RepositorySTDStack::switchToTimeStepOnFlopDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepOnFlopDynamicGrid); }
 void dem::repositories::RepositorySTDStack::switchToTimeStepAndPlotOnFlopDynamicGrid() { _repositoryState.setAction(dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnFlopDynamicGrid); }



 bool dem::repositories::RepositorySTDStack::isActiveAdapterCreateGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterCreateGrid; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterCreateGridAndPlot() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterCreateGridAndPlot; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStep() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStep; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStepAndPlot() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepAndPlot; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterCollision() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterCollision; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterMoveParticles() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterMoveParticles; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterAdopt() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterAdopt; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterAdoptReluctantly() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterAdoptReluctantly; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterFlopAdopt() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterFlopAdopt; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterPlotData() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterPlotData; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStepOnDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepOnDynamicGrid; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStepAndPlotOnDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnDynamicGrid; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStepOnReluctantDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepOnReluctantDynamicGrid; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStepAndPlotOnReluctantDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnReluctantDynamicGrid; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStepOnFlopDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepOnFlopDynamicGrid; }
 bool dem::repositories::RepositorySTDStack::isActiveAdapterTimeStepAndPlotOnFlopDynamicGrid() const { return _repositoryState.getAction() == dem::records::RepositoryState::UseAdapterTimeStepAndPlotOnFlopDynamicGrid; }



peano::grid::Checkpoint<dem::Vertex, dem::Cell>* dem::repositories::RepositorySTDStack::createEmptyCheckpoint() {
  return new peano::grid::Checkpoint<dem::Vertex, dem::Cell>();
} 


void dem::repositories::RepositorySTDStack::writeCheckpoint(peano::grid::Checkpoint<dem::Vertex, dem::Cell> * const checkpoint) {
  _solverState.writeToCheckpoint( *checkpoint );
  _vertexStack.writeToCheckpoint( *checkpoint );
  _cellStack.writeToCheckpoint( *checkpoint );
} 


void dem::repositories::RepositorySTDStack::readCheckpoint( peano::grid::Checkpoint<dem::Vertex, dem::Cell> const * const checkpoint ) {
  assertionMsg( checkpoint->isValid(), "checkpoint has to be valid if you call this operation" );

  _solverState.readFromCheckpoint( *checkpoint );
  _vertexStack.readFromCheckpoint( *checkpoint );
  _cellStack.readFromCheckpoint( *checkpoint );
}


void dem::repositories::RepositorySTDStack::setMaximumMemoryFootprintForTemporaryRegularGrids(double value) {
  _regularGridContainer.setMaximumMemoryFootprintForTemporaryRegularGrids(value);
}


#ifdef Parallel
void dem::repositories::RepositorySTDStack::runGlobalStep() {
  assertion(tarch::parallel::Node::getInstance().isGlobalMaster());

  dem::records::RepositoryState intermediateStateForWorkingNodes;
  intermediateStateForWorkingNodes.setAction( dem::records::RepositoryState::RunOnAllNodes );
  
  tarch::parallel::NodePool::getInstance().broadcastToWorkingNodes(
    intermediateStateForWorkingNodes,
    peano::parallel::SendReceiveBufferPool::getInstance().getIterationManagementTag()
  );
  tarch::parallel::NodePool::getInstance().activateIdleNodes();
}


dem::repositories::RepositorySTDStack::ContinueCommand dem::repositories::RepositorySTDStack::continueToIterate() {
  logTraceIn( "continueToIterate()" );

  assertion( !tarch::parallel::Node::getInstance().isGlobalMaster());

  ContinueCommand result;
  if ( _solverState.hasJoinedWithMaster() ) {
    result = Terminate;
  }
  else {
    int masterNode = tarch::parallel::Node::getInstance().getGlobalMasterRank();
    assertion( masterNode != -1 );

    _repositoryState.receive( masterNode, peano::parallel::SendReceiveBufferPool::getInstance().getIterationManagementTag(), true, records::RepositoryState::ExchangeMode::NonblockingWithPollingLoopOverTests);

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


void dem::repositories::RepositorySTDStack::logIterationStatistics(bool logAllAdapters) const {
  logInfo( "logIterationStatistics()", "|| adapter name \t || iterations \t || total CPU time [t]=s \t || average CPU time/grid sweep [t]=s \t || total real time [t]=s \t || average real time/grid sweep [t]=s  || CPU time properties  || real time properties " );  
   if (logAllAdapters || _measureCreateGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| CreateGrid \t |  " << _measureCreateGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureCreateGridCPUTime.getAccumulatedValue() << " \t |  " << _measureCreateGridCPUTime.getValue()  << " \t |  " << _measureCreateGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureCreateGridCalendarTime.getValue() << " \t |  " << _measureCreateGridCPUTime.toString() << " \t |  " << _measureCreateGridCalendarTime.toString() );
   if (logAllAdapters || _measureCreateGridAndPlotCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| CreateGridAndPlot \t |  " << _measureCreateGridAndPlotCPUTime.getNumberOfMeasurements() << " \t |  " << _measureCreateGridAndPlotCPUTime.getAccumulatedValue() << " \t |  " << _measureCreateGridAndPlotCPUTime.getValue()  << " \t |  " << _measureCreateGridAndPlotCalendarTime.getAccumulatedValue() << " \t |  " << _measureCreateGridAndPlotCalendarTime.getValue() << " \t |  " << _measureCreateGridAndPlotCPUTime.toString() << " \t |  " << _measureCreateGridAndPlotCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStep \t |  " << _measureTimeStepCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepCPUTime.getValue()  << " \t |  " << _measureTimeStepCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepCalendarTime.getValue() << " \t |  " << _measureTimeStepCPUTime.toString() << " \t |  " << _measureTimeStepCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepAndPlotCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepAndPlot \t |  " << _measureTimeStepAndPlotCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepAndPlotCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotCPUTime.getValue()  << " \t |  " << _measureTimeStepAndPlotCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotCalendarTime.getValue() << " \t |  " << _measureTimeStepAndPlotCPUTime.toString() << " \t |  " << _measureTimeStepAndPlotCalendarTime.toString() );
   if (logAllAdapters || _measureCollisionCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| Collision \t |  " << _measureCollisionCPUTime.getNumberOfMeasurements() << " \t |  " << _measureCollisionCPUTime.getAccumulatedValue() << " \t |  " << _measureCollisionCPUTime.getValue()  << " \t |  " << _measureCollisionCalendarTime.getAccumulatedValue() << " \t |  " << _measureCollisionCalendarTime.getValue() << " \t |  " << _measureCollisionCPUTime.toString() << " \t |  " << _measureCollisionCalendarTime.toString() );
   if (logAllAdapters || _measureMoveParticlesCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| MoveParticles \t |  " << _measureMoveParticlesCPUTime.getNumberOfMeasurements() << " \t |  " << _measureMoveParticlesCPUTime.getAccumulatedValue() << " \t |  " << _measureMoveParticlesCPUTime.getValue()  << " \t |  " << _measureMoveParticlesCalendarTime.getAccumulatedValue() << " \t |  " << _measureMoveParticlesCalendarTime.getValue() << " \t |  " << _measureMoveParticlesCPUTime.toString() << " \t |  " << _measureMoveParticlesCalendarTime.toString() );
   if (logAllAdapters || _measureAdoptCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| Adopt \t |  " << _measureAdoptCPUTime.getNumberOfMeasurements() << " \t |  " << _measureAdoptCPUTime.getAccumulatedValue() << " \t |  " << _measureAdoptCPUTime.getValue()  << " \t |  " << _measureAdoptCalendarTime.getAccumulatedValue() << " \t |  " << _measureAdoptCalendarTime.getValue() << " \t |  " << _measureAdoptCPUTime.toString() << " \t |  " << _measureAdoptCalendarTime.toString() );
   if (logAllAdapters || _measureAdoptReluctantlyCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| AdoptReluctantly \t |  " << _measureAdoptReluctantlyCPUTime.getNumberOfMeasurements() << " \t |  " << _measureAdoptReluctantlyCPUTime.getAccumulatedValue() << " \t |  " << _measureAdoptReluctantlyCPUTime.getValue()  << " \t |  " << _measureAdoptReluctantlyCalendarTime.getAccumulatedValue() << " \t |  " << _measureAdoptReluctantlyCalendarTime.getValue() << " \t |  " << _measureAdoptReluctantlyCPUTime.toString() << " \t |  " << _measureAdoptReluctantlyCalendarTime.toString() );
   if (logAllAdapters || _measureFlopAdoptCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| FlopAdopt \t |  " << _measureFlopAdoptCPUTime.getNumberOfMeasurements() << " \t |  " << _measureFlopAdoptCPUTime.getAccumulatedValue() << " \t |  " << _measureFlopAdoptCPUTime.getValue()  << " \t |  " << _measureFlopAdoptCalendarTime.getAccumulatedValue() << " \t |  " << _measureFlopAdoptCalendarTime.getValue() << " \t |  " << _measureFlopAdoptCPUTime.toString() << " \t |  " << _measureFlopAdoptCalendarTime.toString() );
   if (logAllAdapters || _measurePlotDataCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| PlotData \t |  " << _measurePlotDataCPUTime.getNumberOfMeasurements() << " \t |  " << _measurePlotDataCPUTime.getAccumulatedValue() << " \t |  " << _measurePlotDataCPUTime.getValue()  << " \t |  " << _measurePlotDataCalendarTime.getAccumulatedValue() << " \t |  " << _measurePlotDataCalendarTime.getValue() << " \t |  " << _measurePlotDataCPUTime.toString() << " \t |  " << _measurePlotDataCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepOnDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepOnDynamicGrid \t |  " << _measureTimeStepOnDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepOnDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepOnDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepOnDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepOnDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepAndPlotOnDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepAndPlotOnDynamicGrid \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepAndPlotOnDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepOnReluctantDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepOnReluctantDynamicGrid \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepOnReluctantDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepOnReluctantDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepAndPlotOnReluctantDynamicGrid \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepOnFlopDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepOnFlopDynamicGrid \t |  " << _measureTimeStepOnFlopDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepOnFlopDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnFlopDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepOnFlopDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepOnFlopDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepOnFlopDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepOnFlopDynamicGridCalendarTime.toString() );
   if (logAllAdapters || _measureTimeStepAndPlotOnFlopDynamicGridCPUTime.getNumberOfMeasurements()>0) logInfo( "logIterationStatistics()", "| TimeStepAndPlotOnFlopDynamicGrid \t |  " << _measureTimeStepAndPlotOnFlopDynamicGridCPUTime.getNumberOfMeasurements() << " \t |  " << _measureTimeStepAndPlotOnFlopDynamicGridCPUTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnFlopDynamicGridCPUTime.getValue()  << " \t |  " << _measureTimeStepAndPlotOnFlopDynamicGridCalendarTime.getAccumulatedValue() << " \t |  " << _measureTimeStepAndPlotOnFlopDynamicGridCalendarTime.getValue() << " \t |  " << _measureTimeStepAndPlotOnFlopDynamicGridCPUTime.toString() << " \t |  " << _measureTimeStepAndPlotOnFlopDynamicGridCalendarTime.toString() );

}


void dem::repositories::RepositorySTDStack::clearIterationStatistics() {
   _measureCreateGridCPUTime.erase();
   _measureCreateGridAndPlotCPUTime.erase();
   _measureTimeStepCPUTime.erase();
   _measureTimeStepAndPlotCPUTime.erase();
   _measureCollisionCPUTime.erase();
   _measureMoveParticlesCPUTime.erase();
   _measureAdoptCPUTime.erase();
   _measureAdoptReluctantlyCPUTime.erase();
   _measureFlopAdoptCPUTime.erase();
   _measurePlotDataCPUTime.erase();
   _measureTimeStepOnDynamicGridCPUTime.erase();
   _measureTimeStepAndPlotOnDynamicGridCPUTime.erase();
   _measureTimeStepOnReluctantDynamicGridCPUTime.erase();
   _measureTimeStepAndPlotOnReluctantDynamicGridCPUTime.erase();
   _measureTimeStepOnFlopDynamicGridCPUTime.erase();
   _measureTimeStepAndPlotOnFlopDynamicGridCPUTime.erase();

   _measureCreateGridCalendarTime.erase();
   _measureCreateGridAndPlotCalendarTime.erase();
   _measureTimeStepCalendarTime.erase();
   _measureTimeStepAndPlotCalendarTime.erase();
   _measureCollisionCalendarTime.erase();
   _measureMoveParticlesCalendarTime.erase();
   _measureAdoptCalendarTime.erase();
   _measureAdoptReluctantlyCalendarTime.erase();
   _measureFlopAdoptCalendarTime.erase();
   _measurePlotDataCalendarTime.erase();
   _measureTimeStepOnDynamicGridCalendarTime.erase();
   _measureTimeStepAndPlotOnDynamicGridCalendarTime.erase();
   _measureTimeStepOnReluctantDynamicGridCalendarTime.erase();
   _measureTimeStepAndPlotOnReluctantDynamicGridCalendarTime.erase();
   _measureTimeStepOnFlopDynamicGridCalendarTime.erase();
   _measureTimeStepAndPlotOnFlopDynamicGridCalendarTime.erase();

}
