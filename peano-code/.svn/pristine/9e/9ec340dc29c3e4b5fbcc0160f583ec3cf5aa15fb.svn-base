#include "tarch/logging/Log.h"
#include "tarch/logging/CommandLineLogger.h"
#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/Assertions.h"

/**
 * For the machine name. If it doesn't work, switch it off in the file
 * CompilerSpecificSettings.h.
 */
#ifdef CompilerHasUTSName
#include <sys/utsname.h>
#endif

#ifdef Parallel
#include "tarch/parallel/Node.h"
#endif

#include <time.h>


#include "tarch/compiler/CompilerSpecificSettings.h"


#if defined(SharedTBB) || defined(SharedTBBInvade)
tbb::tick_count tarch::logging::Log::_startupTime( tbb::tick_count::now() );
#else
double tarch::logging::Log::_startupTime(0.0);
#endif


tarch::logging::Log::Log(const std::string& className):
  _className( className ) {

  #if !defined(SharedTBB) && !defined(SharedTBBInvade)
  if (_startupTime==0.0) {
  #ifdef SharedOMP
    _startupTime       = omp_get_wtime();
    _startupTime       = tbb::tick_count::now();
  #elif defined(__APPLE__)
    mach_timespec_t mts;
    clock_get_time(cclock, &mts);
    _startupTime = (double)mts.tv_sec + (double)mts.tv_nsec * 1e-09;
  #elif defined(CompilerHasTimespec)
    struct timespec ts;
    if( clock_gettime(CLOCK_REALTIME, &ts) == 0 ) {
      _startupTime = (double)ts.tv_sec + (double)ts.tv_nsec * 1e-09;
    }
  #endif
  }
  #endif
}


tarch::logging::Log::~Log() {
}


#if defined(Debug) && !defined(LogOff)
void tarch::logging::Log::debug(const std::string& methodName, const std::string& message) const {
  UsedLogService::getInstance().debug(getTimeStampSeconds(),getTimeStampHumanReadable(),getMachineInformation(),getTraceInformation(methodName),message);
}
#endif


#if !defined(LogOff)
void tarch::logging::Log::info(const std::string& methodName, const std::string& message) const {
  UsedLogService::getInstance().info(getTimeStampSeconds(),getTimeStampHumanReadable(),getMachineInformation(),getTraceInformation(methodName),message);
}

void tarch::logging::Log::warning(const std::string& methodName, const std::string& message) const {
  UsedLogService::getInstance().warning(getTimeStampSeconds(),getTimeStampHumanReadable(),getMachineInformation(),getTraceInformation(methodName),message);
}

void tarch::logging::Log::error(const std::string& methodName, const std::string& message) const {
  UsedLogService::getInstance().error(getTimeStampSeconds(),getTimeStampHumanReadable(),getMachineInformation(),getTraceInformation(methodName),message);
}

void tarch::logging::Log::indent( bool indent, const std::string& trace, const std::string& message ) const {
  #if defined(Debug)
  UsedLogService::getInstance().indent( indent, trace, message );
  #endif
}
#endif


std::string tarch::logging::Log::getMachineInformation() const {
  std::ostringstream message;

  #ifdef CompilerHasUTSName
  utsname* utsdata = new utsname();
  assertion( utsdata!=NULL );
  uname(utsdata);

  message << "[" << utsdata->nodename << "]";

  #ifdef Parallel
  message << ",";
  #endif

  delete utsdata;
  #endif

  #ifdef Parallel
  if (tarch::parallel::Node::getInstance().isInitialised()) {
    message << "rank:" << tarch::parallel::Node::getInstance().getRank();
  }
  else {
    message << "rank:not-initialised-yet";
  }
  #else
  message << "rank:0";
  #endif

  return message.str();
}


double tarch::logging::Log::getTimeStampSeconds() const {
  #ifdef SharedOMP
    double currentTS       = omp_get_wtime();
    return currentTS - _startupTime;
  #elif defined(SharedTBB) || defined(SharedTBBInvade)
    tbb::tick_count currentTS       = tbb::tick_count::now();
    return (currentTS - _startupTime).seconds();
  #elif defined(__APPLE__)
    mach_timespec_t mts;
    clock_get_time(cclock, &mts);
    double currentTS = (double)mts.tv_sec + (double)mts.tv_nsec * 1e-09;
    return currentTS - _startupTime;
  #elif defined(CompilerHasTimespec)
    struct timespec ts;
    if( clock_gettime(CLOCK_REALTIME, &ts) == 0 ) {
       const double currentTS = (double)ts.tv_sec + (double)ts.tv_nsec * 1e-09;
       return currentTS - _startupTime;
    }
    else {
      return 0.0;
    }
  #else
    return 0.0;
  #endif
}


std::string tarch::logging::Log::getTimeStampHumanReadable() const {
  // calender time: create struct and get time from system
  time_t* timeStamp = new time_t();
  assertion( timeStamp!=NULL );
  time(timeStamp);

  // Break down time into hour, seconds, ...
  // Note that time is only a substructure of timeStamp. Therefore the pointer
  // to time may not be deleted.
  tm*     time      = localtime(timeStamp);
  assertion( time!=NULL );

  std::ostringstream message;

  // write all information
  if (time->tm_hour<10) {
    message << "0";
  }
  message << time->tm_hour << ":";

  if (time->tm_min<10) {
    message << "0";
  }
  message << time->tm_min << ":";

  if (time->tm_sec<10) {
    message << "0";
  }
  message << time->tm_sec;

  delete timeStamp;

  return message.str();
}


std::string tarch::logging::Log::getTraceInformation( const std::string& methodName ) const {
  return _className + "::" + methodName;
}
