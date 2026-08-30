#include "Watch.h"

#include <sys/time.h>
#include <sstream>
#include <string>

tarch::timing::Watch::Watch(
  const std::string& className,
  const std::string& operationName,
  const bool         plotResultInDestructor,
  bool               startToTickImmediately
  ):
  _log( className ),
  _plotResultInDestructor(plotResultInDestructor),
  _operationName( operationName ),
  _startClockTicks( 0 ),
  _startTime(),
  _elapsedClockTicks( 0 ),
  _elapsedTime(),
  _isRunning(startToTickImmediately) {
  
  startTimer();
}


tarch::timing::Watch::~Watch() {
  if (_isRunning) {
    stopTimer();
  }

  if (_plotResultInDestructor) {
    std::ostringstream message;
    message << "total number of clock ticks within block (cpu-time,calendar-time): "
            << "(" << getCPUTime() << "s"
            << "," << getCalendarTime() << "s"
            << ")";
    _log.info( _operationName, message.str() );
  }

}


void tarch::timing::Watch::startTimer() {
  _startClockTicks = std::clock();
  _startTime = std::chrono::high_resolution_clock::now();
  _isRunning           = true;
}


void tarch::timing::Watch::stopTimer() {
  _elapsedClockTicks   = clock() - _startClockTicks;
  _elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _startTime).count();
  _elapsedTime = _elapsedTime / 1000000000.0; // Convert to seconds
  _isRunning           = false;
}


double tarch::timing::Watch::getCPUTime() {
  double lhs = static_cast<double>(_elapsedClockTicks);
  double rhs = static_cast<double>(CLOCKS_PER_SEC);
  return lhs/rhs;
}


std::clock_t tarch::timing::Watch::getCPUTicks() {
  return _elapsedClockTicks;
}


double tarch::timing::Watch::getCalendarTime()  {
  return _elapsedTime;
}


bool tarch::timing::Watch::isOn() const {
  return _isRunning;
}
