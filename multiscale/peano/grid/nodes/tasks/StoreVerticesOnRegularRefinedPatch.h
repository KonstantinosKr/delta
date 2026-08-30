// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_NODES_TASKS_STORE_VERTICES_ON_REGULAR_REFINED_PATCH_H_
#define _PEANO_GRID_NODES_TASKS_STORE_VERTICES_ON_REGULAR_REFINED_PATCH_H_


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"


namespace peano {
  namespace grid {
    namespace nodes {
      namespace tasks {
        template <class Vertex, class Cell, class VertexStack>
        class StoreVerticesOnRegularRefinedPatch;
      }
    }
  }
}


/**
 * I need both the vertex and the cell as template argument as the task fills
 * the grid container with data (only with cells), but the grid container is
 * specified in both vertices and cells.
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell, class VertexStack>
class peano::grid::nodes::tasks::StoreVerticesOnRegularRefinedPatch {
  private:
    static tarch::logging::Log  _log;

    /**
     * @see _activeStoreTasks
     */
    static tarch::multicore::BooleanSemaphore                         _semaphore;

    /**
     * Counts the number of active store processes. Is protected by _semaphore.
     *
     * @see Destructor for a lifecycle description.
     */
    static int                                                        _activeStoreTasks;

    const bool                                                        _isTraversalInverted;
    peano::grid::RegularGridContainer<Vertex,Cell>&                   _regularGridContainer;
    VertexStack&                                                      _vertexStack;

    const bool                                                        _storeProcessRunsInParallelToOtherTasks;

    /**
     * This flag indicates the finest level that may still coarse. In a
     * top-down sense, this is the deepest level that may fork. You may
     * also pass
     *
     * DoNotSplitAndHandleOnlyPatchBoundary
     * PersistentSubtreeIsDrained
     *
     * or -1 to indicate that no forking should be done.
     */
    int                                                               _maxLevelToFork;

    typename VertexStack::PushBlockVertexStackView                    _stackView;

    /**
     * Coarsest level of this task
     *
     * It is a relative quantity, i.e. relative to the regular tree's root.
     */
    const int                                                         _coarsestLevelOfThisTask;


    /**
     * If _coarsestLevelOfThisTask equals 0, this flag has to be zero, too.
     * Otherwise, it says where to take the coarsest cell from in the regular
     * grid container.
     */
    const tarch::la::Vector<DIMENSIONS,int>                           _coarsestCellsOffset;

    #ifdef Asserts
    /**
     * Has task written to output stream
     *
     * I keep track of this property in the assert mode to be able to validate
     * the output stream's / view's cardinality in the destructor.
     */
    bool                                                              _haveWrittenToOutputStack;
    #endif

    /**
     * Mark all the subtrees on level 1 that are handled by other threads. The
     * subtrees are enumerated lexicographically.
     */
    std::bitset<THREE_POWER_D>                                        _forkedSubtree;

    /**
     * Store Vertices Of One Cell
     *
     * Different to the load process, there's only one variant of this
     * operation. An on-the-fly reconstruction of the cells does not
     * make sense, as the cells are always already available in the
     * regular subtree.
     *
     * <h2> Persistent regular subtrees </h2>
     *
     * If whole subtrees are held persistently in the regular grid container,
     * I can terminate the recursion inside this regular region early. There
     * is no need to go to the finest level inside of the tree. The vertices
     * are there already anyway.
     *
     */
    void storeSubVerticesWithCellsFromGridContainer(
      const int                                 currentLevelOfCoarseCell,
      const tarch::la::Vector<DIMENSIONS,int>&  currentCoarseCellPositionWithinUnrolledPatch
    );

    /**
     * Handle Level 1 of Regular Subtree in Shared Memory Mode
     *
     * Is only executed on the primary thread.
     *
     * Analyses on level 1 of the regular subgrid cascade how many vertices
     * will be stored before the automaton enters the central element of the
     * @f$ 3^d @f$ subpatch. The operation then opens a view on the primary
     * thread for these vertices, and, afterwards, forks a different task for
     * the central element. Then it starts to process the tree skipping the
     * central element (this is deployed to a different thread).
     *
     * The idea of this operation is to come up with a concurrency level of two
     * as fast as possible. Afterwards we can continue with the sequential
     * processing of the boundary cells as the forked thread will fork itself
     * again immediately. This change of control flow on the first level thus
     * is the key ingredient to come up with a decent concurrency level.
     */
    void storeSubVerticesWithCellsOnFirstLevelInSharedMemoryMode();

    /**
     * !!! Implementation
     *
     * We explicitly have to copy the vertex enumerator, as we do manipulate
     * its offset. Multiple threads might work on the same level and modify the
     * offset, so we should copy explicitly.
     *
     * @param cellsPositionWithinUnrolledTreeLevel That is the fine grid cell, i.e. currentCell's, position
     */
    void storeVerticesOfOneCellWithinRegularSubtree(
      const Cell&                               currentCell,
      const tarch::la::Vector<DIMENSIONS,int>&  cellsPositionWithinUnrolledTreeLevel,
      const int                                 currentLevel
    );


    /**
     * <h2>Persistent subtrees</h2>
     *
     * If we drain persistent subtrees, we have to be very careful: As no cells
     * have been moved from and to the memory, also the stack statistics are
     * not kept up-to-date. So when we dran a persistent subtree, we have to
     * invalidate all vertices if we run in shared memory, as we have to ensure
     * that the code does not try to handle a new (sub-)tree with multiple
     * parallel loads.
     */
    void storeVerticesOfOneCellAtBoundaryofSubtree(
      const Cell&                               currentCell,
      const tarch::la::Vector<DIMENSIONS,int>&  cellsPositionWithinUnrolledTreeLevel,
      const int                                 currentLevel
    );

    /**
     * Create new task
     */
    StoreVerticesOnRegularRefinedPatch(
      const bool                                                        isTraversalInverted,
      peano::grid::RegularGridContainer<Vertex,Cell>&  regularGridContainer,
      VertexStack&                                                      vertexStack,
      bool                                                              loadProcessRunsInParallelToOtherTasks,
      int                                                               maxLevelToFork,
      const int                                                         currentLevel,
      const tarch::la::Vector<DIMENSIONS,int>&                          offsetWithinPatch,
      typename VertexStack::PushBlockVertexStackView                    stackView,
      const std::bitset<THREE_POWER_D>&                                 forkedSubtree
   );

  public:
    static bool haveAllStoreVerticesTasksTerminated();

    /**
     * Can be handed in as max fork level. As the value -1 is already used by
     * peano::grid::nodes::transformOracleResult() to ensure that absolutely
     * no fork happens at all, I use the -2 here. If this flag is set, we
     * handle solely the patch boundary and do not split. The mode is important
     * for trees that are stored persistently.
     *
     * Please note that setting this flag also terminates the recursion inside
     * the tree early: As interior vertices don't have to be loaded or stored,
     * we don't have to descend here in the tree. Search for the bool recurse
     * in the code (it is used twice).
     *
     * It is important that this mode does set the parents-regular-subtree flag
     * along the whole boundary. This flag vetos refinements and erases. This is
     * important. Otherwise you might leave a regular subgrid stored
     * persistently and a neighbour cell might set the refinement. In the next
     * iteration when the automaton runs through this neighbour region first,
     * the refinement triggered is switched into refining, we refine, and the
     * persistently stored subtree becomes invalid. We therefore set the flag
     * along the whole boundary. Please note that the vertices on the coarsest
     * level have to be set manually in RegularRefined::traverse() as the store
     * process does not touch the vertices of the coarsest level.
     */
    static const int DoNotSplitAndHandleOnlyPatchBoundary = -2;

    /**
     * Has to be passed as max fork level if you drain a persistent subtree.
     * It is important that the store process is aware of the drain. As the
     * vertices of a persistent subtree were taken out from all the streams
     * for a while, they may carry invalid refinement information.
     *
     * Even if they did not change at all, we have set everything invalid. The
     * cells hold some stack statistics, and we have to assume that they are
     * wrong as we do not know whether an even or and odd number of traversals
     * has passed since. So we have to invalidate the whole tree.
     */
    static const int PersistentSubtreeIsDrained           = -3;

    /**
     * @param maxLevelToFork see _maxLevelToFork and
     *   DoNotSplitAndHandleOnlyPatchBoundary as well as
     *   PersistentSubtreeIsDrained.
     */
    StoreVerticesOnRegularRefinedPatch(
      const bool                                                        isTraversalInverted,
      peano::grid::RegularGridContainer<Vertex,Cell>&                   regularGridContainer,
      VertexStack&                                                      vertexStack,
      bool                                                              storeProcessRunsInParallelToOtherTasks,
      int                                                               maxLevelToFork
    );

    /**
     * Copy constructor
     *
     * I have to overload the copy constructor to enable the task counting.
     */
    StoreVerticesOnRegularRefinedPatch( const StoreVerticesOnRegularRefinedPatch<Vertex,Cell,VertexStack>& copy );

    /**
     * Destructor
     *
     * We can validate in the destructor whether the stack view is empty. It
     * either has been empty from the start (on the master thread) or has not
     * been empty. In the latter case, the forked process should have processed
     * the whole view and emptied it.
     *
     * A different behaviour can occur due to technical reasons: If we fork a
     * thread, also the task is copied (several times) and the blueprint of the
     * task on the master thread then is destroyed. In this case, the blueprint
     * might still hold the whole stack view, i.e. has not written any record
     * to the view at all.
     *
     * !!! Asynchronous task model
     *
     * To keep track of the active threads, the class uses a static thread
     * counter _activeStoreTasks. It is incremented by the constructor but not
     * by the copy constructor, i.e. it really counts the tasks not the thread
     * objects. Originally, I planned to free this counter in the destructor
     * (we then could increment it in each constructor and need no additional
     * decrement in the destructor), but that does not work with all TBB
     * versions, as some of them keep dummy copies of the thread object.
     *
     * So, only real 'new' tasks increment the global task counter. This
     * counter in return is decremented by the operator().
     */
    ~StoreVerticesOnRegularRefinedPatch();

    bool operator()();
};


#include "peano/grid/nodes/tasks/StoreVerticesOnRegularRefinedPatch.cpph"


#endif
