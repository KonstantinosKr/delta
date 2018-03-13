#include "tarch/logging/Log.h"
#include "tarch/tests/TestCaseRegistry.h"
#include "tarch/logging/CommandLineLogger.h"
#include "tarch/parallel/Node.h"
#include "tarch/multicore/Jobs.h"

#include "peano/peano.h"

#include "dem/runners/Runner.h"
#include "dem/mappings/CreateGrid.h"
#include "dem/mappings/Collision.h"
#include "dem/mappings/MoveParticles.h"
#include "delta/geometry/material.h"

#include "delta/geometry/properties.h"

#include <iomanip>

tarch::logging::Log _log("");

void printManual()
{
	std::cerr
	  << "##################################################################################################" << std::endl
	  << "#  Delta Library - Peano" << std::endl
	  << "##################################################################################################" << std::endl
	  << "#  A Grid-based Non-Spherical Particle Dynamics Library" << std::endl
	  << "##################################################################################################" << std::endl
	  << " Usage: ./dem-xxx grid_h_max(double) scenario(string) iterations(int) grid-type(string) step-size(int) plot(string) snapshot-frequency(double in seconds) gravity(boolean yes/no) collision-model(string) mesh-density-per-particle(int) [tbb-core-count](int)" << std::endl  << std::endl
	  << " Option Type---------Short Description" << std::endl
	  << " grid_h_max          maximum mesh width of grid" << std::endl
	  << " scenario            which scenario to use. See list below for valid configurations " << std::endl
	  << " iterations          number of iterations or time steps depending on scheme" << std::endl
	  << " grid-type           which grid type to use. See list below for valid configurations " << std::endl
	  << " step-size           negative value trigger adaptive step size with a positive initial step size | positive value triggers static step size" << std::endl
	  << " plot                see plot variants below" << std::endl
	  << " gravity             boolean true/false" << std::endl
	  << " collision-model     choose from none, sphere, bf, penalty, hybrid-on-triangle-pairs, hybrid-on-batches, gjk, add sphere- for sphere check" << std::endl
	  << " max-step-size       adaptive max step" << std::endl
	  << " triangles-per-particle    triangles used to represent one particle" << std::endl
	  << " [core-count]        only required in TBB shared memory" << std::endl
    << " [run-grid-parallel] only required in TBB shared memory" << std::endl
    << " [run-particles-parallel] only required in TBB shared memory" << std::endl
    << " [run-comparisons-in-background] only required in TBB shared memory. Either off or a positive number" << std::endl
	  << std::endl << std::endl << std::endl << std::endl

	  << " OPTIONS: "  << std::endl << std::endl
	  << " Grid H max (double)" << std::endl
	  << " =========" << std::endl
	  << " 0.0 - 0.5" << std::endl

	  << " Scenarios (string)" << std::endl
	  << " =========" << std::endl
	  << "  none" << std::endl
	  << "  blackholeWithRandomlyOrientedCubes" << std::endl
	  << "  blackholeWithCubes" << std::endl
	  << "  blackholeWithGranulates" << std::endl

	  << "  freefallWithRandomlyOrientedCubes" << std::endl
	  << "  freefallWithCubes" << std::endl
	  << "  freefallWiithGranulates" << std::endl

	  << "  two-particles-crash" << std::endl

	  << "  SLA"<< std::endl
	  << "  DLA"<< std::endl
	  << "  nuclearDeck4" << std::endl
	  << "  nuclearDeck32" << std::endl
	  << "  nuclearDeck64" << std::endl
	  << "  nuclearDeck256" << std::endl

	  << "  hopperUniform" << std::endl
	  << "  hopperUniform1k" << std::endl
	  << "  hopperUniform10k" << std::endl
	  << "  hopperUniform100k" << std::endl
	  << "  hopperUniform500k" << std::endl

	  << "  hopperNonUniform" << std::endl
	  << "  hopperNonUniform1k" << std::endl
	  << "  hopperNonUniform10k" << std::endl
	  << "  hopperNonUniform100k" << std::endl
	  << "  hopperNonUniform500k" << std::endl

	  << "  frictionStatic" << std::endl
	  << "  frictionSlide" << std::endl
	  << "  frictionRoll" << std::endl << std::endl

    << " Iterations (int)" << std::endl
    << " =========" << std::endl
    << " 0 - infinity" << std::endl << std::endl

	  << " Grid types (string)" << std::endl
	  << " ==========" << std::endl
	  << "  no-grid" << std::endl
	  << "  regular-grid" << std::endl
	  << "  adaptive-grid" << std::endl
	  << "  reluctant-grid" << std::endl << std::endl

    << " Step-size (double)" << std::endl
    << " =========" << std::endl
    << " -infinity - infinity" << std::endl << std::endl

	  << " Plot variants (string)" << std::endl
	  << " =============" << std::endl
	  << "  never" << std::endl
	  << "  every-iteration" << std::endl
	  << "  upon-change" << std::endl
	  << "  every-batch" << std::endl
	  << "  every-checkpoint" << std::endl
	  << "  range" << std::endl << std::endl

    << " snapshot-frequency (double)" << std::endl
    << " =========" << std::endl
    << " 0.0 - infinity" << std::endl << std::endl

    << " Gravity (boolean)" << std::endl
    << " =========" << std::endl
    << " true/false" << std::endl << std::endl

    << " Collision-model (string)" << std::endl
    << " =========" << std::endl
    << " sphere" << std::endl
    << " bf" << std::endl
    << " penalty" << std::endl
    << " penaltyStat" << std::endl
    << " hybrid-on-triangle-pairs" << std::endl
    << " hybrid-on-batches" << std::endl
    << " hybridStat" << std::endl
    << " gjk" << std::endl
    << " none" << std::endl << std::endl

    << " Mesh-density [if mesh is used] (int)" << std::endl
    << " =========" << std::endl
    << " 0 - infinity" << std::endl << std::endl

    << " tbb-core-count (int)" << std::endl
    << " =========" << std::endl
    << " 0 - infinity" << std::endl << std::endl

	  << " Usage: ./dem-xxx grid_h_max(double) scenario(string) iterations(int) grid-type(string) step-size(int) plot(string) snapshot-frequency(double in seconds) gravity(boolean yes/no) collision-model(string) mesh-density-per-particle(int) [tbb-core-count](int)" << std::endl
	  << " eg: ./dem-xxx 0.5 0.5 0.5 hopperUniform 10000 regular-grid 0.00001 every-batch 10 true bf 50 2" << std::endl;
}

