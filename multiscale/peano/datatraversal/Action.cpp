#include "peano/datatraversal/Action.h"

peano::datatraversal::Action::Action(
  tarch::la::Vector<DIMENSIONS,int> cartesianPosition,
  int                               id
):
  _cartesianPosition(cartesianPosition),
  _id(id) {
}


peano::datatraversal::Action::~Action() {
}
