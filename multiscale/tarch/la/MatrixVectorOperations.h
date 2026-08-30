// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_MATRIXVECTOROPERATIONS_H_
#define _TARCH_LA_MATRIXVECTOROPERATIONS_H_


#include "tarch/la/Vector.h"
#include "tarch/la/Matrix.h"


namespace tarch {
  namespace la {
    /**
     * Performs a matrix x vector multiplication.
     *
     * The result vector has to be created outside and given as a parameter.
     */
    template<int Rows, int Cols, typename Scalar>
    Vector<Rows,Scalar>  multiply (
      const Matrix<Rows,Cols,Scalar>&  matrix,
      const Vector<Cols,Scalar>&       vector
    );

    /**
     * Performs a matrix x vector multiplication.
     *
     * The result vector is created inside, multiply is used to execute the
     * multiplication.
     */
    template<int Rows, int Cols, typename Scalar>
    Vector<Rows,Scalar> operator* (
      const Matrix<Rows,Cols,Scalar>&  matrix,
      const Vector<Cols,Scalar>&       vector
    );

    /**
     * Outer dot product.
     */
    template<int Size, typename Scalar>
    Matrix<Size,Size,Scalar> outerDot (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );
  }
}


#include "MatrixVectorOperations.cpph"

#endif
