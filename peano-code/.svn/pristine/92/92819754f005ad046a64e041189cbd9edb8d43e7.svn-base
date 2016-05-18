// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_SCALAROPERATIONS_H_
#define _TARCH_LA_SCALAROPERATIONS_H_


#include "tarch/la/Scalar.h"

#include <complex>


namespace tarch {
  namespace la {

    /**
     * Returns the absolute value of a type by redirecting to std::abs.
     */
    double abs( double value);

    /**
     * Returns the absolute value of the given int.
     */
    int abs (int value);

    double abs( const std::complex<double>& value );

    /**
     * Computes the i-th power of a in integer arithmetic.
     */
    int aPowI(int i,int a);

    bool greater( double lhs, double rhs, double tolerance = NUMERICAL_ZERO_DIFFERENCE);
    bool greaterEquals( double lhs, double rhs, double tolerance = NUMERICAL_ZERO_DIFFERENCE);
    bool equals( double lhs, double rhs, double tolerance = NUMERICAL_ZERO_DIFFERENCE);
    bool smaller( double lhs, double rhs, double tolerance = NUMERICAL_ZERO_DIFFERENCE);
    bool smallerEquals( double lhs, double rhs, double tolerance = NUMERICAL_ZERO_DIFFERENCE);

    bool equals( const std::complex<double>& lhs, const std::complex<double>& rhs, double tolerance = NUMERICAL_ZERO_DIFFERENCE);

    /**
     * @return -10, or 1 depending on the sign of value
     */
    int sign(double value, double tolerance = NUMERICAL_ZERO_DIFFERENCE);

    int round(double value);
    int round(float value);
  }
}


#endif
