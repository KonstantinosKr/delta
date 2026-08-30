#include "peano/heap/tests/CompressedFloatingPointNumbersTest.h"

#include "peano/heap/CompressedFloatingPointNumbers.h"

#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::heap::tests::CompressedFloatingPointNumbersTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


tarch::logging::Log  peano::heap::tests::CompressedFloatingPointNumbersTest::_log( "peano::heap::tests::CompressedFloatingPointNumbersTest" );

 
peano::heap::tests::CompressedFloatingPointNumbersTest::CompressedFloatingPointNumbersTest():
  tarch::tests::TestCase( "peano::heap::tests::CompressedFloatingPointNumbersTest" ) {
}


peano::heap::tests::CompressedFloatingPointNumbersTest::~CompressedFloatingPointNumbersTest() {
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::run() {
  #if __x86_64__ || __ppc64__
  testMethod( testDecomposeWithLongInt ); // Doesn't work with 32 bit as there's no long int defined
  testMethod( testDecomposeWithInt );
  testMethod( testDecomposeWithChar );
  testMethod( testBatchDecomposition );
  testMethod( testBatchDecompositionWithArrayOfFour );

  testMethod( testDecompose );
  testMethod( testErrorComputation );
  testMethod( testComposeDecompose );

  testMethod( testTinyValues );
  #endif
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testTinyValues() {
  const double value =  3.02873817476661532299e-29;

  int compression = peano::heap::findMostAgressiveCompression( value, 1e-8 );
  validateEquals( compression, 1 );

  double       reconstructedValue;
  char         exponent;
  long int     mantissa;

  peano::heap::decompose( value, exponent, mantissa );
  int    integerExponent;
  std::frexp(value , &integerExponent);
  validateEqualsWithParams7(
	mantissa, 0, static_cast<int>(exponent), value, static_cast<int>(exponent), mantissa,
	integerExponent,
	std::numeric_limits<double>::digits-1,
	static_cast<int>(std::numeric_limits<char>::min())
  );
  validateNumericalEqualsWithParams4( exponent, 0.0, static_cast<int>(exponent), value, static_cast<int>(exponent), mantissa  );

  reconstructedValue = peano::heap::compose(exponent,mantissa);

  validateEqualsWithParams2( reconstructedValue, 0.0, static_cast<int>(exponent),mantissa );

  peano::heap::decompose( value, exponent, mantissa, 5 );
  reconstructedValue = peano::heap::compose(exponent,mantissa);

  validateEqualsWithParams2( reconstructedValue, 0.0, static_cast<int>(exponent),mantissa );
  validateNumericalEqualsWithParams2( static_cast<int>(exponent), 0, value, mantissa  );
  /*
    validateEqualsWithParams8(
  	mantissa, 0, static_cast<int>(exponent), value, static_cast<int>(exponent), mantissa,
  	std::frexp(value , &integerExponent), integerExponent,
  	std::numeric_limits<double>::digits-1,
  	static_cast<int>(std::numeric_limits<char>::min())
    );
  */
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testDecomposeWithLongInt() {
  double       value;
  double       reconstructedValue;
  char         exponent;
  long int     mantissa;

  // Casts here are used to display char as number (otherwise you might get line breaks, e.g.
  value = 1.0;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 0.5;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 0.1;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 0.0;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 2.345678e12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = -2.345678e12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 2.345678e-12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testDecomposeWithInt() {
  double       value;
  double       reconstructedValue;
  char         exponent;
  int          mantissa;

  // Casts here are used to display char as number (otherwise you might get line breaks, e.g.
  value = 1.0;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 0.5;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 0.1;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateNumericalEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 0.0;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateNumericalEqualsWithParams2( reconstructedValue, value, static_cast<int>(exponent),mantissa );

  value = 2.345678e12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e3, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = -2.345678e12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e3, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = 2.345678e-12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e-12, static_cast<int>(exponent),mantissa,reconstructedValue,value );
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testDecomposeWithChar() {
  double       value;
  double       reconstructedValue;
  char         exponent;
  char         mantissa;

  // Casts here are used to display char as number (otherwise you might get line breaks, e.g.
  value = 1.0;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e-12, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = 0.5;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e-12, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = 0.1;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e-2, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = 0.0;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e-12, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = 2.345678e12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e10, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = -2.345678e12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e10, static_cast<int>(exponent),mantissa,reconstructedValue,value );

  value = 2.345678e-12;
  peano::heap::decompose( value, exponent, mantissa );
  reconstructedValue = peano::heap::compose(exponent,mantissa);
  //validateEqualsWithParams3( static_cast<int>(exponent),1,value,static_cast<int>(exponent),mantissa );
  //validateEqualsWithParams3( mantissa,1,value,static_cast<int>(exponent),mantissa );
  validateWithParams4( std::abs(reconstructedValue-value)<1e-12, static_cast<int>(exponent),mantissa,reconstructedValue,value );
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testBatchDecomposition() {
  double       value;

  char          exponent[8];
  long int      mantissa[8];
  double        error[8];


  value = 0.0;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-12, static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-12, static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-12, static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<1e-12, static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[4],mantissa[4])-value)<1e-12, static_cast<int>(exponent[4]),mantissa[4],error[4],peano::heap::compose(exponent[4],mantissa[4]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[5],mantissa[5])-value)<1e-12, static_cast<int>(exponent[5]),mantissa[5],error[5],peano::heap::compose(exponent[5],mantissa[5]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[6],mantissa[6])-value)<1e-12, static_cast<int>(exponent[6]),mantissa[6],error[6],peano::heap::compose(exponent[6],mantissa[6]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[7],mantissa[7])-value)<1e-12, static_cast<int>(exponent[7]),mantissa[7],error[7],peano::heap::compose(exponent[7],mantissa[7]) );


  value = 0.1;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-2,  static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-2,  static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-2,  static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<1e-2,  static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[4],mantissa[4])-value)<1e-12, static_cast<int>(exponent[4]),mantissa[4],error[4],peano::heap::compose(exponent[4],mantissa[4]) );


  bool testPasses;


  // This line seems to fail with older gcc versions (4.8.3 does work, e.g.). Please remove
  // it if you encounter problems - it is not a critical situation.
  testPasses = true;
  testPasses &= std::abs(peano::heap::compose(exponent[5],mantissa[5])-value)<1e-12;
  testPasses &= std::abs(peano::heap::compose(exponent[6],mantissa[6])-value)<1e-12;
  testPasses &= std::abs(peano::heap::compose(exponent[6],mantissa[6])-value)<1e-12;
  testPasses &= std::abs(peano::heap::compose(exponent[7],mantissa[7])-value)<1e-12;
  if (!testPasses) {
    logWarning( "testBatchDecomposition()", "compressions/decompression yields invalid results. Happens on some systems with gcc plus optimisations for long integers. Line " << __LINE__ );
  }

  value = 0.01;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-2,  static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-2,  static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-2,  static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<1e-2,  static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[4],mantissa[4])-value)<1e-12, static_cast<int>(exponent[4]),mantissa[4],error[4],peano::heap::compose(exponent[4],mantissa[4]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[5],mantissa[5])-value)<1e-12, static_cast<int>(exponent[5]),mantissa[5],error[5],peano::heap::compose(exponent[5],mantissa[5]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[6],mantissa[6])-value)<1e-12, static_cast<int>(exponent[6]),mantissa[6],error[6],peano::heap::compose(exponent[6],mantissa[6]) );

  testPasses = true;
  testPasses &= std::abs(peano::heap::compose(exponent[7],mantissa[7])-value)<1e-12;
  if (!testPasses) {
    logWarning( "testBatchDecomposition()", "compressions/decompression yields invalid results. Happens on some systems with gcc plus optimisations for long integers. Line " << __LINE__ );
  }

  value = 0.001;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-3,  static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-3,  static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-3,  static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<2e-12, static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[4],mantissa[4])-value)<1e-12, static_cast<int>(exponent[4]),mantissa[4],error[4],peano::heap::compose(exponent[4],mantissa[4]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[5],mantissa[5])-value)<1e-12, static_cast<int>(exponent[5]),mantissa[5],error[5],peano::heap::compose(exponent[5],mantissa[5]) );


  testPasses = true;
  testPasses &= std::abs(peano::heap::compose(exponent[6],mantissa[6])-value)<1e-12;
  testPasses &= std::abs(peano::heap::compose(exponent[7],mantissa[7])-value)<1e-12;
  if (!testPasses) {
    logWarning( "testBatchDecomposition()", "compressions/decompression yields invalid results. Happens on some systems with gcc plus optimisations for long integers. Line " << __LINE__ );
  }
}





