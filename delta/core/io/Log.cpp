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

#include "delta/core/io/Log.h"

std::chrono::steady_clock::time_point delta::core::io::Log::_start;

delta::core::io::Log::Log(
	std::string name,
	std::chrono::steady_clock::time_point start)
{
  _logIdentifier = name;
  _start = start;
}


iREAL delta::core::io::Log::getCurrentTime()
{
  std::chrono::steady_clock::time_point end = Time::now();
  fsec fs = end - _start;
  return fs.count();
}

delta::core::io::Log::~Log()
{

}

////////////////////////////////////////////////////////////////////////////////////////

delta::core::io::LogTimeStamp::LogTimeStamp(
	std::string name,
	std::chrono::steady_clock::time_point start,
	std::vector<std::string> parameters) : Log(name, start)
{

}

void delta::core::io::LogTimeStamp::log(delta::core::State& state)
{
  std::cout << std::left
			<< std::setfill('0')
			<< std::setw(12)
			<< std::setprecision(5)
			<< getCurrentTime()
            << std::left
			<< std::setw(2)
			<< " | DELTA | i:"
			<< state.getCurrentStepIteration()
			<< " -- cpt:"
			<< state.getCollisions() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////

void delta::core::io::LogTimeStamp::termination()
{
  std::cout << "DELTA ----------Execution Terminated-----------" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////
