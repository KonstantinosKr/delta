#include "tarch/la/tests/VectorTest.h"
#include "tarch/la/Vector.h"
#include "tarch/la/VectorCompare.h"
#include <string>
#include <sstream>
#include <map>

#include "tarch/tests/TestCaseFactory.h"
registerTest(tarch::la::VectorTest)


tarch::la::VectorTest::VectorTest ():
  TestCase ("tarch::la::VectorTest") {
}


void tarch::la::VectorTest::run ()
{
  testMethod (testConstruction);
  testMethod (testAssignment);
  testMethod (testVectorOperations);
  testMethod (testVectorScalarOperations);
  testMethod (testVectorVectorOperations);
  testMethod (testWrappedVector);
  testMethod (testVectorCompare);

  testMethod (testVectorVectorCompare );
}


void tarch::la::VectorTest::testVectorVectorCompare() {
//  Vector<2,double> vector00;
//  Vector<2,double> vector01;
//  Vector<2,double> vector10;
//  Vector<2,double> vector11;
//
//  Vector<2,double> vectorM;
//
//  std::map<tarch::la::Vector<2,double> , int, tarch::la::VectorCompare<2> >  _vertexMap;
//
//  validate( _vertexMap.empty() );
//
//  assignList(vector00) = 0.0, 0.0;
//  validate( _vertexMap.find(vector00)==_vertexMap.end() );
//
//  assignList(vectorM) = 0.0, 0.3;
//  _vertexMap[vectorM] = 2;
//  validate( !_vertexMap.empty() );
//  validate( _vertexMap.size()==1 );
//  validate( _vertexMap.find(vectorM)!=_vertexMap.end() );
//  validate( _vertexMap.find(vector00)==_vertexMap.end() );
}


void tarch::la::VectorTest:: testConstruction() {
  // Test construction with uniform initialization
  Vector<1,int> vector1(1);
  validateEquals (vector1[0], 1);

  Vector<2,int> vector2(1);
  validateEquals (vector2[0], 1);
  validateEquals (vector2[1], 1);

  // Test construction with direct individual initialization
  Vector<2,int> vector3;
  tarch::la::assignList(vector3) = 1, 2;
  validateEquals (vector3[0], 1);
  validateEquals (vector3[1], 2);

  Vector<3,int> vector4;
  tarch::la::assignList(vector4) = 1, 2, 3;
  validateEquals (vector4[0], 1);
  validateEquals (vector4[1], 2);
  validateEquals (vector4[2], 3);

  // Test copy construction
  Vector<3,int> vector5(vector4);
  validateEquals (vector5[0], 1);
  validateEquals (vector5[1], 2);
  validateEquals (vector5[2], 3);

  Vector<3,int> vector6 = vector5;
  validateEquals (vector6[0], 1);
  validateEquals (vector6[1], 2);
  validateEquals (vector6[2], 3);
}


void tarch::la::VectorTest::testAssignment() {
  // Assignment using functionality from VectorAssign.h:
  // Assign a list of scalars
  Vector<3,int> vector;
  assignList(vector) = 1, 2, 3;
  validateEquals (vector[0], 1);
  validateEquals (vector[1], 2);
  validateEquals (vector[2], 3);

  vector = 1, 2, 3;
  validateEquals (vector[0], 1);
  validateEquals (vector[1], 2);
  validateEquals (vector[2], 3);
//
//
//  // Assign a scalar to all vector
//  vector = 0;
//  validateEquals (vector[0], 0);
//  validateEquals (vector[1], 0);
//  validateEquals (vector[2], 0);
//  // Assign a scalar to part of  vector
//  assignRange(vector,0,2) = 0;
//  validateEquals (vector[0], 0);
//  validateEquals (vector[1], 0);
//  validateEquals (vector[2], 0);
//  // Assign a vector to part of  vector
//  Vector<3,int> vector2;
//  vector2 = 1;
//  assignRange(vector,0,2) = vector2;
//  validateEquals (vector[0], 1);
//  validateEquals (vector[1], 1);
//  validateEquals (vector[2], 1);
//
//  assignList(vector) = 1, 2, 3;
//
//  vector = 1;
//  validateEquals (vector[0], 1);
//  validateEquals (vector[1], 1);
//  validateEquals (vector[2], 1);
}


