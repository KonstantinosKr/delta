/*
 * triangles.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include <delta/geometry/mesh/triangle.h>

delta::geometry::mesh::Triangle::Triangle() : delta::geometry::mesh::Vertex()
{
  this->_vertexIndexA = 0;
  this->_vertexIndexB = 0;
  this->_vertexIndexC = 0;
}

int delta::geometry::mesh::Triangle::getVertexIndexOnMapA()
{
  return _vertexIndexA;
}

int delta::geometry::mesh::Triangle::getVertexIndexOnMapB()
{
  return _vertexIndexB;
}

int delta::geometry::mesh::Triangle::getVertexIndexOnMapC()
{
  return _vertexIndexC;
}

delta::geometry::mesh::Triangle::~Triangle() {

}

