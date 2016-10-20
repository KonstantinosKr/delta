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

#include "sys.h"


	int delta::sys::Sys::_noOfParticles;
	int delta::sys::Sys::_noOfObstacles;
	double delta::sys::Sys::_timeStepSize;
	int delta::sys::Sys::_currentIteration;
	int delta::sys::Sys::_totalIterations;

void delta::sys::Sys::checkpoint(){
	/*
	 * ASCII CHECKPOINT FORMAT DELTA
	 *
	 *
	 */


}

void delta::sys::Sys::getSimSetup(int particles, int obstacles, double timeStepSize, int currentIteration, int totalIterations)
{
	_noOfParticles = particles;
	_noOfObstacles = obstacles;
	_timeStepSize = timeStepSize;
	_currentIteration = currentIteration;
	_totalIterations = totalIterations;
}

/*
void delta::sys::Sys::logParticle(int id, double &positionSpatial[3], double &positionReferential[3],
								double &linear[3], double &angular[3], double &positionSpatial[3],
								double &positionReferential[3],double &inertia[9], double mass)
{
	_id.push_back(id);
	_positionSpatial.push_back(positionSpatial);
	_positionReferential.push_back(positionReferential);
	_linear.push_back(linear);
	_angular.push_back(angular);

	_inertia.push_back(inertia);
	_mass.push_back(mass);
}*/