void peano::heap::tests::CompressedFloatingPointNumbersTest::testBatchDecompositionWithArrayOfFour() {
  double   value;

  char     exponent[4];
  int      mantissa[4];
  double   error[4];


  value = 0.0;
  peano::heap::decomposeIntoFourVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-12, static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-12, static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-12, static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<1e-12, static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );


  value = 0.1;
  peano::heap::decomposeIntoFourVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-2,  static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-2,  static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-2,  static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<1e-2,  static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );


  value = 0.01;
  peano::heap::decomposeIntoFourVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-2,  static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-2,  static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-2,  static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<1e-2,  static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );


  value = 0.001;
  peano::heap::decomposeIntoFourVariants( value, exponent, mantissa,error );
  validateWithParams4( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value)<1e-3,  static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[1],mantissa[1])-value)<1e-3,  static_cast<int>(exponent[1]),mantissa[1],error[1],peano::heap::compose(exponent[1],mantissa[1]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[2],mantissa[2])-value)<1e-3,  static_cast<int>(exponent[2]),mantissa[2],error[2],peano::heap::compose(exponent[2],mantissa[2]) );
  validateWithParams4( std::abs(peano::heap::compose(exponent[3],mantissa[3])-value)<2e-12, static_cast<int>(exponent[3]),mantissa[3],error[3],peano::heap::compose(exponent[3],mantissa[3]) );

}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testDecompose() {
  double       value;

  char          exponent[8];
  long int      mantissa[8];
  double        error[8];

  double gluedValue;

  char*     pMantissa = reinterpret_cast<char*>( &(mantissa[0]) );
  long int  reconstructedMantissa;
  char*     pReconstructedMantissa = reinterpret_cast<char*>( &reconstructedMantissa );


  value = 0.00830992 - 0.0468039;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa, error );
  validateWithParams6(error[0]<1e-3, static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]),value,peano::heap::compose(exponent[0],mantissa[0])-value);
  // bps=2 i.e. two chars sufficient
  reconstructedMantissa     = 0;
  pReconstructedMantissa[0] = *pMantissa;
  gluedValue    = peano::heap::compose(exponent[0],reconstructedMantissa,1);
  validateWithParams5(std::abs(gluedValue-value)<1e-3, value, gluedValue, mantissa[0], (int)(exponent[0]), reconstructedMantissa );


  value = -0.015625;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa, error );
  validateWithParams6(error[0]<1e-4, static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]),value,peano::heap::compose(exponent[0],mantissa[0])-value);
  reconstructedMantissa     = 0;
  pReconstructedMantissa[0] = *pMantissa;
  gluedValue    = peano::heap::compose(exponent[0],reconstructedMantissa,1);
  validateWithParams5(std::abs(gluedValue-value)<1e-4, value, gluedValue, mantissa[0], (int)(exponent[0]), reconstructedMantissa );


  value = 0.015625;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa, error );
  validateWithParams6(error[0]<1e-4, static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]),value,peano::heap::compose(exponent[0],mantissa[0])-value);
  reconstructedMantissa     = 0;
  pReconstructedMantissa[0] = *pMantissa;
  gluedValue    = peano::heap::compose(exponent[0],reconstructedMantissa,1);
  validateWithParams5(std::abs(gluedValue-value)<1e-4, value, gluedValue, mantissa[0], (int)(exponent[0]), reconstructedMantissa );


  value = -0.0155759;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa, error );
  validateWithParams6(error[0]<2e-4, static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]),value,peano::heap::compose(exponent[0],mantissa[0])-value);
  reconstructedMantissa     = 0;
  pReconstructedMantissa[0] = *pMantissa;
  gluedValue    = peano::heap::compose(exponent[0],reconstructedMantissa,1);
  validateWithParams5(std::abs(gluedValue-value)<2e-4, value, gluedValue, mantissa[0], (int)(exponent[0]), reconstructedMantissa );


  value = 0.0155759;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa, error );
  validateWithParams6(error[0]<2e-4, static_cast<int>(exponent[0]),mantissa[0],error[0],peano::heap::compose(exponent[0],mantissa[0]),value,peano::heap::compose(exponent[0],mantissa[0])-value);
  reconstructedMantissa     = 0;
  pReconstructedMantissa[0] = *pMantissa;
  gluedValue    = peano::heap::compose(exponent[0],reconstructedMantissa,1);
  validateWithParams5(std::abs(gluedValue-value)<2e-4, value, gluedValue, mantissa[0], (int)(exponent[0]), reconstructedMantissa );
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testErrorComputation() {
  double       value;

  char          exponent[8];
  long int      mantissa[8];
  double        error[8];

  value = 0.920446;
  peano::heap::decomposeIntoEightVariants( value, exponent, mantissa,error );
  validateNumericalEquals( std::abs(peano::heap::compose(exponent[0],mantissa[0])-value), error[0] );
}