void tarch::la::VectorTest::testVectorOperations() {
  Vector<3,int> vector;

  assignList(vector) = 1, -2, 2;

  int result = sum(vector);
  validateEquals (result, 1);

  result = norm1(vector);
  validateEquals (result, 5.0);

  // Test indexMin, indexMax, min, and max methods
  assignList(vector) = 1, 2, 3;
  validateEquals (indexMin(vector), 0);
  validateEquals (indexMax(vector), 2);
  validateEquals (min(vector), 1);
  validateEquals (max(vector), 3);

  Vector<3,int> vector3;
  assignList(vector3) = 1, 2, 3;
  std::ostringstream stream;
  stream << vector3;
  std::string vector3string ( stream.str() );
  validateEquals ( vector3string, std::string("[1,2,3]") );
}


void tarch::la::VectorTest:: testVectorScalarOperations() {
  Vector<3,int> vector(0);

  // Compound assignment operators
  vector += 1;
  validateEquals (vector[0], 1);
  validateEquals (vector[1], 1);
  validateEquals (vector[2], 1);

  vector -= 2;
  validateEquals (vector[0], -1);
  validateEquals (vector[1], -1);
  validateEquals (vector[2], -1);

  vector *= -2;
  validateEquals (vector[0], 2);
  validateEquals (vector[1], 2);
  validateEquals (vector[2], 2);

  vector /= 2;
  validateEquals (vector[0], 1);
  validateEquals (vector[1], 1);
  validateEquals (vector[2], 1);

  // Arithmetic operators
  vector = vector * 2;
  validateEquals (vector[0], 2);
  validateEquals (vector[1], 2);
  validateEquals (vector[2], 2);

  vector = 2 * vector;
  validateEquals (vector[0], 4);
  validateEquals (vector[1], 4);
  validateEquals (vector[2], 4);

  vector = vector / 4;
  validateEquals (vector[0], 1);
  validateEquals (vector[1], 1);
  validateEquals (vector[2], 1);

  vector = vector + 1;
  validateEquals (vector[0], 2);
  validateEquals (vector[1], 2);
  validateEquals (vector[2], 2);

  vector = vector - 1;
  validateEqualsWithParams1(vector[0], 1, vector.toString());
  validateEqualsWithParams1(vector[1], 1, vector.toString());
  validateEqualsWithParams1(vector[2], 1, vector.toString());
}


