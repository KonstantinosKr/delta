#include "peano/grid/SingleLevelEnumerator.h"
#include "tarch/compiler/CompilerSpecificSettings.h"
#include "peano/utils/PeanoOptimisations.h"
#include "peano/utils/Loop.h"
#include "tarch/Assertions.h"



int peano::grid::SingleLevelEnumerator::lineariseCellIndex( const LocalVertexIntegerIndex& cellPosition ) {
  int base   = 1;
  int result = 0;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion1(cellPosition(d)>=0,cellPosition);
    assertion1(cellPosition(d)<=2,cellPosition);
	  result += cellPosition(d)*base;
	  base   *= 3;
  }
  assertion( result>= 0 );
  assertion( result< THREE_POWER_D );
  return result;
}


int peano::grid::SingleLevelEnumerator::lineariseVertexIndex( const LocalVertexIntegerIndex& vertexPosition ) {
  int base   = 1;
  int result = 0;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion1(vertexPosition(d)>=0,vertexPosition);
    assertion1(vertexPosition(d)<=3,vertexPosition);
    result += vertexPosition(d)*base;
    base   *= 4;
  }
  assertion( result>= 0 );
  assertion( result< FOUR_POWER_D );
  return result;
}


peano::grid::SingleLevelEnumerator::~SingleLevelEnumerator() {
}


peano::grid::SingleLevelEnumerator::SingleLevelEnumerator(
  const Vector&                   coarseGridCellSize,
  const Vector&                   domainOffset,
  int                             coarseGridLevel
):
  _discreteOffset(0),
  _fineGridCellSize(coarseGridCellSize/3.0),
  _domainOffset(domainOffset),
  _level(coarseGridLevel+1),
  _adjacentCellsHeight(peano::grid::NotStationary) {
  #ifdef Asserts
  for (int d=0; d<DIMENSIONS; d++) {
    assertion4( !std::isnan( _domainOffset(d) ), toString(), coarseGridCellSize, domainOffset, coarseGridLevel );
    assertion4( !std::isnan( _fineGridCellSize(d) ), toString(), coarseGridCellSize, domainOffset, coarseGridLevel );
    assertion4( !std::isinf( _domainOffset(d) ), toString(), coarseGridCellSize, domainOffset, coarseGridLevel );
    assertion4( !std::isinf( _fineGridCellSize(d) ), toString(), coarseGridCellSize, domainOffset, coarseGridLevel );
    assertion4( _discreteOffset(d)>=0, toString(), coarseGridCellSize, domainOffset, coarseGridLevel );
    assertion4( _discreteOffset(d)<=3, toString(), coarseGridCellSize, domainOffset, coarseGridLevel );
  }
  assertion4( _level>=0, toString(), coarseGridCellSize, domainOffset, coarseGridLevel );
  #endif
}


peano::grid::SingleLevelEnumerator::SingleLevelEnumerator(
  const Vector&                   coarseGridCellSize,
  const Vector&                   domainOffset,
  int                             level,
  peano::grid::CellFlags adjacentCellsHeight
):
  _discreteOffset(0),
  _fineGridCellSize(coarseGridCellSize/3.0),
  _domainOffset(domainOffset),
  _level(level+1),
  _adjacentCellsHeight(adjacentCellsHeight) {
  #ifdef Asserts
  for (int d=0; d<DIMENSIONS; d++) {
    assertion4( !std::isnan( _domainOffset(d) ), toString(), coarseGridCellSize, domainOffset, level );
    assertion4( !std::isnan( _fineGridCellSize(d) ), toString(), coarseGridCellSize, domainOffset, level );
    assertion4( !std::isinf( _domainOffset(d) ), toString(), coarseGridCellSize, domainOffset, level );
    assertion4( !std::isinf( _fineGridCellSize(d) ), toString(), coarseGridCellSize, domainOffset, level );
    assertion4( _discreteOffset(d)>=0, toString(), coarseGridCellSize, domainOffset, level );
    assertion4( _discreteOffset(d)<=3, toString(), coarseGridCellSize, domainOffset, level );
  }
  assertion4( _level>=0, toString(), coarseGridCellSize, domainOffset, level );
  #endif
}


peano::grid::SingleLevelEnumerator::SingleLevelEnumerator( const SingleLevelEnumerator& copy ):
  _discreteOffset( copy._discreteOffset ),
  _fineGridCellSize( copy._fineGridCellSize ),
  _domainOffset( copy._domainOffset ),
  _level( copy._level ),
  _adjacentCellsHeight( copy._adjacentCellsHeight ) {
  #ifdef Asserts
  for (int d=0; d<DIMENSIONS; d++) {
    assertion2( !std::isnan( _domainOffset(d) ), toString(), copy.toString() );
    assertion2( !std::isnan( _fineGridCellSize(d) ), toString(), copy.toString() );
    assertion2( !std::isinf( _domainOffset(d) ), toString(), copy.toString() );
    assertion2( !std::isinf( _fineGridCellSize(d) ), toString(), copy.toString() );
    assertion2( _discreteOffset(d)>=0, toString(), copy.toString() );
    assertion2( _discreteOffset(d)<=3, toString(), copy.toString() );
  }
  assertion2( _level>=0, toString(), copy.toString() );
  #endif
}


