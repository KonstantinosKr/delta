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
     * Performs an in-situ LU-decomposition of the square matrix A. Adds pivot
     * line indices.
     */
    template<int Rows, typename Scalar>
    void lu (
      Matrix<Rows,Rows,Scalar>&  A,
      Vector<Rows,Scalar>&       pivots
    );

    /**
     * In-situ LU without pivoting.
     */
    template<int Rows, typename Scalar>
    void lu (
      Matrix<Rows,Rows,Scalar>&  A
    );
  }
}


#include "tarch/la/LUDecomposition.cpph"


#endif
