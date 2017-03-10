// This file originally was created by pdt (Peano Development Toolkit) as part
// of a code based upon the Peano project by Tobias Weinzierl. For conditions 
// of distribution and use of this project, please see the copyright notice at
// www.peano-framework.org. Feel free to adopt the license and authorship of 
// this file and your project to your needs as long as the license is in 
// agreement with the original Peano user constraints. A reference to/citation  
// of  Peano and its author is highly appreciated.
#ifndef _DEM_RUNNERS_RUNNER_H_ 
#define _DEM_RUNNERS_RUNNER_H_ 


#include "tarch/logging/Log.h"
#include "dem/mappings/CreateGrid.h"


namespace dem {
  namespace runners {
    class Runner;
  }

  namespace repositories {
    class Repository;
  }
}

/**
 * Runner
 *
 */
class dem::runners::Runner {
  public:
    enum Plot {
      EveryIteration,
      Never,
      UponChange,
	  EveryBatch,
	  Adaptive,
	  Range,
	  Track
    };
  private:
    static tarch::logging::Log _log;

    int runAsMaster(dem::repositories::Repository& repository, int numberOfTimeSteps, Plot plot, dem::mappings::CreateGrid::GridType gridType, double timeStepSize, double realSnapshot);
    
    #ifdef Parallel
    int runAsWorker(dem::repositories::Repository& repository);
    
    /**
     * If the master node calls runGlobalStep() on the repository, all MPI 
     * ranks besides rank 0 invoke this operation no matter whether they are 
     * idle or not. Hence, please call this operation manually within 
     * runAsMaster() if you require the master to execute the same function 
     * as well.
     */ 
    void runGlobalStep();
    #endif
  public:
    Runner();
    virtual ~Runner();

    /**
     * Run
     */
    int run(int numberOfTimeSteps, Plot plot, dem::mappings::CreateGrid::GridType gridType, int tbbThreads, double initialTimeStepSize, double realSnapshot);
};

#endif
