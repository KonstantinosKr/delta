// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_PLOTTER_VECTOR_COMPARE_VARIANTS_H_
#define _TARCH_PLOTTER_VECTOR_COMPARE_VARIANTS_H_

#include "tarch/la/VectorCompare.h"


namespace tarch {
  namespace plotter {
    template<int N>
    struct VectorCompare1e_14: public tarch::la::VectorCompare<N> {
      VectorCompare1e_14():
        tarch::la::VectorCompare<N>(1.0e-14) {}
    };


    template<int N>
    struct VectorCompare1e_12: public tarch::la::VectorCompare<N> {
      VectorCompare1e_12():
        tarch::la::VectorCompare<N>(1.0e-12) {}
    };


    template<int N>
    struct VectorCompare1e_8: public tarch::la::VectorCompare<N> {
      VectorCompare1e_8():
        tarch::la::VectorCompare<N>(1.0e-8) {}
    };


    template<int N>
    struct VectorCompare1e_4: public tarch::la::VectorCompare<N> {
      VectorCompare1e_4():
        tarch::la::VectorCompare<N>(1.0e-4) {}
    };
  }
}


#endif
