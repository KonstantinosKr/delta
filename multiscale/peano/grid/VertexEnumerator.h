// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_VERTEX_ENUMERATOR_H_
#define _PEANO_GRID_VERTEX_ENUMERATOR_H_


#include "tarch/la/Vector.h"
#include "peano/utils/Globals.h"
#include "peano/grid/CellFlags.h"
#include <bitset>


namespace peano {
  namespace grid {
    class VertexEnumerator;
  }
}



/**
 * Enumerator Interface.
 *
 * Abstract Enumerator used to access the vertices passed the enterElement()
 * operations. Whenever you get access to vertices, Peano passes you an array
 * of vertices. However, you neither know the size nor the structure of the
 * array. The only thing you can do is to use the co-passed vertex enumerator
 * which gives you access to the @f$ 2^d @f$ vertices of one cell. These
 * vertices are enumerated lexicographically with the vertex 0 being the one
 * nearest to the coordinate system's origin. Then, the enumeration first runs
 * along the x_1 axis, then along x_2, and so forth.
 *
 * The enumerator is a functor, i.e. you can use it like a function. In terms
 * of a function, it is also overloaded. You can pass it an arbitrary integer
 * (encoding the vertices along the enumeration above), but you can also access
 * the vertices of a cell with an integer or bit vector.
 *
 * Example for usage:
 * \code
tarch::la::Vector<TWO_POWER_D,double> result;
dfor2(x)
  result(xScalar) = vertices[ enumerator(x) ]._vertexData.getU();
enddforx return result;
\endcode
 *
 * This simple sample runs over all @f$ 2^d @f$  vertices of one cell and
 * stores values of these vertices in a vector. In real life, I strongly
 * recommend not to do it this way, as the vertex should provide an
 * automatically generated operation that does exactly what the code from
 * above does, too.
 *
 * Besides the functor operations invoked with the normal brackets, there's
 * also a couple of additional functions that give you the position of the
 * vertices in space, e.g. See the interface signature for more information.
 *
 * !!! Lexicographic enumeration
 *
 * The lexicographic enumeration means that we start for each cell with the
 * vertex nearest to the coordinate system's origin. In 2D, this is the bottom
 * left vertex. Then, we enumerate ascending along the x_1, x_2, x_3 and so
 * forth. The picture bellow illustrates this enumeration for the
 * three-dimensional case.
 *
 * @image html peano/grid/lexicographic-enumeration.png
 *
 * @author Tobias Weinzierl
 */
class peano::grid::VertexEnumerator {
  public:
    typedef tarch::la::Vector<DIMENSIONS,int>     LocalVertexIntegerIndex;
    typedef std::bitset<DIMENSIONS>               LocalVertexBitsetIndex;
    typedef tarch::la::Vector<DIMENSIONS,double>  Vector;

    // An interesting behaviour in Peano showed up. The initial observation
    // was that Peano, compiled with OpenMP, crashed in the current version
    // right after startup with the message
    //
    //   pure virtual method called
    //   terminate called without an active exception
    //
    // It showed up that the problem was induced by the virtual destructor
    // below, which was inserted to avoid the warning
    // "'class peano::kernel::gridinterface::VertexEnumerator' has virtual
    // functions but non-virtual destructor" that is thrown by gcc on the
    // Shaheen.
    // There are two ways to solve this problem. One is to keep the virtual
    // destructor and to fix the OpenMP-Problem somehow. The other one is to
    // remove the virtual destructor and to disable the appropriate warning.
    // A virtual destructor is only needed if an abstract class is deleted
    // somewhere. Since this is unlikely in this case I commented the virtual
    // destructor to cope with what seems like a bug in OpenMP. To disable
    // the warning the flag -Wno-non-virtual-dtor was added to the scons-script.

//    virtual ~VertexEnumerator(){}

    /**
     * Map local vertex number to global array.
     *
     * Take a vertex number (something in-between @f$ 0 @f$ and @f$ 2^d-1 @f$ to
     * an index in a global array which is passed to enterElement().
     */
    virtual int operator() (int localVertexNumber) const = 0;

    /**
     * Map a local vertex number (given as vector on @f$ (0,1)^d @f$) to a
     * global array.
     */
    virtual int operator() (const LocalVertexIntegerIndex& localVertexNumber ) const = 0;

    /**
     * Map a local vertex number (given as vector on @f$ (0,1)^d @f$) to a
     * global array.
     */
    virtual int operator() (const LocalVertexBitsetIndex& localVertexNumber ) const = 0;

