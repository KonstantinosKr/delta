/*
 * Structure.cpp
 *
 *  Created on: 8 Jun 2018
 *      Author: konstantinos
 */

#include "delta/core/data/Structure.h"

namespace delta {
  namespace core {
	namespace data {
	  Structure::Structure()
	  {

	  }

	  Structure::Structure(std::vector<delta::geometry::Object> objects)
	  {
		std::array<iREAL,6> boundary = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
		_numberOfObjects = 0;
		_numberOfTriangles = 0;
		for(int i=0; i<objects.size(); i++)
		{
		  delta::core::data::ParticleRecord record(objects[i]);

		  record.refineTree(0.001);
		  this->particles.push_back(record);

		  if(record.getIsObstacle())
		  {
			_numberOfObjects ++;
		  }
		  _numberOfTriangles += record.getNumberOfTriangles();
		}
	  }

	  Structure::~Structure()
	  {

	  }

	  int Structure::getNumberOfParticles()
	  {
		return this->particles.size();
	  }

	  int Structure::getNumberOfObstacles()
	  {
		return _numberOfObjects;
	  }

	  int Structure::getNumberOfTriangles()
	  {
		return _numberOfTriangles;
	  }

	  delta::core::data::ParticleRecord& Structure::getParticle(int i)
	  {
		return this->particles[i];
	  }

	  std::vector<std::array<iREAL, 6>> Structure::getGeometryGrid()
	  {
		std::vector<std::array<iREAL, 6>> grid;
		for(int i=0;i<particles.size();i++)
		{
		  particles[i].getTree().getBoundary(grid);
		}
		return grid;
	  }

	  std::vector<delta::core::data::ParticleRecord>& Structure::getAll()
	  {
		return this->particles;
	  }
	}
  } /* namespace core */
} /* namespace delta */
