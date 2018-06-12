/*
 * Structure.h
 *
 *  Created on: 8 Jun 2018
 *      Author: konstantinos
 */
#include "delta/geometry/Object.h"
#include <vector>

#ifndef DELTA_CORE_STRUCTURE_H_
#define DELTA_CORE_STRUCTURE_H_

namespace delta {
  namespace core {
	class Structure;
  } /* namespace core */
} /* namespace delta */


class delta::core::Structure {
  public:
	Structure();

	Structure(std::vector<delta::geometry::Object> objects);

	delta::geometry::Object getObject(int i);

	std::vector<delta::geometry::Object> getAll();

	int getSize();

	virtual ~Structure();
  private:
	std::vector<delta::geometry::Object>  particles;
};

#endif /* DELTA_CORE_STRUCTURE_H_ */
