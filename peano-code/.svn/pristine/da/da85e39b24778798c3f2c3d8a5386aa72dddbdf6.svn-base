#include "peano/heap/SendReceiveTask.h"

#include "tarch/la/VectorCompare.h"

tarch::logging::Log  peano::heap::SendReceiveTask<double>::_log( "peano::heap::SendReceiveTask<double>" );


#ifdef Asserts
peano::heap::SendReceiveTask<double>::SendReceiveTask():
  _rank(-1),
  _data(0) {
}
#endif


bool peano::heap::SendReceiveTask<double>::fits(
  const tarch::la::Vector<DIMENSIONS, double>&  position,
  int                                           level
) const {
  #ifdef Asserts
  return
    (_metaInformation.getLevel() == -1) ||
    (
      _metaInformation.getLevel() == level &&
      tarch::la::equals(_metaInformation.getPosition(), position)
    );
  #else
  return true;
  #endif
}


void peano::heap::SendReceiveTask<double>::setInvalid() {
   #if defined(Asserts)
  _metaInformation.setLevel(-1);
  #endif
  _metaInformation.setLength(0);
  _data = nullptr;
}


void peano::heap::SendReceiveTask<double>::freeMemoryOfSendTask() {
  if (_freeDataPointer && _metaInformation.getLength()>0) {
    delete[] _data;
  }
}



void peano::heap::SendReceiveTask<double>::sendDataDirectlyFromBuffer(const std::vector<double>& data) {
  assertion( !data.empty() );
  assertion( _data==0 );

  _freeDataPointer = false;
  _data            = const_cast< double* >( data.data() );
}


void peano::heap::SendReceiveTask<double>::wrapData(const std::vector<double>& data) {
  assertion( !data.empty() );
  assertion( _data==0 );

  _freeDataPointer = true;

  _data = new double[data.size()];
  for (int i=0; i<static_cast<int>( data.size() ); i++) {
    _data[i] = data[i];
  }
}


void peano::heap::SendReceiveTask<double>::triggerSend(int tag) {
  assertion( _data!=nullptr );
  assertion( _metaInformation.getLength()>0 );

  #ifdef Parallel
  const int result = MPI_Isend(
    _data, _metaInformation.getLength(), MPI_DOUBLE, _rank,
    tag,
    tarch::parallel::Node::getInstance().getCommunicator(), &_request
  );

  if ( result != MPI_SUCCESS ) {
    logError(
      "triggerSend(int)", "failed to send heap data to node "
      << _rank << ": " << tarch::parallel::MPIReturnValueToString(result)
    );
  }
  #else
  assertionMsg( false, "should not be called if compiled without -DParallel" );
  #endif
}


void peano::heap::SendReceiveTask<double>::triggerReceive(int tag) {
  assertion( _rank >= 0 );
  assertion( _data==0 );

  #ifdef Parallel
  logTraceInWith2Arguments( "triggerReceive(int)", tag, _metaInformation.toString() );
  _data = new typename SendReceiveTask<double>::MPIdouble[ _metaInformation.getLength() ];

  const int  result = MPI_Irecv(
    _data, _metaInformation.getLength(), MPIdouble::doubletype,
    _rank, tag, tarch::parallel::Node::getInstance().getCommunicator(),
    &_request
  );
  if ( result != MPI_SUCCESS ) {
    logError(
      "triggerReceive()",
      "failed to receive heap data from node "
      << _rank << ": " << tarch::parallel::MPIReturnValueToString(result)
    );
  }
  logTraceOut( "triggerReceive(int)" );
  #else
  assertionMsg( false, "should not be called if compiled without -DParallel" );
  #endif
}


std::vector<double> peano::heap::SendReceiveTask<double>::unwrapDataAndFreeMemory() {
  logTraceInWith1Argument( "unwrapdoubleAndFreeMemory()", _metaInformation.toString() );

  std::vector<double> result;

  assertion( _metaInformation.getLength()>=0 );
  if (_metaInformation.getLength()>0) {
    assertion( _data!=0 );
    for (int i=0; i<_metaInformation.getLength(); i++) {
      result.push_back(_data[i]);
    }

    delete[] _data;
  }

  logTraceOutWith1Argument( "unwrapdoubleAndFreeMemory()", result.size() );
  return result;
}


std::string peano::heap::SendReceiveTask<double>::toString() const {
  std::ostringstream out;
  out << "(" << _metaInformation.toString() << ",rank=" << _rank << ",data=" << (_data==nullptr ? "no" : "yes") << ")";
  return out.str();
}
