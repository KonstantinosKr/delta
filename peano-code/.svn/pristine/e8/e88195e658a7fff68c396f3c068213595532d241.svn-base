#include "peano/utils/Globals.h"

#include "tarch/Assertions.h"
#include "tarch/la/ScalarOperations.h"

#include <cmath>

namespace {
  int LookupTableThreePowI[FOUR_POWER_D];
  int LookupTableFourPowI[FOUR_POWER_D];
  int LookupTableTwoPowI[FOUR_POWER_D];
  int LookupTableDPowI[FOUR_POWER_D];
}

int dPowI(int i) {
  assertion( i>=0 );
  assertion( i<FOUR_POWER_D );
  assertionEquals( tarch::la::aPowI(i,DIMENSIONS), LookupTableDPowI[i] );
  return LookupTableDPowI[i];
}

int threePowI(int i) {
  assertion1( i>=0, i );
  assertion2( i<FOUR_POWER_D, i, FOUR_POWER_D );
  assertionEquals( tarch::la::aPowI(i,3), LookupTableThreePowI[i] );
  return LookupTableThreePowI[i];
}

int fourPowI(int i) {
  assertion( i>=0 );
  assertion( i<FOUR_POWER_D );
  assertionEquals( tarch::la::aPowI(i,4), LookupTableFourPowI[i] );
  return LookupTableFourPowI[i];
}

int twoPowI(int i) {
  assertion( i>=0 );
  assertion( i<FOUR_POWER_D );
  assertionEquals4( tarch::la::aPowI(i,2), LookupTableTwoPowI[i], i , LookupTableTwoPowI[0], LookupTableTwoPowI[1], LookupTableTwoPowI[2]);
  return LookupTableTwoPowI[i];
}

void setupLookupTableForDPowI() {
  for (int i=0; i<FOUR_POWER_D; i++) {
    LookupTableDPowI[i] = tarch::la::aPowI(i,DIMENSIONS);
  }
}

void setupLookupTableForThreePowI() {
  for (int i=0; i<FOUR_POWER_D; i++) {
    LookupTableThreePowI[i] = tarch::la::aPowI(i,3);
  }
}

void setupLookupTableForFourPowI() {
  for (int i=0; i<FOUR_POWER_D; i++) {
    LookupTableFourPowI[i] = tarch::la::aPowI(i,4);
  }
}

void setupLookupTableForTwoPowI() {
  for (int i=0; i<FOUR_POWER_D; i++) {
    LookupTableTwoPowI[i] = tarch::la::aPowI(i,2);
  }
}

