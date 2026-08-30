// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_MATRIXASSSIGNLIST_H_
#define _TARCH_LA_MATRIXASSSIGNLIST_H_


#include "tarch/Assertions.h"
#include "tarch/la/Matrix.h"


#include <string>


namespace tarch {
  namespace la {
    template<int Rows, int Cols, typename Scalar>
    class MatrixAssignList;

    /**
     * Returns a wrapper around a matrix to enable comma separated list assignment.
     */
    template<int Rows, int Cols, typename Scalar>
    MatrixAssignList<Rows,Cols,Scalar> assignList( Matrix<Rows,Cols,Scalar>& matrix );
  }
}

/**
 * Wrapper around a matrix to enable comma separated list assignment.
 */
template<int Rows, int Cols, typename Scalar>
class tarch::la::MatrixAssignList {
  private:
    Matrix<Rows,Cols,Scalar>&  _matrix;
    int                        _rowIndex;
    int                        _colIndex;

  public:
    /**
     * Constructor.
     */
    MatrixAssignList(Matrix<Rows,Cols,Scalar>&  matrix);

    /**
     * Constructor.
     */
    MatrixAssignList(Matrix<Rows,Cols,Scalar>&  matrix, int row, int col);

    /**
     * Destructor, asserts that the right amount of values has been assigned.
     */
    ~MatrixAssignList();

    /**
     * Assigns the first value.
     */
    MatrixAssignList<Rows,Cols,Scalar>&  operator=(const Scalar&  toAssign);

    /**
     * Assigns all other values after operator=() has been invoked.
     */
    MatrixAssignList<Rows,Cols,Scalar>& operator, (const Scalar&  toAssign);

    std::string toString() const;
};


#include "tarch/la/MatrixAssignList.cpph"


#endif
