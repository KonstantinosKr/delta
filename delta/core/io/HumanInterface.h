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


#ifndef DELTA_CORE_IO_HUMANINTERFACE_H_
#define DELTA_CORE_IO_HUMANINTERFACE_H_

#include <iostream>
#include <delta/core/data/Meta.h>


namespace delta {
  namespace core {
	namespace io {

	  class HumanInterface {
		public:
		  HumanInterface(int argc, const char* argv[]);
		  void printManualPeano();
		  void printManualDefault();

		  void setPlotScheme(std::string plotID);
		  delta::core::data::Meta::Plot getPlotScheme();
		  void setCollisionScheme(std::string collisionModelID);
		  delta::core::data::Meta::CollisionModel getCollisionScheme();

		  std::string getScenario();
		  delta::core::data::Meta::Simulation getEngineMeta();

		  int getMeshDensity();
		  int getSteps();

		  iREAL getStepSize();

		  virtual ~HumanInterface();
		private:

		  std::string 								_scenario;
		  delta::core::data::Meta::Plot 			_plotScheme;
		  delta::core::data::Meta::CollisionModel 	_collisionModel;
		  delta::core::data::Meta::Simulation 		_engineMeta;

		  bool 	_overlapPreCheck;
		  int 	_steps;
		  int	_meshDensity;
		  iREAL _dt;
	  };

	} /* namespace io */
  } /* namespace core */
} /* namespace delta */

#endif /* DELTA_CORE_IO_HUMANINTERFACE_H_ */
