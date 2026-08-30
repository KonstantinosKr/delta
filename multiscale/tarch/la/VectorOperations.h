// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_VECTOR_OPERATIONS_H_
#define _TARCH_LA_VECTOR_OPERATIONS_H_


#include "tarch/la/ScalarOperations.h"
#include <sstream>
#include <cmath>


namespace tarch {
  namespace la {

    /**
     * Computes the 1-norm of the vector, i.e. it sums up abs. component values.
     */
    template<int Size, typename Scalar>
    Scalar norm1 (const Vector<Size,Scalar>& vector);

    /**
     * Specialisation that is different to standard definition. Works as result
     * type is not part of the signature
     */
    template<int Size>
    double norm1 (const Vector<Size,std::complex<double> >& vector);

    /**
     * Computes the 2-norm of the vector, i.e. it takes the square-root of
     * summed up squared component values.
     */
    template<int Size, typename Scalar>
    Scalar norm2 (const Vector<Size,Scalar>& vector);

    /**
     * Specialisation that is different to standard definition. Works as result
     * type is not part of the signature
     */
    template<int Size>
    double norm2 (const Vector<Size,std::complex<double> >& vector);

    /**
     * Computes the absolute component values of the vector, creating a
     * temporary vector to hold the result.
     *
     * @return A copy of the temporary holding the result.
     */
    template<int Size, typename Scalar>
    Vector<Size,Scalar> abs(const Vector<Size,Scalar>& vector);

    /**
     * Specialisation that is different to standard definition. Works as result
     * type is not part of the signature
     */
    template<int Size>
    Vector<Size,double> abs(const Vector<Size,std::complex<double> >& vector);

    /**
     * Sums up the component values of the vector.
     */
    template<int Size, typename Scalar>
    Scalar sum(const Vector<Size,Scalar>& vector);


    /**
     * Computes the volume of the tetrahedron spanned by the Cartesian unit vectors
     * scaled by the corresponding components of the given vector.
     */
    template<int Size, typename Scalar>
    Scalar volume(const Vector<Size,Scalar>& vector);

    /**
     * Computes the volume of the tetrahedron spanned by the Cartesian unit vectors
     * scaled by the corresponding components of the given vector.
     */
    template<int Size, typename Scalar>
    Scalar average(const Vector<Size,Scalar>& vector);

    /**
     * Returns the index of the element with maximal value (NOT absolute value).
     */
    template<int Size, typename Scalar>
    int indexMax (const Vector<Size,Scalar>& vector);

    /**
     * Returns the index of the element with minimal value (NOT absolute value).
     */
    template<int Size, typename Scalar>
    int indexMin (const Vector<Size,Scalar>& vector);

    /**
     * Returns the element with maximal value (NOT absolute value).
     */
    template<int Size, typename Scalar>
    Scalar max (const Vector<Size,Scalar>& vector);

    /**
     * Returns the element with maximal absolute value.
     */
    template<int Size, typename Scalar>
    Scalar maxAbs (const Vector<Size,Scalar>& vector);

    /**
     * Returns the element with minimal value (NOT absolute value).
     */
    template<int Size, typename Scalar>
    Scalar min (const Vector<Size,Scalar>& vector);


    template<int Size, typename Scalar>
    Vector<Size,Scalar> invertEntries (
      const Vector<Size,Scalar>&  vector
    );

    template<int Size>
    Vector<Size,double> real(
      const Vector<Size,std::complex<double> >&  vector
    );

    template<int Size>
    Vector<Size,double> imag(
      const Vector<Size,std::complex<double> >&  vector
    );

    template<int Size>
    double maxReal(
      const Vector<Size,std::complex<double> >&  vector
    );

    template<int Size>
    double maxImag(
      const Vector<Size,std::complex<double> >&  vector
    );
  }
}


/**
 * Streams the component values into a comma separated representation.
 */
template<int Size, typename Scalar>
std::ostream& operator<<(std::ostream&  os, const tarch::la::Vector<Size,Scalar>&  vector);


#include "VectorOperations.cpph"

#endif
