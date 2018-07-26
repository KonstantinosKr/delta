/*
 * EngineMeta.h
 *
 *  Created on: 3 Jul 2018
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_DATA_META_H_
#define DELTA_CORE_DATA_META_H_

#include <delta/core/data/ParticleRecord.h>
#include <delta/contact/contactpoint.h>

namespace delta {
  namespace core {
	namespace data {
	  namespace Meta {

		struct Collisions
		{
		  delta::core::data::ParticleRecord             _copyOfPartnerParticle;
		  std::vector<delta::contact::contactpoint>     _contactPoints;
		};

	    enum class CollisionModel
		{
		  Sphere,
		  BruteForce,
		  Penalty,
		  PenaltyStat,
		  PenaltyTune,
		  HybridOnTrianglePairs,
		  HybridOnBatches,
		  HybridTriangleStat,
		  HybridBatchStat,
		  GJK,
		  none
	    };

	    enum class Plot
		{
	      EveryIteration,
	      Never,
	      UponChange,
	      EveryBatch,
	      Adaptive,
	      Range,
	      Track
	    };

	    struct EngineMeta {
	      CollisionModel		modelScheme;
	      bool				overlapPreCheck;
	      Plot				plotScheme;
	      iREAL				dt;
	      bool				gravity;
	      iREAL				maxPrescribedRefinement;
	    };
	  }
	} /* namespace data */
  } /* namespace core */
} /* namespace delta */

#endif /* DELTA_CORE_DATA_META_H_ */
