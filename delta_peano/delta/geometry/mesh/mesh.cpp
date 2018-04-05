/*
 * mesh.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include <delta/geometry/mesh/mesh.h>

delta::geometry::mesh::Mesh::Mesh(
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  for(int i=0; i<xCoordinates.size(); i+=3)
  {
	delta::geometry::mesh::Triangle *triangle =
		new delta::geometry::mesh::Triangle( xCoordinates[i], 	yCoordinates[i], 	zCoordinates[i],
											xCoordinates[i+1], 	yCoordinates[i+1], 	zCoordinates[i+1],
											xCoordinates[i+2], 	yCoordinates[i+2], 	zCoordinates[i+2]);
	_triangles.push_back(*triangle);
  }
}

delta::geometry::mesh::Mesh::Mesh(std::vector<delta::geometry::mesh::Triangle> triangles)
{
  _triangles.insert(_triangles.begin(), triangles.begin(), triangles.end());
}

void delta::geometry::mesh::Mesh::flatten (
	std::vector<iREAL>& xCoordinates,
	std::vector<iREAL>& yCoordinates,
	std::vector<iREAL>& zCoordinates)
{
  for(int i=0; i<_triangles.size(); i++)
  {
	xCoordinates.push_back(_triangles[i].getVertexA().getX());
	xCoordinates.push_back(_triangles[i].getVertexB().getX());
	xCoordinates.push_back(_triangles[i].getVertexC().getX());

	yCoordinates.push_back(_triangles[i].getVertexA().getY());
	yCoordinates.push_back(_triangles[i].getVertexB().getY());
	yCoordinates.push_back(_triangles[i].getVertexC().getY());

	zCoordinates.push_back(_triangles[i].getVertexA().getZ());
	zCoordinates.push_back(_triangles[i].getVertexB().getZ());
	zCoordinates.push_back(_triangles[i].getVertexC().getZ());
  }
}

delta::geometry::mesh::Mesh::~Mesh() {

}
