#include "tarch/logging/Log.h"
#include "tarch/tests/TestCaseRegistry.h"
#include "tarch/logging/CommandLineLogger.h"
#include "tarch/parallel/Node.h"

#include "peano/peano.h"

#include "multigrid/runners/Runner.h"
#include "multigrid/mappings/CreateGrid.h"
#include "multigrid/mappings/JacobiSmoother.h"


tarch::logging::Log _log("");


int main(int argc, char** argv) {
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

  int programExitCode = 0;

  if (argc!=3) {
    std::cout << "Usage: ./executable scenario omega" << std::endl
              << std::endl
              << "Valid scenarios:" << std::endl
              <<  "PoissonX" << std::endl
              <<  "AdaptivePoissonX" << std::endl
/*
              <<  "AnisotropicPoisson" << std::endl
              <<  "ShiftedMinimalCheckerboard" << std::endl
              <<  "LayerProblem" << std::endl
              <<  "DiagonalFlow" << std::endl
              <<  "RecirculatingFlow" << std::endl
*/
              << std::endl
              << std::endl
              << "Please replace the X with 2,3,4 or 5. It denotes the maximum depth of the gid."
              << std::endl;
    programExitCode = 1;
  }
  else {
      if (std::string(argv[1])=="Poisson2") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::Poisson2;
      }
      else if (std::string(argv[1])=="Poisson3") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::Poisson3;
      }
      else if (std::string(argv[1])=="Poisson4") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::Poisson4;
      }
      else if (std::string(argv[1])=="Poisson5") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::Poisson5;
      }
      else if (std::string(argv[1])=="AdaptivePoisson2") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::AdaptivePoisson2;
      }
      else if (std::string(argv[1])=="AdaptivePoisson3") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::AdaptivePoisson3;
      }
      else if (std::string(argv[1])=="AdaptivePoisson4") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::AdaptivePoisson4;
      }
      else if (std::string(argv[1])=="AdaptivePoisson5") {
        multigrid::mappings::CreateGrid::_scenario = multigrid::mappings::CreateGrid::AdaptivePoisson5;
      }
      else {
        std::cerr << "invalid scenario. Please run without arguments to see list of supported scenarios" << std::endl;
        programExitCode = 2;
      }

    multigrid::mappings::JacobiSmoother::omega = atof( argv[2] );
  }

  // Configure the output
  tarch::logging::CommandLineLogger::getInstance().clearFilterList();
  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "info", false ) );
  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "debug", true ) );
//  tarch::logging::CommandLineLogger::getInstance().setLogFormat( ... please consult source code documentation );

  // Runs the unit tests
  if (programExitCode==0) {
    tarch::tests::TestCaseRegistry::getInstance().getTestCaseCollection().run();
    programExitCode = tarch::tests::TestCaseRegistry::getInstance().getTestCaseCollection().getNumberOfErrors();
  }

  // dummy call to runner
  if (programExitCode==0) {
    tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "debug", -1, "multigrid", false ) );
    multigrid::runners::Runner runner;
    programExitCode = runner.run();
  }
  
  if (programExitCode==0) {
    #ifdef Parallel
    if (tarch::parallel::Node::getInstance().isGlobalMaster()) {
      logInfo( "main()", "Peano terminates successfully" );
    }
    #else
    logInfo( "main()", "Peano terminates successfully" );
    #endif
  }
  else {
    logInfo( "main()", "quit with error code " << programExitCode );
  }

  peano::shutdownParallelEnvironment();
  peano::shutdownSharedMemoryEnvironment();
  peano::releaseCachedData();

  return programExitCode;
}