    /**
     * Returns position of vertex
     *
     * The operation analyses vertex localVertexNumber given in the cell-wise
     * enumeration. It then returns a vector with the position of this vertex.
     *
     * <h2> Usage </h2>
     *
     * Here is a 2d illustration of the method behaviour. Assume we study a
     * rectangle with a left bottom vertex at (-2,-1) and the dimensions 4x2.
     * Most codes in Peano use cubical mesh elements, but you can squeeze the
     * mesh and the chosen example makes all used coordinates unique. The
     * rectangle has four vertices that are enumerated along the vertex axis.
     *
     * - Vertex 0 is the bottom left vertex.
     * - We continue along the x axis, so vertex 1 is the bottom right vertex.
     * - Vertex 2 is the left top vertex as we do the y axis next, i.e. our
     *   enumeration is some kind of z-curve.
     * - Vertex 3 finally is the top right vertex.
     *
     * With the argument localVertexNumber, you choose the vertex. The result
     * is a vector were you again might want to select entries with the ()
     * operator:
     *
     * - myEnumerator.getVertexPosition(0)(0) consequently returns the vertex
     *   0's x coordinate, i.e. -2.
     * - myEnumerator.getVertexPosition(0)(1) yields its y-coordinate -1.
     * - myEnumerator.getVertexPosition(2)(0) returns -2, too, while
     * - myEnumerator.getVertexPosition(2)(1) returns 1.
     * - myEnumerator.getVertexPosition(3) finally returns a tuple (2,1)
     *   from which you can select the second entry (the y coordinate) with
     *   myEnumerator.getVertexPosition(3)(1).
     *
     *
     *
     * <h2> Pitfall </h2>
     *
     * There is also a getX() operation for each vertex that returns you the
     * position of this vertex. However, getX() is defined in the Debug mode
     * only. If you switch off Debug, Peano saves these d doubles per vertex,
     * as a vertex's position always can be reconstructed by the traversal
     * automaton - due to getVertexPosition()!
     *
     * @param localVertexNumber Local vertex number within the cell.
     */
    virtual Vector getVertexPosition(int localVertexNumber) const = 0;

    /**
     * @see getVertexPosition(int) This is just an overloaded version.
     *
     * @param localVertexNumber  Integer vector describing the discretised
     *                           position of the vertex within the cell as
     *                           d-tuple over @f$ \{0,1\} @f$
     */
    virtual Vector getVertexPosition(const LocalVertexIntegerIndex& localVertexNumber) const = 0;

    /**
     * @see getVertexPosition(int) This is just an overloaded version.
     */
    virtual Vector getVertexPosition(const LocalVertexBitsetIndex& localVertexNumber) const = 0;

    /**
     * This is a shortcut for getVertexPosition(0) and should be optimised.
     */
    virtual Vector getVertexPosition() const = 0;

    virtual Vector getCellCenter() const = 0;

    virtual int getLevel() const = 0;

    /**
     * Returns the size of the current cell.
     */
    virtual Vector      getCellSize() const = 0;
    virtual std::string toString() const = 0;

    /**
     * An enumerator represents a certain part of the overall domain. This flag
     * tells you whether the covered domain is stationary or not.
     */
    virtual peano::grid::CellFlags getCellFlags() const = 0;

    /**
     * Access cell within an array
     *
     * Most mapping operations are bassed individual cells via call-by-reference
     * or call-by-value. In this case, you can directly access the cells. There
     * are few operations however that hand over an array of cells rather than
     * individual cells. In this case, the access mechanism due to the enumerators
     * is exactly the same as for the vertices.
     *
     * You should not run through the array elements individual with a counter.
     * Instead, access the individual entries via this operation, i.e. type in
     *
     * \code
       myCellArray[ myCellEnumerator.cell(myCellIndex) ].doSomething();
       \endcode
     *
     * Again, the vertex enumerator internally knows in which order the cells are
     * enumerated, and it maps a lexicographically cell order onto the array
     * indices.
     */
    virtual int cell(const LocalVertexIntegerIndex& localVertexNumber) const = 0;

    /**
     * A cell enumerator does represent a cell. This operation evaluates
     * whether this cell overlaps with a voxel identified by offset with a
     * given size.
     */
    virtual bool overlaps(const Vector& offset, const Vector& size) const = 0;

    /**
     * Each enumerator corresponds to a patch. This one might be trivial, i.e.
     * have exactly one cell, it might be the standard @f$ 3^d @f$ patch, or it
     * might be bigger due to recursion unrolling. Anyway, this operation tells
     * you whether the vertex at localVertexNumber is at the patch boundary.
     *
     * If you are interested in more intergrid index manipulations, please study
     * the SingleLevelEnumerator. It comes along with a couple of additional static
     * multigrid/multiscale operations.
     */
    virtual bool isVertexAtPatchBoundaryWithinRegularSubtree(const LocalVertexIntegerIndex& localVertexNumber) const = 0;


  protected:
    /**
     * Comparison operators up to relative tolerance.
     */
    static bool greaterEqualsUpToRelativeTolerance(const double& lhs, const double& rhs) {
      const double scaling =
          std::max(
              1.0, std::max( tarch::la::abs(lhs), tarch::la::abs(rhs) )
      );
      return tarch::la::greaterEquals( lhs, rhs, scaling*tarch::la::NUMERICAL_ZERO_DIFFERENCE );
    }

    static bool smallerEqualsUpToRelativeTolerance(const double& lhs, const double& rhs) {
      const double scaling =
          std::max(
              1.0, std::max( tarch::la::abs(lhs), tarch::la::abs(rhs) )
      );
      return tarch::la::smallerEquals( lhs, rhs, scaling*tarch::la::NUMERICAL_ZERO_DIFFERENCE );
    }
};


#endif
