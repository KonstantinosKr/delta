// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_SINGLE_ELEMENT_VERTEX_ENUMERATOR_H_
#define _PEANO_GRID_SINGLE_ELEMENT_VERTEX_ENUMERATOR_H_


#include "peano/grid/VertexEnumerator.h"


namespace peano {
  namespace grid {
    class SingleElementVertexEnumerator;
  }
}


/**
 * Enumerator for a single element.
 *
 * Here the vertices are stored in a @f$ 2^d @f$ array and this enumerator is a
 * simple linearisation of this array along Peano's enumeration convention.
 *
 * @image html peano/grid/SingleElementVertexEnumerator.png
 *
 * @author Tobias Weinzierl
 */
class peano::grid::SingleElementVertexEnumerator: public peano::grid::VertexEnumerator {
  private:
    Vector                   _cellSize;
    Vector                   _cellOffset;
    int                      _level;
  public:
    /**
     * Constructor
     *
     * Set all values directly. They are just copied.
     */
    SingleElementVertexEnumerator(
      const Vector &  cellSize,
      const Vector &  cellOffset,
      int             level = 0
    );

    virtual ~SingleElementVertexEnumerator();

    void setCellOffset( const Vector &  cellOffset );

    virtual int operator() (int localVertexNumber) const;
    virtual int operator() (const LocalVertexIntegerIndex&  localVertexNumber ) const;
    virtual int operator() (const LocalVertexBitsetIndex&   localVertexNumber ) const;

    static int lineariseVertexIndex( int localVertexNumber );
    static int lineariseVertexIndex( const LocalVertexIntegerIndex&  vertexPosition );
    static int lineariseVertexIndex( const LocalVertexBitsetIndex&   localVertexNumber );

    virtual Vector getVertexPosition(int localVertexNumber ) const;
    virtual Vector getVertexPosition(const LocalVertexIntegerIndex& localVertexNumber ) const;
    virtual Vector getVertexPosition(const LocalVertexBitsetIndex& localVertexNumber ) const;
    virtual Vector getVertexPosition() const;

    virtual Vector getCellCenter() const;
    virtual Vector getCellSize() const;
    virtual std::string toString() const;

    virtual int getLevel() const;

    virtual peano::grid::CellFlags getCellFlags() const;

    virtual int cell(const LocalVertexIntegerIndex& localVertexNumber) const;

    virtual bool overlaps(const Vector& offset, const Vector& size) const;

    virtual bool isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const;
};


#endif
