#include "peano/grid/TraversalOrderOnTopLevel.h"

#ifdef Parallel
#include "tarch/parallel/NodePool.h"
#include "peano/parallel/SendReceiveBufferPool.h"
#include "peano/parallel/JoinDataBufferPool.h"
#endif


tarch::logging::Log  peano::grid::TraversalOrderOnTopLevel::_log( "peano::grid::TraversalOrderOnTopLevel" );
const int peano::grid::TraversalOrderOnTopLevel::InvalidOrderEntry(-1);


peano::grid::TraversalOrderOnTopLevel::TraversalOrderOnTopLevel() {
  logTraceIn( "TraversalOrderOnTopLevel()" );
  reset();
  logTraceOut( "TraversalOrderOnTopLevel()" );
}


void peano::grid::TraversalOrderOnTopLevel::initialiseWithPeanoSFC(peano::utils::LoopDirection loopDirection) {
  logTraceInWith1Argument( "initialiseWithPeanoSFC(LoopDirection)", loopDirection );
  int counter = 0;
  zfor3(currentCell,loopDirection) 
    _order[counter] = currentCell;
    counter++;
  endzfor

  for (int i=0; i<THREE_POWER_D-1; i++) {
    _coarseBoundaryInBetweenCells[i] = false;
  }
  logTraceOut( "initialiseWithPeanoSFC(LoopDirection)" );
}


std::string peano::grid::TraversalOrderOnTopLevel::toString() const {
  std::ostringstream msg;

  for (int i=0; i<THREE_POWER_D-1; i++) {
    msg << "| " << _order[i] << " |"
        << _coarseBoundaryInBetweenCells[i];
  }
  msg << "| " << _order[THREE_POWER_D-1] << " |";
  return msg.str();
}


void peano::grid::TraversalOrderOnTopLevel::reset() {
  logTraceIn( "reset()" );
  for (int i=0; i<THREE_POWER_D-1; i++) {
    _order[i]                        = tarch::la::Vector<DIMENSIONS,int>(InvalidOrderEntry);
    _coarseBoundaryInBetweenCells[i] = false;
  }
  _order[THREE_POWER_D-1]            = tarch::la::Vector<DIMENSIONS,int>(InvalidOrderEntry);
  logTraceOutWith1Argument( "reset()", toString() );
}


bool peano::grid::TraversalOrderOnTopLevel::isValid() const {
  logTraceIn( "isValid()" );
  for (int i=0; i<THREE_POWER_D-1; i++) {
    for (int d=0; d<DIMENSIONS; d++) {
      if (_order[i](d)<0) {
        logTraceOutWith1Argument( "isValid()", false );
        return false;
      }
      if (_order[i](d)>2) {
        logTraceOutWith1Argument( "isValid()", false );
        return false;
      }
    }
  }

  logTraceOutWith1Argument( "isValid()", true );
  return true;
}


tarch::la::Vector<DIMENSIONS,int> peano::grid::TraversalOrderOnTopLevel::getCellIdentifiedByReceivedBitset( const std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT>& mask ) {
  logTraceInWith1Argument( "getCellIdentifiedByReceivedBitset(bitset)", mask );
  tarch::la::Vector<DIMENSIONS,int> result(-1);

  dfor2(k)
    if ( mask[kScalar] ) {
      for (int d=0; d<DIMENSIONS; d++) {
        if (k(d) && result(d)==-1) {
          result(d)=0;
        }
        else if (! k(d) && result(d)==-1) {
          result(d)=2;
        }
        else if (k(d) && result(d)==2) {
          result(d)=1;
        }
        else if (! k(d) && result(d)==0) {
          result(d)=1;
        }
      }
    }
  enddforx

  for (int d=0; d<DIMENSIONS; d++) {
    assertion1( result(d)>=0, result );
    assertion1( result(d)<=2, result );
  }

  logTraceOutWith1Argument( "getCellIdentifiedByReceivedBitset(bitset)", result );
  return result;
}


