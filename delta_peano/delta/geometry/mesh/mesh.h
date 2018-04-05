/*
 * mesh.h
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_MESH_MESH_H_
#define GEOMETRY_MESH_MESH_H_

#include <delta/geometry/mesh/triangle.h>
#include <delta/geometry/mesh/vertex.h>
#include <vector>
#include <map>

namespace delta {
  namespace geometry {
	namespace mesh {
	  class Mesh;
	}
  }
}

class delta::geometry::mesh::Mesh {
  public:
	Mesh(std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	Mesh(std::vector<delta::geometry::mesh::Triangle> _triangles);

	void flatten (
		std::vector<iREAL>& xCoordinates,
		std::vector<iREAL>& yCoordinates,
		std::vector<iREAL>& zCoordinates);

	virtual ~Mesh();
  private:
	std::vector<delta::geometry::mesh::Triangle> _triangles;
};

#endif /* GEOMETRY_MESH_MESH_H_ */
