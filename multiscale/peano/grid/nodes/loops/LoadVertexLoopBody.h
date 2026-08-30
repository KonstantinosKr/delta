// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_LOOPS_LOAD_VERTEX_LOOP_BODY_H_
#define _PEANO_GRID_NODES_LOOPS_LOAD_VERTEX_LOOP_BODY_H_


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "tarch/multicore/MulticoreDefinitions.h"

#include "peano/utils/Globals.h"
#include "peano/datatraversal/Action.h"
#include "peano/geometry/Geometry.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace loops {
        template <class Vertex, class Cell, class State, class VertexStack, class EventHandle, int CellFlagsFromEnumerator>
        class LoadVertexLoopBody;
      }
    }
  }
}




/**
 * Load Loop Body
 *
 * This loop body is responsible to fetch data from the vertex containers and
 * write them to the Cartesian grid data structure. See Grid::loadVertices()
 * for a detailed description of the loop body's semantics.
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell, class State, class VertexStack, class EventHandle, int CellFlagsFromEnumerator>
class peano::grid::nodes::loops::LoadVertexLoopBody {
  private:
    static tarch::logging::Log _log;

    State&                                    _state;
    Vertex*                                   _fineGridVertices;
    const SingleLevelEnumerator&              _fineGridVerticesEnumerator;
    Cell&                                     _coarseGridCell;
    Vertex * const                            _coarseGridVertices;
    const SingleLevelEnumerator&              _coarseGridVerticesEnumerator;
    const tarch::la::Vector<DIMENSIONS,int>&  _fineGridPositionOfCell;
    int*                                      _counter;
    VertexStack&                              _vertexStack;

    EventHandle&                              _eventHandle;

    /**
     * Usually, the geometry should be const. See getters of geometry for a
     * description.
     */
    peano::geometry::Geometry&                _geometry;

    void loadVertexFromInputStream(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);
    void createHangingNode(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);

    /**
     *
     * Invokes switchRefinementTriggeredToRefining() and
     * switchCoarseningTriggeredToCoarsening() on the loaded vertex.
     *
     * <h2> Thread-safety </h2>
     *
     * This operation is invoked for vertices fetched from the input stack.
     * The access to the input stack is already serialised, i.e. there's
     * never two simultaneous accesses to the input stack. Consequently, this
     * operation also is serialised and there's no need to make it thread-safe
     * explicitly.
     *
     * <h2> Boundary data exchange with MPI </h2>
     *
     * The create-new-vertex operation can do an immediate refinement, i.e. it
     * can switch a vertex with refinement-triggered into a refining vertex
     * immediately. This way, we can create whole grids in one pass instead of
     * running through the grid for each additional level. However, if the
     * vertex belongs to the parallel boundary, we may not create the new
     * levels immediately. If we did, it might happen that one node refines
     * immediately as it decides only locally to fine, and then the grids are
     * inconsistent. So, for these nodes, we do not update immediately but wait
     * one iteration: Only one level per traversal is refined along parallel
     * boundaries. This way, we ensure that grids along parallel boundaries are
     * consistent.
     *
     * The erase process mirrors that behaviour: Ease in the serial mode
     * propagates immediately down the tree and thus can throw away whole
     * subtrees in one rush. In the parallel code, this can induce starvation
     * (see ParallelMerge::mergeWithVertexFromMaster()). To avoid grid
     * consistencies, erase-triggers are not switched to erasing if subworkers
     * are involved. This check is realised in
     * Vertex::switchEraseTriggeredToErasing().
     *
     * Another option to 'slow down' the erasing would be to allow at most one
     * level to be erased along the parallel boundaries, i.e. to mirror exactly
     * the refinement behaviour. I decided to stop erase totally instead for
     * two reasons:
     *
     * - The information whether we are one level higher than the finest level
     *   is not available at the moment.
     * - Even with one grid level removed per iteration, starvation (i.e. workers
     *   running out of work) still can happen. So I'd have to write all the
     *   code to handle this.
     * - If we support erase between master-worker domains, we also have to
     *   realise refines, as the erase might be followed by a refine
     *   immediately. This makes it even more tricky, as a hanging node
     *   becoming persistent has hold the right adjacency information.
     *   Information that often is not available.
     *
     * The immediate refine is switched off implicitly in operator() where
     * updateRefinementFlagsOfVertexAfterLoad() is called after
     * touchVertexFristTime (which is called through invokeLoadVertexEvents())
     * if and only if IrregularStaticGrid is not set (parallel domain boundaries
     * never are considered to be static, so this never holds) or
     * DomainDecompositionPermitsRefinement is set. DomainDecompositionPermitsRefinement
     * also never holds for boundary vertices.
     *
     * <h2> Switched off boundary data exchange</h2>
     *
     * If boundary data exchange is switched off, i.e. the send receive buffer
     * pool does not deploy valid data, then you may not call
     * updateRefinementFlagsOfVertexAfterLoad(). Again, we may neglect the new
     * branch as vertices in this branch never are refined immediately. For
     * persistent vertices at the domain boundary, we know that
     * IrregularStaticGrid does not hold as boundaries are never static.
     * Besides IrregularStaticGrid, we also ask the pool whether deployed data
     * is valid before we update the refinement flags.
     */
    void updateRefinementFlagsOfVertexAfterLoad(int positionInArray, bool counterIsDelete);

    /**
     *  Create New Vertex
     *
     *  Creates a new vertex, switch it to non-hanging, and analyse its
     *  adjacency flags (in the parallel mode). This operation does not
     *  invoke the create vertex event as it creates an outer vertex.
     *  Later, the vertex's state will be analysed by
     *  updateGeometryAndInvokeLoadVertexEvents() and this operation will
     *  call the create event, i.e. the vertex creation process is the same as
     *  the process for moving geometries: An outer vertex just becomes an
     *  inner/boundary vertex.
     */
    void createNewVertex(
      int                                       positionInArray,
      const tarch::la::Vector<DIMENSIONS,int>&  positionInLocalCell
    );

    /**
     * Updates geoemtry information
     *
     * Asks the scenario server whether the geometry has changed. If it has
     * changed, the operation updates the vertex's state.
     *
     * <h2> Parallelisation </h2>
     *
     * If the code is running in parallel, these checks are performed
     * exclusively for local vertices, i.e. vertices of the shadow layer
     * that are handled by remote ranks are not taken into account.
     *
     * Due to the definition of remote (see Vertex::isRemote()) a vertex
     * is still local even if all adjacent ranks are right now set to
     * fork-triggered or forking. The latter statement sounds strange, but it
     * is necessary:
     *
     * Throughout the fork, all data is forwarded to the new worker. For
     * each grid entity, we call the copy operation and a receive/merge
     * function on the receiving side. On the receiving side, we cannot
     * identify for a cell/vertex whether it is set remote due to the fork
     * neither whether it has just been created on the master throughout
     * the fork process. On the worker, we always assume that everything
     * merged into the local data structures is properly initialised. Also
     * the geometry.
     *
     * In return, this implies that due to dynamic refinement, it can happen
     * that a grid entity is created and immediately destroyed afterwards
     * on the master. We just initialise it to send away properly configured
     * data.
     *
     * @see Node::updateCellsGeometryInformationAfterLoad()
     */
    void updateGeometry(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);

    /**
     *
     * <h2> Parallelisation </h2>
     *
     * See remarks for updateGeometry() for further information.
     *
     * <h2> Optimisation </h2>
     *
     * Peano's mapping specifications would allow to skip this statement for
     * refined vertices if the marker is set to false. However, we observed
     * that the evaluation of the statement on most architectures is more
     * expensive than the actual savings in runtime. Thus, this operation
     * calls touchVertexFirstTime() always.
     */
    void invokeLoadVertexEvents(int positionInArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell);

    /**
     * In debug or assert mode, this operation compares the vertex's position
     * and level to the cell data.
     */
    void validateVertexInArray(
      int                                       positionInVertexArray,
      const tarch::la::Vector<DIMENSIONS,int>&  positionInLocalCell,
      int                                       stackNumber
    ) const;

    void invalidateVertexIfGeometryHasChanged( int positionInVertexArray, const tarch::la::Vector<DIMENSIONS,int>& positionInLocalCell ) const;
  public:
    LoadVertexLoopBody(
      State&                                    state,
      Vertex*                                   fineGridVertices,
      const SingleLevelEnumerator&              fineGridVerticesEnumerator,
      Cell&                                     coarseGridCell,
      Vertex * const                            coarseGridVertices,
      const SingleLevelEnumerator&              coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell,
      int*                                      counter,
      VertexStack&                              vertexStack,
      EventHandle&                              eventHandle,
      peano::geometry::Geometry&                geometry
    );

    ~LoadVertexLoopBody() = default;

    /**
     * Load a Vertex
     *
     *
     *
|| Stack \\ Counter || persistent                        || hanging                           || new                               || delete                            || none
|| InOut            |  load(-1)                          |  create hanging node               |  create persistent node            |  load(-1)                          |  nop (vertex is already loaded)
||                  |  counter->CounterNodeAlreadyLoaded |  counter->CounterNodeAlreadyLoaded |  counter->CounterNodeAlreadyLoaded |  counter->CounterNodeAlreadyLoaded |  inc counter
|| Temporary        |  load(stack number)                |  load(stack number)                |  load(stack number)                |  load(stack number)                |  nop (vertex is already loaded)
     *
     *
     * <h2> Assertions </h2>
     *
     * If we do some rebalancing such as a fork, the grid may not be handled as
     * static. This is an assertion we can do only throughout the load process.
     * Throughout the store, rebalancing might become set for the subsequent
     * traversal while the current traversal still handles whole grid regions
     * as stationary grid parts.
     *
     * <h2> CaseCreateNewNodeTemporarily </h2>
     *
     * When we fork or join a grid region, it can happen that the sender is
     * right in the process of destroying grid entities, while the receiver has
     * to reconstruct exactly these parts. The receiver then is set to
     * CounterNewNodeRefineDueToJoinThoughWorkerIsAlreadyErasing which equals
     * from the load process point of view basically a grid generation.
     * Different to the standard grid generation, this generation is controlled
     * by a remote data source. There is consequently no need to realise an
     * immediate refine.
     *
     * <h2> Immediate refines </h2>
     *
     * In the serial mode, we load data from the input stream, call
     * touchVertexFirstTime() and afterwards update the vertex refinement
     * states. This way, we can refine a grid immediately after the refine()
     * call. This is important to avoid that Peano has to sweep the grid once
     * per new additional level. There are however two restrictions:
     *
     * If we'd assumed that the grid was stationary throughout the descend, i.e.
     * if the tree grammar says it is static, we may not conduct any refinement
     * immediately. Otherwise, we'd load an instationary subtree though our
     * automaton assumes it is stationary. In this case, we just have to wait
     * one iteration, i.e. we have to postpone the refinement. In the subsequent iteration,
     * the stationary flag is reset and we can conduct the refinement/erase.
     *
     * In the parallel state, we might not be allowed to refine or coarse even
     * though the grammar assumes an instationary tree: Here, we have always to
     * wait one iteration for vertices along the parallel boundary, as the
     * refine/coarse command first has to be exchanged with all neighbours. We
     * thus need another check:
     * - If the vertex is not adjacent to a remote rank, we may refine.
     * - Otherwise, the vertex can be refined/coarsened if and only if it has
     *   been set to a triggered state before touchVertexFirstTime() has been
     *   called, i.e. refine()/erase() calls from within touchVertexFirstTime() explicitly
     *   are blocked.
     */
    void operator() (const peano::datatraversal::Action& action);
};


#include "peano/grid/nodes/loops/LoadVertexLoopBody.cpph"


#endif
