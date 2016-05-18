#include "peano/datatraversal/ActionSet.h"


peano::datatraversal::ActionSet::ActionSet():
  _actionSet() {
}


peano::datatraversal::ActionSet::~ActionSet() {
}


void peano::datatraversal::ActionSet::addAction( const Action& action ) {
  _actionSet.push_back(action);
}


int peano::datatraversal::ActionSet::getNumberOfParallelActions() const {
  return static_cast<int>( _actionSet.size() );
}


const peano::datatraversal::Action& peano::datatraversal::ActionSet::getAction( int i ) const {
  assertion( i >= 0 );
  assertion( i<getNumberOfParallelActions() );
  return _actionSet[i];
}