void peano::grid::SingleLevelEnumerator::updateAdjacentCellsFlag( peano::grid::CellFlags newFlag ) {
  _adjacentCellsHeight = newFlag;
}


peano::grid::SingleLevelEnumerator peano::grid::SingleLevelEnumerator::getRefinedEnumerator() const {
  Vector subEnumeratorDomainOffset;
  for (int d=0; d<DIMENSIONS; d++) {
	subEnumeratorDomainOffset(d)  = _discreteOffset(d);
    subEnumeratorDomainOffset(d) *= _fineGridCellSize(d);
    subEnumeratorDomainOffset(d) += _domainOffset(d);
  }

  if (_adjacentCellsHeight>peano::grid::Leaf) {
    return SingleLevelEnumerator(_fineGridCellSize, subEnumeratorDomainOffset, _level, static_cast<peano::grid::CellFlags>(_adjacentCellsHeight-1) );
  }
  else {
    return SingleLevelEnumerator(_fineGridCellSize, subEnumeratorDomainOffset, _level, _adjacentCellsHeight );
  }
}


void peano::grid::SingleLevelEnumerator::setOffset(const LocalVertexIntegerIndex&  gridPointOffset) {
  for (int d=0; d<DIMENSIONS; d++) {
    assertion2( gridPointOffset(d)>=0, gridPointOffset, toString() );
    assertion2( gridPointOffset(d)<=3, gridPointOffset, toString() );
  }
  _discreteOffset = gridPointOffset;
}


#ifdef Asserts
peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex peano::grid::SingleLevelEnumerator::getOffset() const {
  return _discreteOffset;
}
#endif



int peano::grid::SingleLevelEnumerator::operator() (int localVertexNumber) const {
  peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex localOffset;
  int base   = TWO_POWER_D_DIVIDED_BY_TWO;
  for (int d=DIMENSIONS-1; d>=0; d--) {
	  localOffset(d)     = localVertexNumber / base;
	  assertion5( localOffset(d)>=0, localOffset, localVertexNumber, d, base, _discreteOffset );
	  assertion5( localOffset(d)<=1, localOffset, localVertexNumber, d, base, _discreteOffset );
	  localVertexNumber -= localOffset(d) * base;
    base              /= 2;
  }
  localOffset += _discreteOffset;
  return peano::grid::SingleLevelEnumerator::lineariseVertexIndex( localOffset );
}


int peano::grid::SingleLevelEnumerator::operator() (const LocalVertexIntegerIndex&  localVertexNumber ) const {
  return lineariseVertexIndex(localVertexNumber+_discreteOffset);
}


int peano::grid::SingleLevelEnumerator::operator() (const LocalVertexBitsetIndex&   localVertexNumber ) const {
  return (*this)( static_cast<int>( localVertexNumber.to_ulong()) );
}


peano::grid::SingleLevelEnumerator::Vector peano::grid::SingleLevelEnumerator::getVertexPosition(int localVertexNumber) const {
  return getVertexPosition(peano::utils::dDelinearised(localVertexNumber, 2));
}


peano::grid::SingleLevelEnumerator::Vector peano::grid::SingleLevelEnumerator::getVertexPosition(const LocalVertexIntegerIndex& localVertexNumber ) const {
  peano::grid::SingleLevelEnumerator::Vector result( _domainOffset );
  for(int d=0; d<DIMENSIONS;d++) {
    double delta = localVertexNumber(d) + _discreteOffset(d);
	  result(d) += delta * _fineGridCellSize(d);
  }
  return result;
}


peano::grid::SingleLevelEnumerator::Vector peano::grid::SingleLevelEnumerator::getVertexPosition(const LocalVertexBitsetIndex& localVertexNumber ) const {
  return getVertexPosition(static_cast<int>( localVertexNumber.to_ulong()) );
}


peano::grid::SingleLevelEnumerator::Vector peano::grid::SingleLevelEnumerator::getVertexPosition() const {
  return getVertexPosition(LocalVertexIntegerIndex(0));
}


peano::grid::SingleLevelEnumerator::Vector peano::grid::SingleLevelEnumerator::getCellCenter() const {
  return getVertexPosition() + _fineGridCellSize/2.0;
}


peano::grid::SingleLevelEnumerator::Vector peano::grid::SingleLevelEnumerator::getCellSize() const {
  return _fineGridCellSize;
}


