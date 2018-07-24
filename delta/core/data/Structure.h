/*
 * Structure.h
 *
 *  Created on: 8 Jun 2018
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_DATA_STRUCTURE_H_
#define DELTA_CORE_DATA_STRUCTURE_H_

#include "delta/geometry/Object.h"
#include <delta/core/data/ParticleRecord.h>
#include <vector>

namespace delta {
  namespace core {
	namespace data {
	  class Structure;
	}
  } /* namespace core */
} /* namespace delta */


class delta::core::data::Structure {
  public:
	Structure();

	Structure(
			std::vector<delta::geometry::Object> objects);

	Structure(
		std::vector<delta::geometry::Object> objects,
		iREAL maxMeshRefinement);

	delta::core::data::ParticleRecord& getParticle(int i);
	std::vector<delta::core::data::ParticleRecord>& getAll();

	int getNumberOfParticles();
	int getNumberOfObstacles();
	int getNumberOfTriangles();

	std::vector<std::array<iREAL, 6>> getGeometryGrid();

	int getSize();
	virtual ~Structure();
	delta::core::data::OctTree _tree;
  private:

	int _numberOfObjects;
	int _numberOfTriangles;
	std::vector<delta::core::data::ParticleRecord>  particles;
};

#endif /* DELTA_CORE_STRUCTURE_H_ */
