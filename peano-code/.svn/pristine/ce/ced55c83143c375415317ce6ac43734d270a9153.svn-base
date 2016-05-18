#include "peano/heap/AbstractHeap.h"


std::set< peano::heap::AbstractHeap* >  peano::heap::AbstractHeap::_registeredHeaps;
tarch::logging::Log                     peano::heap::AbstractHeap::_log( "peano::heap::AbstractHeap" );


void peano::heap::AbstractHeap::registerHeap( AbstractHeap* newHeap ) {
  _registeredHeaps.insert( newHeap );
}


void peano::heap::AbstractHeap::allHeapsStartToSendSynchronousData() {
  logTraceIn( "allHeapsStartToSendSynchronousData()" );
  for (
    std::set< peano::heap::AbstractHeap* >::iterator p = _registeredHeaps.begin();
    p != _registeredHeaps.end();
    p++
  ) {
    (**p).startToSendSynchronousData();
  }
  logTraceOut( "allHeapsStartToSendSynchronousData()" );
}


void peano::heap::AbstractHeap::allHeapsStartToSendBoundaryData(bool isTraversalInverted) {
  logTraceIn( "allHeapsStartToSendSynchronousData()" );
  for (
    std::set< peano::heap::AbstractHeap* >::iterator p = _registeredHeaps.begin();
    p != _registeredHeaps.end();
    p++
  ) {
    (**p).startToSendBoundaryData(isTraversalInverted);
  }
  logTraceOut( "allHeapsStartToSendSynchronousData()" );
}


void peano::heap::AbstractHeap::allHeapsFinishedToSendSynchronousData() {
  logTraceIn( "allHeapsStartToSendSynchronousData()" );
  for (
    std::set< peano::heap::AbstractHeap* >::iterator p = _registeredHeaps.begin();
    p != _registeredHeaps.end();
    p++
  ) {
    (**p).finishedToSendSynchronousData();
  }
  logTraceOut( "allHeapsStartToSendSynchronousData()" );
}


void peano::heap::AbstractHeap::allHeapsFinishedToSendBoundaryData(bool isTraversalInverted) {
  logTraceIn( "allHeapsStartToSendSynchronousData()" );
  for (
    std::set< peano::heap::AbstractHeap* >::iterator p = _registeredHeaps.begin();
    p != _registeredHeaps.end();
    p++
  ) {
    (**p).finishedToSendBoundaryData(isTraversalInverted);
  }
  logTraceOut( "allHeapsStartToSendSynchronousData()" );
}
