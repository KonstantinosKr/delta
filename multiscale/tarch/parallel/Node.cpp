#include "tarch/parallel/Node.h"
#include "tarch/Assertions.h"
#include "tarch/services/ServiceRepository.h"

#include <sstream>
#include <cstdlib>

#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/multicore/MulticoreDefinitions.h"


/**
 * For the machine name. If it doesn't work, switch it off in the file
 * CompilerSpecificSettings.h.
 */
#ifdef CompilerHasUTSName
#include <sys/utsname.h>
#endif


tarch::logging::Log tarch::parallel::Node::_log("tarch::parallel::Node");


bool tarch::parallel::Node::_initIsCalled = false;

namespace {
  int tagCounter = 0;
}


void tarch::parallel::Node::releaseTag(int tag) {
  if (tag==tagCounter-1) {
    tagCounter--;
  }
}


int tarch::parallel::Node::reserveFreeTag(const std::string& fullQualifiedMessageName) {
  tagCounter++;

  // I protect the tag manually (not via log filter), as many tags are actually
  // grabbed before most applications initialise their log filters properly.
  //
  // We may not use isGlobalMaster() as this query checks whether the code is
  // properly initialised. Please note rank is -1 as long as MPI is not properly
  // initialised, i.e. any tag booking prior to the MPI initialisation is not
  // logged properly.
  if ( getInstance()._rank==getGlobalMasterRank() ) {
    tarch::logging::Log _log("tarch::parallel::Node<static>");
    logInfo(
      "reserveFreeTag()",
      "assigned message " << fullQualifiedMessageName
       << " the free tag " << tagCounter-1
    );
  }

  return tagCounter-1;
}


bool tarch::parallel::Node::isInitialised() const {
  return _initIsCalled;
}


void tarch::parallel::Node::ensureThatMessageQueuesAreEmpty( int fromRank, int tag ) {
  #ifdef Parallel
  int          flag;
  MPI_Iprobe(fromRank, tag, _communicator, &flag, MPI_STATUS_IGNORE);
  if (flag!=0) {
    plotMessageQueues();
  }
  assertion3( flag==0, fromRank, tag, getRank() );
  #endif
}


void tarch::parallel::Node::plotMessageQueues() {
  #ifdef Parallel
  int          flag;
  MPI_Status   status;
  MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, _communicator, &flag, &status);
  if (flag==0) {
    _log.error("plotMessageQueues()", "there are no messages from any sender in MPI queue");
  }
  else {
    logError(
      "plotMessageQueues()",
      "there is still a message in queue "
      " from rank " << status.MPI_SOURCE <<
      " with tag " << status.MPI_TAG
    );
  }
  #endif
}


void tarch::parallel::Node::triggerDeadlockTimeOut(
  const std::string&  className,
  const std::string&  methodName,
  int                 communicationPartnerRank,
  int                 tag,
  int                 numberOfExpectedMessages,
  const std::string&  comment
) {
  std::ostringstream out;
  out << "operation " << className << "::" << methodName << " on node "
      << getRank() << " had to wait more than " << _deadlockTimeOut
      << " seconds for " << numberOfExpectedMessages
      << " message(s) from node " << communicationPartnerRank << " with tag " << tag
      << ". Timeout. " << comment;
  _log.error( "triggerDeadlockTimeOut(...)", out.str() );

  plotMessageQueues();

  exit(DEADLOCK_EXIT_CODE);
}


void tarch::parallel::Node::writeTimeOutWarning(
  const std::string&  className,
  const std::string&  methodName,
  int                 communicationPartnerRank,
  int                 tag,
  int                 numberOfExpectedMessages
) {
  std::ostringstream out;
  out << "operation " << className << "::" << methodName << " on node "
      << getRank() << " had to wait more than " << _timeOutWarning
      << " seconds for " << numberOfExpectedMessages
      << " message(s) from node " << communicationPartnerRank << " with tag " << tag << ". Application "
      << "will terminate after " << _deadlockTimeOut << " seconds because "
      << "of a deadlock";
  _log.warning( "writeTimeOutWarning(...)", out.str() );
}


clock_t tarch::parallel::Node::getDeadlockWarningTimeStamp() const {
  clock_t result = clock() + _timeOutWarning * CLOCKS_PER_SEC;
  assertion4( result>=0, result, clock(), _timeOutWarning, CLOCKS_PER_SEC);

  return result;
}


clock_t tarch::parallel::Node::getDeadlockTimeOutTimeStamp() const {
  clock_t result = clock() + _deadlockTimeOut * CLOCKS_PER_SEC;
  assertion4( result>=0, result, clock(), _timeOutWarning, CLOCKS_PER_SEC);

  return result;
}


