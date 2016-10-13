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

namespace delta {
	namespace sys {
		void simulationCheckpoint();
		void getKineticEnergy();
		double getKineticRotationalEnergy(double velocity[3], double angular[3], double inertia[9], double mass);

		double getKineticEnergy(double velocity[3], double mass);
		double getRotationalEnergy(double angular[3], double inertia[9]);

		void getLinearVelocities();
		void getAngularVelocities();
		void getPosition();
		void getInertia();
		void getMass();
		void getInverse();
		void getScenarioSettings();


		void logsimulationCheckpoint();
		void logKineticEnergy();
		void logLinearVelocities();
		void logAngularVelocities();
		void logPosition();
		void logInertia();
		void logMass();
		void logInverse();
		void logScenarioSettings();

	}
}

#endif /* DELTA_SYS_SYS_H_ */
