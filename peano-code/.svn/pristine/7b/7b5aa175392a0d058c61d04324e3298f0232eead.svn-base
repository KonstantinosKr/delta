#include "myproject/runners/Runner.h"


#include "myproject/repositories/Repository.h"
#include "myproject/repositories/RepositoryFactory.h"

#include "peano/utils/UserInterface.h"

#include "tarch/Assertions.h"

#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"


// @todo Remove this include as soon as you've created your real-world geometry
#include "peano/geometry/Hexahedron.h" 


myproject::runners::Runner::Runner() {
  // @todo Insert your code here
}


myproject::runners::Runner::~Runner() {
  // @todo Insert your code here
}


int myproject::runners::Runner::run() {
  // @todo Insert your geometry generation here and adopt the repository 
  //       generation to your needs. There is a dummy implementation to allow 
  //       for a quick start, but this is really very dummy (it generates 
  //       solely a sphere computational domain and basically does nothing with 
  //       it).
  
  // Start of dummy implementation
  peano::geometry::Hexahedron geometry(
    tarch::la::Vector<DIMENSIONS,double>(1.0),
    tarch::la::Vector<DIMENSIONS,double>(0.0)
   );
  myproject::repositories::Repository* repository = 
    myproject::repositories::RepositoryFactory::getInstance().createWithSTDStackImplementation(
      geometry,
      tarch::la::Vector<DIMENSIONS,double>(1.0),   // domainSize,
      tarch::la::Vector<DIMENSIONS,double>(0.0)    // computationalDomainOffset
    );
  // End of dummy implementation
  
  int result = 0;
  if (tarch::parallel::Node::getInstance().isGlobalMaster()) {
    result = runAsMaster( *repository );
  }
  #ifdef Parallel
  else {
    result = runAsWorker( *repository );
  }
  #endif
  
  delete repository;
  
  return result;
}


int myproject::runners::Runner::runAsMaster(myproject::repositories::Repository& repository) {
  peano::utils::UserInterface userInterface;
  userInterface.writeHeader();

  repository.switchToCreateGridAndPlot();
  repository.iterate();
  
  //repository.getState().setTimeStepSize( 1e-8 );
  repository.getState().setTimeStepSize( 0.5e-7 );
  for (int i=0; i<10000; i++) {
    if (i%100==0) {
      repository.switchToTimeStepAndPlot();
    }
    else {
     repository.switchToTimeStep();
    }
    repository.iterate();
  }
 
  repository.logIterationStatistics();
  repository.terminate();

  return 0;
}
