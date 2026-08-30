#include "peano/heap/SendReceiveTask.h"

#include "tarch/la/VectorCompare.h"

tarch::logging::Log  peano::heap::SendReceiveTask<double>::_log( "peano::heap::SendReceiveTask<double>" );
tarch::logging::Log  peano::heap::SendReceiveTask<char>::_log(   "peano::heap::SendReceiveTask<char>" );


peano::heap::SendReceiveTask<double>::SendReceiveTask():
  #ifdef Parallel
  _request( MPI_REQUEST_NULL ),
  #endif
  _rank(-1),
  _data(0),
  _dataExchangeHasCompleted(false) {
}


bool peano::heap::SendReceiveTask<double>::hasDataExchangeFinished() {
  #ifdef Parallel
  if (!_dataExchangeHasCompleted) {
    int finishedWait;
    if(_metaInformation.getLength() > 0 and _data!=nullptr ) {
      MPI_Test(&(_request), &finishedWait, MPI_STATUS_IGNORE);
      _dataExchangeHasCompleted |= finishedWait!=0;
      return _dataExchangeHasCompleted;
    }
  }
  return true;
  #endif
  return true;
}


bool peano::heap::SendReceiveTask<double>::fits(
  const tarch::la::Vector<DIMENSIONS, double>&  position,
  int                                           level
) const {
  #ifdef Asserts
  const double tolerance =
    tarch::la::NUMERICAL_ZERO_DIFFERENCE *
    std::max(
      1.0, std::max(
        tarch::la::maxAbs(_metaInformation.getPosition()), tarch::la::maxAbs(position)
      )
    );

  return
    (_metaInformation.getLevel() == -1) ||
    (
      _metaInformation.getLevel() == level &&
      tarch::la::equals(_metaInformation.getPosition(), position, tolerance)
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


void peano::heap::SendReceiveTask<double>::freeMemory() {
  if (_freeDataPointer && _metaInformation.getLength()>0) {
    delete[] _data;
  }
  _metaInformation.setLength(0);
}


double* peano::heap::SendReceiveTask<double>::data() {
  return _data;
}


const double* peano::heap::SendReceiveTask<double>::data() const {
  return _data;
}


peano::heap::SendReceiveTask<double>::MetaInformation& peano::heap::SendReceiveTask<double>::getMetaInformation() {
  return _metaInformation;
}


peano::heap::SendReceiveTask<double>::MetaInformation peano::heap::SendReceiveTask<double>::getMetaInformation() const {
  return _metaInformation;
}


void peano::heap::SendReceiveTask<double>::sendDataDirectlyFromBuffer(const double* const data) {
  assertion( _metaInformation.getLength()>0 );
  assertion( _data==0 );

  _freeDataPointer = false;
  _data            = const_cast< double* >( data );
}


void peano::heap::SendReceiveTask<double>::wrapData(const double* const data) {
  assertion( _metaInformation.getLength()>0 );
  assertion( _data==0 );

  _freeDataPointer = true;

  _data = new (std::nothrow) double[ _metaInformation.getLength() ];
  if (_data==nullptr) {
    logError( "wrapData(std::vector<double>)", "memory allocation for wrapped data failed. Terminate" );
    exit(-1);
  }
  for (int i=0; i<_metaInformation.getLength(); i++) {
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


int peano::heap::SendReceiveTask<double>::getRank() const {
  return _rank;
}


void peano::heap::SendReceiveTask<double>::setRank(int value) {
  _rank = value;
}


void peano::heap::SendReceiveTask<double>::triggerReceive(int tag) {
  assertion( _rank >= 0 );
  assertion( _data==0 );

  #ifdef Parallel
  logTraceInWith2Arguments( "triggerReceive(int)", tag, _metaInformation.toString() );
  _data = new (std::nothrow) double[ _metaInformation.getLength() ];
  if (_data==nullptr) {
    logError( "triggerReceive(int)", "memory allocation failed. Terminate" );
    exit(-1);
  }

  #if defined(NonblockingHeapDataReceives)
  const int  result = MPI_Irecv(
    _data, _metaInformation.getLength(), MPI_DOUBLE,
    _rank, tag, tarch::parallel::Node::getInstance().getCommunicator(),
    &_request
  );
  #else
  const int  result = MPI_Recv(
    _data, _metaInformation.getLength(), MPI_DOUBLE,
    _rank, tag, tarch::parallel::Node::getInstance().getCommunicator(),
  	MPI_STATUS_IGNORE
  );
  _request = MPI_REQUEST_NULL;
  #endif

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


std::string peano::heap::SendReceiveTask<double>::toString() const {
  std::ostringstream out;
  out << "(" << _metaInformation.toString() << ",rank=" << _rank << ",data=" << (_data==nullptr ? "no" : "yes") << ")";
  return out.str();
}




peano::heap::SendReceiveTask<char>::SendReceiveTask():
  #ifdef Parallel
  _request( MPI_REQUEST_NULL ),
  #endif
  _rank(-1),
  _data(0),
  _dataExchangeHasCompleted(false)  {
}


bool peano::heap::SendReceiveTask<char>::hasDataExchangeFinished() {
  #ifdef Parallel
  if (!_dataExchangeHasCompleted) {
    int finishedWait;
    if(_metaInformation.getLength() > 0 and _data!=nullptr ) {
      MPI_Test(&(_request), &finishedWait, MPI_STATUS_IGNORE);
      _dataExchangeHasCompleted |= finishedWait!=0;
      return _dataExchangeHasCompleted;
    }
  }
  return true;
  #endif
  return true;
}


bool peano::heap::SendReceiveTask<char>::fits(
  const tarch::la::Vector<DIMENSIONS, double>&  position,
  int                                           level
) const {
  #ifdef Asserts
  const double tolerance =
    tarch::la::NUMERICAL_ZERO_DIFFERENCE *
    std::max(
      1.0, std::max(
        tarch::la::maxAbs(_metaInformation.getPosition()), tarch::la::maxAbs(position)
      )
    );

  return
    (_metaInformation.getLevel() == -1) ||
    (
      _metaInformation.getLevel() == level &&
      tarch::la::equals(_metaInformation.getPosition(), position, tolerance)
    );
  #else
  return true;
  #endif
}


void peano::heap::SendReceiveTask<char>::setInvalid() {
   #if defined(Asserts)
  _metaInformation.setLevel(-1);
  #endif
  _metaInformation.setLength(0);
  _data = nullptr;
}


void peano::heap::SendReceiveTask<char>::freeMemory() {
  if (_freeDataPointer && _metaInformation.getLength()>0) {
    delete[] _data;
  }
  _metaInformation.setLength(0);
}


int peano::heap::SendReceiveTask<char>::getRank() const {
  return _rank;
}


void peano::heap::SendReceiveTask<char>::sendDataDirectlyFromBuffer(const char* const data) {
  assertion( _metaInformation.getLength()>0 );
  assertion( _data==0 );

  _freeDataPointer = false;
  _data            = const_cast< char* >( data );
}


void peano::heap::SendReceiveTask<char>::wrapData(const char* const data) {
  assertion( _metaInformation.getLength()>0 );
  assertion( _data==0 );

  _freeDataPointer = true;

  _data = new (std::nothrow) char[ _metaInformation.getLength() ];
  if (_data==nullptr) {
    logError( "wrapData(std::vector<char>)", "memory allocation for wrapped data failed. Terminate" );
    exit(-1);
  }
  for (int i=0; i<_metaInformation.getLength(); i++) {
    _data[i] = data[i];
  }
}


void peano::heap::SendReceiveTask<char>::triggerSend(int tag) {
  assertion( _data!=nullptr );
  assertion( _metaInformation.getLength()>0 );

  #ifdef Parallel
  const int result = MPI_Isend(
    _data, _metaInformation.getLength(), MPI_CHAR, _rank,
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


void peano::heap::SendReceiveTask<char>::triggerReceive(int tag) {
  assertion( _rank >= 0 );
  assertion( _data==0 );

  #ifdef Parallel
  logTraceInWith2Arguments( "triggerReceive(int)", tag, _metaInformation.toString() );
  _data = new (std::nothrow) char[ _metaInformation.getLength() ];
  if (_data==nullptr) {
    logError( "triggerReceive(int)", "memory allocation failed. Terminate" );
    exit(-1);
  }

  #if defined(NonblockingHeapDataReceives)
  const int  result = MPI_Irecv(
    _data, _metaInformation.getLength(), MPI_CHAR,
    _rank, tag, tarch::parallel::Node::getInstance().getCommunicator(),
    &_request
  );
  #else
  const int  result = MPI_Recv(
    _data, _metaInformation.getLength(), MPI_CHAR,
    _rank, tag, tarch::parallel::Node::getInstance().getCommunicator(),
	MPI_STATUS_IGNORE
  );
  _request = MPI_REQUEST_NULL;
  #endif

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


std::string peano::heap::SendReceiveTask<char>::toString() const {
  std::ostringstream out;
  out << "(" << _metaInformation.toString() << ",rank=" << _rank << ",data=" << (_data==nullptr ? "no" : "yes") << ")";
  return out.str();
}


void peano::heap::SendReceiveTask<char>::setRank(int value) {
  _rank = value;
}


char* peano::heap::SendReceiveTask<char>::data() {
  return _data;
}


const char* peano::heap::SendReceiveTask<char>::data() const {
  return _data;
}


peano::heap::SendReceiveTask<char>::MetaInformation& peano::heap::SendReceiveTask<char>::getMetaInformation() {
  return _metaInformation;
}


peano::heap::SendReceiveTask<char>::MetaInformation  peano::heap::SendReceiveTask<char>::getMetaInformation() const {
  return _metaInformation;
}


bool peano::heap::SendReceiveTask<double>::hasCommunicationCompleted() {
  #ifdef Parallel
  if ( _metaInformation.getLength()==0 or _data==nullptr ) {
    return true;
  }
  else {
	int finishedWait;
    assertion1(_data!=nullptr, toString() );
    MPI_Test(&(_request), &finishedWait, MPI_STATUS_IGNORE);
    return finishedWait;
  }
  #else
  return true;
  #endif
}


bool peano::heap::SendReceiveTask<char>::hasCommunicationCompleted() {
  #ifdef Parallel
  if ( _metaInformation.getLength()==0 or _data==nullptr) {
    return true;
  }
  else {
	int finishedWait;
    assertion1(_data!=nullptr, toString() );
    MPI_Test(&(_request), &finishedWait, MPI_STATUS_IGNORE);
    return finishedWait;
  }
  #else
  return true;
  #endif
}

