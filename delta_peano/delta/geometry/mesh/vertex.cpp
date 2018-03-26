/*
 * vertices.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include <delta/geometry/mesh/vertex.h>

delta::geometry::mesh::Vertex::Vertex()
{
  this->_x = 0;
  this->_y = 0;
  this->_z = 0;
}

delta::geometry::mesh::Vertex::Vertex(iREAL x, iREAL y, iREAL z)
{
  this->_x = x;
  this->_y = y;
  this->_z = z;
}

void delta::geometry::mesh::Vertex::insertVertexToMap(delta::geometry::mesh::Vertex vertex)
{
  //_vertices.insert(std::pair<int, delta::geometry::mesh::Vertex>(_vertices.size(), vertex));
}

void delta::geometry::mesh::Vertex::setX(iREAL value)
{
  this->_x = value;
}

void delta::geometry::mesh::Vertex::setY(iREAL value)
{
  this->_y = value;
}

void delta::geometry::mesh::Vertex::setZ(iREAL value)
{
  this->_z = value;
}

iREAL delta::geometry::mesh::Vertex::getX()
{
  return _x;
}

iREAL delta::geometry::mesh::Vertex::getY()
{
  return _y;
}

iREAL delta::geometry::mesh::Vertex::getZ()
{
  return _z;
}

delta::geometry::mesh::Vertex::~Vertex()
{
  // TODO Auto-generated destructor stub
}

