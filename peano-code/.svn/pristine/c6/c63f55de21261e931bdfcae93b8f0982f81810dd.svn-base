// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_VECTOR_H_
#define _TARCH_LA_VECTOR_H_


namespace tarch {
  namespace la {
    template<int Size, typename Scalar>
    struct Vector;
  }
}


#include "tarch/la/VectorAssignList.h"
#include "tarch/la/VectorOperations.h"
#include "tarch/la/VectorScalarOperations.h"
#include "tarch/la/VectorVectorOperations.h"
#include "tarch/la/VectorSlice.h"


#include "tarch/compiler/CompilerSpecificSettings.h"



/**
 * Simple vector class
 *
 * Most features concerning vectors are deployed into separate headers. Examples:
 *
 * - Access only few elements within vector (subvector): VectorSlice.h
 *
 */
template<int Size, typename Scalar>
struct tarch::la::Vector {
  private:
    //    Scalar _values[Size] __attribute__((aligned(VectorisationAlignment)));
    Scalar _values[Size];

  public:
    Vector ();

    Vector (Scalar value0, Scalar value1);
    Vector (Scalar value0, Scalar value1, Scalar value2);

    /**
     * Assignment operator for any vector type.
     *
     * !!! SSE
     *
     * We do not allow assignment of a vector this itself. Consequently, we can
     * insert an ivdep statement and thus allow the compiler to optimise.
     */
    Vector<Size,Scalar>& operator= (const Vector<Size,Scalar>& toAssign);

    /**
     * Assignment operator for list of comma separated scalar values, that has to
     * match the number of components of the vector. Otherwise a runtime assertion
     * goes wrong.
     */
    VectorAssignList<Size,Scalar> operator=(const Scalar& value);

    /**
     * Copy constructor to copy from any vector type.
     *
     * The only way to accomplish this with enable-if is to specify a second
     * dummy argument with default value, which is (hopefully) optimized away.
     *
     * @see operator= for a discussion of SSE optimisation.
     */
    Vector(const Vector<Size,Scalar>&  toCopy);


    /**
     * Construct new vector and initialize all components with initialValue.
     */
    Vector (const Scalar& initialValue);

    /**
     * Returns the number of components of the vector.
     */
    int size() const;

    /**
     * Returns read-only ref. to component of given index.
     *
     * !!! SSE Optimisation
     *
     * - We have to manually inline this operation. Otherwise, icc interprets operator
     *   calls, i.e. vector element accesses, as function calls and does not vectorise
     *   loops containing vector element accesses.
     */
    inline const Scalar & operator[] (int index) const
      #ifdef UseManualInlining
      __attribute__((always_inline))
      #endif
      {
        assertion3 ( index >= 0, index, Size, toString() );
        assertion3 ( index < Size, index, Size, toString() );
        return _values[index];
      }


    /**
     * Returns ref. to component of given index.
     *
     * @see operator[] for remarks on SSE
     */
    inline Scalar & operator[] (int index)
      #ifdef UseManualInlining
      __attribute__((always_inline))
      #endif
      {
        assertion3 ( index >= 0, index, Size, toString() );
        assertion3 ( index < Size, index, Size, toString() );
        return _values[index];
      }

    /**
     * Returns read-only ref. to component of given index.
     *
     * @see operator[] for remarks on SSE
     */
    inline const Scalar & operator() (int index) const
      #ifdef UseManualInlining
      __attribute__((always_inline))
      #endif
      {
        assertion3 ( index >= 0, index, Size, toString() );
        assertion3 ( index < Size, index, Size, toString() );
        return _values[index];
      }

    /**
     * Returns ref. to component of given index.
     *
     * @see operator[] for remarks on SSE
     */
   inline Scalar & operator() (int index)
     #ifdef UseManualInlining
     __attribute__((always_inline))
     #endif
     {
       assertion3 ( index >= 0, index, Size, toString() );
       assertion3 ( index < Size, index, Size, toString() );
       return _values[index];
     }

   /**
    * Pipes the elements of a vector into a std::string and returns the string.
    */
    std::string toString() const;

    template <typename NewScalarType>
    tarch::la::Vector<Size,NewScalarType> convertScalar() const;
};



#include "tarch/la/VectorIntegerSpecialisation.h"


#include "tarch/la/Vector.cpph"



#endif
