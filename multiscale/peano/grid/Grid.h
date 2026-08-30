// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_GRID_H_
#define _PEANO_GRID_GRID_H_


#include "peano/utils/Globals.h"
#include "peano/geometry/Geometry.h"


#include "peano/grid/nodes/Leaf.h"
#include "peano/grid/nodes/Refined.h"
#include "peano/grid/nodes/RegularRefined.h"
#include "peano/grid/nodes/Root.h"

#include "peano/grid/RegularGridContainer.h"
#include "peano/grid/TraversalOrderOnTopLevel.h"


namespace peano {
    namespace grid {
      template <
        class Vertex,
        class Cell,
        class State,
        class VertexStack,
        class CellStack,
        class EventHandle
      >
      class Grid;
    }
}


/**
 * The root element manages the @f$ 2^d @f$ vertices of the super element that
 * encloses the unit square (if the program is configured to have the domain
 * contained within the unit square). Therefore, the event next to the root
 * element always is a refined element.
 *
 * Furthermore, the root event holds the state corresponding to the whole grid
 * and is responsible for inverting the state.
 *
 * @image html peano/grid/grid_RootElement.gif
 *
 * @author Tobias Weinzierl
 * @version $Revision: 1.55 $
 */
template <class Vertex, class Cell, class State, class VertexStack, class CellStack, class EventHandle>
class peano::grid::Grid {
  public:
    typedef peano::grid::RegularGridContainer<Vertex,Cell>   RegularGridContainer;
  private:
    typedef peano::grid::nodes::Leaf<Vertex,Cell,State,VertexStack,CellStack,EventHandle>            LeafNode;
    typedef peano::grid::nodes::Root<Vertex,Cell,State,VertexStack,CellStack,EventHandle>            RootNode;
    typedef peano::grid::nodes::Refined<Vertex,Cell,State,VertexStack,CellStack,EventHandle>         RefinedNode;
    typedef peano::grid::nodes::RegularRefined<Vertex,Cell,State,VertexStack,CellStack,EventHandle>  RegularRefinedNode;

    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    /**
     * Holds the vertex stack associated to this event.
     */
    VertexStack& _vertexStack;

    /**
     * Holds the cell stack associated to this event. This is important for
     * all events events - even leaves need it as they might refine further.
     */
    CellStack&   _cellStack;

    State&  _state;

    /**
     * Holds the event handle associated to this event.
     */
    EventHandle   _eventHandle;

    /**
     * The grid container for the regular patches is a pure temporary
     * container, i.e. it is only used to swap data temporarily to another
     * data structure. At the same time, it might become really large.
     * Therefore, all the grids should share one implementation, and this
     * is the reason why I made this container a class attribute.
     */
    RegularGridContainer&                   _regularGridContainer;
    peano::grid::TraversalOrderOnTopLevel&  _traversalOrderOnTopLevel;

    /**
     * Nodes of the spacetree (flyweight pattern)
     */
    LeafNode            _leafNode;
    RefinedNode         _refinedNode;
    RegularRefinedNode  _regularRefinedNode;
    RootNode            _root;

  public:
    /**
     * Create new grid
     *
     * Construct a new grid with the given parameters. You have to init
     * the geometry and the stacks before you call any further operations on
     * the grid.
     */
    Grid(
      VertexStack&                                  vertexStack,
      CellStack&                                    cellStack,
      peano::geometry::Geometry&                    geometry,
      State&                                        state,
      const tarch::la::Vector<DIMENSIONS,double>&   domainSize,
      const tarch::la::Vector<DIMENSIONS,double>&   domainOffset,
      RegularGridContainer&                         regularGridContainer,
      TraversalOrderOnTopLevel&                     traversalOrderOnTopLevel
    );

    /**
     * If you use this constructor, you have to initialise the traversal due to
     * traversalOrderOnTopLevel yourself.
     */
    Grid(
      VertexStack&                                  vertexStack,
      CellStack&                                    cellStack,
      peano::geometry::Geometry&                    geometry,
      State&                                        state,
      RegularGridContainer&                         regularGridContainer,
      TraversalOrderOnTopLevel&                     traversalOrderOnTopLevel
    );

    /**
     * Works only on MPI ranks that are not equal to the master node.
     */
    void restart(
      const tarch::la::Vector<DIMENSIONS,double>&  domainSize,
      const tarch::la::Vector<DIMENSIONS,double>&  domainOffset,
      int                                          domainLevel,
      tarch::la::Vector<DIMENSIONS,int>            positionOfCentralElementWithRespectToCoarserRemoteLevel
    );

    /**
     * Run over the grid k times
     *
     * !!! Implementation
     *
     * It is important to release the join data before we release the boundary
     * messages. Otherwise, we create a deadlock: Throughout a fork, the master
     * streams data to its worker. At the same time, it sends parallel boundary
     * data to the new worker. The worker recevies the streamed data, and in
     * turn sends out boundary information to its master (if they share a
     * common boundary). Now, the release operation on the master waits until
     * the worker has sent all its boundary data. At the same time, the worker
     * cannot complete its work, as the master still might buffer some stream
     * data. Therefore, it is important that the master releases the stream
     * data to enable the worker to finish and to send out all the boundary
     * information.
     */
    void iterate();

    /**
     * nop
     */
    void terminate();
};


#endif