void peano::heap::tests::CompressedFloatingPointNumbersTest::testComposeDecompose() {
  /*
    assertion in file /home/tobias/git/ExaHyPE/Code/./ExaHyPE/exahype/solvers/ADERDGSolver.cpp, line 2983 failed: tarch::la::equals( DataHeap::getInstance().getData(heapIndex)[i], reconstructedValue, CompressionAccuracy )
    parameter DataHeap::getInstance().getData(heapIndex)[i]:
    parameter reconstructedValue: 1.01768855695855017984e+03
    parameter DataHeap::getInstance().getData(heapIndex)[i] - reconstructedValue: -1.01599999999999761258e+03
    parameter CompressionAccuracy: 1.00000000000000006228e-09
    parameter bytesForMantissa: 5
    ExaHyPE-Euler: /home/tobias/git/ExaHyPE/Code/./ExaHyPE/exahype/solvers/ADERDGSolver.cpp:2983: void exahype::solvers::ADERDGSolver::glueTogether(int, int, int): Assertion `false' failed.
  */
  double originalValue       = 1.68855695855260723448e+00;
  double CompressionAccuracy = 1.00000000000000006228e-09;

  validateEquals( findMostAgressiveCompression( originalValue, CompressionAccuracy/4.0 ), 5 );

  long int mantissa;
  char     exponent;
  decompose(originalValue,exponent,mantissa,5);

  long int reconstructedMantissa = 0;
  char* pOriginalMantissa      = reinterpret_cast<char*>( &(mantissa) );
  char* pReconstructedMantissa = reinterpret_cast<char*>( &(reconstructedMantissa) );

  for (int i=0; i<5; i++) {
    pReconstructedMantissa[i] = pOriginalMantissa[i];
  }

  double reconstructedValue = compose(exponent,reconstructedMantissa,5);

  validateNumericalEqualsWithParams1( reconstructedValue, originalValue, originalValue - reconstructedValue );

  /*
assertion in file /home/tobias/git/ExaHyPE/Code/./ExaHyPE/exahype/solvers/ADERDGSolver.cpp, line 2984 failed: tarch::la::equals( DataHeap::getInstance().getData(heapIndex)[i], reconstructedValue, CompressionAccuracy )
parameter DataHeap::getInstance().getData(heapIndex)[i]: 2.05771161490379483894e+00
parameter reconstructedValue: 2.05771161615848541260e+00
parameter DataHeap::getInstance().getData(heapIndex)[i] - reconstructedValue: -1.25469057365990011021e-09
parameter CompressionAccuracy: 1.00000000000000006228e-09
parameter bytesForMantissa: 4
ExaHyPE-Euler: /home/tobias/git/ExaHyPE/Code/./ExaHyPE/exahype/solvers/ADERDGSolver.cpp:2984: void exahype::solvers::ADERDGSolver::glueTogether(int, int, int): Assertion `false' failed.
Aborted
t
  */
  originalValue       = 2.05771161490379483894e+00;
  CompressionAccuracy = 1.00000000000000006228e-09;

  validateEquals( findMostAgressiveCompression( originalValue, CompressionAccuracy ), 5 ); // warum net 4?
}


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
