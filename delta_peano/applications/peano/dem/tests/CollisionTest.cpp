#include "dem/tests/CollisionTest.h"


#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/tests/TestCaseFactory.h"


//#include "ttd/ContactPoints.h"


registerTest(dem::tests::CollisionTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif

 
dem::tests::CollisionTest::CollisionTest():
  tarch::tests::TestCase( "dem::tests::CollisionTest" ) {
}


dem::tests::CollisionTest::~CollisionTest() {
}


void dem::tests::CollisionTest::run() {
  testMethod( testTwoParallelTriangles0 );
  testMethod( testTwoParallelTriangles1 );
}


void dem::tests::CollisionTest::testTwoParallelTriangles0() {
/*
  #ifdef Dim3
  int trianglesA = 1;
  double xA[]    = {0.0, 0.0, 0.0};
  double yA[]    = {0.0, 1.0, 0.0};
  double zA[]    = {0.0, 0.0, 1.0};

  int trianglesB = 1;
  double xB[]    = {0.2, 0.2, 0.2};
  double yB[]    = {0.0, 1.0, 0.0};
  double zB[]    = {0.0, 0.0, 1.0};

  int numberOfContactPoints;

  double xC[]    = {-1.0, -1.0, -1.0};
  double yC[]    = {-1.0, -1.0, -1.0};
  double zC[]    = {-1.0, -1.0, -1.0};

  double xN[]    = {-1.0, -1.0, -1.0};
  double yN[]    = {-1.0, -1.0, -1.0};
  double zN[]    = {-1.0, -1.0, -1.0};

  ttd::ContactPoints::computeCollisionPoints(
    trianglesA, xA, yA, zA,
    trianglesB, xB, yB, zB,
    0.1,
    numberOfContactPoints,
    xC, yC, zC,
    xN, yN, zN
  );

  validateEquals(numberOfContactPoints,0);

  ttd::ContactPoints::computeCollisionPoints(
    trianglesA, xA, yA, zA,
    trianglesB, xB, yB, zB,
    0.25,
    numberOfContactPoints,
    xC, yC, zC,
    xN, yN, zN
  );

  validateEquals(numberOfContactPoints,1);

  validateNumericalEqualsWithParams3(xC[0],0.1,xC[0],yC[0],zC[0]);
  validateNumericalEqualsWithParams3(yC[0],0.0,xC[0],yC[0],zC[0]);
  validateNumericalEqualsWithParams3(zC[0],0.0,xC[0],yC[0],zC[0]);

  validateNumericalEqualsWithParams3(xN[0],0.1,xN[0],yN[0],zN[0]);
  validateNumericalEqualsWithParams3(yN[0],0.0,xN[0],yN[0],zN[0]);
  validateNumericalEqualsWithParams3(zN[0],0.0,xN[0],yN[0],zN[0]);
  #endif
*/
}



void dem::tests::CollisionTest::testTwoParallelTriangles1() {
/*
  #ifdef Dim3
  int trianglesA = 1;
  double xA[]    = {0.0, 0.0, -0.3};
  double yA[]    = {0.0, 1.0,  0.0};
  double zA[]    = {0.0, 0.0,  1.0};

  int trianglesB = 1;
  double xB[]    = {0.2, 0.2, -0.1};
  double yB[]    = {0.0, 1.0, 0.0};
  double zB[]    = {0.1, 0.1, 1.1};

  int numberOfContactPoints;

  double xC[]    = {-1.0, -1.0, -1.0};
  double yC[]    = {-1.0, -1.0, -1.0};
  double zC[]    = {-1.0, -1.0, -1.0};

  double xN[]    = {-1.0, -1.0, -1.0};
  double yN[]    = {-1.0, -1.0, -1.0};
  double zN[]    = {-1.0, -1.0, -1.0};

  ttd::ContactPoints::computeCollisionPoints(
    trianglesA, xA, yA, zA,
    trianglesB, xB, yB, zB,
    0.1,
    numberOfContactPoints,
    xC, yC, zC,
    xN, yN, zN
  );

  validateEquals(numberOfContactPoints,0);

  ttd::ContactPoints::computeCollisionPoints(
    trianglesA, xA, yA, zA,
    trianglesB, xB, yB, zB,
    0.25,
    numberOfContactPoints,
    xC, yC, zC,
    xN, yN, zN
  );

  validateEquals(numberOfContactPoints,1);

  // I think these guys are wrong and have to be fixed.
  validateNumericalEqualsWithParams3(xC[0],0.1,xC[0],yC[0],zC[0]);
  validateNumericalEqualsWithParams3(yC[0],0.0,xC[0],yC[0],zC[0]);
  validateNumericalEqualsWithParams3(zC[0],0.0,xC[0],yC[0],zC[0]);

  validateNumericalEqualsWithParams3(xN[0],1.0,xN[0],yN[0],zN[0]);
  validateNumericalEqualsWithParams3(yN[0],0.0,xN[0],yN[0],zN[0]);
  validateNumericalEqualsWithParams3(zN[0],0.0,xN[0],yN[0],zN[0]);
  #endif
*/
}


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
