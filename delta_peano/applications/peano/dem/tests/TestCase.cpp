#include "dem/tests/TestCase.h"


#include "tarch/compiler/CompilerSpecificSettings.h"
#include "tarch/tests/TestCaseFactory.h"
registerTest(dem::tests::TestCase)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif

 
dem::tests::TestCase::TestCase():
  tarch::tests::TestCase( "dem::tests::TestCase" ) {
}


dem::tests::TestCase::~TestCase() {
}


void dem::tests::TestCase::run() {
}



#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif
