// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_VECTORSCALAROPERATIONS_H_
#define _TARCH_LA_VECTORSCALAROPERATIONS_H_


namespace tarch {
  namespace la {

    /**
     * Multiplies every component of the vector with the scalar and assigns the
     * result to the vector.
     *
     * No temporary objects are created during the operation.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar>& operator*= (
      Vector<Size,Scalar>&  vector,
      const Scalar&         scalar
    );

    /**
     * Divides every component of the vector by the scalar and assigns the
     * result to the vector.
     *
     * No temporary objects are created during the operation.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar>& operator/= (
      Vector<Size,Scalar>&  vector,
      const Scalar&         scalar
    );

    /**
     * Adds every component of the vector to the scalar and assigns the
     * result to the vector.
     *
     * No temporary objects are created during the operation.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar>& operator+= (
      Vector<Size,Scalar>&  vector,
      const Scalar&         scalar
    );

    /**
     * Subtracts the scalar from every component of the vector and assigns the
     * result to the vector.
     *
     * No temporary objects are created during the operation.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar>& operator-= (
      Vector<Size,Scalar>&  vector,
      const Scalar&         scalar
    );

    /**
     * Multiplies every component of the vector with the scalar and returns the
     * result.
     *
     * A temporary vector is created during the operation and copied as result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator* (
      const Vector<Size,Scalar>&  vector,
      const Scalar&               scalar
    );

    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator* (
      const Scalar&               scalar,
      const Vector<Size,Scalar>&  vector
    );

    /**
     * Divides every component of the vector by the scalar and returns the
     * result.
     *
     * A temporary vector is created during the operation and copied as result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator/ (
      const Vector<Size,Scalar>&  vector,
      const Scalar&               scalar
    );

    /**
     * Adds the scalar to every component of the vector and returns the
     * result.
     *
     * A temporary vector is created during the operation and copied as result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator+ (
      const Vector<Size,Scalar>&  vector,
      const Scalar&               scalar
    );

    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator+ (
      const Scalar&               scalar,
      const Vector<Size,Scalar>&  vector
    );

    /**
     * Subtracts the scalar from every component of the vector and returns the
     * result.
     *
     * A temporary vector is created during the operation and copied as result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> operator- (
      const Vector<Size,Scalar>&  vector,
      const Scalar&               scalar
    );

    /**
     * !!! Vectorisation remark
     *
     * The Intel compiler refuses to vectorise these code fragments if they
     * exit early. As a consequence, we have to remove early exit instructions
     * though they might be faster for very long vectors. In Peano, vectors
     * typically are pretty short and thus we have no early exit condition if
     * you use the Intel compiler.
     */
    template<int Size, typename Scalar>
    bool equals (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool oneEquals (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool oneGreater (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool oneGreaterEquals (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool allGreater (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool allGreaterEquals (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );


    template<int Size, typename Scalar>
    bool oneSmaller (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool oneSmallerEquals (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool allSmaller (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );

    template<int Size, typename Scalar>
    bool allSmallerEquals (
      const Vector<Size,Scalar>&  lhs,
      const Scalar&               cmp,
      const Scalar                tolerance = NUMERICAL_ZERO_DIFFERENCE
    );


    /**
     * Specialisation for int
     */
    template<int Size>
    bool equals (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool oneEquals (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool oneGreater (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool oneGreaterEquals (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool oneSmaller (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool oneSmallerEquals (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool allGreater (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool allGreaterEquals (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool allSmaller (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );

    template<int Size>
    bool allSmallerEquals (
      const Vector<Size,int>&  lhs,
      const int&               cmp
    );
  }
}


#include "tarch/la/VectorScalarOperations.cpph"

#endif
