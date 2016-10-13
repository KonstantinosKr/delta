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

namespace delta {
	namespace sys
	{
		void simulationCheckpoint(){

		}

		void output(){

		}

		double getKineticRotationalEnergy(double velocity[3], double angular[3], double inertia[9], double mass){
			iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
			iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);

			return rotation+kinetic;
		}

		double getKineticEnergy(double velocity[3], double mass){
			iREAL kinetic = 0.5 * mass*(velocity[0]*velocity[0])+(velocity[1]*velocity[1])+(velocity[2]*velocity[2]);
			return kinetic;
		}

		double getRotationalEnergy(double angular[3], double inertia[9]){
			iREAL rotation = 0.5 * inertia[0]*(angular[0]*angular[0])+0.5*inertia[4]*(angular[1]*angular[1])+0.5*inertia[4]*(angular[2]*angular[2]);
			return rotation;
		}

		void getLinearVelocities(){

		}

		void getAngularVelocities(){

		}

		void getPosition(){

		}

		void getInertia(){

		}

		void getMass(){

		}

		void getInverse(){

		}

		void getScenarioSettings(){

		}
	}
} /* namespace sys */
