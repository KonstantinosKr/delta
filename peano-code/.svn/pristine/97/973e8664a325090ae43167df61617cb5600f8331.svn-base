#include "peano/grid/aspects/CellLocalPeanoCurve.h"



#if defined(CacheActionSets)
std::map<int,peano::datatraversal::ActionSetTraversal*> peano::grid::aspects::CellLocalPeanoCurve::cachedEntriesForWriteVertexSequence;
std::map<int,peano::datatraversal::ActionSetTraversal*> peano::grid::aspects::CellLocalPeanoCurve::cachedEntriesForReadVertexSequence;
#endif


void peano::grid::aspects::CellLocalPeanoCurve::releaseCachedData() {
  #if defined(CacheActionSets)
  for (
    std::map<int,peano::datatraversal::ActionSetTraversal*>::iterator p = cachedEntriesForWriteVertexSequence.begin();
    p != cachedEntriesForWriteVertexSequence.end();
    p++
  ) {
    delete p->second;
  }


  for (
    std::map<int,peano::datatraversal::ActionSetTraversal*>::iterator p = cachedEntriesForReadVertexSequence.begin();
    p != cachedEntriesForReadVertexSequence.end();
    p++
  ) {
    delete p->second;
  }
  #endif
}


peano::grid::aspects::CellLocalPeanoCurve::CellLocalPeanoCurve() {
}


peano::grid::aspects::CellLocalPeanoCurve& peano::grid::aspects::CellLocalPeanoCurve::getInstance() {
  static peano::grid::aspects::CellLocalPeanoCurve singleton;
  return singleton;
}


tarch::la::Vector<DIMENSIONS,int>
peano::grid::aspects::CellLocalPeanoCurve::toIntVector(const Coordinates& coordinates) {
  tarch::la::Vector<DIMENSIONS,int>  result;
  for (int d=0; d<DIMENSIONS; d++) {
    result(d) = coordinates[d] ? 1 : 0;
  }
  return result;
}


int peano::grid::aspects::CellLocalPeanoCurve::getUniqueKey(
  const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const std::bitset<DIMENSIONS>& evenFlags, bool isTraversalInverted, bool isParallel
) {
  const int Offset = 2*DIMENSIONS-1;
  const int Base   = 2*Offset+1;
  int result      = 0;
  int currentBase = 1;
  for (int i=0; i<DIMENSIONS_TIMES_TWO;i++) {
    result      += (accessNumber(i)+Offset) * currentBase;
    currentBase *= Base;
  }
  for (int d=0; d<DIMENSIONS; d++) {
    if (evenFlags[d]) result += currentBase;
    currentBase *= 2;
  }
  if (isTraversalInverted) {
    result += currentBase;
  }
  currentBase *= 2;
  if (isParallel) {
    result += currentBase;
  }

  assertion6( result>=0, result, accessNumber, evenFlags, isTraversalInverted, isParallel, "buffer overflows may happen for big dimensions. Compile with -DnoCacheActionSets (see PeanoOptimisations)" );
  return result;
}


int peano::grid::aspects::CellLocalPeanoCurve::getReadStackIdentifier(
  const Coordinates&                                        coordinates,
  const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>&  accessNumber,
  const std::bitset<DIMENSIONS>&                            evenFlags,
  const bool&                                               isTraversalInverted
) {
  int smallestValue = -2*DIMENSIONS-1;
  int result        = peano::stacks::Constants::InOutStack;
  int direction     = -1;
  const int scaleAccessNumber = isTraversalInverted ? -1 : 1;

  for (int d=0; d<DIMENSIONS; d++) {
    const int face = coordinates[d]==0 ? d : d+DIMENSIONS;
    const int faceAccessNumber = accessNumber(face) * scaleAccessNumber;
    if (faceAccessNumber<0 && faceAccessNumber>smallestValue) {
      result        = face;
      smallestValue = faceAccessNumber;
      direction     = d;
    }
  }

  if ( result!=peano::stacks::Constants::InOutStack && evenFlags[direction] ) {
    result = result<DIMENSIONS ? result+DIMENSIONS : result-DIMENSIONS;
  }

  assertion5(result==peano::stacks::Constants::InOutStack || result>=0,result,coordinates,accessNumber,evenFlags,isTraversalInverted);

  return result;
}


