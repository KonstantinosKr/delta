#include "peano/datatraversal/ActionSetTraversal.h"

#include "peano/utils/Loop.h"

#include <vector>


peano::datatraversal::ActionSetTraversal::ActionSetTraversal(int numberOfActionSets):
  _actionSets(numberOfActionSets) {
}


peano::datatraversal::ActionSetTraversal::~ActionSetTraversal() {
}


int peano::datatraversal::ActionSetTraversal::getMaximumPath() const {
  return static_cast<int>(_actionSets.size());
}


int peano::datatraversal::ActionSetTraversal::getMaximumGrainSize() const {
  int result = 0;
  for (std::vector< ActionSet >::const_iterator p=_actionSets.begin(); p<_actionSets.end(); p++) {
    result = result > p->getNumberOfParallelActions() ? result : p->getNumberOfParallelActions();
  }
  return result;
}


const peano::datatraversal::ActionSet&
peano::datatraversal::ActionSetTraversal::getActionSet( int i ) const {
  assertion( i>=0 );
  assertion( i < getMaximumPath() );
  return _actionSets[i];
}


peano::datatraversal::ActionSet&
peano::datatraversal::ActionSetTraversal::getActionSet( int i ) {
  assertion1( i>=0, toString() );
  assertion3( i < getMaximumPath(), i, getMaximumPath(), toString() );
  return _actionSets[i];
}


std::string peano::datatraversal::ActionSetTraversal::toString() const {
  std::ostringstream out;
  out << "(";

  for (int i=0; i<getMaximumPath(); i++) {
    out << "{";
    for (int j=0; j<_actionSets[i].getNumberOfParallelActions(); j++) {
      out << "(x=";
      out << _actionSets[i].getAction(j)._cartesianPosition;
      out << ",id=";
      out << _actionSets[i].getAction(j)._id;
      out << ")";
    }
    out << "}";
  }

  out << ")";
  return out.str();
}


peano::datatraversal::ActionSetTraversal
peano::datatraversal::ActionSetTraversal::sortIntoIdBins(const ActionSetTraversal& input) {
  peano::datatraversal::ActionSetTraversal result(0);

  for (int i=0; i<input.getMaximumPath(); i++) {
	assertion2( input.getActionSet(i).getNumberOfParallelActions()==1, i, input.getActionSet(i).getNumberOfParallelActions() );
	int  newId = input.getActionSet(i).getAction(0)._id;
	int  currentActionSet = 0;
	bool holdsIdAlready   = true;
	while (currentActionSet<result.getMaximumPath() && holdsIdAlready) {
      holdsIdAlready = false;
      for (int j=0; j<result.getActionSet(currentActionSet).getNumberOfParallelActions(); j++) {
    	holdsIdAlready |= result.getActionSet(currentActionSet).getAction(j)._id == newId;
      }
      if (holdsIdAlready) {
        currentActionSet++;
      }
	}
	if (currentActionSet==result.getMaximumPath()) {
	  result._actionSets.push_back( ActionSet() );
	}
    result.getActionSet(currentActionSet).addAction(input.getActionSet(i).getAction(0));
  }

  return result;
}
