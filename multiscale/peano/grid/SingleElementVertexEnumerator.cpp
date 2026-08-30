#include "peano/grid/SingleElementVertexEnumerator.h"

peano::grid::SingleElementVertexEnumerator::SingleElementVertexEnumerator(
  const Vector &  cellSize,
  const Vector &  cellOffset,
  int             level
):
  _cellSize( cellSize ),
  _cellOffset( cellOffset ),
  _level(level) {
}

peano::grid::SingleElementVertexEnumerator::~SingleElementVertexEnumerator() {

}


int peano::grid::SingleElementVertexEnumerator::operator() (int localVertexNumber) const {
  return lineariseVertexIndex(localVertexNumber);
}


int peano::grid::SingleElementVertexEnumerator::lineariseVertexIndex(int localVertexNumber) {
  assertion1(localVertexNumber>=0, localVertexNumber);
  assertion1(localVertexNumber<TWO_POWER_D, localVertexNumber);
  return localVertexNumber;
}


void peano::grid::SingleElementVertexEnumerator::setCellOffset( const Vector &  cellOffset ) {
  _cellOffset = cellOffset;
}


int peano::grid::SingleElementVertexEnumerator::operator() (const LocalVertexIntegerIndex&  localVertexNumber ) const {
  return lineariseVertexIndex(localVertexNumber);
}


int peano::grid::SingleElementVertexEnumerator::lineariseVertexIndex(const LocalVertexIntegerIndex&  localVertexNumber ) {
  int result = 0;
  int offset = 1;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion1( localVertexNumber(d)>=0, localVertexNumber );
    assertion1( localVertexNumber(d)<=1, localVertexNumber );
    result += localVertexNumber(d)*offset;
    offset *=2;
  }
  return result;
}


int peano::grid::SingleElementVertexEnumerator::operator() (const LocalVertexBitsetIndex&   localVertexNumber ) const {
  return lineariseVertexIndex(localVertexNumber);
}



int peano::grid::SingleElementVertexEnumerator::lineariseVertexIndex(const LocalVertexBitsetIndex&   localVertexNumber ) {
  return static_cast<int>(localVertexNumber.to_ulong());
}


std::string peano::grid::SingleElementVertexEnumerator::toString() const {
  std::ostringstream msg;
  msg << "(cell-size:" << _cellSize
      << ",cell-offset:" << _cellOffset
      << ")[type=SingleElementVertexEnumerator]";
  return msg.str();
}


peano::grid::SingleElementVertexEnumerator::Vector
peano::grid::SingleElementVertexEnumerator::getVertexPosition(
  int            localVertexNumber
) const {
  Vector result = _cellOffset;
  assertion(false); // not implemented yet
  return result;
}


int peano::grid::SingleElementVertexEnumerator::getLevel() const {
  return _level;
}


peano::grid::SingleElementVertexEnumerator::Vector
peano::grid::SingleElementVertexEnumerator::getVertexPosition(
  const LocalVertexIntegerIndex&  localVertexNumber
) const {
  Vector result = _cellOffset;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion( localVertexNumber(d)==0 || localVertexNumber(d)==1 );
    if (localVertexNumber(d)==1) {
      result(d) += _cellSize(d);
    }
  }
  return result;
}


peano::grid::VertexEnumerator::Vector peano::grid::SingleElementVertexEnumerator::getCellSize() const {
  return _cellSize;
}


peano::grid::VertexEnumerator::Vector peano::grid::SingleElementVertexEnumerator::getCellCenter() const {
  return getVertexPosition() + _cellSize/2.0;
}


peano::grid::SingleElementVertexEnumerator::Vector
peano::grid::SingleElementVertexEnumerator::getVertexPosition(
  const LocalVertexBitsetIndex& localVertexNumber
) const {
  Vector result = _cellOffset;
  for (int d=0; d<DIMENSIONS; d++) {
    if (localVertexNumber[d]) {
      result(d) += _cellSize(d);
    }
  }
  return result;
}


peano::grid::SingleElementVertexEnumerator::Vector
peano::grid::SingleElementVertexEnumerator::getVertexPosition() const {
  return _cellOffset;
}


peano::grid::CellFlags peano::grid::SingleElementVertexEnumerator::getCellFlags() const {
  return peano::grid::NotStationary;
}


int peano::grid::SingleElementVertexEnumerator::cell(const LocalVertexIntegerIndex& localVertexNumber) const {
  return 0;
}


bool peano::grid::SingleElementVertexEnumerator::overlaps(const Vector& offset, const Vector& size) const {
  bool result = true;
  for (int d=0; d<DIMENSIONS; d++) {
    result &= smallerEqualsUpToRelativeTolerance( offset(d),         _cellOffset(d)+_cellSize(d) );
    result &= greaterEqualsUpToRelativeTolerance( offset(d)+size(d), _cellOffset(d) );
  }
  return result;
}


bool peano::grid::SingleElementVertexEnumerator::isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const {
  return true;
}
