#include "multigrid/runners/Runner.h"


#include "multigrid/repositories/Repository.h"
#include "multigrid/repositories/RepositoryFactory.h"

#include "peano/utils/UserInterface.h"

#include "tarch/Assertions.h"

#include "tarch/parallel/Node.h"
#include "tarch/parallel/NodePool.h"


// @todo Remove this include as soon as you've created your real-world geometry
#include "peano/geometry/Hexahedron.h" 


tarch::logging::Log  multigrid::runners::Runner::_log( "multigrid::runners::Runner" );


multigrid::runners::Runner::Runner() {
  // @todo Insert your code here
}


multigrid::runners::Runner::~Runner() {
  // @todo Insert your code here
}


int multigrid::runners::Runner::run(Solver solver) {
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
    result = runAsMaster( *repository, solver );
  }
  #ifdef Parallel
  else {
    result = runAsWorker( *repository );
  }
  #endif
  
  delete repository;
  
  return result;
}


void multigrid::runners::Runner::runJacobi(multigrid::repositories::Repository& repository) {
  double oldResidualIn2Norm   = 1.0;
  double oldResidualInMaxNorm = 1.0;
  double totalNumberOfStencilUpdates = 0.0;
  #if defined(Asserts) || defined(Debug)
  repository.switchToJacobiAndPlot();
  #else
  repository.switchToJacobi();
  #endif
  for (int i=0; i<100; i++) {
    repository.getState().clearAccumulatedAttributes();
    repository.iterate();

    totalNumberOfStencilUpdates += repository.getState().getNumberOfStencilUpdates();

    logInfo(
      "runAsMaster(...)",
      "#vertices=" << repository.getState().getNumberOfInnerLeafVertices() <<
      ",|res|_2=" << repository.getState().getResidualIn2Norm() <<
      ",|res|_max=" << repository.getState().getResidualInMaxNorm() <<
      ",|u|_L2=" << repository.getState().getSolutionIn2Norm() <<
      ",|u|_max=" << repository.getState().getSolutionInMaxNorm() <<
      ",#stencil-updates=" << repository.getState().getNumberOfStencilUpdates() <<
      ",rho_2=" << (repository.getState().getResidualIn2Norm()/oldResidualIn2Norm) <<
      ",|res|_max=" << (repository.getState().getResidualInMaxNorm()/oldResidualInMaxNorm) <<
      ",#total-stencil-updates=" << totalNumberOfStencilUpdates
    );

    oldResidualIn2Norm   = repository.getState().getResidualIn2Norm();
    oldResidualInMaxNorm = repository.getState().getResidualInMaxNorm();
  }
}


void multigrid::runners::Runner::runAdditiveMG(multigrid::repositories::Repository& repository) {
  double oldResidualIn2Norm   = 1.0;
  double oldResidualInMaxNorm = 1.0;
  int    iterations           = 0;
  double rhoL2                = 1.0;
  double rhoMax               = 1.0;
  double totalNumberOfStencilUpdates = 0.0;
  #if defined(Asserts) || defined(Debug)
  repository.switchToAdditiveMGAndPlot();
  #else
  repository.switchToAdditiveMG();
  #endif
  while (oldResidualIn2Norm>1e-12 && iterations<1000 && (rhoL2<2.0 || rhoMax<2.0 ) ) {
    repository.getState().clearAccumulatedAttributes();
    repository.iterate();

    rhoL2  = repository.getState().isGridStationary() ? (repository.getState().getResidualIn2Norm()/oldResidualIn2Norm)     : 0.0;
    rhoMax = repository.getState().isGridStationary() ? (repository.getState().getResidualInMaxNorm()/oldResidualInMaxNorm) : 0.0;

    totalNumberOfStencilUpdates += repository.getState().getNumberOfStencilUpdates();

    logInfo(
      "runAsMaster(...)",
      "#vertices=" << repository.getState().getNumberOfInnerLeafVertices() <<
      ",|res|_2=" << repository.getState().getResidualIn2Norm() <<
      ",|res|_max=" << repository.getState().getResidualInMaxNorm() <<
      ",|u|_L2=" << repository.getState().getSolutionIn2Norm() <<
      ",|u|_max=" << repository.getState().getSolutionInMaxNorm() <<
      ",#stencil-updates=" << repository.getState().getNumberOfStencilUpdates() <<
      ",rho_2=" << rhoL2 <<
      ",|res|_max=" << rhoMax <<
      ",level_max=" << repository.getState().getMaxLevel() <<
      ",#total-stencil-updates=" << totalNumberOfStencilUpdates
    );

    oldResidualIn2Norm   = repository.getState().getResidualIn2Norm();
    oldResidualInMaxNorm = repository.getState().getResidualInMaxNorm();
    iterations++;
  }
}


