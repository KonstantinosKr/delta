#include "peano/parallel/LevelTransferOperators.h"
#include "peano/utils/Loop.h"

#include "tarch/parallel/Node.h"

#include <bitset>

namespace peano {
    namespace parallel {
      namespace {
        tarch::la::Vector<TWO_POWER_D,int> getRefinedSublevelList(
          const tarch::la::Vector<TWO_POWER_D,int>&  smallerFlag,
          const tarch::la::Vector<TWO_POWER_D,int>&  biggerFlag,
          int axis
        ) {
          assertion( axis>=0 );
          assertion( axis<DIMENSIONS );

          tarch::la::Vector<TWO_POWER_D,int> result ;

          dfore(i,2,axis,0) {
            tarch::la::Vector<DIMENSIONS,int> smallerIndex;
            tarch::la::Vector<DIMENSIONS,int> biggerIndex;
            smallerIndex = i;
            biggerIndex  = i;
            biggerIndex(axis) += 1;

            int valueOfSmallerFlag = smallerFlag( peano::utils::dLinearised(biggerIndex,2)  );
            int valueOfBiggerFlag  = biggerFlag( peano::utils::dLinearised(smallerIndex,2) );

            if (valueOfSmallerFlag!=valueOfBiggerFlag) {
              // instead of -1 one should write Vertex::UNDEF_NEIGHBOUR, but I don't
              // want this operation to become a template because of an assertion!
              assertion5(
                (valueOfSmallerFlag==-1 || valueOfBiggerFlag==-1) && !(valueOfSmallerFlag==-1 && valueOfBiggerFlag==-1),
                smallerFlag,
                biggerFlag,
                axis,
                i,
                tarch::parallel::Node::getInstance().getRank()
              );

              // instead of <0 one should compare the value to Vertex::UNDEF_NEIGHBOUR,
              // but I don't want this operation to become a template.
              if (valueOfSmallerFlag<0) {
                result( peano::utils::dLinearised(smallerIndex,2) ) = valueOfBiggerFlag;
                result( peano::utils::dLinearised(biggerIndex,2) )  = valueOfBiggerFlag;
              }
              else {
                result( peano::utils::dLinearised(smallerIndex,2) ) = valueOfSmallerFlag;
                result( peano::utils::dLinearised(biggerIndex,2) )  = valueOfSmallerFlag;
              }
            }
            else {
              result( peano::utils::dLinearised(smallerIndex,2) ) = valueOfSmallerFlag;
              result( peano::utils::dLinearised(biggerIndex,2) )  = valueOfSmallerFlag;
            }
          }

          return result;
        }

        tarch::la::Vector<TWO_POWER_D,int> getAdjacencyList(
          const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& coarseGridAdjacencyInformation,
          const std::bitset<DIMENSIONS>&                              isOnSubmanifoldNearOrigin,
          const std::bitset<DIMENSIONS>&                              isOnOppositeSubmanifold,
          int                                                         dimCounter
        ) {
          assertion( dimCounter >= -1 );
          assertion( dimCounter < DIMENSIONS );

          if (dimCounter==-1) {
            int index = 0;
            int base  = TWO_POWER_D;
            for (int d=0; d<DIMENSIONS; d++) {
              assertion3(isOnOppositeSubmanifold[d] || isOnSubmanifoldNearOrigin[d], coarseGridAdjacencyInformation, isOnSubmanifoldNearOrigin, isOnOppositeSubmanifold);
              int multiplyBy = isOnOppositeSubmanifold[d];
              index += multiplyBy * base;
              base *= 2;
            }
            tarch::la::Vector<TWO_POWER_D,int> result =
              tarch::la::slice<TWO_POWER_D>(coarseGridAdjacencyInformation,index);
            return result;
          }
          else {
            if ( isOnSubmanifoldNearOrigin[dimCounter] || isOnOppositeSubmanifold[dimCounter] ) {
              return getAdjacencyList( coarseGridAdjacencyInformation,isOnSubmanifoldNearOrigin,isOnOppositeSubmanifold,dimCounter-1);
            }
            else {
              std::bitset<DIMENSIONS>  isOnSubmanifoldNearOriginLeft(isOnSubmanifoldNearOrigin);
              std::bitset<DIMENSIONS>  isOnOppositeSubmanifoldRight(isOnOppositeSubmanifold);

              isOnSubmanifoldNearOriginLeft[dimCounter] = true;
              isOnOppositeSubmanifoldRight[dimCounter] = true;

              tarch::la::Vector<TWO_POWER_D,int> left   = getAdjacencyList( coarseGridAdjacencyInformation,isOnSubmanifoldNearOriginLeft,isOnOppositeSubmanifold,dimCounter-1);
              tarch::la::Vector<TWO_POWER_D,int> right  = getAdjacencyList( coarseGridAdjacencyInformation,isOnSubmanifoldNearOrigin,isOnOppositeSubmanifoldRight,dimCounter-1);

              return getRefinedSublevelList( left, right, dimCounter );
            }
          }

          assertion(false);

          return tarch::la::Vector<TWO_POWER_D,int>(-1);
        }
      }
    }
}


tarch::la::Vector<TWO_POWER_D,int> peano::parallel::getAdjacencyList(
  const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& coarseGridAdjacencyInformation,
  const tarch::la::Vector<DIMENSIONS,int>&                    fineGridNumberOfGridPoints,
  const tarch::la::Vector<DIMENSIONS,int>&                    fineGridPoint
) {
  std::bitset<DIMENSIONS> isOnSubmanifoldNearOrigin;
  std::bitset<DIMENSIONS> isOnOppositeSubmanifold;

  for (int d=0; d<DIMENSIONS; d++) {
    isOnSubmanifoldNearOrigin[d] = fineGridPoint(d)==0;
    isOnOppositeSubmanifold[d]   = fineGridPoint(d)==fineGridNumberOfGridPoints(d)-1;
  }

  return getAdjacencyList(coarseGridAdjacencyInformation,isOnSubmanifoldNearOrigin,isOnOppositeSubmanifold,DIMENSIONS-1);
}
