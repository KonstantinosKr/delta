#include "peano/datatraversal/dForRange.h"


tarch::logging::Log peano::datatraversal::dForRange::_log( "peano::datatraversal::dForRange" );


peano::datatraversal::dForRange::dForRange(const dForRange& range):
  _offset( range._offset),
  _range( range._range ),
  _grainSize( range._grainSize ) {
  logTraceInWith1Argument( "dForRange(dForRange)", range.toString() );
  assertion3( _grainSize>0, _offset, _range, _grainSize );
  logTraceOut( "dForRange(dForRange)" );
}


peano::datatraversal::dForRange::dForRange( const tarch::la::Vector<DIMENSIONS,int>&  range, int grainSize ):
  _offset(0),
  _range(range),
  _grainSize(grainSize) {
  logTraceInWith2Arguments( "dForRange(...)", range, grainSize );
  for (int d=0; d<DIMENSIONS; d++) {
    assertion3( range(d)>=1, toString(), range, grainSize );
  }
  logTraceOut( "dForRange(...)" );
}


peano::datatraversal::dForRange::dForRange( dForRange& range, Split ):
  _offset(range._offset),
  _range(range._range),
  _grainSize( range._grainSize) {
  logTraceInWith1Argument( "dForRange(Range,Split)", range.toString() );

  assertion(range.is_divisible());

  int splitAlongDimension = tarch::la::indexMax(_range);

  int oldRangeSizeAlongDimension = _range(splitAlongDimension);
  _range(splitAlongDimension) =  oldRangeSizeAlongDimension / 2;

  range._range(splitAlongDimension)  = oldRangeSizeAlongDimension - _range(splitAlongDimension);
  range._offset(splitAlongDimension) = range._offset(splitAlongDimension) + oldRangeSizeAlongDimension - range._range(splitAlongDimension);

  logTraceOutWith2Arguments( "dForRange(Range,Split)", toString(), range.toString() );
}


bool peano::datatraversal::dForRange::empty() const {
  logTraceIn( "empty()" );
  bool result = tarch::la::volume(_range)==0;
  assertion( !is_divisible() || !result );
  logTraceOutWith1Argument( "empty()", result );
  return result;
}


bool peano::datatraversal::dForRange::is_divisible() const {
  logTraceInWith1Argument( "is_divisible()", toString() );
  bool result = tarch::la::volume(_range) > _grainSize;
  logTraceOutWith1Argument( "is_divisible()", result );
  return result;
}


std::string peano::datatraversal::dForRange::toString() const {
  std::ostringstream msg;
  msg << "(range:" << _range << ",offset:" << _offset << ",grain-size:" << _grainSize << ")";
  return msg.str();
}


tarch::la::Vector<DIMENSIONS,int> peano::datatraversal::dForRange::getOffset() const {
  return _offset;
}


tarch::la::Vector<DIMENSIONS,int> peano::datatraversal::dForRange::getRange() const {
  return _range;
}
