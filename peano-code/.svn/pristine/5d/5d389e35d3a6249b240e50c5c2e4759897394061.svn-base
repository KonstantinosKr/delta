#include "multigrid/State.h"
#include "multigrid/Cell.h"
#include "multigrid/Vertex.h"

#include "peano/grid/Checkpoint.h"



multigrid::State::State():
  Base() { 
}


multigrid::State::State(const Base::PersistentState& argument):
  Base(argument) {
}


void multigrid::State::writeToCheckpoint( peano::grid::Checkpoint<multigrid::Vertex,multigrid::Cell>& checkpoint ) const {
  // @todo Insert your code here
}

    
void multigrid::State::readFromCheckpoint( const peano::grid::Checkpoint<multigrid::Vertex,multigrid::Cell>& checkpoint ) {
  // @todo Insert your code here
}


void multigrid::State::clearAccumulatedAttributes() {
  _stateData.setResidual2Norm(0.0);
  _stateData.setResidualMaxNorm(0.0);
  _stateData.setSolutionL2Norm(0.0);
  _stateData.setSolutionMaxNorm(0.0);
  _stateData.setNumberOfStencilUpdates(0.0);
}


void multigrid::State::merge(const State& state) {
  _stateData.setResidual2Norm(          _stateData.getResidual2Norm()          + state._stateData.getResidual2Norm() );
  _stateData.setSolutionL2Norm(         _stateData.getSolutionL2Norm()         + state._stateData.getSolutionL2Norm() );

  _stateData.setResidualMaxNorm(        std::max( _stateData.getResidualMaxNorm(), state._stateData.getResidualMaxNorm() ) );
  _stateData.setSolutionMaxNorm(        std::max( _stateData.getSolutionMaxNorm(), state._stateData.getSolutionMaxNorm() ) );

  _stateData.setNumberOfStencilUpdates( _stateData.getNumberOfStencilUpdates() + state._stateData.getNumberOfStencilUpdates() );
}


double multigrid::State::getResidualIn2Norm() const {
  return std::sqrt( _stateData.getResidual2Norm() );
}


double multigrid::State::getResidualInMaxNorm() const {
  return _stateData.getResidual2Norm();
}


double multigrid::State::getSolutionIn2Norm() const {
  return std::sqrt( _stateData.getSolutionL2Norm() );
}


double multigrid::State::getSolutionInMaxNorm() const {
  return _stateData.getSolutionMaxNorm();
}


double multigrid::State::getNumberOfStencilUpdates() const {
  return _stateData.getNumberOfStencilUpdates();
}


void multigrid::State::notifyAboutFineGridVertexUpdate( double res, double newU, const tarch::la::Vector<DIMENSIONS,double>&  h) {
  _stateData.setResidual2Norm( _stateData.getResidual2Norm() + res*res );

  _stateData.setResidualMaxNorm( std::max( _stateData.getResidualMaxNorm(), std::abs(res) ) );

  _stateData.setSolutionL2Norm( _stateData.getSolutionL2Norm() + newU*newU*tarch::la::volume(h) );

  _stateData.setSolutionMaxNorm( std::max( _stateData.getSolutionMaxNorm(), std::abs(newU) ) );

  _stateData.setNumberOfStencilUpdates( _stateData.getNumberOfStencilUpdates() + 1 );
}


void multigrid::State::setMultiplicativeMultigridPhase(MultigridPhase phase) {
  switch (phase) {
    case Init:
      _stateData.setActiveLevel( getMaxLevel() );
      _stateData.setOldActiveLevel( getMaxLevel() );
      break;
    case Smooth:
      _stateData.setOldActiveLevel( _stateData.getActiveLevel() );
      break;
    case SmoothAndRestrict:
      _stateData.setOldActiveLevel( _stateData.getActiveLevel() );
      _stateData.setActiveLevel( _stateData.getActiveLevel()-1 );
      break;
    case SmoothAndProlong:
      _stateData.setOldActiveLevel( _stateData.getActiveLevel() );
      _stateData.setActiveLevel( _stateData.getActiveLevel()+1 );
      break;
  }
}


void multigrid::State::incNumberOfStencilEvaluations() {
  _stateData.setNumberOfStencilUpdates( _stateData.getNumberOfStencilUpdates() + 1 );
}


int multigrid::State::getActiveSmoothingLevel() const {
  return _stateData.getActiveLevel();
}


int multigrid::State::getOldActiveSmoothingLevel() const {
  return _stateData.getOldActiveLevel();
}
