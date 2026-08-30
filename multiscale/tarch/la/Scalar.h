// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_SCALAR_H_
#define _TARCH_LA_SCALAR_H_

namespace tarch {
  namespace la {
    constexpr double PI = 3.1415926535897932384626433832795028841972;
    constexpr double E = 2.7182818284590452353602874713526624977572;
    #ifdef MACHINE_PRECISION
    constexpr double NUMERICAL_ZERO_DIFFERENCE = MACHINE_PRECISION;
    #else
    constexpr double NUMERICAL_ZERO_DIFFERENCE = 1.0e-12;
    #endif

    double absoluteWeight(
      double value,
	  double relativeError
    );

    double absoluteWeight(
      double value0,
      double value1,
	  double relativeError
    );

    double absoluteWeight(
      double   values[],
	  int      count,
	  double   relativeError
    );
  }
}

#endif
