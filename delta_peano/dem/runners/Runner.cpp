#include <delta/core/sys.h>
#include "dem/runners/Runner.h"

#include "dem/repositories/Repository.h"
#include "dem/repositories/RepositoryFactory.h"

#include "peano/utils/UserInterface.h"

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
#include "dem/mappings/FlopAdoptGrid.h"

#include <stdlib.h>
#include <string>

tarch::logging::Log dem::runners::Runner::_log( "dem::runners::Runner" );

void dem::runners::Runner::precondition(dem::repositories::Repository& repository, dem::mappings::CreateGrid::GridType gridType)
{
  if(gridType == dem::mappings::CreateGrid::GridType::AdaptiveGrid)
  {
	logInfo( "runAsMaster(...)", "adopt grid" );
  } else if(gridType == dem::mappings::CreateGrid::GridType::ReluctantAdaptiveGrid)
  {
	logInfo( "runAsMaster(...)", "reluctantly adopt grid" );
  } else if(gridType == dem::mappings::CreateGrid::GridType::FlopAdaptiveGrid)
  {
	logInfo( "runAsMaster(...)", "flop adopt grid" );
  }

  if(gridType == dem::mappings::CreateGrid::GridType::AdaptiveGrid ||
	 gridType == dem::mappings::CreateGrid::GridType::ReluctantAdaptiveGrid ||
	 gridType == dem::mappings::CreateGrid::GridType::FlopAdaptiveGrid)
  {
	repository.switchToAdopt();
	for(int i=0; i<6; i++)
	{
	  repository.iterate();
	}

	logInfo( "runAsMaster(...)", "pre-conditioning init" );

	repository.getState().setInitialTimeStepSize(1E-16);
	repository.getState().setMaximumVelocityApproach(0.1);

	for(int i=1; i<=8; i++)
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
  /////////////////////////////////////////////////////////////////////
}

dem::runners::Runner::Runner() {
}

dem::runners::Runner::~Runner() {
}

bool dem::runners::Runner::switchrepo(dem::repositories::Repository& repository, int i, dem::mappings::CreateGrid::GridType gridType, Plot plot, double elapsed, double realSnapshot) {
  bool plotThisTraversal = (plot == EveryIteration) ||  (plot == Track) ||
                           (plot == UponChange && (repository.getState().getNumberOfContactPoints()>0 ||
                                                   !repository.getState().isGridStationary() || i%50==0 ||
                                                   repository.getState().getNumberOfParticleReassignments()>0 )) ||
                           (plot == EveryBatch && i%50 == 0) ||
                           ((plot == Adaptive && ((elapsed > realSnapshot) || (i == 0)))) ||
                           (plot == Range && ((i >= dem::mappings::Plot::_mini) && (i < dem::mappings::Plot::_maxi)));

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
    }
    else if (gridType==mappings::CreateGrid::FlopAdaptiveGrid)
    {
      repository.switchToTimeStepAndPlotOnFlopDynamicGrid();
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
    }
    else if(gridType==mappings::CreateGrid::FlopAdaptiveGrid)
    {
      repository.switchToTimeStepOnFlopDynamicGrid();
    }
    else {
      repository.switchToTimeStep();
    }
  }
  return plotThisTraversal;
}

void initHPCEnvironment() {
  peano::performanceanalysis::Analysis::getInstance().enable(false);
}

std::string dem::runners::Runner::initSharedMemory( bool useAutotuning, int tbbThreads, dem::mappings::CreateGrid::GridType gridType)
{
  std::string sharedMemoryPropertiesFileName = "";
  #ifdef SharedMemoryParallelisation
    tarch::multicore::Core::getInstance().configure(tbbThreads, tarch::multicore::Core::UseDefaultStackSize);

    tarch::multicore::Core::getInstance().pinThreads(true);

    if(useAutotuning)
    {
	  sharedMemoryPropertiesFileName = "shared-memory-"+ std::to_string( tbbThreads ) + "-threads-";

	  switch (gridType)
	  {
		case dem::mappings::CreateGrid::NoGrid:
		  sharedMemoryPropertiesFileName += "no-grid";
		  break;
		case dem::mappings::CreateGrid::RegularGrid:
		  sharedMemoryPropertiesFileName += "regular-grid";
		  break;
		case dem::mappings::CreateGrid::AdaptiveGrid:
		  sharedMemoryPropertiesFileName += "adaptive-grid";
		  break;
		case dem::mappings::CreateGrid::FlopAdaptiveGrid:
		  sharedMemoryPropertiesFileName += "flop-grid";
		  break;
		case dem::mappings::CreateGrid::ReluctantAdaptiveGrid:
		  sharedMemoryPropertiesFileName += "reluctant-grid";
		  break;
	  }

	  sharedMemoryPropertiesFileName += ".properties";
    }

    peano::datatraversal::autotuning::OracleForOnePhase *_manual = new peano::datatraversal::autotuning::OracleForOnePhaseDummy(
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
    );

    peano::datatraversal::autotuning::Oracle::getInstance().setOracle(_manual);

  #endif
  return sharedMemoryPropertiesFileName;
}

