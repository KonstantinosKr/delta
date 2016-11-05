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

std::vector<int> delta::sys::Sys::_id;
std::ofstream delta::sys::Sys::_checkpointFile;

/*
 * ASCII CHECKPOINT FORMAT DELTA
 *
 *
 */

void delta::sys::Sys::saveScenario(int particles, int obstacles)
{
	_noOfParticles = particles;
	_noOfObstacles = obstacles;
}

void delta::sys::Sys::saveIteration(double timeStepSize, int currentIteration, int totalIterations)
{
	_timeStepSize = timeStepSize;
	_currentIteration = currentIteration;
	_totalIterations = totalIterations;
}

void delta::sys::Sys::initCheckpoint(int iteration){
	std::string filename = "checkpoint_"+std::to_string(iteration)+".delta";
	_checkpointFile.open (filename);

	_checkpointFile << "#THIS IS A DELTA CHECKPOINT FILE\n";
	_checkpointFile << "#NOPARTICLE|NOOBSTACLES|TIMESTEPSIZE|ITERATION|TOTALITERATIONS\n";
	_checkpointFile << "%:"+std::to_string(_noOfParticles)+":"+
					   std::to_string(_noOfObstacles)+":"+
					   std::to_string(_timeStepSize)+":"+
					   std::to_string(_currentIteration)+":"+
					   std::to_string(_totalIterations)+"\n";
}

void delta::sys::Sys::closeCheckpoint(){
	_checkpointFile.close();
}

void delta::sys::Sys::saveParticleProperties(int id, double positionSpatial[3], double positionReferential[3], double linear[3], double angular[3],
								   double inertia[9], double orientation[9], double mass)
{
	_checkpointFile << "@:"<<":"+std::to_string(id)+":"+
					   std::to_string(positionSpatial[0])+","+std::to_string(positionSpatial[1])+","+std::to_string(positionSpatial[2])+":"+
					   std::to_string(positionReferential[0])+","+std::to_string(positionReferential[1])+","+std::to_string(positionReferential[2])+":"+
					   std::to_string(linear[0])+","+std::to_string(linear[1])+","+std::to_string(linear[2])+":"+
					   std::to_string(angular[0])+","+std::to_string(angular[1])+","+std::to_string(angular[2])+":"+
					   std::to_string(inertia[0])+","+std::to_string(inertia[1])+","+std::to_string(inertia[2])+
					   std::to_string(inertia[3])+","+std::to_string(inertia[4])+","+std::to_string(inertia[5])+
					   std::to_string(inertia[6])+","+std::to_string(inertia[7])+","+std::to_string(inertia[8])+":"+
					   std::to_string(orientation[0])+","+std::to_string(orientation[1])+","+std::to_string(orientation[2])+
					   std::to_string(orientation[3])+","+std::to_string(orientation[4])+","+std::to_string(orientation[5])+
					   std::to_string(orientation[6])+","+std::to_string(orientation[7])+","+std::to_string(orientation[8])+":"+
					   std::to_string(mass)+"\n";
}

void delta::sys::Sys::saveParticleGeometry(int id, std::vector<double> xCoordinatesSpatial, std::vector<double> yCoordinatesSpatial, std::vector<double> zCoordinatesSpatial,
						  std::vector<double> xCoordinatesRef, std::vector<double> yCoordinatesRef, std::vector<double> zCoordinatesRef)
{

	for(int i=0;i<xCoordinatesSpatial.size();i++)
	{
		_checkpointFile << "$:"+std::to_string(id)+":"+
								std::to_string(xCoordinatesSpatial[i])+":"+
								std::to_string(yCoordinatesSpatial[i])+":"+
								std::to_string(zCoordinatesSpatial[i])+":"+
							    std::to_string(xCoordinatesRef[i])+":"+
							    std::to_string(yCoordinatesRef[i])+":"+
								std::to_string(zCoordinatesRef[i])+"\n";
	}
}
