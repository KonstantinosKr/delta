/*
 * Structure.cpp
 *
 *  Created on: 8 Jun 2018
 *      Author: konstantinos
 */

#include "Structure.h"

namespace delta {
  namespace core {

	Structure::Structure()
	{

	}

	Structure::Structure(std::vector<delta::geometry::Object> objects)
	{
	  this->particles = objects;
	}

	Structure::~Structure()
	{

	}

	delta::geometry::Object Structure::getObject(int i)
	{
	  return this->particles[i];
	}

	int Structure::getSize()
	{
	  return this->particles.size();
	}

	std::vector<delta::geometry::Object> Structure::getAll()
	{
	  return this->particles;
	}

  } /* namespace core */
} /* namespace delta */
