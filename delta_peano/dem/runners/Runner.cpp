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
#include "dem/mappings/CreateGrid.h"
#include "dem/mappings/Plot.h"
#include "dem/mappings/ReluctantlyAdoptGrid.h"
#include <stdlib.h>
#include <string>

tarch::logging::Log dem::runners::Runner::_log( "dem::runners::Runner" );

dem::runners::Runner::Runner() {
}

dem::runners::Runner::~Runner() {
}

void initHPCEnvironment() {
  peano::performanceanalysis::Analysis::getInstance().enable(false);
}

std::string initSharedMemory(
    int tbbThreads,
    dem::mappings::CreateGrid::GridType gridType,
    int numberOfTimeSteps,
    double stepSize,
    double realSnapshot,
    bool useAutotuning)
{
  #ifdef SharedMemoryParallelisation
    tarch::multicore::Core::getInstance().configure(tbbThreads, tarch::multicore::Core::UseDefaultStackSize);

    std::string sharedMemoryPropertiesFileName = "shared-memory-"+ std::to_string( tbbThreads ) + "-threads-";

    switch (gridType)
    {
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
        new peano::datatraversal::autotuning::OracleForOnePhaseDummy(
          true, //  bool useMultithreading                  = true,
          0,    //  int  grainSizeOfUserDefinedRegions      = 0,
          peano::datatraversal::autotuning::OracleForOnePhaseDummy::SplitVertexReadsOnRegularSubtree::Split, //  SplitVertexReadsOnRegularSubtree splitTheTree = SplitVertexReadsOnRegularSubtree::Split,
          false, //bool pipelineDescendProcessing          = false,
          false, //  bool pipelineAscendProcessing           = false,
          0,    //  int  smallestProblemSizeForAscendDescend  = tarch::la::aPowI(DIMENSIONS,3*3*3*3/2),
          0,    //   int  grainSizeForAscendDescend          = 3,
          2,    //  int  smallestProblemSizeForEnterLeaveCell = tarch::la::aPowI(DIMENSIONS,9/2),
          2,    //  int  grainSizeForEnterLeaveCell         = 2,
          2,    //  int  smallestProblemSizeForTouchFirstLast = tarch::la::aPowI(DIMENSIONS,3*3*3*3+1),
          2,    //  int  grainSizeForTouchFirstLast         = 64,
          2,    //  int  smallestProblemSizeForSplitLoadStore = tarch::la::aPowI(DIMENSIONS,3*3*3),
          2     //  int  grainSizeForSplitLoadStore         = 8
        ));
    }
    return sharedMemoryPropertiesFileName;
  #endif
  return "";
}

void shutSharedMemory(std::string sharedMemoryPropertiesFileName)
{
  if("" == sharedMemoryPropertiesFileName) {return;}
  #ifdef SharedMemoryParallelisation
    peano::datatraversal::autotuning::Oracle::getInstance().plotStatistics(sharedMemoryPropertiesFileName);
  #endif
}

void initDistributedMemory()
{
  #ifdef Parallel

  #endif
}

void shutDistributedMemory()
{
  #ifdef Parallel
    tarch::parallel::NodePool::getInstance().terminate();
    dem::repositories::RepositoryFactory::getInstance().shutdownAllParallelDatatypes();
  #endif
}

int dem::runners::Runner::run(int numberOfTimeSteps,
    Plot plot,
    dem::mappings::CreateGrid::GridType gridType,
    int tbbThreads,
		double stepSize,
		double realSnapshot,
		bool useAutotuning)
{
  peano::geometry::Hexahedron geometry(tarch::la::Vector<DIMENSIONS,double>(1.0), tarch::la::Vector<DIMENSIONS,double>(0.0));
  dem::repositories::Repository* repository = dem::repositories::RepositoryFactory::getInstance().createWithSTDStackImplementation(geometry,
                                                tarch::la::Vector<DIMENSIONS,double>(1.0),  // domainSize
                                                tarch::la::Vector<DIMENSIONS,double>(0.0)); // computationalDomainOffset
  //initHPCEnvironment();
  std::string sharedMemoryPropertiesFileName = initSharedMemory(tbbThreads, gridType, numberOfTimeSteps, stepSize, realSnapshot, useAutotuning);
  //initDistributedMemory();

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

  shutSharedMemory(sharedMemoryPropertiesFileName);
  shutDistributedMemory();

  delete repository;

  return result;
}

