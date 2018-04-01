/*
 * triangles.h
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_MESH_TRIANGLE_H_
#define GEOMETRY_MESH_TRIANGLE_H_

#include "delta/geometry/mesh/vertex.h"
#include <map>
#include <vector>

namespace delta {
  namespace geometry {
	namespace mesh {
	  class Triangle;
	}
  }
}

class delta::geometry::mesh::Triangle : public delta::geometry::mesh::Vertex {
  public:
	Triangle();

	int getVertexIndexOnMapA();
	int getVertexIndexOnMapB();
	int getVertexIndexOnMapC();
	virtual ~Triangle();

  private:
	int _vertexIndexA, _vertexIndexB, _vertexIndexC;

};

#endif /* GEOMETRY_MESH_TRIANGLE_H_ */
