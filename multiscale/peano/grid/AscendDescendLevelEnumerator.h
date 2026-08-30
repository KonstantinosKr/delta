// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_ASCEND_DESCEND_LEVEL_ENUMERATOR_H_
#define _PEANO_GRID_ASCEND_DESCEND_LEVEL_ENUMERATOR_H_


#include "peano/grid/VertexEnumerator.h"

#include "tarch/logging/Log.h"


namespace peano {
  namespace grid {
    class AscendDescendLevelEnumerator;
    class SingleLevelEnumerator;
  }
}


/**
 * Ascend/Descend Enumerator
 *
 * @author Tobias Weinzierl
 */
class peano::grid::AscendDescendLevelEnumerator: public peano::grid::VertexEnumerator {
  private:
    static tarch::logging::Log  _log;
    Vector                      _fineGridCellSize;
    Vector                      _domainOffset;

    int    _level;

    /**
     * This flag mirrors the data structure in Vertex with the same name.
     * It is the minimum of all the adjacent vertex flags and consequently
     * important to answer to isStationarySubdomain() and
     * subdomainContainsParallelBoundary().
     */
    peano::grid::CellFlags  _adjacentCellsHeight;

    int getFlattenedIndex(const LocalVertexIntegerIndex&  position) const;
  public:
    AscendDescendLevelEnumerator( const SingleLevelEnumerator&  cloneFineGridEnumerator );

    virtual ~AscendDescendLevelEnumerator();

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

    int cell(const LocalVertexIntegerIndex& localVertexNumber) const;

    /**
     * Refers to the coarse grid
     */
    virtual bool overlaps(const Vector& offset, const Vector& size) const;

    virtual bool isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const;
};


#endif
