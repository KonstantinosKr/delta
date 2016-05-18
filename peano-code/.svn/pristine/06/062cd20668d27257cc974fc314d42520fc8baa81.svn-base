#include "multigrid/runners/Runner.h"


#include "multigrid/repositories/Repository.h"
#include "multigrid/repositories/RepositoryFactory.h"

#include "peano/utils/UserInterface.h"

#include "tarch/Assertions.h"

#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"


tarch::logging::Log  multigrid::runners::Runner::_log( "multigrid::runners::Runner" );


// @todo Remove this include as soon as you've created your real-world geometry
#include "peano/geometry/Hexahedron.h" 


multigrid::runners::Runner::Runner() {
  // @todo Insert your code here
}


multigrid::runners::Runner::~Runner() {
  // @todo Insert your code here
}


int multigrid::runners::Runner::run() {
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
  multigrid::repositories::Repository* repository = 
    multigrid::repositories::RepositoryFactory::getInstance().createWithSTDStackImplementation(
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


int multigrid::runners::Runner::runAsMaster(multigrid::repositories::Repository& repository) {
  peano::utils::UserInterface userInterface;
  userInterface.writeHeader();

  // @todo Insert your code here
  
  // Start of dummy implementation
  
  repository.switchToCreateGrid(); repository.iterate();

  repository.switchToJacobiAndPlot();
  for (int i=0; i<100; i++) {
    repository.getState().clearAccumulatedAttributes();
    repository.iterate();

    logInfo(
      "runAsMaster(...)",
      "#vertices=" << repository.getState().getNumberOfInnerLeafVertices() <<
      ",|res|_2=" << repository.getState().getResidualIn2Norm() <<
      ",|res|_max=" << repository.getState().getResidualInMaxNorm() <<
      ",|u|_L2=" << repository.getState().getSolutionIn2Norm() <<
      ",|u|_max=" << repository.getState().getSolutionInMaxNorm() <<
      ",#stencil-updates=" << repository.getState().getNumberOfStencilUpdates()
    );
  }
 
 
  repository.logIterationStatistics();
  repository.terminate();
  // End of dummy implementation

  return 0;
}
