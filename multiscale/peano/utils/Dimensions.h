// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_UTILS_DIMENSIONS_H_
#define _PEANO_UTILS_DIMENSIONS_H_

  #ifdef Dim2
    #define DIMENSIONS 2
    #define DimensionIdentifier "2d"
  #elif defined(Dim3)
    #define DIMENSIONS 3
    #define DimensionIdentifier "3d"
  #elif defined(Dim4)
    #define DIMENSIONS 4
    #define DimensionIdentifier "4d"
  #elif defined(Dim5)
    #define DIMENSIONS 5
    #define DimensionIdentifier "5d"
  #elif defined(Dim6)
    #define DIMENSIONS 6
    #define DimensionIdentifier "6d"
  #elif defined(Dim7)
    #define DIMENSIONS 7
    #define DimensionIdentifier "7d"
  #elif defined(Dim8)
    #define DIMENSIONS 8
    #define DimensionIdentifier "8d"
  #elif defined(Dim9)
    #define DIMENSIONS 9
    #define DimensionIdentifier "9d"
  #elif defined(Dim10)
    #define DIMENSIONS 10
    #define DimensionIdentifier "10d"
  #else
    #error Dimension not supported yet. Ensure -DimX is set where X equals the dimension of your problem. Otherwise, edit utils/Dimensions.h. Please note that some compilers restrict the recursion depth for templates. Thus, for high dimensions one might have to define some constants manually.
  #endif


#ifdef Dim2
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim3
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim4
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim5
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim6
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim7
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim8
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim9
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim10
    #error Only one DimX compiler flag allowed!
  #endif
#endif



#ifdef Dim10
  #ifdef Dim2
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim3
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim4
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim5
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim6
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim7
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim8
    #error Only one DimX compiler flag allowed!
  #endif
  #ifdef Dim9
    #error Only one DimX compiler flag allowed!
  #endif
#endif


#endif
