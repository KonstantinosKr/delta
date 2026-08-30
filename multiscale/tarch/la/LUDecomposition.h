// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_LUDECOMPOSITION_H_
#define _TARCH_LA_LUDECOMPOSITION_H_

#include "tarch/la/Vector.h"
#include "tarch/la/Matrix.h"

#include "tarch/Assertions.h"

namespace tarch {
  namespace la {
    /**
     * Performs an in-situ LU-decomposition of the square matrix A. Returns
     * pivot values, too. The storage format is normalised such that the
     * diagonal values of the upper triangular matrix are one.
     */
    template<int Rows, typename Scalar>
    void lu (
      Matrix<Rows,Rows,Scalar>&  A,
      Vector<Rows,int>&          pivots
    );

    /**
     * In-situ LU without pivoting. See the other lu routine.
     */
    template<int Rows, typename Scalar>
    void lu (
      Matrix<Rows,Rows,Scalar>&  A
    );

    /**
     * Accepts an upper triangular matrix and a rhs. It then returns the
     * solution x to
     *
     * Rx=f
     *
     * i.e. x=R^{-1}f
     */
    template<int Rows, typename Scalar>
    Vector<Rows,Scalar> backSubstitution(
      const Matrix<Rows,Rows,Scalar>&  R,
      const Vector<Rows,Scalar>&       f
    );

    template<typename Scalar>
    Matrix<2,2,Scalar> invert(
      const Matrix<2,2,Scalar>&  R
    );

    template<typename Scalar>
    Matrix<3,3,Scalar> invert(
      const Matrix<3,3,Scalar>&  R
    );

    template<typename Scalar>
    double det(
      const Matrix<2,2,Scalar>&  R
    );

    template<typename Scalar>
    double det(
      const Matrix<3,3,Scalar>&  R
    );
  }
}


#include "tarch/la/LUDecomposition.cpph"


#endif
