// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_GRAMSCHMIDT_H_
#define _TARCH_LA_GRAMSCHMIDT_H_

#include "tarch/la/Matrix.h"
#include "tarch/la/VectorOperations.h"
#include "tarch/la/VectorVectorOperations.h"
#include "tarch/la/VectorScalarOperations.h"
#include "tarch/Assertions.h"

namespace tarch {
  namespace la {
    /**
     * Produces an economy-size QR decomposition of a matrix A, A is changed.
     *
     * Preconditions:
     * - The column vectors of A have to be linear independent.
     * - If A has n columns and m rows, then:
     *   * n <= m for A
     *   * Q has to have same size than A
     *   * R has to be a n * n matrix
     *
     * Postconditions:
     * - A is changed
     * - Q holds an orthonormal basis of range(A) in its column vectors.
     * - R is an upper triangular matrix, with all main diagonal element unequal
     *   to zero iff A has full rank.
     *
     * The algorithm is taken from the book Numerical Mathematics, Second Edition,
     * by Alfio Querteroni, Riccardo Sacco, and Fausto Saleri, Springer, 2007,
     * pages 85-87.
     *
     * Classic usage:
     *
     *   modifiedGramSchmidt(M, Q, R);
     *   tarch::la::Vector<9, double> p = tarch::la::backSubstitution(R,tarch::la::transpose(Q)*f);
     *
     */
    template<int Rows, int Cols, typename Scalar>
    void modifiedGramSchmidt (
      Matrix<Rows,Cols,Scalar>   A,
      Matrix<Rows,Cols,Scalar>&  Q,
      Matrix<Cols,Cols,Scalar>&  R
    );
  }
}


#include "tarch/la/GramSchmidt.cpph"

#endif