bool peano::grid::TraversalOrderOnTopLevel::isCellContainedInOrder( const tarch::la::Vector<DIMENSIONS,int>& cell ) const {
  for (int i=0; i<THREE_POWER_D; i++) {
    if (_order[i]==cell) {
      return true;
    }
  }
  return false;
}


void peano::grid::TraversalOrderOnTopLevel::waitUntilCellIsAvailable(int cellIndexInOrder, bool isTraversalInverted) {
  #ifdef Parallel
  const bool IsUnavailable = _order[cellIndexInOrder](0)==InvalidOrderEntry;
  logTraceInWith4Arguments( "waitUntilCellIsAvailable(int)", cellIndexInOrder, isTraversalInverted, IsUnavailable, toString() );
  if (IsUnavailable) {
    assertion( !tarch::parallel::Node::getInstance().isGlobalMaster() );
    const int                                         MasterRank( tarch::parallel::NodePool::getInstance().getMasterRank() );
    const std::bitset<NUMBER_OF_VERTICES_PER_ELEMENT> receivedFlag( peano::parallel::JoinDataBufferPool::getInstance().getCellMarkerFromStream(MasterRank) );
    tarch::la::Vector<DIMENSIONS,int>  nextCellToBeReceived = getCellIdentifiedByReceivedBitset(receivedFlag);
    logDebug( "waitUntilCellIsAvailable(...)", "received " << receivedFlag << " from master " << MasterRank << " identifying cell " << nextCellToBeReceived );

    if ( isCellContainedInOrder(nextCellToBeReceived) ) {
      if ( !isTraversalInverted ) {
        cellIndexInOrder--;
      }
      assertion( cellIndexInOrder>=0 );
      assertion( cellIndexInOrder<THREE_POWER_D-1 );
      _coarseBoundaryInBetweenCells[cellIndexInOrder] = true;
      logDebug( "waitUntilCellIsAvailable(...)", "set descend at index " << cellIndexInOrder << " to true" );
    }
    else {
      _order[cellIndexInOrder] = nextCellToBeReceived;
      logDebug( "waitUntilCellIsAvailable(...)", "set order " << cellIndexInOrder << " to " << nextCellToBeReceived );
    }
  }
  logTraceOut( "waitUntilCellIsAvailable(int)" );
  #endif
}


tarch::la::Vector<DIMENSIONS,int> peano::grid::TraversalOrderOnTopLevel::getNextCellToTraverseOnLevelOne(
  int     numberOfCell,
  bool    isTraversalInverted
) {
  logTraceInWith2Arguments( "getNextCellToTraverseOnLevelOne(...)", numberOfCell, isTraversalInverted );

  assertion( numberOfCell>=0 );
  assertion( numberOfCell<THREE_POWER_D );

  const int cellEntry = isTraversalInverted ? THREE_POWER_D-1-numberOfCell : numberOfCell;

  waitUntilCellIsAvailable(cellEntry, isTraversalInverted);

  logTraceOutWith2Arguments( "getNextCellToTraverseOnLevelOne(...)", _order[cellEntry], cellEntry );

  return _order[cellEntry];
}


bool peano::grid::TraversalOrderOnTopLevel::descendBeforeContinuingWithCell(
  int     numberOfCell,
  bool    isTraversalInverted
) {
  logTraceInWith2Arguments( "descendBeforeContinuingWithCell(...)", numberOfCell, isTraversalInverted );

  assertion( numberOfCell>0 );
  assertion( numberOfCell<THREE_POWER_D );

  const int cellEntry  = isTraversalInverted ? THREE_POWER_D-1-numberOfCell : numberOfCell;
  const int orderEntry = isTraversalInverted ? THREE_POWER_D-1-numberOfCell : numberOfCell-1;

  waitUntilCellIsAvailable(cellEntry, isTraversalInverted);

  logTraceOutWith2Arguments( "descendBeforeContinuingWithCell(...)", _coarseBoundaryInBetweenCells[orderEntry], orderEntry );

  return _coarseBoundaryInBetweenCells[orderEntry];
}

