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

#ifndef DELTA_CORE_ENGINE_H_
#define DELTA_CORE_ENGINE_H_

#include <contact/contactpoint.h>
#include <contact/detection/sphere.h>
#include <contact/detection/bf.h>
#include <contact/detection/penalty.h>
#include <contact/detection/hybrid.h>
#include <contact/detection/point.h>
#include <contact/detection/box.h>
#include <contact/forces/forces.h>

#include <core/data/Structure.h>
#include <core/data/ParticleRecord.h>
#include <core/data/Meta.h>

#include <core/io/read.h>
#include <core/io/write.h>

#include <core/State.h>

#include <geometry/material.h>
#include <geometry/operators/vertex.h>
#include <dynamics/dynamics.h>

#include <world/World.h>

#include <map>

namespace delta {
  namespace core {
	class Engine;
	using namespace delta::core::data::Meta;

  } /* namespace core */
} /* namespace delta */

class delta::core::Engine
{
  public:
	Engine();
	Engine(
		delta::world::World										world,
		delta::core::data::Meta::EngineMeta 	meta);

	Engine(
		std::vector<delta::world::structure::Object> 	particles,
		std::array<iREAL, 6> 													boundary,
		delta::core::data::Meta::EngineMeta 					meta);

	virtual ~Engine();

	void hyperContacts(
		double epsilonA,
		double epsilonB,
		std::vector<iREAL>& ex,
		std::vector<iREAL>& ey,
		std::vector<iREAL>& ez,
		std::vector<std::array<iREAL,4>>& d);

		void 								iterate();
		delta::core::State 	getState();
		int 								getNumberOfCollisions();
		void 								contactDetection();
		void 								deriveForces();
		void 								updatePosition();
		void 								plot();

	std::vector<delta::core::data::ParticleRecord>& getParticleRecords();

	CollisionModel	_collisionModel;

  private:
	delta::core::data::Meta::Plot 	_plot;
	bool 														_overlapCheck;
	delta::core::State 							_state;
	std::array<iREAL, 6>	 					_boundary;
	iREAL 													_gravity;
	delta::core::data::Structure 		_data;

	/**
	 * Hold all the collissions that are tied to a particular particle
	 * (identified) by the key.
	 */
	std::map<int, std::vector<Collisions> >   _activeCollisions;
	std::map<int, std::vector<Collisions> >   _collisionsOfNextTraversal;

	void addCollision(
		std::vector<delta::contact::contactpoint>& 	newContactPoints,
		delta::core::data::ParticleRecord&        	particleA,
		delta::core::data::ParticleRecord&        	particleB,
		bool sphere
	);
};

#endif
