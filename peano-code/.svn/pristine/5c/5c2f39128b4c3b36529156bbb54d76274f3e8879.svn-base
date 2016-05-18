// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_MATRIXMATRIXOPERATIONS_H_
#define _TARCH_LA_MATRIXMATRIXOPERATIONS_H_


#include "tarch/la/Scalar.h"


namespace tarch {
  namespace la {
    /**
     * Performs a matrix x matrix multiplication.
     *
     * The resulting matrix has to be created outside and given as a parameter.
     */
    template<int Rows, int Cols, int X, typename Scalar>
    Matrix<Rows,Cols,Scalar> multiply (
      const Matrix<Rows,X,Scalar>& lhs,
      const Matrix<X,Cols,Scalar>& rhs
    );

    template<int Rows, int Cols, int X, typename Scalar>
    Matrix<Rows,Cols,Scalar> multiplyComponents (
      const Matrix<Rows,X,Scalar>& lhs,
      const Matrix<X,Cols,Scalar>& rhs
    );

    template<int Rows, int Cols, int X, typename Scalar>
    Matrix<Rows,Cols,Scalar> operator*(
      const Matrix<Rows,X,Scalar>& lhs,
      const Matrix<X,Cols,Scalar>& rhs
    );

    /**
     * Bitwise comparison of the components of two matrices on equality.
     */
    template<int Rows, int Cols, typename Scalar>
    bool operator== (
      const Matrix<Rows,Cols,Scalar>& lhs,
      const Matrix<Rows,Cols,Scalar>& rhs
    );

    /**
     * Compares to matrices on equality by means of a numerical accuracy.
     */
    template<int Rows, int Cols, typename Scalar>
    bool equals (
      const Matrix<Rows,Cols,Scalar>&  lhs,
      const Matrix<Rows,Cols,Scalar>&  rhs,
      const Scalar&                    tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Rows, int Cols, typename Scalar>
    Matrix<Rows,Cols,Scalar> operator+ (
      const Matrix<Rows,Cols,Scalar>&  lhs,
      const Matrix<Rows,Cols,Scalar>&  rhs
    );

    template<int Rows, int Cols, typename Scalar>
    Matrix<Rows,Cols,Scalar> operator- (
      const Matrix<Rows,Cols,Scalar>&  lhs,
      const Matrix<Rows,Cols,Scalar>&  rhs
    );

    /**
     * Return Index of element which is not equals.
     *
     */
    template<int Rows, int Cols, typename Scalar>
    std::pair<int,int> equalsReturnIndex (
      const Matrix<Rows,Cols,Scalar>& lhs,
      const Matrix<Rows,Cols,Scalar>& rhs,
      const Scalar&                   tolerance = NUMERICAL_ZERO_DIFFERENCE
    );
  }
}


#include "tarch/la/MatrixMatrixOperations.cpph"

#endif
