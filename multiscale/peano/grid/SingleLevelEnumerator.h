// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_SINGLE_LEVEL_ENUMERATOR_H_
#define _PEANO_GRID_SINGLE_LEVEL_ENUMERATOR_H_


#include "peano/grid/VertexEnumerator.h"

#include "tarch/logging/Log.h"


namespace peano {
  namespace grid {
      class SingleLevelEnumerator;

      /**
       * Forward declaration for friend
       */
      class AscendDescendLevelEnumerator;

      namespace tests {
        /**
         * Forward declaration for friend
         */
        class SingleLevelEnumeratorTest;
      }
  }
}


/**
 * The single level cell enumerator is an enumerator for one @f$ 3^d @f$ patch.
 *
 * @author Tobias Weinzierl
 */
class peano::grid::SingleLevelEnumerator: public peano::grid::VertexEnumerator {
  private:
    /**
     * Forward declaration for friend
     */
    friend class peano::grid::tests::SingleLevelEnumeratorTest;
    friend class AscendDescendLevelEnumerator;

    LocalVertexIntegerIndex  _discreteOffset;
    Vector                   _fineGridCellSize;
    Vector                   _domainOffset;

    int    _level;

    /**
     * This flag mirrors the data structure in Vertex with the same name.
     * It is the minimum of all the adjacent vertex flags and consequently
     * important to answer to isStationarySubdomain() and
     * subdomainContainsParallelBoundary().
     */
    peano::grid::CellFlags  _adjacentCellsHeight;

    int getFlattenedIndex(const LocalVertexIntegerIndex&  position) const;

    SingleLevelEnumerator(
      const Vector&                coarseGridCellSize,
      const Vector&                domainOffset,
      int                          level,
      peano::grid::CellFlags  adjacentCellsHeight
    );
  public:
    /**
     * Construct Enumerator
     *
     * Construct the enumerator at hands of the data of the coarser enumerator,
     * i.e. the resulting enumerator will have only one third of the specified
     * cell size and one level bigger.
     *
     * Typically, you hand in the coarse node's x and h. It did not provide a
     * constructor taking a state directly as this would introduce an
     * additional dependence between two types.
     *
     * Both enumerators set the state of the adjacent cells to not-stationary.
     */
    SingleLevelEnumerator(
      const Vector&                coarseGridCellSize,
      const Vector&                domainOffset,
      int                          coarseGridLevel
    );

    SingleLevelEnumerator( const SingleLevelEnumerator& copy );

    virtual ~SingleLevelEnumerator();

    /**
     * Create refined enumerator
     *
     * For the time being, we inherit
     *
     * @return Refined enumerator
     */
    SingleLevelEnumerator getRefinedEnumerator() const;

    void setOffset(const LocalVertexIntegerIndex&  gridPointOffset);

    /**
     * Update adjacent cells flag
     *
     * The adjacent cells flag gives the enumerator information on the adjacent
     * trees, i.e. whether they are regular, whether they are instationary, or
     * whether they contain a parallel boundary. This flag is set due to
     * updateRefinedEnumeratorsCellFlag() in the Refined type for each
     * subenumerator, as the global root enumerator hold a adjacent cells flag
     * that is set to invalid.
     *
     * Please note that the flag not only reflects the fact whether adjacent
     * cells are stationary and regular. It also reflects whether a node shall
     * fork or not. If it shall fork, we have to switch off any optimisation.
     */
    void updateAdjacentCellsFlag( peano::grid::CellFlags newFlag );

    virtual int operator() (int localVertexNumber) const;
    virtual int operator() (const LocalVertexIntegerIndex&  localVertexNumber ) const;
    virtual int operator() (const LocalVertexBitsetIndex&   localVertexNumber ) const;

    virtual Vector getVertexPosition(int localVertexNumber) const;
    virtual Vector getVertexPosition(const LocalVertexIntegerIndex& localVertexNumber ) const;
    virtual Vector getVertexPosition(const LocalVertexBitsetIndex& localVertexNumber ) const;
    virtual Vector getVertexPosition() const;

    virtual Vector getCellSize() const;
    virtual Vector getCellCenter() const;
    virtual std::string toString() const;

    static int lineariseCellIndex( const LocalVertexIntegerIndex&    cellPosition );
    static int lineariseVertexIndex( const LocalVertexIntegerIndex&  vertexPosition );

    virtual int getLevel() const;

    /**
     * Get position of vertex witin cell on next coarser level
     *
     * Take a vertex within the current cell (localVertexNumber is from (0,1)^d), and return
     * its position within the whole patch, i.e. as (0,1,2,3)^d vector.
     */
    LocalVertexIntegerIndex getVertexPositionWithinCoarserPatch(const LocalVertexIntegerIndex& localVertexNumber ) const;

    /**
     * Get position of vertex witin 3^d patch on next coarser level
     *
     * Gives you the position of a vertex within the next coarser level. This
     * operation may only be called if isVertexPositionAtCoarseVertexsPosition()
     * does hold.
     *
     * @return Coarse vertex's index (vector of zeros and ones).
     * @see    getVertexPositionWithinCoarserPatch()
     */
    static LocalVertexIntegerIndex getVertexPositionOnCoarserLevel(const LocalVertexIntegerIndex& index );

    /**
     * Return parent vertex
     *
     * Takes the global enumeration of the vertex within the 3^d patch and
     * returns the parent vertex. In 2d, here are some examples: You pass (0,0)
     * and you get (0,0), you pass (1,1) and you get (0,0) as well. You pass (2,0)
     * and you should get (1,0), you pass (1,3) and you get (0,1).
     */
    static LocalVertexIntegerIndex getParentVertex(const LocalVertexIntegerIndex& index );

    /**
     * @return Tells you whether a vertex coincides with a vertex on the next
     * coarser level
     */
    static bool isVertexPositionAlsoACoarseVertexPosition(const LocalVertexIntegerIndex& index );

    /**
     * Returns true if any entry in the index equals 0 or 3, i.e. when the
     * vertex position coincides with coarse grid grid lines.
     */
    static bool isVertexPositionAlsoACoarseGridPosition(const LocalVertexIntegerIndex& index );

    /**
     * A coarse grid index is a d-tuple with zeros and ones. The entries of the
     * fineGridIndex d-tuple are values between zero and three (inclusive). The
     * semantics equal the local prolongation operator in a multigrid setting.
     */
    static bool doesCoarseGridVertexInfluenceFineGridVertex(
      const LocalVertexIntegerIndex&  positionInCoarseGridCell,
      const LocalVertexIntegerIndex&  positionInFineGridCell
    );

    virtual peano::grid::CellFlags getCellFlags() const;

    #ifdef Asserts
    /**
     * Only for debugging/validity checks.
     */
    LocalVertexIntegerIndex getOffset() const;
    #endif

    virtual int cell(const LocalVertexIntegerIndex& localVertexNumber) const;

    virtual bool overlaps(const Vector& offset, const Vector& size) const;

    virtual bool isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const;
};


#endif
