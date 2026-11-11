/*
 The MIT License (MIT)

 Copyright (c) 2022 Konstantinos Krestenitis

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

#include "dim3/contact/contactpoint.h"
#include "dim3/contact/detection/sphere.h"
#include "dim3/contact/detection/bf.h"
#include "dim3/contact/detection/penalty.h"
#include "dim3/contact/detection/hybrid.h"
#include "dim3/contact/detection/point.h"
#include "dim3/contact/detection/box.h"
#include "dim3/contact/forces/forces.h"

#include "core/data/Structure.h"
#include "core/data/ParticleRecord.h"
#include "core/data/Meta.h"

#include "core/io/read.h"
#include "dim3/io/write.h"

#include "core/State.h"

#include "dim3/geometry/material.h"
#include "dim3/geometry/operators/vertex.h"
#include "dim3/dynamics/dynamics.h"

#include "dim3/world/World.h"

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
			delta::world::World						world,
			delta::core::data::Meta::Simulation 	meta
			);

		Engine(
			std::vector<delta::world::structure::Object> 	particles,
			std::array<iREAL, 6> 							boundary,
			delta::core::data::Meta::Simulation 			meta
			);

		virtual ~Engine();

		void hyperContacts(
			double 								epsilonA,
			double 								epsilonB,
			std::vector<iREAL>& 				ex,
			std::vector<iREAL>& 				ey,
			std::vector<iREAL>& 				ez,
			std::vector<std::array<iREAL,4>>& 	d
			);
		
		/* steps */
		void 	iterate();
		void 	contactDetection();
		void 	contactDetectionBruteForce();
		void 	deriveForces();
		void 	updatePosition();
		void 	plot(std::string path);

		/* Contacting particle-id pairs found by the last contact detection pass. */
		std::vector<std::pair<int,int> >	getContactPairs();
		iREAL 	getMaxForceMagnitude();

		CollisionModel	_collisionModel;
		
		/* Getter/Setters */
		int 											getNumberOfCollisions();
		std::vector<delta::core::data::ParticleRecord>& getParticleRecords();
		delta::core::State 								getState();

  private:
		void addCollision(
			std::vector<delta::contact::contactpoint>& 	newContactPoints,
			delta::core::data::ParticleRecord&        	particleA,
			delta::core::data::ParticleRecord&        	particleB,
			bool sphere
		);
		
		/* Candidate enumeration (uniform-cell broad phase and reference scan). */
		void binnedCandidates(std::vector<std::pair<int,int> >& candidates);
		void bruteForceCandidates(std::vector<std::pair<int,int> >& candidates);
		void evaluateCandidates(std::vector<std::pair<int,int> >& candidates);

		bool 							_overlapCheck;
		bool 							_resolveContacts;
		iREAL 							_maxForceMagnitude;
		std::array<iREAL, 3>		_gravity;
		std::array<iREAL, 6>	 		_boundary;
		delta::core::data::Meta::Plot 	_plot;
		delta::core::State 				_state;
		delta::core::data::Structure 	_data;

		/**
		 * Hold all the collissions that are tied to a particular particle
		 * (identified) by the key.
		 */
		std::map<int, std::vector<Collisions> >   _activeCollisions;
		std::map<int, std::vector<Collisions> >   _collisionsOfNextTraversal;
};

#endif
