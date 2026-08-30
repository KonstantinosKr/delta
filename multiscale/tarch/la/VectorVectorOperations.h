// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_VECTORVECTOROPERATIONS_H_
#define _TARCH_LA_VECTORVECTOROPERATIONS_H_


#include "tarch/la/Vector.h"


namespace tarch {
  namespace la {

    /**
     * Adds rVector to lVector and assigns the result to lVector.
     *
     * No temporary vector is created.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar>& operator+=(
      Vector<Size,Scalar>&        lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Subtracts rVector from lVector and assigns the result to lVector.
     *
     * No temporary vector is created.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar>& operator-=(
      Vector<Size,Scalar>&        lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Adds rVector to lVector.
     *
     * A temporary vector is created and copied to store return back the result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator+(
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Subtracts rVector from lVector.
     *
     * A temporary vector is created and copied to store return back the result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator-(
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Multiplies every component of the vectors with each other and writes the
     * results into result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> multiplyComponents(
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Performs the dot (=inner) product of two vectors.
     *
     * For the outer dot product consult the file MatrixVectorOperations.h.
     */
    template<int Size, typename Scalar>
    Scalar operator*(
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Performs the dot (=inner) product of two vectors.
     *
     * For the outer dot product consult the file MatrixVectorOperations.h.
     */
    template<int Size, typename Scalar>
    Scalar dot (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Performs the dot (=inner) product of two vectors.
     *
     * For the outer dot product consult the file MatrixVectorOperations.h.
     */
    template<int Size, typename Scalar>
    Scalar innerDot (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Performs the cross product of two 3D vectors into result.
     */
    template<typename Scalar>
    Vector<3,Scalar> cross (
      const Vector<3,Scalar>&  lhs,
      const Vector<3,Scalar>&  rhs
    );

    /**
     * Compares to vectors on equality by means of a numerical accuracy.
     */
    template<int Size, typename Scalar>
    bool equals (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    /**
     * Compares sequentially every component pair of lVector and rVector, and stops
     * as soon as one is greater than the other.
     *
     * Defines an absolute pairwise ordering between (unequal) vectors.
     */
    template<int Size, typename Scalar>
    bool firstGreater (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    /**
     * Returns true, if one component of lVector is greater than the corresponding
     * component in rVector up to numerical accuracy.
     */
    template<int Size, typename Scalar>
    bool oneGreater (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    /**
     * Returns true, if one component of lVector is greater or equals than the
     * corresponding component in rVector up to numerical accuracy.
     */
    template<int Size, typename Scalar>
    bool oneGreaterEquals (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    /**
     * Returns true, if all components of lVector are greater than the corresponding
     * components in rVector up to numerical accuracy.
     */
    template<int Size, typename Scalar>
    bool allGreater (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    /**
     * Returns true, if all components of lVector are greater or equals than the corresponding
     * components in rVector up to numerical accuracy.
     */
    template<int Size, typename Scalar>
    bool allGreaterEquals (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool allSmaller (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool allSmallerEquals (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    /**
     * Component-wise max evaluation
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> max(
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Bit-wise comparison of the components of two vectors for equality.
     *
     * This method should not be used for floating-point valued vectors. Instead,
     * equals() is the suitable comparison.
     */
    template<int Size, typename Scalar>
    bool operator== (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Bit-wise comparison of the components of two vectors for inequality.
     *
     * This method should not be used for floating-point valued vectors. Instead,
     * !equals() is the suitable comparison.
     */
    template<int Size, typename Scalar>
    bool operator!= (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs
    );

    /**
     * Return Index of element which is not equals.
     *
     */
    template<int Size, typename Scalar>
    int equalsReturnIndex (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );


    template<int Size>
    int equalsReturnIndex (
      const Vector<Size,int>&  lhs,
      const Vector<Size,int>&  rhs
    );


    /**
     * Run through both vectors and count how many entries are the same.
     */
    template<int Size>
    int countEqualEntries (
      const Vector<Size,int>&  lhs,
      const Vector<Size,int>&  rhs
    );

    template<int Size, typename Scalar>
    int countEqualEntries (
      const Vector<Size,Scalar>&  lhs,
      const Vector<Size,Scalar>&  rhs,
      const Scalar  tolerance = NUMERICAL_ZERO_DIFFERENCE
    );
  }
}


#include "tarch/la/VectorVectorOperations.cpph"

#endif
