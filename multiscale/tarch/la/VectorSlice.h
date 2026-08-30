// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_WRAPPEDVECTOR_H_
#define _TARCH_LA_WRAPPEDVECTOR_H_


#include "tarch/la/Vector.h"


namespace tarch {
  namespace la {
    /**
     * Returns subpart of the vector. The name is due to historical reasons and
     * compability with Bernhard Gatzhammer's PreCiSE code. But the implementation
     * is extended such that one can also retrieve discontinuous parts of a vector.
     *
     * !!! Examples
     *
     * Take first 3 element from a vector:
     * \code
         tarch::la::slice<3>(myVector,0)
       \endcode
     *
     * Take the elements 2,3,5,6 from a vector:
     * \code
         tarch::la::slice<3>(myVector,2,2)
       \endcode
     */
    template<int SizeLhs, int SizeRhs, typename Scalar>
    Vector<SizeLhs,Scalar> slice(const Vector<SizeRhs,Scalar>& vector, int fromIndex, int stride = 1);

    /**
     * Setter
     */
    template<int SizeLhs, int SizeRhs, typename Scalar>
    void slice(Vector<SizeLhs,Scalar>& toVector, const Vector<SizeRhs,Scalar>& fromVector, int fromIndexInToVector, int strideInToVector = 1);
  }
}


#include "tarch/la/VectorSlice.cpph"

#endif
