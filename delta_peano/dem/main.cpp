#include "tarch/logging/Log.h"
#include "tarch/tests/TestCaseRegistry.h"
#include "tarch/logging/CommandLineLogger.h"
#include "tarch/parallel/Node.h"

#include "peano/peano.h"

#include "dem/runners/Runner.h"
#include "dem/mappings/CreateGrid.h"
#include "dem/mappings/Collision.h"
#include "dem/mappings/MoveParticles.h"
#include "delta/collision/material.h"

#include <iomanip>

tarch::logging::Log _log("");

void printManual()
{
	std::cerr << "Delta - Peano | Grid-based Non-Spherical Particle Dynamics" << std::endl << std::endl
				  << "Usage: ./dem-xxx grid_h_max particle_diam_min particle_diam_max scenario iterations grid-type step-size plot real-time-snapshot(sec) gravity(boolean) collision-model mesh-multiplier [tbb-core-count]" << std::endl
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
				  << " mesh-per-particle   multiplier for mesh size and sphericity" << std::endl
				  << " [core-count]        only required in TBB shared memory" << std::endl
          << std::endl << std::endl << std::endl << std::endl
          << "Scenarios" << std::endl
          << "=========" << std::endl
          << "  black-hole-with-cubes" << std::endl
          << "  black-hole-with-randomly-oriented-cubes" << std::endl
          << "  random-velocities-with-cubes" << std::endl
          << "  random-velocities-with-granulates" << std::endl

          << "  sla"<< std::endl
          << "  nuclearArray" << std::endl
          << "  two-particles-crash" << std::endl

          << "  hopperUniform" << std::endl
          << "  hopperUniform1k" << std::endl
          << "  hopperUniform10k" << std::endl
          << "  hopperUniform50k" << std::endl

          << "  hopperNonUniform1k" << std::endl
          << "  hopperNonUniform10k" << std::endl
          << "  hopperNonUniform50k" << std::endl

				  << "  freefall" << std::endl

				  << "  frictionStaticSphere" << std::endl
				  << "  frictionSlideSphere" << std::endl
          << "  frictionRollSphere" << std::endl
          << "  frictionStaticMesh" << std::endl
          << "  frictionSlideMesh" << std::endl
          << "  frictionRollMesh" << std::endl

          << "Grid types" << std::endl
          << "==========" << std::endl
          << "  no-grid" << std::endl
          << "  regular-grid" << std::endl
          << "  adaptive-grid" << std::endl
          << "  reluctant-adaptive-grid" << std::endl
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

  	std::cout << "required arguments: " <<  NumberOfArguments << "actual arguments: " <<  argc << std::endl;

  if(argc!=NumberOfArguments || (NumberOfArguments == NumberOfArguments+2 && argv[8] != "range"))
  {
    printManual();
    return -1;
  } else if (argc == NumberOfArguments) {
	  std::cout << "gridHmax:" << std::setprecision(3) << atof(argv[1]) << ", particleDiamMax:" << atof(argv[2]) << ", particleDiamMin:" << atof(argv[3]) << std::endl
              << "scenario:" << (argv[4]) << ", iterations:" << atof(argv[5]) << ", grid:" << (argv[6]) << std::endl
              << "stepSize:" << atof(argv[7]) << ", plot:" << (argv[8]) << ", realSnapshot:" << atoi(argv[9]) << std::endl
              << "gravity:" << (argv[10]) << ", model:" << (argv[11])  << ", triangleNumber:" << atoi(argv[12]) << std::endl;
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
  const bool         gravity             = (argv[10]);
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
  else if (gridTypeIdentifier=="reluctant-adaptive-grid") {
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

  if (scenario=="black-hole-with-cubes") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::BlackHoleWithCubes,
                                            dem::mappings::CreateGrid::randomLinear,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="black-hole-with-randomly-oriented-cubes") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::BlackHoleWithRandomOrientedCubes,
                                            dem::mappings::CreateGrid::randomLinearAngular,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="random-velocities-with-cubes") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::RandomWithCubes,
                                            dem::mappings::CreateGrid::randomLinearAngular,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="random-velocities-with-granulates") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::RandomWithGranulates,
                                            dem::mappings::CreateGrid::randomLinear,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  } else if (scenario=="sla") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::sla,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="nuclearArray") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::nuclearArray,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="two-particles-crash") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::TwoParticlesCrash,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }//hopper diagonal is 0.39
  else if (scenario=="hopperUniform") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperUniform,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="hopperUniform1k") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperUniform1k,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="hopperUniform10k") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperUniform10k,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="hopperUniform50k") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperUniform50k,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="hopperNonUniform") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperNonUniform,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, gridHMax, gridType, meshMultiplier);
  }
  else if (scenario=="hopperNonUniform1k") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperNonUniform1k,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="hopperNonUniform10k") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperNonUniform10k,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="hopperNonUniform50k") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperNonUniform50k,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="freefall") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::freefall,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  //////////////PHYSICS TESTS START/////////////////////////////////////
  else if (scenario=="frictionStaticSphere") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionStaticSphere,
                                            dem::mappings::CreateGrid::crashY,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="frictionSlideSphere") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionSlideSphere,
                                            dem::mappings::CreateGrid::slideX,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="frictionSlideRotateSphere") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionSlideSphere,
                                            dem::mappings::CreateGrid::slideXRotation,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="frictionRollSphere") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionRollSphere,
                                            dem::mappings::CreateGrid::slideXRotation,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="frictionStaticMesh") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionStaticMesh,
                                            dem::mappings::CreateGrid::crashY,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="frictionSlideMesh") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionSlideMesh,
                                            dem::mappings::CreateGrid::slideX,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="frictionSlideRotateMesh") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionSlideMesh,
                                            dem::mappings::CreateGrid::slideXRotation,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  else if (scenario=="frictionRollMesh") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionRollMesh,
                                            dem::mappings::CreateGrid::noVScheme,
                                            gridHMax, particleDiamMin, particleDiamMax, gridType, meshMultiplier);
  }
  //////////////PHYSICS TESTS END/////////////////////////////////////
  else {
    std::cerr << "not a valid scenario. Please run without arguments to see list of valid scenarios" << std::endl;
    programExitCode = 2;
  }

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

  if (collisionModel=="sphere") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::Sphere;
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

  dem::mappings::MoveParticles::gravity	= (gravity==true) ? 9.81 : 0.0;
  //delta::collision::material::initMaterial();

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
    programExitCode = runner.run(iterations, plot, gridType, numberOfCores, stepSize, realSnapshot);
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
