// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org


/**
 * This file defines some macros for d-dimensional loops.
 *
 * @version $Revision: 1.10 $
 * @author Tobias Weinzierl
 */
#ifndef _PEANO_UTILS_LOOP_H_
#define _PEANO_UTILS_LOOP_H_

#include "peano/utils/Globals.h"
#include "tarch/la/Vector.h"
#include "tarch/multicore/Loop.h"


#include <bitset>

namespace peano {
  namespace utils {
    /**
     * Is used by the z-loop. See macro dforz.
     */
    typedef std::bitset<DIMENSIONS> LoopDirection;

    /**
     * This operation performs a d-dimensional increment on a given integer vector:
     * The first component of the vector is incremented. If the first component is
     * greater than max-1, the component is set zero and the next component is
     * incremented by one. This operation is used often by d-dimensional for-loops.
     */
    void dInc(tarch::la::Vector<DIMENSIONS,int>& counter, int max);

    /**
       * This operation performs a d-dimensional decrement on a given integer vector:
       * The first component of the vector is decremented. If the first component is
       * smaller than 0, the component is set to max and the next component is
       * decremented by one.
       */
    void dDec(tarch::la::Vector<DIMENSIONS,int>& counter, int max);

    /**
     * This operation performs a d-dimensional increment on a given integer vector:
     * The first component of the vector is incremented. If the first component is
     * greater than max(0)-1, the component is set zero and the next component is
     * incremented by one. This operation is used often by d-dimensional for-loops.
     */
    void dInc(tarch::la::Vector<DIMENSIONS,int>& counter, const tarch::la::Vector<DIMENSIONS,int>& max);

    /**
     * Perform a d-dimensional increment by value increment: The first component
     * of the counter is incremented by increment. Afterwards, the operation
     * checks the first entry: If it exceeds max, its module value is set, the
     * next component is incremented by increment, and the check continues.
     */
    void dIncByVector(tarch::la::Vector<DIMENSIONS,int>& counter, int max, int increment);

    /**
     * Perform a scalar increment of a vector: The operation equals a sequence of
     * increment calls to dInc().
     */
    void dIncByScalar(tarch::la::Vector<DIMENSIONS,int>& counter, int max, int increment);

    /**
     * Same operation as dInc(tarch::la::Vector<DIMENSIONS,int>,int), but now one dimension is not taken
     * into consideration.
     */
    void dInc(tarch::la::Vector<DIMENSIONS,int>& counter, int max, int doNotExamine);

    /**
     * Operation similar to dInc, but is given a direction bitset that identifies
     * whether the counters has to be incremented or decremented. See the dforz
     * macro for an example how to use dInc.
     */
    void dInc(tarch::la::Vector<DIMENSIONS,int>& counter, int max, LoopDirection& direction );

    /**
     * Element-wise comparison for the for loops.
     * @return true if all entries of counter are smaller max
     */
    int dCmp(const tarch::la::Vector<DIMENSIONS,int>& counter, int max);

    /**
     * Element-wise comparison for the loops.
     * @return true if all entries of counter are smaller than their corresponding
     *         entries in max
     */
    int dCmp(const tarch::la::Vector<DIMENSIONS,int>& counter, const tarch::la::Vector<DIMENSIONS,int>& max);

    /**
     * compares two vectors with regards to their linearised value.
     *
     * @returns true, if dLinearised(counter, XXX) < dLinearised(max, XXX)
     */
    bool dCmpLinearOrder(const tarch::la::Vector<DIMENSIONS,int>& counter, const tarch::la::Vector<DIMENSIONS,int>& max);

    /**
     * This operation is called pretty often and, thus, might cause a significant
     * slowdown in the overall performance. Therefore, I introduced a aggressive
     * optimization based on lookup tables. This optimization is switched on if
     * DLOOP_AGGRESSIVE is specified (default in peano project). Two preconditions
     * have to be fulfilled in this case: All parameters have to stay within
     * certain boundaries (all positive, max smaller or equal to 5)
     * and one has to call both setupLookupTableForDLinearised() and
     * setupLookupTableForDDelinearised() before using dLinearised() or
     * dDelinearised().
     *
     * Obviously, creating a lookup table for these two operations is not that
     * simple, since the parameter space has to be mapped onto a unique key. To
     * end up with a simple mapping, all the constraints from above are added.
     * Although the mapping might be slow, it is still faster than computing the
     * partial sums of a to the power of b.
     *
     * @return the linearisation of the counter, i.e. the k-th component is
     *         multiplied by max^k and the results are accumulated.
     */
    int dLinearised( const tarch::la::Vector<DIMENSIONS,int>& counter, int max );

