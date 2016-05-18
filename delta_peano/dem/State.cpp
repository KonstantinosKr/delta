#include "dem/State.h"
#include "dem/Cell.h"
#include "dem/Vertex.h"

#include "peano/grid/Checkpoint.h"



dem::State::State():
  Base() { 
  // @todo Insert your code here
}


dem::State::State(const Base::PersistentState& argument):
  Base(argument) {
  // @todo Insert your code here
}


void dem::State::writeToCheckpoint( peano::grid::Checkpoint<dem::Vertex,dem::Cell>& checkpoint ) const {
  // @todo Insert your code here
}

    
void dem::State::readFromCheckpoint( const peano::grid::Checkpoint<dem::Vertex,dem::Cell>& checkpoint ) {
  // @todo Insert your code here
}


void dem::State::clearAccumulatedData() {
  _stateData.setNumberOfContactPoints(0.0);
  _stateData.setNumberOfParticleReassignments(0.0);
  _stateData.setNumberOfTriangleComparisons(0.0);
}


double dem::State::getNumberOfContactPoints() const {
  return _stateData.getNumberOfContactPoints();
}


double dem::State::getNumberOfParticleReassignments() const {
  return _stateData.getNumberOfParticleReassignments();
}


double dem::State::getNumberOfTriangleComparisons() const {
  return _stateData.getNumberOfTriangleComparisons();
}


void dem::State::incNumberOfContactPoints(int delta) {
  _stateData.setNumberOfContactPoints( _stateData.getNumberOfContactPoints() + delta );
}


void dem::State::incNumberOfParticleReassignments(int delta) {
  _stateData.setNumberOfParticleReassignments( _stateData.getNumberOfParticleReassignments() + delta );
}


void dem::State::incNumberOfTriangleComparisons(int delta) {
  _stateData.setNumberOfTriangleComparisons( _stateData.getNumberOfTriangleComparisons() + delta );
}


void dem::State::merge( const State& otherState ) {
  _stateData.setNumberOfContactPoints( _stateData.getNumberOfContactPoints() + otherState._stateData.getNumberOfContactPoints() );
  _stateData.setNumberOfParticleReassignments( _stateData.getNumberOfParticleReassignments() + otherState._stateData.getNumberOfParticleReassignments() );
  _stateData.setNumberOfTriangleComparisons( _stateData.getNumberOfTriangleComparisons() + otherState._stateData.getNumberOfTriangleComparisons() );
}