void dem::runners::Runner::tuneOrNotTotune(bool useAutotuning, std::string sharedMemoryPropertiesFileName)
{
  #ifdef SharedMemoryParallelisation
  if (useAutotuning == true)
  {
    peano::datatraversal::autotuning::Oracle::getInstance().setOracle(
     new sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize(true,false));

    peano::datatraversal::autotuning::Oracle::getInstance().loadStatistics(sharedMemoryPropertiesFileName);
  }
  #endif
}

void shutSharedMemory(std::string sharedMemoryPropertiesFileName, bool useAutotuning)
{
  if("" == sharedMemoryPropertiesFileName) {return;}
  #ifdef SharedMemoryParallelisation
	if(useAutotuning)
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

int dem::runners::Runner::run(int numberOfTimeSteps, Plot plot, dem::mappings::CreateGrid::GridType gridType, int tbbThreads, double stepSize, double realSnapshot, int useAutotuning)
{
  peano::geometry::Hexahedron geometry(tarch::la::Vector<DIMENSIONS,double>(1.0), tarch::la::Vector<DIMENSIONS,double>(0.0));
  dem::repositories::Repository* repository = dem::repositories::RepositoryFactory::getInstance().createWithSTDStackImplementation(geometry,
                                                tarch::la::Vector<DIMENSIONS,double>(1.0),  // domainSize
                                                tarch::la::Vector<DIMENSIONS,double>(0.0)); // computationalDomainOffset
  //initHPCEnvironment();
  std::string sharedMemoryPropertiesFileName = initSharedMemory(useAutotuning, tbbThreads, gridType);
  //initDistributedMemory();

  int result = 0;
  if (tarch::parallel::Node::getInstance().isGlobalMaster())
  {
    result = runAsMaster( *repository, useAutotuning, sharedMemoryPropertiesFileName, numberOfTimeSteps, realSnapshot, plot, gridType, stepSize);
  }
  #ifdef Parallel
  else {
    result = runAsWorker(*repository);
  }
  #endif

  shutSharedMemory(sharedMemoryPropertiesFileName, useAutotuning);
  shutDistributedMemory();

  delete repository;

  return result;
}

int dem::runners::Runner::runAsMaster(
	dem::repositories::Repository& repository,
	bool useAutotuning,
	std::string sharedMemoryPropertiesFileName,
	int iterations,
	double realSnapshot,
	Plot plot,
	dem::mappings::CreateGrid::GridType gridType,
	double initialStepSize)
{
  peano::utils::UserInterface::writeHeader();

  dem::mappings::AdoptGrid::_refinementCoefficient = 1.8;
  dem::mappings::AdoptGrid::_coarsenCoefficient = 0.5;

  dem::mappings::ReluctantlyAdoptGrid::_coarsenCoefficientReluctant = 1.8;
  dem::mappings::ReluctantlyAdoptGrid::_refinementCoefficientReluctant = 0.5;

  //dem::mappings::FlopAdoptGrid::_coarsenCoefficient = 1.8;
  //dem::mappings::FlopAdoptGrid::_refinementCoefficient = 0.5;

  logInfo( "runAsMaster(...)", "create grid" );

  repository.switchToCreateGrid();
  repository.iterate();

  precondition(repository, gridType);

  logInfo( "runAsMaster(...)", "start time stepping" );

  repository.getState().clearAccumulatedData();
  repository.getState().setInitialTimeStepSize(initialStepSize);

  double elapsed = 0.0;
  double timestamp = 0.0;

  if(plot == EveryIteration)
  {
     dem::mappings::Plot::_mini = 0;
     dem::mappings::Plot::_maxi = iterations;
  } else {
    dem::mappings::Plot::_mini = 0;
    dem::mappings::Plot::_maxi = 50000;
  }

  tuneOrNotTotune(useAutotuning, sharedMemoryPropertiesFileName);

  int i=0;

  //////////////////PLOT STEP ZERO//////////////////////////////////////////////////////
  if((plot == EveryIteration) ||  (plot == Track) ||
      (plot == UponChange && (repository.getState().getNumberOfContactPoints()>0 ||
                              !repository.getState().isGridStationary() || 0%50==0 ||
                              repository.getState().getNumberOfParticleReassignments()>0 )) ||
      (plot == EveryBatch && 0%50 == 0) ||
      ((plot == Adaptive && ((elapsed > realSnapshot) || (0 == 0)))))
  {
    timestamp = repository.getState().getTime();
    repository.getState().setTimeStep(i);

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

  for (i=i+0; i<iterations; i++)
  {
    printf("entered\n");
    timestamp = repository.getState().getTime();
    repository.getState().setTimeStep(i);
    printf("entered\n");
    bool plotThisTraversal = switchrepo(repository, i, gridType, plot, elapsed, realSnapshot);
    printf("entered\n");
    repository.iterate();
    printf("entered\n");
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
