// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_LEAF_H_
#define _PEANO_GRID_NODES_LEAF_H_


#include "peano/grid/nodes/Node.h"


/**
 * Root Node
 *
 * The root node basically is a refined node with an important difference
 * concerning:
 * Its @f$ 4^d@f$ vertices and its @f$ 3^d @f$ cells are not stored on any
 * stack
 *
 * @author Tobias Weinzierl
 */
template <
  class Vertex,
  class Cell,
  class State,
  class VertexStack,
  class CellStack,
  class EventHandle
>
class peano::grid::nodes::Leaf: public peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle> {
  private:
    typedef peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle>     Base;
    typedef peano::grid::nodes::Refined<Vertex,Cell,State,VertexStack,CellStack,EventHandle>  RefinedNode;

    static tarch::logging::Log _log;

    RefinedNode& _refinedNode;

    /**
     *
     *
     * <h2> Release mode </h2>
     *
     * If you compile without assertions (release mode), then the operation
     * deteriorates to nop. However, we use the dfor macro. Performance
     * analysis reveals that the (2015) Intel compiler does not recognise
     * that the stuff inside the pfor loop is empty and does not remove the
     * routine completely. We therefore have to protect it manually.
     */
    static void validateRefinementStateOfVertices(
      Vertex const * const          fineGridVertices,
      const SingleLevelEnumerator&  fineGridVerticesEnumerator,
      bool                          calledByStore
    );
  public:
    Leaf(
      VertexStack&                vertexStack,
      CellStack&                  cellStack,
      EventHandle&                eventHandle,
      peano::geometry::Geometry&  geometry,
      RefinedNode&                refinedNode
    );

    /**
     * @see Standard constructor. There's nothing to do here.
     */
    ~Leaf();

    void load(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell,
      int                                       counter[FOUR_POWER_D]
    );

    /**
     * Traverse the Leaf
     *
     * In most cases, this operation calls enter the cell directly followed by
     * a leave cell event. Things are different, if a refinement flag is set.
     * In this case, traverse() creates @f$ 3^d @f$ new cells, and behaves like
     * a refined event, i.e. delegates the traversal to the refined object.
     *
     * !!! Parallelisation
     *
     * If a cell refines and if a fork is triggered and if the refining cell
     * will be assigned to the remote worker, we artificially have to reset the
     * new cells to the master's rank. Otherwise updateCellsParallelStateAfterLoad()
     * doesn't work. See Node::updateCellsParallelStateAfterLoad() for details.
     */
    void traverse(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex* const                             fineGridVertices,
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex* const                             coarseGridVertices,
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell
    );

    void store(
      State&                                    state,
      Cell&                                     fineGridCell,
      Vertex                                    fineGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex                                    coarseGridVertices[FOUR_POWER_D],
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell,
      int                                       counter[FOUR_POWER_D]
    );
};


#include "peano/grid/nodes/Leaf.cpph"


#endif
