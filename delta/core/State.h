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

#ifndef DELTA_CORE_STATE_H_
#define DELTA_CORE_STATE_H_

#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "data/Structure.h"
#include "data/Meta.h"

typedef std::chrono::steady_clock Time;
typedef std::chrono::duration<float> fsec;

namespace delta {
	namespace core {
		class State;
	}
}

class delta::core::State {
  public:
	State();

	State(
		  delta::core::data::Structure& 			datastructure,
		  delta::core::data::Meta::Simulation& 		meta);

	void 	initCheckpoint(int iteration);
	void 	closeCheckpoint();
	void 	readState();
	void 	writeState();
	void 	saveParticleGeometry();
	void 	update();

	int 		getCurrentStepIteration();
	int 		getCollisions();
	iREAL 	getStepSize();

	void 	incNumberOfTriangleComparisons(int n);
	void 	incNumberOfParticleComparisons(int n);

	std::chrono::steady_clock::time_point getStartTime();

	iREAL getCurrentTime();

	void log();
	void termination();
  private:
	//physical simulation
	int 		_noOfParticles;
	int 		_noOfObstacles;
	iREAL 	_dt;
	int 		_numberOfTriangleComparisons;
	int 		_numberOfParticleComparisons;
	int 		_numberOfCollisions;

	//engine
	int 		_iteration;

	std::chrono::steady_clock::time_point _start;

	//std::ofstream 	_checkpointFile;
};

#endif /* DELTA_CORE_STATE_H_ */


