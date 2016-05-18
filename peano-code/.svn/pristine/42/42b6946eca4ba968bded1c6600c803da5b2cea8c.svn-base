#include "peano/utils/Loop.h"
#include "peano/utils/PeanoOptimisations.h"

//#include "tarch/la/Utilities.h"


namespace {
  int dLinearisedNotOptimised( const tarch::la::Vector<DIMENSIONS,int> & counter, int max ) {
    int result = counter(DIMENSIONS - 1);
    assertion2(counter(DIMENSIONS - 1) >= 0, counter, max);
    assertion2(counter(DIMENSIONS - 1) < max, counter, max);
    for (int d = DIMENSIONS - 2; d >= 0; d--) {
      assertion2(counter(d) >= 0, counter(d), max);
      assertion2(counter(d) < max, counter(d), max);
      result = result * max + counter(d);
    }
    return result;
  }


  tarch::la::Vector<DIMENSIONS,int> dDelinearisedNotOptimised(int value, int max ) {
    assertion2(value >= 0, value, max);

    tarch::la::Vector<DIMENSIONS,int> result(0);
    for (int d=DIMENSIONS-1; d>=0; d--) {
      result(d)  = value /  tarch::la::aPowI(d,max);
      value     -= result(d) * tarch::la::aPowI(d,max);
    }
    return result;
  }


  #ifdef DloopOptimiseAggressive
  const int MaxIndexOfLinearization = FOUR_POWER_D;
  int       LookupTableDLinearised[FOUR_POWER_D*3];
  tarch::la::Vector<DIMENSIONS,int> LookupTableDDeLinearised[FOUR_POWER_D*3];

  int getKeyForDLinearised(const tarch::la::Vector<DIMENSIONS,int>& counter, int max) {
    int result = 0;
    int key    = 1;

    for (int d=0; d<DIMENSIONS; d++) {
      result += key * counter(d);
      key    *= 4;
    }

    assertionEquals( key, FOUR_POWER_D );
    result += (max-2)*FOUR_POWER_D;

    return result;
  }


  int getKeyForDDeLinearised(int value, int max) {
    return value + (max-2)*FOUR_POWER_D;
  }
  #endif
}


void peano::utils::setupLookupTableForDLinearised() {
  #ifdef DloopOptimiseAggressive
  dfor(k,2) {
    LookupTableDLinearised[getKeyForDLinearised(k,2)] = dLinearisedNotOptimised(k,2);
  }
  dfor(k,3) {
    LookupTableDLinearised[getKeyForDLinearised(k,3)] = dLinearisedNotOptimised(k,3);
  }
  dfor(k,4) {
    LookupTableDLinearised[getKeyForDLinearised(k,4)] = dLinearisedNotOptimised(k,4);
  }
  #endif
}


void peano::utils::setupLookupTableForDDelinearised() {
  #ifdef DloopOptimiseAggressive
  for (int value=0; value<FOUR_POWER_D; value++) {
    LookupTableDDeLinearised[getKeyForDDeLinearised(value,2)] = dDelinearisedNotOptimised(value,2);
    LookupTableDDeLinearised[getKeyForDDeLinearised(value,3)] = dDelinearisedNotOptimised(value,3);
    LookupTableDDeLinearised[getKeyForDDeLinearised(value,4)] = dDelinearisedNotOptimised(value,4);
  }
  #endif
}

int peano::utils::dLinearisedWithoutLookup( const tarch::la::Vector<DIMENSIONS,int>& counter, int max ) {
	return dLinearisedNotOptimised(counter,max);
}


int peano::utils::dLinearised( const tarch::la::Vector<DIMENSIONS,int>& counter, int max ) {
  #ifdef DloopOptimiseAggressive
    assertionEquals2(LookupTableDLinearised[getKeyForDLinearised(counter,max)], dLinearisedNotOptimised(counter,max), counter,max);
    return LookupTableDLinearised[getKeyForDLinearised(counter,max)];
  #else
    return dLinearisedNotOptimised(counter,max);
  #endif
}


int peano::utils::d2Linearised( const tarch::la::Vector<2,int>& counter, int max ) {
  int result = counter(2 - 1);
  assertion2(counter(2 - 1) >= 0, counter, max);
  assertion2(counter(2 - 1) < max, counter, max);
  for (int d = 2 - 2; d >= 0; d--) {
    assertion3(counter(d) >= 0, counter, d, max);
    assertion3(counter(d) < max, counter, d, max);
    result = result * max + counter(d);
  }
  return result;
}


int peano::utils::d3Linearised( const tarch::la::Vector<3,int>& counter, int max ) {
  int result = counter(3 - 1);
  assertion2(counter(3 - 1) >= 0, counter, max);
  assertion2(counter(3 - 1) < max, counter, max);
  for (int d = 3 - 2; d >= 0; d--) {
    assertion2(counter(d) >= 0, counter(d), max);
    assertion2(counter(d) < max, counter(d), max);
    result = result * max + counter(d);
  }
  return result;
}


tarch::la::Vector<DIMENSIONS,int> peano::utils::dDelinearisedWithoutLookup(int value, int max) {
  return dDelinearisedNotOptimised(value, max);
}

tarch::la::Vector<DIMENSIONS,int> peano::utils::dDelinearised(int value, int max ) {
  #ifdef DloopOptimiseAggressive
    for (int d=0; d<DIMENSIONS; d++) {
      assertionEquals2(LookupTableDDeLinearised[getKeyForDDeLinearised(value,max)](d), dDelinearisedNotOptimised(value,max)(d),value,max);
    }
    return LookupTableDDeLinearised[getKeyForDDeLinearised(value,max)];
  #else
    return dDelinearisedNotOptimised(value,max);
  #endif
}


