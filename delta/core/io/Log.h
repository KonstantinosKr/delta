/*
 The MIT License (MIT)

 Copyright (c) 2018 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef DELTA_CORE_IO_LOG_H_
#define DELTA_CORE_IO_LOG_H_

#include <string>
#include <chrono>
#include <vector>
#include <iomanip>
#include <iostream>
#include <delta/core/State.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <string>

typedef std::chrono::steady_clock Time;
typedef std::chrono::duration<float> fsec;

namespace delta {
  namespace core {
	namespace io {

	  class Log;
	  class LogTimeStamp;
	  class LogWarning;
	  class LogError;

	} /* namespace io */
  } /* namespace core */
} /* namespace delta */


class delta::core::io::Log {
  public:
	explicit Log(std::string name, std::chrono::steady_clock::time_point start);
	virtual ~Log();
  private:
	std::string _logIdentifier;
	std::chrono::steady_clock::time_point _start;
  protected:
	iREAL getCurrentTime();
};

class delta::core::io::LogTimeStamp : public delta::core::io::Log {
  public:
	LogTimeStamp(std::string name,  std::chrono::steady_clock::time_point start, std::vector<std::string> parameters);
	void log(State &state);
  private:
	void termination();
};

class delta::core::io::LogWarning : public delta::core::io::Log {
  public:
	LogWarning(std::string name,  std::chrono::steady_clock::time_point start, std::vector<std::string> parameters);
	void log();
};

class delta::core::io::LogError : public delta::core::io::Log {
  public:
	LogError(std::string name,  std::chrono::steady_clock::time_point start, std::vector<std::string> parameters);
	void log();
};

#endif /* DELTA_CORE_IO_LOG_H_ */
