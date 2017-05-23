#include <delta/core/sys.h>
#include "dem/runners/Runner.h"

#include "dem/repositories/Repository.h"
#include "dem/repositories/RepositoryFactory.h"

#include "peano/utils/UserInterface.h"
#include "peano/datatraversal/autotuning/Oracle.h"
#include "peano/datatraversal/autotuning/OracleForOnePhaseDummy.h"
#include "sharedmemoryoracles/OracleForOnePhaseWithShrinkingGrainSize.h"

#include "tarch/Assertions.h"

#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"
#include "tarch/multicore/Core.h"
#include "tarch/logging/Log.h"

#include "peano/geometry/Hexahedron.h" 

#include "dem/mappings/MoveParticles.h"


tarch::logging::Log dem::runners::Runner::_log( "dem::runners::Runner" );

dem::runners::Runner::Runner() {
}

dem::runners::Runner::~Runner() {
}

int dem::runners::Runner::run(int numberOfTimeSteps, Plot plot, dem::mappings::CreateGrid::GridType gridType, int tbbThreads,
								              double stepSize, double realSnapshot, bool useAutotuning)
{
  peano::geometry::Hexahedron geometry(tarch::la::Vector<DIMENSIONS,double>(1.0), tarch::la::Vector<DIMENSIONS,double>(0.0));
  dem::repositories::Repository* repository = dem::repositories::RepositoryFactory::getInstance().createWithSTDStackImplementation(geometry,
                                                tarch::la::Vector<DIMENSIONS,double>(1.0),  // domainSize
                                                tarch::la::Vector<DIMENSIONS,double>(0.0)); // computationalDomainOffset
  
  #ifdef SharedMemoryParallelisation
    tarch::multicore::Core::getInstance().configure(tbbThreads);

    std::string sharedMemoryPropertiesFileName = "shared-memory-"
        + std::to_string( DIMENSIONS ) + "d-"
        + std::to_string( numberOfTimeSteps ) + "-time-steps-"
        + std::to_string( tbbThreads ) + "-threads-"
        + std::to_string( stepSize ) + "-dt-"
        + std::to_string( realSnapshot );

    switch (gridType) {
      case dem::mappings::CreateGrid::NoGrid:
        sharedMemoryPropertiesFileName += "-no-grid";
        break;
      case dem::mappings::CreateGrid::RegularGrid:
        sharedMemoryPropertiesFileName += "-regular-grid";
        break;
      case dem::mappings::CreateGrid::AdaptiveGrid:
        sharedMemoryPropertiesFileName += "-adaptive-grid";
        break;
      case dem::mappings::CreateGrid::ReluctantAdaptiveGrid:
        sharedMemoryPropertiesFileName += "-reluctant-grid";
        break;
    }

    sharedMemoryPropertiesFileName += ".properties";

    if (useAutotuning) {
      peano::datatraversal::autotuning::Oracle::getInstance().setOracle(
       new sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize(true,false));

      peano::datatraversal::autotuning::Oracle::getInstance().loadStatistics(sharedMemoryPropertiesFileName);
    } else {
      peano::datatraversal::autotuning::Oracle::getInstance().setOracle(
        new peano::datatraversal::autotuning::OracleForOnePhaseDummy(true, false));
    }
  #endif

  int result = 0;
  if (tarch::parallel::Node::getInstance().isGlobalMaster())
  {
    result = runAsMaster( *repository, numberOfTimeSteps, plot, gridType, stepSize, realSnapshot);
  }
  #ifdef Parallel
  else {
    result = runAsWorker(*repository);
  }
  #endif
  

  #ifdef SharedMemoryParallelisation
    peano::datatraversal::autotuning::Oracle::getInstance().plotStatistics(sharedMemoryPropertiesFileName);
  #endif

  delete repository;
  
  return result;
}

