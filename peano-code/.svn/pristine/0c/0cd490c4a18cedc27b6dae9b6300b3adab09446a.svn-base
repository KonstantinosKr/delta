#include "myproject/State.h"
#include "myproject/Cell.h"
#include "myproject/Vertex.h"

#include "peano/grid/Checkpoint.h"



myproject::State::State():
  Base() { 
  // @todo Insert your code here
}


myproject::State::State(const Base::PersistentState& argument):
  Base(argument) {
  // @todo Insert your code here
}


void myproject::State::writeToCheckpoint( peano::grid::Checkpoint<myproject::Vertex,myproject::Cell>& checkpoint ) const {
  // @todo Insert your code here
}

    
void myproject::State::readFromCheckpoint( const peano::grid::Checkpoint<myproject::Vertex,myproject::Cell>& checkpoint ) {
  // @todo Insert your code here
}


void myproject::State::setTimeStepSize(double dt) {
  _stateData.setDt(dt);
}


double myproject::State::getTimeStepSize() const {
  return _stateData.getDt();
}

