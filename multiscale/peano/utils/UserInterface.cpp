#include "peano/utils/UserInterface.h"
#include "peano/version.h"
#include "peano/utils/Globals.h"
#include "peano/utils/PeanoOptimisations.h"


#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/services/ServiceRepository.h"


#include "tarch/multicore/MulticoreDefinitions.h"


#include "tarch/multicore/Core.h"


#ifdef Parallel
#include "tarch/parallel/Node.h"
#endif



#include "peano/utils/Globals.h"

#include <sstream>

// @todo Hope this is the only place where we need it.
#if defined(CompilerHasProcStat)
#include <unistd.h>
#endif

#include <bitset>
#include <cstdlib>
#include <cstring>
#include <stdio.h>


tarch::logging::Log peano::utils::UserInterface::_log("peano::utils::UserInterface");


const std::string peano::utils::UserInterface::_PeanoHeader("Application based upon the PDE framework Peano - 3rd Generation");
const std::string peano::utils::UserInterface::_PeanoCopyrightURL("(C) 2005 - 2018   www.peano-framework.org");

const int peano::utils::UserInterface::_WidthOfSolverIterationColumn(22);
const int peano::utils::UserInterface::_WidthOfGridIterationColumn(12);
const int peano::utils::UserInterface::_WidthOfIntegerColumn(5);


peano::utils::UserInterface::UserInterface() {
}


peano::utils::UserInterface::~UserInterface() {
}


void peano::utils::UserInterface::reportRegisteredServices() {
  _log.info("reportRegisteredServices()", "services registered: " + tarch::services::ServiceRepository::getInstance().getListOfRegisteredServices() );
}


std::string peano::utils::UserInterface::format( int SpacesPerColumn, const std::string& text ) {
  int leadingSpaces = SpacesPerColumn - static_cast<int>(text.length());
  leadingSpaces /= 2;

  std::ostringstream result;

  for (int i=0; i<leadingSpaces; i++ ) {
    result << " ";
  }
  result << text;
  while (static_cast<int>(result.str().length()) < SpacesPerColumn) {
    result << " ";
  }

  return result.str();
}


std::string peano::utils::UserInterface::format( int SpacesPerColumn, int value ) {
  std::ostringstream result;

  int leadingSpaces = SpacesPerColumn-1;
  int valueCopy = value;
  while (valueCopy >= 10) {
	leadingSpaces --;
    valueCopy /= 10;
  }

  for (int i=0; i<leadingSpaces; i++) {
	result << " ";
  }
  result << value;

  return result.str();
}


std::string peano::utils::UserInterface::format( int SpacesPerColumn, double value ) {
  std::ostringstream result;

  result << value;

  return format( SpacesPerColumn, result.str() );
}


std::size_t peano::utils::UserInterface::getMemoryUsage() {
  #if defined(CompilerHasProcStat)
  char   work[4096];
  FILE*  f;
  char*  pCh;

  pid_t pid = getpid();

  sprintf(work, "/proc/%d/stat", (int)pid);
  f = fopen(work, "r");

  if (f == NULL) {
    std::ostringstream msg;
    msg << "can't open file " << work;
    _log.error("getMemoryUsage()", msg.str() );
    return(0);
  }
  if(fgets(work, sizeof(work), f) == NULL) {
      _log.error("getMemoryUsage","Error while reading from file");
  }
  fclose(f);
  strtok(work, " ");

  for (int i = 1; i < 23; i++) {
    pCh = strtok(NULL, " ");
  }

  return(atol(pCh));
  #else
  return 0;
  #endif
}


int peano::utils::UserInterface::getMemoryUsageKB() {
  long u = getMemoryUsage();
  long kilo = 1024;
  long usageKB = ((u + (kilo/2)) / kilo );

  const long maxInteger = std::numeric_limits<int>::max();
  if (usageKB>maxInteger) {
    logError( "getMemoryUsageKB()", "cannot cast result to return value: " << usageKB );
  }

  return static_cast<int>(usageKB);
}


int peano::utils::UserInterface::getMemoryUsageMB() {
  long u = getMemoryUsage();
  long mega = 1024 * 1024;
  long usageMB ((u + (mega/2)) / mega );

  const long maxInteger = std::numeric_limits<int>::max();
  if (usageMB>maxInteger) {
    logError( "getMemoryUsageKB()", "cannot cast result to return value: " << usageMB );
  }

  return static_cast<int>(usageMB);
}


