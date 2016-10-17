#include "tarch/logging/Log.h"
#include "tarch/tests/TestCaseRegistry.h"
#include "tarch/logging/CommandLineLogger.h"
#include "tarch/parallel/Node.h"

#include "peano/peano.h"

#include "dem/runners/Runner.h"
#include "dem/mappings/CreateGrid.h"
#include "dem/mappings/Collision.h"
#include "dem/mappings/MoveParticles.h"

#define epsilon 0.002

tarch::logging::Log _log("");

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
  if (sharedMemorySetup!=0) {
    logError("main()", "shared memory initialisation wasn't successful. Application shut down");
    return sharedMemorySetup;
  }

  #ifdef SharedMemoryParallelisation
  const int NumberOfArguments = 13;
  #else
  const int NumberOfArguments = 12;
  #endif

  if (argc!=NumberOfArguments) {
    std::cerr << "Usage: ./dem-xxx grid_h_max particle_diam_max particle_diam_min scenario no-of-time-steps grid-type time-step-size plot gravity collision-model [core-count]" << std::endl
              << std::endl
              << " grid_h_max          maximum mesh width of grid" << std::endl
              << " particle_diam_max   maximal diameter of particles" << std::endl
              << " particle_diam_min   minimal diameter of particles" << std::endl
              << " scenario            which scenario to use. See list below for valid configurations " << std::endl
              << " no-of-iterations    number of iterations or time steps depending on scheme" << std::endl
              << " grid-type           which grid type to use. See list below for valid configurations " << std::endl
              << " time-step-size      floating point number" << std::endl
              << " plot                see plot variants below" << std::endl
              << " gravity             floating point number" << std::endl
              << " collision-model     choose from sphere, bf, penalty, penaltyStat, hybrid-on-triangle-pairs, hybrid-on-batches, hybrid-on-triangle-pairsStats, hybrid-on-batchesStats, gjk, add sphere- for sphere check" << std::endl
              << " [core-count]        only required if you have translated the code with TBBs" << std::endl
              << std::endl << std::endl << std::endl << std::endl
              << "Scenarios" << std::endl
              << "=========" << std::endl
              << "  black-hole-with-aligned-cubes" << std::endl
              << "  black-hole-with-cubes" << std::endl
              << "  black-hole" << std::endl
              << "  random-velocities-with-aligned-cubes" << std::endl
              << "  random-velocities-with-cubes" << std::endl
              << "  random-velocities" << std::endl
              << "  two-particles-crash" << std::endl
			  << "  hopper" << std::endl
			  << "  hopper100" << std::endl
			  << "  hopperBrick" << std::endl
			  << "  freefall" << std::endl
			  << "  flatwall" << std::endl
			  << "  icecube" << std::endl
			  << "  friction" << std::endl
			  << "  frictionSlide" << std::endl
			  << "  twoBricksCrash" << std::endl
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
			  << "  every-checkpoint" << std::endl;

    return -1;
  }

  const double       gridHMax            = atof(argv[1]);
  const double       particleDiamMax     = atof(argv[2]);
  const double       particleDiamMin     = atof(argv[3]);
  const std::string  scenario            = argv[4];
  const int          numberOfTimeSteps   = atoi(argv[5]);
  const std::string  gridTypeIdentifier  = argv[6];
  const double       iterations        = atof(argv[7]);
  const std::string  plotIdentifier      = argv[8];
  const double       gravity             = atof(argv[9]);
  const std::string  collisionModel      = argv[10];
  const double		 realSnapshot		 = atof(argv[11]);
  #ifdef SharedMemoryParallelisation
  const int          numberOfCores       = atoi(argv[12]);
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
    gridType = dem::mappings::CreateGrid::RegularGrid;
  }
  else if (gridTypeIdentifier=="adaptive-grid") {
    gridType = dem::mappings::CreateGrid::AdaptiveGrid;
  }
  else if (gridTypeIdentifier=="reluctant-adaptive-grid") {
    gridType = dem::mappings::CreateGrid::ReluctantAdaptiveGrid;
  }
  else {
    std::cerr << gridTypeIdentifier << " is not a valid grid type. Please run without arguments to see list of valid grid types" << std::endl;
    programExitCode = 2;
  }

  if (scenario=="black-hole-with-cubes") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::BlackHoleWithCubes, gridHMax, particleDiamMax, particleDiamMin, gridType, epsilon);
  }
  else if (scenario=="black-hole-with-aligned-cubes") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::BlackHoleWithAlignedCubes, gridHMax, particleDiamMax, particleDiamMin, gridType, epsilon);
  }
  else if (scenario=="black-hole") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::BlackHole, gridHMax, particleDiamMax, particleDiamMin, gridType, epsilon);
  }
  else if (scenario=="random-velocities-with-aligned-cubes") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::RandomWithAlignedCubes, gridHMax, particleDiamMax, particleDiamMin, gridType, epsilon);
  }
  else if (scenario=="random-velocities-with-cubes") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::RandomWithCubes, gridHMax, particleDiamMax, particleDiamMin, gridType, epsilon);
  }
  else if (scenario=="random-velocities") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::Random, gridHMax, particleDiamMax, particleDiamMin, gridType, epsilon);
  }
  else if (scenario=="two-particles-crash") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::TwoParticlesCrash, gridHMax, particleDiamMax, particleDiamMin, gridType, epsilon);
  }
  else if (scenario=="hopper") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopper, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="hopper300") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopper, 0.11, 0.11, 0.11, gridType, epsilon);
  }
  else if (scenario=="hopperBricks") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperBricks, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="hopperKeys") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::hopperKeys, 0.10, 0.10, 0.10, gridType, epsilon);
  }
  else if (scenario=="freefall") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::freefall, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="freefallBricks") {
     dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::freefallBricks, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="freefallKeys") {
     dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::freefallKeys, 0.15, 0.15, 0.15, gridType, epsilon);
   }
  else if (scenario=="flatwall") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::flatwall, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="icecube"){
	dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::icecube, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="friction") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::friction, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="frictionSlide") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::frictionSlide, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="domino") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::domino, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="stability") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::stability, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else if (scenario=="sla") {
    dem::mappings::CreateGrid::setScenario(dem::mappings::CreateGrid::sla, 0.15, 0.15, 0.15, gridType, epsilon);
  }
  else {
    std::cerr << "not a valid scenario. Please run without arguments to see list of valid scenarios" << std::endl;
    programExitCode = 2;
  }

  dem::runners::Runner::Plot plot;
  if (plotIdentifier=="never") {
    plot = dem::runners::Runner::Never;
  }
  else if (plotIdentifier=="every-iteration") {
    plot = dem::runners::Runner::EveryIteration;
  }
  else if (plotIdentifier=="upon-change") {
    plot = dem::runners::Runner::UponChange;
  }
  else if (plotIdentifier=="every-batch") {
	plot = dem::runners::Runner::EveryBatch;
  }
  else if (plotIdentifier=="adaptive") {
	plot = dem::runners::Runner::Adaptive;
  }
  else {
    std::cerr << "invalid plot option. Please run without arguments to see list of valid plot variants" << std::endl;
    programExitCode = 2;
  }

  if (collisionModel=="sphere") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::Sphere;
  }else if (collisionModel=="bf") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::BruteForce;
  }
  else if (collisionModel=="penalty") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::Penalty;
  }
  else if (collisionModel=="penaltyStat") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::PenaltyStat;
  }
  else if (collisionModel=="hybrid-on-triangle-pairs") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::HybridOnTrianglePairs;
  }
  else if (collisionModel=="hybrid-on-batches") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::HybridOnBatches;
  }
  else if (collisionModel=="hybrid-on-triangle-pairsStats") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::HybridOnTrianglePairs;
  }
  else if (collisionModel=="hybrid-on-batchesStats") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::HybridOnBatches;
  }
  else if (collisionModel=="sphere-hybrid-on-triangle-pairs") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::SphereHybridOnTrianglePairs;
    dem::mappings::Collision::_enableOverlapCheck = true;
  }
  else if (collisionModel=="sphere-hybrid-on-batches") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::SphereHybridOnBatches;
    dem::mappings::Collision::_enableOverlapCheck = true;
  }
  else if (collisionModel=="sphere-bf") {
  	dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::SphereBruteForce;
  	dem::mappings::Collision::_enableOverlapCheck = true;
  }
  else if (collisionModel=="sphere-penalty") {
  	dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::SpherePenalty;
  	dem::mappings::Collision::_enableOverlapCheck = true;
  }
  else if (collisionModel=="gjk") {
    dem::mappings::Collision::_collisionModel = dem::mappings::Collision::CollisionModel::GJK;
  }
  else {
    std::cerr << "invalid collision model option. Please run without arguments to see list of valid variants" << std::endl;
    programExitCode = 2;
  }

  dem::mappings::MoveParticles::gravity	= (gravity>0) ? 9.81 : 0.0;

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
    programExitCode = runner.run( numberOfTimeSteps, plot, gridType, numberOfCores, iterations, realSnapshot);
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
