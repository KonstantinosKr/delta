/*
 The MIT License (MIT)

 Copyright (c) 29 Sep 2016 Konstantinos Krestenitis

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

#ifndef DELTA_CORE_SYS_H_
#define DELTA_CORE_SYS_H_


#include <time.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


namespace delta {
	namespace core {
		class Sys;
	}
}

class delta::core::Sys{
public:
	void initCheckpoint(int iteration);
	void closeCheckpoint();
	void saveScenario(int particles, int obstacles);
	void saveIteration(iREAL timeStepSize, int currentIteration, int iteration);

	void saveParticleProperties(int id, iREAL positionSpatial[3], iREAL positionReferential[3], iREAL linear[3], iREAL angular[3],
							    iREAL inertia[9], iREAL orientation[9], iREAL mass);
	void saveParticleGeometry(int id, std::vector<iREAL> xCoordinatesSpatial, std::vector<iREAL> yCoordinatesSpatial, std::vector<iREAL> zCoordinatesSpatial,
							  std::vector<iREAL> xCoordinatesRef, std::vector<iREAL> yCoordinatesRef, std::vector<iREAL> zCoordinatesRef);
private:
	static int _noOfParticles;
	static int _noOfObstacles;
	static iREAL _timeStepSize;
	static int _currentIteration;
	static int	_totalIterations;

	static std::vector<int> _id;
	static std::ofstream _checkpointFile;
};



#endif /* DELTA_CORE_SYS_H_ */