int dem::runners::Runner::runAsMaster(dem::repositories::Repository& repository, int iterations, Plot plot, dem::mappings::CreateGrid::GridType gridType,
										                  double initialStepSize, double realSnapshot)
{
  peano::utils::UserInterface::writeHeader();

  delta::core::Delta();

  logInfo( "runAsMaster(...)", "create grid" );
  repository.switchToCreateGrid();

  do { repository.iterate(); } while ( !repository.getState().isGridStationary() );

  logInfo( "runAsMaster(...)", "start time stepping" );

  repository.getState().clearAccumulatedData();
  repository.getState().setInitialTimeStepSize(initialStepSize);

  double elapsed = 0.0;
  double timestamp = 0.0;

  int minRange = 4000;
  int maxRange = 8000;

  /////////////////////PRE-STEP
  /*dem::mappings::Collision::CollisionModel model = dem::mappings::Collision::_collisionModel;
  dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::none;
  if (gridType==mappings::CreateGrid::AdaptiveGrid)
  {
    repository.switchToTimeStepOnDynamicGrid();
  }
  else if (gridType==mappings::CreateGrid::ReluctantAdaptiveGrid)
  {
    repository.switchToTimeStepOnReluctantDynamicGrid();
  } else {
    repository.switchToTimeStep();
  }
  repository.iterate();

  dem::mappings::Collision::_collisionModel = model;*/
  ///////////////////////////////////////////////////////////////////////////////////////////

  for (int i=0; i<iterations; i++)
  {
    timestamp = repository.getState().getTime();

    bool plotThisTraversal = (plot == EveryIteration) ||  (plot == Track) ||
                             (plot == UponChange && (repository.getState().getNumberOfContactPoints()>0 ||
                                                     !repository.getState().isGridStationary() || i%50==0 ||
                                                     repository.getState().getNumberOfParticleReassignments()>0 )) ||
                             (plot == EveryBatch && i%50 == 0) ||
                             ((plot == Adaptive && ((elapsed > realSnapshot) || (i == 0)))) ||
                             (plot == Range && ((i >= minRange) && (i < maxRange)));

    if(plotThisTraversal)
    {
      //delta::sys::Sys::saveIteration(repository.getState().getTimeStepSize(), i, iterations);

      if (gridType==mappings::CreateGrid::AdaptiveGrid)
      {
        repository.switchToTimeStepAndPlotOnDynamicGrid();
      }
      else if (gridType==mappings::CreateGrid::ReluctantAdaptiveGrid)
      {
        repository.switchToTimeStepAndPlotOnReluctantDynamicGrid();
      } else {
        repository.switchToTimeStepAndPlot();
      }
    } else {
      if (gridType==mappings::CreateGrid::AdaptiveGrid)
      {
        repository.switchToTimeStepOnDynamicGrid();
      }
      else if (gridType==mappings::CreateGrid::ReluctantAdaptiveGrid)
      {
        repository.switchToTimeStepOnReluctantDynamicGrid();
      } else {
        repository.switchToTimeStep();
      }
    }
    logInfo("runAsMaster(...)", "i=" << i
      << ", reassigns=" << repository.getState().getNumberOfParticleReassignments()
      << ", par-cmp=" << repository.getState().getNumberOfParticleComparisons()
      << ", tri-cmp=" << repository.getState().getNumberOfTriangleComparisons()
      << ", cnpt=" << repository.getState().getNumberOfContactPoints()
      << ", v=" << repository.getState().getNumberOfInnerVertices()
      << ", t=" << repository.getState().getTime()
      << ", dt=" << repository.getState().getTimeStepSize()
      << ", plot=" << plotThisTraversal);
    logInfo("runAsMaster(...)",
          "h_min(prescribed)=" << repository.getState().getPrescribedMinimumMeshWidth()
      <<", h_max(prescribed)=" << repository.getState().getPrescribedMaximumMeshWidth());
    logInfo("runAsMaster(...)",
           "h_min(real)=" << repository.getState().getMinimumMeshWidth()
      << ", h_max(real)=" << repository.getState().getMaximumMeshWidth());

    elapsed = repository.getState().getTime() - timestamp;

    repository.getState().finishedTimeStep(initialStepSize);

    repository.getState().clearAccumulatedData();
    repository.iterate();
  }

  repository.logIterationStatistics(false);
  repository.terminate();

  return 0;
}
