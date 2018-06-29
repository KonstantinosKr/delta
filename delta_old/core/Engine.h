/*
 * Engine.h
 *
 *  Created on: 11 Jun 2018
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_ENGINE_H_
#define DELTA_CORE_ENGINE_H_

#include <map>
#include <delta/geometry/Object.h>
#include <delta/contact/contactpoint.h>
#include <delta/contact/detection/sphere.h>
#include <delta/contact/detection/bf.h>
#include <delta/contact/detection/penalty.h>
#include <delta/contact/detection/hybrid.h>
#include <delta/core/Structure.h>
#include <delta/core/read.h>
#include <delta/geometry/material.h>
#include <delta/core/write.h>
#include <delta/core/State.h>

namespace delta {
  namespace core {
	class Engine;
  } /* namespace core */
} /* namespace delta */

class delta::core::Engine
{
  public:
	delta::core::Structure _data;

	struct Collisions {
	  delta::geometry::Object                       _copyOfPartnerParticle;
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
		CollisionModel collisionModel);

	virtual ~Engine();

	void iterate();

	delta::core::State getState();

	int getNumberOfCollisions();

	void contactDetection();
	void deriveForces(iREAL dt);
	void updatePosition(iREAL dt, bool gravity);


  private:
	bool _plot;
	delta::core::State _state;
	bool _overlapCheck;


	/**
	 * Hold all the collissions that are tied to a particular particle
	 * (identified) by the key.
	 */
	std::map<int, std::vector<delta::core::Engine::Collisions> >   _activeCollisions;
	std::map<int, std::vector<delta::core::Engine::Collisions> >   _collisionsOfNextTraversal;

	void addCollision(
		std::vector<delta::contact::contactpoint> & newContactPoints,
		delta::geometry::Object&                    particleA,
		delta::geometry::Object&                    particleB,
		bool sphere
	);
};

#endif
