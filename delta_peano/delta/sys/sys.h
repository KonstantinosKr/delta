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

#ifndef DELTA_SYS_SYS_H_
#define DELTA_SYS_SYS_H_


#include <time.h>
#include <vector>

namespace delta {
	namespace sys {
		class Sys;
	}
}


class delta::sys::Sys{
public:
	void checkpoint();
	void getSimSetup(int particles, int obstacles, double timeStepSize, int currentIteration, int iteration);
	/*void delta::sys::Sys::logParticle(int id, double &positionSpatial[3], double &positionReferential[3],
									double &linear[3], double &angular[3], double &positionSpatial[3],
									double &positionReferential[3],double &inertia[9], double mass);*/
private:
	static int _noOfParticles;
	static int _noOfObstacles;
	static double _timeStepSize;
	static int _currentIteration;
	static int	_totalIterations;

	static std::vector<int> _id;
	static std::vector<double&> _positionSpatial;
	static std::vector<double&> _positionReferential;
	static std::vector<double&> _linear;
	static std::vector<double&> _angular;
	static std::vector<double&> _inertia;
	static std::vector<double> _mass;
};






#endif /* DELTA_SYS_SYS_H_ */