void multigrid::runners::Runner::runMultiplicativeMG(multigrid::repositories::Repository& repository, int preSmoothingSteps, int postSmoothingSteps) {
  double oldResidualIn2Norm   = 1.0;
  double oldResidualInMaxNorm = 1.0;
  int    iterations           = 0;
  double rhoL2                = 1.0;
  double rhoMax               = 1.0;
  double totalNumberOfStencilUpdates = 0.0;

  repository.switchToMultiplicativeMG();

  repository.getState().setMultiplicativeMultigridPhase(State::Init);

  const int MaxIterations = 1000; // 1000
  repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::Smooth);
  logDebug( "runMultiplicativeMG()","start smoothing" );
  while (oldResidualIn2Norm>1e-12 && iterations<MaxIterations && (rhoL2<2.0 || rhoMax<2.0 ) ) {
    while (repository.getState().getActiveSmoothingLevel()>=2) {
      repository.getState().clearAccumulatedAttributes();
      logDebug( "runMultiplicativeMG()","state=" <<  repository.getState().toString() );
      repository.iterate();
      totalNumberOfStencilUpdates += repository.getState().getNumberOfStencilUpdates();
      for (int i=0; i<preSmoothingSteps-1; i++) {
        repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::Smooth);
        logDebug( "runMultiplicativeMG()","switch to smooth" );
        repository.getState().clearAccumulatedAttributes();
        logDebug( "runMultiplicativeMG()","state=" <<  repository.getState().toString() );
        repository.iterate();
        totalNumberOfStencilUpdates += repository.getState().getNumberOfStencilUpdates();
      }
      repository.getState().setMultiplicativeMultigridPhase( State::MultigridPhase::SmoothAndRestrict );
      logDebug( "runMultiplicativeMG()","switch to smooth and restrict" );
    }
    logDebug( "runMultiplicativeMG()","cancel last switch and then switch to smooth" );
    repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::SmoothAndProlong);
    repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::Smooth);
    while (repository.getState().getActiveSmoothingLevel()<=repository.getState().getMaxLevel()) {
      #if defined(Asserts) || defined(Debug)
      if (repository.getState().getActiveSmoothingLevel()==repository.getState().getMaxLevel()) {
        repository.switchToMultiplicativeMGAndPlot();
      }
      else
      #else
      repository.switchToMultiplicativeMG();
      #endif

      repository.getState().clearAccumulatedAttributes();
      logDebug( "runMultiplicativeMG()","state=" <<  repository.getState().toString() );
      repository.iterate();
      totalNumberOfStencilUpdates += repository.getState().getNumberOfStencilUpdates();

      for (int i=0; i<postSmoothingSteps-1; i++) {
        repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::Smooth);
        logDebug( "runMultiplicativeMG()","switch to smooth" );
        repository.getState().clearAccumulatedAttributes();
        logDebug( "runMultiplicativeMG()","state=" <<  repository.getState().toString() );
        repository.iterate();
        totalNumberOfStencilUpdates += repository.getState().getNumberOfStencilUpdates();
      }


      repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::SmoothAndProlong);
      logDebug( "runMultiplicativeMG()","switch to smooth and prolong" );
    }

    logDebug( "runMultiplicativeMG()","cancel last switch and then switch to smooth" );
    repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::SmoothAndRestrict);
    repository.getState().setMultiplicativeMultigridPhase(State::MultigridPhase::Smooth);

    rhoL2  = repository.getState().isGridStationary() ? (repository.getState().getResidualIn2Norm()/oldResidualIn2Norm)     : 0.0;
    rhoMax = repository.getState().isGridStationary() ? (repository.getState().getResidualInMaxNorm()/oldResidualInMaxNorm) : 0.0;

    totalNumberOfStencilUpdates += repository.getState().getNumberOfStencilUpdates();

    logInfo(
      "runAsMaster(...)",
      "#vertices=" << repository.getState().getNumberOfInnerLeafVertices() <<
      ",|res|_2=" << repository.getState().getResidualIn2Norm() <<
      ",|res|_max=" << repository.getState().getResidualInMaxNorm() <<
      ",|u|_L2=" << repository.getState().getSolutionIn2Norm() <<
      ",|u|_max=" << repository.getState().getSolutionInMaxNorm() <<
      ",rho_2=" << rhoL2 <<
      ",|res|_max=" << rhoMax <<
      ",level_max=" << repository.getState().getMaxLevel() <<
      ",#total-stencil-updates=" << totalNumberOfStencilUpdates
    );

    oldResidualIn2Norm   = repository.getState().getResidualIn2Norm();
    oldResidualInMaxNorm = repository.getState().getResidualInMaxNorm();

    iterations++;
  }
}

int multigrid::runners::Runner::runAsMaster(multigrid::repositories::Repository& repository, Solver solver) {
  peano::utils::UserInterface userInterface;
  userInterface.writeHeader();

  repository.switchToCreateGrid(); repository.iterate();

  switch (solver) {
    case Jacobi:
      runJacobi(repository);
      break;
    case AdditiveMG:
      runAdditiveMG(repository);
      break;
    case MultiplicativeV11:
      runMultiplicativeMG(repository, 1, 1);
      break;
    case MultiplicativeV12:
      runMultiplicativeMG(repository, 1, 2);
      break;
    case MultiplicativeV22:
      runMultiplicativeMG(repository, 2, 1);
      break;
    case MultiplicativeV21:
      runMultiplicativeMG(repository, 2, 1);
      break;
    case MultiplicativeV31:
      runMultiplicativeMG(repository, 3, 1);
      break;
    case MultiplicativeV32:
      runMultiplicativeMG(repository, 3, 2);
      break;
    case MultiplicativeV33:
      runMultiplicativeMG(repository, 3, 3);
      break;
    case MultiplicativeV23:
      runMultiplicativeMG(repository, 2, 3);
      break;
    case MultiplicativeV13:
      runMultiplicativeMG(repository, 1, 3);
      break;
    case None:
      assertionMsg( false, "may not happen" );
      break;
  }

  #if !defined(Asserts) && !defined(Debug)
  repository.switchToPlot();
  repository.iterate();
  #endif

  repository.logIterationStatistics();
  repository.terminate();
  // End of dummy implementation

  return 0;
}
