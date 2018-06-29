/*
 * Engine.h
 *
 *  Created on: 11 Jun 2018
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_ENGINE_H_
#define DELTA_CORE_ENGINE_H_

#include <map>
#include <delta/contact/contactpoint.h>
#include <delta/contact/detection/sphere.h>
#include <delta/contact/detection/bf.h>
#include <delta/contact/detection/penalty.h>
#include <delta/contact/detection/hybrid.h>
#include <delta/contact/forces/forces.h>

#include <delta/core/data/Structure.h>
#include <delta/core/data/ParticleRecord.h>
#include <delta/core/io/read.h>
#include <delta/core/io/write.h>
#include <delta/core/State.h>

#include <delta/geometry/material.h>
#include <delta/dynamics/dynamics.h>

namespace delta {
  namespace core {
	class Engine;
  } /* namespace core */
} /* namespace delta */

class delta::core::Engine
{
  public:

	struct Collisions {
	  delta::core::data::ParticleRecord             _copyOfPartnerParticle;
	  std::vector<delta::contact::contactpoint>     _contactPoints;
	};

	enum class CollisionModel {
	  Sphere,
	  BruteForce,
	  Penalty,
	  HybridOnTrianglePairs,
	  HybridOnBatches,
	  GJK,
	  none
	};

	CollisionModel	_collisionModel;

	Engine();

	Engine(
		bool overlapCheck,
		bool plot,
		iREAL dt,
		bool gravity,
		CollisionModel collisionModel,
		delta::core::data::Structure data);

	Engine(
		bool overlapCheck,
		bool plot,
		iREAL dt,
		bool gravity,
		delta::core::data::Structure data);

	virtual ~Engine();

	void iterate();

	delta::core::State getState();

	int getNumberOfCollisions();

	void contactDetection();
	void deriveForces();
	void updatePosition();
	std::vector<delta::core::data::ParticleRecord>& getParticleRecords();


  private:
	bool _plot;
	delta::core::State _state;
	bool _overlapCheck;
	std::array<iREAL, 6> _boundary;
	iREAL _dt;
	iREAL _gravity;
	delta::core::data::Structure _data;

	/**
	 * Hold all the collissions that are tied to a particular particle
	 * (identified) by the key.
	 */
	std::map<int, std::vector<delta::core::Engine::Collisions> >   _activeCollisions;
	std::map<int, std::vector<delta::core::Engine::Collisions> >   _collisionsOfNextTraversal;

	void addCollision(
		std::vector<delta::contact::contactpoint> & newContactPoints,
		delta::core::data::ParticleRecord&                    particleA,
		delta::core::data::ParticleRecord&                    particleB,
		bool sphere
	);
};

#endif
