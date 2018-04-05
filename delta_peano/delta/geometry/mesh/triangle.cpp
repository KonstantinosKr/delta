/*
 * triangles.cpp
 *
 *  Created on: 23 Mar 2018
 *      Author: konstantinos
 */

#include <delta/geometry/mesh/triangle.h>

delta::geometry::mesh::Triangle::Triangle(
	delta::geometry::mesh::Vertex &A,
	delta::geometry::mesh::Vertex &B,
	delta::geometry::mesh::Vertex &C)
{
  this->_A = &A;
  this->_B = &B;
  this->_C = &C;
}

delta::geometry::mesh::Triangle::Triangle(
	iREAL A[3],
	iREAL B[3],
	iREAL C[3])
{
  this->_A = new delta::geometry::mesh::Vertex(A);
  this->_B = new delta::geometry::mesh::Vertex(B);
  this->_C = new delta::geometry::mesh::Vertex(C);
}

delta::geometry::mesh::Triangle::Triangle(
	iREAL Ax,
	iREAL Ay,
	iREAL Az,

	iREAL Bx,
	iREAL By,
	iREAL Bz,

	iREAL Cx,
	iREAL Cy,
	iREAL Cz)
{
  this->_A = new delta::geometry::mesh::Vertex(Ax, Ay, Az);
  this->_B = new delta::geometry::mesh::Vertex(Bx, By, Bz);
  this->_C = new delta::geometry::mesh::Vertex(Cx, Cy, Cz);
}

delta::geometry::mesh::Vertex delta::geometry::mesh::Triangle::getVertexA()
{
  return *_A;
}

delta::geometry::mesh::Vertex delta::geometry::mesh::Triangle::getVertexB()
{
  return *_B;
}

delta::geometry::mesh::Vertex delta::geometry::mesh::Triangle::getVertexC()
{
  return *_C;
}

delta::geometry::mesh::Triangle::~Triangle() {

}

