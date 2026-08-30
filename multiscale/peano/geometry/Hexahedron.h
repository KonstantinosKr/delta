// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GEOMETRY_HEXAHEDRON_H_
#define _PEANO_GEOMETRY_HEXAHEDRON_H_

#include "peano/geometry/Geometry.h"
#include "tarch/logging/Log.h"

namespace peano {
  namespace geometry {
    class Hexahedron;
  }
}

/**
 *  A hexahedron.
 *
 * @author Tobias Weinzierl, Tobias Neckel, Philipp Neumann
 */
class peano::geometry::Hexahedron: public peano::geometry::Geometry {
  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    /**
     * Holds the offset of the hexahedron.
     */
    tarch::la::Vector<DIMENSIONS,double> _offset;

    /**
     * Width of the hexahedron.
     */
    tarch::la::Vector<DIMENSIONS,double> _width;

    /**
     * Comparison operators up to relative tolerance.
     */
    static bool greaterUpToRelativeTolerance(const double& lhs, const double& rhs);
    static bool smallerUpToRelativeTolerance(const double& lhs, const double& rhs);
    static bool greaterEqualsUpToRelativeTolerance(const double& lhs, const double& rhs);
    static bool smallerEqualsUpToRelativeTolerance(const double& lhs, const double& rhs);

    /**
     * This method checks if a point (tarch::la::Vector<DIMENSIONS,double>) x lies inside the open hexahedron.
     *
     * @param x Point (tarch::la::Vector<DIMENSIONS,double>) to be checked.
     *
     * \see geometry::Tube::isInsideOpenMinkowskiBox()
     */
    bool isInsideOpenHexahedron( const tarch::la::Vector<DIMENSIONS,double>& x ) const;

    /**
     * This method checks if a point (tarch::la::Vector<DIMENSIONS,double>) x lies inside the closed
     * hexahedron.
     *
     * @param x Point (tarch::la::Vector<DIMENSIONS,double>) to be checked.
     */
    bool isInsideClosedHexahedron( const tarch::la::Vector<DIMENSIONS,double>& x ) const;

  public:
    /**
     * Constructor setting basic attributes of the hexahedron.
     *
     * This constructor should be used for test cases only!
     *
     * The two parameters width and offset are not optional any
     * longer in order to avoid misunderstandings.
     *
     * @param width        tarch::la::Vector<DIMENSIONS,double> of hexahedron dimensions (x0, x1, x2 directions).
     * @param offset       tarch::la::Vector<DIMENSIONS,double> of offset of the hexahedron origin.
     */
    Hexahedron(
      const tarch::la::Vector<DIMENSIONS,double>&  width,
      const tarch::la::Vector<DIMENSIONS,double>&  offset
    );

    /**
     * Standard destructor.
     */
    virtual ~Hexahedron();

    tarch::la::Vector<DIMENSIONS,double> getOffset() const;
    tarch::la::Vector<DIMENSIONS,double> getBoundingBox() const;

    virtual bool domainHasChanged( const tarch::la::Vector<DIMENSIONS,double>& x, const tarch::la::Vector<DIMENSIONS,double> &resolution );

    virtual bool refineOuterCellWithExclusivelyOuterVerticesAsItIntersectsDomain( const tarch::la::Vector<DIMENSIONS,double>& x, const tarch::la::Vector<DIMENSIONS,double> &resolution ) const;

    /**
     * Routine for decision if the point and the surrounding box lies outside
     * the geometry.
     *
     * With value zero for the parameter resolution, this gives the exact
     * geometry information of a single point position x (outside yes or no).
     *
     * Please note that the computational domain splits up the whole
     * space @f$ \mathbf{R}^d @f$ into outside, inside and a set of points
     * (submanifold) that are neither inside nor outside. Thus, a pointwise
     * query (h equals the zero vector) returns one of three values mapped onto
     * the two functions isCompletelyInside() and isCompletelyOutside(). The
     * spatial query, where the query point x is added a surrounding box with
     * size 2*resolution where x is in the center is a little bit more complicated: The surrounding box
     * is interpreted to be an open domain and isCompletelyInside() /
     * isCompletelyOutside() returns true iff the intersection of the inner /
     * outer domain with the open surrounding box is the box itself.
     *
     * @image html peano/geometry/geometry_Geometry.png
     *
     * The query is not const anymore, as PreCICE might want to change the
     * underlying spacetree structure.
     *
     */
    bool isCompletelyOutside( const tarch::la::Vector<DIMENSIONS,double>& x, const tarch::la::Vector<DIMENSIONS,double> &resolution );

    /**
     * Routine for decision if the point and the surrounding box lies inside
     * the geometry.
     *
     * With value zero for the parameter resolution, this gives the exact
     * geometry information of a single point position x (outside yes or no).
     *
     * Please note that the computational domain splits up the whole
     * space @f$ \mathbf{R}^d @f$ into outside, inside and a set of points
     * (submanifold) that are neither inside nor outside. Thus, a pointwise
     * query (h equals the zero vector) returns one of three values mapped onto
     * the two functions isCompletelyInside() and isCompletelyOutside(). The
     * spatial query, where the query point x is added a surrounding box with
     * size 2*resolution where x is in the center is a little bit more complicated: The surrounding box
     * is interpreted to be an open domain and isCompletelyInside() /
     * isCompletelyOutside() returns true iff the intersection of the inner /
     * outer domain with the open surrounding box is the box itself.
     *
     * @image html peano/geometry/geometry_Geometry.png
     *
     * The query is not const anymore, as PreCICE might want to change the
     * underlying spacetree structure.
     *
     */
    bool isCompletelyInside( const tarch::la::Vector<DIMENSIONS,double>& x, const tarch::la::Vector<DIMENSIONS,double> &resolution );

    /**
     * A voxel is the intersection of 2*DIMENSIONS halfspaces. For each face two tests
     * have to be done. This tests tell the algorithm whether the hexahedron might be
     * inside the voxel. If one of this 2*DIMENSIONS tests returns false, the operation
     * has to return false. Here's the test for the right face that has a normal along
     * the @f$ x_0 @f$ normal: The voxel might intersect if: The voxel's right face has to
     * be right of the left hexahedron face and the right face is not allowed to be more
     * than a voxel's width right from the right hexahedron's face.
     */
    bool isOutsideClosedDomain( const tarch::la::Vector<DIMENSIONS,double>& x );
};

#endif
