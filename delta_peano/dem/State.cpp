#include "dem/State.h"
#include "dem/Cell.h"
#include "dem/Vertex.h"

#include "peano/grid/Checkpoint.h"
#include "dem/mappings/CreateGrid.h"

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
  _stateData.setTwoParticlesSeparate(false);
  _stateData.setMaxVelocityApproach(_stateData.getMaxVelocityTravel());
  _stateData.setMaxVelocityTravel(0.0); //this is required to track travel velocities when there is no collision such that there is a maxdt all the time
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

  _stateData.setTwoParticlesSeparate(_stateData.getTwoParticlesSeparate() || otherState._stateData.getTwoParticlesSeparate() );

  //printf("%f %f\n", _stateData.getTwoParticlesAreClose(), otherState._stateData.getTwoParticlesAreClose());
  if(_stateData.getTwoParticlesAreClose() > 0.0 && otherState._stateData.getTwoParticlesAreClose() > 0.0)
  {
    _stateData.setTwoParticlesAreClose( std::min(_stateData.getTwoParticlesAreClose(), otherState._stateData.getTwoParticlesAreClose()));
  } else if(_stateData.getTwoParticlesAreClose() > 0.0 || otherState._stateData.getTwoParticlesAreClose() > 0.0)
  {
    if(otherState._stateData.getTwoParticlesAreClose() > 0.0 && (otherState._stateData.getTwoParticlesAreClose() < _stateData.getTwoParticlesAreClose() || _stateData.getTwoParticlesAreClose() == 0.0))
    {
      _stateData.setTwoParticlesAreClose( otherState._stateData.getTwoParticlesAreClose());
    }
  }
  //printf("Result - %f %f\n", _stateData.getTwoParticlesAreClose(), otherState._stateData.getTwoParticlesAreClose());

  if(_stateData.getMaxVelocityApproach() < otherState._stateData.getMaxVelocityApproach())
  {
    _stateData.setMaxVelocityApproach( otherState._stateData.getMaxVelocityApproach());
  }

  if(_stateData.getMaxVelocityTravel() < otherState._stateData.getMaxVelocityTravel())
  {
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
  if(value < 0.0) {
   _stateData.setAdaptiveStepSize(true);
    value = value * -1;
  }
  _stateData.setTimeStepSize(value);
  _stateData.setCurrentTime(0.0);
}

void dem::State::informStateThatTwoParticlesAreClose(double decrementFactor) {

  if(decrementFactor < _stateData.getTwoParticlesAreClose() &&  _stateData.getTwoParticlesAreClose() != 0.0)
  {
    _stateData.setTwoParticlesAreClose(decrementFactor);
  } else if(_stateData.getTwoParticlesAreClose() == 0.0 && decrementFactor > 0.0)
  {
    _stateData.setTwoParticlesAreClose(decrementFactor);
  }
}

void dem::State::informStateThatTwoParticlesAreSeparate() {
  _stateData.setTwoParticlesSeparate(true);
}

void dem::State::finishedTimeStep(double initialTimestep) {
  _stateData.setCurrentTime(_stateData.getCurrentTime() + _stateData.getTimeStepSize());

  if(_stateData.getAdaptiveStepSize())
  {
    adaptiveTimeStep();
  }
}

double dem::State::_maxdt;

void dem::State::adaptiveTimeStep()
{
  double increaseFactor = 1.1;
  _stateData.setStepIncrement(increaseFactor);

  const double mindt = 1E-4;
  _maxdt = _stateData.getMinMeshWidth()(0)/(2.0 * increaseFactor * _stateData.getMaxVelocityApproach()) * 0.80;

  if(_maxdt > _stateData.getMaxMeshWidth()(0))
  {
    //_maxdt = _stateData.getMaxMeshWidth()(0);
    //printf("triggered set maxdt : %f\n", _maxdt);
  }

  if(_stateData.getTwoParticlesAreClose() > 0.0) //approach
  {
    if(_stateData.getTimeStepSize() >= mindt) //critical approach
    {
      double decrementStep = _stateData.getTwoParticlesAreClose();
      //increment carefully
      if(decrementStep > increaseFactor * _stateData.getTimeStepSize())
      {

      } else {
        //decrement
        printf("triggered step decrement : %f\n", decrementStep);
        _stateData.setTimeStepSize(decrementStep);
      }
      return;
    } else if(_stateData.getTimeStepSize() < mindt)
    {
      _stateData.setTimeStepSize(mindt);
    }
  } else //separation or no collision
  {
    printf("triggered step increment : %f\n", increaseFactor);
    if(_stateData.getTimeStep() > 2)
    {
      /*if(_stateData.getTwoParticlesSeparate() && _stateData.getTwoParticlesAreClose() == 0 && dem::mappings::CreateGrid::_gridType == dem::mappings::CreateGrid::ReluctantAdaptiveGrid)
      {
        _stateData.setTimeStepSize(_maxdt);
        return;
      }*/
      _stateData.setTimeStepSize(increaseFactor * _stateData.getTimeStepSize());
    }
  }

  if(_stateData.getTimeStepSize() > _maxdt) //upper bound constraint
  {
    printf("employed step size higher than maxdt, reduce to maxdt: %f\n", _maxdt);
    _stateData.setTimeStepSize(_maxdt);
  }
  //printf("maxdt:%f\n", maxdt);
}

double dem::State::getTwoParticlesAreClose() {
  return _stateData.getTwoParticlesAreClose();
}

void dem::State::setTimeStep(int number) {//name
  _stateData.setTimeStep(number);
}

int dem::State::getTimeStep() {
  return _stateData.getTimeStep();
}

void dem::State::setStepIncrement(double number) {//name
  _stateData.setStepIncrement(number);
}

double dem::State::getStepIncrement() {
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

double dem::State::getMaxDt() {
  return _maxdt;
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
