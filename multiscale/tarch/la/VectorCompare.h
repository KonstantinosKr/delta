// Copyright (C) 2009 Technische Universitaet Muenchen
// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www5.in.tum.de/peano
#ifndef _TARCH_LA_VECTOR_COMPARE_H_
#define _TARCH_LA_VECTOR_COMPARE_H_


#include "tarch/la/Vector.h"


namespace tarch {
  namespace la {
    template<int N>
    struct VectorCompare;

    template<int N>
    struct StrictVectorCompare;
  }
}

/**
 * Comparison operator for boolean vectors.
 *
 * If you wanna store sets of vectors (e.g. in a map), the STL libraries are by
 * default falling back to a bit-wise comparison. This might not be of use if
 * you manage sets/maps of double vectors. In this case, you wanna the container
 * take into account machine precision. In such a case, declare the map e.g. as
 * follows:
 *  \code
  std::map<tarch::la::Vector<DIMENSIONS,double> , int, tarch::la::VectorCompare<DIMENSIONS>  >  _vertex2IndexMap;
 \endcode
 * @author Tobias Weinzierl
 */
template<int N>
struct tarch::la::VectorCompare {
  private:
    const double  _accuracy;
  public:
    VectorCompare(double accuracy=NUMERICAL_ZERO_DIFFERENCE):
      _accuracy(accuracy) {
    }


    bool operator()(
      const Vector<N,double>& left,
      const Vector<N,double>& right
    ) const {
      return firstGreater(right, left, _accuracy);
    }
};


/**
 * Strict lexicographic comparator for tarch::la::Vector used as a
 * std::map key. This is a strict weak ordering (unlike the tolerance-based
 * VectorCompare above), which std::map requires.
 */
template<int N>
struct tarch::la::StrictVectorCompare {
  bool operator()(
    const Vector<N,double>& lhs,
    const Vector<N,double>& rhs
  ) const {
    for (int i=0; i<N; i++) {
      if ( lhs(i) < rhs(i) ) return true;
      if ( rhs(i) < lhs(i) ) return false;
    }
    return false;
  }
};


#endif
