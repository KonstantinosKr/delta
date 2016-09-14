#include "dem/runners/Runner.h"

#include "dem/repositories/Repository.h"
#include "dem/repositories/RepositoryFactory.h"

#include "peano/utils/UserInterface.h"
#include "peano/datatraversal/autotuning/Oracle.h"
#include "peano/datatraversal/autotuning/OracleForOnePhaseDummy.h"

#include "tarch/Assertions.h"

#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"
#include "tarch/multicore/Core.h"

#include "peano/geometry/Hexahedron.h" 

#include "dem/mappings/MoveParticles.h"

tarch::logging::Log dem::runners::Runner::_log( "dem::runners::Runner" );

dem::runners::Runner::Runner() {
}


dem::runners::Runner::~Runner() {
}

int dem::runners::Runner::run(int numberOfTimeSteps, Plot plot, dem::mappings::CreateGrid::GridType gridType, int tbbThreads, double timeStepSize) {
  peano::geometry::Hexahedron geometry(
    tarch::la::Vector<DIMENSIONS,double>(1.0),
    tarch::la::Vector<DIMENSIONS,double>(0.0)
   );
  dem::repositories::Repository* repository = 
    dem::repositories::RepositoryFactory::getInstance().createWithSTDStackImplementation(
      geometry,
      tarch::la::Vector<DIMENSIONS,double>(1.0),   // domainSize,
      tarch::la::Vector<DIMENSIONS,double>(0.0)    // computationalDomainOffset
    );
  
  #ifdef SharedMemoryParallelisation
  tarch::multicore::Core::getInstance().configure(tbbThreads);

  peano::datatraversal::autotuning::Oracle::getInstance().setOracle(
     new peano::datatraversal::autotuning::OracleForOnePhaseDummy(
       true, false
     ));
  #endif

  int result = 0;
  if (tarch::parallel::Node::getInstance().isGlobalMaster()) {
    result = runAsMaster( *repository, numberOfTimeSteps, plot, gridType, timeStepSize );
  }
  #ifdef Parallel
  else {
    result = runAsWorker( *repository );
  }
  #endif
  
  delete repository;
  
  return result;
}

int dem::runners::Runner::runAsMaster(dem::repositories::Repository& repository, int iterations, Plot plot, dem::mappings::CreateGrid::GridType gridType, double initialTimeStepSize) {
  peano::utils::UserInterface::writeHeader();

  logInfo( "runAsMaster(...)", "create grid" );
  repository.switchToCreateGrid();

  do { repository.iterate(); } while ( !repository.getState().isGridStationary() );

  assertion( repository.getState().isGridStationary() );

  logInfo( "runAsMaster(...)", "start time stepping" );
  repository.getState().clearAccumulatedData();

  repository.getState().setInitialTimeStepSize(initialTimeStepSize);

  for (int i=0; i<iterations; i++)
  {
    bool plotThisTraversal = (plot == EveryIteration) || ( plot == UponChange && (repository.getState().getNumberOfContactPoints()>0 ||
    						  !repository.getState().isGridStationary() || i%50==0 || repository.getState().getNumberOfParticleReassignments()>0 )) ||
							  (plot == EveryBatch && i%50 == 0) || (plot == Adaptive && i%50);//fmod(repository.getState().getTime(),0.01) == 0);

    if (plotThisTraversal)
    {
      logInfo("runAsMaster(...)", "iteration i=" << i
        << ", reassignments=" << repository.getState().getNumberOfParticleReassignments()
        << ", triangle-comparisons=" << repository.getState().getNumberOfTriangleComparisons()
        << ", contact-points=" << repository.getState().getNumberOfContactPoints()
        << ", grid-vertices=" << repository.getState().getNumberOfInnerVertices()
        << ", create snapshot");

      if (gridType==mappings::CreateGrid::AdaptiveGrid)
      {
        repository.switchToTimeStepAndPlotOnDynamicGrid();
      }else if (gridType==mappings::CreateGrid::ReluctantAdaptiveGrid)
      {
        repository.switchToTimeStepAndPlotOnReluctantDynamicGrid();
      }else
      {
        repository.switchToTimeStepAndPlot();
      }

    } else {
      logInfo("runAsMaster(...)", "iteration i=" << i
        << ", reassignments=" << repository.getState().getNumberOfParticleReassignments()
        << ", triangle-comparisons=" << repository.getState().getNumberOfTriangleComparisons()
        << ", grid-vertices=" << repository.getState().getNumberOfInnerVertices()
		<< ", t=" << repository.getState().getTime()
		<< ", dt=" << repository.getState().getTimeStepSize());

      if (gridType==mappings::CreateGrid::AdaptiveGrid)
      {
        repository.switchToTimeStepOnDynamicGrid();
      }else if (gridType==mappings::CreateGrid::ReluctantAdaptiveGrid)
      {
        repository.switchToTimeStepOnReluctantDynamicGrid();
      }else
      {
        repository.switchToTimeStep();
      }
    }

    if(plot==Adaptive)  repository.getState().finishedTimeStep();
    repository.getState().clearAccumulatedData();
    repository.iterate();
  }
 
  repository.logIterationStatistics(false);
  repository.terminate();

  return 0;
}
