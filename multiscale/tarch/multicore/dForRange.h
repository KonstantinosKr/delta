// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_MULTICORE_DFOR_RANGE_H_
#define _TARCH_MULTICORE_DFOR_RANGE_H_


#include "tarch/la/Vector.h"
#include "tarch/logging/Log.h"


#include <vector>


namespace tarch {
  namespace multicore {
    template <int D>
    class dForRange;
  }
}



/**
 * Range implementation for the dFor.
 *
 * A range is a d-dimensional hypercube together with a translation. It
 * identifies an index set. Consequently, it is a generalisation of TBB's
 * blocked_range2d.
 *
 * The range basically implements an kd-tree-like approach: Whenever a split
 * is invoked (due to the constructor), it analyses along which coordinate
 * axis that cube is the longest. Then, it cuts the cube along this axis into
 * two parts of (besides rounding) the same size. A cube basically can be
 * split as long as its volume is greater than zero, i.e. as long as it
 * contains more than one vertex along one coordinate axis.
 * However, the standard behaviour of the range also takes into account the
 * minimum grain size. Therefore a cube is not divided down to only one
 * vertex, but the division process may be stopped with a number of vertices
 * equal or larger than the given grain size. This is done due to
 * performance issues in multicore parallelization. An example of the
 * division process is illustrated below on the left-hand side.
 *
 * @image html peano/datatraversal/dForRange.png
 *
 * You find more information on the original blocked_range2d at
 *
 * @see http://www.threadingbuildingblocks.org/uploads/81/91/Latest%20Open%20Source%20Documentation/Reference%20Manual%20(Open%20Source).pdf
 *
 * @author Wolfgang Eckhardt
 * @author Tobias Weinzierl
 */
template <int D>
class tarch::multicore::dForRange {
  private:
    static tarch::logging::Log _log;

    tarch::la::Vector<D,int>  _offset;
    tarch::la::Vector<D,int>  _range;
    const int                 _grainSize;
    const int                 _interleaving;

  public:
    /**
     * I internally create vectors of ranges and to initialise those guys
     * properly, I need a default constructor. Does not make sense to use
     * otherwise
     */
    dForRange();

    /**
     * Copy constructor
     */
    dForRange(const dForRange<D>& range);

    /**
     * Construct a Complete Range
     */
    dForRange( const tarch::la::Vector<D,int>&  offset, const tarch::la::Vector<D,int>&  range, int grainSize, int interleaving );

    /**
     * Split operator
     *
     * @param range Original range
     */
    dForRange split();

    /**
     * Is given an entry from getRange() and translates it into an index, i.e.
     * it adds the offset and takes the interleaving into account.
     */
    tarch::la::Vector<D,int> operator()(const tarch::la::Vector<D,int>& range) const;

    /**
     * Is the range empty?
     *
     * A range is empty, if the corresponding volume of the hyperhexahedron
     * equals zero. If it is empty, it is also not divisible.
     */
    bool empty() const;

    /**
     * A range is divisible, if the volume of the corresponding range
     * hyperhexahedron is bigger than the grain size.
     */
    bool isDivisible() const;

    tarch::la::Vector<D,int> getOffset() const;
    tarch::la::Vector<D,int> getRange() const;

    /**
     * Tuple representation of attributes.
     */
    std::string toString() const;

    /**
     * Takes the minimum grain size and splits up the current range into a set
     * of ranges that all are as small as possible without harming the
     * grain size (cmp to isDivisible()). The result is a vector, as a vector
     * is essentially an array and thus can easily be traversed in parallel.
     */
    std::vector< dForRange<D> >  getMinimalRanges() const;

    /**
     * Simple concatenation of dForRange toString() outputs with surrounding
     * brackets. Often used to dump the output of getMinimalRanges().
     */
    static std::string toString( const std::vector< dForRange<D> >& ranges );
};


#include "tarch/multicore/dForRange.cpph"


#endif
