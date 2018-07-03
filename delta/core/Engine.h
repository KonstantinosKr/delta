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

#include <delta/contact/contactpoint.h>
#include <delta/contact/detection/sphere.h>
#include <delta/contact/detection/bf.h>
#include <delta/contact/detection/penalty.h>
#include <delta/contact/detection/hybrid.h>
#include <delta/contact/forces/forces.h>

#include <delta/core/data/Structure.h>
#include <delta/core/data/ParticleRecord.h>
#include <delta/core/data/Meta.h>
#include <delta/core/io/read.h>
#include <delta/core/io/write.h>
#include <delta/core/io/Log.h>
#include <delta/core/State.h>

#include <delta/geometry/material.h>
#include <delta/dynamics/dynamics.h>

#include <delta/world/World.h>

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

	CollisionModel	_collisionModel;

	Engine();

	Engine(
		delta::world::World					world,
		delta::core::data::Meta::EngineMeta 	meta);

	Engine(
		std::vector<delta::geometry::Object> particles,
		bool 								gravity,
		std::array<iREAL, 6> 				boundary,
		delta::core::data::Meta::EngineMeta 	meta);

	virtual ~Engine();

	void iterate();

	delta::core::State getState();

	int getNumberOfCollisions();

	void contactDetection();
	void deriveForces();
	void updatePosition();
	std::vector<delta::core::data::ParticleRecord>& getParticleRecords();


  private:
	delta::core::io::LogTimeStamp *	_logBook;
	delta::core::io::LogWarning *	_logWarningBook;
	delta::core::io::LogError *		_logErrorBook;

	delta::core::data::Meta::Plot 	_plot;
	bool 							_overlapCheck;
	delta::core::State 				_state;
	std::array<iREAL, 6>	 			_boundary;
	iREAL 							_gravity;
	delta::core::data::Structure 	_data;

	/**
	 * Hold all the collissions that are tied to a particular particle
	 * (identified) by the key.
	 */
	std::map<int, std::vector<Collisions> >   _activeCollisions;
	std::map<int, std::vector<Collisions> >   _collisionsOfNextTraversal;

	void addCollision(
		std::vector<delta::contact::contactpoint> & newContactPoints,
		delta::core::data::ParticleRecord&                    particleA,
		delta::core::data::ParticleRecord&                    particleB,
		bool sphere
	);
};

#endif