    /**
     * Special 2d variant of dLinearised that works also if you compile with other
     * dimensions.
     */
    int d2Linearised( const tarch::la::Vector<2,int>& counter, int max );

    /**
     * Special 3d variant of dLinearised that works also if you compile with other
     * dimensions.
     */
    int d3Linearised( const tarch::la::Vector<3,int>& counter, int max );

    /**
     * Linearisation not Optimised
     *
     * This operation's semantics equals dLinearised, but the operation is not
     * optimised at all. It thus allows to have arbitrary argument values. Yet,
     * this version is not optimised, i.e. it might become a bottleneck.
     */
    int dLinearisedWithoutLookup( const tarch::la::Vector<DIMENSIONS,int>& counter, int max );

    /**
     * Counterpart of dLinearised().
     *
     * This operation's semantics equals dDeLinearised, but the operation is not
     * optimised at all. It thus allows to have arbitrary argument values. Yet,
     * this version is not optimised, i.e. it might become a bottleneck.
     */
    tarch::la::Vector<DIMENSIONS,int> dDelinearised(int value, int max );

    /**
     * Delinearization not optimised.
     */
    tarch::la::Vector<DIMENSIONS,int> dDelinearisedWithoutLookup(int value, int max);


    void setupLookupTableForDLinearised();
    void setupLookupTableForDDelinearised();

    /**
     * @return a vector containing zero values only.
     */
    tarch::la::Vector<DIMENSIONS,int> dStartVector();

    /**
     * @return a vector containing only zero values besides the dim-th entry. This
     *         entry is set value.
     */
    tarch::la::Vector<DIMENSIONS,int> dStartVector(int dim, int value);

    /**
     * Creates a start vector. Each component is set either 0 or max-1 depending
     * on direction: If direction is true, then the value 0 is zero.
     *
     * @return a start vector for an osciallating loop.
     */
    tarch::la::Vector<DIMENSIONS,int> dStartVector( int max, const LoopDirection& direction );
  }
}


/**
 * Very often one needs a d-dimensional for loop. A d-dimensional for loop is
 * something like
 * \code
 *   for (x(0)=0; x(0)<N; x(0)++)
 *    for (x(1)=0; x(1)<N; x(1)++)
 *     for (x(2)=0; x(2)<N; x(2)++)
 * \endcode
 * with d nested for loops. Thus, one has to code such loops for every d
 * manually. This macro offers a d-independend alternative, just write
 * \code
 *   dfor (x,N) {
 *   ...
 *   }
 * \endcode
 * The precompiler extracts this macro and within the loop body, you are able
 * to use the integer tinyvector x.
 *
 * Here is an example:
 * \code
 *   dfor(a,2) {
 *     std::cout << a << ",";
 *   }
 * \endcode
 * results in [0,0], [1,0], [0,1], [1,1] if DIMENSIONS equals 2. If DIMENSION
 * equals 3 the same construct gives you [0,0,0], [1,0,0], [0,1,0], [1,1,0],
 * [0,0,1], [1,0,1], [0,1,1], [1,1,1].
 */
#define dfor(counter,max) \
  for (tarch::la::Vector<DIMENSIONS,int> counter = peano::utils::dStartVector(); peano::utils::dCmp(counter,max); peano::utils::dInc(counter,max) )

/**
 * Shortcut For dfor(counter,4)
 *
 * The usage of this optimised shortcut differs from dfor: You have to
 * replace both the dfor and the opening bracket by this macro, i.e.
 *
 * \code
 * dfor(counter,4) {
 * \endcode
 *
 * becomes
 *
 * \code
 * dfor4(counter)
 * \endcode
 *
 * You usually use this macro with
 * \code
 * #pragma unroll(FOUR_POWER_D)
 * \endcode
 * or
 * \code
 * #pragma omp parallel for schedule(static)
 * \endcode
 *
 * If you work with this specialised version of dfor on a variable k, two
 * counter variables are available within the loop's scope. The variable k
 * itself with type tarch::la::Vector<DIMENSIONS,int>. Furthermore, there's always a variable
 * kScalar giving you k's value linearised.
 */
