#include "peano/performanceanalysis/tests/SpeedupLawsTest.h"

#include "peano/performanceanalysis/SpeedupLaws.h"


#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::performanceanalysis::tests::SpeedupLawsTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


peano::performanceanalysis::tests::SpeedupLawsTest::SpeedupLawsTest() {
}


peano::performanceanalysis::tests::SpeedupLawsTest::~SpeedupLawsTest() {
}


void peano::performanceanalysis::tests::SpeedupLawsTest::run() {
  testMethod( testAmdahl1 );
  testMethod( testAmdahl2 );
  testMethod( testAmdahl3 );
  testMethod( testGetOptimalNumberOfThreadsWithOneRank );
  testMethod( testGetOptimalNumberOfThreadsWithTwoRanks );
  testMethod( testGetOptimalNumberOfThreadsWithSixRanks );
}


double peano::performanceanalysis::tests::SpeedupLawsTest::getAmdahlPrediction(double t1, double f, double c) {
  assertion3(t1>0.0,t1,f,c);
  assertion3(f>=0.0,t1,f,c);
  assertion3(f<=1.0,t1,f,c);
  assertion3(c>=1,t1,f,c);
  return t1 * f + (1-f) * t1 / c;
}


void peano::performanceanalysis::tests::SpeedupLawsTest::testAmdahl1() {
  const double t1 = 4.0;
  const double f  = 0.1;

  SpeedupLaws law(0.1);

  for (int i=0; i<SpeedupLaws::Entries; i++) {
    validateNumericalEquals( getAmdahlPrediction(t1,f,1), t1 );
    law.addMeasurement( 1, getAmdahlPrediction(t1,f,1) );
    law.addMeasurement( 2, getAmdahlPrediction(t1,f,2) );
    law.addMeasurement( 3, getAmdahlPrediction(t1,f,3) );
    law.addMeasurement( 4, getAmdahlPrediction(t1,f,4) );
    law.addMeasurement( 5, getAmdahlPrediction(t1,f,5) );
  }

  law.relaxAmdahlsLaw();

  validateNumericalEqualsWithParams1(law.getSerialCodeFraction(),f,  law.toString());
  validateNumericalEqualsWithParams1(law.getSerialTime(),        t1, law.toString());
}


void peano::performanceanalysis::tests::SpeedupLawsTest::testAmdahl2() {
  const double t1 = 3.0;
  const double f  = 0.8;

  SpeedupLaws law(0.1);

  int cores = 4;
  law.addMeasurement( cores, getAmdahlPrediction(t1,f,cores) );
  law.relaxAmdahlsLaw();

  double differenceT1 = std::abs( t1-law.getSerialTime() );
  double differenceF  = std::abs( f-law.getSerialCodeFraction() );

  cores = 5;
  law.addMeasurement( cores, getAmdahlPrediction(t1,f,cores) );
  law.relaxAmdahlsLaw();
  validateWithParams5( (std::abs( t1-law.getSerialTime() )        < 2.0*differenceT1), t1, f, std::abs( t1-law.getSerialTime() ),        differenceT1, law.toString() );
  validateWithParams5( (std::abs( f-law.getSerialCodeFraction() ) < 2.0*differenceF),  t1, f, std::abs( f-law.getSerialCodeFraction() ), differenceF,  law.toString() );
  differenceT1 = std::abs( t1-law.getSerialTime() );
  differenceF  = std::abs( f-law.getSerialCodeFraction() );

  cores = 6;
  law.addMeasurement( cores, getAmdahlPrediction(t1,f,cores) );
  law.relaxAmdahlsLaw();
  validateWithParams5( (std::abs( t1-law.getSerialTime() )        < 2.0*differenceT1), t1, f, std::abs( t1-law.getSerialTime() ),        differenceT1, law.toString() );
  validateWithParams5( (std::abs( f-law.getSerialCodeFraction() ) < 2.0*differenceF),  t1, f, std::abs( f-law.getSerialCodeFraction() ), differenceF,  law.toString() );
  differenceT1 = std::abs( t1-law.getSerialTime() );
  differenceF  = std::abs( f-law.getSerialCodeFraction() );

  cores = 7;
  law.addMeasurement( cores, getAmdahlPrediction(t1,f,cores) );
  law.relaxAmdahlsLaw();
  validateWithParams5( (std::abs( t1-law.getSerialTime() )        < 2.0*differenceT1), t1, f, std::abs( t1-law.getSerialTime() ),        differenceT1, law.toString() );
  validateWithParams5( (std::abs( f-law.getSerialCodeFraction() ) < 2.0*differenceF),  t1, f, std::abs( f-law.getSerialCodeFraction() ), differenceF,  law.toString() );
  differenceT1 = std::abs( t1-law.getSerialTime() );
  differenceF  = std::abs( f-law.getSerialCodeFraction() );
}