std::string peano::grid::SingleLevelEnumerator::toString() const {
  std::ostringstream out;
  out << "(domain-offset:" << _domainOffset
	  << ",discrete-offset:" << _discreteOffset
	  << ",cell-size:" << _fineGridCellSize
	  << ",level:" << _level
	  << ",adj-flags:" << _adjacentCellsHeight
	  << ")[type=SingleLevelEnumerator]";
  return out.str();
}


int peano::grid::SingleLevelEnumerator::getLevel() const {
  return _level;
}


peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex
peano::grid::SingleLevelEnumerator::getParentVertex(const LocalVertexIntegerIndex& index ) {
  peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex result;
  for (int d=0; d<DIMENSIONS; d++ ) {
    result(d) = index(d)/2;
    assertion( index(d)!=0 || result(d)==0 );
    assertion( index(d)!=1 || result(d)==0 );
    assertion( index(d)!=2 || result(d)==1 );
    assertion( index(d)!=3 || result(d)==1 );
  }
  return result;
}


peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex
peano::grid::SingleLevelEnumerator::getVertexPositionOnCoarserLevel(const LocalVertexIntegerIndex& index ) {
  assertion1( isVertexPositionAlsoACoarseVertexPosition(index), index );
  peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex result;
  for (int d=0; d<DIMENSIONS; d++ ) {
    result(d) = index(d)/3;
  }
  return result;
}


bool peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseVertexPosition(const LocalVertexIntegerIndex& index ) {
  bool result = true;
  // @todo Deploy this to lookup table, at least for d=2,3, and 4
  for (int d=0; d<DIMENSIONS; d++ ) {
    result &= ((index(d)==0) || (index(d)==3));
  }
  return result;
}


bool peano::grid::SingleLevelEnumerator::isVertexPositionAlsoACoarseGridPosition(const LocalVertexIntegerIndex& index ) {
  bool result = false;
  // @todo Deploy this to lookup table, at least for d=2,3, and 4
  for (int d=0; d<DIMENSIONS; d++ ) {
    result  |= ((index(d)==0) || (index(d)==3));
  }
  return result;
}


bool peano::grid::SingleLevelEnumerator::doesCoarseGridVertexInfluenceFineGridVertex(
  const LocalVertexIntegerIndex&  positionInCoarseGridCell,
  const LocalVertexIntegerIndex&  positionInFineGridCell
) {
  bool result = true;
  // @todo Deploy this into lookup table, at least for d=2,3, and 4
  for (int d=0; d<DIMENSIONS; d++) {
    assertion2( positionInFineGridCell(d)>=0,   positionInFineGridCell, positionInCoarseGridCell );
    assertion2( positionInFineGridCell(d)<=3,   positionInFineGridCell, positionInCoarseGridCell );
    assertion2( positionInCoarseGridCell(d)>=0, positionInFineGridCell, positionInCoarseGridCell );
    assertion2( positionInCoarseGridCell(d)<=1, positionInFineGridCell, positionInCoarseGridCell );

    result &=
      (
        (positionInFineGridCell(d)==0 && positionInCoarseGridCell(d)==0) ||
         positionInFineGridCell(d)==1 ||
         positionInFineGridCell(d)==2 ||
        (positionInFineGridCell(d)==3 && positionInCoarseGridCell(d)==1)
      );
  }

  return result;
}


peano::grid::SingleLevelEnumerator::LocalVertexIntegerIndex peano::grid::SingleLevelEnumerator::getVertexPositionWithinCoarserPatch(const LocalVertexIntegerIndex& localVertexNumber ) const {
  return _discreteOffset + localVertexNumber;
}


peano::grid::CellFlags peano::grid::SingleLevelEnumerator::getCellFlags() const {
  return _adjacentCellsHeight;
}


int peano::grid::SingleLevelEnumerator::cell(const LocalVertexIntegerIndex& localVertexNumber) const {
  return 0;
}


bool peano::grid::SingleLevelEnumerator::overlaps(const Vector& offset, const Vector& size) const {
  bool result = true;
  for (int d=0; d<DIMENSIONS; d++) {
    result &= smallerEqualsUpToRelativeTolerance( offset(d),         _domainOffset(d)+(_discreteOffset(d)+1.0)*_fineGridCellSize(d));
    result &= greaterEqualsUpToRelativeTolerance( offset(d)+size(d), _domainOffset(d)+_discreteOffset(d)*_fineGridCellSize(d));
  }
  return result;
}


bool peano::grid::SingleLevelEnumerator::isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const {
  bool result = false;
  for (int d=0; d<DIMENSIONS; d++) {
    result |= _discreteOffset(d)+localVertexNumber(d) == 0;
    result |= _discreteOffset(d)+localVertexNumber(d) == 3;
  }
  return result;
}