void peano::utils::dInc(tarch::la::Vector<DIMENSIONS,int>& counter, int max) {
  counter(0)++;
  for (int i=0; i<DIMENSIONS-1; i++) {
    if ( counter(i) >= max ) {
      counter(i) = 0;
      counter(i+1)++;
    }
  }
}


void peano::utils::dDec(tarch::la::Vector<DIMENSIONS,int>& counter, int max) {
  for (int i = 0; i < DIMENSIONS - 1; i++) {
    assertion(counter(i) >= 0);
  }

  counter(0)--;
  for (int i=0; i<DIMENSIONS-1; i++) {
    if ( counter(i) < 0 ) {
      counter(i) = max-1;
      counter(i+1)--;
    }
  }
}


void peano::utils::dInc(tarch::la::Vector<DIMENSIONS,int>& counter, const tarch::la::Vector<DIMENSIONS,int>& max) {
  counter(0)++;
  for (int i=0; i<DIMENSIONS-1; i++) {
    if ( counter(i) >= max(i) ) {
      counter(i) = 0;
      counter(i+1)++;
    } else {
      break;
    }
  }
}

void peano::utils::dIncByVector(tarch::la::Vector<DIMENSIONS,int>& counter, int max, int increment) {
  counter(0) += increment;
   for (int i=0; i<DIMENSIONS-1; i++) {
     if ( counter(i) >= max ) {
       counter(i) %= max;
       counter(i+1) += increment;
     } else {
       break;
     }
   }
}


void peano::utils::dIncByScalar(tarch::la::Vector<DIMENSIONS,int>& counter, int max, int increment) {
  counter(0) += increment;
  for (int i=0; i<DIMENSIONS-1; i++) {
    while ( counter(i) >= max ) {
      counter(i) -= max;
      counter(i+1) ++;
    }
  }
}


void peano::utils::dInc(tarch::la::Vector<DIMENSIONS,int>& counter, int max, int doNotExamine) {
  assertion( doNotExamine<DIMENSIONS);
  assertion( doNotExamine>=-1);

  if (doNotExamine!=0) {
    counter(0)++;
  }
  else {
    counter(1)++;
  }
  for (int i=0; i<DIMENSIONS-1; i++) {
    if ( counter(i) >= max ) {
      counter(i) = 0;
      if ( doNotExamine != i+1) {
        counter(i+1)++;
      }
      else {
        if ( doNotExamine != DIMENSIONS-1 ) {
          counter(i+2)++;
        }
        else {
          counter(DIMENSIONS-1) = max;
        }
      }
    }
  }
}


void peano::utils::dInc(tarch::la::Vector<DIMENSIONS,int>& counter, int max, peano::utils::LoopDirection& direction ) {
  for (int i=0; i<DIMENSIONS; i++) {
    if ( direction[i] ) {
      counter(i)++;
      bool greater = counter(i) >= max;
      if (greater && (i==DIMENSIONS-1)) {
        counter(i)=max;
        return;
      }
      else if (greater) {
        counter(i)--;
        direction[i] = !direction[i];
      }
      else return;
    }
    else {
      counter(i)--;
      bool smaller = counter(i) < 0;
      if (smaller && (i==DIMENSIONS-1)) {
        counter(i)=max;
        return;
      }
      else if (smaller) {
        counter(i)++;
        direction[i] = !direction[i];
      }
      else return;
    }
  }
}


int peano::utils::dCmp(const tarch::la::Vector<DIMENSIONS,int>& counter, const tarch::la::Vector<DIMENSIONS,int>& max) {
  int result = true;
  for (int i=0; i<DIMENSIONS; i++) {
    assertion2( counter(i)<=max(i), counter,max );
    assertion2( counter(i)>=0,   counter,max );
    result &= (counter(i)<max(i));
  }
  return result;
}


int peano::utils::dCmp(const tarch::la::Vector<DIMENSIONS,int>& counter, int max) {
  int result = true;
  for (int i=0; i<DIMENSIONS; i++) {
    assertion2( counter(i)<=max, counter,max );
    assertion2( counter(i)>=0,   counter,max );
    result &= (counter(i)<max);
  }
  return result;
}


bool peano::utils::dCmpLinearOrder(const tarch::la::Vector<DIMENSIONS,int>& counter, const tarch::la::Vector<DIMENSIONS,int>& max) {
  for (int i = DIMENSIONS - 1; i >= 0; i--) {
    if (counter(i) > max(i)) {
      return false;
    } else if (counter(i) < max(i)) {
      return true;
    }
  }
  return false;
}

tarch::la::Vector<DIMENSIONS,int> peano::utils::dStartVector() {
  tarch::la::Vector<DIMENSIONS,int> result(0);
  return result;
}


tarch::la::Vector<DIMENSIONS,int> peano::utils::dStartVector(int dim, int value) {
  tarch::la::Vector<DIMENSIONS,int> result(0);
  assertion2( dim >= 0, dim, value );
  assertion2( dim <  DIMENSIONS, dim, value );
  assertion2( value >= 0, dim, value );
  result( dim ) = value;
  return result;
}


tarch::la::Vector<DIMENSIONS,int> peano::utils::dStartVector( int max, const peano::utils::LoopDirection& direction ) {
  tarch::la::Vector<DIMENSIONS,int> result;
  assertion2( max>0 && max<=3, max, direction );
  for (int i=0; i<DIMENSIONS; i++) {
    if (direction[i]) {
      result(i) = 0;
    }
    else {
      result(i) = max-1;
    }
  }
  return result;
}
