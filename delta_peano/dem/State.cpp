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
  _stateData.setTwoParticlesAreClose(0.0);
  _stateData.setMaxVelocityApproach(_stateData.getMaxVelocityTravel());
  _stateData.setMaxVelocityTravel(0.0);
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
  _stateData.setNumberOfParticleComparisons( _stateData.getNumberOfParticleComparisons() + otherState._stateData.getNumberOfParticleComparisons() );
  _stateData.setNumberOfTriangleComparisons( _stateData.getNumberOfTriangleComparisons() + otherState._stateData.getNumberOfTriangleComparisons() );

  if(_stateData.getTwoParticlesAreClose() > otherState._stateData.getTwoParticlesAreClose())
  {
    _stateData.setTwoParticlesAreClose( _stateData.getTwoParticlesAreClose());
  } else {
    _stateData.setTwoParticlesAreClose( otherState._stateData.getTwoParticlesAreClose());
  }

  if(_stateData.getMaxVelocityApproach() > otherState._stateData.getMaxVelocityApproach())
  {
    _stateData.setMaxVelocityApproach( _stateData.getMaxVelocityApproach());
  } else {
    _stateData.setMaxVelocityApproach( otherState._stateData.getMaxVelocityApproach());
  }

  if(_stateData.getMaxVelocityTravel() > otherState._stateData.getMaxVelocityTravel())
  {
    _stateData.setMaxVelocityTravel( _stateData.getMaxVelocityTravel());
  } else {
    _stateData.setMaxVelocityTravel( otherState._stateData.getMaxVelocityTravel());
  }
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

void dem::State::informStateThatTwoParticlesAreClose(double decrementFactor) {
  _stateData.setTwoParticlesAreClose(decrementFactor);
}

void dem::State::finishedTimeStep(double initialTimestep) {
  _stateData.setCurrentTime(_stateData.getCurrentTime() + _stateData.getTimeStepSize());

  //if(initialTimestep <= 0)
  {
    adaptiveTimeStep();
  }
}

void dem::State::adaptiveTimeStep()
{
  const double increaseFactor = 1.1;
  _stateData.setStepIncrement(increaseFactor);

  const double mindt = 1E-4;
  double maxdt = _stateData.getMinMeshWidth()(0)/(2.0 * increaseFactor * _stateData.getMaxVelocityApproach());

  if(_stateData.getTwoParticlesAreClose() > 0.0) { //approach

    if(_stateData.getTimeStepSize() > mindt) //critical approach
    {
      double decrementFactor = _stateData.getTwoParticlesAreClose();
      //printf("Decrease Factor: %f\n", decrementFactor);
      _stateData.setTimeStepSize(decrementFactor);
    }
  } else {
    if(_stateData.getTimeStep() > 2)
    _stateData.setTimeStepSize(increaseFactor * _stateData.getTimeStepSize());
  }

  if(_stateData.getTimeStepSize() > maxdt) {
   _stateData.setTimeStepSize(maxdt);
  }

  if(maxdt > 1.0)
    maxdt = 1.0;
}

void dem::State::setTimeStep(int number) {//name
  _stateData.setTimeStep(number);
}

int dem::State::getTimeStep() {
  return _stateData.getTimeStep();
}

void dem::State::setStepIncrement(int number) {//name
  _stateData.setStepIncrement(number);
}

int dem::State::getStepIncrement() {
  return _stateData.getStepIncrement();
}

void dem::State::incNumberOfParticles(int delta) {
	_stateData.setNumberOfParticles(_stateData.getNumberOfParticles()+delta);
}

void dem::State::incNumberOfObstacles(int delta) {
	_stateData.setNumberOfObstacles(_stateData.getNumberOfObstacles()+delta);
}

void dem::State::setMaximumVelocityApproach(double v)
{
  _stateData.setMaxVelocityApproach(v);
}

double dem::State::getMaximumVelocityApproach()
{
  return _stateData.getMaxVelocityApproach();
}

void dem::State::setMaximumVelocityTravel(double v)
{
  _stateData.setMaxVelocityTravel(v);
}

double dem::State::getMaximumVelocityTravel()
{
  return _stateData.getMaxVelocityTravel();
}

int dem::State::getNumberOfParticles() const {
	return _stateData.getNumberOfParticles();
}

int dem::State::getNumberOfObstacles() const {
	return _stateData.getNumberOfObstacles();
}

void dem::State::setPrescribedMinimumMeshWidth(double minwidth) {
  _stateData.setPrescribedMinimumMeshWidth(minwidth);
}

void dem::State::setPrescribedMaximumMeshWidth(double maxwidth) {
  _stateData.setPrescribedMaximumMeshWidth(maxwidth);
}

double dem::State::getPrescribedMinimumMeshWidth() const {
  return _stateData.getPrescribedMinimumMeshWidth();
}

double dem::State::getPrescribedMaximumMeshWidth() const {
  return _stateData.getPrescribedMaximumMeshWidth();
}
