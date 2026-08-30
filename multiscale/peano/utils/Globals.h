// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org

/**
 * This file defines some macros and definitions used by most of the files of
 * the project. Among the definition of assertions and the global constants
 * (DIMENSIONS and NUMBER_OF_VERTICES_PER_ELEMENT) are the numerical comparison
 * operations.
 *
 * @version $Revision: 1.70 $
 * @author Tobias Weinzierl, Tobias Neckel
 *
 * @todo Doku fuer macros in Globals.h vervollstaendigen!
 */
#ifndef _PEANO_UTILS_GLOBALS_H_
#define _PEANO_UTILS_GLOBALS_H_

  /**
   * Required for the inexact (numerical) comparisons.
   */
  #include <limits>

  /**
   * The IO stream operations are required for the assertion error messages.
   */
  #include <iostream>

  /**
   * Needed for the assertion macros.
   */
  #include <cstdlib>

  /**
   * Number of dimensions the code is working on. Furthermore the constant
   * NUMBER_OF_VERTICES_PER_ELEMENT is set. The value of this constant is
   * $f 2^d $f.
   */
  #include "peano/utils/Dimensions.h"

  #include "tarch/compiler/CompilerSpecificSettings.h"


  template <int base, int exponent>
  class BasePowerExponent {
    public:
    enum {value = base*BasePowerExponent<base,exponent-1>::value};
  };

  template <int base>
  class BasePowerExponent<base,0> {
    public:
    enum {value = 1};
  };

  #define TWO_POWER_D   (BasePowerExponent<2,DIMENSIONS>::value)
  #define TWO_POWER_D_TIMES_D   (BasePowerExponent<2,DIMENSIONS>::value*DIMENSIONS)
  #define TWO_POWER_D_MINUS_ONE (BasePowerExponent<2,DIMENSIONS>::value-1)
  #define TWO_POWER_D_TIMES_TWO_POWER_D (BasePowerExponent<2,DIMENSIONS>::value*BasePowerExponent<2,DIMENSIONS>::value)
  #define TWO_POWER_D_TIMES_TWO_POWER_D_DIVIDED_BY_TWO (BasePowerExponent<2,DIMENSIONS>::value*BasePowerExponent<2,DIMENSIONS-1>::value)
  #define TWO_POWER_D_TIMES_THREE_POWER_D (BasePowerExponent<2,DIMENSIONS>::value*BasePowerExponent<3,DIMENSIONS>::value)
  #define TWO_POWER_D_TIMES_FIVE_POWER_D (BasePowerExponent<2,DIMENSIONS>::value*BasePowerExponent<5,DIMENSIONS>::value)
  #define TWO_POWER_D_TIMES_THREE_POWER_D_DIVIDED_BY_THREE (BasePowerExponent<2,DIMENSIONS>::value*BasePowerExponent<3,DIMENSIONS-1>::value)
  #define TWO_POWER_D_DIVIDED_BY_TWO (BasePowerExponent<2,DIMENSIONS-1>::value)
  #define TWO_POWER_D_TIMES_D (BasePowerExponent<2,DIMENSIONS>::value*DIMENSIONS)
  #define TWO_POWER_TWO_POWER_D (BasePowerExponent<2,TWO_POWER_D>::value)
  #define THREE_POWER_D (BasePowerExponent<3,DIMENSIONS>::value)
  #define THREE_POWER_D_MINUS_ONE (BasePowerExponent<3,DIMENSIONS>::value-1)
  #define THREE_POWER_D_DIVIDED_BY_THREE (BasePowerExponent<3,DIMENSIONS-1>::value)
  #define THREE_POWER_D_TIMES_FOUR_POWER_D (BasePowerExponent<3,DIMENSIONS>::value*BasePowerExponent<4,DIMENSIONS>::value)
  #define FOUR_POWER_D  (BasePowerExponent<4,DIMENSIONS>::value)
  #define FOUR_POWER_D_DIVIDED_BY_FOUR  (BasePowerExponent<4,DIMENSIONS-1>::value)
  #define FIVE_POWER_D  (BasePowerExponent<5,DIMENSIONS>::value)
  #define FIVE_POWER_D_DIVIDED_BY_FIVE  (BasePowerExponent<5,DIMENSIONS-1>::value)
  #define SIX_POWER_D  (BasePowerExponent<6,DIMENSIONS>::value)
  #define SEVEN_POWER_D  (BasePowerExponent<7,DIMENSIONS>::value)
  #define NINE_POWER_D  (BasePowerExponent<9,DIMENSIONS>::value)
  #define ELEVEN_POWER_D  (BasePowerExponent<11,DIMENSIONS>::value)
  #define THIRTEEN_POWER_D  (BasePowerExponent<13,DIMENSIONS>::value)
  #define FIFTEEN_POWER_D  (BasePowerExponent<15,DIMENSIONS>::value)
  #define SEVENTEEN_POWER_D  (BasePowerExponent<17,DIMENSIONS>::value)
  #define NUMBER_OF_VERTICES_PER_ELEMENT TWO_POWER_D
  #define DIMENSIONS_TIMES_TWO (2*DIMENSIONS)
  #define DIMENSIONS_TIMES_TWO_POWER_D (DIMENSIONS*TWO_POWER_D)
  #define DIMENSIONS_TIMES_FOUR (4*DIMENSIONS)

  int threePowI(int i);
  int fourPowI(int i);
  int twoPowI(int i);
  int dPowI(int i);

  void setupLookupTableForThreePowI();
  void setupLookupTableForFourPowI();
  void setupLookupTableForTwoPowI();
  void setupLookupTableForDPowI();

  /**
   * The number of temporary stacks equals @f$ 2d @f$. This
   * constant holds the number of temporary stacks that have to be provided
   * by each implementation.
   */
  #define NUMBER_OF_TEMPORARY_STACKS    2*DIMENSIONS

  #define SMALLEST_VALUE (-2*DIMENSIONS-1)
  #define BIGGEST_VALUE (2*DIMENSIONS+1)

  /**
   * Number of edge dofs stored in vertices.
   * Necessary for IDO computations, e.g.
   */
  #define VERTEX_NUMBER_OF_EDGE_DOFS   DIMENSIONS
  /**
   * Number of face dofs stored in vertices. this value is valid for 2D (0) and
   * 3D (3). For higher dimensions than 3, it has not been designed!
   * Necessary for IDO computations, e.g.
   */
  #define VERTEX_NUMBER_OF_FACE_DOFS   ((DIMENSIONS-2)*DIMENSIONS)
  /**
   * Total number of dofs stored in vertices (i.e. vertex itself, plus all
   * necessary edge and face dofs).
   * Necessary for IDO computations, e.g.
   */
  #define VERTEX_TOTAL_NUMBER_OF_DOFS  (1 + VERTEX_NUMBER_OF_EDGE_DOFS + VERTEX_NUMBER_OF_FACE_DOFS)
  /**
   * Total number of dofs stored in vertices times d. This is used for
   * serialised "multidimensional" arrays (such as velocities).
   * Necessary for IDO computations, e.g.
   */
  #define VERTEX_TOTAL_NUMBER_OF_DOFS_TIMES_DIM (VERTEX_TOTAL_NUMBER_OF_DOFS * DIMENSIONS)

  /**
   * Number of edge dofs stored in vertices times d. This is used for
   * serialised "multidimensional" arrays (helper data).
   * Necessary for IDO fluid computations, e.g.
   *
   * Is the same as DIMENSIONS_TIMES_DIMENSIONS, but we keep it nevertheless.
   */
  #define VERTEX_NUMBER_OF_EDGE_DOFS_TIMES_DIM (VERTEX_NUMBER_OF_EDGE_DOFS * DIMENSIONS)

  /**
   * d*d. Necessary for IDO fluid (helper data).
   */
  #define DIMENSIONS_TIMES_DIMENSIONS (DIMENSIONS*DIMENSIONS)

#endif