bool tarch::parallel::Node::isTimeOutDeadlockEnabled() const {
  return _areTimeoutsEnabled and _deadlockTimeOut > 0;
}


bool tarch::parallel::Node::isTimeOutWarningEnabled() const {
  return _areTimeoutsEnabled and _timeOutWarning > 0;
}


void tarch::parallel::Node::suspendTimeouts( bool timeoutsDisabled ) {
  _areTimeoutsEnabled = !timeoutsDisabled;
}


std::string tarch::parallel::MPIReturnValueToString( int result ) {
  std::ostringstream out;

  #ifdef Parallel
  int   resultlen;
  char* string = new char[MPI_MAX_ERROR_STRING];  // (char *)malloc(MPI_MAX_ERROR_STRING * sizeof(char));
  MPI_Error_string(result, string, &resultlen);

  int   errorclass;
  MPI_Error_class(result, &errorclass);

  out << "mpi error class: " << errorclass << "="
      << ", mpi error text: " << string;

  switch ( errorclass ) {
    case MPI_SUCCESS:      out << "MPI_SUCCESS [no error]"; break;
    case MPI_ERR_BUFFER:   out << "MPI_ERR_BUFFER [invalid buffer pointer]"; break;
    case MPI_ERR_COUNT:    out << "MPI_ERR_COUNT [invalid count argument]"; break;
    case MPI_ERR_TYPE:     out << "MPI_ERR_TYPE [invalid datatype]"; break;
    case MPI_ERR_TAG:      out << "MPI_ERR_TAG [invalid tag]"; break;
    case MPI_ERR_COMM:     out << "MPI_ERR_COMM [invalid communicator]"; break;
    case MPI_ERR_RANK:     out << "MPI_ERR_RANK [invalid rank]"; break;
    case MPI_ERR_REQUEST:  out << "MPI_ERR_REQUEST [invalid request handle]"; break;
    case MPI_ERR_ROOT:     out << "MPI_ERR_ROOT [invalid root argument]"; break;
    case MPI_ERR_GROUP:    out << "MPI_ERR_GROUP [invalid group]"; break;
    case MPI_ERR_OP:       out << "MPI_ERR_OP [invalid operation]"; break;
    case MPI_ERR_TOPOLOGY: out << "MPI_ERR_TOPOLOGY [invalid topology]"; break;
    case MPI_ERR_DIMS:     out << "MPI_ERR_DIMS [invalid dimensions]"; break;
    case MPI_ERR_ARG:      out << "MPI_ERR_ARG [invalid argument]"; break;
    case MPI_ERR_UNKNOWN:  out << "MPI_ERR_UNKNOWN [unknown error]"; break;
    case MPI_ERR_TRUNCATE: out << "MPI_ERR_TRUNCATE [message has been truncated by receiver]"; break;
    case MPI_ERR_OTHER:    out << "MPI_ERR_OTHER [other unknown error]"; break;
    case MPI_ERR_INTERN:   out << "MPI_ERR_INTERN [internal mpi error]"; break;
    default: out << "unknown"; break;
  }

  delete[] string;
  #else
  out << "compiled without -DParallel";
  #endif

  return out.str();
}


std::string tarch::parallel::MPIStatusToString( const MPI_Status& status ) {
  std::ostringstream out;
  #ifdef Parallel
  out << "status flag:"
      << " MPI_ERROR=" << status.MPI_ERROR
      << " (" << MPIReturnValueToString(status.MPI_ERROR)
      << ") ,MPI_SOURCE=" << status.MPI_SOURCE
      << ",MPI_TAG=" << status.MPI_TAG;
  #else
  out << "compiled without -DParallel";
  #endif
  return out.str();
}


#ifdef Parallel
tarch::parallel::Node::Node():
  _rank(-1),
  _numberOfProcessors(-1),
  _communicator( MPI_COMM_WORLD),
  _timeOutWarning(0),
  _deadlockTimeOut(0),
  _areTimeoutsEnabled(true) {
}
#else
tarch::parallel::Node::Node():
  _rank(0),
  _numberOfProcessors(1),
  _communicator(-1),
  _timeOutWarning(0),
  _deadlockTimeOut(0),
  _areTimeoutsEnabled(true) {
}
#endif


#ifdef Parallel
tarch::parallel::Node::Node(const parallel::Node& node):
  _rank(-1),
  _numberOfProcessors(-1),
  _communicator( MPI_COMM_WORLD) {
}
#else
tarch::parallel::Node::Node(const parallel::Node& node):
  _rank(0),
  _numberOfProcessors(-1),
  _communicator(-1) {
}
#endif


