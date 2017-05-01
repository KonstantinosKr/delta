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
  _stateData.setNumberOfParticleComparisons(0.0);
  _stateData.setTwoParticlesAreClose(false);
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

double dem::State::getNumberOfParticleComparisons() const {
  return _stateData.getNumberOfParticleComparisons();
}

void dem::State::incNumberOfContactPoints(int delta) {
  _stateData.setNumberOfContactPoints( _stateData.getNumberOfContactPoints() + delta );
}

void dem::State::decNumberOfContactPoints(int delta) {
  _stateData.setNumberOfContactPoints( _stateData.getNumberOfContactPoints() - delta );
}

void dem::State::incNumberOfParticleReassignments(int delta) {
  _stateData.setNumberOfParticleReassignments( _stateData.getNumberOfParticleReassignments() + delta );
}

void dem::State::incNumberOfTriangleComparisons(int delta) {
  _stateData.setNumberOfTriangleComparisons( _stateData.getNumberOfTriangleComparisons() + delta );
}

void dem::State::incNumberOfParticleComparisons(int delta) {
  _stateData.setNumberOfParticleComparisons( _stateData.getNumberOfParticleComparisons() + delta );
}

void dem::State::merge( const State& otherState ) {
  _stateData.setNumberOfContactPoints( _stateData.getNumberOfContactPoints() + otherState._stateData.getNumberOfContactPoints() );
  _stateData.setNumberOfParticleReassignments( _stateData.getNumberOfParticleReassignments() + otherState._stateData.getNumberOfParticleReassignments() );
  _stateData.setNumberOfTriangleComparisons( _stateData.getNumberOfTriangleComparisons() + otherState._stateData.getNumberOfTriangleComparisons() );
  _stateData.setNumberOfParticleComparisons( _stateData.getNumberOfParticleComparisons() + otherState._stateData.getNumberOfParticleComparisons() );
}

double dem::State::getTimeStepSize() const {
  return _stateData.getTimeStepSize();
}

double dem::State::getTime() const {
  return _stateData.getCurrentTime();
}

void dem::State::setInitialTimeStepSize(double value) {
  _stateData.setTimeStepSize(value);
  _stateData.setCurrentTime(0.0);
}

void dem::State::informStateThatTwoParticlesAreClose() {
  _stateData.setTwoParticlesAreClose(true);
}

void dem::State::informStatePenetration(){
  _stateData.setPenetrationOccured(true);
}

bool dem::State::getPenetrationStatus(){
  return _stateData.getPenetrationOccured();
}

void dem::State::finishedTimeStep(double initStep) {
  _stateData.setCurrentTime(_stateData.getCurrentTime() + _stateData.getTimeStepSize());
  if (_stateData.getTwoParticlesAreClose())
  {
	  if(_stateData.getTimeStepSize() < 1E-5) return;
	  _stateData.setTimeStepSize(_stateData.getTimeStepSize()/2.0);
  } else {//replace with max global step
	  if(_stateData.getTimeStepSize() > initStep) return;
	  _stateData.setTimeStepSize(_stateData.getTimeStepSize()*1.01);
  }
}

void dem::State::incNumberOfParticles(int delta) {
	_stateData.setNumberOfParticles(_stateData.getNumberOfParticles()+delta);
}

void dem::State::incNumberOfObstacles(int delta) {
	_stateData.setNumberOfObstacles(_stateData.getNumberOfObstacles()+delta);
}

int dem::State::getNumberOfParticles() const {
	return _stateData.getNumberOfParticles();
}

int dem::State::getNumberOfObstacles() const {
	return _stateData.getNumberOfObstacles();
}

void dem::State::setMinimumMeshWidth(double minwidth) {
  _stateData.setMinimumMeshWidth(minwidth);
}

void dem::State::setMaximumMeshWidth(double maxwidth) {
  _stateData.setMaximumMeshWidth(maxwidth);
}

double dem::State::getMinimumMeshWidth() const {
  return _stateData.getMinimumMeshWidth();
}

double dem::State::getMaximumMeshWidth() const {
  return _stateData.getMaximumMeshWidth();
}
