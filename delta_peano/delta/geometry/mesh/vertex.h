/*
 * vertices.h
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#ifndef GEOMETRY_MESH_VERTEX_H_
#define GEOMETRY_MESH_VERTEX_H_

#include <map>
#include <vector>

namespace delta {
  namespace geometry {
	namespace mesh {
	  class Vertex;
	}
  }
}

class delta::geometry::mesh::Vertex {
  public:

	Vertex();

	Vertex(iREAL x, iREAL y, iREAL z);

	void insertVertexToMap(delta::geometry::mesh::Vertex vertex);

	void setX(iREAL value);
	void setY(iREAL value);
	void setZ(iREAL value);

	iREAL getX();
	iREAL getY();
	iREAL getZ();

	virtual ~Vertex();

  private:
	iREAL _x, _y, _z;

  protected:
    std::map<int, std::vector<delta::geometry::mesh::Vertex> >   	_vertices;

};

#endif /* GEOMETRY_MESH_VERTEX_H_ */