void peano::performanceanalysis::tests::SpeedupLawsTest::testAmdahl3() {
  const double t1 = 4.0;
  const double f  = 0.1;

  SpeedupLaws law(0.1);

  const int cores = 2;
  law.addMeasurement( cores, 0.9*getAmdahlPrediction(t1,f,cores) );
  law.relaxAmdahlsLaw();

  validateWithParams2( std::abs(law.getSerialCodeFraction()-f) < 0.1, f,  law.toString());
  validateWithParams2( std::abs(law.getSerialTime()-t1) < 2.5,        t1, law.toString());

  law.addMeasurement( cores, 1.1*getAmdahlPrediction(t1,f,cores) );
  law.relaxAmdahlsLaw();

  validateWithParams2( std::abs(law.getSerialCodeFraction()-f) < 0.05, f,  law.toString());
  validateWithParams2( std::abs(law.getSerialTime()-t1) < 2.0,         t1, law.toString());

  law.addMeasurement( cores, 0.8*getAmdahlPrediction(t1,f,cores) );
  law.relaxAmdahlsLaw();

  validateWithParams2( std::abs(law.getSerialCodeFraction()-f) < 1.0, f,  law.toString());
  validateWithParams2( std::abs(law.getSerialTime()-t1) < 4.0,        t1, law.toString());
}


void peano::performanceanalysis::tests::SpeedupLawsTest::testGetOptimalNumberOfThreadsWithSixRanks() {
  std::vector<double>  t_1;
  std::vector<double>  f;
  std::vector<double>  s;

  const int MaxThreads = 24;

  t_1.push_back(0.00515314);
  t_1.push_back(0);
  t_1.push_back(0.00516363);
  t_1.push_back(0.00514758);
  t_1.push_back(0.00506833);
  t_1.push_back(0.00516871);

  f.push_back(0.5);
  f.push_back(1.0);
  f.push_back(0.5);
  f.push_back(0.5);
  f.push_back(0.5);
  f.push_back(0.5);

  s.push_back(0);
  s.push_back(1);
  s.push_back(0);
  s.push_back(0);
  s.push_back(0);
  s.push_back(0);

  int recommendation = 0;

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads );
  validateEquals(recommendation,MaxThreads/5+1);
}


void peano::performanceanalysis::tests::SpeedupLawsTest::testGetOptimalNumberOfThreadsWithOneRank() {
  std::vector<double>  t_1;
  std::vector<double>  f;
  std::vector<double>  s;

  const int MaxThreads = 4;

  t_1.push_back(2);
  f.push_back(0.1);
  s.push_back(0.1);

  int recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads );
  validateEquals(recommendation,MaxThreads);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads*2 );
  validateEquals(recommendation,MaxThreads*2-1);

  t_1[0] = 6.456e-6;
  f[0] = 0.5;
  s[0] = 0.0;

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads );
  validateEquals(recommendation,MaxThreads);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads*2 );
  validateEquals(recommendation,MaxThreads*2);
}


void peano::performanceanalysis::tests::SpeedupLawsTest::testGetOptimalNumberOfThreadsWithTwoRanks() {
  std::vector<double>  t_1;
  std::vector<double>  f;
  std::vector<double>  s;

  const int MaxThreads = 4;

  t_1.push_back(2);
  t_1.push_back(2);
  f.push_back(0.1);
  f.push_back(0.1);
  s.push_back(0.1);
  s.push_back(0.1);

  int recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads );
  validateEquals(recommendation,MaxThreads/2+1);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    1, t_1, f, s, MaxThreads );
  validateEquals(recommendation,MaxThreads/2+1);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads*2 );
  validateEquals(recommendation,MaxThreads*2/2);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    1, t_1, f, s, MaxThreads*2 );
  validateEquals(recommendation,MaxThreads*2/2);

  t_1[0] = 6.456e-6;
  t_1[1] = 6.456e-6;
  f[0] = 0.5;
  f[1] = 0.5;
  s[0] = 0.0;
  s[1] = 0.0;

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads );
  validateEquals(recommendation,MaxThreads/2);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    1, t_1, f, s, MaxThreads );
  validateEquals(recommendation,MaxThreads/2);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    0, t_1, f, s, MaxThreads*2 );
  validateEquals(recommendation,MaxThreads*2/2);

  recommendation = peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
    1, t_1, f, s, MaxThreads*2 );
  validateEquals(recommendation,MaxThreads*2/2);
}


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