int peano::grid::aspects::CellLocalPeanoCurve::getWriteStackIdentifier(
  const Coordinates&                                        coordinates,
  const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>&  accessNumber,
  const std::bitset<DIMENSIONS>&                            evenFlags,
  const bool&                                               isTraversalInverted
) {
  int biggestValue = 2*DIMENSIONS+1;
  int result       = peano::stacks::Constants::InOutStack;
  int direction    = -1;
  const int scaleAccessNumber = isTraversalInverted ? -1 : 1;

  for (int d=0; d<DIMENSIONS; d++) {
    const int face = coordinates[d]==0 ? d : d+DIMENSIONS;
    const int faceAccessNumber = accessNumber(face) * scaleAccessNumber;
    if (faceAccessNumber>0 && faceAccessNumber<biggestValue) {
      result       = face;
      biggestValue = faceAccessNumber;
      direction    = d;
    }
  }

  if ( result!=peano::stacks::Constants::InOutStack && evenFlags[direction] ) {
    result = result<DIMENSIONS ? result+DIMENSIONS : result-DIMENSIONS;
  }

  return result;
}


peano::datatraversal::ActionSetTraversal
peano::grid::aspects::CellLocalPeanoCurve::createParallelReadVertexSequence( const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const std::bitset<DIMENSIONS>& evenFlags, const bool& isTraversalInverted ) {
  peano::datatraversal::ActionSetTraversal result =
    createSequentialReadVertexSequence(accessNumber, evenFlags, isTraversalInverted);

  return peano::datatraversal::ActionSetTraversal::sortIntoIdBins( result );
}


peano::datatraversal::ActionSetTraversal
peano::grid::aspects::CellLocalPeanoCurve::createParallelWriteVertexSequence( const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const std::bitset<DIMENSIONS>& evenFlags, const bool& isTraversalInverted ) {
  peano::datatraversal::ActionSetTraversal result =
    createSequentialWriteVertexSequence(accessNumber, evenFlags, isTraversalInverted);

  return peano::datatraversal::ActionSetTraversal::sortIntoIdBins( result );
}


peano::datatraversal::ActionSetTraversal
peano::grid::aspects::CellLocalPeanoCurve::createSequentialReadVertexSequence( const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const std::bitset<DIMENSIONS>& evenFlags, const bool& isTraversalInverted ) {
  peano::datatraversal::ActionSetTraversal result(NUMBER_OF_VERTICES_PER_ELEMENT);

  int coordinates = static_cast<int>( getFirstVertexIndex(evenFlags).to_ulong() );
  for (int i=0; i<NUMBER_OF_VERTICES_PER_ELEMENT; i++) {
    Coordinates currentLocalVertexIndex( coordinates ^ i );

    int  stackIdentifier = getReadStackIdentifier(currentLocalVertexIndex,accessNumber,evenFlags,isTraversalInverted);
    const int actionSet = isTraversalInverted ? NUMBER_OF_VERTICES_PER_ELEMENT - i - 1 : i;
    assertion(result.getActionSet(actionSet).getNumberOfParallelActions()==0);
    result.getActionSet(actionSet).addAction(peano::datatraversal::Action(toIntVector(currentLocalVertexIndex),stackIdentifier));
  }

  return result;
}


peano::datatraversal::ActionSetTraversal
peano::grid::aspects::CellLocalPeanoCurve::createSequentialWriteVertexSequence( const tarch::la::Vector<DIMENSIONS_TIMES_TWO,short int>& accessNumber, const std::bitset<DIMENSIONS>& evenFlags, const bool& isTraversalInverted ) {
  peano::datatraversal::ActionSetTraversal result(NUMBER_OF_VERTICES_PER_ELEMENT);

  int coordinates = static_cast<int>( getFirstVertexIndex(evenFlags).to_ulong() );
  for (int i=0; i<NUMBER_OF_VERTICES_PER_ELEMENT; i++) {
    Coordinates currentLocalVertexIndex( coordinates ^ i );

    int  stackIdentifier = getWriteStackIdentifier(currentLocalVertexIndex,accessNumber,evenFlags,isTraversalInverted);
    const int actionSet = isTraversalInverted ? NUMBER_OF_VERTICES_PER_ELEMENT - i - 1 : i;
    assertion(result.getActionSet(actionSet).getNumberOfParallelActions()==0);
    result.getActionSet(actionSet).addAction(peano::datatraversal::Action(toIntVector(currentLocalVertexIndex),stackIdentifier));
  }

  return result;
}


peano::grid::aspects::CellLocalPeanoCurve::Coordinates
peano::grid::aspects::CellLocalPeanoCurve::getFirstVertexIndex( const std::bitset<DIMENSIONS>& evenFlags ) {
  Coordinates result;
  Coordinates currentMask;

  for (int i=0; i<DIMENSIONS; i++) {
    if ( evenFlags[i] ) {
      currentMask[i] = true;
      currentMask.flip();

      result ^= currentMask;

      currentMask.flip();
      currentMask[i] = false;
    }
  }

  return result;
}
