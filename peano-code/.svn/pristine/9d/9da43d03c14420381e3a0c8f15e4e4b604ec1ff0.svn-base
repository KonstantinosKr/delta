// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_UNROLLED_ASCEND_DESCEND_LEVEL_ENUMERATOR_H_
#define _PEANO_GRID_UNROLLED_ASCEND_DESCEND_LEVEL_ENUMERATOR_H_


#include "peano/grid/UnrolledLevelEnumerator.h"

#include "tarch/logging/Log.h"


namespace peano {
  namespace grid {
    class UnrolledAscendDescendLevelEnumerator;
  }
}


/**
 * The single level cell enumerator is an enumerator for one @f$ 3^d @f$ patch.
 *
 * @author Tobias Weinzierl
 */
class peano::grid::UnrolledAscendDescendLevelEnumerator: public peano::grid::VertexEnumerator {
  private:
    static tarch::logging::Log  _log;

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
    peano::grid::CellFlags   _adjacentCellsHeight;

    int _CellsPerAxis;
    int _VerticesPerAxis;

    int getFlattenedIndex(const LocalVertexIntegerIndex&  position) const;

  public:
    /**
     * Construct Enumerator
     */
    UnrolledAscendDescendLevelEnumerator( const peano::grid::UnrolledLevelEnumerator& enumerator );

    virtual ~UnrolledAscendDescendLevelEnumerator();

    void setOffset(const LocalVertexIntegerIndex&  gridPointOffset);

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

    virtual int getLevel() const;

    virtual peano::grid::CellFlags getCellFlags() const;

    int getCellsPerAxis() const;
    int getVerticesPerAxis() const;

    virtual int cell(const LocalVertexIntegerIndex& localVertexNumber) const;


    /**
     * Forward to UnrolledLevelEnumerator.
     */
    int lineariseCellIndex( const LocalVertexIntegerIndex&    cellPosition ) const;

    /**
     * Forward to UnrolledLevelEnumerator.
     */
    int lineariseVertexIndex( const LocalVertexIntegerIndex&  vertexPosition ) const;

    virtual bool overlaps(const Vector& offset, const Vector& size) const;

    virtual bool isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const;
};


#endif