#define dfor4(counter) \
  for( int counter##Scalar=0; counter##Scalar<FOUR_POWER_D; counter##Scalar++) { \
    tarch::la::Vector<DIMENSIONS,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=DIMENSIONS-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
          counter##aPowI *= 4; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}


/**
 * Shortcut For dfor(counter,3)
 *
 * The usage of this optimised shortcut differs from dfor: You have to
 * replace both the dfor and the opening bracket by this macro, i.e.
 *
 * \code
 * dfor(counter,3) {
 * \endcode
 *
 * becomes
 *
 * \code
 * dfor3(counter)
 * \endcode
 *
 * You usually use this macro with
 * \code
 * #pragma unroll(THREE_POWER_D)
 * \endcode
 * or
 * \code
 * #pragma omp parallel for schedule(static)
 * \endcode
 *
 * If you work with this specialised version of dfor on a variable k, two
 * counter variables are available within the loop's scope. The variable k
 * itself with type tarch::la::Vector<DIMENSIONS,int>. Furthermore, there's always a variable
 * kScalar giving you k's value linearised.
 */
#define dfor3(counter) \
  for( int counter##Scalar=0; counter##Scalar<THREE_POWER_D; counter##Scalar++) { \
    tarch::la::Vector<DIMENSIONS,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=DIMENSIONS-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 3; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}


#define dfor5(counter) \
  for( int counter##Scalar=0; counter##Scalar<FIVE_POWER_D; counter##Scalar++) { \
    tarch::la::Vector<DIMENSIONS,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=DIMENSIONS-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 5; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}

#define dfor7(counter) \
  for( int counter##Scalar=0; counter##Scalar<SEVEN_POWER_D; counter##Scalar++) { \
    tarch::la::Vector<DIMENSIONS,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=DIMENSIONS-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 7; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}


#define dfor9(counter) \
  for( int counter##Scalar=0; counter##Scalar<NINE_POWER_D; counter##Scalar++) { \
    tarch::la::Vector<DIMENSIONS,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=DIMENSIONS-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 9; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}

/**
 * If DIMENSIONS is not set to two, we might nevertheless need
 * two-dimensional loops. So this is the corresponding macro. It is
 * way slower than dfor if you compile with Dim2.
 *
 * Please use this macro with an enddforx macro closing your scope rather than
 * brackets.
 *
 * Please note that counterScalar is already a linearised version of your counter.
 *
 * Please note that you need a specialised linearisation function (depending on d
 * explicitly) to work with 2d index vectors within such a loop. Do not just use
 * dLinearised, but use the d2Linearised or d3Linearised variant instead.
 */
#define d2for(counter,max) \
  for( int counter##Scalar=0; counter##Scalar<tarch::la::aPowI(2,max); counter##Scalar++) { \
    tarch::la::Vector<2,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=2-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= max; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}

/**
 * If DIMENSIONS is not set to two, we might nevertheless need
 * two-dimensional loops. So this is the corresponding macro.
 *
 * Please use enddforx to close a loop started with this macro.
 */
#define d2for2(counter) \
  for( int counter##Scalar=0; counter##Scalar<tarch::la::aPowI(2,2); counter##Scalar++) { \
    tarch::la::Vector<2,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=2-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 2; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}

/**
 * If DIMENSIONS is not set to three, we might nevertheless need
 * three-dimensional loops. So this is the corresponding macro.
 *
 * Please use enddforx to close a loop started with this macro.
 */
#define d3for2(counter) \
  for( int counter##Scalar=0; counter##Scalar<tarch::la::aPowI(3,2); counter##Scalar++) { \
    tarch::la::Vector<3,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=3-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 2; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}



/**
 * If DIMENSIONS is not set to three, we might nevertheless need
 * two-dimensional loops. So this is the corresponding macro. It is
 * way slower than dfor if you compile with Dim2.
 *
 * Please use this macro with an enddforx macro closing your scope rather than
 * brackets.
 *
 * Please note that counterScalar is already a linearised version of your counter.
 *
 * Please note that you need a specialised linearisation function (depending on d
 * explicitly) to work with 2d index vectors within such a loop. Do not just use
 * dLinearised, but use the d2Linearised or d3Linearised variant instead.
 */
#define d3for(counter,max) \
  for( int counter##Scalar=0; counter##Scalar<tarch::la::aPowI(3,max); counter##Scalar++) { \
    tarch::la::Vector<3,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=3-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= max; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}


/**
 * If DIMENSIONS is not set to two, we might nevertheless need
 * two-dimensional loops. So this is the corresponding macro.
 *
 * Please use enddforx to close a loop started with this macro.
 */
#define d2for3(counter) \
  for( int counter##Scalar=0; counter##Scalar<tarch::la::aPowI(2,3); counter##Scalar++) { \
    tarch::la::Vector<2,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=2-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 3; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}

/**
 * If DIMENSIONS is not set to three, we might nevertheless need
 * three-dimensional loops. So this is the corresponding macro.
 *
 * Please use enddforx to close a loop started with this macro.
 */
#define d3for3(counter) \
  for( int counter##Scalar=0; counter##Scalar<tarch::la::aPowI(3,3); counter##Scalar++) { \
    tarch::la::Vector<3,int> counter; \
    { \
    int   copy##counter##Scalar = counter##Scalar; \
    for (int counter##ddd=3-1; counter##ddd>=0; counter##ddd--) { \
      int counter##aPowI = 1; \
      for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
        counter##aPowI *= 3; \
      } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
    }}


/**
 * Shortcut For dfor(counter,2)
 *
 * The usage of this optimised shortcut differs from dfor: You have to
 * replace both the dfor and the opening bracket by this macro, i.e.
 *
 * \code
 * dfor(counter,2) {
 * \endcode
 *
 * becomes
 *
 * \code
 * dfor2(counter)
 * \endcode
 *
 * You usually use this macro with
 * \code
 * #pragma unroll(TWO_POWER_D)
 * \endcode
 * or
 * \code
 * #pragma omp parallel for schedule(static)
 * \endcode
 *
 * If you work with this specialised version of dfor on a variable k, two
 * counter variables are available within the loop's scope. The variable k
 * itself with type tarch::la::Vector<DIMENSIONS,int>. Furthermore, there's always a variable
 * kScalar giving you k's value linearised.
 */

/*
 * bit flipping used for DIMENSIONS = 2, and DIMENSIONS = 3
 * for more information about the idea principle used refer to https://opt-patterns.wiki.tum.de/dfor
 */ 
#if DIMENSIONS == 2 
#define dfor2(counter) \
  for( int counter##Scalar=0, AA##counter = 0, BB##counter = 0; counter##Scalar<TWO_POWER_D; counter##Scalar++) { \
  tarch::la::Vector<DIMENSIONS,int> counter; \
  counter(0) = AA##counter; \
  counter(1) = BB##counter; \
  AA##counter = !AA##counter; \
  BB##counter = !(AA##counter ^ BB##counter); 

#elif DIMENSIONS == 3
#define dfor2(counter) \
  for( int counter##Scalar=0, AA##counter = 0, BB##counter = 0, CC##counter = 0; counter##Scalar<TWO_POWER_D; counter##Scalar++) { \
  tarch::la::Vector<DIMENSIONS,int> counter; \
  counter(0) = AA##counter; \
  counter(1) = BB##counter; \
  counter(2) = CC##counter; \
  AA##counter = !AA##counter; \
  BB##counter = !(AA##counter ^ BB##counter); \
  CC##counter = CC##counter || (!AA##counter && !BB##counter && !CC##counter);  

#else 
#define dfor2(counter) \
  for( int counter##Scalar=0; counter##Scalar<TWO_POWER_D; counter##Scalar++) { \
    tarch::la::Vector<DIMENSIONS,int> counter; \
      { \
      int   copy##counter##Scalar = counter##Scalar; \
      for (int counter##ddd=DIMENSIONS-1; counter##ddd>=0; counter##ddd--) { \
        int counter##aPowI = 1; \
        for (int counter##jjj=0; counter##jjj<counter##ddd; counter##jjj++) { \
          counter##aPowI *= 2; \
        } \
      counter(counter##ddd) = copy##counter##Scalar /  counter##aPowI; \
      copy##counter##Scalar -= counter(counter##ddd) * counter##aPowI; \
      }}

#endif


/**
 * I prefer to use this macro for dforx instead of a closing bracket as many
 * syntax parser fail otherwise.
 */
#define enddforx }


/**
 * This is an exclusive d-dimensional for loop. Exclusive means, there is one
 * dimension that is not manipulated during the for loop. This dimension
 * (entry of the counter) is specified by dim and has the value value
 * throughout the for-loop.
 */
#define dfore(counter,max,dim,value) \
  for (tarch::la::Vector<DIMENSIONS,int> counter = peano::utils::dStartVector(dim,value); peano::utils::dCmp(counter,max); peano::utils::dInc(counter,max,dim) )


/**
 * This is a d-dimensional z-loop. A z-loop is a d-dimensional loop the
 * counter direction changes everytime an inner loop direction has changed.
 * So this is the loop corresponding to a Peano curve. The for loop is passed
 * a counter name, the number of steps to perform in each direction and a
 * direction flag that identifies the initial direction. Note that this
 * argument has to be a real variable, it might not be a constant. The
 * direction flag array identifies for each direction, whether the initial
 * loop goes along the axis or not. The type of direction is LoopDirection.
 *
 * Here are some examples for two dimensions:
 * \code
 *   LoopDirection d(3);  // equals {true,true} and identifies the standard
 *                        // Peano Leitmotiv
 *   zfor( a, 3, d ) {
 *     std::cout << a;
 *   }
 * \endcode
 * yields in [0,0],[1,0],[2,0],[2,1],[1,1],[0,1],[0,2],[1,2],[2,2].
 *
 * \code
 *   LoopDirection d(1);  // equals {true, false} and specifies a Peano curve
 *                        // from the left top to right bottom
 *   zfor( a, 3, d ) {
 *     std::cout << a;
 *   }
 * \endcode
 * yields in [0,2],[1,2],[2,2],[2,1],[1,1],[0,1],[0,0],[1,0],[2,0].
 */
 
 
#define zfor(counter,max,direction) \
  {for (tarch::la::Vector<DIMENSIONS,int> counter = peano::utils::dStartVector(max,direction); peano::utils::dCmp(counter,max); peano::utils::dInc(counter,max,direction) ) {





/* 
 * zfor3 is an optimized version of zfor for max = 3 
 * A lookup table is used for dim=2 and dim=3, for higher dimensions
 * the standard zfor is used instead
 */
#if DIMENSIONS == 2 
  static const int lookupzfor[4][9][2] = {
  {{2,2},{1,2},{0,2},{0,1},{1,1},{2,1},{2,0},{1,0},{0,0}},
    {{0,2},{1,2},{2,2},{2,1},{1,1},{0,1},{0,0},{1,0},{2,0}},
    {{2,0},{1,0},{0,0},{0,1},{1,1},{2,1},{2,2},{1,2},{0,2}},
    {{0,0},{1,0},{2,0},{2,1},{1,1},{0,1},{0,2},{1,2},{2,2}}
  };

#define zfor3(counter, direction) \
  { tarch::la::Vector<DIMENSIONS,int> counter; \
    int counter##initDir = static_cast<int>(direction.to_ulong()); \
    for (int counter##i = 0; counter##i < 9; ++counter##i) { \
      counter(0) = lookupzfor[counter##initDir][counter##i][0]; \
      counter(1) = lookupzfor[counter##initDir][counter##i][1]; 

#elif DIMENSIONS == 3
  static const int lookupzfor[8][27][3] = {
  {{2,2,2},{1,2,2},{0,2,2},{0,1,2},{1,1,2},{2,1,2},{2,0,2},{1,0,2},{0,0,2},{0,0,1},{1,0,1},{2,0,1},{2,1,1},{1,1,1},{0,1,1},{0,2,1},{1,2,1},{2,2,1},{2,2,0},{1,2,0},{0,2,0},{0,1,0},{1,1,0},{2,1,0},{2,0,0},{1,0,0},{0,0,0}},
    {{0,2,2},{1,2,2},{2,2,2},{2,1,2},{1,1,2},{0,1,2},{0,0,2},{1,0,2},{2,0,2},{2,0,1},{1,0,1},{0,0,1},{0,1,1},{1,1,1},{2,1,1},{2,2,1},{1,2,1},{0,2,1},{0,2,0},{1,2,0},{2,2,0},{2,1,0},{1,1,0},{0,1,0},{0,0,0},{1,0,0},{2,0,0}},
    {{2,0,2},{1,0,2},{0,0,2},{0,1,2},{1,1,2},{2,1,2},{2,2,2},{1,2,2},{0,2,2},{0,2,1},{1,2,1},{2,2,1},{2,1,1},{1,1,1},{0,1,1},{0,0,1},{1,0,1},{2,0,1},{2,0,0},{1,0,0},{0,0,0},{0,1,0},{1,1,0},{2,1,0},{2,2,0},{1,2,0},{0,2,0}},
    {{0,0,2},{1,0,2},{2,0,2},{2,1,2},{1,1,2},{0,1,2},{0,2,2},{1,2,2},{2,2,2},{2,2,1},{1,2,1},{0,2,1},{0,1,1},{1,1,1},{2,1,1},{2,0,1},{1,0,1},{0,0,1},{0,0,0},{1,0,0},{2,0,0},{2,1,0},{1,1,0},{0,1,0},{0,2,0},{1,2,0},{2,2,0}},
    {{2,2,0},{1,2,0},{0,2,0},{0,1,0},{1,1,0},{2,1,0},{2,0,0},{1,0,0},{0,0,0},{0,0,1},{1,0,1},{2,0,1},{2,1,1},{1,1,1},{0,1,1},{0,2,1},{1,2,1},{2,2,1},{2,2,2},{1,2,2},{0,2,2},{0,1,2},{1,1,2},{2,1,2},{2,0,2},{1,0,2},{0,0,2}},
    {{0,2,0},{1,2,0},{2,2,0},{2,1,0},{1,1,0},{0,1,0},{0,0,0},{1,0,0},{2,0,0},{2,0,1},{1,0,1},{0,0,1},{0,1,1},{1,1,1},{2,1,1},{2,2,1},{1,2,1},{0,2,1},{0,2,2},{1,2,2},{2,2,2},{2,1,2},{1,1,2},{0,1,2},{0,0,2},{1,0,2},{2,0,2}},
    {{2,0,0},{1,0,0},{0,0,0},{0,1,0},{1,1,0},{2,1,0},{2,2,0},{1,2,0},{0,2,0},{0,2,1},{1,2,1},{2,2,1},{2,1,1},{1,1,1},{0,1,1},{0,0,1},{1,0,1},{2,0,1},{2,0,2},{1,0,2},{0,0,2},{0,1,2},{1,1,2},{2,1,2},{2,2,2},{1,2,2},{0,2,2}},
    {{0,0,0},{1,0,0},{2,0,0},{2,1,0},{1,1,0},{0,1,0},{0,2,0},{1,2,0},{2,2,0},{2,2,1},{1,2,1},{0,2,1},{0,1,1},{1,1,1},{2,1,1},{2,0,1},{1,0,1},{0,0,1},{0,0,2},{1,0,2},{2,0,2},{2,1,2},{1,1,2},{0,1,2},{0,2,2},{1,2,2},{2,2,2}}
  };

#define zfor3(counter, direction) \
  { tarch::la::Vector<DIMENSIONS,int> counter; \
    int counter##initDir = static_cast<int>(direction.to_ulong()); \
    for (int counter##i = 0; counter##i < 27; ++counter##i) { \
      counter(0) = lookupzfor[counter##initDir][counter##i][0]; \
      counter(1) = lookupzfor[counter##initDir][counter##i][1]; \
      counter(2) = lookupzfor[counter##initDir][counter##i][2]; 

#else
#define zfor3(counter, direction) \
  zfor(counter, 3, direction)
  
#endif

  
#define endzfor }}    

#endif