int main(int argc, char** argv)
{
  peano::fillLookupTables();
  int parallelSetup = peano::initParallelEnvironment(&argc,&argv);

  if ( parallelSetup!=0 ) {
    #ifdef Parallel
	  // Please do not use the logging if MPI doesn't work properly.
	  std::cerr << "mpi initialisation wasn't successful. Application shut down" << std::endl;
    #else
	  _log.error("main()", "mpi initialisation wasn't successful. Application shut down");
    #endif
    return parallelSetup;
  }

  int sharedMemorySetup = peano::initSharedMemoryEnvironment();
  if (sharedMemorySetup!=0)
  {
    logError("main()", "shared memory initialisation wasn't successful. Application shut down");
    return sharedMemorySetup;
  }

  #ifdef SharedMemoryParallelisation
  	  const int NumberOfArguments = 16;
  #else
  	  const int NumberOfArguments = 11;
  #endif

  	logInfo( "main(...)", "required arguments: " <<  NumberOfArguments << ", actual arguments: " <<  argc);

  if(argc!=NumberOfArguments || (NumberOfArguments == NumberOfArguments+2 && argv[8] != "range"))
  {
    printManual();
    return -1;
  }

  const double       	gridHMax            	= atof(argv[1]);
  const std::string  	scenario            	= argv[2];
  const int          	iterations         	= atoi(argv[3]);
  const std::string  	gridTypeIdentifier  	= argv[4];
  const double       	stepSize         	= atof(argv[5]);
  const std::string  	plotIdentifier      	= argv[6];
  const double			realSnapshot		    = atof(argv[7]);
  const std::string  	gravity             	= argv[8];
  const std::string  	collisionModel     	= argv[9];
  const int			    meshMultiplier      	= atof(argv[10]);
  bool					autotune 			= false;

  #ifdef SharedMemoryParallelisation
	  const int          numberOfCores       = atoi(argv[11]);

    if (std::string(argv[12])=="true") {
      dem::mappings::Collision::RunGridTraversalInParallel = true;
    }
    else if (std::string(argv[12])=="false") {
      dem::mappings::Collision::RunGridTraversalInParallel = false;
    }
    else {
      logError( "main()", "grid parallelisation either has to be true or false" );
      return -1;
    }

    if (std::string(argv[13])=="true") {
      dem::mappings::Collision::RunParticleLoopInParallel = true;
    }
    else if (std::string(argv[13])=="false") {
      dem::mappings::Collision::RunParticleLoopInParallel = false;
    }
    else {
      logError( "main()", "particle parallelisation either has to be true or false" );
      return -1;
    }

    if (std::string(argv[14])=="off") {
      dem::mappings::Collision::RunParticleComparisionsInBackground = false;
    }
    else if(std::string(argv[14])=="auto") {
      logError( "main()", "max number of background tasks is set to " << numberOfCores );
      dem::mappings::Collision::RunParticleComparisionsInBackground = true;
      tarch::multicore::jobs::Job::setMaxNumberOfRunningBackgroundThreads( numberOfCores );
    }
    else {
      const int  numberOfBackgroundTasks = atoi(argv[14]);

      if (numberOfBackgroundTasks>=1) {
        logError( "main()", "max number of background tasks is set to " << numberOfBackgroundTasks );
        dem::mappings::Collision::RunParticleComparisionsInBackground = true;
        tarch::multicore::jobs::Job::setMaxNumberOfRunningBackgroundThreads( numberOfBackgroundTasks );
      }
      else {
        logError( "main()", "run in background either has to be true or false" );
        return -1;
      }
    }

   if (std::string(argv[15])=="true")
   {
	 autotune = true;
   } else if (std::string(argv[15])=="false")
   {
	 autotune = false;
   }
   else {
     logError( "main()", "autotuning either has to be true or false" );
     return -1;
   }
  #else
	  const int          numberOfCores       = 0;
  #endif

  int programExitCode = 0;
  if (gridHMax>0.5) {
    logError( "main()", "gridHMax has to be smaller than or equal to 0.5" );
    programExitCode = 1;
  }

  dem::mappings::CreateGrid::GridType gridType;

  if (gridTypeIdentifier=="no-grid") {
    gridType = dem::mappings::CreateGrid::NoGrid;
    if(argc != NumberOfArguments)
    {
      printManual();
      return -1;
    }
  }
  else if (gridTypeIdentifier=="regular-grid") {
    if(argc != NumberOfArguments)
    {
      printManual();
      return -1;
    }
    gridType = dem::mappings::CreateGrid::RegularGrid;
  }
  else if (gridTypeIdentifier=="adaptive-grid") {
    gridType = dem::mappings::CreateGrid::AdaptiveGrid;
    if(argc != NumberOfArguments)
    {
    	printManual();
      return -1;
    }
  }
  else if (gridTypeIdentifier=="reluctant-grid") {
    gridType = dem::mappings::CreateGrid::ReluctantAdaptiveGrid;
    if(argc != NumberOfArguments)
    {
    	printManual();
      return -1;
    }
  }
  else if (gridTypeIdentifier == "range")
  {
	  if(argc != NumberOfArguments+2)
	  {
		  printManual();
		  return -1;
	  }
  } else {
    std::cerr << gridTypeIdentifier << " is not a valid grid type. Please run without arguments to see list of valid grid types" << std::endl;
    programExitCode = 2;
  }

  dem::mappings::CreateGrid::Scenario scenarioArray[4];
  if (scenario=="nonescenario") {
    scenarioArray[0] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;

  } else if (scenario=="blackHoleWithRandomlyOrientedCubes") {
    scenarioArray[0] = dem::mappings::CreateGrid::blackHoleWithRandomOrientedCubes,
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="blackHoleWithCubes") {
    scenarioArray[0] = dem::mappings::CreateGrid::blackHoleWithCubes;
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="blackHoleWithGranulates") {
    scenarioArray[0] = dem::mappings::CreateGrid::blackHoleWithGranulates;
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="freefallWithRandomlyOrientedCubes") {
    scenarioArray[0] = dem::mappings::CreateGrid::freefallWithRandomOrientedCubes;
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="freefallWithCubes") {
    scenarioArray[0] = dem::mappings::CreateGrid::freefallWithCubes;
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="freefallWithGranulates") {
    scenarioArray[0] = dem::mappings::CreateGrid::freefallWithGranulates;
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="nuclearSLA") {
    scenarioArray[0] = dem::mappings::CreateGrid::sla;
    scenarioArray[1] = dem::mappings::CreateGrid::nuclear;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="nuclearDLA") {
    scenarioArray[0] = dem::mappings::CreateGrid::dla;
    scenarioArray[1] = dem::mappings::CreateGrid::nuclear;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="nuclearDeck1") {
    scenarioArray[0] = dem::mappings::CreateGrid::nuclearDeck;
    scenarioArray[1] = dem::mappings::CreateGrid::nuclear;
    scenarioArray[2] = dem::mappings::CreateGrid::n1;
  }
  else if (scenario=="nuclearDeck4") {
    scenarioArray[0] = dem::mappings::CreateGrid::nuclearDeck;
    scenarioArray[1] = dem::mappings::CreateGrid::nuclear;
    scenarioArray[2] = dem::mappings::CreateGrid::n4;
  }
  else if (scenario=="nuclearDeck32") {
    scenarioArray[0] = dem::mappings::CreateGrid::nuclearDeck;
    scenarioArray[1] = dem::mappings::CreateGrid::nuclear;
    scenarioArray[2] = dem::mappings::CreateGrid::n32;
  }
  else if (scenario=="nuclearDeck64") {
    scenarioArray[0] = dem::mappings::CreateGrid::nuclearDeck;
    scenarioArray[1] = dem::mappings::CreateGrid::nuclear;
    scenarioArray[2] = dem::mappings::CreateGrid::n64;
  }
  else if (scenario=="nuclearDeck256") {
    scenarioArray[0] = dem::mappings::CreateGrid::nuclearDeck;
    scenarioArray[1] = dem::mappings::CreateGrid::nuclear;
    scenarioArray[2] = dem::mappings::CreateGrid::n256;
  }
  else if (scenario=="two-particles-crash") {
    scenarioArray[0] = dem::mappings::CreateGrid::TwoParticlesCrash;
    scenarioArray[1] = dem::mappings::CreateGrid::nonescenario;
    scenarioArray[2] = dem::mappings::CreateGrid::nonescenario;
  }
  else if (scenario=="hopperUniform") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperUniform;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::uniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n100;
  }
  else if (scenario=="hopperUniform1k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperUniform1k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::uniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n1k;
  }
  else if (scenario=="hopperUniform10k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperUniform10k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::uniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n10k;
  }
  else if (scenario=="hopperUniform100k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperUniform100k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::uniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n100k;
  }
  else if (scenario=="hopperUniform500k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperUniform500k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::uniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n500k;
  }
  else if (scenario=="hopperNonUniform") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperNonUniform;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::nonuniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n100;
  }
  else if (scenario=="hopperNonUniform1k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperNonUniform1k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::nonuniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n1k;
  }
  else if (scenario=="hopperNonUniform10k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperNonUniform10k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::nonuniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n10k;
  }
  else if (scenario=="hopperNonUniform100k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperNonUniform100k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::nonuniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n100k;
  }
  else if (scenario=="hopperNonUniform500k") {
    scenarioArray[0] = dem::mappings::CreateGrid::hopperNonUniform500k;
    scenarioArray[1] = dem::mappings::CreateGrid::hopper;
    scenarioArray[2] = dem::mappings::CreateGrid::nonuniform;
    scenarioArray[3] = dem::mappings::CreateGrid::n500k;
  }
  //////////////PHYSICS TESTS START/////////////////////////////////////
  else if (scenario=="frictionStatic") {
    scenarioArray[0] = dem::mappings::CreateGrid::frictionStatic;
    scenarioArray[1] = dem::mappings::CreateGrid::friction;
    scenarioArray[2] = dem::mappings::CreateGrid::sstatic;
  }
  else if (scenario=="frictionSlide") {
    scenarioArray[0] = dem::mappings::CreateGrid::frictionSlide;
    scenarioArray[1] = dem::mappings::CreateGrid::friction;
    scenarioArray[2] = dem::mappings::CreateGrid::slide;
  }
  else if (scenario=="frictionRoll") {
    scenarioArray[0] = dem::mappings::CreateGrid::frictionRoll;
    scenarioArray[1] = dem::mappings::CreateGrid::friction;
    scenarioArray[2] = dem::mappings::CreateGrid::roll;
  }
  //////////////PHYSICS TESTS END/////////////////////////////////////
  else {
    std::cerr << "not a valid scenario. Please run without arguments to see list of valid scenarios" << std::endl;
    programExitCode = 2;
  }

  dem::mappings::CreateGrid::setScenario(scenarioArray, gridHMax, gridType, meshMultiplier);

  dem::runners::Runner::Plot plot;
  //int trackID = -1;
  if (plotIdentifier=="never") {
    plot = dem::runners::Runner::Never;
  }
  else if(plotIdentifier=="every-iteration") {
    plot = dem::runners::Runner::EveryIteration;
  }
  else if(plotIdentifier=="upon-change") {
    plot = dem::runners::Runner::UponChange;
  }
  else if(plotIdentifier=="every-batch") {
    plot = dem::runners::Runner::EveryBatch;
  }
  else if(plotIdentifier=="adaptive") {
    plot = dem::runners::Runner::Adaptive;
  }
  else if(plotIdentifier=="range") {
  	plot = dem::runners::Runner::Range;
  }
  else if(plotIdentifier=="track") {
    plot = dem::runners::Runner::Track;
    //dem::mappings::Plot::_trackID = trackID;
    //dem::mappings::Plot::_trackParticleID.push_back(0);
    //dem::mappings::Plot::_trackParticleID.push_back(1);
  }else {
    std::cerr << "invalid plot option: " << plotIdentifier << "Please run without arguments to see list of valid plot variants" << std::endl;
    programExitCode = 2;
  }

  dem::mappings::Collision::_enableOverlapCheck = false;
  if (collisionModel=="sphere" || collisionModel=="sphere-sphere") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::Sphere;
    if(collisionModel=="sphere-sphere") {dem::mappings::Collision::_enableOverlapCheck = true;}
  }
  else if((collisionModel=="bf") || (collisionModel=="sphere-bf"))
  {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::BruteForce;
    if(collisionModel=="sphere-bf") {dem::mappings::Collision::_enableOverlapCheck = true;}
  }
  else if((collisionModel=="penalty") || (collisionModel=="sphere-penalty"))
  {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::Penalty;
    if(collisionModel=="sphere-penalty") {dem::mappings::Collision::_enableOverlapCheck = true;}
  }
  else if (collisionModel=="penaltyStat") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::PenaltyStat;
  }
  else if (collisionModel=="penaltyTune") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::PenaltyTune;
  }
  else if((collisionModel=="hybrid-on-triangle-pairs") || (collisionModel=="sphere-hybrid-on-triangle-pairs"))
  {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::HybridOnTrianglePairs;
    if(collisionModel=="sphere-hybrid-on-triangle-pairs") {dem::mappings::Collision::_enableOverlapCheck = true;}
  }
  else if ((collisionModel=="hybrid-on-batches") || (collisionModel=="sphere-hybrid-on-batches"))
  {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::HybridOnBatches;
    if(collisionModel=="sphere-hybrid-on-batches") {dem::mappings::Collision::_enableOverlapCheck = true;}
  }
  else if ((collisionModel=="hybridStat") || (collisionModel=="sphere-hybridStat"))
  {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::HybridStat;
    if(collisionModel=="sphere-hybridStat") {dem::mappings::Collision::_enableOverlapCheck = true;}
  }
  else if (collisionModel=="gjk") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::GJK;
    if(collisionModel=="sphere-hybridStat") {dem::mappings::Collision::_enableOverlapCheck = true;}
  }
  else if (collisionModel=="none") {
      dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::none;
  } else {
    std::cerr << "invalid collision model option. Please run without arguments to see list of valid variants" << std::endl;
    programExitCode = 2;
  }

  dem::mappings::Collision::gravity	= (gravity=="true") ? 9.81 : 0.0;

  logInfo( "main(...)", "scenario:" << (scenario)                              << ", "  << "iterations:" << (iterations) << ", grid:"            << (gridType)
                    << ", stepSize:" << (stepSize)                             << ", plot:"     << (plotIdentifier)      << ", realSnapshot:"    << (realSnapshot)
                    << ", gravity:"  << (dem::mappings::Collision::gravity)    << ", model:"    << (collisionModel)      << ", triangleNumber:"  << (meshMultiplier));

  // Configure the output
  tarch::logging::CommandLineLogger::getInstance().clearFilterList();
  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "info", false ) );
  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "debug", true ) );

  if (programExitCode==0)
  {
    tarch::tests::TestCaseRegistry::getInstance().getTestCaseCollection().run();
    programExitCode = tarch::tests::TestCaseRegistry::getInstance().getTestCaseCollection().getNumberOfErrors();
  }

  if (programExitCode==0)
  {
    tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "debug", -1, "dem", false ) );
    tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "info", -1, "peano", true ) );
    tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "info", -1, "peano::utils", false ) );
    dem::runners::Runner runner;
    programExitCode = runner.run(iterations, plot, gridType, numberOfCores, stepSize, realSnapshot, autotune);
  }
  
  if (programExitCode==0)
  {
    #ifdef Parallel
    if (tarch::parallel::Node::getInstance().isGlobalMaster()) {
      logInfo( "main()", "Peano terminates successfully" );
    }
    #else
    logInfo( "main()", "Peano terminates successfully" );
    #endif
  }
  else
  {
    logInfo( "main()", "quit with error code " << programExitCode );
  }

  peano::shutdownParallelEnvironment();
  peano::shutdownSharedMemoryEnvironment();
  peano::releaseCachedData();

  return programExitCode;
}
