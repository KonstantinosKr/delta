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
	  delta::core::data::Structure& 			datastructure,
	  delta::core::data::Meta::EngineMeta& 	meta)
{
  _start = Time::now();

  _noOfParticles 	= datastructure.getNumberOfParticles();
  _noOfObstacles 	= datastructure.getNumberOfObstacles();
  _dt 				= meta.dt;

  _numberOfTriangleComparisons 	= 0;
  _numberOfParticleComparisons 	= 0;
  _numberOfCollisions 			= 0;
  _iteration 					= 0;

  std::vector<std::string> parameters;

  std::cout << "-----------------------------------------" << std::endl;
  std::cout << "-Delta Library - Kostantinos Krestenitis-" << std::endl;
  std::cout << "-----------------------------------------" << std::endl;
  std::cout << "----------Simulation Scenario------------" << std::endl;
  std::cout << "Delta t			: " 	<< getStepSize() 		<< std::endl
			<< "Overlap	PreCheck	: " 	<< meta.overlapPreCheck	<< std::endl
			<< "# Particles		: " 	<< datastructure.getNumberOfParticles()	<< std::endl
			<< "# Triangles		: " 	<< datastructure.getNumberOfTriangles()	<< std::endl;
  std::cout << "----------Simulation Run----------------" << std::endl;

  //_checkpointFile = "output.out";
}

void delta::core::State::update()
{
  log();
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

std::chrono::steady_clock::time_point delta::core::State::getStartTime()
{
  return _start;
}

iREAL delta::core::State::getCurrentTime()
{
  std::chrono::steady_clock::time_point end = Time::now();
  fsec fs = end - _start;
  return fs.count();
}

void delta::core::State::log()
{
  std::cout << std::left
			<< std::setfill('0')
			<< std::setw(12)
			<< std::setprecision(5)
			<< getCurrentTime()
            << std::left
			<< std::setw(2)
			<< " | DELTA | i:"
			<< getCurrentStepIteration()
			<< " -- cpt:"
			<< getCollisions() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////

void delta::core::State::termination()
{
  std::cout << "DELTA ----------Execution Terminated-----------" << std::endl;
}


void delta::core::State::saveParticleGeometry()
{

}
