/*
 * write.h
 *
 *  Created on: 2 Apr 2017
 *      Author: konstantinos
 */

#ifndef DELTA_CORE_WRITE_H_
#define DELTA_CORE_WRITE_H_


#include "delta/geometry/Object.h"
#include <vector>
#include "stdio.h"
#include "string.h"

class write {
public:
  write();
  void writeGeometryToVTK(int step, int numVertices, std::vector<delta::geometry::Object> vectorGeometries, iREAL *t[6][3]);
  void writeGeometry();
  virtual ~write();
};

#endif /* DELTA_CORE_WRITE_H_ */