void tarch::la::VectorTest:: testVectorVectorOperations() {
  Vector<3,int> vector(1);

  // Addition assignment
  vector += vector;
  validateEquals (vector[0], 2);
  validateEquals (vector[1], 2);
  validateEquals (vector[2], 2);

  // Subtraction assignment
  vector -= vector;
  validateEquals (vector[0], 0);
  validateEquals (vector[1], 0);
  validateEquals (vector[2], 0);

  // Addition
  assignList(vector) = 1, 2, 3;
  vector = vector + vector;
  validateEquals (vector[0], 2);
  validateEquals (vector[1], 4);
  validateEquals (vector[2], 6);

  // Subtraction
  assignList(vector) = 1, 2, 3;
  vector = vector - vector;
  validateEquals (vector[0], 0);
  validateEquals (vector[1], 0);
  validateEquals (vector[2], 0);

  // Dot product
  assignList(vector) = 2, 3, 4;
  int dotresult = dot(vector, vector);
  validateEquals (dotresult, 29);
  validateEquals (dotresult, 29);
  dotresult = vector * vector;
  validateEquals (dotresult, 29);

  // Cross product (only works for same types)
  Vector<3,int> b;
  tarch::la::assignList(b) = 2, 3, 4;
  Vector<3,int> a;
  tarch::la::assignList(a) = 1, 2, 3;
  Vector<3,int> crossresult(0);
  crossresult = cross(a, b);
  validateEquals (crossresult[0], a[1]*b[2] - a[2]*b[1]);
  validateEquals (crossresult[1], a[2]*b[0] - a[0]*b[2]);
  validateEquals (crossresult[2], a[0]*b[1] - a[1]*b[0]);

  // Comparisons
  Vector<3,double> vec0;
  tarch::la::assignList(vec0) = 1.0, 2.0, 3.0;
  Vector<3,double> vec1(vec0);
  validate (equals(vec0,vec1));
  validate (! oneGreater(vec0,vec1));
  validate (oneGreaterEquals(vec0,vec1));
  validate (! allGreater(vec0,vec1));
  validate (! firstGreater(vec0,vec1));

  assignList(vec0) = 2.0, 2.0, 3.0;
  validate (! equals(vec0,vec1));
  validate (oneGreater(vec0,vec1));
  validate (oneGreaterEquals(vec0,vec1));
  validate (! allGreater(vec0,vec1));
  validate (firstGreater(vec0,vec1));

  assignList(vec0) = 2.0, 3.0, 4.0;
  validate (! equals(vec0,vec1));
  validate (oneGreater(vec0,vec1));
  validate (oneGreaterEquals(vec0,vec1));
  validate (allGreater(vec0,vec1));

  // up to here vec1=vec0
  const double tolerance = 1e-14;
  vec0(0) = vec1(0);
  vec0(1) = vec1(1);
  vec0(2) = vec1(2) + 0.99 * tolerance;
  validateWithParams3(equals(vec0,vec1,tolerance),vec0,vec1,vec0(2)-vec1(2));
  validate (! oneGreater(vec0,vec1,tolerance));
  validate (oneGreaterEquals(vec0,vec1));
  validate (! allGreater(vec0,vec1,tolerance));
  validateWithParams2(! firstGreater(vec0,vec1,tolerance), vec0, vec1);

  vec0(2) = vec1(2) + 10.0 * tolerance;
  validate (! equals(vec0,vec1,tolerance));
  validate (oneGreater(vec0,vec1,tolerance));
  validate (oneGreaterEquals(vec0,vec1));
  validate (! allGreater(vec0,vec1,tolerance));
  validateWithParams3 (firstGreater(vec0,vec1,tolerance), vec0, vec1,vec0(2)-vec1(2));

  assignList(vec0) = 1.0, 2.0, 3.0;
  vec0 += 10.0 * tolerance;
  validate (! equals(vec0,vec1,tolerance));
  validate (oneGreater(vec0,vec1,tolerance));
  validate (oneGreaterEquals(vec0,vec1));
  validate (allGreater(vec0,vec1,tolerance));
  validate (firstGreater(vec0,vec1,tolerance));

  assignList(vec0) = 1.0, 2.0, 3.0;
  vec0 -= 0.99 * tolerance;
  validate (oneGreaterEquals(vec0,vec1));

  assignList(vec0) = 1.0, 1.0, 4.0;
  validate (! firstGreater(vec0,vec1));
  assignList(vec0) = 2.0, 0.0, 0.0;
  validate (firstGreater(vec0,vec1));
  assignList(vec0) = 1.0, 2.0 + 10.0 * tolerance, 2.0;
  validate (firstGreater(vec0,vec1,tolerance));

  //Test equalsReturnIndex
  assignList(vec0) = 1.0, 2.0, 3.0 + 0.99 * tolerance;
  assignList(vec1) = 1.0, 2.0, 3.0 ;
  int i=equalsReturnIndex(vec0,vec1,tolerance);
  validateEquals (i,-1);
}

void tarch::la::VectorTest::testWrappedVector() {
  Vector<3,int> vector;
  Vector<6,int> longVector(1);
  assignList(vector) = 2, 3, 4;
  slice(longVector,vector,3);
  validateEquals (longVector[0], 1);
  validateEquals (longVector[1], 1);
  validateEquals (longVector[2], 1);
  validateEquals (longVector[3], 2);
  validateEquals (longVector[4], 3);
  validateEquals (longVector[5], 4);

  Vector<3,int> vectorCopy = slice<3>(longVector,3);
  validateEquals (vectorCopy[0], 2);
  validateEquals (vectorCopy[1], 3);
  validateEquals (vectorCopy[2], 4);
}


void tarch::la::VectorTest::testVectorCompare () {
  typedef Vector<2,double> Vector;
  std::map<Vector,int,VectorCompare<2> > vectors;
  double eps = NUMERICAL_ZERO_DIFFERENCE;
  Vector vec0;
  tarch::la::assignList(vec0) = 1.0, 2.0;
  Vector vec1;
  tarch::la::assignList(vec1) = 1.0 + 10.0 * eps, 1.0;
  Vector vec2;
  tarch::la::assignList(vec2) = 1.0 + 10.0 * eps, 1.5;
  Vector vec3;
  tarch::la::assignList(vec3) = 2.0, 0.0;
//  vectors[vec2] = 2;
//  vectors[vec3] = 3;
//  vectors[vec0] = 0;
//  vectors[vec1] = 1;
//
//  validateEquals (vectors[vec0], 0);
//  validateEquals (vectors[vec1], 1);
//  validateEquals (vectors[vec2], 2);
//  validateEquals (vectors[vec3], 3);
}
