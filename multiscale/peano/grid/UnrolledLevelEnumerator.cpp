#include "peano/grid/UnrolledLevelEnumerator.h"

#include "peano/utils/Loop.h"

tarch::logging::Log  peano::grid::UnrolledLevelEnumerator::_log( "peano::grid::UnrolledLevelEnumerator" );


int peano::grid::UnrolledLevelEnumerator::lineariseCellIndex( const LocalVertexIntegerIndex& cellPosition ) const {
  logTraceInWith1Argument( "lineariseCellIndex(...)", cellPosition );
  int base   = 1;
  int result = 0;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion2(cellPosition(d)>=0,cellPosition,toString());
    assertion2(cellPosition(d)<_CellsPerAxis,cellPosition,toString());
	  result += cellPosition(d)*base;
	  base   *= _CellsPerAxis;
  }
  assertion( result>= 0 );
  logTraceOutWith1Argument( "lineariseCellIndex(...)", result );
  return result;
}


int peano::grid::UnrolledLevelEnumerator::lineariseVertexIndex( const LocalVertexIntegerIndex& vertexPosition ) const {
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


peano::grid::UnrolledLevelEnumerator::~UnrolledLevelEnumerator() {
}


peano::grid::UnrolledLevelEnumerator::UnrolledLevelEnumerator(const UnrolledLevelEnumerator& enumerator):
  _discreteOffset(      enumerator._discreteOffset ),
  _fineGridCellSize(    enumerator._fineGridCellSize ),
  _domainOffset(        enumerator._domainOffset ),
  _level(               enumerator._level ),
  _adjacentCellsHeight( enumerator._adjacentCellsHeight ),
  _CellsPerAxis(        enumerator._CellsPerAxis ),
  _VerticesPerAxis(     enumerator._VerticesPerAxis ) {
}


peano::grid::UnrolledLevelEnumerator::UnrolledLevelEnumerator(
  const Vector&                coarsestGridCellSize,
  const Vector&                domainOffset,
  int                          coarseGridEnumeratorsLevel,
  int                          totalHeightOfSubtree,
  int                          relativeLevelOfThisEnumerator
):
  _discreteOffset(0),
  _fineGridCellSize(),
  _domainOffset(domainOffset),
  _level(coarseGridEnumeratorsLevel+relativeLevelOfThisEnumerator),
  _adjacentCellsHeight(static_cast<peano::grid::CellFlags>(totalHeightOfSubtree-relativeLevelOfThisEnumerator)),
  _CellsPerAxis(threePowI(relativeLevelOfThisEnumerator)),
  _VerticesPerAxis(_CellsPerAxis+1) {
  logTraceInWith5Arguments( "UnrolledLevelEnumerator(...)", coarsestGridCellSize, domainOffset, coarseGridEnumeratorsLevel, totalHeightOfSubtree, relativeLevelOfThisEnumerator );

  assertion(totalHeightOfSubtree>0);
  assertion(totalHeightOfSubtree>=relativeLevelOfThisEnumerator);

  const int ThreePowerActualLevel = threePowI(relativeLevelOfThisEnumerator);

  _fineGridCellSize = coarsestGridCellSize / static_cast<double>(ThreePowerActualLevel);

  logTraceOutWith7Arguments( "UnrolledLevelEnumerator(...)", _discreteOffset, _fineGridCellSize, _domainOffset, _level, _adjacentCellsHeight, _CellsPerAxis, _VerticesPerAxis );
}


int peano::grid::UnrolledLevelEnumerator::getCellsPerAxis() const {
  logTraceIn( "getCellsPerAxis()" );
  logTraceOutWith1Argument( "getCellsPerAxis()", _CellsPerAxis );
  return _CellsPerAxis;
}


int peano::grid::UnrolledLevelEnumerator::getVerticesPerAxis() const {
  logTraceIn( "getVerticesPerAxis()" );
  logTraceOutWith1Argument( "getVerticesPerAxis()", _VerticesPerAxis );
  return _VerticesPerAxis;
}


void peano::grid::UnrolledLevelEnumerator::setOffset(const LocalVertexIntegerIndex&  gridPointOffset) {
  logTraceInWith1Argument( "setOffset(Vector)", gridPointOffset );

  for (int d=0; d<DIMENSIONS; d++) {
    assertion2( gridPointOffset(d)>=0,             gridPointOffset, toString() );
    assertion2( gridPointOffset(d)<=_CellsPerAxis, gridPointOffset, toString() ); // if they are equal, we may only access the left/bottom points due to the enumerator
  }
  _discreteOffset = gridPointOffset;

  logTraceOut( "setOffset(Vector)" );
}



peano::grid::VertexEnumerator::LocalVertexIntegerIndex peano::grid::UnrolledLevelEnumerator::getOffset() const {
  return _discreteOffset;
}


int peano::grid::UnrolledLevelEnumerator::operator() (int localVertexNumber) const {
  logTraceInWith1Argument( "operator()(int)", localVertexNumber );
  peano::grid::UnrolledLevelEnumerator::LocalVertexIntegerIndex localOffset;
  int base   = TWO_POWER_D_DIVIDED_BY_TWO;
  for (int d=DIMENSIONS-1; d>=0; d--) {
	  localOffset(d)     = localVertexNumber / base;
	  assertion5( localOffset(d)>=0, localOffset, localVertexNumber, d, base, _discreteOffset );
	  assertion5( localOffset(d)<=1, localOffset, localVertexNumber, d, base, _discreteOffset );
	  localVertexNumber -= localOffset(d) * base;
    base              /= 2;
  }
  localOffset += _discreteOffset;
  int result = lineariseVertexIndex( localOffset );
  logTraceOutWith1Argument( "operator()(int)", result );
  return result;
}


int peano::grid::UnrolledLevelEnumerator::operator() (const LocalVertexIntegerIndex&  localVertexNumber ) const {
  return lineariseVertexIndex(localVertexNumber+_discreteOffset);
}


int peano::grid::UnrolledLevelEnumerator::operator() (const LocalVertexBitsetIndex&   localVertexNumber ) const {
  return (*this)( static_cast<int>( localVertexNumber.to_ulong()) );
}


peano::grid::UnrolledLevelEnumerator::Vector peano::grid::UnrolledLevelEnumerator::getVertexPosition(int localVertexNumber) const {
  return getVertexPosition(peano::utils::dDelinearisedWithoutLookup(localVertexNumber, _VerticesPerAxis));
}


peano::grid::UnrolledLevelEnumerator::Vector peano::grid::UnrolledLevelEnumerator::getVertexPosition(const LocalVertexIntegerIndex& localVertexNumber ) const {
  peano::grid::UnrolledLevelEnumerator::Vector result( _domainOffset );
  for(int d=0; d<DIMENSIONS;d++) {
	  double delta = localVertexNumber(d) + _discreteOffset(d);
	  result(d) += delta * _fineGridCellSize(d);
  }
  return result;
}


peano::grid::UnrolledLevelEnumerator::Vector peano::grid::UnrolledLevelEnumerator::getVertexPosition(const LocalVertexBitsetIndex& localVertexNumber ) const {
  return (*this)(static_cast<int>( localVertexNumber.to_ulong()) );
}


peano::grid::UnrolledLevelEnumerator::Vector peano::grid::UnrolledLevelEnumerator::getVertexPosition() const {
  return getVertexPosition(LocalVertexIntegerIndex(0));
}


peano::grid::UnrolledLevelEnumerator::Vector peano::grid::UnrolledLevelEnumerator::getCellCenter() const {
  return getVertexPosition() + _fineGridCellSize/2.0;
}


peano::grid::UnrolledLevelEnumerator::Vector peano::grid::UnrolledLevelEnumerator::getCellSize() const {
  return _fineGridCellSize;
}


std::string peano::grid::UnrolledLevelEnumerator::toString() const {
  std::ostringstream out;
  out << "(domain-offset:" << _domainOffset
	  << ",discrete-offset:" << _discreteOffset
	  << ",cell-size:" << _fineGridCellSize
	  << ",level:" << _level
	  << ",adj-flags:" << _adjacentCellsHeight
	  << ",cells-per-axis:" << _CellsPerAxis
	  << ",vertices-per-axis:" << _VerticesPerAxis
	  << ")[type=UnrolledLevelEnumerator]";
  return out.str();
}


int peano::grid::UnrolledLevelEnumerator::getLevel() const {
  return _level;
}


peano::grid::UnrolledLevelEnumerator::LocalVertexIntegerIndex
peano::grid::UnrolledLevelEnumerator::getVertexPositionOnCoarserLevel(const LocalVertexIntegerIndex& index ) {
  assertion( isVertexPositionAtCoarseVertexsPosition(index) );
  peano::grid::UnrolledLevelEnumerator::LocalVertexIntegerIndex result;
  for (int d=0; d<DIMENSIONS; d++ ) {
    result(d) = index(d)/3;
  }
  return result;
}


bool peano::grid::UnrolledLevelEnumerator::isVertexPositionAtCoarseVertexsPosition(const LocalVertexIntegerIndex& index ) {
  bool result = true;
  for (int d=0; d<DIMENSIONS; d++ ) {
    result &= ((index(d)==0) || (index(d)==3));
  }
  return result;
}


peano::grid::CellFlags peano::grid::UnrolledLevelEnumerator::getCellFlags() const {
  return _adjacentCellsHeight;
}


bool peano::grid::UnrolledLevelEnumerator::isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const {
  bool result = false;
  for (int d=0; d<DIMENSIONS; d++) {
    result |= localVertexNumber(d)+_discreteOffset(d) == 0;
    assertionEquals(getCellsPerAxis(), getVerticesPerAxis()-1);
    result |= localVertexNumber(d)+_discreteOffset(d) == getCellsPerAxis();
  }
  return result;
}


int peano::grid::UnrolledLevelEnumerator::cell(const LocalVertexIntegerIndex& localVertexNumber) const {
  assertionMsg( false, "not supported yet" );
  return 0;
}


bool peano::grid::UnrolledLevelEnumerator::overlaps(const Vector& offset, const Vector& size) const {
  bool result = true;
  for (int d=0; d<DIMENSIONS; d++) {
    result &= smallerEqualsUpToRelativeTolerance( offset(d),         _domainOffset(d)+(_discreteOffset(d)+1.0)*_fineGridCellSize(d));
    result &= greaterEqualsUpToRelativeTolerance( offset(d)+size(d), _domainOffset(d)+_discreteOffset(d)*_fineGridCellSize(d));
  }
  return result;
}
