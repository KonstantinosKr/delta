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
		_numberOfObjects = 0;
		for(int i=0; i<objects.size(); i++)
		{
		  delta::core::data::ParticleRecord record(objects[i]);
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

	  std::vector<delta::core::data::ParticleRecord>& Structure::getAll()
	  {
		return this->particles;
	  }
	}
  } /* namespace core */
} /* namespace delta */