void peano::utils::UserInterface::writeHeader(const std::string& experimentName) {
  _log.info("writeHeader()", experimentName );
  _log.info("writeHeader()", _PeanoHeader );
  // This cast is work around GCC realisations that are not compliant with the
  // C++ standard:
  // http://stackoverflow.com/questions/14617950/ambiguous-call-to-overloaded-function-stdto-string
  // http://stackoverflow.com/questions/10664699/stdto-string-more-than-instance-of-overloaded-function-matches-the-argument
  _log.info("writeHeader()", "revision: " + std::to_string(static_cast<long long>(PEANO_VERSION)) );
  #ifdef Parallel
  int numberOfProcesses = tarch::parallel::Node::getInstance().getNumberOfNodes();
  #else
  int numberOfProcesses = 1;
  #endif

  #if defined(SharedMemoryParallelisation)
  int numberOfThreads = tarch::multicore::Core::getInstance().getNumberOfThreads();
  #else
  int numberOfThreads = 1;
  #endif

  std::ostringstream buildString;
  buildString << "build: ";
  #if defined(SharedTBB)
  buildString << "multicore=tbb ";
  #elif defined(SharedCPP)
  buildString << "multicore=c++14 ";
  #elif defined(SharedOMP)
  buildString << "multicore=openMP ";
  #endif
  buildString << "dim=" << DIMENSIONS;

  std::ostringstream optimisationString;
  optimisationString << "optimisations: ";
  #ifdef DloopOptimiseAggressive
  optimisationString << "d-loop ";
  #endif
  #ifdef StoreOnlyPersistentAttributes
  optimisationString << "persistent-attributes ";
  #endif
  #ifdef PackedRecords
  optimisationString << "packed ";
  #endif
  #ifdef DistinguishStationaryAndInstationarySubtrees
  optimisationString << "opt-static-subtrees ";
  #endif
  #ifdef UseRecursionUnrollingOnRegularPatches
  optimisationString << "recursion-unrolling ";
  #endif
  #ifdef PersistentRegularSubtrees
  optimisationString << "persistent-regular-subtrees";
  #endif

  _log.info("writeHeader()", buildString.str() );
  _log.info("writeHeader()", optimisationString.str() );
  _log.info("writeHeader()", _PeanoCopyrightURL );
  std::ostringstream msg;
  if (numberOfProcesses>0) {
    msg << "processes: " << numberOfProcesses;
  }
  if (numberOfThreads>0 && numberOfProcesses>0) {
    msg << ", ";
  }
  if (numberOfThreads>0) {
    msg << "threads: " << numberOfThreads;
    #ifdef TBBInvade
    msg << "+x";
    #endif
  }

  #ifdef Parallel
  msg << " (per process)";
  #endif

  if (numberOfProcesses>0 || numberOfThreads>0) {
    _log.info("writeHeader()",msg.str());
  }
}


void peano::utils::UserInterface::writeDatatypeStatisticsHeader() {
  _log.info( "writeDatatypeStatisticsHeader()", " --- Statistics ---" );

  std::ostringstream generalNotes;
  generalNotes << "dimensions:" << DIMENSIONS;
  _log.info( "writeDatatypeStatisticsHeader()", generalNotes.str() );

  std::ostringstream numericalEquality;
  numericalEquality << "numerical equality epsilon: " << tarch::la::NUMERICAL_ZERO_DIFFERENCE;
  _log.info( "writeDatatypeStatisticsHeader()", numericalEquality.str() );

  std::ostringstream sizeOfPrimitiveTypes;
  sizeOfPrimitiveTypes << sizeof(char) << " byte(s) per char, "
                       << sizeof(int) << " byte(s) per int, "
                       << sizeof(long int) << " byte(s) per long int, "
                       << sizeof(double) << " byte(s) per double, "
                       << sizeof(bool) << " byte(s) per bool, and "
                       << sizeof(std::bitset<DIMENSIONS>) << " byte(s) per bitset<DIM>";
  _log.info( "writeDatatypeStatisticsHeader()", sizeOfPrimitiveTypes.str() );

  #ifdef ComponentGrid
//#error Does not work
//  std::ostringstream sizeOfTraversionAutomatonState;
//  sizeOfTraversionAutomatonState << sizeof(grid::TraversalAutomatonState)
//                                << " byte(s) per grid::TraversalAutomatonState";
//  _log.info( "writeDatatypeStatisticsHeader()", sizeOfTraversionAutomatonState.str() );
  #endif
}


void peano::utils::UserInterface::writeDatatypeStatisticsTrailer() {
  _log.info( "writeDatatypeStatisticsTrailer()", " ---            ---" );
}
