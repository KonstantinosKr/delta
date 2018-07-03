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

#include "delta/core/State.h"

delta::core::State::State()
{

}

delta::core::State::State(
	int 		noOfParticles,
	int 		noOfObstacles,
	iREAL 	dt)
{
  _start = Time::now();

  _noOfParticles = noOfParticles;
  _noOfObstacles = noOfObstacles;
  _dt = dt;

  _numberOfTriangleComparisons = 0;
  _numberOfParticleComparisons = 0;
  _numberOfCollisions = 0;

  //_checkpointFile = "output.out";

  _iteration = 0;
}

void delta::core::State::update()
{
  _iteration ++;
}

int delta::core::State::getCurrentStepIteration()
{
  return _iteration;
}

int delta::core::State::getCollisions()
{
  return _numberOfCollisions;
}

iREAL delta::core::State::getStepSize()
{
  return _dt;
}

void delta::core::State::writeState()
{
  std::string filename = "checkpoint_.delta";

  //_checkpointFile.open (filename);
  /*
  _checkpointFile << "#THIS IS A DELTA CHECKPOINT FILE\n";
  _checkpointFile << "#NOPARTICLE|NOOBSTACLES|TIMESTEPSIZE|ITERATION|TOTALITERATIONS\n";
  _checkpointFile << "%:" << _noOfParticles << ":" << _noOfObstacles+":" <<
					 _timeStepSize<<":" << _currentIteration << ":" << _totalIterations<<"\n";
					 */
}

void delta::core::State::incNumberOfTriangleComparisons(int n)
{
  _numberOfTriangleComparisons += n;
}

void delta::core::State::incNumberOfParticleComparisons(int n)
{
  _numberOfParticleComparisons += n;
}

void delta::core::State::readState()
{
}

std::chrono::system_clock::time_point delta::core::State::getStartTime()
{
  return _start;
}

void delta::core::State::saveParticleGeometry()
{

}
