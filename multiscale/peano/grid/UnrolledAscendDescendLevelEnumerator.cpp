#include "peano/grid/UnrolledAscendDescendLevelEnumerator.h"
#include "peano/grid/UnrolledLevelEnumerator.h"

#include "peano/utils/Loop.h"

tarch::logging::Log  peano::grid::UnrolledAscendDescendLevelEnumerator::_log( "peano::grid::UnrolledAscendDescendLevelEnumerator" );


peano::grid::UnrolledAscendDescendLevelEnumerator::~UnrolledAscendDescendLevelEnumerator() {
}


peano::grid::UnrolledAscendDescendLevelEnumerator::UnrolledAscendDescendLevelEnumerator(
  const peano::grid::UnrolledLevelEnumerator& enumerator
):
  _discreteOffset(enumerator._discreteOffset),
  _fineGridCellSize(enumerator._fineGridCellSize),
  _domainOffset(enumerator._domainOffset),
  _level(enumerator._level),
  _adjacentCellsHeight(enumerator._adjacentCellsHeight),
  _CellsPerAxis(enumerator._CellsPerAxis),
  _VerticesPerAxis(enumerator._VerticesPerAxis) {
  logTraceInWith1Argument( "UnrolledAscendDescendLevelEnumerator(...)", enumerator.toString() );
  logTraceOutWith7Arguments( "UnrolledAscendDescendLevelEnumerator(...)", _discreteOffset, _fineGridCellSize, _domainOffset, _level, _adjacentCellsHeight, _CellsPerAxis, _VerticesPerAxis );
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::getCellsPerAxis() const {
  logTraceIn( "getCellsPerAxis()" );
  logTraceOutWith1Argument( "getCellsPerAxis()", _CellsPerAxis );
  return _CellsPerAxis;
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::getVerticesPerAxis() const {
  logTraceIn( "getVerticesPerAxis()" );
  logTraceOutWith1Argument( "getVerticesPerAxis()", _VerticesPerAxis );
  return _VerticesPerAxis;
}


void peano::grid::UnrolledAscendDescendLevelEnumerator::setOffset(const LocalVertexIntegerIndex&  gridPointOffset) {
  logTraceInWith1Argument( "setOffset(Vector)", gridPointOffset );

  for (int d=0; d<DIMENSIONS; d++) {
    assertion2( gridPointOffset(d)>=0,               gridPointOffset, toString() );
    assertion2( gridPointOffset(d)<=_CellsPerAxis+1, gridPointOffset, toString() ); // if they are equal, we may only access the left/bottom points due to the enumerator
  }
  _discreteOffset = gridPointOffset;

  logTraceOut( "setOffset(Vector)" );
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::operator() (int localVertexNumber) const {
  logTraceInWith1Argument( "operator()(int)", localVertexNumber );
  peano::grid::UnrolledAscendDescendLevelEnumerator::LocalVertexIntegerIndex localOffset;
  int base   = FOUR_POWER_D_DIVIDED_BY_FOUR;
  for (int d=DIMENSIONS-1; d>=0; d--) {
	  localOffset(d)     = localVertexNumber / base;
	  assertion5( localOffset(d)>=0, localOffset, localVertexNumber, d, base, _discreteOffset );
	  assertion5( localOffset(d)<=3, localOffset, localVertexNumber, d, base, _discreteOffset );
	  localVertexNumber -= localOffset(d) * base;
    base              /= 4;
  }
  localOffset += _discreteOffset;
  int result =  lineariseVertexIndex( localOffset );
  logTraceOutWith1Argument( "operator()(int)", result );
  return result;
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::operator() (const LocalVertexIntegerIndex&  localVertexNumber ) const {
  return lineariseVertexIndex(localVertexNumber+_discreteOffset);
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::operator() (const LocalVertexBitsetIndex&   localVertexNumber ) const {
  return (*this)( static_cast<int>( localVertexNumber.to_ulong()) );
}


peano::grid::UnrolledAscendDescendLevelEnumerator::Vector peano::grid::UnrolledAscendDescendLevelEnumerator::getVertexPosition(int localVertexNumber) const {
  assertionMsg(false, "not implemented yet");
  return getVertexPosition(peano::utils::dDelinearised(localVertexNumber, _VerticesPerAxis));
}


peano::grid::UnrolledAscendDescendLevelEnumerator::Vector peano::grid::UnrolledAscendDescendLevelEnumerator::getVertexPosition(const LocalVertexIntegerIndex& localVertexNumber ) const {
  peano::grid::UnrolledAscendDescendLevelEnumerator::Vector result( _domainOffset );
  for(int d=0; d<DIMENSIONS;d++) {
	  double delta = localVertexNumber(d) + _discreteOffset(d);
	  result(d) += delta * _fineGridCellSize(d);
  }
  return result;
}


peano::grid::UnrolledAscendDescendLevelEnumerator::Vector peano::grid::UnrolledAscendDescendLevelEnumerator::getVertexPosition(const LocalVertexBitsetIndex& localVertexNumber ) const {
  return (*this)(static_cast<int>( localVertexNumber.to_ulong()) );
}


peano::grid::UnrolledAscendDescendLevelEnumerator::Vector peano::grid::UnrolledAscendDescendLevelEnumerator::getVertexPosition() const {
  return getVertexPosition(LocalVertexIntegerIndex(0));
}


peano::grid::UnrolledAscendDescendLevelEnumerator::Vector peano::grid::UnrolledAscendDescendLevelEnumerator::getCellCenter() const {
  return getVertexPosition() + _fineGridCellSize/2.0;
}


peano::grid::UnrolledAscendDescendLevelEnumerator::Vector peano::grid::UnrolledAscendDescendLevelEnumerator::getCellSize() const {
  return _fineGridCellSize;
}


std::string peano::grid::UnrolledAscendDescendLevelEnumerator::toString() const {
  std::ostringstream out;
  out << "(domain-offset:" << _domainOffset
	  << ",discrete-offset:" << _discreteOffset
	  << ",cell-size:" << _fineGridCellSize
	  << ",level:" << _level
	  << ",adj-flags:" << _adjacentCellsHeight
	  << ",cells-per-axis:" << _CellsPerAxis
	  << ",vertices-per-axis:" << _VerticesPerAxis
	  << ")[type=UnrolledAscendDescendLevelEnumerator]";
  return out.str();
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::getLevel() const {
  return _level;
}


peano::grid::CellFlags peano::grid::UnrolledAscendDescendLevelEnumerator::getCellFlags() const {
  return _adjacentCellsHeight;
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::cell(const LocalVertexIntegerIndex& localCellNumber) const {
  return lineariseCellIndex(_discreteOffset + localCellNumber);
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::lineariseCellIndex( const LocalVertexIntegerIndex& cellPosition ) const {
  int base   = 1;
  int result = 0;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion2(cellPosition(d)>=0,cellPosition,toString());
    assertion2(cellPosition(d)<_CellsPerAxis,cellPosition,toString());
    result += cellPosition(d)*base;
    base   *= _CellsPerAxis;
  }
  assertion( result>= 0 );
  return result;
}


int peano::grid::UnrolledAscendDescendLevelEnumerator::lineariseVertexIndex( const LocalVertexIntegerIndex& vertexPosition ) const {
  logTraceInWith1Argument( "lineariseVertexIndex(...)", vertexPosition );
  int base   = 1;
  int result = 0;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion2(vertexPosition(d)>=0,vertexPosition,toString());
    assertion4(vertexPosition(d)<_VerticesPerAxis,d,vertexPosition,_VerticesPerAxis,  toString());
    result += vertexPosition(d)*base;
    base   *= _VerticesPerAxis;
  }
  assertion( result>= 0 );
  logTraceOutWith1Argument( "lineariseVertexIndex(...)", result );
  return result;
}


bool peano::grid::UnrolledAscendDescendLevelEnumerator::overlaps(const Vector& offset, const Vector& size) const {
  bool result = true;
  for (int d=0; d<DIMENSIONS; d++) {
    result &= smallerEqualsUpToRelativeTolerance( offset(d),         _domainOffset(d)+(_discreteOffset(d)+1.0)*_fineGridCellSize(d));
    result &= greaterEqualsUpToRelativeTolerance( offset(d)+size(d), _domainOffset(d)+_discreteOffset(d)*_fineGridCellSize(d));
  }
  return result;
}


bool peano::grid::UnrolledAscendDescendLevelEnumerator::isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const {
  bool result = false;
  for (int d=0; d<DIMENSIONS; d++) {
    result |= localVertexNumber(d)+_discreteOffset(d) == 0;
    assertionEquals(getCellsPerAxis(), getVerticesPerAxis()-1);
    result |= localVertexNumber(d)+_discreteOffset(d) == getCellsPerAxis();
  }
  return result;
}