tarch::parallel::Node::~Node() {
}


void tarch::parallel::Node::shutdown() {
  #ifdef Parallel
  assertion( _rank!=-1 );

  MPI_Barrier( _communicator );
  MPI_Finalize();
  _communicator = MPI_COMM_WORLD;
  #endif

  _rank         = -1;
}


int tarch::parallel::Node::getGlobalMasterRank() {
  return 0;
}


bool tarch::parallel::Node::isGlobalMaster() const {
  #ifdef Parallel
  assertion(_initIsCalled);
  return getRank() == getGlobalMasterRank();
  #else
  return true;
  #endif
}


void tarch::parallel::Node::logStatus() const {
  std::ostringstream statusMessage;
  statusMessage << "MPI status:";

  #ifdef CompilerHasUTSName
  utsname* utsdata = new utsname();
  assertion( utsdata!=NULL );
  uname(utsdata);
  statusMessage << " nodename=" << utsdata->nodename;
  delete utsdata;
  #else
  statusMessage << " nodename=undef";
  #endif

  statusMessage << ", rank=" << _rank;
  statusMessage << ", communicator=" << _communicator;
  statusMessage << ", #processors=" << _numberOfProcessors;

  _log.info( "logStatus()", statusMessage.str() );
}


bool tarch::parallel::Node::init(int* argc, char*** argv) {
  #ifdef Parallel
  int result = MPI_SUCCESS;

  #if defined( SharedMemoryParallelisation ) && defined( MultipleThreadsMayTriggerMPICalls )

  int initThreadProvidedThreadLevelSupport;
  result = MPI_Init_thread( argc, argv, MPI_THREAD_MULTIPLE, &initThreadProvidedThreadLevelSupport );
  if (initThreadProvidedThreadLevelSupport!=MPI_THREAD_MULTIPLE ) {
    std::cerr << "warning: MPI implementation does not support MPI_THREAD_MULTIPLE. Support multithreading level is "
              << initThreadProvidedThreadLevelSupport << " instead of " << MPI_THREAD_MULTIPLE
              << ". Disable MultipleThreadsMayTriggerMPICalls in the compiler-specific settings or via -DnoMultipleThreadsMayTriggerMPICalls."<< std::endl;
    exit(-1);
  }
  #else
  result = MPI_Init( argc, argv );
  #endif

  if (result!=MPI_SUCCESS) {
    std::cerr << "init(int*,char***)\t initialisation failed: " + MPIReturnValueToString(result) + " (no logging available yet)";
    return false;
  }

  result = MPI_Comm_size( MPI_COMM_WORLD, &_numberOfProcessors );
  if (result!=MPI_SUCCESS) {
    std::cerr << "init(int*,char***)\t initialisation failed: " + MPIReturnValueToString(result) + " (no logging available yet)";
    return false;
  }

  result = MPI_Comm_rank( MPI_COMM_WORLD, &_rank );
  if (result!=MPI_SUCCESS) {
    std::cerr << "init(int*,char***)\t initialisation failed: " + MPIReturnValueToString(result) + " (no logging available yet)";
    return false;
  }

  #endif

  _initIsCalled = true;
  return true;
}


int tarch::parallel::Node::getRank() const {
  #ifdef Parallel
  assertion(_initIsCalled);
  #endif
  return _rank;
}


tarch::parallel::Node& tarch::parallel::Node::getInstance() {
  static Node singleton;
  return singleton;
}


MPI_Comm tarch::parallel::Node::getCommunicator() const {
  assertion(_initIsCalled);
  return _communicator;
}


int tarch::parallel::Node::getNumberOfNodes() const {
  #ifdef Parallel
  assertion(_initIsCalled);
  #endif
  return _numberOfProcessors;
}


void tarch::parallel::Node::setTimeOutWarning( const clock_t & value ) {
  assertion( value>=0 );
  _timeOutWarning = value;
}


void tarch::parallel::Node::setDeadlockTimeOut( const clock_t & value ) {
  assertion( value>=0 );
  _deadlockTimeOut = value;
}


void tarch::parallel::Node::setCommunicator( MPI_Comm communicator ) {
  _communicator = communicator;
}


void tarch::parallel::Node::receiveDanglingMessages() {
  static int counter = 0;
  counter++;
  if (counter>IprobeEveryKIterations) {
    #ifdef Parallel
    int          flag;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, _communicator, &flag, MPI_STATUS_IGNORE);
    if (flag) {
    #endif
      counter = 0;
      tarch::services::ServiceRepository::getInstance().receiveDanglingMessages();
    #ifdef Parallel
    }
    #endif
  }
}
