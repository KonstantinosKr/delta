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

#ifndef SCENARIO_SCENARIOS_H_
#define SCENARIO_SCENARIOS_H_

#include <vector>
#include <array>
#include <delta/geometry/Object.h>

namespace delta {
  namespace world {
	namespace scenarios {

	  void hopper(
		  std::vector<delta::geometry::Object> &coarse,
		  std::vector<delta::geometry::Object> &insitu,
		  iREAL centre[3],
		  int xzcuts,
		  int ycuts,
		  bool uniform,
		  bool isSphere,
		  int meshDensity,
		  iREAL epsilon);

	  void turbine(
	  	std::vector<delta::geometry::Object>& 	coarseObjects,
	  	bool 									isSphere,
	  	int 										meshDensity,
	  	iREAL 									epsilon);

	  void friction(
		  int 									scenario,
		  bool 									isSphere,
		  iREAL 									centre[3],
		  int 									meshDensity,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object> &	coarse);

	  void nuclear(
		  int 									scenario,
		  bool 									isSphere,
		  iREAL 									centre[3],
		  int 									meshDensity,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object> &	coarse,
		  std::vector<delta::geometry::Object> &	fine);

	  void freeFall(
		  int 									scenario,
		  bool 									isSphere,
		  iREAL 									centre[3],
		  int 									meshDensity,
		  iREAL 									epsilon,
		  std::vector<delta::geometry::Object> &	fine);

	  void twoParticlesCrash(
		  std::vector<delta::geometry::Object>& 	coarseObject,
		  bool 									isSphere,
		  int 									meshDensity,
		  iREAL 									epsilon);

	  void rotateParticle(
		  std::vector<delta::geometry::Object>& 	coarseObject,
		  bool 									isSphere,
		  int 									meshDensity,
		  iREAL 									epsilon);

	} /* namespace scenarios */
  } /* namespace world */
} /* namespace delta */

#endif /* WORLD_SCENARIOS_H_ */
