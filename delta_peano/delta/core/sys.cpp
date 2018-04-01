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

#include <delta/core/sys.h>


int delta::core::Sys::_noOfParticles;
int delta::core::Sys::_noOfObstacles;
iREAL delta::core::Sys::_timeStepSize;
int delta::core::Sys::_currentIteration;
int delta::core::Sys::_totalIterations;

std::vector<int> delta::core::Sys::_id;
std::ofstream delta::core::Sys::_checkpointFile;

/*
 * ASCII CHECKPOINT FORMAT DELTA
 *
 *
 */

void delta::core::Sys::saveScenario(int particles, int obstacles)
{
	_noOfParticles = particles;
	_noOfObstacles = obstacles;
}

void delta::core::Sys::saveIteration(iREAL timeStepSize, int currentIteration, int totalIterations)
{
	_timeStepSize = timeStepSize;
	_currentIteration = currentIteration;
	_totalIterations = totalIterations;
}

void delta::core::Sys::initCheckpoint(int iteration){
	//std::string filename = "checkpoint_"+std::to_string(iteration)+".delta";
	std::string filename = "checkpoint_.delta";

	_checkpointFile.open (filename);

	_checkpointFile << "#THIS IS A DELTA CHECKPOINT FILE\n";
	_checkpointFile << "#NOPARTICLE|NOOBSTACLES|TIMESTEPSIZE|ITERATION|TOTALITERATIONS\n";
	_checkpointFile << "%:" << _noOfParticles << ":" << _noOfObstacles+":" <<
					   _timeStepSize<<":" << _currentIteration << ":" << _totalIterations<<"\n";
}

void delta::core::Sys::closeCheckpoint(){
	_checkpointFile.close();
}

void delta::core::Sys::saveParticleProperties(
	int id,
	iREAL positionSpatial[3],
	iREAL positionReferential[3],
	iREAL linear[3],
	iREAL angular[3],
	iREAL inertia[9],
	iREAL orientation[9],
	iREAL mass)
{
	_checkpointFile << "@:"<< ":" << id <<":"<<
					   positionSpatial[0] << "," << positionSpatial[1] << "," << positionSpatial[2] << ":" <<
					   positionReferential[0] << "," << positionReferential[1] << "," << positionReferential[2] << ":" <<
					   linear[0] << "," << linear[1] << "," << linear[2] << ":" <<
					   angular[0] << "," << angular[1] << "," << angular[2] << ":" <<
					   inertia[0] << "," << inertia[1] << "," << inertia[2] <<
					   inertia[3] << "," << inertia[4] << "," << inertia[5] <<
					   inertia[6] << "," << inertia[7] << "," << inertia[8] << ":" <<
					   orientation[0] << "," << orientation[1] << "," << orientation[2] <<
					   orientation[3] << "," << orientation[4] << "," << orientation[5] <<
					   orientation[6] << "," << orientation[7] << "," << orientation[8] << ":" <<
					   mass << "\n";
}

void delta::core::Sys::saveParticleGeometry(
	int id,
	std::vector<iREAL> xCoordinatesSpatial,
	std::vector<iREAL> yCoordinatesSpatial,
	std::vector<iREAL> zCoordinatesSpatial,
	std::vector<iREAL> xCoordinatesRef,
	std::vector<iREAL> yCoordinatesRef,
	std::vector<iREAL> zCoordinatesRef)
{

	for(unsigned i=0;i<xCoordinatesSpatial.size();i++)
	{
		_checkpointFile << "$:" << id << ":" <<
								xCoordinatesSpatial[i] << ":" <<
								yCoordinatesSpatial[i] << ":" <<
								zCoordinatesSpatial[i] << ":" <<
							    xCoordinatesRef[i] << ":" <<
							    yCoordinatesRef[i] << ":" <<
								zCoordinatesRef[i] << "\n";
	}
}
