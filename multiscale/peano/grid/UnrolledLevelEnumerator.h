// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_UNROLLED_LEVEL_ENUMERATOR_H_
#define _PEANO_GRID_UNROLLED_LEVEL_ENUMERATOR_H_


#include "peano/grid/VertexEnumerator.h"

#include "tarch/logging/Log.h"


namespace peano {
  namespace grid {
    class UnrolledLevelEnumerator;

    /**
     * Forward declaration. Class is friend as its constructor requires access
     * to this enumerator.
     */
    class UnrolledAscendDescendLevelEnumerator;
  }
}


/**
 * The single level cell enumerator is an enumerator for one @f$ 3^d @f$ patch.
 *
 * @author Tobias Weinzierl
 */
class peano::grid::UnrolledLevelEnumerator: public peano::grid::VertexEnumerator {
  private:
    static tarch::logging::Log  _log;

    friend class UnrolledAscendDescendLevelEnumerator;

    LocalVertexIntegerIndex  _discreteOffset;
    Vector                   _fineGridCellSize;
    Vector                   _domainOffset;

    /**
     * Should be const. However, I copy the enumerator in several places, and
     * there it does not work anymore if the field is const. See
     * CallLeaveCellLoopBodyOnRegularRefinedPatch::setLevel(), e.g.
     */
    int   _level;

    /**
     * This flag mirrors the data structure in Vertex with the same name.
     * It is the minimum of all the adjacent vertex flags and consequently
     * important to answer to isStationarySubdomain() and
     * subdomainContainsParallelBoundary().
     */
    peano::grid::CellFlags   _adjacentCellsHeight;

    /**
     * Should be const. However, I copy the enumerator in several places, and
     * there it does not work anymore if the field is const. See
     * CallLeaveCellLoopBodyOnRegularRefinedPatch::setLevel(), e.g.
     */
    int                      _CellsPerAxis;

    /**
     * Should be const. However, I copy the enumerator in several places, and
     * there it does not work anymore if the field is const. See
     * CallLeaveCellLoopBodyOnRegularRefinedPatch::setLevel(), e.g.
     */
    int                      _VerticesPerAxis;

    int getFlattenedIndex(const LocalVertexIntegerIndex&  position) const;

  public:
     /**
     * Construct Enumerator
     *
     * Typically, you hand in the coarse node's x and h. It did not provide a
     * constructor taking a state directly as this would introduce an
     * additional dependence between two types.
     *
     * Both enumerators set the state of the adjacent cells to not-stationary.
     *
     * @param totalHeightOfSubtree This flag is used to do some assertions, and
     *            the operation derives the static tree flags using this
     *            property and relativeLevelOfThisEnumerator.
     */
    UnrolledLevelEnumerator(
      const Vector&                coarsestGridCellSize,
      const Vector&                domainOffset,
      int                          coarseGridEnumeratorsLevel,
      int                          totalHeightOfSubtree,
      int                          relativeLevelOfThisEnumerator
    );

    UnrolledLevelEnumerator(const UnrolledLevelEnumerator& enumerator);

    virtual ~UnrolledLevelEnumerator();

    void setOffset(const LocalVertexIntegerIndex&  gridPointOffset);
    LocalVertexIntegerIndex getOffset() const;

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

    /**
     * Does not take into account current offset
     */
    int lineariseCellIndex( const LocalVertexIntegerIndex&    cellPosition ) const;

    /**
     * Does not take into account current offset
     */
    int lineariseVertexIndex( const LocalVertexIntegerIndex&  vertexPosition ) const;

    virtual int getLevel() const;

    static LocalVertexIntegerIndex getVertexPositionOnCoarserLevel(const LocalVertexIntegerIndex& index );
    static bool isVertexPositionAtCoarseVertexsPosition(const LocalVertexIntegerIndex& index );

    virtual peano::grid::CellFlags getCellFlags() const;

    int getCellsPerAxis() const;
    int getVerticesPerAxis() const;

    virtual bool isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const;

    virtual int cell(const LocalVertexIntegerIndex& localVertexNumber) const;

    virtual bool overlaps(const Vector& offset, const Vector& size) const;
};


#endif
