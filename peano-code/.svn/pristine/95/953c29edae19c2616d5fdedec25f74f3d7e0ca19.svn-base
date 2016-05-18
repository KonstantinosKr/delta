// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_ROOT_H_
#define _PEANO_GRID_NODES_ROOT_H_


#include "peano/utils/Globals.h"
#include "peano/grid/TraversalOrderOnTopLevel.h"
#include "peano/grid/nodes/Node.h"
#include "peano/geometry/Geometry.h"
#include "peano/grid/SingleElementVertexEnumerator.h"
#include "peano/grid/SingleLevelEnumerator.h"


/**
 * Root Node
 *
 * The root node basically is a refined node. However, neither its root data
 * nor its level-one cells are stored on stacks. They are instead held locally.
 * Also, this refined node not necessarily does traverse the first level of the
 * spacetree in the space-filling curve order. If the node is used on a
 * parallel worker, its level-one order has to preserve the global sfc, but not
 * necessarily the local curve.
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
class peano::grid::nodes::Root: public peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle> {
  private:
    typedef peano::grid::nodes::Node<Vertex,Cell,State,VertexStack,CellStack,EventHandle>     Base;
    typedef peano::grid::nodes::Leaf<Vertex,Cell,State,VertexStack,CellStack,EventHandle>     LeafNode;
    typedef peano::grid::nodes::Refined<Vertex,Cell,State,VertexStack,CellStack,EventHandle>  RefinedNode;

    static tarch::logging::Log _log;

    /**
     * Vertices passed to sub elements. Note that it is a @f$ 4^d @f$ array
     * altough root event would need @f$ 2^d @f$ vertices only. The reason is
     * a standard refined event is used.
     */
    Vertex  _coarseGridVertices[FOUR_POWER_D];

    /**
     * This is the root node of the spacetree, i.e. it is not an inner cell but
     * basically the bounding box of the simulation domain plus its surrounding
     * cells.
     */
    Cell    _coarseGridCell;

    /**
     * Holds the relative position of the level one central cell to the master
     * cell. In the serial code, on the master, and technically, the central
     * cell is always the middle one in the @f$ 3^d @f$ patch. However, in the
     * multigrid code, it might not be the middle one on the worker. In this
     * case, this field holds this information from the master and is used to
     * reconstruct the coarse grid enumerator accordingly.
     */
    tarch::la::Vector<DIMENSIONS,int>     _positionOfRootCellRelativeToCoarserCellOnMaster;

    #ifdef Parallel
    /**
     * These vertices are the @f$ 2^d @f$ vertices adjacent to the central
     * element. Originally, I planned to merge the received data immediately
     * into the local records when the central cell is received. However, this
     * does not work always: If the worker descends within an outer cell
     * multiple levels before it steps into the central cell, it also calls
     * touchFirstTime() on all the vertices adjacent to that outer part of the
     * tree. This is totally irrelevant for most solvers, but solvers that
     * transport information from one level to the other (such as Bart's PIC
     * code) it is horrible. These codes rely on the fact that all data is
     * merged into the local root's data before it descends to be able to
     * transport information down.
     *
     * So I do a rather cheap trick: I receive the data from the master
     * immediately and store it within _verticesFromMaster and
     * _centralCellFromMaster. From this buffer, I copy the date if needed.
     *
     * This whole discussion is irrelevant for the send back process. Here,
     * we always send back when the traversal completely has terminated.
     * This way, we always ensure that all multiscale operation on the
     * worker successfully have been called.
     *
     * @see receiveCellAndVerticesFromMaster()
     */
    Vertex  _masterVertices[TWO_POWER_D];
    bool    _haveMergedMasterVertex[TWO_POWER_D];
    Cell    _masterCell;
    #endif

    tarch::la::Vector<DIMENSIONS,double>  _sizeOfEnclosingCoarseCell;
    tarch::la::Vector<DIMENSIONS,double>  _offsetOfEnclosingCoarseCell;
    int                                   _levelOfEnclosingCell;

    peano::grid::TraversalOrderOnTopLevel&  _cellTraversal;

    LeafNode&                             _leafNode;
    RefinedNode&                          _refinedNode;

    peano::grid::SingleLevelEnumerator          getCoarseGridEnumeratorForLocalData() const;
    peano::grid::SingleElementVertexEnumerator  getCoarseGridEnumeratorForReceivedData() const;
    peano::grid::SingleLevelEnumerator          getLevelOneGridEnumeratorForLocalData() const;
    peano::grid::SingleElementVertexEnumerator  getLevelOneGridEnumeratorForReceivedData() const;

    /**
     * Initialise fine grid cells
     *
     * Create the level one cells and stream these data to the output stack.
     * This operation shall not be invoked if the stacks are not empty.
     * In the latter case, another root event has already initialised the
     * data and there's no need to initialise it twice.
     *
     * See comments on restart() and in particular the Section Implementation
     * there for some rationale.
     *
     * Some attributes of the generated cells might be invalid if the root
     * node runs on a worker. For this case, see the documentation of
     * traverse().
     */
    void createFineGridCellsAndFillCellStacks();

    /**
     * The level one vertices are stored explitely (those on the patch boundary).
     */
    void createLevelOnePatchBoundaryVertices();

    /**
     * Perpare Level One Vertices for Next Traversal
     *
     * Create boundary vertices of the level 1 patch, i.e. all these hanging
     * nodes. Also set the inner counters to persistent or new such that the
     * load events later either create them or load them from the stacks. This
     * operation is called before any traversal. Hence, we check the input
     * stacks: If they are totally empty, no traversal ever has been called,
     * and we set the counters of the central @f$ 2^d @f$ vertices to
     * 'create a new one'. Otherwise, we fetch them from the input stacks.
     *
     * The outer @f$ 4^d-2^d @f$ vertices are set to hanging nodes.
     *
     * If one of the top level vertices is a hanging node, we can handle it
     * nevertheless as persistent node on the worker. We just have to ensure
     * that it is unrefined and that it does not hold any adjacency flag not
     * identifying the local rank.
     *
     * !!! Debug Details
     *
     * If Peano is compiled in debug mode, all vertices are assigned the
     * correct position and level in this operation.
     */
    void prepareLevelOneVertices(
      Vertex  fineGridVertices[FOUR_POWER_D],
      int     counter[FOUR_POWER_D]
    );

    #ifdef Parallel
    /**
     * Counterpart of Refined::splitUpGrid()
     *
     * Different to the refined cell's operation, this operation is interested
     * exclusively in the middle cell and always tries to fork this one iff
     * the operation is invoked on the global master.
     *
     * Consequently, the implementation is very similar to the other split
     * operation with a couple of minor differences:
     *
     * - We do not need the coarse grid cell or its vertices, as these are
     *   attributes of the class.
     * - We return immediately if the middle cell already is remote, as we
     *   are only interested in this cell anyway.
     *
     */
    void splitUpGrid(
      State&                                       state,
      const SingleLevelEnumerator&                 coarseGridVerticesEnumerator,
      Cell&                                        centralFineGridCell,
      const SingleLevelEnumerator&                 centralFineGridVerticesEnumerator,
      Vertex*                                      fineGridVertices
    );

    /**
     * Receive data from master
     *
     * !!! Assertions
     *
     * Both the master and the worker vertex either
     * - hold the same refinement state,
     * - master is refined but the worker just switched to refining, i.e. it
     *   reconstructs the master's grid due to a fork, or
     * - master hasn't refined a vertex whereas the vertex actually is refined.
     *
     * @image peano/grid/nodes/parallel-root-cell-communication.png
     *
     * The latter case occurs if all the @f$ 2^d @f$ adjacent cells of the
     * vertex are assigned to nodes different than the master. In the image
     * above that is the very upper left cell if the master (coarse cell
     * top, very left) has deployed all the adjacent @f$ 3^d -1 @f$ cells to
     * different nodes.
     *
     * There is no data to be received (or sent as well) if the root node is
     * running on the global master of if the root node belongs to a process
     * that is right now joining with its master.
     *
     * @see Node::updateCellsParallelStateAfterLoad()
     * @see _verticesFromMaster
     */
    void receiveCellAndVerticesFromMaster(
      State&  state,
      bool    skipMPICalls
    );

    /**
     * Take received data and merge it into local copies
     */
    void mergeReceivedCellAndVerticesFromMasterIntoLocalDataStructure(
      const State&                                 state,
      Cell&                                        fineGridCell,
      Vertex*                                      fineGridVertices,
      const tarch::la::Vector<DIMENSIONS,int>&     currentLevelOneCell
    );

    /**
     * @see Node::updateCellsParallelStateBeforeStore()
     * @see _verticesFromMaster
     * @see Grid::sendStateToMaster() for a discussion on different send semantics.
     */
    void sendCellAndVerticesToMaster(
      const State&                                 state,
      Cell&                                        fineGridCell,
      const SingleLevelEnumerator&                 fineGridVerticesEnumerator,
      Vertex*                                      fineGridVertices,
      bool                                         skipMPICalls
    );
    #endif

    /**
     * Set Data of Coarsest Level
     *
     * Set the attributes identifying the domain and switch coarsest element to
     * root. Afterwards, we initialise the vertices belonging to the root node of
     * the embedding tree, i.e. we assign them a position and a level. Also, we
     * make them outside and invalidate their adjacency information.
     *
     * !!! Parallelisation
     *
     * The root cell belongs to the master node if the current rank is not
     * equal to the global master. That assignment is important as the subcells
     * on level one do inherit the responsible rank from the root cell, i.e.
     * they are then also belonging to the master node and consequently not
     * considered on the rank.
     *
     * Tricky is the adjacency information of the coarse grid vertices: They
     * are explicitely set to belong exclusively to the local worker. The
     * reason is simple: For the level one vertices at the boundary of the
     * patch (these are the hanging nodes on level one), the coarses vertices
     * are not relevant at all as these vertices are created artificially
     * anyway each traversal by prepareLevelOneVertices(). The @f$ 2^d @f$
     * vertices in the middle however are created by the creational events,
     * i.e. by the load vertices loop, and they inherit from the coarsest
     * level, i.e. they inherit the local rank. As a consequence, the fork
     * algorithm detects that these vertices did belong to the master before,
     * so all adjacent cells and the vertices themselves are to be copied
     * from the master. See Node::updateCellsParallelStateAfterLoad() for
     * details. Due to this copy process, the level one vertices then
     * receive the valid adjacency information. q.e.d.
     */
    void setCoarsestLevelAttributes(
      const tarch::la::Vector<DIMENSIONS,double>&  sizeOfCentralElement,
      const tarch::la::Vector<DIMENSIONS,double>&  offsetOfCentralElement,
      int                                          levelOfCentralElement,
      const tarch::la::Vector<DIMENSIONS,int>&     positionOfFineGridCellRelativeToCoarseGridCell
    );
  public:
    /**
     * Constructor
     * 
     * This constructor does not initialise the root element of the spacetree completely. 
     * Instead, you always have to call restart() explicitly.
     */
    Root(
      VertexStack&                                 vertexStack,
      CellStack&                                   cellStack,
      EventHandle&                                 eventHandle,
      peano::geometry::Geometry&                   geometry,
      LeafNode&                                    leafNode,
      RefinedNode&                                 refinedNode,
      peano::grid::TraversalOrderOnTopLevel&       cellTraversal
    );

    /**
     * @see Standard constructor
     */
    virtual ~Root();

    /**
     * Initialises the root (master node only).
     *
     * The operation takes the state and sets the vertices of the state
     * accordingly. Furthermore, it might initialise the cells of the
     * first level (see implementation).
     *
     * This is the restart() operation you should call if the root node is
     * a standard root node, i.e. not used as worker in an MPI context.
     * Consequently, I could merge it with one of the constructors, but
     * decided to do it this way.
     *
     * !!! Implementation
     *
     * restart() is called for each root element, i.e. each adapter that is
     * defined for a repository. As the coarse cell, i.e. the global root
     * cell, and the adjacent vertices are held by each root element, these
     * data exist several times. This ain't a consistency problem as the level
     * zero data do not hold any information.
     *
     * The level one data is a different story. Level one is to be set up once
     * at the construction of the repository. And all the data of level one has
     * to be shared by all the root instances. Consequently, these data are
     * shared among all the root instances. The root instances check whether
     * the stacks are empty. If they are not, another root object already has
     * initialised level one. If they are empty, the root event sets up the
     * @f$ 3^d @f$ cells and the @f$ 4^d @f$ vertices of level one. Set up
     * means to stream them to the stacks and to switch the stacks afterwards.
     * As a consequence, traverse() has to re-load them from the stacks before
     * it starts.
     *
     * @see traverse()
     *
     */
    void restart(
      State&                                       state,
      const tarch::la::Vector<DIMENSIONS,double>&  sizeOfCentralElement,
      const tarch::la::Vector<DIMENSIONS,double>&  offsetOfCentralElement,
      int                                          levelOfCentralElement
    );


    #ifdef Parallel
    void restart(
      const tarch::la::Vector<DIMENSIONS,double>&  sizeOfCentralElement,
      const tarch::la::Vector<DIMENSIONS,double>&  offsetOfCentralElement,
      int                                          levelOfCentralElement,
      const tarch::la::Vector<DIMENSIONS,int>&     positionOfFineGridCellRelativeToCoarseGridCell
    );
    #endif

    /**
     * Traverse the grid
     *
     * !!! Activities
     *
     * - Create data structures for all teh data of level one.
     * - Prepare the vertices on level one: prepareLevelOneVertices().
     * - Invoke beginIteration() event.
     * - Load the level one data (cells and vertices) into appropriate data
     *   structures. These objects are needed to descend.
     * - Traverse level one cells.
     * - Store the level one data to the output stacks in reverse order.
     * - Switch in and out stacks.
     * - Invoke endIteration() event.
     *
     * !!! Implementation of the three loops
     *
     * Instead of a loop, we work with three counters. First, we do the
     * load operations on the level one cells and increment the load
     * counter up to a given level. Afterwards, we increment the traverse
     * counter until it equals the load counter. Finally, we increment the
     * store counter until it equals the load counter. If the load counter
     * is smaller then @f$ 3^d @f$, we continue again with loads. The
     * load counter sprints through the array in steps. The other two
     * counters follow it.
     *
     * For the sequential code, this distinction is an overkill. Three
     * simple loops are sufficient, as the load counter always traversed
     * the whole cell patch, i.e. the other two counters also run through
     * the complete patch.
     *
     * For the parallel code, it is important that load-traverse-store
     * sequences can occur multiple times, as the traversal of the tree
     * does not follow the (local) space-filling curve while it follows the
     * global sfc. See class TraversalOrderOnTopLevel for a detailed
     * information. Also the image below illustrates this fact:
     *
     * @image html peano/grid/nodes/../../parallel/JoinDataBuffer.png
     *
     * !!! Validity of cell data
     *
     * In the serial code, the cells on level one are derived from the root
     * cell. We take the root cell and call refine. The resulting @f$ 3^d @f$
     * cells then are used as level-one cells. This procedure does not work
     * for the parallel workers. See the illustration above.
     *
     * For them, the access lists might differ as the local curve does not
     * follow the Peano space-filling curve's motif. It rather follows the
     * global space-filling curve. Consequently, we take the data from the
     * JoinDataBuffer (which should actually be renamed to JoinForkDataBuffer)
     * in this case and overwrite our local cells. More precisely, we overwrite
     * the access flags, as all the other information should be the same.
     */
    void traverse(State& state);

    /**
     * Only in parallel mode
     *
     * Reset in particular root and coarse grid cells to enable the code
     * to rerun for a different subpartition after a join.
     */
    void terminate();
};


#include "peano/grid/nodes/Root.cpph"


#endif
