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

#ifndef DELTA_WORLD_WORLD_H_
#define DELTA_WORLD_WORLD_H_

#include <delta/world/scenario/scenarios.h>
#include <delta/core/data/Meta.h>
#include <string>
#include <array>

namespace delta {
  namespace world {

	class World;

  } /* namespace world */
} /* namespace delta */

class delta::world::World {

  public:
	World(
	  std::string 								scenario,
	  bool 										gravity,
	  delta::core::data::Meta::CollisionModel 	collisionModelID,
	  iREAL 										meshDensity);

	virtual ~World();

	void 								createWorld();
	bool 								hasGravity();
	bool 								getIsSphere();
	int 									getNumberOfTriangles();
	int 									getGlobalPrescribedMeshDensity();
	iREAL								getGlobalPrescribedEpsilon();
	std::array<iREAL, 6> 				getBoundary();
	std::vector<delta::geometry::Object> getObjects();

  private:
	int 										_triangles;
	iREAL 									_gravity;
	iREAL 									_globalPrescribedEpsilon;
	iREAL 									_globalPrescribedMeshDensity;
	bool 									_isSphere;
	std::string								_scenario;
	std::array<iREAL, 6> 					_boundary;
	iREAL 									_minParticleDiam;
	iREAL 									_maxParticleDiam;
	std::vector<delta::geometry::Object> 	_particles;
};

#endif /* DELTA_WORLD_WORLD_H_ */
