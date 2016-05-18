// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_MATRIXSCALAROPERATIONS_H_
#define _TARCH_LA_MATRIXSCALAROPERATIONS_H_


namespace tarch {
  namespace la {
    template<int Rows, int Cols, typename Scalar>
    Matrix<Rows,Cols,Scalar>& operator*= (
      Matrix<Rows,Cols,Scalar>&  matrix,
      const Scalar&              scalar
    );

    /**
      * Multiplies every component of the matrix with the scalar
      *
      * No temporary objects are created during the operation.
     */
    template<int Rows, int Cols, typename Scalar>
    Matrix<Rows,Cols,Scalar> operator*(
      const Matrix<Rows,Cols,Scalar>&  matrix,
      const Scalar&                    scalar
    );

    /**
      * Multiplies every component of the matrix with the scalar
      *
      * No temporary objects are created during the operation.
     */
    template<int Rows, int Cols, typename Scalar>
    Matrix<Rows,Cols,Scalar> operator*(
      const Scalar&                    scalar,
      const Matrix<Rows,Cols,Scalar>&  matrix
    );


    template<int Rows, int Cols>
    Matrix<Rows,Cols,std::complex<double> > operator*(
      const double&                                  scalar,
      const Matrix<Rows,Cols,std::complex<double> >&  matrix
    );

  }
}



#include "tarch/la/MatrixScalarOperations.cpph"

#endif