int dem::runners::Runner::runAsMaster(dem::repositories::Repository& repository,
    int iterations,
    Plot plot,
    dem::mappings::CreateGrid::GridType gridType,
    double initialStepSize,
    double realSnapshot)
{
  peano::utils::UserInterface::writeHeader();

  delta::core::Delta();

  dem::mappings::AdoptGrid::_refinementCoefficient = 1.8;
  dem::mappings::AdoptGrid::_coarsenCoefficient = 0.5;

  dem::mappings::ReluctantlyAdoptGrid::_coarsenCoefficientReluctant = 1.8;
  dem::mappings::ReluctantlyAdoptGrid::_refinementCoefficientReluctant = 0.5;

  logInfo( "runAsMaster(...)", "create grid" );

  repository.switchToCreateGrid();
  repository.iterate();

  if(gridType == dem::mappings::CreateGrid::GridType::AdaptiveGrid)
  {
	logInfo( "runAsMaster(...)", "adopt grid" );
  } else if(gridType == dem::mappings::CreateGrid::GridType::ReluctantAdaptiveGrid)
  {
	logInfo( "runAsMaster(...)", "reluctantly adopt grid" );
  }

  if(gridType == dem::mappings::CreateGrid::GridType::AdaptiveGrid ||
	  gridType == dem::mappings::CreateGrid::GridType::ReluctantAdaptiveGrid)
  {
	repository.switchToAdopt();
	for(int i=0; i<6; i++)
	{
	  repository.iterate();
	}

	logInfo( "runAsMaster(...)", "pre-conditioning init" );

	repository.getState().setInitialTimeStepSize(1E-16);
	repository.getState().setMaximumVelocityApproach(0.1);

	for(int i=0; i<8; i++)
	{
	  repository.switchToMoveParticles();
	  repository.iterate();
	  repository.switchToAdopt();
	  repository.iterate();
	  logInfo( "runAsMaster(...)", "pre-conditioning step " << i << " out of " << 8);
	}
  }

  logInfo( "runAsMaster(...)", "pre-conditioning end" );
  /////////////////////////////////////////////////////////////////////
  logInfo( "runAsMaster(...)", "start time stepping" );

  repository.getState().clearAccumulatedData();
  repository.getState().setInitialTimeStepSize(initialStepSize);

  double elapsed = 0.0;
  double timestamp = 0.0;

  int minRange = 0;
  int maxRange = 50000;

  if(plot == EveryIteration)
  {
     dem::mappings::Plot::_mini = 0;
     dem::mappings::Plot::_maxi = iterations;
  } else {
    dem::mappings::Plot::_mini = minRange;
    dem::mappings::Plot::_maxi = maxRange;
  }

  /*
  //////////////////PLOT TIME ZERO//////////////////////////////////////////////////////
  if((plot == EveryIteration) ||  (plot == Track) ||
      (plot == UponChange && (repository.getState().getNumberOfContactPoints()>0 ||
                              !repository.getState().isGridStationary() || 0%50==0 ||
                              repository.getState().getNumberOfParticleReassignments()>0 )) ||
      (plot == EveryBatch && 0%50 == 0) ||
      ((plot == Adaptive && ((elapsed > realSnapshot) || (0 == 0)))))
  {
    timestamp = repository.getState().getTime();
    repository.getState().setTimeStep(0);

    repository.switchToPlotData();
    repository.iterate();

    logInfo("runAsMaster(...)", "i=" << 0
      << ", reassigns=" << repository.getState().getNumberOfParticleReassignments()
      << ", par-cmp=" << repository.getState().getNumberOfParticleComparisons()
      << ", tri-cmp=" << repository.getState().getNumberOfTriangleComparisons()
      << ", cnpt=" << repository.getState().getNumberOfContactPoints()
      << ", v=" << repository.getState().getNumberOfInnerVertices()
      << ", t=" << repository.getState().getTime()
      << ", dt=" << repository.getState().getTimeStepSize()
      << ", mvij=" << repository.getState().getMaximumVelocityApproach()
      << ", plot=" << 1);

    logInfo("runAsMaster(...)",
           "h_min(real)=" << repository.getState().getMinimumMeshWidth()
      << ", h_max(real)=" << repository.getState().getMaximumMeshWidth());

    elapsed = repository.getState().getTime() - timestamp;
    repository.getState().finishedTimeStep(initialStepSize);
  }
  */
  ///////////////////////////////////////////////////////////////////////////////////////

  for (int i=0; i<iterations; i++)
  {
    timestamp = repository.getState().getTime();
    repository.getState().setTimeStep(i);

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
        repository.switchToTimeStepOnDynamicGridMerged();
      }
      else if (gridType==mappings::CreateGrid::ReluctantAdaptiveGrid)
      {
        repository.switchToTimeStepOnReluctantDynamicGridMerged();
      } else {
        repository.switchToTimeStep();
      }
    }

    repository.iterate();

    elapsed = repository.getState().getTime() - timestamp;

    logInfo("runAsMaster(...)", "i=" << i
      << ", reassigns=" << repository.getState().getNumberOfParticleReassignments()
      << ", par-cmp=" << repository.getState().getNumberOfParticleComparisons()
      << ", tri-cmp=" << repository.getState().getNumberOfTriangleComparisons()
      << ", cnpt=" << repository.getState().getNumberOfContactPoints()
      << ", v=" << repository.getState().getNumberOfInnerVertices()
      << ", t=" << repository.getState().getTime()
      << ", dt=" << repository.getState().getTimeStepSize()
      << ", mvij=" << repository.getState().getMaximumVelocityApproach()
      << ", plot=" << plotThisTraversal);
    logInfo("runAsMaster(...)",
           "h_min(real)=" << repository.getState().getMinimumMeshWidth()
      << ", h_max(real)=" << repository.getState().getMaximumMeshWidth());

    repository.getState().finishedTimeStep(initialStepSize);
    repository.getState().clearAccumulatedData();
  }

  repository.logIterationStatistics(false);
  repository.terminate();

  return 0;
}
