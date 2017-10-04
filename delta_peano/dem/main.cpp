#include "tarch/logging/Log.h"
#include "tarch/tests/TestCaseRegistry.h"
#include "tarch/logging/CommandLineLogger.h"
#include "tarch/parallel/Node.h"

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
	std::cerr << "Delta - Peano | Grid-based Non-Spherical Particle Dynamics" << std::endl << std::endl
				  << "Usage: ./dem-xxx grid_h_max particle_diam_min particle_diam_max scenario iterations grid-type step-size plot real-time-snapshot(sec) gravity(boolean) collision-model tri-per-particle [tbb-core-count]" << std::endl
          << std::endl
          << " grid_h_max          maximum mesh width of grid" << std::endl
          << " particle_diam_min   minimal diameter of particles" << std::endl
          << " particle_diam_max   maximal diameter of particles" << std::endl
          << " scenario            which scenario to use. See list below for valid configurations " << std::endl
          << " iterations          number of iterations or time steps depending on scheme" << std::endl
          << " grid-type           which grid type to use. See list below for valid configurations " << std::endl
          << " step-size           floating point number" << std::endl
          << " plot                see plot variants below" << std::endl
          << " gravity             boolean true/false" << std::endl
          << " collision-model     choose from none, sphere, bf, penalty, hybrid-on-triangle-pairs, hybrid-on-batches, gjk, add sphere- for sphere check" << std::endl
				  << " max-step-size       adaptive max step" << std::endl
				  << " tri-per-particle    triangles used to represent one particle" << std::endl
				  << " [core-count]        only required in TBB shared memory" << std::endl
          << std::endl << std::endl << std::endl << std::endl
          << "Scenarios" << std::endl
          << "=========" << std::endl
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
          << "  frictionRoll" << std::endl

          << "Grid types" << std::endl
          << "==========" << std::endl
          << "  no-grid" << std::endl
          << "  regular-grid" << std::endl
          << "  adaptive-grid" << std::endl
          << "  reluctant-grid" << std::endl
          << "Plot variants" << std::endl
          << "=============" << std::endl
          << "  never" << std::endl
          << "  every-iteration" << std::endl
          << "  upon-change" << std::endl
          << "  every-batch" << std::endl
				  << "  every-checkpoint" << std::endl
				  << "  range" << std::endl << std::endl
          << "Usage: ./dem-xxx grid_h_max particle_diam_min particle_diam_max scenario iterations grid-type step-size plot real-time-snapshot(sec) gravity(boolean) collision-model mesh-multiplier [tbb-core-count]" << std::endl
				  << "eg: ./dem-xxx 0.5 0.5 0.5 hopperUniform 10000 regular-grid 0.00001 every-batch 10 true bf 50 2" << std::endl;
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
  	  const int NumberOfArguments = 14;
  #else
  	  const int NumberOfArguments = 13;
  #endif

  	std::cout << "required arguments: " <<  NumberOfArguments << ", actual arguments: " <<  argc << std::endl;

  if(argc!=NumberOfArguments || (NumberOfArguments == NumberOfArguments+2 && argv[8] != "range"))
  {
    printManual();
    return -1;
  }

  const double       gridHMax            = atof(argv[1]);
  const double       particleDiamMin     = atof(argv[2]);
  const double       particleDiamMax     = atof(argv[3]);
  const std::string  scenario            = argv[4];
  const int          iterations          = atoi(argv[5]);
  const std::string  gridTypeIdentifier  = argv[6];
  const double       stepSize         	 = atof(argv[7]);
  const std::string  plotIdentifier      = argv[8];
  const double		   realSnapshot		     = atof(argv[9]);
  const std::string  gravity             = argv[10];
  const std::string  collisionModel      = argv[11];
  const int			     meshMultiplier 	   = atof(argv[12]);

  #ifdef SharedMemoryParallelisation
  	  const int          numberOfCores       = atoi(argv[13]);
  #else
  	  const int          numberOfCores       = 0;
  #endif

  int programExitCode = 0;
  if (gridHMax>0.5) {
    logError( "main()", "gridHMax has to be smaller than or equal to 0.5" );
    programExitCode = 1;
  }
  if (gridHMax<particleDiamMax) {
    logError( "main()", "grid_h_max has to be smaller or equal to particles' maximum diameter" );
    programExitCode = 1;
  }
  if (particleDiamMax<particleDiamMin) {
    logError( "main()", "grid_h_max has to be greater or equal to grid_h_min" );
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

  dem::mappings::CreateGrid::setScenario(scenarioArray, gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);

  dem::runners::Runner::Plot plot;
  int trackID = -1;
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

  logInfo( "run(...)", "scenario:" << (scenario)                                 << ", "  << "iterations:" << (iterations) << ", grid:"            << (gridType)       << std::endl
                    << "stepSize:" << (stepSize)                                 << ", plot:"     << (plotIdentifier)      << ", realSnapshot:"    << (realSnapshot)   << std::endl
                    << "gravity:"  << (dem::mappings::Collision::gravity)    << ", model:"    << (collisionModel)      << ", triangleNumber:"  << (meshMultiplier));

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
    programExitCode = runner.run(iterations, plot, gridType, numberOfCores, stepSize, realSnapshot, true);
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
