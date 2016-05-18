#include "peano/geometry/Hexahedron.h"
#include "peano/utils/Loop.h"


tarch::logging::Log peano::geometry::Hexahedron::_log( "peano::geometry::Hexahedron" );


peano::geometry::Hexahedron::Hexahedron(
  const tarch::la::Vector<DIMENSIONS,double>&  width,
  const tarch::la::Vector<DIMENSIONS,double>&  offset
):
  _offset(offset),
  _width(width) {

  for (int d=0; d<DIMENSIONS; d++) {
    assertion2( _width(d)>0.0, _width, _offset);
  }
  logDebug(
    "Hexahedron(...)",
    "created hexahedron with width " << _width
      << " at " << _offset << " (left bottom (front) point)"
  );
}


peano::geometry::Hexahedron::~Hexahedron() {}


bool peano::geometry::Hexahedron::domainHasChanged( const tarch::la::Vector<DIMENSIONS,double>& x, const tarch::la::Vector<DIMENSIONS,double> &resolution ) {
  return false;
}


bool peano::geometry::Hexahedron::isInsideOpenHexahedron( const tarch::la::Vector<DIMENSIONS,double>& x ) const {
  logTraceInWith3Arguments("isInsideOpenHexahedron(...)", x, _offset, _width);
  bool result = true;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion( _width(d) >= 0.0 );
    result &= tarch::la::greater(x(d),_offset(d));
    result &= tarch::la::smaller(x(d),_offset(d)+_width(d));
  }
  logTraceOutWith1Argument("isInsideOpenHexahedron(...)", result);
  return result;
}


bool peano::geometry::Hexahedron::isInsideClosedHexahedron( const tarch::la::Vector<DIMENSIONS,double>& x ) const {
  logTraceInWith1Argument("isInsideClosedHexahedron(...)", x);
  bool result = true;
  for (int d=0; d<DIMENSIONS; d++) {
    assertion( _width(d) >= 0.0 );
    result &= !tarch::la::smaller(x(d),_offset(d));
    result &= !tarch::la::greater(x(d),_offset(d)+_width(d));
  }
  logTraceOutWith1Argument("isInsideClosedHexahedron(...)", result);
  return result;
}


tarch::la::Vector<DIMENSIONS,double> peano::geometry::Hexahedron::getOffset() const {
  return _offset;
}


tarch::la::Vector<DIMENSIONS,double> peano::geometry::Hexahedron::getBoundingBox() const {
  return _width;
}


bool peano::geometry::Hexahedron::isOutsideClosedDomain( const tarch::la::Vector<DIMENSIONS,double>& x ) {
  return !isInsideClosedHexahedron(x);
}


bool peano::geometry::Hexahedron::isCompletelyOutside( const tarch::la::Vector<DIMENSIONS,double>& x, const tarch::la::Vector<DIMENSIONS,double> &resolution ) {
  bool result = false;
  for( int i = 0; i < DIMENSIONS; i++ ){
    bool dimResult = true;
    dimResult &= !(tarch::la::smaller(x(i) - resolution(i), _offset(i) + _width(i)) &&
                tarch::la::greater(x(i) - resolution(i), _offset(i)));
    dimResult &= !(tarch::la::smaller(x(i) + resolution(i), _offset(i) + _width(i)) &&
                tarch::la::greater(x(i) + resolution(i), _offset(i)));
    dimResult &= !(!tarch::la::greater(x(i) - resolution(i), _offset(i)) &&
                !tarch::la::smaller(x(i) + resolution(i), _offset(i) + _width(i)));
    result |= dimResult;
  }

  return result;
}


bool peano::geometry::Hexahedron::isCompletelyInside( const tarch::la::Vector<DIMENSIONS,double>& x, const tarch::la::Vector<DIMENSIONS,double> &resolution ) {
  logTraceInWith2Arguments( "isCompletelyInside(...)", x, resolution);
  bool result = isInsideOpenHexahedron(x);

  dfor2(i)
    tarch::la::Vector<DIMENSIONS,double> currentPoint0;
    tarch::la::Vector<DIMENSIONS,double> currentPoint1;
    for (int d=0; d<DIMENSIONS; d++) {
      currentPoint0(d) = (i(d)==0) ? x(d)-0.5*resolution(d) : x(d)+0.5*resolution(d);
      currentPoint1(d) = (i(d)==0) ? x(d)-1.0*resolution(d) : x(d)+1.0*resolution(d);
    }
    result &= isInsideClosedHexahedron(currentPoint0);
    result &= isInsideClosedHexahedron(currentPoint1);
   enddforx

  logTraceOutWith1Argument("isCompletelyInside(...)", result);
  return result;
}


bool peano::geometry::Hexahedron::refineOuterCellWithExclusivelyOuterVerticesAsItIntersectsDomain(
  const tarch::la::Vector<DIMENSIONS,double>&   x,
  const tarch::la::Vector<DIMENSIONS,double>&   resolution
) const {
  logTraceInWith3Arguments("refineOuterCellWithExclusivelyOuterVerticesAsItIntersectsDomain(...)", x, resolution, _width);
  bool result = tarch::la::oneGreater(resolution, _width / 2.0);

  logTraceOutWith1Argument("refineOuterCellWithExclusivelyOuterVerticesAsItIntersectsDomain(...)", result);
  return result;
}
